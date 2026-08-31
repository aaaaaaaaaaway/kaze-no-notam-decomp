/**
 * kaze_color_edit_popup_sprites_init  (menu, 0x80059d08)
 *
 * Builds the RGB-edit popup: 0xc0x0x48 semi-trans window SPRT
 * kaze_color_edit_popup_window_sprite, 3 channel-label SPRT_16s at
 * kaze_color_edit_channel_label_sprites (CLUT 0x3a0,0xf2), and a 9x0x25
 * cursor TILE kaze_color_edit_popup_cursor_tiles.
 */

#include "types.h"
#include "kaze_syms.h"
extern u8 kaze_color_edit_popup_window_clut[];
extern u8 kaze_color_edit_channel_label_cluts[];
extern u8 kaze_color_edit_popup_window_sprite[];
extern u8 kaze_color_edit_popup_window_tpage_prims[];
extern u8 kaze_color_edit_channel_label_sprites[];
extern u8 kaze_color_edit_popup_cursor_tiles[];

extern void SetSprt(int);
extern void SetSprt16(int);
extern void SetShadeTex(int, int);
extern void SetSemiTrans(int, int);
extern u16 GetClut(int, int);
extern u16 GetTPage(int, int, int, int);
extern void SetDrawTPage(u8 *, int, int, int);
extern void SetTile(int);

void kaze_color_edit_popup_sprites_init(void)
{
    register int p  asm("$16");
    register int s1 asm("$17");
    register int s2 asm("$18");
    register int s3 asm("$19");
    register int s4 asm("$20");
    int s6;
    int s7;
    u16 clut;
    int i;
    int base;

    i = 0;
    base = (int)kaze_color_edit_channel_label_sprites;
    do {
        register int c80o asm("$8");
        register int v48o asm("$2");
        int cx, cy;
        s7 = i * 0x14;
        p = s7 + (int)kaze_color_edit_popup_window_sprite;
        SetSprt(p);
        SetShadeTex(p, 1);
        SetSemiTrans(p, 1);
        __asm__ volatile("addiu %0,$0,0x370" : "=r"(cx));
        __asm__ volatile("addiu %0,$0,0xF2" : "=r"(cy));
        *(short *)(p + 8) = -0x60;
        *(short *)(p + 0xA) = -0x24;
        *(short *)(p + 0x10) = 0xC0;
        __asm__ volatile("addiu %0,$0,0x48" : "=r"(v48o));
        __asm__ volatile("addiu %0,$0,0x80" : "=r"(c80o));
        *(short *)(p + 0x12) = v48o;
        *(u8 *)(p + 0xC) = 0;
        *(u8 *)(p + 0xD) = c80o;
        clut = GetClut(cx, cy);
        *(u16 *)(kaze_color_edit_popup_window_clut + s7) = clut;
        clut = GetTPage(0, 0, 0x340, 0x100);
        SetDrawTPage(kaze_color_edit_popup_window_tpage_prims + i * 8, 0, 1, clut);
        s4 = 0;
        s3 = -0x1B;
        s6 = i * 0x30;
        s1 = s6;
        s2 = base;
        do {
            register int v1 asm("$3");
            register int c80 asm("$8");
            register int vE0 asm("$2");
            int cx2, cy2;
            p = s6 + s2;
            SetSprt16(p);
            SetShadeTex(p, 1);
            __asm__ volatile("addiu %0,$0,0x3A0" : "=r"(cx2));
            __asm__ volatile("addiu %0,$0,0xF2" : "=r"(cy2));
            v1 = s1 + base;
            *(short *)(v1 + 8) = -0x4C;
            __asm__ volatile("addiu %0,$0,0xE0" : "=r"(vE0));
            __asm__ volatile("addiu %0,$0,0x80" : "=r"(c80));
            *(short *)(v1 + 0xA) = s3;
            *(u8 *)(v1 + 0xC) = vE0;
            *(u8 *)(v1 + 0xD) = c80;
            clut = GetClut(cx2, cy2);
            s3 += 0x12;
            *(u16 *)(kaze_color_edit_channel_label_cluts + s1) = clut;
            s1 += 0x10;
            s4 += 1;
            s2 += 0x10;
        } while (s4 < 3);
        p = i * 0x10;
        p += (int)kaze_color_edit_popup_cursor_tiles;
        SetTile(p);
        i += 1;
        *(short *)(p + 8) = 0x49;
        *(short *)(p + 0xA) = -0x12;
        *(short *)(p + 0xC) = 9;
        *(short *)(p + 0xE) = 0x25;
    } while (i < 2);
}
