#include "types.h"
#include "kaze_syms.h"
typedef struct { char c[8]; } S8;

#define gte_rtpt() __asm__ volatile("cop2 0x280030")
#define gte_rtps() __asm__ volatile("cop2 0x180001")
#define gte_nclip() __asm__ volatile("cop2 0x1400006")
#define gte_avsz4() __asm__ volatile("cop2 0x168002E")
#define gte_nccs() __asm__ volatile("cop2 0x108041B")
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
#define gte_ldrgb(x) __asm__ volatile("lwc2 $6, 0(%0)" :: "r"(x))
#define gte_strgb(x) __asm__ volatile("swc2 $22, 0(%0)" :: "r"(x) : "memory")
#define gte_stopz(x) __asm__ volatile("swc2 $24, 0(%0)" :: "r"(x) : "memory")
#define gte_stotz(x) __asm__ volatile("swc2 $7, 0(%0)" :: "r"(x) : "memory")
#define gte_stsxy(x) __asm__ volatile("swc2 $14, 0(%0)" :: "r"(x) : "memory")
#define gte_stsxy3_f3(x)                                                       \
    __asm__ volatile("swc2 $12, 8(%0)\n\t"                                     \
                     "swc2 $13, 12(%0)\n\t"                                    \
                     "swc2 $14, 16(%0)"                                        \
                     :: "r"(x) : "memory")

/**
 * kaze_tmd_quad_flat_lit_tpage_0924  (library, 0x80040924)
 *
 * Flat-quad TMD worker with tpage preamble word: 4 vertices, one
 * normal+color via nccs, emits POLY_F4 code-0x2a (semi-trans) packets into
 * the OT.
 */

void kaze_tmd_quad_flat_lit_tpage_0924(u32 *arg)
{
    register u32 *p asm("$5");
    register u32 tag asm("$7");
    register u32 mask asm("$6");
    u32 *vp;
    u32 h;

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
    gte_stsxy3_f3((void *)(p[3] + 8));
    vp = (u32 *)p[0x34];
    gte_ldv0((void *)vp[0]);
    p[0x34] = (u32)(vp + 1);
    gte_rtps();
    gte_stsxy((void *)(p[3] + 0x1c));
    __asm__ volatile("nop");
    __asm__ volatile("nop");
    gte_avsz4();
    gte_stotz(p + 0x29);
    if ((int)p[0x29] > 0) {
        gte_nclip();
        gte_stopz(p + 0x2b);
        if (((int)p[0x2a] > 0) || ((int)p[0x2b] <= 0)) {
            if ((p[0x1f] & tag) == 0) {
                gte_ldv0(*(u32 *)p[0x35]);
                gte_ldrgb(p[0x33]);
                __asm__ volatile("nop");
                __asm__ volatile("nop");
                gte_nccs();
            }
            p[0x29] = (int)p[0x29] >> p[0];
            *(u32 *)p[3] = ((u32)((u32 *)p[3] + 2) & mask) | tag;
            *(u32 *)(p[3] + 4) = p[0x23];
            *(u32 *)(p[3] + 8) = (*(u32 *)(p[0x29] * 4 + p[5]) & mask) | 0x5000000;
            *(u32 *)(p[0x29] * 4 + p[5]) = p[3] & mask;
            if ((p[0x1f] & tag) == 0) {
                gte_strgb(p + 0x2f);
                p[0x2f] = p[0x2f] & mask;
            } else {
                p[0x2f] = 0x808080;
            }
            h = p[0x2f] | 0x2a000000;
            *(u32 *)(p[3] + 0xc) = p[0x20] | h;
            p[3] = p[3] + 0x20;
        }
    }
    p[0x33] = p[0x33] + 4;
    p[0x35] = p[0x35] + 4;
    if (p[0x28] < p[1]) {
        goto loop;
    }
}

#define gte_ncct() __asm__ volatile("cop2 0x118043F")
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
 * kaze_tmd_quad_gouraud_lit_0bac  (library, 0x80040bac)
 *
 * Lit gouraud-quad TMD worker: 3+1 normals (ncct then nccs), conditional
 * semi-trans tpage preamble, emits POLY_G4 0x38/0x3a packets into the OT.
 */

void kaze_tmd_quad_gouraud_lit_0bac(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 c;
    u32 m24;
    u32 m1;

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
            if ((*(u32 *)ctx[0x33] & 0x2000000) != 0) {
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                ctx[0x1e] = 0x3a000000;
                ctx[4] = ctx[3];
                ctx[3] = ctx[3] + 8;
            } else {
                ctx[0x1e] = 0x38000000;
                ctx[4] = ctx[3];
            }
            gte_stopz(ctx + 0x2a);
            gte_stsxy3_g3(ctx[3]);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x20);
            __asm__ volatile("nop");
            __asm__ volatile("nop");
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
                ctx[3] = ctx[4];
            } else {
                gte_nclip();
                gte_stopz(ctx + 0x2b);
                if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                    ctx[3] = ctx[4];
                } else {
                    if ((ctx[0x1f] & m1) == 0) {
                        p = (u32 *)ctx[0x35];
                        gte_ldv3(p[0], p[1], p[2]);
                        gte_ldrgb(ctx[0x33]);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_ncct();
                    }
                    ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                    *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x8000000;
                    *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                    if ((ctx[0x1f] & m1) == 0) {
                        gte_strgb3(ctx + 0x2f, ctx + 0x30, ctx + 0x31);
                        gte_ldv0(*(u32 *)(ctx[0x35] + 0xc));
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x32);
                    } else {
                        c = *(u32 *)ctx[0x33];
                        ctx[0x32] = c;
                        ctx[0x31] = c;
                        ctx[0x30] = c;
                        ctx[0x2f] = c;
                    }
                    *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | (ctx[0x2f] & m24);
                    *(u32 *)(ctx[3] + 0xc) = ctx[0x30];
                    *(u32 *)(ctx[3] + 0x14) = ctx[0x31];
                    *(u32 *)(ctx[3] + 0x1c) = ctx[0x32];
                    ctx[3] = ctx[3] + 0x24;
                }
            }
            ctx[0x33] = ctx[0x33] + 4;
            ctx[0x35] = ctx[0x35] + 0x10;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_tmd_quad_gouraud_lit_tpage_0f24  (library, 0x80040f24)
 *
 * Lit gouraud-quad TMD worker with fixed tpage preamble: 3+1 normals
 * (ncct+nccs), emits POLY_G4 code-0x3a packets into the OT.
 */

void kaze_tmd_quad_gouraud_lit_tpage_0f24(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 c;
    u32 h;
    u32 m24;
    u32 m1;

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
            gte_stsxy3_g3(ctx[3] + 8);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x28);
            __asm__ volatile("nop");
            __asm__ volatile("nop");
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                gte_nclip();
                gte_stopz(ctx + 0x2b);
                if ((0 < (int)ctx[0x2a]) || ((int)ctx[0x2b] < 1)) {
                    if ((ctx[0x1f] & m1) == 0) {
                        p = (u32 *)ctx[0x35];
                        gte_ldv3(p[0], p[1], p[2]);
                        gte_ldrgb(ctx[0x33]);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_ncct();
                    }
                    ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                    *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1;
                    *(u32 *)(ctx[3] + 4) = ctx[0x23];
                    *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x8000000;
                    *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                    if ((ctx[0x1f] & m1) == 0) {
                        gte_strgb3(ctx + 0x2f, ctx + 0x30, ctx + 0x31);
                        gte_ldv0(*(u32 *)(ctx[0x35] + 0xc));
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx + 0x32);
                    } else {
                        c = *(u32 *)ctx[0x33];
                        ctx[0x32] = c;
                        ctx[0x31] = c;
                        ctx[0x30] = c;
                        ctx[0x2f] = c;
                    }
                    h = (ctx[0x2f] & m24) | 0x3a000000;
                    *(u32 *)(ctx[3] + 0xc) = ctx[0x20] | h;
                    *(u32 *)(ctx[3] + 0x14) = ctx[0x30];
                    *(u32 *)(ctx[3] + 0x1c) = ctx[0x31];
                    *(u32 *)(ctx[3] + 0x24) = ctx[0x32];
                    ctx[3] = ctx[3] + 0x2c;
                }
            }
            ctx[0x33] = ctx[0x33] + 4;
            ctx[0x35] = ctx[0x35] + 0x10;
        } while (ctx[0x28] < ctx[1]);
    }
}

#define gte_ldv3_table(vp) __asm__ volatile( \
    "lw $8, 0(%0)\n\t" \
    "lw $9, 4(%0)\n\t" \
    "lw $10, 8(%0)\n\t" \
    "lwc2 $0, 0($8)\n\t" \
    "lwc2 $1, 4($8)\n\t" \
    "lwc2 $2, 0($9)\n\t" \
    "lwc2 $3, 4($9)\n\t" \
    "lwc2 $4, 0($10)\n\t" \
    "lwc2 $5, 4($10)" \
    :: "r"(vp) : "$8", "$9", "$10")
#define gte_ldv0_table1(vp) __asm__ volatile( \
    "lw $9, 0(%0)\n\t" \
    "nop\n\t" \
    "lwc2 $0, 0($9)\n\t" \
    "lwc2 $1, 4($9)" \
    :: "r"(vp) : "$9")
#undef gte_ldv0
#define gte_ldv0(v) __asm__ volatile( \
    "lwc2 $0, 0(%0)\n\t" \
    "lwc2 $1, 4(%0)" \
    :: "r"(v))
#undef gte_ldrgb
#define gte_ldrgb(pp) __asm__ volatile("lwc2 $6, 0(%0)" :: "r"(pp))
#define gte_ldrgb0(base) __asm__ volatile( \
    "lw $10, 0xcc(%0)\n\t" \
    "nop\n\t" \
    "lwc2 $6, 0($10)" \
    :: "r"(base) : "$10")
#undef gte_stopz
#define gte_stopz(pp) __asm__ volatile("swc2 $24, 0(%0)" :: "r"(pp) : "memory")
#undef gte_stotz
#define gte_stotz(pp) __asm__ volatile("swc2 $7, 0(%0)" :: "r"(pp) : "memory")
#undef gte_strgb
#define gte_strgb(pp) __asm__ volatile("swc2 $22, 0(%0)" :: "r"(pp) : "memory")
#undef gte_stsxy
#define gte_stsxy(pp) __asm__ volatile("swc2 $14, 0(%0)" :: "r"(pp) : "memory")
#define gte_stsxy3(base) __asm__ volatile( \
    "lw $8, 0xc(%0)\n\t" \
    "nop\n\t" \
    "swc2 $12, 8($8)\n\t" \
    "swc2 $13, 16($8)\n\t" \
    "swc2 $14, 24($8)" \
    :: "r"(base) : "$8", "memory")

/**
 * kaze_tmd_quad_grad_1normal_1240  (library, 0x80041240)
 *
 * Gradation-quad TMD worker: one normal, four per-vertex colors via nccs,
 * conditional semi-trans preamble, emits POLY_G4 0x38/0x3a packets.
 */

void kaze_tmd_quad_grad_1normal_1240(uint *p)
{
    uint *vp;
    uint otz_idx;

    p[1] = *(uint *)p[2];
    p[0x33] = *(uint *)(p[2] + 4);
    p[0x34] = *(uint *)(p[2] + 8);
    p[0x35] = *(uint *)(p[2] + 0xc);
    p[0x28] = 0;
    if (p[1] == 0) {
        return;
    }
    do {
        vp = (uint *)p[0x34];
        gte_ldv3_table(vp);
        p[0x34] = (uint)(vp + 3);
        gte_rtpt();
        p[0x28] = p[0x28] + 1;
        gte_nclip();
        p[4] = p[3];
        if ((*(uint *)p[0x33] & 0x2000000) != 0) {
            *(uint *)p[3] = ((uint)((uint *)p[3] + 2) & 0xffffff) | 0x1000000;
            *(uint *)(p[3] + 4) = p[0x23];
            p[0x1e] = 0x3a000000;
            p[3] = p[3] + 8;
        } else {
            p[0x1e] = 0x38000000;
        }
        gte_stopz(p + 0x2a);
        gte_stsxy3(p);
        vp = (uint *)p[0x34];
        gte_ldv0_table1(vp);
        p[0x34] = (uint)(vp + 1);
        gte_rtps();
        gte_stsxy(p[3] + 0x20);
        __asm__ volatile("nop");
        __asm__ volatile("nop");
        gte_avsz4();
        gte_stotz(p + 0x29);
        if ((int)p[0x29] < 1) {
            p[3] = p[4];
        } else {
            gte_nclip();
            gte_stopz(p + 0x2b);
            if ((int)p[0x2a] < 1 && 0 < (int)p[0x2b]) {
                p[3] = p[4];
            } else {
                otz_idx = (int)p[0x29] >> p[0];
                p[0x29] = otz_idx;
                *(uint *)p[3] = (*(uint *)(otz_idx * 4 + p[5]) & 0xffffff) | 0x8000000;
                *(uint *)(p[0x29] * 4 + p[5]) = p[4] & 0xffffff;
                if ((p[0x1f] & 0x1000000) == 0) {
                    gte_ldv0((void *)*(uint *)p[0x35]);
                    gte_ldrgb0(p);
                    __asm__ volatile("nop");
                    __asm__ volatile("nop");
                    gte_nccs();
                    gte_strgb(p + 0x2f);
                    gte_ldrgb((void *)(p[0x33] + 4));
                    __asm__ volatile("nop");
                    __asm__ volatile("nop");
                    gte_nccs();
                    gte_strgb(p[3] + 0xc);
                    gte_ldrgb((void *)(p[0x33] + 8));
                    __asm__ volatile("nop");
                    __asm__ volatile("nop");
                    gte_nccs();
                    gte_strgb(p[3] + 0x14);
                    gte_ldrgb((void *)(p[0x33] + 0xc));
                    __asm__ volatile("nop");
                    __asm__ volatile("nop");
                    gte_nccs();
                    gte_strgb(p[3] + 0x1c);
                } else {
                    p[0x2f] = *(uint *)p[0x33];
                    *(uint *)(p[3] + 0xc) = *(uint *)(p[0x33] + 4);
                    *(uint *)(p[3] + 0x14) = *(uint *)(p[0x33] + 8);
                    *(uint *)(p[3] + 0x1c) = *(uint *)(p[0x33] + 0xc);
                }
                *(uint *)(p[3] + 4) = p[0x1e] | p[0x20] | (p[0x2f] & 0xffffff);
                p[3] = p[3] + 0x24;
            }
        }
        p[0x33] = p[0x33] + 0x10;
        p[0x35] = p[0x35] + 4;
    } while (p[0x28] < p[1]);
}

#undef gte_ldv0_table1
#define gte_ldv0_table1(vp) __asm__ volatile( \
    "lw $8, 0(%0)\n\t" \
    "nop\n\t" \
    "lwc2 $0, 0($8)\n\t" \
    "lwc2 $1, 4($8)" \
    :: "r"(vp) : "$8")
#undef gte_ldv0
#define gte_ldv0(v) __asm__ volatile( \
    "lwc2 $0, 0(%0)\n\t" \
    "lwc2 $1, 4(%0)" \
    :: "r"(v))
#undef gte_ldrgb
#define gte_ldrgb(pp) __asm__ volatile("lwc2 $6, 0(%0)" :: "r"(pp))
#undef gte_ldrgb0
#define gte_ldrgb0(base) __asm__ volatile( \
    "lw $9, 0xcc(%0)\n\t" \
    "nop\n\t" \
    "lwc2 $6, 0($9)" \
    :: "r"(base) : "$9")
#undef gte_stopz
#define gte_stopz(pp) __asm__ volatile("swc2 $24, 0(%0)" :: "r"(pp) : "memory")
#undef gte_stotz
#define gte_stotz(pp) __asm__ volatile("swc2 $7, 0(%0)" :: "r"(pp) : "memory")
#undef gte_strgb
#define gte_strgb(pp) __asm__ volatile("swc2 $22, 0(%0)" :: "r"(pp) : "memory")
#undef gte_stsxy
#define gte_stsxy(pp) __asm__ volatile("swc2 $14, 0(%0)" :: "r"(pp) : "memory")
#undef gte_stsxy3
#define gte_stsxy3(base) __asm__ volatile( \
    "lw $2, 0xc(%0)\n\t" \
    "nop\n\t" \
    "addiu $2, $2, 8\n\t" \
    "swc2 $12, 8($2)\n\t" \
    "swc2 $13, 16($2)\n\t" \
    "swc2 $14, 24($2)" \
    :: "r"(base) : "$2", "memory")

/**
 * kaze_tmd_quad_grad_1normal_tpage_15d0  (library, 0x800415d0)
 *
 * Gradation-quad TMD worker with fixed tpage preamble: one normal, four
 * colors via nccs, emits POLY_G4 code-0x3a packets into the OT.
 */

void kaze_tmd_quad_grad_1normal_tpage_15d0(uint *p)
{
    uint *vp;
    uint mask;
    uint tag;
    uint code;

    p[1] = *(uint *)p[2];
    p[0x33] = *(uint *)(p[2] + 4);
    p[0x34] = *(uint *)(p[2] + 8);
    p[0x35] = *(uint *)(p[2] + 0xc);
    p[0x28] = 0;
    if (p[1] == 0) {
        return;
    }
    mask = 0xffffff;
    tag = 0x1000000;
    do {
        vp = (uint *)p[0x34];
        gte_ldv3_table(vp);
        p[0x34] = (uint)(vp + 3);
        gte_rtpt();
        p[0x28] = p[0x28] + 1;
        gte_nclip();
        gte_stopz(p + 0x2a);
        gte_stsxy3(p);
        vp = (uint *)p[0x34];
        gte_ldv0_table1(vp);
        p[0x34] = (uint)(vp + 1);
        gte_rtps();
        gte_stsxy(p[3] + 0x28);
        __asm__ volatile("nop");
        __asm__ volatile("nop");
        gte_avsz4();
        gte_stotz(p + 0x29);
        if (0 < (int)p[0x29]) {
            gte_nclip();
            gte_stopz(p + 0x2b);
            if (0 < (int)p[0x2a] || (int)p[0x2b] < 1) {
                p[0x29] = (int)p[0x29] >> p[0];
                *(uint *)p[3] = ((uint)((uint *)p[3] + 2) & mask) | tag;
                *(uint *)(p[3] + 4) = p[0x23];
                *(uint *)(p[3] + 8) = (*(uint *)(p[0x29] * 4 + p[5]) & mask) | 0x8000000;
                *(uint *)(p[0x29] * 4 + p[5]) = p[3] & mask;
                if ((p[0x1f] & tag) == 0) {
                    gte_ldv0((void *)*(uint *)p[0x35]);
                    gte_ldrgb0(p);
                    __asm__ volatile("nop");
                    __asm__ volatile("nop");
                    gte_nccs();
                    gte_strgb(p + 0x2f);
                    gte_ldrgb((void *)(p[0x33] + 4));
                    __asm__ volatile("nop");
                    __asm__ volatile("nop");
                    gte_nccs();
                    gte_strgb(p[3] + 0x14);
                    gte_ldrgb((void *)(p[0x33] + 8));
                    __asm__ volatile("nop");
                    __asm__ volatile("nop");
                    gte_nccs();
                    gte_strgb(p[3] + 0x1c);
                    gte_ldrgb((void *)(p[0x33] + 0xc));
                    __asm__ volatile("nop");
                    __asm__ volatile("nop");
                    gte_nccs();
                    gte_strgb(p[3] + 0x24);
                } else {
                    p[0x2f] = *(uint *)p[0x33];
                    *(uint *)(p[3] + 0x14) = *(uint *)(p[0x33] + 4);
                    *(uint *)(p[3] + 0x1c) = *(uint *)(p[0x33] + 8);
                    *(uint *)(p[3] + 0x24) = *(uint *)(p[0x33] + 0xc);
                }
                code = (p[0x2f] & mask) | 0x3a000000;
                *(uint *)(p[3] + 0xc) = p[0x20] | code;
                p[3] = p[3] + 0x2c;
            }
        }
        p[0x33] = p[0x33] + 0x10;
        p[0x35] = p[0x35] + 4;
    } while (p[0x28] < p[1]);
}

/**
 * kaze_tmd_quad_gouraud_4normal_1910  (library, 0x80041910)
 *
 * TMD worker for quads with four normal pointers and four colors (nccs per
 * vertex), conditional semi-trans preamble, emits POLY_G4 0x38/0x3a packets.
 */

void kaze_tmd_quad_gouraud_4normal_1910(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 m24;
    u32 m1;

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
            ctx[4] = ctx[3];
            if ((*(u32 *)ctx[0x33] & 0x2000000) != 0) {
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                ctx[0x1e] = 0x3a000000;
                ctx[3] = ctx[3] + 8;
            } else {
                ctx[0x1e] = 0x38000000;
            }
            gte_stopz(ctx + 0x2a);
            gte_stsxy3_g3(ctx[3]);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x20);
            __asm__ volatile("nop");
            __asm__ volatile("nop");
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
                ctx[3] = ctx[4];
            } else {
                gte_nclip();
                gte_stopz(ctx + 0x2b);
                if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                    ctx[3] = ctx[4];
                } else {
                    ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                    *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x8000000;
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
                        gte_strgb(ctx[3] + 0xc);
                        gte_ldv0(*(u32 *)(ctx[0x35] + 8));
                        gte_ldrgb(ctx[0x33] + 8);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx[3] + 0x14);
                        gte_ldv0(*(u32 *)(ctx[0x35] + 0xc));
                        gte_ldrgb(ctx[0x33] + 0xc);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx[3] + 0x1c);
                    } else {
                        ctx[0x2f] = *(u32 *)ctx[0x33];
                        *(u32 *)(ctx[3] + 0xc) = *(u32 *)(ctx[0x33] + 4);
                        *(u32 *)(ctx[3] + 0x14) = *(u32 *)(ctx[0x33] + 8);
                        *(u32 *)(ctx[3] + 0x1c) = *(u32 *)(ctx[0x33] + 0xc);
                    }
                    *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | (ctx[0x2f] & m24);
                    ctx[3] = ctx[3] + 0x24;
                }
            }
            ctx[0x33] = ctx[0x33] + 0x10;
            ctx[0x35] = ctx[0x35] + 0x10;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_tmd_quad_gouraud_4normal_tpage_1ce8  (library, 0x80041ce8)
 *
 * TMD worker for quads with four normal pointers and four colors, fixed
 * tpage preamble, emits POLY_G4 code-0x3a packets into the OT.
 */

void kaze_tmd_quad_gouraud_4normal_tpage_1ce8(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 h;
    u32 m24;
    u32 m1;

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
            gte_stsxy3_g3(ctx[3] + 8);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x28);
            __asm__ volatile("nop");
            __asm__ volatile("nop");
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                gte_nclip();
                gte_stopz(ctx + 0x2b);
                if ((0 < (int)ctx[0x2a]) || ((int)ctx[0x2b] < 1)) {
                    ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                    *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1;
                    *(u32 *)(ctx[3] + 4) = ctx[0x23];
                    *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x8000000;
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
                        gte_strgb(ctx[3] + 0x14);
                        gte_ldv0(*(u32 *)(ctx[0x35] + 8));
                        gte_ldrgb(ctx[0x33] + 8);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx[3] + 0x1c);
                        gte_ldv0(*(u32 *)(ctx[0x35] + 0xc));
                        gte_ldrgb(ctx[0x33] + 0xc);
                        __asm__ volatile("nop");
                        __asm__ volatile("nop");
                        gte_nccs();
                        gte_strgb(ctx[3] + 0x24);
                    } else {
                        ctx[0x2f] = *(u32 *)ctx[0x33];
                        *(u32 *)(ctx[3] + 0x14) = *(u32 *)(ctx[0x33] + 4);
                        *(u32 *)(ctx[3] + 0x1c) = *(u32 *)(ctx[0x33] + 8);
                        *(u32 *)(ctx[3] + 0x24) = *(u32 *)(ctx[0x33] + 0xc);
                    }
                    h = (ctx[0x2f] & m24) | 0x3a000000;
                    *(u32 *)(ctx[3] + 0xc) = ctx[0x20] | h;
                    ctx[3] = ctx[3] + 0x2c;
                }
            }
            ctx[0x33] = ctx[0x33] + 0x10;
            ctx[0x35] = ctx[0x35] + 0x10;
        } while (ctx[0x28] < ctx[1]);
    }
}

void kaze_tmd_clip_tri_flat_lit_23e0();
void kaze_tmd_clip_tri_flat_prelit_265c();

/**
 * kaze_tmd_clip_sort_dispatch_2070  (library, 0x80042070)
 *
 * Near-clip TMD sort entry: builds packet context (OT shift, light flags,
 * DR_MODE word 0xe1000200), copies 3 matrix rows, walks the 14-class
 * primitive table at model+0x198 backwards dispatching
 * kaze_tmd_clip_tri_flat_lit_23e0..kaze_tmd_clip_quad_gtex_lit_49d8.
 */

s32 kaze_tmd_clip_sort_dispatch_2070(uint *model, s32 flags, uint arg2, s32 ret, uint arg4, uint *ctx)
{
    uint *prim;
    uint *scratch;
    uint count;
    uint i;
    register uint arg2_hold asm("$4");

    ctx[0x24] = flags;
    arg2_hold = arg2;
    scratch = ctx + 0x41;
    if (flags < 0) {
        return ret;
    }

    ctx[5] = arg2_hold;
    ctx[0] = arg4;
    ctx[2] = (uint)(model + 0x66);
    ctx[1] = 0;
    ctx[3] = ret;
    ctx[0x25] = 0;
    ctx[0x20] = ((uint)ctx[0x24] >> 5) & 0x02000000;
    ctx[0x1f] = (((uint)ctx[0x24] >> 5) & 0x02000000) | ((ctx[0x24] << 18) & 0x01000000);
    ctx[0x21] = ((uint)ctx[0x24] >> 7) & 0x00600000;
    ctx[0x23] = (((uint)ctx[0x24] >> 23) & 0x60) | 0xe1000200;

    do {
        i = ctx[0x25] * 4;
        *(uint *)(i + (uint)ctx + 0x18) = *(uint *)(i + (uint)model + 0x8);
        i = ctx[0x25] * 4;
        *(uint *)(i + (uint)ctx + 0x38) = *(uint *)(i + (uint)model + 0x28);
        i = ctx[0x25] * 4;
        *(uint *)(i + (uint)ctx + 0x58) = *(uint *)(i + (uint)model + 0x48);
        ctx[0x25] = ctx[0x25] + 1;
    } while (ctx[0x25] < 8);

    prim = (uint *)ctx[2] - 4;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_quad_gtex_lit_49d8(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 8;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_quad_ftex_lit_4648(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 8;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_quad_gouraud_4normal_42b0(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 4;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_quad_grad_1normal_3f60(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 4;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_quad_gouraud_lit_3c5c(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 4;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_quad_flat_prelit_3a18(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 4;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_quad_flat_lit_3760(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 8;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_tri_gtex_lit_2e30(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 8;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_tri_ftex_lit_2b0c(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 8;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_tri_gouraud_3normal_3450(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 4;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_tri_grad_1normal_3170(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 4;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_tri_gouraud_lit_285c(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 4;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_tri_flat_prelit_265c(ctx, (uint)scratch);
    }
    prim = (uint *)ctx[2] - 4;
    ctx[2] = (uint)prim;
    if (*prim != 0) {
        kaze_tmd_clip_tri_flat_lit_23e0(ctx, (uint)scratch);
    }

    count = ctx[3];
    return count;
}

#include "gte.h"
#define gte_avsz3() __asm__ volatile("cop2 0x158002D")
#undef gte_ldrgb
#define gte_ldrgb(p) __asm__ volatile("lwc2 $6, 0(%0)" :: "r"(p))
#undef gte_stopz
#define gte_stopz(p) __asm__ volatile("swc2 $24, 0(%0)" :: "r"(p) : "memory")
#undef gte_stotz
#define gte_stotz(p) __asm__ volatile("swc2 $7, 0(%0)" :: "r"(p) : "memory")
#undef gte_strgb
#define gte_strgb(p) __asm__ volatile("swc2 $22, 0(%0)" :: "r"(p) : "memory")

/**
 * kaze_tmd_clip_tri_flat_lit_23e0  (library, 0x800423e0)
 *
 * Clip-aware flat-triangle worker: rtpt with gte_stflg overflow check;
 * overflow/edge prims are copied unaligned into scratch and subdivided via
 * kaze_tmd_divide_tri_flat_4db8; one normal+color via nccs.
 */

void kaze_tmd_clip_tri_flat_lit_23e0(uint *p, uint out)
{
    uint *r0;
    uint u;

    p[1] = *(uint *)p[2];
    p[0x2d] = *(uint *)(p[2] + 4);
    p[0x2e] = *(uint *)(p[2] + 8);
    p[0x2f] = *(uint *)(p[2] + 0xc);
    p[0x25] = 0;
    if (p[1] != 0) {
        r0 = p + 0x2b;
        do {
            uint *v = (uint *)p[0x2e];
            gte_ldv3((void *)v[0], (void *)v[1], (void *)v[2]);
            p[0x25] = p[0x25] + 1;
            gte_rtpt();
            if ((p[0x24] & 0x100) == 0) {
                gte_stflg(r0);
                if ((int)p[0x2b] >= 0) {
                    gte_nclip();
                    gte_stopz(r0);
                    if ((int)p[0x2b] <= 0) goto next;
                }
            }
            *(S8 *)out = *(S8 *)*(uint *)p[0x2e];
            *(S8 *)(out + 0xc) = *(S8 *)*(uint *)(p[0x2e] + 4);
            gte_avsz3();
            *(S8 *)(out + 0x18) = *(S8 *)*(uint *)(p[0x2e] + 8);
            gte_stotz(r0);
            if (0 < (int)p[0x2b]) {
                p[4] = p[5] + ((int)p[0x2b] >> p[0]) * 4;
                if ((p[0x1f] & 0x1000000) == 0) {
                    gte_ldv0((void *)*(uint *)p[0x2f]);
                    gte_ldrgb((void *)p[0x2d]);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 8);
                } else {
                    *(uint *)(out + 8) = *(uint *)p[0x2d];
                }
                u = *(uint *)(out + 8);
                *(uint *)(out + 0x20) = u;
                *(uint *)(out + 0x14) = u;
                kaze_tmd_divide_tri_flat_4db8(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 4;
            p[0x2e] = p[0x2e] + 0xc;
            p[0x2f] = p[0x2f] + 4;
        } while (p[0x25] < p[1]);
    }
}

#undef gte_stopz
#define gte_stopz(p) __asm__ volatile("swc2 $24, 0(%0)" :: "r"(p) : "memory")
#undef gte_stotz
#define gte_stotz(p) __asm__ volatile("swc2 $7, 0(%0)" :: "r"(p) : "memory")

/**
 * kaze_tmd_clip_tri_flat_prelit_265c  (library, 0x8004265c)
 *
 * Clip-aware flat-triangle worker for prelit primitives (no light calc);
 * overflow/off-screen triangles are handed to the F3 subdivider
 * kaze_tmd_divide_tri_flat_4db8.
 */

void kaze_tmd_clip_tri_flat_prelit_265c(uint *p, uint out)
{
    uint *r0;
    uint u;

    p[1] = *(uint *)p[2];
    p[0x2d] = *(uint *)(p[2] + 4);
    p[0x2e] = *(uint *)(p[2] + 8);
    p[0x25] = 0;
    if (p[1] != 0) {
        r0 = p + 0x2b;
        do {
            uint *v = (uint *)p[0x2e];
            gte_ldv3((void *)v[0], (void *)v[1], (void *)v[2]);
            p[0x25] = p[0x25] + 1;
            gte_rtpt();
            if ((p[0x24] & 0x100) == 0) {
                gte_stflg(r0);
                if ((int)p[0x2b] >= 0) {
                    gte_nclip();
                    gte_stopz(r0);
                    if ((int)p[0x2b] <= 0) goto next;
                }
            }
            *(S8 *)out = *(S8 *)*(uint *)p[0x2e];
            *(S8 *)(out + 0xc) = *(S8 *)*(uint *)(p[0x2e] + 4);
            gte_avsz3();
            *(S8 *)(out + 0x18) = *(S8 *)*(uint *)(p[0x2e] + 8);
            gte_stotz(r0);
            if (0 < (int)p[0x2b]) {
                p[4] = p[5] + ((int)p[0x2b] >> p[0]) * 4;
                u = *(uint *)p[0x2d];
                *(uint *)(out + 8) = u;
                *(uint *)(out + 0x20) = u;
                *(uint *)(out + 0x14) = u;
                kaze_tmd_divide_tri_flat_4db8(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 4;
            p[0x2e] = p[0x2e] + 0xc;
        } while (p[0x25] < p[1]);
    }
}
