#include "types.h"
#include "kaze_syms.h"
extern int kaze_live_stage_level;
extern int kaze_min_gas;
extern int rand(void);
typedef struct {
    int a, b, c, d;
} Slot;

/**
 * kaze_dynamic_object_spawn_slot  (objective, 0x8001ceec)
 *
 * Fills one +0x5901c dynamic-object slot (stride 0x78): type, position,
 * rotation, unit scale, model id (+0x42, negative = remap), brightness 0x80,
 * spin terms, second node position, movement params, extras.
 */

void kaze_dynamic_object_spawn_slot(
    uint slot, int type, short arg2, int *pos, ushort *rot, int *vel, int *accel,
    ushort *scale, short *extra)
{
    extern int kaze_state_base_ptr;
    int *obj;
    unsigned int rz;

    if (slot < 0x12 && type != 0) {
        obj = (int *)(kaze_state_base_ptr + 0x5901c + (((slot << 4) - slot) << 3));
        *(short *)((int)obj + 0x42) = arg2;
        obj[0] = type;
        obj[3] = 0;
        obj[2] = 0;
        obj[4] = pos[0];
        obj[5] = pos[1];
        obj[6] = pos[2];
        *(ushort *)((int)obj + 0x28) = rot[0] & 0xfff;
        *(ushort *)((int)obj + 0x2a) = rot[1] & 0xfff;
        rz = rot[2];
        obj[14] = 0x1000;
        obj[13] = 0x1000;
        obj[12] = 0x1000;
        obj[17] = 0x80;
        *(ushort *)((int)obj + 0x2c) = rz & 0xfff;
        obj[18] = scale[0] & 0xfff;
        obj[19] = scale[1] & 0xfff;
        obj[20] = scale[2] & 0xfff;
        obj[21] = vel[0];
        obj[22] = vel[1];
        obj[23] = vel[2];
        obj[24] = accel[0];
        obj[25] = accel[1];
        obj[26] = accel[2];
        obj[27] = extra[0];
        obj[28] = extra[1];
        obj[29] = extra[2];
    }
}

extern Slot kaze_min_gas_base_table[];
extern int kaze_stage_flags;

/**
 * kaze_min_gas_init  (flight, 0x8001d054)
 *
 * Initializes kaze_min_gas from kaze_min_gas_base_table[stage index],
 * selecting one of that entry's four fields based on kaze_stage_flags
 * (0/1/3/5), shifted left 12.
 */

void kaze_min_gas_init(void)
{
    int i = kaze_live_stage_level - 1;
    int v;

    if (kaze_stage_flags == 1) goto c1;
    if (kaze_stage_flags == 0) goto c0;
    if (kaze_stage_flags == 3) goto c3;
    if (kaze_stage_flags == 5) goto c5;
    return;
c0:
    v = kaze_min_gas_base_table[i].a << 12;
    goto store;
c1:
    v = kaze_min_gas_base_table[i].b << 12;
    goto store;
c3:
    v = kaze_min_gas_base_table[i].c << 12;
    goto store;
c5:
    v = kaze_min_gas_base_table[i].d << 12;
store:
    kaze_min_gas = v;
}

/**
 * kaze_stage_weather_fx_roll  (core, 0x8001d100)
 *
 * Random weather roll: zeroes counters +0xed348/+0xed34c/+0xed378; 1-in-20
 * sets +0xed37c=0xe10 with flag 0x10000000 (variant 3/level 1) or
 * +0xed350=0x2a30 with flag 0x20000000 (variant 0/level 3) in
 * kaze_stage_flags.
 */

void kaze_stage_weather_fx_roll(void)
{
    extern char *kaze_state_base_ptr;
    char *base;
    register int *flags_ptr asm("$16");

    flags_ptr = &kaze_stage_flags;
    {
        register char *init_base asm("$2");

        init_base = kaze_state_base_ptr;
        *(int *)(init_base + 0xED348) = 0;
        *(int *)(init_base + 0xED34C) = 0;
        *(int *)(init_base + 0xED378) = 0;
    }

    if (*flags_ptr == 3 && kaze_live_stage_level == 1) {
        if ((rand() % 20) != 0) {
            return;
        }
        {
            register int timer asm("$2");
            register char *base1 asm("$3");

            timer = 0xE10;
            base1 = kaze_state_base_ptr;
            *(int *)(base1 + 0xED378) = 0;
            *(int *)(base1 + 0xED37C) = timer;
        }
        {
            register int flags asm("$2");
            register int mask asm("$3");

            flags = *flags_ptr;
            mask = 0x10000000;
            *flags_ptr = flags | mask;
        }
        return;
    }

    if (*flags_ptr != 0) {
        return;
    }
    if (kaze_live_stage_level != 3) {
        return;
    }

    if ((rand() % 20) != 0) {
        return;
    }
    {
        register char *base2 asm("$2");
        register int timer asm("$3");
        register int flags asm("$2");
        register int mask asm("$3");

        timer = 0x2A30;
        base2 = kaze_state_base_ptr;
        *(int *)(base2 + 0xED348) = 0;
        *(int *)(base2 + 0xED34C) = 0;
        *(int *)(base2 + 0xED350) = timer;
        flags = *flags_ptr;
        mask = 0x20000000;
        *flags_ptr = flags | mask;
    }
}

extern int kaze_input_current_a;
extern int kaze_input_current_b;
extern int kaze_input_bind_pause;
extern int RotMatrix(int *, int *);
extern int kaze_trans_matrix(int *, int *);

/**
 * kaze_demo_input_playback  (core, 0x8001d274)
 *
 * Scripted/demo input playback gated by state flag 0x00100000: feeds
 * {command,timer} entries at +0xed388 (cursor +0xed42c) into the input mask,
 * ramps the +0xed428 fade, and requests exit via root +0x8/+0x4/+0x0; also
 * rebuilds the camera rotation matrix each frame.
 */

void kaze_demo_input_playback(void)
{
    extern int *kaze_state_base_ptr;
    int *state_ptr;
    int *r;
    int unused_i2;
    int *m;
    unsigned int cmd;

    if ((kaze_state_base_ptr[5] & 0x100000) != 0) {
        if ((((kaze_input_current_a & kaze_input_bind_pause) != 0) && ((kaze_input_current_b & kaze_input_bind_pause) == 0)) ||
            (((kaze_input_current_a & 0x40) != 0) && ((kaze_input_current_b & 0x40) == 0))) {
            kaze_state_base_ptr[2] = 1;
        }
        kaze_input_current_a = 0;
        {
            int *b = kaze_state_base_ptr;
            int *cp;
            cp = b[0x3b50b] * 2 + b;
            cmd = cp[0x3b4e3];
            if (cmd != 0xffffffff) {
                int k;
                int *p;
                k = *(volatile int *)&b[0x3b50b];
                kaze_input_current_a = cmd;
                p = b + k * 2;
                if ((int)p[0x3b4e2] < 1) {
                    b[0x3b50b] = k + 1;
                } else {
                    p[0x3b4e2] = p[0x3b4e2] - b[0x29];
                }
            } else {
                b[2] = 1;
            }
        }
        state_ptr = kaze_state_base_ptr;
        if (kaze_state_base_ptr[2] != 1) {
            int dd = kaze_state_base_ptr[0x29] * -2 + kaze_state_base_ptr[0x3b50a];
            kaze_state_base_ptr[0x3b50a] = dd;
            if (dd < 0) {
                state_ptr[0x3b50a] = 0;
            }
        } else {
            int ii = kaze_state_base_ptr[0x3b50a] + kaze_state_base_ptr[0x29] * 2;
            kaze_state_base_ptr[0x3b50a] = ii;
            if (0xfe < ii) {
                state_ptr[0x3b50a] = 0xff;
                state_ptr[1] = 0;
                state_ptr[0] = 1;
            }
        }
    }
    kaze_stage_completion_check();
    kaze_flight_control_dispatch();
    r = kaze_state_base_ptr + 0x12;
    m = kaze_state_base_ptr + 10;
    *(unsigned short *)((int)kaze_state_base_ptr + 0x4a) =
        *(unsigned short *)((int)kaze_state_base_ptr + 0x4a) & 0xfff;
    RotMatrix(r, m);
    kaze_trans_matrix(kaze_state_base_ptr + 10, kaze_state_base_ptr + 0x14);
    return;
}
