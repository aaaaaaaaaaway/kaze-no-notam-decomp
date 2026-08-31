/**
 * kaze_memcard_load_flow  (menu, 0x80060934)
 *
 * Load flow (state 0x14, phases kaze_memory_card_load_state): confirm, card
 * check kaze_memcard_probe_blocking, locate file BISLPS-00912
 * (kaze_memcard_file_count/bb58), read via kaze_memcard_file_read into
 * kaze_memcard_work_buffer and apply via kaze_save_verify_unpack_full with
 * up to 3 retries; no-file/error phases exit to menu.
 */


#include "kaze_syms.h"
extern char kaze_save_file_name[];
extern int kaze_memory_card_slot;
extern int kaze_memcard_file_count(int);
extern int kaze_memcard_file_read(int, char *, int, int);
extern int kaze_memcard_find_file(int, char *);
extern int kaze_memcard_probe_blocking(int);
extern int kaze_save_verify_unpack_full(void);
extern int kaze_memcard_work_buffer;
extern int kaze_memory_card_prompt_latch;
extern int kaze_memcard_slot_index;
extern unsigned char kaze_memory_card_load_state;
extern unsigned int kaze_input_edge;
/* Volatile-typed alias to the same input-edge global.  Case 7's tail is
 * byte-identical to case 0's (`lw kaze_input_edge; andi 0x40; j b54`), and old GCC's
 * cross-jump pass would merge the two, deleting one copy and dropping the
 * function 4 bytes short.  The target keeps both copies.  Reading the reload in
 * case 7 through a volatile-typed alias makes that load a distinct (volatile)
 * RTL insn, so cross-jump refuses to merge it, while a direct global reference
 * still folds to `lui/lw %lo(...)` and lets maspsx pack the `andi` into the jump
 * delay slot -- byte-exact, no scheduling barrier needed. */
extern volatile unsigned int D_800B1438_v __asm__("kaze_input_edge");

extern void kaze_sfx_play_dispatch(int);

void kaze_memcard_load_flow(void)
{
    int file_result;
    int status;
    unsigned int edge_bits;
    int sfx_id;

    switch (kaze_memory_card_load_state) {
    case 0:
        if ((kaze_input_edge & 0x20) != 0) {
            kaze_memory_card_slot = 0;
            kaze_memory_card_load_state = kaze_memory_card_load_state + 1;
            kaze_sfx_play_dispatch(2);
        }
        edge_bits = kaze_input_edge & 0x40;
        goto b54;
    case 1:
    case 3:
        kaze_memory_card_load_state = kaze_memory_card_load_state + 1;
        break;
    case 2:
        status = kaze_memcard_probe_blocking(kaze_memcard_slot_index);
        if (status == -1) {
            kaze_memory_card_load_state = 9;
        } else {
            file_result = kaze_memcard_file_count(kaze_memcard_slot_index);
            file_result = kaze_memcard_find_file(file_result, kaze_save_file_name);
            if (file_result != 0) {
                kaze_memory_card_load_state = 3;
            } else {
                kaze_memory_card_load_state = 6;
            }
        }
        break;
    case 4:
        status = kaze_memcard_file_read(kaze_memcard_slot_index, kaze_save_file_name, kaze_memcard_work_buffer, 1);
        if (status == 0) {
            status = kaze_save_verify_unpack_full();
            if (status == 0) {
                kaze_memory_card_load_state = 5;
            } else {
                if (kaze_memory_card_slot < 3) {
                    kaze_memory_card_slot = kaze_memory_card_slot + 1;
                } else {
                    kaze_memory_card_load_state = 8;
                }
            }
        } else {
            kaze_memory_card_load_state = 7;
        }
        break;
    case 5:
        if ((kaze_input_edge & 0x60) == 0) {
            return;
        }
        kaze_memory_card_prompt_latch = 1;
        sfx_id = 0x88;
        goto b68;
    case 7:
        if ((kaze_input_edge & 0x20) != 0) {
            kaze_memory_card_load_state = 1;
            kaze_sfx_play_dispatch(2);
        }
        edge_bits = D_800B1438_v & 0x40;
        goto b54;
    case 6:
    case 8:
    case 9:
        edge_bits = kaze_input_edge & 0x60;
    b54:
        if (edge_bits != 0) {
            kaze_memory_card_prompt_latch = 1;
            sfx_id = 0x65;
        b68:
            kaze_sfx_play_dispatch(sfx_id);
        }
        break;
    }
}
