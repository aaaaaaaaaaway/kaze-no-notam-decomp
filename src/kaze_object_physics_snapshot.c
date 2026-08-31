/**
 * kaze_object_physics_snapshot  (flight, 0x8001f328)
 *
 * Consumes per-frame deltas (0xb16e8..0xb16f0) and maintains previous-
 * position/route-offset snapshots at live-root +0x78..+0x90.
 */

#include "types.h"
#include "kaze_syms.h"
extern int kaze_balloon_render_rot0;
extern int kaze_pos_y;
extern int kaze_approach_value(int, int, int);
extern int kaze_stage_score_compute(int);
struct rot8
{
  char b[8];
};
struct blk16
{
  int w[4];
};
struct kphys
{
  int x, y, z, pad0;
  int vx, vy, vz, pad1;
  int pad2, pad3, pad4, pad5;
  int ox, oy, oz, pad6;
  struct rot8 rot0;
  struct rot8 rot2;
};
struct kroot
{
  int f00, f04, f08;
  int score;
  int record;
  uint flags;
  int f18, f1c, f20, f24, f28, f2c, f30, f34, f38, f3c, f40, f44, f48, f4c;
  int c50, f54, c58, f5c;
  int idx, f64;
  int x68, x6c, x70, f74;
  int p78, p7c, p80, f84;
  int w88, f8c, w90, f94;
  int f98, f9c, fa0, fa4;
};
extern struct kphys kaze_pos_x;
extern struct kroot *kaze_state_base_ptr;
extern int kaze_pos_z;
extern int kaze_impact_flash_timer;
extern int kaze_wind_active_angle;
extern int kaze_camera_anchor_target_x[];
extern int kaze_camera_anchor_target_z[];
extern int kaze_stage_index;
extern int kaze_stage_mode;
extern int kaze_stage_variant;
extern void kaze_play_sound_effect(int, int, int, int);
extern void kaze_trail_particle_spawn(void *);
extern int kaze_record_table_insert(uint, int, int);
extern int rsin(int);
extern int rcos(int);
extern int SquareRoot0(int);
void kaze_object_physics_snapshot(void)
{
  int cand_pos[3];
  int hit_result[2];
  int *p;
  int *q;
  int *rz;
  int *ry;
  int *xp;
  register int u __asm__("$5");
  register int t __asm__("$3");
  register int u5 __asm__("$6");
  register int t5 __asm__("$3");
  int u3, t3, u4, t4,  u6, t6, u7, t7, u8, t8, u9, t9;
  struct kroot *o1, *o2, *o3, *o4, *o5, *o6, *o7;
  int i, amp, amp3, wx, wz, sz, r2, mw, tmpw, i2;
  register int qa __asm__("$4");
  register int sh __asm__("$2");
  register int q2 __asm__("$9");
  register int c2 __asm__("$3");
  register int mw2 __asm__("$8");
  register int wx2 __asm__("$2");
  register int w1 __asm__("$4");
  register int w2 __asm__("$5");
  register int w3 __asm__("$6");
  register int sx __asm__("$16");
  register int m __asm__("$2");
  register int a7c __asm__("$4");
  register int coef __asm__("$17");
  register int dx __asm__("$16");
  register int dz __asm__("$18");
  int d, dt1, dt2, zl, zr;
  uint n;
  int s;
  int *pd8, *pe0, *pdc;
  register volatile int *wp __asm__("$16");
  register int *fp __asm__("$8");

  p = (int *)&kaze_pos_x;
  p[0] = p[0] - kaze_pos_x.vx;
  kaze_pos_x.y = kaze_pos_x.y + kaze_pos_x.vy;
  kaze_pos_x.z = kaze_pos_x.z - kaze_pos_x.vz;
  *(struct blk16 *)(p + 12) = *(struct blk16 *)p;

  q = p + 12;
  u = *q;
  if (u >= 0)
  {
    if (u < 0x76c00) t = u;
    else t = 0x76bff;
  }
  else t = 0;
  *q = t;

  rz = &kaze_pos_x.oz;
  u = *rz;
  if (u >= 0)
  {
    if (u < 0x76c00) t = u;
    else t = 0x76bff;
  }
  else t = 0;
  *rz = t;

  ry = &kaze_pos_x.oy;
  u3 = *ry;
  if (u3 >= -0x3a02)
  {
    t3 = u3;
    if (t3 > 0) t3 = 0;
  }
  else t3 = -0x3a02;
  *ry = t3;

  kaze_pos_x.rot2 = *(struct rot8 *)&kaze_balloon_render_rot0;
  xp = (int *)((char *)&kaze_pos_x.rot2 - 0x18);

  kaze_pos_x.oy = kaze_pos_x.oy + 0x73;
  cand_pos[0] = *xp;
  cand_pos[1] = -kaze_pos_x.oy;
  cand_pos[2] = kaze_pos_x.oz;
  if (kaze_terrain_collision_neighbor_query(cand_pos, 0x32, hit_result) != 0)
  {
    kaze_landing_goal_handler(hit_result[0], 0);
    kaze_trail_particle_spawn(xp);
  }
  else
  {
    cand_pos[0] = kaze_pos_x.x;
    cand_pos[1] = -kaze_pos_x.y + 300;
    cand_pos[2] = kaze_pos_x.z;
    if (kaze_terrain_collision_neighbor_query(cand_pos, 300, hit_result) != 0)
    {
      kaze_impact_flash_timer = 0x4000;
      kaze_play_sound_effect(8, 100, 1, 1);
      kaze_vertical_control_update(hit_result[0], 0);
    }
  }

  o1 = kaze_state_base_ptr;
  w1 = o1->x68;
  o1->p78 = w1;
  w2 = o1->x6c;
  o1->p7c = w2;
  i = o1->idx;
  w3 = o1->x70;
  o1->p80 = w3;
  __asm__("" : "=r"(fp) : "0"(fp));
  fp = (int *)kaze_camera_anchor_target_z;
  amp = fp[i * 8];
  if (amp < 0) amp += 7;
  wp = &kaze_wind_active_angle;
  amp3 = amp >> 3;
  mw2 = rsin(*wp) * amp3;
  wx2 = -mw2;
  if (wx2 < 0) wx2 += 0xfff;
  tmpw = *wp;
  sx = wx2 >> 12;
  wz = rcos(tmpw) * amp3;
  if (wz < 0) wz += 0xfff;
  sz = wz >> 12;

  o2 = kaze_state_base_ptr;
  i2 = o2->idx;
  if (o2->c50 != kaze_camera_anchor_target_x[i2 * 8] || o2->c58 != kaze_camera_anchor_target_z[i2 * 8])
  {
    c2 = o2->fa4;
    sh = c2 << 3;
  }
  else
  {
    c2 = o2->fa4;
    sh = c2 << 1;
  }
  coef = sh + c2;
  __asm__("" : "=r"(coef) : "0"(coef));

  r2 = kaze_approach_value(kaze_state_base_ptr->w88, sx, coef);
  kaze_state_base_ptr->w88 = r2;
  r2 = kaze_approach_value(kaze_state_base_ptr->w90, sz, coef);
  o3 = kaze_state_base_ptr;
  o3->w90 = r2;
  o3->x68 = kaze_pos_x.x + o3->w88;
  o3->x6c = kaze_pos_x.y;
  o3->x70 = kaze_pos_x.z + o3->w90;

  pd8 = (int *)&kaze_pos_x;
  u4 = kaze_pos_x.x;
  if (u4 >= 0)
  {
    if (u4 < 0x76c00) t4 = u4;
    else t4 = 0x76bff;
  }
  else t4 = 0;
  *pd8 = t4;

  pe0 = &kaze_pos_z;
  u5 = *pe0;
  if (u5 >= 0)
  {
    if (u5 < 0x76c00) t5 = u5;
    else t5 = 0x76bff;
  }
  else t5 = 0;
  *pe0 = t5;

  pdc = &kaze_pos_y;
  u6 = *pdc;
  if (u6 >= -0x3a02)
  {
    t6 = u6;
    if (t6 > 0) t6 = 0;
  }
  else t6 = -0x3a02;
  *pdc = t6;

  o4 = kaze_state_base_ptr;
  u7 = o4->x68;
  if (u7 < 0x7e2c) t7 = 0x7e2c;
  else if (u7 < 0x6edd5) t7 = u7;
  else t7 = 0x6edd4;
  o4->x68 = t7;

  o5 = kaze_state_base_ptr;
  u8 = o5->x70;
  if (u8 < 0x7e2c) t8 = 0x7e2c;
  else if (u8 < 0x6edd5) t8 = u8;
  else t8 = 0x6edd4;
  o5->x70 = t8;

  o6 = kaze_state_base_ptr;
  u9 = o6->x6c;
  if (u9 >= -0x3a02)
  {
    if (u9 < -0x1643) t9 = u9;
    else t9 = -0x1644;
  }
  else t9 = -0x3a02;
  o6->x6c = t9;

  if ((uint)(kaze_pos_x.x - 0x7e2c) > 0x66fa8 || (uint)(kaze_pos_x.z - 0x7e2c) > 0x66fa8)
  {
    kaze_state_base_ptr->flags |= 0x10000;
  }

  if (kaze_pos_x.y >= 0 && (kaze_state_base_ptr->flags & 0xc00000) == 0)
  {
    m = kaze_stage_index;
    a7c = kaze_stage_variant;
    m = m + 1;
    n = m & (-((uint)(m < 3)));
    if (a7c == 0 && kaze_stage_mode == 1)
    {
      s = kaze_stage_score_compute(1);
      kaze_state_base_ptr->score = s;
      if (s >= 0)
      {
        kaze_success_terminal_handler();
        s = kaze_record_table_insert(n, 0, kaze_state_base_ptr->score);
        kaze_state_base_ptr->record = s;
        goto Ldist;
      }
    }
    kaze_failure_terminal_handler();
  }
Ldist:
  o7 = kaze_state_base_ptr;
  dt1 = o7->x68 - kaze_pos_x.x;
  zl = o7->x70;
  zr = kaze_pos_x.z;
  dx = dt1;
  if (dt1 < 0) dx = -dx;
  dt2 = zl - zr;
  dz = dt2;
  if (dt2 < 0) dz = -dz;
  if (dx >= 0xfb || dz >= 0xfb || o7->x6c != kaze_pos_x.y)
  {
    kaze_state_base_ptr->flags |= 0x8000;
    qa = dx * dx;
    q2 = dz * dz;
    d = SquareRoot0(qa + q2);
    if (d >= 0x76c1 && (kaze_state_base_ptr->flags & 0xc00000) == 0)
    {
      m = kaze_stage_index;
      a7c = kaze_stage_variant;
      m = m + 1;
      n = m & (-((uint)(m < 3)));
      if (a7c == 0 && kaze_stage_mode == 1)
      {
        s = kaze_stage_score_compute(1);
        kaze_state_base_ptr->score = s;
        if (s >= 0)
        {
          kaze_success_terminal_handler();
          s = kaze_record_table_insert(n, 0, kaze_state_base_ptr->score);
          kaze_state_base_ptr->record = s;
          return;
        }
      }
      kaze_failure_terminal_handler();
    }
  }
  return;
}
