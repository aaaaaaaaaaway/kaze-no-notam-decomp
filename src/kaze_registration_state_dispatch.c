/**
 * kaze_registration_state_dispatch  (menu, 0x800575f4)
 *
 * Dispatches the kaze_task_menu_state registration machine: 0 balloon
 * select, 10 confirm, 0x14 paint editor, 0x15/0x16 zoom in/out, 0x1e RGB
 * edit, 0x28/0x29/0x2b/0x2c task pages via kaze_task_carousel_frame_draw; on
 * 99 restores kaze_task_record_index.
 */


#include "kaze_syms.h"
extern int kaze_task_record_index;
extern int kaze_task_menu_confirmed_index;
extern int kaze_balloon_paint_hovered_poly_idx;
extern int kaze_balloon_paint_poly_pick(int);
extern unsigned int kaze_task_menu_state;

int kaze_registration_state_dispatch(int arg, int draw_only)
{
    register int saved_arg __asm__("$16");

    saved_arg = arg;
    switch (kaze_task_menu_state) {
    case 0:
        if (draw_only == 0) {
            kaze_balloon_select_input();
        }
        kaze_balloon_select_draw(saved_arg);
        break;
    case 10:
        kaze_registration_confirm_input();
        kaze_registration_confirm_draw(saved_arg);
        break;
    case 21:
        kaze_balloon_editor_zoom_in_anim(saved_arg);
        kaze_balloon_editor_zoom_in_draw(saved_arg);
        break;
    case 20:
        kaze_balloon_paint_cursor_move(saved_arg);
        kaze_balloon_paint_hovered_poly_idx = kaze_balloon_paint_poly_pick(saved_arg);
        kaze_balloon_paint_highlight_update(saved_arg);
        kaze_balloon_view_rotate_input();
        kaze_balloon_paint_input();
        kaze_balloon_paint_draw(saved_arg);
        break;
    case 22:
        kaze_balloon_editor_zoom_out_anim(saved_arg);
        kaze_balloon_editor_zoom_out_draw(saved_arg);
        break;
    case 30:
        kaze_balloon_color_channel_edit();
        kaze_balloon_color_editor_draw(saved_arg);
        break;
    case 41:
        kaze_task_carousel_intro_anim();
        goto draw_task;
    case 40:
        kaze_task_carousel_update();
        goto draw_task;
    case 44:
        kaze_task_carousel_autospin_step();
        goto draw_task;
    case 43:
        kaze_task_carousel_exit_update();
draw_task:
        kaze_task_carousel_frame_draw(saved_arg);
        break;
    }

    if (kaze_task_menu_state == 99) {
        kaze_task_record_index = kaze_task_menu_confirmed_index;
        kaze_gs_view_set_pose_94c8(0, 0, 0, 0, 0, -0x180);
        kaze_gs_light_init_default_93b0();
        return 1;
    }
    return 0;
}
