/**
 * kaze_ending_camera_reset_defaults  (menu, 0x80065954)
 *
 * Resets the ending viewer camera/light to defaults: camera at z=-0xbb8 with
 * zero angles, balloon pos kaze_menu_camera_pos_x/e4/e8 and
 * kaze_menu_camera_rot_x.. zeroed, light dir (0,1000,1000) and white color.
 */

#include "types.h"
#include "kaze_syms.h"
extern int kaze_ending_light0_dir_x;
extern int kaze_ending_light0_dir_y;
extern int kaze_ending_light0_dir_z;
extern u8 kaze_ending_light0_color_r;
extern u8 kaze_ending_light0_color_g;
extern u8 kaze_ending_light0_color_b;
extern int kaze_ending_camera_pos_x;
extern int kaze_ending_camera_pos_y;
extern int kaze_ending_camera_pos_z;
extern int kaze_menu_camera_pos_z;
extern int kaze_menu_camera_scroll;
extern s16 kaze_ending_camera_angle_x;
extern s16 kaze_ending_camera_angle_y;
extern s16 kaze_ending_camera_angle_z;
extern int kaze_menu_camera_pos_x;
extern s16 kaze_menu_camera_rot_x;
extern s16 kaze_menu_camera_rot_y;
extern s16 kaze_menu_camera_rot_z;

void kaze_ending_camera_reset_defaults(void)
{
    int c;

    kaze_ending_camera_pos_z = -3000;
    kaze_ending_light0_dir_y = 1000;
    kaze_ending_light0_dir_z = 1000;
    c = 0xff;
    kaze_ending_camera_angle_x = 0;
    kaze_ending_camera_angle_y = 0;
    kaze_ending_camera_angle_z = 0;
    kaze_ending_camera_pos_x = 0;
    kaze_ending_camera_pos_y = 0;
    kaze_menu_camera_pos_x = 0;
    kaze_menu_camera_scroll = 0;
    kaze_menu_camera_pos_z = 0;
    kaze_menu_camera_rot_x = 0;
    kaze_menu_camera_rot_y = 0;
    kaze_menu_camera_rot_z = 0;
    kaze_ending_light0_dir_x = 0;
    kaze_ending_light0_color_r = c;
    kaze_ending_light0_color_g = c;
    kaze_ending_light0_color_b = c;
}
