

/**
 * kaze_ss_ut_reverb_off  (library, 0x800861a0)
 *
 * PsyQ SsUtReverbOff (UT_ROFF.OBJ): disables SPU reverb via SpuSetReverb(0).
 */


#include "kaze_syms.h"
extern int SpuSetReverb(int);
void kaze_ss_ut_reverb_off(void)
{
    SpuSetReverb(0);
}

/**
 * kaze_ss_ut_reverb_on  (library, 0x800861c0)
 *
 * PsyQ SsUtReverbOn (UT_RON.OBJ): enables SPU reverb via SpuSetReverb(1).
 */

void kaze_ss_ut_reverb_on(void)
{
    SpuSetReverb(1);
}
