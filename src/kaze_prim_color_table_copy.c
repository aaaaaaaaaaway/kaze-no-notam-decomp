/**
 * kaze_prim_color_table_copy  (render, 0x80012410)
 *
 * Copies 0x108 RGB byte triples (stride 4) from the color table at
 * kaze_balloon_color_table_r into the primitive/color array pointed to by
 * struct+0x8c of base.
 */



#include "types.h"
extern u8 kaze_balloon_color_table_r;

void kaze_prim_color_table_copy(int base)
{
    u8 u;
    u8 *p2;
    u8 *p3;
    u8 *p4;
    u8 *p5;
    int i;

    p5 = *(u8 **)(base + 0x8C);
    p4 = &kaze_balloon_color_table_r;
    i = 0;
    p3 = p4 + 2;
    p2 = p5 + 2;
    do {
        i++;
        u = *p4;
        p4 += 4;
        *p5 = u;
        u = p3[-1];
        p5 += 4;
        p2[-1] = u;
        u = *p3;
        p3 += 4;
        *p2 = u;
        p2 += 4;
    } while (i < 0x108);
}
