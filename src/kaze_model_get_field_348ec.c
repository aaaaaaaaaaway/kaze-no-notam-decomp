/**
 * kaze_model_get_field_348ec  (render, 0x800348ec)
 *
 * Getter: reads the short at model draw struct offset arg0+0x2a+idx*4 into
 * *arg2.
 */



#include "types.h"
void kaze_model_get_field_348ec(int arg0, int arg1, u16 *arg2)
{
    *arg2 = *(u16 *)(arg0 + arg1 * 4 + 0x2A);
}
