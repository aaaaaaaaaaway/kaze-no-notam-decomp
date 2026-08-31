#include "types.h"
#include "kaze_syms.h"
extern int kaze_burner_ramp;
extern int kaze_gas_floor;
extern int kaze_min_gas;
extern int kaze_target_gas;
extern int write(int, void *, int);
extern int kaze_pos_y;
typedef struct 
{
  short vx;
  short vy;
  short vz;
  short pad;
} SVECTOR;
typedef struct 
{
  int vx;
  int vy;
  int vz;
  int pad;
} VECTOR;
typedef struct 
{
  short m[3][3];
  short pad;
  int t[3];
} MATRIX;

extern int kaze_input_current_a;
extern int kaze_input_current_b;
extern int kaze_input_bind_descend;
extern void kaze_sfx_voice_keyon_34108(int, int);

/**
 * kaze_gas_floor_reset  (flight, 0x8001ff94)
 *
 * Resets kaze_gas_floor (kaze_gas_floor) to kaze_min_gas (kaze_min_gas).
 */

int kaze_gas_floor_reset(int lo, int hi)
{
    extern char *kaze_state_base_ptr;
    int overshot;
    int delta;
    int clamped;
    int new_floor;
    int *p;
    int cur;
    int res;
    register int tail_v1 asm("$3");
    register int tail_v0 asm("$2");
    int ret;
    register int gsnap asm("$4");
    register int tsnap asm("$3");
    int emask;

    emask = kaze_input_current_a & kaze_input_bind_descend;
    delta = -0x30;
    if (emask != 0) {
        delta = -0x140;
    }
    if (kaze_gas_floor < kaze_target_gas) {
        int step_a = *(int *)(kaze_state_base_ptr + 0xa4);
        register int ramp_a asm("$2");
        int quot_a = (kaze_target_gas - kaze_gas_floor) / step_a;
        if (quot_a <= kaze_burner_ramp) {
            ramp_a = quot_a / 2;
        } else {
            ramp_a = kaze_burner_ramp + step_a * 4;
        }
        kaze_burner_ramp = ramp_a;
        { int _mt = ramp_a; __asm__("" :: "r"(_mt) : "memory"); }
        p = &kaze_burner_ramp;
        cur = *p;
        if (delta <= cur) {
            if (cur < 0x101) {
                res = cur;
            } else {
                res = 0x100;
            }
        } else {
            res = delta;
        }
        __asm__("" : : "r"(res));
        *p = res;
        gsnap = kaze_gas_floor;
        {
            int snap_late;
            register int gas2 __asm__("$2") = kaze_burner_ramp;
            register int prod2 __asm__("$2");
            prod2 = gas2 * *(int *)(kaze_state_base_ptr + 0xa4);
            snap_late = kaze_target_gas;
            __asm__ volatile("" : "=r"(prod2) : "0"(prod2));
            delta = prod2;
            tsnap = snap_late;
        }
        overshot = tsnap < gsnap + delta;
    } else if (kaze_target_gas < kaze_gas_floor) {
        int step_b = *(int *)(kaze_state_base_ptr + 0xa4);
        int quot_b = (kaze_target_gas - kaze_gas_floor) / step_b;
        if (kaze_burner_ramp <= quot_b) {
            kaze_burner_ramp = quot_b / 2;
        } else {
            int ramp_b;
            __asm__("" : "=r"(emask) : "0"(emask));
            if (emask != 0) {
                do {
                    if ((kaze_input_current_b & kaze_input_bind_descend) == 0) {
                        kaze_sfx_voice_keyon_34108(0x17, 1);
                    }
                } while (0);
                kaze_burner_ramp = kaze_burner_ramp + *(int *)(kaze_state_base_ptr + 0xa4) * -4;
                goto clamp2;
            }
            ramp_b = kaze_burner_ramp - step_b;
            kaze_burner_ramp = ramp_b;
        }
clamp2:
        p = &kaze_burner_ramp;
        cur = *p;
        if (delta <= cur) {
            if (cur < 0x101) {
                res = cur;
            } else {
                res = 0x100;
            }
        } else {
            res = delta;
        }
        *p = res;
        gsnap = kaze_gas_floor;
        {
            int snap_late;
            register int gas2 __asm__("$2") = kaze_burner_ramp;
            register int prod2 __asm__("$2");
            prod2 = gas2 * *(int *)(kaze_state_base_ptr + 0xa4);
            snap_late = kaze_target_gas;
            __asm__ volatile("" : "=r"(prod2) : "0"(prod2));
            delta = prod2;
            tsnap = snap_late;
        }
        overshot = gsnap + delta < tsnap;
    } else {
        goto Lzero;
    }
    if (overshot) {
        delta = tsnap - gsnap;
    }
    goto LAB_80020234;
Lzero:
    kaze_burner_ramp = 0;
    delta = 0;
LAB_80020234:
    {
        register int *gp __asm__("$2");
        register int *storep __asm__("$5");
        gp = &kaze_gas_floor;
        __asm__("" : "=r"(gp) : "0"(gp));
        new_floor = *gp + delta;
        *gp = new_floor;
        __asm__ volatile("" ::: "memory");
        storep = gp;
        clamped = kaze_min_gas;
        if (clamped <= new_floor) {
            if (new_floor <= 0x64000) {
                clamped = new_floor;
            } else {
                clamped = 0x64000;
            }
        }
        *storep = clamped;
    }
    tail_v1 = (kaze_gas_floor - kaze_min_gas) + -0xf000;
    __asm__("" : "=r"(tail_v1) : "0"(tail_v1));
    if (tail_v1 < 0) {
        tail_v1 += 0xf;
    }
    tail_v0 = -kaze_pos_y - (tail_v1 >> 4);
    __asm__("" : "=r"(tail_v0) : "0"(tail_v0));
    tail_v1 = tail_v0 / 2;
    __asm__("" : "=r"(tail_v1) : "0"(tail_v1));
    if (tail_v1 < lo) {
        ret = lo;
    } else {
        ret = hi;
        if (!(ret < tail_v1)) {
            ret = tail_v1;
        }
    }
    return ret;
}

int kaze_terrain_zone_collision_query();

/**
 * kaze_terrain_collision_neighbor_query  (collision, 0x80020310)
 *
 * Terrain collision wrapper: converts world X/Z to 0xed8 tile coords, tests
 * the center tile via kaze_terrain_zone_collision_query, and near tile
 * edges/corners also tests adjacent tiles (0..0x7e), ORing direction masks
 * into *out.
 */

int kaze_terrain_collision_neighbor_query(int *p1, int radius, u32 *out)
{
    int ix;
    int iz;
    register u32 acc asm("$19");
    int hit;

    *out = 0;
    ix = p1[0] / 0xed8;
    iz = p1[2] / 0xed8;
    if (kaze_terrain_zone_collision_query(p1, ix, iz, radius, out) != 0) {
        return 1;
    }
    acc = 0;
    hit = 0;
    if (radius >= p1[0] - ix * 0xed8 && ix >= 2) {
        if (radius >= p1[2] - iz * 0xed8) {
            if (iz > 0) {
                if (kaze_terrain_zone_collision_query(p1, ix, iz - 1, radius, out) != 0) {
                    acc |= *out;
                    hit = 1;
                }
                if (kaze_terrain_zone_collision_query(p1, ix - 1, iz - 1, radius, out) != 0) {
                    acc |= *out;
                    hit = 1;
                }
                if (hit != 0) goto write;
            }
            return kaze_terrain_zone_collision_query(p1, ix - 1, iz, radius, out);
        }
        if (p1[2] - iz * 0xed8 + radius >= 0xed8) {
            if (iz < 0x7f) {
                if (kaze_terrain_zone_collision_query(p1, ix, iz + 1, radius, out) != 0) {
                    acc |= *out;
                    hit = 1;
                }
                if (kaze_terrain_zone_collision_query(p1, ix - 1, iz + 1, radius, out) != 0) {
                    acc |= *out;
                    hit = 1;
                }
                if (hit != 0) goto write;
            }
            return kaze_terrain_zone_collision_query(p1, ix - 1, iz, radius, out);
        }
        return kaze_terrain_zone_collision_query(p1, ix - 1, iz, radius, out);
    }
    if (p1[0] - ix * 0xed8 + radius >= 0xed8 && ix < 0x7f) {
        if (radius >= p1[2] - iz * 0xed8) {
            if (iz > 0) {
                if (kaze_terrain_zone_collision_query(p1, ix, iz - 1, radius, out) != 0) {
                    acc |= *out;
                    hit = 1;
                }
                if (kaze_terrain_zone_collision_query(p1, ix + 1, iz - 1, radius, out) != 0) {
                    acc |= *out;
                    hit = 1;
                }
                if (hit != 0) goto write;
            }
            return kaze_terrain_zone_collision_query(p1, ix + 1, iz, radius, out);
        }
        if (p1[2] - iz * 0xed8 + radius >= 0xed8) {
            if (iz < 0x7f) {
                if (kaze_terrain_zone_collision_query(p1, ix, iz + 1, radius, out) != 0) {
                    acc |= *out;
                    hit = 1;
                }
                if (kaze_terrain_zone_collision_query(p1, ix + 1, iz + 1, radius, out) != 0) {
                    acc |= *out;
                    hit = 1;
                }
                if (hit != 0) {
write:
                    *out = acc;
                    return 1;
                }
            }
            return kaze_terrain_zone_collision_query(p1, ix + 1, iz, radius, out);
        }
        return kaze_terrain_zone_collision_query(p1, ix + 1, iz, radius, out);
    }
    if (radius >= p1[2] - iz * 0xed8 && iz >= 2) {
        return kaze_terrain_zone_collision_query(p1, ix, iz - 1, radius, out);
    }
    if (p1[2] - iz * 0xed8 + radius < 0xed8) {
        return 0;
    }
    if (iz >= 0x7f) {
        return 0;
    }
    return kaze_terrain_zone_collision_query(p1, ix, iz + 1, radius, out);
}

long kaze_collision_shape_test();

/**
 * kaze_terrain_zone_collision_query  (collision, 0x80020788)
 *
 * Terrain/zone collision query: converts tile grid coords to world
 * (*0xed8+0x76c), resolves remapped zone ids through +0xe1e60 using the
 * +0xecaf8 page, tests +0xe36e0 shapes via kaze_collision_shape_test with
 * the +0x5ae8c fallback; returns 1 = shape hit (direction mask) or 2 = below
 * terrain (mask 4).
 */

int kaze_terrain_zone_collision_query(int obj, int tile_x, int tile_z, int height_pad, u32 *out)
{
    extern char *kaze_state_base_ptr;
    register u32 tile asm("$19");
    u32 zone_raw;
    register u32 zone asm("$17");
    register int t_v0 asm("$2");
    register int hit asm("$22") = 0;
    register int i asm("$18");
    int world[3];
    volatile int frame_pad[2];
    register char *shape asm("$16");
    register int flags asm("$20");
    register int remap_base asm("$4");
    register int shape_base asm("$5");
    char *root;
    register char *root2 asm("$6");
    char *entry;
    int shape_index;
    int alt;
    register u32 *out_r asm("$21") = out;

    asm("" : "=r"(hit) : "0"(hit));
    *out_r = 0;
    root = kaze_state_base_ptr;
    (void)frame_pad;

    tile = *(u32 *)(root + 0x5b6e0 + ((tile_z * 0x80 + tile_x) * 4));
    world[0] = tile_x * 0xed8 + 0x76c;
    world[2] = tile_z * 0xed8 + 0x76c;
    alt = (tile >> 28) * 0x76c;
    world[1] = alt;

    if ((tile & 0x0c000000) == 0) {
        goto direct_zone;
    }
    remap_base = 0xe1e60;
    zone_raw = *(u32 *)(root + ((tile & 0x1f00) >> 1) + remap_base +
                        *(int *)(root + 0xecaf8) * 4);
    goto zone_done;
direct_zone:
    zone_raw = (tile & 0x1ffff) >> 8;
zone_done:
    zone = zone_raw - 1;
    asm("" : "=r"(zone) : "0"(zone));

    if (zone < 0x200U && alt < *(int *)(obj + 4) - height_pad) {
        t_v0 = (int)zone * 4;
        root2 = kaze_state_base_ptr;
        entry = root2 + t_v0;
        t_v0 = *(s16 *)(entry + 0xe2ee0);
        if (t_v0 > 0) {
            shape_base = 0xe36e0;
            zone = t_v0;
            asm("" : "=r"(zone) : "0"(zone));
            i = 0;
            remap_base = tile >> 14;
            shape_index = *(s16 *)(entry + 0xe2ee2);
            tile = 0;
            flags = remap_base & 0xc00;
            t_v0 = shape_index * 0xc + shape_base;
            shape = root2 + t_v0;
            if (0 < (int)zone) {
                do {
                    if (height_pad >= kaze_collision_shape_test(shape, world, flags, obj, out_r)) {
                        hit = 1;
                        tile |= *out_r;
                    }
                    i++;
                    shape += 0xc;
                } while (i < (int)zone);
            }
            *out_r = tile;
        }
        if (hit != 0) {
            return hit;
        }
    }

    *out_r = 0;
    if (height_pad >= kaze_collision_shape_test(kaze_state_base_ptr + 0x5ae8c, world, 0, obj, out_r)) {
        return 1;
    }
    if (*(int *)(obj + 4) <= world[1]) {
        *out_r = 4;
        hit = 2;
    }
    return hit;
}

extern int rcos(int);
extern int rsin(int);
extern int SquareRoot0(int);

/**
 * kaze_collision_shape_test  (collision, 0x80020a10)
 *
 * Tests one 12-byte collision shape record: negative X half-extent selects a
 * sphere distance test, otherwise a yaw-rotated box with per-axis
 * penetration in GTE scratchpad; result mask encodes hit direction.
 */

long kaze_collision_shape_test(short *shape, int *world, int angle, int *obj, uint *out)
{
    int dist_sq;
    long dist;
    int *sp = (int *)0x1f800000;

    *out = 0;
    if (shape[3] < 0) {
        {
            int c = rcos(angle);
            int sn = rsin(angle);
            int d = c * *shape - sn * shape[2];
            if (d < 0) {
                d = d + 0xfff;
            }
            sp[0] = (*world + (d >> 0xc)) - *obj;
        }
        {
            int sn = rsin(angle);
            int c = rcos(angle);
            int d = sn * *shape + c * shape[2];
            if (d < 0) {
                d = d + 0xfff;
            }
            sp[2] = (world[2] + (d >> 0xc)) - obj[2];
        }
        sp[1] = (world[1] + (int)shape[1]) - obj[1];
        if (sp[0] < 1) {
            *out |= 0x10;
        } else {
            *out |= 1;
        }
        if (sp[1] < 1) {
            *out |= 4;
        } else {
            *out |= 0x40;
        }
        if (sp[2] < 1) {
            *out |= 0x20;
        } else {
            *out |= 2;
        }
        dist_sq = sp[0] * sp[0];
        dist_sq += sp[1] * sp[1];
        dist_sq += sp[2] * sp[2];
        dist = SquareRoot0(dist_sq);
        return dist + shape[3];
    }
    {
        int c = rcos(angle);
        int sn = rsin(angle);
        int d = c * *shape - sn * shape[2];
        if (d < 0) {
            d = d + 0xfff;
        }
        sp[0] = *world + (d >> 0xc);
    }
    sp[1] = world[1] + (int)shape[1];
    {
        int sn = rsin(angle);
        int c = rcos(angle);
        int d = sn * *shape + c * shape[2];
        if (d < 0) {
            d = d + 0xfff;
        }
        sp[2] = world[2] + (d >> 0xc);
    }
    sp[7] = (int)shape[4];
    if ((angle == 0x400) || (angle == 0xc00)) {
        sp[6] = (int)shape[5];
        sp[8] = (int)shape[3];
    } else {
        sp[6] = (int)shape[3];
        sp[8] = (int)shape[5];
    }
    if (*obj <= sp[0] - sp[6]) {
        sp[9] = 0;
        sp[3] = (sp[0] - sp[6]) - *obj;
    } else {
        if (*obj >= sp[0] + sp[6]) {
            sp[9] = 4;
            sp[3] = *obj - (sp[0] + sp[6]);
        } else {
            sp[9] = 4;
            sp[3] = 0;
        }
    }
    if (obj[1] <= sp[1] - sp[7]) {
        sp[10] = 4;
        sp[4] = (sp[1] - sp[7]) - obj[1];
    } else {
        if (obj[1] >= sp[1] + sp[7]) {
            sp[10] = 0;
            sp[4] = obj[1] - (sp[1] + sp[7]);
        } else {
            sp[10] = 0;
            sp[4] = 0;
        }
    }
    if (obj[2] <= sp[2] - sp[8]) {
        sp[11] = 0;
        sp[5] = (sp[2] - sp[8]) - obj[2];
    } else {
        if (obj[2] >= sp[2] + sp[8]) {
            sp[11] = 4;
            sp[5] = obj[2] - (sp[2] + sp[8]);
        } else {
            sp[11] = 4;
            sp[5] = 0;
        }
    }
    if (sp[3] == 0 && sp[4] == 0 && sp[5] == 0) {
        *out = 0x77;
    } else if (sp[3] != 0 && sp[4] == 0 && sp[5] == 0) {
        *out = 1 << sp[9];
    } else if (sp[3] == 0 && sp[4] != 0 && sp[5] == 0) {
        *out = 4 << sp[10];
    } else if (sp[3] != 0 && sp[4] != 0 && sp[5] == 0) {
        if (sp[4] <= sp[3]) {
            *out |= 1 << sp[9];
        }
        if (!(sp[4] < sp[3])) {
            *out |= 4 << sp[10];
        }
    } else if (sp[3] == 0 && sp[4] == 0 && sp[5] != 0) {
        *out = 2 << sp[11];
    } else if (sp[3] != 0 && sp[4] == 0 && sp[5] != 0) {
        if (sp[5] <= sp[3]) {
            *out |= 1 << sp[9];
        }
        if (!(sp[5] < sp[3])) {
            *out |= 2 << sp[11];
        }
    } else if (sp[3] == 0 && sp[4] != 0 && sp[5] != 0) {
        if (sp[5] <= sp[4]) {
            *out |= 4 << sp[10];
        }
        if (!(sp[5] < sp[4])) {
            *out |= 2 << sp[11];
        }
    } else if (sp[3] != 0 && sp[4] != 0 && sp[5] != 0) {
        if (sp[4] <= sp[3] && sp[5] <= sp[3]) {
            *out |= 1 << sp[9];
        }
        if (!(sp[4] < sp[3]) && sp[5] <= sp[4]) {
            *out |= 4 << sp[10];
        }
        if (!(sp[5] < sp[3]) && sp[4] <= sp[5]) {
            *out |= 2 << sp[11];
        }
    }
    dist_sq = sp[3] * sp[3];
    dist_sq += sp[4] * sp[4];
    dist_sq += sp[5] * sp[5];
    dist = SquareRoot0(dist_sq);
    return dist;
}

extern void PushMatrix(void);
extern void PopMatrix(void);
extern void RotMatrixZYX(SVECTOR *, MATRIX *);
extern void ApplyMatrixLV(MATRIX *, VECTOR *, VECTOR *);
int kaze_object_shape_test();

/**
 * kaze_dynamic_object_collision_query  (collision, 0x800211e8)
 *
 * Dynamic-object collision query over the 18-slot +0x5901c table: coarse
 * 0x2c89 axis cull, rotates the delta into object space, then tests the
 * object's +0xe36e0 shape run via kaze_object_shape_test.
 */

int kaze_dynamic_object_collision_query(int *pos, int radius, u32 *out)
{
    extern int kaze_state_base_ptr;
  int test_result;
  int shape_idx_raw;
  register int shape_idx asm("$17");
  register int count asm("$17");
  int shape;
  int _sc;
  int _gb;
  int shape_test_i;
  register short *obj_ptr asm("$19");
  register int slot_i asm("$20");
 int * p1;
  int * active_ptr;
  register u32 *p3 asm("$23") = out;
  VECTOR delta;
  SVECTOR inv_rot;
  MATRIX mtx;
  volatile int frame_pad[2];
  (void) frame_pad;
  active_ptr = (int *) (kaze_state_base_ptr + 0x5901c);
  obj_ptr = (short *) (((char *) active_ptr) + 0x2c);
  slot_i = 0;
  *p3 = 0;
  inv_rot.vx = 0;
  inv_rot.vy = 0;
  inv_rot.vz = 0;
  do
  {
    if ((*active_ptr) != 0)
    {
      shape_idx_raw = (int) obj_ptr[0xb];
      shape_idx = shape_idx_raw;
      if (shape_idx_raw < 0)
      {
        register int remap_base asm("$4") = 0xe1e60;
        { int _lhs = (kaze_state_base_ptr + (shape_idx * (-0x80))) + remap_base;
          register int _t asm("$2") = *((int *) (_lhs + ((*((int *) (kaze_state_base_ptr + 0xecaf8))) * 4)));
          __asm__("":"=r"(_t):"0"(_t));
          shape_idx = _t + (-1); }
      }
      _gb = kaze_state_base_ptr + (shape_idx * 4);
      if ((*((short *) (_gb + 0xe2ee0))) != 0)
      {
        register int absx asm("$3");
        p1 = pos;
        { int _px = *p1;
          delta.vx = (*((int *) (obj_ptr + (-0xe)))) - _px; }
        absx = delta.vx;
        { int _mt = absx; __asm__("" :: "r"(_mt) : "memory"); }
        if (absx < 0)
        {
          absx = -absx;
        }
        delta.vy = (-(p1[1])) - (*((int *) (obj_ptr + (-0xc))));
        delta.vz = p1[2] - (*((int *) (obj_ptr + (-10))));
        if (absx < 0x2c89)
        {
          register int absv asm("$2");
          absv = delta.vy;
          if (delta.vy < 0)
          {
            absv = -absv;
          }
          if (absv < 0x2c89)
          {
            absv = delta.vz;
            if (delta.vz < 0)
            {
              absv = -absv;
            }
            if (absv < 0x2c89)
            {
              shape_test_i = 0;
              PushMatrix();
              inv_rot.vx = 0x1000 - obj_ptr[-2];
              inv_rot.vy = 0x1000 - obj_ptr[-1];
              inv_rot.vz = 0x1000 - (*obj_ptr);
              __asm__("":"=r"(obj_ptr):"0"(obj_ptr));
              RotMatrixZYX(&inv_rot, &mtx);
              ApplyMatrixLV(&mtx, &delta, &delta);
              PopMatrix();
              delta.vy = -delta.vy;
              delta.vz = -delta.vz;
              {
                int base = kaze_state_base_ptr + (shape_idx * 4);
                count = (int) (*((short *) (base + 0xe2ee0)));
                _sc = ((*((short *) (base + 0xe2ee2))) * 0xc) + 0xe36e0;
                shape = kaze_state_base_ptr + _sc;
              }
              if (0 < count)
              {
                do
                {
                  test_result = kaze_object_shape_test((void *) shape, &delta, p3);
                  { int _mt = test_result; __asm__("" :: "r"(_mt) : "memory"); }
                  shape_test_i = shape_test_i + 1;
                  if (test_result <= radius)
                  {
                    return slot_i;
                  }
                  shape = shape + 0xc;
                }
                while (shape_test_i < count);
              }
            }
          }
        }
      }
    }
    slot_i = slot_i + 1;
    obj_ptr = obj_ptr + 0x3c;
    active_ptr = active_ptr + 0x1e;
  }
  while (slot_i < 0x12);
  return -1;
}

/**
 * kaze_object_shape_test  (collision, 0x80021480)
 *
 * Per-shape test used by the dynamic-object collision query (object-space
 * variant of kaze_collision_shape_test).
 */

int kaze_object_shape_test(short *obj, int *pos, int *out)
{
    int qx, qy, qz;
    int rx, dx2, ry, rz;
    int p1x, p2x, dxlow;
    int n2, n3;

    *out = 0x77;
    rx = obj[3];
    dx2 = rx << 1;
    if (rx < 0) {
        qx = obj[0] - pos[0];
        qy = obj[2] - pos[2];
        qz = obj[1] - pos[1];
        return SquareRoot0(qx * qx + qy * qy + qz * qz) + obj[3];
    }
    p1x = obj[0];
    p2x = pos[0];
    dxlow = p2x - (p1x - rx);
    __asm__("" :: "r"(rx));
    {
        int chain1;
        chain1 = dxlow << 1;
        qx = chain1 * rx / dx2;
    }
    ry = obj[4];
    {
        int chain2;
        chain2 = pos[1] - (obj[1] - ry) << 1;
        qy = chain2 * ry / (ry << 1);
    }
    __asm__("" : "=r"(qy) : "0"(qy));
    __asm__("" :: "r"(qy));
    rz = obj[5];
    {
        int chain3;
        chain3 = pos[2] - (obj[2] - rz) << 1;
        qz = chain3 * rz / (rz << 1);
    }
    __asm__("" :: "r"(qz));
    __asm__("" :: "r"(dx2));
    {
        int t;
        if (qx < 0) {
            t = dxlow;
        } else if (dx2 < qx) {
            t = p2x - (p1x + rx);
        } else {
            t = 0;
        }
        qx = t;
    }
    {
        int t;
        if (qy < 0) {
            t = obj[1] - ry;
            t = pos[1] - t;
        } else if (ry << 1 < qy) {
            t = obj[1] + ry;
            t = pos[1] - t;
        } else {
            t = 0;
        }
        qy = t;
    }
    qz = (qz < 0) ? (pos[2] - (obj[2] - rz)) : (rz << 1 < qz) ? (pos[2] - (obj[2] + rz)) : 0;
    return SquareRoot0(qx * qx + qy * qy + qz * qz);
}

/**
 * kaze_mode_object_proximity_test  (collision, 0x800216f4)
 *
 * Proximity test against the six +0x58e14 mode-object slots (skips the given
 * slot index, flags 0x84000000): 3D distance with -700 Y bias; under the
 * given vertical threshold plus 700 it sets direction bits (1/0x10, 4/0x40,
 * 2/0x20), returns slot or -1.
 */

int kaze_mode_object_proximity_test(int *pos, int radius_arg, u32 *out_arg, int limit_arg)
{
    extern char *kaze_state_base_ptr;
    register int radius asm("$23") = radius_arg;
    register u32 *out asm("$16") = out_arg;
    register int dx asm("$17");
    register int temp asm("$2");
    register int slot_base asm("$6");
    int dz;
    int dy;
    int offset;
    int i;
    u32 bits;
    int distance;
    int limit;

    limit = limit_arg;
    i = 0;
    offset = 0;
    *out = 0;
    do {
        if (i != limit) {
            temp = (int)kaze_state_base_ptr;
            slot_base = temp + offset;
            if ((*(u32 *)(slot_base + 0x58e58) & 0x84000000) == 0) {
                register int px asm("$4");
                register int py asm("$3");
                register int pz asm("$5");
                register int objx asm("$2");
                register int objy asm("$4");
                register int objz asm("$2");

                px = pos[0];
                objx = *(int *)(slot_base + 0x58e14);
                py = pos[1];
                pz = pos[2];
                dx = px - objx;
                asm("" : "=r"(dx) : "0"(dx));
                py = py - 700;
                objy = *(int *)(slot_base + 0x58e18);
                asm("" : "=r"(objy) : "0"(objy));
                objz = *(int *)(slot_base + 0x58e1c);
                dz = pz - objz;
                temp = dx;
                if (dx < 0) {
                    temp = -temp;
                }
                if (temp < 0x76c) {
                    dy = py + objy;
                    temp = dy;
                    if (dy < 0) {
                        temp = -temp;
                    }
                    if (temp < 0x76c) {
                        temp = dz;
                        if (dz < 0) {
                            temp = -temp;
                        }
                        if (temp < 0x76c) {
                            distance = SquareRoot0(dx * dx + dy * dy + dz * dz);
                            if (distance < radius + 700) {
                                if (dx < 0) {
                                    bits = *out | 1;
                                } else {
                                    bits = *out | 0x10;
                                }
                                *out = bits;
                                if (dy < 0) {
                                    bits = *(volatile u32 *)out | 0x40;
                                } else {
                                    bits = *(volatile u32 *)out | 4;
                                }
                                *out = bits;
                                if (dz < 0) {
                                    bits = *(volatile u32 *)out | 2;
                                } else {
                                    bits = *(volatile u32 *)out | 0x20;
                                }
                                *out = bits;
                                return i;
                            }
                        }
                    }
                }
            }
        }
        i = i + 1;
        offset = offset + 0x58;
    } while (i < 6);
    return -1;
}
