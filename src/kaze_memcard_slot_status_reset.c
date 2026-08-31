/**
 * kaze_memcard_slot_status_reset  (menu, 0x800604c8)
 *
 * Resets both card-slot status words
 * (kaze_memcard_slot_request_state/154c=0x11 request flags,
 * kaze_memcard_slot_status_flags/0e7c=0) and calls _card_wait(0) and
 * _card_wait(1).
 */


#include "kaze_syms.h"
extern void _card_wait(int);
extern int kaze_memcard_slot_request_state;
extern int kaze_memcard_slot_status_flags;

void kaze_memcard_slot_status_reset(void)
{
    int i;
    int val;
    int *p2;
    int *p1;

    i = 0;
    val = 0x11;
    p2 = &kaze_memcard_slot_request_state;
    p1 = &kaze_memcard_slot_status_flags;
    do {
        *p1 = 0;
        *p2 = val;
        p2++;
        i++;
        p1++;
    } while (i < 2);
    _card_wait(0);
    _card_wait(1);
}
