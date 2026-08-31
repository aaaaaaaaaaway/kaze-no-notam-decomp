#include "kaze_syms.h"
extern short D_800B1728, kaze_balloon_sway_offset_y, D_800B172C;
extern int D_800B18A8;
extern int kaze_camera_anchor_route1_x;
extern int kaze_camera_anchor_route1_y;
extern int kaze_camera_anchor_route1_z;
extern int D_800B18C0;
extern int D_800B18C4;
extern int D_800B18C8;
extern char kaze_record_table_a, kaze_record_table_b, kaze_record_table_c, kaze_record_table_pad;
extern int kaze_live_stage_level;
struct triple { int a, b, c; };
struct block48 { int w[12]; };

extern int kaze_input_prev_pad2_mask;
extern int kaze_input_current_pad2_mask;
extern int kaze_input_current_b;
extern char kaze_wind_layer0_dir[];
extern int kaze_camera_anchor_target_y;
extern int kaze_camera_anchor_target_z;
extern int kaze_camera_control_speed_x;
extern int kaze_camera_control_speed_y;
extern int kaze_camera_anchor_target_x;
extern void bzero(char *, int);

/**
 * kaze_input_light_defaults_init  (core, 0x80011f18)
 *
 * Resets input masks (kaze_input_current_a/98, kaze_input_current_b/14),
 * zeroes the 0x28-byte wind block at kaze_wind_layer0_dir, and seeds default
 * lighting/scalar fields from kaze_camera_anchor_target_x through an unnamed
 * camera-anchor field further in the same block (200, 0x708, 0x10s, -0x32,
 * 4s).
 */

void kaze_input_light_defaults_init(void)
{
    extern int kaze_input_current_a;
    kaze_input_prev_pad2_mask = 0;
    kaze_input_current_pad2_mask = 0;
    kaze_input_current_b = 0;
    kaze_input_current_a = 0;
    bzero(kaze_wind_layer0_dir, 0x28);
    kaze_camera_anchor_target_y = 200;
    kaze_camera_anchor_target_z = 0x708;
    kaze_camera_control_speed_x = 0x10;
    kaze_camera_control_speed_y = 0x10;
    D_800B18A8 = 0x10;
    kaze_camera_anchor_route1_y = -0x32;
    kaze_camera_anchor_target_x = 0;
    kaze_camera_anchor_route1_x = 0;
    kaze_camera_anchor_route1_z = 0;
    D_800B18C0 = 4;
    D_800B18C4 = 4;
    D_800B18C8 = 4;
}

/* MATCH under gcc 2.7.2-psx -O2 (508B, byte-exact). Closes the 43/127 one-short near-miss.
 * Four levers, all via cc1 -dg / objdump comparison:
 *  1) The missing instruction was a LICM hoist of &src_words into a persistent
 *     caller-clobbered temp. Force it with an explicit `int *base_ptr = src_words;`
 *     ... but the assignment must live AFTER bzero (else base_ptr is live across the
 *     call -> forced onto a callee-saved reg -> extra $s0 save/restore, +2 insns).
 *  2) Pin base_ptr to $13 (asm("$13")) so the addiu materializes DIRECTLY into the
 *     persistent reg; an unpinned local emits addiu $2 + move $13 (one extra copy).
 *  3) Emit order at loop entry: outer_i=0, base_ptr=src_words, outer_off=0 (base between
 *     the two counter zeroings, matching the target's move/addiu/move sequence).
 *  4) Register $6/$7 contest (mid_off vs src_word_ptr) + move ordering: put src_word_ptr=base_ptr
 *     before mid_off=0 in the outer top, hoist base_off=outer_off up as the 2nd outer stmt,
 *     and place the inner counter increment inner_i+=1 LAST in the inner loop so it
 *     fills the *src_word load-delay slot instead of floating to the loop top. */
extern struct triple kaze_record_default_score_table;
extern int kaze_balloon_scale_x, kaze_balloon_scale_y, kaze_balloon_scale_z;
extern int kaze_stage_index, kaze_stage_mode, kaze_stage_flags;
extern int kaze_wind_active_angle, kaze_balloon_angle, kaze_impact_flash_timer;
extern short kaze_balloon_render_rot0, kaze_balloon_render_rot_y, kaze_balloon_render_rot1;
extern int kaze_pos_x, kaze_pos_y, kaze_pos_z;
extern int kaze_record_table_score;
extern int kaze_staged_stage_index;

/**
 * kaze_game_state_defaults_init  (core, 0x80011fd4)
 *
 * Full game-state reset: zeroes live stage block kaze_stage_index.., balloon
 * pos kaze_pos_x/dc/e0=(0x3b600,-0x1000,0x3b600), motion/input fields, fills
 * the kaze_record_table_a color table, copies stage block to
 * kaze_staged_stage_index.
 */

void kaze_game_state_defaults_init(void)
{
    extern int D_800B17D4, kaze_input_prev_pad2_mask, kaze_input_current_pad2_mask, kaze_input_current_b, kaze_input_current_a;
    int elem_off;
    int inner_off;
    int inner_i;
    int base_off;
    int mid_off;
    int *src_word_ptr;
    int *src_word;
    int mid_i;
    int outer_off;
    int outer_i;
    int src_words[4];
    register int *base_ptr asm("$13");

    *(struct triple *)src_words = kaze_record_default_score_table;
    kaze_live_stage_level = 1;
    kaze_balloon_scale_x = 0x1000;
    kaze_balloon_scale_y = 0x1000;
    kaze_balloon_scale_z = 0x1000;
    D_800B17D4 = 0;
    kaze_input_prev_pad2_mask = 0;
    kaze_input_current_pad2_mask = 0;
    kaze_input_current_b = 0;
    kaze_input_current_a = 0;
    kaze_stage_index = 0;
    kaze_stage_mode = 0;
    kaze_stage_flags = 0;
    kaze_wind_active_angle = 0;
    kaze_balloon_angle = 0;
    kaze_impact_flash_timer = 0;
    D_800B1728 = 0;
    kaze_balloon_sway_offset_y = 0;
    D_800B172C = 0;
    kaze_balloon_render_rot0 = 0;
    kaze_balloon_render_rot_y = 0;
    kaze_balloon_render_rot1 = 0;
    kaze_pos_x = 0x3b600;
    kaze_pos_y = 0xfffff000;
    kaze_pos_z = 0x3b600;
    bzero(kaze_wind_layer0_dir, 0x28);
    outer_i = 0;
    base_ptr = src_words;
    outer_off = 0;
    do {
        mid_i = 0;
        base_off = outer_off;
        src_word_ptr = base_ptr;
        mid_off = 0;
        do {
            inner_i = 0;
            src_word = src_word_ptr;
            inner_off = mid_off;
            do {
                elem_off = inner_off + base_off;
                (&kaze_record_table_a)[elem_off] = 0;
                (&kaze_record_table_b)[elem_off] = 0;
                (&kaze_record_table_c)[elem_off] = 0;
                (&kaze_record_table_pad)[elem_off] = 0;
                *(int *)((int)&kaze_record_table_score + elem_off) = *src_word;
                inner_off = inner_off + 8;
                inner_i = inner_i + 1;
            } while (inner_i < 6);
            src_word_ptr = src_word_ptr + 1;
            mid_i = mid_i + 1;
            mid_off = mid_off + 0x30;
        } while (mid_i < 3);
        outer_i = outer_i + 1;
        outer_off = outer_off + 0x90;
    } while (outer_i < 3);
    *(struct block48 *)&kaze_staged_stage_index = *(struct block48 *)&kaze_stage_index;
}
