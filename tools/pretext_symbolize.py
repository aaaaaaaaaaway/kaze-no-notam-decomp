#!/usr/bin/env python3
"""Emit relocatable assembly for pre_text gap regions.

Each gap (a slice of assets/pre_text.bin not covered by a C switch-table
rodata range) is emitted as `.word` data where every word that points into
a known function or named symbol becomes a symbol reference
(`.word fn` / `.word fn + 0xNN` for jump-table targets), and labels are
defined at every address inside the gap that other code references
(from config/symbol_addrs.txt and the splat-generated undefined_syms).
Non-pointer words stay as raw constants, so a byte-identical link is
guaranteed and the output is shift-ready.
"""
from __future__ import annotations

import bisect
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
VRAM_BASE = 0x80010000
PRE_TEXT_END = 0x80011DA8
TEXT_START = 0x80011DA8
DATA_START = 0x8008C6A4


def _sym_lines(path: Path):
    if not path.is_file():
        return
    for ln in path.read_text().splitlines():
        m = re.match(r"^([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+);", ln.strip())
        if m:
            yield m.group(1), int(m.group(2), 16)


def load_context():
    """(function extents sorted by addr, addr->name, labels-in-pre_text)."""
    manifest = json.loads(
        (ROOT / "docs/function-manifest.json").read_text()
    )
    extents = sorted(
        (int(f["address"], 16), f["size"]) for f in manifest["functions"]
    )
    names: dict[int, str] = {}
    labels: dict[int, str] = {}
    for path in (ROOT / "config/symbol_addrs.txt",
                 ROOT / "undefined_syms_auto.txt",
                 ROOT / "undefined_funcs_auto.txt"):
        for name, addr in _sym_lines(path):
            names.setdefault(addr, name)
            if VRAM_BASE <= addr < PRE_TEXT_END:
                labels.setdefault(addr, name)
    return extents, names, labels


def target_expr(v: int, extents, names) -> str | None:
    """Symbol expression for a pointer value, or None if not a known target."""
    if names.get(v):
        return names[v]
    if TEXT_START <= v < DATA_START:
        starts = [a for a, _ in extents]
        i = bisect.bisect_right(starts, v) - 1
        if i >= 0:
            a, size = extents[i]
            if a <= v < a + size and names.get(a):
                off = v - a
                return names[a] if off == 0 else f"{names[a]} + 0x{off:X}"
    return None


def emit_gap_asm(pre_text: bytes, start: int, end: int, out_s: Path,
                 extents, names, labels) -> dict:
    """Write one gap as .s; returns stats."""
    stats = {"words": 0, "sym": 0, "labels": 0}
    lines = ['.include "macro.inc"', "", '.section .data, "wa"', ""]
    pos = start
    while pos < end:
        vaddr = VRAM_BASE + pos
        if vaddr in labels:
            lines.append(f"dlabel {labels[vaddr]}")
            stats["labels"] += 1
        if pos + 4 <= end and (pos & 3) == 0:
            v = int.from_bytes(pre_text[pos:pos + 4], "little")
            expr = None
            if VRAM_BASE <= v < 0x800B5700:
                expr = target_expr(v, extents, names)
            if expr is not None:
                lines.append(f"    .word {expr} /* {vaddr:08X} = {v:08X} */")
                stats["sym"] += 1
            else:
                lines.append(f"    .word 0x{v:08X} /* {vaddr:08X} */")
            stats["words"] += 1
            pos += 4
        else:
            lines.append(f"    .byte 0x{pre_text[pos]:02X} /* {vaddr:08X} */")
            pos += 1
    out_s.write_text("\n".join(lines) + "\n")
    return stats
