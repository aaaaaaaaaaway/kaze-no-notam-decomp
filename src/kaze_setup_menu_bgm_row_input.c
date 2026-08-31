/**
 * kaze_setup_menu_bgm_row_input  (menu, 0x80052c24)
 *
 * Setup menu BGM-row input (state 5): Left/Right cycle
 * kaze_menu_weather_source 0..9 and map it to BGM id
 * kaze_setup_menu_bgm_track_id (0xd default, else 0..8); Up to state 4,
 * Circle to confirm 6, Cross back to mode row 2.
 */


#include "kaze_syms.h"
extern int kaze_setup_menu_bgm_track_id;
extern unsigned char kaze_setup_row1_flash_bgm;
extern unsigned char kaze_setup_row2_flash_bgm;
extern int kaze_title_menu_state;
extern int kaze_input_edge;
extern int kaze_menu_weather_source;

extern void kaze_sfx_play_dispatch(int);

void kaze_setup_menu_bgm_row_input(void)
{
    unsigned int input;

    if ((kaze_input_edge & 0x2000) != 0) {
        kaze_setup_row2_flash_bgm = 0xE0;
        if (kaze_menu_weather_source < 9) {
            kaze_menu_weather_source = kaze_menu_weather_source + 1;
        } else {
            kaze_menu_weather_source = 0;
        }
        kaze_sfx_play_dispatch(1);
    } else if ((kaze_input_edge & 0x8000) != 0) {
        kaze_setup_row1_flash_bgm = 0xE0;
        if (kaze_menu_weather_source > 0) {
            kaze_menu_weather_source = kaze_menu_weather_source - 1;
        } else {
            kaze_menu_weather_source = 9;
        }
        kaze_sfx_play_dispatch(1);
    }

    input = kaze_input_edge;
    if ((input & 0x1000) != 0) {
        kaze_title_menu_state = 4;
        kaze_setup_menu_bgm_track_id = 0xD;
        kaze_sfx_play_dispatch(1);
    } else if ((input & 0x20) != 0) {
        kaze_title_menu_state = 6;
        kaze_setup_menu_bgm_track_id = 0xD;
        kaze_sfx_play_dispatch(0x88);
    } else if ((input & 0x40) != 0) {
        kaze_title_menu_state = 2;
        kaze_setup_menu_bgm_track_id = 0xD;
        kaze_sfx_play_dispatch(0x65);
    } else {
        if ((unsigned)kaze_menu_weather_source < 10) {
            switch (kaze_menu_weather_source) {
                case 0:
                    kaze_setup_menu_bgm_track_id = 0xD;
                    break;
                case 1:
                    kaze_setup_menu_bgm_track_id = 0;
                    break;
                case 2:
                    kaze_setup_menu_bgm_track_id = 1;
                    break;
                case 3:
                    kaze_setup_menu_bgm_track_id = 2;
                    break;
                case 4:
                    kaze_setup_menu_bgm_track_id = 5;
                    break;
                case 5:
                    kaze_setup_menu_bgm_track_id = 4;
                    break;
                case 6:
                    kaze_setup_menu_bgm_track_id = 3;
                    break;
                case 7:
                    kaze_setup_menu_bgm_track_id = 6;
                    break;
                case 8:
                    kaze_setup_menu_bgm_track_id = 7;
                    break;
                case 9:
                    kaze_setup_menu_bgm_track_id = 8;
                    break;
            }
        }
    }
}
