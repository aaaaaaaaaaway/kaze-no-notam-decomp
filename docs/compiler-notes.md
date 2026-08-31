# Matching notes

## The maspsx submodule needs a patch to build byte-identical

`tools/maspsx` is a git submodule pinned to upstream commit `42b862c`. On
its own, that commit is not enough: this game's original assembler
handled one instruction differently from every version of maspsx, and
without correcting for that, the rebuilt executable does not match the
original byte for byte.

The instruction is `li` (load immediate), which the assembler expands
into one or two real MIPS instructions. For a positive value below
0x8000, upstream maspsx always expands `li` into `ori reg,$zero,value`.
This game's original assembler instead expanded it into
`addiu reg,$zero,value` for values below 0x8000, and only used `ori` for
the 0x8000-0xFFFF range. `ori` and `addiu` produce the same result for
most PS1 code, but they are different opcodes, so the assembled bytes
differ whenever this case is hit, and it is hit often enough (hundreds of
call sites) that the effect is not subtle: without the fix, the payload
comparison in `build.sh` fails with hundreds of scattered single-byte
mismatches spread across most of the matched functions.

`tools/maspsx-li-expansion.patch` holds the five-line fix (a small
`git apply`-compatible patch against `tools/maspsx/maspsx/__init__.py`),
with a comment at the top of the file explaining the same thing in more
detail. `configure.py --deps` applies it automatically, right after it
confirms the submodule is checked out: it runs `git apply --check` first
so re-running `configure.py --deps` is a no-op once the patch is already
in place, rather than an error. If the patch cannot be applied at all
(for example the submodule content has changed upstream in a way the
patch no longer matches), `configure.py` stops with an error naming the
patch file rather than continuing silently into a build that will not
match.

If you ever run `git submodule update --init` by hand instead of going
through `configure.py`, remember to run `./configure.py --deps`
afterward, or the checked-out `maspsx` will be unpatched and
`./build.sh --resplit` will fail its byte-identical check.

## Compiler identification (2026-07-03)

Probe: `kaze_input_poll` (0x800121d0, 41 instructions, 14 global stores).

| candidate | result |
| --- | --- |
| gcc 2.6.3-psx -O1/-O2 | 16/41 diff (perfect 18-insn prefix) |
| gcc 2.7.2-psx -O1/-O2 | 16/41 diff (perfect 18-insn prefix) |
| gcc 2.7.2-psx -O2 + explicit -fschedule-insns(2) | identical output (flags ignored) |
| gcc 2.8.0/2.8.1/2.91.66/2.95.2-psx -O2 | 37-38/41 diff (wrong codegen family) |

Trivial probe (`kaze_model_set_rotation_component_3489c`, 5 insns) matches
all 2.6/2.7 combos exactly.

Conclusion: 2.6.3/2.7.2-psx class, `-O2 -mips1 -G0`. The residual 16-diff
tail in kaze_input_poll is constant-load hoisting (`li $v1,0x40` issued 6
stores before use; `li $v0,8` issued 3 stores before use) that our cc1 does
not produce with any tested flag; locals/chained-assignment source shapes did
not reproduce it either.

Open leads for the hoist quirk, in order:
1. maspsx `--aspsx-version` sweep (real ASPSX did assembler-side reordering;
   we ran maspsx defaults).
2. PsyQ CCPSX driver defaults (the SDK driver may pass extra cc1 flags; find
   a CCPSX.EXE listing or DMPSX docs).
3. gcc 2.6.0-psx / 2.5.7-psx (earlier PsyQ releases).
4. A `-O3` / `-finline-functions` variant changing the RTL enough to alter
   scheduling.

## Build gotchas (already encoded in build.sh/config)

- GNU as MIPS `.text` default alignment is 16; the game's text starts at ROM
  0x1da8 (8-aligned). `objcopy --set-section-alignment .text=4` after
  assembling, and `align: 4` + `ld_align_segment_start: 4` in the splat
  config, or the whole image shifts by 8/16 bytes.
- maspsx reads stdin; passing a filename hangs it.
- Symbol names: `2MBYTE_OBJ_B4` needed an underscore prefix (digit-led
  symbol), two `memcpy` copies needed address suffixes.


## Optimisation level: a per-function mix

- **Initial discovery: `-O1 -mips1 -G0`.** Investigating the kaze_input_poll
  mismatch above showed that at -O1, gcc keeps local variable constant
  assignments where written, while -O2's CSE flattens them. The
  kaze_input_poll "hoisted constants" were real locals in the original source
  (`start`, `cross`, `select` button-mask temporaries; Cross is bound to two
  config slots, hence the reused local). That explained the codegen
  difference, but it was not the whole picture: -O1 is not the flag used
  everywhere in the binary.
- **Finished result: a per-function mix.** `config/matched.json` records the
  optimization level used for each of the 648 matched functions: 580 at
  `-O2` and 68 at `-O1`. Both `-mips1 -G0`.
- **maspsx li-expansion patch** (tools/maspsx/maspsx/__init__.py, documented
  local change): this game's ASPSX emits `addiu reg,$zero,imm` for
  0 < imm < 0x8000 and `ori` only for 0x8000..0xFFFF; upstream maspsx emits
  `ori` for all positive 16-bit values.
- **First byte-true match:** kaze_input_poll (164 bytes) via
  `tools/match.py src/input_defaults.c kaze_input_poll`, and the full EXE
  rebuilds byte-identically with that function compiled from C
  (`./build.sh --resplit` → sha1 f8f44c2d...).

### Matching method that worked (use as the template)

1. Start from a readable decompilation of the target function (Ghidra output,
   renamed to plain symbols).
2. Write plain decomp-style C (extern int globals, direct constants).
3. Diff with tools/match.py; read the register pattern of the mismatch:
   - a constant materialized far from its store = a local variable assigned
     earlier in source;
   - two constants alive across a store = both were locals declared in order;
   - register numbers reveal declaration/assignment order at -O1.
4. Iterate source shape (locals, statement order), not compiler flags.


## Match history

The binary mixes `-O1` and `-O2` per translation unit; `config/matched.json`
records the optimization level used for each function.

## GTE matching approach

The GTE functions (131 of them) needed a dedicated GTE macro header before
they could be matched, so they were handled on a track separate from the
plain/big/jumptable functions.

They fell into two classes:
1. Leaves using only cop2 load/store/control (`ctc2`, `swc2`, `lwc2`,
   `mfc2`): matched with plain `__asm__ volatile("ctc2 %0, $26" :: "r"(x))`,
   since GNU `as` already knows these mnemonics. Register in/out via
   `"r"`/`"=r"`; C does any shifts.
2. GTE command functions (`rtps`, `rtpt`, `nclip`, `nccs`, `avsz3`, `mvmva`,
   etc.): GNU `as` does not know these mnemonics, so assembling them
   directly fails. The fix was `include/gte.h`, a macro header defining each
   command as its `.word 0x...` cop2 encoding for use from inline asm
   (ported from the assembly-side encodings already in
   `include/gte_macros.inc`). That resolved the blocker for GTE-command
   functions such as `kaze_gte_project_vertex` (rtps plus a scratchpad
   round-trip at `0x1F800200`) and the rest of the GTE family.

## Outcome

Functions were matched in batches: each candidate was compiled, diffed
against the original bytes with the match tool, merged into `src/`, and the
full image was rebuilt to confirm it stayed byte-identical. Recurring
technique notes from that process (per-function compiler and optimization
level selection, PRE-TEXT rodata placement for switch tables, and the GTE
command/data-movement split above) are the parts with lasting value; the
day-to-day batch tallies are not, since they describe a mid-process state
that no longer exists.

The matching decompilation finished at 648/648 functions and
344,020/344,020 bytes, byte-identical against the original EXE (sha1
`f8f44c2ddaf4969f4132fb17d1abab06a75c2def`). `tools/progress.py` is the
current source of truth for these numbers.
