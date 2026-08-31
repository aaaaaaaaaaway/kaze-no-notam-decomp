#include "types.h"
typedef struct {
    short x, y, z;
    u8 pad6, pad7;
    u8 r, g, b, w;
} Vtx2;
typedef struct {
    char c[4];
} S4;
typedef struct {
    short x, y, z;
    u8 u, v;
} Vtx;

/* Tail is an unaligned 32-bit copy (ulw/usw). cc1 emits "ulw; #nop; usw" and
   the original ASPSX kept the load-delay nop; maspsx does not know ulw is a
   load and drops it, so the copy is written as inline asm with the explicit
   nop. Codegen is otherwise plain -O1 C. */

/**
 * kaze_tmd_midpoint_vertex_6d58  (library, 0x80046d58)
 *
 * Subdivision midpoint helper for flat prims: averages two vertices' x/y/z
 * shorts and copies the trailing word with unaligned load/store.
 */

void kaze_tmd_midpoint_vertex_6d58(short *dst, short *a, short *b)
{
    int t;

    dst[0] = (a[0] + b[0]) / 2;
    dst[1] = (a[1] + b[1]) / 2;
    dst[2] = (a[2] + b[2]) / 2;
    __asm__ __volatile__("ulw\t%0,8(%1)\n\tnop\n\tusw\t%0,8(%2)"
                         : "=&r"(t)
                         : "r"(a), "r"(dst)
                         : "memory");
}

/**
 * kaze_tmd_midpoint_vertex_rgb_6dd4  (library, 0x80046dd4)
 *
 * Subdivision midpoint helper for gouraud prims: averages x/y/z and per-
 * vertex RGB bytes, copies the code byte at +0xb.
 */

void kaze_tmd_midpoint_vertex_rgb_6dd4(Vtx2 *dst, Vtx2 *a, Vtx2 *b)
{
    dst->x = (a->x + b->x) / 2;
    dst->y = (a->y + b->y) / 2;
    dst->z = (a->z + b->z) / 2;
    dst->r = (a->r + b->r) >> 1;
    dst->g = (a->g + b->g) >> 1;
    dst->b = (a->b + b->b) >> 1;
    dst->w = a->w;
}

/**
 * kaze_tmd_midpoint_vertex_uv_6e88  (library, 0x80046e88)
 *
 * Subdivision midpoint helper for textured flat prims: averages x/y/z and
 * U/V bytes, copies the trailing word with unaligned access.
 */

void kaze_tmd_midpoint_vertex_uv_6e88(Vtx *dst, Vtx *a, Vtx *b)
{
    dst->x = (a->x + b->x) / 2;
    dst->y = (a->y + b->y) / 2;
    dst->z = (a->z + b->z) / 2;
    dst->u = (a->u + b->u) >> 1;
    dst->v = (a->v + b->v) >> 1;
    *(S4 *)((int)dst + 8) = *(S4 *)((int)a + 8);
}
