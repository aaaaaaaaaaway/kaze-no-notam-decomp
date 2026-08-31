/**
 * kaze_vertex_midpoint_lerp  (library, 0x80030988)
 *
 * Averages two SVECTOR+UV vertices into a midpoint: (x,y,z) shorts and the
 * two texture-coordinate bytes stored in the pad field.
 */


#include "types.h"
void kaze_vertex_midpoint_lerp(s16 *out, s16 *a, s16 *b)
{
    out[0] = (a[0] + b[0]) / 2;
    out[1] = (a[1] + b[1]) / 2;
    out[2] = (a[2] + b[2]) / 2;
    *((u8 *)out + 6) = (*((u8 *)a + 6) + *((u8 *)b + 6)) >> 1;
    *((u8 *)out + 7) = (*((u8 *)a + 7) + *((u8 *)b + 7)) >> 1;
}
