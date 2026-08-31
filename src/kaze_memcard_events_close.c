/**
 * kaze_memcard_events_close  (library, 0x8004b174)
 *
 * CloseEvent the 8 memory-card events at kaze_memcard_slot0_event_a..a4.
 */


#include "kaze_syms.h"
extern int kaze_memcard_slot0_event_a;
extern int kaze_memcard_slot0_event_b;
extern int kaze_memcard_slot0_event_c;
extern int kaze_memcard_slot0_event_d;
extern int kaze_memcard_slot1_event_a;
extern int kaze_memcard_slot1_event_b;
extern int kaze_memcard_slot1_event_c;
extern int kaze_memcard_slot1_event_d;
extern void CloseEvent(int);
extern void kaze_enter_critical_section(void);
extern void kaze_exit_critical_section(void);

void kaze_memcard_events_close(void)
{
    kaze_enter_critical_section();
    CloseEvent(kaze_memcard_slot0_event_a);
    CloseEvent(kaze_memcard_slot0_event_b);
    CloseEvent(kaze_memcard_slot0_event_c);
    CloseEvent(kaze_memcard_slot0_event_d);
    CloseEvent(kaze_memcard_slot1_event_a);
    CloseEvent(kaze_memcard_slot1_event_b);
    CloseEvent(kaze_memcard_slot1_event_c);
    CloseEvent(kaze_memcard_slot1_event_d);
    kaze_exit_critical_section();
}
