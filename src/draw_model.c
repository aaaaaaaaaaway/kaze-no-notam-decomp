#include "types.h"
#include "kaze_syms.h"
typedef struct Ctx
{
  int unk0;
  int unk4;
  int *unk8;
  int unkC;
  int unk10;
  int unk14;
  int row0[8];
  int row1[8];
  int row2[8];
  int unk78;
  int unk7C;
  int unk80;
  int unk84;
  int unk88;
  int unk8C;
  int unk90;
  int unk94;
  int unk98;
  int unk9C;
  int unkA0;
} Ctx;

void kaze_model_quad_f4_37ab8();
void kaze_model_quad_f4_nolight_3c3d0();
void kaze_model_quad_f4_nolight_semitrans_3c6f4();
void kaze_model_quad_f4_semitrans_37dd8();
void kaze_model_quad_ft4_3a500();
void kaze_model_quad_ft4_nolight_3d3e8();
void kaze_model_quad_g4_38124();
void kaze_model_quad_g4_38720();
void kaze_model_quad_g4_392d4();
void kaze_model_quad_g4_39898();
void kaze_model_quad_g4_nolight_3c9bc();
void kaze_model_quad_g4_nolight_semitrans_3cf00();
void kaze_model_quad_g4_semitrans_38cc0();
void kaze_model_quad_g4_semitrans_39ef4();
void kaze_model_quad_gt4_3a904();
void kaze_model_quad_gt4_nolight_3d778();
void kaze_model_tri_f3_4f98();
void kaze_model_tri_f3_nolight_3af60();
void kaze_model_tri_f3_nolight_semitrans_3b1fc();
void kaze_model_tri_f3_semitrans_352c4();
void kaze_model_tri_ft3_37234();
void kaze_model_tri_ft3_nolight_3bc48();
void kaze_model_tri_g3_355ac();
void kaze_model_tri_g3_3normal_36864();
void kaze_model_tri_g3_3normal_semitrans_36d68();
void kaze_model_tri_g3_nolight_3b460();
void kaze_model_tri_g3_nolight_semitrans_3b870();
void kaze_model_tri_g3_pervtx_35ec4();
void kaze_model_tri_g3_pervtx_semitrans_363b0();
void kaze_model_tri_g3_semitrans_35a54();
void kaze_model_tri_gt3_375ac();
void kaze_model_tri_gt3_nolight_3bf54();

/**
 * kaze_draw_model  (render, 0x80034964)
 *
 * Master model draw dispatch: sets up GTE draw/light state in the 1F800xxx
 * scratchpad, copies rotation/translation/scale arrays (+8/+0x28/+0x48),
 * then dispatches each submodel to its primitive rasterizer. Anchor:
 * draws...
 */

int kaze_draw_model(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
  Ctx *sp = (Ctx *) 0x1F800200;
  int i;
  int t;
  sp->unk90 = arg1;
  if (arg1 < 0)
  {
    return arg3;
  }
  sp->unk0 = arg4;
  sp->unk8 = (int *) (arg0 + 0x198);
  t = (((u32) arg1) >> 5) & 0x02000000;
  sp->unk80 = t;
  sp->unk4 = 0;
  sp->unkC = arg3;
  sp->unk14 = arg2;
  sp->unk7C = t | ((arg1 << 0x12) & 0x01000000);
  sp->unk84 = (((u32) arg1) >> 7) & 0x600000;
  sp->unk8C = ((((u32) arg1) >> 0x17) & 0x60) | 0xE1000200;
  sp->unk94 = arg5;
  sp->unk98 = arg6;
  sp->unk9C = arg7;
  sp->unkA0 = 0;
  do
  {
    {
      int k = sp->unkA0 * 4;
      *((int *) (k + (int) sp + 0x18)) = *((int *) ((k + arg0) + 8));
    }
    {
      int k = sp->unkA0 * 4;
      *((int *) (k + (int) sp + 0x38)) = *((int *) ((k + arg0) + 0x28));
    }
    {
      int k = sp->unkA0 * 4;
      *((int *) (k + (int) sp + 0x58)) = *((int *) ((k + arg0) + 0x48));
    }
    sp->unkA0 = (i = sp->unkA0 + 1);
  }
  while (((u32) i) < 8);
  if (sp->unk8[0] != 0)
  {
    kaze_model_quad_gt4_nolight_3d778(sp);
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_quad_gt4_3a904(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_quad_ft4_nolight_3d3e8(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_quad_ft4_3a500(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_quad_g4_nolight_semitrans_3cf00(sp);
      }
      else
      {
        kaze_model_quad_g4_nolight_3c9bc(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_quad_g4_semitrans_39ef4(sp);
      }
      else
      {
        kaze_model_quad_g4_39898(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_quad_g4_392d4(sp);
      }
      else
      {
        kaze_model_quad_g4_semitrans_38cc0(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_quad_g4_38720(sp);
      }
      else
      {
        kaze_model_quad_g4_38124(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_quad_f4_nolight_semitrans_3c6f4(sp);
      }
      else
      {
        kaze_model_quad_f4_nolight_3c3d0(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_quad_f4_semitrans_37dd8(sp);
      }
      else
      {
        kaze_model_quad_f4_37ab8(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_tri_gt3_nolight_3bf54(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_tri_gt3_375ac(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_tri_ft3_nolight_3bc48(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_tri_ft3_37234(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_tri_g3_nolight_semitrans_3b870(sp);
      }
      else
      {
        kaze_model_tri_g3_nolight_3b460(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_tri_g3_3normal_semitrans_36d68(sp);
      }
      else
      {
        kaze_model_tri_g3_3normal_36864(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_tri_g3_pervtx_semitrans_363b0(sp);
      }
      else
      {
        kaze_model_tri_g3_pervtx_35ec4(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_tri_g3_semitrans_35a54(sp);
      }
      else
      {
        kaze_model_tri_g3_355ac(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_tri_f3_nolight_semitrans_3b1fc(sp);
      }
      else
      {
        kaze_model_tri_f3_nolight_3af60(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_tri_f3_semitrans_352c4(sp);
      }
      else
      {
        kaze_model_tri_f3_4f98(sp);
      }
    }
  }
  return sp->unkC;
}

#include "gte.h"
#define gte_nclip() __asm__ volatile("cop2 0x1400006")
#define gte_avsz3() __asm__ volatile("cop2 0x158002D")
#define gte_stopz(x) __asm__ volatile("swc2 $24, 0(%0)" :: "r"(x) : "memory")
#define gte_stotz(x) __asm__ volatile("swc2 $7, 0(%0)" :: "r"(x) : "memory")
#define gte_stlvl(x) __asm__ volatile("swc2 $22, 0(%0)" :: "r"(x) : "memory")

/**
 * kaze_model_tri_f3_4f98  (render, 0x80034f98)
 *
 * TMD submodel rasterizer dispatched by kaze_draw_model: flat-shaded
 * triangles (gte_ldv3/rtpt_b/nclip, gte_stsxy3_f3, nccs single color, POLY
 * code 0x20/0x22000000) with per-model RGB modulation.
 */

void kaze_model_tri_f3_4f98(u32 *arg)
{
    register u32 *p asm("$5");
    register u32 mask asm("$8");
    register u32 tag asm("$9");
    register u32 red_mask asm("$10");
    u32 *vp;
    u32 c;
    u32 r;
    u32 g;
    u32 b;

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
    red_mask = 0x00ff0000;
loop:
    vp = (u32 *)p[0x34];
    gte_ldv3((void *)vp[0], (void *)vp[1], (void *)vp[2]);
    p[0x34] = (u32)(vp + 3);
    gte_rtpt();
    p[0x28] = p[0x28] + 1;
    if ((p[0x24] & 0x100) == 0) {
        gte_nclip();
        gte_stopz(p + 0x2a);
        if ((int)p[0x2a] <= 0) {
            goto next;
        }
    }
    gte_avsz3();
    gte_stotz(p + 0x29);
    if ((int)p[0x29] > 0) {
        if (*(u32 *)p[0x33] & 0x2000000) {
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
            gte_ldnrm((u32 *)*(u32 *)p[0x35]);
            gte_nccs_col(p[0x33]);
        }
        p[0x29] = (int)p[0x29] >> p[0];
        *(u32 *)p[3] = (*(u32 *)(p[0x29] * 4 + p[5]) & mask) | 0x4000000;
        *(u32 *)(p[0x29] * 4 + p[5]) = p[4] & mask;
        if ((p[0x1f] & tag) == 0) {
            gte_stlvl(p + 0x2f);
        } else {
            p[0x2f] = *(u32 *)p[0x33];
        }
        r = 0xff;
        c = (u32)(u8)p[0x2f] * p[0x25] >> 7;
        p[0x2c] = c;
        if ((c & 0xffffff00) == 0) r = c & 0xff;
        g = 0xff00;
        p[0x2c] = r;
        c = (p[0x2f] & 0xff00) * p[0x26] >> 7;
        p[0x2d] = c;
        if ((c & 0xffff0000) == 0) g = c & 0xff00;
        b = 0xff0000;
        p[0x2d] = g;
        c = (p[0x2f] & red_mask) * p[0x27] >> 7;
        p[0x2e] = c;
        if ((c & 0xff000000) == 0) b = c & red_mask;
        p[0x2e] = b;
        *(u32 *)(p[3] + 4) = p[0x1e] | p[0x20] | p[0x2c] | p[0x2d] | b;
        p[3] = p[3] + 0x14;
    }
next:
    p[0x33] = p[0x33] + 4;
    p[0x35] = p[0x35] + 4;
    if (p[0x28] < p[1]) {
        goto loop;
    }
}

#define gte_nccs() __asm__ volatile("cop2 0x108041B")
#define gte_ldrgb(x) __asm__ volatile("lwc2 $6, 0(%0)" :: "r"(x))
#define gte_strgb(x) __asm__ volatile("swc2 $22, 0(%0)" :: "r"(x) : "memory")

/**
 * kaze_model_tri_f3_semitrans_352c4  (render, 0x800352c4)
 *
 * TMD submodel rasterizer (semi-transparent variant): flat-shaded triangles
 * via gte_stsxy3_f3/nccs, POLY code 0x22000000, stride 0x1c, RGB modulated
 * by param+0x25..0x27.
 */

void kaze_model_tri_f3_semitrans_352c4(u32 *arg)
{
    register u32 *p asm("$5");
    register u32 tag asm("$9");
    register u32 mask asm("$8");
    register u32 blue_mask asm("$10");
    u32 *vp;
    u32 c;
    u32 r;
    u32 g;
    u32 b;

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
    blue_mask = 0x00ff0000;
loop:
    vp = (u32 *)p[0x34];
    gte_ldv3((void *)vp[0], (void *)vp[1], (void *)vp[2]);
    p[0x34] = (u32)(vp + 3);
    gte_rtpt();
    p[0x28] = p[0x28] + 1;
    if ((p[0x24] & 0x100) == 0) {
        gte_nclip();
        gte_stopz(p + 0x2a);
        if ((int)p[0x2a] <= 0) {
            goto tail;
        }
    }
    gte_avsz3();
    gte_stotz(p + 0x29);
    if ((int)p[0x29] > 0) {
        gte_stsxy3_f3((void *)(p[3] + 8));
        if ((p[0x1f] & tag) == 0) {
            gte_ldv0(*(u32 *)p[0x35]);
            gte_ldrgb(p[0x33]);
            gte_nop();
            gte_nop();
            gte_nccs();
        }
        p[0x29] = (int)p[0x29] >> p[0];
        *(u32 *)p[3] = ((u32)((u32 *)p[3] + 2) & mask) | tag;
        *(u32 *)(p[3] + 4) = p[0x23];
        *(u32 *)(p[3] + 8) = (*(u32 *)(p[0x29] * 4 + p[5]) & mask) | 0x4000000;
        *(u32 *)(p[0x29] * 4 + p[5]) = p[3] & mask;
        if ((p[0x1f] & tag) == 0) {
            gte_strgb(p + 0x2f);
        } else {
            p[0x2f] = *(u32 *)p[0x33];
        }
        r = 0xff;
        c = (u32)(u8)p[0x2f] * p[0x25] >> 7;
        p[0x2c] = c;
        if ((c & 0xffffff00) == 0) r = c & 0xff;
        g = 0xff00;
        p[0x2c] = r;
        c = (p[0x2f] & 0xff00) * p[0x26] >> 7;
        p[0x2d] = c;
        if ((c & 0xffff0000) == 0) g = c & 0xff00;
        b = 0xff0000;
        p[0x2d] = g;
        c = (p[0x2f] & blue_mask) * p[0x27] >> 7;
        p[0x2e] = c;
        if ((c & 0xff000000) == 0) b = c & blue_mask;
        p[0x2e] = b;
        *(u32 *)(p[3] + 0xc) = 0x22000000 | p[0x20] | p[0x2c] | p[0x2d] | b;
        p[3] = p[3] + 0x1c;
    }
tail:
    p[0x33] = p[0x33] + 4;
    p[0x35] = p[0x35] + 4;
    if (p[0x28] < p[1]) {
        goto loop;
    }
}

#define gte_ncct() __asm__ volatile("cop2 0x118043F")

/**
 * kaze_model_tri_g3_355ac  (render, 0x800355ac)
 *
 * TMD submodel rasterizer: gouraud triangles (gte_stsxy3_g3, gte_ncct_b,
 * gte_strgb3 three colors, POLY code 0x30/0x32000000) with per-vertex RGB
 * modulation.
 */

void kaze_model_tri_g3_355ac(u32 *arg)
{
    register u32 *ctx asm("$5");
    register u32 m24 asm("$9");
    register u32 m01 asm("$13");
    register u32 mr asm("$12");
    register u32 mg asm("$11");
    register u32 mb0 asm("$8");
    register u32 mb asm("$10");
    u32 *vp;
    u32 tmp;
    u32 c;
    u32 cc;
    u32 r;
    u32 g;
    u32 b;
    u32 g2;
    u32 b2;

    ctx = arg;
    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    tmp = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = tmp;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        m01 = 0x1000000;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            vp = (u32 *)ctx[0x34];
            gte_ldv3((void *)vp[0], (void *)vp[1], (void *)vp[2]);
            ctx[0x34] = (u32)(vp + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            if ((ctx[0x24] & 0x100) == 0) {
                gte_nclip();
                gte_stopz(ctx + 0x2a);
                if ((int)ctx[0x2a] <= 0) goto reject;
            }
            gte_avsz3();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
reject:
                ctx[0x35] = ctx[0x35] + 0xc;
            } else {
                ctx[4] = ctx[3];
                if (*(u32 *)ctx[0x33] & 0x2000000) {
                    *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m01;
                    *(u32 *)(ctx[3] + 4) = ctx[0x23];
                    ctx[0x1e] = 0x32000000;
                    ctx[3] = ctx[3] + 8;
                } else {
                    ctx[0x1e] = 0x30000000;
                }
                gte_stsxy3_g3((void *)ctx[3]);
                if ((ctx[0x1f] & m01) == 0) {
                    vp = (u32 *)ctx[0x35];
                    gte_ldv3((void *)vp[0], (void *)vp[1], (void *)vp[2]);
                    gte_ldrgb((void *)ctx[0x33]);
                    ctx[0x35] = (u32)(vp + 3);
                    gte_ncct();
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x6000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                if ((ctx[0x1f] & m01) == 0) {
                    gte_strgb3(ctx + 0x2f, ctx + 0x30, ctx + 0x31);
                } else {
                    cc = *(u32 *)ctx[0x33];
                    ctx[0x35] = ctx[0x35] + 0xc;
                    ctx[0x31] = cc;
                    ctx[0x30] = cc;
                    ctx[0x2f] = cc;
                }
                r = 0xff;
                c = (u32)(u8)ctx[0x2f] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g = c & 0xff00;
                b = 0xff0000;
                ctx[0x2d] = g;
                c = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b = c & mb0;
                ctx[0x2e] = b;
                *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | b;
                r = 0xff;
                c = (u32)(u8)ctx[0x30] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g2 = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g2 = c & 0xff00;
                b2 = 0xff0000;
                ctx[0x2d] = g2;
                c = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b2 = c & mb0;
                ctx[0x2e] = b2;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x2c] | ctx[0x2d] | b2;
                r = 0xff;
                c = (u32)(u8)ctx[0x31] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g2 = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g2 = c & 0xff00;
                b2 = 0xff0000;
                ctx[0x2d] = g2;
                c = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b2 = c & mb0;
                ctx[0x2e] = b2;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | b2;
                ctx[3] = ctx[3] + 0x1c;
            }
            ctx[0x33] = ctx[0x33] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_tri_g3_semitrans_35a54  (render, 0x80035a54)
 *
 * TMD submodel rasterizer: gouraud triangles semi-transparent variant
 * (gte_stsxy3_g3/ncct/strgb3, POLY code 0x32000000, stride 0x24).
 */

void kaze_model_tri_g3_semitrans_35a54(u32 *arg)
{
    register u32 *ctx asm("$5");
    register u32 m24 asm("$9");
    register u32 m01 asm("$13");
    register u32 mr asm("$12");
    register u32 mg asm("$11");
    register u32 mb0 asm("$8");
    register u32 mb asm("$10");
    u32 *vp;
    u32 tmp;
    u32 c;
    u32 cc;
    u32 r;
    u32 g;
    u32 b;
    u32 g2;
    u32 b2;

    ctx = arg;
    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    tmp = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = tmp;
    if (ctx[1] != 0) {
        m01 = 0x1000000;
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            vp = (u32 *)ctx[0x34];
            gte_ldv3((void *)vp[0], (void *)vp[1], (void *)vp[2]);
            ctx[0x34] = (u32)(vp + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            if ((ctx[0x24] & 0x100) == 0) {
                gte_nclip();
                gte_stopz(ctx + 0x2a);
                if ((int)ctx[0x2a] <= 0) goto reject;
            }
            gte_avsz3();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
reject:
                ctx[0x35] = ctx[0x35] + 0xc;
            } else {
                gte_stsxy3_g3((void *)(ctx[3] + 8));
                if ((ctx[0x1f] & m01) == 0) {
                    vp = (u32 *)ctx[0x35];
                    gte_ldv3((void *)vp[0], (void *)vp[1], (void *)vp[2]);
                    gte_ldrgb((void *)ctx[0x33]);
                    ctx[0x35] = (u32)(vp + 3);
                    gte_ncct();
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m01;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x6000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                if ((ctx[0x1f] & m01) == 0) {
                    gte_strgb3(ctx + 0x2f, ctx + 0x30, ctx + 0x31);
                } else {
                    cc = *(u32 *)ctx[0x33];
                    ctx[0x35] = ctx[0x35] + 0xc;
                    ctx[0x31] = cc;
                    ctx[0x30] = cc;
                    ctx[0x2f] = cc;
                }
                r = 0xff;
                c = (u32)(u8)ctx[0x2f] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g = c & 0xff00;
                b = 0xff0000;
                ctx[0x2d] = g;
                c = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b = c & mb0;
                ctx[0x2e] = b;
                *(u32 *)(ctx[3] + 0xc) = 0x32000000 | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | b;
                r = 0xff;
                c = (u32)(u8)ctx[0x30] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g2 = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g2 = c & 0xff00;
                b2 = 0xff0000;
                ctx[0x2d] = g2;
                c = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b2 = c & mb0;
                ctx[0x2e] = b2;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | b2;
                r = 0xff;
                c = (u32)(u8)ctx[0x31] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g2 = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g2 = c & 0xff00;
                b2 = 0xff0000;
                ctx[0x2d] = g2;
                c = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b2 = c & mb0;
                ctx[0x2e] = b2;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | b2;
                ctx[3] = ctx[3] + 0x24;
            }
            ctx[0x33] = ctx[0x33] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_tri_g3_pervtx_35ec4  (render, 0x80035ec4)
 *
 * TMD submodel rasterizer: gouraud triangles lit per-vertex (three
 * gte_ldv0/nccs/strgb passes) with POLY code 0x30/0x32000000, stride 0x1c.
 */

void kaze_model_tri_g3_pervtx_35ec4(u32 *ctx)
{
    u32 t0v;
    u32 *p;
    u32 t;
    u32 ra;
    u32 rb;
    u32 rc;
    u32 m24;
    u32 m1p;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t0v = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t0v;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        m1p = 0x1000000;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            if ((ctx[0x24] & 0x100) == 0) {
                gte_nclip();
                gte_stopz(ctx + 0x2a);
                if ((int)ctx[0x2a] <= 0) goto reject;
            }
            gte_avsz3();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
            reject:
                ctx[0x35] = ctx[0x35] + 4;
            } else {
                ctx[4] = ctx[3];
                if (*(u32 *)ctx[0x33] & 0x2000000) {
                    *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1p;
                    *(u32 *)(ctx[3] + 4) = ctx[0x23];
                    ctx[0x1e] = 0x32000000;
                    ctx[3] = ctx[3] + 8;
                } else {
                    ctx[0x1e] = 0x30000000;
                }
                gte_stsxy3_g3(ctx[3]);
                if ((ctx[0x1f] & m1p) == 0) {
                    p = (u32 *)ctx[0x35];
                    gte_ldv0(p[0]);
                    ctx[0x35] = (u32)(p + 1);
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x6000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                if ((ctx[0x1f] & m1p) == 0) {
                    gte_ldrgb(ctx[0x33]);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x2f);
                    gte_ldrgb(ctx[0x33] + 4);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x30);
                    gte_ldrgb(ctx[0x33] + 8);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x31);
                } else {
                    ctx[0x2f] = *(u32 *)ctx[0x33];
                    ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                    t = *(u32 *)(ctx[0x33] + 8);
                    ctx[0x35] = ctx[0x35] + 4;
                    ctx[0x31] = t;
                }
                ra = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    ra = ctx[0x2c] & 0xff;
                }
                rb = 0xff00;
                ctx[0x2c] = ra;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    rb = ctx[0x2d] & 0xff00;
                }
                rc = 0xff0000;
                ctx[0x2d] = rb;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    rc = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = rc;
                *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | rc;
                ra = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    ra = ctx[0x2c] & 0xff;
                }
                rc = 0xff00;
                ctx[0x2c] = ra;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    rc = ctx[0x2d] & 0xff00;
                }
                ra = 0xff0000;
                ctx[0x2d] = rc;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    ra = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = ra;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x2c] | ctx[0x2d] | ra;
                ra = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    ra = ctx[0x2c] & 0xff;
                }
                rc = 0xff00;
                ctx[0x2c] = ra;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    rc = ctx[0x2d] & 0xff00;
                }
                ra = 0xff0000;
                ctx[0x2d] = rc;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    ra = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = ra;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | ra;
                ctx[3] = ctx[3] + 0x1c;
            }
            ctx[0x33] = ctx[0x33] + 0xc;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_tri_g3_pervtx_semitrans_363b0  (render, 0x800363b0)
 *
 * TMD submodel rasterizer: gouraud triangles lit per-vertex, semi-
 * transparent variant (gte_stsxy3_g3, POLY code 0x32000000, stride 0x24).
 */

void kaze_model_tri_g3_pervtx_semitrans_363b0(u32 *ctx)
{
    u32 t0v;
    u32 *p;
    u32 t;
    u32 ra;
    u32 rb;
    u32 rc;
    u32 m24;
    u32 m1p;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t0v = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t0v;
    if (ctx[1] != 0) {
        m1p = 0x1000000;
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            if ((ctx[0x24] & 0x100) == 0) {
                gte_nclip();
                gte_stopz(ctx + 0x2a);
                if ((int)ctx[0x2a] <= 0) goto reject;
            }
            gte_avsz3();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
            reject:
                ctx[0x35] = ctx[0x35] + 4;
            } else {
                gte_stsxy3_g3(ctx[3] + 8);
                if ((ctx[0x1f] & m1p) == 0) {
                    p = (u32 *)ctx[0x35];
                    gte_ldv0(p[0]);
                    ctx[0x35] = (u32)(p + 1);
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1p;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x6000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                if ((ctx[0x1f] & m1p) == 0) {
                    gte_ldrgb(ctx[0x33]);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x2f);
                    gte_ldrgb(ctx[0x33] + 4);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x30);
                    gte_ldrgb(ctx[0x33] + 8);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x31);
                } else {
                    ctx[0x2f] = *(u32 *)ctx[0x33];
                    ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                    t = *(u32 *)(ctx[0x33] + 8);
                    ctx[0x35] = ctx[0x35] + 4;
                    ctx[0x31] = t;
                }
                ra = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    ra = ctx[0x2c] & 0xff;
                }
                rb = 0xff00;
                ctx[0x2c] = ra;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    rb = ctx[0x2d] & 0xff00;
                }
                rc = 0xff0000;
                ctx[0x2d] = rb;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    rc = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = rc;
                *(u32 *)(ctx[3] + 0xc) = 0x32000000 | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | rc;
                ra = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    ra = ctx[0x2c] & 0xff;
                }
                rc = 0xff00;
                ctx[0x2c] = ra;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    rc = ctx[0x2d] & 0xff00;
                }
                ra = 0xff0000;
                ctx[0x2d] = rc;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    ra = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = ra;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | ra;
                ra = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    ra = ctx[0x2c] & 0xff;
                }
                rc = 0xff00;
                ctx[0x2c] = ra;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    rc = ctx[0x2d] & 0xff00;
                }
                ra = 0xff0000;
                ctx[0x2d] = rc;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    ra = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = ra;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | ra;
                ctx[3] = ctx[3] + 0x24;
            }
            ctx[0x33] = ctx[0x33] + 0xc;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_tri_g3_3normal_36864  (render, 0x80036864)
 *
 * TMD submodel rasterizer: gouraud triangles with three separate vertex
 * normals (ldv0/ldrgb/nccs x3), POLY code 0x30/0x32000000, stride 0x1c.
 */

void kaze_model_tri_g3_3normal_36864(u32 *ctx)
{
    u32 t0v;
    u32 *p;
    u32 r5;
    u32 r4;
    u32 m24;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t0v = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t0v;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            if ((ctx[0x24] & 0x100) == 0) {
                gte_nclip();
                gte_stopz(ctx + 0x2a);
                if ((int)ctx[0x2a] <= 0) goto reject;
            }
            gte_avsz3();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
            reject:
                ctx[0x35] = ctx[0x35] + 0xc;
            } else {
                ctx[4] = ctx[3];
                if (*(u32 *)ctx[0x33] & 0x2000000) {
                    *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | 0x1000000;
                    *(u32 *)(ctx[3] + 4) = ctx[0x23];
                    ctx[0x1e] = 0x32000000;
                    ctx[3] = ctx[3] + 8;
                } else {
                    ctx[0x1e] = 0x30000000;
                }
                gte_stsxy3_g3(ctx[3]);
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x6000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                if ((ctx[0x1f] & 0x1000000) == 0) {
                    gte_ldv0(*(u32 *)ctx[0x35]);
                    gte_ldrgb(ctx[0x33]);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x2f);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 4));
                    gte_ldrgb(ctx[0x33] + 4);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x30);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 8));
                    gte_ldrgb(ctx[0x33] + 8);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x31);
                } else {
                    ctx[0x2f] = *(u32 *)ctx[0x33];
                    ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                    ctx[0x31] = *(u32 *)(ctx[0x33] + 8);
                }
                r5 = 0xff;
                ctx[0x35] = ctx[0x35] + 0xc;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r5 = ctx[0x2c] & 0xff;
                }
                r4 = 0xff00;
                ctx[0x2c] = r5;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r4 = ctx[0x2d] & 0xff00;
                }
                r5 = 0xff0000;
                ctx[0x2d] = r4;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r5 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r5;
                *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r5;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | r4;
                ctx[3] = ctx[3] + 0x1c;
            }
            ctx[0x33] = ctx[0x33] + 0xc;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_tri_g3_3normal_semitrans_36d68  (render, 0x80036d68)
 *
 * TMD submodel rasterizer: gouraud triangles with three vertex normals,
 * semi-transparent variant (gte_stsxy3_g3, POLY code 0x32000000, stride
 * 0x24).
 */

void kaze_model_tri_g3_3normal_semitrans_36d68(u32 *ctx)
{
    u32 t0v;
    u32 *p;
    u32 r5;
    u32 r4;
    u32 m24;
    u32 m01;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t0v = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t0v;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        m01 = 0x1000000;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            if ((ctx[0x24] & 0x100) == 0) {
                gte_nclip();
                gte_stopz(ctx + 0x2a);
                if ((int)ctx[0x2a] <= 0) goto reject;
            }
            gte_avsz3();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
            reject:
                ctx[0x35] = ctx[0x35] + 0xc;
            } else {
                gte_stsxy3_g3(ctx[3] + 8);
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m01;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                *(u32 *)(ctx[3] + 8) =
                    (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x6000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                if ((ctx[0x1f] & m01) == 0) {
                    gte_ldv0(*(u32 *)ctx[0x35]);
                    gte_ldrgb(ctx[0x33]);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x2f);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 4));
                    gte_ldrgb(ctx[0x33] + 4);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x30);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 8));
                    gte_ldrgb(ctx[0x33] + 8);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x31);
                } else {
                    ctx[0x2f] = *(u32 *)ctx[0x33];
                    ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                    ctx[0x31] = *(u32 *)(ctx[0x33] + 8);
                }
                r5 = 0xff;
                ctx[0x35] = ctx[0x35] + 0xc;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r5 = ctx[0x2c] & 0xff;
                }
                r4 = 0xff00;
                ctx[0x2c] = r5;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r4 = ctx[0x2d] & 0xff00;
                }
                r5 = 0xff0000;
                ctx[0x2d] = r4;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r5 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r5;
                *(u32 *)(ctx[3] + 0xc) =
                    0x32000000 | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r5;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | r4;
                ctx[3] = ctx[3] + 0x24;
            }
            ctx[0x33] = ctx[0x33] + 0xc;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_tri_ft3_37234  (render, 0x80037234)
 *
 * TMD submodel rasterizer: textured flat triangles (gte_stsxy3_g3/ncs,
 * TPage/CLUT at +0x36, POLY code 0x7000000) with texture-window fields.
 */

void kaze_model_tri_ft3_37234(u32 *ctx)
{
    u32 c;
    u32 x;
    u32 idx;
    u32 *p;
    u32 m1;
    u32 m24;
    u32 mb0;
    u32 r4;
    u32 r5;
    u32 r6;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x35] = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    if (ctx[1] != 0) {
        m1 = 0x1000000;
        m24 = 0xffffff;
        mb0 = 0xff0000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            if ((ctx[0x24] & 0x100) == 0) {
                gte_nclip();
                gte_stopz(ctx + 0x2a);
                if ((int)ctx[0x2a] < 1) {
                    goto next;
                }
            }
            gte_avsz3();
            ctx[0x1e] = *(u32 *)ctx[0x33] & 0xfe000000;
            if (ctx[0x20] != 0) {
                x = ctx[0x21];
                c = *(u16 *)(ctx[0x33] + 8) | x;
            } else {
                c = *(u32 *)(ctx[0x33] + 8) & 0x60ffff;
            }
            ctx[0x22] = c;
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                gte_stsxy3_g3(ctx[3]);
                if ((ctx[0x1f] & m1) == 0) {
                    gte_ldv0(*(u32 *)ctx[0x35]);
                    gte_nop();
                    gte_nop();
                    gte_ncs();
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x7000000;
                idx = *(u16 *)(ctx[0x33] + 6) & 7;
                ctx[0x36] = idx;
                *(u32 *)(ctx[3] + 0xc) =
                    *(u16 *)(ctx[0x33] + 4) | ctx[idx + 0xe] | ctx[idx + 0x16];
                *(u32 *)(ctx[3] + 0x14) =
                    ctx[0x22] | ctx[ctx[0x36] + 6] | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x1c) =
                    *(u32 *)(ctx[0x33] + 0xc) | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                if ((ctx[0x1f] & m1) == 0) {
                    gte_strgb(ctx + 0x2f);
                } else {
                    ctx[0x2f] = 0x808080;
                }
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 8;
                if ((ctx[0x2c] & 0xffffff00) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 8;
                if ((ctx[0x2d] & 0xffff0000) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r6 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 8;
                if (((ctx[0x2e] & 0xff000000) << 8) == 0) {
                    r6 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r6;
                *(u32 *)(ctx[3] + 4) =
                    ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r6;
                ctx[3] = ctx[3] + 0x20;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x10;
            ctx[0x35] = ctx[0x35] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_tri_gt3_375ac  (render, 0x800375ac)
 *
 * TMD submodel rasterizer: textured gouraud triangles (gte_stsxy3_gt3,
 * gte_nct_b, strgb3, TPage/CLUT, POLY code 0x9000000).
 */

void kaze_model_tri_gt3_375ac(u32 *ctx)
{
    u32 t0v;
    u32 *p;
    u32 ra;
    u32 rb;
    u32 rc;
    u32 m1p;
    u32 m24;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t0v = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t0v;
    if (ctx[1] != 0) {
        m1p = 0x1000000;
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            if ((ctx[0x24] & 0x100) == 0) {
                gte_nclip();
                gte_stopz(ctx + 0x2a);
                if ((int)ctx[0x2a] <= 0) goto reject;
            }
            gte_avsz3();
            ctx[0x1e] = *(u32 *)(ctx[0x33] + 8) & 0xfe000000;
            if (ctx[0x20] != 0) {
                ctx[0x22] = *(u16 *)(ctx[0x33] + 4) | ctx[0x21];
            } else {
                ctx[0x22] = *(u32 *)(ctx[0x33] + 4) & 0x60ffff;
            }
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
            reject:
                ctx[0x35] = ctx[0x35] + 0xc;
            } else {
                gte_stsxy3_gt3(ctx[3]);
                if ((ctx[0x1f] & m1p) == 0) {
                    p = (u32 *)ctx[0x35];
                    gte_ldv3(p[0], p[1], p[2]);
                    ctx[0x35] = (u32)(p + 3);
                    gte_nct();
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x9000000;
                ctx[0x36] = *(u16 *)(ctx[0x33] + 2) & 7;
                *(u32 *)(ctx[3] + 0xc) =
                    (u32)*(u16 *)ctx[0x33] | ctx[ctx[0x36] + 0xe] | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x18) =
                    ctx[0x22] | ctx[ctx[0x36] + 6] | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x24) = *(u32 *)(ctx[0x33] + 8) | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                if ((ctx[0x1f] & m1p) == 0) {
                    gte_strgb3(ctx + 0x2f, ctx + 0x30, ctx + 0x31);
                } else {
                    ctx[0x31] = 0x808080;
                    ctx[0x30] = 0x808080;
                    ctx[0x2f] = 0x808080;
                    ctx[0x35] = ctx[0x35] + 0xc;
                }
                ra = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 8;
                if ((ctx[0x2c] & mr) == 0) {
                    ra = ctx[0x2c] & 0xff;
                }
                rb = 0xff00;
                ctx[0x2c] = ra;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 8;
                if ((ctx[0x2d] & mg) == 0) {
                    rb = ctx[0x2d] & 0xff00;
                }
                rc = 0xff0000;
                ctx[0x2d] = rb;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 8;
                if ((ctx[0x2e] & mb) == 0) {
                    rc = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = rc;
                *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | rc;
                ra = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 8;
                if ((ctx[0x2c] & mr) == 0) {
                    ra = ctx[0x2c] & 0xff;
                }
                rc = 0xff00;
                ctx[0x2c] = ra;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 8;
                if ((ctx[0x2d] & mg) == 0) {
                    rc = ctx[0x2d] & 0xff00;
                }
                ra = 0xff0000;
                ctx[0x2d] = rc;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 8;
                if ((ctx[0x2e] & mb) == 0) {
                    ra = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = ra;
                *(u32 *)(ctx[3] + 0x10) = ctx[0x2c] | ctx[0x2d] | ra;
                ra = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 8;
                if ((ctx[0x2c] & mr) == 0) {
                    ra = ctx[0x2c] & 0xff;
                }
                rc = 0xff00;
                ctx[0x2c] = ra;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 8;
                if ((ctx[0x2d] & mg) == 0) {
                    rc = ctx[0x2d] & 0xff00;
                }
                ra = 0xff0000;
                ctx[0x2d] = rc;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 8;
                if ((ctx[0x2e] & mb) == 0) {
                    ra = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = ra;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | ra;
                ctx[3] = ctx[3] + 0x28;
            }
            ctx[0x33] = ctx[0x33] + 0xc;
        } while (ctx[0x28] < ctx[1]);
    }
}

#define gte_avsz4() __asm__ volatile("cop2 0x168002E")
#define gte_stsxy(x) __asm__ volatile("swc2 $14, 0(%0)" :: "r"(x) : "memory")

/**
 * kaze_model_quad_f4_37ab8  (render, 0x80037ab8)
 *
 * TMD submodel rasterizer: flat-shaded quads (rtpt+rtps 4th vertex,
 * gte_stsxy3_f3+stsxy, avsz4, nccs, POLY code 0x5000000, stride 0x18).
 */

void kaze_model_quad_f4_37ab8(u32 *ctx)
{
    u32 tmp;
    u32 *p;
    u32 idx;
    u32 prim;
    u32 shift;
    u32 header;
    u32 r;
    u32 g;
    u32 b;
    u32 mask;
    u32 tag;
    u32 blue_mask;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    tmp = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = tmp;
    if (ctx[1] != 0) {
        tag = 0x1000000;
        mask = 0xffffff;
        blue_mask = 0xff0000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            gte_stsxy3_f3(ctx[3]);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x14);
            gte_nop();
            gte_nop();
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        goto next;
                    }
                }
                if ((ctx[0x1f] & tag) == 0) {
                    gte_ldv0(*(u32 *)ctx[0x35]);
                    gte_ldrgb(ctx[0x33]);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                }
                {
                    register u32 idx_r asm("$2");
                    register u32 shift_r asm("$3");
                    register u32 prim_r asm("$4");

                    idx_r = ctx[0x29];
                    shift_r = ctx[0];
                    prim_r = ctx[3];
                    idx_r = (int)idx_r >> shift_r;
                    ctx[0x29] = idx_r;
                    *(u32 *)prim_r = (*(u32 *)(idx_r * 4 + ctx[5]) & mask) | 0x5000000;
                    *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & mask;
                }
                if ((ctx[0x1f] & tag) == 0) {
                    gte_strgb(ctx + 0x2f);
                } else {
                    ctx[0x2f] = *(u32 *)ctx[0x33];
                }

                r = 0xff;
                idx = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                ctx[0x2c] = idx;
                if ((idx & 0xffffff00) == 0) {
                    r = idx & 0xff;
                }
                g = 0xff00;
                ctx[0x2c] = r;
                idx = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = idx;
                if ((idx & 0xffff0000) == 0) {
                    g = idx & 0xff00;
                }
                b = 0xff0000;
                ctx[0x2d] = g;
                idx = (ctx[0x2f] & blue_mask) * ctx[0x27] >> 7;
                ctx[0x2e] = idx;
                if ((idx & 0xff000000) == 0) {
                    b = idx & blue_mask;
                }
                ctx[0x2e] = b;
                header = 0x28000000 | ctx[0x2c];
                *(u32 *)(ctx[3] + 4) = ctx[0x20] | header | ctx[0x2d] | b;
                ctx[3] = ctx[3] + 0x18;
            }
next:
            ctx[0x33] = ctx[0x33] + 4;
            ctx[0x35] = ctx[0x35] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_f4_semitrans_37dd8  (render, 0x80037dd8)
 *
 * TMD submodel rasterizer: flat-shaded quads semi-transparent variant (avsz4
 * quad, gte_stsxy3_f3, POLY code 0x5000000, stride 0x20).
 */

void kaze_model_quad_f4_semitrans_37dd8(u32 *ctx)
{
    u32 tmp;
    u32 *p;
    u32 idx;
    u32 r;
    u32 g;
    u32 b;
    u32 mask;
    u32 tag;
    u32 blue_mask;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    tmp = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = tmp;
    if (ctx[1] != 0) {
        tag = 0x1000000;
        mask = 0xffffff;
        blue_mask = 0xff0000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            gte_stsxy3_f3(ctx[3] + 8);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x1c);
            gte_nop();
            gte_nop();
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        goto next;
                    }
                }
                if ((ctx[0x1f] & tag) == 0) {
                    gte_ldv0(*(u32 *)ctx[0x35]);
                    gte_ldrgb(ctx[0x33]);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & mask) | tag;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & mask) | 0x5000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & mask;
                if ((ctx[0x1f] & tag) == 0) {
                    gte_strgb(ctx + 0x2f);
                } else {
                    ctx[0x2f] = *(u32 *)ctx[0x33];
                }
                r = 0xff;
                idx = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                ctx[0x2c] = idx;
                if ((idx & 0xffffff00) == 0) {
                    r = idx & 0xff;
                }
                g = 0xff00;
                ctx[0x2c] = r;
                idx = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = idx;
                if ((idx & 0xffff0000) == 0) {
                    g = idx & 0xff00;
                }
                b = 0xff0000;
                ctx[0x2d] = g;
                idx = (ctx[0x2f] & blue_mask) * ctx[0x27] >> 7;
                ctx[0x2e] = idx;
                if ((idx & 0xff000000) == 0) {
                    b = idx & blue_mask;
                }
                ctx[0x2e] = b;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x20] | 0x2a000000 | ctx[0x2c] | ctx[0x2d] | b;
                ctx[3] = ctx[3] + 0x20;
            }
        next:
            ctx[0x33] = ctx[0x33] + 4;
            ctx[0x35] = ctx[0x35] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_g4_38124  (render, 0x80038124)
 *
 * TMD submodel rasterizer: gouraud quads (rtpt+rtps 4th vertex,
 * gte_stsxy3_g3, gte_ncct + extra nccs, strgb3, POLY code 0x38/0x3a000000,
 * stride 0x24).
 */

void kaze_model_quad_g4_38124(u32 *ctx)
{
    u32 tmp;
    u32 *p;
    u32 c;
    u32 r4;
    u32 r5;
    u32 r6;
    u32 m24;
    u32 m01;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    tmp = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = tmp;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        m01 = 0x1000000;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            if (*(u32 *)ctx[0x33] & 0x2000000) {
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m01;
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
            gte_nop();
            gte_nop();
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
                ctx[3] = ctx[4];
            } else {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        ctx[3] = ctx[4];
                        goto next;
                    }
                }
                if ((ctx[0x1f] & m01) == 0) {
                    p = (u32 *)ctx[0x35];
                    gte_ldv3(p[0], p[1], p[2]);
                    gte_ldrgb(ctx[0x33]);
                    gte_nop();
                    gte_nop();
                    gte_ncct();
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x8000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                if ((ctx[0x1f] & m01) == 0) {
                    gte_strgb3(ctx + 0x2f, ctx + 0x30, ctx + 0x31);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 0xc));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x32);
                } else {
                    c = *(u32 *)ctx[0x33];
                    ctx[0x32] = c;
                    ctx[0x31] = c;
                    ctx[0x30] = c;
                    ctx[0x2f] = c;
                }
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r6 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r6 = ctx[0x2d] & 0xff00;
                }
                r5 = 0xff0000;
                ctx[0x2d] = r6;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r5 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r5;
                *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r5;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x32) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x32] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x32] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | r4;
                ctx[3] = ctx[3] + 0x24;
            }
        next:
            ctx[0x33] = ctx[0x33] + 4;
            ctx[0x35] = ctx[0x35] + 0x10;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_g4_38720  (render, 0x80038720)
 *
 * TMD submodel rasterizer: gouraud quads (gte_stsxy3_g3 quad path, avsz4,
 * per-vertex RGB modulation, POLY code 0x38000000-family).
 */

void kaze_model_quad_g4_38720(u32 *ctx)
{
    u32 tmp;
    u32 *p;
    u32 c;
    u32 r4;
    u32 r5;
    u32 r6;
    u32 m01;
    u32 m24;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    tmp = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = tmp;
    if (ctx[1] != 0) {
        m01 = 0x1000000;
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
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
            gte_nop();
            gte_nop();
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        goto next;
                    }
                }
                if ((ctx[0x1f] & m01) == 0) {
                    p = (u32 *)ctx[0x35];
                    gte_ldv3(p[0], p[1], p[2]);
                    gte_ldrgb(ctx[0x33]);
                    gte_nop();
                    gte_nop();
                    gte_ncct();
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m01;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x8000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                if ((ctx[0x1f] & m01) == 0) {
                    gte_strgb3(ctx + 0x2f, ctx + 0x30, ctx + 0x31);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 0xc));
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x32);
                } else {
                    c = *(u32 *)ctx[0x33];
                    ctx[0x32] = c;
                    ctx[0x31] = c;
                    ctx[0x30] = c;
                    ctx[0x2f] = c;
                }
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r6 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r6 = ctx[0x2d] & 0xff00;
                }
                r5 = 0xff0000;
                ctx[0x2d] = r6;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r5 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r5;
                *(u32 *)(ctx[3] + 0xc) = (ctx[0x20] | 0x3a000000) | ctx[0x2c] | ctx[0x2d] | r5;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x32) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x32] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x32] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x24) = ctx[0x2c] | ctx[0x2d] | r4;
                ctx[3] = ctx[3] + 0x2c;
            }
        next:
            ctx[0x33] = ctx[0x33] + 4;
            ctx[0x35] = ctx[0x35] + 0x10;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_g4_semitrans_38cc0  (render, 0x80038cc0)
 *
 * TMD submodel rasterizer: gouraud quads semi-transparent variant
 * (gte_stsxy3_g3 quad, POLY code 0x38/0x3a000000).
 */

void kaze_model_quad_g4_semitrans_38cc0(u32 *ctx)
{
    u32 tmp;
    u32 *p;
    u32 r4;
    u32 r5;
    u32 r6;
    u32 m24;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    tmp = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = tmp;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            ctx[4] = ctx[3];
            if (*(u32 *)ctx[0x33] & 0x2000000) {
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | 0x1000000;
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
            gte_nop();
            gte_nop();
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
                ctx[3] = ctx[4];
            } else {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        ctx[3] = ctx[4];
                        goto next;
                    }
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x8000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                if ((ctx[0x1f] & 0x1000000) == 0) {
                    gte_ldv0(*(u32 *)ctx[0x35]);
                    gte_ldrgb(ctx[0x33]);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x2f);
                    gte_ldrgb(ctx[0x33] + 4);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x30);
                    gte_ldrgb(ctx[0x33] + 8);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x31);
                    gte_ldrgb(ctx[0x33] + 0xc);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x32);
                } else {
                    ctx[0x2f] = *(u32 *)ctx[0x33];
                    ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                    ctx[0x31] = *(u32 *)(ctx[0x33] + 8);
                    ctx[0x32] = *(u32 *)(ctx[0x33] + 0xc);
                }
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r6 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r6 = ctx[0x2d] & 0xff00;
                }
                r5 = 0xff0000;
                ctx[0x2d] = r6;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r5 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r5;
                *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r5;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x32) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x32] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x32] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | r4;
                ctx[3] = ctx[3] + 0x24;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x10;
            ctx[0x35] = ctx[0x35] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_g4_392d4  (render, 0x800392d4)
 *
 * TMD submodel rasterizer: gouraud quads variant (gte_stsxy3_g3 quad path
 * with per-vertex normals, POLY code 0x38000000-family).
 */

void kaze_model_quad_g4_392d4(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 c1;
    u32 r4;
    u32 r5;
    u32 r6;
    u32 m24;
    u32 m1;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        m1 = 0x1000000;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
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
            gte_nop();
            gte_nop();
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        goto next;
                    }
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x8000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                if ((ctx[0x1f] & m1) == 0) {
                    gte_ldv0(*(u32 *)ctx[0x35]);
                    gte_ldrgb(ctx[0x33]);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x2f);
                    gte_ldrgb(ctx[0x33] + 4);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x30);
                    gte_ldrgb(ctx[0x33] + 8);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x31);
                    gte_ldrgb(ctx[0x33] + 0xc);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x32);
                } else {
                    ctx[0x2f] = *(u32 *)ctx[0x33];
                    ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                    ctx[0x31] = *(u32 *)(ctx[0x33] + 8);
                    ctx[0x32] = *(u32 *)(ctx[0x33] + 0xc);
                }
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r6 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r6 = ctx[0x2d] & 0xff00;
                }
                r5 = 0xff0000;
                ctx[0x2d] = r6;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r5 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r5;
                c1 = ctx[0x2c] | 0x3a000000;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x20] | c1 | ctx[0x2d] | r5;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x32) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x32] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x32] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x24) = ctx[0x2c] | ctx[0x2d] | r4;
                ctx[3] = ctx[3] + 0x2c;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x10;
            ctx[0x35] = ctx[0x35] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_g4_39898  (render, 0x80039898)
 *
 * TMD submodel rasterizer: gouraud quads variant (gte_stsxy3_g3 quad, per-
 * vertex lighting, POLY code 0x38/0x3a000000).
 */

void kaze_model_quad_g4_39898(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 r4;
    u32 r5;
    u32 r6;
    u32 m24;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            ctx[4] = ctx[3];
            if (*(u32 *)ctx[0x33] & 0x2000000) {
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | 0x1000000;
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
            gte_nop();
            gte_nop();
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
                ctx[3] = ctx[4];
            } else {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        ctx[3] = ctx[4];
                        goto next;
                    }
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x8000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                if ((ctx[0x1f] & 0x1000000) == 0) {
                    gte_ldv0(*(u32 *)ctx[0x35]);
                    gte_ldrgb(ctx[0x33]);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x2f);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 4));
                    gte_ldrgb(ctx[0x33] + 4);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x30);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 8));
                    gte_ldrgb(ctx[0x33] + 8);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x31);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 0xc));
                    gte_ldrgb(ctx[0x33] + 0xc);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x32);
                } else {
                    ctx[0x2f] = *(u32 *)ctx[0x33];
                    ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                    ctx[0x31] = *(u32 *)(ctx[0x33] + 8);
                    ctx[0x32] = *(u32 *)(ctx[0x33] + 0xc);
                }
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r6 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r6 = ctx[0x2d] & 0xff00;
                }
                r5 = 0xff0000;
                ctx[0x2d] = r6;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r5 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r5;
                *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r5;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x32) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x32] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x32] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | r4;
                ctx[3] = ctx[3] + 0x24;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x10;
            ctx[0x35] = ctx[0x35] + 0x10;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_g4_semitrans_39ef4  (render, 0x80039ef4)
 *
 * TMD submodel rasterizer: gouraud quads semi-transparent variant
 * (gte_stsxy3_g3 quad, POLY code 0x3a000000).
 */

void kaze_model_quad_g4_semitrans_39ef4(u32 *ctx)
{
    u32 *p;
    u32 t;
    u32 c1;
    u32 r4;
    u32 r5;
    u32 r6;
    u32 m24;
    u32 m1;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        m1 = 0x1000000;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
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
            gte_nop();
            gte_nop();
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        goto next;
                    }
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | m1;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x8000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                if ((ctx[0x1f] & m1) == 0) {
                    gte_ldv0(*(u32 *)ctx[0x35]);
                    gte_ldrgb(ctx[0x33]);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x2f);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 4));
                    gte_ldrgb(ctx[0x33] + 4);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x30);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 8));
                    gte_ldrgb(ctx[0x33] + 8);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x31);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 0xc));
                    gte_ldrgb(ctx[0x33] + 0xc);
                    gte_nop();
                    gte_nop();
                    gte_nccs();
                    gte_strgb(ctx + 0x32);
                } else {
                    ctx[0x2f] = *(u32 *)ctx[0x33];
                    ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                    ctx[0x31] = *(u32 *)(ctx[0x33] + 8);
                    ctx[0x32] = *(u32 *)(ctx[0x33] + 0xc);
                }
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r6 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r6 = ctx[0x2d] & 0xff00;
                }
                r5 = 0xff0000;
                ctx[0x2d] = r6;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r5 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r5;
                c1 = ctx[0x2c] | 0x3a000000;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x20] | c1 | ctx[0x2d] | r5;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x32) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x32] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x32] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x24) = ctx[0x2c] | ctx[0x2d] | r4;
                ctx[3] = ctx[3] + 0x2c;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x10;
            ctx[0x35] = ctx[0x35] + 0x10;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_ft4_3a500  (render, 0x8003a500)
 *
 * TMD submodel rasterizer: textured quads (rtpt+rtps 4th vertex,
 * gte_stsxy3_g3, avsz4_b, TPage/CLUT from +0x33 &0xfe000000).
 */

void kaze_model_quad_ft4_3a500(u32 *ctx)
{
    u32 c;
    u32 x;
    u32 idx;
    u32 *p;
    u32 m1;
    u32 m24;
    u32 mb0;
    u32 r4;
    u32 r5;
    u32 r6;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x35] = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    if (ctx[1] != 0) {
        m1 = 0x1000000;
        m24 = 0xffffff;
        mb0 = 0xff0000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            gte_stsxy3_g3(ctx[3]);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x20);
            ctx[0x1e] = *(u32 *)ctx[0x33] & 0xfe000000;
            gte_avsz4();
            if (ctx[0x20] != 0) {
                x = ctx[0x21];
                c = *(u16 *)(ctx[0x33] + 8) | x;
            } else {
                c = *(u32 *)(ctx[0x33] + 8) & 0x60ffff;
            }
            ctx[0x22] = c;
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        goto next;
                    }
                }
                if ((ctx[0x1f] & m1) == 0) {
                    gte_ldv0(*(u32 *)ctx[0x35]);
                    gte_nop();
                    gte_nop();
                    gte_ncs();
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x9000000;
                idx = *(u16 *)(ctx[0x33] + 6) & 7;
                ctx[0x36] = idx;
                *(u32 *)(ctx[3] + 0xc) =
                    *(u16 *)(ctx[0x33] + 4) | ctx[idx + 0xe] | ctx[idx + 0x16];
                *(u32 *)(ctx[3] + 0x14) =
                    ctx[0x22] | ctx[ctx[0x36] + 6] | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x1c) =
                    *(u32 *)(ctx[0x33] + 0xc) | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x24) =
                    *(u32 *)(ctx[0x33] + 0x10) | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                if ((ctx[0x1f] & m1) == 0) {
                    gte_strgb(ctx + 0x2f);
                } else {
                    ctx[0x2f] = 0x808080;
                }
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 8;
                if ((ctx[0x2c] & 0xffffff00) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 8;
                if ((ctx[0x2d] & 0xffff0000) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r6 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 8;
                if (((ctx[0x2e] & 0xff000000) << 8) == 0) {
                    r6 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r6;
                *(u32 *)(ctx[3] + 4) =
                    ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r6;
                ctx[3] = ctx[3] + 0x28;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x14;
            ctx[0x35] = ctx[0x35] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_gt4_3a904  (render, 0x8003a904)
 *
 * TMD submodel rasterizer: textured gouraud quads (gte_stsxy3_gt3 quad path,
 * TPage/CLUT, per-vertex color, POLY code 0x9000000-family).
 */

void kaze_model_quad_gt4_3a904(u32 *ctx)
{
    u32 t0v;
    u32 *p;
    u32 t;
    u32 r4;
    u32 r7;
    u32 r6;
    u32 m1;
    u32 m24;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    t0v = *(u32 *)(ctx[2] + 0xc);
    ctx[0x28] = 0;
    ctx[0x35] = t0v;
    if (ctx[1] != 0) {
        m1 = 0x1000000;
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            gte_stsxy3_gt3(ctx[3]);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x2c);
            ctx[0x1e] = *(u32 *)(ctx[0x33] + 0xc) & 0xfe000000;
            gte_avsz4();
            if (ctx[0x20] != 0) {
                ctx[0x22] = *(u16 *)(ctx[0x33] + 4) | ctx[0x21];
            } else {
                ctx[0x22] = *(u32 *)(ctx[0x33] + 4) & 0x60ffff;
            }
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        goto next;
                    }
                }
                if ((ctx[0x1f] & m1) == 0) {
                    p = (u32 *)ctx[0x35];
                    gte_ldv3(p[0], p[1], p[2]);
                    gte_nop();
                    gte_nop();
                    gte_nct();
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0xc000000;
                ctx[0x36] = *(u16 *)(ctx[0x33] + 2) & 7;
                *(u32 *)(ctx[3] + 0xc) = *(u16 *)ctx[0x33] | ctx[ctx[0x36] + 0xe] | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x18) = ctx[0x22] | ctx[ctx[0x36] + 6] | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x24) = *(u32 *)(ctx[0x33] + 8) | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x30) = *(u32 *)(ctx[0x33] + 0xc) | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                if ((ctx[0x1f] & m1) == 0) {
                    gte_strgb3(ctx + 0x2f, ctx + 0x30, ctx + 0x31);
                    gte_ldv0(*(u32 *)(ctx[0x35] + 0xc));
                    gte_nop();
                    gte_nop();
                    gte_ncs();
                    gte_strgb(ctx + 0x32);
                } else {
                    ctx[0x32] = 0x808080;
                    ctx[0x31] = 0x808080;
                    ctx[0x30] = 0x808080;
                    ctx[0x2f] = 0x808080;
                }
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 8;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r7 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 8;
                if ((ctx[0x2d] & mg) == 0) {
                    r7 = ctx[0x2d] & 0xff00;
                }
                r6 = 0xff0000;
                ctx[0x2d] = r7;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 8;
                if ((ctx[0x2e] & mb) == 0) {
                    r6 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r6;
                *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r6;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 8;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r6 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 8;
                if ((ctx[0x2d] & mg) == 0) {
                    r6 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r6;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 8;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x10) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 8;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r6 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 8;
                if ((ctx[0x2d] & mg) == 0) {
                    r6 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r6;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 8;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x32) * ctx[0x25] >> 8;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r6 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x32] & 0xff00) * ctx[0x26] >> 8;
                if ((ctx[0x2d] & mg) == 0) {
                    r6 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r6;
                ctx[0x2e] = (ctx[0x32] & mb0) * ctx[0x27] >> 8;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x28) = ctx[0x2c] | ctx[0x2d] | r4;
                ctx[3] = ctx[3] + 0x34;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x10;
            ctx[0x35] = ctx[0x35] + 0x10;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_tri_f3_nolight_3af60  (render, 0x8003af60)
 *
 * TMD submodel rasterizer: flat triangles using stored colors without
 * lighting (gte_stsxy3_f3, POLY code 0x20/0x22000000); reads verts from
 * param+0x34 only.
 */

void kaze_model_tri_f3_nolight_3af60(u32 *arg)
{
    register u32 *p asm("$5");
    register u32 mask asm("$7");
    register u32 red_mask asm("$8");
    register u32 np asm("$3");
    u32 *vp;
    u32 c;
    u32 r;
    u32 g;
    u32 b;

    p = arg;
    p[1] = *(u32 *)p[2];
    p[0x33] = *(u32 *)(p[2] + 4);
    p[0x34] = *(u32 *)(p[2] + 8);
    p[0x28] = 0;
    if (p[1] == 0) {
        return;
    }

    mask = 0x00ffffff;
    red_mask = 0x00ff0000;
loop:
    vp = (u32 *)p[0x34];
    gte_ldv3((void *)vp[0], (void *)vp[1], (void *)vp[2]);
    p[0x34] = (u32)(vp + 3);
    gte_rtpt();
    p[0x28] = p[0x28] + 1;
    if ((p[0x24] & 0x100) == 0) {
        gte_nclip();
        gte_stopz(p + 0x2a);
        if ((int)p[0x2a] <= 0) {
            goto next;
        }
    }
    gte_avsz3();
    gte_stotz(p + 0x29);
    if ((int)p[0x29] > 0) {
        p[4] = p[3];
        if (*(u32 *)p[0x33] & 0x2000000) {
            *(u32 *)p[3] = ((u32)((u32 *)p[3] + 2) & mask) | 0x1000000;
            *(u32 *)(p[3] + 4) = p[0x23];
            p[0x1e] = 0x22000000;
            np = p[3] + 8;
            p[3] = np;
        } else {
            p[0x1e] = 0x20000000;
        }
        gte_stsxy3_f3((void *)p[3]);
        p[0x29] = (int)p[0x29] >> p[0];
        *(u32 *)p[3] = (*(u32 *)(p[0x29] * 4 + p[5]) & mask) | 0x4000000;
        *(u32 *)(p[0x29] * 4 + p[5]) = p[4] & mask;
        p[0x2f] = *(u32 *)p[0x33];
        r = 0xff;
        c = (u32)(u8)p[0x2f] * p[0x25] >> 7;
        p[0x2c] = c;
        if ((c & 0xffffff00) == 0) r = c & 0xff;
        g = 0xff00;
        p[0x2c] = r;
        c = (p[0x2f] & 0xff00) * p[0x26] >> 7;
        p[0x2d] = c;
        if ((c & 0xffff0000) == 0) g = c & 0xff00;
        b = 0xff0000;
        p[0x2d] = g;
        c = (p[0x2f] & red_mask) * p[0x27] >> 7;
        p[0x2e] = c;
        if ((c & 0xff000000) == 0) b = c & red_mask;
        p[0x2e] = b;
        *(u32 *)(p[3] + 4) = p[0x1e] | p[0x20] | p[0x2c] | p[0x2d] | b;
        p[3] = p[3] + 0x14;
    }
next:
    p[0x33] = p[0x33] + 4;
    if (p[0x28] < p[1]) {
        goto loop;
    }
}

/**
 * kaze_model_tri_f3_nolight_semitrans_3b1fc  (render, 0x8003b1fc)
 *
 * TMD submodel rasterizer: unlit flat triangles semi-transparent variant
 * (gte_stsxy3_f3, POLY code 0x22000000, stride 0x1c).
 */

void kaze_model_tri_f3_nolight_semitrans_3b1fc(u32 *arg)
{
    register u32 *p asm("$5");
    register u32 mask asm("$7");
    register u32 blue_mask asm("$8");
    u32 *vp;
    u32 c;
    u32 r;
    u32 g;
    u32 b;

    p = arg;
    p[1] = *(u32 *)p[2];
    p[0x33] = *(u32 *)(p[2] + 4);
    p[0x34] = *(u32 *)(p[2] + 8);
    p[0x28] = 0;
    if (p[1] == 0) {
        return;
    }

    mask = 0x00ffffff;
    blue_mask = 0x00ff0000;
loop:
    vp = (u32 *)p[0x34];
    gte_ldv3((void *)vp[0], (void *)vp[1], (void *)vp[2]);
    p[0x34] = (u32)(vp + 3);
    gte_rtpt();
    p[0x28] = p[0x28] + 1;
    if ((p[0x24] & 0x100) == 0) {
        gte_nclip();
        gte_stopz(p + 0x2a);
        if ((int)p[0x2a] <= 0) {
            goto tail;
        }
    }
    gte_avsz3();
    gte_stotz(p + 0x29);
    if ((int)p[0x29] > 0) {
        gte_stsxy3_f3((void *)(p[3] + 8));
        p[0x29] = (int)p[0x29] >> p[0];
        *(u32 *)p[3] = ((u32)((u32 *)p[3] + 2) & mask) | 0x1000000;
        *(u32 *)(p[3] + 4) = p[0x23];
        *(u32 *)(p[3] + 8) = (*(u32 *)(p[0x29] * 4 + p[5]) & mask) | 0x4000000;
        *(u32 *)(p[0x29] * 4 + p[5]) = p[3] & mask;
        p[0x2f] = *(u32 *)p[0x33];
        r = 0xff;
        c = (u32)(u8)p[0x2f] * p[0x25] >> 7;
        p[0x2c] = c;
        if ((c & 0xffffff00) == 0) r = c & 0xff;
        g = 0xff00;
        p[0x2c] = r;
        c = (p[0x2f] & 0xff00) * p[0x26] >> 7;
        p[0x2d] = c;
        if ((c & 0xffff0000) == 0) g = c & 0xff00;
        b = 0xff0000;
        p[0x2d] = g;
        c = (p[0x2f] & blue_mask) * p[0x27] >> 7;
        p[0x2e] = c;
        if ((c & 0xff000000) == 0) b = c & blue_mask;
        p[0x2e] = b;
        *(u32 *)(p[3] + 0xc) = p[0x20] | 0x22000000 | p[0x2c] | p[0x2d] | b;
        p[3] = p[3] + 0x1c;
    }
tail:
    p[0x33] = p[0x33] + 4;
    if (p[0x28] < p[1]) {
        goto loop;
    }
}

/**
 * kaze_model_tri_g3_nolight_3b460  (render, 0x8003b460)
 *
 * TMD submodel rasterizer: unlit gouraud triangles using stored per-vertex
 * colors (gte_stsxy3_g3, POLY code 0x30/0x32000000).
 */

void kaze_model_tri_g3_nolight_3b460(u32 *arg)
{
    register u32 *ctx asm("$5");
    register u32 m24 asm("$9");
    register u32 mr asm("$12");
    register u32 mg asm("$11");
    register u32 mb0 asm("$8");
    register u32 mb asm("$10");
    u32 *vp;
    u32 tmp;
    u32 c;
    u32 r;
    u32 g;
    u32 b;
    u32 g2;
    u32 b2;

    ctx = arg;
    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    tmp = *(u32 *)(ctx[2] + 8);
    ctx[0x28] = 0;
    ctx[0x34] = tmp;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            vp = (u32 *)ctx[0x34];
            gte_ldv3((void *)vp[0], (void *)vp[1], (void *)vp[2]);
            ctx[0x34] = (u32)(vp + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            if ((ctx[0x24] & 0x100) == 0) {
                gte_nclip();
                gte_stopz(ctx + 0x2a);
                if ((int)ctx[0x2a] <= 0) goto skip;
            }
            gte_avsz3();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] >= 1) {
                ctx[4] = ctx[3];
                if (*(u32 *)ctx[0x33] & 0x2000000) {
                    *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | 0x1000000;
                    *(u32 *)(ctx[3] + 4) = ctx[0x23];
                    ctx[0x1e] = 0x32000000;
                    ctx[3] = ctx[3] + 8;
                } else {
                    ctx[0x1e] = 0x30000000;
                }
                gte_stsxy3_g3((void *)ctx[3]);
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x6000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                ctx[0x2f] = *(u32 *)ctx[0x33];
                ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                r = 0xff;
                ctx[0x31] = *(u32 *)(ctx[0x33] + 8);
                c = (u32)(u8)ctx[0x2f] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g = c & 0xff00;
                b = 0xff0000;
                ctx[0x2d] = g;
                c = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b = c & mb0;
                ctx[0x2e] = b;
                *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | b;
                r = 0xff;
                c = (u32)(u8)ctx[0x30] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g2 = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g2 = c & 0xff00;
                b2 = 0xff0000;
                ctx[0x2d] = g2;
                c = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b2 = c & mb0;
                ctx[0x2e] = b2;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x2c] | ctx[0x2d] | b2;
                r = 0xff;
                c = (u32)(u8)ctx[0x31] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g2 = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g2 = c & 0xff00;
                b2 = 0xff0000;
                ctx[0x2d] = g2;
                c = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b2 = c & mb0;
                ctx[0x2e] = b2;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | b2;
                ctx[3] = ctx[3] + 0x1c;
            }
skip:
            ctx[0x33] = ctx[0x33] + 0xc;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_tri_g3_nolight_semitrans_3b870  (render, 0x8003b870)
 *
 * TMD submodel rasterizer: unlit gouraud triangles semi-transparent variant
 * (gte_stsxy3_g3, POLY code 0x32000000).
 */

void kaze_model_tri_g3_nolight_semitrans_3b870(u32 *arg)
{
    register u32 *ctx asm("$5");
    register u32 m24 asm("$9");
    register u32 mr asm("$12");
    register u32 mg asm("$11");
    register u32 mb0 asm("$8");
    register u32 mb asm("$10");
    u32 *vp;
    u32 tmp;
    u32 c;
    u32 r;
    u32 g;
    u32 b;
    u32 g2;
    u32 b2;

    ctx = arg;
    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    tmp = *(u32 *)(ctx[2] + 8);
    ctx[0x28] = 0;
    ctx[0x34] = tmp;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            vp = (u32 *)ctx[0x34];
            gte_ldv3((void *)vp[0], (void *)vp[1], (void *)vp[2]);
            ctx[0x34] = (u32)(vp + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            if ((ctx[0x24] & 0x100) == 0) {
                gte_nclip();
                gte_stopz(ctx + 0x2a);
                if ((int)ctx[0x2a] <= 0) goto tail;
            }
            gte_avsz3();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] > 0) {
                gte_stsxy3_g3((void *)(ctx[3] + 8));
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | 0x1000000;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x6000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                ctx[0x2f] = *(u32 *)ctx[0x33];
                ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                r = 0xff;
                ctx[0x31] = *(u32 *)(ctx[0x33] + 8);
                c = (u32)(u8)ctx[0x2f] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g = c & 0xff00;
                b = 0xff0000;
                ctx[0x2d] = g;
                c = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b = c & mb0;
                ctx[0x2e] = b;
                *(u32 *)(ctx[3] + 0xc) =
                    ctx[0x20] | 0x32000000 | ctx[0x2c] | ctx[0x2d] | b;
                r = 0xff;
                c = (u32)(u8)ctx[0x30] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g2 = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g2 = c & 0xff00;
                b2 = 0xff0000;
                ctx[0x2d] = g2;
                c = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b2 = c & mb0;
                ctx[0x2e] = b2;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | b2;
                r = 0xff;
                c = (u32)(u8)ctx[0x31] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g2 = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g2 = c & 0xff00;
                b2 = 0xff0000;
                ctx[0x2d] = g2;
                c = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b2 = c & mb0;
                ctx[0x2e] = b2;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | b2;
                ctx[3] = ctx[3] + 0x24;
            }
tail:
            ctx[0x33] = ctx[0x33] + 0xc;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_tri_ft3_nolight_3bc48  (render, 0x8003bc48)
 *
 * TMD submodel rasterizer: unlit textured triangles (gte_stsxy3, TPage/CLUT
 * from +0x33 &0xfe000000, texture window fields).
 */

void kaze_model_tri_ft3_nolight_3bc48(u32 *arg)
{
    register u32 *ctx asm("$5");
    register u32 m24 asm("$8");
    register u32 mb0 asm("$9");
    u32 c;
    u32 cc;
    u32 col;
    u32 x;
    u32 idx;
    u32 *p;
    u32 r4;
    u32 r5;
    u32 r6;

    ctx = arg;
    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x28] = 0;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mb0 = 0xff0000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            if ((ctx[0x24] & 0x100) == 0) {
                gte_nclip();
                gte_stopz(ctx + 0x2a);
                if ((int)ctx[0x2a] < 1) {
                    goto next;
                }
            }
            gte_avsz3();
            ctx[0x1e] = *(u32 *)ctx[0x33] & 0xfe000000;
            if (ctx[0x20] != 0) {
                x = ctx[0x21];
                cc = *(u16 *)(ctx[0x33] + 8) | x;
            } else {
                cc = *(u32 *)(ctx[0x33] + 8) & 0x60ffff;
            }
            ctx[0x22] = cc;
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                gte_stsxy3_g3(ctx[3]);
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x7000000;
                idx = *(u16 *)(ctx[0x33] + 6) & 7;
                ctx[0x36] = idx;
                *(u32 *)(ctx[3] + 0xc) =
                    *(u16 *)(ctx[0x33] + 4) | ctx[idx + 0xe] | ctx[idx + 0x16];
                *(u32 *)(ctx[3] + 0x14) =
                    ctx[0x22] | ctx[ctx[0x36] + 6] | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x1c) =
                    *(u32 *)(ctx[0x33] + 0xc) | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                col = *(u32 *)ctx[0x33];
                ctx[0x2f] = col;
                r4 = 0xff;
                c = (col & 0xff) * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & 0xffffff00) == 0) {
                    r4 = c & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                c = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & 0xffff0000) == 0) {
                    r5 = c & 0xff00;
                }
                r6 = 0xff0000;
                ctx[0x2d] = r5;
                c = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & 0xff000000) == 0) {
                    r6 = c & mb0;
                }
                ctx[0x2e] = r6;
                *(u32 *)(ctx[3] + 4) =
                    ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r6;
                ctx[3] = ctx[3] + 0x20;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x10;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_tri_gt3_nolight_3bf54  (render, 0x8003bf54)
 *
 * TMD submodel rasterizer: unlit textured gouraud triangles (gte_stsxy3_gt3,
 * TPage/CLUT, stored per-vertex colors).
 */

void kaze_model_tri_gt3_nolight_3bf54(u32 *arg)
{
    register u32 *ctx asm("$5");
    register u32 m24 asm("$9");
    register u32 mr asm("$12");
    register u32 mg asm("$11");
    register u32 mb0 asm("$8");
    register u32 mb asm("$10");
    u32 *p;
    u32 cc;
    u32 x;
    u32 idx;
    u32 c;
    u32 r;
    u32 g;
    u32 b;
    u32 g2;
    u32 b2;

    ctx = arg;
    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x28] = 0;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            if ((ctx[0x24] & 0x100) == 0) {
                gte_nclip();
                gte_stopz(ctx + 0x2a);
                if ((int)ctx[0x2a] < 1) {
                    goto next;
                }
            }
            gte_avsz3();
            ctx[0x1e] = *(u32 *)ctx[0x33] & 0xfe000000;
            if (ctx[0x20] != 0) {
                x = ctx[0x21];
                cc = *(u16 *)(ctx[0x33] + 0xc) | x;
            } else {
                cc = *(u32 *)(ctx[0x33] + 0xc) & 0x60ffff;
            }
            ctx[0x22] = cc;
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                gte_stsxy3_gt3(ctx[3]);
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x9000000;
                idx = *(u16 *)(ctx[0x33] + 6) & 7;
                ctx[0x36] = idx;
                *(u32 *)(ctx[3] + 0xc) =
                    *(u16 *)(ctx[0x33] + 4) | ctx[idx + 0xe] | ctx[idx + 0x16];
                *(u32 *)(ctx[3] + 0x18) =
                    ctx[0x22] | ctx[ctx[0x36] + 6] | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x24) =
                    *(u32 *)(ctx[0x33] + 0x14) | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                ctx[0x2f] = *(u32 *)ctx[0x33];
                ctx[0x30] = *(u32 *)(ctx[0x33] + 8);
                r = 0xff;
                ctx[0x31] = *(u32 *)(ctx[0x33] + 0x10);
                c = (u32)(u8)ctx[0x2f] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g = c & 0xff00;
                b = 0xff0000;
                ctx[0x2d] = g;
                c = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b = c & mb0;
                ctx[0x2e] = b;
                *(u32 *)(ctx[3] + 4) =
                    ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | b;
                r = 0xff;
                c = (u32)(u8)ctx[0x30] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g2 = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g2 = c & 0xff00;
                b2 = 0xff0000;
                ctx[0x2d] = g2;
                c = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b2 = c & mb0;
                ctx[0x2e] = b2;
                *(u32 *)(ctx[3] + 0x10) = ctx[0x2c] | ctx[0x2d] | b2;
                r = 0xff;
                c = (u32)(u8)ctx[0x31] * ctx[0x25] >> 7;
                ctx[0x2c] = c;
                if ((c & mr) == 0) r = c & 0xff;
                g2 = 0xff00;
                ctx[0x2c] = r;
                c = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                ctx[0x2d] = c;
                if ((c & mg) == 0) g2 = c & 0xff00;
                b2 = 0xff0000;
                ctx[0x2d] = g2;
                c = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                ctx[0x2e] = c;
                if ((c & mb) == 0) b2 = c & mb0;
                ctx[0x2e] = b2;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | b2;
                ctx[3] = ctx[3] + 0x28;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x18;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_f4_nolight_3c3d0  (render, 0x8003c3d0)
 *
 * TMD submodel rasterizer: unlit flat quads (rtpt+rtps 4th vertex,
 * gte_stsxy3_f3, avsz4, POLY code 0x28/0x2a000000).
 */

void kaze_model_quad_f4_nolight_3c3d0(u32 *ctx)
{
    u32 *p;
    u32 r5;
    u32 r4;
    u32 m24;
    u32 mb0;
    u32 r6;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x28] = 0;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mb0 = 0xff0000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            if (*(u32 *)ctx[0x33] & 0x2000000) {
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | 0x1000000;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                ctx[0x1e] = 0x2a000000;
                ctx[4] = ctx[3];
                ctx[3] = ctx[3] + 8;
            } else {
                ctx[0x1e] = 0x28000000;
                ctx[4] = ctx[3];
            }
            gte_stopz(ctx + 0x2a);
            gte_stsxy3_f3(ctx[3]);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x14);
            gte_nop();
            gte_nop();
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
                ctx[3] = ctx[4];
            } else {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        ctx[3] = ctx[4];
                        goto next;
                    }
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x5000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                ctx[0x2f] = *(u32 *)ctx[0x33];
                r5 = 0xff;
                ctx[0x2c] = (ctx[0x2f] & 0xff) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & 0xffffff00) == 0) {
                    r5 = ctx[0x2c] & 0xff;
                }
                r4 = 0xff00;
                ctx[0x2c] = r5;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & 0xffff0000) == 0) {
                    r4 = ctx[0x2d] & 0xff00;
                }
                r6 = 0xff0000;
                ctx[0x2d] = r4;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & 0xff000000) == 0) {
                    r6 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r6;
                *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r6;
                ctx[3] = ctx[3] + 0x18;
            }
        next:
            ctx[0x33] = ctx[0x33] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_f4_nolight_semitrans_3c6f4  (render, 0x8003c6f4)
 *
 * TMD submodel rasterizer: unlit flat quads semi-transparent variant
 * (gte_stsxy3_f3 quad, avsz4, POLY code 0x2a000000, stride 0x20).
 */

void kaze_model_quad_f4_nolight_semitrans_3c6f4(u32 *ctx)
{
    u32 *p;
    u32 r5;
    u32 r4;
    u32 m24;
    u32 mb0;
    u32 r6;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x28] = 0;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mb0 = 0xff0000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            gte_stsxy3_f3(ctx[3] + 8);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x1c);
            gte_nop();
            gte_nop();
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        goto next;
                    }
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | 0x1000000;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x5000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                ctx[0x2f] = *(u32 *)ctx[0x33];
                r5 = 0xff;
                ctx[0x2c] = (ctx[0x2f] & 0xff) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & 0xffffff00) == 0) {
                    r5 = ctx[0x2c] & 0xff;
                }
                r4 = 0xff00;
                ctx[0x2c] = r5;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & 0xffff0000) == 0) {
                    r4 = ctx[0x2d] & 0xff00;
                }
                r6 = 0xff0000;
                ctx[0x2d] = r4;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & 0xff000000) == 0) {
                    r6 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r6;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x20] | 0x2a000000 | ctx[0x2c] | ctx[0x2d] | r6;
                ctx[3] = ctx[3] + 0x20;
            }
        next:
            ctx[0x33] = ctx[0x33] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_g4_nolight_3c9bc  (render, 0x8003c9bc)
 *
 * TMD submodel rasterizer: unlit gouraud quads using stored colors
 * (gte_stsxy3_g3 quad, avsz4, POLY code 0x38/0x3a000000).
 */

void kaze_model_quad_g4_nolight_3c9bc(u32 *ctx)
{
    u32 *p;
    u32 r5;
    u32 r4;
    u32 m24;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x28] = 0;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            if (*(u32 *)ctx[0x33] & 0x2000000) {
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | 0x1000000;
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
            gte_nop();
            gte_nop();
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] < 1) {
                ctx[3] = ctx[4];
            } else {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        ctx[3] = ctx[4];
                        goto next;
                    }
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x8000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[4] & m24;
                ctx[0x2f] = *(u32 *)ctx[0x33];
                ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                ctx[0x31] = *(u32 *)(ctx[0x33] + 8);
                r5 = 0xff;
                ctx[0x32] = *(u32 *)(ctx[0x33] + 0xc);
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r5 = ctx[0x2c] & 0xff;
                }
                r4 = 0xff00;
                ctx[0x2c] = r5;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r4 = ctx[0x2d] & 0xff00;
                }
                r5 = 0xff0000;
                ctx[0x2d] = r4;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r5 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r5;
                *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r5;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x32) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x32] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x32] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | r4;
                ctx[3] = ctx[3] + 0x24;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x10;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_g4_nolight_semitrans_3cf00  (render, 0x8003cf00)
 *
 * TMD submodel rasterizer: unlit gouraud quads semi-transparent variant
 * (gte_stsxy3_g3 quad, rtps 4th vertex, POLY code 0x3a000000).
 */

void kaze_model_quad_g4_nolight_semitrans_3cf00(u32 *ctx)
{
    u32 *p;
    u32 r5;
    u32 r4;
    u32 m24;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;
    u32 hh;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x28] = 0;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
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
            gte_nop();
            gte_nop();
            gte_avsz4();
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        goto next;
                    }
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = ((u32)((u32 *)ctx[3] + 2) & m24) | 0x1000000;
                *(u32 *)(ctx[3] + 4) = ctx[0x23];
                *(u32 *)(ctx[3] + 8) = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x8000000;
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                ctx[0x2f] = *(u32 *)ctx[0x33];
                ctx[0x30] = *(u32 *)(ctx[0x33] + 4);
                ctx[0x31] = *(u32 *)(ctx[0x33] + 8);
                r5 = 0xff;
                ctx[0x32] = *(u32 *)(ctx[0x33] + 0xc);
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r5 = ctx[0x2c] & 0xff;
                }
                r4 = 0xff00;
                ctx[0x2c] = r5;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r4 = ctx[0x2d] & 0xff00;
                }
                r5 = 0xff0000;
                ctx[0x2d] = r4;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r5 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r5;
                hh = ctx[0x2c] | 0x3a000000;
                *(u32 *)(ctx[3] + 0xc) = ctx[0x20] | hh | ctx[0x2d] | r5;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x14) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x32) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x32] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x32] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x24) = ctx[0x2c] | ctx[0x2d] | r4;
                ctx[3] = ctx[3] + 0x2c;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x10;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_ft4_nolight_3d3e8  (render, 0x8003d3e8)
 *
 * TMD submodel rasterizer: unlit textured quads (gte_stsxy3_g3+rtps 4th
 * vertex, avsz4_b, TPage/CLUT from +0x33 &0xfe000000).
 */

void kaze_model_quad_ft4_nolight_3d3e8(u32 *ctx)
{
    u32 c;
    u32 x;
    u32 idx;
    u32 *p;
    u32 m24;
    u32 mb0;
    u32 r4;
    u32 r5;
    u32 r6;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x28] = 0;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mb0 = 0xff0000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            gte_stsxy3_g3(ctx[3]);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x20);
            ctx[0x2f] = *(u32 *)ctx[0x33];
            gte_avsz4();
            ctx[0x1e] = *(u32 *)ctx[0x33] & 0xfe000000;
            if (ctx[0x20] != 0) {
                x = ctx[0x21];
                c = *(u16 *)(ctx[0x33] + 8) | x;
            } else {
                c = *(u32 *)(ctx[0x33] + 8) & 0x60ffff;
            }
            ctx[0x22] = c;
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        goto next;
                    }
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x9000000;
                idx = *(u16 *)(ctx[0x33] + 6) & 7;
                ctx[0x36] = idx;
                *(u32 *)(ctx[3] + 0xc) =
                    *(u16 *)(ctx[0x33] + 4) | ctx[idx + 0xe] | ctx[idx + 0x16];
                *(u32 *)(ctx[3] + 0x14) =
                    ctx[0x22] | ctx[ctx[0x36] + 6] | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x1c) =
                    *(u32 *)(ctx[0x33] + 0xc) | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x24) =
                    *(u32 *)(ctx[0x33] + 0x10) | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & 0xffffff00) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & 0xffff0000) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r6 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & 0xff000000) == 0) {
                    r6 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r6;
                *(u32 *)(ctx[3] + 4) =
                    ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r6;
                ctx[3] = ctx[3] + 0x28;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x14;
        } while (ctx[0x28] < ctx[1]);
    }
}

/**
 * kaze_model_quad_gt4_nolight_3d778  (render, 0x8003d778)
 *
 * TMD submodel rasterizer: unlit textured gouraud quads (gte_stsxy3_gt3 quad
 * path, TPage/CLUT, stored per-vertex colors).
 */

void kaze_model_quad_gt4_nolight_3d778(u32 *ctx)
{
    u32 *p;
    u32 r5;
    u32 r4;
    u32 m24;
    u32 mr;
    u32 mg;
    u32 mb0;
    u32 mb;
    u32 t;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x28] = 0;
    if (ctx[1] != 0) {
        m24 = 0xffffff;
        mr = 0xffffff00;
        mg = 0xffff0000;
        mb0 = 0xff0000;
        mb = 0xff000000;
        do {
            p = (u32 *)ctx[0x34];
            gte_ldv3(p[0], p[1], p[2]);
            ctx[0x34] = (u32)(p + 3);
            gte_rtpt();
            ctx[0x28] = ctx[0x28] + 1;
            gte_nclip();
            gte_stopz(ctx + 0x2a);
            gte_stsxy3_gt3(ctx[3]);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x2c);
            ctx[0x1e] = *(u32 *)ctx[0x33] & 0xfe000000;
            gte_avsz4();
            if (ctx[0x20] != 0) {
                ctx[0x22] = *(u16 *)(ctx[0x33] + 0xc) | ctx[0x21];
            } else {
                ctx[0x22] = *(u32 *)(ctx[0x33] + 0xc) & 0x60ffff;
            }
            gte_stotz(ctx + 0x29);
            if ((int)ctx[0x29] > 0) {
                if ((ctx[0x24] & 0x100) == 0) {
                    gte_nclip();
                    gte_stopz(ctx + 0x2b);
                    if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                        goto next;
                    }
                }
                ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0xc000000;
                t = *(u16 *)(ctx[0x33] + 6) & 7;
                ctx[0x36] = t;
                *(u32 *)(ctx[3] + 0xc) =
                    *(u16 *)(ctx[0x33] + 4) | ctx[t + 0xe] | ctx[t + 0x16];
                *(u32 *)(ctx[3] + 0x18) =
                    ctx[0x22] | ctx[ctx[0x36] + 6] | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x24) =
                    *(u32 *)(ctx[0x33] + 0x14) | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[3] + 0x30) =
                    *(u32 *)(ctx[0x33] + 0x1c) | ctx[ctx[0x36] + 0x16];
                *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                ctx[0x2f] = *(u32 *)ctx[0x33];
                ctx[0x30] = *(u32 *)(ctx[0x33] + 8);
                ctx[0x31] = *(u32 *)(ctx[0x33] + 0x10);
                r5 = 0xff;
                ctx[0x32] = *(u32 *)(ctx[0x33] + 0x18);
                ctx[0x2c] = *(u8 *)(ctx + 0x2f) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r5 = ctx[0x2c] & 0xff;
                }
                r4 = 0xff00;
                ctx[0x2c] = r5;
                ctx[0x2d] = (ctx[0x2f] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r4 = ctx[0x2d] & 0xff00;
                }
                r5 = 0xff0000;
                ctx[0x2d] = r4;
                ctx[0x2e] = (ctx[0x2f] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r5 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r5;
                *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2c] | ctx[0x2d] | r5;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x30) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x30] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x30] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x10) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x31) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x31] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x31] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x1c) = ctx[0x2c] | ctx[0x2d] | r4;
                r4 = 0xff;
                ctx[0x2c] = *(u8 *)(ctx + 0x32) * ctx[0x25] >> 7;
                if ((ctx[0x2c] & mr) == 0) {
                    r4 = ctx[0x2c] & 0xff;
                }
                r5 = 0xff00;
                ctx[0x2c] = r4;
                ctx[0x2d] = (ctx[0x32] & 0xff00) * ctx[0x26] >> 7;
                if ((ctx[0x2d] & mg) == 0) {
                    r5 = ctx[0x2d] & 0xff00;
                }
                r4 = 0xff0000;
                ctx[0x2d] = r5;
                ctx[0x2e] = (ctx[0x32] & mb0) * ctx[0x27] >> 7;
                if ((ctx[0x2e] & mb) == 0) {
                    r4 = ctx[0x2e] & mb0;
                }
                ctx[0x2e] = r4;
                *(u32 *)(ctx[3] + 0x28) = ctx[0x2c] | ctx[0x2d] | r4;
                ctx[3] = ctx[3] + 0x34;
            }
        next:
            ctx[0x33] = ctx[0x33] + 0x20;
        } while (ctx[0x28] < ctx[1]);
    }
}

void kaze_model_quad_gt4_fixedlight_3ecdc();
void kaze_model_quad_ft4_fixedlight_3e97c();
void kaze_model_tri_gt3_fixedlight_3e638();
void kaze_model_tri_ft3_fixedlight_3e358();

/**
 * kaze_draw_model_flatlit_3dd2c  (render, 0x8003dd2c)
 *
 * Secondary model draw dispatch (matrix/model helper): like kaze_draw_model
 * but seeds a fixed 0x808080 light color and routes to an alternate set of
 * submodel primitive rasterizers, branching on flag 0x40000000. Anchor:...
 */

int kaze_draw_model_flatlit_3dd2c(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
  Ctx *sp = (Ctx *) 0x1F800200;
  int i;
  int t;
  sp->unk90 = arg1;
  if (arg1 < 0)
  {
    return arg3;
  }
  sp->unk0 = arg4;
  sp->unk8 = (int *) (arg0 + 0x198);
  t = (((u32) arg1) >> 5) & 0x02000000;
  sp->unk80 = t;
  sp->unk4 = 0;
  sp->unkC = arg3;
  sp->unk14 = arg2;
  sp->unk7C = t | ((arg1 << 0x12) & 0x01000000);
  sp->unk84 = (((u32) arg1) >> 7) & 0x600000;
  sp->unk8C = ((((u32) arg1) >> 0x17) & 0x60) | 0xE1000200;
  sp->unk94 = 0x80;
  sp->unk98 = 0x80;
  sp->unk9C = 0x80;
  sp->unkA0 = 0;
  do
  {
    {
      int k = sp->unkA0 * 4;
      *((int *) (k + (int) sp + 0x18)) = *((int *) ((k + arg0) + 8));
    }
    {
      int k = sp->unkA0 * 4;
      *((int *) (k + (int) sp + 0x38)) = *((int *) ((k + arg0) + 0x28));
    }
    {
      int k = sp->unkA0 * 4;
      *((int *) (k + (int) sp + 0x58)) = *((int *) ((k + arg0) + 0x48));
    }
    sp->unkA0 = (i = sp->unkA0 + 1);
  }
  while (((u32) i) < 8);
  if (sp->unk8[0] != 0)
  {
    kaze_model_quad_gt4_nolight_3d778(sp);
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_quad_gt4_fixedlight_3ecdc(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_quad_ft4_nolight_3d3e8(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_quad_ft4_fixedlight_3e97c(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_quad_g4_nolight_semitrans_3cf00(sp);
      }
      else
      {
        kaze_model_quad_g4_nolight_3c9bc(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_tmd_quad_gouraud_4normal_tpage_1ce8(sp);
      }
      else
      {
        kaze_tmd_quad_gouraud_4normal_1910(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_tmd_quad_grad_1normal_tpage_15d0(sp);
      }
      else
      {
        kaze_tmd_quad_grad_1normal_1240(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_tmd_quad_gouraud_lit_tpage_0f24(sp);
      }
      else
      {
        kaze_tmd_quad_gouraud_lit_0bac(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_quad_f4_nolight_semitrans_3c6f4(sp);
      }
      else
      {
        kaze_model_quad_f4_nolight_3c3d0(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_tmd_quad_flat_lit_tpage_0924(sp);
      }
      else
      {
        kaze_tmd_quad_flat_lit_06c8(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_tri_gt3_nolight_3bf54(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_tri_gt3_fixedlight_3e638(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_tri_ft3_nolight_3bc48(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      kaze_model_tri_ft3_fixedlight_3e358(sp);
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_tri_g3_nolight_semitrans_3b870(sp);
      }
      else
      {
        kaze_model_tri_g3_nolight_3b460(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_tmd_tri_gouraud_3normal_tpage_03dc(sp);
      }
      else
      {
        kaze_tmd_tri_gouraud_3normal_00b8(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_tmd_tri_grad_1normal_tpage_fde4(sp);
      }
      else
      {
        kaze_tmd_tri_grad_1normal_fad8(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_tmd_tri_gouraud_lit_tpage_f848(sp);
      }
      else
      {
        kaze_tmd_tri_gouraud_lit_f580(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_model_tri_f3_nolight_semitrans_3b1fc(sp);
      }
      else
      {
        kaze_model_tri_f3_nolight_3af60(sp);
      }
    }
  }
  {
    int *p = sp->unk8;
    sp->unk8 = p - 4;
    if (p[-4] != 0)
    {
      if (sp->unk90 & 0x40000000)
      {
        kaze_tmd_tri_flat_lit_f348(sp);
      }
      else
      {
        kaze_model_tri_f3_fixedlight_3f0cc(sp);
      }
    }
  }
  return sp->unkC;
}

/**
 * kaze_model_tri_ft3_fixedlight_3e358  (render, 0x8003e358)
 *
 * TMD submodel rasterizer (used by kaze_draw_model_flatlit): textured flat
 * triangles with a single preloaded RGB (gte_stsxy3_g3/nccs, TPage/CLUT,
 * POLY code 0x7000000).
 */

void kaze_model_tri_ft3_fixedlight_3e358(u32 *ctx)
{
    u32 *p;
    u32 m24;
    u32 m1;
    u32 idx;
    u32 mi;
    u32 c;
    u32 x;
    u32 sc;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x35] = *(u32 *)(ctx[2] + 0xc);
    gte_ldrgb(ctx[0x33]);
    ctx[0x28] = 0;
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
            if ((int)ctx[0x2a] > 0) {
                gte_avsz3();
                ctx[0x1e] = *(u32 *)ctx[0x33] & 0xfe000000;
                if (ctx[0x20] != 0) {
                    x = ctx[0x21];
                    c = *(u16 *)(ctx[0x33] + 8) | x;
                } else {
                    c = *(u32 *)(ctx[0x33] + 8) & 0x60ffff;
                }
                ctx[0x22] = c;
                gte_stotz(ctx + 0x29);
                if ((int)ctx[0x29] > 0) {
                    gte_stsxy3_g3(ctx[3]);
                    if ((ctx[0x1f] & m1) == 0) {
                        gte_ldv0(*(u32 *)ctx[0x35]);
                        gte_nop();
                        gte_nop();
                        gte_nccs();
                    }
                    idx = (int)ctx[0x29] >> ctx[0];
                    ctx[0x29] = idx;
                    *(u32 *)ctx[3] = (*(u32 *)(idx * 4 + ctx[5]) & m24) | 0x7000000;
                    mi = *(u16 *)(ctx[0x33] + 6) & 7;
                    ctx[0x36] = mi;
                    *(u32 *)(ctx[3] + 0xc) =
                        *(u16 *)(ctx[0x33] + 4) | ctx[mi + 0xe] | ctx[mi + 0x16];
                    *(u32 *)(ctx[3] + 0x14) =
                        ctx[0x22] | ctx[ctx[0x36] + 6] | ctx[ctx[0x36] + 0x16];
                    *(u32 *)(ctx[3] + 0x1c) =
                        *(u32 *)(ctx[0x33] + 0xc) | ctx[ctx[0x36] + 0x16];
                    *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                    if ((ctx[0x1f] & m1) == 0) {
                        gte_strgb(ctx + 0x2f);
                        sc = ctx[0x2f] & m24;
                    } else {
                        sc = 0x808080;
                    }
                    ctx[0x2f] = sc;
                    __asm__ volatile("" ::: "memory");
                    *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2f];
                    ctx[3] = ctx[3] + 0x20;
                }
            }
            ctx[0x33] = ctx[0x33] + 0x10;
            ctx[0x35] = ctx[0x35] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

#undef gte_ldrgb
#define gte_ldrgb(x) __asm__ volatile("lwc2 $6, 0(%0)" :: "r"(x) : "memory")

/**
 * kaze_model_tri_gt3_fixedlight_3e638  (render, 0x8003e638)
 *
 * TMD submodel rasterizer: textured gouraud triangles with default 0x808080
 * light (gte_stsxy3_gt3/ncct/strgb3, TPage/CLUT, POLY code 0x9000000).
 */

void kaze_model_tri_gt3_fixedlight_3e638(u32 *arg)
{
    register u32 *p asm("$5");
    register u32 mask asm("$6");
    register u32 tag asm("$7");
    register u32 *rgb0 asm("$8");
    register u32 *rp asm("$3");
    u32 *vp;

    p = arg;
    p[1] = *(u32 *)p[2];
    p[0x33] = *(u32 *)(p[2] + 4);
    p[0x34] = *(u32 *)(p[2] + 8);
    p[0x35] = *(u32 *)(p[2] + 0xc);
    p[0x2f] = 0x808080;
    rp = p + 0x2f;
    __asm__ volatile("" : "=r"(rp) : "0"(rp));
    gte_ldrgb(rp);
    p[0x28] = 0;
    if (p[1] == 0) {
        return;
    }
    tag = 0x1000000;
    mask = 0xffffff;
    rgb0 = rp;
    do {
        vp = (u32 *)p[0x34];
        gte_ldv3(vp[0], vp[1], vp[2]);
        p[0x34] = (u32)(vp + 3);
        gte_rtpt();
        p[0x28] = p[0x28] + 1;
        gte_nclip();
        gte_stopz(p + 0x2a);
        if ((int)p[0x2a] > 0) {
            gte_avsz3();
            p[0x1e] = *(u32 *)(p[0x33] + 8) & 0xfe000000;
            if (p[0x20] != 0) {
                p[0x22] = *(u16 *)(p[0x33] + 4) | p[0x21];
            } else {
                p[0x22] = *(u32 *)(p[0x33] + 4) & 0x60ffff;
            }
            gte_stotz(p + 0x29);
            if ((int)p[0x29] > 0) {
                goto render;
            }
        }
        p[0x35] = p[0x35] + 0xc;
        goto next;
    render:
        gte_stsxy3_gt3(p[3]);
        if ((p[0x1f] & tag) == 0) {
            vp = (u32 *)p[0x35];
            gte_ldv3(vp[0], vp[1], vp[2]);
            p[0x35] = (u32)(vp + 3);
            gte_ncct();
        }
        p[0x29] = (int)p[0x29] >> p[0];
        *(u32 *)p[3] = (*(u32 *)(p[0x29] * 4 + p[5]) & mask) | 0x9000000;
        p[0x36] = *(u16 *)(p[0x33] + 2) & 7;
        *(u32 *)(p[3] + 0xc) =
            *(u16 *)p[0x33] | p[p[0x36] + 0xe] | p[p[0x36] + 0x16];
        *(u32 *)(p[3] + 0x18) = p[0x22] | p[p[0x36] + 6] | p[p[0x36] + 0x16];
        *(u32 *)(p[3] + 0x24) = *(u32 *)(p[0x33] + 8) | p[p[0x36] + 0x16];
        *(u32 *)(p[0x29] * 4 + p[5]) = p[3] & mask;
        if ((p[0x1f] & tag) == 0) {
            gte_strgb3(rgb0, p + 0x30, p + 0x31);
        } else {
            u32 hi = 0x800000;
            u32 flat;
            __asm__ volatile("" : "=r"(hi) : "0"(hi));
            flat = hi | 0x8080;
            p[0x31] = flat;
            p[0x30] = flat;
            p[0x2f] = flat;
            p[0x35] = p[0x35] + 0xc;
        }
        *(u32 *)(p[3] + 4) = p[0x1e] | p[0x20] | (p[0x2f] & mask);
        *(u32 *)(p[3] + 0x10) = p[0x30];
        *(u32 *)(p[3] + 0x1c) = p[0x31];
        p[3] = p[3] + 0x28;
    next:
        p[0x33] = p[0x33] + 0xc;
    } while (p[0x28] < p[1]);
}

/**
 * kaze_model_quad_ft4_fixedlight_3e97c  (render, 0x8003e97c)
 *
 * TMD submodel rasterizer: textured flat quads with preloaded RGB (rtpt+rtps
 * 4th vertex, gte_stsxy3_g3, avsz4_b, TPage/CLUT, texture window fields).
 */

void kaze_model_quad_ft4_fixedlight_3e97c(u32 *ctx)
{
    u32 idx;
    u32 *p;
    u32 m1;
    u32 m24;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x35] = *(u32 *)(ctx[2] + 0xc);
    gte_ldrgb(ctx[0x33]);
    ctx[0x28] = 0;
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
            gte_stsxy3_g3(ctx[3]);
            p = (u32 *)ctx[0x34];
            gte_ldv0(p[0]);
            ctx[0x34] = (u32)(p + 1);
            gte_rtps();
            gte_stsxy(ctx[3] + 0x20);
            ctx[0x1e] = *(u32 *)ctx[0x33] & 0xfe000000;
            gte_avsz4();
            if (ctx[0x20] != 0) {
                ctx[0x22] = *(u16 *)(ctx[0x33] + 8) | ctx[0x21];
            } else {
                ctx[0x22] = *(u32 *)(ctx[0x33] + 8) & 0x60ffff;
            }
            gte_stotz(ctx + 0x29);
            if (0 < (int)ctx[0x29]) {
                gte_nclip();
                gte_stopz(ctx + 0x2b);
                if ((0 < (int)ctx[0x2a]) || ((int)ctx[0x2b] < 1)) {
                    if ((ctx[0x1f] & m1) == 0) {
                        gte_ldv0(*(u32 *)ctx[0x35]);
                        gte_nop();
                        gte_nop();
                        gte_nccs();
                    }
                    ctx[0x29] = (int)ctx[0x29] >> ctx[0];
                    *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0x9000000;
                    idx = *(u16 *)(ctx[0x33] + 6) & 7;
                    ctx[0x36] = idx;
                    *(u32 *)(ctx[3] + 0xc) =
                        *(u16 *)(ctx[0x33] + 4) | ctx[idx + 0xe] | ctx[idx + 0x16];
                    *(u32 *)(ctx[3] + 0x14) =
                        ctx[0x22] | ctx[ctx[0x36] + 6] | ctx[ctx[0x36] + 0x16];
                    *(u32 *)(ctx[3] + 0x1c) =
                        *(u32 *)(ctx[0x33] + 0xc) | ctx[ctx[0x36] + 0x16];
                    *(u32 *)(ctx[3] + 0x24) =
                        *(u32 *)(ctx[0x33] + 0x10) | ctx[ctx[0x36] + 0x16];
                    *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
                    if ((ctx[0x1f] & m1) == 0) {
                        gte_strgb(ctx + 0x2f);
                        ctx[0x2f] = ctx[0x2f] & m24;
                    } else {
                        ctx[0x2f] = 0x808080;
                    }
                    *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | ctx[0x2f];
                    ctx[3] = ctx[3] + 0x28;
                }
            }
            ctx[0x33] = ctx[0x33] + 0x14;
            ctx[0x35] = ctx[0x35] + 4;
        } while (ctx[0x28] < ctx[1]);
    }
}

#undef gte_ldrgb
#define gte_ldrgb(x) __asm__ volatile("lwc2 $6, 0(%0)" :: "r"(x) : "memory")

/**
 * kaze_model_quad_gt4_fixedlight_3ecdc  (render, 0x8003ecdc)
 *
 * TMD submodel rasterizer: textured gouraud quads with default 0x808080
 * light (gte_stsxy3_gt3 quad, rtps 4th vertex, avsz4_b, TPage/CLUT).
 */

void kaze_model_quad_gt4_fixedlight_3ecdc(u32 *ctx)
{
    register u32 *p asm("$2");
    register u32 *rgbp asm("$4");
    u32 m1;
    u32 m24;
    u32 flat;

    ctx[1] = *(u32 *)ctx[2];
    ctx[0x33] = *(u32 *)(ctx[2] + 4);
    ctx[0x34] = *(u32 *)(ctx[2] + 8);
    ctx[0x35] = *(u32 *)(ctx[2] + 0xc);
    ctx[0x2f] = 0x808080;
    gte_ldrgb(ctx + 0x2f);
    ctx[0x28] = 0;
    if (ctx[1] == 0) {
        return;
    }
    m1 = 0x1000000;
    m24 = 0xffffff;
    flat = 0x808080;
    do {
        p = (u32 *)ctx[0x34];
        gte_ldv3(p[0], p[1], p[2]);
        ctx[0x34] = (u32)(p + 3);
        gte_rtpt();
        ctx[0x28] = ctx[0x28] + 1;
        gte_nclip();
        gte_stopz(ctx + 0x2a);
        gte_stsxy3_gt3(ctx[3]);
        p = (u32 *)ctx[0x34];
        gte_ldv0(p[0]);
        ctx[0x34] = (u32)(p + 1);
        gte_rtps();
        gte_stsxy(ctx[3] + 0x2c);
        ctx[0x1e] = *(u32 *)(ctx[0x33] + 0xc) & 0xfe000000;
        gte_avsz4();
        if (ctx[0x20] != 0) {
            ctx[0x22] = *(u16 *)(ctx[0x33] + 4) | ctx[0x21];
        } else {
            ctx[0x22] = *(u32 *)(ctx[0x33] + 4) & 0x60ffff;
        }
        gte_stotz(ctx + 0x29);
        if (0 < (int)ctx[0x29]) {
            gte_nclip();
            gte_stopz(ctx + 0x2b);
            if (((int)ctx[0x2a] < 1) && (0 < (int)ctx[0x2b])) {
                goto next;
            }
            if ((ctx[0x1f] & m1) == 0) {
                p = (u32 *)ctx[0x35];
                gte_ldv3(p[0], p[1], p[2]);
                gte_nop();
                gte_nop();
                gte_ncct();
            }
            ctx[0x29] = (int)ctx[0x29] >> ctx[0];
            *(u32 *)ctx[3] = (*(u32 *)(ctx[0x29] * 4 + ctx[5]) & m24) | 0xc000000;
            ctx[0x36] = *(u16 *)(ctx[0x33] + 2) & 7;
            *(u32 *)(ctx[3] + 0xc) = *(u16 *)ctx[0x33] | ctx[ctx[0x36] + 0xe] | ctx[ctx[0x36] + 0x16];
            *(u32 *)(ctx[3] + 0x18) = ctx[0x22] | ctx[ctx[0x36] + 6] | ctx[ctx[0x36] + 0x16];
            *(u32 *)(ctx[3] + 0x24) = *(u32 *)(ctx[0x33] + 8) | ctx[ctx[0x36] + 0x16];
            *(u32 *)(ctx[3] + 0x30) = *(u32 *)(ctx[0x33] + 0xc) | ctx[ctx[0x36] + 0x16];
            *(u32 *)(ctx[0x29] * 4 + ctx[5]) = ctx[3] & m24;
            if ((ctx[0x1f] & m1) == 0) {
                rgbp = ctx + 0x2f;
                __asm__ volatile("" : "=r"(rgbp) : "0"(rgbp));
                gte_strgb3(rgbp, ctx + 0x30, ctx + 0x31);
                gte_ldv0(*(u32 *)(ctx[0x35] + 0xc));
                gte_nop();
                gte_nop();
                gte_nccs();
                gte_strgb(ctx + 0x32);
            } else {
                ctx[0x32] = flat;
                ctx[0x31] = flat;
                ctx[0x30] = flat;
                ctx[0x2f] = flat;
            }
            *(u32 *)(ctx[3] + 4) = ctx[0x1e] | ctx[0x20] | (ctx[0x2f] & m24);
            *(u32 *)(ctx[3] + 0x10) = ctx[0x30];
            *(u32 *)(ctx[3] + 0x1c) = ctx[0x31];
            *(u32 *)(ctx[3] + 0x28) = ctx[0x32];
            ctx[3] = ctx[3] + 0x34;
        }
    next:
        ctx[0x33] = ctx[0x33] + 0x10;
        ctx[0x35] = ctx[0x35] + 0x10;
    } while (ctx[0x28] < ctx[1]);
}
