/**
 * kaze_memcard_result_decode  (library, 0x8004b56c)
 *
 * Decodes a memcard poll result into per-slot status bits at
 * kaze_memcard_slot_status_flags.
 */



#include "types.h"
extern uint kaze_memcard_slot_status_flags[];
extern uint kaze_memcard_slot_request_state[];

int kaze_memcard_result_decode(int slot, uint result)
{
    uint *p;
    int ret = 1;

    if (result == 0xfffffffe) {
        return 1;
    }
    if (result == 0xffffffff) {
        uint flags;
        slot <<= 2;
        flags = *(uint *)((char *)kaze_memcard_slot_request_state + slot);
        if (flags & 0x100) {
            uint *q = (uint *)((char *)kaze_memcard_slot_status_flags + slot);
            *q |= 0x10000;
        } else if (flags & 0x400) {
            uint *q = (uint *)((char *)kaze_memcard_slot_status_flags + slot);
            *q |= 0x40000;
        }
        return 0;
    }

    {
        uint mask = 0xFFF80000;
        uint t = (uint)kaze_memcard_slot_status_flags;
        p = (uint *)(t + (slot <<= 2));
        t = *p;
        mask |= 0xFFFF;
        *p = t & mask;
    }

    if (result & 0x100) {
        result &= 0xfffffeff;
        if (result == (uint)ret) goto cA_100;
        if (result == 2) goto cA_4;
        return ret;
    cA_100:
        *p = 0x100;
        return ret;
    cA_4:
        *p = 4;
        return ret;
    }
    if (result & 0x200) {
        result &= 0xfffffdff;
        if (result == 0) {
            uint *q;
            uint f;
            q = (uint *)((char *)kaze_memcard_slot_request_state + slot);
            f = *q;
            ret = 0;
            *q = f | 4;
            goto done;
        }
        if (result == 2) {
            *p = 4;
            return ret;
        }
        *p = 0x100;
        return ret;
    }
    if (result & 0x400) {
        result &= 0xfffffbff;
        if (result == (uint)ret) goto cB_100;
        if ((int)result < 2) {
            if (result == 0) goto cB_1;
            return ret;
        } else {
            if (result == 2) goto cB_4;
            if (result == 3) goto cB_2;
            return ret;
        }
    cB_100:
        *p = 0x100;
        goto done;
    cB_1:
        *p = 1;
        goto done;
    cB_4:
        *p = 4;
        goto done;
    cB_2:
        *p = 2;
        goto done;
    }
done:
    return ret;
}
