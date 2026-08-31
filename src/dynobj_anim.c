#include "types.h"
#include "kaze_syms.h"
extern int abs(int);
extern int rand(void);
typedef struct 
{
  int f00;
  int f04;
  int f08;
  int f0c;
  int f10;
  int f14;
  int f18;
  int f1c;
  int f20;
  s16 h24;
  s16 h26;
  s16 h28;
  s16 h2a;
  s16 h2c;
  s16 h2e;
  int f30;
  int f34;
  int f38;
  int f3c;
  int f40;
  int f44;
  int f48;
  int f4c;
  int f50;
  int f54;
} KSlotA;
typedef struct 
{
  int g0;
  int f00;
  int f04;
  int f08;
  int f0c;
  int f10;
  int f14;
  int f18;
  int f1c;
  int f20;
  s16 h24;
  s16 h26;
  s16 h28;
  s16 h2a;
  s16 h2c;
  s16 h2e;
  int f30;
  int f34;
  int f38;
  int f3c;
  int f40;
  int f44;
  int f48;
  int f4c;
  int f50;
} KSlotB;
typedef struct {
    u32 active;     /* +0x00 */
    u32 onscreen;   /* +0x04 */
    int x;          /* +0x08 */
    int y;          /* +0x0c */
    int z;          /* +0x10 */
    int pad14;      /* +0x14 */
    s16 rel_x;      /* +0x18 */
    s16 rel_y;      /* +0x1a */
    s16 rel_z;      /* +0x1c */
    s16 pad1e;      /* +0x1e */
    s16 ang20;      /* +0x20 */
    s16 ang22;      /* +0x22 */
    s16 pad24;      /* +0x24 */
    s16 pad26;      /* +0x26 */
    s16 pad28;      /* +0x28 */
    s16 swing2a;    /* +0x2a */
    s16 heading2c;  /* +0x2c */
    s16 pad2e;      /* +0x2e */
    int speed;      /* +0x30 */
    int f34;        /* +0x34 */
    int idx38;      /* +0x38 */
    int off3c;      /* +0x3c */
    int off40;      /* +0x40 */
    int off44;      /* +0x44 */
    int pad48;      /* +0x48 */
    int pad4c;      /* +0x4c */
    u32 timer50;    /* +0x50 */
    int pad54;      /* +0x54 */
    int pad58;      /* +0x58 */
} MarkerSlot;
typedef struct {
    int pad[4];
    int px;
    int py;
    int pz;
    int rest[23];
} DynSlot;
typedef struct {
    char pad[0x5901c];
    DynSlot d[18];
} KRootDyn;

extern int SquareRoot0(int);
extern int rsin(int);
extern int rcos(int);

/**
 * kaze_dynobj_patrol_shuttle  (objective, 0x80027168)
 *
 * Dynamic-object two-point shuttle: moves between node positions at
 * +0x54/+0x5c (toggle +0x70), flips heading +0x800 and waits (timer +0x48)
 * at each end, integrates by rsin/rcos of +0x4c at speed +0x6c, turns yaw
 * +0x2a.
 */

void kaze_dynobj_patrol_shuttle(int p)
{
    extern int kaze_state_base_ptr;
    int t; int tb; int tf; int tq; int dist; int t2; int dx; int dz;
    if (*(int *)(p + 0x48) != 0) {
        t = *(int *)(p + 0x48) - *(int *)(kaze_state_base_ptr + 0xA4);
        *(int *)(p + 0x48) = t;
        if (t < 0) { *(int *)(p + 0x48) = 0; }
    } else {
        tb = p + *(int *)(p + 0x70) * 0xc;
        dx = *(int *)(p + 0x10) - *(int *)(tb + 0x54);
        dz = *(int *)(p + 0x18) - *(int *)(tb + 0x5c);
        tb = abs(dx);
        if (tb < 0x4000) {
            tb = abs(dz);
            if (tb >= 0x4000) goto far_step;
            {
                register int dxs asm("$2");
                register int dzs asm("$3");
                dxs = dx * dx;
                dzs = dz * dz;
                dist = SquareRoot0(dxs + dzs);
            }
        } else {
far_step:
            dist = *(int *)(p + 0x6c) * *(int *)(kaze_state_base_ptr + 0xA4);
        }
        {
        register int s2 asm("$7");
        register int cmp asm("$2");
        {
            register int a1r asm("$3");
            register int a2r asm("$2");
            a1r = *(int *)(p + 0x6c);
            a2r = *(int *)(kaze_state_base_ptr + 0xA4);
            s2 = a1r * a2r;
        }
        cmp = dist < s2;
        if (cmp) {
            tq = *(int *)(p + 0x70) * 0xc;
            *(int *)(p + 0x10) = *(int *)(p + tq + 0x54);
            tq = *(int *)(p + 0x70) * 0xc;
            dz = *(int *)(p + tq + 0x5c);
            *(uint *)(p + 0x4c) = (*(int *)(p + 0x4c) + 0x800U) & 0xfff;
            *(uint *)(p + 0x70) = *(uint *)(p + 0x70) ^ 1;
            *(int *)(p + 0x18) = dz;
            *(int *)(p + 0x48) = *(int *)(p + 0x50);
        } else {
            {
                register int rtmp asm("$3");
                rtmp = rsin(*(int *)(p + 0x4c)) * *(int *)(p + 0x6c);
                *(int *)(p + 0x10) = *(int *)(p + 0x10) - rtmp * *(int *)(kaze_state_base_ptr + 0xA4) / 0x1000;
            }
            {
                register int rc asm("$3");
                register int m18 asm("$2");
                rc = rcos(*(int *)(p + 0x4c)) * *(int *)(p + 0x6c);
                rc = rc * *(int *)(kaze_state_base_ptr + 0xA4) / 0x1000;
                m18 = *(int *)(p + 0x18);
                *(int *)(p + 0x18) = m18 + rc;
            }
        }
        }
    }
    {
        register int active asm("$6");
        active = *(int *)(p + 0x58);
        if (active != 0) {
            register int stepbase asm("$3");
            register int t2 asm("$3");
            stepbase = kaze_state_base_ptr;
            stepbase = *(int *)(stepbase + 0xA4);
            t2 = active * stepbase;
            dz = (*(int *)(p + 0x4c) - 0x400U) & 0xfff;
            dx = *(short *)(p + 0x2a);
            tf = abs(dz - dx);
            if (tf >= 0x800) {
                dz = dz - 0x1000;
                if (dz < -0xfff) { dz = -dz; }
            }
            tf = abs(dz - dx);
            if (tf < t2) { tf = dz; }
            else if (dx < dz) { tf = dx + t2; }
            else { tf = dx - t2; }
            *(ushort *)(p + 0x2a) = tf & 0xfff;
        }
    }
}

extern int kaze_route_node_next_table;
extern int kaze_route_node_x_table;
extern int kaze_route_node_y_table;
extern int kaze_route_node_z_table;
extern int ratan2(int, int);

/**
 * kaze_dynobj_route_follow_smooth  (objective, 0x80027428)
 *
 * Route follower over the kaze_route_node_next_table node table (stride
 * 0x10): at each node advances and recomputes yaw/pitch words 0x13/0x12 via
 * ratan2 (state 1 when next<0); accelerates 0x1b to 0x1d, turns yaw +0x2a,
 * integrates.
 */

void kaze_dynobj_route_follow_smooth(int *obj)
{
    extern int kaze_state_base_ptr;
    int res;
    int t;
    int next_z;
    int n1;
    int nx;
    int adv;
    int adv2;
    int cx;
    int sx;
    int tr;
    int tr2;
    int clamped;
    int dz;
    int dx;
    int gx;
    int gy;
    int gz;
    int gw;
    uint hu;
    int hs1;
    int hs2;
    int hf1;
    int hf2;
    int ht;

    dx = obj[4] - obj[0x15];
    dz = obj[6] - obj[0x17];
    t = dx;
    if (t < 0) {
        __asm__("" : "=r"(t) : "0"(t));
        t = -t;
    }
    if (t < 0x4000) {
        __asm__("" :: "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz));
        __asm__("" :: "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz));
        __asm__("" :: "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz));
        __asm__("" :: "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz));
        __asm__("" :: "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj));
        __asm__("" :: "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj));
        __asm__("" :: "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj));
        __asm__("" :: "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj));
        __asm__("" :: "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj));
        __asm__("" :: "r"(obj), "r"(obj), "r"(obj));
        __asm__("" :: "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx));
        __asm__("" :: "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx));
        t = dz;
        if (t < 0) {
            __asm__("" : "=r"(t) : "0"(t));
            t = -t;
        }
        if (t < 0x4000) {
            res = SquareRoot0(dx * dx + dz * dz);
            n1 = obj[0x1b];
            if (res < n1 * *(int *)(kaze_state_base_ptr + 0xa4)) {
                obj[4] = obj[0x15];
                obj[6] = obj[0x17];
                nx = *(int *)((char *)&kaze_route_node_next_table + obj[0x1c] * 0x10);
                obj[0x1c] = nx;
                if (nx < 0) {
                    *obj = 1;
                    return;
                }
                obj[0x15] = *(int *)((char *)&kaze_route_node_x_table + nx * 0x10);
                obj[0x16] = *(int *)((char *)&kaze_route_node_y_table + obj[0x1c] * 0x10);
                next_z = *(int *)((char *)&kaze_route_node_z_table + obj[0x1c] * 0x10);
                gx = obj[0x15] - obj[4];
                obj[0x17] = next_z;
                if (gx < 0) {
                    gx = gx + 0xf;
                }
                gy = obj[0x16] - obj[5];
                dx = gx >> 4;
                if (gy < 0) {
                    gy = gy + 0xf;
                }
                gw = gy >> 4;
                gz = next_z - obj[6];
                if (gz < 0) {
                    gz = gz + 0xf;
                }
                dz = gz >> 4;
                __asm__("" :: "r"(gz), "r"(gz), "r"(gz));
                res = ratan2(dx, dz);
                obj[0x13] = 0x1000U - res & 0xfff;
                res = SquareRoot0(dx * dx + dz * dz);
                res = ratan2(gw, res);
                obj[0x12] = 0x1000U - res & 0xfff;
                return;
            }
            goto LAB_800275fc;
        }
    }
LAB_800275fc:
    adv = obj[0x1b];
    if (adv < obj[0x1d]) {
        adv2 = adv + obj[0x1a] * *(int *)(kaze_state_base_ptr + 0xa4);
        obj[0x1b] = adv2;
        if (0 <= adv2) {
            int cap = obj[0x1d];
            __asm__("" :: "r"(cap));
            clamped = cap;
            if (adv2 <= clamped) {
                clamped = adv2;
            }
        }
        else {
            clamped = 0;
        }
        obj[0x1b] = clamped;
    }
    hf1 = *(int *)(kaze_state_base_ptr + 0xa4) * 4;
    hs1 = (int)*(short *)((int)obj + 0x2a);
    hu = obj[0x13] - 0x400 & 0xfff;
    __asm__("" :: "r"(hs1), "r"(hu));
    __asm__("" : "=r"(hu) : "0"(hu), "r"(hf1));
    ht = hu - hs1;
    __asm__("" :: "r"(ht));
    __asm__("" :: "r"(ht));
    if (ht < 0) {
        ht = -ht;
    }
    if (0x7ff < ht) {
        hu = hu - 0x1000;
        if ((int)hu < -0xfff) {
            hu = -hu;
        }
    }
    ht = hu - hs1;
    if (ht < 0) {
        ht = -ht;
    }
    if (ht < hf1) {
        ht = hu;
    } else if (hs1 < (int)hu) {
        ht = hs1 + hf1;
    } else {
        ht = hs1 - hf1;
    }
    *(ushort *)((int)obj + 0x2a) = ht & 0xfff;
    hf2 = *(int *)(kaze_state_base_ptr + 0xa4) * 4;
    hs2 = (int)*(short *)(obj + 0xb);
    hu = 0x1000 - obj[0x12] & 0xfff;
    __asm__("" :: "r"(hs2), "r"(hu));
    __asm__("" : "=r"(hu) : "0"(hu), "r"(hf2));
    ht = hu - hs2;
    __asm__("" :: "r"(ht));
    __asm__("" :: "r"(ht));
    if (ht < 0) {
        ht = -ht;
    }
    if (0x7ff < ht) {
        hu = hu - 0x1000;
        if ((int)hu < -0xfff) {
            hu = -hu;
        }
    }
    ht = hu - hs2;
    if (ht < 0) {
        ht = -ht;
    }
    if (ht < hf2) {
        ht = hu;
    } else if (hs2 < (int)hu) {
        ht = hs2 + hf2;
    } else {
        ht = hs2 - hf2;
    }
    *(ushort *)(obj + 0xb) = ht & 0xfff;
    cx = rcos(obj[0x12]);
    cx = cx * obj[0x1b] * *(int *)(kaze_state_base_ptr + 0xa4);
    if (cx < 0) {
        cx = cx + 0xfff;
    }
    dx = cx >> 0xc;
    sx = rsin(obj[0x12]);
    sx = sx * obj[0x1b] * *(int *)(kaze_state_base_ptr + 0xa4);
    if (sx < 0) {
        sx = sx + 0xfff;
    }
    gw = sx >> 0xc;
    tr = rsin(obj[0x13]);
    tr = tr * dx;
    if (tr < 0) {
        tr = tr + 0xfff;
    }
    obj[4] = obj[4] - (tr >> 0xc);
    tr2 = rcos(obj[0x13]);
    tr2 = tr2 * dx;
    if (tr2 < 0) {
        tr2 = tr2 + 0xfff;
    }
    obj[6] = obj[6] + (tr2 >> 0xc);
    obj[5] = obj[5] - gw;
    __asm__("" :: "r"(gw));
    return;
}

/**
 * kaze_dynobj_fall_respawn_anim  (objective, 0x80027870)
 *
 * Dynamic-object fall/respawn animation: lowers Y (word 5) by fall speed
 * +0x5c per tick while ramping phase +0x54 by 0xc; past 0x1000 resets phase
 * and snaps Y back to base +0x58; scale words +0x30/34/38 follow the phase.
 */

void kaze_dynobj_fall_respawn_anim(int obj)
{
    extern int kaze_state_base_ptr;
    int base;
    int s;
    int v;

    base = kaze_state_base_ptr;
    *(int *)(obj + 0x14) = *(int *)(obj + 0x14) - *(int *)(obj + 0x5C) * *(int *)(base + 0xA4);
    s = *(int *)(obj + 0x54) + *(int *)(base + 0xA4) * 0xC;
    *(int *)(obj + 0x54) = s;
    if (s > 0x1000) {
        *(int *)(obj + 0x54) = 0;
        *(int *)(obj + 0x14) = *(int *)(obj + 0x58);
    }
    v = *(int *)(obj + 0x54);
    *(int *)(obj + 0x38) = v;
    *(int *)(obj + 0x34) = v;
    *(int *)(obj + 0x30) = v;
    return;
}

/**
 * kaze_dynobj_bob_anim  (objective, 0x800278ec)
 *
 * Dynamic-object bobbing animation: phase +0x60 advances 0xc/tick mod
 * 0x1000; Y (word 5) = base +0x58 + rsin(phase)*amp +0x64; scale words
 * +0x30/34/38 = 0x1000 + rsin(phase)*amp +0x68.
 */

void kaze_dynobj_bob_anim(int obj)
{
    extern int kaze_state_base_ptr;
    int phase;
    int v;
    register int scale asm("$2");

    phase = (*(int *)(obj + 0x60) + (*(int *)(kaze_state_base_ptr + 0xA4) * 0xC)) & 0xFFF;
    *(int *)(obj + 0x60) = phase;

    v = rsin(phase);
    v *= *(int *)(obj + 0x64);
    if (v < 0) {
        v += 0xFFF;
    }
    *(int *)(obj + 0x14) = *(int *)(obj + 0x58) + (v >> 0xC);

    scale = rsin(*(int *)(obj + 0x60));
    scale *= *(int *)(obj + 0x68);
    if (scale < 0) {
        scale += 0xFFF;
    }
    scale = (scale >> 0xC) + 0x1000;
    *(int *)(obj + 0x38) = scale;
    *(int *)(obj + 0x34) = scale;
    *(int *)(obj + 0x30) = scale;
}

/**
 * kaze_dynobj_route_follow_direct  (objective, 0x800279a0)
 *
 * Simplified kaze_route_node_next_table route follower: same node-
 * advance/yaw-pitch computation and acceleration as the smooth variant but
 * integrates position directly from words 0x12/0x13 without model turn
 * smoothing (flying objects).
 */

void kaze_dynobj_route_follow_direct(int *obj)
{
    extern int kaze_state_base_ptr;
    int res;
    int t;
    int next_z;
    int n1;
    int nx;
    int adv;
    int adv2;
    int cx;
    int sx;
    int tr;
    int tr2;
    int clamped;
    int dz;
    int dx;
    int gx;
    int gy;
    int gz;
    int gw;

    dx = obj[4] - obj[0x15];
    dz = obj[6] - obj[0x17];
    t = dx;
    if (t < 0) {
        __asm__("" : "=r"(t) : "0"(t));
        t = -t;
    }
    if (t < 0x4000) {
        __asm__("" :: "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz));
        __asm__("" :: "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz));
        __asm__("" :: "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz));
        __asm__("" :: "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz), "r"(dz));
        __asm__("" :: "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj));
        __asm__("" :: "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj));
        __asm__("" :: "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj));
        __asm__("" :: "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj));
        __asm__("" :: "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj), "r"(obj));
        __asm__("" :: "r"(obj), "r"(obj), "r"(obj));
        __asm__("" :: "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx));
        __asm__("" :: "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx), "r"(dx));
        t = dz;
        if (t < 0) {
            __asm__("" : "=r"(t) : "0"(t));
            t = -t;
        }
        if (t < 0x4000) {
            res = SquareRoot0(dx * dx + dz * dz);
            n1 = obj[0x1b];
            if (res < n1 * *(int *)(kaze_state_base_ptr + 0xa4)) {
                obj[4] = obj[0x15];
                obj[6] = obj[0x17];
                nx = *(int *)((char *)&kaze_route_node_next_table + obj[0x1c] * 0x10);
                obj[0x1c] = nx;
                if (nx < 0) {
                    *obj = 1;
                    return;
                }
                obj[0x15] = *(int *)((char *)&kaze_route_node_x_table + nx * 0x10);
                obj[0x16] = *(int *)((char *)&kaze_route_node_y_table + obj[0x1c] * 0x10);
                next_z = *(int *)((char *)&kaze_route_node_z_table + obj[0x1c] * 0x10);
                gx = obj[0x15] - obj[4];
                obj[0x17] = next_z;
                if (gx < 0) {
                    gx = gx + 0xf;
                }
                gy = obj[0x16] - obj[5];
                dx = gx >> 4;
                if (gy < 0) {
                    gy = gy + 0xf;
                }
                gw = gy >> 4;
                gz = next_z - obj[6];
                if (gz < 0) {
                    gz = gz + 0xf;
                }
                dz = gz >> 4;
                __asm__("" :: "r"(gz), "r"(gz), "r"(gz));
                res = ratan2(dx, dz);
                obj[0x13] = 0x1000U - res & 0xfff;
                res = SquareRoot0(dx * dx + dz * dz);
                res = ratan2(gw, res);
                obj[0x12] = 0x1000U - res & 0xfff;
                return;
            }
            goto LAB_80027b74;
        }
    }
LAB_80027b74:
    adv = obj[0x1b];
    if (adv < obj[0x1d]) {
        adv2 = adv + obj[0x1a] * *(int *)(kaze_state_base_ptr + 0xa4);
        obj[0x1b] = adv2;
        if (0 <= adv2) {
            int cap = obj[0x1d];
            __asm__("" :: "r"(cap));
            clamped = cap;
            if (adv2 <= clamped) {
                clamped = adv2;
            }
        }
        else {
            clamped = 0;
        }
        obj[0x1b] = clamped;
    }
    cx = rcos(obj[0x12]);
    cx = cx * obj[0x1b] * *(int *)(kaze_state_base_ptr + 0xa4);
    if (cx < 0) {
        cx = cx + 0xfff;
    }
    dx = cx >> 0xc;
    sx = rsin(obj[0x12]);
    sx = sx * obj[0x1b] * *(int *)(kaze_state_base_ptr + 0xa4);
    if (sx < 0) {
        sx = sx + 0xfff;
    }
    gw = sx >> 0xc;
    tr = rsin(obj[0x13]);
    tr = tr * dx;
    if (tr < 0) {
        tr = tr + 0xfff;
    }
    obj[4] = obj[4] - (tr >> 0xc);
    tr2 = rcos(obj[0x13]);
    tr2 = tr2 * dx;
    if (tr2 < 0) {
        tr2 = tr2 + 0xfff;
    }
    obj[6] = obj[6] + (tr2 >> 0xc);
    obj[5] = obj[5] - gw;
    __asm__("" :: "r"(gw));
    return;
}

/**
 * kaze_dynobj_state_timer  (objective, 0x80027ce0)
 *
 * Dynamic-object timer: resets word 0x18 when trigger word 3 is clear or
 * pause flag 0x2000000 is set, else accumulates by the vsync divisor and
 * switches the slot to state 3 when threshold word 0x19 is reached.
 */

void kaze_dynobj_state_timer(int *obj)
{
    extern int *kaze_state_base_ptr;
    int t;

    if ((obj[3] != 0) && ((kaze_state_base_ptr[5] & 0x2000000) == 0)) {
        t = obj[0x18] + kaze_state_base_ptr[0x29];
        obj[0x18] = t;
        if (obj[0x19] <= t) {
            obj[0] = 3;
        }
    } else {
        obj[0x18] = 0;
    }
}

extern int kaze_stage_mode;

/**
 * kaze_mode_object_update  (objective, 0x80027d48)
 *
 * Per-frame update of the +0x58e0c mode-object slots: trig velocity,
 * gravity, terrain/target collision via the collision queries, per-slot cull
 * flags; in sub-mode 1 also rotates the +0xecb00 animation ring into a graph
 * buffer.
 */

void kaze_mode_object_update(void)
{
    extern void kaze_trail_particle_spawn(int *);
    extern char *kaze_state_base_ptr;

  char *d;
  KSlotB *s7;
  int i;
  int coord[3];
  int cbuf[4];
  d = kaze_state_base_ptr;
  s7 = (KSlotB *) (d + 0x58e0c);
  if (kaze_stage_mode == 1)
  {
    u16 *dst;
    u16 hv;
    char *pa;
    {
      int vb = (((int) d) + 0x48d84) + ((*((int *) (d + 0x98))) * 0x44);
__asm__("" : "=r"(vb) : "0"(vb));
      dst = (u16 *) (vb + 0x10);
    }
    hv = *((u16 *) (d + 0xecb1e));
    {
      int c1;
      int c2;
      i = 0xf;
      c1 = 0xecb00;
      c2 = 0xecafe;
__asm__("" : "=r"(c1) : "0"(c1));
__asm__("" : "=r"(c2) : "0"(c2));
      pa = d + 0x1e;
      do
      {
__asm__("" : "=r"(pa) : "0"(pa));
        *((u16 *) (pa + c1)) = *((u16 *) (pa + c2));
        pa -= 2;
        i -= 1;
      }
      while (i >= 2);
    }
    i = 0;
    *((u16 *) (kaze_state_base_ptr + 0xecb02)) = hv;
    do
    {
      *dst = *((u16 *) ((kaze_state_base_ptr + (i * 2)) + 0xecb00));
      dst += 1;
      i += 1;
    }
    while (i < 0x10);
  }
  if ((((u32) (kaze_stage_mode - 1)) < 2) && ((d = kaze_state_base_ptr, ((*((int *) (d + 0x14))) & 0xc0001000) == 0)))
  {
    if (kaze_stage_mode == 1)
    {
      i = 0;
      if ((*((int *) (d + 0x24))) > 0)
      {
        u32 K8 = 0x8000;
        u32 KFF = 0xffff;
        KSlotA *cur = (KSlotA *) (((int *) s7) + 1);
        do
        {
          int dx;
          int dy;
          int dz;
__asm__("" : "=r"(cur) : "0"(cur));
          dx = (*((int *) (d + 0x68))) - cur->f04;
          dy = cur->f08 - (*((int *) (d + 0x6c)));
          dz = cur->f0c - (*((int *) (d + 0x70)));
          {

            register u32 rv __asm__("$2");
            if (((((u32) (dx + K8)) <= KFF) && (((u32) (dz + K8)) <= KFF)) && (((u32) (dy + K8)) <= KFF))
            {
              int tmp = SquareRoot0((dx * dx) + (dz * dz));
              if (((int) tmp) < 0x76c1)
              {
                rv = tmp;
                cur->h24 = dx;
                cur->h26 = dy;
                cur->h28 = dz;
              }
              else
              {
                rv = 0xffffffff;
              }
            }
            else
            {
              rv = 0xffffffff;
            }
            {
              int val = (~rv) >> 31;
              d = kaze_state_base_ptr;
__asm__("" : "=r"(i) : "0"(i), "r"(val));
              i += 1;
              cur->f00 = val;
              cur += 1;
            }
          }
        }
        while (i < (*((int *) (d + 0x24))));
      }
    }
    else
    {
      i = 0;
      do
      {
        int st;
        int dx;
        int dy;
        int dz;
        cbuf[0] = 0;
        st = s7->f48;
        if (st == 0x81000000)
        {
          int p1 = s7->f04;
          int p3;
          int q1;
          int q3;
          s7->f08 += 0x100;
          coord[0] = p1;
          coord[1] = (-s7->f08) + 700;
          p3 = s7->f0c;
          coord[2] = p3;
          q1 = p1 / 3800;
          q3 = p3 / 3800;
          if (kaze_terrain_zone_collision_query(coord, q1, q3, 700, cbuf) != 0)
          {
            s7->f48 = 0x82000000;
            kaze_trail_particle_spawn(&s7->f04);
          }
        }
        else
          if (st == 0x82000000)
        {
          int a;
          int v;
          v = s7->f14 + 0x100;
          s7->f14 = v;
          if (v >= 0x1000)
          {
            a = 0x2000;
            if (v < 0x2001)
            {
              a = v;
            }
          }
          else
          {
            a = 0x1000;
          }
          s7->f14 = a;
          v = s7->f1c + 0x100;
          s7->f1c = v;
          if (v >= 0x1000)
          {
            a = 0x2000;
            if (v < 0x2001)
            {
              a = v;
            }
          }
          else
          {
            a = 0x1000;
          }
          s7->f1c = a;
          v = s7->f18 - 0x80;
          s7->f18 = v;
          if (v >= 0x800)
          {
            a = 0x1000;
            if (v < 0x1001)
            {
              a = v;
            }
          }
          else
          {
            a = 0x800;
          }
          s7->f18 = a;
          if (s7->f14 >= 0x2000)
          {
            s7->f48 = 0x84000000;
          }
        }
        else
          if ((st & 0x84000000) == 0)
        {
          int qd;
          int t;
          int ang;
          int v;
          int a;
          do
          {
            qd = (-s7->f08) / 1900;
            t = rsin(s7->f38) * s7->f3c;
          }
          while (0);
          if (t < 0)
          {
            t += 0xfff;
          }
          coord[0] = s7->f04 - (t >> 12);
          t = rcos(s7->f38) * s7->f3c;
          if (t < 0)
          {
            t += 0xfff;
          }
          coord[2] = s7->f0c + (t >> 12);
          {
            int adj = s7->f08;
            if (s7->f34 <= qd)
            {
              if (s7->f34 < qd)
              {
                adj += 0x32;
              }
            }
            else
            {
              adj -= 0x32;
            }
            coord[1] = adj;
          }
          coord[1] = (-s7->f08) + 700;
          if ((((((s7->f50 != 0) || (kaze_terrain_collision_neighbor_query(coord, 700, cbuf) != 0)) || (kaze_mode_object_proximity_test(coord, 700, cbuf, i) >= 0)) || (kaze_dynamic_object_collision_query(coord, 700, cbuf) >= 0)) || (((u32) (coord[0] - 0x33f4)) > 0x70418)) || (((u32) (coord[2] - 0x33f4)) > 0x70418))
          {
            s7->f38 += 0x100;
            s7->f40 = ((rand() % 5) + 3) * 0x3c;
            s7->f34 = (-s7->f08) / 1900;
          }
          else
          {
            s7->f04 = coord[0];
            s7->f0c = coord[2];
            {
              int nv = coord[1] - 700;
__asm__("" : "=r"(nv) : "0"(nv));
              s7->f08 = -nv;
            }
            v = s7->f40 - (*((int *) (kaze_state_base_ptr + 0xa4)));
            s7->f40 = v;
            if (v < 0)
            {
              ang = s7->f38;
              {
                int vv;
                if ((rand() & 3) == 0)
                {
                  if ((rand() & 1) == 0)
                  {
                    vv = ang + 0x80;
                  }
                  else
                  {
                    vv = ang - 0x80;
                  }
                }
                else
                {
                  vv = ang;
                }
                s7->f38 = vv & 0xfff;
              }
              s7->f40 = ((rand() % 5) + 3) * 0x3c;
              if ((rand() & 3) == 0)
              {
                int w;
                if ((rand() & 1) == 0)
                {
                  w = qd - 1;
                }
                else
                {
                  w = qd + 1;
                }
                s7->f34 = w;
                if (w >= 2)
                {
                  a = 8;
                  if (w < 9)
                  {
                    a = w;
                  }
                }
                else
                {
                  a = 2;
                }
                s7->f34 = a;
              }
            }
          }
          s7->f50 = 0;
          {
            int h = (s7->f38 + 0xc00) & 0xfff;
            int raw = *((int *) (kaze_state_base_ptr + 0xa4));
            register int t2 __asm__("$2") = raw * 2;
            register int t3 __asm__("$2") = t2 + raw;
            register int step __asm__("$5");
            int cur;
__asm__("" : "=r"(t3) : "0"(t3));
__asm__("" :: "r"(t2) : "memory");
            cur = s7->h2e;
            step = t3 << 3;
            {
              int diff = h - cur;
__asm__("" :: "r"(diff));
              if (diff < 0)
              {
                diff = -diff;
              }
              if (diff >= 0x800)
              {
                h -= 0x1000;
                if (h < (-0xfff))
                {
                  h = -h;
                }
              }
            }
            {
              int res = h - cur;
              if (res < 0)
              {
                res = -res;
              }
              {
                int out;
                if (res < step)
                {
                  out = h;
                }
                else
                  if (cur < h)
                {
                  out = cur + step;
                }
                else
                {
                  out = cur - step;
                }
                s7->h2e = out & 0xfff;
              }
            }
          }
        }
        dx = (*((int *) (kaze_state_base_ptr + 0x68))) - s7->f04;
        dy = s7->f08 - (*((int *) (kaze_state_base_ptr + 0x6c)));
        dz = s7->f0c - (*((int *) (kaze_state_base_ptr + 0x70)));
        {

          register u32 rv __asm__("$2");
          if (((((u32) (dx + 0x8000)) < 0x10000) && (((u32) (dz + 0x8000)) < 0x10000)) && (((u32) (dy + 0x8000)) < 0x10000))
          {
            int tmp = SquareRoot0((dx * dx) + (dz * dz));
            if (((int) tmp) < 0x76c1)
            {
              rv = tmp;
              s7->h24 = dx;
              s7->h26 = dy;
              s7->h28 = dz;
            }
            else
            {
              rv = 0xffffffff;
            }
          }
          else
          {
            rv = 0xffffffff;
          }
          s7->f00 = (~rv) >> 31;
        }
        i += 1;
        s7 += 1;
      }
      while (i < 6);
    }
  }
}

void kaze_marker_attach_to_dynobj();
extern int kaze_sfx_voice_keyon_34108(int, int);
/* 0x5c */
/* 0x78 */

/**
 * kaze_marker_slot_update  (objective, 0x80028540)
 *
 * State machine for the 10 mode-0 marker slots (+0x58a74): moves markers,
 * tests terrain collision, attaches to dynamic objects, and computes on-
 * screen visibility (+0x4).
 */

void kaze_marker_slot_update(void)
{
    extern int kaze_trail_particle_spawn(int);
    extern int kaze_state_base_ptr;
    int heading;
    u32 state;
    register u32 res asm("$2");
    u32 t;
    int mtmp;
    int base;
    int tdec;
    int speed_clamped;
    int hspeed;
    int calc;
    int old_y;
    register int q asm("$5");
    int dx;
    int dy;
    int dz;
    MarkerSlot *slot;
    int slot_addr;
    int slot_i;
    int pos[3];
    unsigned char query_out[8];

    slot = (MarkerSlot *)(kaze_state_base_ptr + 0x58a74);
    if ((*(u32 *)(kaze_state_base_ptr + 0x14) & 0xc0001000) == 0) {
        slot_i = 0;
        do {
            slot_addr = (int)&slot->x;
            if (slot->active != 0) {
                slot->timer50 = slot->timer50 + *(int *)(kaze_state_base_ptr + 0xa4) * 8 & 0x1ff;
                state = slot->active;
                switch (state) {
                case 1:
                    slot->swing2a = slot->ang22 + 0x400;
                    mtmp = rcos(slot->ang20);
                    mtmp = mtmp * slot->speed * *(int *)(kaze_state_base_ptr + 0xa4);
                    if (mtmp < 0) {
                        mtmp = mtmp + 0xfff;
                    }
                    old_y = slot->y;
                    hspeed = mtmp >> 0xc;
                    calc = rsin(slot->ang22);
                    calc = calc * hspeed;
                    if (calc < 0) {
                        calc = calc + 0xfff;
                    }
                    slot->x = slot->x - (calc >> 0xc);
                    calc = rcos(slot->ang22);
                    calc = calc * hspeed;
                    if (calc < 0) {
                        calc = calc + 0xfff;
                    }
                    slot->z = slot->z + (calc >> 0xc);
                    calc = rsin(slot->ang20);
                    calc = calc * slot->speed * *(int *)(kaze_state_base_ptr + 0xa4);
                    if (calc < 0) {
                        calc = calc + 0xfff;
                    }
                    slot->y = slot->y + (calc >> 0xc);
                    calc = slot->speed - *(int *)(kaze_state_base_ptr + 0xa4);
                    slot->speed = calc;
                    if (0 <= calc) {
                        speed_clamped = 1000;
                        if (calc < 0x3e9) {
                            speed_clamped = calc;
                        }
                    } else {
                        speed_clamped = 0;
                    }
                    slot->speed = speed_clamped;
                    calc = slot->y + (slot->f34 - speed_clamped);
                    slot->y = calc;
                    heading = ratan2(hspeed, calc - old_y);
                    slot->heading2c = (short)heading;
                    pos[0] = slot->x;
                    pos[1] = -slot->y;
                    pos[2] = slot->z;
                    if ((kaze_stage_mode == 2) &&
                        (q = kaze_mode_object_proximity_test((u32 *)pos, 100, query_out, 0xffffffff),
                         ({ __asm__("" : "=r"(q) : "0"(q)); 0; }),
                         -1 < q)) {
                        slot->active = 0;
                        base = kaze_state_base_ptr + q * 0x58;
                        tdec = *(int *)(base + 0x58e5c) + -1;
                        *(int *)(base + 0x58e5c) = tdec;
                        if (tdec < 1) {
                            *(int *)(base + 0x58e5c) = 0;
                            *(u32 *)(base + 0x58e58) = 0x81000000;
                        } else {
                            *(u32 *)(base + 0x58e58) = *(u32 *)(base + 0x58e58) | 0x10;
                            goto LAB_800289c0;
                        }
                    } else {
                        q = kaze_dynamic_object_collision_query((u32 *)pos, 100, query_out);
                        __asm__("" : "=r"(q) : "0"(q));
                        if (-1 < q) {
                            if ((u32)kaze_stage_mode < 2) goto LAB_800288f0;
                            slot->active = 0;
                        } else {
                            hspeed = kaze_terrain_collision_neighbor_query((u32 *)pos, 100, query_out);
                            if (hspeed == 0) goto LAB_800289c0;
                            if ((u32)kaze_stage_mode < 2) {
                                slot->active = 0x80000000;
                            } else {
                                __asm__ volatile("# xj1");
                                slot->active = 0;
                            }
                        }
                    }
                    goto LAB_800288fc;
                case 0x80000000:
                    pos[0] = slot->x;
                    pos[1] = -slot->y;
                    pos[2] = slot->z;
                    q = kaze_dynamic_object_collision_query((u32 *)pos, 100, query_out);
                    __asm__("" : "=r"(q) : "0"(q));
                    if (-1 < q) {
LAB_800288f0:
                        kaze_marker_attach_to_dynobj((u32 *)slot);
LAB_800288fc:
                        {
                            register int ta asm("$4") = slot_addr;
                            __asm__ volatile("# jointop");
                            kaze_trail_particle_spawn(ta);
                        }
                        kaze_sfx_voice_keyon_34108(2, 1);
                    } else {
                        slot->swing2a = slot->swing2a + 0x80;
                    }
                    break;
                case 0x81000000:
                    __asm__("" :: "r"((int)&slot->x));
                    slot->swing2a = slot->swing2a + 0x80;
                    slot->x = ((KRootDyn *)kaze_state_base_ptr)->d[slot->idx38].px + slot->off3c;
                    slot->y = ((KRootDyn *)kaze_state_base_ptr)->d[slot->idx38].py + slot->off40;
                    slot->z = ((KRootDyn *)kaze_state_base_ptr)->d[slot->idx38].pz + slot->off44;
                    break;
                }
LAB_800289c0:
                dx = *(int *)(kaze_state_base_ptr + 0x68) - slot->x;
                dy = slot->y - *(int *)(kaze_state_base_ptr + 0x6c);
                dz = slot->z - *(int *)(kaze_state_base_ptr + 0x70);
                if (((dx + 0x8000U < 0x10000) && (dz + 0x8000U < 0x10000)) &&
                    (dy + 0x8000U < 0x10000)) {
                    t = SquareRoot0(dx * dx + dz * dz);
                    if ((int)t < 0x76c1) {
                        slot->rel_x = (short)dx;
                        slot->rel_y = (short)dy;
                        slot->rel_z = (short)dz;
                        res = t;
                    } else {
                        res = 0xffffffff;
                    }
                } else {
                    res = 0xffffffff;
                }
                slot->onscreen = ~res >> 0x1f;
            }
            slot_i = slot_i + 1;
            slot = slot + 1;
        } while (slot_i < 10);
    }
    return;
}

/**
 * kaze_marker_attach_to_dynobj  (objective, 0x80028abc)
 *
 * Attaches a completed marker to dynamic object idx: sets marker state
 * 0x81000000, stores the object index at word 0xe and the marker-minus-
 * object offset (from +0x5901c slot pos +0x5902c/30/34) into words
 * 0xf..0x11.
 */

void kaze_marker_attach_to_dynobj(int *p, int idx)
{
    extern char *kaze_state_base_ptr;
    char *r;

    p[0] = 0x81000000;
    p[0xe] = idx;
    r = kaze_state_base_ptr + idx * 120;
    p[0xf] = p[2] - *(int *)(r + 0x5902c);
    p[0x10] = p[3] - *(int *)(r + 0x59030);
    p[0x11] = p[4] - *(int *)(r + 0x59034);
}

extern int kaze_stage_flags;

/**
 * kaze_stage_flag_cloth_anim  (render, 0x80028b38)
 *
 * Flag/cloth vertex animator: perturbs model slot 8 vertices with rsin
 * driven by the +0xed348 phase counter.
 */

void kaze_stage_flag_cloth_anim(void)
{
    extern int kaze_state_base_ptr;
  int sin_amt;
  int u_delta;
  int v_delta;
  int unused_int0;
  int wind_v_hi;
  int wind_v_lo;
  int vtx_i;
  short phase;
  char *vtx_ptr;
  char *wave_ptr;
  int base;
  int s2;
  int j;
  int r1;
  int q;
  int q3;

  register int r2 __asm__("$2");
  int j3;
  int r3;
  int c4;
  int sp4;
  base = kaze_state_base_ptr;
  s2 = base + 0xED328;
  if (((*((uint *) (base + 0x14))) & 0xc0000000) == 0)
  {
    if ((kaze_stage_flags & 1) != 0)
    {
      sin_amt = rsin(0x400);
      {
        int sd = (*((int *) (kaze_state_base_ptr + 0xa4))) * 10;
__asm__("" : "=r"(sd) : "0"(sd));
        sin_amt = sin_amt * sd;
      }
      if (sin_amt < 0)
      {
        sin_amt = sin_amt + 0xfff;
      }
      u_delta = (sin_amt >> 0xc) + ((*((int *) (kaze_state_base_ptr + 0x78))) - (*((int *) (kaze_state_base_ptr + 0x68))));
      vtx_i = 0;
      v_delta = rcos(0x400);
      {
        int sd = (*((int *) (kaze_state_base_ptr + 0xa4))) * 10;
__asm__("" : "=r"(sd) : "0"(sd));
        v_delta = v_delta * sd;
      }
      if (v_delta < 0)
      {
        v_delta = v_delta + 0xfff;
      }
      v_delta = (v_delta >> 0xc) - ((*((int *) (kaze_state_base_ptr + 0x80))) - (*((int *) (kaze_state_base_ptr + 0x70))));
      vtx_ptr = (char *) kaze_state_base_ptr;
      loop1:
      vtx_i = vtx_i + 1;

      *((ushort *) (vtx_ptr + 0x506d4)) = (*((ushort *) (vtx_ptr + 0x506d4))) - u_delta;
      *((ushort *) (vtx_ptr + 0x506d8)) = (*((ushort *) (vtx_ptr + 0x506d8))) - v_delta;
      vtx_ptr = vtx_ptr + 8;
      if (vtx_i < 0x40)
      {
        goto loop1;
      }
    }
    if ((kaze_stage_flags & 5) == 5)
    {
      j = 0;
      do
      {
        r1 = rand();
        r2 = rand();
        {
          int b1 = kaze_state_base_ptr;
          char *pa = (char *) (b1 + (j * 8));
          q = r1 / 50;
          r1 = r1 - (q * 50);
__asm__("" : "=r"(r2) : "0"(r2), "r"(b1));
          q = r2 / 50;
          r2 = r2 - (q * 50);
          r1 = r1 - r2;
          r1 = r1 - ((*((int *) (kaze_state_base_ptr + 0x78))) - (*((int *) (kaze_state_base_ptr + 0x68))));
          *((ushort *) (pa + 0x50094)) = (*((ushort *) (pa + 0x50094))) + r1;
__asm__ volatile ("" : "=r"(pa));
        }
        r1 = rand();
        r2 = rand();
        {
          char *pb = (char *) (kaze_state_base_ptr + (j * 8));
          wind_v_hi = *((int *) (kaze_state_base_ptr + 0x80));
          wind_v_lo = *((int *) (kaze_state_base_ptr + 0x70));
          phase = (*((short *) (pb + 0x50096))) + 0x32;
          *((short *) (pb + 0x50096)) = phase;
          q = r1 / 50;
          r1 = r1 - (q * 50);
          q = r2 / 50;
          r2 = r2 - (q * 50);
          r1 = r1 - r2;
          r1 = r1 + (wind_v_hi - wind_v_lo);
          *((ushort *) (pb + 0x50098)) = (*((ushort *) (pb + 0x50098))) + r1;
          if (9999 < phase)
          {
            *((short *) (pb + 0x50096)) = -16150;
          }
        }
        {
          char *pc = (char *) (kaze_state_base_ptr + (j * 8));
          int tc = *((short *) (pc + 0x50094));
          if (5999 < tc)
          {
            *((short *) (pc + 0x50094)) = (*((short *) (pc + 0x50094))) - 12000;
          }
          else
            if (tc < (-5999))
          {
            *((short *) (pc + 0x50094)) = (*((short *) (pc + 0x50094))) + 12000;
          }
        }
        {
          char *pd = (char *) (kaze_state_base_ptr + (j * 8));
          int td = *((short *) (pd + 0x50098));
          if (5999 < td)
          {
            *((short *) (pd + 0x50098)) = (*((short *) (pd + 0x50098))) - 12000;
          }
          else
            if (td < (-5999))
          {
            *((short *) (pd + 0x50098)) = (*((short *) (pd + 0x50098))) + 12000;
          }
        }
        j = j + 1;
      }
      while (j < 200);
    }
    else
      if ((kaze_stage_flags & 3) == 3)
    {
      j3 = 0;
      do
      {
        r3 = rand();
        {
          char *p3 = (char *) (kaze_state_base_ptr + (j3 * 8));
          int t1000 = (*((ushort *) (p3 + 0x50096))) + 1000;
          q3 = t1000 + (r3 % 0x200);
          *((short *) (p3 + 0x50096)) = (short) q3;
          if (9999 < ((q3 * 0x10000) >> 0x10))
          {
            *((short *) (p3 + 0x50096)) = -16150;
          }
        }
        j3 = j3 + 1;
      }
      while (j3 < 200);
    }
    if (((kaze_stage_flags & 0x20000000) != 0) || ((*((int *) (s2 + 0x24))) != 0))
    {
      *((short *) (s2 + 0xa)) = -10000;
      sp4 = rsin(0);
      *((short *) (s2 + 0x8)) = (short) (sp4 / 2);
      sp4 = rcos(0);
      *((short *) (s2 + 0xc)) = (short) (sp4 / 2);

      {
        int mb = kaze_state_base_ptr;
        *((uint *) (s2 + 0x20)) = ((*((int *) (s2 + 0x20))) + 0x10U) & 0xfff;
        c4 = 0;
        wave_ptr = (char *) ((*((int *) ((*((int *) (mb + 0x58a08))) + 0x1b8))) + 0xc);
      }
      loop4:
      sp4 = rsin((*((int *) (s2 + 0x20))) + (c4 * 0xf1)) * 800;

      if (sp4 < 0)
      {
        sp4 = sp4 + 0xfff;
      }
      {
        uint us = ((uint) sp4) >> 0xc;
__asm__("" : "=r"(c4) : "0"(c4), "r"(us));
        *((ushort *) (wave_ptr + 0x88)) = (ushort) us;
        *((ushort *) (wave_ptr - 8)) = (ushort) us;
      }
      c4 = c4 + 1;
      sp4 = rsin((*((int *) (s2 + 0x20))) + (c4 * 0xf1)) * 800;
      if (sp4 < 0)
      {
        sp4 = sp4 + 0xfff;
      }
      *((ushort *) (wave_ptr + 0x90)) = (ushort) (((uint) sp4) >> 0xc);
      *((ushort *) wave_ptr) = (ushort) (((uint) sp4) >> 0xc);
      wave_ptr = wave_ptr + 8;
      if (c4 < 0x11)
      {
        goto loop4;
      }
      {
        int lv = *((int *) (s2 + 0x24));
        int av;
        int cv;
        if ((kaze_stage_flags & 0x20000000) != 0)
        {
          av = lv + (*((int *) (kaze_state_base_ptr + 0xa4)));
        }
        else
        {
          av = lv - (*((int *) (kaze_state_base_ptr + 0xa4)));
        }
        *((int *) (s2 + 0x24)) = av;
        if (0 <= av)
        {
          cv = 0x80;
          if (av < 0x81)
          {
            cv = av;
          }
        }
        else
        {
          cv = 0;
        }
      {
        register int b3 __asm__("$2");
        int tv;
        b3 = kaze_state_base_ptr;
        tv = *((int *) (s2 + 0x28));
        *((int *) (s2 + 0x24)) = cv;
        tv = tv - (*((int *) (b3 + 0xa4)));
        *((int *) (s2 + 0x28)) = tv;
        if (tv < 0)
      {
        *((int *) (s2 + 0x28)) = 0;
        {
          int *fp = (int *) (&kaze_stage_flags);
          *fp = (*fp) & 0xdfffffff;
        }
      }
      }
      }
    }
    s2 = s2 + 0x30;
    if (((kaze_stage_flags & 0x10000000) != 0) || ((*((int *) (s2 + 0x20))) != 0))
    {
      {
        int lv = *((int *) (s2 + 0x20));
        int av;
        int cv;
        if ((kaze_stage_flags & 0x10000000) != 0)
        {
          av = lv + (*((int *) (kaze_state_base_ptr + 0xa4)));
        }
        else
        {
          av = lv - (*((int *) (kaze_state_base_ptr + 0xa4)));
        }
        *((int *) (s2 + 0x20)) = av;
        if (0 <= av)
        {
          cv = 0x80;
          if (av < 0x81)
          {
            cv = av;
          }
        }
        else
        {
          cv = 0;
        }
      {
        register int b3 __asm__("$2");
        int tv;
        b3 = kaze_state_base_ptr;
        tv = *((int *) (s2 + 0x24));
        *((int *) (s2 + 0x20)) = cv;
        tv = tv - (*((int *) (b3 + 0xa4)));
        *((int *) (s2 + 0x24)) = tv;
        if (tv < 0)
      {
        *((int *) (s2 + 0x24)) = 0;
        {
          int *fp = (int *) (&kaze_stage_flags);
          *fp = (*fp) & 0xefffffff;
        }
      }
      }
      }
    }
  }
  return;
}
