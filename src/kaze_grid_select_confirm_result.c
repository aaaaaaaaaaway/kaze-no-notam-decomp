/**
 * kaze_grid_select_confirm_result  (menu, 0x80054e90)
 *
 * Routes the kaze_task_menu_state_machine dialog result: 1 enters grid state
 * 10; 2 exits (kaze_grid_select_state=99) clearing kaze_menu_variant_source/
 * kaze_grid_select_reentry_flag/kaze_menu_screen_mode.
 */


#include "kaze_syms.h"
extern int kaze_grid_select_state;
extern int kaze_grid_select_reentry_flag;
extern int kaze_menu_screen_mode;
extern int kaze_menu_variant_source;


void kaze_grid_select_confirm_result(int buf)
{
    switch (kaze_task_menu_state_machine(buf, 0, 1)) {
    case 1:
        kaze_grid_select_state = 10;
        break;
    case 2:
        kaze_grid_select_state = 99;
        kaze_menu_variant_source = 0;
        kaze_grid_select_reentry_flag = 0;
        kaze_menu_screen_mode = 0;
        break;
    }
    return;
}
