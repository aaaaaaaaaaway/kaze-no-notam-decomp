#include "types.h"
#include "kaze_syms.h"
extern char kaze_setup_menu_preview_tpage[];
extern char kaze_setup_menu_caption_tpage[];
extern char kaze_setup_menu_textline_sprites[];
extern char kaze_setup_menu_textline_clut[];
extern char kaze_setup_menu_textline_tpage[];
extern char kaze_setup_menu_textline_underline_a[];
extern char kaze_setup_menu_textline_underline_b[];
extern char kaze_setup_menu_mode_panel_sprites[];
extern char kaze_setup_menu_mode_icon_sprites[];
typedef struct 
{
  char a;
  char b;
  char c;
} S3;

extern int kaze_title_menu_state;
extern void kaze_linef3_gradient_bar(int, int, int, int, int);
extern void kaze_drmove_addprim_a0ec(int, int);

/**
 * kaze_setup_menu_option_rows_draw  (menu, 0x80052fa8)
 *
 * Option/confirm rows draw pass: highlight box at y 0x60/0x80/0xa0 per state
 * kaze_title_menu_state (3/4/5), label variant to
 * kaze_title_panel_prims_add_3edc, then
 * kaze_title_row_groups_fade_draw/kaze_title_icon_rows_draw widgets and
 * background.
 */

void kaze_setup_menu_option_rows_draw(int arg0)
{
    int state;
    int label;
    int panel_arg;

    state = kaze_title_menu_state;
    if (state == 4) {
        goto state4;
    }
    if (state < 5) {
        goto state_default;
    }
    if (state == 5) {
        goto state5;
    }
    if (state == 6) {
        goto state6;
    }

state_default:
    kaze_linef3_gradient_bar(arg0, 0x68, 0x60, 0x70, 0x10);
    panel_arg = arg0;
    label = 2;
    goto draw_rest;

state4:
    kaze_linef3_gradient_bar(arg0, 0x68, 0x80, 0x70, 0x10);
    panel_arg = arg0;
    label = 3;
    goto draw_rest;

state5:
    kaze_linef3_gradient_bar(arg0, 0x68, 0xA0, 0x70, 0x10);
    panel_arg = arg0;
    label = 4;
    goto draw_rest;

state6:
    panel_arg = arg0;
    label = 5;

draw_rest:
    kaze_title_panel_prims_add_3edc(panel_arg, label);
    kaze_title_cursor_frame_draw_3e00(arg0, 0, 0);
    kaze_title_row_groups_fade_draw(arg0);
    kaze_title_icon_rows_draw(arg0);
    kaze_drmove_addprim_a0ec(arg0, 0xFFF);
}

extern char kaze_setup_menu_preview_sprite[];
extern char kaze_setup_menu_caption_sprite[];
extern void SetSprt(void *);
extern void SetShadeTex(void *, int);
extern u16 GetClut(int, int);
extern u16 GetTPage(int, int, int, int);
extern void SetDrawTPage(void *, int, int, unsigned int);

/**
 * kaze_setup_menu_header_prim_init  (menu, 0x800530ac)
 *
 * Inits setup-menu header sprites per buffer at
 * kaze_setup_menu_preview_sprite: a 64x64 preview SPRT at (-0xa0,-0x78)
 * (clut 0x3c0) and a 128x32 caption SPRT at (-0x60,-0x58) (clut 0x3d0) with
 * DR_TPAGEs.
 */

void kaze_setup_menu_header_prim_init(void)
{
    register int i __asm__("$19");
    register int ofs __asm__("$18");
    register int shade __asm__("$20");
    register char *prim __asm__("$16");
    register int dr_ofs __asm__("$17");
    u16 tpage;

    i = 0;
    shade = 0x80;
    ofs = 0;
    do {
        prim = kaze_setup_menu_preview_sprite + ofs;
        SetSprt(prim);
        SetShadeTex(prim, 1);
        *(short *)(prim + 0x08) = -0xA0;
        *(short *)(prim + 0x0A) = -0x78;
        *(short *)(prim + 0x10) = 0x40;
        *(short *)(prim + 0x12) = 0x40;
        prim[0x0C] = shade;
        prim[0x0D] = shade;
        *(u16 *)(kaze_setup_menu_preview_sprite + ofs + 0x0E) = GetClut(0x3C0, 0xF1);
        tpage = GetTPage(0, 0, 0x200, 0x100);
        dr_ofs = i << 3;
        SetDrawTPage(kaze_setup_menu_preview_tpage + dr_ofs, 0, 1, tpage);

        prim = kaze_setup_menu_caption_sprite + ofs;
        SetSprt(prim);
        SetShadeTex(prim, 1);
        *(short *)(prim + 0x08) = -0x60;
        *(short *)(prim + 0x0A) = -0x58;
        *(short *)(prim + 0x12) = 0x20;
        *(short *)(prim + 0x10) = shade;
        prim[0x0C] = shade;
        prim[0x0D] = 0xC0;
        *(u16 *)(kaze_setup_menu_caption_sprite + ofs + 0x0E) = GetClut(0x3D0, 0xF1);
        tpage = GetTPage(0, 0, 0x200, 0x100);
        SetDrawTPage(kaze_setup_menu_caption_tpage + dr_ofs, 0, 1, tpage);
        i++;
        ofs += 0x14;
    } while (i < 2);
}

extern void SetLineF3(void *);

/**
 * kaze_setup_menu_textline_prim_init  (menu, 0x80053228)
 *
 * Inits six 256x16 setup-menu text-line SPRTs per buffer at
 * kaze_setup_menu_textline_sprites (y 0x58, v0=i*16, clut(0x340,0xf1)) plus
 * DR_TPAGE and two LINE_F3 underline frames.
 */

void kaze_setup_menu_textline_prim_init(void)
{
    int outer;
    int line_ofs;
    int outer_ofs;
    char *scan;
    int inner;
    int inner_ofs;
    char *prim;
    char *entry;
    char *line1;
    char *line2;
    u16 tpage;

    outer = 0;
    do {
        outer_ofs = outer * 0x78;
        line_ofs = outer * 0x18;
        inner = 0;
        inner_ofs = outer_ofs;
        scan = kaze_setup_menu_textline_sprites;
        do {
            prim = (char *)(outer_ofs + (int)scan);
            SetSprt(prim);
            SetShadeTex(prim, 1);
            entry = (char *)(inner_ofs + (int)kaze_setup_menu_textline_sprites);
            *(short *)(entry + 8) = -0x80;
            *(short *)(entry + 0xA) = 0x58;
            *(short *)(entry + 0x10) = 0x100;
            *(short *)(entry + 0x12) = 0x10;
            *(char *)(entry + 0xC) = 0;
            *(char *)(entry + 0xD) = inner << 4;
            *(u16 *)(kaze_setup_menu_textline_clut + inner_ofs) = GetClut(0x340, 0xF1);
            inner_ofs += 0x14;
            inner++;
            scan += 0x14;
        } while (inner < 6);
        tpage = GetTPage(0, 0, 0x1C0, 0x100);
        SetDrawTPage(kaze_setup_menu_textline_tpage + outer * 8, 0, 1, tpage);
        line1 = (char *)(line_ofs + (int)kaze_setup_menu_textline_underline_a);
        SetLineF3(line1);
        line2 = (char *)(line_ofs + (int)kaze_setup_menu_textline_underline_b);
        SetLineF3(line2);
        outer++;
        *(short *)(line1 + 8) = -0x81;
        *(short *)(line1 + 0xA) = 0x68;
        *(short *)(line1 + 0xC) = -0x81;
        *(short *)(line1 + 0xE) = 0x57;
        *(short *)(line1 + 0x10) = 0x80;
        *(short *)(line1 + 0x12) = 0x57;
        *(short *)(line2 + 8) = 0x80;
        *(short *)(line2 + 0xC) = 0x80;
        *(short *)(line2 + 0xA) = 0x57;
        *(short *)(line2 + 0xE) = 0x68;
        *(short *)(line2 + 0x10) = -0x81;
        *(short *)(line2 + 0x12) = 0x68;
        *(char *)(line1 + 4) = 0xA0;
        *(char *)(line1 + 5) = 0xA0;
        *(char *)(line1 + 6) = 0xA0;
        *(char *)(line2 + 4) = 0x60;
        *(char *)(line2 + 5) = 0x60;
        *(char *)(line2 + 6) = 0x60;
    } while (outer < 2);
}

extern char kaze_setup_menu_stage_panel_sprites[];
extern char kaze_setup_menu_stage_panel_tpage[];
extern int kaze_menu_stage_source;

/**
 * kaze_setup_menu_stage_panel_prim_init  (menu, 0x800533f0)
 *
 * Inits the three 0x40x0x80 stage-choice panel SPRTs per buffer at
 * kaze_setup_menu_stage_panel_sprites (x -0x70/-0x20/0x30, cluts
 * 0x360+i*0x10); the choice selected by kaze_menu_stage_source gets
 * brightness 0x80, others 0x40.
 */

void kaze_setup_menu_stage_panel_prim_init(void)
{
    register int page __asm__("$23");
    register int group_ofs __asm__("$22");
    register int choice __asm__("$17");
    register int clut_x __asm__("$21");
    register int x __asm__("$20");
    register int store_ofs __asm__("$18");
    register char *scan __asm__("$19");
    register char *prim __asm__("$16");
    register int clut_arg_x __asm__("$4");
    register int clut_arg_y __asm__("$5");
    register int dr_ofs __asm__("$3");
    register int y_const __asm__("$2");
    int menu_choice;
    int bright;
    register int dim __asm__("$8");
    u16 clut;

    page = 0;
    bright = 0x80;
    group_ofs = 0;
    do {
        choice = 0;
        clut_x = 0x360;
        x = -0x70;
        store_ofs = group_ofs;
        scan = kaze_setup_menu_stage_panel_sprites;
        do {
            prim = (char *)(group_ofs + (int)scan);
            SetSprt(prim);
            SetShadeTex(prim, 1);
            clut_arg_x = clut_x;
            clut_arg_y = 0xF1;
            __asm__ volatile("" : "=r"(clut_arg_y) : "0"(clut_arg_y));
            dim = (int)kaze_setup_menu_stage_panel_sprites;
            prim = (char *)(store_ofs + dim);
            y_const = -0x30;
            dim = 0x40;
            *(short *)(prim + 0x0A) = y_const;
            *(short *)(prim + 0x08) = x;
            *(short *)(prim + 0x10) = dim;
            *(short *)(prim + 0x12) = bright;
            prim[0x0C] = choice << 6;
            prim[0x0D] = 0;
            clut = GetClut(clut_arg_x, clut_arg_y);
            menu_choice = kaze_menu_stage_source;
            *(u16 *)(kaze_setup_menu_stage_panel_sprites + store_ofs + 0x0E) = clut;
            dim = 0x40;
            if (menu_choice == choice) {
                prim[0x04] = bright;
                prim[0x05] = bright;
                prim[0x06] = bright;
            } else {
                prim[0x04] = dim;
                prim[0x05] = dim;
                prim[0x06] = dim;
            }
            clut_x += 0x10;
            x += 0x50;
            store_ofs += 0x14;
            choice++;
            scan += 0x14;
        } while (choice < 3);
        clut = GetTPage(0, 0, 0x200, 0x100);
        dr_ofs = page << 3;
        SetDrawTPage(kaze_setup_menu_stage_panel_tpage + dr_ofs, 0, 1, clut);
        page++;
        group_ofs += 0x3C;
    } while (page < 2);
}

extern S3 kaze_mode_panel_uv_u_table;
extern S3 kaze_mode_panel_uv_v_table;
extern char kaze_setup_menu_mode_icon_tpage[];
extern int kaze_menu_mode_source;

/**
 * kaze_setup_menu_mode_panel_prim_init  (menu, 0x80053560)
 *
 * Inits the three mode-choice panel SPRTs at
 * kaze_setup_menu_mode_panel_sprites (uv from
 * kaze_mode_panel_uv_u_table/kaze_mode_panel_uv_v_table tables, cluts
 * 0x390+i*0x10, selected kaze_menu_mode_source bright) plus three 0x18x0x20
 * icon SPRTs (clut 0x3f0).
 */

void kaze_setup_menu_mode_panel_prim_init(void)
{
  register int choice asm("$17");
  register int store_ofs asm("$18");
  char *panel_base;
  register char *panel_ptr asm("$19");
  register int panel_y asm("$20");
  register char *icon_ptr asm("$21");
  register int icon_y asm("$22");
  register int icon_x asm("$23");
  register char *sprt asm("$16");
  int group_ofs;
  int page;
  int menu_choice;
  u8 uv_u[8];
  int icon_base;
  u8 uv_v[8];
  u16 clut;
  char *p;
  register char *p2 asm("$3");
  *((S3 *) uv_u) = kaze_mode_panel_uv_u_table;
  *((S3 *) uv_v) = kaze_mode_panel_uv_v_table;
  group_ofs = 0;
  page = 0;
  do
  {
    choice = 0;
    icon_x = -0x80;
    icon_y = -0x78;
    store_ofs = group_ofs;
    icon_ptr = kaze_setup_menu_mode_icon_sprites;
    panel_y = -0x70;
    panel_ptr = kaze_setup_menu_mode_panel_sprites;
    do
    {
      sprt = (char *) (group_ofs + ((int) panel_ptr));
      panel_base = kaze_setup_menu_mode_panel_sprites;
      SetSprt(sprt);
      SetShadeTex(sprt, 1);
      p = (char *) (store_ofs + ((int) panel_base));
      *((short *) (p + 0x0A)) = -0x30;
      *((short *) (p + 0x10)) = 0x40;
      *((short *) (p + 0x08)) = panel_y;
      *((short *) (p + 0x12)) = 0x80;
      p[0x0C] = uv_u[choice];
      p[0x0D] = uv_v[choice];
      clut = GetClut((choice * 0x10) + 0x390, 0xF1);
      menu_choice = kaze_menu_mode_source;
      *((u16 *) ((kaze_setup_menu_mode_panel_sprites + store_ofs) + 0x0E)) = clut;
      icon_base = (int) kaze_setup_menu_mode_icon_sprites;
      if (menu_choice == choice)
      {
        p[0x04] = 0x80;
        p[0x05] = 0x80;
        p[0x06] = 0x80;
      }
      else
      {
        p[0x04] = 0x40;
        p[0x05] = 0x40;
        p[0x06] = 0x40;
      }
      sprt = (char *) (group_ofs + ((int) icon_ptr));
      SetSprt(sprt);
      SetShadeTex(sprt, 1);
      p2 = (char *) (store_ofs + icon_base);
      *((short *) (p2 + 0x0A)) = -0x38;
      *((short *) (p2 + 0x10)) = 0x18;
      *((short *) (p2 + 0x12)) = 0x20;
      *((short *) (p2 + 0x08)) = icon_y;
      p2[0x0C] = icon_x;
      p2[0x0D] = 0xE0;
      clut = GetClut(0x3F0, 0xF1);
      icon_x += 0x18;
      icon_y += 0x50;
      *((u16 *) ((kaze_setup_menu_mode_icon_sprites + store_ofs) + 0x0E)) = clut;
      store_ofs += 0x14;
      icon_ptr += 0x14;
      panel_y += 0x50;
      choice += 1;
      panel_ptr += 0x14;
    }
    while (choice < 3);
    clut = GetTPage(0, 0, 0x200, 0x100);
    SetDrawTPage(kaze_setup_menu_mode_icon_tpage + (page * 8), 0, 1, (unsigned int) clut);
    group_ofs += 0x3C;
    page += 1;
 do { } while (0);
  }
  while (page < 2);
}
