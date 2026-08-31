/**
 * kaze_ss_init  (library, 0x800816d0)
 *
 * PsyQ SsInit (SSINIT_C.OBJ): ResetCallback, SPU init via kaze_spu_init,
 * then _SsInit for the sound system.
 */


#include "kaze_syms.h"
extern int _SsInit(void);
extern int ResetCallback(void);

void kaze_ss_init(void)
{
    ResetCallback();
    kaze_spu_init();
    _SsInit();
}
