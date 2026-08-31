/**
 * kaze_task_carousel_constants_init  (menu, 0x8005ab0c)
 *
 * Sets the carousel camera/animation constants kaze_task_carousel_transition
 * _amplitude..kaze_task_letter_delete_phase2_amplitude (lift amplitude,
 * letter fly-in offsets, easing divisors) used by the letter add/delete
 * animations.
 */


#include "kaze_syms.h"
extern int D_800AC53C;
extern int kaze_task_letter_add_spin_amplitude;
extern int kaze_task_letter_add_x_offset_0;
extern int kaze_task_letter_add_x_offset_1;
extern int kaze_task_letter_add_x_offset_2;
extern int kaze_task_letter_add_row_ease_slope;
extern int D_800AC554;
extern int kaze_task_letter_delete_phase1_divisor;
extern int kaze_task_letter_delete_phase2_divisor;
extern int kaze_task_letter_delete_phase1_amplitude;
extern int kaze_task_letter_delete_phase2_amplitude;
extern int kaze_task_carousel_transition_amplitude;
void kaze_task_carousel_constants_init(void)
{
    int pad[2];
    int a, b, c, d;

    a = 0xffff5556;
    b = 0xfffef112;
    __asm__("");
    c = 0xfffeeeef;
    kaze_task_carousel_transition_amplitude = -0x64;
    D_800AC53C = -0x1fc;
    kaze_task_letter_add_spin_amplitude = -0x200;
    kaze_task_letter_add_x_offset_2 = 0xaaaa;
    kaze_task_letter_delete_phase1_amplitude = -0xc0;
    kaze_task_letter_delete_phase2_amplitude = 0x230;
    kaze_task_letter_delete_phase1_divisor = 6;
    d = 0x11;
    kaze_task_letter_add_x_offset_0 = a;
    kaze_task_letter_add_x_offset_1 = 0;
    kaze_task_letter_add_row_ease_slope = b;
    D_800AC554 = c;
    kaze_task_letter_delete_phase2_divisor = d;
}
