#!/usr/bin/env python3
"""Prepare the tree for building.

Extracts SLPS_009.12 from your own disc image, checks it, and fetches the
toolchain. Run this once before the first build.

    ./configure.py /path/to/kaze.cue
    ./configure.py --deps
"""
import argparse
import hashlib
import platform
import re
import shutil
import subprocess
import sys
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent
EXE_NAME = "SLPS_009.12"
EXE_SHA1 = "f8f44c2ddaf4969f4132fb17d1abab06a75c2def"
EXE_SIZE = 622592
HEADER_SIZE = 2048

CANDIDATE_LAYOUTS = [(2048, 0), (2352, 24), (2352, 16), (2336, 8)]


def detect_layout(data: bytes):
    """Return (sector_size, data_offset) by finding CD001 in the PVD at LBA 16."""
    for size, off in CANDIDATE_LAYOUTS:
        pos = 16 * size + off
        if pos + 6 > len(data):
            continue
        if data[pos] == 1 and data[pos + 1:pos + 6] == b"CD001":
            return size, off
    raise ValueError("not an ISO9660 image: no primary volume descriptor at LBA 16")


def read_sector(data: bytes, lba: int, layout=None) -> bytes:
    size, off = layout or detect_layout(data)
    start = lba * size + off
    return data[start:start + 2048]


def _records(block: bytes):
    """Walk directory records.

    ISO9660 records never span a sector boundary, so each 2048-byte sector
    is zero-padded when the next record will not fit. A zero length byte
    means "advance to the next sector", not "end of directory".
    """
    i = 0
    while i < len(block):
        length = block[i]
        if length == 0:
            i = (i // 2048 + 1) * 2048
            continue
        yield block[i:i + length]
        i += length


def find_file(data: bytes, name: str):
    """Return (lba, size) for a file in the root directory."""
    layout = detect_layout(data)
    pvd = read_sector(data, 16, layout)
    root = pvd[156:190]
    root_lba = int.from_bytes(root[2:6], "little")
    root_len = int.from_bytes(root[10:14], "little")

    block = b""
    for n in range((root_len + 2047) // 2048):
        block += read_sector(data, root_lba + n, layout)

    want = name.upper()
    for rec in _records(block):
        namelen = rec[32]
        entry = rec[33:33 + namelen].decode("ascii", "replace").upper()
        entry = entry.split(";")[0]
        if entry == want:
            return int.from_bytes(rec[2:6], "little"), int.from_bytes(rec[10:14], "little")
    raise FileNotFoundError(f"{name} not found in the disc image root directory")


def extract(data: bytes) -> bytes:
    layout = detect_layout(data)
    lba, size = find_file(data, EXE_NAME)
    out = b""
    for n in range((size + 2047) // 2048):
        out += read_sector(data, lba + n, layout)
    return out[:size]


def _human(n: int) -> str:
    if n >= 1 << 20:
        return f"{n / (1 << 20):.1f} MB"
    if n >= 1 << 10:
        return f"{n / (1 << 10):.1f} KB"
    return f"{n} bytes"


def resolve_input(path: Path):
    if path.suffix.lower() == ".cue":
        text = path.read_text(errors="replace")
        m = re.search(r'FILE\s+"([^"]+)"', text)
        if not m:
            sys.exit(f"could not find a FILE line in {path}")
        binp = path.parent / m.group(1)
        if not binp.is_file():
            sys.exit(f"{path} points at {binp.name}, which is missing")
        print(f"[cue] using {binp.name}")
        return binp.read_bytes(), binp.name
    return path.read_bytes(), path.name


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("disc", nargs="?", help="disc image (.cue/.bin/.iso) or an extracted SLPS_009.12")
    ap.add_argument("--deps", action="store_true", help="fetch the toolchain")
    args = ap.parse_args()

    if args.disc:
        path = Path(args.disc).expanduser()
        if not path.is_file():
            sys.exit(f"no such file: {path}")
        raw, display = resolve_input(path)

        if len(raw) == EXE_SIZE:
            # Right size for the executable, so treat it as one and let the sha1
            # gate below diagnose it. Falling through to the ISO parser here would
            # report a confusing "not an ISO9660 image" error for what is really a
            # bad or wrong-region dump.
            print(f"[exe] {display} is the right size for {EXE_NAME}")
            exe = raw
        else:
            print(f"[iso] reading {display} ({_human(len(raw))})")
            try:
                size, off = detect_layout(raw)
                print(f"[iso] sector size {size}, data offset {off}")
                exe = extract(raw)
            except (ValueError, FileNotFoundError) as exc:
                sys.exit(f"[!!] cannot read {display}: {exc}\n"
                         f"     Expected a PlayStation disc image (.cue, .bin or .iso)\n"
                         f"     or an extracted {EXE_NAME}.")
            print(f"[iso] found {EXE_NAME} ({len(exe)} bytes)")

        got = hashlib.sha1(exe).hexdigest()
        if got != EXE_SHA1:
            sys.exit(f"[!!] sha1 mismatch\n     expected {EXE_SHA1}\n     got      {got}\n"
                     f"     This build targets the Japanese release. A different region or a\n"
                     f"     bad dump will not rebuild byte-identically.")
        print(f"[ok] sha1 {got}")

        (ROOT / "baserom").mkdir(exist_ok=True)
        (ROOT / "baserom" / EXE_NAME).write_bytes(exe)
        (ROOT / "baserom.bin").write_bytes(exe[HEADER_SIZE:])
        print(f"[out] baserom/{EXE_NAME}")
        print(f"[out] baserom.bin ({len(exe) - HEADER_SIZE} bytes)")

    if args.deps:
        fetch_deps()

    if not args.disc and not args.deps:
        ap.print_help()


def fetch_deps() -> None:
    if sys.version_info < (3, 9):
        sys.exit(f"[!!] Python 3.9 or newer is required (found "
                 f"{sys.version.split()[0]}).\n"
                 f"     splat and spimdisasm do not support older versions.")

    if platform.machine() not in ("x86_64", "amd64"):
        print(f"[!!] the PSX compilers are 32-bit x86 binaries; on "
              f"{platform.machine()} you will need an emulation layer such as "
              f"qemu-user to run them.")

    missing = [t for t in ("mipsel-linux-gnu-as", "mipsel-linux-gnu-ld",
                           "mipsel-linux-gnu-objcopy", "mipsel-linux-gnu-objdump")
               if shutil.which(t) is None]
    if missing:
        print("[!!] missing binutils: " + ", ".join(missing))
        print("     Debian/Ubuntu: sudo apt install binutils-mipsel-linux-gnu")
        print("[..] toolchain incomplete")
        sys.exit(1)

    venv = ROOT / ".venv"
    if not venv.is_dir():
        try:
            import ensurepip  # noqa: F401
        except ModuleNotFoundError:
            sys.exit("[!!] Python venv support is missing.\n"
                     "     Debian/Ubuntu: sudo apt install python3-venv")
        print("[venv] creating .venv")
        subprocess.check_call([sys.executable, "-m", "venv", str(venv)])
    print("[venv] installing splat and spimdisasm")
    subprocess.check_call([str(venv / "bin/pip"), "install", "-q", "-r", str(ROOT / "requirements.txt")])

    for name, url in [
        ("gcc-2.7.2-psx", "https://github.com/decompals/old-gcc/releases/download/0.17/gcc-2.7.2-psx.tar.gz"),
        ("gcc-2.6.3-psx", "https://github.com/decompals/old-gcc/releases/download/0.17/gcc-2.6.3-psx.tar.gz"),
    ]:
        d = ROOT / "tools" / name
        if d.is_dir():
            print(f"[cc] {name} already present")
            continue
        d.mkdir(parents=True)
        print(f"[cc] fetching {name}")
        tgz = d / "t.tar.gz"
        urllib.request.urlretrieve(url, tgz)
        subprocess.check_call(["tar", "xzf", str(tgz), "-C", str(d)])
        tgz.unlink()

    missing = [p for p in ("tools/maspsx/maspsx.py", "tools/asm-differ/diff.py")
               if not (ROOT / p).is_file()]
    if missing:
        print("[!!] empty submodule(s): " + ", ".join(m.rsplit("/", 1)[0] for m in missing))
        print("     Run: git submodule update --init")
        print("[..] toolchain incomplete")
        sys.exit(1)
    else:
        apply_maspsx_patch()
        print("[ok] toolchain ready. Next: make")


def apply_maspsx_patch() -> None:
    """Apply the game-specific li-expansion patch to the maspsx submodule.

    This game's original assembler emits addiu, not ori, for positive
    immediates below 0x8000; without this patch the rebuilt payload is
    not byte-identical (see docs/compiler-notes.md). Idempotent: checks
    with `git apply --check` first, so a second run reports the patch is
    already applied instead of erroring.
    """
    maspsx_dir = ROOT / "tools" / "maspsx"
    patch = ROOT / "tools" / "maspsx-li-expansion.patch"

    forward = subprocess.run(["git", "apply", "--check", str(patch)],
                              cwd=maspsx_dir, capture_output=True, text=True)
    if forward.returncode == 0:
        subprocess.check_call(["git", "apply", str(patch)], cwd=maspsx_dir)
        print("[patch] maspsx li expansion applied")
        return

    reverse = subprocess.run(["git", "apply", "--check", "--reverse", str(patch)],
                              cwd=maspsx_dir, capture_output=True, text=True)
    if reverse.returncode == 0:
        print("[patch] maspsx already patched")
        return

    sys.exit(f"[!!] {patch.relative_to(ROOT)} does not apply to tools/maspsx.\n"
             f"     {forward.stderr.strip()}\n"
             f"     The rebuild will not be byte-identical without this patch;\n"
             f"     see docs/compiler-notes.md.")


if __name__ == "__main__":
    main()
