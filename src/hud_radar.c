#include "types.h"
#include "kaze_syms.h"
extern u16 kaze_radar_wind_needle_x_offsets[];
extern u16 kaze_radar_wind_needle_y_offsets[];
struct R110 { char _p[0x494B4]; short f494B4; short f494B6; };
typedef struct {
    char pad[0x58e58];
    uint flags;
} Big;

extern int kaze_pos_x;
extern int kaze_pos_z;
extern int kaze_wind_current_angle;
extern int kaze_stage_variant;
void kaze_panel_frame_vertices();
void kaze_radar_dot_colors_update();

/**
 * kaze_minimap_radar_builder  (hud, 0x80025d28)
 *
 * Builds the on-screen radar/minimap: maps player position/heading and the
 * +0x58a74/+0x58e14 object arrays into dot/needle/frame vertices
 * (+0x494b4/+0x496d4/+0x496f4/+0x4972c).
 */

void kaze_minimap_radar_builder(short arg0, short arg1)
{
    extern int SquareRoot0(int);
    extern int kaze_state_base_ptr;
    int pad8[2];
    short var_t4;
    short var_t5;
    int b;
    int b1;
    int b2;
    int b3;
    int b4;
    int b5;
    int b6;
    int b7;
    int b8;
    int b9;
    int rawang;
    int var_t2;
    int var_a2;
    int temp_a2_2;
    int temp_a0;
    int temp_t0;
    int temp_a3;
    int temp_a2_3;
    int temp_a1;
    int var_a3;
    int var_s6;
    int var_s5;
    int outerA;
    int outerB;
    int var_s2;
    int var_s3;
    int temp_s0;
    int temp_s1;
    int inner_s5;
    int inner_s6;
    int magic;
    int var_v0;
    int var_v0_2;
    int p1;
    int p2;
    int var_t2_2;
    int var_a3_2;
    int var_a2_2;
    int var_t2_3;
    int var_a2_3;
    int temp_a0_3;
    int tv5a; int tv5b; int tv5c; int tv5d; int tv5e; int tv5f;
    int temp_v1_3;
    int temp_v1_4;
    int temp_v1_5;
    int temp_v1_6;
    int temp_v1_7;
    int temp_v1_8;
    short temp_a0_4;
    short temp_a0_5;
    short temp_a0_6;
    short temp_a0_7;
    short temp_a0_8;
    short temp_a0_9;
    int temp_v0_11;
    int temp_v0_12;
    short temp_a3_2;
    short temp_a0_10;
    short temp_a2_4;
    short temp_a0_11;
    short temp_a1_2;
    short temp_a0_12;
    int stage;
    int toffA;
    int toffB;
    int toffC;
    int addrA;
    int addrC;
    int addrB;
    int addrD;
    int toff7;
    int toff8;
    int toff9;
    int toff6;
    int toff;
    int toff2;
    int toff3;
    int toff4;
    int toff5;

    b = kaze_state_base_ptr;
    var_t4 = arg0;
    var_t5 = arg1;
    *(short *)(b + (*(int *)(b + 0x98) * 0x14) + 0x492CC) = var_t4;
    *(short *)(b + (*(int *)(b + 0x98) * 0x14) + 0x492CE) = var_t5;
    toffB = (*(int *)(b + 0x98) * 0x78) + 0x492FC;
    toffC = (*(int *)(b + 0x98) * 0x60) + 0x493EC;
    kaze_panel_frame_vertices(
        (void *)(b + toffB),
        (void *)(b + toffC),
        arg0, arg1, 0x80, 0x80);
    b1 = kaze_state_base_ptr;
    toff7 = *(int *)(b1 + 0x98) * 0x110;
    *(short *)(b1 + toff7 + 0x494B4) = (arg0 + (kaze_pos_x / 0xed8)) - 3;
    toff7 = *(int *)(b1 + 0x98) * 0x110;
    *(short *)(b1 + toff7 + 0x494B6) = (arg1 + (kaze_pos_z / 0xed8)) - 3;
    rawang = (0x1800 - kaze_wind_current_angle) & 0xFFF;
    temp_s0 = rawang / 0x200;
    var_t2 = 0;
    addrD = b1 + (*(int *)(b1 + 0x98) * 0x10);
    *(u8 *)(addrD + 0x496D8) = (u8)(temp_s0 * 8);
    b2 = kaze_state_base_ptr;
    toff8 = *(int *)(b2 + 0x98) * 0x10;
    toff9 = *(int *)(b2 + 0x98) * 0x110;
    *(short *)(b2 + toff8 + 0x496D4) =
        (u16)((struct R110 *)(b2 + toff9))->f494B4 + kaze_radar_wind_needle_x_offsets[temp_s0];
    toff8 = *(int *)(b2 + 0x98) * 0x10;
    toff9 = *(int *)(b2 + 0x98) * 0x110;
    *(short *)(b2 + toff8 + 0x496D6) =
        (u16)((struct R110 *)(b2 + toff9))->f494B6 + kaze_radar_wind_needle_y_offsets[temp_s0];
    b3 = b2;
    do {
        var_a2 = var_t2 * 2;
        temp_a2_2 = var_a2 + var_t2;
        temp_a0 = b3 + (((temp_a2_2 * 8) - var_t2) * 4);
        temp_t0 = var_t2 + 1;
        temp_a3 = temp_t0 * 0x10;
        toff6 = *(int *)(b3 + 0x98) * 0x110;
        ((struct R110 *)(b3 + (temp_a3 + toff6)))->f494B4 =
            (var_t4 + (*(int *)(temp_a0 + 0x58A7C) / 0xed8)) - 3;
        temp_a2_3 = b3 + (((temp_a2_2 * 4) - var_t2) * 8);
        temp_a3 = temp_a3 + (*(int *)(b3 + 0x98) * 0x110);
        addrA = b3 + temp_a3;
        ((struct R110 *)addrA)->f494B6 =
            (var_t5 + (*(int *)(temp_a0 + 0x58A84) / 0xed8)) - 3;
        temp_a1 = (var_t2 + 4) * 0x10;
        toff6 = *(int *)(b3 + 0x98) * 0x110;
        ((struct R110 *)(b3 + (temp_a1 + toff6)))->f494B4 =
            (var_t4 + (*(int *)(temp_a2_3 + 0x58E14) / 0xed8)) - 3;
        var_t2 = temp_t0;
        temp_a1 = temp_a1 + (*(int *)(b3 + 0x98) * 0x110);
        addrB = b3 + temp_a1;
        ((struct R110 *)addrB)->f494B6 =
            (var_t5 + (*(int *)(temp_a2_3 + 0x58E1C) / 0xed8)) - 3;
    } while (var_t2 < 3);
    var_a3 = 0x3E7;
    outerB = 0;
    outerA = 0;
    do {
        var_s6 = outerA * 0x10 + 0x70;
        var_s5 = outerB * 0x5C;
        __asm__ volatile("" :: "r"(var_s5));
        b4 = kaze_state_base_ptr;
        toff2 = *(int *)(b4 + 0x98) * 0x110;
        ((struct R110 *)(b4 + (var_s6 + toff2)))->f494B4 = var_a3;
        toff2 = *(int *)(b4 + 0x98) * 0x110;
        ((struct R110 *)(b4 + (var_s6 + toff2)))->f494B6 = var_a3;
        if ((*(int *)(b4 + var_s5 + 0x58A74) < 0) &&
            (var_s2 = 0, (*(int *)(b4 + 0x24) > 0))) {
            inner_s5 = var_s5;
            inner_s6 = var_s6;
            __asm__ volatile("" :: "r"(inner_s6), "r"(inner_s6));
            do {
                __asm__ volatile("" :: "r"(var_s2));
                b5 = kaze_state_base_ptr;
                p1 = b5 + inner_s5;
                p2 = b5 + var_s2 * 0x58;
                temp_s0 = *(int *)(p1 + 0x58A7C) - *(int *)(p2 + 0x58E14);
                temp_s1 = *(int *)(p1 + 0x58A84) - *(int *)(p2 + 0x58E1C);
                var_v0 = temp_s0 >= 0 ? temp_s0 : -temp_s0;
                if (var_v0 < 0xED8) {
                    var_v0_2 = temp_s1 >= 0 ? temp_s1 : -temp_s1;
                    if ((var_v0_2 < 0xED8) &&
                        (SquareRoot0((temp_s0 * temp_s0) + (temp_s1 * temp_s1)) < 0x76D)) {
                        b6 = kaze_state_base_ptr;
                        toff3 = *(int *)(b6 + 0x98) * 0x110;
                        ((struct R110 *)(b6 + (inner_s6 + toff3)))->f494B4 =
                            var_t4 + ((temp_s0 << 7) / 0xed8) + 0x3D;
                        toff3 = *(int *)(b6 + 0x98) * 0x110;
                        ((struct R110 *)(b6 + (inner_s6 + toff3)))->f494B6 =
                            var_t5 + ((temp_s1 << 7) / 0xed8) + 0x3D;
                        break;
                    }
                }
                var_s2 += 1;
            } while (var_s2 < *(int *)(kaze_state_base_ptr + 0x24));
        }
        outerB += 1;
        outerA += 1;
    } while (outerB * 0x5C < 0x114);
    b7 = kaze_state_base_ptr;
    var_t2 = 0;
    var_a2_2 = b7;
    do {
        var_a3_2 = var_t2 * 0x10 + 0xA0;
        toff4 = *(int *)(b7 + 0x98) * 0x110;
        ((struct R110 *)(b7 + (var_a3_2 + toff4)))->f494B4 =
            (var_t4 + (*(volatile int *)(var_a2_2 + 0x58E14) / 0xed8)) - 3;
        toff4 = *(int *)(b7 + 0x98) * 0x110;
        ((struct R110 *)(b7 + (var_a3_2 + toff4)))->f494B6 =
            (var_t5 + (*(volatile int *)(var_a2_2 + 0x58E1C) / 0xed8)) - 3;
        var_t2 += 1;
        var_a2_2 += 0x58;
    } while (var_t2 < 6);
    var_t2 = 0;
    b8 = kaze_state_base_ptr;
    var_a2_3 = 0;
    do {
        temp_a0_3 = var_t2 / 2;
        toff5 = *(int *)(b8 + 0x98) * 0x140;
        *(short *)(b8 + (var_a2_3 + toff5) + 0x49808) =
            var_t4 + ((var_t2 - (temp_a0_3 * 2)) << 6);
        toff5 = *(int *)(b8 + 0x98) * 0x140;
        *(short *)(b8 + (var_a2_3 + toff5) + 0x4980A) =
            var_t5 + (temp_a0_3 << 6);
        tv5a = b8 + (var_a2_3 + (*(int *)(b8 + 0x98) * 0x140));
        *(short *)(tv5a + 0x49810) = *(u16 *)(tv5a + 0x49808) + 0x40;
        tv5b = b8 + (var_a2_3 + (*(int *)(b8 + 0x98) * 0x140));
        *(u16 *)(tv5b + 0x49812) = *(u16 *)(tv5b + 0x4980A);
        tv5c = b8 + (var_a2_3 + (*(int *)(b8 + 0x98) * 0x140));
        *(u16 *)(tv5c + 0x49818) = *(u16 *)(tv5c + 0x49808);
        tv5d = b8 + (var_a2_3 + (*(int *)(b8 + 0x98) * 0x140));
        *(short *)(tv5d + 0x4981A) = *(u16 *)(tv5d + 0x4980A) + 0x40;
        tv5e = b8 + (var_a2_3 + (*(int *)(b8 + 0x98) * 0x140));
        *(short *)(tv5e + 0x49820) = *(u16 *)(tv5e + 0x49808) + 0x40;
        var_t2 += 1;
        tv5f = b8 + (var_a2_3 + (*(int *)(b8 + 0x98) * 0x140));
        *(short *)(tv5f + 0x49822) = *(u16 *)(tv5f + 0x4980A) + 0x40;
        var_a2_3 += 0x28;
    } while (var_t2 < 4);
    toffA = (*(int *)(kaze_state_base_ptr + 0x98) * 0x110) + 0x494AC;
    kaze_radar_dot_colors_update((void *)(kaze_state_base_ptr + toffA));
    b9 = kaze_state_base_ptr;
    temp_v1_3 = *(int *)(b9 + 0x98);
    toff = temp_v1_3 * 0x110;
    temp_a0_4 = *(u16 *)(b9 + toff + 0x494C4) + 3;
    toff = temp_v1_3 * 0x1C;
    *(short *)(b9 + toff + 0x496F4) = temp_a0_4;
    toff = temp_v1_3 * 0x14;
    *(short *)(b9 + toff + 0x4972C) = temp_a0_4;
    temp_v1_4 = *(int *)(b9 + 0x98);
    toff = temp_v1_4 * 0x110;
    temp_a0_5 = *(u16 *)(b9 + toff + 0x494C6) + 3;
    toff = temp_v1_4 * 0x1C;
    *(short *)(b9 + toff + 0x496F6) = temp_a0_5;
    toff = temp_v1_4 * 0x14;
    *(short *)(b9 + toff + 0x4972E) = temp_a0_5;
    temp_v1_5 = *(int *)(b9 + 0x98);
    toff = temp_v1_5 * 0x110;
    temp_a0_6 = *(u16 *)(b9 + toff + 0x494D4) + 3;
    toff = temp_v1_5 * 0x1C;
    *(short *)(b9 + toff + 0x496F8) = temp_a0_6;
    toff = temp_v1_5 * 0x14;
    *(short *)(b9 + toff + 0x49730) = temp_a0_6;
    temp_v1_6 = *(int *)(b9 + 0x98);
    toff = temp_v1_6 * 0x110;
    temp_a0_7 = *(u16 *)(b9 + toff + 0x494D6) + 3;
    toff = temp_v1_6 * 0x1C;
    *(short *)(b9 + toff + 0x496FA) = temp_a0_7;
    toff = temp_v1_6 * 0x14;
    *(short *)(b9 + toff + 0x49732) = temp_a0_7;
    temp_v1_7 = *(int *)(b9 + 0x98);
    toff = temp_v1_7 * 0x110;
    temp_a0_8 = *(u16 *)(b9 + toff + 0x494E4) + 3;
    toff = temp_v1_7 * 0x1C;
    *(short *)(b9 + toff + 0x496FC) = temp_a0_8;
    toff = temp_v1_7 * 0x14;
    *(short *)(b9 + toff + 0x49734) = temp_a0_8;
    temp_v1_8 = *(int *)(b9 + 0x98);
    toff = temp_v1_8 * 0x110;
    temp_a0_9 = *(u16 *)(b9 + toff + 0x494E6) + 3;
    toff = temp_v1_8 * 0x1C;
    *(short *)(b9 + toff + 0x496FE) = temp_a0_9;
    toff = temp_v1_8 * 0x14;
    *(short *)(b9 + toff + 0x49736) = temp_a0_9;
    temp_v0_11 = b9 + (*(int *)(b9 + 0x98) * 0x1C);
    *(u16 *)(temp_v0_11 + 0x49700) = *(u16 *)(temp_v0_11 + 0x496F4);
    temp_v0_12 = b9 + (*(int *)(b9 + 0x98) * 0x1C);
    *(u16 *)(temp_v0_12 + 0x49702) = *(u16 *)(temp_v0_12 + 0x496F6);
    __asm__ volatile("" ::: "memory");
    if (kaze_stage_variant == 7) {
        temp_a3_2 = var_t5 + 0x25;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x49768) = var_t4;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x4976A) = temp_a3_2;
        temp_a0_10 = var_t4 + 0x80;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x4976C) = temp_a0_10;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x4976E) = temp_a3_2;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x49770) = var_t4;
        temp_a2_4 = var_t5 + 0x5B;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x49772) = temp_a2_4;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x49774) = temp_a0_10;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x49776) = temp_a2_4;
        temp_a0_11 = var_t4 + 0x25;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x49780) = temp_a0_11;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x49782) = var_t5;
        temp_a1_2 = var_t4 + 0x5B;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x49784) = temp_a1_2;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x49786) = var_t5;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x49788) = temp_a0_11;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x4978A) = temp_a3_2;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x4978C) = temp_a1_2;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x4978E) = temp_a3_2;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x49798) = temp_a0_11;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x4979A) = temp_a2_4;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x4979C) = temp_a1_2;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x4979E) = temp_a2_4;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x497A0) = temp_a0_11;
        temp_a0_12 = var_t5 + 0x80;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x497A2) = temp_a0_12;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x497A4) = temp_a1_2;
        *(short *)((*(int *)(b9 + 0x98) * 0x48) + b9 + 0x497A6) = temp_a0_12;
    }
}

/**
 * kaze_panel_frame_vertices  (hud, 0x80026c8c)
 *
 * Writes screen XY vertex shorts for a bordered panel: inner box at
 * (x-4,y-4), outer border at -8, size unused_w/unused_h, filling two
 * primitive groups at p0 (+0x8..+0x76) and p1 (+0x8..+0x5a).
 */

void kaze_panel_frame_vertices(int p0, int p1, int x, int y, int unused_w, int unused_h)
{
    int x4;
    register int y4 __asm__("$8");
    int x8;
    int y8;
    int xr4;
    int xr8;
    int yr4;
    int yr8;
    int w;
    int h;

    x4 = x - 4;
    y4 = y - 4;
    x8 = x - 8;
    *(short *)(p0 + 0x08) = x4;
    *(short *)(p0 + 0x0A) = y4;
    *(short *)(p0 + 0x0E) = y4;
    *(short *)(p0 + 0x10) = x4;
    *(short *)(p0 + 0x20) = x8;
    /* Narrow old-GCC scheduling island: keep stack arg loads at original points. */
    __asm__ volatile("lw %0, 0x10($sp)" : "=r"(w));
    y8 = y - 8;
    *(short *)(p0 + 0x22) = y8;
    *(short *)(p0 + 0x26) = y8;
    *(short *)(p0 + 0x28) = x8;
    *(short *)(p0 + 0x2A) = y4;
    *(short *)(p0 + 0x2E) = y4;
    *(short *)(p0 + 0x38) = x8;
    *(short *)(p0 + 0x40) = x8;
    *(short *)(p0 + 0x50) = x8;
    *(short *)(p0 + 0x52) = y4;
    *(short *)(p0 + 0x54) = x4;
    *(short *)(p0 + 0x56) = y4;
    *(short *)(p0 + 0x58) = x8;
    *(short *)(p0 + 0x5C) = x4;
    __asm__ volatile("" ::: "memory");
    xr4 = x + w + 4;
    __asm__ volatile("lw %0, 0x14($sp)" : "=r"(h));
    xr8 = x + w + 8;
    *(short *)(p0 + 0x0C) = xr4;
    *(short *)(p0 + 0x14) = xr4;
    *(short *)(p0 + 0x24) = xr8;
    *(short *)(p0 + 0x2C) = xr8;
    *(short *)(p0 + 0x3C) = xr8;
    *(short *)(p0 + 0x44) = xr8;
    __asm__ volatile("" ::: "memory");
    yr4 = y + h + 4;
    yr8 = y + h + 8;
    *(short *)(p0 + 0x12) = yr4;
    *(short *)(p0 + 0x16) = yr4;
    *(short *)(p0 + 0x3A) = yr4;
    *(short *)(p0 + 0x3E) = yr4;
    *(short *)(p0 + 0x42) = yr8;
    *(short *)(p0 + 0x46) = yr8;
    *(short *)(p0 + 0x5A) = yr4;
    *(short *)(p0 + 0x5E) = yr4;
    *(short *)(p0 + 0x68) = xr4;
    *(short *)(p0 + 0x6A) = y4;
    *(short *)(p0 + 0x6C) = xr8;
    *(short *)(p0 + 0x6E) = y4;
    *(short *)(p0 + 0x70) = xr4;
    *(short *)(p0 + 0x72) = yr4;
    *(short *)(p0 + 0x74) = xr8;
    *(short *)(p0 + 0x76) = yr4;

    *(short *)(p1 + 0x08) = xr8;
    *(short *)(p1 + 0x0A) = y8;
    *(short *)(p1 + 0x0C) = x8;
    *(short *)(p1 + 0x0E) = y8;
    *(short *)(p1 + 0x10) = x8;
    *(short *)(p1 + 0x12) = yr8;
    *(short *)(p1 + 0x20) = xr4;
    *(short *)(p1 + 0x22) = y4;
    *(short *)(p1 + 0x24) = x4;
    *(short *)(p1 + 0x26) = y4;
    *(short *)(p1 + 0x28) = x4;
    *(short *)(p1 + 0x2A) = yr4;
    *(short *)(p1 + 0x38) = xr4;
    *(short *)(p1 + 0x3A) = y4;
    *(short *)(p1 + 0x3C) = xr4;
    *(short *)(p1 + 0x3E) = yr4;
    *(short *)(p1 + 0x40) = x4;
    *(short *)(p1 + 0x42) = yr4;
    *(short *)(p1 + 0x50) = xr8;
    *(short *)(p1 + 0x52) = y8;
    *(short *)(p1 + 0x54) = xr8;
    *(short *)(p1 + 0x56) = yr8;
    *(short *)(p1 + 0x58) = x8;
    *(short *)(p1 + 0x5A) = yr8;
}

/**
 * kaze_radar_dot_colors_update  (hud, 0x80026dc0)
 *
 * Sets color/code bytes on the radar primitive array: POLY_F4/F3 codes with
 * blue values 0x40/0x48/0x50/0x58, then per +0x58e14 mode-object slot picks
 * 0x50 (active) or 0x58 (flags 0x84000000 in +0x58e58) for its dot.
 */

void kaze_radar_dot_colors_update(int p)
{
    extern char *kaze_state_base_ptr;
    int i;
    int q;

    i = 0;
    *(u8 *)(p + 0xc) = 0x40;
    *(u8 *)(p + 0xd) = 0x28;
    do {
        *(u8 *)(p + i * 0x10 + 0x1c) = 0x48;
        *(u8 *)(p + i * 0x10 + 0x1d) = 0x20;
        *(u8 *)(p + i * 0x10 + 0x4c) = 0x50;
        *(u8 *)(p + i * 0x10 + 0x4d) = 0x28;
        *(u8 *)(p + i * 0x10 + 0x7c) = 0x58;
        *(u8 *)(p + i * 0x10 + 0x7d) = 0x28;
        i = i + 1;
    } while (i < 3);
    i = 0;
    do {
        q = i * 0x58;
        if ((((Big *)(kaze_state_base_ptr + q))->flags & 0x84000000) != 0) {
            *(u8 *)(p + i * 0x10 + 0xac) = 0x58;
            *(u8 *)(p + i * 0x10 + 0xad) = 0x20;
        } else {
            *(u8 *)(p + i * 0x10 + 0xac) = 0x50;
            *(u8 *)(p + i * 0x10 + 0xad) = 0x20;
        }
        i = i + 1;
    } while (i < 6);
}

/**
 * kaze_radar_range_delta  (hud, 0x80026e84)
 *
 * Radar range test: delta from focus point root+0x68/+0x6c/+0x70 to the
 * given object's position; within +-0x8000 all axes and horizontal distance
 * < 0x76c1, stores short dx/dy/dz into the given output pointer and returns
 * the distance, else -1.
 */

int kaze_radar_range_delta(int *pos, short *out)
{
    extern int kaze_state_base_ptr;
    extern int SquareRoot0(int arg0);
    int dist;
    int dz;
    int dx;
    int dy;

    dx = *(int *)(kaze_state_base_ptr + 0x68) - pos[0];
    dy = pos[1] - *(int *)(kaze_state_base_ptr + 0x6C);
    dz = pos[2] - *(int *)(kaze_state_base_ptr + 0x70);
    if (dx + 0x8000U < 0x10000) {
        if (0xFFFF < dz + 0x8000U) {
            return -1;
        }
        if (0xFFFF < dy + 0x8000U) {
            return -1;
        }
        dist = SquareRoot0(dx * dx + dz * dz);
        if (dist < 0x76C1) {
            goto store_it;
        }
    }
    return -1;
store_it:
    out[0] = dx;
    out[1] = dy;
    out[2] = dz;
    return dist;
}
