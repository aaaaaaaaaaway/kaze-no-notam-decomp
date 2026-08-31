#include "types.h"
#include "kaze_syms.h"
extern u8 kaze_map_bg_tile_present_table[];
extern int kaze_map_target_marker_pulse_phase;
extern int kaze_map_scroll_arrow_pulse;
extern int kaze_map_location_valid;
typedef struct {
    int ot;
    int pad[4];
} MapPanelSlot;
typedef struct {
    int prim;
    int pad[4];
} FrameSlot;
typedef struct { void *ptr; int pad[4]; } Slot;
typedef struct {
    void *ob;
    char pad[16];
} Slot20;

extern void SetSprt(void *);
extern void SetShadeTex(void *, int);
extern u16 GetClut(int, int, void *);
extern u16 GetTPage(int, int, int, int);
extern void SetDrawTPage(void *, int, int, unsigned int);

/**
 * kaze_map_tile_prim_init  (map_screen, 0x8004f478)
 *
 * Inits map background tile SPRTs per buffer from the 8x10 presence table
 * kaze_map_bg_tile_present_table (32x32 tiles from origin (-0xa0,-0x78),
 * clut(0x140,0x1f2)) plus DR_TPAGE at +0x1ee8.
 */

void kaze_map_tile_prim_init(void)
{
    extern int kaze_map_screen_prim_buffer;
  int pass;
  int a2base;
  int scan_ofs;
  int tpage_ofs;
  int prim_base;
  register int idx asm("$19");
  register int row asm("$23");
  int row_ofs;
  register int col asm("$18");
  register int xpos asm("$21");
  register int prim_ofs asm("$17");
  register int clut_ofs asm("$20");
  register int pb asm("$22");
  int sprt_off;
  char *v1;
  register char *a2 asm("$6");
  register int v1i asm("$3");
  char *clutp;
  u16 tpage;
  u16 clut_result;
  register int t asm("$2");
  pass = 0;
  tpage_ofs = 0x1EE8;
  prim_base = 0;
  do
  {
    idx = 0;
    row = 0;
    row_ofs = 0;
    do
    {
      col = 0;
      xpos = -0xA0;
      t = idx * 0x14;
      pb = prim_base;
      clut_ofs = t + pb;
      prim_ofs = t;
      scan_ofs = row_ofs;
      do
      {
        if ((*((u8 *) ((scan_ofs + col) + ((int) kaze_map_bg_tile_present_table)))) != 0)
        {
          sprt_off = pb + 0x18A8;
          idx += 1;
          SetSprt((char *) ((kaze_map_screen_prim_buffer + sprt_off) + prim_ofs));
          SetShadeTex((char *) ((kaze_map_screen_prim_buffer + sprt_off) + prim_ofs), 1);
          v1i = kaze_map_screen_prim_buffer;
          v1i = pb + v1i;
          v1i = prim_ofs + v1i;
          v1 = (char *) v1i;
          *((char *) (v1 + 0x18B4)) = 0;
          a2base = prim_ofs + (pb + kaze_map_screen_prim_buffer);
          *((short *) (v1 + 0x18B2)) = (short) ((row << 5) - 0x78);
          *((short *) (v1 + 0x18B8)) = 0x20;
          *((short *) (v1 + 0x18BA)) = 0x20;
          *((short *) (v1 + 0x18B0)) = xpos;
          a2 = (char *) a2base;
          *((char *) (a2 + 0x18B5)) = 0x60;
          clut_result = GetClut(0x140, 0x1F2, a2);
          prim_ofs += 0x14;
          clutp = (char *) (kaze_map_screen_prim_buffer + clut_ofs);
          clut_ofs += 0x14;
          *((u16 *) (clutp + 0x18B6)) = clut_result;
        }
        col += 1;
        xpos += 0x20;
      }
      while (col < 0xA);
      row += 1;
      row_ofs += 0xA;
    }
    while (row < 8);
    tpage = GetTPage(1, 0, 0x2C0, 0x100);
    prim_base += 0x320;
    pass += 1;
    SetDrawTPage((void *) (kaze_map_screen_prim_buffer + tpage_ofs), 0, 1, tpage & 0xFFFF);
    tpage_ofs += 8;
  }
  while (pass < 2);
}

/**
 * kaze_map_panel_draw  (map_screen, 0x8004f670)
 *
 * AddPrims the map panel SPRT (+buf*0x14) and its DR_TPAGE (+0x28) into the
 * map-screen OT (&kaze_map_screen_prim_ot_ptr)[buf*5].
 */

void kaze_map_panel_draw(int buf)
{
    extern void AddPrim(int, char *);
    extern char *kaze_map_screen_prim_buffer;
    extern MapPanelSlot kaze_map_screen_prim_ot_ptr[];
    int off;

    AddPrim(kaze_map_screen_prim_ot_ptr[buf].ot, kaze_map_screen_prim_buffer + buf * 0x14);
    off = buf * 8 + 0x28;
    AddPrim(kaze_map_screen_prim_ot_ptr[buf].ot, kaze_map_screen_prim_buffer + off);
    return;
}

/**
 * kaze_map_frame_lines_draw  (map_screen, 0x8004f6e8)
 *
 * AddPrims the two LINE_F3 map frame lines (+0x38/+0x68 of
 * kaze_map_screen_prim_buffer) into the map-screen OT.
 */

void kaze_map_frame_lines_draw(int idx)
{
    extern int AddPrim(void *, void *);
    extern char *kaze_map_screen_prim_buffer;
    extern FrameSlot kaze_map_screen_prim_ot_ptr[];
    AddPrim((void *)kaze_map_screen_prim_ot_ptr[idx].prim, kaze_map_screen_prim_buffer + (idx * 0x18 + 0x38));
    AddPrim((void *)kaze_map_screen_prim_ot_ptr[idx].prim, kaze_map_screen_prim_buffer + (idx * 0x18 + 0x68));
}

extern int kaze_map_cursor_screen_x;
extern int kaze_map_cursor_screen_y;

/**
 * kaze_map_cursor_draw  (map_screen, 0x8004f76c)
 *
 * Draws the map cursor: picks the valid/invalid SPRT_16 via
 * kaze_map_location_valid, positions it at raw cursor
 * (kaze_map_cursor_screen_x-0xa0, kaze_map_cursor_screen_y-0x78), AddPrims
 * it plus its TPAGE.
 */

void kaze_map_cursor_draw(int idx)
{
    extern void AddPrim(void *, void *);
    extern Slot kaze_map_screen_prim_ot_ptr[];
    extern int kaze_map_screen_prim_buffer;
    s16 sy; int i2, i3, i4, j, m;
    i3 = ((kaze_map_location_valid + 1U) & 1) * 0x10;
    m = idx * 0x20;
    i2 = i3 + (m + kaze_map_screen_prim_buffer);
    sy = kaze_map_cursor_screen_y;
    m += 0x98;
    i4 = kaze_map_screen_prim_buffer + m;
    *(s16 *)(i2 + 0xa0) = kaze_map_cursor_screen_x - 0xa0;
    *(s16 *)(i2 + 0xa2) = sy - 0x78;
    AddPrim(kaze_map_screen_prim_ot_ptr[idx].ptr, (void *)(i4 + i3));
    j = idx * 8 + 0xd8;
    AddPrim(kaze_map_screen_prim_ot_ptr[idx].ptr, (void *)(kaze_map_screen_prim_buffer + j));
}

extern int rsin(int);

/**
 * kaze_map_target_marker_draw  (map_screen, 0x8004f82c)
 *
 * Draws the pulsing 8x8 target marker at the even-rounded cursor position
 * with green brightness from rsin(kaze_map_target_marker_pulse_phase);
 * advances the pulse phase by 0x30 mod 0xfff.
 */

void kaze_map_target_marker_draw(int idx)
{
    extern Slot kaze_map_screen_prim_ot_ptr[];
    extern int kaze_map_screen_prim_buffer;
    extern void AddPrim(void *, void *);
    register int raw_x asm("$3");
    register int x asm("$6");
    register int y asm("$5");
    register int phase asm("$4");
    register int bright asm("$3");
    int raw;
    register int slot16 asm("$17");
    int ot_ofs;
    register int prim1_ofs asm("$5");
    register int prim2_ofs asm("$5");

    raw_x = kaze_map_cursor_screen_x;
    if (raw_x & 1) {
        x = raw_x - 4;
    } else {
        x = raw_x - 3;
    }
    y = kaze_map_cursor_screen_y - 4;
    if ((kaze_map_cursor_screen_y & 1) == 0) {
        y = kaze_map_cursor_screen_y - 3;
    }

    slot16 = idx << 4;
    {
        register int coord_ptr asm("$2");
        register int ycoord asm("$3");

        __asm__ __volatile__(
            ".set\tnoreorder\n\t"
            "addiu\t%1,%4,-160\n\t"
            "lw\t%0,kaze_map_screen_prim_buffer\n\t"
            "lw\t%2,kaze_map_target_marker_pulse_phase\n\t"
            "addu\t%0,%3,%0\n\t"
            "sh\t%1,240(%0)\n\t"
            "addiu\t%1,%5,-120\n\t"
            ".set\treorder"
            : "=r"(coord_ptr), "=r"(ycoord), "=r"(phase)
            : "r"(slot16), "r"(x), "r"(y)
            : "memory");
        *(s16 *)(coord_ptr + 0xF2) = ycoord;
    }

    raw = rsin(phase);
    __asm__ __volatile__(
        ".set\tnoreorder\n\t"
        "bgez\t%2,1f\n\t"
        "nop\n\t"
        "addiu\t%2,%2,0xf\n"
        "1:\n\t"
        "sra\t%2,%2,4\n\t"
        "bgez\t%2,2f\n\t"
        "addu\t%0,%2,$zero\n\t"
        "negu\t%0,%0\n"
        "2:\n\t"
        "slti\t%2,%0,0x100\n\t"
        "bnez\t%2,3f\n\t"
        "sll\t%1,%4,2\n\t"
        "addiu\t%0,$zero,0xff\n"
        "3:\n\t"
        ".set\treorder"
        : "=&r"(bright), "=&r"(ot_ofs), "=r"(raw)
        : "2"(raw), "r"(idx));

    __asm__ __volatile__(
        ".set\tnoreorder\n\t"
        "lw\t$2,kaze_map_screen_prim_buffer\n\t"
        "addu\t%0,%0,%3\n\t"
        "addu\t$2,%2,$2\n\t"
        "sb\t$0,0xec($2)\n\t"
        "lw\t$2,kaze_map_screen_prim_buffer\n\t"
        "sll\t%0,%0,2\n\t"
        "addu\t$2,%2,$2\n\t"
        "sb\t%4,0xed($2)\n\t"
        "lw\t$2,kaze_map_screen_prim_buffer\n\t"
        "addiu\t%1,%2,0xe8\n\t"
        "addu\t$2,%2,$2\n\t"
        "sb\t$0,0xee($2)\n\t"
        ".set\treorder"
        : "=r"(ot_ofs), "=r"(prim1_ofs)
        : "r"(slot16), "r"(idx), "r"(bright), "0"(ot_ofs)
        : "$2", "memory");

    AddPrim(*(void **)((int)kaze_map_screen_prim_ot_ptr + ot_ofs), (void *)(kaze_map_screen_prim_buffer + prim1_ofs));
    prim2_ofs = (idx << 3) + 0x108;
    AddPrim(*(void **)((int)kaze_map_screen_prim_ot_ptr + ot_ofs), (void *)(kaze_map_screen_prim_buffer + prim2_ofs));
    kaze_map_target_marker_pulse_phase = (kaze_map_target_marker_pulse_phase + 0x30) % 0xFFF;
}

/**
 * kaze_map_scroll_arrow_draw  (map_screen, 0x8004f9ac)
 *
 * Draws the blinking scroll arrows: up icon when the map panel v0 != 0, down
 * icon when v0 != 0x60, red channel pulsed via kaze_map_scroll_arrow_pulse
 * (+8 mod 0x100), plus TPAGE.
 */

void kaze_map_scroll_arrow_draw(int idx)
{
    extern void AddPrim(void *, void *);
    extern int kaze_map_screen_prim_ot_ptr;
    extern u8 *kaze_map_screen_prim_buffer;
    int ofs;
    int t4;

    t4 = idx << 2;
    ofs = (t4 + idx) << 2;
    if (*(kaze_map_screen_prim_buffer + ofs + 0xd) != 0) {
        int t = idx * 0x20;
        *(u8 *)(t + (int)kaze_map_screen_prim_buffer + 0x11c) = kaze_map_scroll_arrow_pulse;
        *(u8 *)(t + (int)kaze_map_screen_prim_buffer + 0x11d) = 0;
        *(u8 *)(t + (int)kaze_map_screen_prim_buffer + 0x11e) = 0;
        AddPrim(*(void **)((int)&kaze_map_screen_prim_ot_ptr + ofs), kaze_map_screen_prim_buffer + (t + 0x118));
    }
    if (*(kaze_map_screen_prim_buffer + ofs + 0xd) != 0x60) {
        int t = idx * 0x20;
        *(u8 *)(t + (int)kaze_map_screen_prim_buffer + 0x12c) = kaze_map_scroll_arrow_pulse;
        *(u8 *)(t + (int)kaze_map_screen_prim_buffer + 0x12d) = 0;
        *(u8 *)(t + (int)kaze_map_screen_prim_buffer + 0x12e) = 0;
        AddPrim(*(void **)((int)&kaze_map_screen_prim_ot_ptr + ofs), (void *)(t + (int)kaze_map_screen_prim_buffer + 0x128));
    }
    AddPrim(*(void **)((int)&kaze_map_screen_prim_ot_ptr + ofs), kaze_map_screen_prim_buffer + (idx * 8 + 0x158));
    kaze_map_scroll_arrow_pulse = kaze_map_scroll_arrow_pulse + 8 & 0xff;
}

/**
 * kaze_map_banner_draw  (map_screen, 0x8004fb18)
 *
 * AddPrims the selected message banner SPRT (+0x168 + the given banner
 * index*0x14, index from kaze_map_feedback_state), its DR_TPAGE, and the two
 * banner LINE_F3 frames.
 */

void kaze_map_banner_draw(int idx, int banner_index)
{
    extern int kaze_map_screen_prim_buffer;
    extern Slot20 kaze_map_screen_prim_ot_ptr[];
    extern void AddPrim(void *ob, void *prim);
    AddPrim(kaze_map_screen_prim_ot_ptr[idx].ob, (void *)((char *)kaze_map_screen_prim_buffer + (idx * 0x78 + 0x168) + banner_index * 0x14));
    AddPrim(kaze_map_screen_prim_ot_ptr[idx].ob, (void *)((char *)kaze_map_screen_prim_buffer + (idx * 8 + 0x258)));
    AddPrim(kaze_map_screen_prim_ot_ptr[idx].ob, (void *)((char *)kaze_map_screen_prim_buffer + (idx * 0x18 + 0x268)));
    AddPrim(kaze_map_screen_prim_ot_ptr[idx].ob, (void *)((char *)kaze_map_screen_prim_buffer + (idx * 0x18 + 0x298)));
}
