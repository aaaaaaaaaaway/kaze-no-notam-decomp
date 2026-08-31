/**
 * kaze_wind_motion_update  (flight, 0x8001edd0)
 *
 * Per-frame wind/motion update: selects the active wind layer, blends
 * transitions (wind transition latch +0x20), and updates motion angle/speed
 * fields at 0xb173c..0xb1760.
 */


#include "kaze_syms.h"
extern int kaze_delta_z;
extern int kaze_extra_angle;
extern int kaze_extra_strength;
extern int kaze_landing_lockout;
extern int kaze_vertical_control;
extern int kaze_vertical_wind;
extern int kaze_wind_active_strength;
extern int kaze_wind_layer0_strength;
extern int kaze_wind_previous_angle;
extern int kaze_wind_previous_strength;
extern int kaze_balloon_angle;
extern int kaze_pos_y;
extern int kaze_approach_angle(int, int, int);
extern int kaze_gas_floor_reset(int, int);
extern int kaze_state_base_ptr;
extern int kaze_wind_layer0_dir;
extern int kaze_delta_y;
extern int kaze_impact_flash_timer;
extern int kaze_wind_current_angle;
extern int kaze_wind_active_angle;
extern int kaze_motion_angle;

extern int rsin(int);
extern int rcos(int);
extern int ratan2(int, int);
extern int SquareRoot0(int);

void kaze_wind_motion_update(void)
{
    int strength_new;
    int idx;
    long angle_raw;
    unsigned int balloon_raw;
    int alt;
    int *wp;
    int *wq;
    int target_strength;
    register int var_v1_5 asm("$3");
    int var_v0_4;

    alt = -kaze_pos_y / 0x76c + -3;
    if (alt >= 0) {
        idx = 4;
        if (alt < 5) {
            idx = alt;
        }
    } else {
        idx = 0;
    }
    {
        register int idx8 asm("$2");
        register int dirv asm("$3");
        register int nv asm("$5");
        register int *ap asm("$6");
        idx8 = idx << 3;
        dirv = *(int *)((char *)&kaze_wind_layer0_dir + idx8);
        __asm__ volatile("" ::: "memory");
        ap = &kaze_wind_current_angle;
        __asm__("" : "=r"(ap) : "0"(ap));
        nv = dirv << 9;
        ap[0] = nv;
        target_strength = *(int *)((char *)&kaze_wind_layer0_strength + idx8);
        if ((ap[1] != nv) || (*(int *)(kaze_state_base_ptr + 0x20) != 0)) {
            ap[1] = nv;
            kaze_wind_active_strength = 0;
            kaze_wind_previous_angle = kaze_motion_angle;
            kaze_wind_previous_strength = kaze_horizontal_speed;
            *(int *)(kaze_state_base_ptr + 0x20) = 0;
        }
    }
    wp = &kaze_wind_previous_angle;
    __asm__ ("" : "=r"(wp) : "0"(wp));
    {
    int t0 = rsin(wp[0]);
    t0 = -(t0 * wp[1]);
    if (t0 < 0) {
        t0 = t0 + 0xfff;
    }
    wp[-24] = t0 >> 0xc;
    }
    {
    int t1 = rcos(wp[0]);
    t1 = t1 * wp[1];
    if (t1 < 0) {
        t1 = t1 + 0xfff;
    }
    kaze_delta_z = t1 >> 0xc;
    }
    wq = wp - 1;
    {
    int t2 = rsin(kaze_wind_active_angle);
    {
        register int td asm("$3");
        td = t2 * wp[-1];
        t2 = td * *(int *)(kaze_state_base_ptr + 0xa4);
    }
    if (t2 < 0) {
        t2 = t2 + 0xfff;
    }
    wp[-24] = wp[-24] - (t2 >> 0xc);
    }
    {
    int t3 = rcos(kaze_wind_active_angle);
    {
        register int te asm("$3");
        te = t3 * wp[-1];
        t3 = te * *(int *)(kaze_state_base_ptr + 0xa4);
    }
    if (t3 < 0) {
        t3 = t3 + 0xfff;
    }
    kaze_delta_z = kaze_delta_z + (t3 >> 0xc);
    }
    {
    int t4 = rsin(kaze_extra_angle);
    t4 = t4 * kaze_extra_strength;
    if (t4 < 0) {
        t4 = t4 + 0xfff;
    }
    wp[-24] = wp[-24] + (t4 >> 0xc);
    }
    {
    int t5 = rcos(kaze_extra_angle);
    t5 = t5 * kaze_extra_strength;
    if (t5 < 0) {
        t5 = t5 + 0xfff;
    }
    kaze_delta_z = kaze_delta_z - (t5 >> 0xc);
    }
    angle_raw = ratan2(kaze_delta_z, wp[-24]);
    kaze_motion_angle = ((int)((angle_raw - 0x400U & 0xfff) + 8)) / 16 * 16;
    kaze_horizontal_speed = SquareRoot0(kaze_delta_z * kaze_delta_z + wp[-24] * wp[-24]);
    {
    register int *sq asm("$5");
    sq = wq;
    if (wp[-1] < target_strength) {
        strength_new = wp[-1] + *(int *)(kaze_state_base_ptr + 0xa4);
        wp[-1] = strength_new;
        if (strength_new < 0) goto setzero;
        var_v1_5 = target_strength;
        var_v0_4 = var_v1_5 < strength_new;
        goto LAB;
setzero:
        var_v1_5 = 0;
        goto store;
    } else if (target_strength < wp[-1]) {
        strength_new = wp[-1] - *(int *)(kaze_state_base_ptr + 0xa4);
        wp[-1] = strength_new;
        if (strength_new < target_strength) goto setmin;
        {
        int cmp7 = 0x7fffffff;
        __asm__("" : "=r"(cmp7) : "0"(cmp7));
        var_v1_5 = 0x7fffffff;
        var_v0_4 = cmp7 < strength_new;
        }
LAB:
        if (var_v0_4 != 0) goto store;
        var_v1_5 = strength_new;
        goto store;
setmin:
        var_v1_5 = target_strength;
store:
        *sq = var_v1_5;
    }
    }
    {
        int lr2 = kaze_state_base_ptr;
        int *qp = &kaze_wind_previous_strength;
        __asm__("" : "=r"(qp) : "0"(qp));
        {
        register int dec1 asm("$2");
        register int st1 asm("$3");
        st1 = *(int *)(lr2 + 0xa4);
        dec1 = qp[0] - st1;
        qp[0] = dec1;
        if (dec1 < 0) {
            dec1 = 0;
        }
        qp[0] = dec1;
        }
        {
        register int dec2 asm("$2");
        register int st2 asm("$3");
        int w75c = kaze_extra_strength;
        __asm__ volatile("" ::: "memory");
        st2 = *(int *)(lr2 + 0xa4);
        dec2 = w75c - st2;
        kaze_extra_strength = dec2;
        if (dec2 < 0) {
            dec2 = 0;
        }
        kaze_extra_strength = dec2;
        }
        balloon_raw = kaze_approach_angle(kaze_balloon_angle, kaze_wind_current_angle, *(int *)(lr2 + 0xa4) << 4);
    }
    kaze_balloon_angle = balloon_raw & 0xfff;
    kaze_delta_y = kaze_vertical_wind + kaze_vertical_control;
    kaze_vertical_wind = kaze_gas_floor_reset(0xffffffe0, 0x20);
    if (kaze_vertical_control >= 1) {
        kaze_vertical_control = kaze_vertical_control - *(int *)(kaze_state_base_ptr + 0xa4);
        if (-1 < kaze_vertical_control)
            goto LAB_8001f288;
    } else {
        if ((-1 < kaze_vertical_control) ||
            (kaze_vertical_control = kaze_vertical_control + *(int *)(kaze_state_base_ptr + 0xa4), kaze_vertical_control < 1))
            goto LAB_8001f288;
    }
    kaze_vertical_control = 0;
LAB_8001f288:
    {
        int lr = kaze_state_base_ptr;
        int *bp;
        __asm__ volatile("" ::: "memory");
        bp = &kaze_impact_flash_timer;
        __asm__("" : "=r"(bp) : "0"(bp));
        {
        int dec3 = bp[0];
        dec3 = dec3 + *(int *)(lr + 0xa4) * -0x100;
        bp[0] = dec3;
        if (dec3 < 0) {
            bp[0] = 0;
        }
        }
        if (bp[0] < 0x2000) {
            *(unsigned int *)(lr + 0x14) = *(unsigned int *)(lr + 0x14) | 0x80;
        }
    }
    kaze_landing_lockout = kaze_landing_lockout - *(int *)(kaze_state_base_ptr + 0xa4);
    if (kaze_landing_lockout < 0) {
        kaze_landing_lockout = 0;
    }
    return;
}
