#include "types.h"
#include "kaze_syms.h"
extern int kaze_live_stage_level;
extern int kaze_draw_model_flatlit_3dd2c(int, int, int, int, int);
extern int kaze_tmd_clip_sort_dispatch_2070(int, int, int, int, int, void *);
extern int kaze_tmd_sort_const_color_trans_8274(int, int, int, int, int, int, int, int);
typedef struct 
{
  short vx;
  short vy;
  short vz;
  short pad;
} SVECTOR;

extern int SetPolyFT4(int);
extern int SetShadeTex(int, int);
extern int GetTPage(int, int, int, int);
extern int GetClut(int, int);
extern int rcos(int);
extern int rsin(int);
#define gte_SetRotMatrix(r0)                                                   \
    __asm__ volatile("lw $12, 0(%0)\n\t"                                       \
                     "lw $13, 4(%0)\n\t"                                       \
                     "ctc2 $12, $0\n\t"                                        \
                     "ctc2 $13, $1\n\t"                                        \
                     "lw $12, 8(%0)\n\t"                                       \
                     "lw $13, 12(%0)\n\t"                                      \
                     "lw $14, 16(%0)\n\t"                                      \
                     "ctc2 $12, $2\n\t"                                        \
                     "ctc2 $13, $3\n\t"                                        \
                     "ctc2 $14, $4"                                            \
                     :: "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0)                                                 \
    __asm__ volatile("lw $12, 20(%0)\n\t"                                      \
                     "lw $13, 24(%0)\n\t"                                      \
                     "ctc2 $12, $5\n\t"                                        \
                     "lw $14, 28(%0)\n\t"                                      \
                     "ctc2 $13, $6\n\t"                                        \
                     "ctc2 $14, $7"                                            \
                     :: "r"(r0) : "$12", "$13", "$14")
#define gte_ldclmv(p)                                                          \
    __asm__ volatile("lhu $12, 0(%0)\n\t"                                      \
                     "lhu $13, 6(%0)\n\t"                                      \
                     "lhu $14, 12(%0)\n\t"                                     \
                     "mtc2 $12, $9\n\t"                                        \
                     "mtc2 $13, $10\n\t"                                       \
                     "mtc2 $14, $11"                                           \
                     :: "r"(p) : "$12", "$13", "$14")
#define gte_rtir()                                                             \
    __asm__ volatile("nop\n\t"                                                 \
                     "nop\n\t"                                                 \
                     "cop2 0x49E012")
#define gte_stclmv(p)                                                          \
    __asm__ volatile("mfc2 $12, $9\n\t"                                        \
                     "mfc2 $13, $10\n\t"                                       \
                     "mfc2 $14, $11\n\t"                                       \
                     "sh $12, 0(%0)\n\t"                                       \
                     "sh $13, 6(%0)\n\t"                                       \
                     "sh $14, 12(%0)"                                          \
                     :: "r"(p) : "$12", "$13", "$14", "memory")
#define gte_ldv0(p)                                                            \
    __asm__ volatile("lwc2 $0, 0(%0)\n\t"                                      \
                     "lwc2 $1, 4(%0)"                                          \
                     :: "r"(p))
#define gte_rt()                                                               \
    __asm__ volatile("nop\n\t"                                                 \
                     "nop\n\t"                                                 \
                     "cop2 0x480012")
#define gte_stlvnl(p)                                                          \
    __asm__ volatile("swc2 $25, 0(%0)\n\t"                                     \
                     "swc2 $26, 4(%0)\n\t"                                     \
                     "swc2 $27, 8(%0)"                                         \
                     :: "r"(p) : "memory")

/**
 * kaze_sky_cloud_quads_render  (render, 0x8002e670)
 *
 * Renders the 0x40-entry sky cloud layer at +0x506d4 as shade-tex POLY_FT4
 * quads: CLUT row from stage level kaze_live_stage_level, billboard tilt
 * from camera pitch +0x4a via +0x508d4, altitude -0x1000-y, via
 * kaze_quad_subdivide_render.
 */

void kaze_sky_cloud_quads_render(int ot)
{
    extern int RotMatrix(char *, int);
    extern int ReadRotMatrix(int *);
    extern int PopMatrix(void);
    extern int PushMatrix(void);
    extern char *kaze_state_base_ptr;
    int c;
    u16 *q;
    int *r0;
    char *k;
    int i;
    int h;
    int pos;
    int b6;
    int *r00;
    int n;

    SetPolyFT4(0x1f80007c);
    SetShadeTex(0x1f80007c, 1);
    c = 0x80;
    *(u8 *)0x1f800080 = c;
    *(u8 *)0x1f800081 = c;
    *(u8 *)0x1f800082 = c;
    *(u16 *)0x1f800092 = GetTPage(0, 0, 0x200, 0x100);
    *(u16 *)0x1f80008a = GetClut(kaze_live_stage_level * 0x10 + 0x170, 0x1f9);
    *(int *)0x1f800040 = *(int *)(kaze_state_base_ptr + 0x9c);
    q = (u16 *)0x1f8000c8;
    *(int *)0x1f800044 = ot;
    i = rcos(*(s16 *)(kaze_state_base_ptr + 0x4a)) * -400;
    k = kaze_state_base_ptr;
    r0 = (int *)0x1f800000;
    if (i < 0) {
        i = i + 0xfff;
    }
    h = *(s16 *)(k + 0x4a);
    *(s16 *)(k + 0x508d4) = i >> 12;
    *(s16 *)(k + 0x508d6) = 0;
    i = rsin(h) * -400;
    k = kaze_state_base_ptr;
    if (i < 0) {
        i = i + 0xfff;
    }
    *(s16 *)(k + 0x508d8) = i >> 12;
    *(u16 *)0x1f800060 = 0;
    pos = *(int *)(k + 0x6c);
    *(u8 *)0x1f8000e6 = 0xbf;
    *(u8 *)0x1f8000d6 = 0xbf;
    *(u8 *)0x1f8000e7 = 0x3f;
    *(u8 *)0x1f8000df = 0x3f;
    *(u16 *)0x1f800064 = 0;
    *(u8 *)0x1f8000de = c;
    *(u8 *)0x1f8000ce = c;
    *(u8 *)0x1f8000d7 = 0;
    *(u8 *)0x1f8000cf = 0;
    *(u16 *)0x1f8000e2 = 0;
    *(u16 *)0x1f8000da = 0;
    *(u16 *)0x1f8000d2 = 0;
    *(u16 *)0x1f8000ca = 0;
    *(s16 *)0x1f8000d8 = -0x1000;
    *(s16 *)q = -0x1000;
    *(s16 *)0x1f8000d4 = -0x1000;
    *(s16 *)0x1f8000cc = -0x1000;
    *(u16 *)0x1f8000e0 = 0x1000;
    *(u16 *)0x1f8000d0 = 0x1000;
    *(u16 *)0x1f8000e4 = 0x1000;
    *(u16 *)0x1f8000dc = 0x1000;
    *(s16 *)0x1f800062 = -pos - 0x1000;
    PushMatrix();
    ReadRotMatrix((int *)0x1f800020);
    RotMatrix(kaze_state_base_ptr + 0x508d4, 0x1f800000);
    gte_SetRotMatrix(0x1f800020);
    gte_ldclmv(r0);
    gte_rtir();
    gte_stclmv(r0);
    gte_ldclmv((int *)0x1f800002);
    gte_rtir();
    gte_stclmv((int *)0x1f800002);
    gte_ldclmv((int *)0x1f800004);
    gte_rtir();
    gte_stclmv((int *)0x1f800004);
    PopMatrix();
    gte_ldv0(0x1f800060);
    gte_rt();
    gte_stlvnl(0x1f800014);
    PushMatrix();
    gte_SetRotMatrix(r0);
    gte_SetTransMatrix(r0);
    b6 = 0x506d4;
    r00 = (int *)0x1f800014;
    *(int *)0x1f800068 = 0;
    do {
        ReadRotMatrix(r0);
        gte_ldv0(r0[0x1a] * 8 + b6 + kaze_state_base_ptr);
        gte_rt();
        gte_stlvnl(r00);
        PushMatrix();
        gte_SetRotMatrix(r0);
        gte_SetTransMatrix(r0);
        kaze_quad_subdivide_render(q, r0, 0);
        PopMatrix();
        n = r0[0x1a] + 1;
        r0[0x1a] = n;
    } while (n < 0x40);
    PopMatrix();
    *(int *)(kaze_state_base_ptr + 0x9c) = r0[0x10];
}

extern void SetDrawArea(void *, void *);
extern void AddPrim(void *, void *);

/**
 * kaze_draw_area_anim  (render, 0x8002eb40)
 *
 * Animates the draw-area RECT at +0x48204..+0x4820a.
 */

void kaze_draw_area_anim(void *ot_arg)
{
    extern char *kaze_state_base_ptr;

  register char *base __asm__("$5");

  register void *ot __asm__("$17");
  short old_width;
  base = kaze_state_base_ptr;
  old_width = *((short *) (base + 0x48208));
  ot = ot_arg;
  if ((old_width < 0x140) || ((*((short *) (base + 0x4820A))) < 0xF0))
  {
    short new_width;
    short new_height;
    int origin_x;

    register int origin_y __asm__("$2");

    register char *base2 __asm__("$4");
    *((short *) (base + 0x48208)) = old_width + ((*((int *) (base + 0xA4))) * 4);
    {
      register int delta2 __asm__("$4") = *((int *) (base + 0xA4));
      int inc3 = delta2 * 3;
      short old_height = *((unsigned short *) (base + 0x4820A));
asm volatile("" : : : "memory");
      base2 = base;
      old_width = old_height + inc3;
      __asm__("" : "=r"(base) : "0"(base));
      *((short *) (base + 0x4820A)) = old_width;
    }
    {
      short w = *((short *) (base2 + 0x48208));
      short v;
      if (w < 0)
      {
        goto width_neg;
      }
      v = w;
      if (w <= 0x140)
      {
        goto width_done;
      }
      v = 0x140;
      goto width_done;
      width_neg:
      v = 0;

      width_done:
      new_width = v;

    }
    base = kaze_state_base_ptr;
    *((short *) (base2 + 0x48208)) = new_width;
    {
      short h = *((short *) (base + 0x4820A));
      short v;
      if (h < 0)
      {
        goto height_neg;
      }
      v = h;
      if (h <= 0xF0)
      {
        goto height_done;
      }
      v = 0xF0;
      goto height_done;
      height_neg:
      v = 0;

      height_done:
      new_height = v;

    }
    base2 = kaze_state_base_ptr;
    *((short *) (base + 0x4820A)) = new_height;
    origin_x = (0x140 - (*((short *) (base2 + 0x48208)))) / 2;
    *((short *) (base2 + 0x48204)) = origin_x;
asm volatile("" : : : "memory");
    origin_y = (0xF0 - (*((short *) (base2 + 0x4820A)))) / 2;
    if ((*((int *) (base2 + 0x98))) != 0)
    {
      origin_y += 0x100;
    }
    {

      register int base_const __asm__("$16");
      base_const = 0x4820C;
      {

        register char *base3 __asm__("$3");

        register int idx __asm__("$2");
        base3 = kaze_state_base_ptr;
        *((short *) (base2 + 0x48206)) = origin_y;
        idx = *((int *) (base3 + 0x98));
        SetDrawArea(base3 + ((idx * 0xC) + base_const), base3 + 0x48204);
      }
      {

        register char *base4 __asm__("$3");

        register int idx __asm__("$2");
        base4 = kaze_state_base_ptr;
        idx = *((int *) (base4 + 0x98));
        AddPrim(ot, base4 + ((idx * 0xC) + base_const));
      }
    }
  }
}

#include "gte.h"
void kaze_terrain_tile_window_cull();
extern void RotTrans(s16 *, int *, int *);
void kaze_terrain_tile_render();

/**
 * kaze_terrain_render_setup  (render, 0x8002ed44)
 *
 * Terrain render wrapper: builds camera-local translation from balloon
 * position mod 0xed8 (root+0x68/+0x6c/+0x70) via RotTrans, sets GTE
 * matrices, runs terrain tile renderer kaze_terrain_tile_render and water
 * pass kaze_env_water_render_pass.
 */

void kaze_terrain_render_setup(int a, int b)
{
    extern void PopMatrix(void);
    extern void ReadRotMatrix(int *);
    extern void PushMatrix(void);
    extern int kaze_state_base_ptr;
    s16 sv[4];
    int m[8];
    int tv[2];
    int base;

    kaze_terrain_tile_window_cull();
    PushMatrix();
    ReadRotMatrix(m);
    base = kaze_state_base_ptr;
    sv[0] = *(int *)(base + 0x68) % 0xed8;
    sv[1] = -*(int *)(base + 0x6c);
    sv[2] = -*(int *)(base + 0x70) % 0xed8;
    RotTrans(sv, m + 5, tv);
    gte_ld_rotm(m);
    gte_ld_transv(m);
    kaze_terrain_tile_render(a);
    kaze_env_water_render_pass(a, b);
    PopMatrix();
}

/**
 * kaze_terrain_tile_window_cull  (render, 0x8002eea0)
 *
 * Rebuilds the 8x8 visible-tile window at +0xec6e0 from the 0x80-wide tile
 * map at +0x5b6e0 around the balloon tile (pos/0xed8), zeroing tiles outside
 * the camera-yaw FOV wedge or masked out in kaze_terrain_tile_cull_mask.
 */

void kaze_terrain_tile_window_cull(void)
{
    extern u16 kaze_terrain_tile_cull_mask;
    extern char *kaze_state_base_ptr;
  char *sp = (char *) 0x1f800000;
  int angle_offset;
  int tile_dx;
  int new_var;
  *((char **) (sp + 0x30)) = kaze_state_base_ptr + 0x48;
  {
    int base_angle = rsin((int) (*((short *) (kaze_state_base_ptr + 0x48))));
    if (base_angle < 0)
    {
      base_angle = base_angle + 0x1f;
    }
    base_angle = base_angle >> 5;
    *((int *) (sp + 0x2c)) = base_angle;
    angle_offset = base_angle + (-0x200);
    *((int *) (sp + 0x0c)) = rsin(((int) (*((short *) ((*((char **) (sp + 0x30))) + 2)))) + angle_offset);
  }
  {
    int adjusted_angle = *((int *) (sp + 0x2c)) - 0x200;
    *((int *) (sp + 0x10)) = rsin(((int) (*((short *) ((*((char **) (sp + 0x30))) + 2)))) - adjusted_angle);
  }
  {
    int adjusted_angle = *((int *) (sp + 0x2c)) - 0x200;
    *((int *) (sp + 0x14)) = rcos(((int) (*((short *) ((*((char **) (sp + 0x30))) + 2)))) + adjusted_angle);
  }
  new_var = (*((int *) (sp + 0x2c))) + (-0x200);
  *((int *) (sp + 0x18)) = rcos(((int) (*((short *) ((*((char **) (sp + 0x30))) + 2)))) - new_var);
  *((int *) (sp + 0x24)) = (*((int *) (kaze_state_base_ptr + 0x68))) / 0xed8;
  *((int *) (sp + 0x28)) = (*((int *) (kaze_state_base_ptr + 0x70))) / 0xed8;
  {
    int sin_val = rsin((int) (*((short *) ((*((char **) (sp + 0x30))) + 2))));
    if (sin_val < 0)
    {
      sin_val = sin_val + 0x3ff;
    }
    *((int *) (sp + 0x1c)) = (*((int *) (sp + 0x24))) - (sin_val >> 10);
  }
  {
    int cos_val = rcos((int) (*((short *) ((*((char **) (sp + 0x30))) + 2))));
    if (cos_val < 0)
    {
      cos_val = cos_val + 0x3ff;
    }
    *((int *) (sp + 0x20)) = (*((int *) (sp + 0x28))) - (cos_val >> 10);
    *((u16 **) (sp + 0x08)) = &kaze_terrain_tile_cull_mask;
    *((int *) (sp + 0x3c)) = -8;
  }
  *((int **) (sp + 0x00)) = ((int *) (kaze_state_base_ptr + 0x5b6e0)) + (((*((int *) (sp + 0x28))) * 0x80) + (*((int *) (sp + 0x24))));
  *((int **) (sp + 0x04)) = (int *) (kaze_state_base_ptr + 0xec6e0);
  do
  {
    *((int *) (sp + 0x38)) = -8;
    do
    {
      int tile_dz;
      tile_dx = ((*((int *) (sp + 0x38))) + (*((int *) (sp + 0x24)))) - (*((int *) (sp + 0x1c)));
      tile_dz = ((*((int *) (sp + 0x3c))) + (*((int *) (sp + 0x28)))) - (*((int *) (sp + 0x20)));
      if (((*((int *) (sp + 0x0c)) * tile_dx) + (*((int *) (sp + 0x14)) * tile_dz)) >= 0)
      {
        if (((*((int *) (sp + 0x10)) * tile_dx) + (*((int *) (sp + 0x18)) * tile_dz)) >= 0 && **((u16 **) (sp + 0x08)) != 0)
        {
          *(*((int **) (sp + 0x04))) = *((*((int **) (sp + 0x00))) + (*((int *) (sp + 0x3c)) * 0x80) + *((int *) (sp + 0x38)));
        }
        else
        {
          *(*((int **) (sp + 0x04))) = 0;
        }
      }
      else
      {
        *(*((int **) (sp + 0x04))) = 0;
      }
      *((int *) (sp + 0x38)) = (*((int *) (sp + 0x38))) + 1;
      *((int **) (sp + 0x04)) = (*((int **) (sp + 0x04))) + 1;
      *((u16 **) (sp + 0x08)) = (*((u16 **) (sp + 0x08))) + 1;
    }
    while ((*((int *) (sp + 0x38))) < 8);
    *((int *) (sp + 0x3c)) = (*((int *) (sp + 0x3c))) + 1;
  }
  while ((*((int *) (sp + 0x3c))) < 8);
  return;
}

extern int kaze_tile_edge_factor(int);

/**
 * kaze_terrain_tile_render  (render, 0x8002f1c8)
 *
 * Terrain/map tile renderer: iterates an 8x8 tile grid, resolves tile models
 * from the +0xecb28 table, draws with the +0x509dc GPU code.
 */

void kaze_terrain_tile_render(int ot)
{
    extern int RotMatrix(void *r, void *m);
    extern int PopMatrix(void);
    extern int PushMatrix(void);
    extern char kaze_terrain_tile_cull_mask;
    extern char *kaze_state_base_ptr;
  SVECTOR *r = (SVECTOR *) 0x1f800000;
  int i;
  int j;
  int lod;
  int mask;
  int gpu;
  int new_var;
  int cursor;
  int model;
  int bright;
  void *clip_scratch = (void *) 0x1f800158;
  gpu = *((int *) (kaze_state_base_ptr + 0x509dc));
  *((int *) 0x1f800128) = *((int *) (kaze_state_base_ptr + 0x9c));
  *((int *) 0x1f80012c) = (int) (kaze_state_base_ptr + 0xec6e0);
  *((int *) 0x1f800134) = (int) (&kaze_terrain_tile_cull_mask);
  *((int *) 0x1f800150) = ((*((int *) (kaze_state_base_ptr + 0x68))) % 0xed8) + (-0x76c);
  *((int *) 0x1f800154) = ((*((int *) (kaze_state_base_ptr + 0x70))) % 0xed8) + (-0x76c);
  PushMatrix();
  r->vz = 0;
  r->vx = 0;
  *((int *) (&r[0x28].vz)) = 0;
  do
  {
    r->vy = ((*((int *) (&r[0x28].vz))) * 0x400) + 0x800;
    RotMatrix(r, (void *) (((int) (&r[1].vx)) + ((*((int *) (&r[0x28].vz))) * 0x20)));
    {
      void *_gp = (void *) ((void *) (kaze_state_base_ptr + 0x481b8));
      asm volatile("lw $12, 0(%0)\n\tlw $13, 4(%0)\n\tctc2 $12, $0\n\tctc2 $13, $1\n\tlw $12, 8(%0)\n\tlw $13, 12(%0)\n\tlw $14, 16(%0)\n\tctc2 $12, $2\n\tctc2 $13, $3\n\tctc2 $14, $4" : : "r"(_gp) : "$12", "$13", "$14");
    }
    {
      void *_gp = (void *) ((void *) (((int) (&r[1].vx)) + ((*((int *) (&r[0x28].vz))) * 0x20)));
      asm volatile("lhu $12, 0(%0)\n\tlhu $13, 6(%0)\n\tlhu $14, 12(%0)\n\tmtc2 $12, $9\n\tmtc2 $13, $10\n\tmtc2 $14, $11" : : "r"(_gp) : "$12", "$13", "$14");
    }
    asm volatile("nop\n\tnop\n\tcop2 0x49E012");
    {
      void *_gp = (void *) ((void *) (((int) (&r[0x11].vx)) + ((*((int *) (&r[0x28].vz))) * 0x20)));
      asm volatile("mfc2 $12, $9\n\tmfc2 $13, $10\n\tmfc2 $14, $11\n\tsh $12, 0(%0)\n\tsh $13, 6(%0)\n\tsh $14, 12(%0)" : : "r"(_gp) : "$12", "$13", "$14", "memory");
    }
    {
      void *_gp = (void *) (&((SVECTOR *) (((int) (&r[1].vx)) + ((*((int *) (&r[0x28].vz))) * 0x20)))->vy);
      asm volatile("lhu $12, 0(%0)\n\tlhu $13, 6(%0)\n\tlhu $14, 12(%0)\n\tmtc2 $12, $9\n\tmtc2 $13, $10\n\tmtc2 $14, $11" : : "r"(_gp) : "$12", "$13", "$14");
    }
    asm volatile("nop\n\tnop\n\tcop2 0x49E012");
    {
      void *_gp = (void *) (&((SVECTOR *) (((int) (&r[0x11].vx)) + ((*((int *) (&r[0x28].vz))) * 0x20)))->vy);
      asm volatile("mfc2 $12, $9\n\tmfc2 $13, $10\n\tmfc2 $14, $11\n\tsh $12, 0(%0)\n\tsh $13, 6(%0)\n\tsh $14, 12(%0)" : : "r"(_gp) : "$12", "$13", "$14", "memory");
    }
    {
      void *_gp = (void *) (&((SVECTOR *) (((int) (&r[1].vx)) + ((*((int *) (&r[0x28].vz))) * 0x20)))->vz);
      asm volatile("lhu $12, 0(%0)\n\tlhu $13, 6(%0)\n\tlhu $14, 12(%0)\n\tmtc2 $12, $9\n\tmtc2 $13, $10\n\tmtc2 $14, $11" : : "r"(_gp) : "$12", "$13", "$14");
    }
    asm volatile("nop\n\tnop\n\tcop2 0x49E012");
    {
      void *_gp = (void *) (&((SVECTOR *) (((int) (&r[0x11].vx)) + ((*((int *) (&r[0x28].vz))) * 0x20)))->vz);
      asm volatile("mfc2 $12, $9\n\tmfc2 $13, $10\n\tmfc2 $14, $11\n\tsh $12, 0(%0)\n\tsh $13, 6(%0)\n\tsh $14, 12(%0)" : : "r"(_gp) : "$12", "$13", "$14", "memory");
    }
    {
      void *_gp = (void *) ((void *) (kaze_state_base_ptr + 0x28));
      asm volatile("lw $12, 0(%0)\n\tlw $13, 4(%0)\n\tctc2 $12, $0\n\tctc2 $13, $1\n\tlw $12, 8(%0)\n\tlw $13, 12(%0)\n\tlw $14, 16(%0)\n\tctc2 $12, $2\n\tctc2 $13, $3\n\tctc2 $14, $4" : : "r"(_gp) : "$12", "$13", "$14");
    }
    {
      void *_gp = (void *) ((void *) (((int) (&r[1].vx)) + ((*((int *) (&r[0x28].vz))) * 0x20)));
      asm volatile("lhu $12, 0(%0)\n\tlhu $13, 6(%0)\n\tlhu $14, 12(%0)\n\tmtc2 $12, $9\n\tmtc2 $13, $10\n\tmtc2 $14, $11" : : "r"(_gp) : "$12", "$13", "$14");
    }
    asm volatile("nop\n\tnop\n\tcop2 0x49E012");
    {
      void *_gp = (void *) ((void *) (((int) (&r[1].vx)) + ((*((int *) (&r[0x28].vz))) * 0x20)));
      asm volatile("mfc2 $12, $9\n\tmfc2 $13, $10\n\tmfc2 $14, $11\n\tsh $12, 0(%0)\n\tsh $13, 6(%0)\n\tsh $14, 12(%0)" : : "r"(_gp) : "$12", "$13", "$14", "memory");
    }
    {
      void *_gp = (void *) (&r[((*((int *) (&r[0x28].vz))) * 4) + 1].vy);
      asm volatile("lhu $12, 0(%0)\n\tlhu $13, 6(%0)\n\tlhu $14, 12(%0)\n\tmtc2 $12, $9\n\tmtc2 $13, $10\n\tmtc2 $14, $11" : : "r"(_gp) : "$12", "$13", "$14");
    }
    asm volatile("nop\n\tnop\n\tcop2 0x49E012");
    {
      void *_gp = (void *) (&r[((*((int *) (&r[0x28].vz))) * 4) + 1].vy);
      asm volatile("mfc2 $12, $9\n\tmfc2 $13, $10\n\tmfc2 $14, $11\n\tsh $12, 0(%0)\n\tsh $13, 6(%0)\n\tsh $14, 12(%0)" : : "r"(_gp) : "$12", "$13", "$14", "memory");
    }
    {
      void *_gp = (void *) (&r[((*((int *) (&r[0x28].vz))) * 4) + 1].vz);
      asm volatile("lhu $12, 0(%0)\n\tlhu $13, 6(%0)\n\tlhu $14, 12(%0)\n\tmtc2 $12, $9\n\tmtc2 $13, $10\n\tmtc2 $14, $11" : : "r"(_gp) : "$12", "$13", "$14");
    }
    asm volatile("nop\n\tnop\n\tcop2 0x49E012");
    {
      void *_gp = (void *) (&r[((*((int *) (&r[0x28].vz))) * 4) + 1].vz);
      asm volatile("mfc2 $12, $9\n\tmfc2 $13, $10\n\tmfc2 $14, $11\n\tsh $12, 0(%0)\n\tsh $13, 6(%0)\n\tsh $14, 12(%0)" : : "r"(_gp) : "$12", "$13", "$14", "memory");
    }
    *((int *) (&r[0x28].vz)) = (*((int *) (&r[0x28].vz))) + 1;
  }
  while ((*((int *) (&r[0x28].vz))) < 4);
  PopMatrix();
  mask = 0x1ff00;
  lod = 2;
  r->vz = -0x6f54;
  *((int *) (&r[0x28].vz)) = -8;
  do
  {
    int lj;
    r->vx = 0x6f54;
    *((int *) (&r[0x28].vx)) = -8;
    do
    {
      unsigned int tile = *(*((unsigned int **) (&r[0x25].vz)));
      int li;
      *((int *) (&r[0x27].vx)) = tile;
      if ((tile != 0) && ((tile & mask) != 0))
      {
        unsigned int w;
        PushMatrix();
        r->vy = (((unsigned int) (*((int *) (&r[0x27].vx)))) >> 0x1c) * (-0x76c);
        asm volatile("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(r));
        *((int *) (&r[0x29].vx)) = ((*((unsigned int *) (&r[0x27].vx))) >> 0x18) & 3;
        asm volatile("cop2 0x480012");
        {
          int ti = (unsigned short) (*(*((unsigned short **) (&r[0x26].vz))));
          int tj = *((volatile int *) (&r[0x29].vx));
          int c0;
          int c1;
          int c2;
          int c3;
          *((int *) (&r[0x27].vz)) = ti;
          c0 = *((int *) (&r[(tj * 4) + 1].vx));
          c1 = *((int *) (&r[(tj * 4) + 1].vz));
          c2 = *((int *) (&r[(tj * 4) + 2].vx));
          c3 = *((int *) (&r[(tj * 4) + 2].vz));
          *((int *) (&r[0x21].vx)) = c0;
          *((int *) (&r[0x21].vz)) = c1;
          *((int *) (&r[0x22].vx)) = c2;
          *((int *) (&r[0x22].vz)) = c3;
          c0 = *((int *) (&r[(tj * 4) + 3].vx));
          c1 = *((int *) (&r[(tj * 4) + 3].vz));
          c2 = *((int *) (&r[(tj * 4) + 4].vx));
          c3 = *((int *) (&r[(tj * 4) + 4].vz));
          *((int *) (&r[0x23].vx)) = c0;
          *((int *) (&r[0x23].vz)) = c1;
          *((int *) (&r[0x24].vx)) = c2;
          *((int *) (&r[0x24].vz)) = c3;
        }
        asm volatile("" : : : "memory");
        {
          void *ctbase = (void *) (r + 0x21);
          void *swbase = (void *) (&r[0x23].vz);
          asm volatile("swc2 $25, 0(%0)\n\tswc2 $26, 4(%0)\n\tswc2 $27, 8(%0)\n\tlw $12, 0(%1)\n\tlw $13, 4(%1)\n\tctc2 $12, $0\n\tctc2 $13, $1\n\tlw $12, 8(%1)\n\tlw $13, 12(%1)\n\tlw $14, 16(%1)\n\tctc2 $12, $2\n\tctc2 $13, $3\n\tctc2 $14, $4" : : "r"(swbase), "r"(ctbase) : "$12", "$13", "$14", "memory");
        }
        {
          void *_gp = (void *) (r + 0x21);
          asm volatile("lw $12, 20(%0)\n\tlw $13, 24(%0)\n\tctc2 $12, $5\n\tlw $14, 28(%0)\n\tctc2 $13, $6\n\tctc2 $14, $7" : : "r"(_gp) : "$12", "$13", "$14");
        }
        {
          void *_gp = (void *) (r + (((*((int *) (&r[0x29].vx))) * 4) + 0x11));
          asm volatile("lw $12, 0(%0)\n\tlw $13, 4(%0)\n\tctc2 $12, $8\n\tctc2 $13, $9\n\tlw $12, 8(%0)\n\tlw $13, 12(%0)\n\tlw $14, 16(%0)\n\tctc2 $12, $10\n\tctc2 $13, $11\n\tctc2 $14, $12" : : "r"(_gp) : "$12", "$13", "$14");
        }
        w = *((unsigned int *) (&r[0x27].vx));
        if ((w & 0xc000000) != 0)
        {
          int jj = (*((int *) ((new_var = (((int) kaze_state_base_ptr) + ((w & 0x1f00) >> 1)) + 0xe1e60) + ((*((int *) (kaze_state_base_ptr + 0xecaf8))) * 4)))) - 1;
          *((int *) (&r[0x29].vx)) = jj;
          *((int *) (&r[0x26].vx)) = *((int *) (((jj * 4) + ((int) kaze_state_base_ptr)) + 0xecb28));
        }
        else
        {
          int raw = (w & mask) >> 8;
          int *base = (int *) kaze_state_base_ptr;
          int jj;
          asm volatile("" : : "r"(base));
          jj = raw - 1;
          *((int *) (&r[0x29].vx)) = jj;
          *((int *) (&r[0x26].vx)) = base[(jj * 4 + 0xecb28) / 4];
        }
        if (((*((unsigned int *) (&r[0x27].vz))) & 0x88) != 0)
        {
          int ci = kaze_tile_edge_factor((*((unsigned int *) (&r[0x27].vz))) & 0xffff);
          *((int *) (&r[0x29].vz)) = ci;
          if (kaze_live_stage_level == lod)
          {
            int cj = ci << 1;
            if (ci >= 0x80)
            {
              cj = (0x100 - ci) * 2;
              *((int *) (&r[0x29].vz)) = cj;
              if (cj >= 0)
              {
                ci = 0xff;
                if (cj < 0x100)
                {
                  ci = cj;
                }
              }
              else
              {
                ci = 0;
              }
              *((int *) (&r[0x29].vz)) = ci;
              *((int *) (&r[0x27].vz)) = 0;
            }
            else
            {
              *((int *) (&r[0x29].vz)) = cj;
              if (cj >= 0)
              {
                ci = 0xff;
                if (cj < 0x100)
                {
                  ci = cj;
                }
              }
              else
              {
                ci = 0;
              }
              *((int *) (&r[0x29].vz)) = ci;
            }
          }
          *((int *) (&r[0x25].vx)) = kaze_tmd_sort_const_color_trans_8274(*((int *) (&r[0x26].vx)), gpu, ot, *((int *) (&r[0x25].vx)), lod, *((int *) (&r[0x29].vz)), *((int *) (&r[0x29].vz)), *((int *) (&r[0x29].vz)));
        }
        w = *((unsigned int *) (&r[0x27].vz));
        if ((w & 0x8000) != 0)
        {
          *((int *) (&r[0x25].vx)) = kaze_tmd_clip_sort_dispatch_2070(*((int *) (&r[0x26].vx)), 0, ot, *((int *) (&r[0x25].vx)), lod, clip_scratch);
        }
        else
        {
          if (w != 0)
          {
            *((int *) (&r[0x25].vx)) = kaze_draw_model_flatlit_3dd2c(*((int *) (&r[0x26].vx)), 0, ot, *((int *) (&r[0x25].vx)), lod);
          }
        }
        PopMatrix();
      }
      li = *((int *) (&r[0x25].vz));
      *((int *) (&r[0x28].vx)) = (*((int *) (&r[0x28].vx))) + 1;
      r->vx = r->vx + (-0xed8);
      *((int *) (&r[0x25].vz)) = li + 4;
      lj = *((int *) (&r[0x28].vx));
      *((int *) (&r[0x26].vz)) = (*((int *) (&r[0x26].vz))) + 2;
    }
    while (lj < 8);
    *((int *) (&r[0x28].vz)) = (*((int *) (&r[0x28].vz))) + 1;
    r->vz = r->vz + 0xed8;
    i = *((int *) (&r[0x28].vz));
  }
  while (i < 8);
  *((int *) (kaze_state_base_ptr + 0x9c)) = *((int *) (&r[0x25].vx));
}
