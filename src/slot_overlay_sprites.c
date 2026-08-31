#include "types.h"
#include "kaze_syms.h"
extern u16 kaze_slot_overlay_glyph_cluts;
extern char kaze_slot_overlay_row_icon_cluts[];
extern char kaze_slot_overlay_row_highlight_cluts[];

extern u8 kaze_slot_overlay_glyph_sprites[];
extern u8 kaze_slot_overlay_glyph_tpages[];
extern void SetSprt(int);
extern void SetShadeTex(int, int);

/**
 * kaze_slot_overlay_glyph_sprites_init  (menu, 0x80061f78)
 *
 * Initializes 2x2 double-buffered glyph SPRTs at
 * kaze_slot_overlay_glyph_sprites (16px tall strips, CLUT 0x380, v stepping
 * by 0x10) plus DR_TPAGE prims at kaze_slot_overlay_glyph_tpages for the
 * slot-select overlay.
 */

void kaze_slot_overlay_glyph_sprites_init(void)
{
    extern void SetDrawTPage(u8 *, int, int, int);
    extern u16 GetTPage(int, int, int, int);
    extern u16 GetClut(int, int);
    u16 clut;
    int o2;
    int p;
    int q;
    int base;
    int v;
    int c;
    int j;
    int off;
    int i;

    i = 0;
    base = (int)kaze_slot_overlay_glyph_sprites;
    off = 0;
    do {
        j = 0;
        c = 0x60;
        v = -0x18;
        o2 = off;
        p = base;
        do {
            SetSprt(off + p);
            SetShadeTex(off + p, 1);
            q = o2 + base;
            *(short *)(q + 8) = -0x40;
            *(short *)(q + 0x10) = 0x80;
            *(short *)(q + 0xA) = v;
            *(short *)(q + 0x12) = 0x10;
            *(u8 *)(q + 0xC) = 0;
            *(u8 *)(q + 0xD) = c;
            clut = GetClut(0x380, 0xF4);
            c += 0x10;
            v += 0x20;
            *(u16 *)((char *)&kaze_slot_overlay_glyph_cluts + o2) = clut;
            o2 += 0x14;
            j += 1;
            p += 0x14;
        } while (j < 2);
        clut = GetTPage(0, 0, 0x240, 0x100);
        SetDrawTPage(kaze_slot_overlay_glyph_tpages + i * 8, 0, 1, clut);
        i += 1;
        off += 0x28;
    } while (i < 2);
}

extern char kaze_slot_overlay_row_icon_sprites[];
extern char kaze_slot_overlay_row_highlight_sprites[];
extern char kaze_slot_overlay_row_tpage_prims[];

/**
 * kaze_slot_overlay_row_sprites_init  (menu, 0x800620b4)
 *
 * Initializes the two selectable row sprite pairs at
 * kaze_slot_overlay_row_icon_sprites/kaze_slot_overlay_row_highlight_sprites
 * (CLUTs 0x360/0x350, x from -0x60 stepping 0x80) plus DR_TPAGE at
 * kaze_slot_overlay_row_tpage_prims for the slot-select overlay.
 */

void kaze_slot_overlay_row_sprites_init(void)
{
    extern void SetDrawTPage(void *, int, int, int);
    extern int GetTPage(int, int, int, int);
    extern int GetClut(int, int);
    register int o2 asm("$17");
    register int j asm("$18");
    register int y asm("$19");
    register int row asm("$20");
    register int row2 asm("$21");
    register int off asm("$22");
    int i;
    int size;
    int clut;

    i = 0;
    size = 0x40;
    off = 0;
    do {
        j = 0;
        row2 = (int)kaze_slot_overlay_row_highlight_sprites;
        y = -0x60;
        o2 = off;
        row = (int)kaze_slot_overlay_row_icon_sprites;
        do {
            register int sym asm("$8");
            register char *q asm("$3");
            register int c80 asm("$2");
            int cx, cy;

            SetSprt(off + row);
            SetShadeTex(off + row, 1);
            __asm__("addiu %0,$0,0x360" : "=r"(cx));
            __asm__("addiu %0,$0,0xF4" : "=r"(cy));
            sym = (int)kaze_slot_overlay_row_icon_sprites;
            q = (char *)(o2 + sym);
            *(short *)(q + 0xA) = -0x28;
            *(short *)(q + 0x12) = 0x10;
            *(char *)(q + 0xC) = 0xC0;
            *(short *)(q + 0x8) = y;
            *(short *)(q + 0x10) = size;
            *(char *)(q + 0xD) = j << 4;
            clut = GetClut(cx, cy);
            *(short *)(kaze_slot_overlay_row_icon_cluts + o2) = clut;

            SetSprt(off + row2);
            SetShadeTex(off + row2, 0);
            __asm__("addiu %0,$0,0x350" : "=r"(cx));
            __asm__("addiu %0,$0,0xF4" : "=r"(cy));
            sym = (int)kaze_slot_overlay_row_highlight_sprites;
            q = (char *)(o2 + sym);
            *(short *)(q + 0xA) = -0x10;
            *(char *)(q + 0xC) = (j << 6) + 0x40;
            __asm__ volatile("addiu %0,$0,0x80" : "=r"(c80));
            *(short *)(q + 0x8) = y;
            *(short *)(q + 0x10) = size;
            *(short *)(q + 0x12) = size;
            *(char *)(q + 0xD) = 0;
            *(char *)(q + 0x4) = c80;
            *(char *)(q + 0x5) = c80;
            *(char *)(q + 0x6) = c80;
            clut = GetClut(cx, cy);
            row2 += 0x14;
            y += 0x80;
            *(short *)(kaze_slot_overlay_row_highlight_cluts + o2) = clut;
            o2 += 0x14;
            j += 1;
            row += 0x14;
        } while (j < 2);
        clut = GetTPage(0, 0, 0x240, 0x100);
        SetDrawTPage(kaze_slot_overlay_row_tpage_prims + i * 8, 0, 1, clut & 0xFFFF);
        i += 1;
        off += 0x28;
    } while (i < 2);
}
