#include "types.h"
#include "kaze_syms.h"
extern s16 kaze_ss_mono_flag;

/**
 * kaze_ss_set_mono  (library, 0x8008942c)
 *
 * PsyQ SsSetMono (VS_MONO.OBJ): sets the mono output flag kaze_ss_mono_flag
 * to 1.
 */

void kaze_ss_set_mono(void)
{
    kaze_ss_mono_flag = 1;
}

/**
 * kaze_ss_set_stereo  (library, 0x80089440)
 *
 * PsyQ SsSetStereo (VS_MONO.OBJ): clears the mono output flag
 * kaze_ss_mono_flag.
 */

void kaze_ss_set_stereo(void)
{
    kaze_ss_mono_flag = 0;
}
