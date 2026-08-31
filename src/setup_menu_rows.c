#include "kaze_syms.h"
extern int kaze_menu_screen_mode;


extern int kaze_input_edge;
extern int kaze_title_menu_state;
extern void kaze_sfx_play_dispatch(int);

/**
 * kaze_setup_menu_confirm_input  (menu, 0x80052da8)
 *
 * Setup menu confirm state (6): Circle exits and starts the game
 * (kaze_title_menu_state=99, kaze_menu_screen_mode=9 so
 * kaze_settings_menu_dispatch commits settings); Cross returns to option row
 * state 3.
 */

void kaze_setup_menu_confirm_input(void)
{
    if ((kaze_input_edge & 0x20) != 0) {
        kaze_title_menu_state = 99;
        kaze_menu_screen_mode = 9;
        kaze_sfx_play_dispatch(2);
    }
    if ((kaze_input_edge & 0x40) != 0) {
        kaze_title_menu_state = 3;
        kaze_sfx_play_dispatch(0x65);
    }
    return;
}

extern int kaze_menu_mode_source;
extern unsigned int kaze_setup_menu_mode_texture_latch;
extern int kaze_menu_mode_preview_tim_table[];
extern void kaze_tim_load_to_vram(int, int, int, int, int);

/**
 * kaze_setup_menu_mode_texture_upload  (menu, 0x80052e18)
 *
 * Uploads the preview texture for the selected mode: kaze_tim_load_to_vram
 * on (&kaze_menu_mode_preview_tim_table)[kaze_menu_mode_source], guarded by
 * the change-latch bitmask kaze_setup_menu_mode_texture_latch.
 */

void kaze_setup_menu_mode_texture_upload(void)
{
    unsigned int mask;
    int pad[2];

    mask = 1 << kaze_menu_mode_source;
    if ((kaze_setup_menu_mode_texture_latch & mask) != mask) {
        kaze_tim_load_to_vram(kaze_menu_mode_preview_tim_table[kaze_menu_mode_source], -1, -1, -1, -1);
        kaze_setup_menu_mode_texture_latch = mask;
    }
    return;
}

extern int kaze_menu_stage_source;
extern void kaze_linef3_gradient_bar(int, int, int, int, int);
extern void kaze_drmove_addprim_a0ec(int, int);

/**
 * kaze_setup_menu_stage_row_draw  (menu, 0x80052e88)
 *
 * Stage-row draw pass: highlight box at kaze_menu_stage_source*0x50+0x33,
 * shared setup panels (kaze_title_cursor_frame_draw_3e00,
 * kaze_title_panel_prims_add_3edc(0), kaze_title_rows3_highlight_draw), then
 * the menu background.
 */

void kaze_setup_menu_stage_row_draw(int arg)
{
    kaze_linef3_gradient_bar(arg, kaze_menu_stage_source * 0x50 + 0x33, 0x4F, 0x3A, 0x72);
    kaze_title_cursor_frame_draw_3e00(arg, 0, 0);
    kaze_title_panel_prims_add_3edc(arg, 0);
    kaze_title_rows3_highlight_draw(arg);
    kaze_drmove_addprim_a0ec(arg, 0xFFF);
}

/**
 * kaze_setup_menu_mode_row_draw  (menu, 0x80052f08)
 *
 * Mode-row draw pass: shared panels, mode widgets
 * kaze_settings_mode_rows_draw, highlight box at
 * kaze_menu_mode_source*0x50+0x33, then the menu background.
 */

void kaze_setup_menu_mode_row_draw(int arg)
{
    kaze_title_cursor_frame_draw_3e00(arg, 0, 0);
    kaze_title_panel_prims_add_3edc(arg, 1);
    kaze_settings_mode_rows_draw(arg);
    kaze_linef3_gradient_bar(arg, kaze_menu_mode_source * 0x50 + 0x33, 0x4F, 0x3A, 0x72);
    kaze_drmove_addprim_a0ec(arg, 0xFFF);
}
