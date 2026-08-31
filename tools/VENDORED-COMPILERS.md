# Vendored PSX GCC toolchains (provenance)

All are PSX-era GCC builds from **decompals/old-gcc** (https://github.com/decompals/old-gcc),
release **0.17** Linux tarballs (ELF 32-bit LSB, Intel i386, statically linked).
Our `tools/gcc-2.7.2-psx/cc1` is byte-identical to that release's `gcc-2.7.2-psx.tar.gz`,
confirming the source. Each `tools/gcc-<name>/` plugs into `tools/match.py` and
`tools/wall_sweep.py` via `--compiler=<name>`.

The four 2.7.2 sibling builds (2.7.2, 2.7.2.1, 2.7.2.2, 2.7.2-cdk) were added
2026-07-09 for the multi-compiler wall sweep and carry their own per-dir README.md.
The others predate this session (vendored 2026-07-03); provenance is the same
decompals/old-gcc release line (0.15-0.17 rebuilds; sha256 below are the
as-vendored bytes).

Note: gcc 2.6.4 does NOT exist in the decompals set (only 2.6.0 and 2.6.3);
the 2.6.x target-set entry is therefore not fetchable and is bracketed by
2.6.0/2.6.3, both swept.

| toolchain dir | --compiler name | gcc version | cc1 sha256 | per-dir README |
|---|---|---|---|---|
| tools/gcc-2.5.7-psx | 2.5.7-psx | 2.5.7 | `3ffb42baf340cd017c7f6ff7a7dc769c...` | no (pre-existing) |
| tools/gcc-2.6.0-psx | 2.6.0-psx | 2.6.0 | `1fb4f513d73c33f7eed74d3adbe47115...` | no (pre-existing) |
| tools/gcc-2.6.3-psx | 2.6.3-psx | 2.6.3 | `f4391c56fe7c602a0287619cfc4c94a3...` | no (pre-existing) |
| tools/gcc-2.7.0 | 2.7.0 | 2.7.0 | `6bbab4dbdd17805599d84ac2fcb143a9...` | no (pre-existing) |
| tools/gcc-2.7.1 | 2.7.1 | 2.7.1 | `69e4bf7feb2eaae048843c8af869ed43...` | no (pre-existing) |
| tools/gcc-2.7.2 | 2.7.2 | 2.7.2 | `fee572bf052e83f7610b77ca776aa8c3...` | yes |
| tools/gcc-2.7.2-cdk | 2.7.2-cdk | ? | `90db2db7bcfe372244ca4d9785a7bfbf...` | yes |
| tools/gcc-2.7.2-psx | 2.7.2-psx | 2.7.2 | `0359379289db8e3904b8ed3b25b422ca...` | no (pre-existing) |
| tools/gcc-2.7.2.1 | 2.7.2.1 | 2.7.2 | `ded080a488cb4f6e2c3ac096fd1044d4...` | yes |
| tools/gcc-2.7.2.2 | 2.7.2.2 | 2.7.2 | `50f52902babff9e212013a82e3c000fc...` | yes |
| tools/gcc-2.7.2.3 | 2.7.2.3 | 2.7.2 | `63a8a89cebacfd0229ac681f0b10780c...` | no (pre-existing) |
| tools/gcc-2.8.0-psx | 2.8.0-psx | 2.8.0 | `f752a0865ae4b9e7570c2f245e6562c7...` | no (pre-existing) |
| tools/gcc-2.8.1-psx | 2.8.1-psx | 2.8.1 | `1a48ec5ac87a146a074a9d66d7a67734...` | no (pre-existing) |
| tools/gcc-2.91.66-psx | 2.91.66-psx | ? | `ab7bacfde8c88dd781d093362b5a57ba...` | no (pre-existing) |
| tools/gcc-2.95.2-psx | 2.95.2-psx | 2.95.2 | `2aa85925dfa10855107c78e29ec36965...` | no (pre-existing) |
