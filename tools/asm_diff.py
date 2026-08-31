#!/usr/bin/env python3
"""Instruction-level diff for a candidate C function vs the original bytes.

Where tools/match.py answers "does it match?" with a raw hex word summary,
this answers *how* it differs at MIPS instruction granularity: it prints
`lw $t0` vs `lw $t1`, a swapped delay slot, or a wrong branch target instead
of `orig 8fa80010 ours 8fa90010`.

It reuses tools/match.py's exact compile + link pipeline, so the disassembly
reflects the *real relocated bytes* the linker produces (register allocation,
delay-slot scheduling, %hi/%lo, jump targets) rather than pre-link source asm.

Usage:
  tools/asm_diff.py <src.c> <function> [--compiler 2.7.2-psx] [--opt -O2]
                    [--context N] [--all] [--color]

Exit status mirrors match.py (0 = byte-identical, 1 = differs), so it can be a
drop-in "why did it miss" companion. It never mutates tracked state.
"""

from __future__ import annotations

import argparse
import importlib.util
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

# Reuse match.py's helpers/constants so the two tools can never drift on
# addresses, sizes, symbol tables, or rodata handling.
_spec = importlib.util.spec_from_file_location("kaze_match", ROOT / "tools/match.py")
match = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(match)  # type: ignore[union-attr]

OBJDUMP = "mipsel-linux-gnu-objdump"
INSN_RE = re.compile(r"^\s*([0-9a-fA-F]+):\s+([0-9a-fA-F]{8})\s+(.*?)\s*$")

# ANSI (only emitted with --color / a tty)
RED = "\033[31m"
GRN = "\033[32m"
DIM = "\033[2m"
RST = "\033[0m"


def fix_opt_argv(argv: list[str]) -> list[str]:
    """Let `--opt -O2` work as well as `--opt=-O2` (argparse otherwise reads the
    -O2 as a flag). Callers pass either form; the hook in match.py passes space."""
    out, i = [], 0
    while i < len(argv):
        if argv[i] == "--opt" and i + 1 < len(argv) and argv[i + 1].startswith("-O"):
            out.append(f"--opt={argv[i + 1]}")
            i += 2
        else:
            out.append(argv[i])
            i += 1
    return out


def _run(cmd, **kw):
    return subprocess.run(cmd, check=True, **kw)


def compile_function_bytes(src: str, function: str, compiler: str, opt: str) -> bytes:
    """Compile+link `function` from `src` exactly like match.py and return the
    linked .text bytes (fully relocated)."""
    rodata_entry = match.load_rodata_entry(function)
    address, size = match.load_function(function, rodata_entry)
    gcc = ROOT / f"tools/gcc-{compiler}"
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = Path(tmpdir)
        cpp = subprocess.run(
            [str(gcc / "cpp"), "-Iinclude", src],
            capture_output=True, text=True, check=True, cwd=ROOT,
        )
        cc1 = subprocess.run(
            [str(gcc / "cc1"), opt, "-mips1", "-G0", "-quiet", "-o", "-", "-"],
            input=cpp.stdout, capture_output=True, text=True, check=True,
        )
        mas = subprocess.run(
            [sys.executable, str(ROOT / "tools/maspsx/maspsx.py"), "--expand-li", "--expand-div"],
            input=cc1.stdout, capture_output=True, text=True, check=True, timeout=60,
        )
        gas_s = tmp / "f.s"
        gas_s.write_text(mas.stdout)
        obj = tmp / "f.o"
        _run([
            "mipsel-linux-gnu-as", "-EL", "-march=r3000", "-mabi=32",
            "-no-pad-sections", "-o", str(obj), str(gas_s),
        ])
        _run([
            "mipsel-linux-gnu-objcopy", "--set-section-alignment", ".text=4",
            "--set-section-alignment", ".data=4",
            "--set-section-alignment", ".rodata=4", str(obj),
        ])
        syms = match.symbol_defs(tmp)
        defs = [d for d in syms.read_text().splitlines() if not d.startswith(f"{function} ")]
        syms.write_text("\n".join(defs) + "\n")
        lds = tmp / "f.ld"
        if rodata_entry:
            lds.write_text(
                "SECTIONS { . = 0x%X; .text : { *(.text) } "
                ". = 0x%X; .rodata : { *(.rodata) } /DISCARD/ : { *(*) } }\n"
                % (address, int(rodata_entry["rodata_vram"], 16))
            )
        else:
            lds.write_text(
                "SECTIONS { . = 0x%X; .text : { *(.text) } /DISCARD/ : { *(*) } }\n" % address
            )
        elf = tmp / "f.elf"
        _run([
            "mipsel-linux-gnu-ld", "-EL", "-T", str(lds), "-T", str(syms),
            "-e", hex(address), "--no-check-sections", "-o", str(elf), str(obj),
        ])
        binout = tmp / "f.bin"
        _run(["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text", str(elf), str(binout)])
        return binout.read_bytes()


def disassemble(blob: bytes, vram: int) -> list[tuple[int, str, str]]:
    """Return [(offset, word_hex, insn_text)] for a raw MIPS-I blob at `vram`.

    Both sides go through the identical raw-binary path so formatting matches
    and only real instruction differences survive the diff. Addresses are set
    to true vram, so branch/jump targets compare as absolute (already relocated
    on both sides)."""
    with tempfile.TemporaryDirectory() as tmpdir:
        binp = Path(tmpdir) / "d.bin"
        binp.write_bytes(blob)
        out = subprocess.run(
            [OBJDUMP, "-D", "-b", "binary", "-m", "mips", "-EL",
             f"--adjust-vma={vram:#x}", str(binp)],
            capture_output=True, text=True, check=True,
        ).stdout
    insns: list[tuple[int, str, str]] = []
    for line in out.splitlines():
        m = INSN_RE.match(line)
        if not m:
            continue
        addr = int(m.group(1), 16)
        text = re.sub(r"\s+", " ", m.group(3)).strip()
        # drop objdump's trailing "# 0x..." address hints (noise for diffing)
        text = re.sub(r"\s*#.*$", "", text)
        insns.append((addr - vram, m.group(2).lower(), text))
    return insns


def _first_token_diff(a: str, b: str) -> str:
    """Label the class of an instruction difference for quick scanning."""
    ma, mb = a.split(" ", 1)[0], b.split(" ", 1)[0]
    if ma != mb:
        return "opcode"
    ta = a.split(" ", 1)[1] if " " in a else ""
    tb = b.split(" ", 1)[1] if " " in b else ""
    ra = set(re.findall(r"\$?\b([svatkgf][0-9p]|ra|zero|at|sp|fp|gp|hi|lo)\b", ta))
    rb = set(re.findall(r"\$?\b([svatkgf][0-9p]|ra|zero|at|sp|fp|gp|hi|lo)\b", tb))
    if ra != rb:
        return "register"
    if re.findall(r"0x[0-9a-f]+|-?\d+", ta) != re.findall(r"0x[0-9a-f]+|-?\d+", tb):
        return "immediate/target"
    return "operand"


def render(orig, ours, address, size, context, show_all, color) -> tuple[str, int]:
    n = max(len(orig), len(ours))
    diff_idx = [
        i for i in range(n)
        if (orig[i][1:] if i < len(orig) else None) != (ours[i][1:] if i < len(ours) else None)
    ]
    lines: list[str] = []

    def c(s, col):
        return f"{col}{s}{RST}" if color else s

    if not diff_idx:
        lines.append(c(f"MATCH {size}B / {len(orig)} instructions, byte identical", GRN))
        return "\n".join(lines), 0

    lines.append(c(
        f"MISMATCH {address:#x} ({size}B): "
        f"{len(diff_idx)}/{len(orig)} instructions differ", RED))
    if len(ours) != len(orig):
        lines.append(c(
            f"  size drift: ours {len(ours)} vs original {len(orig)} instructions", RED))
    lines.append("")

    show = set()
    for i in diff_idx:
        for j in range(i - context, i + context + 1):
            if 0 <= j < n:
                show.add(j)
    indices = sorted(show) if not show_all else range(n)

    prev = None
    for i in indices:
        if prev is not None and i != prev + 1:
            lines.append(c("        ...", DIM))
        prev = i
        o = orig[i] if i < len(orig) else (i * 4, "--------", "(end)")
        u = ours[i] if i < len(ours) else (i * 4, "--------", "(end)")
        off = o[0]
        if o[1:] == u[1:]:
            lines.append(c(f"  {off:#06x} {o[2]}", DIM))
        else:
            klass = _first_token_diff(o[2], u[2]) if "(end)" not in (o[2], u[2]) else "size"
            lines.append(
                f"{c('✗', RED)} {off:#06x} [{klass:>16}]"
                f"  orig: {c(o[2], GRN):<34}  ours: {c(u[2], RED)}"
            )
    return "\n".join(lines), 1


def main() -> int:
    sys.argv[1:] = fix_opt_argv(sys.argv[1:])
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("src")
    ap.add_argument("function")
    ap.add_argument("--compiler", default="2.7.2-psx")
    ap.add_argument("--opt", default="-O2")
    ap.add_argument("--context", type=int, default=2,
                    help="instructions of unchanged context around each diff")
    ap.add_argument("--all", action="store_true", help="print the whole function")
    ap.add_argument("--color", action="store_true", help="force ANSI color")
    args = ap.parse_args()

    color = args.color or sys.stdout.isatty()
    rodata_entry = match.load_rodata_entry(args.function)
    address, size = match.load_function(args.function, rodata_entry)
    rom_start = address - match.VRAM_BASE
    original = match.BASEROM.read_bytes()[rom_start : rom_start + size]

    try:
        ours = compile_function_bytes(args.src, args.function, args.compiler, args.opt)
    except subprocess.CalledProcessError as exc:
        stderr = (exc.stderr or b"" if isinstance(exc.stderr, bytes) else exc.stderr) or ""
        if isinstance(stderr, bytes):
            stderr = stderr.decode(errors="replace")
        print(f"COMPILE FAILED for {args.function}:\n{stderr.strip()[-2000:]}", file=sys.stderr)
        return 2

    orig_insns = disassemble(original, address)
    our_insns = disassemble(ours, address)
    text, rc = render(orig_insns, our_insns, address, size, args.context, args.all, color)
    print(text)
    return rc


if __name__ == "__main__":
    raise SystemExit(main())
