/**
 * kaze_memcard_save_flow  (menu, 0x80060b80)
 *
 * Save flow (state 0x1e, phases kaze_memory_card_save_state): checks free
 * blocks kaze_memcard_free_blocks and existing BISLPS-00912 file (overwrite
 * confirm), builds the save image via kaze_save_image_pack, writes with
 * kaze_memcard_file_write; success/no-space/write-failed phases handle
 * retry/exit.
 */


#include "kaze_syms.h"
extern char kaze_save_file_name[];
extern unsigned char kaze_memcard_save_overwrite_flag;
extern int kaze_memcard_file_count(int);
extern unsigned char kaze_memory_card_save_state;
/* Case 0/3 (`state++`) and case 8 (`state = 4`) both end in `sb v0,
 * kaze_memory_card_save_state` immediately before the shared sfx(2) at b08.  GCC 2.7.x's
 * cross-jump pass tail-merges the two identical stores into a single copy at
 * b08, dropping the function 12 bytes short and deferring the store.  The
 * target keeps both stores.  Writing case 8's store through a volatile-typed
 * alias makes it a distinct (volatile) RTL insn cross-jump refuses to merge,
 * while still emitting the same `sb`. */
extern volatile unsigned char D_800AE084_v __asm__("kaze_memory_card_save_state");
extern int kaze_memcard_slot_index;
extern int kaze_memcard_work_buffer;
extern int kaze_memory_card_prompt_latch;
extern unsigned int kaze_input_edge;

extern void kaze_sfx_play_dispatch(int);

void kaze_memcard_save_flow(void)
{
    int file_count;
    unsigned int edge_bits;
    int sfx_id;

    switch (kaze_memory_card_save_state) {
    case 0:
    case 3:
        if ((kaze_input_edge & 0x20) != 0) {
            kaze_memory_card_save_state = kaze_memory_card_save_state + 1;
            kaze_sfx_play_dispatch(2);
        }
        goto b10;
    case 1:
        kaze_memory_card_save_state = kaze_memory_card_save_state + 1;
        break;
    case 2:
        if (kaze_memcard_probe_blocking(kaze_memcard_slot_index) == -1) {
            goto b04;
        }
        file_count = kaze_memcard_file_count(kaze_memcard_slot_index);
        if (kaze_memcard_free_blocks(file_count) > 0) {
            if (kaze_memcard_find_file(file_count, kaze_save_file_name) == 0) {
                kaze_memory_card_save_state = 4;
                kaze_memcard_save_overwrite_flag = 1;
                return;
            }
        } else {
            if (kaze_memcard_find_file(file_count, kaze_save_file_name) == 0) {
                goto b07;
            }
        }
        kaze_memory_card_save_state = 3;
        kaze_memcard_save_overwrite_flag = 0;
        return;
    b07:
        kaze_memory_card_save_state = 7;
        return;
    case 4:
        kaze_save_image_pack();
        kaze_memory_card_save_state = 5;
        break;
    case 5:
        if (kaze_memcard_probe_blocking(kaze_memcard_slot_index) == -1) {
        b04:
            kaze_memory_card_save_state = 9;
        } else {
            file_count = kaze_memcard_file_count(kaze_memcard_slot_index);
            if (kaze_memcard_find_file(file_count, kaze_save_file_name) != 0) {
                kaze_memcard_save_overwrite_flag = 0;
            } else {
                kaze_memcard_save_overwrite_flag = 1;
            }
            if (kaze_memcard_file_write(kaze_memcard_slot_index, kaze_save_file_name,
                                        (void *)kaze_memcard_work_buffer, 1, kaze_memcard_save_overwrite_flag) == 0) {
                kaze_memory_card_save_state = 6;
            } else {
                kaze_memory_card_save_state = 8;
            }
            return;
        }
        break;
    case 6:
        if ((kaze_input_edge & 0x60) == 0) {
            return;
        }
        kaze_memory_card_prompt_latch = 1;
        sfx_id = 0x88;
        goto b34;
    case 7:
    case 9:
        edge_bits = kaze_input_edge & 0x60;
        goto b20;
    case 8:
        if ((kaze_input_edge & 0x20) != 0) {
            D_800AE084_v = 4;
            kaze_sfx_play_dispatch(2);
        } else {
            goto b10;
        }
    b10:
        edge_bits = kaze_input_edge & 0x40;
    b20:
        if (edge_bits != 0) {
            kaze_memory_card_prompt_latch = 1;
            sfx_id = 0x65;
        b34:
            kaze_sfx_play_dispatch(sfx_id);
        }
    }
}
