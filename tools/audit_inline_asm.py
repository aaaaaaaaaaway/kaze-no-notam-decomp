#!/usr/bin/env python3
"""List direct nonempty inline-assembly templates in matched C sources."""

from __future__ import annotations

import argparse
import ast
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ASM_START = re.compile(r"\b(?:__asm__|asm)\s*(?:volatile\s*)?\(\s*")
STRING = re.compile(r'"(?:\\.|[^"\\])*"')


def templates(path: Path) -> list[dict[str, object]]:
    text = path.read_text()
    found = []
    for match in ASM_START.finditer(text):
        pos = match.end()
        parts: list[str] = []
        while True:
            while pos < len(text) and text[pos].isspace():
                pos += 1
            literal = STRING.match(text, pos)
            if literal is None:
                break
            parts.append(ast.literal_eval(literal.group(0)))
            pos = literal.end()
        template = "".join(parts).strip()
        if not template:
            continue
        found.append(
            {
                "line": text.count("\n", 0, match.start()) + 1,
                "template": template,
                "instructions": len([line for line in template.splitlines() if line.strip()]),
            }
        )
    return found


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--json", action="store_true")
    parser.add_argument(
        "--min-instructions",
        type=int,
        default=2,
        help="minimum template instruction lines to report (default: 2)",
    )
    args = parser.parse_args()

    matched_path = ROOT / "config/matched.json"
    matched = json.loads(matched_path.read_text()) if matched_path.is_file() else []
    report = []
    for entry in matched:
        path = ROOT / entry["src"]
        direct = [
            item for item in templates(path) if item["instructions"] >= args.min_instructions
        ]
        if direct:
            report.append({"function": entry["function"], "source": entry["src"], "asm": direct})

    if args.json:
        print(json.dumps(report, indent=2))
    else:
        for entry in report:
            details = ", ".join(
                f"line {item['line']} ({item['instructions']} insn)" for item in entry["asm"]
            )
            print(f"{entry['function']}: {details}")
        print(
            f"direct asm with >= {args.min_instructions} instructions: "
            f"{len(report)} / {len(matched)} matched functions"
        )
    return 1 if report else 0


if __name__ == "__main__":
    raise SystemExit(main())
