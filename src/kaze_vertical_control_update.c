/**
 * kaze_vertical_control_update  (flight, 0x8001fab8)
 *
 * Vertical control/burner path updating vertical control, vertical wind,
 * landing lockout, and vertical penalty fields at 0xb1764..0xb1770.
 */


struct KZ {
    int f00[7];
    int latch;      /* 0x1c */
    int f20[33];
    int fa4;
};

extern char *kaze_state_base_ptr;
extern int kaze_delta_y[];          /* array view: [0]=delta_y, [30]=vertical control */
extern unsigned int kaze_wind_active_angle[]; /* array view: [0]=wind angle, [9]=vertical control */
extern unsigned int kaze_motion_angle;   /* motion angle */
extern int kaze_horizontal_speed;            /* horizontal speed */

extern int DELTA_S asm("kaze_delta_y");         /* scalar view of delta_y */
extern unsigned int WIND_S asm("kaze_wind_active_angle"); /* scalar view of wind angle */

void kaze_vertical_control_update(unsigned int flags_arg, int min_speed_arg)
{
    register unsigned int flags asm("$4") = flags_arg;
    register int min_speed asm("$5") = min_speed_arg;
    char *root_tmp;
    char *root;
    unsigned int stack_pad0;
    unsigned int umo;
    unsigned int uw;
    int tmp;
    int latch_val;
    int stack_pad1;
    int stack_pad2;
    int *v;
    int *v2;

    if ((flags & 0x77) == 0x77) {
        register unsigned int *q asm("$4") = kaze_wind_active_angle;
        unsigned int w;
        unsigned int mo;
        asm volatile("" : "=r"(q) : "0"(q));
        w = q[0];
        mo = kaze_motion_angle;
        q[0] = w + 0x800 & 0xfff;
        kaze_motion_angle = mo + 0x800 & 0xfff;
        *(int *)(kaze_state_base_ptr + 0x20) = 1;
        if (0 < DELTA_S) {
            tmp = DELTA_S * -2;
            ((int *)q)[9] = tmp;
            q += 9;
            {
                register int sel asm("$2");
                if (-0x81 < tmp) {
                    sel = tmp;
                    if (0 < sel) {
                        sel = 0;
                    }
                } else {
                    sel = -0x80;
                }
                *(int *)q = sel;
            }
        } else if (DELTA_S < 0) {
            tmp = DELTA_S * -2;
            ((int *)q)[9] = tmp;
            {
                register int *v2p asm("$6");
                register int sel asm("$4");
                v2p = (int *)q + 9;
                if (-1 < tmp) {
                    sel = 0x80;
                    if (tmp < 0x81) {
                        sel = tmp;
                    }
                } else {
                    sel = 0;
                }
                *v2p = sel;
            }
        }
        goto LAB_8001fe48;
    }
    if ((flags & 1) != 0) {
        unsigned int *m = &kaze_motion_angle;
        if (m[0] < 0x801) {
            unsigned int umo1, uw1;
            flags = flags & 0xffffffef;
            asm volatile("" :: "r"(flags));
            umo1 = m[0];
            umo1 = umo1 - 0x1000;
            umo1 = -umo1;
            umo1 = umo1 & 0xfff;
            m[0] = umo1;
            uw1 = WIND_S;
            uw1 = uw1 - 0x1000;
            uw1 = -uw1;
            uw1 = uw1 & 0xfff;
            WIND_S = uw1;
            {
                unsigned int uv1;
                if (0xf7f < (int)umo1) {
                    uv1 = 0xf80;
                } else {
                    if (0x880 < (int)umo1) goto LAB_8001fc00;
                    uv1 = 0x880;
                }
                m[0] = uv1;
            }
        }
LAB_8001fc00:
        *(int *)(kaze_state_base_ptr + 0x20) = 1;
    }
    if ((flags & 0x10) != 0) {
        unsigned int *m = &kaze_motion_angle;
        if (0x7ff < (int)m[0]) {
            unsigned int newm, umo2, uw2;
            umo2 = m[0] - 0x1000;
            umo2 = -umo2;
            newm = umo2 & 0xfff;
            m[0] = newm;
            uw2 = WIND_S;
            uw2 = uw2 - 0x1000;
            uw2 = -uw2;
            uw2 = uw2 & 0xfff;
            WIND_S = uw2;
            {
                unsigned int uv2;
                if ((int)newm < 0x81) {
                    uv2 = 0x80;
                } else {
                    if ((int)newm < 0x780) goto LAB_8001fc7c;
                    uv2 = 0x780;
                }
                m[0] = uv2;
            }
        }
LAB_8001fc7c:
        *(int *)(kaze_state_base_ptr + 0x20) = 1;
    }
    if ((flags & 2) != 0) {
        unsigned int *m = &kaze_motion_angle;
        if (m[0] - 0x400 < 0x801) {
            unsigned int umo3, uw3;
            flags = flags & 0xffffffdf;
            asm volatile("" :: "r"(flags));
            umo3 = m[0];
            umo3 = umo3 - 0x1800;
            umo3 = -umo3;
            umo3 = umo3 & 0xfff;
            m[0] = umo3;
            uw3 = WIND_S;
            uw3 = uw3 - 0x1800;
            uw3 = -uw3;
            uw3 = uw3 & 0xfff;
            WIND_S = uw3;
            {
                unsigned int uv3;
                if (umo3 - 0x800 < 0x381) {
                    uv3 = 0xb80;
                } else {
                    if (0x480 < umo3 - 0x380) goto LAB_8001fd0c;
                    uv3 = 0x380;
                }
                m[0] = uv3;
            }
        }
LAB_8001fd0c:
        *(int *)(kaze_state_base_ptr + 0x20) = 1;
    }
    if ((flags & 0x20) != 0) {
        unsigned int *m = &kaze_motion_angle;
        if (0x7fe < m[0] - 0x401) {
            unsigned int newm, umo4, uw4;
            umo4 = m[0] - 0x1800;
            umo4 = -umo4;
            newm = umo4 & 0xfff;
            m[0] = newm;
            uw4 = WIND_S;
            uw4 = uw4 - 0x1800;
            uw4 = -uw4;
            uw4 = uw4 & 0xfff;
            WIND_S = uw4;
            {
                unsigned int uv4;
                if ((int)newm < 0x481) {
                    uv4 = 0x480;
                } else {
                    if ((int)newm < 0xb80) goto LAB_8001fd8c;
                    uv4 = 0xb80;
                }
                m[0] = uv4;
            }
        }
LAB_8001fd8c:
        *(int *)(kaze_state_base_ptr + 0x20) = 1;
    }
    if ((flags & 4) != 0) {
        unsigned int mk = 0xffffffbf;
        register int *d asm("$7") = kaze_delta_y;
        asm volatile("" : "=r"(d) : "0"(d));
        flags = flags & mk;
        if (0 < d[0]) {
            int raw;
            int sel;
            raw = d[0] * -4;
            d[30] = raw;
            d += 30;
            if (-0x81 < raw) {
                sel = -0x50;
                if (raw < -0x4f) {
                    sel = raw;
                }
            } else {
                sel = -0x80;
            }
            *d = sel;
        }
    }
    if ((flags & 0x40) != 0) {
        register int *d asm("$3") = kaze_delta_y;
        asm volatile("" :: "r"(flags));
        asm volatile("" : "=r"(d) : "0"(d));
        if (d[0] < 0) {
            int raw;
            register int *vv asm("$6");
            register int sel asm("$3");
            raw = d[0] * -4;
            d[30] = raw;
            vv = d + 30;
            if (0x4f < raw) {
                sel = 0x80;
                if (raw < 0x81) {
                    sel = raw;
                }
            } else {
                sel = 0x50;
            }
            *vv = sel;
        }
    }
LAB_8001fe48:
    {
        register int *h asm("$6") = &kaze_horizontal_speed;
        if (h[0] < min_speed) {
            h[0] = min_speed;
        }
        root_tmp = kaze_state_base_ptr;
        asm volatile("" :: "r"(min_speed));
        tmp = *(int *)(kaze_state_base_ptr + 0xa4) * 0x28;
        if (h[0] < tmp) {
            h[0] = tmp;
        }
        tmp = *(int *)(kaze_state_base_ptr + 0x1c) + 1;
        *(int *)(kaze_state_base_ptr + 0x1c) = tmp;
        root = kaze_state_base_ptr;
        if (-1 < tmp) {
            int cmpv = 99999999;
            asm("" : "=r"(cmpv) : "0"(cmpv));
            latch_val = 99999999;
            if (!(cmpv < tmp)) {
                latch_val = tmp;
            }
        } else {
            latch_val = 0;
        }
        ((struct KZ *)root)->latch = latch_val;
        *(unsigned int *)(kaze_state_base_ptr + 0x14) = *(unsigned int *)(kaze_state_base_ptr + 0x14) & 0xffffff7f;
    }
    return;
}
