/**
 * kaze_button_config_cursor_reset  (menu, 0x80055f58)
 *
 * Zeroes the button-config cursor state
 * kaze_button_config_action_row/958/95c/960 (action row, toggle row, column,
 * exit-row flag).
 */

#include "types.h"
#include "kaze_syms.h"
extern int kaze_button_config_action_row;
extern int kaze_button_config_toggle_row;
extern s16 kaze_button_config_column_flag;
extern s16 kaze_button_config_remap_armed_flag;

void kaze_button_config_cursor_reset(void)
{
    kaze_button_config_action_row = 0;
    kaze_button_config_toggle_row = 0;
    kaze_button_config_column_flag = 0;
    kaze_button_config_remap_armed_flag = 0;
}
