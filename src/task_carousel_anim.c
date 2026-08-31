#include "types.h"
#include "kaze_syms.h"
extern int kaze_task_record_index;
extern int kaze_task_glyph_count;
extern int kaze_task_letter_count;
extern int kaze_task_menu_camera_bob_phase;
extern int kaze_task_carousel_angle_step;
extern int kaze_task_letter_add_spin_amplitude;
extern int kaze_task_letter_add_x_offset_0;
extern int kaze_task_letter_add_row_ease_slope;
extern int kaze_task_letter_delete_arm_timer;
extern int kaze_task_letter_delete_phase1_divisor;
extern int kaze_task_letter_delete_phase2_divisor;
extern int kaze_task_letter_delete_phase1_amplitude;
extern int kaze_task_letter_delete_phase2_amplitude;
extern int kaze_task_carousel_scatter_step_x[];
extern int kaze_task_carousel_scatter_step_y[];
extern int kaze_task_carousel_scatter_target_x[];
extern int kaze_task_carousel_scatter_target_y[];
extern int kaze_task_carousel_scatter_frame_count[];
extern int kaze_menu_task_model_ptr;
extern int kaze_task_carousel_transition_amplitude;
extern int abs(int);
extern int kaze_menu_camera_scroll;
typedef struct { int f0; int f4; int f8; int fc; } S16;
typedef struct { short f0; short f2; short f4; short f6; } S8;
typedef struct { char b[0x50]; } S50;
typedef struct { int f0; void *f4; int f8; int fc; } SD20;
typedef struct { int f0; int pad[19]; } S80;

extern int kaze_task_menu_state;
void kaze_task_carousel_layout_update();
void kaze_task_menu_camera_bob();

/**
 * kaze_task_carousel_autospin_step  (menu, 0x8005b1a4)
 *
 * State-0x2c auto-spin: steps ring base kaze_task_carousel_visible_index and
 * position counter kaze_task_carousel_selected_index in direction
 * kaze_task_carousel_direction, rebuilds the angle ring; at count 0x28 sets
 * menu state kaze_task_menu_state and kaze_task_carousel_input_index=1,
 * ticking sound 0x78 every 3 steps.
 */

void kaze_task_carousel_autospin_step(void)
{
    extern void kaze_sfx_play_dispatch(int);
    extern int kaze_task_carousel_direction;
    extern int kaze_task_carousel_selected_index;
    extern int kaze_task_carousel_input_index;
    extern int kaze_task_carousel_visible_index;
    extern uint kaze_task_carousel_angle_ring[];
    int step;
    int span;
    int index;
    int angle;
    int center;
    uint *dst;

    if (kaze_task_carousel_direction == 0) {
        if (kaze_task_carousel_visible_index > 0) {
            kaze_task_carousel_visible_index = kaze_task_carousel_visible_index - 1;
        } else {
            kaze_task_carousel_visible_index = 0x28;
        }
        if (kaze_task_carousel_selected_index < 0x28) {
            kaze_task_carousel_selected_index = kaze_task_carousel_selected_index + 1;
        } else {
            kaze_task_carousel_selected_index = 0;
        }
    } else if (kaze_task_carousel_direction == 1) {
        if (kaze_task_carousel_visible_index < 0x28) {
            kaze_task_carousel_visible_index = kaze_task_carousel_visible_index + 1;
        } else {
            kaze_task_carousel_visible_index = 0;
        }
        if (kaze_task_carousel_selected_index > 0) {
            kaze_task_carousel_selected_index = kaze_task_carousel_selected_index - 1;
        } else {
            kaze_task_carousel_selected_index = 0x28;
        }
    }

    step = 0;
    center = 0x800;
    dst = kaze_task_carousel_angle_ring;
    index = kaze_task_carousel_visible_index;
    span = kaze_task_carousel_angle_step;
    do {
        angle = index + step;
        if (angle >= 0x29) {
            angle = angle - 0x29;
        }
        step++;
        *dst = (center - ((angle * span) >> 12)) & 0xFFF;
        dst++;
    } while (step < 0x29);

    kaze_task_carousel_layout_update();
    kaze_task_menu_camera_bob();

    if (kaze_task_carousel_selected_index == 0x28) {
        kaze_task_menu_state = kaze_task_carousel_selected_index;
        kaze_task_carousel_input_index = 1;
    }
    if (kaze_task_carousel_selected_index == (kaze_task_carousel_selected_index / 3) * 3) {
        kaze_sfx_play_dispatch(0x78);
    }
}

extern s32 kaze_input_current_c;
extern u32 kaze_input_hold_timer;

/**
 * kaze_task_carousel_rotate_input  (menu, 0x8005b330)
 *
 * Carousel left/right input: press mask kaze_input_edge and held mask
 * kaze_input_current_c with repeat timer kaze_input_hold_timer move target
 * kaze_task_carousel_input_index; computes shortest signed delta to
 * kaze_task_carousel_visible_index and starts rotation (dir
 * kaze_task_carousel_direction, steps
 * kaze_task_carousel_transition_duration/52c) with sound 0x78.
 */

void kaze_task_carousel_rotate_input(void)
{
    extern s32 kaze_input_edge;
    extern s32 kaze_task_carousel_exit_timer;
    extern s32 kaze_task_carousel_transition_duration;
    extern s32 kaze_task_carousel_transition_ticks;
    extern s32 kaze_task_carousel_direction;
    extern s32 kaze_task_carousel_previous_index;
    extern s32 kaze_task_carousel_selected_index;
    extern s32 kaze_task_carousel_input_index;
    extern s32 kaze_task_carousel_visible_index;
    extern void kaze_sfx_play_dispatch(s32);
    int dir1_val;
    int prev_sel;
    s32 ticks;
    int delta;
    register int sel1 asm("$2");

    if (kaze_task_carousel_exit_timer != 0) {
        return;
    }
    if ((kaze_input_edge & 0x2000) != 0) {
        if (kaze_task_carousel_input_index < 1) {
            kaze_task_carousel_input_index = 0x28;
        } else {
            kaze_task_carousel_input_index = kaze_task_carousel_input_index + -1;
        }
    }
    if (((kaze_input_current_c & 0x2000) != 0) && (0x14 < kaze_input_hold_timer) && ((kaze_input_hold_timer & 3) == 0)) {
        if (kaze_task_carousel_input_index < 1) {
            kaze_task_carousel_input_index = 0x28;
        } else {
            kaze_task_carousel_input_index = kaze_task_carousel_input_index + -1;
        }
    }
    if ((kaze_input_edge & 0x8000) != 0) {
        if (kaze_task_carousel_input_index < 0x28) {
            kaze_task_carousel_input_index = kaze_task_carousel_input_index + 1;
        } else {
            kaze_task_carousel_input_index = 0;
        }
    }
    if (((kaze_input_current_c & 0x8000) != 0) && (0x14 < kaze_input_hold_timer) && ((kaze_input_hold_timer & 3) == 0)) {
        if (kaze_task_carousel_input_index < 0x28) {
            kaze_task_carousel_input_index = kaze_task_carousel_input_index + 1;
        } else {
            kaze_task_carousel_input_index = 0;
        }
    }
    delta = kaze_task_carousel_input_index - kaze_task_carousel_visible_index;
    if (0x14 < delta) {
        delta = delta + -0x29;
    }
    if (delta < -0x14) {
        delta = delta + 0x29;
    }
    if (kaze_task_carousel_transition_ticks != 0) {
        return;
    }
    if (delta != 0) {
        kaze_sfx_play_dispatch(0x78);
    }
    if (delta == -1) {
        ticks = 0xc;
        goto dir0;
    }
    if (delta < -1) {
        ticks = 4;
dir0:
        kaze_task_carousel_transition_duration = ticks;
        kaze_task_carousel_transition_ticks = ticks;
        kaze_task_carousel_direction = 0;
        goto LAB_8005b554;
    }
    dir1_val = 1;
    if (delta == dir1_val) {
        ticks = 0xc;
        goto dir1;
    }
    if (delta < 2) {
        return;
    }
    ticks = 4;
dir1:
    kaze_task_carousel_transition_duration = ticks;
    kaze_task_carousel_transition_ticks = ticks;
    kaze_task_carousel_direction = dir1_val;
LAB_8005b554:
    if (kaze_task_carousel_direction == 0) {
        if (kaze_task_carousel_visible_index < 1) {
            kaze_task_carousel_visible_index = 0x28;
        } else {
            kaze_task_carousel_visible_index = kaze_task_carousel_visible_index + -1;
        }
        prev_sel = kaze_task_carousel_selected_index;
        kaze_task_carousel_previous_index = prev_sel;
        if (prev_sel < 0x28) {
            kaze_task_carousel_selected_index = prev_sel + 1;
        } else {
            kaze_task_carousel_selected_index = 0;
        }
        return;
    }
    if (kaze_task_carousel_direction != 1) {
        return;
    }
    if (kaze_task_carousel_visible_index < 0x28) {
        kaze_task_carousel_visible_index = kaze_task_carousel_visible_index + 1;
    } else {
        kaze_task_carousel_visible_index = 0;
    }
    sel1 = kaze_task_carousel_selected_index;
    kaze_task_carousel_previous_index = sel1;
    if (0 < sel1) {
        kaze_task_carousel_selected_index = sel1 + -1;
    } else {
        kaze_task_carousel_selected_index = 0x28;
    }
    return;
}

/**
 * kaze_task_carousel_rotate_interp  (menu, 0x8005b628)
 *
 * Rotation interpolator: while countdown kaze_task_carousel_transition_ticks
 * is active, rebuilds the kaze_task_carousel_angle_ring angle ring from base
 * index and adds a fractional per-frame rotation offset in the current
 * direction; clears kaze_task_carousel_previous_index when done.
 */

void kaze_task_carousel_rotate_interp(void) {
    extern int kaze_task_carousel_transition_duration;
    extern int kaze_task_carousel_transition_ticks;
    extern int kaze_task_carousel_direction;
    extern int kaze_task_carousel_previous_index;
    extern int kaze_task_carousel_visible_index;
    extern int kaze_task_carousel_angle_ring[];
    register int ticks0 __asm__("$2");

    ticks0 = kaze_task_carousel_transition_ticks;
    if (ticks0 != 0) {
        register int i __asm__("$4");
        register int base __asm__("$8");
        register int *angle __asm__("$5");
        register int index __asm__("$7");
        register int span __asm__("$6");
        register int pos __asm__("$3");
        int product;

        i = 0;
        base = 0x800;
        angle = kaze_task_carousel_angle_ring;
        index = kaze_task_carousel_visible_index;
        span = kaze_task_carousel_angle_step;
        kaze_task_carousel_transition_ticks = ticks0 - 1;

        do {
            pos = index + i;
            product = pos * span;
            if (pos >= 0x29) {
                pos -= 0x29;
                product = pos * span;
            }
            i++;
            *angle = (base - (product >> 0xC)) & 0xFFF;
            angle++;
        } while (i < 0x29);

        {
            register int ticks_reload __asm__("$2");
            register int ticks __asm__("$8");
            register int one __asm__("$10");
            register int *p __asm__("$4");
            register int *end __asm__("$9");
            register int direction __asm__("$7");
            register int span2 __asm__("$6");
            register int duration __asm__("$5");
            register int step __asm__("$2");
            int product2;
            int delta;

            ticks_reload = kaze_task_carousel_transition_ticks;
            if (ticks_reload != 0) {
                ticks = ticks_reload;
                __asm__ volatile("addiu %0,$zero,1" : "=r"(one));
                p = kaze_task_carousel_angle_ring;
                end = p + 0x29;
                direction = kaze_task_carousel_direction;
                span2 = kaze_task_carousel_angle_step;
                duration = kaze_task_carousel_transition_duration;
            do {
                if (direction == 0) {
                    step = span2 / duration;
                    product2 = ticks * step;
                    delta = product2 >> 0xC;
                    *p = *p - delta;
                } else if (direction == one) {
                    step = span2 / duration;
                    product2 = ticks * step;
                    delta = product2 >> 0xC;
                    *p = *p + delta;
                }
                p++;
            } while ((int)p < (int)end);
            } else {
                kaze_task_carousel_previous_index = -1;
            }
        }
    }
}

void kaze_task_carousel_scatter_init();

/**
 * kaze_task_carousel_select_input  (menu, 0x8005b7b0)
 *
 * Circle-press select on the carousel: slot 0x27 arms letter delete
 * (kaze_task_letter_delete_arm_timer=0x18, snd 0x79), slot 0x28 ends entry
 * (state 0x2b + scatter init kaze_task_carousel_scatter_init, snd 2),
 * otherwise arms add-letter anim (kaze_task_carousel_exit_timer=0x1e, snd
 * 0x7a) when fewer than 3 letters.
 */

void kaze_task_carousel_select_input(void)
{
    extern int kaze_task_carousel_selected_index;
    extern int kaze_input_edge;
    extern int kaze_task_carousel_exit_timer;
    extern int kaze_task_carousel_transition_ticks;
    extern void kaze_sfx_play_dispatch(int arg0);
    int sound_id;

    if ((kaze_task_carousel_transition_ticks == 0) && (kaze_task_carousel_exit_timer == 0) && (kaze_task_letter_delete_arm_timer == 0) && ((kaze_input_edge & 0x20) != 0)) {
        if (kaze_task_carousel_selected_index == 0x27) {
            if (kaze_task_letter_count < 1) {
                return;
            }
            kaze_task_letter_delete_arm_timer = 0x18;
            sound_id = 0x79;
        } else if (kaze_task_carousel_selected_index == 0x28) {
            kaze_task_menu_state = 0x2B;
            kaze_task_carousel_scatter_init();
            sound_id = 2;
        } else {
            if (2 < kaze_task_letter_count) {
                return;
            }
            kaze_task_carousel_exit_timer = 0x1E;
            sound_id = 0x7A;
        }
        kaze_sfx_play_dispatch(sound_id);
    }
}

extern int rsin(int);
extern void GsInitCoordinate2(void *, void *);
extern void GsLinkObject4(int, void *, int);
extern S8 kaze_task_glyph_rotation_x[];
extern SD20 kaze_task_name_letter_dobjs[];
extern S8 kaze_task_name_letter_rotation_x[];
extern S16 kaze_task_name_letter_pos_x[];

/**
 * kaze_task_letter_add_anim_commit  (menu, 0x8005b898)
 *
 * Add-letter animation/commit: eases the selected glyph toward the name row
 * with rsin (constants kaze_task_letter_add_spin_amplitude/544/550) and
 * spins it; on completion links a new letter model, stores the glyph code
 * into kaze_task_name_letter0_table[slot+task*3], and at 3 letters sets
 * state 0x2c.
 */

void kaze_task_letter_add_anim_commit(void)
{
    extern S50 kaze_task_name_letter_coordinates[];
    extern S16 kaze_task_carousel_glyph_pos_x[];
    extern int kaze_task_carousel_exit_timer;
    extern int kaze_task_carousel_direction;
    extern int kaze_task_carousel_selected_index;
    extern signed char kaze_task_name_letter0_table[];
    int letter_idx;
    register int stage_val __asm__("$2");
    int phase;
    register int timer __asm__("$6");
    int selected_idx;

    {
        register int t0 __asm__("$2");
        t0 = kaze_task_carousel_exit_timer;
        if (t0 == 0) {
            return;
        }
        timer = t0 + -1;
    }
    kaze_task_carousel_exit_timer = timer;
    if (0 < timer) {
        register int p1 __asm__("$3");
        register int f4v __asm__("$2");
        int p2, m2;
        phase = 0x1e - timer;
        p1 = kaze_task_letter_add_row_ease_slope * phase;
        m2 = (&kaze_task_letter_add_x_offset_0)[kaze_task_letter_count];
        p2 = m2 * phase;
        f4v = (p1 >> 0xc) + -100;
        kaze_task_carousel_glyph_pos_x[kaze_task_carousel_selected_index].f4 = f4v;
        kaze_task_carousel_glyph_pos_x[kaze_task_carousel_selected_index].f0 = kaze_task_carousel_glyph_pos_x[kaze_task_carousel_selected_index].f0 + (p2 >> 0xc);
        if (timer >= 0xf) {
            register int pr __asm__("$3");
            register int i8 __asm__("$2");
            int rv = rsin((phase * 0x400) / 0xf);
            int prs;
            i8 = kaze_task_carousel_selected_index;
            pr = kaze_task_letter_add_spin_amplitude * (rv << 1);
            if (pr < 0) {
                pr = pr + 0xfff;
            }
            prs = pr >> 0xc;
            {
                register int si __asm__("$4");
                si = i8 << 4;
                *(int *)((char *)kaze_task_carousel_glyph_pos_x + 8 + si) =
                    *(int *)((char *)kaze_task_carousel_glyph_pos_x + 8 + si) + prs;
            }
        }
        else {
            register int pr __asm__("$3");
            register int i8 __asm__("$2");
            int rv, prs;
            phase = 0xf - timer;
            __asm__("" : "=r"(phase) : "0"(phase));
            rv = rsin((phase * 0x400) / 0xf + 0x400);
            i8 = kaze_task_carousel_selected_index;
            pr = kaze_task_letter_add_spin_amplitude * rv;
            if (pr < 0) {
                pr = pr + 0xfff;
            }
            prs = pr >> 0xc;
            {
                register int si __asm__("$4");
                register int cur __asm__("$3");
                si = i8 << 4;
                cur = *(int *)((char *)kaze_task_carousel_glyph_pos_x + 8 + si);
                *(int *)((char *)kaze_task_carousel_glyph_pos_x + 8 + si) = cur + (kaze_task_letter_add_spin_amplitude + prs);
            }
        }
        {
            int c30 = 0x1e;
            int i5, si5;
            int uv;
            i5 = kaze_task_carousel_selected_index;
            __asm__ volatile("" ::: "memory");
            si5 = i5 << 3;
            uv = (c30 - kaze_task_carousel_exit_timer) * 0x44444;
            uv = uv >> 0xc;
            *(short *)((char *)kaze_task_glyph_rotation_x + si5) = (short)uv;
            *(short *)((char *)kaze_task_glyph_rotation_x + 2 + si5) = (short)uv;
            *(short *)((char *)kaze_task_glyph_rotation_x + 4 + si5) = *(short *)((char *)kaze_task_glyph_rotation_x + si5);
        }
        return;
    }
    letter_idx = kaze_task_letter_count;
    {
        int e58i = letter_idx << 4;
        GsInitCoordinate2((void *)0x0, &kaze_task_name_letter_coordinates[letter_idx]);
        GsLinkObject4(kaze_menu_task_model_ptr + 0xc, (void *)((char *)kaze_task_name_letter_dobjs + e58i), kaze_task_carousel_selected_index);
        *(int *)((char *)kaze_task_name_letter_dobjs + e58i) = 0;
        *(void **)((char *)kaze_task_name_letter_dobjs + 4 + e58i) = &kaze_task_name_letter_coordinates[letter_idx];
    {
        S8 *p40 = &kaze_task_name_letter_rotation_x[letter_idx];
        p40->f4 = 0;
        p40->f2 = 0;
    }
    kaze_task_name_letter_rotation_x[letter_idx].f0 = 0;
    __asm__ volatile("" ::: "memory");
    {
        register int ac __asm__("$3");
        ac = kaze_task_letter_count;
        if (ac == 1) goto STORE0;
        if (ac < 2) {
            stage_val = 0xfffffec0;
            if (ac == 0) goto STOREV;
            goto LAB_8005bbb4;
        }
        if (ac == 2) goto SET2;
        goto LAB_8005bbb4;
    }
STORE0:
    *(int *)((char *)kaze_task_name_letter_pos_x + e58i) = 0;
    goto LAB_8005bbb4;
SET2:
    stage_val = 0x140;
STOREV:
    *(int *)((char *)kaze_task_name_letter_pos_x + e58i) = stage_val;
LAB_8005bbb4:
    kaze_task_name_letter_pos_x[letter_idx].f4 = 0xfffffda0;
    kaze_task_name_letter_pos_x[letter_idx].f8 = 0xfffff800;
    selected_idx = kaze_task_carousel_selected_index;
    {
        S8 *p948;
        int acn = kaze_task_letter_count + 1;
        p948 = &kaze_task_glyph_rotation_x[selected_idx];
        p948->f4 = 0;
        p948->f2 = 0;
        kaze_task_glyph_rotation_x[selected_idx].f0 = 0;
        kaze_task_letter_count = acn;
        if (acn == 3) {
            kaze_task_menu_state = 0x2c;
            if (selected_idx >= 0x15) {
                kaze_task_carousel_direction = 0;
            }
            else {
                kaze_task_carousel_direction = 1;
            }
        }
    }
    {
        signed char *dp;
        int d0 = kaze_task_record_index;
        dp = kaze_task_name_letter0_table;
        (dp + d0 * 3)[letter_idx] = (char)kaze_task_carousel_selected_index;
    }
    }
}

/**
 * kaze_task_letter_delete_input  (menu, 0x8005bc94)
 *
 * Cross-press delete-letter input: first press arms
 * kaze_task_letter_delete_arm_timer=0x18 with sound 0x79; pressing again
 * immediately clears the last name char to 0xff and decrements the letter
 * count kaze_task_letter_count.
 */

void kaze_task_letter_delete_input(void)
{
    extern void kaze_sfx_play_dispatch(int);
    extern char kaze_task_name_letter0_table[];
    extern int kaze_input_edge;
    extern int kaze_task_carousel_exit_timer;
    register int count asm("$5");
    register int idx asm("$3");
    register char *base asm("$4");
    register int addr asm("$2");

    if (kaze_task_carousel_exit_timer == 0) {
        count = kaze_task_letter_count;
        if (count > 0) {
            if (kaze_task_menu_state != 0x2C) {
                if ((kaze_input_edge & 0x40) != 0) {
                    if (kaze_task_letter_delete_arm_timer != 0) {
                        idx = kaze_task_record_index;
                        base = kaze_task_name_letter0_table;
                        addr = idx * 2;
                        addr = addr + idx;
                        addr = addr + (int)base;
                        addr = addr + count;
                        idx = -1;
                        *(char *)(addr - 1) = idx;
                        kaze_task_letter_delete_arm_timer = 0;
                        kaze_task_letter_count = kaze_task_letter_count - 1;
                    } else {
                        kaze_task_letter_delete_arm_timer = 0x18;
                        kaze_sfx_play_dispatch(0x79);
                    }
                }
            }
        }
    }
}

/**
 * kaze_task_letter_delete_anim  (menu, 0x8005bd68)
 *
 * Delete-letter animation: kaze_task_letter_delete_arm_timer countdown eases
 * the last letter model up then away with rsin (constants
 * kaze_task_letter_delete_phase1_divisor/560/564/568) and per-slot drift,
 * then clears the char in kaze_task_name_letter0_table to 0xff and
 * decrements kaze_task_letter_count.
 */

void kaze_task_letter_delete_anim(void)
{
    extern S80 kaze_task_name_letter_coordinates[];
    extern signed char kaze_task_name_letter0_table;
    int letter_idx;
    register int ac __asm__("$3");
    register int tmp __asm__("$4");

    ac = kaze_task_letter_delete_arm_timer;
    if (ac == 0) {
        return;
    }
    tmp = ac + -1;
    kaze_task_letter_delete_arm_timer = tmp;
    letter_idx = kaze_task_letter_count + -1;
    if (tmp > 0) {
        register int den __asm__("$3");
        register int num __asm__("$4");
        den = kaze_task_letter_delete_phase1_divisor;
        if (tmp >= 0x18 - den) {
            int fv;
            num = (0x18 - tmp) * 0x400;
            num = num / den;
            fv = kaze_task_letter_delete_phase1_amplitude * rsin(num);
            if (fv < 0) {
                fv = fv + 0xfff;
            }
            kaze_task_name_letter_pos_x[letter_idx].f4 = (fv >> 0xc) + -0x260;
        }
        else {
            register int b __asm__("$2");
            int fv;
            b = 0x18 - tmp;
            num = b - den;
            b = kaze_task_letter_delete_phase2_divisor;
            __asm__("" : "=r"(num) : "0"(num), "r"(b));
            num = num * 0x400;
            num = num / b;
            fv = kaze_task_letter_delete_phase2_amplitude * rsin(num + 0x400);
            if (fv < 0) {
                fv = fv + 0xfff;
            }
            kaze_task_name_letter_pos_x[letter_idx].f4 = -0xf0 - (fv >> 0xc);
        }
        switch (kaze_task_letter_count) {
        case 1: {
            int x_pos = kaze_task_name_letter_pos_x[letter_idx].f0 + -10;
            int y_pos = kaze_task_name_letter_pos_x[letter_idx].f8 + -0x14;
            kaze_task_name_letter_pos_x[letter_idx].f0 = x_pos;
            kaze_task_name_letter_pos_x[letter_idx].f8 = y_pos;
            kaze_task_name_letter_rotation_x[letter_idx].f4 = kaze_task_name_letter_rotation_x[letter_idx].f4 + -0x55;
            break;
        }
        case 2:
            kaze_task_name_letter_pos_x[letter_idx].f8 = kaze_task_name_letter_pos_x[letter_idx].f8 + -0x14;
            kaze_task_name_letter_rotation_x[letter_idx].f0 = kaze_task_name_letter_rotation_x[letter_idx].f0 + -0x2a;
            break;
        case 3: {
            int x_pos = kaze_task_name_letter_pos_x[letter_idx].f0 + 10;
            int y_pos = kaze_task_name_letter_pos_x[letter_idx].f8 + -0x14;
            kaze_task_name_letter_pos_x[letter_idx].f0 = x_pos;
            kaze_task_name_letter_pos_x[letter_idx].f8 = y_pos;
            kaze_task_name_letter_rotation_x[letter_idx].f4 = kaze_task_name_letter_rotation_x[letter_idx].f4 + 0x55;
            break;
        }
        }
        kaze_task_name_letter_coordinates[letter_idx].f0 = 0;
    }
    else {
        signed char *dp;
        int d0 = kaze_task_record_index;
        dp = &kaze_task_name_letter0_table;
        kaze_task_letter_count = letter_idx;
        (dp + d0 * 3)[letter_idx] = -1;
    }
}

/*
 * kaze_task_carousel_scatter_init -- LAWFUL 0-word MATCH (452B), gcc 2.7.2-psx -O2.
 * PURE C: zero asm statements, zero register pins, island_audit 0/0.
 * Verified 3x: MATCH kaze_task_carousel_scatter_init (452 bytes).
 *
 * The "proven 23-word scheduler-tie floor" was NOT a floor.  The prior BEST.c
 * spelled abs as `if (x < 0) x = -x;` -- REAL branches that split the loop body
 * into small basic blocks.  gcc 2.7.2's sched1 is strictly block-local, so the
 * /0x28 reciprocal-magic load (split into lui/ori by the mips.md large_int
 * define_split) was trapped in the tiny post-abs join block: lui/ori adjacent
 * and late, allocated to $2.  No pin/reorder could ever fix that -- the wall
 * was CONTROL-FLOW SHAPE, not a scheduler tie.
 *
 * Winning mechanisms (each verified in dumps):
 *  1. abs() builtin -> mips.md "abssi2" define_insn (length-3 multi template
 *     `bgez $x,1f; nop; negu $x,$x; 1:`, mips.md:1526).  All four target abs
 *     sites are this template (branch targets = next insn + 1).  A single RTL
 *     insn => NO basic-block boundaries => the whole loop body is one block
 *     => sched1's critical-path priority hoists the magic `lui` to 0x48, its
 *     live range then spans the region and global alloc lands it in $4. This
 *     also yields the two target `nop` delay slots for free.
 *  2. `off = i * 4;` emitted BEFORE the first array ref.  Without it, the
 *     first ref (kaze_task_carousel_scatter_target_x) expands as base-la / i<<2 / add, giving the base
 *     pseudo luid-lifetime 2; loop.c's move test
 *     (threshold*savings*lifetime >= insn_count, loop.c:1637) then hoists it
 *     to the preheader (callee-saved reg, +12B) and, once the base is
 *     invariant, the three 7fc addresses become givs and strength-reduce to a
 *     walking pointer.  With off pre-computed, delete_trivially_dead_insns
 *     (post-cse) leaves base-la adjacent to its add => lifetime 1 => "not
 *     desirable" => no LICM, no givs, target per-iteration address code.
 *  3. Reusing r for the sum (r = abs(..) + abs(..)) merges neg and sum into
 *     one pseudo whose live range spans the +100 value's $2 use, forcing it
 *     to $8/t0 exactly as the target (negu t0 / addu t0 / div ... t0) and
 *     cascading the entire remaining register rotation into place.
 *  4. `int pad; (void)&pad;` -- one addressed-but-unused stack local grows the
 *     frame 32 -> 40 (0x28) to match the target prologue (pure C, no asm).
 * Divisions are native C: /0x28 = magic multiply, /r = maspsx --expand-div
 * checked ladder (break 7 / break 6).
 */
extern int kaze_rand_range();

/**
 * kaze_task_carousel_scatter_init  (menu, 0x8005c04c)
 *
 * Initializes the carousel dismiss scatter: per glyph picks random targets
 * via kaze_rand_range and precomputes per-frame step deltas (over 0x28
 * frames) into kaze_task_carousel_scatter_frame_count/kaze_task_carousel_sca
 * tter_target_x/kaze_task_carousel_scatter_target_y/kaze_task_carousel_scatt
 * er_step_mag/kaze_task_carousel_scatter_remaining/kaze_task_carousel_scatte
 * r_step_x/kaze_task_carousel_scatter_step_y.
 */

void kaze_task_carousel_scatter_init(void)
{
    extern int kaze_task_carousel_scatter_remaining[];
    extern int kaze_task_carousel_scatter_step_mag[];
    int i;
    int *p;
    int r, off;
    int pad;

    i = 0;
    if (0 < kaze_task_glyph_count) {
        p = kaze_task_carousel_scatter_frame_count;
        (void)&pad;
        do {
            *p++ = kaze_rand_range(10, 0x32);
            r = -kaze_rand_range(400, -0x258);
            off = i * 4;
            kaze_task_carousel_scatter_target_x[i] = r + 100;
            kaze_task_carousel_scatter_target_y[i] = 0x4b0 - r;
            r = abs(kaze_task_carousel_scatter_target_x[i]) + abs(kaze_task_carousel_scatter_target_y[i]);
            kaze_task_carousel_scatter_step_mag[i] = r / 0x28;
            kaze_task_carousel_scatter_remaining[i] = kaze_task_carousel_scatter_step_mag[i];
            kaze_task_carousel_scatter_step_x[i] = abs(kaze_task_carousel_scatter_step_mag[i] * kaze_task_carousel_scatter_target_x[i] / r);
            kaze_task_carousel_scatter_step_y[i] = abs(kaze_task_carousel_scatter_step_mag[i] * kaze_task_carousel_scatter_target_y[i] / r);
            i++;
        } while (i < kaze_task_glyph_count);
    }
}

/* kaze_task_carousel_exit_handler (0x8005c210), 692 bytes, gcc 2.7.2-psx -O2.
 * LAWFUL REDO: native C division (--expand-div emits the checked-div ladder
 * byte-for-byte), no SDIV islands, no lw asm island.
 */
extern int rcos(int);
extern int kaze_task_glyph_coordinates[];
extern int kaze_task_carousel_glyph_pos_y[];
extern int kaze_task_carousel_glyph_pos_z[];

/**
 * kaze_task_carousel_exit_handler  (menu, 0x8005c210)
 *
 * Nested task-letter carousel handler around nested state 0x2b; current
 * natural-route blocker for reaching settings rows without RAM writes.
 */

void kaze_task_carousel_exit_handler(void)
{
    extern int kaze_task_carousel_glyph_pos_x[];
    extern int kaze_task_carousel_scatter_step_mag;
    extern int kaze_task_carousel_scatter_remaining;
    extern int kaze_task_carousel_angle_ring[];
    int i;
    struct { int flag, pad1, pad2; } frame;
    int off9;
    int off6;
    int off5;
    int *p7;
    int *p8;

    i = 0;
    frame.flag = 0;
    if (0 < kaze_task_glyph_count) {
        __asm__("" : : "m"(frame.pad1), "m"(frame.pad2));
        off9 = 0;
        off6 = 0;
        p7 = &kaze_task_carousel_scatter_remaining;
        p8 = &kaze_task_carousel_scatter_step_mag;
        off5 = 0;
        do {
            int cur = *p7 + -1;
            if (0 < *p7) {
                int base, hi, rs, diff;
                register int pv asm("$6");
                int *pr, *pw;
                *p7 = cur;
                pv = *p8;
                __asm__("" : : "r"(cur), "r"(cur));
                base = *(int *)((char *)kaze_task_carousel_scatter_step_x + off5);
                hi = pv - base;
                {
                    register int flag_value asm("$7") = 1;
                    frame.flag = flag_value;
                }
                if (cur >= hi) {
                    int d = base;
                    int q;
                    diff = pv - cur;
                    {
                        register int nn asm("$4") = diff * 0x400;
                        q = nn / d;
                    }
                    rs = rsin(q);
                    {
                        int mem = *(int *)((char *)kaze_task_carousel_scatter_target_x + off5);
                        int r2 = rs;
                        rs = mem * r2;
                    }
                    if (rs < 0) rs = rs + 0xfff;
                    rs = (rs >> 0xc) + -100;
                } else {
                    int d;
                    int q;
                    diff = hi - cur;
                    d = *(int *)((char *)kaze_task_carousel_scatter_step_y + off5);
                    {
                        register int nn asm("$4") = diff * 0x400;
                        q = nn / d;
                    }
                    {
                        register int er asm("$3");
                        er = rsin(q + 0x400);
                        er = *(int *)((char *)kaze_task_carousel_scatter_target_y + off5) * er;
                        if (er < 0) er = er + 0xfff;
                        rs = 0x4b0 - (er >> 0xc);
                    }
                }
                *(int *)((char *)kaze_task_carousel_glyph_pos_y + off6) = rs;
                pr = (int *)((char *)kaze_task_carousel_angle_ring + off5);
                diff = *p8 - *p7;
                rs = rsin(*pr);
                pw = (int *)((char *)kaze_task_carousel_scatter_frame_count + off5);
                {
                    register int t asm("$3") = diff * *pw;
                    int r2 = rs;
                    t = t + 0x600;
                    rs = t * r2;
                }
                if (rs < 0) rs = rs + 0xfff;
                *(int *)((char *)kaze_task_carousel_glyph_pos_x + off6) = rs >> 0xc;
                {
                    int rc = rcos(*pr);
                    register int t asm("$3") = diff * *pw;
                    rs = (t + 0x600) * rc;
                }
                if (rs < 0) rs = rs + 0xfff;
                *(int *)((char *)kaze_task_carousel_glyph_pos_z + off6) = rs >> 0xc;
                *(int *)((char *)kaze_task_glyph_coordinates + off9) = 0;
            }
            off9 += 0x50;
            off6 += 0x10;
            p7 += 1;
            p8 += 1;
            off5 += 4;
            i += 1;
        } while (i < kaze_task_glyph_count);
        __asm__("" : : "r"(off6));
    }
    {
        register int flag_value asm("$7") = frame.flag;
        if (!flag_value) {
        kaze_task_menu_state = 99;
        }
    }
}

/**
 * kaze_task_carousel_layout_update  (menu, 0x8005c4c4)
 *
 * Recomputes glyph ring positions from the kaze_task_carousel_angle_ring
 * angles (radius 0x600 rsin/rcos into kaze_task_carousel_glyph_pos_x/98),
 * lifting the selected kaze_task_carousel_selected_index (rcos) and previous
 * kaze_task_carousel_previous_index (rsin) by rotation-phase bump
 * kaze_task_carousel_transition_amplitude.
 */

void kaze_task_carousel_layout_update(void)
{
    extern int kaze_task_carousel_glyph_pos_x[];
    extern int kaze_task_carousel_transition_duration;
    extern int kaze_task_carousel_transition_ticks;
    extern int kaze_task_carousel_previous_index;
    extern int kaze_task_carousel_selected_index;
    extern int kaze_task_carousel_angle_ring[];
    int i;
    int off7;
    int off6;
    int off5;
    int *p;
    int t;
    int *q;
    int rs;
    int rc;

    register int res __asm__("$2");
    register int dnum __asm__("$4");
    register int dden __asm__("$2");
    register int sel __asm__("$3");
    int pad;

    i = 0;
    if (0 < kaze_task_glyph_count) {
        __asm__("" : : "m"(pad));
        off7 = 0;
        off6 = 0;
        p = kaze_task_carousel_glyph_pos_y;
        off5 = 0;
        do {
            q = (int *)((char *)kaze_task_carousel_angle_ring + off6);
            rs = rsin(*q) * 0x600 / 0x1000;
            *(int *)((char *)kaze_task_carousel_glyph_pos_x + off5) = rs;
            rc = rcos(*q);
            rc = rc * 0x600 / 0x1000;
            sel = kaze_task_carousel_selected_index;
            *(int *)((char *)kaze_task_carousel_glyph_pos_z + off5) = rc;
            *p = 0;
            if (sel == i) {
                dnum = kaze_task_carousel_transition_ticks << 10;
                dden = kaze_task_carousel_transition_duration;
                dnum = dnum / dden;
                res = rcos(dnum);
                goto bump;
            } else if (kaze_task_carousel_previous_index == i) {
                dnum = kaze_task_carousel_transition_ticks << 10;
                dden = kaze_task_carousel_transition_duration;
                dnum = dnum / dden;
                res = rsin(dnum);
            bump:
                t = kaze_task_carousel_transition_amplitude * res / 0x1000;
                *p = *p + t;
            }
            *(int *)((char *)kaze_task_glyph_coordinates + off7) = 0;
            off7 += 0x50;
            off6 += 4;
            p += 4;
            off5 += 0x10;
            i += 1;
        } while (i < kaze_task_glyph_count);
    }
}

extern u16 kaze_menu_camera_rot_y;
extern int kaze_menu_camera_coord_flags;

/**
 * kaze_task_menu_camera_bob  (menu, 0x8005c6c0)
 *
 * Idle task-menu camera motion: spins yaw kaze_menu_camera_rot_y by 8/frame
 * and bobs camera height kaze_menu_camera_scroll with rsin of phase
 * kaze_task_menu_camera_bob_phase.
 */

void kaze_task_menu_camera_bob(void)
{
    u16 *rot;
    int s;

    rot = &kaze_menu_camera_rot_y;
    *rot = (*rot + 8) & 0xFFF;
    s = rsin(kaze_task_menu_camera_bob_phase);
    kaze_menu_camera_scroll = s / 64 - 0x180;
    kaze_menu_camera_coord_flags = 0;
    kaze_task_menu_camera_bob_phase = kaze_task_menu_camera_bob_phase + 8;
    return;
}
