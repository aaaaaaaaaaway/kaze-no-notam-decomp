#include "types.h"
#include "kaze_syms.h"
extern int kaze_task_record_index;
extern int kaze_gs_view_pos_x;
extern int kaze_gs_view_pos_z;
extern int kaze_gs_packet_buffer_a_ptr;
extern int kaze_gs_packet_buffer_b_ptr;
extern int kaze_gs_light0_dir_y;
extern int kaze_gs_light0_dir_z;
extern u8 kaze_gs_light0_color_r;
extern u8 kaze_gs_light0_color_g;
extern u8 kaze_gs_light0_color_b;
extern int kaze_gs_view_matrix;
extern short D_800B1508;
extern short D_800B150A;
extern short D_800B150C;
extern int kaze_menu_camera_trans_x;
extern int kaze_menu_camera_trans_y;
extern int kaze_menu_camera_trans_z;
extern int kaze_menu_gs_ot_buf1_length;
extern int kaze_menu_gs_ot_buf1_org;
extern int kaze_menu_model1_gsobj;
extern int kaze_menu_model1_coord_ptr;
extern int kaze_menu_model2_coord_ptr;
extern int kaze_menu_ot_work_buf1;
extern char *kaze_menu_view_coord_ptr;
extern short kaze_gs_light1_dir_z;
extern int D_800B56A4;
extern u8 kaze_gs_light1_color_r;
extern u8 kaze_gs_light1_color_g;
extern u8 kaze_gs_light1_color_b;
extern void GsInit3D(void);
extern void GsInitGraph(int, int, int, int, int);
extern void GsSetAmbient(int, int, int);
extern void GsSetLightMode(int);
extern void SetDispMask(int);
extern int kaze_menu_camera_pos_z;
extern int kaze_menu_camera_scroll;
typedef struct 
{
  char pad00[8];
  s16 x0;
  s16 y0;
  s16 x1;
  s16 y1;
  s16 x2;
  s16 y2;
} Tri;
typedef struct 
{
  char pad00[0x98];
  int f0;
  int f1;
  int f2;
  char a4[4];
  int avg;
  char ac[0xC4 - 0xAC];
  s16 bx0;
  s16 bx1;
  s16 by0;
  s16 by1;
} Out;
typedef struct 
{
  char pad00[8];
  s16 x0;
  s16 y0;
  s16 x1;
  s16 y1;
  s16 x2;
  s16 y2;
  s16 x3;
  s16 y3;
} Quad;
typedef struct 
{
  char pad00[0x98];
  int f0;
  int f1;
  int f2;
  int f3;
  int avg;
  char ac[0xC4 - 0xAC];
  s16 bx0;
  s16 bx1;
  s16 by0;
  s16 by1;
} Out__1;
typedef struct 
{
  char p[8];
  s16 x0;
  s16 y0;
  char p1[4];
  s16 x1;
  s16 y1;
  char p2[4];
  s16 x2;
  s16 y2;
} Tri__1;
typedef struct 
{
  char p[8];
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
} Quad__1;
typedef struct 
{
  char p[8];
  s16 x0;
  s16 y0;
  char p1[8];
  s16 x1;
  s16 y1;
  char p2[8];
  s16 x2;
  s16 y2;
} Tri__2;
typedef struct 
{
  char p[8];
  s16 x0;
  s16 y0;
  char p1[8];
  s16 x1;
  s16 y1;
  char p2[8];
  s16 x2;
  s16 y2;
  char p3[8];
  s16 x3;
  s16 y3;
} Quad__2;
typedef struct {
    int m[8];
} Block32;
typedef struct { int x, y; } XY;
typedef struct {
    s16 x, y, w, h;
} RECT;
typedef struct {
    char bytes[0x14A0];
} ModelBlock;

/**
 * kaze_tmd_midpoint_vertex_uv_rgb_6f34  (library, 0x80046f34)
 *
 * Subdivision midpoint helper for gouraud-textured prims: averages x/y/z,
 * U/V, and RGB bytes, copies the code byte at +0xb.
 */

void kaze_tmd_midpoint_vertex_uv_rgb_6f34(short *dst, short *v1, short *v2)
{
    dst[0] = (v1[0] + v2[0]) / 2;
    dst[1] = (v1[1] + v2[1]) / 2;
    dst[2] = (v1[2] + v2[2]) / 2;
    *(u8 *)((char *)dst + 6) = (*(u8 *)((char *)v1 + 6) + *(u8 *)((char *)v2 + 6)) >> 1;
    *(u8 *)((char *)dst + 7) = (*(u8 *)((char *)v1 + 7) + *(u8 *)((char *)v2 + 7)) >> 1;
    *(u8 *)((char *)dst + 8) = (*(u8 *)((char *)v1 + 8) + *(u8 *)((char *)v2 + 8)) >> 1;
    *(u8 *)((char *)dst + 9) = (*(u8 *)((char *)v1 + 9) + *(u8 *)((char *)v2 + 9)) >> 1;
    *(u8 *)((char *)dst + 0xA) = (*(u8 *)((char *)v1 + 0xA) + *(u8 *)((char *)v2 + 0xA)) >> 1;
    *(u8 *)((char *)dst + 0xB) = *(u8 *)((char *)v1 + 0xB);
}

/**
 * kaze_tmd_subdiv_screen_test_tri_7018  (library, 0x80047018)
 *
 * Subdivision screen test for flat tris: computes OTZ from 3 screen Zs and
 * min/max screen X/Y; returns 0 only when fully inside the 320x240 screen,
 * else 1 (subdivide/cull).
 */

int kaze_tmd_subdiv_screen_test_tri_7018(Tri *q, Out *t)
{
  int ret;
  int new_var;
  if (((t->f0 >= 0) || (t->f1 >= 0)) || (t->f2 >= 0))
  {
    t->avg = (((t->f0 + t->f1) + t->f2) / 3) >> 2;
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
    new_var = q->y1 < q->y0;
    if (new_var)
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
    if (t->by0 < 0)
    {
      ret = 1;
      goto done;
    }
    if (t->by1 < 0xF1)
    {
      new_var = 0;
      ret = new_var;
      goto done;
    }
    ret = 1;
    done:
    return ret;

  }
  return 1;
}

/**
 * kaze_tmd_subdiv_screen_test_quad_71bc  (library, 0x800471bc)
 *
 * Subdivision screen test for flat quads: rejects when all 4 screen Zs
 * negative, OTZ = sum>>4, min/max X/Y vs 320x240; 0 = fully on-screen, 1 =
 * subdivide/cull.
 */

int kaze_tmd_subdiv_screen_test_quad_71bc(Quad *q, Out__1 *t)
{
  int ret;
  if ((((t->f0 >= 0) || (t->f1 >= 0)) || (t->f2 >= 0)) || (t->f3 >= 0))
  {
    t->avg = ((((t->f0 + t->f1) + t->f2) + t->f3) / 4) >> 2;
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

/**
 * kaze_tmd_subdiv_screen_test_tri_g_73dc  (library, 0x800473dc)
 *
 * Subdivision screen test for gouraud-stride triangle scratch vertices: OTZ
 * from 3 screen Zs plus 320x240 bounding-box check; 0 = emit directly, 1 =
 * subdivide/cull.
 */

int kaze_tmd_subdiv_screen_test_tri_g_73dc(Tri__1 *q, Out *t)
{
  int ret;
  if (((t->f0 >= 0) || (t->f1 >= 0)) || (t->f2 >= 0))
  {
    t->avg = (((t->f0 + t->f1) + t->f2) / 3) >> 2;
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
    if (t->bx0 < 0)
    {
      ret = 1;
      goto done;
    }
    ret = t->bx1;
    if (ret >= 0x141)
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

/**
 * kaze_tmd_subdiv_screen_test_quad_g_7580  (library, 0x80047580)
 *
 * Subdivision screen test for gouraud-stride quad scratch vertices: 4 screen
 * Zs, OTZ, and 320x240 bounding-box check; 0 = emit directly, 1 =
 * subdivide/cull.
 */

int kaze_tmd_subdiv_screen_test_quad_g_7580(Quad__1 *q, Out__1 *t)
{
  int ret;
  if ((((t->f0 >= 0) || (t->f1 >= 0)) || (t->f2 >= 0)) || (t->f3 >= 0))
  {
    t->avg = ((((t->f0 + t->f1) + t->f2) + t->f3) / 4) >> 2;
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

/**
 * kaze_tmd_subdiv_screen_test_tri_ft_77a0  (library, 0x800477a0)
 *
 * Subdivision screen test for textured-flat-stride triangle scratch
 * vertices: OTZ plus 320x240 bounding-box check; 0 = emit directly, 1 =
 * subdivide/cull.
 */

int kaze_tmd_subdiv_screen_test_tri_ft_77a0(Tri__1 *q, Out *t)
{
  int ret;
  if (((t->f0 >= 0) || (t->f1 >= 0)) || (t->f2 >= 0))
  {
    t->avg = (((t->f0 + t->f1) + t->f2) / 3) >> 2;
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

/**
 * kaze_tmd_subdiv_screen_test_quad_ft_7944  (library, 0x80047944)
 *
 * Subdivision screen test for textured-flat-stride quad scratch vertices: 4
 * screen Zs, OTZ, and 320x240 bounding-box check; 0 = emit directly, 1 =
 * subdivide/cull.
 */

int kaze_tmd_subdiv_screen_test_quad_ft_7944(Quad__1 *q, Out__1 *t)
{
  int ret;
  if ((((t->f0 >= 0) || (t->f1 >= 0)) || (t->f2 >= 0)) || (t->f3 >= 0))
  {
    t->avg = ((((t->f0 + t->f1) + t->f2) + t->f3) / 4) >> 2;
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

/**
 * kaze_tmd_subdiv_screen_test_tri_gt_7b64  (library, 0x80047b64)
 *
 * Subdivision screen test for gouraud-textured-stride triangle scratch
 * vertices: OTZ plus 320x240 bounding-box check; 0 = emit directly, 1 =
 * subdivide/cull.
 */

int kaze_tmd_subdiv_screen_test_tri_gt_7b64(Tri__2 *q, Out *t)
{
  int ret;
  if (((t->f0 >= 0) || (t->f1 >= 0)) || (t->f2 >= 0))
  {
    t->avg = (t->f0 + t->f1) + t->f2;
    t->avg = (t->avg / 3) >> 2;
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
    if (t->by0 < 0)
    {
      ret = 1;
      goto done;
    }
    if (t->by1 < 0xF1)
    {
      ret = 0;
      goto done;
    }
    q++;
    q--;
    ret = 1;
    done:
    return ret;

  }
  return 1;
}

/**
 * kaze_tmd_subdiv_screen_test_quad_gt_7d08  (library, 0x80047d08)
 *
 * Subdivision screen test for gouraud-textured-stride quad scratch vertices:
 * 4 screen Zs, OTZ, and 320x240 bounding-box check; 0 = emit directly, 1 =
 * subdivide/cull.
 */

int kaze_tmd_subdiv_screen_test_quad_gt_7d08(Quad__2 *q, Out__1 *t)
{
  int ret;
  if ((((t->f0 >= 0) || (t->f1 >= 0)) || (t->f2 >= 0)) || (t->f3 >= 0))
  {
    t->avg = ((((t->f0 + t->f1) + t->f2) + t->f3) / 4) >> 2;
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

#include "gte.h"

/**
 * kaze_tmd_emit_poly_f3_7f28  (library, 0x80047f28)
 *
 * Subdivision leaf emit: rtpt's the sub-triangle, builds setPolyF3 (code
 * 0x20) with stored color, links the 4-word packet into the OT and advances
 * the packet cursor 0x14.
 */

void kaze_tmd_emit_poly_f3_7f28(int v0, int v1, int v2, int ctx)
{
    u32 *r0 = *(u32 **)(ctx + 0xc);

    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(v0));
    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(v0));
    __asm__ volatile("lwc2 $2, 0(%0)" :: "r"(v1));
    __asm__ volatile("lwc2 $3, 4(%0)" :: "r"(v1));
    __asm__ volatile("lwc2 $4, 0(%0)" :: "r"(v2));
    __asm__ volatile("lwc2 $5, 4(%0)" :: "r"(v2));
    r0[1] = *(u32 *)(v0 + 8);
    gte_rtpt();
    *((u8 *)r0 + 3) = 4;
    *((u8 *)r0 + 7) = 0x20;
    gte_stflg(ctx + 0xac);
    if (*(int *)(ctx + 0xac) >= 0) {
        __asm__ volatile("swc2 $12, 8(%0)" :: "r"(r0) : "memory");
        __asm__ volatile("swc2 $13, 12(%0)" :: "r"(r0) : "memory");
        __asm__ volatile("swc2 $14, 16(%0)" :: "r"(r0) : "memory");
        *r0 = (**(u32 **)(ctx + 0x10) & 0xffffff) | 0x4000000;
        **(u32 **)(ctx + 0x10) = (u32)r0 & 0xffffff;
        *(int *)(ctx + 0xc) += 0x14;
    }
}

/**
 * kaze_tmd_emit_poly_g3_7fd4  (library, 0x80047fd4)
 *
 * Subdivision leaf emit: rtpt's the sub-triangle, builds setPolyG3 (code
 * 0x30) with per-vertex colors, links the 6-word packet into the OT and
 * advances the cursor 0x1c.
 */

void kaze_tmd_emit_poly_g3_7fd4(int v0, int v1, int v2, int ctx)
{
    u32 *r0 = *(u32 **)(ctx + 0xc);

    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(v0));
    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(v0));
    __asm__ volatile("lwc2 $2, 0(%0)" :: "r"(v1));
    __asm__ volatile("lwc2 $3, 4(%0)" :: "r"(v1));
    __asm__ volatile("lwc2 $4, 0(%0)" :: "r"(v2));
    __asm__ volatile("lwc2 $5, 4(%0)" :: "r"(v2));
    r0[1] = *(u32 *)(v0 + 8);
    r0[3] = *(u32 *)(v1 + 8);
    gte_rtpt();
    r0[5] = *(u32 *)(v2 + 8);
    *((u8 *)r0 + 3) = 6;
    *((u8 *)r0 + 7) = 0x30;
    gte_stflg(ctx + 0xac);
    if (*(int *)(ctx + 0xac) >= 0) {
        __asm__ volatile("swc2 $12, 8(%0)" :: "r"(r0) : "memory");
        __asm__ volatile("swc2 $13, 16(%0)" :: "r"(r0) : "memory");
        __asm__ volatile("swc2 $14, 24(%0)" :: "r"(r0) : "memory");
        *r0 = (**(u32 **)(ctx + 0x10) & 0xffffff) | 0x6000000;
        **(u32 **)(ctx + 0x10) = (u32)r0 & 0xffffff;
        *(int *)(ctx + 0xc) += 0x1c;
    }
}

/**
 * kaze_tmd_emit_poly_ft3_8094  (library, 0x80048094)
 *
 * Subdivision leaf emit: rtpt's the sub-triangle, builds setPolyFT3 (code
 * 0x24) with UVs/clut/tpage, links the 7-word packet into the OT and
 * advances the cursor 0x20.
 */

void kaze_tmd_emit_poly_ft3_8094(int v0, int v1, int v2, int ctx)
{
    u32 *r0 = *(u32 **)(ctx + 0xc);

    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(v0));
    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(v0));
    __asm__ volatile("lwc2 $2, 0(%0)" :: "r"(v1));
    __asm__ volatile("lwc2 $3, 4(%0)" :: "r"(v1));
    __asm__ volatile("lwc2 $4, 0(%0)" :: "r"(v2));
    __asm__ volatile("lwc2 $5, 4(%0)" :: "r"(v2));
    r0[1] = *(u32 *)(v0 + 8);
    *(u16 *)((u8 *)r0 + 0xc) = *(u16 *)(v0 + 6);
    gte_rtpt();
    *(u16 *)((u8 *)r0 + 0x14) = *(u16 *)(v1 + 6);
    *(u16 *)((u8 *)r0 + 0x1c) = *(u16 *)(v2 + 6);
    *(u16 *)((u8 *)r0 + 0xe) = *(u16 *)(ctx + 0xcc);
    *(u16 *)((u8 *)r0 + 0x16) = *(u16 *)(ctx + 0xce);
    *((u8 *)r0 + 3) = 7;
    *((u8 *)r0 + 7) = 0x24;
    gte_stflg(ctx + 0xac);
    if (*(int *)(ctx + 0xac) >= 0) {
        __asm__ volatile("swc2 $12, 8(%0)" :: "r"(r0) : "memory");
        __asm__ volatile("swc2 $13, 16(%0)" :: "r"(r0) : "memory");
        __asm__ volatile("swc2 $14, 24(%0)" :: "r"(r0) : "memory");
        *r0 = (**(u32 **)(ctx + 0x10) & 0xffffff) | 0x7000000;
        **(u32 **)(ctx + 0x10) = (u32)r0 & 0xffffff;
        *(int *)(ctx + 0xc) += 0x20;
    }
}

/**
 * kaze_tmd_emit_poly_gt3_8178  (library, 0x80048178)
 *
 * Subdivision leaf emit: rtpt's the sub-triangle, builds setPolyGT3 (code
 * 0x34) with per-vertex UV+RGB, links the 9-word packet into the OT and
 * advances the cursor 0x28.
 */

void kaze_tmd_emit_poly_gt3_8178(int v0, int v1, int v2, int ctx)
{
    u32 *r0 = *(u32 **)(ctx + 0xc);

    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(v0));
    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(v0));
    __asm__ volatile("lwc2 $2, 0(%0)" :: "r"(v1));
    __asm__ volatile("lwc2 $3, 4(%0)" :: "r"(v1));
    __asm__ volatile("lwc2 $4, 0(%0)" :: "r"(v2));
    __asm__ volatile("lwc2 $5, 4(%0)" :: "r"(v2));
    r0[1] = *(u32 *)(v0 + 8);
    r0[4] = *(u32 *)(v1 + 8);
    r0[7] = *(u32 *)(v2 + 8);
    *(u16 *)((u8 *)r0 + 0xc) = *(u16 *)(v0 + 6);
    gte_rtpt();
    *(u16 *)((u8 *)r0 + 0x18) = *(u16 *)(v1 + 6);
    *(u16 *)((u8 *)r0 + 0x24) = *(u16 *)(v2 + 6);
    *(u16 *)((u8 *)r0 + 0xe) = *(u16 *)(ctx + 0xcc);
    *(u16 *)((u8 *)r0 + 0x1a) = *(u16 *)(ctx + 0xce);
    *((u8 *)r0 + 3) = 9;
    *((u8 *)r0 + 7) = 0x34;
    gte_stflg(ctx + 0xac);
    if (*(int *)(ctx + 0xac) >= 0) {
        __asm__ volatile("swc2 $12, 8(%0)" :: "r"(r0) : "memory");
        __asm__ volatile("swc2 $13, 20(%0)" :: "r"(r0) : "memory");
        __asm__ volatile("swc2 $14, 32(%0)" :: "r"(r0) : "memory");
        *r0 = (**(u32 **)(ctx + 0x10) & 0xffffff) | 0x9000000;
        **(u32 **)(ctx + 0x10) = (u32)r0 & 0xffffff;
        *(int *)(ctx + 0xc) += 0x28;
    }
}

void kaze_tmd_const_color_quad_8858();
void kaze_tmd_const_color_tri_86d0();
#define CLAMP8(dst, src) \
    do { \
        if ((src) >= 0) { \
            (dst) = 0xff; \
            if ((src) < 0x100) { \
                (dst) = (src); \
            } \
        } else { \
            (dst) = 0; \
        } \
    } while (0)
#define TRY_QUAD_FIRST() \
    { \
        if (*(int *)p[2] != 0) { \
            kaze_tmd_const_color_quad_8858(p); \
        } \
    }
#define TRY_QUAD_STEP() \
    { \
        u32 old = p[2]; \
        p[2] = old - 0x10; \
        if (*(int *)(old - 0x10) != 0) { \
            kaze_tmd_const_color_quad_8858(p); \
        } \
    }
#define TRY_TRI_STEP() \
    { \
        u32 old = p[2]; \
        p[2] = old - 0x10; \
        if (*(int *)(old - 0x10) != 0) { \
            kaze_tmd_const_color_tri_86d0(p); \
        } \
    }

/**
 * kaze_tmd_sort_const_color_trans_8274  (library, 0x80048274)
 *
 * Draws a TMD model as constant-color semi-transparent silhouette: clamps
 * RGB into scratchpad words 0x22/0x2a (F3/F4+ABE) at 0x1f8002b0.., walks 20
 * primitive classes calling
 * kaze_tmd_const_color_quad_8858/kaze_tmd_const_color_tri_86d0.
 */

int kaze_tmd_sort_const_color_trans_8274(
    int ot, int tag, int ctx, int out, int shift, int r, int g, int b)
{
    register u32 *p __asm__("$16");
    register int shift_r __asm__("$11");
    register int r_r __asm__("$8");
    register int g_r __asm__("$9");
    register int b_r __asm__("$10");
    int t;
    int gg;
    register int bb __asm__("$5");
    register int rr_h __asm__("$2");
    register int gg_h __asm__("$3");
    register u32 c __asm__("$4");
    register int bb_h __asm__("$2");
    int ot_tail;

    p = (u32 *)0x1f800200;
    r_r = r;
    shift_r = shift;
    g_r = g;
    b_r = b;
    tag |= 0x40000000;
    *(u32 *)0x1f800290 = tag;
    if (tag < 0) {
        return out;
    }

    ot_tail = ot + 0x198;
    p[0] = shift_r;
    *(u32 *)0x1f800208 = ot_tail;
    *(u32 *)0x1f80020c = out;
    *(u32 *)0x1f800214 = ctx;
    *(u32 *)0x1f80028c = (((u32)tag >> 23) & 0x60) | 0xe1000200;

    CLAMP8(t, r_r);
    r_r = t;
    p[0x2c] = r_r;
    CLAMP8(t, g_r);
    gg = t << 8;
    p[0x2d] = gg;
    CLAMP8(bb, b_r);
    bb = bb << 0x10;
    p[0x2e] = bb;
    gg_h = p[0x2d];
    rr_h = p[0x2c];
    c = 0x22000000;
    gg_h |= c;
    rr_h |= gg_h;
    rr_h |= bb;
    p[0x2f] = rr_h;
    c = 0x2a000000;
    rr_h = p[0x2d];
    gg_h = p[0x2c];
    rr_h |= c;
    gg_h |= rr_h;
    bb_h = p[0x2e];
    gg_h |= bb_h;
    p[0x30] = gg_h;

    TRY_QUAD_FIRST();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    p[2] = p[2] - 0x10;

    return p[3];
}

/**
 * kaze_tmd_const_color_tri_86d0  (library, 0x800486d0)
 *
 * Constant-color triangle worker for the silhouette draw: transform/cull 3
 * vertices, emits a fixed-color F3 packet (color word ctx[0x2f]) behind a
 * tpage word into the OT.
 */

void kaze_tmd_const_color_tri_86d0(u32 *p)
{
    u32 tail;
    u32 mask;
    u32 *vp;
    u32 *sx;

    p[1] = **(u32 **)(p + 2);
    tail = *(u32 *)(p[2] + 8);
    p[0x28] = 0;
    p[0x34] = tail;
    if (p[1] != 0) {
        mask = 0xffffff;
loop:
        vp = (u32 *)p[0x34];
        gte_ldv3(vp[0], vp[1], vp[2]);
        p[0x34] = (u32)(vp + 3);
        gte_rtpt();
        p[0x28] = p[0x28] + 1;
        __asm__ volatile("cop2 0x1400006"); /* nclip */
        __asm__ volatile("swc2 $24, 0(%0)" :: "r"(p + 0x2a) : "memory");
        if ((int)p[0x2a] > 0) {
            __asm__ volatile("cop2 0x158002D"); /* avsz3 */
            __asm__ volatile("swc2 $7, 0(%0)" :: "r"(p + 0x29) : "memory");
            if ((int)p[0x29] > 0) {
                sx = (u32 *)(p[3] + 8);
                __asm__ volatile("swc2 $12, 8(%0)" :: "r"(sx) : "memory");
                __asm__ volatile("swc2 $13, 12(%0)" :: "r"(sx) : "memory");
                __asm__ volatile("swc2 $14, 16(%0)" :: "r"(sx) : "memory");
                p[0x29] = (int)p[0x29] >> *(int *)p;
                *(u32 *)p[3] = ((u32)((u32 *)p[3] + 2) & mask) | 0x1000000;
                *(u32 *)(p[3] + 4) = p[0x23];
                *(u32 *)(p[3] + 8) = (*(u32 *)(p[0x29] * 4 + p[5]) & mask) | 0x4000000;
                *(u32 *)(p[0x29] * 4 + p[5]) = p[3] & mask;
                *(u32 *)(p[3] + 0xc) = p[0x2f];
                p[3] = p[3] + 0x1c;
            }
        }
        if (p[0x28] < p[1])
            goto loop;
    }
}

/**
 * kaze_tmd_const_color_quad_8858  (library, 0x80048858)
 *
 * Constant-color quad worker for the silhouette draw: transform/cull 4
 * vertices, emits a fixed-color F4 packet (color word ctx[0x30]) behind a
 * tpage word into the OT.
 */

void kaze_tmd_const_color_quad_8858(u32 *arg)
{
    register u32 *p asm("$5");
    u32 *vp;
    register u32 mask asm("$6");
    register u32 *va asm("$7");
    register u32 *vb asm("$8");
    register u32 *vc asm("$9");
    register u32 *v0 asm("$7");
    u32 *prim;

    p = arg;
    p[1] = **(u32 **)(p + 2);
    p[0x34] = *(u32 *)(p[2] + 8);
    p[0x28] = 0;
    if (p[1] == 0) {
        return;
    }

    mask = 0xffffff;
loop:
    vp = (u32 *)p[0x34];
    va = (u32 *)vp[0];
    vb = (u32 *)vp[1];
    vc = (u32 *)vp[2];
    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(va));
    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(va));
    __asm__ volatile("lwc2 $2, 0(%0)" :: "r"(vb));
    __asm__ volatile("lwc2 $3, 4(%0)" :: "r"(vb));
    __asm__ volatile("lwc2 $4, 0(%0)" :: "r"(vc));
    __asm__ volatile("lwc2 $5, 4(%0)" :: "r"(vc));
    p[0x34] = (u32)(vp + 3);
    gte_rtpt();
    p[0x28] = p[0x28] + 1;
    __asm__ volatile("cop2 0x1400006");
    __asm__ volatile("swc2 $24, 0(%0)" :: "r"(p + 0x2a) : "memory");
    prim = (u32 *)(p[3] + 8);
    __asm__ volatile("swc2 $12, 8(%0)" :: "r"(prim) : "memory");
    __asm__ volatile("swc2 $13, 12(%0)" :: "r"(prim) : "memory");
    __asm__ volatile("swc2 $14, 16(%0)" :: "r"(prim) : "memory");

    vp = (u32 *)p[0x34];
    v0 = (u32 *)vp[0];
    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(v0));
    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(v0));
    p[0x34] = (u32)(vp + 1);
    gte_rtps_stsxy2_off(p, 0x1c);
    __asm__ volatile("cop2 0x168002E");
    __asm__ volatile("swc2 $7, 0(%0)" :: "r"(p + 0x29) : "memory");

    if ((int)p[0x29] > 0) {
        __asm__ volatile("cop2 0x1400006");
        __asm__ volatile("swc2 $24, 0(%0)" :: "r"(p + 0x2b) : "memory");
        if (((int)p[0x2a] > 0) || ((int)p[0x2b] <= 0)) {
            p[0x29] = (int)p[0x29] >> *(int *)p;
            *(u32 *)p[3] = ((u32)((u32 *)p[3] + 2) & mask) | 0x1000000;
            *(u32 *)(p[3] + 4) = p[0x23];
            *(u32 *)(p[3] + 8) = (*(u32 *)(p[0x29] * 4 + p[5]) & mask) | 0x5000000;
            *(u32 *)(p[0x29] * 4 + p[5]) = p[3] & mask;
            *(u32 *)(p[3] + 0xc) = p[0x30];
            p[3] = p[3] + 0x20;
        }
    }

    if (p[0x28] < p[1]) {
        goto loop;
    }
}

void kaze_tmd_const_color_opaque_quad_8fc0();
void kaze_tmd_const_color_opaque_tri_8e68();
#undef TRY_QUAD_FIRST
#define TRY_QUAD_FIRST() \
    { \
        if (*(int *)p[2] != 0) { \
            kaze_tmd_const_color_opaque_quad_8fc0(p); \
        } \
    }
#undef TRY_QUAD_STEP
#define TRY_QUAD_STEP() \
    { \
        u32 old = p[2]; \
        p[2] = old - 0x10; \
        if (*(int *)(old - 0x10) != 0) { \
            kaze_tmd_const_color_opaque_quad_8fc0(p); \
        } \
    }
#undef TRY_TRI_STEP
#define TRY_TRI_STEP() \
    { \
        u32 old = p[2]; \
        p[2] = old - 0x10; \
        if (*(int *)(old - 0x10) != 0) { \
            kaze_tmd_const_color_opaque_tri_8e68(p); \
        } \
    }

/**
 * kaze_tmd_sort_const_color_opaque_8a38  (library, 0x80048a38)
 *
 * Opaque variant of the constant-color TMD model draw: builds RGB words with
 * codes 0x20/0x28 (no ABE, no tpage preamble) and walks 20 primitive classes
 * calling kaze_tmd_const_color_opaque_quad_8fc0/kaze_tmd_const_color_opaque_
 * tri_8e68.
 */

int kaze_tmd_sort_const_color_opaque_8a38(
    int ot, int tag, int ctx, int out, int shift, int r, int g, int b)
{
    register u32 *p __asm__("$16");
    register int shift_r __asm__("$3");
    register int r_r __asm__("$8");
    register int g_r __asm__("$9");
    register int b_r __asm__("$10");
    int t;
    int gg;
    register int bb __asm__("$5");
    register int rr_h __asm__("$2");
    register int gg_h __asm__("$3");
    register u32 c __asm__("$4");
    register int bb_h __asm__("$2");
    int ot_tail;

    p = (u32 *)0x1f800200;
    r_r = r;
    shift_r = shift;
    g_r = g;
    b_r = b;
    if (tag < 0) {
        return out;
    }

    ot_tail = ot + 0x198;
    p[0] = shift_r;
    p[2] = ot_tail;
    p[3] = out;
    p[5] = ctx;

    CLAMP8(t, r_r);
    r_r = t;
    p[0x2c] = r_r;
    CLAMP8(t, g_r);
    gg = t << 8;
    p[0x2d] = gg;
    CLAMP8(bb, b_r);
    bb = bb << 0x10;
    p[0x2e] = bb;
    gg_h = p[0x2d];
    rr_h = p[0x2c];
    c = 0x20000000;
    gg_h |= c;
    rr_h |= gg_h;
    rr_h |= bb;
    p[0x2f] = rr_h;
    c = 0x28000000;
    rr_h = p[0x2d];
    gg_h = p[0x2c];
    rr_h |= c;
    gg_h |= rr_h;
    bb_h = p[0x2e];
    gg_h |= bb_h;
    p[0x30] = gg_h;

    TRY_QUAD_FIRST();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_QUAD_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    TRY_TRI_STEP();
    p[2] = p[2] - 0x10;

    return p[3];
}

/**
 * kaze_tmd_const_color_opaque_tri_8e68  (library, 0x80048e68)
 *
 * Constant-color opaque triangle worker: transform/cull 3 vertices, emits a
 * fixed-color POLY_F3 packet (4 words, color ctx[0x2f]) directly into the
 * OT.
 */

void kaze_tmd_const_color_opaque_tri_8e68(u32 *p)
{
    u32 tail;
    u32 *opz_ptr;
    u32 *otz_ptr;
    u32 mask;
    u32 *vp;
    int shifted;

    p[1] = **(u32 **)(p + 2);
    tail = *(u32 *)(p[2] + 8);
    p[0x28] = 0;
    p[0x34] = tail;
    if (p[1] != 0) {
        opz_ptr = p + 0x2a;
        otz_ptr = p + 0x29;
        mask = 0xffffff;
loop:
        vp = (u32 *)p[0x34];
        gte_ldv3_ptr3(vp);
        p[0x34] = (u32)(vp + 3);
        gte_rtpt();
        p[0x28] = p[0x28] + 1;
        __asm__ volatile(
            "cop2 0x1400006\n\t"
            "swc2 $24, 0(%0)"
            :: "r"(opz_ptr) : "memory");
        if ((int)p[0x2a] > 0) {
            __asm__ volatile(
                "cop2 0x158002D\n\t"
                "swc2 $7, 0(%0)"
                :: "r"(otz_ptr) : "memory");
            if ((int)p[0x29] > 0) {
                gte_stsxy3_f3_mptr(p[3]);
                shifted = (int)p[0x29] >> *(int *)p;
                p[0x29] = shifted;
                *(u32 *)p[3] = (*(u32 *)(shifted * 4 + p[5]) & mask) | 0x4000000;
                *(u32 *)(p[0x29] * 4 + p[5]) = p[3] & mask;
                *(u32 *)(p[3] + 4) = p[0x2f];
                p[3] = p[3] + 0x14;
            }
        }
        if (p[0x28] < p[1])
            goto loop;
    }
}

/**
 * kaze_tmd_const_color_opaque_quad_8fc0  (library, 0x80048fc0)
 *
 * Constant-color opaque quad worker: transform/cull 4 vertices, emits a
 * fixed-color POLY_F4 packet (5 words, color ctx[0x30]) directly into the
 * OT.
 */

void kaze_tmd_const_color_opaque_quad_8fc0(u32 *arg)
{
    register u32 *p asm("$5");
    u32 *vp;
    register u32 mask asm("$6");
    register u32 *prim asm("$7");
    register u32 *v0 asm("$8");
    register u32 *va asm("$7");
    register u32 *vb asm("$8");
    register u32 *vc asm("$9");
    u32 shifted;

    p = arg;
    p[1] = **(u32 **)(p + 2);
    p[0x34] = *(u32 *)(p[2] + 8);
    p[0x28] = 0;
    if (p[1] == 0) {
        return;
    }

    mask = 0xffffff;
loop:
    vp = (u32 *)p[0x34];
    va = (u32 *)vp[0];
    vb = (u32 *)vp[1];
    vc = (u32 *)vp[2];
    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(va));
    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(va));
    __asm__ volatile("lwc2 $2, 0(%0)" :: "r"(vb));
    __asm__ volatile("lwc2 $3, 4(%0)" :: "r"(vb));
    __asm__ volatile("lwc2 $4, 0(%0)" :: "r"(vc));
    __asm__ volatile("lwc2 $5, 4(%0)" :: "r"(vc));
    p[0x34] = (u32)(vp + 3);
    gte_rtpt();
    p[0x28] = p[0x28] + 1;
    __asm__ volatile("cop2 0x1400006");
    __asm__ volatile("swc2 $24, 0(%0)" :: "r"(p + 0x2a) : "memory");
    prim = (u32 *)p[3];
    __asm__ volatile("swc2 $12, 8(%0)" :: "r"(prim) : "memory");
    __asm__ volatile("swc2 $13, 12(%0)" :: "r"(prim) : "memory");
    __asm__ volatile("swc2 $14, 16(%0)" :: "r"(prim) : "memory");

    vp = (u32 *)p[0x34];
    v0 = (u32 *)vp[0];
    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(v0));
    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(v0));
    p[0x34] = (u32)(vp + 1);
    gte_rtps_stsxy2_off(p, 0x14);
    __asm__ volatile("cop2 0x168002E");
    __asm__ volatile("swc2 $7, 0(%0)" :: "r"(p + 0x29) : "memory");

    if ((int)p[0x29] > 0) {
        __asm__ volatile("cop2 0x1400006");
        __asm__ volatile("swc2 $24, 0(%0)" :: "r"(p + 0x2b) : "memory");
        if (((int)p[0x2a] > 0) || ((int)p[0x2b] <= 0)) {
            shifted = (int)p[0x29] >> *(int *)p;
            p[0x29] = shifted;
            *(u32 *)p[3] = (*(u32 *)(shifted * 4 + p[5]) & mask) | 0x5000000;
            *(u32 *)(p[0x29] * 4 + p[5]) = p[3] & mask;
            *(u32 *)(p[3] + 4) = p[0x30];
            p[3] = p[3] + 0x18;
        }
    }

    if (p[0x28] < p[1]) {
        goto loop;
    }
}

extern int kaze_menu_ot_work_buf0;
extern int kaze_menu_gs_ot_buf;
extern int kaze_menu_gs_ot_buf0_org;
extern void VSync(int);
void kaze_vram_clear_black_95b8();
extern int kaze_heap_alloc(int, int);
void kaze_gs_view_init_92c8();
void kaze_gs_light_init_default_93b0();

/**
 * kaze_gs_graphics_init_9170  (render, 0x80049170)
 *
 * Gs render init: GsInitGraph(320,240)/GsInit3D, clears VRAM, allocates two
 * 48000-byte OT buffers (kaze_menu_ot_work_buf0/c4) and two 0x4000-byte
 * packet buffers (kaze_gs_packet_buffer_a_ptr/8c), OT length 12
 * (kaze_menu_gs_ot_buf/9c), inits view+lights.
 */

void kaze_gs_graphics_init_9170(void)
{
    int *p;

    VSync(0);
    SetDispMask(0);
    GsInitGraph(0x140, 0xF0, 4, 1, 0);
    kaze_gs_def_disp_buff(0, 0, 0, 0xF0);
    GsInit3D();
    kaze_vram_clear_black_95b8();
    kaze_menu_ot_work_buf0 = kaze_heap_alloc(48000, 1);
    kaze_menu_ot_work_buf1 = kaze_heap_alloc(48000, 1);
    p = &kaze_gs_packet_buffer_a_ptr;
    *p = kaze_heap_alloc(0x1000, 4);
    kaze_gs_packet_buffer_b_ptr = kaze_heap_alloc(0x1000, 4);
    kaze_menu_gs_ot_buf = 0xC;
    kaze_menu_gs_ot_buf1_length = 0xC;
    kaze_menu_gs_ot_buf0_org = *p;
    kaze_menu_gs_ot_buf1_org = kaze_gs_packet_buffer_b_ptr;
    kaze_gs_view_init_92c8();
    kaze_gs_light_init_default_93b0();
    SetDispMask(1);
    return;
}

extern void kaze_heap_free(int);

/**
 * kaze_gs_graphics_free_9270  (render, 0x80049270)
 *
 * Frees the Gs render buffers allocated by kaze_gs_graphics_init_9170 (OT
 * buffers kaze_menu_ot_work_buf0/c4 and packet buffers
 * kaze_gs_packet_buffer_a_ptr/8c) via heap free kaze_heap_free.
 */

void kaze_gs_graphics_free_9270(void)
{
    kaze_heap_free(kaze_menu_ot_work_buf0);
    kaze_heap_free(kaze_menu_ot_work_buf1);
    kaze_heap_free(kaze_gs_packet_buffer_a_ptr);
    kaze_heap_free(kaze_gs_packet_buffer_b_ptr);
}

extern void kaze_gs_set_projection(int arg0);
extern int kaze_gs_view_pos_y;
extern Block32 kaze_gs_view2_template;

/**
 * kaze_gs_view_init_92c8  (render, 0x800492c8)
 *
 * Resets view position kaze_gs_view_pos_x..68 (z=-0xfa0) and angles
 * kaze_gs_view_rot_x.., inits GsCOORDINATE2 kaze_gs_view_coord2, copies
 * matrix kaze_gs_view2_template into GsVIEW2 kaze_menu_gs_view2, GsSetView2,
 * sets projection via kaze_gs_set_projection(0x180).
 */

void kaze_gs_view_init_92c8(void)
{
    extern Block32 kaze_menu_gs_view2;
    extern char kaze_gs_view_coord2[];
    extern short kaze_gs_view_rot_z;
    extern short kaze_gs_view_rot_y;
    extern short kaze_gs_view_rot_x;
    extern void GsSetView2(void *arg0);
    extern void GsInitCoordinate2(int arg0, void *arg1);
    kaze_gs_view_pos_x = 0;
    kaze_gs_view_pos_y = 0;
    kaze_gs_view_pos_z = -0xFA0;
    kaze_gs_view_rot_x = 0;
    kaze_gs_view_rot_y = 0;
    kaze_gs_view_rot_z = 0;
    D_800B150C = 0;
    D_800B150A = 0;
    D_800B1508 = 0;
    GsInitCoordinate2(0, kaze_gs_view_coord2);
    kaze_menu_gs_view2 = kaze_gs_view2_template;
    kaze_menu_view_coord_ptr = kaze_gs_view_coord2;
    GsSetView2(&kaze_menu_gs_view2);
    kaze_gs_set_projection(0x180);
}

extern int kaze_gs_light0_dir_x;

/**
 * kaze_gs_light_init_default_93b0  (render, 0x800493b0)
 *
 * Sets flat light 0 pointing +Z (0,0,0x1000) with white color, ambient
 * 0x400, GsSetLightMode(0); also seeds secondary light params:
 * kaze_gs_light1_dir_z=0xd556 and an unnamed word right before the light1
 * color bytes, set to 0x1400000.
 */

void kaze_gs_light_init_default_93b0(void)
{
    extern void GsSetFlatLight(int, void *);
    int *p = &kaze_gs_light0_dir_x;
    int a = 0x1000;
    int c = 0xff;

    __asm__("");
    *p = 0;
    kaze_gs_light0_dir_z = a;
    kaze_gs_light1_dir_z = -0x2aaa;
    kaze_gs_light0_dir_y = 0;
    kaze_gs_light0_color_r = c;
    kaze_gs_light0_color_g = c;
    kaze_gs_light0_color_b = c;
    D_800B56A4 = 0x1400000;
    kaze_gs_light1_color_r = c;
    kaze_gs_light1_color_g = c;
    kaze_gs_light1_color_b = c;
    GsSetFlatLight(0, p);
    GsSetAmbient(0x400, 0x400, 0x400);
    GsSetLightMode(0);
}

/**
 * kaze_gs_light_init_alt_9454  (render, 0x80049454)
 *
 * Alternate light setup: flat light 0 pointing +X (0x1000,0,0) with white
 * color, ambient 0x800, GsSetLightMode(0); writes GsF_LIGHT block at
 * kaze_gs_light0_dir_x.
 */

void kaze_gs_light_init_alt_9454(void)
{
    extern void GsSetFlatLight(int, int *);
    int *p;

    p = &kaze_gs_light0_dir_x;
    *p = 0x1000;
    kaze_gs_light0_dir_y = 0;
    kaze_gs_light0_dir_z = 0;
    kaze_gs_light0_color_r = 0xFF;
    kaze_gs_light0_color_g = 0xFF;
    kaze_gs_light0_color_b = 0xFF;
    GsSetFlatLight(0, p);
    GsSetAmbient(0x800, 0x800, 0x800);
    GsSetLightMode(0);
    return;
}

extern void RotMatrix(s16 *, int *);
extern void kaze_trans_matrix(int *, int *);

/**
 * kaze_gs_view_set_pose_94c8  (render, 0x800494c8)
 *
 * Sets camera angles kaze_gs_view_rot_x/aa/ac and translation
 * kaze_gs_view_pos_x/64/68, rebuilds the view matrix via RotMatrix into
 * kaze_gs_view_matrix, clears the coord flag kaze_gs_view_coord2, and re-
 * applies GsSetView2(kaze_menu_gs_view2).
 */

void kaze_gs_view_set_pose_94c8(s16 a, s16 b, s16 c, int d, XY p)
{
    extern void GsSetView2(int *);
    extern int kaze_menu_gs_view2;
    extern int kaze_gs_view_coord2;
    extern s16 kaze_gs_view_rot_z;
    extern s16 kaze_gs_view_rot_y;
    extern s16 kaze_gs_view_rot_x;
    s16 *sv = &kaze_gs_view_rot_x;
    int *m = &kaze_gs_view_matrix;
    int *tr = &kaze_gs_view_pos_x;
    int ev, fv;

    *sv = a;
    ev = p.x;
    fv = p.y;
    kaze_gs_view_rot_y = b;
    kaze_gs_view_rot_z = c;
    *tr = d;
    kaze_gs_view_pos_y = ev;
    kaze_gs_view_pos_z = fv;
    RotMatrix(sv, m);
    kaze_trans_matrix(m, tr);
    kaze_gs_view_coord2 = 0;
    GsSetView2(&kaze_menu_gs_view2);
}

/**
 * kaze_gs_light_set_color_9568  (render, 0x80049568)
 *
 * Writes RGB bytes for flat light N into kaze_gs_light0_color_r+N*0x10 and
 * re-registers the light via GsSetFlatLight(N, kaze_gs_light0_dir_x+N*0x10).
 */

void kaze_gs_light_set_color_9568(int idx, u8 r, u8 g, u8 b)
{
    extern void GsSetFlatLight(int, int *);
    int i = idx * 0x10;

    (&kaze_gs_light0_color_r)[i] = r;
    (&kaze_gs_light0_color_g)[i] = g;
    (&kaze_gs_light0_color_b)[i] = b;
    GsSetFlatLight(idx, &kaze_gs_light0_dir_x + idx * 4);
}

extern void ClearImage(RECT *, int, int, int);
extern void DrawSync(int);

/**
 * kaze_vram_clear_black_95b8  (render, 0x800495b8)
 *
 * Clears the 320x480 VRAM rect (both framebuffers) to black via ClearImage
 * and waits with DrawSync(0).
 */

void kaze_vram_clear_black_95b8(void)
{
    RECT rect;

    rect.w = 0x140;
    rect.x = 0;
    rect.y = 0;
    rect.h = 0x1E0;
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
}

extern int kaze_save_slot_unpack_buffer[];
extern int kaze_menu_camera_coord_flags;
extern short kaze_menu_camera_rot_x;
extern short kaze_menu_camera_rot_y;
extern short kaze_menu_camera_rot_z;
extern int kaze_menu_camera_pos_x;
extern short kaze_menu_render_rot_x;
extern short kaze_menu_render_rot_y;
extern short kaze_menu_render_rot_z;
extern void GsMapModelingData(ulong *);
extern void GsLinkObject4(int, void *, int);

/**
 * kaze_gs_model_pair_link_init_9604  (render, 0x80049604)
 *
 * Maps two TMD models and links them as GsDOBJ2 kaze_menu_model1_gsobj and
 * its paired model-2 structure just before kaze_menu_model2_coord_ptr
 * (coords kaze_menu_camera_coord_flags/kaze_menu_camera_coord_word); builds
 * 264-entry packet tables, copies a per-index (kaze_task_record_index)
 * 0x528-word block into model 1, zeroes both poses.
 */

void kaze_gs_model_pair_link_init_9604(int arg0, int arg1)
{
    extern void GsInitCoordinate2(void *, void *);
    register int *second asm("$18");
    ModelBlock *src;
    ModelBlock *dst;
    char *coord;
    char *obj;

    second = (int *)arg1;
    __asm__("" : "=r"(second) : "0"(second));

    {
        register int *model asm("$17");

        model = (int *)arg0;
        model += 1;
        __asm__("" : "=r"(model) : "0"(model));
        GsMapModelingData((ulong *)model++);
        model += 1;
        __asm__("" : "=r"(model) : "0"(model));
        kaze_model_packet_table_build_97ec((int)model);
        kaze_model_packet_triple_table_build_9828((int)model);

        dst = (ModelBlock *)model[4];
        src = (ModelBlock *)kaze_save_slot_unpack_buffer[kaze_task_record_index];
        *dst = *src;

        {
            register void *zero asm("$4");

            zero = 0;
            __asm__("" : "=r"(zero) : "0"(zero));
            coord = (char *)&kaze_menu_camera_coord_flags;
            GsInitCoordinate2(zero, coord);
        }
        obj = (char *)&kaze_menu_model1_gsobj;
        GsLinkObject4((int)model, obj, 0);
    }
    *(int *)obj = 0;
    kaze_menu_model1_coord_ptr = (int)coord;

    second += 1;
    __asm__("" : "=r"(second) : "0"(second));
    GsMapModelingData((ulong *)second++);
    second += 1;
    {
        register void *zero asm("$4");

        zero = 0;
        __asm__("" : "=r"(zero) : "0"(zero));
        coord += 0x50;
        GsInitCoordinate2(zero, coord);
    }
    GsLinkObject4((int)second, obj + 0x10, 0);
    *(int *)(obj + 0x10) = 0;
    kaze_menu_model2_coord_ptr = (int)coord;

    kaze_menu_camera_rot_x = 0;
    kaze_menu_camera_rot_z = 0;
    kaze_menu_camera_rot_y = 0;
    kaze_menu_camera_pos_x = 0;
    kaze_menu_camera_scroll = 0;
    kaze_menu_camera_pos_z = 0;
    kaze_menu_render_rot_x = 0;
    kaze_menu_render_rot_z = 0;
    kaze_menu_render_rot_y = 0;
    kaze_menu_camera_trans_x = 0;
    kaze_menu_camera_trans_y = 0;
    kaze_menu_camera_trans_z = 0;
    kaze_title_shadow_line_setup();
}
