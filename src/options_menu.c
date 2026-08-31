#include "kaze_syms.h"
extern int kaze_options_state;
extern int kaze_options_fade_counter;
extern int kaze_options_fade_mode;
extern int kaze_sound_mode_cursor;
extern int kaze_options_fade_reverse_flag;
extern int kaze_menu_screen_mode;


extern int kaze_input_edge;

/**
 * kaze_options_fade_step  (menu, 0x80055d20)
 *
 * Advances fade counter kaze_options_fade_counter to 0x2a via
 * kaze_drmove_wipe_tiles (mode 3 while kaze_options_fade_reverse_flag set),
 * input skips; clears kaze_options_fade_mode and returns 1 when done.
 */

int kaze_options_fade_step(int arg)
{
    int ret;

    if (kaze_options_fade_reverse_flag != 0) {
        kaze_drmove_wipe_tiles(arg, kaze_options_fade_counter, 3);
    } else {
        kaze_drmove_wipe_tiles(arg, kaze_options_fade_counter, 0);
    }
    if (kaze_options_fade_counter > 0x29 || kaze_input_edge != 0) {
        kaze_options_fade_mode = 0;
        ret = 1;
    } else {
        kaze_options_fade_counter++;
        ret = 0;
    }
    return ret;
}

extern int kaze_sound_mode_setting;
extern int kaze_options_top_menu_row;
extern void kaze_sfx_play_dispatch(int);

/**
 * kaze_options_top_menu_input  (menu, 0x80055db0)
 *
 * Up/down over 4 rows kaze_options_top_menu_row; accept routes row 0 to
 * state 10 (kaze_records_screen_init sub-screen), 1 to registration init
 * kaze_registration_screen_init, 2 to button config, 3 to sound mode; cancel
 * exits with kaze_menu_screen_mode=0.
 */

void kaze_options_top_menu_input(void)
{
    int row;
    int mode;

    if (((kaze_input_edge & 0x4000) != 0) && (kaze_options_top_menu_row < 3)) {
        kaze_options_top_menu_row = kaze_options_top_menu_row + 1;
        kaze_sfx_play_dispatch(1);
    }
    if (((kaze_input_edge & 0x1000) != 0) && (kaze_options_top_menu_row > 0)) {
        kaze_options_top_menu_row = kaze_options_top_menu_row - 1;
        kaze_sfx_play_dispatch(1);
    }
    if ((kaze_input_edge & 0x20) != 0) {
        row = kaze_options_top_menu_row;
        if (row == 1) {
            goto accept_registration;
        }
        if (row < 2) {
            if (row == 0) {
                goto accept_records;
            }
            goto accept_common_2;
        }
        if (row == 2) {
            goto accept_buttons;
        }
        if (row == 3) {
            goto accept_sound;
        }
        goto accept_common_2;

accept_records:
        kaze_options_state = 10;
        kaze_records_screen_init();
        goto accept_common_2;

accept_registration:
        kaze_options_state = 2;
        kaze_registration_screen_init();
        goto accept_common_2;

accept_buttons:
        kaze_options_state = 4;
        kaze_button_config_cursor_reset();
        goto accept_common_2;

accept_sound:
        kaze_options_state = 6;
        kaze_sound_mode_cursor = *(unsigned char *)&kaze_sound_mode_setting;

accept_common_2:
        mode = 2;
        kaze_options_fade_counter = 0;
        kaze_options_fade_mode = mode;
        kaze_backbuffer_copy_offscreen();
        kaze_sfx_play_dispatch(2);
    }
    if ((kaze_input_edge & 0x40) != 0) {
        kaze_options_state = 99;
        kaze_menu_screen_mode = 0;
        kaze_sfx_play_dispatch(0x65);
    }
}
