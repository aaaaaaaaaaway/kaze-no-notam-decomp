/**
 * kaze_memcard_slot_select  (menu, 0x80060524)
 *
 * Card-slot select state: watches slot status
 * kaze_memcard_slot_status_flags/7c, Left/Right pick slot
 * kaze_memcard_slot_index; Circle routes to format prompt (0xb) on
 * unformatted flag 2, recheck (0xc) on flag 0x100, else load (0x14) or save
 * (0x1e) per row kaze_memcard_menu_row; Cross returns to menu.
 */


#include "kaze_syms.h"
extern int kaze_memcard_menu_row;
extern int kaze_memcard_slot_poll_counter;
extern int kaze_memory_card_prompt_latch;
extern int kaze_memcard_slot_index;
extern char kaze_memcard_format_phase;
extern char kaze_memory_card_load_state;
extern char kaze_memory_card_save_state;
extern int kaze_memcard_status_retry_count;
extern unsigned int kaze_memcard_slot_status_flags[];
extern unsigned int kaze_memcard_slot1_status_flags;
extern unsigned int kaze_input_edge;
extern void kaze_sfx_play_dispatch(int);

void kaze_memcard_slot_select(void)
{
    unsigned int flags;
    int arg;

    if ((kaze_memcard_slot_status_flags[0] & 0x40000) != 0) {
        return;
    }
    if ((kaze_memcard_slot1_status_flags & 0x40000) != 0) {
        return;
    }
    if ((kaze_memcard_slot_status_flags[kaze_memcard_slot_index] & 4) != 0) {
        kaze_memcard_slot_index = (kaze_memcard_slot_index == 0);
    }
    if (((kaze_memcard_slot_status_flags[0] & 0x103) != 0) && ((kaze_memcard_slot1_status_flags & 0x103) != 0)) {
        if ((kaze_input_edge & 0x8000) != 0) {
            kaze_memcard_slot_index = 0;
            kaze_sfx_play_dispatch(1);
        }
        if ((kaze_input_edge & 0x2000) != 0) {
            kaze_memcard_slot_index = 1;
            kaze_sfx_play_dispatch(1);
        }
    }
    if (kaze_memcard_slot_poll_counter < 0x10) {
        kaze_memcard_slot_poll_counter = kaze_memcard_slot_poll_counter + 1;
        return;
    }
    if ((kaze_input_edge & 0x20) != 0) {
        flags = kaze_memcard_slot_status_flags[kaze_memcard_slot_index];
        if ((flags & 0x103) != 0) {
            if ((flags & 0x100) != 0) {
                kaze_memory_card_prompt_latch = 0xc;
            } else if ((flags & 2) != 0) {
                kaze_memory_card_prompt_latch = 0xb;
                kaze_memcard_status_retry_count = 0;
                kaze_memcard_format_phase = 0;
                arg = 0x88;
                goto play;
            } else if (kaze_memcard_menu_row != 0) {
                kaze_memory_card_prompt_latch = 0x1e;
                kaze_memory_card_save_state = 0;
            } else {
                kaze_memory_card_prompt_latch = 0x14;
                kaze_memory_card_load_state = 0;
            }
            kaze_memcard_status_retry_count = 0;
            arg = 0x88;
            goto play;
        }
    }
    if ((kaze_input_edge & 0x40) == 0) {
        return;
    }
    kaze_memory_card_prompt_latch = 1;
    arg = 0x65;
play:
    kaze_sfx_play_dispatch(arg);
}
