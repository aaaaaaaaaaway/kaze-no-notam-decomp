#!/usr/bin/env python3
"""Basic-block match map for a big function, so it can be closed one block at a
time instead of all at once.

Large functions are hard to match in one pass: Chris Lewis reports automated
decompilation tools "more or less give up immediately" past ~1000 instructions,
and the 71 `big` functions here hold most of the remaining unmatched *bytes*.
The unit of progress on a big function is a basic block: with a whole-function
candidate compiled, this shows which blocks already reproduce the original
bytes and which don't, so the fix only needs to target the differing block
(and can read its instruction diff in isolation).

It reuses tools/asm_diff.py's compile+link and disassembly, so block bytes are
the real relocated bytes. Blocks are derived from the ORIGINAL disassembly
(branch/jump boundaries), independent of the candidate.

Usage:
  tools/region_match.py <src.c> <function> [--compiler 2.7.2-psx] [--opt -O2]
       [--block N]            # show the instruction diff for one block
       [--range START:END]    # compare an explicit byte window (hex or dec)
Exit: 0 if every compared block matches, 1 otherwise.
"""

from __future__ import annotations

import argparse
import difflib
import importlib.util
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

_spec = importlib.util.spec_from_file_location("kaze_asm_diff", ROOT / "tools/asm_diff.py")
asm_diff = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(asm_diff)  # type: ignore[union-attr]
match = asm_diff.match

BRANCH = {
    "j", "jal", "jr", "jalr", "b", "beq", "bne", "blez", "bgtz", "bltz", "bgez",
    "bltzal", "bgezal", "beqz", "bnez", "bgezl", "bltzl", "beql", "bnel",
    "blezl", "bgtzl", "bc1t", "bc1f", "bc1tl", "bc1fl",
}


def basic_block_leaders(insns: list[tuple[int, str, str]], vram: int, size: int) -> list[int]:
    """Instruction indices that begin a basic block, from branch boundaries."""
    n = len(insns)
    leaders = {0}
    for idx, (_off, _hx, text) in enumerate(insns):
        mn = text.split(" ", 1)[0] if text else ""
        if mn in BRANCH:
            m = re.search(r"0x([0-9a-fA-F]+)", text)
            if m:
                tgt = int(m.group(1), 16)
                if vram <= tgt < vram + size:
                    leaders.add((tgt - vram) // 4)
            if idx + 2 < n:          # branch(idx) + delay slot(idx+1); next block idx+2
                leaders.add(idx + 2)
    return sorted(l for l in leaders if 0 <= l < n)


def parse_range(text: str) -> tuple[int, int]:
    a, b = text.split(":")
    to_int = lambda s: int(s, 16) if s.lower().startswith("0x") else int(s)
    return to_int(a), to_int(b)


def normalized_instruction(text: str) -> str:
    """Normalize layout-sensitive control targets for sequence alignment."""
    mnemonic = text.split(" ", 1)[0] if text else ""
    if mnemonic in BRANCH:
        return re.sub(r"0x[0-9a-fA-F]+", "<target>", text)
    return text


def alignment_report(
    orig: list[tuple[int, str, str]],
    ours: list[tuple[int, str, str]],
    address: int,
) -> tuple[int, int]:
    """Print structural edit spans after ignoring relocated branch targets."""
    a = [normalized_instruction(insn[2]) for insn in orig]
    b = [normalized_instruction(insn[2]) for insn in ours]
    matcher = difflib.SequenceMatcher(a=a, b=b, autojunk=False)
    edits = 0
    displaced = 0
    print("alignment (branch targets normalized):")
    for tag, i1, i2, j1, j2 in matcher.get_opcodes():
        if tag == "equal":
            continue
        edits += max(i2 - i1, j2 - j1)
        displaced += (i2 - i1) + (j2 - j1)
        print(
            f"  {tag:<7} orig[{i1:>3}:{i2:<3}] "
            f"{address + i1 * 4:#010x}  ours[{j1:>3}:{j2:<3}] "
            f"{address + j1 * 4:#010x}"
        )
        for idx in range(i1, min(i2, i1 + 2)):
            print(f"    - {a[idx]}")
        if i2 - i1 > 2:
            print(f"    - ... ({i2 - i1 - 2} more)")
        for idx in range(j1, min(j2, j1 + 2)):
            print(f"    + {b[idx]}")
        if j2 - j1 > 2:
            print(f"    + ... ({j2 - j1 - 2} more)")
    print(
        f"alignment summary: {edits} edit-width, {displaced} displaced instructions, "
        f"{len(matcher.get_matching_blocks()) - 1} matching runs"
    )
    return edits, displaced


def main() -> int:
    sys.argv[1:] = asm_diff.fix_opt_argv(sys.argv[1:])
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("src")
    ap.add_argument("function")
    ap.add_argument("--compiler", default="2.7.2-psx")
    ap.add_argument("--opt", default="-O2")
    ap.add_argument("--block", type=int, help="show instruction diff for this block index")
    ap.add_argument("--range", help="compare only byte window START:END (hex or dec)")
    ap.add_argument(
        "--align",
        action="store_true",
        help="report sequence edit spans with branch targets normalized",
    )
    args = ap.parse_args()

    rodata_entry = match.load_rodata_entry(args.function)
    address, size = match.load_function(args.function, rodata_entry)
    rom_start = address - match.VRAM_BASE
    original = match.BASEROM.read_bytes()[rom_start : rom_start + size]

    try:
        ours = asm_diff.compile_function_bytes(args.src, args.function, args.compiler, args.opt)
    except Exception as exc:  # noqa: BLE001 - report, don't crash the loop
        print(f"COMPILE FAILED for {args.function}: {exc}", file=sys.stderr)
        return 2

    orig = asm_diff.disassemble(original, address)
    our = asm_diff.disassemble(ours, address)
    our_by_off = {o[0]: o for o in our}

    def word_diffs(lo_i: int, hi_i: int) -> int:
        d = 0
        for i in range(lo_i, hi_i):
            off = orig[i][0]
            u = our_by_off.get(off)
            if u is None or u[1] != orig[i][1]:
                d += 1
        return d

    color = sys.stdout.isatty()

    if args.range:
        lo, hi = parse_range(args.range)
        lo_i, hi_i = lo // 4, min(hi // 4, len(orig))
        d = word_diffs(lo_i, hi_i)
        print(f"range [{lo:#x}:{hi:#x}]  {hi_i - lo_i} insns  "
              f"{'MATCH' if d == 0 else f'DIFF {d}/{hi_i - lo_i}'}")
        if d:
            text, _ = asm_diff.render(orig[lo_i:hi_i], our[lo_i:hi_i], address + lo,
                                      (hi_i - lo_i) * 4, 0, True, color)
            print(text)
        return 0 if d == 0 else 1

    leaders = basic_block_leaders(orig, address, size)
    bounds = leaders + [len(orig)]
    blocks = [(bounds[k], bounds[k + 1]) for k in range(len(leaders))]

    if args.block is not None:
        if not (0 <= args.block < len(blocks)):
            print(f"block {args.block} out of range (0..{len(blocks) - 1})")
            return 2
        lo_i, hi_i = blocks[args.block]
        text, rc = asm_diff.render(orig[lo_i:hi_i], our[lo_i:hi_i],
                                   address + lo_i * 4, (hi_i - lo_i) * 4, 0, True, color)
        print(f"# block {args.block}: insns [{lo_i}:{hi_i}] "
              f"@ {address + lo_i * 4:#x}")
        print(text)
        return rc

    total_diff = 0
    matched_blocks = 0
    print(f"# {args.function}: {len(orig)} insns, {len(blocks)} basic blocks "
          f"({size}B)  [ours {len(our)} insns]")
    for k, (lo_i, hi_i) in enumerate(blocks):
        d = word_diffs(lo_i, hi_i)
        total_diff += d
        matched_blocks += d == 0
        tag = "MATCH" if d == 0 else f"DIFF {d}/{hi_i - lo_i}"
        print(f"  block {k:>3} [{address + lo_i * 4:#010x} +{lo_i * 4:#06x}] "
              f"{hi_i - lo_i:>3} insns  {tag}")
    print(f"summary: {matched_blocks}/{len(blocks)} blocks match, "
          f"{total_diff}/{len(orig)} instructions differ")
    if args.align:
        alignment_report(orig, our, address)
    if total_diff and matched_blocks < len(blocks):
        first_bad = next(k for k, (a, b) in enumerate(blocks) if word_diffs(a, b))
        print(f"next: fix block {first_bad}  -> tools/region_match.py {args.src} "
              f"{args.function} --opt={args.opt} --block {first_bad}")
    return 0 if total_diff == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
