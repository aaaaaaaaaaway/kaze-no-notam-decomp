
#include "types.h"

#include "gte.h"
#define gte_nclip() __asm__ volatile("cop2 0x1400006")
#define gte_avsz3() __asm__ volatile("cop2 0x158002D")
#define gte_nccs() __asm__ volatile("cop2 0x108041B")
#define gte_ldrgb(x) __asm__ volatile("lwc2 $6, 0(%0)" :: "r"(x))
#define gte_stopz(x) __asm__ volatile("swc2 $24, 0(%0)" :: "r"(x) : "memory")
#define gte_stotz(x) __asm__ volatile("swc2 $7, 0(%0)" :: "r"(x) : "memory")
#define gte_strgb(x) __asm__ volatile("swc2 $22, 0(%0)" :: "r"(x) : "memory")

/**
 * kaze_model_tri_f3_fixedlight_3f0cc  (render, 0x8003f0cc)
 *
 * TMD submodel rasterizer: flat triangles (gte_stsxy3_f3, nclip/avsz3, POLY
 * code 0x20/0x22000000) in the kaze_draw_model_flatlit dispatch set.
 */

void kaze_model_tri_f3_fixedlight_3f0cc(u32 *arg)
{
    register u32 *p asm("$5");
    register u32 mask asm("$6");
    register u32 tag asm("$7");
    u32 *vp;

    p = arg;
    p[1] = *(u32 *)p[2];
    p[0x33] = *(u32 *)(p[2] + 4);
    p[0x34] = *(u32 *)(p[2] + 8);
    p[0x35] = *(u32 *)(p[2] + 0xc);
    p[0x28] = 0;
    if (p[1] == 0) {
        return;
    }

    mask = 0x00ffffff;
    tag = 0x01000000;
loop:
    vp = (u32 *)p[0x34];
    gte_ldv3((void *)vp[0], (void *)vp[1], (void *)vp[2]);
    p[0x34] = (u32)(vp + 3);
    gte_rtpt();
    p[0x28] = p[0x28] + 1;
    gte_nclip();
    gte_stopz(p + 0x2a);
    if ((int)p[0x2a] > 0) {
        gte_avsz3();
        gte_stotz(p + 0x29);
        {
        register u32 semi_flag asm("$3") = 0x02000000;
        if ((int)p[0x29] > 0) {
            if (*(u32 *)p[0x33] & semi_flag) {
                *(u32 *)p[3] = ((u32)((u32 *)p[3] + 2) & mask) | tag;
                *(u32 *)(p[3] + 4) = p[0x23];
                p[0x1e] = 0x22000000;
                p[4] = p[3];
                p[3] = p[3] + 8;
            } else {
                p[0x1e] = 0x20000000;
                p[4] = p[3];
            }
            gte_stsxy3_f3((void *)p[3]);
            if ((p[0x1f] & tag) == 0) {
                gte_ldv0(*(u32 *)p[0x35]);
                gte_ldrgb(p[0x33]);
                gte_nop();
                gte_nop();
                gte_nccs();
            }
            p[0x29] = (int)p[0x29] >> p[0];
            *(u32 *)p[3] = (*(u32 *)(p[0x29] * 4 + p[5]) & mask) | 0x4000000;
            *(u32 *)(p[0x29] * 4 + p[5]) = p[4] & mask;
            if ((p[0x1f] & tag) == 0) {
                gte_strgb(p + 0x2f);
            } else {
                p[0x2f] = *(u32 *)p[0x33];
            }
            *(u32 *)(p[3] + 4) = p[0x1e] | p[0x20] | (p[0x2f] & mask);
            p[3] = p[3] + 0x14;
        }
        }
    }
    p[0x33] = p[0x33] + 4;
    p[0x35] = p[0x35] + 4;
    if (p[0x28] < p[1]) {
        goto loop;
    }
}

/**
 * kaze_tmd_tri_flat_lit_f348  (library, 0x8003f348)
 *
 * TMD primitive worker: rtpt/nclip/avsz3 transforms 3 vertices, lights one
 * normal+color via nccs, emits a flat-triangle packet behind a tpage word
 * (ctx[0x23]) into the OT at ctx[5].
 */

void kaze_tmd_tri_flat_lit_f348(u32 *arg)
{
    register u32 *p asm("$5");
    register u32 tag asm("$7");
    register u32 mask asm("$6");
    u32 *vp;

    p = arg;
    p[1] = *(u32 *)p[2];
    p[0x33] = *(u32 *)(p[2] + 4);
    p[0x34] = *(u32 *)(p[2] + 8);
    p[0x35] = *(u32 *)(p[2] + 0xc);
    p[0x28] = 0;
    if (p[1] == 0) {
        return;
    }

    tag = 0x01000000;
    mask = 0x00ffffff;
loop:
    vp = (u32 *)p[0x34];
    __asm__ volatile(
        "lw $8, 0(%0)\n\t"
        "lw $9, 4(%0)\n\t"
        "lw $10, 8(%0)\n\t"
        "lwc2 $0, 0($8)\n\t"
        "lwc2 $1, 4($8)\n\t"
        "lwc2 $2, 0($9)\n\t"
        "lwc2 $3, 4($9)\n\t"
        "lwc2 $4, 0($10)\n\t"
        "lwc2 $5, 4($10)"
        :: "r"(vp) : "$8", "$9", "$10");
    p[0x34] = (u32)(vp + 3);
    gte_rtpt();
    p[0x28] = p[0x28] + 1;
    __asm__ volatile(
        "cop2 0x1400006\n\t"
        "addiu $2, %0, 0xa8\n\t"
        "swc2 $24, 0($2)"
        :: "r"(p) : "$2", "memory");
    if ((int)p[0x2a] > 0) {
        __asm__ volatile(
            "cop2 0x158002D\n\t"
            "addiu $2, %0, 0xa4\n\t"
            "swc2 $7, 0($2)"
            :: "r"(p) : "$2", "memory");
        if ((int)p[0x29] > 0) {
            __asm__ volatile(
                "lw $2, 12(%0)\n\t"
                "nop\n\t"
                "addiu $2, $2, 8\n\t"
                "swc2 $12, 8($2)\n\t"
                "swc2 $13, 12($2)\n\t"
                "swc2 $14, 16($2)"
                :: "r"(p) : "$2", "memory");
            if ((p[0x1f] & tag) == 0) {
                __asm__ volatile(
                    "lw $2, 0xd4(%0)\n\t"
                    "nop\n\t"
                    "lw $2, 0($2)\n\t"
                    "nop\n\t"
                    "lwc2 $0, 0($2)\n\t"
                    "lwc2 $1, 4($2)\n\t"
                    "lw $8, 0xcc(%0)\n\t"
                    "nop\n\t"
                    "lwc2 $6, 0($8)\n\t"
                    "nop\n\t"
                    "nop\n\t"
                    "cop2 0x108041B"
                    :: "r"(p) : "$2", "$8");
            }
            {
                p[0x29] = (int)p[0x29] >> p[0];
                ((u32 *)p[3])[0] = ((p[3] + 8) & mask) | tag;
                ((u32 *)p[3])[1] = p[0x23];
                ((u32 *)p[3])[2] = ((u32 *)p[5])[p[0x29]] & mask | 0x04000000;
                ((u32 *)p[5])[p[0x29]] = p[3] & mask;
            }
            if ((p[0x1f] & tag) == 0) {
                u32 *otz = &p[0x2f];
                __asm__ volatile("swc2 $22, 0(%0)" :: "r"(otz) : "memory");
            } else {
                p[0x2f] = **(u32 **)&p[0x33];
            }
            {
                register u32 color __asm__("$3");
                color = (p[0x2f] & mask) | 0x22000000;
                ((u32 *)p[3])[3] = p[0x20] | color;
                p[3] = p[3] + 0x1c;
            }
        }
    }
    p[0x33] = p[0x33] + 4;
    p[0x35] = p[0x35] + 4;
    if (p[0x28] < p[1]) {
        goto loop;
    }
}

#define gte_rtpt() __asm__ volatile("cop2 0x280030")
#define gte_ncct() __asm__ volatile("cop2 0x118043F")
#define gte_ldv3(x0, x1, x2)                                                   \
    __asm__ volatile("lwc2 $0, 0(%0)\n\t"                                      \
                     "lwc2 $1, 4(%0)\n\t"                                      \
                     "lwc2 $2, 0(%1)\n\t"                                      \
                     "lwc2 $3, 4(%1)\n\t"                                      \
                     "lwc2 $4, 0(%2)\n\t"                                      \
                     "lwc2 $5, 4(%2)"                                          \
                     :: "r"(x0), "r"(x1), "r"(x2))
#define gte_strgb3(x0, x1, x2)                                                 \
    __asm__ volatile("swc2 $20, 0(%0)\n\t"                                     \
                     "swc2 $21, 0(%1)\n\t"                                     \
                     "swc2 $22, 0(%2)"                                         \
                     :: "r"(x0), "r"(x1), "r"(x2) : "memory")
#define gte_stsxy3_g3(x)                                                       \
    __asm__ volatile("swc2 $12, 8(%0)\n\t"                                     \
                     "swc2 $13, 16(%0)\n\t"                                    \
                     "swc2 $14, 24(%0)"                                        \
                     :: "r"(x) : "memory")

/**
 * kaze_tmd_tri_gouraud_lit_f580  (library, 0x8003f580)
 *
 * TMD worker for lit gouraud triangles: 3 normals via ncct, emits POLY_G3
 * (0x30, or 0x32 plus tpage preamble when the color word has semi-trans bit
 * 0x2000000) into the OT.
 */

void kaze_tmd_tri_gouraud_lit_f580(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 m24;
    u32 m1;
    u32 idx;
    u32 c;
    u32 *r0;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        m1 = 0x1000000;
        r0 = ctx + 0x2f;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            if ((int)ctx[0x2a] < 1) {
                ctx[0x35] = ctx[0x35] + 0xc;
            } else {
                gte_avsz3();
                gte_stotz(ctx + 0x29);
                if ((int)ctx[0x29] < 1) {
                    ctx[0x35] = ctx[0x35] + 0xc;
                } else {
                    ctx[4] = ctx[3];
                    if ((*(u32 *)ctx[0x33] & 0x2000000) != 0) {
                        *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1;
                        *(u32 *)(ctx[3] + 4) = ctx[0x23];
                        ctx[0x1e] = 0x32000000;
                        ctx[3] = ctx[3] + 8;
                    } else {
                        ctx[0x1e] = 0x30000000;
                    }
                    gte_stsxy3_g3(ctx[3]);
                    if ((ctx[0x1f] & m1) == 0) {
                        p = (u32 *)ctx[0x35];
                        gte_ldv3(p[0], p[1], p[2]);
                        gte_ldrgb(ctx[0x33]);
                        ctx[0x35] = (u32)(p + 3);
                        gte_ncct();
                    }
                    idx = (int)ctx[0x29] >> ctx[0];
                    ctx[0x29] = idx;
                    *(u32 *)ctx[3] = (*(u32 *)(idx * 4 + ctx[5]) & m24) | 0x6000000;
                    *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                    if ((ctx[0x1f] & m1) == 0) {
                        gte_strgb3(r0, ctx + 0x30, ctx + 0x31);
                    } else {
                        c = *(u32 *)ctx[0x33];
                        ctx[0x35] = ctx[0x35] + 0xc;
                        ctx[0x31] = c;
                        ctx[0x30] = c;
                        ctx[0x2f] = c;
                    }
                    *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | (ctx[0x2f] & m24);
                    *(u32 *)(ctx[3] + 0xc) = ctx[0x30];
                    *(u32 *)(ctx[3] + 0x14) = ctx[0x31];
                    ctx[3] = ctx[3] + 0x1c;
                }
            }
            ctx[0x33] = ctx[0x33] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_tmd_tri_gouraud_lit_tpage_f848  (library, 0x8003f848)
 *
 * TMD worker for lit gouraud triangles with unconditional tpage/DR_MODE
 * preamble word: 3 normals via ncct, emits POLY_G3 code-0x32 packets into
 * the OT.
 */

void kaze_tmd_tri_gouraud_lit_tpage_f848(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 m24;
    u32 m1;
    u32 c;
    u32 h;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t;
    if (ctx[1] != 0) {
        m1 = 0x1000000;
        m24 = 0xffffff;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            if ((int)ctx[0x2a] < 1) {
                if (m1) {
                    ctx[0x35] = ctx[0x35] + 0xc;
                } else {
                    ctx[0x35] = ctx[0x35] + 0xc;
                }
            } else {
                gte_avsz3();
                gte_stotz(ctx + 0x29);
                if ((int)ctx[0x29] < 1) {
                    ctx[0x35] = ctx[0x35] + 0xc;
                } else {
                    gte_stsxy3_g3(ctx[3] + 8);
                    if ((ctx[0x1f] & m1) == 0) {
                        p = (u32 *)ctx[0x35];
                        gte_ldv3(p[0], p[1], p[2]);
                        gte_ldrgb(ctx[0x33]);
                        ctx[0x35] = (u32)(p + 3);
                        gte_ncct();
                    }
                    ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                    *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1;
                    *(u32 *)(ctx[3] + 4) = ctx[0x23];
                    *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x6000000;
                    *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                    if ((ctx[0x1f] & m1) == 0) {
                        gte_strgb3(ctx + 0x2f, ctx + 0x30, ctx + 0x31);
                    } else {
                        c = *(u32 *)ctx[0x33];
                        ctx[0x35] = ctx[0x35] + 0xc;
                        ctx[0x31] = c;
                        ctx[0x30] = c;
                        ctx[0x2f] = c;
                    }
                    h = (ctx[0x2f] & m24) | 0x32000000;
                    *(u32 *)(ctx[3] + 0xc) = ctx[0x20] | h;
                    *(u32 *)(ctx[3] + 0x14) = ctx[0x30];
                    *(u32 *)(ctx[3] + 0x1c) = ctx[0x31];
                    ctx[3] = ctx[3] + 0x24;
                }
            }
            ctx[0x33] = ctx[0x33] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

#define gte_ldv0(x)                                                            \
    __asm__ volatile("lwc2 $0, 0(%0)\n\t"                                      \
                     "lwc2 $1, 4(%0)"                                          \
                     :: "r"(x))

/**
 * kaze_tmd_tri_grad_1normal_fad8  (library, 0x8003fad8)
 *
 * TMD worker for gradation triangles: one shared normal, three per-vertex
 * colors lit by nccs, conditional semi-trans preamble, emits POLY_G3
 * 0x30/0x32 packets.
 */

void kaze_tmd_tri_grad_1normal_fad8(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 m24;
    u32 m1;
    u32 idx;
    u32 c;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        m1 = 0x1000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            if ((int)ctx[0x2a] < 1) {
                ctx[0x35] = ctx[0x35] + 4;
            } else {
                gte_avsz3();
                gte_stotz(ctx + 0x29);
                if ((int)ctx[0x29] < 1) {
                    ctx[0x35] = ctx[0x35] + 4;
                } else {
                    ctx[4] = ctx[3];
                    if ((*(u32 *)ctx[0x33] & 0x2000000) != 0) {
                        *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1;
                        *(u32 *)(ctx[3] + 4) = ctx[0x23];
                        ctx[0x1e] = 0x32000000;
                        ctx[3] = ctx[3] + 8;
                    } else {
                        ctx[0x1e] = 0x30000000;
                    }
                    gte_stsxy3_g3(ctx[3]);
                    if ((ctx[0x1f] & m1) == 0) {
                        p = (u32 *)ctx[0x35];
                        gte_ldv0(*p);
                        ctx[0x35] = (u32)(p + 1);
                    }
                    idx = (int)ctx[0x29] >> ctx[0];
                    ctx[0x29] = idx;
                    *(u32 *)ctx[3] = (*(u32 *)(idx * 4 + ctx[5]) & m24) | 0x6000000;
                    *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                    if ((ctx[0x1f] & m1) == 0) {
                        gte_ldrgb(ctx[0x33]);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x2f);
                        gte_ldrgb(ctx[0x33] + 4);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x30);
                        gte_ldrgb(ctx[0x33] + 8);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x31);
                    } else {
                        ctx[0x2f] = *(u32 *)ctx[0x33];
                        ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                        c = *(u32 *)(ctx[0x33] + 8);
                        ctx[0x35] = ctx[0x35] + 4;
                        ctx[0x31] = c;
                    }
                    *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | (ctx[0x2f] & m24);
                    *(u32 *)(ctx[3] + 0xc) = ctx[0x30];
                    *(u32 *)(ctx[3] + 0x14) = ctx[0x31];
                    ctx[3] = ctx[3] + 0x1c;
                }
            }
            ctx[0x33] = ctx[0x33] + 0xc;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_tmd_tri_grad_1normal_tpage_fde4  (library, 0x8003fde4)
 *
 * TMD worker for gradation triangles with fixed tpage preamble: one normal,
 * three colors via nccs, emits POLY_G3 code-0x32 packets into the OT.
 */

void kaze_tmd_tri_grad_1normal_tpage_fde4(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 m24;
    u32 m1;
    u32 c;
    u32 h;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t;
    if (ctx[1] != 0) {
        m1 = 0x1000000;
        m24 = 0xffffff;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            if ((int)ctx[0x2a] < 1) {
                ctx[0x35] = ctx[0x35] + 4;
            } else {
                gte_avsz3();
                gte_stotz(ctx + 0x29);
                if ((int)ctx[0x29] < 1) {
                    ctx[0x35] = ctx[0x35] + 4;
                } else {
                    gte_stsxy3_g3(ctx[3] + 8);
                    if ((ctx[0x1f] & m1) == 0) {
                        p = (u32 *)ctx[0x35];
                        gte_ldv0(*p);
                        ctx[0x35] = (u32)(p + 1);
                    }
                    ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                    *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1;
                    *(u32 *)(ctx[3] + 4) = ctx[0x23];
                    *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x6000000;
                    *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                    if ((ctx[0x1f] & m1) == 0) {
                        gte_ldrgb(ctx[0x33]);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x2f);
                        gte_ldrgb(ctx[0x33] + 4);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x30);
                        gte_ldrgb(ctx[0x33] + 8);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x31);
                    } else {
                        ctx[0x2f] = *(u32 *)ctx[0x33];
                        ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                        c = *(u32 *)(ctx[0x33] + 8);
                        ctx[0x35] = ctx[0x35] + 4;
                        ctx[0x31] = c;
                    }
                    h = (ctx[0x2f] & m24) | 0x32000000;
                    *(u32 *)(ctx[3] + 0xc) = ctx[0x20] | h;
                    *(u32 *)(ctx[3] + 0x14) = ctx[0x30];
                    *(u32 *)(ctx[3] + 0x1c) = ctx[0x31];
                    ctx[3] = ctx[3] + 0x24;
                }
            }
            ctx[0x33] = ctx[0x33] + 0xc;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_tmd_tri_gouraud_3normal_00b8  (library, 0x800400b8)
 *
 * TMD worker for triangles with three separate normal pointers and three
 * colors (nccs per vertex), conditional semi-trans preamble, emits POLY_G3
 * 0x30/0x32.
 */

void kaze_tmd_tri_gouraud_3normal_00b8(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 m24;
    u32 m1;
    u32 idx;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        m1 = 0x1000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            if ((int)ctx[0x2a] < 1) {
                ctx[0x35] = ctx[0x35] + 0xc;
            } else {
                gte_avsz3();
                gte_stotz(ctx + 0x29);
                if ((int)ctx[0x29] < 1) {
                    ctx[0x35] = ctx[0x35] + 0xc;
                } else {
                    ctx[4] = ctx[3];
                    if ((*(u32 *)ctx[0x33] & 0x2000000) != 0) {
                        *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1;
                        *(u32 *)(ctx[3] + 4) = ctx[0x23];
                        ctx[0x1e] = 0x32000000;
                        ctx[3] = ctx[3] + 8;
                    } else {
                        ctx[0x1e] = 0x30000000;
                    }
                    gte_stsxy3_g3(ctx[3]);
                    idx = (int)ctx[0x29] >> ctx[0];
                    ctx[0x29] = idx;
                    *(u32 *)ctx[3] = (*(u32 *)(idx * 4 + ctx[5]) & m24) | 0x6000000;
                    *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                    if ((ctx[0x1f] & m1) == 0) {
                        gte_ldv0(*(u32 *)ctx[0x35]);
                        gte_ldrgb(ctx[0x33]);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x2f);
                        gte_ldv0(*(u32 *)(ctx[0x35] + 4));
                        gte_ldrgb(ctx[0x33] + 4);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x30);
                        gte_ldv0(*(u32 *)(ctx[0x35] + 8));
                        gte_ldrgb(ctx[0x33] + 8);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x31);
                    } else {
                        ctx[0x2f] = *(u32 *)ctx[0x33];
                        ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                        ctx[0x31] = *(u32 *)(ctx[0x33] + 8);
                    }
                    ctx[0x35] = ctx[0x35] + 0xc;
                    *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | (ctx[0x2f] & m24);
                    *(u32 *)(ctx[3] + 0xc) = ctx[0x30];
                    *(u32 *)(ctx[3] + 0x14) = ctx[0x31];
                    ctx[3] = ctx[3] + 0x1c;
                }
            }
            ctx[0x33] = ctx[0x33] + 0xc;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_tmd_tri_gouraud_3normal_tpage_03dc  (library, 0x800403dc)
 *
 * TMD worker for triangles with three normal pointers and three colors,
 * fixed tpage preamble, emits POLY_G3 code-0x32 packets into the OT.
 */

void kaze_tmd_tri_gouraud_3normal_tpage_03dc(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 m24;
    u32 m1;
    u32 h;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        m1 = 0x1000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            if ((int)ctx[0x2a] < 1) {
                ctx[0x35] = ctx[0x35] + 0xc;
            } else {
                gte_avsz3();
                gte_stotz(ctx + 0x29);
                if ((int)ctx[0x29] < 1) {
                    ctx[0x35] = ctx[0x35] + 0xc;
                } else {
                    gte_stsxy3_g3(ctx[3] + 8);
                    ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                    *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1;
                    *(u32 *)(ctx[3] + 4) = ctx[0x23];
                    *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x6000000;
                    *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                    if ((ctx[0x1f] & m1) == 0) {
                        gte_ldv0(*(u32 *)ctx[0x35]);
                        gte_ldrgb(ctx[0x33]);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x2f);
                        gte_ldv0(*(u32 *)(ctx[0x35] + 4));
                        gte_ldrgb(ctx[0x33] + 4);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x30);
                        gte_ldv0(*(u32 *)(ctx[0x35] + 8));
                        gte_ldrgb(ctx[0x33] + 8);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x31);
                    } else {
                        ctx[0x2f] = *(u32 *)ctx[0x33];
                        ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                        ctx[0x31] = *(u32 *)(ctx[0x33] + 8);
                    }
                    ctx[0x35] = ctx[0x35] + 0xc;
                    h = (ctx[0x2f] & m24) | 0x32000000;
                    *(u32 *)(ctx[3] + 0xc) = ctx[0x20] | h;
                    *(u32 *)(ctx[3] + 0x14) = ctx[0x30];
                    *(u32 *)(ctx[3] + 0x1c) = ctx[0x31];
                    ctx[3] = ctx[3] + 0x24;
                }
            }
            ctx[0x33] = ctx[0x33] + 0xc;
        } while (ctx[0x28] < ctx[1]);
    }
}

#define gte_avsz4()                                                            \
    __asm__ volatile("nop\n\t"                                                 \
                     "nop\n\t"                                                 \
                     "cop2 0x168002E")
#undef gte_ldv0
#define gte_ldv0(v0)                                                           \
    __asm__ volatile("lwc2 $0, 0(%0)\n\t"                                      \
                     "lwc2 $1, 4(%0)"                                          \
                     :: "r"(v0))
#define gte_ldnrm(nrm)                                                         \
    __asm__ volatile("lwc2 $0, 0(%0)\n\t"                                      \
                     "lwc2 $1, 4(%0)"                                          \
                     :: "r"(nrm))
#undef gte_nccs
#define gte_nccs(col)                                                          \
    __asm__ volatile("lwc2 $6, 0(%0)\n\t"                                      \
                     "nop\n\t"                                                 \
                     "nop\n\t"                                                 \
                     "cop2 0x108041B"                                          \
                     :: "r"(col))
#define gte_stlvl(x) __asm__ volatile("swc2 $22, 0(%0)" :: "r"(x) : "memory")
#define gte_stsxy2(x) __asm__ volatile("swc2 $14, 0(%0)" :: "r"(x) : "memory")
#define gte_stsxy3_f3(x)                                                       \
    __asm__ volatile("swc2 $12, 8(%0)\n\t"                                     \
                     "swc2 $13, 12(%0)\n\t"                                    \
                     "swc2 $14, 16(%0)"                                        \
                     :: "r"(x) : "memory")

/**
 * kaze_tmd_quad_flat_lit_06c8  (library, 0x800406c8)
 *
 * TMD worker for flat quads: rtpt+rtps/avsz4 transforms 4 vertices, one
 * normal+color via nccs (default 0x808080 when prelit), emits POLY_F4
 * code-0x28 packets into the OT.
 */

void kaze_tmd_quad_flat_lit_06c8(u32 *arg)
{
    register u32 *p asm("$5");
    register u32 tag asm("$7");
    register u32 mask asm("$6");
    u32 *vp;
    u32 acc;

    p = arg;
    p[1] = *(u32 *)p[2];
    p[0x33] = *(u32 *)(p[2] + 4);
    p[0x34] = *(u32 *)(p[2] + 8);
    p[0x35] = *(u32 *)(p[2] + 0xc);
    p[0x28] = 0;
    if (p[1] == 0) {
        return;
    }

    tag = 0x01000000;
    mask = 0x00ffffff;
loop:
    vp = (u32 *)p[0x34];
    gte_ldv3((void *)vp[0], (void *)vp[1], (void *)vp[2]);
    p[0x34] = (u32)(vp + 3);
    gte_rtpt();
    p[0x28] = p[0x28] + 1;
    gte_nclip();
    gte_stopz(p + 0x2a);
    gte_stsxy3_f3((void *)p[3]);
    vp = (u32 *)p[0x34];
    gte_ldv0((void *)vp[0]);
    p[0x34] = (u32)(vp + 1);
    gte_rtps();
    gte_stsxy2((void *)(p[3] + 0x14));
    gte_avsz4();
    gte_stotz(p + 0x29);
    if ((int)p[0x29] > 0) {
        gte_nclip();
        gte_stopz(p + 0x2b);
        if ((int)p[0x2a] > 0 || (int)p[0x2b] <= 0) {
            if ((p[0x1f] & tag) == 0) {
                gte_ldnrm((u32 *)*(u32 *)p[0x35]);
                gte_nccs(p[0x33]);
            }
            p[0x29] = (int)p[0x29] >> p[0];
            *(u32 *)p[3] = (*(u32 *)(p[0x29] * 4 + p[5]) & mask) | 0x5000000;
            *(u32 *)(p[0x29] * 4 + p[5]) = p[3] & mask;
            if ((p[0x1f] & tag) == 0) {
                gte_stlvl(p + 0x2f);
                p[0x2f] = p[0x2f] & mask;
            } else {
                p[0x2f] = 0x00808080;
            }
            acc = p[0x2f] | 0x28000000;
            *(u32 *)(p[3] + 4) = p[0x20] | acc;
            p[3] = p[3] + 0x18;
        }
    }
    p[0x33] = p[0x33] + 4;
    p[0x35] = p[0x35] + 4;
    if (p[0x28] < p[1]) {
        goto loop;
    }
}
