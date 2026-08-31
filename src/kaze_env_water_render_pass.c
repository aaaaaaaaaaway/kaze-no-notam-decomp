/**
 * kaze_env_water_render_pass  (render, 0x8002fac8)
 *
 * Below-horizon water/reflection pass gated by state flag 0x10000000:
 * animated reflected surface from terrain CLUT/TPage lookups, reflected sky
 * POLY_G4 block (+0x48504), and water horizon fill.
 */

#include "types.h"
#include "kaze_syms.h"
extern int kaze_live_stage_level;
extern char *kaze_state_base_ptr;
extern u16 kaze_terrain_tile_cull_mask;
extern int SetPolyFT4(int);
extern int SetSemiTrans(int, int);
extern void AddPrim(void *, void *);
extern int kaze_tile_edge_factor(void);
void kaze_env_water_render_pass(int ot, int ctx)
{
  s16 *g;
  char *c;
  int stage_off;
  int ac;
  int a4;

  g = (s16 *) 0x1f8000c8;
  c = (char *) 0x1f800000;
  if (((*((uint *) (kaze_state_base_ptr + 0x14))) & 0x10000000) != 0)
  {
    register int cc __asm__("$4");
    register char *base __asm__("$4");
    SetPolyFT4(0x1f80007c);
    SetSemiTrans(0x1f80007c, 0);
    base = kaze_state_base_ptr;
    {
      int cur = *((int *) (base + 0x9c));
      *((int *) 0x1f8000b4) = 2;
      *((int *) 0x1f800048) = ot;
      *((s16 *) 0x1f8000e2) = 0;
      *((s16 *) 0x1f8000da) = 0;
      *((s16 *) 0x1f8000d2) = 0;
      *((s16 *) 0x1f8000ca) = 0;
      {
        register char *tc __asm__("$3");
        tc = base + 0xec6e0;
        *((char **) 0x1f80004c) = tc;
      }
      *((u16 **) 0x1f800050) = &kaze_terrain_tile_cull_mask;
      *((int *) 0x1f800040) = cur;
    }
__asm__("" ::: "memory");
    {
      int t;
      {
        int *_p = (int *) base;
        t = (_p[73846] * 0x7f) / 0x1000;
      }
      *((int *) 0x1f8000b8) = t;
      if (t >= 0x30)
      {
        cc = 0xff;
        if (t < 0x100)
        {
          cc = t;
        }
      }
      else
      {
        cc = 0x30;
      }
      *((int *) (c + 0xb8)) = cc;
      *((u8 *) (c + 0x80)) = (u8) cc;
    }
    {
      int t;
      t = ((*((int *) (kaze_state_base_ptr + 0x481dc))) * 0x7f) / 0x1000;
      *((int *) (c + 0xbc)) = t;
      if (t >= 0x30)
      {
        cc = 0xff;
        if (t < 0x100)
        {
          cc = t;
        }
      }
      else
      {
        cc = 0x30;
      }
      *((int *) (c + 0xbc)) = cc;
      *((u8 *) (c + 0x81)) = (u8) cc;
    }
    {
      int t;
      t = ((*((int *) (kaze_state_base_ptr + 0x481e0))) * 0x7f) / 0x1000;
      *((int *) (c + 0xc0)) = t;
      if (t >= 0x30)
      {
        cc = 0xff;
        if (t < 0x100)
        {
          cc = t;
        }
      }
      else
      {
        cc = 0x30;
      }
      *((int *) (c + 0xc0)) = cc;
      *((u8 *) (c + 0x82)) = (u8) cc;
    }
    a4 = 0x40;
    if (kaze_live_stage_level == 1)
    {
      a4 = 0x20;
    }
    *((int *) (c + 0xa4)) = a4;
    *((int *) (c + 0x70)) = -8;
    do
    {
      *((int *) (c + 0x6c)) = -8;
      do
      {
        {
          int val = *(*((int **) (c + 0x4c)));
          *((int *) (c + 0x54)) = val;
          if ((val != 0) && ((val & 0x80) != 0))
          {
            register char *b __asm__("$5");
            u16 *p;
            uint u;
            int k5;
            int ci;
            int cj;
__asm__("");
            k5 = 0x5ae98;
            __asm__ __volatile__("" : "=r"(k5) : "0"(k5));
            ci = *((int *) (c + 0x6c));
            cj = *((int *) (c + 0x70));
            __asm__ __volatile__("");
            b = kaze_state_base_ptr;
            __asm__ __volatile__("" ::: "memory");
            {
              register int t1 __asm__("$3");
              int t2;
              int val2;
              t1 = ci * (-0xed8);
              {
                int t2a = cj * 0x1db;
                val2 = *((int *) (c + 0x54));
                __asm__ __volatile__("");
                t2 = t2a << 3;
              }
              __asm__ __volatile__("");
              g[8] = t1;
              g[0] = t1;
              g[6] = t2;
              g[2] = t2;
              u = val2 & 0x1f;
            }
            *((u16 *) (c + 0x5c)) = (u16) u;
            {
              int t3 = (*((u16 *) (g + 0))) - 0xed8;
              int t4 = (*((u16 *) (g + 2))) + 0xed8;
              g[12] = t3;
              g[4] = t3;
              g[14] = t4;
              g[10] = t4;
            }
            {
              register u16 tpv __asm__("$2");
              u16 clutv;
              register char *ca2 __asm__("$4");
              char *pb;
              register uint idx __asm__("$4");
              register uint pg __asm__("$3");
              tpv = *((u16 *) ((b + ((u >> 4) * 2)) + 0x5b698));
              __asm__ __volatile__("" : "=r"(tpv) : "0"(tpv));
              idx = *((u16 *) (c + 0x5c));
              pg = *((volatile u16 *) (c + 0x5c));
              ca2 = (char *) ((idx * 2) + (uint) b);
              pb = b + ((pg & 0xf) * 0x80 + k5);
              __asm__ __volatile__("");
              *((u16 *) (c + 0x92)) = tpv;
              __asm__ __volatile__("");
              clutv = *((u16 *) (ca2 + 0x5b69c));
              p = (u16 *) (pb + (((*((uint *) (c + 0x54))) & 0x60) >> 2));
              *((u16 **) (c + 0x58)) = p + 1;
              *((u16 *) (c + 0x8a)) = clutv;
            }
            {
              u16 *p1 = *((u16 **) (c + 0x58));
              u16 g3v = *p;
              *((u16 **) (c + 0x58)) = p1 + 1;
              g[3] = g3v;
              {
                u16 *p2 = *((u16 **) (c + 0x58));
                u16 g7v = *p1;
                *((u16 **) (c + 0x58)) = p2 + 1;
                g[7] = g7v;
                {
                  u16 g11v = *p2;
                  u16 *p3 = *((u16 **) (c + 0x58));
                  g[11] = g11v;
                  g[15] = *p3;
                }
              }
            }
            __asm__ __volatile__("");
            {
              register uint tv __asm__("$4");
              uint tvcond;
              tv = (uint) (*(*((u16 **) (c + 0x50))));
              tvcond = tv & 0x88;
              __asm__ __volatile__("");
              *((int *) (c + 0xb0)) = tv;
              if (tvcond != 0)
              {
                ac = kaze_tile_edge_factor();
__asm__("":"=r"(ac):"0"(ac));
                *((int *) (c + 0xac)) = ac;
                if (kaze_live_stage_level == 2)
                {
                  register int fade;
                  fade = ac << 1;
                  if (0x7f < ac)
                  {

                    register int shade_hi __asm__("$6");
                    fade = (0x100 - ac) * 2;
                    *((int *) (c + 0xac)) = fade;
                    if (fade >= 0)
                    {
                      shade_hi = 0xff;
                      if (fade < 0x100)
                      {
                        shade_hi = fade;
                      }
                    }
                    else
                    {
                      shade_hi = 0;
                    }
                    {
                      register int sh16 __asm__("$3");
                      int w;
                      int w2;
                      s16 *ga = g;
                      char *ca = c;
                      __asm__ __volatile__("" : "=r"(ga), "=r"(ca) : "0"(ga), "1"(ca));
                      w = (shade_hi << 8) | 0x2a000000;
                      w2 = shade_hi | w;
                      sh16 = shade_hi << 0x10;
                      w2 = w2 | sh16;
                      *((int *) (c + 0xac)) = shade_hi;
                      *((int *) (c + 0x54)) = w2;
                      kaze_gte_poly_project_clip_1424(ga, ca);
                    }
                    goto LAB_8002ff30;
                  }
                  *((int *) (c + 0xac)) = fade;
                  if (fade >= 0)
                  {
                    ac = 0xff;
                    if (fade < 0x100)
                    {
                      ac = fade;
                    }
                  }
                  else
                  {
                    ac = 0;
                  }
                  *((int *) (c + 0xac)) = ac;
                }
                {
                  int t = *((int *) (c + 0xac));
                  int w;
                  int w2;
                  w = (t << 8) | 0x2a000000;
                  w2 = t | w;
                  w2 = w2 | (t << 0x10);
                  *((int *) (c + 0x54)) = w2;
                }
                kaze_gte_poly_project_clip_1424(g, c);
              }
            }
            kaze_water_quad_subdivide_render(g, c, 0);
          }
        }
        LAB_8002ff30:
        *((int *) (c + 0x6c)) = (*((int *) (c + 0x6c))) + 1;

        *((int **) (c + 0x4c)) = (*((int **) (c + 0x4c))) + 1;
        *((u16 **) (c + 0x50)) = (*((u16 **) (c + 0x50))) + 1;
      }
      while ((*((int *) (c + 0x6c))) < 8);
      *((int *) (c + 0x70)) = (*((int *) (c + 0x70))) + 1;
    }
    while ((*((int *) (c + 0x70))) < 8);
    *((int *) (kaze_state_base_ptr + 0x9c)) = *((int *) (c + 0x40));
    *((void **) (c + 0x44)) = (void *) (ctx + 4);
    stage_off = (*((int *) (kaze_state_base_ptr + 0x98))) * 0xd8;
    if ((*((char *) ((kaze_state_base_ptr + stage_off) + 0x485be))) != '\0')
    {
      AddPrim(*((void **) (c + 0x44)), (kaze_state_base_ptr + (stage_off + 0x48504)) + 0x90);
      AddPrim(*((void **) (c + 0x44)), (kaze_state_base_ptr + (((*((int *) (kaze_state_base_ptr + 0x98))) * 0xd8) + 0x48504)) + 0xb4);
      AddPrim(*((void **) (c + 0x44)), (kaze_state_base_ptr + (((*((int *) (kaze_state_base_ptr + 0x98))) * 0x18) + 0x486e4)) + 0x10);
    }
    AddPrim(*((void **) (c + 0x44)), kaze_state_base_ptr + (((*((int *) (kaze_state_base_ptr + 0x98))) * 0xd8) + 0x48504));
    AddPrim(*((void **) (c + 0x44)), (kaze_state_base_ptr + (((*((int *) (kaze_state_base_ptr + 0x98))) * 0xd8) + 0x48504)) + 0x24);
    AddPrim(*((void **) (c + 0x44)), kaze_state_base_ptr + (((*((int *) (kaze_state_base_ptr + 0x98))) * 0x18) + 0x486e4));
    stage_off = (*((int *) (kaze_state_base_ptr + 0x98))) * 0xd8;
    if ((*((char *) ((kaze_state_base_ptr + stage_off) + 0x48576))) != '\0')
    {
      AddPrim(*((void **) (c + 0x44)), (kaze_state_base_ptr + (stage_off + 0x48504)) + 0x48);
      AddPrim(*((void **) (c + 0x44)), (kaze_state_base_ptr + (((*((int *) (kaze_state_base_ptr + 0x98))) * 0xd8) + 0x48504)) + 0x6c);
      AddPrim(*((void **) (c + 0x44)), (kaze_state_base_ptr + (((*((int *) (kaze_state_base_ptr + 0x98))) * 0x18) + 0x486e4)) + 0x8);
    }
    AddPrim(*((void **) (c + 0x44)), (kaze_state_base_ptr + (((*((int *) (kaze_state_base_ptr + 0x98))) * 0x30) + 0x482f4)) + 0x18);
  }
  return;
}
