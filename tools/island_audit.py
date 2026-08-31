#!/usr/bin/env python3
"""island_audit.py -- audit src/ (or any .c set) for code-EMITTING multi-insn
inline asm ("islands"). Lawful constructs pass: register pins
(`register int x __asm__("$17")`), zero-code barriers/launders ("" templates,
"m"/"r" keepers), and one-insn constant forms (`asm("li %0,K")`, single
mnemonic). Everything with >1 emitted MIPS instruction in one asm template is
flagged.

Motivation (2026-07-10): 15 of 26 functions merged on 2026-07-10 carry
multi-insn islands, escalating to 96-insn whole-body transcriptions
(grid_select_pulse). The island policy decision is owned by the project's
strict-until-re-decision rule; this tool makes the lawful board visible.

2026-07-11 macro fix: the scanner previously saw only literal asm string
templates. `#define`-wrapped islands were invisible twice over (the
string-literal walker stopped at backslash-newline continuations, and a
template hidden behind a macro name like `SDIV_BODY` was never resolved).
Two independent sweeps proved evasions (SDIV in 10+ merged src
files; CHECKED_DIV in kaze_gas_floor_reset). The scanner now
splices continuations, builds a FILE-LOCAL macro table (nested references
resolved), expands macro invocations in the scan text (definitions themselves
are not counted; each invocation is), and counts a broader mnemonic set
(bnez/beqz/break/divu/... were previously missed). Macros from #include'd
headers (e.g. include/gte.h sanctioned GTE forms) are deliberately NOT
resolved -- the sanction registry, not this scanner, owns that class.
Newly-flagged files change no registry by themselves: quarantine/sanction
bookkeeping in config/island_quarantine.json stays user-owned policy.

    python3 tools/island_audit.py                 # audit src/*.c, summary
    python3 tools/island_audit.py --verbose       # per-island detail
    python3 tools/island_audit.py FILE.c ...      # audit specific files
"""
from __future__ import annotations

import argparse
import glob
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

ASM_OPEN = re.compile(r'(?:__)?asm(?:__)?\s*(?:volatile\s*)?\(', re.S)
STR_LIT = re.compile(r'\s*"((?:[^"\\]|\\.)*)"')
MNEMONIC = re.compile(r'\b(lw|sw|lh|sh|lb|sb|lbu|lhu|addiu|addu|subu|sll|srl|sra'
                      r'|lui|ori|andi|mult|div|mflo|mfhi|j|jal|jr|beq|bne|nop'
                      r'|lwl|lwr|swl|swr|mfc2|mtc2|cfc2|ctc2|slt|slti|sltu|sltiu'
                      r'|and|or|xor|nor|sub|add'
                      r'|bnez|beqz|blez|bgez|bltz|bgtz|b|break|divu|multu'
                      r'|mtlo|mthi|jalr|negu|move|li|la|xori|sllv|srlv|srav'
                      r'|lwc2|swc2|seq|sne)\b')
CONT = re.compile(r'\\\r?\n')
DEFINE = re.compile(r'^[ \t]*#[ \t]*define[ \t]+(\w+)(\([^)]*\))?[ \t]*(.*?)[ \t]*$',
                    re.M)


def _macro_table(text):
    """FILE-LOCAL macro name -> body, with nested references resolved
    (bounded). #include'd macros (e.g. sanctioned gte.h forms) are
    deliberately out of scope: the sanction registry owns that class."""
    macros = {m.group(1): m.group(3) for m in DEFINE.finditer(text)}
    for _ in range(8):
        changed = False
        for name, body in list(macros.items()):
            new = re.sub(
                r'\b(\w+)\b',
                lambda mm: macros[mm.group(1)]
                if mm.group(1) in macros and mm.group(1) != name
                else mm.group(1),
                body)
            if new != body:
                macros[name] = new
                changed = True
        if not changed:
            break
    return macros


def _scan_text(text):
    """Spliced, macro-expanded text to scan: continuations joined, #define
    lines removed (definitions alone emit nothing), every macro INVOCATION
    replaced by its resolved body so each use is counted."""
    text = CONT.sub(' ', text)
    macros = _macro_table(text)
    scan = DEFINE.sub(' ', text)
    if macros:
        pat = re.compile(r'\b(' + '|'.join(re.escape(n) for n in macros) + r')\b')
        for _ in range(8):
            new = pat.sub(lambda mm: macros[mm.group(1)], scan)
            if new == scan:
                break
            scan = new
    return scan


def islands(path):
    """Return (multi, pads) for path: multi = [(insn_count, tpl), ...] for
    multi-insn asm templates; pads = [tpl, ...] for single-insn templates with
    no %-operand substitution (bare hardcoded instructions such as
    `asm volatile("nop")`: pure code-emitting pads). Lawful
    one-insn constant forms (`asm("li %0,K" : "=r"(v))`) carry a %-operand and
    are not flagged. Macro-wrapped asm is expanded per _scan_text (2026-07-11
    fix): each macro invocation is counted, definitions alone are not."""
    try:
        text = Path(path).read_text()
    except OSError:
        return [], []
    multi, pads = [], []
    scan = _scan_text(text)
    for m in ASM_OPEN.finditer(scan):
        pos = m.end()
        tpl = ""
        while (sm := STR_LIT.match(scan, pos)):
            tpl += sm.group(1)
            pos = sm.end()
        cleaned = re.sub(r'\\[nt]|[;\n\t]', ' ', tpl)
        n = len(MNEMONIC.findall(cleaned))
        if n > 1:
            multi.append((n, tpl))
        elif n == 1 and "%" not in tpl:
            pads.append(tpl)
    return multi, pads


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("files", nargs="*", help="default: src/*.c")
    ap.add_argument("--verbose", action="store_true")
    ap.add_argument("--json", help="write full report")
    ap.add_argument("--board", action="store_true",
                    help="print lawful vs nominal board using the regenerated "
                    "config/lawful_review.json exclusion set")
    args = ap.parse_args()
    if args.board:
        matched = json.loads((ROOT / "config/matched.json").read_text())
        q = json.loads((ROOT / "config/island_quarantine.json").read_text())
        qfns = set(q["functions"])
        sanctioned = set(q.get("sanctioned", {}))
        review_path = ROOT / "config/lawful_review.json"
        review = json.loads(review_path.read_text()) if review_path.is_file() else []
        review_fns = {e["function"] for e in review}
        matched_fns = {e["function"] for e in matched}
        unknown = review_fns - matched_fns
        if unknown:
            raise SystemExit("lawful review contains non-matched functions: "
                             + ", ".join(sorted(unknown)))
        total = 648
        nominal = len(matched)
        lawful = nominal - len(review_fns)
        print(f"nominal board : {nominal}/{total} ({nominal/total:.2%}), includes "
              f"{len(qfns)} quarantined island-assisted merges "
              f"(+{len(sanctioned)} ratified GTE-class, counted lawful)")
        print(f"lawful board  : {lawful}/{total} ({lawful/total:.2%}), the "
              f"strict-policy metric; excludes {len(review_fns)} entries from "
              f"the regenerated lawful review")
        return 0
    files = args.files or sorted(glob.glob(str(ROOT / "src/*.c")))
    report = []
    pad_report = []
    for f in files:
        isl, pads = islands(f)
        if pads:
            pad_report.append({"file": str(f), "pads": len(pads),
                               "templates": [t[:120] for t in pads]})
        if not isl:
            continue
        report.append({"file": str(f), "islands": len(isl),
                       "insns": sum(n for n, _ in isl),
                       "templates": [t[:120] for _n, t in isl]})
        if args.verbose:
            print(f"{f}: {len(isl)} island(s), {sum(n for n, _ in isl)} insns")
            for n, t in isl:
                print(f"    [{n}] {t[:110]}")
    clean = len(files) - len(report)
    total_insns = sum(r["insns"] for r in report)
    print(f"{len(report)} of {len(files)} files carry multi-insn asm islands "
          f"({total_insns} emitted insns total); {clean} clean.")
    for r in sorted(report, key=lambda r: -r["insns"]):
        print(f"  {Path(r['file']).name:50} {r['islands']:>3} islands "
              f"{r['insns']:>4} insns")
    if pad_report:
        total_pads = sum(r["pads"] for r in pad_report)
        print(f"PAD WARNING: {len(pad_report)} file(s) carry "
              f"{total_pads} single-insn no-operand code-emitting asm pad(s) "
              f"(bare nop/hardcoded insns). Pre-existing GTE-family hazard "
              f"nops are sanctioned; anything NEW fails the merge gate.")
        if args.verbose:
            for r in pad_report:
                print(f"  {Path(r['file']).name:50} {r['pads']:>3} pad(s): "
                      f"{'; '.join(r['templates'])}")
    if args.json:
        Path(args.json).write_text(json.dumps(
            {"islands": report, "pads": pad_report}, indent=1) + "\n")
        print(f"wrote {args.json}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
