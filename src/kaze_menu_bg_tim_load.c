/**
 * kaze_menu_bg_tim_load  (menu, 0x80050cd0)
 *
 * Loads the menu background image BAL\K\MBG.TIM and applies palette/display
 * setup via kaze_drmove_pair_setup_a064(0x140,0) and
 * kaze_semitrans_quad_setup(2).
 */


#include "kaze_syms.h"
extern char kaze_menu_bg_tim_path[]; /* "BAL\\K\\MBG.TIM" */

extern int kaze_drmove_pair_setup_a064(int, int);
extern int kaze_semitrans_quad_setup(int);

void kaze_menu_bg_tim_load(void)
{
    kaze_tim_display_by_name(kaze_menu_bg_tim_path);
    kaze_drmove_pair_setup_a064(0x140, 0);
    kaze_semitrans_quad_setup(2);
}
