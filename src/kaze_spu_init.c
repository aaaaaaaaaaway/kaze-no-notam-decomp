/**
 * kaze_spu_init  (library, 0x80089a28)
 *
 * PsyQ SpuInit (S_I.OBJ): initializes the SPU via _SpuInit(0).
 */


#include "kaze_syms.h"
extern void _SpuInit(int);
void kaze_spu_init(void)
{
    _SpuInit(0);
}
