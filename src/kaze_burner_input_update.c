/**
 * kaze_burner_input_update  (flight, 0x8001ec38)
 *
 * Burner input: button (kaze_input_current_a & kaze_input_bind_burner) with
 * gas kaze_gas>0 sets ramp kaze_burner_state (0x1000/0xe00), sound on,
 * target kaze_target_gas=0x64000; else decays ramp, clamps target above
 * kaze_min_gas.
 */


#include "kaze_syms.h"
extern int kaze_gas;
extern int kaze_min_gas;
extern int kaze_target_gas;
extern char *kaze_state_base_ptr;
extern unsigned int kaze_input_current_a;
extern unsigned int kaze_input_bind_burner;
extern int kaze_burner_state;

void kaze_burner_input_update(void)
{
    int target;
    int burner;
    int current_target;
    register int max_target __asm__("$2");
    volatile int *gas_state;
    register int *target_state __asm__("$6");
    register int sound_enable __asm__("$4");

    sound_enable = 0;
    if ((*(unsigned int *)(kaze_state_base_ptr + 0x14) & 1) != 0) {
        if (((kaze_input_current_a & kaze_input_bind_burner) != 0) && (burner = 0x1000, kaze_gas > 0)) {
            if (kaze_burner_state == 0x1000) {
                burner = 0xE00;
            }
            kaze_burner_state = burner;
            kaze_burner_sound_toggle(1);
            kaze_target_gas = 0x64000;
            goto clamp_target;
        }

        {
            register int *burner_state __asm__("$3");
            register int step __asm__("$2");
            register int decayed __asm__("$4");
            register int clamped __asm__("$5");

            burner_state = &kaze_burner_state;
            step = *(int *)(kaze_state_base_ptr + 0xA4);
            decayed = *burner_state;
            step <<= 7;
            decayed -= step;
            *burner_state = decayed;
            if (decayed >= 0) {
                clamped = 0x1000;
                if (decayed < 0x1001) {
                    clamped = decayed;
                }
            } else {
                clamped = 0;
            }
            sound_enable = 0;
            *burner_state = clamped;
        }
    }

    kaze_burner_sound_toggle(sound_enable);
    kaze_target_gas = 0;

clamp_target:
    gas_state = &kaze_gas;
    if (*gas_state > 0) {
        target = kaze_min_gas + 0x28000;
        target_state = gas_state + 1;
        current_target = *target_state;
        if (current_target < target) {
            goto store_positive_target;
        }
        max_target = 0x64000;
        target = 0x64000;
        if (max_target < current_target) {
            goto store_positive_target;
        }
        target = current_target;
store_positive_target:
        *target_state = target;
        return;
    }

    {
        register int *inactive_target_state __asm__("$5");

        inactive_target_state = gas_state + 1;
        current_target = *inactive_target_state;
        target = kaze_min_gas;
        if (current_target < target) {
            goto store_inactive_target;
        }
        max_target = 0x64000;
        target = 0x64000;
        if (max_target < current_target) {
            goto store_inactive_target;
        }
        target = current_target;
store_inactive_target:
        *inactive_target_state = target;
        __asm__ volatile("" : : : "memory");
    }
}
