#!/usr/bin/env python3
"""Matching progress: functions and bytes matched vs the 648-function goal."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
catalog = json.loads((ROOT / "docs/function-catalog.json").read_text())
manifest = json.loads((ROOT / "docs/function-manifest.json").read_text())
sizes = {int(f["address"], 16): f["size"] for f in manifest["functions"]}
by_name = {e["name"]: e for e in catalog["functions"]}
rodata_path = ROOT / "config/rodata_matches.json"
rodata_sizes = {}
if rodata_path.is_file():
    for entry in json.loads(rodata_path.read_text()):
        if "text_size" in entry:
            rodata_sizes[entry["function"]] = int(entry["text_size"])


def function_size(name: str) -> int:
    if name in rodata_sizes:
        return rodata_sizes[name]
    return sizes[int(by_name[name]["address"], 16)]

game = [e for e in catalog["functions"] if e["raw"].startswith("FUN_")]
game_bytes = sum(function_size(e["name"]) for e in game)

ledger_path = ROOT / "config/matched.json"
ledger = json.loads(ledger_path.read_text()) if ledger_path.is_file() else []
matched_bytes = sum(function_size(e["function"]) for e in ledger)
matched_names = {e["function"] for e in ledger}

review_path = ROOT / "config/lawful_review.json"
lawful_review = json.loads(review_path.read_text()) if review_path.is_file() else []
review_names = {entry["function"] for entry in lawful_review}
unknown_review = review_names - matched_names
if unknown_review:
    raise SystemExit(
        "lawful review contains non-matched functions: " + ", ".join(sorted(unknown_review))
    )
lawful_bytes = matched_bytes - sum(function_size(name) for name in review_names)
lawful_count = len(ledger) - len(review_names)

print(f"matched functions: {len(ledger)} / {len(game)} ({100*len(ledger)/len(game):.2f}%)")
print(f"matched bytes:     {matched_bytes} / {game_bytes} ({100*matched_bytes/game_bytes:.2f}%)")
if lawful_review:
    print(f"lawful accepted:   {lawful_count} / {len(game)} ({100*lawful_count/len(game):.2f}%)")
    print(f"lawful bytes:      {lawful_bytes} / {game_bytes} ({100*lawful_bytes/game_bytes:.2f}%)")
    print("lawful review:     " + ", ".join(sorted(review_names)))
