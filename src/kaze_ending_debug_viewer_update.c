/**
 * kaze_ending_debug_viewer_update  (menu, 0x800659f8)
 *
 * Ending state-0 debug viewer: pad-2 new presses (kaze_pad2_edge_mask)
 * toggle flags/start the credits (kaze_ending_screen_state^=1) and step
 * credit pages (uploading via kaze_tim_load_to_vram); runs free camera
 * kaze_ending_debug_camera_control, fade, credit prims, and model draw
 * kaze_title_model_sort_dispatch.
 */

#include "types.h"
#include "kaze_syms.h"
extern int kaze_ending_camera_pos_x;
extern int kaze_ending_camera_pos_y;
extern int kaze_ending_camera_pos_z;
extern int kaze_ending_screen_state;
extern int kaze_ending_credits_substate;
extern int kaze_ending_debug_toggle_flag;
extern int kaze_ending_fade_phase;
extern int kaze_ending_fade_level;
extern int kaze_ending_credits_frame_counter;
extern int kaze_ending_credit_page_texture_table[];
extern int kaze_pad2_edge_mask;
extern int kaze_pad1_edge_mask;
extern int kaze_ending_credit_page_index;
extern s16 kaze_ending_camera_angle_x;
extern s16 kaze_ending_camera_angle_y;
extern s16 kaze_ending_camera_angle_z;
extern int kaze_ending_ot_headers[];

extern void kaze_tim_load_to_vram();
extern void kaze_ending_credit_prims_add();
extern void kaze_title_model_sort_dispatch();

void kaze_ending_debug_viewer_update(int buf)
{
    if ((kaze_pad2_edge_mask & 0x800) != 0) {
        kaze_ending_debug_toggle_flag = kaze_ending_debug_toggle_flag ^ 1;
    }
    if (((kaze_pad2_edge_mask & 0x100) != 0) && (kaze_ending_screen_state = kaze_ending_screen_state ^ 1, kaze_ending_screen_state == 1)) {
        kaze_ending_credits_substate = 0;
        kaze_ending_fade_phase = 0;
        kaze_ending_fade_level = 0xff;
        kaze_ending_credits_frame_counter = 0;
    }
    if ((kaze_pad1_edge_mask & 0x800) != 0) {
        kaze_ending_fade_phase = 1;
    }
    /* cc1 otherwise loads the six call arguments in a different register order. */
    __asm__ __volatile__(
        ".set\tnoreorder\n\t"
        "lw\t$2,kaze_ending_camera_pos_y\n\t"
        "lw\t$7,kaze_ending_camera_pos_x\n\t"
        "lh\t$4,kaze_ending_camera_angle_x\n\t"
        "lh\t$5,kaze_ending_camera_angle_y\n\t"
        "lh\t$6,kaze_ending_camera_angle_z\n\t"
        "lw\t$3,kaze_ending_camera_pos_z\n\t"
        "sw\t$2,16($sp)\n\t"
        "jal\tkaze_ending_debug_camera_control\n\t"
        "sw\t$3,20($sp)\n\t"
        ".set\treorder"
        :
        :
        : "$2", "$3", "$4", "$5", "$6", "$7", "memory");
    if ((kaze_pad2_edge_mask & 0x20) != 0) {
        kaze_ending_credit_page_index = kaze_ending_credit_page_index + 1;
        if (kaze_ending_credit_page_index == (kaze_ending_credit_page_index / 0xc) * 0xc) {
            kaze_ending_credit_page_index = 0;
        }
        kaze_ending_fade_level = 0xff;
        kaze_tim_load_to_vram((kaze_ending_fade_phase = 0, kaze_ending_credit_page_texture_table[kaze_ending_credit_page_index]), 0x140,
                              0x100, 0, 0);
    }
    kaze_ending_fade_poly_update(buf);
    kaze_ending_credit_prims_add(buf);
    kaze_title_model_sort_dispatch(buf, &kaze_ending_ot_headers[buf * 5]);
    return;
}
