#!/usr/bin/env python3
"""Generate config/kaze.yaml from the matched-function ledger.

Reads config/matched.json ([{"function": name, "src": "src/file.c"}, ...]),
resolves each function's rom range, and emits a splat config where every
matched function's range is a `c` subsegment (compiled from src/) and the
remaining text stays `asm`. Contiguous matched functions sharing one src file
are merged into a single subsegment.
"""

from __future__ import annotations

import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CATALOG = ROOT / "docs/function-catalog.json"
MANIFEST = ROOT / "docs/function-manifest.json"
LEDGER = ROOT / "config/matched.json"
RODATA_MATCHES = ROOT / "config/rodata_matches.json"
OUT = ROOT / "config/kaze.yaml"

VRAM_BASE = 0x80010000
TEXT_START = 0x1DA8
TEXT_END = 0x7C6A4
ROM_END = 0x97800

HEADER = """\
options:
  platform: psx
  basename: kaze
  base_path: ..
  target_path: baserom.bin
  symbol_addrs_path: config/symbol_addrs.txt
  asm_path: asm
  src_path: src
  data_path: bin
  ld_script_path: kaze.ld
  compiler: GCC
  find_file_boundaries: false
  use_legacy_include_asm: false
  asm_function_macro: glabel
  asm_jtbl_label_macro: jlabel
  asm_data_macro: dlabel
  disassemble_all: True
  ld_align_segment_vram_end: false
  ld_align_section_vram_end: false
  ld_align_segment_start: 4
segments:
  - name: pre_text
    type: code
    start: 0x0
    vram: 0x80010000
    subalign: null
    align: 4
    subsegments:
      - [0x0, bin, pre_text]
  - name: game
    type: code
    start: 0x1da8
    vram: 0x80011da8
    subalign: null
    align: 4
    subsegments:
"""

FOOTER = """\
  - name: game_data
    type: code
    start: 0x7c6a4
    vram: 0x8008c6a4
    subalign: null
    align: 4
    subsegments:
      - [0x7c6a4, data, game_data]
  - [0x97800]
"""


def main() -> int:
    catalog = json.loads(CATALOG.read_text())
    by_name = {e["name"]: e for e in catalog["functions"]}
    manifest = json.loads(MANIFEST.read_text())
    sizes = {int(f["address"], 16): f["size"] for f in manifest["functions"]}
    rodata_entries = (
        json.loads(RODATA_MATCHES.read_text()) if RODATA_MATCHES.is_file() else []
    )
    size_overrides = {
        entry["function"]: int(entry["text_size"])
        for entry in rodata_entries
        if "text_size" in entry
    }

    ledger = json.loads(LEDGER.read_text()) if LEDGER.is_file() else []
    ranges = []  # (rom_start, rom_end, src_stem)
    for entry in ledger:
        fn = by_name[entry["function"]]
        addr = int(fn["address"], 16)
        rom_start = addr - VRAM_BASE
        rom_end = rom_start + size_overrides.get(entry["function"], sizes[addr])
        stem = Path(entry["src"]).stem
        ranges.append((rom_start, rom_end, stem))
    ranges.sort()

    # merge adjacent ranges with the same src stem
    merged = []
    for rom_start, rom_end, stem in ranges:
        if merged and merged[-1][2] == stem and merged[-1][1] == rom_start:
            merged[-1] = (merged[-1][0], rom_end, stem)
        else:
            merged.append((rom_start, rom_end, stem))

    lines = []
    cursor = TEXT_START
    asm_index = 0
    for rom_start, rom_end, stem in merged:
        if rom_start < cursor:
            raise SystemExit(f"overlapping ledger range at {rom_start:#x}")
        if rom_start > cursor:
            lines.append(f"      - [0x{cursor:x}, asm, game_text_{asm_index}]")
            asm_index += 1
        lines.append(f"      - [0x{rom_start:x}, c, {stem}]")
        cursor = rom_end
    if cursor < TEXT_END:
        lines.append(f"      - [0x{cursor:x}, asm, game_text_{asm_index}]")

    OUT.write_text(HEADER + "\n".join(lines) + "\n" + FOOTER)
    print(f"wrote {OUT} with {len(merged)} c subsegment(s), {asm_index + 1} asm chunk(s)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
