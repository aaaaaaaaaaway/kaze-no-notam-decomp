#include "types.h"
#include "kaze_syms.h"
typedef struct {
    short vx, vy, vz;
    unsigned short pad;
} SVECTOR;

#define gte_rtpt() __asm__ volatile("cop2 0x280030")
#define gte_rtps() __asm__ volatile("cop2 0x180001")
#define gte_ldv3(x0, x1, x2)                                                   \
    __asm__ volatile("lwc2 $0, 0(%0)\n\t"                                      \
                     "lwc2 $1, 4(%0)\n\t"                                      \
                     "lwc2 $2, 0(%1)\n\t"                                      \
                     "lwc2 $3, 4(%1)\n\t"                                      \
                     "lwc2 $4, 0(%2)\n\t"                                      \
                     "lwc2 $5, 4(%2)"                                          \
                     :: "r"(x0), "r"(x1), "r"(x2))
#define gte_ldv0(x)                                                            \
    __asm__ volatile("lwc2 $0, 0(%0)\n\t"                                      \
                     "lwc2 $1, 4(%0)"                                          \
                     :: "r"(x))
#define gte_stflg(p)                                                           \
    __asm__ volatile("cfc2 $12, $31\n\t"                                       \
                     "nop\n\t"                                                 \
                     "sw $12, 0(%0)"                                           \
                     :: "r"(p) : "$12", "memory")
#define gte_stsxy(x) __asm__ volatile("swc2 $14, 0(%0)" :: "r"(x) : "memory")
#define gte_stsxy3_g3(x)                                                       \
    __asm__ volatile("swc2 $12, 8(%0)\n\t"                                     \
                     "swc2 $13, 16(%0)\n\t"                                    \
                     "swc2 $14, 24(%0)"                                        \
                     :: "r"(x) : "memory")
#define gte_stsz4(a, b, c, d)                                                  \
    __asm__ volatile("swc2 $16, 0(%0)\n\t"                                     \
                     "swc2 $17, 0(%1)\n\t"                                     \
                     "swc2 $18, 0(%2)\n\t"                                     \
                     "swc2 $19, 0(%3)"                                         \
                     :: "r"(a), "r"(b), "r"(c), "r"(d) : "memory")

/**
 * kaze_quad_subdivide_render  (render, 0x80030a1c)
 *
 * Recursive subdividing POLY_FT4 quad renderer: projects 4 SVECTOR+UV
 * vertices, clip-tests via kaze_quad_screen_clip_test, emits an FT4 packet
 * into ctx+0x40 when small/at depth 4, else recurses on midpoint subquads
 * (sky clouds).
 */

void kaze_quad_subdivide_render(SVECTOR *verts, int ctx, int depth)
{
    SVECTOR *quad = verts;
    SVECTOR *sub_verts;
    register SVECTOR *recurse_verts __asm__("$21");
    u32 *r0;
    u32 *out_prim;
    int next_ptr;
    int byte3_val;
    int byte7_val;
    char *r0_00;
    SVECTOR *r1;
    SVECTOR *s10;
    SVECTOR *s18;
    SVECTOR *s20;
    SVECTOR *s28;

    gte_ldv3(quad, quad + 1, quad + 2);
    sub_verts = quad + 4;
    recurse_verts = sub_verts;
    gte_rtpt();
    *(short *)(ctx + 0x88) = quad->pad;
    r0 = (u32 *)(ctx + 0x7c);
    *(short *)(ctx + 0x90) = quad[1].pad;
    gte_stflg(ctx + 0xa8);
    gte_stsxy3_g3(r0);
    gte_ldv0(quad + 3);
    *(short *)(ctx + 0x98) = quad[2].pad;
    gte_rtps();
    *(short *)(ctx + 0xa0) = quad[3].pad;
    gte_stflg(ctx + 0xac);
    *(u32 *)(ctx + 0xa8) = *(u32 *)(ctx + 0xa8) | *(u32 *)(ctx + 0xac);
    gte_stsxy(ctx + 0x9c);
    gte_stsz4(ctx + 0xb8, ctx + 0xbc, ctx + 0xc0, ctx + 0xc4);
    if (kaze_quad_screen_clip_test(r0, ctx) != 0) {
        return;
    }
    if (depth == 4) {
        if (-1 < *(int *)(ctx + 0xa8)) {
            goto LAB_ba4;
        }
        return;
    }
    if (*(int *)(ctx + 0xa8) < 0) {
        goto LAB_c70;
    }
    if ((int)*(short *)(ctx + 0x74) - (int)*(short *)(ctx + 0x76) >= 0xff) {
        goto LAB_c70;
    }
    if ((int)*(short *)(ctx + 0x78) - (int)*(short *)(ctx + 0x7a) >= 0x7f) {
        goto LAB_c70;
    }
LAB_ba4:
    {
        **(u32 **)(ctx + 0x40) = **(u32 **)(ctx + 0x44) & 0xffffff | 0x9000000;
        *(u32 *)(*(int *)(ctx + 0x40) + 4) = r0[1];
        *(u32 *)(*(int *)(ctx + 0x40) + 8) = r0[2];
        *(u32 *)(*(int *)(ctx + 0x40) + 0xc) = r0[3];
        *(u32 *)(*(int *)(ctx + 0x40) + 0x10) = r0[4];
        *(u32 *)(*(int *)(ctx + 0x40) + 0x14) = r0[5];
        *(u32 *)(*(int *)(ctx + 0x40) + 0x18) = r0[6];
        *(u32 *)(*(int *)(ctx + 0x40) + 0x1c) = r0[7];
        *(u32 *)(*(int *)(ctx + 0x40) + 0x20) = r0[8];
        *(u32 *)(*(int *)(ctx + 0x40) + 0x24) = r0[9];
        **(u32 **)(ctx + 0x44) = *(int *)(ctx + 0x40) & 0xffffff;
        next_ptr = *(int *)(ctx + 0x40) + 0x28;
        goto LAB_store;
    }
LAB_c70:
    {
        /* --- subdivide: block 1 --- */
        sub_verts[0] = quad[0];
        sub_verts[1].vx = (short)(((int)quad[0].vx + (int)quad[1].vx) / 2);
        sub_verts[1].vy = (short)(((int)quad[0].vy + (int)quad[1].vy) / 2);
        sub_verts[1].vz = (short)(((int)quad[0].vz + (int)quad[1].vz) / 2);
        *(char *)&sub_verts[1].pad =
            (char)((int)(*(u8 *)&quad[0].pad + *(u8 *)&quad[1].pad) >> 1);
        *((char *)&sub_verts[1].pad + 1) =
            (char)((int)(*((u8 *)&quad[0].pad + 1) + *((u8 *)&quad[1].pad + 1)) >> 1);
        sub_verts[2].vx = (short)(((int)quad[0].vx + (int)quad[2].vx) / 2);
        sub_verts[2].vy = (short)(((int)quad[0].vy + (int)quad[2].vy) / 2);
        sub_verts[2].vz = (short)(((int)quad[0].vz + (int)quad[2].vz) / 2);
        *(char *)&sub_verts[2].pad =
            (char)((int)(*(u8 *)&quad[0].pad + *(u8 *)&quad[2].pad) >> 1);
        *((char *)&sub_verts[2].pad + 1) =
            (char)((int)(*((u8 *)&quad[0].pad + 1) + *((u8 *)&quad[2].pad + 1)) >> 1);
        sub_verts[3].vx = (short)(((int)quad[0].vx + (int)quad[3].vx) / 2);
        depth = depth + 1;
        sub_verts[3].vy = (short)(((int)quad[0].vy + (int)quad[3].vy) / 2);
        sub_verts[3].vz = (short)(((int)quad[0].vz + (int)quad[3].vz) / 2);
        *(char *)&sub_verts[3].pad =
            (char)((int)(*(u8 *)&quad[0].pad + *(u8 *)&quad[3].pad) >> 1);
        *((char *)&sub_verts[3].pad + 1) =
            (char)((int)(*((u8 *)&quad[0].pad + 1) + *((u8 *)&quad[3].pad + 1)) >> 1);
        kaze_quad_subdivide_render(recurse_verts, ctx, depth);
        s10 = sub_verts + 1;
        r1 = quad + 1;
        s18 = sub_verts + 2;
        s20 = quad + 2;
        s28 = quad + 3;
        gte_ldv3(quad, r1, s10);
        out_prim = *(u32 **)(ctx + 0x40);
        out_prim[1] = *(u32 *)(ctx + 0x80);
        gte_rtpt();
        byte3_val = 7;
        byte7_val = 0x24;
        r0_00 = (char *)ctx + 0xa8;
        *(char *)((int)out_prim + 3) = byte3_val;
        *(char *)((int)out_prim + 7) = byte7_val;
        gte_stflg(r0_00);
        if (-1 < *(int *)(ctx + 0xa8)) {
            gte_stsxy3_g3(out_prim);
            *(short *)(out_prim + 3) = quad[0].pad;
            *(short *)(out_prim + 5) = quad[1].pad;
            *(short *)(out_prim + 7) = sub_verts[1].pad;
            *(u16 *)((int)out_prim + 0x16) = *(u16 *)(ctx + 0x92);
            *(u16 *)((int)out_prim + 0xe) = *(u16 *)(ctx + 0x8a);
            *out_prim = **(u32 **)(ctx + 0x44) & 0xffffff | 0x7000000;
            **(u32 **)(ctx + 0x44) = (u32)out_prim & 0xffffff;
            *(int *)(ctx + 0x40) = *(int *)(ctx + 0x40) + 0x20;
        }

        /* --- block 2 --- */
        sub_verts[0] = quad[1];
        sub_verts[2].vx = (short)(((int)quad[1].vx + (int)quad[3].vx) / 2);
        sub_verts[2].vy = (short)(((int)quad[1].vy + (int)quad[3].vy) / 2);
        sub_verts[2].vz = (short)(((int)quad[1].vz + (int)quad[3].vz) / 2);
        *(char *)&sub_verts[2].pad =
            (char)((int)(*(u8 *)&quad[1].pad + *(u8 *)&quad[3].pad) >> 1);
        *((char *)&sub_verts[2].pad + 1) =
            (char)((int)(*((u8 *)&quad[1].pad + 1) + *((u8 *)&quad[3].pad + 1)) >> 1);
        kaze_quad_subdivide_render(recurse_verts, ctx, depth);
        gte_ldv3(r1, s28, s18);
        out_prim = *(u32 **)(ctx + 0x40);
        out_prim[1] = *(u32 *)(ctx + 0x80);
        gte_rtpt();
        *(char *)((int)out_prim + 3) = byte3_val;
        *(char *)((int)out_prim + 7) = byte7_val;
        gte_stflg(r0_00);
        if (-1 < *(int *)(ctx + 0xa8)) {
            gte_stsxy3_g3(out_prim);
            *(short *)(out_prim + 3) = quad[1].pad;
            *(short *)(out_prim + 5) = quad[3].pad;
            *(short *)(out_prim + 7) = sub_verts[2].pad;
            *(u16 *)((int)out_prim + 0x16) = *(u16 *)(ctx + 0x92);
            *(u16 *)((int)out_prim + 0xe) = *(u16 *)(ctx + 0x8a);
            *out_prim = **(u32 **)(ctx + 0x44) & 0xffffff | 0x7000000;
            **(u32 **)(ctx + 0x44) = (u32)out_prim & 0xffffff;
            *(int *)(ctx + 0x40) = *(int *)(ctx + 0x40) + 0x20;
        }

        /* --- block 3 --- */
        sub_verts[0] = quad[3];
        sub_verts[1].vx = (short)(((int)quad[3].vx + (int)quad[2].vx) / 2);
        sub_verts[1].vy = (short)(((int)quad[3].vy + (int)quad[2].vy) / 2);
        sub_verts[1].vz = (short)(((int)quad[3].vz + (int)quad[2].vz) / 2);
        *(char *)&sub_verts[1].pad =
            (char)((int)(*(u8 *)&quad[3].pad + *(u8 *)&quad[2].pad) >> 1);
        *((char *)&sub_verts[1].pad + 1) =
            (char)((int)(*((u8 *)&quad[3].pad + 1) + *((u8 *)&quad[2].pad + 1)) >> 1);
        kaze_quad_subdivide_render(recurse_verts, ctx, depth);
        gte_ldv3(s28, s20, s10);
        out_prim = *(u32 **)(ctx + 0x40);
        out_prim[1] = *(u32 *)(ctx + 0x80);
        gte_rtpt();
        *(char *)((int)out_prim + 3) = byte3_val;
        *(char *)((int)out_prim + 7) = byte7_val;
        gte_stflg(r0_00);
        if (-1 < *(int *)(ctx + 0xa8)) {
            gte_stsxy3_g3(out_prim);
            *(short *)(out_prim + 3) = quad[3].pad;
            *(short *)(out_prim + 5) = quad[2].pad;
            *(short *)(out_prim + 7) = sub_verts[1].pad;
            *(u16 *)((int)out_prim + 0x16) = *(u16 *)(ctx + 0x92);
            *(u16 *)((int)out_prim + 0xe) = *(u16 *)(ctx + 0x8a);
            *out_prim = **(u32 **)(ctx + 0x44) & 0xffffff | 0x7000000;
            **(u32 **)(ctx + 0x44) = (u32)out_prim & 0xffffff;
            *(int *)(ctx + 0x40) = *(int *)(ctx + 0x40) + 0x20;
        }

        /* --- block 4 --- */
        sub_verts[0] = quad[2];
        sub_verts[2].vx = (short)(((int)quad[0].vx + (int)quad[2].vx) / 2);
        sub_verts[2].vy = (short)(((int)quad[0].vy + (int)quad[2].vy) / 2);
        sub_verts[2].vz = (short)(((int)quad[0].vz + (int)quad[2].vz) / 2);
        *(char *)&sub_verts[2].pad =
            (char)((int)(*(u8 *)&quad[0].pad + *(u8 *)&quad[2].pad) >> 1);
        *((char *)&sub_verts[2].pad + 1) =
            (char)((int)(*((u8 *)&quad[0].pad + 1) + *((u8 *)&quad[2].pad + 1)) >> 1);
        kaze_quad_subdivide_render(recurse_verts, ctx, depth);
        gte_ldv3(s20, quad, s18);
        out_prim = *(u32 **)(ctx + 0x40);
        out_prim[1] = *(u32 *)(ctx + 0x80);
        gte_rtpt();
        *(char *)((int)out_prim + 3) = byte3_val;
        *(char *)((int)out_prim + 7) = byte7_val;
        gte_stflg(r0_00);
        if (*(int *)(ctx + 0xa8) < 0) {
            return;
        }
        gte_stsxy3_g3(out_prim);
        *(short *)(out_prim + 3) = quad[2].pad;
        *(short *)(out_prim + 5) = quad[0].pad;
        *(short *)(out_prim + 7) = sub_verts[2].pad;
        *(u16 *)((int)out_prim + 0x16) = *(u16 *)(ctx + 0x92);
        *(u16 *)((int)out_prim + 0xe) = *(u16 *)(ctx + 0x8a);
        *out_prim = **(u32 **)(ctx + 0x44) & 0xffffff | 0x7000000;
        **(u32 **)(ctx + 0x44) = (u32)out_prim & 0xffffff;
        next_ptr = *(int *)(ctx + 0x40) + 0x20;
    }
LAB_store:
    *(int *)(ctx + 0x40) = next_ptr;
    return;
}

#include "gte.h"
#define GTE_STSXY(dst) \
    __asm__ volatile("swc2 $14, 0(%0)" :: "r"(dst) : "memory")

/**
 * kaze_gte_poly_project_clip_1424  (render, 0x80031424)
 *
 * GTE poly projection/clip helper: ldv3/rtpt a tri, stores screen XY into a
 * primitive struct (param+0x40), clips to screen bounds, builds a POLY with
 * draw TPage/mode and links it into the OT.
 */

void kaze_gte_poly_project_clip_1424(int verts, int ctx)
{
    register int c asm("$7") = ctx;
    register int p asm("$5");
    register int xy asm("$6");
    int scratch[8];
    int prim;
    int tmp;
    int z;

    __asm__ volatile("" : "=m"(scratch));

    p = *(int *)(c + 0x40);
    xy = p + 8;
    gte_ldv3(verts, verts + 8, verts + 0x10);
    gte_nop();
    gte_nop();
    gte_rtpt();
    gte_stflg(c + 0xa8);
    gte_stsxy3_f3(xy);
    gte_ldv0(verts + 0x18);
    gte_nop();
    gte_nop();
    gte_rtps();
    gte_stflg(c + 0xac);
    GTE_STSXY(p + 0x1c);
    gte_stsz4(c + 0xb8, c + 0xbc, c + 0xc0, c + 0xc4);

    {
        register int near_z asm("$4");
        register int far_z asm("$3");

        near_z = *(int *)(c + 0xb8);
        __asm__ volatile("" :: "r"(near_z));
        far_z = *(int *)(c + 0xbc);
        if (far_z < near_z) {
            *(int *)(c + 0xac) = near_z;
        } else {
            *(int *)(c + 0xac) = far_z;
        }
    }
    tmp = *(int *)(c + 0xc0);
    if (*(int *)(c + 0xac) < tmp) {
        *(int *)(c + 0xac) = tmp;
    }
    tmp = *(int *)(c + 0xc4);
    if (*(int *)(c + 0xac) < tmp) {
        *(int *)(c + 0xac) = tmp;
    }

    {
        register int max_z asm("$3");
        register int shifted asm("$2");

        max_z = *(int *)(c + 0xac);
        shifted = *(int *)(c + 0xb4);
        shifted = max_z >> shifted;
        if (shifted < 0) {
            shifted += 3;
        }
        z = (shifted >> 2) << 2;
    }
    *(int *)(c + 0x44) = *(int *)(c + 0x48) + z;
    *(u32 *)(c + 0xa8) = *(u32 *)(c + 0xa8) | *(u32 *)(c + 0xac);

    if (*(int *)(c + 0xb8) < 0x7d &&
        *(int *)(c + 0xbc) < 0x7d &&
        *(int *)(c + 0xc0) < 0x7d &&
        *(int *)(c + 0xc4) < 0x7d) {
        return;
    }

    if (*(s16 *)(xy + 0xc) < *(s16 *)(xy + 8)) {
        *(s16 *)(c + 0x74) = *(s16 *)(xy + 8);
        *(u16 *)(c + 0x76) = *(u16 *)(xy + 0xc);
    } else {
        *(s16 *)(c + 0x76) = *(s16 *)(xy + 8);
        *(u16 *)(c + 0x74) = *(u16 *)(xy + 0xc);
    }

    {
        register int x asm("$4");
        register int value asm("$3");

        x = *(s16 *)(xy + 0x10);
        value = x;
        if (*(s16 *)(c + 0x74) < x) {
            *(s16 *)(c + 0x74) = value;
        } else if (x < *(s16 *)(c + 0x76)) {
            *(s16 *)(c + 0x76) = value;
        }
        __asm__ volatile("" :: "r"(x));
    }
    {
        register int x asm("$4");
        register int value asm("$3");

        x = *(s16 *)(xy + 0x14);
        value = x;
        if (*(s16 *)(c + 0x74) < x) {
            *(s16 *)(c + 0x74) = value;
        } else if (x < *(s16 *)(c + 0x76)) {
            *(s16 *)(c + 0x76) = value;
        }
        __asm__ volatile("" :: "r"(x));
    }

    if (*(s16 *)(c + 0x74) < 0 || 0x140 < *(s16 *)(c + 0x76)) {
        return;
    }

    if (*(s16 *)(xy + 0xe) < *(s16 *)(xy + 0xa)) {
        *(s16 *)(c + 0x78) = *(s16 *)(xy + 0xa);
        *(u16 *)(c + 0x7a) = *(u16 *)(xy + 0xe);
    } else {
        *(s16 *)(c + 0x7a) = *(s16 *)(xy + 0xa);
        *(u16 *)(c + 0x78) = *(u16 *)(xy + 0xe);
    }

    {
        register int y asm("$4");
        register int value asm("$3");

        y = *(s16 *)(xy + 0x12);
        value = y;
        if (*(s16 *)(c + 0x78) < y) {
            *(s16 *)(c + 0x78) = value;
        } else if (y < *(s16 *)(c + 0x7a)) {
            *(s16 *)(c + 0x7a) = value;
        }
        __asm__ volatile("" :: "r"(y));
    }
    {
        register int y asm("$5");
        register int value asm("$3");

        y = *(s16 *)(xy + 0x16);
        value = y;
        if (*(s16 *)(c + 0x78) < y) {
            *(s16 *)(c + 0x78) = value;
        } else if (y < *(s16 *)(c + 0x7a)) {
            *(s16 *)(c + 0x7a) = value;
        }
        __asm__ volatile("" :: "r"(y));
    }

    if (*(s16 *)(c + 0x78) >= 0 &&
        *(s16 *)(c + 0x7a) < 0xf1 &&
        *(int *)(c + 0xa8) >= 0 &&
        (int)*(s16 *)(c + 0x74) - (int)*(s16 *)(c + 0x76) < 0xff &&
        (int)*(s16 *)(c + 0x78) - (int)*(s16 *)(c + 0x7a) < 0x7f) {
        prim = *(int *)(c + 0x40);
        *(u32 *)prim = ((u32)(prim + 8) & 0xffffff) | 0x1000000;
        *(u32 *)(*(int *)(c + 0x40) + 4) = *(u32 *)(c + 0xa4) | 0xe1000200;
        *(u32 *)(*(int *)(c + 0x40) + 8) =
            (*(u32 *)*(int *)(c + 0x44) & 0xffffff) | 0x5000000;
        *(u32 *)(*(int *)(c + 0x40) + 0xc) = *(u32 *)(c + 0x54);
        *(u32 *)*(int *)(c + 0x44) = *(u32 *)(c + 0x40) & 0xffffff;
        *(int *)(c + 0x40) = *(int *)(c + 0x40) + 0x20;
    }
}
