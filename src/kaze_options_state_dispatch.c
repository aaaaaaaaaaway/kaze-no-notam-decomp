/**
 * kaze_options_state_dispatch  (menu, 0x80055b80)
 *
 * State machine kaze_options_state: 0/1 top menu, 2/3 balloon registration
 * (kaze_registration_state_dispatch), 4/5 button config, 6/7 sound mode,
 * 10/0xb sub-screen kaze_records_screen_frame; fades via
 * kaze_options_fade_step.
 */


#include "kaze_syms.h"
extern int kaze_options_state;
extern int kaze_options_fade_counter;
extern int kaze_options_fade_mode;
extern int kaze_options_fade_reverse_flag;
void kaze_options_state_dispatch(int arg)
{
    register int saved_arg __asm__("$16");
    int done;

    saved_arg = arg;
    switch (kaze_options_state) {
    case 0:
        if (kaze_options_fade_step(saved_arg) != 0) {
            kaze_options_state = 1;
        }
        goto top_menu_draw;
    case 1:
        kaze_options_fade_reverse_flag = 0;
        kaze_options_top_menu_input();
top_menu_draw:
        kaze_options_top_menu_draw(saved_arg);
        break;
    case 10:
        if (kaze_options_fade_step(saved_arg) != 0) {
            kaze_options_state = 11;
        }
        kaze_records_screen_frame(saved_arg, 1);
        break;
    case 11:
        done = kaze_records_screen_frame(saved_arg, 0);
        goto sub_screen_done;
    case 2:
        if (kaze_options_fade_step(saved_arg) != 0) {
            kaze_options_state = 3;
        }
        kaze_registration_state_dispatch(saved_arg, 1);
        break;
    case 3:
        done = kaze_registration_state_dispatch(saved_arg, 0);
        goto sub_screen_done;
sub_screen_done:
        if (done != 0) {
            kaze_options_state = 0;
            kaze_options_fade_counter = 0;
            kaze_options_fade_mode = 2;
            kaze_backbuffer_copy_offscreen();
        }
        break;
    case 4:
        if (kaze_options_fade_step(saved_arg) != 0) {
            kaze_options_state = 5;
        }
        goto button_config_draw;
    case 5:
        kaze_button_config_input();
button_config_draw:
        kaze_button_config_screen_draw(saved_arg);
        break;
    case 6:
        if (kaze_options_fade_step(saved_arg) != 0) {
            kaze_options_state = 7;
        }
        goto sound_mode_draw;
    case 7:
        kaze_sound_mode_input();
sound_mode_draw:
        kaze_sound_mode_screen_draw(saved_arg);
        break;
    }
}
