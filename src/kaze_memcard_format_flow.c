/**
 * kaze_memcard_format_flow  (menu, 0x80060730)
 *
 * Format flow state machine (phases kaze_memcard_format_phase): Circle
 * confirm, card test kaze_memcard_probe_blocking then format
 * kaze_memcard_format with up to 5 retries
 * (kaze_memcard_status_retry_count); success proceeds to load/save state,
 * failure phases return to the menu.
 */


#include "kaze_syms.h"
extern int kaze_memcard_menu_row;
extern int kaze_memcard_format(int);
extern int kaze_memcard_probe_blocking(int);
extern int kaze_memory_card_prompt_latch;
extern int kaze_memcard_slot_index;
extern unsigned char kaze_memcard_format_phase;
extern char kaze_memory_card_load_state;
extern char kaze_memory_card_save_state;
extern int kaze_memcard_status_retry_count;
extern unsigned int kaze_input_edge;

extern void kaze_sfx_play_dispatch(int);

void kaze_memcard_format_flow(void)
{
    int result;
    int sfx_id;

    switch (kaze_memcard_format_phase) {
    case 0:
        if ((kaze_input_edge & 0x20) != 0) {
            kaze_memcard_format_phase = 1;
            kaze_sfx_play_dispatch(2);
        }
        if ((kaze_input_edge & 0x40) == 0) {
            return;
        }
        kaze_memory_card_prompt_latch = 1;
        sfx_id = 0x65;
        break;
    case 1:
    case 2:
        kaze_memcard_format_phase = kaze_memcard_format_phase + 1;
        return;
    case 3:
        result = kaze_memcard_probe_blocking(kaze_memcard_slot_index);
        if (result == 1) goto set6;
        result = kaze_memcard_format(kaze_memcard_slot_index);
        if (result == -1) goto set5;
        if (result >= 0) goto chk0;
        if (result == -2) goto retry;
        return;
    chk0:
        if (result != 0) {
            return;
        }
        kaze_memcard_format_phase = 4;
        return;
    set5:
        kaze_memcard_format_phase = 5;
        return;
    retry:
        if (kaze_memcard_status_retry_count < 5) goto increment;
    set6:
        kaze_memcard_format_phase = 6;
        return;
    increment:
        kaze_memcard_status_retry_count = kaze_memcard_status_retry_count + 1;
        return;
    case 4:
        if ((kaze_input_edge & 0x60) == 0) {
            return;
        }
        if (kaze_memcard_menu_row == 0) goto load_state;
        kaze_memory_card_prompt_latch = 0x1E;
        kaze_memory_card_save_state = 0;
        goto after4;
    load_state:
        kaze_memory_card_prompt_latch = 0x14;
        kaze_memory_card_load_state = 0;
    after4:
        kaze_memcard_status_retry_count = 0;
        sfx_id = 0x88;
        break;
    case 5:
    case 6:
        if ((kaze_input_edge & 0x60) == 0) {
            return;
        }
        kaze_memory_card_prompt_latch = 1;
        sfx_id = 0x88;
        break;
    default:
        return;
    }
    kaze_sfx_play_dispatch(sfx_id);
}
