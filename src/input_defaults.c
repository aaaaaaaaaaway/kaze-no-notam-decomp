/**
 * kaze_input_poll  (library, 0x800121d0)
 *
 * Controller input poll; source of the button mask constants (Triangle 0x10,
 * Circle 0x20, Cross 0x40, Square 0x80, Start 0x800, Up 0x1000, Right
 * 0x2000, Down 0x4000, Left 0x8000).
 */

extern int kaze_input_bind_up, kaze_input_bind_down, kaze_input_bind_left, kaze_input_bind_right, D_800B33B0, kaze_input_bind_pause;
extern int kaze_input_bind_burner, kaze_input_bind_marker_drop, kaze_input_action_button_mask, kaze_camera_invert_pitch, kaze_camera_invert_yaw, kaze_input_bind_descend;
extern int D_800B5640, kaze_input_bind_confirm;

void kaze_input_poll(void) {
    int start;
    int cross;
    int select;

    kaze_input_bind_up = 0x1000;
    kaze_input_bind_down = 0x4000;
    kaze_input_bind_left = 0x8000;
    kaze_input_bind_right = 0x2000;
    D_800B33B0 = 0x100;
    kaze_input_bind_pause = 0x800;
    start = 0x10;
    cross = 0x40;
    kaze_input_bind_burner = start;
    kaze_input_bind_marker_drop = 0x20;
    kaze_input_action_button_mask = 0x80;
    select = 8;
    kaze_camera_invert_pitch = 0;
    kaze_camera_invert_yaw = 0;
    kaze_input_bind_descend = cross;
    D_800B5640 = cross;
    kaze_input_bind_confirm = select;
}
