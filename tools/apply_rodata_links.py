#!/usr/bin/env python3
"""Patch generated linker output for PRE-TEXT rodata-backed C matches.

Splat currently emits the whole 0x80010000..0x80011DA8 PRE-TEXT region as one
binary blob. Some original switch tables live in that region, while their
functions live in the game text segment. This script replaces the generated
pre_text blob input with exact raw gaps plus selected C object .rodata ranges,
and removes those same .rodata inputs from the generated game-tail rodata list.
"""

from __future__ import annotations

import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RODATA_MATCHES = ROOT / "config/rodata_matches.json"
PRE_TEXT_BIN = ROOT / "assets/pre_text.bin"
LD_SCRIPT = ROOT / "kaze.ld"
VRAM_BASE = 0x80010000


def run(cmd: list[str]) -> None:
    subprocess.run(cmd, check=True)


def wrap_bin(path: Path, out: Path) -> None:
    tmp = out.with_suffix(out.suffix + ".tmp.o")
    run(["mipsel-linux-gnu-ld", "-r", "-b", "binary", "-o", str(tmp), str(path)])
    run(
        [
            "mipsel-linux-gnu-objcopy",
            "--rename-section",
            ".data=.data",
            "--set-section-flags",
            ".data=alloc,contents,load,data",
            str(tmp),
            str(out),
        ]
    )
    tmp.unlink()


def assemble_gap(src_s: Path, out: Path) -> None:
    run(
        [
            "mipsel-linux-gnu-as", "-EL", "-march=r3000", "-mabi=32",
            "-no-pad-sections", "-I", str(ROOT / "include"),
            "-o", str(out), str(src_s),
        ]
    )
    run(
        [
            "mipsel-linux-gnu-objcopy",
            "--set-section-alignment", ".data=4",
            str(out),
        ]
    )


def main() -> int:
    if not RODATA_MATCHES.is_file():
        return 0

    sys.path.insert(0, str(ROOT / "tools"))
    from pretext_symbolize import emit_gap_asm, load_context

    extents, names, labels = load_context()
    gap_stats = {"words": 0, "sym": 0, "labels": 0}

    def emit_gap(pre_text: bytes, cursor: int, upto: int, gap_dir: Path) -> str:
        src_s = gap_dir / f"pre_text_{cursor:06x}_{upto:06x}.s"
        out = src_s.with_suffix(".s.o")
        st = emit_gap_asm(pre_text, cursor, upto, src_s, extents, names, labels)
        for k in gap_stats:
            gap_stats[k] += st[k]
        assemble_gap(src_s, out)
        return f"        {out.relative_to(ROOT)}(.data);"

    entries = json.loads(RODATA_MATCHES.read_text())
    entries = [e for e in entries if "rodata_vram" in e and "rodata_size" in e]
    if not entries:
        return 0

    pre_text = PRE_TEXT_BIN.read_bytes()
    spans = []
    for entry in entries:
        start = int(entry["rodata_vram"], 16) - VRAM_BASE
        size = int(entry["rodata_size"])
        end = start + size
        if start < 0 or end > len(pre_text):
            sys.exit(f"rodata range outside pre_text for {entry['function']}: {start:#x}..{end:#x}")
        spans.append((start, end, Path(entry["src"]).stem, entry["function"]))
    spans.sort()

    for idx, (start, end, _, fn) in enumerate(spans):
        if idx and start < spans[idx - 1][1]:
            prev = spans[idx - 1][3]
            sys.exit(f"overlapping rodata ranges: {prev} and {fn}")

    gap_dir = ROOT / "build/pre_text_gaps"
    gap_dir.mkdir(parents=True, exist_ok=True)
    for old in gap_dir.glob("*"):
        old.unlink()

    pre_text_lines: list[str] = []
    cursor = 0
    for start, end, stem, _ in spans:
        if start > cursor:
            pre_text_lines.append(emit_gap(pre_text, cursor, start, gap_dir))
        pre_text_lines.append(f"        build/src/{stem}.c.o(.rodata);")
        cursor = end
    if cursor < len(pre_text):
        pre_text_lines.append(emit_gap(pre_text, cursor, len(pre_text), gap_dir))
    print(
        f"pre_text gaps: {gap_stats['words']} words, "
        f"{gap_stats['sym']} symbolized, {gap_stats['labels']} labels"
    )

    text = LD_SCRIPT.read_text()
    for _, _, stem, _ in spans:
        text = text.replace(f"        build/src/{stem}.c.o(.rodata);\n", "")

    old = "        build/assets/pre_text.bin.o(.data);"
    if old not in text:
        sys.exit("could not find generated pre_text blob line in kaze.ld")
    text = text.replace(old, "\n".join(pre_text_lines))

    LD_SCRIPT.write_text(text)
    print(f"patched {LD_SCRIPT} with {len(spans)} PRE-TEXT rodata range(s)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
