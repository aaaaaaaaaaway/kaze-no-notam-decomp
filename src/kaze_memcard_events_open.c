/**
 * kaze_memcard_events_open  (library, 0x8004afcc)
 *
 * OpenEvent/EnableEvent for 8 memory-card events (both slots) stored at
 * kaze_memcard_slot0_event_a..a4.
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
extern void EnableEvent(int);
extern int OpenEvent(int, int, int, int);
extern void kaze_enter_critical_section(void);
extern void kaze_exit_critical_section(void);

void kaze_memcard_events_open(void)
{
    kaze_enter_critical_section();
    kaze_memcard_slot0_event_a = OpenEvent(0xF4000001, 4, 0x2000, 0);
    kaze_memcard_slot0_event_b = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);
    kaze_memcard_slot0_event_c = OpenEvent(0xF4000001, 0x100, 0x2000, 0);
    kaze_memcard_slot0_event_d = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);
    kaze_memcard_slot1_event_a = OpenEvent(0xF0000011, 4, 0x2000, 0);
    kaze_memcard_slot1_event_b = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);
    kaze_memcard_slot1_event_c = OpenEvent(0xF0000011, 0x100, 0x2000, 0);
    kaze_memcard_slot1_event_d = OpenEvent(0xF0000011, 0x2000, 0x2000, 0);
    kaze_exit_critical_section();
    EnableEvent(kaze_memcard_slot0_event_a);
    EnableEvent(kaze_memcard_slot0_event_b);
    EnableEvent(kaze_memcard_slot0_event_c);
    EnableEvent(kaze_memcard_slot0_event_d);
    EnableEvent(kaze_memcard_slot1_event_a);
    EnableEvent(kaze_memcard_slot1_event_b);
    EnableEvent(kaze_memcard_slot1_event_c);
    EnableEvent(kaze_memcard_slot1_event_d);
}
