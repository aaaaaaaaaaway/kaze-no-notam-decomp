#include "types.h"
#include "kaze_syms.h"
extern void SetPolyG3(void *);
typedef struct { char c[8]; } S8;
typedef struct { char b[12]; } V12;

#include "gte.h"
void kaze_tmd_divide_tri_gouraud_5530();
#define gte_nclip() __asm__ volatile("cop2 0x1400006")
#define gte_avsz3() __asm__ volatile("cop2 0x158002D")
#define gte_ncct() __asm__ volatile("cop2 0x118043F")
#define gte_ldrgb(p) __asm__ volatile("lwc2 $6, 0(%0)" :: "r"(p))
#define gte_stopz(p) __asm__ volatile("swc2 $24, 0(%0)" :: "r"(p) : "memory")
#define gte_stotz(p) __asm__ volatile("swc2 $7, 0(%0)" :: "r"(p) : "memory")

/**
 * kaze_tmd_clip_tri_gouraud_lit_285c  (library, 0x8004285c)
 *
 * Clip-aware lit gouraud-triangle worker: 3 normals via ncct/strgb3;
 * overflow/edge prims subdivided through the G3 divider
 * kaze_tmd_divide_tri_gouraud_5530.
 */

void kaze_tmd_clip_tri_gouraud_lit_285c(uint *p, uint out)
{
    uint *r0;
    uint out8;
    uint u;

    p[1] = *(uint *)p[2];
    p[0x2d] = *(uint *)(p[2] + 4);
    p[0x2e] = *(uint *)(p[2] + 8);
    p[0x2f] = *(uint *)(p[2] + 0xc);
    SetPolyG3(p + 0x34);
    p[0x25] = 0;
    if (p[1] != 0) {
        r0 = p + 0x2b;
        out8 = out + 8;
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
                    uint *n = (uint *)p[0x2f];
                    gte_ldv3((void *)n[0], (void *)n[1], (void *)n[2]);
                    gte_ldrgb((void *)p[0x2d]);
                    gte_nop();
                    gte_nop();
                    gte_ncct();
                    gte_strgb3(out8, out + 0x14, out + 0x20);
                } else {
                    u = *(uint *)p[0x2d];
                    *(uint *)(out + 0x20) = u;
                    *(uint *)(out + 0x14) = u;
                    *(uint *)(out + 8) = u;
                }
                kaze_tmd_divide_tri_gouraud_5530(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 4;
            p[0x2e] = p[0x2e] + 0xc;
            p[0x2f] = p[0x2f] + 0xc;
        } while (p[0x25] < p[1]);
    }
}

void kaze_tmd_divide_tri_ftex_5cf8();
#define gte_nccs() __asm__ volatile("cop2 0x108041B")
#define gte_strgb(p) __asm__ volatile("swc2 $22, 0(%0)" :: "r"(p) : "memory")

/**
 * kaze_tmd_clip_tri_ftex_lit_2b0c  (library, 0x80042b0c)
 *
 * Clip-aware textured flat-triangle worker (POLY_FT3 family): one
 * normal+color via nccs; overflow/edge prims subdivided via
 * kaze_tmd_divide_tri_ftex_5cf8.
 */

void kaze_tmd_clip_tri_ftex_lit_2b0c(u32 *p, u32 out)
{
    u32 *r0;
    u32 u;

    p[1] = *(u32 *)p[2];
    p[0x2d] = *(u32 *)(p[2] + 4);
    p[0x2e] = *(u32 *)(p[2] + 8);
    p[0x2f] = *(u32 *)(p[2] + 0xc);
    p[0x25] = 0;
    if (p[1] != 0) {
        r0 = p + 0x2b;
        do {
            {
                u32 *base = (u32 *)p[0x2e];
                register u32 a asm("$7") = base[0];
                register u32 b asm("$8") = base[1];
                register u32 c asm("$9") = base[2];
                __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(a));
                __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(a));
                __asm__ volatile("lwc2 $2, 0(%0)" :: "r"(b));
                __asm__ volatile("lwc2 $3, 4(%0)" :: "r"(b));
                __asm__ volatile("lwc2 $4, 0(%0)" :: "r"(c));
                __asm__ volatile("lwc2 $5, 4(%0)" :: "r"(c));
            }
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
            *(S8 *)out = *(S8 *)*(u32 *)p[0x2e];
            *(S8 *)(out + 0xc) = *(S8 *)*(u32 *)(p[0x2e] + 4);
            gte_avsz3();
            *(S8 *)(out + 0x18) = *(S8 *)*(u32 *)(p[0x2e] + 8);
            p[0x30] = *(u16 *)(p[0x2d] + 6) & 7;
            gte_stotz(r0);
            if (0 < (int)p[0x2b]) {
                p[4] = p[5] + ((int)p[0x2b] >> p[0]) * 4;
                *(u16 *)((u8 *)p + 0xcc) = *(u16 *)((u8 *)p + p[0x30] * 4 + 0x3a);
                *(u16 *)((u8 *)p + 0xce) = *(u16 *)((u8 *)p + p[0x30] * 4 + 0x1a);
                *(u16 *)(out + 6) = *(u32 *)(p[0x2d] + 4) | p[p[0x30] + 0x16];
                *(u16 *)(out + 0x12) = *(u32 *)(p[0x2d] + 8) | p[p[0x30] + 0x16];
                *(u16 *)(out + 0x1e) = *(u32 *)(p[0x2d] + 0xc) | p[p[0x30] + 0x16];
                if ((p[0x1f] & 0x1000000) == 0) {
                    register u32 n0 asm("$2") = *(u32 *)p[0x2f];
                    register u32 *o8 asm("$2");
                    register u32 rgb asm("$7");
                    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(n0));
                    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(n0));
                    rgb = p[0x2d];
                    __asm__ volatile("lwc2 $6, 0(%0)" :: "r"(rgb));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    o8 = (u32 *)(out + 8);
                    __asm__ volatile("swc2 $22, 0(%0)" :: "r"(o8) : "memory");
                } else {
                    *(u32 *)(out + 8) = *(u32 *)p[0x2d];
                }
                u = *(u32 *)(out + 8);
                *(u32 *)(out + 0x20) = u;
                *(u32 *)(out + 0x14) = u;
                kaze_tmd_divide_tri_ftex_5cf8(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 0x10;
            p[0x2e] = p[0x2e] + 0xc;
            p[0x2f] = p[0x2f] + 4;
        } while (p[0x25] < p[1]);
    }
}

void kaze_tmd_divide_tri_gtex_6500();

/**
 * kaze_tmd_clip_tri_gtex_lit_2e30  (library, 0x80042e30)
 *
 * Clip-aware lit gouraud-textured triangle worker (POLY_GT3 family): 3
 * normals via ncct; overflow/edge prims subdivided via
 * kaze_tmd_divide_tri_gtex_6500.
 */

void kaze_tmd_clip_tri_gtex_lit_2e30(u32 *p, u32 out)
{
    register u32 color asm("$18");
    register u32 *r0 asm("$19");

    p[1] = *(u32 *)p[2];
    p[0x2d] = *(u32 *)(p[2] + 4);
    p[0x2e] = *(u32 *)(p[2] + 8);
    p[0x2f] = *(u32 *)(p[2] + 0xc);
    p[0x25] = 0;
    if (p[1] != 0) {
        r0 = p + 0x2b;
        color = 0x34808080;
        do {
            {
                u32 *base = (u32 *)p[0x2e];
                register u32 a asm("$7") = base[0];
                register u32 b asm("$8") = base[1];
                register u32 c asm("$9") = base[2];
                __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(a));
                __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(a));
                __asm__ volatile("lwc2 $2, 0(%0)" :: "r"(b));
                __asm__ volatile("lwc2 $3, 4(%0)" :: "r"(b));
                __asm__ volatile("lwc2 $4, 0(%0)" :: "r"(c));
                __asm__ volatile("lwc2 $5, 4(%0)" :: "r"(c));
            }
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
            *(S8 *)out = *(S8 *)*(u32 *)p[0x2e];
            *(S8 *)(out + 0xc) = *(S8 *)*(u32 *)(p[0x2e] + 4);
            gte_avsz3();
            *(S8 *)(out + 0x18) = *(S8 *)*(u32 *)(p[0x2e] + 8);
            p[0x30] = *(u16 *)(p[0x2d] + 2) & 7;
            gte_stotz(r0);
            if (0 < (int)p[0x2b]) {
                p[4] = p[5] + ((int)p[0x2b] >> p[0]) * 4;
                *(u16 *)((u8 *)p + 0xcc) = *(u16 *)((u8 *)p + p[0x30] * 4 + 0x3a);
                *(u16 *)((u8 *)p + 0xce) = *(u16 *)((u8 *)p + p[0x30] * 4 + 0x1a);
                *(u16 *)(out + 6) = *(u32 *)p[0x2d] | p[p[0x30] + 0x16];
                *(u16 *)(out + 0x12) = *(u32 *)(p[0x2d] + 4) | p[p[0x30] + 0x16];
                *(u16 *)(out + 0x1e) = *(u32 *)(p[0x2d] + 8) | p[p[0x30] + 0x16];
                if ((p[0x1f] & 0x1000000) == 0) {
                    u32 *np = (u32 *)p[0x2f];
                    register u32 na asm("$7") = np[0];
                    register u32 nb asm("$8") = np[1];
                    register u32 nc asm("$9") = np[2];
                    register u32 *o20 asm("$4");
                    register u32 *o8 asm("$3");
                    register u32 *o14 asm("$2");
                    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(na));
                    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(na));
                    __asm__ volatile("lwc2 $2, 0(%0)" :: "r"(nb));
                    __asm__ volatile("lwc2 $3, 4(%0)" :: "r"(nb));
                    __asm__ volatile("lwc2 $4, 0(%0)" :: "r"(nc));
                    __asm__ volatile("lwc2 $5, 4(%0)" :: "r"(nc));
                    o20 = (u32 *)(out + 0x20);
                    *(u32 *)(out + 0x20) = color;
                    gte_ldrgb_ncct(o20);
                    o8 = (u32 *)(out + 8);
                    o14 = (u32 *)(out + 0x14);
                    __asm__ volatile("swc2 $20, 0(%0)" :: "r"(o8) : "memory");
                    __asm__ volatile("swc2 $21, 0(%0)" :: "r"(o14) : "memory");
                    __asm__ volatile("swc2 $22, 0(%0)" :: "r"(o20) : "memory");
                } else {
                    *(u32 *)(out + 0x20) = color;
                    *(u32 *)(out + 0x14) = color;
                    *(u32 *)(out + 8) = color;
                }
                kaze_tmd_divide_tri_gtex_6500(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 0xc;
            p[0x2e] = p[0x2e] + 0xc;
            p[0x2f] = p[0x2f] + 0xc;
        } while (p[0x25] < p[1]);
    }
}

/**
 * kaze_tmd_clip_tri_grad_1normal_3170  (library, 0x80043170)
 *
 * Clip-aware gradation-triangle worker: one normal, three colors (nccs x3);
 * overflow/edge prims subdivided through the G3 divider
 * kaze_tmd_divide_tri_gouraud_5530.
 */

void kaze_tmd_clip_tri_grad_1normal_3170(uint *p, uint out)
{
    uint *r0;

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
                    gte_ldrgb((void *)(p[0x2d] + 4));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 0x14);
                    gte_ldrgb((void *)(p[0x2d] + 8));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 0x20);
                } else {
                    *(uint *)(out + 8) = *(uint *)p[0x2d];
                    *(uint *)(out + 0x14) = *(uint *)(p[0x2d] + 4);
                    *(uint *)(out + 0x20) = *(uint *)(p[0x2d] + 8);
                }
                kaze_tmd_divide_tri_gouraud_5530(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 0xc;
            p[0x2e] = p[0x2e] + 0xc;
            p[0x2f] = p[0x2f] + 4;
        } while (p[0x25] < p[1]);
    }
}

/**
 * kaze_tmd_clip_tri_gouraud_3normal_3450  (library, 0x80043450)
 *
 * Clip-aware triangle worker with three normal pointers and three colors
 * (ldv0/nccs x3); subdivided through the G3 divider
 * kaze_tmd_divide_tri_gouraud_5530 when clipped.
 */

void kaze_tmd_clip_tri_gouraud_3normal_3450(uint *p, uint out)
{
    uint *r0;

    p[1] = *(uint *)p[2];
    p[0x2d] = *(uint *)(p[2] + 4);
    p[0x2e] = *(uint *)(p[2] + 8);
    p[0x2f] = *(uint *)(p[2] + 0xc);
    p[0x25] = 0;
    if (p[1] != 0) {
        r0 = p + 0x2b;
        do {
            gte_ldv3_clip_list(p);
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
                    gte_ldrgb_color0(p);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 8);
                    gte_ldv0((void *)*(uint *)(p[0x2f] + 4));
                    gte_ldrgb((void *)(p[0x2d] + 4));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 0x14);
                    gte_ldv0((void *)*(uint *)(p[0x2f] + 8));
                    gte_ldrgb((void *)(p[0x2d] + 8));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 0x20);
                } else {
                    *(uint *)(out + 8) = *(uint *)p[0x2d];
                    *(uint *)(out + 0x14) = *(uint *)(p[0x2d] + 4);
                    *(uint *)(out + 0x20) = *(uint *)(p[0x2d] + 8);
                }
                kaze_tmd_divide_tri_gouraud_5530(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 0xc;
            p[0x2e] = p[0x2e] + 0xc;
            p[0x2f] = p[0x2f] + 0xc;
        } while (p[0x25] < p[1]);
    }
}

void kaze_tmd_divide_quad_flat_510c();
#define gte_avsz4() __asm__ volatile("cop2 0x168002E")

/**
 * kaze_tmd_clip_quad_flat_lit_3760  (library, 0x80043760)
 *
 * Clip-aware flat-quad worker: 4 vertices, one normal+color via nccs;
 * overflow/edge quads subdivided via the F4 divider
 * kaze_tmd_divide_quad_flat_510c.
 */

void kaze_tmd_clip_quad_flat_lit_3760(uint *p, uint out)
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
            *(S8 *)out = *(S8 *)(*(uint *)p[0x2e]);
            gte_nclip();
            *(S8 *)(out + 0xc) = *(S8 *)(*(uint *)(p[0x2e] + 4));
            gte_stopz(p + 0x2c);
            gte_ldv0((void *)*(uint *)(p[0x2e] + 0xc));
            *(S8 *)(out + 0x18) = *(S8 *)(*(uint *)(p[0x2e] + 8));
            gte_rtps();
            *(S8 *)(out + 0x24) = *(S8 *)(*(uint *)(p[0x2e] + 0xc));
            gte_avsz4();
            gte_stotz(r0);
            if (0 < (int)p[0x2b]) {
                p[4] = p[5] + ((int)p[0x2b] >> p[0]) * 4;
                if ((p[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(r0);
                    if ((0 < (int)p[0x2b]) && ((int)p[0x2c] < 1)) goto next;
                }
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
                *(uint *)(out + 0x2c) = u;
                *(uint *)(out + 0x20) = u;
                *(uint *)(out + 0x14) = u;
                kaze_tmd_divide_quad_flat_510c(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 4;
            p[0x2e] = p[0x2e] + 0x10;
            p[0x2f] = p[0x2f] + 4;
        } while (p[0x25] < p[1]);
    }
}

/**
 * kaze_tmd_clip_quad_flat_prelit_3a18  (library, 0x80043a18)
 *
 * Clip-aware flat-quad worker for prelit primitives (no light calc);
 * overflow/off-screen quads handed to the F4 subdivider
 * kaze_tmd_divide_quad_flat_510c.
 */

void kaze_tmd_clip_quad_flat_prelit_3a18(uint *p, uint out)
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
            *(S8 *)out = *(S8 *)(*(uint *)p[0x2e]);
            gte_nclip();
            *(S8 *)(out + 0xc) = *(S8 *)(*(uint *)(p[0x2e] + 4));
            gte_stopz(p + 0x2c);
            gte_ldv0((void *)*(uint *)(p[0x2e] + 0xc));
            *(S8 *)(out + 0x18) = *(S8 *)(*(uint *)(p[0x2e] + 8));
            gte_rtps();
            *(S8 *)(out + 0x24) = *(S8 *)(*(uint *)(p[0x2e] + 0xc));
            gte_avsz4();
            gte_stotz(r0);
            if (0 < (int)p[0x2b]) {
                p[4] = p[5] + ((int)p[0x2b] >> p[0]) * 4;
                if ((p[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(r0);
                    if ((0 < (int)p[0x2b]) && ((int)p[0x2c] < 1)) goto next;
                }
                u = *(uint *)p[0x2d];
                *(uint *)(out + 8) = u;
                *(uint *)(out + 0x2c) = u;
                *(uint *)(out + 0x20) = u;
                *(uint *)(out + 0x14) = u;
                kaze_tmd_divide_quad_flat_510c(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 4;
            p[0x2e] = p[0x2e] + 0x10;
        } while (p[0x25] < p[1]);
    }
}

void kaze_tmd_divide_quad_gouraud_58a4();

/**
 * kaze_tmd_clip_quad_gouraud_lit_3c5c  (library, 0x80043c5c)
 *
 * Clip-aware lit gouraud-quad worker: 3+1 normals (ncct+nccs); overflow/edge
 * quads subdivided through the G4 divider kaze_tmd_divide_quad_gouraud_58a4.
 */

void kaze_tmd_clip_quad_gouraud_lit_3c5c(uint *p, uint out)
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
            {
                register uint *v asm("$2");
                v = (uint *)p[0x2e];
                gte_ldv3_clip(v);
            }
            p[0x25] = p[0x25] + 1;
            gte_rtpt();
            *(S8 *)out = *(S8 *)(*(uint *)p[0x2e]);
            gte_nclip();
            *(S8 *)(out + 0xc) = *(S8 *)(*(uint *)(p[0x2e] + 4));
            gte_stopz(p + 0x2c);
            {
                register uint *v asm("$2");
                v = (uint *)p[0x2e];
                gte_ldv0_fourth(v);
            }
            *(S8 *)(out + 0x18) = *(S8 *)(*(uint *)(p[0x2e] + 8));
            gte_rtps();
            *(S8 *)(out + 0x24) = *(S8 *)(*(uint *)(p[0x2e] + 0xc));
            gte_avsz4();
            gte_stotz(r0);
            if (0 < (int)p[0x2b]) {
                uint clip_flags = p[0x24];
                p[4] = p[5] + ((int)p[0x2b] >> p[0]) * 4;
                if ((clip_flags & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(r0);
                    if ((0 < (int)p[0x2b]) && ((int)p[0x2c] < 1)) goto next;
                }
                if ((p[0x1f] & 0x1000000) == 0) {
                    register uint *n asm("$2");
                    register void *rgb asm("$8");
                    n = (uint *)p[0x2f];
                    gte_ldv3_color(n);
                    rgb = (void *)p[0x2d];
                    gte_ldrgb(rgb);
                    gte_nop();
                    gte_nop();
                    gte_ncct();
                    gte_strgb3(out + 8, out + 0x14, out + 0x20);
                    gte_ldv0((void *)*(uint *)(p[0x2f] + 0xc));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 0x2c);
                } else {
                    u = *(uint *)p[0x2d];
                    *(uint *)(out + 0x2c) = u;
                    *(uint *)(out + 0x20) = u;
                    *(uint *)(out + 0x14) = u;
                    *(uint *)(out + 8) = u;
                }
                kaze_tmd_divide_quad_gouraud_58a4(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 4;
            p[0x2e] = p[0x2e] + 0x10;
            p[0x2f] = p[0x2f] + 0x10;
        } while (p[0x25] < p[1]);
    }
}

/* Scoped GTE load islands pin only the latency-sensitive register family. */

/**
 * kaze_tmd_clip_quad_grad_1normal_3f60  (library, 0x80043f60)
 *
 * Clip-aware gradation-quad worker: one normal, four colors (nccs x4);
 * overflow/edge quads subdivided through the G4 divider
 * kaze_tmd_divide_quad_gouraud_58a4.
 */

void kaze_tmd_clip_quad_grad_1normal_3f60(uint *p, uint out)
{
    uint *r0;

    p[1] = *(uint *)p[2];
    p[0x2d] = *(uint *)(p[2] + 4);
    p[0x2e] = *(uint *)(p[2] + 8);
    p[0x2f] = *(uint *)(p[2] + 0xc);
    p[0x25] = 0;
    if (p[1] != 0) {
        r0 = p + 0x2b;
        do {
            uint *v = (uint *)p[0x2e];
            register uint *vp asm("$2");
            vp = v;
            gte_ldv3_table(vp);
            p[0x25] = p[0x25] + 1;
            gte_rtpt();
            *(S8 *)out = *(S8 *)(*(uint *)p[0x2e]);
            gte_nclip();
            *(S8 *)(out + 0xc) = *(S8 *)(*(uint *)(p[0x2e] + 4));
            gte_stopz(p + 0x2c);
            v = (uint *)p[0x2e];
            vp = v;
            gte_ldv0_table3(vp);
            *(S8 *)(out + 0x18) = *(S8 *)(*(uint *)(p[0x2e] + 8));
            gte_rtps();
            *(S8 *)(out + 0x24) = *(S8 *)(*(uint *)(p[0x2e] + 0xc));
            gte_avsz4();
            gte_stotz(r0);
            if (0 < (int)p[0x2b]) {
                p[4] = p[5] + ((int)p[0x2b] >> p[0]) * 4;
                if ((p[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(r0);
                    if ((0 < (int)p[0x2b]) && ((int)p[0x2c] < 1)) goto next;
                }
                if ((p[0x1f] & 0x1000000) == 0) {
                    gte_ldv0((void *)*(uint *)p[0x2f]);
                    gte_ldrgb_p2d(p);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 8);
                    gte_ldrgb((void *)(p[0x2d] + 4));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 0x14);
                    gte_ldrgb((void *)(p[0x2d] + 8));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 0x20);
                    gte_ldrgb((void *)(p[0x2d] + 0xc));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 0x2c);
                } else {
                    *(uint *)(out + 8) = *(uint *)p[0x2d];
                    *(uint *)(out + 0x14) = *(uint *)(p[0x2d] + 4);
                    *(uint *)(out + 0x20) = *(uint *)(p[0x2d] + 8);
                    *(uint *)(out + 0x2c) = *(uint *)(p[0x2d] + 0xc);
                }
                kaze_tmd_divide_quad_gouraud_58a4(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 0x10;
            p[0x2e] = p[0x2e] + 0x10;
            p[0x2f] = p[0x2f] + 4;
        } while (p[0x25] < p[1]);
    }
}

/**
 * kaze_tmd_clip_quad_gouraud_4normal_42b0  (library, 0x800442b0)
 *
 * Clip-aware quad worker with four normal pointers and four colors
 * (ldv0/nccs x4); subdivided through the G4 divider
 * kaze_tmd_divide_quad_gouraud_58a4 when clipped.
 */

void kaze_tmd_clip_quad_gouraud_4normal_42b0(uint *p, uint out)
{
    uint *r0;

    p[1] = *(uint *)p[2];
    p[0x2d] = *(uint *)(p[2] + 4);
    p[0x2e] = *(uint *)(p[2] + 8);
    p[0x2f] = *(uint *)(p[2] + 0xc);
    p[0x25] = 0;
    if (p[1] != 0) {
        r0 = p + 0x2b;
        do {
            {
                register uint *v asm("$2");
                v = (uint *)p[0x2e];
                gte_ldv3_clip(v);
            }
            p[0x25] = p[0x25] + 1;
            gte_rtpt();
            *(S8 *)out = *(S8 *)(*(uint *)p[0x2e]);
            gte_nclip();
            *(S8 *)(out + 0xc) = *(S8 *)(*(uint *)(p[0x2e] + 4));
            gte_stopz(p + 0x2c);
            {
                register uint *v asm("$2");
                v = (uint *)p[0x2e];
                gte_ldv0_fourth(v);
            }
            *(S8 *)(out + 0x18) = *(S8 *)(*(uint *)(p[0x2e] + 8));
            gte_rtps();
            *(S8 *)(out + 0x24) = *(S8 *)(*(uint *)(p[0x2e] + 0xc));
            gte_avsz4();
            gte_stotz(r0);
            if (0 < (int)p[0x2b]) {
                p[4] = p[5] + ((int)p[0x2b] >> p[0]) * 4;
                if ((p[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(r0);
                    if ((0 < (int)p[0x2b]) && ((int)p[0x2c] < 1)) goto next;
                }
                if ((p[0x1f] & 0x1000000) == 0) {
                    gte_ldv0((void *)*(uint *)p[0x2f]);
                    gte_ldrgb_p2d(p);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 8);
                    gte_ldv0((void *)*(uint *)(p[0x2f] + 4));
                    gte_ldrgb((void *)(p[0x2d] + 4));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 0x14);
                    gte_ldv0((void *)*(uint *)(p[0x2f] + 8));
                    gte_ldrgb((void *)(p[0x2d] + 8));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 0x20);
                    gte_ldv0((void *)*(uint *)(p[0x2f] + 0xc));
                    gte_ldrgb((void *)(p[0x2d] + 0xc));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 0x2c);
                } else {
                    *(uint *)(out + 8) = *(uint *)p[0x2d];
                    *(uint *)(out + 0x14) = *(uint *)(p[0x2d] + 4);
                    *(uint *)(out + 0x20) = *(uint *)(p[0x2d] + 8);
                    *(uint *)(out + 0x2c) = *(uint *)(p[0x2d] + 0xc);
                }
                kaze_tmd_divide_quad_gouraud_58a4(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 0x10;
            p[0x2e] = p[0x2e] + 0x10;
            p[0x2f] = p[0x2f] + 0x10;
        } while (p[0x25] < p[1]);
    }
}

void kaze_tmd_divide_quad_ftex_608c();

/**
 * kaze_tmd_clip_quad_ftex_lit_4648  (library, 0x80044648)
 *
 * Clip-aware textured flat-quad worker (POLY_FT4 family): one normal+color
 * via nccs; overflow/edge quads subdivided via
 * kaze_tmd_divide_quad_ftex_608c.
 */

void kaze_tmd_clip_quad_ftex_lit_4648(uint *p, uint out)
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
            register uint *vp asm("$2");
            vp = v;
            gte_ldv3_table(vp);
            p[0x25] = p[0x25] + 1;
            gte_rtpt();
            *(S8 *)out = *(S8 *)(*(uint *)p[0x2e]);
            gte_nclip();
            *(S8 *)(out + 0xc) = *(S8 *)(*(uint *)(p[0x2e] + 4));
            gte_stopz(p + 0x2c);
            v = (uint *)p[0x2e];
            vp = v;
            gte_ldv0_table3(vp);
            *(S8 *)(out + 0x18) = *(S8 *)(*(uint *)(p[0x2e] + 8));
            gte_rtps();
            *(S8 *)(out + 0x24) = *(S8 *)(*(uint *)(p[0x2e] + 0xc));
            p[0x30] = *(u16 *)(p[0x2d] + 6) & 7;
            gte_avsz4();
            gte_stotz(r0);
            if (0 < (int)p[0x2b]) {
                p[4] = p[5] + ((int)p[0x2b] >> p[0]) * 4;
                if ((p[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(r0);
                    if ((0 < (int)p[0x2b]) && ((int)p[0x2c] < 1)) goto next;
                }
                *(u16 *)((u8 *)p + 0xcc) = *(u16 *)((u8 *)p + p[0x30] * 4 + 0x3a);
                *(u16 *)((u8 *)p + 0xce) = *(u16 *)((u8 *)p + p[0x30] * 4 + 0x1a);
                *(u16 *)(out + 6) = *(uint *)(p[0x2d] + 4) | p[p[0x30] + 0x16];
                *(u16 *)(out + 0x12) = *(uint *)(p[0x2d] + 8) | p[p[0x30] + 0x16];
                *(u16 *)(out + 0x1e) = *(uint *)(p[0x2d] + 0xc) | p[p[0x30] + 0x16];
                *(u16 *)(out + 0x2a) = *(uint *)(p[0x2d] + 0x10) | p[p[0x30] + 0x16];
                if ((p[0x1f] & 0x1000000) == 0) {
                    gte_ldv0((void *)*(uint *)p[0x2f]);
                    gte_ldrgb_p2d(p);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 8);
                } else {
                    *(uint *)(out + 8) = *(uint *)p[0x2d];
                }
                u = *(uint *)(out + 8);
                *(uint *)(out + 0x2c) = u;
                *(uint *)(out + 0x20) = u;
                *(uint *)(out + 0x14) = u;
                kaze_tmd_divide_quad_ftex_608c(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 0x14;
            p[0x2e] = p[0x2e] + 0x10;
            p[0x2f] = p[0x2f] + 4;
        } while (p[0x25] < p[1]);
    }
}

void kaze_tmd_divide_quad_gtex_68b4();

/**
 * kaze_tmd_clip_quad_gtex_lit_49d8  (library, 0x800449d8)
 *
 * Clip-aware lit gouraud-textured quad worker (POLY_GT4 family): 3+1 normals
 * (ncct+nccs); overflow/edge quads subdivided via
 * kaze_tmd_divide_quad_gtex_68b4.
 */

void kaze_tmd_clip_quad_gtex_lit_49d8(uint *p, uint out)
{
    uint *r0;
    uint neutral;

    p[1] = *(uint *)p[2];
    p[0x2d] = *(uint *)(p[2] + 4);
    p[0x2e] = *(uint *)(p[2] + 8);
    p[0x2f] = *(uint *)(p[2] + 0xc);
    p[0x25] = 0;
    if (p[1] != 0) {
        r0 = p + 0x2b;
        neutral = 0x3c808080;
        do {
            {
                register uint *v asm("$2");
                v = (uint *)p[0x2e];
                gte_ldv3_clip(v);
            }
            p[0x25] = p[0x25] + 1;
            gte_rtpt();
            *(S8 *)out = *(S8 *)(*(uint *)p[0x2e]);
            gte_nclip();
            *(S8 *)(out + 0xc) = *(S8 *)(*(uint *)(p[0x2e] + 4));
            gte_stopz(p + 0x2c);
            {
                register uint *v asm("$2");
                v = (uint *)p[0x2e];
                gte_ldv0_fourth(v);
            }
            *(S8 *)(out + 0x18) = *(S8 *)(*(uint *)(p[0x2e] + 8));
            gte_rtps();
            *(S8 *)(out + 0x24) = *(S8 *)(*(uint *)(p[0x2e] + 0xc));
            p[0x30] = *(u16 *)(p[0x2d] + 2) & 7;
            gte_avsz4();
            gte_stotz(r0);
            if (0 < (int)p[0x2b]) {
                uint clip_flags = p[0x24];
                p[4] = p[5] + ((int)p[0x2b] >> p[0]) * 4;
                if ((clip_flags & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(r0);
                    if ((0 < (int)p[0x2b]) && ((int)p[0x2c] < 1)) goto next;
                }
                *(u16 *)((u8 *)p + 0xcc) = *(u16 *)((u8 *)p + p[0x30] * 4 + 0x3a);
                *(u16 *)((u8 *)p + 0xce) = *(u16 *)((u8 *)p + p[0x30] * 4 + 0x1a);
                *(u16 *)(out + 6) = *(uint *)p[0x2d] | p[p[0x30] + 0x16];
                *(u16 *)(out + 0x12) = *(uint *)(p[0x2d] + 4) | p[p[0x30] + 0x16];
                *(u16 *)(out + 0x1e) = *(uint *)(p[0x2d] + 8) | p[p[0x30] + 0x16];
                *(u16 *)(out + 0x2a) = *(uint *)(p[0x2d] + 0xc) | p[p[0x30] + 0x16];
                if ((p[0x1f] & 0x1000000) == 0) {
                    register uint *n asm("$2");
                    n = (uint *)p[0x2f];
                    gte_ldv3_color(n);
                    *(uint *)(out + 0x2c) = neutral;
                    gte_ldrgb((void *)(out + 0x2c));
                    gte_nop();
                    gte_nop();
                    gte_ncct();
                    gte_strgb3(out + 8, out + 0x14, out + 0x20);
                    gte_ldv0((void *)*(uint *)(p[0x2f] + 0xc));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(out + 0x2c);
                } else {
                    *(uint *)(out + 0x2c) = neutral;
                    *(uint *)(out + 0x20) = neutral;
                    *(uint *)(out + 0x14) = neutral;
                    *(uint *)(out + 8) = neutral;
                }
                kaze_tmd_divide_quad_gtex_68b4(out, p, 0);
            }
next:
            p[0x2d] = p[0x2d] + 0x10;
            p[0x2e] = p[0x2e] + 0x10;
            p[0x2f] = p[0x2f] + 0x10;
        } while (p[0x25] < p[1]);
    }
}

extern void kaze_tmd_midpoint_vertex_6d58(V12 *, V12 *, V12 *);
extern void kaze_tmd_emit_poly_f3_7f28(V12 *, V12 *, V12 *, u32 *);

/**
 * kaze_tmd_divide_tri_flat_4db8  (library, 0x80044db8)
 *
 * Recursive flat-triangle subdivider: splits into 4 sub-tris via midpoint
 * helper kaze_tmd_midpoint_vertex_6d58, screen-tests with
 * kaze_tmd_subdiv_screen_test_tri_7018, emits POLY_F3 leaves via
 * kaze_tmd_emit_poly_f3_7f28 or recurses.
 */

void kaze_tmd_divide_tri_flat_4db8(V12 *, u32 *, int);

void kaze_tmd_divide_tri_flat_4db8(V12 *tri, u32 *ctx, int depth)
{
    V12 *base;
    V12 *work;
    u32 *screen;
    V12 *v1p;
    V12 *v2p;
    V12 *m01;
    V12 *m02;

    base = tri;
    gte_ldv3(base, &tri[1], &tri[2]);
    tri += 3;
    screen = ctx + 0x34;
    gte_rtpt();
    work = tri;
    gte_stflg(ctx + 0x2b);
    gte_stsxy3_f3(screen);
    gte_stsz3(ctx + 0x26, ctx + 0x27, ctx + 0x28);

    if (kaze_tmd_subdiv_screen_test_tri_7018(screen) != 0) {
        return;
    }

    ctx[4] = ctx[5] + (((int)ctx[0x2a] >> (int)ctx[0]) << 2);

    if (depth >= 4) {
        if ((int)ctx[0x2b] >= 0) {
            goto emit;
        }
        return;
    }

    if ((int)ctx[0x2b] >= 0 &&
        *(s16 *)((char *)ctx + 0xc4) - *(s16 *)((char *)ctx + 0xc6) < 0xff &&
        *(s16 *)((char *)ctx + 0xc8) - *(s16 *)((char *)ctx + 0xca) < 0x7f) {
emit:
        *(u32 *)ctx[3] = (*(u32 *)ctx[4] & 0xffffff) | 0x4000000;
        ((u32 *)ctx[3])[1] = ((u32 *)base)[2];
        ((u32 *)ctx[3])[2] = screen[2];
        ((u32 *)ctx[3])[3] = screen[3];
        ((u32 *)ctx[3])[4] = screen[4];
        *(u32 *)ctx[4] = ctx[3] & 0xffffff;
        ctx[3] += 0x14;
        return;
    }

    depth += 1;
    *work = *base;
    m01 = &work[1];
    v1p = &base[1];
    kaze_tmd_midpoint_vertex_6d58(m01, base, v1p);
    m02 = &work[2];
    v2p = &base[2];
    kaze_tmd_midpoint_vertex_6d58(m02, base, v2p);
    kaze_tmd_divide_tri_flat_4db8(tri, ctx, depth);
    kaze_tmd_emit_poly_f3_7f28(base, v1p, m01, ctx);
    kaze_tmd_midpoint_vertex_6d58(work, v1p, v2p);
    kaze_tmd_divide_tri_flat_4db8(tri, ctx, depth);
    kaze_tmd_emit_poly_f3_7f28(v1p, v2p, work, ctx);
    work[1] = base[2];
    kaze_tmd_divide_tri_flat_4db8(tri, ctx, depth);
    kaze_tmd_emit_poly_f3_7f28(v2p, base, m02, ctx);
    work[2] = base[1];
    kaze_tmd_midpoint_vertex_6d58(m01, base, v1p);
    kaze_tmd_divide_tri_flat_4db8(tri, ctx, depth);
}

/**
 * kaze_tmd_divide_quad_flat_510c  (library, 0x8004510c)
 *
 * Recursive flat-quad subdivider: rtpt/rtps+stsz4, splits into 4 sub-quads
 * via kaze_tmd_midpoint_vertex_6d58 midpoints, tests with
 * kaze_tmd_subdiv_screen_test_quad_71bc, emits flat leaves via
 * kaze_tmd_emit_poly_f3_7f28 or recurses.
 */

void kaze_tmd_divide_quad_flat_510c(V12 *, u32 *, int);

#define gte_stsxy2(p) __asm__ volatile( \
    "swc2 $14, 0(%0)" :: "r"(p) : "memory")

void kaze_tmd_divide_quad_flat_510c(V12 *quad_arg, u32 *ctx_arg, int depth_arg)
{
    u32 *ctx;
    V12 *quad;
    V12 *next;
    V12 *work;
    u32 *screen;
    V12 *v1p;
    V12 *v2p;
    V12 *v3p;
    V12 *m01;
    V12 *m02;
    int depth;

    ctx = ctx_arg;
    quad = quad_arg;
    depth = depth_arg;
    gte_ldv3(quad, &quad[1], &quad[2]);
    next = &quad[4];
    screen = ctx + 0x34;
    gte_rtpt();
    work = next;
    gte_stflg(ctx + 0x2b);
    gte_stsxy3_f3(screen);
    gte_ldv0(&quad[3]);
    gte_rtps_pad();
    gte_stflg(ctx + 0x2c);
    gte_stsxy2(ctx + 0x39);
    ctx[0x2b] |= ctx[0x2c];
    gte_stsz4(ctx + 0x26, ctx + 0x27, ctx + 0x28, ctx + 0x29);

    if (kaze_tmd_subdiv_screen_test_quad_71bc(screen, ctx) != 0) {
        return;
    }

    ctx[4] = ctx[5] + (((int)ctx[0x2a] >> (int)ctx[0]) << 2);

    if (depth >= 4) {
        if ((int)ctx[0x2b] >= 0) {
            goto emit;
        }
        return;
    }

    if ((int)ctx[0x2b] >= 0 &&
        *(s16 *)((char *)ctx + 0xc4) - *(s16 *)((char *)ctx + 0xc6) < 0xff &&
        *(s16 *)((char *)ctx + 0xc8) - *(s16 *)((char *)ctx + 0xca) < 0x7f) {
emit:
        *(u32 *)ctx[3] = (*(u32 *)ctx[4] & 0xffffff) | 0x5000000;
        ((u32 *)ctx[3])[1] = ((u32 *)quad)[2];
        ((u32 *)ctx[3])[2] = screen[2];
        ((u32 *)ctx[3])[3] = screen[3];
        ((u32 *)ctx[3])[4] = screen[4];
        ((u32 *)ctx[3])[5] = screen[5];
        *(u32 *)ctx[4] = ctx[3] & 0xffffff;
        ctx[3] += 0x18;
        return;
    }

    m01 = &work[1];
    v1p = &quad[1];
    depth += 1;
    *work = *quad;
    kaze_tmd_midpoint_vertex_6d58(m01, quad, v1p);
    m02 = &work[2];
    v2p = &quad[2];
    kaze_tmd_midpoint_vertex_6d58(m02, quad, v2p);
    v3p = &quad[3];
    kaze_tmd_midpoint_vertex_6d58(&work[3], quad, v3p);
    kaze_tmd_divide_quad_flat_510c(next, ctx, depth);
    kaze_tmd_emit_poly_f3_7f28(quad, v1p, m01, ctx);
    *work = quad[1];
    kaze_tmd_midpoint_vertex_6d58(m02, v1p, v3p);
    kaze_tmd_divide_quad_flat_510c(next, ctx, depth);
    kaze_tmd_emit_poly_f3_7f28(v1p, v3p, m02, ctx);
    *work = quad[3];
    kaze_tmd_midpoint_vertex_6d58(m01, v2p, v3p);
    kaze_tmd_divide_quad_flat_510c(next, ctx, depth);
    kaze_tmd_emit_poly_f3_7f28(v3p, v2p, m01, ctx);
    *work = quad[2];
    kaze_tmd_midpoint_vertex_6d58(m02, quad, v2p);
    kaze_tmd_divide_quad_flat_510c(next, ctx, depth);
    kaze_tmd_emit_poly_f3_7f28(v2p, quad, m02, ctx);
    __asm__ volatile("" :: "r"(next));
    __asm__ volatile("" :: "r"(next));
    __asm__ volatile("" :: "r"(next));
}

extern void kaze_tmd_midpoint_vertex_rgb_6dd4(V12 *, V12 *, V12 *);
extern void kaze_tmd_emit_poly_g3_7fd4(V12 *, V12 *, V12 *, u32 *);

/**
 * kaze_tmd_divide_tri_gouraud_5530  (library, 0x80045530)
 *
 * Recursive gouraud-triangle subdivider: midpoints vertices+RGB via
 * kaze_tmd_midpoint_vertex_rgb_6dd4, screen-tests with
 * kaze_tmd_subdiv_screen_test_tri_g_73dc, emits POLY_G3 leaves via
 * kaze_tmd_emit_poly_g3_7fd4 or recurses.
 */

void kaze_tmd_divide_tri_gouraud_5530(V12 *, u32 *, int);

void kaze_tmd_divide_tri_gouraud_5530(V12 *tri, u32 *ctx, int depth)
{
    V12 *base;
    V12 *work;
    u32 *screen;
    V12 *v1p;
    V12 *v2p;
    V12 *m01;
    V12 *m02;

    base = tri;
    gte_ldv3(base, &tri[1], &tri[2]);
    tri += 3;
    screen = ctx + 0x34;
    gte_rtpt();
    work = tri;
    gte_stflg(ctx + 0x2b);
    gte_stsxy3_g3(screen);
    gte_stsz3(ctx + 0x26, ctx + 0x27, ctx + 0x28);

    if (kaze_tmd_subdiv_screen_test_tri_g_73dc(screen) != 0) {
        return;
    }

    ctx[4] = ctx[5] + (((int)ctx[0x2a] >> (int)ctx[0]) << 2);

    if (depth >= 4) {
        if ((int)ctx[0x2b] >= 0) {
            goto emit;
        }
        return;
    }

    if ((int)ctx[0x2b] >= 0 &&
        *(s16 *)((char *)ctx + 0xc4) - *(s16 *)((char *)ctx + 0xc6) < 0xff &&
        *(s16 *)((char *)ctx + 0xc8) - *(s16 *)((char *)ctx + 0xca) < 0x7f) {
emit:
        *(u32 *)ctx[3] = (*(u32 *)ctx[4] & 0xffffff) | 0x6000000;
        ((u32 *)ctx[3])[1] = ((u32 *)base)[2];
        ((u32 *)ctx[3])[2] = screen[2];
        ((u32 *)ctx[3])[3] = ((u32 *)base)[5];
        ((u32 *)ctx[3])[4] = screen[4];
        ((u32 *)ctx[3])[5] = ((u32 *)base)[8];
        ((u32 *)ctx[3])[6] = screen[6];
        *(u32 *)ctx[4] = ctx[3] & 0xffffff;
        ctx[3] += 0x1C;
        return;
    }

    depth += 1;
    *work = *base;
    m01 = &work[1];
    v1p = &base[1];
    kaze_tmd_midpoint_vertex_rgb_6dd4(m01, base, v1p);
    m02 = &work[2];
    v2p = &base[2];
    kaze_tmd_midpoint_vertex_rgb_6dd4(m02, base, v2p);
    kaze_tmd_divide_tri_gouraud_5530(tri, ctx, depth);
    kaze_tmd_emit_poly_g3_7fd4(base, v1p, m01, ctx);
    kaze_tmd_midpoint_vertex_rgb_6dd4(work, v1p, v2p);
    kaze_tmd_divide_tri_gouraud_5530(tri, ctx, depth);
    kaze_tmd_emit_poly_g3_7fd4(v1p, v2p, work, ctx);
    work[1] = base[2];
    kaze_tmd_divide_tri_gouraud_5530(tri, ctx, depth);
    kaze_tmd_emit_poly_g3_7fd4(v2p, base, m02, ctx);
    work[2] = base[1];
    kaze_tmd_midpoint_vertex_rgb_6dd4(m01, base, v1p);
    kaze_tmd_divide_tri_gouraud_5530(tri, ctx, depth);
}

/**
 * kaze_tmd_divide_quad_gouraud_58a4  (library, 0x800458a4)
 *
 * Recursive gouraud-quad subdivider: midpoints vertices+RGB via
 * kaze_tmd_midpoint_vertex_rgb_6dd4, tests with
 * kaze_tmd_subdiv_screen_test_quad_g_7580, emits POLY_G3 leaves via
 * kaze_tmd_emit_poly_g3_7fd4 or recurses.
 */

void kaze_tmd_divide_quad_gouraud_58a4(V12 *, u32 *, int);

#define gte_stsxy2(p) __asm__ volatile( \
    "swc2 $14, 0(%0)" :: "r"(p) : "memory")

void kaze_tmd_divide_quad_gouraud_58a4(V12 *quad_arg, u32 *ctx_arg, int depth_arg)
{
    u32 *ctx;
    V12 *quad;
    V12 *next;
    V12 *work;
    u32 *screen;
    V12 *v1p;
    V12 *v2p;
    V12 *v3p;
    V12 *m01;
    V12 *m02;
    int depth;

    ctx = ctx_arg;
    quad = quad_arg;
    depth = depth_arg;
    gte_ldv3(quad, &quad[1], &quad[2]);
    next = &quad[4];
    screen = ctx + 0x34;
    gte_rtpt();
    work = next;
    gte_stflg(ctx + 0x2b);
    gte_stsxy3_g3(screen);
    gte_ldv0(&quad[3]);
    gte_rtps_pad();
    gte_stflg(ctx + 0x2c);
    gte_stsxy2(ctx + 0x3c);
    ctx[0x2b] |= ctx[0x2c];
    gte_stsz4(ctx + 0x26, ctx + 0x27, ctx + 0x28, ctx + 0x29);

    if (kaze_tmd_subdiv_screen_test_quad_g_7580(screen, ctx) != 0) {
        return;
    }

    ctx[4] = ctx[5] + (((int)ctx[0x2a] >> (int)ctx[0]) << 2);

    if (depth >= 4) {
        if ((int)ctx[0x2b] >= 0) {
            goto emit;
        }
        return;
    }

    if ((int)ctx[0x2b] >= 0 &&
        *(s16 *)((char *)ctx + 0xc4) - *(s16 *)((char *)ctx + 0xc6) < 0xff &&
        *(s16 *)((char *)ctx + 0xc8) - *(s16 *)((char *)ctx + 0xca) < 0x7f) {
emit:
        *(u32 *)ctx[3] = (*(u32 *)ctx[4] & 0xffffff) | 0x8000000;
        ((u32 *)ctx[3])[1] = ((u32 *)quad)[2];
        ((u32 *)ctx[3])[2] = screen[2];
        ((u32 *)ctx[3])[3] = ((u32 *)quad)[5];
        ((u32 *)ctx[3])[4] = screen[4];
        ((u32 *)ctx[3])[5] = ((u32 *)quad)[8];
        ((u32 *)ctx[3])[6] = screen[6];
        ((u32 *)ctx[3])[7] = ((u32 *)quad)[11];
        ((u32 *)ctx[3])[8] = screen[8];
        *(u32 *)ctx[4] = ctx[3] & 0xffffff;
        ctx[3] += 0x24;
        return;
    }

    m01 = &work[1];
    v1p = &quad[1];
    depth += 1;
    *work = *quad;
    kaze_tmd_midpoint_vertex_rgb_6dd4(m01, quad, v1p);
    m02 = &work[2];
    v2p = &quad[2];
    kaze_tmd_midpoint_vertex_rgb_6dd4(m02, quad, v2p);
    v3p = &quad[3];
    kaze_tmd_midpoint_vertex_rgb_6dd4(&work[3], quad, v3p);
    kaze_tmd_divide_quad_gouraud_58a4(next, ctx, depth);
    kaze_tmd_emit_poly_g3_7fd4(quad, v1p, m01, ctx);
    *work = quad[1];
    kaze_tmd_midpoint_vertex_rgb_6dd4(m02, v1p, v3p);
    kaze_tmd_divide_quad_gouraud_58a4(next, ctx, depth);
    kaze_tmd_emit_poly_g3_7fd4(v1p, v3p, m02, ctx);
    *work = quad[3];
    kaze_tmd_midpoint_vertex_rgb_6dd4(m01, v2p, v3p);
    kaze_tmd_divide_quad_gouraud_58a4(next, ctx, depth);
    kaze_tmd_emit_poly_g3_7fd4(v3p, v2p, m01, ctx);
    *work = quad[2];
    kaze_tmd_midpoint_vertex_rgb_6dd4(m02, quad, v2p);
    kaze_tmd_divide_quad_gouraud_58a4(next, ctx, depth);
    kaze_tmd_emit_poly_g3_7fd4(v2p, quad, m02, ctx);
    __asm__ volatile("" :: "r"(next));
    __asm__ volatile("" :: "r"(next));
    __asm__ volatile("" :: "r"(next));
}

extern void kaze_tmd_midpoint_vertex_uv_6e88(V12 *, V12 *, V12 *);
extern void kaze_tmd_emit_poly_ft3_8094(V12 *, V12 *, V12 *, u32 *);

/**
 * kaze_tmd_divide_tri_ftex_5cf8  (library, 0x80045cf8)
 *
 * Recursive textured-triangle subdivider: midpoints vertices+UV via
 * kaze_tmd_midpoint_vertex_uv_6e88, screen-tests with
 * kaze_tmd_subdiv_screen_test_tri_ft_77a0, emits POLY_FT3 leaves via
 * kaze_tmd_emit_poly_ft3_8094 or recurses.
 */

void kaze_tmd_divide_tri_ftex_5cf8(V12 *, u32 *, int);

void kaze_tmd_divide_tri_ftex_5cf8(V12 *tri, u32 *ctx, int depth)
{
    V12 *base;
    V12 *work;
    u32 *screen;
    V12 *v1p;
    V12 *v2p;
    V12 *m01;
    V12 *m02;

    base = tri;
    gte_ldv3(base, &tri[1], &tri[2]);
    tri += 3;
    screen = ctx + 0x34;
    gte_rtpt();
    work = tri;
    gte_stflg(ctx + 0x2b);
    gte_stsxy3_g3(screen);
    gte_stsz3(ctx + 0x26, ctx + 0x27, ctx + 0x28);

    if (kaze_tmd_subdiv_screen_test_tri_ft_77a0(screen) != 0) {
        return;
    }

    ctx[4] = ctx[5] + (((int)ctx[0x2a] >> (int)ctx[0]) << 2);

    if (depth >= 4) {
        if ((int)ctx[0x2b] >= 0) {
            goto emit;
        }
        return;
    }

    if ((int)ctx[0x2b] >= 0 &&
        *(s16 *)((char *)ctx + 0xc4) - *(s16 *)((char *)ctx + 0xc6) < 0xff &&
        *(s16 *)((char *)ctx + 0xc8) - *(s16 *)((char *)ctx + 0xca) < 0x7f) {
emit:
        *(u32 *)ctx[3] = (*(u32 *)ctx[4] & 0xffffff) | 0x7000000;
        ((u32 *)ctx[3])[1] = ((u32 *)base)[2];
        ((u32 *)ctx[3])[2] = screen[2];
        ((u32 *)ctx[3])[3] = *(u16 *)((char *)base + 6) | (*(u16 *)((char *)ctx + 0xcc) << 16);
        ((u32 *)ctx[3])[4] = screen[4];
        ((u32 *)ctx[3])[5] = *(u16 *)((char *)base + 0x12) | (*(u16 *)((char *)ctx + 0xce) << 16);
        ((u32 *)ctx[3])[6] = screen[6];
        ((u32 *)ctx[3])[7] = *(u16 *)((char *)base + 0x1e);
        *(u32 *)ctx[4] = ctx[3] & 0xffffff;
        ctx[3] += 0x20;
        return;
    }

    depth += 1;
    *work = *base;
    m01 = &work[1];
    v1p = &base[1];
    kaze_tmd_midpoint_vertex_uv_6e88(m01, base, v1p);
    m02 = &work[2];
    v2p = &base[2];
    kaze_tmd_midpoint_vertex_uv_6e88(m02, base, v2p);
    kaze_tmd_divide_tri_ftex_5cf8(tri, ctx, depth);
    kaze_tmd_emit_poly_ft3_8094(base, v1p, m01, ctx);
    kaze_tmd_midpoint_vertex_uv_6e88(work, v1p, v2p);
    kaze_tmd_divide_tri_ftex_5cf8(tri, ctx, depth);
    kaze_tmd_emit_poly_ft3_8094(v1p, v2p, work, ctx);
    work[1] = base[2];
    kaze_tmd_divide_tri_ftex_5cf8(tri, ctx, depth);
    kaze_tmd_emit_poly_ft3_8094(v2p, base, m02, ctx);
    work[2] = base[1];
    kaze_tmd_midpoint_vertex_uv_6e88(m01, base, v1p);
    kaze_tmd_divide_tri_ftex_5cf8(tri, ctx, depth);
}

/**
 * kaze_tmd_divide_quad_ftex_608c  (library, 0x8004608c)
 *
 * Recursive textured-quad subdivider: midpoints vertices+UV via
 * kaze_tmd_midpoint_vertex_uv_6e88, tests with
 * kaze_tmd_subdiv_screen_test_quad_ft_7944, emits POLY_FT3 leaves via
 * kaze_tmd_emit_poly_ft3_8094 or recurses.
 */

void kaze_tmd_divide_quad_ftex_608c(V12 *, u32 *, int);

#define gte_stsxy2(p) __asm__ volatile( \
    "swc2 $14, 0(%0)" :: "r"(p) : "memory")

void kaze_tmd_divide_quad_ftex_608c(V12 *quad_arg, u32 *ctx_arg, int depth_arg)
{
    u32 *ctx;
    V12 *quad;
    V12 *next;
    V12 *work;
    u32 *screen;
    V12 *v1p;
    V12 *v2p;
    V12 *v3p;
    V12 *m01;
    V12 *m02;
    int depth;

    ctx = ctx_arg;
    quad = quad_arg;
    depth = depth_arg;
    gte_ldv3(quad, &quad[1], &quad[2]);
    next = &quad[4];
    screen = ctx + 0x34;
    gte_rtpt();
    work = next;
    gte_stflg(ctx + 0x2b);
    gte_stsxy3_g3(screen);
    gte_ldv0(&quad[3]);
    gte_rtps_pad();
    gte_stflg(ctx + 0x2c);
    gte_stsxy2(ctx + 0x3c);
    ctx[0x2b] |= ctx[0x2c];
    gte_stsz4(ctx + 0x26, ctx + 0x27, ctx + 0x28, ctx + 0x29);

    if (kaze_tmd_subdiv_screen_test_quad_ft_7944(screen, ctx) != 0) {
        return;
    }

    ctx[4] = ctx[5] + (((int)ctx[0x2a] >> (int)ctx[0]) << 2);

    if (depth >= 4) {
        if ((int)ctx[0x2b] >= 0) {
            goto emit;
        }
        return;
    }

    if ((int)ctx[0x2b] >= 0 &&
        *(s16 *)((char *)ctx + 0xc4) - *(s16 *)((char *)ctx + 0xc6) < 0xff &&
        *(s16 *)((char *)ctx + 0xc8) - *(s16 *)((char *)ctx + 0xca) < 0x7f) {
emit:
        *(u32 *)ctx[3] = (*(u32 *)ctx[4] & 0xffffff) | 0x9000000;
        ((u32 *)ctx[3])[1] = ((u32 *)quad)[2];
        ((u32 *)ctx[3])[2] = screen[2];
        ((u32 *)ctx[3])[3] = *(u16 *)((char *)quad + 6) | (*(u16 *)((char *)ctx + 0xcc) << 16);
        ((u32 *)ctx[3])[4] = screen[4];
        ((u32 *)ctx[3])[5] = *(u16 *)((char *)quad + 0x12) | (*(u16 *)((char *)ctx + 0xce) << 16);
        ((u32 *)ctx[3])[6] = screen[6];
        ((u32 *)ctx[3])[7] = *(u16 *)((char *)quad + 0x1e);
        ((u32 *)ctx[3])[8] = screen[8];
        ((u32 *)ctx[3])[9] = *(u16 *)((char *)quad + 0x2a);
        *(u32 *)ctx[4] = ctx[3] & 0xffffff;
        ctx[3] += 0x28;
        return;
    }

    m01 = &work[1];
    v1p = &quad[1];
    depth += 1;
    *work = *quad;
    kaze_tmd_midpoint_vertex_uv_6e88(m01, quad, v1p);
    m02 = &work[2];
    v2p = &quad[2];
    kaze_tmd_midpoint_vertex_uv_6e88(m02, quad, v2p);
    v3p = &quad[3];
    kaze_tmd_midpoint_vertex_uv_6e88(&work[3], quad, v3p);
    kaze_tmd_divide_quad_ftex_608c(next, ctx, depth);
    kaze_tmd_emit_poly_ft3_8094(quad, v1p, m01, ctx);
    *work = quad[1];
    kaze_tmd_midpoint_vertex_uv_6e88(m02, v1p, v3p);
    kaze_tmd_divide_quad_ftex_608c(next, ctx, depth);
    kaze_tmd_emit_poly_ft3_8094(v1p, v3p, m02, ctx);
    *work = quad[3];
    kaze_tmd_midpoint_vertex_uv_6e88(m01, v2p, v3p);
    kaze_tmd_divide_quad_ftex_608c(next, ctx, depth);
    kaze_tmd_emit_poly_ft3_8094(v3p, v2p, m01, ctx);
    *work = quad[2];
    kaze_tmd_midpoint_vertex_uv_6e88(m02, quad, v2p);
    kaze_tmd_divide_quad_ftex_608c(next, ctx, depth);
    kaze_tmd_emit_poly_ft3_8094(v2p, quad, m02, ctx);
    __asm__ volatile("" :: "r"(next));
    __asm__ volatile("" :: "r"(next));
    __asm__ volatile("" :: "r"(next));
}

extern void kaze_tmd_midpoint_vertex_uv_rgb_6f34(V12 *, V12 *, V12 *);
extern void kaze_tmd_emit_poly_gt3_8178(V12 *, V12 *, V12 *, u32 *);

/**
 * kaze_tmd_divide_tri_gtex_6500  (library, 0x80046500)
 *
 * Recursive gouraud-textured triangle subdivider: midpoints vertices+UV+RGB
 * via kaze_tmd_midpoint_vertex_uv_rgb_6f34, tests with
 * kaze_tmd_subdiv_screen_test_tri_gt_7b64, emits POLY_GT3 leaves via
 * kaze_tmd_emit_poly_gt3_8178 or recurses.
 */

void kaze_tmd_divide_tri_gtex_6500(V12 *, u32 *, int);

void kaze_tmd_divide_tri_gtex_6500(V12 *tri, u32 *ctx, int depth)
{
    V12 *base;
    V12 *work;
    u32 *screen;
    V12 *v1p;
    V12 *v2p;
    V12 *m01;
    V12 *m02;

    base = tri;
    gte_ldv3(base, &tri[1], &tri[2]);
    tri += 3;
    screen = ctx + 0x34;
    gte_rtpt();
    work = tri;
    gte_stflg(ctx + 0x2b);
    gte_stsxy3_gt3(screen);
    gte_stsz3(ctx + 0x26, ctx + 0x27, ctx + 0x28);

    if (kaze_tmd_subdiv_screen_test_tri_gt_7b64(screen) != 0) {
        return;
    }

    ctx[4] = ctx[5] + (((int)ctx[0x2a] >> (int)ctx[0]) << 2);

    if (depth >= 4) {
        if ((int)ctx[0x2b] >= 0) {
            goto emit;
        }
        return;
    }

    if ((int)ctx[0x2b] >= 0 &&
        *(s16 *)((char *)ctx + 0xc4) - *(s16 *)((char *)ctx + 0xc6) < 0xff &&
        *(s16 *)((char *)ctx + 0xc8) - *(s16 *)((char *)ctx + 0xca) < 0x7f) {
emit:
        *(u32 *)ctx[3] = (*(u32 *)ctx[4] & 0xffffff) | 0x9000000;
        ((u32 *)ctx[3])[1] = ((u32 *)base)[2];
        ((u32 *)ctx[3])[2] = screen[2];
        ((u32 *)ctx[3])[3] = *(u16 *)((char *)base + 6) | (*(u16 *)((char *)ctx + 0xcc) << 16);
        ((u32 *)ctx[3])[4] = ((u32 *)base)[5];
        ((u32 *)ctx[3])[5] = screen[5];
        ((u32 *)ctx[3])[6] = *(u16 *)((char *)base + 0x12) | (*(u16 *)((char *)ctx + 0xce) << 16);
        ((u32 *)ctx[3])[7] = ((u32 *)base)[8];
        ((u32 *)ctx[3])[8] = screen[8];
        ((u32 *)ctx[3])[9] = *(u16 *)((char *)base + 0x1e);
        *(u32 *)ctx[4] = ctx[3] & 0xffffff;
        ctx[3] += 0x28;
        return;
    }

    depth += 1;
    *work = *base;
    m01 = &work[1];
    v1p = &base[1];
    kaze_tmd_midpoint_vertex_uv_rgb_6f34(m01, base, v1p);
    m02 = &work[2];
    v2p = &base[2];
    kaze_tmd_midpoint_vertex_uv_rgb_6f34(m02, base, v2p);
    kaze_tmd_divide_tri_gtex_6500(tri, ctx, depth);
    kaze_tmd_emit_poly_gt3_8178(base, v1p, m01, ctx);
    kaze_tmd_midpoint_vertex_uv_rgb_6f34(work, v1p, v2p);
    kaze_tmd_divide_tri_gtex_6500(tri, ctx, depth);
    kaze_tmd_emit_poly_gt3_8178(v1p, v2p, work, ctx);
    work[1] = base[2];
    kaze_tmd_divide_tri_gtex_6500(tri, ctx, depth);
    kaze_tmd_emit_poly_gt3_8178(v2p, base, m02, ctx);
    work[2] = base[1];
    kaze_tmd_midpoint_vertex_uv_rgb_6f34(m01, base, v1p);
    kaze_tmd_divide_tri_gtex_6500(tri, ctx, depth);
}

/**
 * kaze_tmd_divide_quad_gtex_68b4  (library, 0x800468b4)
 *
 * Recursive gouraud-textured quad subdivider: midpoints vertices+UV+RGB via
 * kaze_tmd_midpoint_vertex_uv_rgb_6f34, tests with
 * kaze_tmd_subdiv_screen_test_quad_gt_7d08, emits POLY_GT3 leaves via
 * kaze_tmd_emit_poly_gt3_8178 or recurses.
 */

void kaze_tmd_divide_quad_gtex_68b4(V12 *, u32 *, int);

#define gte_stsxy2(p) __asm__ volatile( \
    "swc2 $14, 0(%0)" :: "r"(p) : "memory")

void kaze_tmd_divide_quad_gtex_68b4(V12 *quad_arg, u32 *ctx_arg, int depth_arg)
{
    u32 *ctx;
    V12 *quad;
    V12 *next;
    V12 *work;
    u32 *screen;
    V12 *v1p;
    V12 *v2p;
    V12 *v3p;
    V12 *m01;
    V12 *m02;
    int depth;

    ctx = ctx_arg;
    quad = quad_arg;
    depth = depth_arg;
    gte_ldv3(quad, &quad[1], &quad[2]);
    next = &quad[4];
    screen = ctx + 0x34;
    gte_rtpt();
    work = next;
    gte_stflg(ctx + 0x2b);
    gte_stsxy3_gt3(screen);
    gte_ldv0(&quad[3]);
    gte_rtps_pad();
    gte_stflg(ctx + 0x2c);
    gte_stsxy2(ctx + 0x3f);
    ctx[0x2b] |= ctx[0x2c];
    gte_stsz4(ctx + 0x26, ctx + 0x27, ctx + 0x28, ctx + 0x29);

    if (kaze_tmd_subdiv_screen_test_quad_gt_7d08(screen, ctx) != 0) {
        return;
    }

    ctx[4] = ctx[5] + (((int)ctx[0x2a] >> (int)ctx[0]) << 2);

    if (depth >= 4) {
        if ((int)ctx[0x2b] >= 0) {
            goto emit;
        }
        return;
    }

    if ((int)ctx[0x2b] >= 0 &&
        *(s16 *)((char *)ctx + 0xc4) - *(s16 *)((char *)ctx + 0xc6) < 0xff &&
        *(s16 *)((char *)ctx + 0xc8) - *(s16 *)((char *)ctx + 0xca) < 0x7f) {
emit:
        *(u32 *)ctx[3] = (*(u32 *)ctx[4] & 0xffffff) | 0xc000000;
        ((u32 *)ctx[3])[1] = ((u32 *)quad)[2];
        ((u32 *)ctx[3])[2] = screen[2];
        ((u32 *)ctx[3])[3] = *(u16 *)((char *)quad + 6) | (*(u16 *)((char *)ctx + 0xcc) << 16);
        ((u32 *)ctx[3])[4] = ((u32 *)quad)[5];
        ((u32 *)ctx[3])[5] = screen[5];
        ((u32 *)ctx[3])[6] = *(u16 *)((char *)quad + 0x12) | (*(u16 *)((char *)ctx + 0xce) << 16);
        ((u32 *)ctx[3])[7] = ((u32 *)quad)[8];
        ((u32 *)ctx[3])[8] = screen[8];
        ((u32 *)ctx[3])[9] = *(u16 *)((char *)quad + 0x1e);
        ((u32 *)ctx[3])[10] = ((u32 *)quad)[11];
        ((u32 *)ctx[3])[11] = screen[11];
        ((u32 *)ctx[3])[12] = *(u16 *)((char *)quad + 0x2a);
        *(u32 *)ctx[4] = ctx[3] & 0xffffff;
        ctx[3] += 0x34;
        return;
    }

    m01 = &work[1];
    v1p = &quad[1];
    depth += 1;
    *work = *quad;
    kaze_tmd_midpoint_vertex_uv_rgb_6f34(m01, quad, v1p);
    m02 = &work[2];
    v2p = &quad[2];
    kaze_tmd_midpoint_vertex_uv_rgb_6f34(m02, quad, v2p);
    v3p = &quad[3];
    kaze_tmd_midpoint_vertex_uv_rgb_6f34(&work[3], quad, v3p);
    kaze_tmd_divide_quad_gtex_68b4(next, ctx, depth);
    kaze_tmd_emit_poly_gt3_8178(quad, v1p, m01, ctx);
    *work = quad[1];
    kaze_tmd_midpoint_vertex_uv_rgb_6f34(m02, v1p, v3p);
    kaze_tmd_divide_quad_gtex_68b4(next, ctx, depth);
    kaze_tmd_emit_poly_gt3_8178(v1p, v3p, m02, ctx);
    *work = quad[3];
    kaze_tmd_midpoint_vertex_uv_rgb_6f34(m01, v2p, v3p);
    kaze_tmd_divide_quad_gtex_68b4(next, ctx, depth);
    kaze_tmd_emit_poly_gt3_8178(v3p, v2p, m01, ctx);
    *work = quad[2];
    kaze_tmd_midpoint_vertex_uv_rgb_6f34(m02, quad, v2p);
    kaze_tmd_divide_quad_gtex_68b4(next, ctx, depth);
    kaze_tmd_emit_poly_gt3_8178(v2p, quad, m02, ctx);
    __asm__ volatile("" :: "r"(next));
    __asm__ volatile("" :: "r"(next));
    __asm__ volatile("" :: "r"(next));
}
