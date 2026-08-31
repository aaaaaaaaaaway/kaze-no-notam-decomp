/**
 * kaze_setup_menu_loop  (menu, 0x800523d8)
 *
 * Game setup menu loop (menu mode 1): VSync/GsOT loop over states
 * kaze_title_menu_state (10 intro, 0xb memcard prompt, 1 stage row, 2 mode
 * row via kaze_settings_mode_row_handler, 3/4 option rows, 5 BGM row, 6
 * confirm, 0x14) until 99; keeps BGM kaze_setup_menu_bgm_track_id playing
 * each frame via kaze_xa_audio_manager_35a0.
 */


#include "kaze_syms.h"
extern int kaze_setup_menu_bgm_track_id;
extern int kaze_menu_vcount_timestamp;
extern int kaze_setup_menu_intro_scroll(int);
extern int kaze_title_menu_state;
extern char kaze_menu_gs_ot_buf;
extern int kaze_menu_ot_work_buf0[];

extern void GsInitVcount(void);
extern int VSync(int);
extern void kaze_gs_clear_vcount(void);
extern int GsGetActiveBuff(void);
extern void kaze_gs_set_work_base(int);
extern void GsClearOt(int, int, void *);
extern void kaze_task_menu_state_machine(int, int, int);
extern int DrawSync(int);
extern int GsGetVcount(void);
extern void kaze_input_edge_detect(void);
extern void GsSwapDispBuff(void);
extern void GsDrawOt(void *);

void kaze_setup_menu_loop(void)
{
    int active;
    int done;
    int ot_base;

    kaze_setup_menu_init();
    GsInitVcount();
    VSync(0);
    kaze_gs_clear_vcount();

    if (kaze_title_menu_state == 99) {
        return;
    }
    ot_base = (int)&kaze_menu_gs_ot_buf;
    do {
        active = GsGetActiveBuff();
        kaze_gs_set_work_base(kaze_menu_ot_work_buf0[active]);
        GsClearOt(0, 0, (void *)(active * 20 + ot_base));
        switch (kaze_title_menu_state) {
        case 10:
            done = kaze_setup_menu_intro_scroll(active);
            if (done != 0) {
                kaze_title_menu_state = 0xB;
            }
            kaze_task_menu_state_machine(active, 1, 0);
            break;
        case 0xB:
            kaze_setup_menu_memcard_prompt_state(active);
            break;
        case 1:
            kaze_setup_menu_stage_row_input();
            kaze_setup_menu_stage_row_draw(active);
            break;
        case 2:
            kaze_settings_mode_row_handler();
            kaze_setup_menu_mode_row_draw(active);
            break;
        case 0x14:
            kaze_setup_menu_ack_input_2978();
            kaze_setup_menu_draw_2f88(active);
            break;
        case 3:
            kaze_setup_menu_option_row_29b4();
            kaze_setup_menu_option_rows_draw(active);
            break;
        case 4:
            kaze_setup_menu_option_row_2ad8();
            kaze_setup_menu_option_rows_draw(active);
            break;
        case 5:
            kaze_setup_menu_bgm_row_input();
            kaze_setup_menu_option_rows_draw(active);
            break;
        case 6:
            kaze_setup_menu_confirm_input();
            kaze_setup_menu_option_rows_draw(active);
            break;
        }
        DrawSync(0);
        kaze_menu_vcount_timestamp = GsGetVcount();
        VSync(0);
        kaze_gs_clear_vcount();
        kaze_input_edge_detect();
        GsSwapDispBuff();
        GsDrawOt((void *)(active * 20 + ot_base));
        kaze_xa_audio_manager_35a0(kaze_setup_menu_bgm_track_id, 1);
    } while (kaze_title_menu_state != 99);
}
