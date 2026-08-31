/**
 * kaze_clear_image_rect  (library, 0x800123cc)
 *
 * GPU wrapper: builds a RECT from four shorts, ClearImage to black,
 * DrawSync(0).
 */



#include "types.h"
typedef struct {
    s16 x, y, w, h;
} RECT;

extern void ClearImage(RECT *, int, int, int);
extern void DrawSync(int);

void kaze_clear_image_rect(short x, short y, short w, short h)
{
    RECT rect;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
}
