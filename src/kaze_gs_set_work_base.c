/**
 * kaze_gs_set_work_base  (library, 0x8006da54)
 *
 * PsyQ GsSetWorkBase: stores the current GPU packet work area pointer in
 * kaze_gs_work_base_ptr; called each frame by screen loops with the active
 * packet buffer.
 */


#include "kaze_syms.h"
extern int kaze_gs_work_base_ptr;
void kaze_gs_set_work_base(int arg0)
{
    kaze_gs_work_base_ptr = arg0;
}
