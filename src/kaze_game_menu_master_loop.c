/**
 * kaze_game_menu_master_loop  (menu, 0x80050b04)
 *
 * Game-menu master loop: loads MBG.TIM + GM.MDL/GM.TXP, fades in, dispatches
 * kaze_menu_screen_mode modes (0 main select, 1 setup, 2/4/5 sub-screens)
 * until 8/9; on 9 commits settings via kaze_settings_menu_dispatch; frees
 * model, returns state !...
 */


#include "kaze_syms.h"
extern int kaze_menu_screen_mode;
extern int kaze_staged_stage_variant;
int kaze_game_menu_master_loop(void)
{
    kaze_gs_graphics_init_9170();
    kaze_game_menu_mode_init();
    kaze_gs_view_set_pose_94c8(0, 0, 0, 0, 0, -0x180);
    kaze_menu_bg_tim_load();
    kaze_menu_fade_transition(0);
    kaze_menu_model_load();
    kaze_cd_setmode_vsync_341c();
    if (kaze_menu_screen_mode != 9) {
        do {
            switch (kaze_menu_screen_mode) {
        case 1:
            kaze_setup_menu_loop();
            break;
        case 2:
            kaze_memcard_screen_loop();
            break;
        case 5:
            kaze_grid_select_screen_loop();
            break;
        case 4:
            kaze_options_screen_loop();
            break;
        case 0:
        case 3:
        default:
            kaze_main_menu_loop();
            break;
            }
            if (kaze_menu_screen_mode == 8) {
                kaze_staged_stage_variant = 0;
                break;
            }
        } while (kaze_menu_screen_mode != 9);
    }
    kaze_menu_fade_transition(1);
    if (kaze_menu_screen_mode == 9) {
        kaze_settings_menu_dispatch();
    }
    kaze_menu_model_free();
    kaze_gs_graphics_free_9270();
    kaze_xa_audio_stop_33a3c();
    return kaze_menu_screen_mode != 9;
}
