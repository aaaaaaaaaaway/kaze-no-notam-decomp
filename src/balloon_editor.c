#include "types.h"
#include "kaze_syms.h"
extern int kaze_balloon_view_pitch_dir_counter;
extern int kaze_menu_camera_pos_z;
extern int kaze_menu_camera_scroll;
extern int kaze_balloon_view_yaw_dir_flag;
extern int kaze_balloon_view_zoom_dir_counter;
extern int kaze_balloon_view_zoom_timer;
extern int kaze_balloon_view_pitch_timer;
extern int kaze_balloon_view_yaw_timer;
extern int kaze_task_record_index;
extern u8 kaze_balloon_color_shade_table[];
extern int kaze_balloon_editor_skip_face_table[];
extern int kaze_balloon_paint_held_flag;
extern int kaze_task_menu_confirmed_index;
extern int kaze_task_menu_confirm_flag;
extern short kaze_balloon_switch_direction_flag;
extern int kaze_balloon_switch_anim_counter;
extern int kaze_balloon_color_edit_channel;
extern u8 kaze_balloon_color_edit_g;
extern u8 kaze_balloon_color_edit_b;
extern int kaze_balloon_paint_hovered_poly_idx;
extern int kaze_balloon_highlight_phase_r;
extern int kaze_balloon_highlight_phase_g;
extern int kaze_balloon_highlight_phase_b;
extern char kaze_balloon_name_row_sprites[];
extern short kaze_balloon_name_row_sprites_x[];
extern char kaze_balloon_name_row_clut[];
extern char kaze_balloon_name_row_tpage_prims[];
extern char kaze_registration_confirm_label_sprites[];
extern char kaze_registration_confirm_tpage_prims[];
extern short kaze_balloon_editor_label_sprites_x[];
extern u8 kaze_balloon_editor_label_clut[];
extern short kaze_balloon_editor_preview_sprite_x[];
extern short kaze_balloon_editor_swatch_tiles_x[];
extern short kaze_balloon_paint_cursor_x[];
extern short kaze_balloon_paint_cursor_y[];
extern int kaze_menu_camera_trans_x;
extern int kaze_menu_camera_trans_y;
extern int kaze_menu_camera_trans_z;
extern int kaze_menu_camera_coord_word;
extern int kaze_model_prim_ptr_table[];
extern int kaze_menu_model1_gsobj;
extern int kaze_menu_model1_coord_ptr;
extern int kaze_menu_balloon_model_ptr;
extern int kaze_task_transition_counter;
extern long RotNclip3(int, int, int, int *, int *, int *, int *, int *, int *);
extern void SetTile16(void *);
typedef struct {
    char bytes[0x14A0];
} ModelBlock;
struct BalloonImg { char b[0x14a0]; };
typedef struct {
    short m[3][3];
    short pad;
    int t[3];
} MATRIX;
typedef struct {
    int prim;
    int pad[4];
} Slot;

void kaze_balloon_select_row_sprites_init();
void kaze_registration_confirm_sprites_init();
void kaze_balloon_editor_panel_sprites_init();
extern void kaze_balloon_entry_grid_sprites_init(void);

/**
 * kaze_registration_sprites_init_all  (menu, 0x80057794)
 *
 * Calls the six registration/balloon-editor sprite builders
 * kaze_balloon_select_row_sprites_init,
 * kaze_registration_confirm_sprites_init,
 * kaze_balloon_editor_panel_sprites_init,
 * kaze_color_edit_popup_sprites_init, kaze_paint_cursor_sprites_init, and
 * kaze_balloon_entry_grid_sprites_init.
 */

void kaze_registration_sprites_init_all(void)
{
    kaze_balloon_select_row_sprites_init();
    kaze_registration_confirm_sprites_init();
    kaze_balloon_editor_panel_sprites_init();
    kaze_color_edit_popup_sprites_init();
    kaze_paint_cursor_sprites_init();
    kaze_balloon_entry_grid_sprites_init();
}

extern int kaze_task_menu_state;
extern short kaze_menu_camera_rot_z;
extern short kaze_menu_render_rot_x;
extern short kaze_menu_render_rot_y;
extern short kaze_menu_render_rot_z;
extern void GsLinkObject4(int, void *, int);

/**
 * kaze_registration_screen_init  (menu, 0x800577dc)
 *
 * Resets the kaze_task_menu_state state block, copies balloon model
 * (&kaze_save_slot_unpack_buffer)[kaze_task_record_index] (0x14a0 bytes)
 * into the GPU buffer, links GsDOBJ2 kaze_menu_model1_gsobj, and sets view
 * vars kaze_menu_camera_pos_x..
 */

void kaze_registration_screen_init(void)
{
    extern u16 kaze_balloon_paint_tri_vertex_ax, kaze_balloon_paint_tri_vertex_bx, kaze_balloon_paint_tri_vertex_cx, kaze_balloon_paint_tri_vertex_ay, kaze_balloon_paint_tri_vertex_by;
    extern ModelBlock **kaze_menu_model1_data_ptr;
    extern int kaze_save_slot_unpack_buffer[];
    extern int kaze_menu_camera_coord_flags;
    extern int kaze_menu_camera_pos_x;
    extern short kaze_menu_camera_rot_y;
    extern short kaze_menu_camera_rot_x;
    extern char kaze_balloon_paint_color_b;
    extern char kaze_balloon_paint_color_g;
    extern char kaze_balloon_paint_color_r;
    extern short kaze_registration_confirm_cursor;
    int zero;
    int idx;

    idx = kaze_task_record_index;
    kaze_balloon_highlight_phase_g = 0x40;
    kaze_task_menu_state = 0;
    kaze_task_menu_confirm_flag = 0;
    kaze_registration_confirm_cursor = 0;
    kaze_task_transition_counter = 0;
    kaze_balloon_color_edit_channel = 0;
    kaze_balloon_switch_anim_counter = 0;
    kaze_balloon_switch_direction_flag = 0;
    kaze_balloon_paint_hovered_poly_idx = 0;
    kaze_balloon_highlight_phase_r = 0;
    kaze_balloon_paint_color_r = 0;
    kaze_balloon_paint_color_g = 0;
    kaze_balloon_paint_color_b = 0;
    kaze_balloon_view_pitch_timer = 0;
    kaze_balloon_view_yaw_timer = 0;
    kaze_balloon_view_zoom_timer = 0;
    kaze_balloon_view_pitch_dir_counter = 0;
    kaze_balloon_view_yaw_dir_flag = 0;
    kaze_balloon_view_zoom_dir_counter = 0;
    kaze_balloon_highlight_phase_b = 0x20;
    kaze_task_menu_confirmed_index = idx;
    *kaze_menu_model1_data_ptr[4] = *(ModelBlock *)kaze_save_slot_unpack_buffer[idx];
    GsLinkObject4(kaze_menu_balloon_model_ptr + 0xC, &kaze_menu_model1_gsobj, 0);
    kaze_menu_camera_rot_x = 0;
    kaze_menu_camera_rot_z = 0;
    zero = 0;
    kaze_menu_camera_rot_y = zero;
    kaze_menu_camera_pos_x = 0x140;
    kaze_menu_camera_scroll = -0x60;
    kaze_menu_camera_pos_z = 0x600;
    kaze_menu_camera_coord_flags = 0;
    kaze_menu_render_rot_x = zero;
    kaze_menu_render_rot_z = 0;
    kaze_menu_render_rot_y = 0;
    kaze_menu_camera_trans_x = 0x140;
    kaze_menu_camera_trans_y = 0x120;
    do { } while (0);
    kaze_menu_camera_trans_z = 0x600;
    kaze_menu_camera_coord_word = zero;
    kaze_gs_light_init_alt_9454(0x140);
    kaze_task_carousel_state_init();
}

extern void kaze_sfx_play_dispatch(int);

/**
 * kaze_balloon_select_input  (menu, 0x80057a04)
 *
 * Up/down cycles balloon index kaze_task_record_index (0..4), recopying that
 * model into the display buffer; accept (0x20) enters zoom state 0x15,
 * cancel (0x40) exits 99 with kaze_task_menu_confirm_flag=1.
 */

void kaze_balloon_select_input(void)
{
    extern ModelBlock **kaze_menu_model1_data_ptr;
    extern int kaze_save_slot_unpack_buffer[];
    extern int kaze_input_edge;
    if ((kaze_input_edge & 0x4000) && kaze_task_record_index < 4) {
        if (kaze_balloon_switch_anim_counter != 0) {
            *kaze_menu_model1_data_ptr[4] = *(ModelBlock *)kaze_save_slot_unpack_buffer[kaze_task_record_index];
            GsLinkObject4(kaze_menu_balloon_model_ptr + 0xC, &kaze_menu_model1_gsobj, 0);
        }
        kaze_balloon_switch_anim_counter = 0x20;
        kaze_balloon_switch_direction_flag = 0;
        kaze_task_record_index = kaze_task_record_index + 1;
        kaze_sfx_play_dispatch(1);
    }
    if ((kaze_input_edge & 0x1000) && 0 < kaze_task_record_index) {
        if (kaze_balloon_switch_anim_counter != 0) {
            *kaze_menu_model1_data_ptr[4] = *(ModelBlock *)kaze_save_slot_unpack_buffer[kaze_task_record_index];
            GsLinkObject4(kaze_menu_balloon_model_ptr + 0xC, &kaze_menu_model1_gsobj, 0);
        }
        kaze_balloon_switch_anim_counter = 0x20;
        kaze_balloon_switch_direction_flag = 1;
        kaze_task_record_index = kaze_task_record_index - 1;
        kaze_sfx_play_dispatch(1);
    }
    if ((kaze_input_edge & 0x20) && kaze_balloon_switch_anim_counter == 0) {
        kaze_task_menu_state = 0x15;
        kaze_task_transition_counter = 0;
        kaze_sfx_play_dispatch(0x88);
    }
    if ((kaze_input_edge & 0x40) && kaze_balloon_switch_anim_counter == 0) {
        kaze_task_menu_confirm_flag = 1;
        kaze_task_menu_state = 99;
        kaze_sfx_play_dispatch(0x65);
    }
}

/**
 * kaze_registration_confirm_input  (menu, 0x80057d18)
 *
 * Two-option confirm cursor kaze_registration_confirm_cursor; accept on row
 * 0 enters zoom state 0x15, row 1 exits 99; cancel returns to balloon-select
 * state 0.
 */

void kaze_registration_confirm_input(void)
{
    extern s16 kaze_registration_confirm_cursor;
    extern int kaze_input_edge;
    int a;

    if ((kaze_input_edge & 0x4000) && kaze_registration_confirm_cursor < 1) {
        kaze_registration_confirm_cursor = kaze_registration_confirm_cursor + 1;
        kaze_sfx_play_dispatch(1);
    }
    if ((kaze_input_edge & 0x1000) && kaze_registration_confirm_cursor > 0) {
        kaze_registration_confirm_cursor = kaze_registration_confirm_cursor - 1;
        kaze_sfx_play_dispatch(1);
    }
    if (kaze_input_edge & 0x20) {
        if (kaze_registration_confirm_cursor == 0) {
            kaze_task_menu_state = 0x15;
            kaze_task_transition_counter = 0;
            a = 0x88;
        } else {
            kaze_task_menu_state = 99;
            a = 2;
        }
        kaze_sfx_play_dispatch(a);
    }
    if (kaze_input_edge & 0x40) {
        kaze_task_menu_state = 0;
        kaze_sfx_play_dispatch(0x65);
    }
}

extern int kaze_input_current_c;
extern int kaze_menu_input_prev_mask;
extern u8 kaze_balloon_color_edit_r;

/**
 * kaze_balloon_paint_input  (menu, 0x80057e2c)
 *
 * Paint editor input: held 0x20 paints RGB kaze_balloon_paint_color_r/48/4c
 * into hovered poly
 * (&kaze_model_prim_ptr_table)[kaze_balloon_paint_hovered_poly_idx]+4..6,
 * 0x40 eyedrops it, 0x10 opens RGB edit (0x1e), 0x800 copies the model back
 * and exits via 0x16.
 */

void kaze_balloon_paint_input(void)
{
    extern short kaze_registration_confirm_cursor;
    extern int kaze_menu_model1_data_ptr;
    extern struct BalloonImg *kaze_save_slot_unpack_buffer[];
    extern int kaze_input_edge;
    extern u8 kaze_balloon_paint_color_b;
    extern u8 kaze_balloon_paint_color_g;
    extern u8 kaze_balloon_paint_color_r;
    int poly;
    int *unused_ptr;

    if ((kaze_input_current_c & 0x20) != 0 && (kaze_menu_input_prev_mask & 0x20) == 0) {
        kaze_balloon_paint_held_flag = 1;
    }
    if ((kaze_input_current_c & 0x20) == 0) {
        if ((kaze_menu_input_prev_mask & 0x20) == 0) {
            kaze_balloon_paint_held_flag = 0;
        }
    } else if (kaze_balloon_paint_held_flag != 0 && kaze_balloon_paint_hovered_poly_idx != -1 &&
               (poly = kaze_model_prim_ptr_table[kaze_balloon_paint_hovered_poly_idx],
                *(char *)(poly + 4) != kaze_balloon_paint_color_r ||
                *(u8 *)(poly + 5) != kaze_balloon_paint_color_g ||
                *(char *)(poly + 6) != kaze_balloon_paint_color_b)) {
        *(char *)(kaze_model_prim_ptr_table[kaze_balloon_paint_hovered_poly_idx] + 4) = kaze_balloon_paint_color_r;
        *(u8 *)(kaze_model_prim_ptr_table[kaze_balloon_paint_hovered_poly_idx] + 5) = kaze_balloon_paint_color_g;
        *(char *)(kaze_model_prim_ptr_table[kaze_balloon_paint_hovered_poly_idx] + 6) = kaze_balloon_paint_color_b;
        kaze_sfx_play_dispatch(0x76);
    }
    if ((kaze_input_edge & 0x40) != 0 && kaze_balloon_paint_hovered_poly_idx != -1) {
        kaze_balloon_paint_color_r = *(char *)(kaze_model_prim_ptr_table[kaze_balloon_paint_hovered_poly_idx] + 4);
        kaze_balloon_paint_color_g = *(u8 *)(kaze_model_prim_ptr_table[kaze_balloon_paint_hovered_poly_idx] + 5);
        kaze_balloon_paint_color_b = *(char *)(kaze_model_prim_ptr_table[kaze_balloon_paint_hovered_poly_idx] + 6);
        kaze_sfx_play_dispatch(0x77);
    }
    if ((kaze_input_edge & 0x10) != 0) {
        kaze_task_menu_state = 0x1e;
        kaze_balloon_color_edit_r = kaze_balloon_paint_color_r;
        kaze_balloon_color_edit_g = kaze_balloon_paint_color_g;
        kaze_balloon_color_edit_b = kaze_balloon_paint_color_b;
        kaze_sfx_play_dispatch(0x67);
    }
    if ((kaze_input_edge & 0x800) != 0) {
        register struct BalloonImg *dst asm("$7");
        register struct BalloonImg *src asm("$6");
        kaze_task_menu_state = 0x16;
        dst = kaze_save_slot_unpack_buffer[kaze_task_record_index];
        src = *(struct BalloonImg **)(kaze_menu_model1_data_ptr + 0x10);
        kaze_registration_confirm_cursor = 1;
        *dst = *src;
        kaze_sfx_play_dispatch(2);
    }
}

extern unsigned int kaze_input_hold_timer;

/**
 * kaze_balloon_color_channel_edit  (menu, 0x80058164)
 *
 * RGB editor: up/down picks channel kaze_balloon_color_edit_channel (0..2);
 * left/right adjusts (&kaze_balloon_color_edit_r)[ch] by 2 (8 after hold
 * kaze_input_hold_timer>=0x11); accept commits to
 * kaze_balloon_paint_color_r/48/4c, cancel back to 0x14.
 */

void kaze_balloon_color_channel_edit(void)
{
    extern int kaze_input_edge;
    extern u8 kaze_balloon_paint_color_b;
    extern u8 kaze_balloon_paint_color_g;
    extern u8 kaze_balloon_paint_color_r;
    u8 *p;
    unsigned a;

    if ((kaze_input_edge & 0x4000) && kaze_balloon_color_edit_channel < 2) {
        kaze_balloon_color_edit_channel += 1;
        kaze_sfx_play_dispatch(0x68);
    }
    if ((kaze_input_edge & 0x1000) && kaze_balloon_color_edit_channel > 0) {
        kaze_balloon_color_edit_channel -= 1;
        kaze_sfx_play_dispatch(0x68);
    }
    if (kaze_input_current_c & 0x2000) {
        if (kaze_input_hold_timer >= 0x11) {
            p = &kaze_balloon_color_edit_r; p += kaze_balloon_color_edit_channel;
            a = *p;
            if (a < 0xF8) {
                *p = a + 8;
            } else {
                goto set_ff;
            }
        } else {
            p = &kaze_balloon_color_edit_r; p += kaze_balloon_color_edit_channel;
            a = *p;
            if (a < 0xFE) {
                *p = a + 2;
            } else {
set_ff:
                *p = 0xFF;
            }
        }
    }
    if (kaze_input_current_c & 0x8000) {
        if (kaze_input_hold_timer >= 0x11) {
            p = &kaze_balloon_color_edit_r; p += kaze_balloon_color_edit_channel;
            a = *p;
            if (a >= 8) {
                *p = a - 8;
            } else {
                goto set_zero;
            }
        } else {
            p = &kaze_balloon_color_edit_r; p += kaze_balloon_color_edit_channel;
            a = *p;
            if (a >= 2) {
                *p = a - 2;
            } else {
set_zero:
                *p = 0;
            }
        }
    }
    if (kaze_input_edge & 0x20) {
        kaze_task_menu_state = 0x14;
        kaze_balloon_paint_color_r = kaze_balloon_color_edit_r;
        kaze_balloon_paint_color_g = kaze_balloon_color_edit_g;
        kaze_balloon_paint_color_b = kaze_balloon_color_edit_b;
        kaze_sfx_play_dispatch(0x88);
    }
    if (kaze_input_edge & 0x40) {
        kaze_task_menu_state = 0x14;
        kaze_sfx_play_dispatch(0x65);
    }
    if ((kaze_input_edge & 0x2000) && (&kaze_balloon_color_edit_r)[kaze_balloon_color_edit_channel] != 0xFF) {
        kaze_sfx_play_dispatch(0x73);
    } else if ((kaze_input_edge & 0x8000) && (&kaze_balloon_color_edit_r)[kaze_balloon_color_edit_channel] != 0) {
        kaze_sfx_play_dispatch(0x73);
    } else if (!(kaze_input_current_c & 0xA000) || (&kaze_balloon_color_edit_r)[kaze_balloon_color_edit_channel] == 0 ||
        (&kaze_balloon_color_edit_r)[kaze_balloon_color_edit_channel] == 0xFF) {
        kaze_sfx_stop_dispatch(0x73);
    }
}

extern short kaze_balloon_entry_grid_sprite_x[];
extern short kaze_balloon_entry_grid_sprite_y[];

/**
 * kaze_balloon_editor_zoom_in_anim  (menu, 0x8005845c)
 *
 * 0x20-frame transition into the paint editor: slides label/panel sprite
 * positions and camera kaze_menu_camera_pos_x/e4/e8 by counter
 * kaze_task_transition_counter, then sets state 0x14.
 */

void kaze_balloon_editor_zoom_in_anim(int slot)
{
    extern int kaze_menu_camera_coord_flags;
    extern int kaze_menu_camera_pos_x[];
    register int loop_cnt __asm__("$7");   /* a3 : outer / countdown counter */
    int base;
    register int row_step __asm__("$11"); /* t3 : -0x1c stepping            */
    register int row_off __asm__("$10");  /* t2 : row offset                */
    register int c8    __asm__("$13");  /* t5 : counter * 8               */
    register int inner_i __asm__("$6");   /* a2 : inner counter             */
    register int accum __asm__("$5");   /* a1 : accumulator               */
    register int work_reg __asm__("$4");   /* a0 : inner / abe6c offset       */
    register int col_off __asm__("$2");
    register int idx __asm__("$3");
    int frames_left;
    int counter;
    int off;
    int off1;
    int off2;
    int val;

    loop_cnt = 0;
    base = (slot * 15) << 4;
    row_step = -0x1c;
    row_off = 0;
    counter = kaze_task_transition_counter;
    frames_left = 0x20 - counter;
    c8 = counter * 8;
    do {
        int t1;
        int step;
        inner_i = 0;
        t1 = c8 + -0x5e;
        step = row_step;
        accum = 0;
        work_reg = row_off;
        do {
            col_off = accum - t1;
            accum = accum + 0xc;
            idx = work_reg + base;
            *(short *)((char *)kaze_balloon_entry_grid_sprite_x + idx) = col_off + -0xa0;
            *(short *)((char *)kaze_balloon_entry_grid_sprite_y + idx) = step;
            work_reg = work_reg + 0x10;
            inner_i = inner_i + 1;
        } while (inner_i < 3);
        row_step = row_step + 0x18;
        loop_cnt = loop_cnt + 1;
        row_off = row_off + 0x30;
    } while (loop_cnt < 5);
    loop_cnt = 4;
    { register int t100 __asm__("$2"); t100 = slot * 100; work_reg = t100 + 0x50; }
    counter = kaze_task_transition_counter;
    do {
        *(short *)((char *)kaze_balloon_name_row_sprites_x + work_reg) = -0x90 - counter * 8;
        loop_cnt = loop_cnt + -1;
        work_reg = work_reg + -0x14;
    } while (-1 < loop_cnt);
    {
        int a3v = kaze_task_transition_counter;
        int a1v = slot;
        __asm__("" : "=r"(a1v) : "0"(a1v), "r"(a3v));
        kaze_options_header_set_y(a1v, 3, a3v * -4);
    }
    val = -0x90 - frames_left * 3;
    loop_cnt = 3;
    off1 = slot * 0xa0 + 0x3c;
    do {
        *(short *)((char *)kaze_balloon_editor_label_sprites_x + off1) = val;
        loop_cnt = loop_cnt + -1;
        off1 = off1 + -0x14;
    } while (-1 < loop_cnt);
    loop_cnt = 4;
    { register int t3 __asm__("$2"); t3 = frames_left * 3; work_reg = t3 + 0x50; }
    off2 = slot * 0xa0 + 0x50;
    do {
        *(short *)((char *)kaze_balloon_editor_label_sprites_x + off2) = work_reg;
        loop_cnt = loop_cnt + 1;
        off2 = off2 + 0x14;
    } while (loop_cnt < 8);
    { register int t60 __asm__("$2"); t60 = frames_left * 0x60; accum = -t60; }
    kaze_balloon_editor_preview_sprite_x[slot * 10] = frames_left * 4 + 0x60;
    kaze_balloon_editor_swatch_tiles_x[slot * 6] = frames_left * 4 + 0x68;
    __asm__ __volatile__("la\t%0, kaze_menu_camera_pos_x" : "=r"(work_reg));
    *(int *)(work_reg + 0) = frames_left * 10;
    {
        int rawe;
        register int sh __asm__("$2");
        if (accum < 0) {
            accum = accum + 0x1f;
        }
        rawe = kaze_task_transition_counter;
        sh = accum >> 5;
        *(int *)(work_reg + 4) = sh;
        off = rawe * -0x200;
    }
    if (off < 0) {
        off = off + 0x1f;
    }
    *(int *)(work_reg + 8) = (off >> 5) + 0x600;
    kaze_menu_camera_coord_flags = 0;
    if (kaze_task_transition_counter < 0x20) {
        kaze_task_transition_counter = kaze_task_transition_counter + 1;
    } else {
        kaze_task_menu_state = 0x14;
        kaze_task_transition_counter = 0;
    }
    return;
}

extern signed char kaze_task_name_letter0_table[];

/**
 * kaze_balloon_editor_zoom_out_anim  (menu, 0x800586c0)
 *
 * 0x20-frame reverse transition out of the paint editor; on completion
 * promotes pending flags kaze_task_name_letter0_table[balloon*3+i] from -2
 * to -1, calls kaze_task_name_letter_models_link, and sets state 0x29.
 */

void kaze_balloon_editor_zoom_out_anim(int idx)
{
    register int idx_r __asm__("$6");
    register int i __asm__("$4");
    int base;
    int offset;
    int counter;
    register int phase __asm__("$5");
    signed char *flags;

    idx_r = idx;
    i = 3;
    base = idx_r * 0xA0;
    counter = kaze_task_transition_counter;
    offset = base + 0x3C;
    do {
        *(short *)((char *)kaze_balloon_editor_label_sprites_x + offset) = -0x90 - counter * 3;
        i--;
        offset -= 0x14;
    } while (i >= 0);

    i = 4;
    base = idx_r * 0xA0;
    counter = kaze_task_transition_counter;
    offset = base + 0x50;
    do {
        *(short *)((char *)kaze_balloon_editor_label_sprites_x + offset) = counter * 3 + 0x50;
        i++;
        offset += 0x14;
    } while (i < 8);

    phase = kaze_task_transition_counter;
    kaze_balloon_editor_preview_sprite_x[idx_r * 10] = phase * 4 + 0x60;
    kaze_balloon_editor_swatch_tiles_x[idx_r * 6] = phase * 4 + 0x68;

    if (phase < 0x20) {
        register int next __asm__("$2");

        next = phase + 1;
        kaze_task_transition_counter = next;
        return;
    }

    i = 0;
    flags = kaze_task_name_letter0_table;
    kaze_task_menu_state = 0x29;
    kaze_task_transition_counter = 0;
    do {
        signed char *slot = (signed char *)(kaze_task_record_index * 3 + (int)flags) + i;

        if (*slot == -2) {
            *slot = -1;
        }
        i++;
    } while (i < 3);

    kaze_task_name_letter_models_link();
}

#define XR(i)  (*(short *)((int)kaze_balloon_paint_cursor_x + (i)))
#define YR(i)  (*(short *)((int)kaze_balloon_paint_cursor_y + (i)))
#define XRU(i) (*(unsigned short *)((int)kaze_balloon_paint_cursor_x + (i)))

/**
 * kaze_balloon_paint_cursor_move  (menu, 0x80058834)
 *
 * Moves the free paint cursor ((&kaze_balloon_paint_cursor_x/1fe)[buf*8])
 * with held d-pad kaze_input_current_c, step 1 (or 4 after hold timer
 * kaze_input_hold_timer>=0x11), clamped to +/-0x90 x and +/-0x68 y.
 */

void kaze_balloon_paint_cursor_move(int buf)
{
    char pad[64];
    int si = (((buf + 1) & 1) << 4);
    unsigned int u3;
    register int di asm("$5");
    di = buf << 4;
    XR(di) = XRU(si);
    u3 = kaze_input_current_c & 0x2000;
    YR(di) = *(unsigned short *)((int)kaze_balloon_paint_cursor_y + si);
    if (u3 != 0) {
        register int v asm("$2");
        register int cpy asm("$3");
        __asm__("" : "=m"(pad));
        if (0x10 < kaze_input_hold_timer) {
            v = XR(di); cpy = v; __asm__("" : "=r"(cpy) : "0"(cpy));
            if (v < 0x8c) v = cpy + 4; else v = 0x90;
        } else {
            v = XR(di); cpy = v; __asm__("" : "=r"(cpy) : "0"(cpy));
            if (v < 0x8f) v = cpy + 1; else v = 0x90;
        }
        XR(di) = v;
    }
    if ((kaze_input_current_c & 0x8000) != 0) {
        register int idx asm("$3");
        register int v asm("$2");
        register int cpy asm("$5");
        idx = buf << 4;
        if (0x10 < kaze_input_hold_timer) {
            v = XR(idx); cpy = v; __asm__("" : "=r"(cpy) : "0"(cpy));
            if (v < -0x8c) v = -0x90; else v = cpy - 4;
        } else {
            v = XR(idx); cpy = v; __asm__("" : "=r"(cpy) : "0"(cpy));
            if (v < -0x8f) v = -0x90; else v = cpy - 1;
        }
        XR(idx) = v;
    }
    if ((kaze_input_current_c & 0x4000) != 0) {
        register int idx asm("$3");
        register int v asm("$2");
        register int cpy asm("$5");
        idx = buf << 4;
        if (0x10 < kaze_input_hold_timer) {
            v = YR(idx); cpy = v; __asm__("" : "=r"(cpy) : "0"(cpy));
            if (v < 0x64) v = cpy + 4; else v = 0x68;
        } else {
            v = YR(idx); cpy = v; __asm__("" : "=r"(cpy) : "0"(cpy));
            if (v < 0x67) v = cpy + 1; else v = 0x68;
        }
        YR(idx) = v;
    }
    if ((kaze_input_current_c & 0x1000) != 0) {
        register int idx asm("$4");
        register int v asm("$2");
        register int cpy asm("$3");
        if (0x10 < kaze_input_hold_timer) {
            idx = buf << 4;
            v = YR(idx); cpy = v; __asm__("" : "=r"(cpy) : "0"(cpy));
            if (v < -0x64) v = -0x68; else v = cpy - 4;
        } else {
            idx = buf << 4;
            v = YR(idx); cpy = v; __asm__("" : "=r"(cpy) : "0"(cpy));
            if (v < -0x67) v = -0x68; else v = cpy - 1;
        }
        YR(idx) = v;
    }
    return;
}

extern long ratan2(int, int);
extern int rsin(int);
extern int rcos(int);
extern void RotMatrix(void *, void *);
extern void kaze_trans_matrix(void *, void *);
extern void GsGetLws(int, MATRIX *, MATRIX *);
extern void GsSetLightMatrix(MATRIX *);
extern void GsSetLsMatrix(MATRIX *);
extern int kaze_menu_camera_matrix;
extern int kaze_model_vertex_ptr_table;

/**
 * kaze_balloon_paint_poly_pick  (menu, 0x80058aa8)
 *
 * Picks the balloon polygon under the paint cursor: RotMatrix/GsGetLws
 * setup, RotNclip3-projects up to 0x108 tris from vertex table
 * kaze_model_vertex_ptr_table, point-in-triangle tests via ratan2/rsin;
 * returns index or -1.
 */

int kaze_balloon_paint_poly_pick(int arg0) {
    extern short kaze_balloon_paint_tri_vertex_ax, kaze_balloon_paint_tri_vertex_bx, kaze_balloon_paint_tri_vertex_cx, kaze_balloon_paint_tri_vertex_ay, kaze_balloon_paint_tri_vertex_by, kaze_balloon_paint_tri_vertex_cy;
    extern int kaze_menu_camera_pos_x;
    extern int kaze_menu_camera_rot_x;
    register int angle_a asm("$16") = arg0;
    int sin_prod;
    int cos_prod;
    int angle_b;
    unsigned int usign;
    register int pofs asm("$19");
    int vtx_off;
    int tri_idx;
    int skip_idx;
    int result;
    register MATRIX *pa asm("$5");
    MATRIX *pb;
    register MATRIX *pa2 asm("$6");
    MATRIX MStack_a0;
    MATRIX MStack_80;
    int sc_a[4];
    int sc_b[4];
    int vtx_a_xy;
    int vtx_b_xy;
    int vtx_c_xy;
    int clip_p;
    int clip_otz;
    int clip_flag;

    result = -1;
    RotMatrix(&kaze_menu_camera_rot_x, &kaze_menu_camera_matrix);
    kaze_trans_matrix(&kaze_menu_camera_matrix, &kaze_menu_camera_pos_x);
    pa = &MStack_a0;
    pb = &MStack_80;
    pa2 = pb;
    __asm__ volatile("");
    tri_idx = 0;
    skip_idx = 0;
    __asm__("" : "=r"(angle_a) : "0"(angle_a));
    pofs = angle_a * 0x10;
    __asm__ volatile("" : : "r"(pofs));
    vtx_off = 0;
    GsGetLws(kaze_menu_model1_coord_ptr, pa, pa2);
    GsSetLightMatrix(&MStack_a0);
    GsSetLsMatrix(pb);
    do {
        if (tri_idx == kaze_balloon_editor_skip_face_table[skip_idx]) {
            skip_idx = skip_idx + 1;
        } else {
            if (0 < RotNclip3(*(int *)((int)&kaze_model_vertex_ptr_table + vtx_off),
                              *(int *)((int)&kaze_model_vertex_ptr_table + 4 + vtx_off),
                              *(int *)((int)&kaze_model_vertex_ptr_table + 8 + vtx_off), &vtx_a_xy, &vtx_b_xy, &vtx_c_xy,
                              &clip_p, &clip_otz, &clip_flag)) {
                kaze_balloon_paint_tri_vertex_ax = vtx_a_xy;
                kaze_balloon_paint_tri_vertex_ay = vtx_a_xy >> 0x10;
                kaze_balloon_paint_tri_vertex_bx = vtx_b_xy;
                kaze_balloon_paint_tri_vertex_by = vtx_b_xy >> 0x10;
                kaze_balloon_paint_tri_vertex_cx = vtx_c_xy;
                kaze_balloon_paint_tri_vertex_cy = vtx_c_xy >> 0x10;
                angle_a = ratan2(kaze_balloon_paint_tri_vertex_bx - kaze_balloon_paint_tri_vertex_ax, (vtx_a_xy >> 0x10) - (vtx_b_xy >> 0x10)) + 0x400;
                angle_b = ratan2(kaze_balloon_paint_tri_vertex_cx - kaze_balloon_paint_tri_vertex_ax, kaze_balloon_paint_tri_vertex_ay - kaze_balloon_paint_tri_vertex_cy);
                sc_a[0] = rsin(angle_a);
                sc_a[1] = rcos(angle_a);
                sc_b[0] = rsin(angle_b);
                sc_b[1] = rcos(angle_b);
                sin_prod = sc_a[0] * sc_b[0];
                cos_prod = sc_a[1] * sc_b[1];
                usign = ~(sin_prod + cos_prod);
                angle_a = ratan2(kaze_balloon_paint_tri_vertex_bx - kaze_balloon_paint_tri_vertex_ax, kaze_balloon_paint_tri_vertex_ay - kaze_balloon_paint_tri_vertex_by) + 0x400;
                angle_b = ratan2(*(short *)((int)kaze_balloon_paint_cursor_x + pofs) - kaze_balloon_paint_tri_vertex_ax,
                               kaze_balloon_paint_tri_vertex_ay - *(short *)((int)kaze_balloon_paint_cursor_x + pofs + 2));
                sc_a[0] = rsin(angle_a);
                sc_a[1] = rcos(angle_a);
                sc_b[0] = rsin(angle_b);
                sc_b[1] = rcos(angle_b);
                usign = usign >> 0x1f;
                if (usign == (unsigned int)~(sc_a[0] * sc_b[0] + sc_a[1] * sc_b[1]) >> 0x1f) {
                    angle_a = ratan2(kaze_balloon_paint_tri_vertex_cx - kaze_balloon_paint_tri_vertex_bx, kaze_balloon_paint_tri_vertex_by - kaze_balloon_paint_tri_vertex_cy) + 0x400;
                    angle_b = ratan2(*(short *)((int)kaze_balloon_paint_cursor_x + pofs) - kaze_balloon_paint_tri_vertex_bx,
                                   kaze_balloon_paint_tri_vertex_by - *(short *)((int)kaze_balloon_paint_cursor_x + pofs + 2));
                    sc_a[0] = rsin(angle_a);
                    sc_a[1] = rcos(angle_a);
                    sc_b[0] = rsin(angle_b);
                    sc_b[1] = rcos(angle_b);
                    if (usign == (unsigned int)~(sc_a[0] * sc_b[0] + sc_a[1] * sc_b[1]) >> 0x1f) {
                        angle_a = ratan2(kaze_balloon_paint_tri_vertex_ax - kaze_balloon_paint_tri_vertex_cx, kaze_balloon_paint_tri_vertex_cy - kaze_balloon_paint_tri_vertex_ay) + 0x400;
                        angle_b = ratan2(*(short *)((int)kaze_balloon_paint_cursor_x + pofs) - kaze_balloon_paint_tri_vertex_cx,
                                       kaze_balloon_paint_tri_vertex_cy - *(short *)((int)kaze_balloon_paint_cursor_x + pofs + 2));
                        sc_a[0] = rsin(angle_a);
                        sc_a[1] = rcos(angle_a);
                        sc_b[0] = rsin(angle_b);
                        sc_b[1] = rcos(angle_b);
                        if (usign == (unsigned int)~(sc_a[0] * sc_b[0] + sc_a[1] * sc_b[1]) >> 0x1f) {
                            result = tri_idx;
                            goto done;
                        }
                    }
                }
            }
        }
        tri_idx = tri_idx + 1;
        vtx_off = vtx_off + 0xc;
    } while (tri_idx < 0x108);
done:
    return result;
}

/**
 * kaze_balloon_paint_highlight_update  (menu, 0x80058f64)
 *
 * Animates the rainbow LINE_F4 outline (kaze_balloon_paint_highlight_prims
 * block) around the hovered poly using color phases
 * kaze_balloon_highlight_phase_r/5c/60 (mod 0x60) into table
 * kaze_balloon_color_shade_table and the projected vertices.
 */

void kaze_balloon_paint_highlight_update(int idx)
{
    extern u16 kaze_balloon_paint_tri_vertex_ax, kaze_balloon_paint_tri_vertex_bx, kaze_balloon_paint_tri_vertex_cx, kaze_balloon_paint_tri_vertex_ay, kaze_balloon_paint_tri_vertex_by;
    extern u16 kaze_balloon_paint_tri_vertex_cy;
    extern u8 kaze_balloon_paint_highlight_prims[];
    u16 p0;
    u16 p1;
    u16 p2;
    u16 p3;
    u16 p4;
    u16 p5;
    register int raw0 asm("$6");
    int phase0;
    register int sign0 asm("$3");
    register int idx_keep asm("$10");
    register int phase_keep asm("$11");

    raw0 = kaze_balloon_highlight_phase_r + 1;
    sign0 = raw0 >> 0x1f;
    phase0 = (kaze_balloon_highlight_phase_r = raw0 % 0x60);
    kaze_balloon_highlight_phase_g = (kaze_balloon_highlight_phase_g + 1) % 0x60;
    kaze_balloon_highlight_phase_b = (kaze_balloon_highlight_phase_b + 1) % 0x60;
    phase_keep = phase0;

    if (kaze_balloon_paint_hovered_poly_idx != -1) {
        char pad[8];
        register u8 *base asm("$9");
        u8 *prim;
        int idx_ofs;

        __asm__("" : "=m"(pad));
        base = kaze_balloon_paint_highlight_prims;
        __asm__ volatile("" : : "r"(base));
        p0 = kaze_balloon_paint_tri_vertex_ax;
        p3 = kaze_balloon_paint_tri_vertex_ay;
        p1 = kaze_balloon_paint_tri_vertex_bx;
        p4 = kaze_balloon_paint_tri_vertex_by;
        p2 = kaze_balloon_paint_tri_vertex_cx;
        p5 = kaze_balloon_paint_tri_vertex_cy;
        idx_keep = idx;
        __asm__("" : "=r"(idx_keep) : "0"(idx_keep));
        idx_ofs = (idx_keep << 3) - idx_keep;
        idx_ofs <<= 2;
        prim = base + idx_ofs;
        *(u16 *)(prim + 0x08) = p0;
        *(u16 *)(prim + 0x0A) = p3;
        *(u16 *)(prim + 0x0C) = p1;
        *(u16 *)(prim + 0x0E) = p4;
        *(u16 *)(prim + 0x10) = p2;
        *(u16 *)(prim + 0x12) = p5;
        *(u16 *)(prim + 0x14) = p0;
        *(u16 *)(prim + 0x16) = p3;
        prim[4] = kaze_balloon_color_shade_table[phase_keep] << 3;
        prim[5] = kaze_balloon_color_shade_table[kaze_balloon_highlight_phase_g] << 3;
        prim[6] = kaze_balloon_color_shade_table[kaze_balloon_highlight_phase_b] << 3;
    }
}

/**
 * kaze_balloon_view_rotate_input  (menu, 0x800590fc)
 *
 * Paint-editor view control: buttons rotate model yaw kaze_menu_camera_rot_y
 * / pitch kaze_menu_camera_rot_x in timed 0x10-frame steps and zoom camera
 * kaze_menu_camera_scroll/e8 in 0x14-frame steps (direction parity
 * kaze_balloon_view_zoom_dir_counter).
 */

void kaze_balloon_view_rotate_input(void)
{
    extern int kaze_menu_camera_scroll,kaze_menu_camera_pos_z,kaze_menu_camera_coord_flags;
    extern unsigned short kaze_menu_camera_rot_x,kaze_menu_camera_rot_y;
    extern int kaze_balloon_view_pitch_timer,kaze_balloon_view_yaw_timer,kaze_balloon_view_zoom_timer,kaze_balloon_view_pitch_dir_counter,kaze_balloon_view_yaw_dir_flag,kaze_balloon_view_zoom_dir_counter,kaze_input_edge;
    unsigned short *var_v1; unsigned short var_v0; int minus_temp;
    if (kaze_balloon_view_yaw_timer != 0) {
        kaze_balloon_view_yaw_timer -= 1;
        if (kaze_balloon_view_yaw_dir_flag != 0) { var_v1 = &kaze_menu_camera_rot_y; var_v0 = *var_v1 - 0x20; }
        else { var_v1 = &kaze_menu_camera_rot_y; goto block_8; }
        goto block_9;
    }
    if (kaze_balloon_view_pitch_timer != 0) {
        kaze_balloon_view_pitch_timer -= 1;
        if (kaze_balloon_view_pitch_dir_counter & 2) { var_v1 = &kaze_menu_camera_rot_x; minus_temp = *var_v1; var_v0 = minus_temp - 0x20; }
        else { var_v1 = &kaze_menu_camera_rot_x;
block_8:   var_v0 = *var_v1 + 0x20; }
block_9:  *var_v1 = var_v0; kaze_menu_camera_coord_flags = 0; return;
    }
    if (kaze_balloon_view_zoom_timer != 0) {
        register int *ps asm("$2"); register int sv asm("$3"); register int pz asm("$4"); register int flag asm("$2");
        sv = kaze_balloon_view_zoom_timer - 1;
        flag = kaze_balloon_view_zoom_dir_counter;
        kaze_balloon_view_zoom_timer = sv;
        if (flag & 1) { ps = &kaze_menu_camera_scroll; sv = *ps - 8; pz = kaze_menu_camera_pos_z - 0x20; }
        else { ps = &kaze_menu_camera_scroll; sv = *ps + 8; pz = kaze_menu_camera_pos_z + 0x20; }
        *ps = sv; kaze_menu_camera_pos_z = pz; kaze_menu_camera_coord_flags = 0; return;
    }
    if (kaze_input_edge & 8) { kaze_balloon_view_yaw_timer = 0x10; kaze_balloon_view_yaw_dir_flag = 0; goto block_23; }
    if (kaze_input_edge & 4) { kaze_balloon_view_yaw_timer = 0x10; kaze_balloon_view_yaw_dir_flag = 1; goto block_23; }
    if (kaze_input_edge & 2) { kaze_balloon_view_pitch_timer = 0x10; kaze_balloon_view_pitch_dir_counter += 1; goto block_23; }
    if (kaze_input_edge & 1) { kaze_balloon_view_zoom_timer = 0x14; kaze_balloon_view_zoom_dir_counter += 1;
block_23: kaze_balloon_view_rotate_input(); }
}

extern void kaze_linef3_gradient_bar(int, int, int, int, int);
extern void kaze_options_header_prims_add(int, int, int);
void kaze_balloon_switch_swing_anim();
extern void kaze_title_model_sort_dispatch(int, int *);
extern void kaze_drmove_addprim_a0ec(int, int);

/**
 * kaze_balloon_select_draw  (menu, 0x8005930c)
 *
 * Draws the balloon-select screen: cursor ring at row
 * kaze_task_record_index, header group 3, list prims
 * kaze_task_name_grid_draw/kaze_task_menu_row_sprites_draw, swing
 * kaze_balloon_switch_swing_anim, spins model (kaze_menu_camera_rot_y+=8)
 * and renders via kaze_title_model_sort_dispatch.
 */

void kaze_balloon_select_draw(int arg0)
{
    extern int kaze_menu_gs_ot_buf[];
    extern int kaze_menu_camera_coord_flags;
    extern u16 kaze_menu_camera_rot_y;
    u16 *rot;

    kaze_linef3_gradient_bar(arg0, 0x17, kaze_task_record_index * 0x18 + 0x5B, 0x72, 0x12);
    kaze_options_header_prims_add(arg0, 3, 1);
    kaze_task_name_grid_draw(arg0, 1);
    kaze_task_menu_row_sprites_draw(arg0, 1);
    kaze_balloon_switch_swing_anim();
    rot = &kaze_menu_camera_rot_y;
    kaze_menu_camera_coord_flags = 0;
    *rot = (*rot + 8) & 0xFFF;
    kaze_title_model_sort_dispatch(arg0, &kaze_menu_gs_ot_buf[arg0 * 5]);
    kaze_drmove_addprim_a0ec(arg0, 0xFFF);
}

/**
 * kaze_registration_confirm_draw  (menu, 0x800593d8)
 *
 * Draws the confirm dialog: cursor ring at kaze_registration_confirm_cursor,
 * header group 3, prims kaze_menu_two_row_sprites_draw_a210, spins the
 * balloon model and renders the 3D OT via kaze_title_model_sort_dispatch.
 */

void kaze_registration_confirm_draw(int arg)
{
    extern int kaze_menu_gs_ot_buf;
    extern int kaze_menu_camera_coord_flags;
    extern u16 kaze_menu_camera_rot_y;
    extern s16 kaze_registration_confirm_cursor;
    u16 *p;

    kaze_linef3_gradient_bar(arg, 0x17, kaze_registration_confirm_cursor * 0x18 + 0x5b, 0x72, 0x12);
    kaze_options_header_prims_add(arg, 3, 1);
    kaze_menu_two_row_sprites_draw_a210(arg, 1);
    p = &kaze_menu_camera_rot_y;
    kaze_menu_camera_coord_flags = 0;
    *p = (*p + 8) & 0xfff;
    kaze_title_model_sort_dispatch(arg, &kaze_menu_gs_ot_buf + arg * 5);
    kaze_drmove_addprim_a0ec(arg, 0xfff);
}

extern Slot kaze_menu_gs_ot_buf0_org[];
extern int AddPrim(void *, void *);

/**
 * kaze_balloon_paint_draw  (menu, 0x80059490)
 *
 * Draws the paint editor: panel prims
 * kaze_menu_prim_pair_draw_a6a4/kaze_menu_panel_grid_draw_a308, hovered-poly
 * highlight LINE_F4 (kaze_balloon_paint_highlight_prims) when
 * kaze_balloon_paint_hovered_poly_idx!=-1, then 3D model via
 * kaze_title_model_sort_primary.
 */

void kaze_balloon_paint_draw(int idx)
{
    extern void kaze_title_model_sort_primary(void *);
    extern char kaze_menu_gs_ot_buf[];
    extern char kaze_balloon_paint_highlight_prims[];
    kaze_menu_prim_pair_draw_a6a4();
    kaze_menu_panel_grid_draw_a308(idx, 1);
    if (kaze_balloon_paint_hovered_poly_idx != -1) {
        AddPrim((void *)kaze_menu_gs_ot_buf0_org[idx].prim, kaze_balloon_paint_highlight_prims + idx * 0x1C);
    }
    kaze_title_model_sort_primary(kaze_menu_gs_ot_buf + idx * 0x14);
    kaze_drmove_addprim_a0ec(idx, 0xFFF);
}

/**
 * kaze_balloon_editor_zoom_in_draw  (menu, 0x80059530)
 *
 * Draw pass for zoom-in state 0x15: header group 3 without reposition, list
 * prims kaze_task_name_grid_draw/kaze_task_menu_row_sprites_draw/kaze_menu_p
 * anel_grid_draw_a308 at animated positions, plus 3D model
 * kaze_title_model_sort_primary.
 */

void kaze_balloon_editor_zoom_in_draw(int idx)
{
    extern void kaze_title_model_sort_primary(int *);
    extern int kaze_menu_gs_ot_buf[];
    kaze_options_header_prims_add(idx, 3, 0);
    kaze_task_name_grid_draw(idx, 0);
    kaze_task_menu_row_sprites_draw(idx, 0);
    kaze_menu_panel_grid_draw_a308(idx, 0);
    kaze_title_model_sort_primary(kaze_menu_gs_ot_buf + idx * 5);
    kaze_drmove_addprim_a0ec(idx, 0xFFF);
    return;
}

/**
 * kaze_balloon_editor_zoom_out_draw  (menu, 0x800595ac)
 *
 * Draw pass for zoom-out state 0x16: panel prims
 * kaze_menu_panel_grid_draw_a308 plus the 3D balloon model via
 * kaze_title_model_sort_primary and fade overlay.
 */

void kaze_balloon_editor_zoom_out_draw(int idx)
{
    extern void kaze_title_model_sort_primary(int *);
    extern int kaze_menu_gs_ot_buf;
    kaze_menu_panel_grid_draw_a308(idx, 0);
    kaze_title_model_sort_primary(&kaze_menu_gs_ot_buf + idx * 5);
    kaze_drmove_addprim_a0ec(idx, 0xFFF);
}

/**
 * kaze_balloon_color_editor_draw  (menu, 0x80059600)
 *
 * Draw pass for RGB-edit state 0x1e: popup prims via
 * kaze_menu_rgb_slider_panel_draw, then the 3D model
 * kaze_title_model_sort_primary and fade overlay kaze_drmove_addprim_a0ec.
 */

void kaze_balloon_color_editor_draw(int arg0)
{
    extern void kaze_title_model_sort_primary(int *);
    extern int kaze_menu_gs_ot_buf[];
    kaze_menu_rgb_slider_panel_draw();
    kaze_title_model_sort_primary(kaze_menu_gs_ot_buf + arg0 * 5);
    kaze_drmove_addprim_a0ec(arg0, 0xFFF);
}

/**
 * kaze_balloon_switch_swing_anim  (menu, 0x80059650)
 *
 * During balloon switch (counter kaze_balloon_switch_anim_counter) swings
 * camera yaw kaze_menu_camera_scroll with rcos, and at midpoint 0x10 copies
 * the newly selected model into the display buffer and relinks GsDOBJ2
 * kaze_menu_model1_gsobj.
 */

void kaze_balloon_switch_swing_anim(void)
{
    extern ModelBlock **kaze_menu_model1_data_ptr;
    extern int kaze_save_slot_unpack_buffer[];
    int angle;
    int t;
    int base;

    if (kaze_balloon_switch_anim_counter != 0) {
        kaze_balloon_switch_anim_counter = kaze_balloon_switch_anim_counter - 1;
        if (kaze_balloon_switch_anim_counter == 0x10) {
            *kaze_menu_model1_data_ptr[4] = *(ModelBlock *)kaze_save_slot_unpack_buffer[kaze_task_record_index];
            GsLinkObject4(kaze_menu_balloon_model_ptr + 0xC, &kaze_menu_model1_gsobj, 0);
        }
        angle = (0x20 - kaze_balloon_switch_anim_counter) * 0x40;
        if (kaze_balloon_switch_direction_flag == 0) {
            if (kaze_balloon_switch_anim_counter >= 0x10) {
                kaze_menu_camera_scroll = rcos(angle) * 0x480 / 4096 + -0x4E0;
            } else {
                kaze_menu_camera_scroll = rcos(angle) * 0x480 / 4096 + 0x420;
            }
        } else {
            if (kaze_balloon_switch_anim_counter >= 0x10) {
                t = rcos(angle) * 0x480 / 4096;
                base = 0x420;
            } else {
                t = rcos(angle) * 0x480 / 4096;
                base = -0x4E0;
            }
            kaze_menu_camera_scroll = base - t;
        }
    }
}

/**
 * kaze_balloon_select_row_sprites_init  (menu, 0x80059858)
 *
 * Builds the 5 balloon-name row SPRTs per buffer at
 * kaze_balloon_name_row_sprites (CLUT 0x340,0xf2, tpage 0x340); their X
 * coords are animated by the editor zoom.
 */

void kaze_balloon_select_row_sprites_init(void)
{
    extern void SetDrawTPage(char *, int, int, unsigned int);
    extern u16 GetTPage(int, int, int, int);
    extern u16 GetClut(int, int);
    extern void SetShadeTex(char *, int);
    extern void SetSprt(char *);
    u16 clut;
    int inner;
    int offset;
    char *row;
    char *base;
    short y;
    int outer_offset;
    int outer;

    outer = 0;
    base = kaze_balloon_name_row_sprites;
    outer_offset = 0;
    do {
        inner = 0;
        y = -0x1C;
        offset = outer_offset;
        row = base;
        do {
            char *prim = (char *)(outer_offset + (int)row);
            char *slot;
            short sc;
            int clut_x, clut_y;

            SetSprt(prim);
            SetShadeTex(prim, 1);
            __asm__ volatile("addiu %0,$0,0x340" : "=r"(clut_x));
            __asm__ volatile("addiu %0,$0,0xF2" : "=r"(clut_y));
            slot = (char *)(offset + (int)base);
            __asm__ volatile("addiu %0,$0,-144" : "=r"(sc));
            *(short *)(slot + 8) = sc;
            __asm__ volatile("addiu %0,$0,128" : "=r"(sc));
            *(short *)(slot + 0x10) = sc;
            __asm__ volatile("addiu %0,$0,16" : "=r"(sc));
            *(short *)(slot + 0x12) = sc;
            *(short *)(slot + 0xA) = y;
            *(char *)(slot + 0xC) = 0;
            *(char *)(slot + 0xD) = inner << 4;
            clut = GetClut(clut_x, clut_y);
            y += 0x18;
            *(u16 *)(kaze_balloon_name_row_clut + offset) = clut;
            offset += 0x14;
            inner++;
            row += 0x14;
        } while (inner < 5);
        clut = GetTPage(0, 0, 0x340, 0x100);
        SetDrawTPage(kaze_balloon_name_row_tpage_prims + outer * 8, 0, 1, clut);
        outer++;
        outer_offset += 100;
    } while (outer < 2);
}

/**
 * kaze_registration_confirm_sprites_init  (menu, 0x80059988)
 *
 * Builds the 2 confirm-option label SPRTs per buffer at
 * kaze_registration_confirm_label_sprites (CLUT 0x340,0xf2) plus DR_TPAGE
 * kaze_registration_confirm_tpage_prims.
 */

void kaze_registration_confirm_sprites_init(void)
{
    extern void SetDrawTPage(void *, int, int, int);
    extern void SetShadeTex(void *, int);
    extern void SetSprt(void *);
    extern int GetTPage(int, int, int, int);
    extern int GetClut(int, int);
    register int page __asm__("$23");
    char *root;
    register int group_ofs __asm__("$22");
    register int row __asm__("$21");
    register int u __asm__("$20");
    register int y __asm__("$19");
    register int ofs __asm__("$17");
    register char *base __asm__("$18");
    register char *p __asm__("$16");
    register char *q __asm__("$3");
    register char *draw_base __asm__("$4");
    register int clut_x __asm__("$4");
    register int clut_y __asm__("$5");
    int t;

    page = 0;
    root = kaze_registration_confirm_label_sprites;
    group_ofs = 0;
    do {
        row = 0;
        u = 0x50;
        y = -0x1C;
        ofs = group_ofs;
        base = root;
        do {
            p = (char *)((int)group_ofs + (int)base);
            SetSprt(p);
            SetShadeTex(p, 1);
            clut_x = 0x340;
            clut_y = 0xF2;
            __asm__ volatile("" ::: "memory");
            q = (char *)((int)ofs + (int)root);
            *(short *)(q + 0x08) = -0x90;
            *(short *)(q + 0x10) = 0x80;
            *(short *)(q + 0x0A) = y;
            *(short *)(q + 0x12) = 0x10;
            q[0x0C] = 0;
            q[0x0D] = u;
            t = GetClut(clut_x, clut_y);
            u += 0x10;
            y += 0x18;
            *(short *)(kaze_registration_confirm_label_sprites + ofs + 0x0E) = t;
            ofs += 0x14;
            row++;
            base += 0x14;
        } while (row < 2);
        t = GetTPage(0, 0, 0x340, 0x100);
        q = (char *)(page * 8);
        draw_base = kaze_registration_confirm_tpage_prims;
        draw_base = (int)q + draw_base;
        SetDrawTPage(draw_base, 0, 1, t & 0xFFFF);
        page++;
        group_ofs += 0x28;
    } while (page < 2);
}

/* NEAR-MISS 17/145 (was 28). Found via cc1 -dS asm-diff triage.
 * KEY LEVER (cluster1, closed 11 words 28->17): the callee-saved prologue
 * register-saves for the pinned vars (s4-s8) are scheduled as RTL and each
 * pairs with its zero-init move; their emit order follows the INIT-STATEMENT
 * source order (LUID). Reordering the inits to col=0; base=kaze_balloon_editor_label_sprites;
 * tile_offset=0; preview_offset=0; label_row_off=0 reproduces the target save/init interleave
 * (s6,s8,s7,s5,s4). The natural base-first source order bunches them wrong.
 * REMAINING 17 (inner-loop scheduler dead-ends): (cluster2 ~5w @0x74) the
 * GetClut(0x380,0xf2) li a0/a1 arg leaves float up before the sra and
 * `move a2,s1` sorts before `addu a3` -- priority-1 leaf float, same class as
 * the move-a2,zero dead-end; moving grp before q or the GetClut call earlier
 * both regress (q=$a3 must die into its stores before the call).
 * (cluster3 ~12w @0xa0) the 6-store body (grp*0xe0, sub_idx*0x18, grp*0x40,
 * sub_idx*0x10 ...) is scheduled with the value-computations interleaved and the
 * grp*4 temp in v1 not v0; every store/compute reordering tried regresses. */
extern u8 kaze_balloon_editor_label_sprites[];
extern u8 kaze_balloon_editor_label_tpage_prims[];
extern u8 kaze_balloon_editor_preview_sprite[];
extern u8 kaze_balloon_editor_preview_tpage_prims[];
extern u8 kaze_balloon_editor_swatch_tiles[];
extern void SetSemiTrans(void *, int);

/**
 * kaze_balloon_editor_panel_sprites_init  (menu, 0x80059ac4)
 *
 * Builds the editor side panel: 8 semi-trans label SPRTs at
 * kaze_balloon_editor_label_sprites (2 columns), current-color preview SPRT
 * kaze_balloon_editor_preview_sprite (CLUT 0x390,0xf2), and color swatch
 * TILE_16 kaze_balloon_editor_swatch_tiles.
 */

void kaze_balloon_editor_panel_sprites_init(void)
{
    extern void SetDrawTPage(void *, int, int, unsigned int);
    extern u16 GetTPage(int, int, int, int);
    extern u16 GetClut(int, int);
    extern void SetShadeTex(void *, int);
    extern void SetSprt(void *);
    register int col asm("$22");
    register int preview_offset asm("$21");
    register int label_row_off asm("$20");
    register int tile_offset asm("$23");
    register int row asm("$17");
    register int label_offset asm("$18");
    register int grp asm("$6");
        int sub_idx;
    register u8 *label_ptr asm("$19");
    register u8 *sprt_ptr asm("$16");
    register u8 *label_dst asm("$7");
    u8 *base;
    u16 clut;

    col = 0;
    base = kaze_balloon_editor_label_sprites;
    tile_offset = 0;
    preview_offset = 0;
    label_row_off = 0;
    do {
        row = 0;
        label_offset = label_row_off;
        label_ptr = base;
        do {
            sprt_ptr = (u8 *)(label_row_off + (int)label_ptr);
            SetSprt(sprt_ptr);
            SetShadeTex(sprt_ptr, 1);
            SetSemiTrans(sprt_ptr, 1);
grp = row;
            label_dst = (u8 *)(label_offset + (int)base);
            if (row < 0) {
                grp = row + 3;
            }
            grp = grp >> 2;
            { register int t4 asm("$3") = grp * 4; sub_idx = row - t4; }
*(short *)(label_dst + 8) = grp * 0xe0 - 0x90;
            *(short *)(label_dst + 0xa) = sub_idx * 0x18 + 8;
            *(u8 *)(label_dst + 0xc) = grp * 0x40 + 0x40;
            *(u8 *)(label_dst + 0xd) = sub_idx * 0x10 - 0x60;
            *(short *)(label_dst + 0x10) = 0x40;
            *(short *)(label_dst + 0x12) = 0x10;
            clut = GetClut(0x380, 0xf2);
            *(u16 *)(kaze_balloon_editor_label_clut + label_offset) = clut;
            label_offset += 0x14;
            row += 1;
            label_ptr += 0x14;
        } while (row < 8);
        clut = GetTPage(0, 0, 0x3c0, 0);
        SetDrawTPage(kaze_balloon_editor_label_tpage_prims + col * 8, 0, 1, (unsigned int)clut);
        sprt_ptr = kaze_balloon_editor_preview_sprite + preview_offset;
        SetSprt(sprt_ptr);
        SetShadeTex(sprt_ptr, 1);
        *(short *)(sprt_ptr + 8) = 0x60;
        *(short *)(sprt_ptr + 0xa) = -0x58;
        *(short *)(sprt_ptr + 0x10) = 0x20;
        *(short *)(sprt_ptr + 0x12) = 0x20;
        *(u8 *)(sprt_ptr + 0xc) = 0xc0;
        *(u8 *)(sprt_ptr + 0xd) = 0x80;
        clut = GetClut(0x390, 0xf2);
        *(u16 *)(kaze_balloon_editor_preview_sprite + preview_offset + 0xe) = clut;
        clut = GetTPage(0, 0, 0x340, 0x100);
        SetDrawTPage(kaze_balloon_editor_preview_tpage_prims + col * 8, 0, 1, (unsigned int)clut);
        sprt_ptr = kaze_balloon_editor_swatch_tiles + tile_offset;
        SetTile16(sprt_ptr);
        tile_offset += 0xc;
        preview_offset += 0x14;
        col += 1;
        *(short *)(sprt_ptr + 8) = 0x68;
        *(short *)(sprt_ptr + 0xa) = -0x50;
        label_row_off += 0xa0;
    } while (col < 2);
}
