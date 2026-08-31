/**
 * kaze_st_unset_ring  (library, 0x8007f5b8)
 *
 * PsyQ StUnSetRing (C_003.OBJ): clears the CD data/ready callbacks and CDROM
 * registers inside a critical section, ending ring-buffer streaming.
 */

#include "types.h"
#include "kaze_syms.h"
extern volatile u8 *D_800A55EC;
extern volatile u8 *D_800A55F8;
extern void CdDataCallback(int);
extern void CdReadyCallback(int);

extern void kaze_enter_critical_section(void);
extern void kaze_exit_critical_section(void);

void kaze_st_unset_ring(void)
{
    kaze_enter_critical_section();
    CdDataCallback(0);
    CdReadyCallback(0);
    *D_800A55EC = 0;
    *D_800A55F8 = 0;
    kaze_exit_critical_section();
}
