#!/usr/bin/env python3
"""Regenerate config/lawful_review.json as the HONEST cheat-exclusion set.

The "lawful" board = matched functions with zero cheats. A cheat is either:
  (a) a hand-transcribed / multi-insn asm island in src (island_audit detects it), OR
  (b) a strict-policy violation island_audit can't see ($30/$fp register pins, etc.)
      -- these stay tracked in config/island_quarantine.json "functions".

So the honest exclusion set = island_audit-detected islands  UNION  manual quarantine,
minus the sanctioned-GTE list. This script rebuilds lawful_review.json from that union
so the metric is self-checking and can never silently over-count lawful again.

Run after ANY src/ change (merge, lawful swap). Idempotent.
"""
import json, re, subprocess, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

def main() -> int:
    out = subprocess.run([sys.executable, str(ROOT / "tools/island_audit.py")],
                         capture_output=True, text=True).stdout
    island_fns = set(re.findall(r'(kaze_[a-z0-9_]+)\.c\s+\d+ islands', out))
    qj = json.loads((ROOT / "config/island_quarantine.json").read_text())
    quarantine = set(qj["functions"].keys())
    sanctioned = set(qj.get("sanctioned", {}).keys()) if isinstance(qj.get("sanctioned"), dict) else set()
    exclude = (island_fns | quarantine) - sanctioned

    review_path = ROOT / "config/lawful_review.json"
    old = {e["function"]: e for e in json.loads(review_path.read_text())} if review_path.is_file() else {}
    review = []
    for fn in sorted(exclude):
        e = old.get(fn, {"function": fn})
        if fn in island_fns and "reason" not in e:
            e = {**e, "reason": "carries hand-transcribed asm island(s) per island_audit"}
        review.append(e)
    review_path.write_text(json.dumps(review, indent=1))
    print(f"lawful_review regenerated: {len(review)} excluded "
          f"({len(island_fns)} island-carrying, {len(quarantine)} quarantine, {len(sanctioned)} sanctioned)")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
