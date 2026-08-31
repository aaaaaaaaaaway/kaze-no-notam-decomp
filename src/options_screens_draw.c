#include "types.h"
#include "kaze_syms.h"
extern int kaze_options_state;
extern int kaze_options_fade_counter;
extern int kaze_button_config_action_row;
extern int kaze_button_config_toggle_row;
extern int kaze_options_fade_mode;
extern int kaze_sound_mode_cursor;

extern int kaze_input_edge;
extern unsigned char kaze_sound_mode_setting;
extern void kaze_xa_stream_enable_33b0();
extern void kaze_xa_stream_disable_33e8();
extern void kaze_sfx_play_dispatch();

/**
 * kaze_sound_mode_input  (menu, 0x80056530)
 *
 * Stereo/mono cursor kaze_sound_mode_cursor with immediate preview via
 * kaze_xa_stream_enable_33b0/kaze_xa_stream_disable_33e8; accept (0x20)
 * stores kaze_sound_mode_setting, cancel (0x40) restores the saved mode.
 */

void kaze_sound_mode_input(void)
{
    if (((kaze_input_edge & 0x1000) != 0) && (kaze_sound_mode_cursor == 1)) {
        kaze_sound_mode_cursor = 0;
        kaze_xa_stream_enable_33b0();
        kaze_sfx_play_dispatch(1);
    }

    if (((kaze_input_edge & 0x4000) != 0) && (kaze_sound_mode_cursor == 0)) {
        kaze_sound_mode_cursor = 1;
        kaze_xa_stream_disable_33e8();
        kaze_sfx_play_dispatch(1);
    }

    if ((kaze_input_edge & 0x60) != 0) {
        kaze_options_state = 0;
        kaze_options_fade_counter = 0;
        kaze_options_fade_mode = 2;
        kaze_backbuffer_copy_offscreen();

        if ((kaze_input_edge & 0x20) == 0) {
            goto cancel_sound_mode;
        }
        kaze_sound_mode_setting = kaze_sound_mode_cursor;
        kaze_sfx_play_dispatch(2);
        return;

cancel_sound_mode:
        if ((kaze_input_edge & 0x40) == 0) {
            return;
        }
        if (kaze_sound_mode_setting == 0) {
            goto enable_stream;
        }
        if (kaze_sound_mode_setting == 1) {
            goto disable_stream;
        }
        goto cancel_dispatch;

enable_stream:
        kaze_xa_stream_enable_33b0();
        goto cancel_dispatch;

disable_stream:
        kaze_xa_stream_disable_33e8();

cancel_dispatch:
        kaze_sfx_play_dispatch(0x65);
    }
}

extern int kaze_options_top_menu_row;

/**
 * kaze_options_top_menu_draw  (menu, 0x80056684)
 *
 * Draws the top options menu: cursor ring at row kaze_options_top_menu_row
 * (0x18 pitch), header group 0 via kaze_options_header_prims_add, row labels
 * kaze_options_rows_highlight_draw, and fade overlay.
 */

void kaze_options_top_menu_draw(int page)
{
    extern void kaze_drmove_addprim_a0ec(int, int);
    extern void kaze_options_header_prims_add(int, int, int);
    extern void kaze_linef3_gradient_bar(int, int, int, int, int);
    kaze_linef3_gradient_bar(page, 0x67, kaze_options_top_menu_row * 0x18 + 0x5F, 0x72, 0x12);
    kaze_options_header_prims_add(page, 0, 1);
    kaze_options_rows_highlight_draw(page);
    kaze_drmove_addprim_a0ec(page, 0xFFF);
    return;
}

extern void kaze_button_config_prims_draw(int arg0);
extern s16 kaze_button_config_remap_armed_flag;
extern s16 kaze_button_config_column_flag;

/**
 * kaze_button_config_screen_draw  (menu, 0x800566f8)
 *
 * Draws the key-config screen: computes the cursor ring rect from
 * kaze_button_config_action_row/958/95c/960, header group 1, and all config
 * prims via kaze_button_config_prims_draw.
 */

void kaze_button_config_screen_draw(int page)
{
    extern void kaze_drmove_addprim_a0ec(int arg0, int arg1);
    extern void kaze_options_header_prims_add(int arg0, int arg1, int arg2);
    extern void kaze_linef3_gradient_bar(int arg0, int arg1, int arg2, int arg3, int arg4);
    if (kaze_button_config_remap_armed_flag != 0) {
        kaze_linef3_gradient_bar(page, 0x67, 0xBF, 0x72, 0x12);
    } else if (kaze_button_config_column_flag == 0) {
        kaze_linef3_gradient_bar(page, 0x47, kaze_button_config_action_row * 0x14 + 0x4F, 0x72, 0x12);
    } else {
        kaze_linef3_gradient_bar(page, 0xCF, kaze_button_config_toggle_row * 0x38 + 0x4F, 0x2A, 0x2A);
    }
    kaze_options_header_prims_add(page, 1, 1);
    kaze_button_config_prims_draw(page);
    kaze_drmove_addprim_a0ec(page, 0xFFF);
}

/**
 * kaze_sound_mode_screen_draw  (menu, 0x800567d4)
 *
 * Draws the sound-mode screen: cursor ring at kaze_sound_mode_cursor (0x20
 * pitch), header group 2, labels kaze_sound_mode_prims_add, fade overlay.
 */

void kaze_sound_mode_screen_draw(int arg0)
{
    extern void kaze_drmove_addprim_a0ec(int, int);
    extern void kaze_options_header_prims_add(int, int, int);
    extern void kaze_linef3_gradient_bar(int, int, int, int, int);
    kaze_linef3_gradient_bar(arg0, 0x67, kaze_sound_mode_cursor * 0x20 + 0x5f, 0x72, 0x12);
    kaze_options_header_prims_add(arg0, 2, 1);
    kaze_sound_mode_prims_add(arg0);
    kaze_drmove_addprim_a0ec(arg0, 0xfff);
}
