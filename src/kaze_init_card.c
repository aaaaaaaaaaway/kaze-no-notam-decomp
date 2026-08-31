/**
 * kaze_init_card  (library, 0x80067570)
 *
 * PsyQ InitCARD: ChangeClearPAD(0), InitCARD2 with the given share flag,
 * applies _patch_card/_patch_card2 inside a critical section.
 */


#include "kaze_syms.h"
extern void InitCARD2(int);
extern void _patch_card(void);
extern void _patch_card2(void);
extern void ChangeClearPAD(int);
extern void kaze_enter_critical_section(void);
extern void kaze_exit_critical_section(void);

void kaze_init_card(int val)
{
    ChangeClearPAD(0);
    kaze_enter_critical_section();
    InitCARD2(val);
    _patch_card();
    _patch_card2();
    kaze_exit_critical_section();
}
