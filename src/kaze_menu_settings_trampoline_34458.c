/**
 * kaze_menu_settings_trampoline_34458  (menu, 0x80034458)
 *
 * Thin trampoline into the title/settings-menu neighborhood; simply calls
 * kaze_game_menu_master_loop.
 */


#include "kaze_syms.h"
void kaze_menu_settings_trampoline_34458(void)
{
    kaze_game_menu_master_loop();
}
