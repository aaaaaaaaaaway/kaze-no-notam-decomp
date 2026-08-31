/**
 * kaze_task_carousel_exit_update  (menu, 0x8005b17c)
 *
 * Exit-state (0x2b) update pair: runs the nested carousel exit handler
 * kaze_task_carousel_exit_handler and the camera-descend step
 * kaze_task_menu_camera_descend.
 */


extern int kaze_task_carousel_exit_handler(void);
extern int kaze_task_menu_camera_descend(void);

void kaze_task_carousel_exit_update(void)
{
    kaze_task_carousel_exit_handler();
    kaze_task_menu_camera_descend();
}
