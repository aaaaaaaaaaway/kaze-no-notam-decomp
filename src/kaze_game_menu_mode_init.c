/**
 * kaze_game_menu_mode_init  (menu, 0x80050c7c)
 *
 * Menu mode init: kaze_menu_screen_mode=0 normally or 5 when the skip/replay
 * flag kaze_staged_stage_variant is set; mirrors that into
 * kaze_grid_select_reentry_flag and sets kaze_game_menu_entered_flag=1.
 */

#include "types.h"
#include "kaze_syms.h"
extern int kaze_grid_select_reentry_flag;
extern int kaze_menu_screen_mode;
extern int kaze_staged_stage_variant;
extern u8 kaze_game_menu_entered_flag;

void kaze_game_menu_mode_init(void)
{
    if (kaze_staged_stage_variant != 0) {
        kaze_menu_screen_mode = 5;
        kaze_grid_select_reentry_flag = 1;
    } else {
        kaze_menu_screen_mode = 0;
        kaze_grid_select_reentry_flag = 0;
    }
    kaze_game_menu_entered_flag = 1;
}
