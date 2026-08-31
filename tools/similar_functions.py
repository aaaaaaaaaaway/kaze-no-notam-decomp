#!/usr/bin/env python3
"""Find already-matched functions whose machine code most resembles a target.

Past ~50% matched, the highest-leverage scheduling signal is no longer "how
hard does this look" but "which solved function is this most like": a matched
neighbour's hand-written C is the single best reference for the target's C
(Chris Lewis, "The Long Tail of LLM-Assisted Decompilation"; likewise the
few-shot examples in Macabeus's Mizuchi). We had tractability ranking but no
similarity retrieval; this adds it.

Similarity = normalized opcode-sequence edit distance (a Coddog-style bounded
Levenshtein over mnemonics, operands stripped) plus an opcode 3-gram Jaccard
tie-breaker. Opcodes come from disassembling the baserom text once (cached),
so this needs no pre-parsed .s and stays consistent with tools/asm_diff.py.

Usage:
  tools/similar_functions.py <target_function> [--k 5] [--json]
                             [--pool matched|all] [--with-c] [--min 0.30]
"""

from __future__ import annotations

import argparse
import importlib.util
import json
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CATALOG = ROOT / "docs/function-catalog.json"
MANIFEST = ROOT / "docs/function-manifest.json"
MATCHED = ROOT / "config/matched.json"
BASEROM = ROOT / "baserom.bin"
CACHE = ROOT / "tools/.cache/opcode_index.json"
VRAM_BASE = 0x80010000

_spec = importlib.util.spec_from_file_location("kaze_asm_diff", ROOT / "tools/asm_diff.py")
asm_diff = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(asm_diff)  # type: ignore[union-attr]
INSN_RE = asm_diff.INSN_RE
OBJDUMP = asm_diff.OBJDUMP


def read_json(path: Path, default):
    return json.loads(path.read_text()) if path.is_file() else default


def build_opcode_index() -> dict[str, list[str]]:
    """name -> [mnemonic,...] for every function, disassembling baserom once."""
    manifest = read_json(MANIFEST, {"functions": []})["functions"]
    catalog = read_json(CATALOG, {"functions": []})["functions"]
    name_by_addr = {int(e["address"], 16): e["name"] for e in catalog}
    rom = BASEROM.read_bytes()

    key = f"{BASEROM.stat().st_size}:{MANIFEST.stat().st_mtime_ns}:{CATALOG.stat().st_mtime_ns}"
    if CACHE.is_file():
        cached = read_json(CACHE, {})
        if cached.get("key") == key:
            return cached["index"]

    funcs = [(int(f["address"], 16), f["size"]) for f in manifest]
    funcs = [(a, s) for a, s in funcs if a - VRAM_BASE >= 0 and a - VRAM_BASE + s <= len(rom)]
    if not funcs:
        return {}
    lo = min(a for a, _ in funcs)
    hi = max(a + s for a, s in funcs)
    span = rom[lo - VRAM_BASE : hi - VRAM_BASE]

    with tempfile.TemporaryDirectory() as td:
        binp = Path(td) / "text.bin"
        binp.write_bytes(span)
        out = subprocess.run(
            [OBJDUMP, "-D", "-b", "binary", "-m", "mips", "-EL",
             f"--adjust-vma={lo:#x}", str(binp)],
            capture_output=True, text=True, check=True,
        ).stdout

    mnem_by_addr: dict[int, str] = {}
    for line in out.splitlines():
        m = INSN_RE.match(line)
        if m:
            mnem_by_addr[int(m.group(1), 16)] = m.group(3).split()[0] if m.group(3).split() else "?"

    index: dict[str, list[str]] = {}
    for addr, size in funcs:
        name = name_by_addr.get(addr)
        if not name:
            continue
        seq = [mnem_by_addr[a] for a in range(addr, addr + size, 4) if a in mnem_by_addr]
        if seq:
            index[name] = seq

    CACHE.parent.mkdir(parents=True, exist_ok=True)
    CACHE.write_text(json.dumps({"key": key, "index": index}))
    return index


def bounded_levenshtein(a: list[str], b: list[str]) -> int:
    if a == b:
        return 0
    if not a:
        return len(b)
    if not b:
        return len(a)
    prev = list(range(len(b) + 1))
    for i, ca in enumerate(a, 1):
        cur = [i] + [0] * len(b)
        for j, cb in enumerate(b, 1):
            cur[j] = min(prev[j] + 1, cur[j - 1] + 1, prev[j - 1] + (ca != cb))
        prev = cur
    return prev[-1]


def ngram_jaccard(a: list[str], b: list[str], n: int = 3) -> float:
    def grams(seq):
        return {tuple(seq[i : i + n]) for i in range(len(seq) - n + 1)} or {tuple(seq)}
    ga, gb = grams(a), grams(b)
    if not ga or not gb:
        return 0.0
    return len(ga & gb) / len(ga | gb)


def similarity(a: list[str], b: list[str]) -> float:
    """Blend edit-distance similarity (structure/order) with n-gram Jaccard
    (shared idioms), so neither a pure length nor a pure bag-of-ops dominates."""
    lo, hi = sorted((len(a), len(b)))
    if hi == 0 or lo / hi < 0.4:  # length pre-filter: too different to be a useful reference
        return 0.0
    edit_sim = 1.0 - bounded_levenshtein(a, b) / max(len(a), len(b))
    return 0.65 * edit_sim + 0.35 * ngram_jaccard(a, b)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("function")
    ap.add_argument("--k", type=int, default=5)
    ap.add_argument("--pool", choices=("matched", "all"), default="matched")
    ap.add_argument("--min", type=float, default=0.30, help="minimum similarity to report")
    ap.add_argument("--with-c", action="store_true", help="print matched neighbour C")
    ap.add_argument("--json", action="store_true")
    args = ap.parse_args()

    index = build_opcode_index()
    if args.function not in index:
        print(f"no opcode sequence for {args.function} "
              f"(unknown or zero-size)", file=sys.stderr)
        return 2
    target = index[args.function]

    matched = {e["function"]: e for e in read_json(MATCHED, [])}
    if args.pool == "matched":
        pool = [n for n in index if n in matched]
    else:
        pool = [n for n in index if n != args.function]

    scored = []
    for name in pool:
        if name == args.function:
            continue
        s = similarity(target, index[name])
        if s >= args.min:
            scored.append((s, name))
    scored.sort(reverse=True)
    scored = scored[: args.k]

    results = []
    for s, name in scored:
        entry = matched.get(name, {})
        results.append({
            "function": name,
            "similarity": round(s, 3),
            "insns": len(index[name]),
            "src": entry.get("src"),
            "opt": entry.get("opt"),
            "compiler": entry.get("compiler"),
        })

    if args.json:
        print(json.dumps({"target": args.function, "target_insns": len(target),
                          "neighbours": results}, indent=2))
        return 0

    print(f"# {args.function}: {len(target)} instructions, "
          f"top {len(results)} matched neighbours by opcode similarity")
    for r in results:
        print(f"  {r['similarity']:.3f}  {r['insns']:>4}i  {r['function']}"
              f"  ({r['opt'] or '?'} {r['compiler'] or ''})  {r['src'] or ''}")
        if args.with_c and r["src"]:
            p = ROOT / r["src"]
            if p.is_file():
                print("    " + "-" * 60)
                for ln in p.read_text().splitlines():
                    print(f"    {ln}")
                print("    " + "-" * 60)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
