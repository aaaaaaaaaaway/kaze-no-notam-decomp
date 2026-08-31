/**
 * kaze_map_wind_quad_reset  (map_screen, 0x8004f2d4)
 *
 * Resets one wind-overlay quad's vertex positions for the grid cell at the
 * given column and row, in 32px cells, with alternate diagonal layouts when
 * direction mode kaze_map_selected_wind is 2 or 6.
 */


#include "kaze_syms.h"
extern int kaze_map_selected_wind;
extern int kaze_map_screen_prim_buffer;

#define WIND_QUAD_BASE(out)                                \
    do {                                                   \
        register int xofs __asm__("$4");                   \
        register int yofs __asm__("$3");                   \
        register int frame_ofs __asm__("$2");              \
        register int work_base __asm__("$5");              \
                                                           \
        xofs = col << 2;                               \
        xofs += col;                                   \
        xofs <<= 3;                                        \
        yofs = row << 1;                               \
        yofs += row;                                   \
        yofs <<= 3;                                        \
        yofs += row;                                   \
        yofs <<= 4;                                        \
        frame_ofs = frame << 1;                            \
        frame_ofs += frame;                                \
        frame_ofs <<= 2;                                   \
        frame_ofs -= frame;                                \
        frame_ofs <<= 4;                                   \
        frame_ofs -= frame;                                \
        work_base = kaze_map_screen_prim_buffer;                            \
        frame_ofs <<= 4;                                   \
        frame_ofs += work_base;                            \
        yofs += frame_ofs;                                 \
        xofs += yofs;                                      \
        out = xofs;                                        \
    } while (0)

void kaze_map_wind_quad_reset(int frame_arg, int col, int row)
{
    register int frame __asm__("$7");
    register int x __asm__("$11");
    register int x0 __asm__("$8");
    register int y __asm__("$10");
    register int y0 __asm__("$9");
    register int base __asm__("$4");
    register int mode __asm__("$3");
    register int tail_x __asm__("$2");
    register int tail_y __asm__("$3");

    frame = frame_arg;
    __asm__ volatile("" : "=r"(frame) : "0"(frame));
    x = col << 5;
    x0 = x + -0xA0;
    y = row << 5;
    mode = kaze_map_selected_wind;
    y0 = y + -0x78;
    if (mode == 2) {
        goto wind_2;
    }
    if (mode == 6) {
        goto wind_6;
    }
    goto wind_normal;

wind_2:
    WIND_QUAD_BASE(base);
    tail_x = x + -0x80;
    tail_y = y + -0x58;
    *(short *)(base + 0x2D0) = tail_x;
    *(short *)(base + 0x2D8) = tail_x;
    tail_x = x0;
    *(short *)(base + 0x2D2) = y0;
    *(short *)(base + 0x2DA) = tail_y;
    *(short *)(base + 0x2E0) = tail_x;
    *(short *)(base + 0x2E2) = y0;
    goto wind_tail;

wind_6:
    WIND_QUAD_BASE(base);
    {
        register int yplus __asm__("$5");

        yplus = y + -0x58;
        tail_y = y0;
        tail_x = x + -0x80;
        *(short *)(base + 0x2D0) = x0;
        *(short *)(base + 0x2D2) = yplus;
        *(short *)(base + 0x2D8) = x0;
        *(short *)(base + 0x2DA) = tail_y;
        *(short *)(base + 0x2E0) = tail_x;
        *(short *)(base + 0x2E2) = yplus;
    }

wind_tail:
    *(short *)(base + 0x2E8) = tail_x;
    *(short *)(base + 0x2EA) = tail_y;
    return;

wind_normal:
    WIND_QUAD_BASE(base);
    tail_y = x0 + 0x20;
    tail_x = y0 + 0x20;
    *(short *)(base + 0x2D0) = x0;
    *(short *)(base + 0x2D2) = y0;
    *(short *)(base + 0x2D8) = tail_y;
    *(short *)(base + 0x2DA) = y0;
    *(short *)(base + 0x2E0) = x0;
    *(short *)(base + 0x2E2) = tail_x;
    *(short *)(base + 0x2E8) = tail_y;
    *(short *)(base + 0x2EA) = tail_x;
}
