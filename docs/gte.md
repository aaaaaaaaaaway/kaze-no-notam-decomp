# GTE decompilation skill (Kaze no Notam)

Domain reference for matching the **GTE / COP2** functions: 38 functions in
this codebase that touch the GTE coprocessor directly. This is
the analog of Chris Lewis's `gfxdis.f3dex2` skill: the category where the
default decompilation tool (m2c) models the coprocessor poorly, so it needs a
domain map instead.

**Why the default pipeline fails here:** m2c cannot represent `cop2` command ops
or `lwc2/swc2/mfc2/mtc2/ctc2/cfc2` data movement, so its output for a GTE
function is misleading. Do **not** base GTE C on m2c output. Base it on the
matched sibling whose C is closest (`tools/similar_functions.py <fn>`; the GTE
rasterizer family is large and highly self-similar) and on the recipe below.

---

## 1. Command ops → macros (`include/gte.h`)

GNU `as` has no named mnemonics for GTE *command* ops, so they are emitted as the
generic `cop2 imm25` encoding (`0x4A000000 | imm`) via macros. Include `include/gte.h`
and call the macro; it is byte-identical to the original.

| Macro | PsyQ | Purpose |
|---|---|---|
| `gte_rtps()` | RTPS | perspective-transform 1 vertex |
| `gte_rtpt()` | RTPT | perspective-transform 3 vertices |
| `gte_ncds()` / `gte_ncdt()` | NCDS/NCDT | normal-color-depth-cue 1 / 3 verts |
| `gte_ncs()` / `gte_nct()` | NCS/NCT | normal-color 1 / 3 verts |
| `gte_dpcs()` `gte_dpct()` `gte_dpcl()` | DPCS/DPCT/DPCL | depth cueing |
| `gte_intpl()` | INTPL | interpolate vector & far color |
| `gte_sqr12()` / `gte_sqr0()` | SQR | square of IR (1.19.12 / 1.31.0) |
| `gte_rtv0..2()`, `gte_rtir12()`, `…tr()`, `…bk()` | MVMVA family | matrix×vector (+tr/+bk), rot matrix |
| `gte_llv*`, `gte_ll()`, `gte_llvir` | MVMVA | ×light matrix |
| `gte_lcv*`, `gte_lc()`, `gte_lev*bk` | MVMVA | ×color matrix |

There is **no NCLIP / AVSZ3 / AVSZ4 macro** in this header; those appear in the
matched TMD helpers as raw `cop2` islands; copy them verbatim from a matched
sibling (they encode as `cop2 0x1400006`, `cop2 0x158002d`, `cop2 0x168002e`
respectively). Add a macro only if you first confirm the exact `imm25` against a
matched function.

## 2. Data movement: raw mnemonics (write inline directly)

`lwc2/swc2` (load/store GTE data reg from memory), `mfc2/mtc2` (move data reg
↔ CPU reg), `cfc2/ctc2` (move control reg ↔ CPU reg) are normal assembler
mnemonics. Keep them as small inline-asm islands with correct clobbers/`memory`.

### GTE data registers (`lwc2 $n` / `swc2 $n` / `mfc2 rt,$n`)
```
 0 VXY0   1 VZ0    2 VXY1   3 VZ1    4 VXY2   5 VZ2     ← input vertices
 6 RGBC   7 OTZ
 8 IR0    9 IR1   10 IR2   11 IR3
12 SXY0  13 SXY1  14 SXY2  15 SXYP   ← projected screen XY FIFO (RTPS→SXY2)
16 SZ0   17 SZ1   18 SZ2   19 SZ3    ← Z FIFO (depth; OTZ = SZ3>>… )
20 RGB0  21 RGB1  22 RGB2  23 RES1
24 MAC0  25 MAC1  26 MAC2  27 MAC3
28 IRGB  29 ORGB  30 LZCS  31 LZCR
```
### GTE control registers (`ctc2 rt,$n` / `cfc2 rt,$n`)
```
 0-4 rotation matrix R11..R33   5-7 TR (translation X/Y/Z)
 8-12 light matrix L           13-15 BK (background color)
16-20 light-color matrix LC    21-23 FC (far color)
24 OFX  25 OFY  26 H (proj. plane dist)  27 DQA  28 DQB  29 ZSF3  30 ZSF4
31 FLAG (calc error flags; read via cfc2)
```
This is why `kaze_set_geom_screen` is `ctc2 h, $26` (H) and the geometry-offset
setter writes `$24/$25` (OFX/OFY).

## 3. The winning recipe

The whole TMD/model rasterizer family matched by the *same* skeleton; transfer
it, don't re-derive:

1. **Loop and control flow stay in C.** Only GTE data-movement and command
   latency go into inline asm. If you find yourself putting a branch or a loop
   in asm, stop: that is a skeleton problem, not a GTE problem.
2. **Pin the register roles the family uses:** context pointer → `$a1`,
   mask/tag → `$a2`/`$a3`. Matching a sibling's register discipline is usually
   what closes the last few differing words.
3. **Scoped islands, not one big block.** Bound inline asm to four kinds of
   island: (a) vertex loads `lwc2 $0..$5`, (b) the command + its latency
   (`nop`s or interleaved C between the command and the first `mfc2/swc2` that
   reads its result), (c) packet/OT writes `swc2`/`mfc2`, (d) `DR_MODE`/tpage
   setup for textured/semitrans variants.
4. **Respect GTE latency.** RTPS/RTPT results are not ready for a couple of
   cycles; the originals hide unrelated work (or `nop`s) between the command and
   the first read. Copy the exact spacing from the matched sibling.
5. **Use `include/gte.h` for commands, raw mnemonics for data movement**, with precise
   clobbers (`"$12"`, `"memory"`).

## 4. Worked examples (verbatim matched C)

### Minimal projection: `kaze_gte_project_vertex` (100B, matched)
Load VXY0/VZ0, two latency `nop`s, `RTPS`, store SXY2, read depth from SZ3
(`$19`) via `mfc2`, shift to OTZ:
```c
#include "gte.h"
long kaze_gte_project_vertex(long *sxy, short *v) {
    long *scratch = (long *)0x1F800200;
    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(v));   /* VXY0 */
    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(v));   /* VZ0  */
    __asm__ volatile("nop");                        /* load latency */
    __asm__ volatile("nop");
    gte_rtps();                                     /* project */
    __asm__ volatile("swc2 $14, 0(%0)" :: "r"(sxy) : "memory");  /* SXY2 */
    __asm__ volatile("mfc2 $12, $19\n\tnop\n\tsra $12, $12, 2\n\tsw $12, 0(%0)"
                     :: "r"(scratch) : "$12", "memory");         /* SZ3 → OTZ */
    return *scratch;
}
```

### One-triangle packet: `kaze_tmd_emit_poly_f3_7f28` (matched)
Six `lwc2` loads (V0/V1/V2), `RTPT`, read the winding flag via `cfc2 $31`
(FLAG), and only emit the primitive + advance the OT when it is front-facing.
Note the control (`if (... >= 0)`) is C; the islands are just load / command /
store:
```c
u32 *r0 = *(u32 **)(ctx + 0xc);
__asm__ volatile("lwc2 $0, 0(%0)" :: "r"(v0));   /* … $1..$5 for v0.z,v1,v2 … */
r0[1] = *(u32 *)(v0 + 8);
gte_rtpt();
*((u8 *)r0 + 3) = 4;  *((u8 *)r0 + 7) = 0x20;
__asm__ volatile("cfc2 $12, $31\n\tnop\n\tsw $12, 0(%0)" :: "r"(ctx + 0xac) : "$12","memory");
if (*(int *)(ctx + 0xac) >= 0) {                 /* front-facing → keep */
    __asm__ volatile("swc2 $12, 8(%0)" :: "r"(r0) : "memory");   /* SXY0 */
    /* … swc2 $13/$14 (SXY1/SXY2), link into OT, advance ctx+0xc … */
}
```
Full sources: `kaze_gte_project_vertex` in `src/object_render.c`, `kaze_tmd_emit_poly_f3_7f28` in `src/gs_graphics.c`.

## 5. Checklist for a new GTE function

- [ ] `tools/similar_functions.py <fn> --with-c` → start from the closest matched sibling, not m2c.
- [ ] Identify command ops → map to `include/gte.h` macros; keep data movement as raw `lwc2/swc2/mfc2/mtc2/ctc2/cfc2`.
- [ ] Keep every loop/branch in C; islands only around loads / command+latency / stores / DR_MODE.
- [ ] Pin context→`$a1`, mask/tag→`$a2`/`$a3` if the sibling family does.
- [ ] `tools/match.py … && tools/asm_diff.py …`: read the instruction diff; latency/`nop` spacing and register roles are the usual last misses.
- [ ] Sweep `-O1/-O2` × `2.6.3-psx/2.7.2-psx`.
- [ ] Full-function raw-asm/`.word` bridges are **not** a match (merge rejects them). `kaze_init_geom` is the one promoted whole-function-asm exception (CP0/COP2 setup around `_patch_gte`).

## 6. Related

- `include/gte.h`: the macro definitions (edit `gte_macros.inc`-style only with a verified `imm25`).
- Matched GTE siblings: `grep -rlE 'gte_|lwc2|cop2' src/`.
