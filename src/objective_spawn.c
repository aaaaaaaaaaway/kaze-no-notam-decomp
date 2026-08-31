#include "types.h"
#include "kaze_syms.h"
extern char kaze_mode1_target_x_table[];
extern char kaze_mode1_target_y_table[];
extern char kaze_mode1_target_z_table[];
extern int kaze_mode1_target_variant2_x;
extern int kaze_mode1_target_variant2_y;
extern int kaze_mode1_target_variant2_z;
extern int kaze_dynobj_placement_table_0;
extern int kaze_dynobj_placement_table_1;
extern int kaze_dynobj_placement_table_2;
extern int kaze_burner_ramp;
extern int kaze_delta_x;
extern int kaze_delta_z;
extern int kaze_extra_angle;
extern int kaze_extra_strength;
extern int kaze_gas;
extern int kaze_gas_floor;
extern int kaze_gas_initial;
extern int kaze_landing_lockout;
extern int kaze_live_stage_weather;
extern int kaze_mode_state;
extern int kaze_start_wind_dir;
extern int kaze_start_x;
extern int kaze_start_z;
extern int kaze_target_gas;
extern int kaze_vertical_control;
extern int kaze_vertical_penalty;
extern int kaze_vertical_wind;
extern int kaze_wind_active_strength;
extern int kaze_wind_layer0_strength;
extern int kaze_wind_previous_angle;
extern int kaze_wind_previous_strength;
extern int rand(void);
extern int kaze_balloon_scale_x;
extern int kaze_input_bind_left;
extern int kaze_input_bind_down;
extern int kaze_input_bind_marker_drop;
extern int kaze_input_bind_up;
extern int kaze_balloon_angle;
extern int kaze_pos_y;
typedef struct {
    int a, b, c, d;
} Blk;
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short vx, vy, vz, pad; } SVEC;

extern Blk kaze_staged_stage_index[];
extern int kaze_stage_variant;
extern int kaze_wind_layer0_dir;
extern int kaze_pos_z;
extern int kaze_delta_y;
extern int kaze_balloon_scale_y;
extern int kaze_balloon_scale_z;
extern int kaze_impact_flash_timer;
extern int kaze_wind_current_angle;
extern int kaze_wind_active_angle;
extern int kaze_motion_angle;
extern int kaze_burner_state;
extern int kaze_camera_anchor_target_x;
extern int kaze_camera_anchor_target_y;
extern int kaze_camera_anchor_target_z;
extern int kaze_input_current_a;
extern int kaze_input_current_pad2_mask;
extern int kaze_input_current_b;
extern int kaze_input_prev_pad2_mask;
extern int kaze_input_bind_descend;
extern int kaze_camera_invert_pitch;
extern int kaze_input_bind_confirm;
extern int kaze_input_bind_burner;
void kaze_mode1_slot_populate();
void kaze_dynamic_object_spawner();
extern void kaze_sfx_voice_keyoff_34170(int);

/**
 * kaze_flight_state_init  (flight, 0x8001bb88)
 *
 * Flight/stage state initializer: wind layers at 0xb16a8, mode-0 marker
 * segment at base+0x58a74 when stage variant is 1, stage-flag visual effect
 * blocks, and the post-event sound latch.
 */

void kaze_flight_state_init(int demo_mode)
{
    extern void bzero(char *, int);
    extern int kaze_pos_x;
    extern Blk kaze_stage_index[];
    extern char *kaze_state_base_ptr;
    Blk *dst;
    Blk *src;
    Blk *end;
    int pad[4];
    int strength;
    int i;
    int wind;
    int dir;
    int n;
    int idx;
    int ang0;

    dst = kaze_stage_index;
    src = kaze_staged_stage_index;
    end = src + 3;
    do {
        *dst++ = *src++;
    } while (src != end);

    strength = 0x14;
    i = 0;
    do {
        wind = kaze_start_wind_dir;
        if ((rand() & 3) == 0) {
            if (rand() & 1) {
                dir = (wind - 1) & 7;
            } else {
                dir = (wind + 1) & 7;
            }
        } else {
            dir = wind & 7;
        }
        *(int *)((int)&kaze_wind_layer0_dir + i) = dir;
        *(int *)((int)&kaze_wind_layer0_strength + i) = strength;
        i += 8;
    } while (i < 0x28);

    kaze_pos_x = kaze_start_x;
    kaze_pos_z = kaze_start_z;
    n = 5;
    if (demo_mode != 0) {
        *(int *)(kaze_state_base_ptr + 0x18) = n;
    } else {
        *(int *)(kaze_state_base_ptr + 0x18) =
            *(unsigned int *)(kaze_state_base_ptr +
                              (kaze_start_z / 0xED8 * 0x80 + kaze_start_x / 0xED8) * 4 +
                              0x5B6E0) >> 28;
    }

    kaze_pos_y = *(int *)(kaze_state_base_ptr + 0x18) * -0x76C - 200;
    n = *(int *)(kaze_state_base_ptr + 0x18);
    if (n >= 0) {
        idx = 4;
        if (n < 5) {
            idx = n;
        }
    } else {
        idx = 0;
    }

    ang0 = *(int *)((int)&kaze_wind_layer0_dir + idx * 8);
    kaze_gas_initial = 0x64000;
    kaze_gas = 0x64000;
    kaze_mode_state = 3;
    kaze_wind_previous_strength = 0;
    kaze_horizontal_speed = 0;
    kaze_wind_active_strength = 0;
    kaze_extra_strength = 0;
    kaze_extra_angle = 0;
    kaze_target_gas = 0;
    kaze_gas_floor = 0x46000;
    kaze_burner_ramp = 0;
    kaze_landing_lockout = 0;
    kaze_vertical_penalty = 0;
    kaze_vertical_wind = 0;
    kaze_vertical_control = 0;
    kaze_impact_flash_timer = 0;
    kaze_burner_state = 0;
    kaze_balloon_scale_x = 0x1000;
    kaze_balloon_scale_y = 0x1000;
    kaze_balloon_scale_z = 0x1000;
    kaze_delta_x = 0;
    kaze_delta_y = 0;
    kaze_delta_z = 0;
    {
        char *pz = kaze_state_base_ptr + 0x58A74;
        kaze_wind_current_angle = ang0 << 9;
        kaze_motion_angle = kaze_wind_current_angle;
        kaze_wind_previous_angle = kaze_wind_current_angle;
        kaze_wind_active_angle = kaze_wind_current_angle;
        kaze_balloon_angle = kaze_wind_current_angle;
        *(int *)(kaze_state_base_ptr + 0x1C) = 0;
        *(int *)(kaze_state_base_ptr + 0x20) = 0;
        bzero(pz, 0x398);
    }

    if (kaze_stage_variant == 1) {
        kaze_mode_state = kaze_stage_variant;
        *(int *)(kaze_state_base_ptr + 0x58AD0) = 0x80000000;
        *(int *)(kaze_state_base_ptr + 0x58A74) = 0x80000000;
        *(int *)(kaze_state_base_ptr + 0x58ADC) = 0;
        *(int *)(kaze_state_base_ptr + 0x58A80) = 0;
        *(int *)(kaze_state_base_ptr + 0x58A7C) = 0x13F24;
        *(int *)(kaze_state_base_ptr + 0x58A84) = 0x22CA4;
        *(int *)(kaze_state_base_ptr + 0x58AD8) = 0x685EC;
        *(int *)(kaze_state_base_ptr + 0x58AE0) = 0x67714;
    }

    kaze_mode1_slot_populate();
    kaze_dynamic_object_spawner(kaze_stage_index[0].a);

    {
        char *p6 = kaze_state_base_ptr;
        int h;

        *(int *)(p6 + 0x60) = 0;
        if (demo_mode != 0) {
            int angt;
            *(short *)(p6 + 0x48) = 0x200;
            angt = kaze_wind_current_angle;
            *(short *)(p6 + 0x4C) = 0;
            h = angt + 0x800;
        } else {
            int ange;
            *(short *)(p6 + 0x48) = 0x400;
            ange = kaze_wind_current_angle;
            *(short *)(p6 + 0x4C) = 0;
            h = 0x1A33 - ange;
        }
        *(unsigned short *)(p6 + 0x4A) = h & 0xFFF;
    }

    {
        char *p7 = kaze_state_base_ptr;
        int t;

        *(int *)(p7 + 0x50) = *(int *)((int)&kaze_camera_anchor_target_x + *(int *)(p7 + 0x60) * 32);
        *(int *)(p7 + 0x54) = *(int *)((int)&kaze_camera_anchor_target_y + *(int *)(p7 + 0x60) * 32);
        *(int *)(p7 + 0x58) = *(int *)((int)&kaze_camera_anchor_target_z + *(int *)(p7 + 0x60) * 32);
        t = kaze_pos_x;
        n = 0;
        *(int *)(p7 + 0x68) = t;
        *(int *)(p7 + 0x78) = t;
        t = kaze_pos_y;
        *(int *)(p7 + 0x6C) = t;
        *(int *)(p7 + 0x7C) = t;
        t = kaze_pos_z;
        *(int *)(p7 + 0x90) = 0;
        *(int *)(p7 + 0x8C) = 0;
        *(int *)(p7 + 0x88) = 0;
        *(int *)(p7 + 0x492BC) = 0;
        *(int *)(p7 + 0x70) = t;
        *(int *)(p7 + 0x80) = t;
        do {
            *(int *)(p7 + 0x492A4 + n * 4) = 0;
            n++;
        } while (n < 3);
    }

    {
        char *p8 = kaze_state_base_ptr;

        *(short *)(p8 + 0x48E16) = 0;
        *(short *)(p8 + 0x48E14) = 0;
        *(short *)(p8 + 0x48E12) = 0;
        *(short *)(p8 + 0x48E10) = 0;
        *(int *)(p8 + 0x14) = demo_mode != 0 ? 0x3000B7 : 0x2000030;
    }

    {
        char *p9 = kaze_state_base_ptr;
        int t;

        *(int *)(p9 + 0x48280) = 0;
        *(int *)(p9 + 0xC) = 0;
        *(int *)(p9 + 0x10) = 0;
        *(int *)(p9 + 0x48284) = 0;
        *(int *)(p9 + 0x4A190) = 0;
        t = kaze_live_stage_weather - 1;
        *(int *)(p9 + 0xECAE0) = t;
        if (t < 0) {
            *(int *)(kaze_state_base_ptr + 0xECAE0) = rand() % 9;
        }
    }
    *(int *)(kaze_state_base_ptr + 0xECAE4) = *(int *)(kaze_state_base_ptr + 0xECAE0);

    kaze_min_gas_init();
    kaze_stage_weather_fx_roll();
    kaze_sfx_voice_keyoff_34170(0);

    {
        char *wk = kaze_state_base_ptr;

        kaze_input_prev_pad2_mask = 0;
        kaze_input_current_pad2_mask = 0;
        kaze_input_current_b = 0;
        kaze_input_current_a = 0;
        *(int *)(wk + 0x481F8) = 0;
        *(int *)(wk + 0x481FC) = 0;
        *(int *)(wk + 0x48200) = 0;
        *(int *)(wk + 0xECAE8) = 0;
        *(int *)(wk + 0xECAEC) = 0;
        *(int *)(wk + 0xECAF0) = 0;
        *(int *)(wk + 0xECAFC) = 0;
        *(int *)(wk + 0xECAF8) = 0;
        *(int *)(wk + 0xECAF4) = 0xF;
        if (demo_mode != 0) {
            int t5648;
            int t1790;
            int t17d0;
            int t33a0;
            register int x __asm__("$2");
            char *w1b;

            t5648 = kaze_input_bind_burner;
            t1790 = kaze_input_bind_descend;
            t17d0 = kaze_input_bind_left;
            t33a0 = kaze_input_bind_confirm;
            *(int *)(wk + 0xED428) = 0xFF;
            *(int *)(wk + 0xED388) = 0x12C;
            *(int *)(wk + 0xED390) = 0xB4;
            *(int *)(wk + 0xED3A0) = 0xB4;
            *(int *)(wk + 0xED3B0) = 0x3C;
            x = kaze_camera_invert_pitch;
            *(int *)(wk + 0xED398) = 0x78;
            *(int *)(wk + 0xED3A8) = 0x78;
            *(int *)(wk + 0xED3B8) = 0x78;
            *(int *)(wk + 0xED42C) = 0;
            *(int *)(wk + 0xED38C) = 0;
            *(int *)(wk + 0xED39C) = 0;
            *(int *)(wk + 0xED394) = t5648;
            *(int *)(wk + 0xED3A4) = t1790;
            *(int *)(wk + 0xED3AC) = t17d0;
            *(int *)(wk + 0xED3B4) = t33a0;
            w1b = wk;
            __asm__("" : "=r"(wk) : "0"(wk));
            if (x != 0) {
                x = kaze_input_bind_down;
            } else {
                x = kaze_input_bind_up;
            }
            *(int *)(w1b + 0xED3BC) = x;

            {
                register char *w2 __asm__("$2") = kaze_state_base_ptr;
                int x2;

                *(int *)(w2 + 0xED3C0) = 0x78;
                *(int *)(w2 + 0xED3C8) = 0x78;
                x2 = kaze_camera_invert_pitch;
                *(int *)(w2 + 0xED3C4) = 0;
                if (x2 != 0) {
                    x2 = kaze_input_bind_up;
                } else {
                    x2 = kaze_input_bind_down;
                }
                *(int *)(w2 + 0xED3CC) = x2;
            }

            {
                register char *w3 __asm__("$2");
                char *w3b;
                register int x3 __asm__("$3");
                int t33a0b;

                t33a0b = kaze_input_bind_confirm;
                w3 = kaze_state_base_ptr;
                *(int *)(w3 + 0xED3DC) = kaze_input_bind_marker_drop;
                *(int *)(w3 + 0xED3E0) = 0xB4;
                *(int *)(w3 + 0xED3F0) = 0x78;
                x3 = kaze_camera_invert_pitch;
                *(int *)(w3 + 0xED3D0) = 0x3C;
                *(int *)(w3 + 0xED3D8) = 0x3C;
                *(int *)(w3 + 0xED3E8) = 0x3C;
                *(int *)(w3 + 0xED3F8) = 0x3C;
                *(int *)(w3 + 0xED3D4) = 0;
                *(int *)(w3 + 0xED3E4) = 0;
                *(int *)(w3 + 0xED3EC) = t33a0b;
                *(int *)(w3 + 0xED3F4) = 0;
                w3b = w3;
                if (x3 != 0) {
                    x3 = kaze_input_bind_up;
                } else {
                    x3 = kaze_input_bind_down;
                }
                x3 = x3 | kaze_input_bind_burner;
                *(int *)(w3b + 0xED3FC) = x3;
            }

            __asm__ volatile("" ::: "memory");
            {
                char *w4;
                int t5648c;

                w4 = kaze_state_base_ptr;
                t5648c = kaze_input_bind_burner;
                *(int *)(w4 + 0xED400) = 0x78;
                *(int *)(w4 + 0xED408) = 0xB4;
                *(int *)(w4 + 0xED404) = t5648c;
                *(int *)(w4 + 0xED40C) = 0;
                *(int *)(w4 + 0xED410) = -1;
                *(int *)(w4 + 0xED414) = -1;
            }
        }
    }
}

extern int kaze_stage_mode;
extern int rsin(int);
extern int rcos(int);
extern int kaze_terrain_collision_neighbor_query(int *, int, void *);

/**
 * kaze_mode1_slot_populate  (objective, 0x8001c51c)
 *
 * Populates mode-1 objective slots at base+0x58e14/+0x58e18/+0x58e1c from
 * the static target table at 0x8008c6a4 when the live stage mode is 1.
 */

void kaze_mode1_slot_populate(void)
{
    extern void bzero(void *, int);
    extern int kaze_pos_x[];
    extern int kaze_stage_index;
    extern int kaze_state_base_ptr;
    int i;
    int b;
    int c;
    int off1;
    int *ip;
    int *vp;
    int reserved0;
    int zt;
    int u2;
    int p2;
    int zv;
    int off2;
    int acc;
    int i6;
    int tr;
    int q;
    int t;
    int p;
    int ps;
    int j;
    int j2;
    int j3;
    int c2;
    int cnt;
    int k7;
    int k50;
    int k84;
    int k4096;
    int *wind;
    int cand_pos[4];
    uchar collision_out[16];

    bzero((uchar *)(kaze_state_base_ptr + 0x58e0c), 0x210);
    if (kaze_stage_mode == 1) goto MODE1;
    if (kaze_stage_mode < 2) goto END;
    i = 0;
    if (kaze_stage_mode == 2) goto MODE2;
    goto END;

MODE1:
    i = 0;
    c = kaze_state_base_ptr;
    do {
        b = c + i * 0x58;
        *(int *)(b + 0x58e58) = 0x84000000;
        *(int *)(b + 0x58e14) = 0xed800;
        *(int *)(b + 0x58e1c) = 0xed800;
        i = i + 1;
    } while (i < 6);
    c = kaze_state_base_ptr;
    i = 0;
    if (0 < *(int *)(c + 0x24)) {
        k7 = 7;
        ip = &kaze_stage_index;
        k50 = 0x50000000;
        k84 = 0x84000000;
        k4096 = 0x1000;
        off1 = 0;
        do {
            *(int *)(c + off1 + 0x58e0c) = k7;
            *(int *)(c + off1 + 0x58e10) = 0;
            j = *ip * 0x10;
            *(int *)(c + off1 + 0x58e14) = *(int *)(&kaze_mode1_target_x_table[j]);
            j2 = *ip * 0x10;
            *(int *)(c + off1 + 0x58e18) = *(int *)(&kaze_mode1_target_y_table[j2]);
            j3 = *ip * 0x10;
            zt = *(int *)(&kaze_mode1_target_z_table[j3]);
            *(int *)(c + off1 + 0x58e4c) = 0;
            *(int *)(c + off1 + 0x58e48) = 0;
            *(int *)(c + off1 + 0x58e5c) = 0;
            *(int *)(c + off1 + 0x58e60) = 0;
            *(int *)(c + off1 + 0x58e54) = k50;
            *(int *)(c + off1 + 0x58e58) = k84;
            *(short *)(c + off1 + 0x58e3c) = 0;
            *(short *)(c + off1 + 0x58e3e) = 0;
            *(short *)(c + off1 + 0x58e40) = 0;
            *(int *)(c + off1 + 0x58e24) = k4096;
            *(int *)(c + off1 + 0x58e28) = k4096;
            *(int *)(c + off1 + 0x58e2c) = k4096;
            *(int *)(c + off1 + 0x58e1c) = zt;
            cnt = *(int *)(c + 0x24);
            asm("" : "=r"(i), "=r"(off1) : "0"(i), "1"(off1), "r"(cnt));
            i = i + 1;
            off1 = off1 + 0x58;
        } while (i < cnt);
    }
    vp = &kaze_stage_variant;
    if (*vp == 2) {
        p2 = kaze_state_base_ptr;
        *(int *)(p2 + 0x58e14) = kaze_mode1_target_variant2_x;
        *(int *)(p2 + 0x58e18) = kaze_mode1_target_variant2_y;
        *(int *)(p2 + 0x58e1c) = kaze_mode1_target_variant2_z;
    }
    i = 0;
    if (*vp == 5) {
        off1 = 0x30;
        c2 = kaze_state_base_ptr;
        do {
            b = c2 + i * 0x58;
            *(int *)(b + 0x58e14) = *(int *)(&kaze_mode1_target_x_table[off1]);
            *(int *)(b + 0x58e18) = *(int *)(&kaze_mode1_target_y_table[off1]);
            *(int *)(b + 0x58e1c) = *(int *)(&kaze_mode1_target_z_table[off1]);
            i = i + 1;
            off1 = off1 + 0x10;
        } while (i < 3);
    }
    goto END;

MODE2:
    do {
        ps = kaze_state_base_ptr + i * 0x58;
        *(int *)(ps + 0x58e10) = 0;
        *(int *)(ps + 0x58e58) = 0;
        *(int *)(ps + 0x58e54) = 0;
        *(int *)(ps + 0x58e60) = 0;
        *(int *)(ps + 0x58e0c) = i % 3 + 3;
        *(int *)(ps + 0x58e48) = (i * 0x1000) / 6 + 0x800;
        *(int *)(ps + 0x58e4c) = (i % 3) * 0x19 + 0x32;
        wind = kaze_pos_x;
        u2 = 1;
        if (kaze_stage_variant == 8) {
            u2 = 0x32;
        }
        asm("" : "=r"(ps) : "0"(ps));
        *(int *)(ps + 0x58e5c) = u2;
        *(short *)(ps + 0x58e3c) = 0;
        *(short *)(ps + 0x58e40) = 0;
        *(int *)(ps + 0x58e24) = 0x1000;
        *(int *)(ps + 0x58e28) = 0x1000;
        *(int *)(ps + 0x58e2c) = 0x1000;
        *(ushort *)(ps + 0x58e3e) =
            (short)*(int *)(ps + 0x58e48) + 0xc00U & 0xfff;
        *(int *)(kaze_state_base_ptr + (i * 0x2c << 1) + 0x58e50) = (rand() % 5 + 3) * 0x3c;
        tr = rsin((i * 0x1000) / 6) * 0x5d10;
        if (tr < 0) {
            tr = tr + 0xfff;
        }
        cand_pos[0] = wind[0] + (tr >> 0xc);
        tr = rcos((i * 0x1000) / 6) * 0x5d10;
        if (tr < 0) {
            tr = tr + 0xfff;
        }
        i6 = 0x5d10;
        while (1) {
            cand_pos[1] = 0x1cb6;
            cand_pos[2] = wind[2] - (tr >> 0xc);
            for (;; cand_pos[1] += 0x3b6) {
                if (kaze_terrain_collision_neighbor_query(cand_pos, 0x578, collision_out) == 0)
                    goto LAB_8001cac8;
                if (!(cand_pos[1] < 0x3e1c)) break;
            }
            i6 = i6 - 0x3b6;
            if (i6 < 0x400) break;
            tr = rsin((i * 0x1000) / 6) * i6;
            if (tr < 0) {
                tr = tr + 0xfff;
            }
            cand_pos[0] = wind[0] + (tr >> 0xc);
            tr = rcos((i * 0x1000) / 6) * i6;
            if (tr < 0) {
                tr = tr + 0xfff;
            }
        }
LAB_8001cac8:
        p = kaze_state_base_ptr;
        p = p + i * 0x58;
        *(int *)(p + 0x58e14) = cand_pos[0];
        *(int *)(p + 0x58e1c) = cand_pos[2];
        i = i + 1;
        t = cand_pos[1] - 700;
        *(int *)(p + 0x58e18) = -t;
        *(int *)(p + 0x58e44) = t / 0x76c;
    } while (i < 6);
END:
    return;
}

extern char kaze_route_node_next_table[];
extern char kaze_route_node_x_table[];
extern char kaze_route_node_y_table[];
extern char kaze_route_node_z_table[];
extern long SquareRoot0(int);
extern long ratan2(int, int);
extern void kaze_dynamic_object_spawn_slot(int, int, int, VEC *, SVEC *, VEC *, VEC *, SVEC *, SVEC *);

/**
 * kaze_dynamic_object_spawner  (objective, 0x8001cb74)
 *
 * Per-stage dynamic-object spawner: zeroes all 18 slots of the +0x5901c
 * table, then spawns records from the per-stage placement tables kaze_dynobj
 * _placement_table_0/kaze_dynobj_placement_table_1/kaze_dynobj_placement_tab
 * le_2 (0x40-byte records; types 3/10/6 derive orientation from node pairs
 * in kaze_route_node_next_table) via kaze_dynamic_object_spawn_slot.
 */

void kaze_dynamic_object_spawner(int arg)
{
    extern u8 *kaze_state_base_ptr;
    u8 *p;
    int i;
    int *p8;
    int type;
    int idx2;
    int dx, dy, dz;
    long r;
    VEC pos;
    VEC pos2;
    VEC vel;
    SVEC rot;
    SVEC rot2;
    SVEC misc;
    int off;

    i = 0x11;
    p = kaze_state_base_ptr + 0x7F8;
    do {
        *(int *)(p + 0x5901C) = 0;
        i--;
        p -= 0x78;
    } while (i >= 0);

    switch (arg) {
    case 0:
        p8 = &kaze_dynobj_placement_table_0;
        break;
    case 1:
        p8 = &kaze_dynobj_placement_table_1;
        break;
    case 2:
        p8 = &kaze_dynobj_placement_table_2;
        break;
    default:
        return;
    }
    i = 0;
    do {
        type = *p8;
        if (type == 0) {
            return;
        }
        if (type == 3 || type == 10 || type == 6) {
            off = p8[8] * 0x10;
            idx2 = *(int *)(kaze_route_node_next_table + off);
            pos.vx = *(int *)(kaze_route_node_x_table + off);
            pos.vy = *(int *)(kaze_route_node_y_table + off);
            pos.vz = *(int *)(kaze_route_node_z_table + off);
            off = idx2 * 0x10;
            pos2.vx = *(int *)(kaze_route_node_x_table + off);
            pos2.vy = *(int *)(kaze_route_node_y_table + off);
            pos2.vz = *(int *)(kaze_route_node_z_table + off);
            vel.vx = 0;
            vel.vy = p8[9];
            vel.vz = p8[10];
            dx = (pos2.vx - pos.vx) / 16;
            dy = (pos2.vy - pos.vy) / 16;
            dz = (pos2.vz - pos.vz) / 16;
            r = SquareRoot0(dx * dx + dz * dz);
            r = ratan2(dy, r);
            rot2.vx = (0x1000 - r) & 0xFFF;
            r = ratan2(dx, dz);
            rot2.vy = (0x1000 - r) & 0xFFF;
            rot2.vz = 0;
            rot.vx = 0;
            rot.vy = rot2.vy - 0x400;
            rot.vz = 0x1000 - rot2.vx;
            misc.vx = p8[15];
            misc.vy = idx2;
            misc.vz = p8[14];
        } else {
            pos.vx = p8[2];
            pos.vy = p8[3];
            pos.vz = p8[4];
            pos2.vx = p8[5];
            pos2.vy = p8[6];
            pos2.vz = p8[7];
            vel.vx = p8[8];
            vel.vy = p8[9];
            vel.vz = p8[10];
            rot.vx = *(u16 *)(p8 + 11);
            rot.vy = *(s16 *)((int)p8 + 0x2E);
            rot.vz = *(u16 *)(p8 + 12);
            rot2.vx = *(u16 *)((int)p8 + 0x32);
            rot2.vy = *(u16 *)(p8 + 13);
            rot2.vz = *(u16 *)((int)p8 + 0x36);
            misc.vx = p8[14];
            misc.vy = 0;
            misc.vz = 0;
        }
        kaze_dynamic_object_spawn_slot(i, *p8, p8[1], &pos, &rot, &pos2, &vel, &rot2, &misc);
        i++;
        p8 += 0x10;
    } while (i < 0x12);
}
