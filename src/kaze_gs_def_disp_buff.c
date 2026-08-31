/**
 * kaze_gs_def_disp_buff  (library, 0x8006d0b0)
 *
 * PsyQ GsDefDispBuff: stores the double-buffer display/draw origins into
 * kaze_gs_draw_buff_clip_x0.. and kaze_gs_disp_buff_x0/0eaa/0f44/0f46
 * (zeroed in interlace mode kaze_gs_interlace_flag), then
 * GsSetDrawBuffClip/GsSetDrawBuffOffset.
 */


#include "kaze_syms.h"
extern short kaze_gs_disp_buff_x0;
extern short kaze_gs_disp_buff_x1;
extern short kaze_gs_disp_buff_y0;
extern short kaze_gs_disp_buff_y1;
extern short kaze_gs_draw_buff_clip_x0;
extern short kaze_gs_draw_buff_clip_x1;
extern short kaze_gs_draw_buff_clip_y0;
extern short kaze_gs_draw_buff_clip_y1;
extern short kaze_gs_interlace_flag;
extern void GsSetDrawBuffClip(void);
extern void GsSetDrawBuffOffset(void);
void kaze_gs_def_disp_buff(short x0, short y0, short x1, short y1)
{
    kaze_gs_disp_buff_x0 = x0;
    kaze_gs_disp_buff_x1 = x1;
    kaze_gs_disp_buff_y0 = y0;
    kaze_gs_disp_buff_y1 = y1;
    if (kaze_gs_interlace_flag != 0) {
        kaze_gs_draw_buff_clip_x0 = 0;
        kaze_gs_draw_buff_clip_x1 = 0;
        kaze_gs_draw_buff_clip_y0 = 0;
        kaze_gs_draw_buff_clip_y1 = 0;
    } else {
        kaze_gs_draw_buff_clip_x0 = x0;
        kaze_gs_draw_buff_clip_x1 = x1;
        kaze_gs_draw_buff_clip_y0 = y0;
        kaze_gs_draw_buff_clip_y1 = y1;
    }
    GsSetDrawBuffClip();
    GsSetDrawBuffOffset();
}
