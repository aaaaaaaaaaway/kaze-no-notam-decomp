/**
 * kaze_task_menu_enter_init  (menu, 0x8005ce24)
 *
 * Task-menu entry init: copies the current task's 0x14a0-byte model block
 * from (&kaze_save_slot_unpack_buffer)[kaze_task_record_index] to
 * *(kaze_menu_model1_data_ptr+0x10), links GsDOBJ2 kaze_menu_model1_gsobj,
 * resets state kaze_task_menu_state=0 and camera fields
 * kaze_menu_camera_pos_x.., then kaze_gs_light_init_alt_9454 + carousel
 * state init.
 */


#include "kaze_syms.h"
extern int kaze_task_record_index;
extern int kaze_task_menu_confirmed_index;
extern int kaze_task_menu_confirm_flag;
extern short kaze_balloon_switch_direction_flag;
extern int kaze_balloon_switch_anim_counter;
extern int kaze_menu_camera_trans_x;
extern int kaze_menu_camera_trans_y;
extern int kaze_menu_camera_trans_z;
extern int kaze_menu_camera_coord_word;
extern int kaze_menu_model1_gsobj;
extern int kaze_menu_balloon_model_ptr;
extern int kaze_task_transition_counter;
extern int kaze_menu_camera_pos_z;
extern int kaze_menu_camera_scroll;
typedef struct {
    char bytes[0x14A0];
} ModelBlock;

extern int kaze_task_menu_state;
extern short kaze_menu_camera_rot_x;
extern short kaze_menu_camera_rot_y;
extern short kaze_menu_camera_rot_z;
extern short kaze_menu_render_rot_x;
extern short kaze_menu_render_rot_y;
extern short kaze_menu_render_rot_z;
extern int kaze_menu_camera_pos_x;
extern int kaze_menu_camera_coord_flags;
extern int kaze_save_slot_unpack_buffer[];
extern ModelBlock **kaze_menu_model1_data_ptr;

extern void GsLinkObject4(int, void *, int);

void kaze_task_menu_enter_init(void)
{
    int zero;
    int idx;

    idx = kaze_task_record_index;
    kaze_task_menu_state = 0;
    kaze_task_menu_confirm_flag = 0;
    kaze_task_transition_counter = 0;
    kaze_balloon_switch_anim_counter = 0;
    kaze_balloon_switch_direction_flag = 0;
    kaze_task_menu_confirmed_index = idx;
    *kaze_menu_model1_data_ptr[4] = *(ModelBlock *)kaze_save_slot_unpack_buffer[idx];
    GsLinkObject4(kaze_menu_balloon_model_ptr + 0xC, &kaze_menu_model1_gsobj, 0);
    kaze_menu_camera_rot_x = 0;
    kaze_menu_camera_rot_z = 0;
    zero = 0;
    kaze_menu_camera_rot_y = zero;
    kaze_menu_camera_pos_x = 0x140;
    kaze_menu_camera_scroll = -0x60;
    kaze_menu_camera_pos_z = 0x600;
    kaze_menu_camera_coord_flags = 0;
    kaze_menu_render_rot_x = zero;
    kaze_menu_render_rot_z = 0;
    kaze_menu_render_rot_y = 0;
    kaze_menu_camera_trans_x = 0x140;
    kaze_menu_camera_trans_y = 0x120;
    do { } while (0);
    kaze_menu_camera_trans_z = 0x600;
    kaze_menu_camera_coord_word = zero;
    kaze_gs_light_init_alt_9454(0x140);
    kaze_task_carousel_state_init();
}
