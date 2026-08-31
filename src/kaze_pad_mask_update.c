/**
 * kaze_pad_mask_update  (core, 0x80012274)
 *
 * Per-frame pad read: latches previous masks into kaze_input_current_b/14,
 * decodes both digital pad buffers (kaze_pad1_buffer/kaze_pad2_status_byte,
 * type nibble 4) into button masks, and ORs pad 2 into the main input mask
 * kaze_input_current_a.
 */

#include "types.h"
#include "kaze_syms.h"
extern u8 kaze_pad1_type_byte;
extern u8 kaze_pad1_buttons_hi;
extern u8 kaze_pad1_buttons_lo;
extern u8 kaze_pad2_status_byte;
extern u8 kaze_pad2_type_byte;
extern u8 kaze_pad2_buttons_hi;
extern u8 kaze_pad2_buttons_lo;
extern int kaze_input_current_a;
extern int kaze_input_current_pad2_mask;
extern int kaze_input_current_b;
extern int kaze_input_prev_pad2_mask;
extern u8 kaze_pad1_buffer;

void kaze_pad_mask_update(void)
{
    kaze_input_current_b = kaze_input_current_a;
    kaze_input_current_a = 0;
    kaze_input_prev_pad2_mask = kaze_input_current_pad2_mask;
    if (kaze_pad1_buffer == 0 && (kaze_pad1_type_byte >> 4) == 4) {
        kaze_input_current_a = ((kaze_pad1_buttons_hi << 8) | kaze_pad1_buttons_lo) ^ 0xFFFF;
    }
    kaze_input_current_pad2_mask = 0;
    if (kaze_pad2_status_byte == 0 && (kaze_pad2_type_byte >> 4) == 4) {
        kaze_input_current_pad2_mask = ((kaze_pad2_buttons_hi << 8) | kaze_pad2_buttons_lo) ^ 0xFFFF;
    }
    kaze_input_current_a = kaze_input_current_a | kaze_input_current_pad2_mask;
}
