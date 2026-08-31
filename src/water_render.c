
#include "types.h"
typedef struct {
    short vx, vy, vz;
    unsigned short pad;
} SVECTOR;
typedef struct {
    u8 b[8];
} CopyBlk;
typedef struct 
{
  char p0[8];
  s16 x0;
  s16 y0;
  char p1[4];
  s16 x1;
  s16 y1;
  char p2[4];
  s16 x2;
  s16 y2;
  char p3[4];
  s16 x3;
  s16 y3;
} Quad;
typedef struct 
{
  char p0[0x74];
  s16 bx0;
  s16 bx1;
  s16 by0;
  s16 by1;
  char p1[0xB8 - 0x7C];
  int f0;
  int f1;
  int f2;
  int f3;
} Target;

int kaze_quad_screen_clip_test();
extern void kaze_vertex_midpoint_lerp(SVECTOR *, SVECTOR *, SVECTOR *);
void kaze_gte_emit_textured_tri();
#define gte_rtpt() __asm__ volatile("cop2 0x280030")
#define gte_rtps() __asm__ volatile("cop2 0x180001")
#define gte_ldv3(x0, x1, x2)                                                   \
    __asm__ volatile("lwc2 $0, 0(%0)\n\t""lwc2 $1, 4(%0)\n\t""lwc2 $2, 0(%1)\n\t""lwc2 $3, 4(%1)\n\t""lwc2 $4, 0(%2)\n\t""lwc2 $5, 4(%2)" :: "r"(x0), "r"(x1), "r"(x2))
#define gte_ldv0(x) __asm__ volatile("lwc2 $0, 0(%0)\n\t""lwc2 $1, 4(%0)" :: "r"(x))
#define gte_stflg(p) __asm__ volatile("cfc2 $12, $31\n\tnop\n\tsw $12, 0(%0)" :: "r"(p) : "$12", "memory")
#define gte_stsxy(x) __asm__ volatile("swc2 $14, 0(%0)" :: "r"(x) : "memory")
#define gte_stsxy3_g3(x) __asm__ volatile("swc2 $12, 8(%0)\n\t""swc2 $13, 16(%0)\n\t""swc2 $14, 24(%0)" :: "r"(x) : "memory")
#define gte_stsz4(a, b, c, d) __asm__ volatile("swc2 $16, 0(%0)\n\t""swc2 $17, 0(%1)\n\t""swc2 $18, 0(%2)\n\t""swc2 $19, 0(%3)" :: "r"(a), "r"(b), "r"(c), "r"(d) : "memory")

/**
 * kaze_water_quad_subdivide_render  (render, 0x800301fc)
 *
 * Subdividing textured-quad renderer for the water pass: projects 4
 * SVECTOR+UV vertices, clip-tests via kaze_quad_screen_clip_test, splits at
 * midpoints (kaze_vertex_midpoint_lerp) when too large/clipped, emits
 * POLY_FT3 tris via kaze_gte_emit_textured_tri.
 */

void kaze_water_quad_subdivide_render(SVECTOR *verts_arg, int ctx_arg, int depth_arg)
{
    register int ctx __asm__("$21") = ctx_arg;
    register SVECTOR *verts __asm__("$22") = verts_arg;
    int depth = depth_arg;
    SVECTOR *pRec;
    SVECTOR *pDrv;
    u32 *r0;
    int shift_idx;
    register SVECTOR *v3 __asm__("$16");
    register SVECTOR *v1 __asm__("$17");
    SVECTOR *v2;
    register SVECTOR *mid_b __asm__("$19");
    register SVECTOR *mid_a __asm__("$20");

    gte_ldv3(verts, verts + 1, verts + 2);
    pRec = verts + 4;
    pDrv = pRec;
    gte_rtpt();
    *(short *)(ctx + 0x88) = verts->pad;
    r0 = (u32 *)(ctx + 0x7c);
    *(short *)(ctx + 0x90) = verts[1].pad;
    gte_stflg(ctx + 0xa8);
    gte_stsxy3_g3(r0);
    gte_ldv0(verts + 3);
    *(short *)(ctx + 0x98) = verts[2].pad;
    gte_rtps();
    *(short *)(ctx + 0xa0) = verts[3].pad;
    gte_stflg(ctx + 0xac);
    *(u32 *)(ctx + 0xa8) = *(u32 *)(ctx + 0xa8) | *(u32 *)(ctx + 0xac);
    gte_stsxy(ctx + 0x9c);
    gte_stsz4(ctx + 0xb8, ctx + 0xbc, ctx + 0xc0, ctx + 0xc4);
    if (*(int *)(ctx + 0xb8) > *(int *)(ctx + 0xbc)) {
        *(int *)(ctx + 0xac) = *(int *)(ctx + 0xb8);
    } else {
        *(int *)(ctx + 0xac) = *(int *)(ctx + 0xbc);
    }
    if (*(int *)(ctx + 0xc0) > *(int *)(ctx + 0xac)) {
        *(int *)(ctx + 0xac) = *(int *)(ctx + 0xc0);
    }
    if (*(int *)(ctx + 0xc4) > *(int *)(ctx + 0xac)) {
        *(int *)(ctx + 0xac) = *(int *)(ctx + 0xc4);
    }
    shift_idx = *(int *)(ctx + 0xac) >> *(int *)(ctx + 0xb4);
    if (shift_idx < 0) {
        shift_idx = shift_idx + 3;
    }
    *(int *)(ctx + 0x44) = *(int *)(ctx + 0x48) + (shift_idx >> 2) * 4;
    if (kaze_quad_screen_clip_test(r0, ctx) == 0) {
        enum { CLIP_DEPTH = 4 };
        if (({ register int t8c __asm__("$8") = depth; t8c; }) == CLIP_DEPTH) {
            if (*(int *)(ctx + 0xa8) >= 0) {
                goto LAB_emit;
            }
            return;
        }
        mid_a = pDrv + 1;
        if (-1 < *(int *)(ctx + 0xa8)) {
            v1 = verts + 1;
            if ((int)*(short *)(ctx + 0x74) - (int)*(short *)(ctx + 0x76) < 0xff &&
                (int)*(short *)(ctx + 0x78) - (int)*(short *)(ctx + 0x7a) < 0x7f) {
LAB_emit:
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
                *(int *)(ctx + 0x40) = *(int *)(ctx + 0x40) + 0x28;
                return;
            }
        }
        v1 = verts + 1;
        /*
         * Depth counter bump + first vertex copy. All constructs here are
         * zero-code or register pins (no code-emitting asm): the "m"(depth)
         * fence keeps depth stack-homed (a register-allocated depth steals a
         * callee-saved reg and rotates the file) and stops the copy from
         * interleaving with the increment; the $8 pin picks the target's
         * counter temp; the $6 pin lets the a2 arg move fill the counter
         * load's delay slot while a0/a1 stay down at the call.
         */
        {
            register SVECTOR *call_a2 __asm__("$6");
            register int t8 __asm__("$8");
            t8 = depth;
            call_a2 = v1;
            t8 = t8 + 1;
            depth = t8;
            __asm__ volatile("" :: "m"(depth));
            *(CopyBlk *)pDrv = *(CopyBlk *)verts;
            kaze_vertex_midpoint_lerp(mid_a, verts, call_a2);
        }

        /* block 1 */
        mid_b = pDrv + 2;
        v2 = verts + 2;
        kaze_vertex_midpoint_lerp(mid_b, verts, v2);
        v3 = verts + 3;
        kaze_vertex_midpoint_lerp(pDrv + 3, verts, v3);
        kaze_water_quad_subdivide_render(pRec, ctx, depth);
        kaze_gte_emit_textured_tri(verts, v1, mid_a, ctx);

        /* block 2 */
        *(CopyBlk *)pDrv = *(CopyBlk *)(verts + 1);
        kaze_vertex_midpoint_lerp(mid_b, v1, v3);
        kaze_water_quad_subdivide_render(pRec, ctx, depth);
        kaze_gte_emit_textured_tri(v1, v3, mid_b, ctx);

        /* block 3 */
        *(CopyBlk *)pDrv = *(CopyBlk *)(verts + 3);
        kaze_vertex_midpoint_lerp(mid_a, v3, v2);
        kaze_water_quad_subdivide_render(pRec, ctx, depth);
        kaze_gte_emit_textured_tri(v3, v2, mid_a, ctx);

        /* block 4 */
        *(CopyBlk *)pDrv = *(CopyBlk *)(verts + 2);
        kaze_vertex_midpoint_lerp(mid_b, verts, v2);
        kaze_water_quad_subdivide_render(pRec, ctx, depth);
        kaze_gte_emit_textured_tri(v2, verts, mid_b, ctx);
    }
    return;
}

#include "gte.h"

/**
 * kaze_gte_emit_textured_tri  (library, 0x800306ac)
 *
 * GTE helper: rtpt-projects three SVECTORs and emits one POLY_FT3 packet
 * (code 0x24) with UVs from vertex pad bytes and CLUT/TPage from ctx
 * +0x8a/+0x92 into the packet buffer at ctx+0x40, tag-linking through
 * ctx+0x44.
 */

void kaze_gte_emit_textured_tri(int v0, int v1, int v2, int ctx)
{
    u32 *r0;

    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(v0));
    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(v0));
    __asm__ volatile("lwc2 $2, 0(%0)" :: "r"(v1));
    __asm__ volatile("lwc2 $3, 4(%0)" :: "r"(v1));
    __asm__ volatile("lwc2 $4, 0(%0)" :: "r"(v2));
    __asm__ volatile("lwc2 $5, 4(%0)" :: "r"(v2));
    r0 = *(u32 **)(ctx + 0x40);
    r0[1] = *(u32 *)(ctx + 0x80);
    gte_rtpt();
    *((u8 *)r0 + 3) = 7;
    *((u8 *)r0 + 7) = 0x24;
    gte_stflg(ctx + 0xa8);
    if (*(int *)(ctx + 0xa8) >= 0) {
        __asm__ volatile("swc2 $12, 8(%0)" :: "r"(r0) : "memory");
        __asm__ volatile("swc2 $13, 16(%0)" :: "r"(r0) : "memory");
        __asm__ volatile("swc2 $14, 24(%0)" :: "r"(r0) : "memory");
        *(u16 *)((u8 *)r0 + 0xc) = *(u16 *)(v0 + 6);
        *(u16 *)((u8 *)r0 + 0x14) = *(u16 *)(v1 + 6);
        *(u16 *)((u8 *)r0 + 0x1c) = *(u16 *)(v2 + 6);
        *(u16 *)((u8 *)r0 + 0x16) = *(u16 *)(ctx + 0x92);
        *(u16 *)((u8 *)r0 + 0xe) = *(u16 *)(ctx + 0x8a);
        *r0 = (**(u32 **)(ctx + 0x44) & 0xffffff) | 0x7000000;
        **(u32 **)(ctx + 0x44) = (u32)r0 & 0xffffff;
        *(int *)(ctx + 0x40) += 0x20;
    }
}

/**
 * kaze_quad_screen_clip_test  (render, 0x80030790)
 *
 * Screen clip/size test for a projected quad: builds min/max X into
 * ctx+0x74/76 and Y into +0x78/7a from the four SXYs; returns 1
 * (reject/subdivide) when any SZ > 0x7c or the bbox is outside 0..0x140 x
 * 0..0xf0, else 0.
 */

int kaze_quad_screen_clip_test(Quad *q, Target *t)
{
  int ret;
  if ((((t->f0 >= 0x7D) || (t->f1 >= 0x7D)) || (t->f2 >= 0x7D)) || (t->f3 >= 0x7D))
  {
    if (q->x1 < q->x0)
    {
      t->bx0 = q->x0;
      t->bx1 = q->x1;
    }
    else
    {
      t->bx1 = q->x0;
      t->bx0 = q->x1;
    }
    if (t->bx0 < q->x2)
    {
      t->bx0 = q->x2;
    }
    else
      if (q->x2 < t->bx1)
    {
      t->bx1 = q->x2;
    }
    if (t->bx0 < q->x3)
    {
      t->bx0 = q->x3;
    }
    else
      if (q->x3 < t->bx1)
    {
      t->bx1 = q->x3;
    }
    if (t->bx0 < 0)
    {
      ret = 1;
      goto done;
    }
    if (t->bx1 >= 0x141)
    {
      ret = 1;
      goto done;
    }
    if (q->y1 < q->y0)
    {
      t->by0 = q->y0;
      t->by1 = q->y1;
    }
    else
    {
      t->by1 = q->y0;
      t->by0 = q->y1;
    }
    if (t->by0 < q->y2)
    {
      t->by0 = q->y2;
    }
    else
      if (q->y2 < t->by1)
    {
      t->by1 = q->y2;
    }
    if (t->by0 < q->y3)
    {
      t->by0 = q->y3;
    }
    else
      if (q->y3 < t->by1)
    {
      t->by1 = q->y3;
    }
    if (t->by0 < 0)
    {
      ret = 1;
      goto done;
    }
    ret = t->by1 < 0xF1;
    if (ret)
    {
      ret = 0;
      goto done;
    }
    ret = 1;
    done:
    return ret;

  }
  return 1;
}
