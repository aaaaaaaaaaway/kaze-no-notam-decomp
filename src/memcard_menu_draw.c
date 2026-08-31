#include "kaze_syms.h"
extern int kaze_memcard_menu_row;


extern int kaze_memory_card_prompt_latch;
extern int kaze_memcard_slot_index;
extern char kaze_memcard_format_phase;
extern char kaze_memory_card_load_state;
extern char kaze_memory_card_save_state;
extern volatile int kaze_memcard_status_retry_count;
extern int D_800AE08C_NV __asm__("kaze_memcard_status_retry_count");
extern unsigned int kaze_input_edge;
extern void kaze_sfx_play_dispatch(int);

/**
 * kaze_memcard_card_recheck  (menu, 0x80060e50)
 *
 * Card recheck state (0xc): when slot status kaze_memcard_slot_status_flags
 * clears, routes to format/load/save; while flag 0x100 persists re-arms
 * status requests in kaze_memcard_slot_request_state up to 5 tries
 * (kaze_memcard_status_retry_count), then allows Circle/Cross to return to
 * the menu.
 */

void kaze_memcard_card_recheck(void)
{
    extern unsigned int kaze_memcard_slot_request_state[];
    extern unsigned int kaze_memcard_slot_status_flags[];
    unsigned int flags;
    unsigned int new_flags;
    unsigned int *slot;
    int retry;
    register int idx asm("$2");
    register int offset asm("$4");

    idx = kaze_memcard_slot_index;
    offset = idx << 2;
    flags = *(unsigned int *)((char *)kaze_memcard_slot_status_flags + offset);
    if ((flags & 0x100) != 0) {
        retry = D_800AE08C_NV;
        if (retry >= 5) {
            asm volatile("" : : : "memory");
            if ((kaze_input_edge & 0x60) != 0) {
                kaze_memory_card_prompt_latch = 1;
                kaze_sfx_play_dispatch(0x88);
            }
        } else {
            slot = (unsigned int *)((char *)kaze_memcard_slot_request_state + offset);
            if ((*slot & 4) == 0) {
                retry++;
                asm volatile("" : : "r"(retry));
                new_flags = *(volatile unsigned int *)slot;
                D_800AE08C_NV = retry;
                *slot = new_flags | 0x11;
            }
        }
    } else {
        if ((flags & 2) != 0) {
            kaze_memory_card_prompt_latch = 0xB;
            kaze_memcard_status_retry_count = 0;
            kaze_memcard_format_phase = 0;
            kaze_memcard_status_retry_count = 0;
        } else if (kaze_memcard_menu_row != 0) {
            kaze_memory_card_prompt_latch = 0x1E;
            kaze_memory_card_save_state = 0;
            kaze_memcard_status_retry_count = 0;
            kaze_memcard_status_retry_count = 0;
        } else {
            kaze_memory_card_prompt_latch = 0x14;
            kaze_memory_card_load_state = 0;
            kaze_memcard_status_retry_count = 0;
            kaze_memcard_status_retry_count = 0;
        }
    }
}

extern void kaze_linef3_gradient_bar(int, int, int, int, int);

/**
 * kaze_memcard_menu_draw  (menu, 0x80060f88)
 *
 * Draw for intro/menu states: cursor box via kaze_linef3_gradient_bar at row
 * kaze_memcard_menu_row (y=row*0x20+0x5f), panel prims
 * kaze_slot_overlay_add_header_prims/kaze_slot_overlay_add_glyph_prims,
 * finish kaze_drmove_addprim_a0ec.
 */

void kaze_memcard_menu_draw(int arg0)
{
    extern void kaze_drmove_addprim_a0ec(int, int);
    extern void kaze_slot_overlay_add_header_prims(int);
    kaze_linef3_gradient_bar(arg0, 0x67, kaze_memcard_menu_row * 0x20 + 0x5f, 0x72, 0x12);
    kaze_slot_overlay_add_header_prims(arg0);
    kaze_slot_overlay_add_glyph_prims(arg0);
    kaze_drmove_addprim_a0ec(arg0, 0xfff);
}

extern void kaze_slot_overlay_add_rows(int arg0);
extern void kaze_slot_overlay_add_status_strip(int arg0, int arg1, int arg2);
extern int kaze_memcard_slot1_request_state;
extern int kaze_memcard_slot1_status_flags;

/**
 * kaze_memcard_slot_select_draw  (menu, 0x80060fec)
 *
 * Slot-select draw: panels
 * kaze_slot_overlay_add_header_prims/kaze_slot_overlay_add_rows, then picks
 * a message id from card status words kaze_memcard_slot_request_state/154c
 * and kaze_memcard_slot_status_flags/7c (checking 8, no card 9, select
 * prompt 0/1) rendered via kaze_slot_overlay_add_status_strip.
 */

void kaze_memcard_slot_select_draw(int page)
{
    extern int kaze_memcard_slot_status_flags;
    extern int kaze_memcard_slot_request_state;
    extern void kaze_drmove_addprim_a0ec(int arg0, int arg1);
    extern void kaze_slot_overlay_add_header_prims(void);
    kaze_slot_overlay_add_header_prims();
    kaze_slot_overlay_add_rows(page);
    if (((kaze_memcard_slot_request_state & 4) != 0) || ((kaze_memcard_slot1_request_state & 4) != 0)) {
        kaze_slot_overlay_add_status_strip(page, 8, 0);
    } else if (((kaze_memcard_slot_request_state & 0x10) != 0) || ((kaze_memcard_slot1_request_state & 0x10) != 0)) {
        kaze_slot_overlay_add_status_strip(page, 8, 0);
    } else if (((kaze_memcard_slot_status_flags & 4) != 0) && ((kaze_memcard_slot1_status_flags & 4) != 0)) {
        kaze_slot_overlay_add_status_strip(page, 0, 1);
    } else {
        kaze_slot_overlay_add_status_strip(page, 9, 0);
    }
    kaze_drmove_addprim_a0ec(page, 0xFFF);
}
