/**
 * kaze_task_menu_state_machine  (menu, 0x8005cfcc)
 *
 * Task-menu master state machine on kaze_task_menu_state: 0 list, 2 slide-
 * out, 0x28 carousel, 0x29 zoom, 0x2b exit anim, 0x2c auto-spin; draws the
 * 3D frame, and at state 99 returns 1 (cancel, fade
 * kaze_gs_view_set_pose_94c8) or 2 (task chosen, restores
 * kaze_task_record_index=kaze_task_menu_confirmed_index).
 */


#include "kaze_syms.h"
extern int kaze_task_record_index;
extern int kaze_task_menu_confirmed_index;
extern int kaze_task_menu_confirm_flag;
extern unsigned int kaze_task_menu_state;
extern char kaze_menu_gs_ot_buf[];
extern void kaze_task_carousel_exit_handler(void);
extern void kaze_task_menu_camera_descend(void);
extern void kaze_title_model_sort_dispatch(int, void *);
extern void kaze_drmove_addprim_a0ec(int, int);
int kaze_task_menu_state_machine(int buf, int draw_only, int mode_arg)
{
  register int saved_arg asm("$16");
  register int mode asm("$17");
  ;
  mode = mode_arg;
  switch (kaze_task_menu_state)
  {
    case 0:
      if (draw_only == 0)
    {
      kaze_balloon_select_input();
    }
      kaze_task_name_empty_check();
      kaze_task_list_draw(buf, mode);

    default:
      goto state_check;

    case 2:
      kaze_task_list_slide_out_anim(buf);
      kaze_task_list_slide_draw(buf, mode);
      goto state_check;

    case 0x29:
      kaze_task_carousel_zoom_anim();
      break;

    case 0x28:
      kaze_task_carousel_rotate_input();
      kaze_task_carousel_rotate_interp();
      kaze_task_carousel_layout_update();
      kaze_task_carousel_select_input();
      kaze_task_letter_add_anim_commit();
      kaze_task_letter_delete_input();
      kaze_task_letter_delete_anim();
      kaze_task_menu_camera_bob();
      break;

    case 0x2c:
      kaze_task_carousel_autospin_step();
      break;

    case 0x2b:
      kaze_task_carousel_exit_handler();
      kaze_task_menu_camera_descend();

  }

  kaze_gs_light_set_color_9568(0, 0xff, 0xff, 0xff);
  kaze_title_model_sort_dispatch(buf, kaze_menu_gs_ot_buf + (buf * 20));
  kaze_task_carousel_glyph_render(buf);
  kaze_task_name_letters_render(buf);
  kaze_drmove_addprim_a0ec(buf, 0xfff);
  state_check:
  if (kaze_task_menu_state == 99)
  {
    if (kaze_task_menu_confirm_flag != 0)
    {
      kaze_task_record_index = kaze_task_menu_confirmed_index;
      return 2;
    }
    kaze_gs_view_set_pose_94c8(0, 0, 0, 0, 0, -0x180);
    kaze_gs_light_init_default_93b0();
    return 1;
  }

  return 0;
}
