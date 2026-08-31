#include "kaze_syms.h"
extern int kaze_memcard_active_slot_index;
extern int kaze_memcard_intro_scroll_timer;
extern int kaze_memcard_menu_row;
extern int kaze_memcard_slot_poll_counter;
extern int kaze_menu_screen_mode;


extern int kaze_memory_card_prompt_latch;
extern int kaze_input_edge;

/**
 * kaze_memcard_intro_scroll  (menu, 0x80060360)
 *
 * Intro scroll state: advances counter kaze_memcard_intro_scroll_timer to
 * 0x2a while drawing the sliding panel via kaze_drmove_wipe_tiles, then
 * enters menu state 1 (or immediately on any button).
 */

void kaze_memcard_intro_scroll(int arg0)
{
    kaze_drmove_wipe_tiles(arg0, kaze_memcard_intro_scroll_timer, 1);
    if ((kaze_memcard_intro_scroll_timer >= 0x2a) || (kaze_input_edge != 0)) {
        kaze_memory_card_prompt_latch = 1;
    } else {
        kaze_memcard_intro_scroll_timer = kaze_memcard_intro_scroll_timer + 1;
    }
}

extern int kaze_memcard_slot_index;
extern void kaze_sfx_play_dispatch(int);

/**
 * kaze_memcard_menu_select  (menu, 0x800603d0)
 *
 * Save/load row select: Up/Down toggles row kaze_memcard_menu_row (snd 1);
 * Circle enters slot select (state 10, resets
 * kaze_memcard_slot_poll_counter/kaze_memcard_active_slot_index, card status
 * reset kaze_memcard_slot_status_reset, snd 0x88); Cross exits (state 99,
 * clears kaze_menu_screen_mode, snd 0x65).
 */

void kaze_memcard_menu_select(void)
{
    int sfx;

    if (((kaze_input_edge & 0x4000) != 0) && (kaze_memcard_menu_row < 1)) {
        kaze_memcard_menu_row = kaze_memcard_menu_row + 1;
        kaze_sfx_play_dispatch(1);
    }
    if (((kaze_input_edge & 0x1000) != 0) && (0 < kaze_memcard_menu_row)) {
        kaze_memcard_menu_row = kaze_memcard_menu_row - 1;
        kaze_sfx_play_dispatch(1);
    }
    if ((kaze_input_edge & 0x20) != 0) {
        kaze_memory_card_prompt_latch = 10;
        kaze_memcard_slot_poll_counter = 0;
        kaze_memcard_active_slot_index = 0;
        kaze_memcard_slot_index = 0;
        kaze_memcard_slot_status_reset();
        sfx = 0x88;
    } else {
        sfx = 0x65;
        if ((kaze_input_edge & 0x40) == 0) {
            return;
        }
        kaze_memory_card_prompt_latch = 0x63;
        kaze_menu_screen_mode = 0;
    }
    kaze_sfx_play_dispatch(sfx);
    return;
}
