/**
 * kaze_settings_menu_dispatch  (menu, 0x80050ff8)
 *
 * Settings menu row dispatcher; row 2 adjusts the mode source
 * kaze_menu_mode_source via kaze_settings_mode_row_handler (source 0 stages
 * mode 1, 1 stages mode 0, 2 stages mode 2).
 */


#include "kaze_syms.h"
extern int kaze_staged_stage_flags;
extern int kaze_staged_stage_level;
extern int kaze_staged_stage_mode;
extern int kaze_staged_stage_variant;
extern int kaze_staged_stage_weather;
extern int kaze_input_bind_marker_drop;
extern int kaze_menu_flags_source;
extern int kaze_menu_level_source;
extern int kaze_menu_stage_source;
extern int kaze_staged_stage_index;
extern int kaze_menu_mode_source;
extern int kaze_menu_weather_source;
extern int kaze_menu_variant_source;
extern unsigned int kaze_camera_invert_yaw;
extern unsigned int kaze_camera_invert_pitch;
extern unsigned char kaze_settings_toggle_a;
extern unsigned char kaze_settings_toggle_b;
extern unsigned char kaze_button_action_map;
extern unsigned char kaze_sound_mode_setting;
extern int kaze_input_bind_burner;
extern int kaze_input_bind_descend;
extern int kaze_input_action_button_mask;
extern int kaze_input_bind_confirm;

extern void kaze_xa_stream_enable_33b0(void);
extern void kaze_xa_stream_disable_33e8(void);

void kaze_settings_menu_dispatch(void)
{
    static void *const tbl1[10] = {
        &&b0, &&b1, &&b2, &&b3, &&b4, &&b5, &&b6, &&b7, &&b8, &&b9
    };
    static void *const tbl2[8] = {
        &&c0, &&c1, &&c2, &&c3, &&c4, &&c5, &&c6, &&c7
    };
    int flag_bit;
    int *dest_ptr;
    int **dest_slot;
    int i;
    int idx;
    void *const *jt;
    int *flag_dest[5];

    switch (kaze_menu_stage_source) {
    case 0:
        kaze_staged_stage_index = 2;
        break;
    case 1:
        kaze_staged_stage_index = 0;
        break;
    case 2:
        kaze_staged_stage_index = 1;
        break;
    }
    switch (kaze_menu_mode_source) {
    case 0:
        kaze_staged_stage_mode = 1;
        break;
    case 1:
        kaze_staged_stage_mode = 0;
        break;
    case 2:
        kaze_staged_stage_mode = kaze_menu_mode_source;
        break;
    }
    switch (kaze_menu_flags_source) {
    case 0:
        kaze_staged_stage_flags = 0;
        goto after_b3;
    case 1:
        flag_bit = 3;
        break;
    case 2:
        kaze_staged_stage_flags = 1;
        goto after_b3;
    case 3:
        flag_bit = 5;
        break;
    default:
        goto after_b3;
    }
    kaze_staged_stage_flags = flag_bit;
after_b3:
    switch (kaze_menu_level_source) {
    case 0:
        kaze_staged_stage_level = 1;
        goto after_b4;
    case 1:
        flag_bit = 2;
        break;
    case 2:
        flag_bit = 3;
        break;
    default:
        goto after_b4;
    }
    kaze_staged_stage_level = flag_bit;
after_b4:
    idx = kaze_menu_weather_source;
    if ((unsigned int)idx >= 10) goto after_b5;
    goto *tbl1[idx];
b0:
    kaze_staged_stage_weather = 0;
    goto after_b5;
b1:
    kaze_staged_stage_weather = 1;
    goto after_b5;
b2:
    kaze_staged_stage_weather = 2;
    goto after_b5;
b3:
    kaze_staged_stage_weather = 3;
    goto after_b5;
b4:
    kaze_staged_stage_weather = 6;
    goto after_b5;
b5:
    kaze_staged_stage_weather = 5;
    goto after_b5;
b6:
    kaze_staged_stage_weather = 4;
    goto after_b5;
b7:
    kaze_staged_stage_weather = 7;
    goto after_b5;
b8:
    kaze_staged_stage_weather = 8;
    goto after_b5;
b9:
    kaze_staged_stage_weather = 9;
after_b5:
    kaze_staged_stage_variant = kaze_menu_variant_source;
    kaze_balloon_color_apply();
    i = 0;
    jt = tbl2;
    dest_slot = flag_dest;
    kaze_camera_invert_yaw = (unsigned int)kaze_settings_toggle_a;
    kaze_camera_invert_pitch = (unsigned int)kaze_settings_toggle_b;
    flag_dest[0] = &kaze_input_bind_burner;
    flag_dest[1] = &kaze_input_bind_descend;
    flag_dest[2] = &kaze_input_bind_marker_drop;
    flag_dest[3] = &kaze_input_action_button_mask;
    flag_dest[4] = &kaze_input_bind_confirm;
    do {
        idx = (&kaze_button_action_map)[i];
        if ((unsigned int)idx >= 8) goto switchD_800512dc_default;
        goto *jt[idx];
c0:
        dest_ptr = *dest_slot;
        flag_bit = 0x20;
        goto store;
c1:
        dest_ptr = *dest_slot;
        flag_bit = 0x40;
        goto store;
c2:
        dest_ptr = *dest_slot;
        flag_bit = 0x10;
        goto store;
c3:
        dest_ptr = *dest_slot;
        flag_bit = 0x80;
        goto store;
c4:
        dest_ptr = *dest_slot;
        flag_bit = 4;
        goto store;
c5:
        dest_ptr = *dest_slot;
        flag_bit = 1;
        goto store;
c6:
        dest_ptr = *dest_slot;
        flag_bit = 8;
        goto store;
c7:
        dest_ptr = *dest_slot;
        flag_bit = 2;
store:
        *dest_ptr = flag_bit;
switchD_800512dc_default:
        i = i + 1;
        dest_slot = dest_slot + 1;
        if (4 < i) {
            if (kaze_sound_mode_setting == '\0') goto Lenable;
            if (kaze_sound_mode_setting == '\x01') goto Ldisable;
            goto Lret;
Lenable:
            kaze_xa_stream_enable_33b0();
            goto Lret;
Ldisable:
            kaze_xa_stream_disable_33e8();
Lret:
            return;
        }
    } while (1);
}
