/**
 * kaze_score_banner_renderer  (hud, 0x800318c8)
 *
 * Post-event results/score banner renderer: positions icon/label/digit SPRTs
 * (+0x49ea0..+0x49fc6) from root +0x8/+0x10 and stage fields, delegates
 * confirm box to kaze_confirm_box_renderer and radar to
 * kaze_minimap_radar_builder.
 */

#include "types.h"
#include "kaze_syms.h"
struct SH { short v; };
struct UH { unsigned short v; };
struct S78 { char q[0x78]; };
struct BAN { char pad[0x4a008]; struct S78 arr[8]; };
struct S14 { int q[5]; };
struct P14 { char pad[0x49ea0]; struct S14 arr[8]; };
struct S3C { char q[0x3c]; };
struct Pf90 { char pad[0x49f90]; struct S3C arr[8]; };
struct Pfa4 { char pad[0x49fa4]; struct S3C arr[8]; };
struct Pf18 { char pad[0x49f18]; struct S3C arr[8]; };
struct Pf2c { char pad[0x49f2c]; struct S3C arr[8]; };
struct Pfb8 { char pad[0x49fb8]; struct S3C arr[8]; };
struct Pf40 { char pad[0x49f40]; struct S3C arr[8]; };
struct S8 { char q[8]; };
struct Pf8 { char pad[0x4a0f8]; struct S8 arr[8]; };
struct S18 { char q[0x18]; };
struct P94 { char pad[0x48894]; struct S18 arr[8]; };
struct Pc4 { char pad[0x488c4]; struct S8 arr[8]; };

extern int kaze_state_base_ptr __asm__("kaze_state_base_ptr");
extern int kaze_stage_variant;
extern int kaze_stage_mode;
extern byte kaze_stage_index;

extern void kaze_hud_two_digit_counter_284c(int, int, int, int, int);
extern u_short GetClut(int, int);
extern void AddPrim(void *, void *);

void kaze_score_banner_renderer(void *ot)
{
    byte unused_byte;
    u_short clut;
    register int rank asm("$18");
    register uchar shade asm("$5");
    uint stage_idx;
    uint mode_byte;
    uint result_kind;
    register uint result_byte asm("$16");
    register int prim_ofs asm("$16");
    register int x_ofs asm("$17");
    int half_idx;
    int unused_int0;
    int unused_int1;
    register int iClamp asm("$3");
    int iDelta;
    register int iY asm("$4");
    int iLoop;
    register short y_pos asm("$20");
    register int iYf asm("$21");

    rank = *(int *)(kaze_state_base_ptr + 8);
    if (rank >= 0) {
        iClamp = 0xd;
        if (rank < 0xe) {
            iClamp = rank;
        }
    } else {
        iClamp = 0;
    }
    rank = iClamp;
    if (*(int *)(kaze_state_base_ptr + 8) == 0xe) {
        kaze_confirm_box_renderer(ot, 0xa0, 0xb8);
    }
    x_ofs = 0x40;
    if (kaze_stage_variant != 0) {
        x_ofs = 0x50;
    }
    iDelta = 0xd - rank;
    ((struct UH *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x14 + 0x49ea8))->v = 0x20;
    iY = iDelta * 0x20;
    ((struct SH *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x14 + 0x49eaa))->v = rank * 8 + -0x4e;
    y_pos = (short)iY + 0xd4;
    ((struct SH *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49f20))->v = y_pos;
    ((struct SH *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49f22))->v = x_ofs;
    iYf = iY + 0xb4;
    ((struct UH *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49f98))->v = (u_short)iYf;
    ((struct SH *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49f9a))->v = x_ofs + 0x14;
    ((struct SH *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49fac))->v = y_pos;
    ((struct SH *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49fae))->v = x_ofs + 0x24;
    ((struct SH *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49f34))->v = y_pos;
    ((struct SH *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49f36))->v = x_ofs + 0x38;
    ((struct SH *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49f48))->v = y_pos;
    ((struct UH *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49f4a))->v = 0xa0;
    { int _e = *(int *)(kaze_state_base_ptr + 0xc);
    kaze_hud_two_digit_counter_284c((int)(((struct BAN *)kaze_state_base_ptr)->arr + *(int *)(kaze_state_base_ptr + 0x98)), iYf,
                                    x_ofs + 0x4c, kaze_stage_mode, _e); }
    stage_idx = (uint)(byte)kaze_stage_index;
    half_idx = (int)(stage_idx >> 1);
    *(char *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49f9c) = (char)((stage_idx & 1) << 7);
    *(byte *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49f9d) = (byte)(half_idx * 0x10 + 0x50);
    clut = GetClut(stage_idx * 0x10 + 0x1b0, 500);
    mode_byte = *(byte *)&kaze_stage_mode;
    { int _c = *(int *)(kaze_state_base_ptr + 0x98) * 0x3c; ((struct UH *)(kaze_state_base_ptr + _c + 0x49f9e))->v = clut; }
    *(char *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49fb0) = (char)(mode_byte << 6);
    *(char *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49fb1) = 0x70;
    clut = GetClut(mode_byte * 0x10 + 0x1e0, 500);
    { int _c = *(int *)(kaze_state_base_ptr + 0x98) * 0x3c; ((struct UH *)(kaze_state_base_ptr + _c + 0x49fb2))->v = clut; }
    result_kind = 6;
    if (-1 < (int)*(uint *)(kaze_state_base_ptr + 0x10)) {
        result_kind = *(uint *)(kaze_state_base_ptr + 0x10);
    }
    result_byte = result_kind & 0xff;
    { register int _i asm("$2") = *(int *)(kaze_state_base_ptr + 0x98); *(char *)(kaze_state_base_ptr + _i * 0x3c + 0x49fc4) = (char)((result_byte & 3) << 6); }
    *(char *)(kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x49fc5) =
        (char)(result_byte >> 2) * '\x10' + -0x70;
    clut = GetClut(result_byte * 0x10 + 0x140, 0x1f5);
    { int b = kaze_state_base_ptr;
    { int _c = *(int *)(b + 0x98) * 0x3c; ((struct UH *)(b + _c + 0x49fc6))->v = clut; }
    if (result_byte < 6) {
        ((struct UH *)(b + *(int *)(b + 0x98) * 0x3c + 0x49fc8))->v = 0x40;
        ((struct UH *)(b + *(int *)(b + 0x98) * 0x3c + 0x49fca))->v = 0x10;
        ((struct SH *)(b + *(int *)(b + 0x98) * 0x3c + 0x49fc0))->v = y_pos;
    } else {
        ((struct UH *)(b + *(int *)(b + 0x98) * 0x3c + 0x49fc8))->v = 0x80;
        ((struct UH *)(b + *(int *)(b + 0x98) * 0x3c + 0x49fca))->v = 0x10;
        ((struct UH *)(b + *(int *)(b + 0x98) * 0x3c + 0x49fc0))->v = (u_short)iYf;
    }
    ((struct UH *)(b + *(int *)(b + 0x98) * 0x3c + 0x49fc2))->v = 0xb4; }
    __asm__ volatile("" ::: "memory");
    kaze_minimap_radar_builder((rank * 0x20 + -0x17e) * 0x10000 >> 0x10, 0x48);
    shade = (uchar)(rank << 4);
    iLoop = 0;
    rank = 0x4a008;
    { register int b asm("$3") = kaze_state_base_ptr;
      register int idx asm("$4") = *(int *)(b + 0x98);
      prim_ofs = 0;
      { int bs = b + idx * 0x18;
      *(char *)(bs + 0x4889a) = shade;
      *(char *)(bs + 0x48899) = shade;
      *(char *)(bs + 0x48898) = shade; } }
    do {
    int b;
        b = kaze_state_base_ptr;
        AddPrim(ot, (void *)((b + (*(int *)(b + 0x98) * 0x78 + rank)) + prim_ofs));
        prim_ofs = prim_ofs + 0x14;
        iLoop = iLoop + 1;
    } while (iLoop < 6);
    { int b = kaze_state_base_ptr; AddPrim(ot, (void *)(((struct P14 *)b)->arr + *(int *)(b + 0x98))); }
    { int b = kaze_state_base_ptr; AddPrim(ot, (void *)(((struct Pf90 *)b)->arr + *(int *)(b + 0x98))); }
    { int b = kaze_state_base_ptr; AddPrim(ot, (void *)((int)(((struct Pf90 *)b)->arr + *(int *)(b + 0x98)) + 0x14)); }
    { int b = kaze_state_base_ptr; AddPrim(ot, (void *)(((struct Pf18 *)b)->arr + *(int *)(b + 0x98))); }
    { int b = kaze_state_base_ptr; AddPrim(ot, (void *)((int)(((struct Pf18 *)b)->arr + *(int *)(b + 0x98)) + 0x14)); }
    if (kaze_stage_variant == 0) {
        { int b = kaze_state_base_ptr; AddPrim(ot, (void *)((int)(((struct Pf90 *)b)->arr + *(int *)(b + 0x98)) + 0x28)); }
        { int b = kaze_state_base_ptr; AddPrim(ot, (void *)((int)(((struct Pf18 *)b)->arr + *(int *)(b + 0x98)) + 0x28)); }
    }
    { int b = kaze_state_base_ptr; AddPrim(ot, (void *)(((struct Pf8 *)b)->arr + *(int *)(b + 0x98))); }
    kaze_marker_brightness_anim(ot);
    { int b = kaze_state_base_ptr; AddPrim(ot, (void *)(((struct P94 *)b)->arr + *(int *)(b + 0x98))); }
    { int b = kaze_state_base_ptr; AddPrim(ot, (void *)(((struct Pc4 *)b)->arr + *(int *)(b + 0x98))); }
    return;
}
