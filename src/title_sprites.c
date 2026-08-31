#include "types.h"
#include "kaze_syms.h"
extern char kaze_setup_menu_preview_tpage[];
extern char kaze_setup_menu_caption_tpage[];
extern char kaze_setup_menu_textline_sprites[];
extern char kaze_setup_menu_textline_tpage[];
extern char kaze_setup_menu_textline_underline_a[];
extern char kaze_setup_menu_textline_underline_b[];
extern char kaze_setup_menu_level_row_clut[];
extern char kaze_setup_menu_flags_row_clut[];
extern char kaze_title_weather_rows_clut[];
extern char kaze_title_bg_sprt_prims[];
extern char kaze_title_bg_left_clut[];
extern char kaze_title_bg_right_clut[];
extern char kaze_title_bg_tpage_prims[];
extern char kaze_setup_menu_level_row_sprites[];
extern char kaze_setup_menu_flags_row_sprites[];
extern char kaze_setup_menu_weather_row_sprites[];
typedef struct {
    char pad0[8];
    short x;
    short y;
    char pad1[8];
} Prim20;
typedef struct {
    void *ob;
    char pad[16];
} Slot20;

extern char kaze_title_rows_tpage_prims[];
extern void SetSprt(void *);
extern void SetShadeTex(void *, int);
extern u16 GetClut(int, int);
extern u16 GetTPage(int, int, int, int);

/**
 * kaze_title_menu_row_sprites_init  (menu, 0x80053798)
 *
 * Builds double-buffered title-menu SPRT rows (3/4/10 entries) at kaze_setup
 * _menu_level_row_sprites/kaze_setup_menu_flags_row_sprites/kaze_setup_menu_
 * weather_row_sprites with CLUTs from (0x350,0xf1) plus a DR_TPAGE per
 * buffer.
 */

void kaze_title_menu_row_sprites_init(void)
{
    extern void SetDrawTPage(void *, int, int, unsigned int);
    u16 clut;
    int row_off;
    int sprt_off;
    int i;
    signed char shade;
    signed char shade2;
    int b2;
    int b3;
    int b1v;
    int buf;
    int base3;
    int base2;
    int base1;

    b1v = (int)kaze_setup_menu_level_row_sprites;
    buf = 0;
    base3 = 0;
    base2 = 0;
    base1 = 0;
    do {
        i = 0;
        shade = 0x70;
        row_off = base1;
        sprt_off = (int)kaze_setup_menu_level_row_sprites;
        do {
            char *p;
            SetSprt((void *)(base1 + sprt_off));
            SetShadeTex((void *)(base1 + sprt_off), 1);
            p = (char *)(row_off + b1v);
            *(s16 *)(p + 8) = -0x40;
            *(s16 *)(p + 0xa) = -0x18;
            *(s16 *)(p + 0x10) = 0x80;
            *(s16 *)(p + 0x12) = 0x10;
            p[0xc] = 0;
            p[0xd] = shade;
            clut = GetClut(0x350, 0xf1);
            shade = shade + 0x10;
            *(u16 *)(kaze_setup_menu_level_row_clut + row_off) = clut;
            row_off = row_off + 0x14;
            sprt_off = sprt_off + 0x14;
            i = i + 1;
            p[4] = 0x80;
            p[5] = 0x80;
            p[6] = 0x80;
        } while (i < 3);
        i = 0;
        {char sh2;
        int calloff2;
        __asm__ volatile ("":"=r"(sh2):"0"((char)0x40));
        shade = -0x60;
        b2 = (int)kaze_setup_menu_flags_row_sprites;
        __asm__ volatile ("");
        __asm__ volatile ("");
        __asm__ volatile ("");
        __asm__ volatile ("");
        __asm__ volatile ("");
        __asm__ volatile ("");
        __asm__ volatile ("");
        __asm__ volatile ("");
        sprt_off = b2;
        calloff2 = base2;
        row_off = base2;
        do {
            char *p;
            SetSprt((void *)(calloff2 + sprt_off));
            SetShadeTex((void *)(calloff2 + sprt_off), 1);
            p = (char *)(row_off + b2);
            *(s16 *)(p + 8) = -0x40;
            *(s16 *)(p + 0xa) = 8;
            *(s16 *)(p + 0x10) = 0x80;
            *(s16 *)(p + 0x12) = 0x10;
            p[0xc] = 0;
            p[0xd] = shade;
            clut = GetClut(0x350, 0xf1);
            shade = shade + 0x10;
            *(u16 *)(kaze_setup_menu_flags_row_clut + row_off) = clut;
            row_off = row_off + 0x14;
            sprt_off = sprt_off + 0x14;
            i = i + 1;
            p[4] = sh2;
            p[5] = sh2;
            p[6] = sh2;
        } while (i < 4);}
        i = 0;
        shade2 = 0x50;
        shade = -0x20;
        {int calloff3;
        b3 = (int)kaze_setup_menu_weather_row_sprites;
        calloff3 = base3;
        sprt_off = b3;
        row_off = base3;
        do {
            char *p;
            SetSprt((void *)(calloff3 + sprt_off));
            SetShadeTex((void *)(calloff3 + sprt_off), 1);
            p = (char *)(row_off + b3);
            *(s16 *)(p + 8) = -0x40;
            *(s16 *)(p + 0xa) = 0x28;
            *(s16 *)(p + 0x12) = 0x10;
            *(s16 *)(p + 0x10) = 0x80;
            if (i < 2) {
                p[0xc] = 0;
                p[0xd] = shade;
            } else {
                p[0xc] = 0x80;
                p[0xd] = shade2;
            }
            clut = GetClut(0x350, 0xf1);
            *(u16 *)(kaze_title_weather_rows_clut + row_off) = clut;
            __asm__("":"=r"(row_off):"0"(row_off));
            {register char *q asm("$2");
            q = (char *)(row_off + b3);
            row_off = row_off + 0x14;
            shade2 = shade2 + 0x10;
            shade = shade + 0x10;
            i = i + 1;
            {char sh3;
            __asm__ volatile ("":"=r"(sh3):"0"((char)0x40));
            q[4] = sh3;
            q[5] = sh3;
            q[6] = sh3;}}
            sprt_off = sprt_off + 0x14;
        } while (i < 10);}
        __asm__ volatile ("" :: "r"(i), "r"(i));
        clut = GetTPage(0, 0, 0x1c0, 0x100);
        SetDrawTPage(kaze_title_rows_tpage_prims + buf * 8, 0, 1, (unsigned int)clut);
        base3 = base3 + 200;
        base2 = base2 + 0x50;
        base1 = base1 + 0x3c;
        buf = buf + 1;
    } while (buf < 2);
}

extern char kaze_title_icon_row1_sprites[];
extern char kaze_title_icon_row2_sprites[];
extern char kaze_title_icon_rows_tpage_prims[];
extern char kaze_setup_row1_flash_level[];
extern char kaze_setup_row2_flash_level[];
extern void SetSprt16(void *);

/**
 * kaze_title_icon_sprites_init  (menu, 0x80053a84)
 *
 * Builds two rows of 3 SPRT_16 icons per draw buffer at
 * kaze_title_icon_row1_sprites/kaze_title_icon_row2_sprites (CLUT
 * 0x3e0,0xf1, tpage 0x200) and resets their brightness bytes
 * kaze_setup_row1_flash_level/kaze_setup_row2_flash_level.
 */

void kaze_title_icon_sprites_init(void)
{
    extern void SetDrawTPage(void *, int, int, int);
    register int draw __asm__("$23");
    register int draw_ofs __asm__("$22");
    register int shade __asm__("$21");
    register int i __asm__("$17");
    register int y __asm__("$20");
    register int ofs __asm__("$18");
    register char *base __asm__("$19");
    register char *sprt __asm__("$16");
    register int v __asm__("$2");
    register int clut_x __asm__("$4");
    register int clut_y __asm__("$5");
    register char *base_t0 __asm__("$8");
    register int draw_shift __asm__("$3");
    int fp_ofs;
    u16 tpage;

    draw = 0;
    shade = 0x80;
    draw_ofs = 0;
    do {
        i = 0;
        y = -0x18;
        ofs = draw_ofs;
        base = kaze_title_icon_row1_sprites;
        do {
            sprt = (char *)(draw_ofs + (int)base);
            SetSprt16(sprt);
            SetShadeTex(sprt, 0);
            clut_x = 0x3E0;
            clut_y = 0xF1;
            __asm__ volatile("" : : "r"(clut_x), "r"(clut_y));
            base_t0 = kaze_title_icon_row1_sprites;
            sprt = (char *)(ofs + (int)base_t0);
            v = -0x50;
            *(short *)(sprt + 8) = v;
            v = 0xC0;
            sprt[0xC] = v;
            v = 0xA0;
            *(short *)(sprt + 0xA) = y;
            sprt[0xD] = v;
            *(u16 *)(kaze_title_icon_row1_sprites + ofs + 0xE) = GetClut(clut_x, clut_y);
            y += 0x20;
            ofs += 0x10;
            sprt[4] = shade;
            sprt[5] = shade;
            sprt[6] = shade;
            kaze_setup_row1_flash_level[i] = shade;
            i++;
            base += 0x10;
        } while (i < 3);

        i = 0;
        fp_ofs = draw_ofs;
        y = -0x18;
        ofs = draw_ofs;
        base = kaze_title_icon_row2_sprites;
        do {
            sprt = (char *)(fp_ofs + (int)base);
            SetSprt16(sprt);
            SetShadeTex(sprt, 0);
            clut_x = 0x3E0;
            clut_y = 0xF1;
            __asm__ volatile("" : : "r"(clut_x), "r"(clut_y));
            base_t0 = kaze_title_icon_row2_sprites;
            sprt = (char *)(ofs + (int)base_t0);
            v = 0x40;
            *(short *)(sprt + 8) = v;
            v = 0xC0;
            sprt[0xC] = v;
            v = 0xB0;
            *(short *)(sprt + 0xA) = y;
            sprt[0xD] = v;
            *(u16 *)(kaze_title_icon_row2_sprites + ofs + 0xE) = GetClut(clut_x, clut_y);
            y += 0x20;
            ofs += 0x10;
            sprt[4] = shade;
            sprt[5] = shade;
            sprt[6] = shade;
            kaze_setup_row2_flash_level[i] = shade;
            i++;
            base += 0x10;
        } while (i < 3);

        tpage = GetTPage(0, 0, 0x200, 0x100);
        draw_shift = draw << 3;
        SetDrawTPage((void *)(draw_shift + (int)kaze_title_icon_rows_tpage_prims), 0, 1, tpage);
        draw++;
        draw_ofs += 0x30;
    } while (draw < 2);
}

/**
 * kaze_title_bg_sprites_init  (menu, 0x80053c70)
 *
 * Builds the fullscreen title background per buffer: 256x240 SPRT at
 * kaze_title_bg_sprt_prims plus a second, unnamed 64x240 SPRT further into
 * that block, with tpages 0x280/0x300 and CLUT (0x140,0xf0).
 */

void kaze_title_bg_sprites_init(void)
{
    extern void SetDrawTPage(void *, int, int, int);
    register int i __asm__("$20");
    register int y __asm__("$22");
    register int h __asm__("$21");
    register char *draw_base __asm__("$23");
    char *draw_base_right;
    register char *left __asm__("$17");
    register char *right __asm__("$18");
    register int clut_off __asm__("$19");
    register int draw_off __asm__("$16");
    register char *base __asm__("$2");
    u16 tpage;

    i = 0;
    y = -0x78;
    h = 0xF0;
    draw_base = kaze_title_bg_tpage_prims;
    draw_base_right = draw_base + 8;
    base = kaze_title_bg_sprt_prims;
    right = base + 0x14;
    left = base;
    clut_off = 0;
    do {
        SetSprt(left);
        SetShadeTex(left, 1);
        *(short *)(left + 8) = -0xA0;
        *(short *)(left + 0xA) = y;
        *(short *)(left + 0x10) = 0x100;
        *(short *)(left + 0x12) = h;
        left[0xC] = 0;
        left[0xD] = 0;
        *(u16 *)(kaze_title_bg_left_clut + clut_off) = GetClut(0x140, 0xF0);
        tpage = GetTPage(1, 0, 0x280, 0);
        draw_off = i << 4;
        SetDrawTPage((void *)(draw_off + (int)draw_base), 0, 1, tpage);

        SetSprt(right);
        SetShadeTex(right, 1);
        *(short *)(right + 8) = 0x60;
        *(short *)(right + 0xA) = y;
        *(short *)(right + 0x10) = 0x40;
        *(short *)(right + 0x12) = h;
        right[0xC] = 0;
        right[0xD] = 0;
        *(u16 *)(kaze_title_bg_right_clut + clut_off) = GetClut(0x140, 0xF0);
        tpage = GetTPage(1, 0, 0x300, 0);
        SetDrawTPage((void *)(draw_off + (int)draw_base_right), 0, 1, tpage);

        right += 0x28;
        left += 0x28;
        i++;
        clut_off += 0x28;
    } while (i < 2);
}

extern void AddPrim(void *ob, void *prim);
extern Prim20 kaze_setup_menu_preview_sprite[];
extern Prim20 kaze_setup_menu_caption_sprite[];
extern Slot20 kaze_menu_gs_ot_buf0_org[];

/**
 * kaze_title_cursor_frame_draw_3e00  (menu, 0x80053e00)
 *
 * Positions the two-part title cursor/frame at
 * (x-0xa0,y-0x78)/(x-0x60,y-0x58) and AddPrims 4 prims
 * (kaze_setup_menu_preview_sprite/204/214/23c) into OT
 * (&kaze_menu_gs_ot_buf0_org)[buf*5].
 */

void kaze_title_cursor_frame_draw_3e00(int buf, short x, short y)
{
    Prim20 *p1;
    Prim20 *p2;

    p1 = kaze_setup_menu_preview_sprite;
    p1 += buf;
    p1->x = x - 0xA0;
    p1->y = y - 0x78;
    p2 = kaze_setup_menu_caption_sprite + buf;
    p2->x = x - 0x60;
    p2->y = y - 0x58;
    AddPrim(kaze_menu_gs_ot_buf0_org[buf].ob, p1);
    AddPrim(kaze_menu_gs_ot_buf0_org[buf].ob, kaze_setup_menu_preview_tpage + buf * 8);
    AddPrim(kaze_menu_gs_ot_buf0_org[buf].ob, p2);
    AddPrim(kaze_menu_gs_ot_buf0_org[buf].ob, kaze_setup_menu_caption_tpage + buf * 8);
}

/**
 * kaze_title_panel_prims_add_3edc  (menu, 0x80053edc)
 *
 * Adds a title-menu panel group
 * (kaze_setup_menu_textline_underline_a/37c/33c) plus the row prim selected
 * by the given row index (kaze_setup_menu_textline_sprites + row*0x14) into
 * OT (&kaze_menu_gs_ot_buf0_org)[buf*5].
 */

void kaze_title_panel_prims_add_3edc(int buf, int row)
{
    AddPrim(kaze_menu_gs_ot_buf0_org[buf].ob, kaze_setup_menu_textline_underline_a + buf * 0x18);
    AddPrim(kaze_menu_gs_ot_buf0_org[buf].ob, kaze_setup_menu_textline_underline_b + buf * 0x18);
    AddPrim(kaze_menu_gs_ot_buf0_org[buf].ob, kaze_setup_menu_textline_sprites + buf * 0x78 + row * 0x14);
    AddPrim(kaze_menu_gs_ot_buf0_org[buf].ob, kaze_setup_menu_textline_tpage + buf * 8);
}
