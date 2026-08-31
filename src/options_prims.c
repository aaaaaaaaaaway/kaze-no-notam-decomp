#include "types.h"
#include "kaze_syms.h"
extern char kaze_options_header_left_sprites[];
extern char kaze_options_header_left_sprites_y[];
extern char kaze_options_header_right_sprites[];
extern char kaze_options_header_right_sprites_y[];
extern char kaze_options_header_tpage_prims[];
extern u8 kaze_options_row_label_tint;
extern u16 kaze_button_config_action_label_clut;
extern u8 kaze_button_config_panel_bg_sprite[];
extern u16 kaze_button_config_glyph_clut;
extern char kaze_button_config_toggle_glyph_u;
extern u16 kaze_button_config_toggle_glyph_clut;
extern char kaze_button_config_toggle_glyph2_u;
extern char kaze_sound_mode_label_sprites[];
extern char kaze_sound_mode_tpage_prims[];
typedef struct {
    char pad[4];
    u8 unk4;
    u8 unk5;
    u8 unk6;
} Row;

extern void SetSprt(void *);
extern void SetShadeTex(void *, int);

/**
 * kaze_options_row_sprites_init  (menu, 0x80056a4c)
 *
 * Builds the 4 top-menu row-label SPRTs per buffer at
 * kaze_options_row_label_sprites (CLUT 0x350,0xf3, tpage 0x380) with neutral
 * 0x80 tint.
 */

void kaze_options_row_sprites_init(void)
{
    extern void SetDrawTPage(void *, int, int, int);
    extern int GetTPage(int, int, int, int);
    extern int GetClut(int, int);
    extern char kaze_options_row_label_tpage_prims[];
    extern char kaze_options_row_label_sprites[];
    int page;
    int group_ofs;
    int row;
    int u;
    int y;
    register int ofs __asm__("$17");
    int size;
    register char *base __asm__("$18");
    register char *store_base __asm__("$8");
    char *p;
    register char *q __asm__("$3");
    int tpage;

    page = 0;
    size = 0x80;
    group_ofs = 0;
    do {
        row = 0;
        u = 0x40;
        y = -0x18;
        ofs = group_ofs;
        base = kaze_options_row_label_sprites;
        do {
            p = (char *)(group_ofs + (int)base);
            SetSprt(p);
            SetShadeTex(p, 1);
            store_base = kaze_options_row_label_sprites;
            q = (char *)(ofs + (int)store_base);
            *(short *)(q + 0x08) = -0x40;
            *(short *)(q + 0x0A) = y;
            *(short *)(q + 0x10) = size;
            *(short *)(q + 0x12) = 0x10;
            q[0x0C] = 0;
            q[0x0D] = u;
            q[0x04] = size;
            q[0x05] = size;
            q[0x06] = size;
            tpage = GetClut(0x350, 0xF3);
            u += 0x10;
            y += 0x18;
            *(short *)(ofs + (int)kaze_options_row_label_sprites + 0x0E) = tpage;
            ofs += 0x14;
            row++;
            base += 0x14;
        } while (row < 4);
        tpage = GetTPage(0, 0, 0x380, 0x100);
        SetDrawTPage(kaze_options_row_label_tpage_prims + page * 8, 0, 1, tpage & 0xFFFF);
        page++;
        group_ofs += 0x50;
    } while (page < 2);
}

/**
 * kaze_button_config_sprites_init  (menu, 0x80056b98)
 *
 * Builds key-config sprites per buffer: 6 action labels at
 * kaze_button_config_action_label_sprites, 5 button glyphs at UNK_800abc70
 * (CLUT 0x360,0xf2), and 2 toggle glyphs at UNK_800abd48 (CLUT 0x380,0xf3).
 */

void kaze_button_config_sprites_init(void)
{
    extern void SetDrawTPage(void *, int, int, unsigned int);
    extern u16 GetTPage(int, int, int, int);
    extern u16 GetClut(int, int);
    extern u8 kaze_button_config_toggle_tpage_prims[];
    extern u8 kaze_button_config_toggle_glyph_sprites[];
    extern u8 kaze_button_config_glyph_tpage_prims[];
    extern u8 kaze_button_config_glyph_sprites[];
    extern u8 kaze_button_config_action_tpage_prims[];
    extern u8 kaze_button_config_action_label_sprites[];
    u16 tex_attr;
    int inner_i;
    int off;
    char *sprite_ptr;
    short y_pos;
    int x_pos;
    int buf_off;
    int buf_idx;
    int tpage_off;
    int toggle_off;
    int button_off;
    int new_var;
    register char * sprt_ptr asm("$3");

    buf_off = 0;
    buf_idx = 0;
    tpage_off = 0;
    toggle_off = 0;
    button_off = 0;
    do {
        inner_i = 0;
        y_pos = -0x28;
        off = buf_off;
        sprite_ptr = kaze_button_config_action_label_sprites;
        do {
            SetSprt((void *)(buf_off + (int)sprite_ptr));
            SetShadeTex((void *)(buf_off + (int)sprite_ptr), 1);
            { int clx; int cly;
              __asm__ volatile("li %0,0x350" : "=r"(clx));
              __asm__ volatile("li %0,0xf2" : "=r"(cly));
            { char *rawbase = kaze_button_config_action_label_sprites;
              sprt_ptr = (char *)(off + (int)rawbase);
              __asm__ volatile("" : : "r"(rawbase)); }
            *(short *)(sprt_ptr + 8) = 0xffa0;
            *(short *)(sprt_ptr + 0x10) = 0x80;
            *(short *)(sprt_ptr + 0x12) = 0x10;
            *(short *)(sprt_ptr + 0xa) = y_pos;
            sprt_ptr[0xc] = 0x80;
            sprt_ptr[0xd] = (char)(inner_i << 4);
            tex_attr = GetClut(clx, cly);
            }
            y_pos = y_pos + 0x14;
            *(u16 *)((int)&kaze_button_config_action_label_clut + off) = tex_attr;
            off = off + 0x14;
            inner_i = inner_i + 1; __asm__(""::"r"(inner_i)); __asm__(""::"r"(inner_i)); __asm__(""::"r"(inner_i));
            sprite_ptr = sprite_ptr + 0x14;
        } while (inner_i < 6);
        { register int gx asm("$4");
          register int gy asm("$5");
          register int gw asm("$6");
          register int gh asm("$7");
          register char *drawp asm("$3");
          gx = 0; gy = 0; gw = 0x340; gh = 0x100;
          __asm__ volatile("" : : "r"(gx), "r"(gy), "r"(gw), "r"(gh));
          drawp = (char *)((int)kaze_button_config_panel_bg_sprite + buf_off);
          *(short *)(drawp + 8) = 0xffc0;
          *(short *)(drawp + 0xa) = 0x48;
          tex_attr = GetTPage(gx, gy, gw, gh); }
        SetDrawTPage((void *)((int)kaze_button_config_action_tpage_prims + tpage_off), 0, 1, (unsigned int)tex_attr);
        inner_i = 0;
        {
        char *base2 = kaze_button_config_glyph_sprites;
        new_var = button_off;
        sprite_ptr = base2;
        off = new_var;
        do {
            SetSprt((void *)(new_var + (int)sprite_ptr));
            SetShadeTex((void *)(new_var + (int)sprite_ptr), 1);
            { int clx; int cly;
              __asm__ volatile("li %0,0x360" : "=r"(clx));
              __asm__ volatile("li %0,0xf2" : "=r"(cly));
            sprt_ptr = (char *)(off + (int)base2);
            { int wh;
              __asm__ volatile("li %0,0x20" : "=r"(wh));
              *(short *)(sprt_ptr + 0x10) = wh;
              __asm__ volatile("li %0,0x10" : "=r"(wh));
              *(short *)(sprt_ptr + 0x12) = wh; }
            tex_attr = GetClut(clx, cly);
            }
            *(u16 *)((int)&kaze_button_config_glyph_clut + off) = tex_attr;
            off = off + 0x14;
            inner_i = inner_i + 1; __asm__(""::"r"(inner_i));
            sprite_ptr = sprite_ptr + 0x14;
        } while (inner_i < 5);
        }
        tex_attr = GetTPage(0, 0, 0x340, 0x100);
        SetDrawTPage((void *)((int)kaze_button_config_glyph_tpage_prims + tpage_off), 0, 1, (unsigned int)tex_attr);
        inner_i = 0;
        x_pos = -0x80;
        y_pos = -0x28;
        {
            char *pB;
            int held38 = toggle_off;
            pB = kaze_button_config_toggle_glyph_sprites;
            off = held38;
            sprite_ptr = pB;
            do {
                SetSprt((void *)(held38 + (int)sprite_ptr));
                SetShadeTex((void *)(held38 + (int)sprite_ptr), 1);
                { int clx; int cly;
                  __asm__ volatile("li %0,0x380" : "=r"(clx));
                  __asm__ volatile("li %0,0xf3" : "=r"(cly));
                sprt_ptr = (char *)(off + (int)pB);
                { register int wh asm("$2");
                  __asm__ volatile("li %0,0x30" : "=r"(wh));
                  *(short *)(sprt_ptr + 8) = wh;
                  __asm__ volatile("li %0,0x28":"=r"(wh));
                  sprt_ptr[0xc] = (
                      *(short *)(sprt_ptr + 0xa) = y_pos,
                      *(short *)(sprt_ptr + 0x10) = wh,
                      *(short *)(sprt_ptr + 0x12) = wh,
                      0x80); }
                sprt_ptr[0xd] = x_pos;
                tex_attr = GetClut(clx, cly);
                }
                x_pos = x_pos + 0x28;
                y_pos = y_pos + 0x38;
                *(u16 *)((int)&kaze_button_config_toggle_glyph_clut + off) = tex_attr;
                off = off + 0x14;
                inner_i = inner_i + 1; __asm__(""::"r"(inner_i));
                sprite_ptr = sprite_ptr + 0x14;
            } while (inner_i < 2);
        }
        tex_attr = GetTPage(0, 0, 0x2c0, 0x100);
        SetDrawTPage((void *)((int)kaze_button_config_toggle_tpage_prims + tpage_off), 0, 1, (unsigned int)tex_attr);
        tpage_off = tpage_off + 8;
        toggle_off = toggle_off + 0x28;
        __asm__("" :: "r"(toggle_off) : "memory");
        button_off = button_off + 100;
        buf_off = buf_off + 0x78;
        buf_idx = buf_idx + 1;
    } while (buf_idx < 2);
}

/**
 * kaze_sound_mode_sprites_init  (menu, 0x80056ea4)
 *
 * Builds the 2 stereo/mono label SPRTs per buffer at
 * kaze_sound_mode_label_sprites (CLUT 0x350,0xf3, tpage 0x380).
 */

void kaze_sound_mode_sprites_init(void)
{
    extern void SetDrawTPage(void *, int, int, int);
    extern int GetTPage(int, int, int, int);
    extern int GetClut(int, int);
    register int page __asm__("$23");
    int color;
    register int group_ofs __asm__("$22");
    register int row __asm__("$21");
    register int u __asm__("$20");
    register int y __asm__("$19");
    register int ofs __asm__("$17");
    register char *base __asm__("$18");
    register char *p __asm__("$16");
    register char *global_base __asm__("$8");
    register char *q __asm__("$3");
    register char *draw_base __asm__("$4");
    register int clut_x __asm__("$4");
    register int clut_y __asm__("$5");
    int t;

    page = 0;
    color = 0x80;
    group_ofs = 0;
    do {
        row = 0;
        u = 0x40;
        y = -0x18;
        ofs = group_ofs;
        base = kaze_sound_mode_label_sprites;
        do {
            p = (char *)((int)group_ofs + (int)base);
            SetSprt(p);
            SetShadeTex(p, 1);
            clut_x = 0x350;
            clut_y = 0xF3;
            __asm__ volatile("" ::: "memory");
            global_base = kaze_sound_mode_label_sprites;
            q = ofs + global_base;
            *(short *)(q + 0x08) = -0x40;
            *(short *)(q + 0x0A) = y;
            *(short *)(q + 0x10) = color;
            *(short *)(q + 0x12) = 0x10;
            q[0x0C] = color;
            q[0x0D] = u;
            t = GetClut(clut_x, clut_y);
            u += 0x10;
            y += 0x20;
            *(short *)(kaze_sound_mode_label_sprites + ofs + 0x0E) = t;
            ofs += 0x14;
            row++;
            base += 0x14;
        } while (row < 2);
        t = GetTPage(0, 0, 0x380, 0x100);
        q = (char *)(page * 8);
        draw_base = kaze_sound_mode_tpage_prims;
        draw_base = (int)q + draw_base;
        SetDrawTPage(draw_base, 0, 1, t & 0xFFFF);
        page++;
        group_ofs += 0x28;
    } while (page < 2);
}

/**
 * kaze_options_header_set_y  (menu, 0x80056fe4)
 *
 * Sets the Y coordinates of the header banner sprite pair
 * (kaze_options_header_left_sprites_y/kaze_options_header_right_sprites_y)
 * for the given buffer/group; used by the editor zoom animation.
 */

void kaze_options_header_set_y(int arg0, int arg1, short arg2)
{
    *(s16 *)(kaze_options_header_left_sprites_y + arg1 * 20 + arg0 * 80) = arg2 - 0x78;
    *(s16 *)(kaze_options_header_right_sprites_y + arg1 * 20 + arg0 * 80) = arg2 - 0x58;
}

extern char kaze_menu_gs_ot_buf0_org[];

/**
 * kaze_options_header_prims_add  (menu, 0x80057028)
 *
 * Optionally resets the group header pair to default positions, then
 * AddPrims the two header sprites plus DR_TPAGE
 * kaze_options_header_tpage_prims into OT
 * (&kaze_menu_gs_ot_buf0_org)[buf*5].
 */

void kaze_options_header_prims_add(int arg0, int arg1, int arg2)
{
    extern int AddPrim(void *, void *);
    int reset_ofs;
    int slot;
    int ot_ofs;
    int prim_ofs;
    char *prim;

    if (arg2 != 0) {
        register char *prim2 asm("$3");

        prim = kaze_options_header_left_sprites;
        reset_ofs = arg0 * 0x50 + arg1 * 0x14;
        prim = prim + reset_ofs;
        *(short *)(prim + 8) = -0xA0;
        *(short *)(prim + 10) = -0x78;
        prim2 = kaze_options_header_right_sprites + reset_ofs;
        *(short *)(prim2 + 8) = -0x60;
        *(short *)(prim2 + 10) = -0x58;
    }
    slot = arg0 * 5;
    ot_ofs = slot * 4;
    slot = slot * 0x10;
    prim_ofs = arg1 * 0x14;
    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + ot_ofs), kaze_options_header_left_sprites + slot + prim_ofs);
    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + ot_ofs), kaze_options_header_right_sprites + slot + prim_ofs);
    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + ot_ofs), kaze_options_header_tpage_prims + arg0 * 8);
}

extern s32 kaze_options_top_menu_row;

/**
 * kaze_options_rows_highlight_draw  (menu, 0x80057138)
 *
 * Fades the 4 top-menu row labels toward the selected tint for row
 * kaze_options_top_menu_row and AddPrims them plus tpage prim
 * kaze_options_row_label_tpage_prims.
 */

void kaze_options_rows_highlight_draw(s32 arg0) {
    extern u8 kaze_options_row_label_tpage_prims;
    extern u8 kaze_options_row_label_sprites;
    extern void AddPrim(void *, void *);
    register s32 off asm("$16");
    s32 prim_ofs;
    register s32 i asm("$18");
    register char *row asm("$19");
    register s32 arg asm("$22");
    s32 arg5;
    register s32 base60 asm("$20");
    register u8 *b3ac asm("$23");
    Row *r;
    int v0;
    int v1;
    int cc;
    u8 c;
    void *temp_a1;
    void *prim_ptr;

    arg = arg0;
    i = 0;
    row = (char *)&kaze_options_row_label_sprites;
    b3ac = (u8 *)row;
    arg5 = arg * 5;
    base60 = arg5 * 0x10;
    off = base60;
    do {
        asm volatile("" : "=r"(arg) : "0"(arg));
        if (i == kaze_options_top_menu_row) {
            c = *(&kaze_options_row_label_tint + off);
            if (c < 0xB1U) {
                *(&kaze_options_row_label_tint + off) = c + 0x10;
                v0 = *(&kaze_options_row_label_tint + 1 + off) + 0x10;
                v1 = *(&kaze_options_row_label_tint + 2 + off) - 0x10;
                goto store12;
            }
            r = (Row *)(off + (int)b3ac);
            cc = 0xC0;
            r->unk4 = cc;
            r->unk5 = cc;
            cc = 0x40;
            goto storeclamp6;
        }
        c = *(&kaze_options_row_label_tint + off);
        if (c >= 0x90U) {
            *(&kaze_options_row_label_tint + off) = c - 0x10;
            v0 = *(&kaze_options_row_label_tint + 1 + off) - 0x10;
            v1 = *(&kaze_options_row_label_tint + 2 + off) + 0x10;
        store12:
            *(&kaze_options_row_label_tint + 1 + off) = v0;
            *(&kaze_options_row_label_tint + 2 + off) = v1;
            prim_ofs = arg5 * 4;
        } else {
            r = (Row *)(off + (int)b3ac);
            cc = 0x80;
            r->unk4 = cc;
            r->unk5 = cc;
        storeclamp6:
            r->unk6 = cc;
            prim_ofs = arg5 * 4;
        }
        prim_ptr = *(void **)(kaze_menu_gs_ot_buf0_org + prim_ofs);
        asm volatile("" :: "r"(prim_ptr));
        temp_a1 = (void *)(base60 + (int)row);
        row += 0x14;
        off += 0x14;
        i++;
        AddPrim(prim_ptr, temp_a1);
    } while (i < 4);
    cc = arg * 8;
    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + prim_ofs), (void *)(cc + (int)&kaze_options_row_label_tpage_prims));
}

extern u8 kaze_button_action_map;
extern u8 kaze_settings_toggle_a;
extern u8 kaze_settings_toggle_b;

/**
 * kaze_button_config_prims_draw  (menu, 0x800572f4)
 *
 * Sets the 5 button-glyph UVs from mapping kaze_button_action_map and the 2
 * toggle glyph UVs from kaze_settings_toggle_a/1d, then AddPrims all key-
 * config sprites/tpages.
 */

void kaze_button_config_prims_draw(int arg0)
{
    extern char kaze_button_config_toggle_tpage_prims;
    extern char kaze_button_config_toggle_glyph_sprites;
    extern char kaze_button_config_glyph_tpage_prims;
    extern char kaze_button_config_action_tpage_prims;
    extern char kaze_button_config_action_label_sprites;
    extern char kaze_button_config_glyph_sprites;
    extern void AddPrim(void *, void *);
  int i;
  int base60;
  int io;
  short y;
  int p1;
  char *pw;
  volatile char * base;
  char *new_var;
  int ot_ofs;
  int j;
  register int ot2 asm("$2");
  register int ot_loop asm("$20");
  register int ot3 asm("$18");
  register int joff asm("$17");
  p1 = arg0;
  i = 0;
  {
    int t;
    t = (p1 << 2) + p1;
    ot_ofs = t << 2;
  }
  base = &kaze_button_config_glyph_sprites;
  new_var = base;
  pw = new_var;
  y = -0x28;
  base60 = p1 * 100;
  io = base60;
  do
  {
    register int b asm("$3");
    int t;
    register int c asm("$6");
    int adjusted;
    volatile char *p;
    register char * sp asm("$4");
    b = (u8) (&kaze_button_action_map)[i];
    sp = (char *)(io + (int)base);
    *((short *) (sp + 8)) = -0x10;
    *((short *) (sp + 0xA)) = y;
    if (b < 0) adjusted = b + 3;
    else adjusted = b;
    c = adjusted >> 2;
    __asm__("":"=r"(adjusted):"0"(adjusted));
    __asm__("" :: "r"(adjusted));
    do { sp[0xC] = ((b + (c * (-4))) * 0x20) + (-0x80); } while (0);
    __asm__("" :: "r"(b) : "memory");
    p = (char *)(base60 + (int)(p = pw));
    pw += 0x14;
    y += 0x14;
    io = io + 0x14;
    sp[0xD] = (c * 0x10) + 0x60;
    { int _mt = c; __asm__("" :: "r"(_mt) : "memory"); }
    i += 1;
    AddPrim(*((void **) (kaze_menu_gs_ot_buf0_org + ot_ofs)), p);
  }
  while (i < 5);
  __asm__("":"=r"(i):"0"(i));
  {
    int t;
    t = (p1 << 2) + p1;
    ot2 = t << 2;
  }
  {
    register void *call_ot asm("$4");
    register char *call_prim asm("$5");
    call_ot = *((void **) (kaze_menu_gs_ot_buf0_org + ot2));
    call_prim = (&kaze_button_config_glyph_tpage_prims) + (p1 * 8);
    j = 0;
    ot_loop = ot2;
    __asm__("" :: "r"(call_ot), "r"(call_prim), "r"(ot_loop) : "memory");
    AddPrim(call_ot, call_prim);
  }
  joff = 0;
  do
  {
    register void *otarg asm("$4");
    register char *primarg asm("$5");
    otarg = *((void **) (kaze_menu_gs_ot_buf0_org + ot_loop));
    primarg = &kaze_button_config_action_label_sprites;
    primarg += joff;
    primarg = (char *)((p1 * 0x78) + (int)primarg);
    joff += 0x14;
    j += 1;
    AddPrim(otarg, primarg);
    __asm__("" :: "r"(ot_loop) : "memory");
  }
  while (j < 6);
  { int _mt = j; __asm__("" :: "r"(_mt) : "memory"); }
  joff = (p1 << 2) + p1;
  ot3 = joff << 2;
  __asm__("" :: "r"(ot3) : "memory");
  p1 <<= 3;
  AddPrim(*((void **) (kaze_menu_gs_ot_buf0_org + ot3)), (&kaze_button_config_action_tpage_prims) + p1);
  {
    int color = kaze_settings_toggle_a;
    joff <<= 3;
    (&kaze_button_config_toggle_glyph_u)[joff] = (color * 0x28) + (-0x80);
  }
  (&kaze_button_config_toggle_glyph2_u)[joff] = (kaze_settings_toggle_b * 0x28) + (-0x80);
  __asm__("" :: "r"(p1) : "memory");
  AddPrim(*((void **) (kaze_menu_gs_ot_buf0_org + ot3)), (&kaze_button_config_toggle_glyph_sprites) + joff);
  AddPrim(*((void **) (kaze_menu_gs_ot_buf0_org + ot3)), ((&kaze_button_config_toggle_glyph_sprites) + joff) + 0x14);
  AddPrim(*((void **) (kaze_menu_gs_ot_buf0_org + ot3)), (&kaze_button_config_toggle_tpage_prims) + p1);
}

/**
 * kaze_sound_mode_prims_add  (menu, 0x80057544)
 *
 * Adds the two stereo/mono label sprites and their DR_TPAGE for the given
 * buffer to OT (&kaze_menu_gs_ot_buf0_org)[buf*5].
 */

void kaze_sound_mode_prims_add(int idx)
{
    extern void AddPrim(void *, void *);
    int i;
    int slot;
    int ofs20;
    int ofs40;
    char *p;

    i = 0;
    slot = idx * 5;
    ofs20 = slot * 4;
    ofs40 = slot * 8;
    p = kaze_sound_mode_label_sprites;
    do {
        AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ofs20), (void *)(ofs40 + (int)p));
        p += 0x14;
        i++;
    } while (i < 2);
    AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + (idx * 5 * 4)), kaze_sound_mode_tpage_prims + (idx * 8));
}
