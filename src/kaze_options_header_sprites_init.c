/**
 * kaze_options_header_sprites_init  (menu, 0x80056840)
 *
 * Builds 4 header banner sprite pairs per buffer at
 * kaze_options_header_left_sprites/kaze_options_header_right_sprites with
 * CLUT coords from ROM tables
 * kaze_header_sprite_clut_x_table/kaze_header_sprite_clut_y_table, tpage
 * (0x380,0x100).
 */

#include "types.h"
#include "kaze_syms.h"
extern char kaze_options_header_left_sprites[];
extern char kaze_options_header_left_sprites_clut[];
extern char kaze_options_header_right_sprites[];
extern char kaze_options_header_right_sprites_clut[];
extern char kaze_options_header_tpage_prims[];
struct S4 { int a, b, c, d; };

extern struct S4 kaze_header_sprite_clut_x_table;
extern struct S4 kaze_header_sprite_clut_y_table;

extern void SetSprt(int);
extern void SetShadeTex(int, int);
extern int GetClut(int, int);
extern int GetTPage(int, int, int, int);
extern void SetDrawTPage(void *, int, int, int);

void kaze_options_header_sprites_init(void)
{
    register int s0 asm("$16");
    register int s1 asm("$17");
    register int s2 asm("$18");
    register int s3 asm("$19");
    register int s4 asm("$20");
    register int s5 asm("$21");
    register int s6 asm("$22");
    int s7;
    struct S4 tbl[2];
    int base10;
    int clut;

    tbl[0] = kaze_header_sprite_clut_x_table;
    tbl[1] = kaze_header_sprite_clut_y_table;
    s7 = 0;
    base10 = (int)kaze_options_header_right_sprites;
    s6 = 0;
    do {
        s2 = 0;
        s5 = -0x80;
        s4 = base10;
        s1 = s6;
        s3 = (int)kaze_options_header_left_sprites;
        do {
            register int sym asm("$8");
            register int q asm("$3");
            int a0, a1;

            s0 = s6 + s3;
            SetSprt(s0);
            SetShadeTex(s0, 1);
            s0 = s6 + s4;
            s4 += 0x14;
            s3 += 0x14;
            sym = (int)kaze_options_header_left_sprites;
            q = s1 + sym;
            *(s16 *)(q + 8) = -0xA0;
            *(s16 *)(q + 0xA) = -0x78;
            *(s16 *)(q + 0x10) = 0x40;
            *(s16 *)(q + 0x12) = 0x40;
            *(u8 *)(q + 0xC) = s2 << 6;
            *(u8 *)(q + 0xD) = 0;
            a0 = ((int *)tbl)[s2];
            a1 = ((int *)tbl)[s2 + 4];
            s2 += 1;
            clut = GetClut(a0, a1);
            *(s16 *)(kaze_options_header_left_sprites_clut + s1) = clut;
            SetSprt(s0);
            SetShadeTex(s0, 1);
            *(s16 *)(s1 + base10 + 8) = -0x60;
            *(s16 *)(s1 + base10 + 0xA) = -0x58;
            *(s16 *)(s1 + base10 + 0x10) = 0x80;
            *(s16 *)(s1 + base10 + 0x12) = 0x20;
            *(u8 *)(s1 + base10 + 0xC) = 0;
            *(u8 *)(s1 + base10 + 0xD) = s5;
            clut = GetClut(0x360, 0xF3);
            s5 += 0x20;
            *(s16 *)(kaze_options_header_right_sprites_clut + s1) = clut;
            s1 += 0x14;
        } while (s2 < 4);
        clut = GetTPage(0, 0, 0x380, 0x100);
        SetDrawTPage(kaze_options_header_tpage_prims + s7 * 8, 0, 1, clut & 0xFFFF);
        s7 += 1;
        s6 += 0x50;
    } while (s7 < 2);
}
