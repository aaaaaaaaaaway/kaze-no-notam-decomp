#!/usr/bin/env python3
"""Export the matched-function ledger as an objdiff v2 report for decomp.dev.

Runs without a disc image. Scope is the same as tools/progress.py: the 648 game
functions. PsyQ SDK code is assembled from disassembly and is not counted.
Units are the src/*.c files; a function not in the ledger gets its own unit.
Schema: https://github.com/encounter/objdiff/blob/main/objdiff-core/protos/report.proto
"""
import argparse
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def measures(functions, matched):
    size = sum(f["size"] for f in functions)
    done = [f for f in functions if f["name"] in matched]
    done_size = sum(f["size"] for f in done)
    byte_percent = 100.0 * done_size / size if size else 0.0
    return {
        "fuzzy_match_percent": byte_percent,
        "total_code": str(size),
        "matched_code": str(done_size),
        "matched_code_percent": byte_percent,
        "total_functions": len(functions),
        "matched_functions": len(done),
        "matched_functions_percent": 100.0 * len(done) / len(functions) if functions else 0.0,
        "complete_code": str(done_size),
        "complete_code_percent": byte_percent,
        "total_units": 1,
        "complete_units": 1 if functions and len(done) == len(functions) else 0,
    }


def build_report(root):
    load = lambda name: json.loads((root / name).read_text())
    catalog = load("docs/function-catalog.json")
    sizes = {int(f["address"], 16): f["size"] for f in load("docs/function-manifest.json")["functions"]}
    rodata_sizes = {}
    if (root / "config/rodata_matches.json").is_file():
        for entry in load("config/rodata_matches.json"):
            if "text_size" in entry:
                rodata_sizes[entry["function"]] = int(entry["text_size"])
    ledger = load("config/matched.json")
    src_by_name = {e["function"]: e["src"] for e in ledger}

    functions = []
    for e in catalog["functions"]:
        if not e["raw"].startswith("FUN_"):
            continue
        address = int(e["address"], 16)
        functions.append({"name": e["name"], "address": address,
                          "size": rodata_sizes.get(e["name"], sizes[address])})
    functions.sort(key=lambda f: f["address"])
    unknown = set(src_by_name) - {f["name"] for f in functions}
    if unknown:
        raise ValueError("ledger names functions outside the game set: " + ", ".join(sorted(unknown)))
    matched = set(src_by_name)

    groups = {}
    for f in functions:
        groups.setdefault(src_by_name.get(f["name"], "unmatched/" + f["name"]), []).append(f)
    units = []
    for path, members in sorted(groups.items(), key=lambda kv: kv[1][0]["address"]):
        unit_measures = measures(members, matched)
        metadata = {"complete": unit_measures["complete_units"] == 1, "module_name": "SLPS_009.12"}
        if path.startswith("src/"):
            metadata["source_path"] = path
        units.append({
            "name": path[4:-2] if path.startswith("src/") else path,
            "measures": unit_measures,
            "functions": [{"name": f["name"], "size": str(f["size"]),
                           "fuzzy_match_percent": 100.0 if f["name"] in matched else 0.0,
                           "metadata": {"virtual_address": str(f["address"])}}
                          for f in members],
            "metadata": metadata,
        })
    total = measures(functions, matched)
    total["total_units"] = len(units)
    total["complete_units"] = sum(u["measures"]["complete_units"] for u in units)
    return {"measures": total, "units": units, "version": 2, "categories": []}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    report = build_report(ROOT)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2, allow_nan=False) + "\n")
    m = report["measures"]
    print(f"wrote {len(report['units'])} units to {args.output}: "
          f"{m['matched_functions']}/{m['total_functions']} functions, "
          f"{m['matched_code']}/{m['total_code']} bytes")


if __name__ == "__main__":
    main()
