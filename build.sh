#!/usr/bin/env bash
# Ledger-driven rebuild of SLPS_009.12 with byte-identity verification.
# Matched functions in config/matched.json compile from src/*.c with the
# identified original toolchain; everything else assembles from splat asm.
set -euo pipefail
cd "$(dirname "$0")"

AS=mipsel-linux-gnu-as
LD=mipsel-linux-gnu-ld
OBJCOPY=mipsel-linux-gnu-objcopy
ASFLAGS="-EL -march=r3000 -mabi=32 -no-pad-sections -Iinclude"
CC1FLAGS="-O1 -mips1 -G0 -quiet"

if [ "${1:-}" = "--resplit" ]; then
  echo "[gen] regenerating config + split..."
  python3 tools/gen_config.py
  rm -rf asm kaze.ld kaze.ld.pristine undefined_syms_auto.txt undefined_funcs_auto.txt
  .venv/bin/python -m splat split config/kaze.yaml >/dev/null
  cp kaze.ld kaze.ld.pristine
elif [ -f kaze.ld.pristine ]; then
  # apply_rodata_links.py patches kaze.ld in place; restore the pristine
  # splat output so plain rebuilds are idempotent
  cp kaze.ld.pristine kaze.ld
fi

mkdir -p build/asm build/asm/bin build/assets build/src

echo "[1/5] assembling asm chunks..."
for s in asm/*.s asm/bin/*.s; do
  [ -e "$s" ] || continue
  o="build/${s}.o"
  $AS $ASFLAGS -o "$o" "$s"
  $OBJCOPY --set-section-alignment .text=4 --set-section-alignment .data=4 \
    --set-section-alignment .rodata=4 --set-section-alignment .bss=4 "$o"
done

echo "[2/5] compiling matched C..."
python3 - << 'PYEOF' > build/cflags.txt
import json
from pathlib import Path
ledger = json.loads(Path("config/matched.json").read_text()) if Path("config/matched.json").is_file() else []
for e in ledger:
    print(Path(e["src"]).stem, e.get("opt", "-O1"), e.get("compiler", "2.7.2-psx"))
PYEOF
for c in src/*.c; do
  [ -e "$c" ] || continue
  stem="$(basename "$c" .c)"
  opt="$(awk -v s="$stem" '$1==s {print $2; exit}' build/cflags.txt)"
  opt="${opt:--O1}"
  compiler="$(awk -v s="$stem" '$1==s {print $3; exit}' build/cflags.txt)"
  compiler="${compiler:-2.7.2-psx}"
  gcc_dir="tools/gcc-${compiler}"
  o="build/src/$(basename "$c").o"
  "$gcc_dir/cpp" -Iinclude "$c" \
    | "$gcc_dir/cc1" $opt -mips1 -G0 -quiet -o - - \
    | python3 tools/maspsx/maspsx.py --expand-li --expand-div \
    | $AS $ASFLAGS -o "$o" -
  $OBJCOPY --set-section-alignment .text=4 --set-section-alignment .data=4 \
    --set-section-alignment .rodata=4 --set-section-alignment .bss=4 "$o"
done

echo "[3/5] wrapping binary blobs..."
for bin in assets/*.bin; do
  name="$(basename "$bin" .bin)"
  $LD -r -b binary -o "build/assets/${name}.bin.tmp.o" "$bin"
  $OBJCOPY --rename-section .data=.data \
    --set-section-flags .data=alloc,contents,load,data \
    "build/assets/${name}.bin.tmp.o" "build/assets/${name}.bin.o"
done
python3 tools/apply_rodata_links.py

echo "[4/5] linking + extracting..."
$LD -EL -T kaze.ld -T undefined_syms_auto.txt -T undefined_funcs_auto.txt \
  --no-check-sections -Map build/kaze.map -o build/kaze.elf
$OBJCOPY -O binary build/kaze.elf build/payload.bin
truncate -s 620544 build/payload.bin

echo "[5/5] verifying..."
if cmp -s build/payload.bin baserom.bin; then
  echo "PAYLOAD OK (byte-identical)"
else
  echo "PAYLOAD MISMATCH:"
  python3 - << 'EOF'
a = open('build/payload.bin','rb').read()
b = open('baserom.bin','rb').read()
diffs = [i for i in range(min(len(a),len(b))) if a[i] != b[i]]
print('sizes:', len(a), len(b), 'diff bytes:', len(diffs))
if diffs:
    print('first diffs at vram:', [hex(0x80010000 + d) for d in diffs[:8]])
EOF
  exit 1
fi

dd if=baserom/SLPS_009.12 of=build/header.bin bs=2048 count=1 2>/dev/null
cat build/header.bin build/payload.bin > build/SLPS_009.12
cmp -s build/SLPS_009.12 baserom/SLPS_009.12 \
  && echo "EXE OK (byte-identical): $(sha1sum build/SLPS_009.12 | cut -d' ' -f1)"
