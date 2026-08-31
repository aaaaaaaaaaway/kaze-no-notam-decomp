/**
 * kaze_memcard_state_dispatch  (menu, 0x80060278)
 *
 * Memcard screen per-state update+draw dispatcher on
 * kaze_memory_card_prompt_latch: 0 intro scroll, 1 save/load menu, 10 slot
 * select, 0xb format flow, 0xc card recheck, 0x14 load flow, 0x1e save flow,
 * each paired with its draw function.
 */


#include "kaze_syms.h"
extern unsigned int kaze_memory_card_prompt_latch;

void kaze_memcard_state_dispatch(int arg)
{
    switch (kaze_memory_card_prompt_latch) {
    case 0:
        kaze_memcard_intro_scroll(arg);
        kaze_memcard_menu_draw(arg);
        break;
    case 1:
        kaze_memcard_menu_select();
        kaze_memcard_menu_draw(arg);
        break;
    case 10:
        kaze_memcard_slot_select();
        kaze_memcard_slot_select_draw(arg);
        break;
    case 11:
        kaze_memcard_format_flow();
        kaze_memcard_format_draw(arg);
        break;
    case 12:
        kaze_memcard_card_recheck();
        kaze_slot_select_overlay_draw(arg);
        break;
    case 20:
        kaze_memcard_load_flow();
        kaze_memcard_load_draw(arg);
        break;
    case 30:
        kaze_memcard_save_flow();
        kaze_memcard_save_draw(arg);
        break;
    }
}
