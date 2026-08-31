#include "types.h"
#include "kaze_syms.h"
extern int kaze_camera_anchor_route1_x;
extern int kaze_camera_anchor_route1_y;
extern int kaze_camera_anchor_route1_z;
extern int kaze_gas_floor;
extern int kaze_min_gas;
extern int kaze_mode_state;
extern int kaze_prev_frame_pos_y;
extern int kaze_prev_frame_rot0;
extern int kaze_target_gas;
extern int kaze_balloon_sway_phase;
extern int kaze_input_bind_left;
extern int kaze_input_bind_down;
extern int kaze_input_bind_right;
extern int kaze_input_bind_up;
extern int kaze_balloon_render_rot0;
extern int kaze_pos_y;
extern int kaze_approach_angle(int, int, int);
extern int kaze_approach_value(int, int, int);
struct S16 { int w0, w1, w2, w3; };
struct S8 { char b[8]; };

extern char *kaze_state_base_ptr;
extern int kaze_pos_x;
extern int kaze_pos_z;
extern int kaze_stage_variant;
extern void kaze_sfx_voice_keyon_34108(int, int);

/**
 * kaze_marker_drop  (objective, 0x8002193c)
 *
 * Drops a marker into +0x58a74 slot slot: decrements remaining
 * kaze_mode_state, state 1, scale 0x100, position from balloon
 * kaze_pos_x/dc/e0 (+0x32 Y), rotation from camera root+0x48/4a/4c, plays
 * sound (1,1).
 */

void kaze_marker_drop(unsigned int slot)
{
    extern unsigned int kaze_stage_mode;
    char *root;
    char *entry;
    char *rot;
    int *remaining_ptr;
    int remaining;
    int rot_off;
    int idx;

    if (slot >= 10) {
        return;
    }

    remaining_ptr = &kaze_mode_state;
    remaining = *remaining_ptr;
    if (remaining == 0) {
        return;
    }

    if ((kaze_stage_mode < 2) || (kaze_stage_variant == 6)) {
        *remaining_ptr = remaining - 1;
    }
    __asm__ volatile("" ::: "memory");

    rot_off = 0x58a94;
    idx = slot * 0x5c;
    root = kaze_state_base_ptr;
    entry = root + idx;
    rot = entry + rot_off;

    *(int *)(entry + 0x58a74) = 1;
    *(int *)(entry + 0x58aa4) = 0x100;
    *(int *)(entry + 0x58aa8) = 0x100;
    *(short *)rot = 0;
    *(short *)(entry + 0x58a96) = 0;
    *(short *)(entry + 0x58a98) = 0;
    *(int *)(entry + 0x58ac0) = 0;
    *(int *)(entry + 0x58ac4) = 0;
    *(int *)(entry + 0x58ac8) = 0;
    *(int *)(entry + 0x58a7c) = kaze_pos_x;
    *(int *)(entry + 0x58a80) = kaze_pos_y;
    *(int *)(entry + 0x58a84) = kaze_pos_z;
    *(short *)rot = *(unsigned short *)(root + 0x48);
    *(short *)(entry + 0x58a96) = *(unsigned short *)(root + 0x4a);
    *(short *)(entry + 0x58a98) = *(unsigned short *)(root + 0x4c);
    *(int *)(entry + 0x58a80) = *(int *)(entry + 0x58a80) + 0x32;
    *(short *)(entry + 0x58a96) = 0x1000 - *(unsigned short *)(entry + 0x58a96);

    kaze_sfx_voice_keyon_34108(1, 1);
}

extern int kaze_delta_y;
extern unsigned int kaze_wind_current_angle;
extern short kaze_balloon_sway_offset_y;
extern int kaze_prev_frame_pos_x;
extern int kaze_balloon_render_rot1;
extern int rsin(int);
extern void kaze_play_sound_effect(int, int, int, int);
void kaze_camera_control_update();

/**
 * kaze_target_gas_law  (flight, 0x80021aec)
 *
 * Burner control law: kaze_target_gas = kaze_min_gas +
 * kaze_terrain_height_layer * 0x76c0 + 0x28000; also compares gas floor
 * against min gas with -0xf000/-0xeff1 thresholds.
 */

void kaze_target_gas_law(void)
{
    extern int kaze_stage_mode;
    register int k asm("$16") = 8;
    int smode;
    int *wind_angle_ptr;
    char *root;
    register char *pb asm("$5");
    int calc;
    int va;
    unsigned int heading_delta;
    unsigned int flags;
    int clamp;
    int unused_int0;
    int n;
    int p3;

    root = kaze_state_base_ptr;
    {
        int t177 = *(int *)(root + 0x18) * 0x76c0 + 0x28000;
        kaze_target_gas = kaze_min_gas + t177;
    }
    kaze_gas_floor = kaze_target_gas;
    flags = *(unsigned int *)(kaze_state_base_ptr + 0x14) & 0xffff7fff;
    *(unsigned int *)(kaze_state_base_ptr + 0x14) = flags;
    if (2 < kaze_stage_mode) goto reset_oob;
    if (kaze_stage_mode < 0) goto reset_oob;
    smode = *(int *)(root + 8);
    if (smode == 0) goto big;
    if (0 < smode) goto ba8;
    if (smode == -1) goto sound;
    goto LAB_80021f58;
ba8:
    if (smode == 1) goto sub1;
    if (smode == 2) goto sub2;
    goto LAB_80021f58;
sound:
    kaze_play_sound_effect(3, 100, 1, 1);
    *(int *)(kaze_state_base_ptr + 8) = *(int *)(kaze_state_base_ptr + 8) + 1;
big:
    va = (kaze_gas_floor - kaze_min_gas) + -0xf000;
    if (va < 0) {
        va = va + 0xf;
    }
    calc = va >> 4;
    calc = (-kaze_pos_y - calc) / 2;
    clamp = *(int *)(kaze_state_base_ptr + 0xa4) * 0x18;
    if (calc < -clamp) {
        clamp = -clamp;
    } else if (calc <= clamp) {
        clamp = calc;
    }
    {
        int *pd = &kaze_delta_y;
        int *pp;
        __asm__("" : "=r"(pd) : "0"(pd));
        calc = clamp;
        pd[0] = calc;
        va = pd[-4] + calc;
        pp = pd - 4;
        pd[-4] = va;
        if (-0x3a03 < va) {
            n = va;
            if (0 < n) {
                n = 0;
            }
        } else {
            n = -0x3a02;
        }
        pb = kaze_state_base_ptr;
        *pp = n;
    }
    calc = -*(int *)(pb + 0x58) - *(int *)(pb + 0x18) * 0x76c;
    {
        unsigned int fl = *(unsigned int *)(pb + 0x14);
        *(int *)(pb + 0x6c) = calc;
        *(int *)(pb + 0x7c) = calc;
        *(unsigned int *)(pb + 0x14) = fl | 0x8000;
    }
    if (kaze_delta_y == 0) {
        *(int *)(pb + 8) = *(int *)(pb + 8) + 1;
    }
    *(unsigned short *)(kaze_state_base_ptr + 0x4a) =
        *(unsigned short *)(kaze_state_base_ptr + 0x4a) + k * *(int *)(kaze_state_base_ptr + 0xa4) & 0xfff;
    goto LAB_80021f58;
sub1:
    {
        register int ob2 asm("$2") = *(int *)(root + 0x6c);
        int av = ob2 - kaze_pos_y;
        if (av < 0) {
            av = -av;
        }
        calc = -(av / 16);
    }
    if (-0x21 < calc) {
        clamp = -1;
        if (calc < 0) {
            clamp = calc;
        }
    } else {
        clamp = -0x20;
    }
    calc = clamp;
    *(int *)(kaze_state_base_ptr + 0x6c) =
        *(int *)(kaze_state_base_ptr + 0x6c) + calc * *(int *)(kaze_state_base_ptr + 0xa4);
    if (-9 < calc) {
        va = -1;
        if (calc < 0) {
            va = calc;
        }
    } else {
        va = -8;
    }
    calc = va;
    pb = kaze_state_base_ptr;
    {
        int t2 = calc * *(int *)(pb + 0xa4);
        va = t2;
        __asm__("" :: "l"(t2));
    }
    __asm__("" : "=r"(pb) : "0"(pb));
    p3 = calc * *(int *)(pb + 0xa4);
    __asm__ volatile("" ::: "memory");
    *(unsigned short *)(pb + 0x4a) =
        *(short *)(pb + 0x4a) - va & 0xfff;
    {
        int *py = &kaze_pos_y;
        int ob = *(int *)(pb + 0x6c);
        int pv = *py;
        n = ob - pv;
        if (n < 0) {
            n = -n;
        }
        calc = p3;
        __asm__("" : "=r"(calc) : "0"(calc));
        if (n < calc) {
            *(int *)(pb + 0x6c) = pv;
        }
        *(int *)(kaze_state_base_ptr + 0x7c) = *(int *)(kaze_state_base_ptr + 0x6c);
        if (*(int *)(kaze_state_base_ptr + 0x6c) <= *py) {
            *(int *)(kaze_state_base_ptr + 0x6c) = *py;
            *(int *)(kaze_state_base_ptr + 0x7c) = *py;
            *(int *)(kaze_state_base_ptr + 8) = *(int *)(kaze_state_base_ptr + 8) + 1;
        }
    }
    *(unsigned int *)(kaze_state_base_ptr + 0x14) = *(unsigned int *)(kaze_state_base_ptr + 0x14) | 0x8000;
    goto LAB_80021f58;
sub2:
    heading_delta = 0x1800U - kaze_wind_current_angle & 0xfff;
    if (*(short *)(root + 0x48) == 0x60) {
        *(int *)(root + 8) = 0;
        *(int *)(root + 0x14) = 0x2000b7;
    } else {
        int d8 = *(int *)(root + 0xa4);
        *(unsigned int *)(root + 0x14) = flags | 0x8000;
        {
            register int df asm("$2") = (unsigned int)*(unsigned short *)(root + 0x4a) +
                    d8 * 8 & 0xfff;
            int d8b;
            *(short *)(root + 0x4a) = (short)df;
            df = df - (int)heading_delta;
            d8b = *(int *)(root + 0xa4);
            if (df < 0) {
                df = -df;
            }
            if (df <= d8b << 3) {
                *(short *)(root + 0x4a) = (short)heading_delta;
            }
        }
    }
    goto LAB_80021f58;
reset_oob:
    *(int *)(kaze_state_base_ptr + 8) = 0;
    *(int *)(kaze_state_base_ptr + 0x14) = 0x2000b7;
LAB_80021f58:
    wind_angle_ptr = &kaze_balloon_sway_phase;
    {
        int tr = rsin(*wind_angle_ptr);
        tr = tr * 10;
        if (tr < 0) {
            tr = tr + 0xfff;
        }
        kaze_balloon_sway_offset_y = (short)(tr >> 0xc);
    }
    *wind_angle_ptr = *wind_angle_ptr + *(int *)(kaze_state_base_ptr + 0xa4) * 0x20 & 0xfff;
    __asm__ volatile("");
    *(struct S16 *)&kaze_prev_frame_pos_x = *(struct S16 *)&kaze_pos_x;
    *(struct S8 *)&kaze_prev_frame_rot0 = *(struct S8 *)&kaze_balloon_render_rot0;
    __asm__ volatile("");
    kaze_prev_frame_pos_y = kaze_prev_frame_pos_y + 0x73;
    kaze_camera_control_update();
    return;
}

extern int kaze_input_current_a;
extern int kaze_camera_control_speed_x[];
extern int kaze_camera_control_speed_y[];
extern int kaze_camera_anchor_target_x[];
extern int kaze_camera_anchor_target_y[];
extern int kaze_camera_anchor_target_z[];
extern int kaze_camera_invert_pitch;
extern int kaze_camera_invert_yaw;
extern int ratan2(int, int);
extern int SquareRoot0(int);
#define FLAGS   (*(int *)(kaze_state_base_ptr + 0x14))
#define PITCH   (*(u16 *)(kaze_state_base_ptr + 0x48))
#define HEADING (*(u16 *)(kaze_state_base_ptr + 0x4a))
#define ROLL    (*(u16 *)(kaze_state_base_ptr + 0x4c))
#define ANCH_X  (*(int *)(kaze_state_base_ptr + 0x50))
#define ANCH_Y  (*(int *)(kaze_state_base_ptr + 0x54))
#define ANCH_Z  (*(int *)(kaze_state_base_ptr + 0x58))
#define ROUTE   (*(int *)(kaze_state_base_ptr + 0x60))
#define OBJ_X   (*(int *)(kaze_state_base_ptr + 0x68))
#define OBJ_Y   (*(int *)(kaze_state_base_ptr + 0x6c))
#define OBJ_Z   (*(int *)(kaze_state_base_ptr + 0x70))
#define STEP    (*(int *)(kaze_state_base_ptr + 0xa4))

/**
 * kaze_camera_control_update  (core, 0x80022044)
 *
 * Camera update: manual pitch/yaw from input masks
 * (kaze_input_bind_up/1808/1ab4/17d0) with speeds
 * kaze_camera_control_speed_x/a4; auto-look (flag 0x8000) aims
 * root+0x48/4a/4c at the balloon; smooths +0x50/54/58 to
 * kaze_camera_anchor_target_x/94/98.
 */

void kaze_camera_control_update(void)
{
    char *pitch_ptr;
    char *root2;
    char *anchor_ptr;
    register int p9 asm("$5");
    int q9;
    s16 sp9;
    int stepv, targ, pitchv;
    int gneg;
    int tz, tx;
    long calc_val;
    s16 pitch_clamped;
    u32 flags_masked;
    int dz;
    int dx;
    int rv;
    s16 local[3];

    local[0] = 0;
    local[1] = 0;
    local[2] = 0;
    if ((FLAGS & 2) != 0) {
        if ((kaze_input_current_a & (kaze_input_bind_left | kaze_input_bind_right | kaze_input_bind_down | kaze_input_bind_up)) != 0) {
            if ((kaze_input_current_a & kaze_input_bind_up) != 0) {
                local[0] = kaze_camera_control_speed_x[ROUTE * 8] * STEP;
            }
            if ((kaze_input_current_a & kaze_input_bind_down) != 0) {
                local[0] = (int)local[0] - kaze_camera_control_speed_x[ROUTE * 8] * STEP;
            }
            if ((kaze_input_current_a & kaze_input_bind_right) != 0) {
                local[1] = kaze_camera_control_speed_y[ROUTE * 8] * STEP;
            }
            if ((kaze_input_current_a & kaze_input_bind_left) != 0) {
                local[1] = (int)local[1] - kaze_camera_control_speed_y[ROUTE * 8] * STEP;
            }
            if ((FLAGS & 0x100) != 0) {
                if (kaze_camera_invert_pitch != 0) {
                    gneg = local[0];
                    local[0] = -gneg;
                }
                if (kaze_camera_invert_yaw != 0) {
                    gneg = local[1];
                    local[1] = -gneg;
                }
            }
        }
    }
    if ((FLAGS & 0x8000) == 0) goto code_r0x80022424;
    stepv = OBJ_X;
    p9 = kaze_pos_x;
    dx = stepv - p9;
    if ((stepv != p9) || (OBJ_Z != kaze_pos_z)) {
        dz = OBJ_Z - kaze_pos_z;
        calc_val = ratan2(dz, dx);
        tz = kaze_approach_angle((int)(s16)HEADING, 0x1000 - (calc_val + 0x400U & 0xfff),
                                 STEP << 5) - HEADING;
        local[1] = tz;
        q9 = ANCH_Z;
        tz = dz;
        if (dz < 0) {
            __asm__("" : "=r"(tz) : "0"(tz));
            tz = -tz;
        }
        dz = tz + q9;
        p9 = dz * dz;
        q9 = ANCH_X;
        tx = dx;
        if (dx < 0) {
            __asm__("" : "=r"(tx) : "0"(tx));
            tx = -tx;
        }
        dx = tx + q9;
        q9 = dx * dx;
        calc_val = SquareRoot0(p9 + q9);
        calc_val = ratan2(OBJ_Y - kaze_pos_y, calc_val);
        stepv = STEP;
        pitchv = *(s16 *)(kaze_state_base_ptr + 0x48);
        targ = -calc_val;
    } else if ((OBJ_Y != kaze_pos_y) || ((FLAGS & 0x2001000) != 0)) {
        calc_val = SquareRoot0(ANCH_X * ANCH_X + ANCH_Z * ANCH_Z);
        calc_val = ratan2(OBJ_Y - kaze_pos_y, calc_val);
        calc_val = calc_val * 2;
        targ = -calc_val;
        targ = targ + 0x60;
        stepv = STEP;
        pitchv = *(s16 *)(kaze_state_base_ptr + 0x48);
    } else {
        goto LAB_roll;
    }
    local[0] = kaze_approach_angle(pitchv, targ, stepv << 5) - PITCH;
LAB_roll:
    local[2] = kaze_approach_angle((int)(s16)ROLL, 0, STEP << 5) - ROLL;
code_r0x80022424:
    root2 = kaze_state_base_ptr;
    if ((*(int *)(root2 + 0x14) & 0x10) != 0) {
        pitch_ptr = root2;
        __asm__("" : "=r"(pitch_ptr) : "0"(pitch_ptr));
        PITCH = PITCH + local[0];
        HEADING = HEADING + local[1];
        ROLL = ROLL + local[2];
        __asm__ volatile("" ::: "memory");
        sp9 = *(s16 *)(pitch_ptr + 0x48);
        if (sp9 >= -0x370) {
            if (0x370 < sp9) {
                pitch_clamped = 0x370;
            } else {
                pitch_clamped = sp9;
            }
        } else {
            pitch_clamped = -0x370;
        }
        anchor_ptr = kaze_state_base_ptr;
        *(u16 *)(pitch_ptr + 0x48) = pitch_clamped;
        rv = kaze_approach_value(*(int *)(anchor_ptr + 0x50), kaze_camera_anchor_target_x[*(int *)(anchor_ptr + 0x60) * 8], *(int *)(anchor_ptr + 0xa4) * 0x32);
        ANCH_X = rv;
        rv = kaze_approach_value(ANCH_Y, kaze_camera_anchor_target_y[ROUTE * 8], STEP * 0xf);
        ANCH_Y = rv;
        rv = kaze_approach_value(ANCH_Z, kaze_camera_anchor_target_z[ROUTE * 8], STEP * 0x32);
        ANCH_Z = rv;
        flags_masked = FLAGS & 0xfffffeff;
        FLAGS = flags_masked;
        if ((((ROUTE == 1) && (ANCH_X == kaze_camera_anchor_route1_x)) && (ANCH_Y == kaze_camera_anchor_route1_y)) &&
            (ANCH_Z == kaze_camera_anchor_route1_z)) {
            FLAGS = flags_masked | 0x100;
        }
    }
    return;
}
