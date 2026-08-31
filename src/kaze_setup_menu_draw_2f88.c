/**
 * kaze_setup_menu_draw_2f88  (menu, 0x80052f88)
 *
 * Setup menu state-0x14 draw wrapper; just calls kaze_title_bg_prims_add.
 */


#include "kaze_syms.h"
void kaze_setup_menu_draw_2f88(void)
{
    kaze_title_bg_prims_add();
}
