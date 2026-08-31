#!/usr/bin/env python3
"""Repository consistency checks. Runs without a disc image."""
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
errors = []

for p in sorted(ROOT.glob("config/*.json")) + sorted(ROOT.glob("docs/*.json")):
    try:
        json.loads(p.read_text())
    except Exception as exc:
        errors.append(f"{p.relative_to(ROOT)}: invalid JSON: {exc}")

ledger = json.loads((ROOT / "config/matched.json").read_text())
for e in ledger:
    if not (ROOT / e["src"]).is_file():
        errors.append(f"config/matched.json references missing file {e['src']}")

names = {e["function"] for e in ledger}
if len(names) != len(ledger):
    errors.append(f"config/matched.json has duplicate function entries")

srcs = {p.name for p in (ROOT / "src").glob("*.c")}
listed = {Path(e["src"]).name for e in ledger}
for orphan in sorted(srcs - listed):
    errors.append(f"src/{orphan} is not listed in config/matched.json")

if errors:
    print(f"{len(errors)} problem(s):")
    for e in errors:
        print("  " + e)
    sys.exit(1)
print(f"consistency OK: {len(ledger)} functions, {len(srcs)} source files")
