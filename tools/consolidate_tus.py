#!/usr/bin/env python3
"""Consolidate per-function src/*.c files into multi-function translation units.

Grouping: contiguous-in-ROM runs of matched functions sharing (opt, compiler).
Functions with rodata_matches entries (switch tables) and files containing
file-scope `static` stay standalone (they interact with linking/rodata
placement); a run splits around them.

Merge: concatenate files in address order under a single deduplicated prelude
(typedefs, #includes, #defines, struct/union blocks, extern declarations).
A file whose prelude conflicts (same name, different text) with the run's
prelude splits the run at that point.

Verification: every merged TU is compiled with the run's exact toolchain
pipeline and its .text section byte-compared against the baserom slice
covering the run. Only verified TUs are written; matched.json is updated.
Run ./build.sh --resplit afterwards for the whole-EXE proof.

Usage: consolidate_tus.py [--dry-run] [--keep-singles]
"""
from __future__ import annotations

import argparse
import json
import re
import subprocess
import tempfile
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
VRAM_BASE = 0x80010000

AS = "mipsel-linux-gnu-as"
OBJCOPY = "mipsel-linux-gnu-objcopy"
ASFLAGS = ["-EL", "-march=r3000", "-mabi=32", "-no-pad-sections", "-Iinclude"]

PRELUDE_KINDS = ("typedef", "include", "define", "block", "extern", "other")
COMPILE_ERRS: list = []


def load_inputs():
    catalog = json.loads((ROOT / "docs/function-catalog.json").read_text())
    by_name = {e["name"]: e for e in catalog["functions"]}
    manifest = json.loads(
        (ROOT / "docs/function-manifest.json").read_text()
    )
    sizes = {int(f["address"], 16): f["size"] for f in manifest["functions"]}
    rodata = json.loads((ROOT / "config/rodata_matches.json").read_text())
    size_ov = {e["function"]: int(e["text_size"]) for e in rodata if "text_size" in e}
    rodata_fns = {e["function"] for e in rodata}
    ledger = json.loads((ROOT / "config/matched.json").read_text())
    fmap = json.loads((ROOT / "docs/function-map.json").read_text())["functions"]
    categ = {e["name"]: e["category"] for e in fmap}
    rows = []
    for e in ledger:
        fn = e["function"]
        addr = int(by_name[fn]["address"], 16)
        rom_start = addr - VRAM_BASE
        rom_end = rom_start + size_ov.get(fn, sizes[addr])
        rows.append(
            {
                "fn": fn,
                "src": ROOT / e["src"],
                "rom_start": rom_start,
                "rom_end": rom_end,
                "opt": e.get("opt", "-O1"),
                "compiler": e.get("compiler", "2.7.2-psx"),
                "category": categ.get(fn, "misc"),
                "standalone": fn in rodata_fns,
            }
        )
    rows.sort(key=lambda r: r["rom_start"])
    return rows, ledger


STATIC_RE = re.compile(r"^static\b", re.M)


def split_file(text: str, fn: str):
    """Split a single-function file into (prelude_items, body_text).

    prelude_items: list of (kind, key, text) where key identifies the
    declaration for dedup/conflict detection. Returns None on parse trouble.
    """
    lines = text.split("\n")
    # find the top-level line where the function definition starts: the
    # signature line containing "<fn>(" at brace depth 0, possibly with the
    # return type on the previous line.
    depth = 0
    sig_idx = None
    for i, ln in enumerate(lines):
        if depth == 0 and re.search(r"^\s*[A-Za-z_].*\b%s\s*\(" % re.escape(fn), ln):
            sig_idx = i
            break
        depth += ln.count("{") - ln.count("}")
    if sig_idx is None:
        return None
    # return type on its own preceding line (e.g. "void\nfn(...)")
    if sig_idx > 0 and re.match(
        r"^\s*[A-Za-z_][A-Za-z0-9_ \t*]*$", lines[sig_idx - 1] or " "
    ) and lines[sig_idx - 1].strip():
        sig_idx -= 1
    body = "\n".join(lines[sig_idx:])
    prelude_text = "\n".join(lines[:sig_idx])

    # strip the leading doc comment (it belongs with the body)
    doc = ""
    m = re.match(r"\s*(/\*\*.*?\*/\s*\n)", prelude_text, re.S)
    if m:
        doc = m.group(1)
        prelude_text = prelude_text[m.end():]
    body = doc + body

    items = []
    rest = prelude_text
    while rest.strip():
        rest = rest.lstrip()
        m = re.match(r"/\*.*?\*/\s*\n?", rest, re.S)
        if m:
            items.append(("other", None, m.group(0).rstrip("\n")))
            rest = rest[m.end():]
            continue
        m = re.match(r"#\s*include[^\n]*", rest)
        if m:
            items.append(("include", m.group(0).strip(), m.group(0)))
            rest = rest[m.end():]
            continue
        m = re.match(r"#\s*define\s+(\w+)(?:\([^)]*\))?(?:[^\n]*\\\n)*[^\n]*", rest)
        if m:
            items.append(("define", "define:" + m.group(1), m.group(0)))
            rest = rest[m.end():]
            continue
        m = re.match(r"typedef[^{;]*;", rest)
        if m:
            name = re.findall(r"(\w+)\s*;$", m.group(0).strip())
            items.append(("typedef", "typedef:" + (name[0] if name else m.group(0)), m.group(0)))
            rest = rest[m.end():]
            continue
        # brace-balanced block: struct/union/enum/typedef-struct definition
        m = re.match(r"(?:typedef\s+)?(?:struct|union|enum)\b[^{;]*", rest)
        if m and rest[m.end():m.end() + 1] == "{":
            i = m.end()
            depth = 0
            while i < len(rest):
                if rest[i] == "{":
                    depth += 1
                elif rest[i] == "}":
                    depth -= 1
                    if depth == 0:
                        break
                i += 1
            end = rest.index(";", i) + 1
            block = rest[:end]
            tag = re.match(r"(?:typedef\s+)?(?:struct|union|enum)\s+(\w+)?", block)
            trailing = re.findall(r"}\s*([\w, *]+)\s*;$", block)
            key = "block:" + (tag.group(1) or (trailing[0] if trailing else block[:40]))
            items.append(("block", key, block))
            rest = rest[end:]
            continue
        m = re.match(r"(?:extern|[A-Za-z_])[^;{]*;", rest)
        if m:
            decl = m.group(0)
            names = re.findall(r"(\w+)\s*(?:\[[^\]]*\])?\s*(?:,|;|\))", decl)
            key = "extern:" + (names[-1] if names else decl)
            # asm-alias externs must keep their exact text as identity
            items.append(("extern", key, decl))
            rest = rest[m.end():]
            continue
        return None  # unrecognized prelude construct
    return items, body


def norm(text: str) -> str:
    return re.sub(r"\s+", " ", text).strip()


def blank_comments(text: str) -> str:
    return re.sub(r"/\*.*?\*/", lambda m: " " * len(m.group(0)), text, flags=re.S)


def inject_block_decl(body: str, decl: str, fn: str):
    """Move a file-scope extern decl to block scope in the function body."""
    blanked = blank_comments(body)
    m = re.search(r"\b%s\s*\(" % re.escape(fn), blanked)
    if not m:
        return None
    brace = blanked.find("{", m.end())
    if brace == -1:
        return None
    d = decl.strip()
    if not d.startswith("extern"):
        d = "extern " + d
    return body[: brace + 1] + "\n    " + d + body[brace + 1:]


def decl_symbol(decl: str) -> str | None:
    """Best-effort name of the symbol a file-scope declaration declares."""
    d = norm(decl)
    m = re.search(r"(\w+)\s*\(", d)
    if m and m.group(1) not in ("asm", "__asm__", "volatile"):
        return m.group(1)
    names = re.findall(r"(\w+)\s*(?:\[[^\]]*\])?\s*[,;=]", d)
    return names[-1] if names else None


def derive_prototype(body: str, fn: str) -> str | None:
    """Build an unprototyped forward declaration (`<ret> fn();`) from the
    definition inside `body`. Unprototyped keeps caller codegen identical even
    when the original caller declared/called it with a different arity."""
    blanked = blank_comments(body)
    m = re.search(r"([A-Za-z_][\w \t*]*?)\b%s\s*\([^{;]*\)\s*\{" % re.escape(fn),
                  blanked)
    if not m:
        return None
    ret = norm(m.group(1)) or "int"
    return f"{ret} {fn}();"


def try_merge(run, gcc_dir: Path, opt: str, baserom: bytes, why: Counter):
    """Return merged TU text if it byte-verifies, else None."""
    parsed_files = []
    for r in run:
        parsed = split_file(r["src"].read_text(), r["fn"])
        if parsed is None:
            why["parse"] += 1
            return None
        parsed_files.append(parsed)

    defined = {r["fn"]: i for i, r in enumerate(run)}

    # sweep 1: hoist all typedef/struct/union/enum definitions to the TU top,
    # renaming conflicting same-name-different-text variants per file
    hoisted: list[str] = []
    type_variants: dict[tuple[str, str], str] = {}  # (name, normtext) -> name
    type_names_used: dict[str, int] = {}
    file_renames: list[dict[str, str]] = []
    for i, (items, body) in enumerate(parsed_files):
        renames: dict[str, str] = {}
        for kind, key, text in items:
            if kind not in ("typedef", "block"):
                continue
            name = key.split(":", 1)[1]
            if not re.fullmatch(r"\w+", name):
                why["type-key"] += 1
                return None
            variant = (name, norm(text))
            if variant in type_variants:
                assigned = type_variants[variant]
            else:
                n = type_names_used.get(name, 0)
                assigned = name if n == 0 else f"{name}__{n}"
                type_names_used[name] = n + 1
                type_variants[variant] = assigned
                hoisted.append(
                    text if assigned == name
                    else re.sub(r"\b%s\b" % re.escape(name), assigned, text)
                )
            if assigned != name:
                renames[name] = assigned
        file_renames.append(renames)

    def apply_renames(text: str, renames: dict[str, str]) -> str:
        for old, new in renames.items():
            text = re.sub(r"\b%s\b" % re.escape(old), new, text)
        return text

    bodies = [
        apply_renames(body, file_renames[i])
        for i, (_, body) in enumerate(parsed_files)
    ]
    protos = {
        run[i]["fn"]: derive_prototype(bodies[i], run[i]["fn"])
        for i in range(len(run))
    }

    # find extern symbols declared with conflicting text across files
    decl_texts: dict[str, set[str]] = {}
    for i, (items, _) in enumerate(parsed_files):
        for kind, key, text in items:
            if kind == "extern":
                sym = decl_symbol(text) or text
                decl_texts.setdefault(sym, set()).add(
                    norm(apply_renames(text, file_renames[i]))
                )
    conflicted = {s for s, texts in decl_texts.items() if len(texts) > 1}

    # sweep 2: emit per-file preludes (sans hoisted types) + bodies
    seen: dict[str, str] = {}
    chunks: list[str] = []
    for i, (items, _) in enumerate(parsed_files):
        fn = run[i]["fn"]
        body = bodies[i]
        pre_out = []
        for kind, key, text in items:
            if kind in ("typedef", "block"):
                continue
            text = apply_renames(text, file_renames[i])
            if kind == "extern":
                sym = decl_symbol(text) or ""
                if sym in defined:
                    if defined[sym] < i:
                        continue  # definition already visible
                    proto = protos.get(sym)
                    if proto is None:
                        why["no-proto"] += 1
                        return None
                    if seen.get("proto:" + sym) != proto:
                        seen["proto:" + sym] = proto
                        pre_out.append(proto)
                    continue
                if sym in conflicted:
                    new_body = inject_block_decl(body, text, fn)
                    if new_body is None:
                        why["extern-conflict"] += 1
                        return None
                    body = new_body
                    continue
                key = "extern:" + sym
            if key is not None and key in seen:
                if norm(seen[key]) == norm(text):
                    continue  # identical duplicate, already emitted
                if kind == "define":
                    pre_out.append("#undef " + key.split(":", 1)[1])
                    pre_out.append(text)
                    continue
                why["conflict-" + kind] += 1
                return None
            if key is not None:
                seen[key] = text
            pre_out.append(text)
        chunks.append("\n".join(pre_out + ["", body.rstrip("\n")]))
    tu = "\n".join(hoisted) + "\n\n" + "\n\n".join(chunks) + "\n"

    with tempfile.TemporaryDirectory() as td:
        tmp = Path(td)
        cfile = tmp / "tu.c"
        cfile.write_text(tu)
        obj = tmp / "tu.o"
        try:
            cpp = subprocess.run(
                [str(gcc_dir / "cpp"), "-Iinclude", str(cfile)],
                cwd=ROOT, capture_output=True, check=True,
            )
            cc1 = subprocess.run(
                [str(gcc_dir / "cc1"), opt, "-mips1", "-G0", "-quiet", "-o", "-", "-"],
                cwd=ROOT, input=cpp.stdout, capture_output=True, check=True,
            )
            masp = subprocess.run(
                ["python3", "tools/maspsx/maspsx.py", "--expand-li", "--expand-div"],
                cwd=ROOT, input=cc1.stdout, capture_output=True, check=True,
            )
            subprocess.run(
                [AS, *ASFLAGS, "-o", str(obj), "-"],
                cwd=ROOT, input=masp.stdout, capture_output=True, check=True,
            )
            subprocess.run(
                [OBJCOPY, "--set-section-alignment", ".text=4",
                 "--set-section-alignment", ".data=4",
                 "--set-section-alignment", ".rodata=4", str(obj)],
                cwd=ROOT, capture_output=True, check=True,
            )
            # link .text at the run's vram with all known symbol addresses so
            # relocations resolve exactly as in the full build
            own = {r["fn"] for r in run}
            defs = []
            for p in ("config/symbol_addrs.txt", "undefined_syms_auto.txt",
                      "undefined_funcs_auto.txt"):
                fp = ROOT / p
                if not fp.is_file():
                    continue
                for line in fp.read_text().splitlines():
                    line = line.split("//")[0].strip()
                    if re.match(r"^[A-Za-z_]\w*\s*=\s*0x[0-9A-Fa-f]+;?$", line):
                        if line.split("=")[0].strip() in own:
                            continue
                        defs.append(line if line.endswith(";") else line + ";")
            (tmp / "syms.ld").write_text("\n".join(defs) + "\n")
            vram = run[0]["rom_start"] + VRAM_BASE
            (tmp / "tu.ld").write_text(
                "SECTIONS { . = 0x%X; .text : { *(.text) } /DISCARD/ : { *(*) } }\n"
                % vram
            )
            subprocess.run(
                ["mipsel-linux-gnu-ld", "-EL", "-T", str(tmp / "tu.ld"),
                 "-T", str(tmp / "syms.ld"), "-e", hex(vram),
                 "--no-check-sections", "-o", str(tmp / "tu.elf"), str(obj)],
                cwd=ROOT, capture_output=True, check=True,
            )
            subprocess.run(
                [OBJCOPY, "-O", "binary", "--only-section", ".text",
                 str(tmp / "tu.elf"), str(tmp / "text.bin")],
                cwd=ROOT, capture_output=True, check=True,
            )
        except subprocess.CalledProcessError as e:
            why["compile"] += 1
            if len(COMPILE_ERRS) < 3:
                COMPILE_ERRS.append(
                    (run[0]["fn"], (e.stderr or b"")[-500:]))
            return None
        got = (tmp / "text.bin").read_bytes()
    want = baserom[run[0]["rom_start"]: run[-1]["rom_end"]]
    if got != want:
        why["bytes"] += 1
        return None
    return tu


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    rows, ledger = load_inputs()
    baserom = (ROOT / "baserom.bin").read_bytes()
    for r in rows:
        if STATIC_RE.search(r["src"].read_text()):
            r["standalone"] = True

    # contiguous same-flag runs, split at standalone functions
    runs, cur = [], []
    for r in rows:
        if r["standalone"]:
            if cur:
                runs.append(cur)
                cur = []
            continue
        if cur and (
            r["rom_start"] != cur[-1]["rom_end"]
            or (r["opt"], r["compiler"]) != (cur[-1]["opt"], cur[-1]["compiler"])
        ):
            runs.append(cur)
            cur = []
        cur.append(r)
    if cur:
        runs.append(cur)
    multi = [x for x in runs if len(x) >= 2]
    print(f"{len(runs)} runs, {len(multi)} mergeable (>=2 fns), "
          f"{sum(len(x) for x in multi)} functions")

    counters: Counter = Counter()
    why: Counter = Counter()
    merged_map = {}  # fn -> new src path
    new_files = {}
    for run in multi:
        gcc_dir = ROOT / f"tools/gcc-{run[0]['compiler']}"
        # try the whole run; on failure, greedily bisect into halves
        stack = [run]
        accepted = []
        while stack:
            seg = stack.pop()
            if len(seg) < 2:
                counters["left_single"] += len(seg)
                continue
            tu = try_merge(seg, gcc_dir, seg[0]["opt"], baserom, why)
            if tu is not None:
                accepted.append((seg, tu))
            else:
                mid = len(seg) // 2
                stack.append(seg[:mid])
                stack.append(seg[mid:])
        for seg, tu in accepted:
            cat = Counter(x["category"] for x in seg).most_common(1)[0][0]
            counters["merged_tus"] += 1
            counters["merged_fns"] += len(seg)
            name = f"{cat}_{seg[0]['rom_start'] + VRAM_BASE:08x}.c"
            new_files[name] = (seg, tu)

    print(f"verified TUs: {counters['merged_tus']} covering "
          f"{counters['merged_fns']} functions; "
          f"functions left standalone in failed segments: {counters['left_single']}")
    print("failure reasons:", dict(why))
    for fn, err in COMPILE_ERRS:
        print(f"  compile-sample [{fn}]:", err.decode(errors="replace")[-400:])
    if args.dry_run:
        for name, (seg, _) in sorted(new_files.items()):
            print(f"  {name}: {len(seg)} fns "
                  f"({seg[0]['fn']} .. {seg[-1]['fn']})")
        return

    for name, (seg, tu) in new_files.items():
        (ROOT / "src" / name).write_text(tu)
        for r in seg:
            merged_map[r["fn"]] = f"src/{name}"
            r["src"].unlink()
    for e in ledger:
        if e["function"] in merged_map:
            e["src"] = merged_map[e["function"]]
    (ROOT / "config/matched.json").write_text(json.dumps(ledger, indent=1) + "\n")
    print(f"wrote {len(new_files)} TU files, updated config/matched.json; "
          f"src now has {len(list((ROOT / 'src').glob('*.c')))} files")


if __name__ == "__main__":
    main()
