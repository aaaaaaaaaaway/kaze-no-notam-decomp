#include "types.h"
#include "kaze_syms.h"
extern int kaze_options_state;
extern int kaze_options_fade_counter;
extern int kaze_button_config_action_row;
extern int kaze_button_config_toggle_row;
extern int kaze_options_fade_mode;

extern int kaze_input_edge;
/* input edge mask */
extern short kaze_button_config_remap_armed_flag;
/* remap-armed flag */
extern short kaze_button_config_column_flag;
/* toggle-column flag */
/* action row */
/* toggle row */
extern unsigned char kaze_settings_toggle_a;
/* per-toggle flag bytes */
void kaze_button_remap_assign();

/**
 * kaze_button_config_input  (menu, 0x80055f80)
 *
 * Navigates the key-config grid: rows kaze_button_config_action_row (5
 * actions) / kaze_button_config_toggle_row (2 toggles), column
 * kaze_button_config_column_flag, exit row
 * kaze_button_config_remap_armed_flag; toggles kaze_settings_toggle_a flags
 * and hands remap to kaze_button_remap_assign.
 */

void kaze_button_config_input(void)
{
    extern void kaze_sfx_play_dispatch(int);
    unsigned char *toggle_byte;
    int sfx_id;

    if ((kaze_input_edge & 0x8000) != 0 && kaze_button_config_remap_armed_flag == 0 && kaze_button_config_column_flag != 0) {
        kaze_button_config_action_row = 0;
        kaze_button_config_column_flag = 0;
        kaze_sfx_play_dispatch(1);
    }
    if ((kaze_input_edge & 0x2000) != 0 && kaze_button_config_remap_armed_flag == 0 && kaze_button_config_column_flag == 0) {
        kaze_button_config_toggle_row = 0;
        kaze_button_config_column_flag = 1;
        kaze_sfx_play_dispatch(1);
    }
    if ((kaze_input_edge & 0x4000) != 0 && kaze_button_config_remap_armed_flag == 0) {
        if (kaze_button_config_column_flag == 0) {
            if (kaze_button_config_action_row == 4) goto LAB_arm;
            if (kaze_button_config_action_row < 4) {
                kaze_button_config_action_row = kaze_button_config_action_row + 1;
            }
            goto LAB_arm_sfx;
        }
        if (kaze_button_config_toggle_row != 1) goto LAB_c4;
LAB_arm:
        kaze_button_config_remap_armed_flag = 1;
        goto LAB_arm_sfx;
LAB_c4:
        if (kaze_button_config_toggle_row > 0) goto LAB_arm_sfx;
        kaze_button_config_toggle_row = kaze_button_config_toggle_row + 1;
LAB_arm_sfx:
        kaze_sfx_play_dispatch(1);
    }
    if ((kaze_input_edge & 0x1000) != 0) {
        if (kaze_button_config_remap_armed_flag != 0) {
            kaze_button_config_remap_armed_flag = 0;
            goto LAB_dec_sfx;
        }
        if (kaze_button_config_column_flag == 0) {
            if (kaze_button_config_action_row > 0) {
                kaze_button_config_action_row = kaze_button_config_action_row - 1;
                goto LAB_dec_sfx;
            }
        } else if (kaze_button_config_toggle_row > 0) {
            kaze_button_config_toggle_row = kaze_button_config_toggle_row - 1;
LAB_dec_sfx:
            kaze_sfx_play_dispatch(1);
        }
    }
    if ((kaze_input_edge & 0x20) != 0) {
        if (kaze_button_config_remap_armed_flag != 0) {
            kaze_options_state = 0;
            kaze_options_fade_counter = 0;
            kaze_options_fade_mode = 2;
            kaze_backbuffer_copy_offscreen();
            sfx_id = 2;
        } else {
            sfx_id = 0x88;
            if (kaze_button_config_column_flag == 0) goto LAB_remap;
            toggle_byte = &kaze_settings_toggle_a + kaze_button_config_toggle_row;
            *toggle_byte = (*toggle_byte + 1) & 1;
        }
        kaze_sfx_play_dispatch(sfx_id);
    }
    if (kaze_button_config_column_flag != 0) {
        return;
    }
LAB_remap:
    if (kaze_button_config_remap_armed_flag == 0) {
        kaze_button_remap_assign(kaze_button_config_action_row);
    }
}

extern u8 kaze_button_action_map[];
extern int kaze_button_map_find_action(unsigned int arg0);
extern void kaze_button_map_copy_slot(int arg0, int arg1);

/**
 * kaze_button_remap_assign  (menu, 0x80056254)
 *
 * Assigns the pressed pad button (masks 0x20/0x80/0x10/0x40/4/1/8/2 mapped
 * to action codes 0..7) into config slot (&kaze_button_action_map)[row],
 * swapping conflicts via
 * kaze_button_map_find_action/kaze_button_map_copy_slot.
 */

void kaze_button_remap_assign(int row)
{
    extern void kaze_sfx_play_dispatch(int arg0);
    int conflict_slot;

    if ((kaze_input_edge & 0x20) != 0) {
        conflict_slot = kaze_button_map_find_action(0);
        if (conflict_slot != -1) kaze_button_map_copy_slot(row, conflict_slot);
        if (kaze_button_action_map[row] == 0) return;
        kaze_button_action_map[row] = 0;
        goto play;
    }
    if ((kaze_input_edge & 0x80) != 0) {
        conflict_slot = kaze_button_map_find_action(3);
        if (conflict_slot != -1) kaze_button_map_copy_slot(row, conflict_slot);
        if (kaze_button_action_map[row] == 3) return;
        kaze_button_action_map[row] = 3;
        goto play;
    }
    if ((kaze_input_edge & 0x10) != 0) {
        conflict_slot = kaze_button_map_find_action(2);
        if (conflict_slot != -1) kaze_button_map_copy_slot(row, conflict_slot);
        if (kaze_button_action_map[row] == 2) return;
        kaze_button_action_map[row] = 2;
        goto play;
    }
    if ((kaze_input_edge & 0x40) != 0) {
        conflict_slot = kaze_button_map_find_action(1);
        if (conflict_slot != -1) kaze_button_map_copy_slot(row, conflict_slot);
        if (kaze_button_action_map[row] == 1) return;
        kaze_button_action_map[row] = 1;
        goto play;
    }
    if ((kaze_input_edge & 4) != 0) {
        conflict_slot = kaze_button_map_find_action(4);
        if (conflict_slot != -1) kaze_button_map_copy_slot(row, conflict_slot);
        if (kaze_button_action_map[row] == 4) return;
        kaze_button_action_map[row] = 4;
        goto play;
    }
    if ((kaze_input_edge & 1) != 0) {
        conflict_slot = kaze_button_map_find_action(5);
        if (conflict_slot != -1) kaze_button_map_copy_slot(row, conflict_slot);
        if (kaze_button_action_map[row] == 5) return;
        kaze_button_action_map[row] = 5;
        goto play;
    }
    if ((kaze_input_edge & 8) != 0) {
        conflict_slot = kaze_button_map_find_action(6);
        if (conflict_slot != -1) kaze_button_map_copy_slot(row, conflict_slot);
        if (kaze_button_action_map[row] == 6) return;
        kaze_button_action_map[row] = 6;
        goto play;
    }
    if ((kaze_input_edge & 2) != 0) {
        conflict_slot = kaze_button_map_find_action(7);
        if (conflict_slot != -1) kaze_button_map_copy_slot(row, conflict_slot);
        if (kaze_button_action_map[row] == 7) return;
        kaze_button_action_map[row] = 7;
        goto play;
    }
    return;
play:
    kaze_sfx_play_dispatch(0x88);
    return;
}
