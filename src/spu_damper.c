#include "types.h"
#include "kaze_syms.h"
extern s16 kaze_ss_vm_damper_flag;

/**
 * kaze_ss_vm_damper_off  (library, 0x800863a8)
 *
 * PsyQ _SsVmDamperOff (VM_DOFF.OBJ): clears the voice-manager damper flag
 * kaze_ss_vm_damper_flag.
 */

void kaze_ss_vm_damper_off(void)
{
    kaze_ss_vm_damper_flag = 0;
}

/**
 * kaze_ss_vm_damper_on  (library, 0x800863b8)
 *
 * PsyQ _SsVmDamperOn (VM_DON.OBJ): sets the voice-manager damper flag
 * kaze_ss_vm_damper_flag to 2.
 */

void kaze_ss_vm_damper_on(void)
{
    kaze_ss_vm_damper_flag = 2;
}
