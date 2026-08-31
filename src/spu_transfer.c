

/**
 * kaze_spu_set_in_transfer  (library, 0x8008bfc8)
 *
 * PsyQ _spu_setInTransfer (S_IT.OBJ): sets the SPU transfer-busy flag
 * kaze_spu_transfer_busy_flag from the given flag value.
 */


#include "kaze_syms.h"
extern int kaze_spu_transfer_busy_flag;
void kaze_spu_set_in_transfer(int arg0) {
    int one = 1;

    if (arg0 == one) {
        kaze_spu_transfer_busy_flag = 0;
    } else {
        kaze_spu_transfer_busy_flag = one;
    }
}

/**
 * kaze_spu_get_in_transfer  (library, 0x8008bff4)
 *
 * PsyQ _spu_getInTransfer (S_IT.OBJ): returns whether the SPU transfer-busy
 * flag kaze_spu_transfer_busy_flag indicates a transfer in progress.
 */

int kaze_spu_get_in_transfer(void)
{
    return kaze_spu_transfer_busy_flag != 1;
}
