#include "kaze_syms.h"
extern int kaze_setup_menu_scroll_counter;
extern int kaze_setup_menu_bgm_track_id;
extern int D_800AB1D8;
extern unsigned char kaze_setup_row1_flash_flags;
extern unsigned char kaze_setup_row2_flash_flags;
extern int kaze_menu_screen_mode;
extern int kaze_menu_flags_source;
extern int kaze_menu_level_source;


extern int kaze_title_menu_state;
extern int kaze_menu_stage_source;
extern int kaze_menu_mode_source;
extern int kaze_setup_menu_mode_texture_latch;
extern int kaze_menu_weather_source;

/**
 * kaze_setup_menu_init  (menu, 0x800525e4)
 *
 * Setup menu init: state kaze_title_menu_state=10, BGM 0xd, row defaults
 * (kaze_menu_stage_source=1, kaze_menu_mode_source=1,
 * kaze_menu_level_source/kaze_menu_flags_source/kaze_menu_weather_source=0),
 * then runs the setup-screen primitive initializers.
 */

void kaze_setup_menu_init(void)
{
    kaze_title_menu_state = 10;
    kaze_setup_menu_bgm_track_id = 13;
    kaze_menu_stage_source = 1;
    kaze_menu_mode_source = 1;
    kaze_setup_menu_scroll_counter = 0;
    kaze_setup_menu_mode_texture_latch = 0;
    kaze_menu_flags_source = 0;
    kaze_menu_level_source = 0;
    kaze_menu_weather_source = 0;
    D_800AB1D8 = 2;
    kaze_backbuffer_copy_offscreen();
    kaze_setup_menu_header_prim_init();
    kaze_setup_menu_textline_prim_init();
    kaze_setup_menu_stage_panel_prim_init();
    kaze_setup_menu_mode_panel_prim_init();
    kaze_title_menu_row_sprites_init();
    kaze_title_icon_sprites_init();
    kaze_title_bg_sprites_init();
    kaze_linef3_init();
    kaze_task_menu_prim_init();
    kaze_task_menu_enter_init();
}

/**
 * kaze_setup_menu_intro_scroll  (menu, 0x800526b4)
 *
 * Setup menu intro state: draws the background at scroll
 * kaze_setup_menu_scroll_counter (0..0x2a); returns 1 on input or completion
 * so the loop advances to the memcard prompt state 0xb.
 */

int kaze_setup_menu_intro_scroll(int buf)
{
    extern int kaze_input_edge;
    int ret;

    kaze_drmove_wipe_tiles(buf, kaze_setup_menu_scroll_counter, 0);
    if ((kaze_setup_menu_scroll_counter >= 0x2A) || (kaze_input_edge != 0)) {
        D_800AB1D8 = 0;
        ret = 1;
    } else {
        kaze_setup_menu_scroll_counter = kaze_setup_menu_scroll_counter + 1;
        ret = 0;
    }
    return ret;
}


/**
 * kaze_setup_menu_memcard_prompt_state  (menu, 0x80052724)
 *
 * Setup menu memory-card prompt state: kaze_task_menu_state_machine result 1
 * enters the stage row (state 1); result 2 exits the setup menu
 * (kaze_title_menu_state=99) back to the main menu
 * (kaze_menu_screen_mode=0).
 */

void kaze_setup_menu_memcard_prompt_state(int arg0)
{
    switch (kaze_task_menu_state_machine(arg0, 0, 0)) {
    case 1:
        kaze_title_menu_state = 1;
        break;
    case 2:
        kaze_title_menu_state = 99;
        kaze_menu_screen_mode = 0;
        break;
    }
}

/**
 * kaze_setup_menu_stage_row_input  (menu, 0x80052788)
 *
 * Setup menu stage-row input: Left/Right cycle kaze_menu_stage_source 0..2
 * (committed to staging kaze_staged_stage_index by
 * kaze_settings_menu_dispatch); Circle advances to the mode row (state 2),
 * Cross back to the prompt state 0xb.
 */

void kaze_setup_menu_stage_row_input(void)
{
    extern int kaze_input_edge;
    extern void kaze_sfx_play_dispatch(int arg0);
    if (((kaze_input_edge & 0x2000) != 0) && (kaze_menu_stage_source < 2)) {
        kaze_menu_stage_source = kaze_menu_stage_source + 1;
        kaze_sfx_play_dispatch(1);
    }
    if (((kaze_input_edge & 0x8000) != 0) && (0 < kaze_menu_stage_source)) {
        kaze_menu_stage_source = kaze_menu_stage_source - 1;
        kaze_sfx_play_dispatch(1);
    }
    if ((kaze_input_edge & 0x20) != 0) {
        kaze_title_menu_state = 2;
        kaze_sfx_play_dispatch(0x88);
    }
    if ((kaze_input_edge & 0x40) != 0) {
        kaze_title_menu_state = 0xB;
        kaze_task_menu_enter_init();
        kaze_sfx_play_dispatch(0x65);
    }
}

/**
 * kaze_settings_mode_row_handler  (menu, 0x8005286c)
 *
 * Row-2 handler adjusting the mode source value kaze_menu_mode_source.
 */

void kaze_settings_mode_row_handler(void)
{
    extern void kaze_sfx_play_dispatch(int);
    extern unsigned int kaze_input_edge;
    int v;

    if (kaze_input_edge & 0x2000) {
        v = kaze_menu_mode_source;
        if (v < 2) {
            kaze_menu_mode_source = v + 1;
            kaze_sfx_play_dispatch(1);
        }
    }
    if (kaze_input_edge & 0x8000) {
        if (kaze_menu_mode_source > 0) {
            kaze_menu_mode_source = kaze_menu_mode_source - 1;
            kaze_sfx_play_dispatch(1);
        }
    }
    if (kaze_input_edge & 0x20) {
        kaze_title_menu_state = 3;
        kaze_sfx_play_dispatch(0x88);
    }
    if (kaze_input_edge & 0x40) {
        kaze_title_menu_state = 1;
        kaze_sfx_play_dispatch(0x65);
    }
    if (kaze_input_edge & 0x10) {
        kaze_title_menu_state = 0x14;
        kaze_sfx_play_dispatch(0x67);
        kaze_setup_menu_mode_texture_upload();
    }
}

/**
 * kaze_setup_menu_ack_input_2978  (menu, 0x80052978)
 *
 * Setup menu state-0x14 input: Circle or Cross returns to the mode row
 * (kaze_title_menu_state=2) with a cancel sound.
 */

void kaze_setup_menu_ack_input_2978(void)
{
    extern void kaze_sfx_play_dispatch(int);
    extern int kaze_input_edge;
    if ((kaze_input_edge & 0x60) != 0) {
        kaze_title_menu_state = 2;
        kaze_sfx_play_dispatch(0x65);
    }
}

extern unsigned char kaze_setup_row2_flash_level;
extern unsigned char kaze_setup_row1_flash_level;

/**
 * kaze_setup_menu_option_row_29b4  (menu, 0x800529b4)
 *
 * Setup menu option-row input (state 3): Left/Right cycle
 * kaze_menu_level_source 0..2 with arrow-flash bytes
 * kaze_setup_row1_flash_level/kaze_setup_row2_flash_level; Down to state 4,
 * Circle to confirm state 6, Cross back to mode row 2.
 */

void kaze_setup_menu_option_row_29b4(void)
{
    extern void kaze_sfx_play_dispatch(int);
    extern unsigned int kaze_input_edge;
    int v;

    if (kaze_input_edge & 0x2000) {
        v = kaze_menu_level_source;
        kaze_setup_row2_flash_level = 0xE0;
        if (v >= 2) {
            kaze_menu_level_source = 0;
        } else {
            kaze_menu_level_source = v + 1;
        }
        kaze_sfx_play_dispatch(1);
    }
    if (kaze_input_edge & 0x8000) {
        kaze_setup_row1_flash_level = 0xE0;
        v = kaze_menu_level_source;
        if (v > 0) {
            kaze_menu_level_source = v - 1;
        } else {
            kaze_menu_level_source = 2;
        }
        kaze_sfx_play_dispatch(1);
    }
    if (kaze_input_edge & 0x4000) {
        kaze_title_menu_state = 4;
        kaze_sfx_play_dispatch(1);
    }
    if (kaze_input_edge & 0x20) {
        kaze_title_menu_state = 6;
        kaze_sfx_play_dispatch(0x88);
    }
    if (kaze_input_edge & 0x40) {
        kaze_title_menu_state = 2;
        kaze_sfx_play_dispatch(0x65);
    }
}

/**
 * kaze_setup_menu_option_row_2ad8  (menu, 0x80052ad8)
 *
 * Setup menu option-row input (state 4): Left/Right cycle
 * kaze_menu_flags_source 0..3 (arrow flash
 * kaze_setup_row1_flash_flags/kaze_setup_row2_flash_flags); Up to state 3,
 * Down to BGM row 5, Circle to confirm 6, Cross back to mode row 2.
 */

void kaze_setup_menu_option_row_2ad8(void)
{
    extern void kaze_sfx_play_dispatch(int);
    extern unsigned int kaze_input_edge;
    int v;

    if (kaze_input_edge & 0x2000) {
        v = kaze_menu_flags_source;
        kaze_setup_row2_flash_flags = 0xE0;
        if (v >= 3) {
            kaze_menu_flags_source = 0;
        } else {
            kaze_menu_flags_source = v + 1;
        }
        kaze_sfx_play_dispatch(1);
    }
    if (kaze_input_edge & 0x8000) {
        kaze_setup_row1_flash_flags = 0xE0;
        v = kaze_menu_flags_source;
        if (v > 0) {
            kaze_menu_flags_source = v - 1;
        } else {
            kaze_menu_flags_source = 3;
        }
        kaze_sfx_play_dispatch(1);
    }
    if (kaze_input_edge & 0x4000) {
        kaze_title_menu_state = 5;
        kaze_sfx_play_dispatch(1);
    }
    if (kaze_input_edge & 0x1000) {
        kaze_title_menu_state = 3;
        kaze_sfx_play_dispatch(1);
    }
    if (kaze_input_edge & 0x20) {
        kaze_title_menu_state = 6;
        kaze_sfx_play_dispatch(0x88);
    }
    if (kaze_input_edge & 0x40) {
        kaze_title_menu_state = 2;
        kaze_sfx_play_dispatch(0x65);
    }
}
