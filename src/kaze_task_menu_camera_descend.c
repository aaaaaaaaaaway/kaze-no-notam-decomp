/**
 * kaze_task_menu_camera_descend  (menu, 0x8005c734)
 *
 * Exit-state camera step: keeps yaw kaze_menu_camera_rot_y spinning while
 * lowering camera height kaze_menu_camera_scroll by 0x10 per frame.
 */

#include "types.h"
#include "kaze_syms.h"
extern int kaze_menu_camera_scroll;
extern int kaze_menu_camera_coord_flags;
extern u16 kaze_menu_camera_rot_y;

void kaze_task_menu_camera_descend(void)
{
    u16 *rot;
    int *scroll;
    u16 a;
    int b;

    rot = &kaze_menu_camera_rot_y;
    a = *rot;
    scroll = &kaze_menu_camera_scroll;
    kaze_menu_camera_coord_flags = 0;
    b = *scroll;
    a = (a + 8) & 0xFFF;
    b = b - 0x10;
    *rot = a;
    *scroll = b;
}
