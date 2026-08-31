/**
 * kaze_gs_clear_vcount  (library, 0x8006cbe0)
 *
 * PsyQ GsClearVcount/GsGetVcount helper: resets the vsync root counter via
 * ResetRCnt(0xf2000001).
 */


#include "kaze_syms.h"
extern int ResetRCnt(unsigned int);
void kaze_gs_clear_vcount(void)
{
    ResetRCnt(0xF2000001);
}
