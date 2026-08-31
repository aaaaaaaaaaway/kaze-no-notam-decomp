#include "types.h"

extern u8 kaze_button_action_map[];

/**
 * kaze_button_map_find_action  (menu, 0x800564dc)
 *
 * Searches the 5-slot button-config array kaze_button_action_map for the
 * given action code; returns the slot index or -1.
 */

int kaze_button_map_find_action(unsigned int arg0)
{
    int i = 0;

    do {
        if (kaze_button_action_map[i] == arg0) {
            return i;
        }
        i++;
    } while (i < 5);
    return -1;
}

/**
 * kaze_button_map_copy_slot  (menu, 0x80056510)
 *
 * Copies the button-config value from slot arg0 to slot arg1 in
 * kaze_button_action_map (conflict-swap helper for remapping).
 */

void kaze_button_map_copy_slot(int arg0, int arg1)
{
    kaze_button_action_map[arg1] = kaze_button_action_map[arg0];
}
