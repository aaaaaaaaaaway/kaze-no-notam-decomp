

/**
 * kaze_set_video_mode  (library, 0x80081264)
 *
 * PsyQ SetVideoMode (VMODE.OBJ): swaps the NTSC/PAL mode flag
 * kaze_video_mode and returns the previous value.
 */


#include "kaze_syms.h"
extern int kaze_video_mode;
int kaze_set_video_mode(int arg0)
{
    int old;

    old = kaze_video_mode;
    kaze_video_mode = arg0;
    return old;
}

/**
 * kaze_get_video_mode  (library, 0x8008127c)
 *
 * PsyQ GetVideoMode (VMODE.OBJ): returns the current NTSC/PAL mode flag
 * kaze_video_mode.
 */

int kaze_get_video_mode(void)
{
    return kaze_video_mode;
}
