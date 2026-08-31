/**
 * kaze_map_wind_overlay_anim  (map_screen, 0x8004fc04)
 *
 * Animates the 7x10 map wind overlay: 8-direction scroll (dir
 * kaze_map_selected_wind, phase kaze_map_wind_overlay_phase 0..0x1f, cell
 * reset via kaze_map_wind_quad_reset) with rsin(kaze_map_wind_overlay_angle)
 * brightness; AddPrims all 70 quads.
 */

#include "types.h"
#include "kaze_syms.h"
extern int kaze_map_wind_overlay_phase;
extern int kaze_map_wind_overlay_angle;
extern int kaze_map_selected_wind;
extern int kaze_map_screen_prim_buffer;
extern int kaze_map_screen_prim_ot_ptr[];
extern int rsin(int);
extern int AddPrim(int, int);
void kaze_map_wind_overlay_anim(int frame_arg)
{
  register int frame asm("$21") = frame_arg;
  int wind_sin;
  register int shifted asm("$3");
  u8 brightness;
  register int cell_off asm("$4");
  register int col_off asm("$16");
  register int row_screen_off asm("$18");
  register short dy asm("$19");
  register short dx asm("$20");
  register int col_i asm("$22");
  register int row_i asm("$23");
  int row_accum;
  register int row_base asm("$17");
  struct { int p11; int pad; int page_base; } v;
  switch (kaze_map_selected_wind)
  {
    case 0:
      dx = 0;
      dy = -1;
      break;

    case 1:
      dx = 1;
      dy = -1;
      break;

    case 2:
      dx = 1;
      goto LAB_8004fcbc;

    case 3:
      dx = 1;
      dy = 1;
      break;

    case 4:
      dx = 0;
      dy = 1;
      break;

    case 5:
      dx = -1;
      dy = 1;
      break;

    case 6:
      dx = -1;
      goto LAB_8004fcbc;

    case 7:
      dx = -1;
      dy = -1;
      break;

    default:
      dx = 0;
      LAB_8004fcbc:
    dy = 0;

  }

  row_i = 0;
  v.p11 = frame * 11;
  row_accum = 0;
  do
  {
    col_i = 0;
    {
      register int raw asm("$7");
      register int calc asm("$2");
      row_base = row_accum;
      raw = *((volatile int *)&v.p11);
      __asm__("" : "=r"(raw) : "0"(raw));
      col_off = 0;
      calc = raw * 16;
      __asm__("" : "=r"(calc) : "0"(calc));
      calc = calc - frame;
      row_screen_off = calc * 16;
    }
    {
      register int next asm("$7");
      next = row_screen_off + 0x2c8;
      __asm__("" : "=r"(next) : "0"(next));
      v.page_base = next;
    }
    __asm__("" : "=m"(v.page_base) : "m"(v.page_base));
    do
    {
      wind_sin = rsin(kaze_map_wind_overlay_angle);
      shifted = wind_sin >> 6;
      if (wind_sin < 0)
      {
        wind_sin = wind_sin + 0x3f;
        shifted = wind_sin >> 6;
      }
      if (shifted < 0)
      {
        shifted = 0;
      }
      brightness = (u8) shifted;
      *((u8 *) ((col_off + (row_base + (row_screen_off + kaze_map_screen_prim_buffer))) + 0x2cc)) = brightness;
      *((u8 *) ((col_off + (row_base + (row_screen_off + kaze_map_screen_prim_buffer))) + 0x2cd)) = brightness;
      *((u8 *) ((col_off + (row_base + (row_screen_off + kaze_map_screen_prim_buffer))) + 0x2ce)) = brightness;
      if (kaze_map_wind_overlay_phase == 0)
        goto RESET_QUAD;
      {
        register int dst_off asm("$5");
        register int dst_base asm("$3");
        register int parity_bit asm("$3");
        register int parity asm("$2");
        cell_off = col_off + row_base;
        dst_off = cell_off + row_screen_off;
        parity_bit = (frame + 1U) & 1;
        __asm__("" : "=r"(parity_bit) : "0"(parity_bit));
        parity = parity_bit * 0xaf0;
        __asm__("" : "=r"(parity) : "0"(parity));
        cell_off = cell_off + parity;
        dst_base = kaze_map_screen_prim_buffer;
        cell_off = dst_base + cell_off;
        dst_base = dst_base + dst_off;
        ;
        *((short *) (dst_base + 0x2d0)) = (*((short *) (cell_off + 0x2d0))) + dx;
        *((short *) (dst_base + 0x2d2)) = (*((short *) (cell_off + 0x2d2))) + dy;
        *((short *) (dst_base + 0x2d8)) = (*((short *) (cell_off + 0x2d8))) + dx;
        *((short *) (dst_base + 0x2da)) = (*((short *) (cell_off + 0x2da))) + dy;
        *((short *) (dst_base + 0x2e0)) = (*((short *) (cell_off + 0x2e0))) + dx;
        *((short *) (dst_base + 0x2e2)) = (*((short *) (cell_off + 0x2e2))) + dy;
        *((short *) (dst_base + 0x2e8)) = (*((short *) (cell_off + 0x2e8))) + dx;
        *((short *) (dst_base + 0x2ea)) = (*((short *) (cell_off + 0x2ea))) + dy;
      }
      goto AFTER_QUAD;
RESET_QUAD:
      kaze_map_wind_quad_reset(frame, col_i, row_i);
AFTER_QUAD:
      col_i = col_i + 1;
      {
        register int off asm("$2");
        register int prim_arg asm("$4");
        register int prim_base asm("$5");
        register int page_off asm("$7");
        off = frame * 0x14;
        __asm__("" : "=r"(off) : "0"(off));
        prim_base = kaze_map_screen_prim_buffer;
        page_off = v.page_base;
        prim_arg = *(int *)((char *)kaze_map_screen_prim_ot_ptr + off);
        __asm__("" : "=r"(prim_base), "=r"(page_off), "=r"(prim_arg)
                   : "0"(prim_base), "1"(page_off), "2"(prim_arg));
        prim_base = prim_base + page_off;
        prim_base = prim_base + row_base;
        prim_base = prim_base + col_off;
        AddPrim(prim_arg, prim_base);
      }
      __asm__("" : "=r"(frame) : "0"(frame));
      col_off = col_off + 0x28;
    }
    while (col_i < 10);
    row_i = row_i + 1;
    row_accum = row_accum + 400;
  }
  while (row_i < 7);
  {
    register int old_state asm("$3");
    register int current asm("$4");
    register int wrapped asm("$2");
    register int angle asm("$3");
    old_state = kaze_map_wind_overlay_phase;
    __asm__("" : "=r"(old_state) : "0"(old_state));
    current = old_state + 1;
    __asm__("" : "=r"(current) : "0"(current));
    kaze_map_wind_overlay_phase = current;
    wrapped = current;
    if (current < 0)
    {
      wrapped = old_state + 0x20;
    }
    wrapped = (wrapped >> 5) << 5;
    angle = kaze_map_wind_overlay_angle;
    __asm__("" : "=r"(angle) : "0"(angle));
    wrapped = current - wrapped;
    kaze_map_wind_overlay_phase = wrapped;
    __asm__("" : "=m"(kaze_map_wind_overlay_phase) : "m"(kaze_map_wind_overlay_phase));
    angle = (angle + 0x20) & 0xfff;
    kaze_map_wind_overlay_angle = angle;
  }
  __asm__ volatile("" : : "m"(v.pad));
}
