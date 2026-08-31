/**
 * kaze_pause_exit_prompt  (menu, 0x8001e6c8)
 *
 * Pause/exit prompt branch; sets the exit prompt latch at base+0xecafc.
 */

#include "types.h"
#include "kaze_syms.h"
extern int kaze_marker_slot_find_free(void);
extern int kaze_stage_score_compute(int);
extern char *kaze_state_base_ptr;
extern char *volatile D_8008D9D0_v __asm__("kaze_state_base_ptr");

extern int kaze_input_current_a, kaze_input_current_b, kaze_input_prev_pad2_mask;
extern int D_800B33B0, kaze_input_bind_pause, kaze_input_bind_confirm, kaze_input_bind_marker_drop, kaze_input_action_button_mask;
extern int kaze_burner_hold_charge, kaze_balloon_sway_phase, kaze_impact_flash_timer, kaze_balloon_scale_x, kaze_balloon_scale_z, kaze_balloon_scale_y;
extern short kaze_balloon_sway_offset_y;
extern int kaze_stage_index, kaze_stage_mode, kaze_stage_variant;

extern void kaze_sfx_voice_keyon_34108(int, int);
extern int kaze_record_table_insert(int, int, int);
extern int rsin(int);

void kaze_pause_exit_prompt(void)
{
    uint next_stage;
    int scratch;
    int marker_slot;
    register int t1514 asm("$2");
    register uint alt_flags asm("$3");

    if ((((*(uint *)(kaze_state_base_ptr + 0x14) & 4) != 0) && ((kaze_input_current_a & kaze_input_bind_pause) != 0)) &&
        ((kaze_input_current_b & kaze_input_bind_pause) == 0)) {
        *(uint *)(kaze_state_base_ptr + 0x14) = *(uint *)(kaze_state_base_ptr + 0x14) | 0x80000000;
        kaze_sfx_voice_keyon_34108(3, 1);
        *(int *)(kaze_state_base_ptr + 0xecafc) = 1;
        return;
    }
    {
    uint fB = *(uint *)(kaze_state_base_ptr + 0x14);
    if ((((fB & 4) != 0) && ((kaze_input_current_a & D_800B33B0) != 0)) &&
        ((kaze_input_current_b & D_800B33B0) == 0)) {
        register char *baseA asm("$7") = kaze_state_base_ptr;
        register int kA0 asm("$4");
    register int kA1 asm("$5");
        kA0 = 0x24;
        { uint *_p = (uint *)baseA; _p[5] = fB | 0x40000000; }
        kaze_input_current_b = kaze_input_current_b | D_800B33B0;
        t1514 = kaze_input_prev_pad2_mask | D_800B33B0;
        *(int *)(baseA + 0x48e0c) = 1;
        kaze_input_prev_pad2_mask = t1514;
        kA1 = 1;
        kaze_sfx_voice_keyon_34108(kA0, kA1);
        return;
    }
    }
    {
    register char *jb __asm__("$6") = D_8008D9D0_v;
    alt_flags = *(uint *)(jb + 0x14);
    __asm__("":"=r"(alt_flags):"0"(alt_flags));
    if ((alt_flags & 0x20) == 0) {
        goto LAB_burner;
    }
    if (((kaze_input_current_a & kaze_input_action_button_mask) != 0) && ((kaze_input_current_b & kaze_input_action_button_mask) == 0)) {
        register int kB0 asm("$4");
        register int kB1 asm("$5");
        kB0 = 0x24;
        kB1 = 1;
        {
        uint fv = alt_flags;
        __asm__("" : "=r"(fv) : "0"(fv), "r"(kB0), "r"(kB1));
        { register uint tf asm("$2") = fv | 0x40000000; *(uint *)(jb + 0x14) = tf; }
        }
        kaze_input_current_b = kaze_input_current_b | kaze_input_action_button_mask;
        *(short *)(jb + 0x492c0) = 0x60;
        { short *_p = (short *)jb; _p[149857] = 0xff38; }
        t1514 = kaze_input_prev_pad2_mask | kaze_input_action_button_mask;
        *(int *)(jb + 0x48e0c) = 10;
        kaze_input_prev_pad2_mask = t1514;
        kaze_sfx_voice_keyon_34108(kB0, kB1);
        return;
    }
    }
    {
    uint flags;
    register uint m1 asm("$5") = 0xffff7fff;
    register uint m2 asm("$4") = 0xfffeffff;
        flags = *(uint *)(kaze_state_base_ptr + 0x14);
        flags &= m1;
        __asm__("" : : "r"(flags));
        flags &= m2;
        __asm__("" :: "r"(flags));
        *(uint *)(kaze_state_base_ptr + 0x14) = flags;
    }
    kaze_burner_input_update();
    kaze_wind_motion_update();
    kaze_object_physics_snapshot();
    goto LAB_e9dc;
LAB_burner:
    {
    register uint bt asm("$2") = alt_flags & 0x1000;
    if (bt != 0) {
        kaze_burner_sound_toggle(0);
        {
        register char *bb asm("$5") = kaze_state_base_ptr;
    int * p1738;
        p1738 = &kaze_burner_hold_charge;
        { int *_p = (int *)bb; *p1738 = *p1738 + _p[41] * 0x20; }
        if ((0x1000 < *p1738) &&
            (*p1738 = 0x1000, (*(uint *)(bb + 0x14) & 0xc00000) == 0)) {
            next_stage = kaze_stage_index + 1;
            __asm__("" :: "r"(next_stage));
            next_stage = next_stage & -(uint)((int)next_stage < 3);
            if ((kaze_stage_variant == 0) && (kaze_stage_mode == 1)) {
                scratch = kaze_stage_score_compute(1);
                { int *_p = (int *)kaze_state_base_ptr; _p[3] = scratch; }
                if (-1 < scratch) {
                    kaze_success_terminal_handler();
                    scratch = kaze_record_table_insert(next_stage, 0,
                                                     *(int *)(kaze_state_base_ptr + 0xc));
                    *(int *)(kaze_state_base_ptr + 0x10) = scratch;
                    goto LAB_e9dc;
                }
            }
            kaze_failure_terminal_handler();
        }
        }
    }
    }
LAB_e9dc:
    {
    int *p1730 = &kaze_balloon_sway_phase;
    scratch = rsin(*p1730);
    scratch = scratch * 10;
    if (scratch < 0) {
        scratch = scratch + 0xfff;
    }
    kaze_balloon_sway_offset_y = (short)(scratch >> 0xc);
    *p1730 = *p1730 + *(int *)(kaze_state_base_ptr + 0xa4) * 0x20 & 0xfff;
    }
    scratch = rsin(kaze_impact_flash_timer);
    if (scratch < 0) {
        scratch = scratch + 0xf;
    }
    kaze_balloon_scale_z = (scratch >> 4) + 0x1000;
    kaze_balloon_scale_x = kaze_balloon_scale_z;
    scratch = rsin(kaze_impact_flash_timer);
    if (scratch < 0) {
        scratch = scratch + 0xf;
    }
    kaze_balloon_scale_y = 0x1000 - (scratch >> 4);
    if ((*(uint *)(kaze_state_base_ptr + 0x14) & 0x20) == 0) {
        goto LAB_reset;
    }
    if ((kaze_input_current_a & kaze_input_bind_confirm) == 0) {
        goto LAB_eb50;
    }
    if (((kaze_input_current_b & kaze_input_bind_confirm) == 0) && ((*(uint *)(kaze_state_base_ptr + 0x14) & 0x18000) == 0) &&
        (*(int *)(kaze_state_base_ptr + 0x60) == 0)) {
        *(int *)(kaze_state_base_ptr + 0x60) = 1;
        kaze_input_current_b = kaze_input_current_b | kaze_input_bind_confirm;
    }
    if (((kaze_input_current_a & kaze_input_bind_confirm) != 0) && ((kaze_input_current_b & kaze_input_bind_confirm) == 0)) {
        goto LAB_eb70;
    }
LAB_eb50:
    if ((*(uint *)(kaze_state_base_ptr + 0x14) & 0x18000) == 0) {
        goto LAB_eb90;
    }
LAB_eb70:
    if (*(int *)(kaze_state_base_ptr + 0x60) == 1) {
        *(int *)(kaze_state_base_ptr + 0x60) = 0;
    }
LAB_eb90:
    if ((((kaze_input_current_a & kaze_input_bind_marker_drop) != 0) && ((kaze_input_current_b & kaze_input_bind_marker_drop) == 0)) &&
        ((*(uint *)(kaze_state_base_ptr + 0x14) & 0x18000) == 0)) {
        if ((*(uint *)(kaze_state_base_ptr + 0x14) & 0x100) != 0) {
            marker_slot = kaze_marker_slot_find_free();
            kaze_marker_drop(marker_slot);
        }
    }
    goto LAB_ec1c;
LAB_reset:
    if (*(int *)(kaze_state_base_ptr + 0x60) == 1) {
        *(int *)(kaze_state_base_ptr + 0x60) = 0;
    }
LAB_ec1c:
    kaze_camera_control_update();
    return;
}
