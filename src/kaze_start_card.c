/**
 * kaze_start_card  (library, 0x800675c4)
 *
 * PsyQ StartCARD: StartCARD2 plus ChangeClearPAD(0) inside a critical
 * section.
 */


#include "kaze_syms.h"
extern int StartCARD2(void);
extern int kaze_enter_critical_section(void);
extern int ChangeClearPAD(int);
extern int kaze_exit_critical_section(void);

void kaze_start_card(void)
{
    kaze_enter_critical_section();
    StartCARD2();
    ChangeClearPAD(0);
    kaze_exit_critical_section();
}
