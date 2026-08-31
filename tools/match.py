#!/usr/bin/env python3
"""Byte-true single-function match harness.

Usage: tools/match.py <src.c> <function_name> [--compiler 2.7.2-psx] [--quiet]

Compiles the C with the identified toolchain (cc1 -O1 -mips1 -G0 via maspsx).
src.c may hold one function or many (the repo consolidates matched functions
into multi-function translation units, in address order, per config/
matched.json). Either way, the whole file is compiled and its .text is
linked as one blob at the address of the TU's first (lowest-address)
function -- with every other function defined in that same TU excluded from
the external symbol defs, since the object itself now defines them. The
result is then sliced to the byte range belonging to <function_name> (per
its own address/size from docs/function-catalog.json and docs/function-
manifest.json) before comparing against the original bytes in baserom.bin.
Exit 0 = perfect match for that one function, non-zero otherwise -- this
holds for both single- and multi-function source files.

Pass --whole-tu to instead compare the ENTIRE compiled TU blob against the
matching baserom span in one shot (a stronger, file-level check); <function>
is still required as a positional argument but is not used to pick the
comparison range in that mode.

If a function's slice cannot be determined (its size/address can't be
resolved, or the compiled TU turns out shorter than the slice needs), this
exits nonzero with an explicit error -- it never silently falls back to a
whole-TU comparison and prints a verdict for a different question than the
one asked.
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CATALOG = ROOT / "docs/function-catalog.json"
MANIFEST = ROOT / "docs/function-manifest.json"
MATCHED = ROOT / "config/matched.json"
RODATA_MATCHES = ROOT / "config/rodata_matches.json"
BASEROM = ROOT / "baserom.bin"
VRAM_BASE = 0x80010000
TEXT_START = 0x1DA8


def run(cmd, **kw):
    return subprocess.run(cmd, check=True, **kw)


def load_rodata_entry(name: str) -> dict | None:
    if not RODATA_MATCHES.is_file():
        return None
    entries = json.loads(RODATA_MATCHES.read_text())
    return next((entry for entry in entries if entry["function"] == name), None)


def load_function(name: str, rodata_entry: dict | None = None) -> tuple[int, int]:
    catalog = json.loads(CATALOG.read_text())
    by_name = {e["name"]: e for e in catalog["functions"]}
    by_raw = {e["raw"]: e for e in catalog["functions"]}
    entry = by_name.get(name) or by_raw.get(name)
    if entry is None:
        sys.exit(f"unknown function: {name}")
    address = int(entry["address"], 16)
    manifest = json.loads(MANIFEST.read_text())
    size = next(
        (f["size"] for f in manifest["functions"] if int(f["address"], 16) == address),
        None,
    )
    if size is None:
        sys.exit(f"no size for {name} at {address:#x}")
    if rodata_entry and "text_size" in rodata_entry:
        size = int(rodata_entry["text_size"])
    return address, size


def tu_functions(src: str) -> list[dict]:
    """All functions config/matched.json records as living in `src`, each
    with its real address/size, sorted by address (lowest first). This is
    the ground truth for what one compile of `src` actually contains, used
    both to place .text correctly (at the lowest address, so every function
    in the TU lands at its true vram) and to exclude sibling functions from
    the external symbol defs (the object itself now defines them)."""
    if not MATCHED.is_file():
        sys.exit(f"cannot determine functions in {src}: {MATCHED} is missing")
    matched = json.loads(MATCHED.read_text())
    names = [e["function"] for e in matched if e.get("src") == src]
    if not names:
        # No exact path match -- e.g. `src` points at a scratch copy of a
        # repo file (same name, different directory), which is the normal
        # way to test a deliberately broken variant without touching the
        # repo. Fall back to a same-basename match, but only if it is
        # unambiguous.
        base = Path(src).name
        by_base = {Path(e["src"]).name for e in matched if Path(e["src"]).name == base}
        if len(by_base) == 1:
            names = [e["function"] for e in matched if Path(e["src"]).name == base]
    if not names:
        sys.exit(f"cannot determine functions in {src}: no entries with src=={src!r} in {MATCHED}")
    catalog = json.loads(CATALOG.read_text())
    by_name = {e["name"]: e for e in catalog["functions"]}
    by_raw = {e["raw"]: e for e in catalog["functions"]}
    manifest = json.loads(MANIFEST.read_text())
    size_by_addr = {int(f["address"], 16): f["size"] for f in manifest["functions"]}
    out = []
    for name in names:
        entry = by_name.get(name) or by_raw.get(name)
        if entry is None:
            sys.exit(f"cannot resolve address for TU member {name!r} (src {src}) in {CATALOG}")
        addr = int(entry["address"], 16)
        fn_size = size_by_addr.get(addr)
        if fn_size is None:
            sys.exit(f"cannot resolve size for TU member {name!r} at {addr:#x} in {MANIFEST}")
        out.append({"name": name, "address": addr, "size": fn_size})
    out.sort(key=lambda f: f["address"])
    return out


def symbol_defs(tmp: Path) -> Path:
    out = tmp / "syms.ld"
    lines = []
    for path in (
        ROOT / "config/symbol_addrs.txt",
        ROOT / "undefined_syms_auto.txt",
        ROOT / "undefined_funcs_auto.txt",
    ):
        if not path.is_file():
            continue
        for line in path.read_text().splitlines():
            line = line.split("//")[0].strip()
            if re.match(r"^[A-Za-z_][A-Za-z0-9_]*\s*=\s*0x[0-9A-Fa-f]+;?$", line):
                if not line.endswith(";"):
                    line += ";"
                lines.append(line)
    out.write_text("\n".join(lines) + "\n")
    return out


def section_bytes(elf: Path, section: str, out: Path) -> bytes:
    proc = subprocess.run(
        [
            "mipsel-linux-gnu-objcopy", "-O", "binary", "-j", section,
            str(elf), str(out),
        ],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    if proc.returncode != 0 or not out.is_file():
        return b""
    return out.read_bytes()


def find_unique_pre_text_rodata(blob: bytes, needle: bytes) -> int | None:
    if not needle:
        return None
    hits: list[int] = []
    pos = blob.find(needle)
    while pos != -1:
        if pos % 4 == 0:
            hits.append(pos)
        pos = blob.find(needle, pos + 1)
    if len(hits) == 1:
        return hits[0]
    return None


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("src")
    ap.add_argument("function")
    ap.add_argument("--compiler", default="2.7.2-psx")
    ap.add_argument("--opt", default="-O1")
    ap.add_argument("--quiet", action="store_true")
    ap.add_argument(
        "--auto-rodata",
        action="store_true",
        help="discover a unique PRE-TEXT .rodata placement for switch-table candidates",
    )
    ap.add_argument(
        "--asm-diff",
        action="store_true",
        help="# --asm-diff hook (apply_match_asmdiff.py): on mismatch, also print tools/asm_diff.py instruction diff",
    )
    ap.add_argument(
        "--whole-tu",
        action="store_true",
        help="compare the ENTIRE compiled translation unit against the matching "
             "baserom span in one shot, instead of slicing out one function",
    )
    args = ap.parse_args()

    rodata_entry = load_rodata_entry(args.function)
    address, size = load_function(args.function, rodata_entry)
    rom_start = address - VRAM_BASE
    original = BASEROM.read_bytes()[rom_start : rom_start + size]

    tu = tu_functions(args.src)
    tu_names = {f["name"] for f in tu}
    tu_addrs = {f["address"] for f in tu}
    if address not in tu_addrs:
        sys.exit(
            f"function {args.function} ({address:#x}) is not among the functions "
            f"config/matched.json records for {args.src} -- cannot determine its "
            f"slice within that TU's compiled .text"
        )
    base_address = tu[0]["address"]
    target_offset = address - base_address
    if target_offset < 0:
        sys.exit(
            f"internal error: {args.function} address {address:#x} precedes "
            f"TU base {base_address:#x} for {args.src}"
        )
    original_rodata = b""
    if rodata_entry:
        rodata_vram = int(rodata_entry["rodata_vram"], 16)
        rodata_size = int(rodata_entry["rodata_size"])
        rodata_start = rodata_vram - VRAM_BASE
        original_rodata = BASEROM.read_bytes()[rodata_start : rodata_start + rodata_size]

    auto_rodata_vram: int | None = None
    auto_rodata_size = 0
    auto_rodata_bytes = b""
    auto_rodata_generated = b""
    auto_rodata_unique = True
    gcc = ROOT / f"tools/gcc-{args.compiler}"
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = Path(tmpdir)
        cpp = subprocess.run(
            [str(gcc / "cpp"), "-Iinclude", args.src],
            capture_output=True, text=True, check=True, cwd=ROOT, timeout=60,
        )
        cc1 = subprocess.run(
            [str(gcc / "cc1"), args.opt, "-mips1", "-G0", "-quiet", "-o", "-", "-"],
            input=cpp.stdout, capture_output=True, text=True, check=True,
            timeout=60,
        )
        mas = subprocess.run(
            [sys.executable, str(ROOT / "tools/maspsx/maspsx.py"), "--expand-li", "--expand-div"],
            input=cc1.stdout, capture_output=True, text=True, check=True,
            timeout=60,
        )
        gas_s = tmp / "f.s"
        gas_s.write_text(mas.stdout)
        obj = tmp / "f.o"
        run([
            "mipsel-linux-gnu-as", "-EL", "-march=r3000", "-mabi=32",
            "-no-pad-sections", "-o", str(obj), str(gas_s),
        ])
        # GNU as 16-aligns .text; functions at addresses != 0 mod 16 would
        # shift and corrupt absolute j targets. Force 4-byte alignment.
        run([
            "mipsel-linux-gnu-objcopy", "--set-section-alignment", ".text=4",
            "--set-section-alignment", ".data=4",
            "--set-section-alignment", ".rodata=4", str(obj),
        ])
        # link the TU's .text at the vram of its first (lowest-address)
        # function, with all known EXTERNAL symbol addresses. Every function
        # this TU itself defines (not just the one requested) must come from
        # the object, not the defs file, or the object's own definition and
        # the defs-file assignment would name the same symbol twice.
        syms = symbol_defs(tmp)
        defs = syms.read_text().splitlines()
        defs = [d for d in defs if not any(d.startswith(f"{n} ") for n in tu_names)]
        syms.write_text("\n".join(defs) + "\n")

        def link_at(rodata_vram: int | None) -> Path:
            lds = tmp / ("f_auto.ld" if rodata_vram is not None else "f.ld")
            if rodata_vram is not None:
                lds.write_text(
                    "SECTIONS { "
                    ". = 0x%X; .text : { *(.text) } "
                    ". = 0x%X; .rodata : { *(.rodata) } "
                    "/DISCARD/ : { *(*) } }\n"
                    % (address, rodata_vram)
                )
            else:
                lds.write_text(
                    "SECTIONS { . = 0x%X; .text : { *(.text) } /DISCARD/ : { *(*) } }\n"
                    % address
                )
            elf = tmp / ("f_auto.elf" if rodata_vram is not None else "f.elf")
            run([
                "mipsel-linux-gnu-ld", "-EL", "-T", str(lds), "-T", str(syms),
                "-e", hex(address), "--no-check-sections", "-o", str(elf), str(obj),
            ])
            return elf

        if rodata_entry is None and args.auto_rodata:
            probe_elf = link_at(VRAM_BASE)
            probe_rodata = section_bytes(probe_elf, ".rodata", tmp / "probe.rodata.bin")
            auto_rodata_generated = probe_rodata
            if probe_rodata:
                pre_text = BASEROM.read_bytes()[:TEXT_START]
                hit = find_unique_pre_text_rodata(pre_text, probe_rodata)
                if hit is not None:
                    auto_rodata_vram = VRAM_BASE + hit
                    auto_rodata_size = len(probe_rodata)
                    auto_rodata_bytes = pre_text[hit : hit + len(probe_rodata)]
                else:
                    auto_rodata_unique = False

        # Place .text at the TU's own base address (its lowest-address
        # member), not the requested function's address: with the whole TU
        # compiled as one blob, only that placement puts every function --
        # including ones ahead of the requested one in the file -- at its
        # true vram, which is what makes intra-TU calls and the eventual
        # per-function slice below both come out byte-correct.
        lds = tmp / "f.ld"
        if rodata_entry:
            lds.write_text(
                "SECTIONS { "
                ". = 0x%X; .text : { *(.text) } "
                ". = 0x%X; .rodata : { *(.rodata) } "
                "/DISCARD/ : { *(*) } }\n"
                % (base_address, int(rodata_entry["rodata_vram"], 16))
            )
        else:
            if auto_rodata_vram is not None:
                lds.write_text(
                    "SECTIONS { "
                    ". = 0x%X; .text : { *(.text) } "
                    ". = 0x%X; .rodata : { *(.rodata) } "
                    "/DISCARD/ : { *(*) } }\n"
                    % (base_address, auto_rodata_vram)
                )
            elif args.auto_rodata and auto_rodata_generated:
                lds.write_text(
                    "SECTIONS { "
                    ". = 0x%X; .text : { *(.text) } "
                    ". = 0x%X; .rodata : { *(.rodata) } "
                    "/DISCARD/ : { *(*) } }\n"
                    % (base_address, VRAM_BASE)
                )
            else:
                lds.write_text(
                    "SECTIONS { . = 0x%X; .text : { *(.text) } /DISCARD/ : { *(*) } }\n"
                    % base_address
                )
        elf = tmp / "f.elf"
        run([
            "mipsel-linux-gnu-ld", "-EL", "-T", str(lds), "-T", str(syms),
            "-e", hex(base_address), "--no-check-sections", "-o", str(elf), str(obj),
        ])
        binout = tmp / "f.bin"
        run([
            "mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text",
            str(elf), str(binout),
        ])
        ours_full = binout.read_bytes()
        ours_rodata = b""
        if rodata_entry:
            rodata_out = tmp / "f.rodata.bin"
            ours_rodata = section_bytes(elf, ".rodata", rodata_out)
        elif auto_rodata_vram is not None:
            ours_rodata = section_bytes(elf, ".rodata", tmp / "f.rodata.bin")
            original_rodata = auto_rodata_bytes
        elif args.auto_rodata and auto_rodata_generated:
            ours_rodata = section_bytes(elf, ".rodata", tmp / "f.rodata.bin")

    if args.whole_tu:
        tu_original = BASEROM.read_bytes()[
            base_address - VRAM_BASE : base_address - VRAM_BASE + len(ours_full)
        ]
        if ours_full == tu_original:
            if not args.quiet:
                print(
                    f"WHOLE-TU MATCH {args.src}: {len(ours_full)}B from "
                    f"{tu[0]['name']} @ {base_address:#x}, {len(tu)} function(s)"
                )
            return 0
        if not args.quiet:
            n = min(len(ours_full), len(tu_original))
            diffs = [i for i in range(0, n, 4) if ours_full[i : i + 4] != tu_original[i : i + 4]]
            print(
                f"WHOLE-TU MISMATCH {args.src}: ours {len(ours_full)}B vs "
                f"original {len(tu_original)}B, {len(diffs)} differing word(s)"
            )
            for off in diffs[:10]:
                o = int.from_bytes(tu_original[off : off + 4], "little")
                u = int.from_bytes(ours_full[off : off + 4], "little")
                print(f"  +{off:#06x} vram {base_address + off:#x}: orig {o:08x} ours {u:08x}")
        return 1

    # Slice out just the requested function's bytes from the linked TU. Fail
    # loudly rather than silently comparing something other than what was
    # asked for -- a confidently wrong verdict is worse than no verdict.
    if target_offset + size > len(ours_full):
        sys.exit(
            f"cannot verify {args.function}: its slice [{target_offset:#x}:"
            f"{target_offset + size:#x}) exceeds the compiled TU's .text length "
            f"{len(ours_full):#x} bytes for {args.src} -- refusing to fall back "
            f"to a whole-TU comparison"
        )
    ours = ours_full[target_offset : target_offset + size]

    if ours == original and ours_rodata == original_rodata:
        if not args.quiet:
            extra = f" + {len(original_rodata)}B rodata" if rodata_entry else ""
            if auto_rodata_vram is not None:
                extra = f" + {len(original_rodata)}B rodata @ {auto_rodata_vram:#x}"
            print(f"MATCH {args.function} ({size} bytes{extra})")
            if auto_rodata_vram is not None:
                print(
                    "RODATA "
                    + json.dumps(
                        {
                            "function": args.function,
                            "src": args.src,
                            "opt": args.opt,
                            "compiler": args.compiler,
                            "text_size": size,
                            "rodata_vram": f"0x{auto_rodata_vram:08x}",
                            "rodata_size": len(original_rodata),
                        },
                        sort_keys=True,
                    )
                )
        return 0
    if not args.quiet:
        print(f"MISMATCH {args.function}: ours {len(ours)}B vs original {size}B")
        n = min(len(ours), len(original))
        diffs = [i for i in range(0, n, 4) if ours[i : i + 4] != original[i : i + 4]]
        print(f"differing words: {len(diffs)} of {size // 4}")
        for off in diffs[:10]:
            o = int.from_bytes(original[off : off + 4], "little")
            u = int.from_bytes(ours[off : off + 4], "little") if off + 4 <= len(ours) else 0
            print(f"  +{off:#06x} vram {address + off:#x}: orig {o:08x} ours {u:08x}")
        if args.auto_rodata and rodata_entry is None and auto_rodata_vram is None:
            if auto_rodata_generated:
                why = "no unique" if not auto_rodata_unique else "no"
                print(
                    f"auto-rodata: {why} PRE-TEXT .rodata match discovered "
                    f"for {len(auto_rodata_generated)} generated byte(s)"
                )
            else:
                print("auto-rodata: candidate did not emit .rodata")
        if (rodata_entry or auto_rodata_vram is not None) and ours_rodata != original_rodata:
            n = min(len(ours_rodata), len(original_rodata))
            rdiffs = [
                i for i in range(0, n, 4)
                if ours_rodata[i : i + 4] != original_rodata[i : i + 4]
            ]
            print(
                "rodata mismatch:"
                f" ours {len(ours_rodata)}B vs original {len(original_rodata)}B,"
                f" differing words {len(rdiffs)}"
            )
            base = int(rodata_entry["rodata_vram"], 16) if rodata_entry else auto_rodata_vram
            for off in rdiffs[:10]:
                o = int.from_bytes(original_rodata[off : off + 4], "little")
                u = (
                    int.from_bytes(ours_rodata[off : off + 4], "little")
                    if off + 4 <= len(ours_rodata)
                    else 0
                )
                print(f"  +{off:#06x} vram {base + off:#x}: orig {o:08x} ours {u:08x}")
    if getattr(args, "asm_diff", False):  # --asm-diff hook (apply_match_asmdiff.py)
        try:
            import subprocess as _sp
            print("\n--- asm_diff (instruction-level) ---")
            sys.stdout.flush()  # else buffered prints land after the subprocess output
            _sp.run(
                [sys.executable, str(ROOT / "tools/asm_diff.py"), args.src,
                 args.function, "--compiler", args.compiler, "--opt", args.opt],
                check=False,
            )
        except Exception:
            pass
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
