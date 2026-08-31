# Contributing

The decompilation is complete, so most useful work is now clarity: better
names, better documentation, better structure. All of it has to keep the
build byte-identical.

## The rule

Every change under `src/` ends with:

    make

and that must print:

    EXE OK (byte-identical): f8f44c2ddaf4969f4132fb17d1abab06a75c2def

If it does not, the change is not mergeable, however good it looks.

## Checking one function

    python3 tools/match.py src/<file>.c <function> --opt=-O2 --compiler=2.7.2-psx

Take the optimisation level and compiler from that function's entry in
`config/matched.json`. Exit status 0 means the bytes match. This works for a
function inside a shared multi-function file the same way it works for a
standalone one.

When it does not match, see where:

    python3 tools/asm_diff.py src/<file>.c <function>
    ./diff.py <function>

## Safe and unsafe changes

Safe: comments, local and parameter names, moving a function between files,
reordering includes.

Unsafe: changing a declared type, reordering statements, introducing or
removing a temporary, changing an expression's shape. Any of these can change
register allocation or scheduling. Verify.

## Tools

| Tool | Purpose |
| --- | --- |
| `tools/progress.py` | matched function and byte counts |
| `tools/match.py` | verify one function |
| `tools/asm_diff.py` | instruction-level diff on a mismatch |
| `tools/region_match.py` | block-level map for large functions |
| `tools/similar_functions.py` | find functions with similar shape |
| `tools/island_audit.py` | strict review board |
| `tools/consolidate_tus.py` | merge single-function files into subsystem units |
| `tools/inject_doc_comments.py` | regenerate doc comments from docs/function-map.json |

## Style

Plain descriptions. No em-dashes. Name things after what they are. If a
parameter is a live-root pointer, call it `state`, not a bare letter and not
a generic placeholder name.
