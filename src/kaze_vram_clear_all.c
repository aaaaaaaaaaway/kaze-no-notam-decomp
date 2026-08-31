/**
 * kaze_vram_clear_all  (render, 0x80012364)
 *
 * Clears the whole 1024x512 VRAM as four 512x256 ClearImage rects via
 * kaze_clear_image_rect.
 */


#include "kaze_syms.h"
void kaze_vram_clear_all(void)
{
    kaze_clear_image_rect(0, 0, 0x200, 0x100);
    kaze_clear_image_rect(0, 0x100, 0x200, 0x100);
    kaze_clear_image_rect(0x200, 0, 0x200, 0x100);
    kaze_clear_image_rect(0x200, 0x100, 0x200, 0x100);
}
