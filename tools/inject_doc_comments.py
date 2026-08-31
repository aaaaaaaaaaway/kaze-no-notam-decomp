#!/usr/bin/env python3
"""Inject function-map purpose docs as header comments into src/*.c.

For every entry in config/matched.json, looks up the function in
docs/function-map.json and writes (or refreshes) a doc comment naming
that function:

    /**
     * kaze_foo  (category, 0x80012345)
     * <wrapped purpose text>
     */

Idempotent: an existing header whose title names the function is found
by that name (wherever it sits in the file -- immediately above the
definition for a multi-function subsystem TU, or at the top of a
single-function file ahead of its includes/externs) and replaced in
place, so map edits can be re-synced by rerunning this script. A file
with no existing header for a function gets one inserted directly above
that function's definition.
Comments are stripped by cpp, so this cannot affect codegen; still, run
./build.sh afterwards to verify byte-identity.
"""
import json
import re
import textwrap
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
FMAP = ROOT / "docs/function-map.json"

HEADER_BLOCK_RE = re.compile(r"/\*\*.*?\*/\n\n?", re.DOTALL)
TITLE_NAME_RE = re.compile(r"\s*\*\s*(\S+)")


def header_for(entry: dict) -> str:
    title = f"{entry['name']}  ({entry.get('category', 'misc')}, {entry['address']})"
    body = textwrap.wrap(entry.get("purpose", "").strip(), width=74)
    lines = ["/**", f" * {title}", " *"] + [f" * {ln}" for ln in body] + [" */", ""]
    if not body:
        lines = ["/**", f" * {title}", " */", ""]
    return "\n".join(lines) + "\n"


def existing_headers(text: str) -> dict:
    """Map function name -> (start, end) span of its existing header
    comment, identified by the first word of the header's title line
    (the function name always leads that line, old or new format)."""
    headers = {}
    for m in HEADER_BLOCK_RE.finditer(text):
        lines = m.group(0).split("\n")
        if len(lines) < 2:
            continue
        tm = TITLE_NAME_RE.match(lines[1])
        if tm:
            headers[tm.group(1)] = (m.start(), m.end())
    return headers


def find_def_start(text: str, name: str):
    """Locate the start of a function's definition line (not a declaration
    or an indented extern reference inside another function's body)."""
    pattern = re.compile(r"^\w[\w \*]*?\b" + re.escape(name) + r"\s*\([^;{}]*\)\s*\{?\s*$", re.M)
    m = pattern.search(text)
    return m.start() if m else None


def inject_into_file(path: Path, entries: list) -> list:
    text = path.read_text()
    headers = existing_headers(text)
    ops = []  # (start, end, entry); start == end means "insert, no existing header"
    missing = []
    for entry in entries:
        name = entry["name"]
        if name in headers:
            start, end = headers[name]
            ops.append((start, end, entry))
            continue
        pos = find_def_start(text, name)
        if pos is None:
            missing.append(name)
            continue
        ops.append((pos, pos, entry))
    # Process bottom-to-top so earlier offsets stay valid as text is edited.
    ops.sort(key=lambda t: t[0], reverse=True)
    for start, end, entry in ops:
        new_header = header_for(entry)
        if start == end:
            pre = text[:start]
            sep = "" if not pre or pre.endswith("\n\n") else "\n"
            text = pre + sep + new_header + text[start:]
        else:
            text = text[:start] + new_header + text[end:]
    path.write_text(text)
    return missing


def main() -> None:
    fmap = json.loads(FMAP.read_text())["functions"]
    by_name = {e["name"]: e for e in fmap}
    ledger = json.loads((ROOT / "config/matched.json").read_text())

    by_file = {}
    for e in ledger:
        fn = e["function"]
        entry = by_name.get(fn) or by_name.get("kaze_" + fn)
        by_file.setdefault(e["src"], []).append((fn, entry))

    done, missing = 0, []
    for src, items in by_file.items():
        path = ROOT / src
        entries = [entry for fn, entry in items if entry is not None]
        missing.extend(fn for fn, entry in items if entry is None)
        file_missing = inject_into_file(path, entries)
        missing.extend(file_missing)
        done += len(entries) - len(file_missing)
    print(f"doc comments injected: {done}/{len(ledger)}")
    if missing:
        print(f"no function-map entry for {len(missing)}: {missing[:10]}")


if __name__ == "__main__":
    main()
