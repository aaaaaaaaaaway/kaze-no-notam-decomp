#include "types.h"
#include "kaze_syms.h"
extern int kaze_balloon_color_edit_channel;
extern char kaze_balloon_name_row_sprites[];
extern char kaze_balloon_name_row_tpage_prims[];
extern char kaze_registration_confirm_label_sprites[];
extern char kaze_registration_confirm_tpage_prims[];
extern char kaze_color_edit_channel_label_x[];
extern u8 kaze_balloon_paint_cursor_cluts[];
extern int kaze_task_glyph_count;
extern int kaze_task_letter_count;
extern int kaze_task_menu_camera_bob_phase;
extern int kaze_task_carousel_angle_step;
extern char kaze_task_glyph_dobj_coord_ptr[];
extern int kaze_menu_task_model_ptr;
extern void SetLineF4(void *);
typedef struct {
    int ot;
    int pad[4];
} MenuTitlebarSlot;
typedef struct 
{
  void *ot;
  char pad[16];
} Slot;
typedef struct 
{
  char pad0[8];
  s16 unk8;
  s16 unkA;
  s8 unkC;
  s8 unkD;
  char pad1[2];
} Prim;

extern u8 kaze_balloon_paint_highlight_prims[];
extern u16 GetClut(int, int);
extern u16 GetTPage(int, int, int, int);

/**
 * kaze_paint_cursor_sprites_init  (menu, 0x80059ee4)
 *
 * Builds the paint-cursor SPRT_16 at kaze_balloon_paint_cursor_sprites
 * (position words kaze_balloon_paint_cursor_x/1fe) and the LINE_F4 poly-
 * highlight prim kaze_balloon_paint_highlight_prims per buffer.
 */

void kaze_paint_cursor_sprites_init(void)
{
    extern void SetDrawTPage(void *, int, int, unsigned int);
    extern void SetShadeTex(void *, int);
    extern void SetSprt16(void *);
    extern u8 kaze_balloon_paint_cursor_tpages[];
    extern u8 kaze_balloon_paint_cursor_sprites[];
    u16 attr;
    int sprt_ofs;
    int i;
    int line_ofs;
    u8 *p;

    i = 0;
    line_ofs = 0;
    sprt_ofs = 0;
    do {
        p = kaze_balloon_paint_cursor_sprites;
        p = sprt_ofs + p;
        SetSprt16(p);
        SetShadeTex(p, 1);
        p[0xC] = 0xE0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 0xA) = 0;
        p[0xD] = 0x90;
        attr = GetClut(0x3B0, 0xF2);
        *(u16 *)((int)kaze_balloon_paint_cursor_cluts + sprt_ofs) = attr;
        attr = GetTPage(0, 0, 0x340, 0x100);
        SetDrawTPage(i * 8 + kaze_balloon_paint_cursor_tpages, 0, 1, attr);
        p = kaze_balloon_paint_highlight_prims;
        p = line_ofs + p;
        SetLineF4(p);
        SetShadeTex(p, 1);
        line_ofs = line_ofs + 0x1C;
        i = i + 1;
        sprt_ofs = sprt_ofs + 0x10;
    } while (i < 2);
    return;
}

/**
 * kaze_balloon_entry_grid_sprites_init  (menu, 0x80059fe0)
 *
 * Builds a 5x3 SPRT_16 grid per buffer at kaze_balloon_entry_grid_sprites
 * (CLUT 0x370,0xf3, tpage 0x380); its positions
 * (kaze_balloon_entry_grid_sprite_x/7e) are laid out by the editor zoom-in
 * animation.
 */

void kaze_balloon_entry_grid_sprites_init(void)
{
    extern void SetDrawTPage(char *, int, int, unsigned int);
    extern void SetShadeTex(char *, int);
    extern void SetSprt16(char *);
    extern char kaze_balloon_entry_grid_tpages[];
    extern char kaze_balloon_entry_grid_sprites[];
    u16 clut;
    char *p;
    int inner_offset;
    int inner;
    char *row;
    int row_offset;
    char *base;
    int y;
    int buffer_offset;
    int buffer;

    buffer = 0;
    base = kaze_balloon_entry_grid_sprites;
    buffer_offset = 0;
    do {
        y = 0;
        row = base;
        row_offset = 0;
        do {
            inner = 0;
            inner_offset = row_offset;
            p = (char *)(buffer_offset + (int)row);
            do {
                SetSprt16(p);
                SetShadeTex(p, 1);
                clut = GetClut(0x370, 0xF3);
                /* Match the original symbol-offset CLUT store without hoisting its base. */
                __asm__ volatile(
                    "sh %0,kaze_balloon_entry_grid_cluts(%1)"
                    : : "r"(clut), "r"(inner_offset + buffer_offset));
                inner_offset += 0x10;
                inner++;
                p += 0x10;
            } while (inner < 3);
            row += 0x30;
            y++;
            row_offset += 0x30;
        } while (y < 5);
        clut = GetTPage(0, 0, 0x380, 0x100);
        SetDrawTPage(kaze_balloon_entry_grid_tpages + buffer * 8, 0, 1, clut);
        buffer++;
        buffer_offset += 0xF0;
    } while (buffer < 2);
}

/**
 * kaze_task_menu_row_sprites_draw  (menu, 0x8005a104)
 *
 * Draws the 5 task-list row SPRTs at kaze_balloon_name_row_sprites
 * (repositioned to x=-0x90, y=-0x1c step 0x18 when arg1) plus their DR_TPAGE
 * into the menu OT (&kaze_menu_gs_ot_buf0_org)[buf*5].
 */

void kaze_task_menu_row_sprites_draw(int arg0, int arg1)
{
    extern int AddPrim(void *, void *);
    extern char kaze_menu_gs_ot_buf0_org[];
    register int page __asm__("$17");
    int reset;
    register int row __asm__("$20");
    register int ot_ofs __asm__("$23");
    register char *base __asm__("$22");
    register char *scan __asm__("$19");
    register int y __asm__("$18");
    register int prim_ofs __asm__("$21");
    register int set_ofs __asm__("$16");

    page = arg0;
    __asm__("" : "=r"(page) : "0"(page));
    reset = arg1;
    row = 0;
    {
        register int tmp __asm__("$2");

        tmp = (page << 2) + page;
        ot_ofs = tmp << 2;
    }
    __asm__("" : "=r"(ot_ofs) : "0"(ot_ofs));
    base = kaze_balloon_name_row_sprites;
    scan = base;
    y = -0x1C;
    {
        register int tmp __asm__("$2");

        tmp = (page << 1) + page;
        tmp = (tmp << 3) + page;
        prim_ofs = tmp << 2;
    }
    set_ofs = prim_ofs;

    do {
        register char *set_ptr __asm__("$3");
        register void *ot __asm__("$4");
        char *prim;

        set_ptr = (char *)(set_ofs + (int)base);
        if (reset != 0) {
            register int xpos __asm__("$2");

            xpos = -0x90;
            *(short *)(set_ptr + 8) = xpos;
            *(short *)(set_ptr + 10) = y;
        }
        ot = *(void **)(kaze_menu_gs_ot_buf0_org + ot_ofs);
        prim = (char *)(prim_ofs + (int)scan);
        scan += 0x14;
        y += 0x18;
        set_ofs += 0x14;
        row++;
        AddPrim(ot, prim);
    } while (row < 5);

    {
        register void *ot __asm__("$4");
        register int final_ofs __asm__("$2");
        register char *ptr __asm__("$5");

        ot = *(void **)(kaze_menu_gs_ot_buf0_org + (page * 5) * 4);
        final_ofs = page << 3;
        ptr = kaze_balloon_name_row_tpage_prims + final_ofs;
        AddPrim(ot, ptr);
    }
}

/**
 * kaze_menu_two_row_sprites_draw_a210  (menu, 0x8005a210)
 *
 * Draws a 2-row menu sprite column at
 * kaze_registration_confirm_label_sprites (same x=-0x90/y step 0x18 layout
 * as the task list) plus DR_TPAGE kaze_registration_confirm_tpage_prims into
 * the menu OT.
 */

void kaze_menu_two_row_sprites_draw_a210(int idx, int reset_pos)
{
    extern void AddPrim(void *, void *);
    extern char kaze_menu_gs_ot_buf0_org[];
    char *reset_p;
    int prim_ofs;
    int i;
    int idx5;
    char *row_base;
    int ot_ofs;
    volatile int reset_stack;

    reset_stack = reset_pos;
    i = 0;
    row_base = kaze_registration_confirm_label_sprites;
    idx5 = idx * 5;
    prim_ofs = idx5 << 3;
    do {
        {
            register int c asm("$6");
            __asm__ volatile("lw\t%0,%1" : "=r"(c) : "m"(reset_stack));
            if (c != 0) {
                register int nv asm("$2");
                reset_p = (char *)((prim_ofs + i * 0x14) + (int)row_base);
                nv = -0x90;
                *(short *)(reset_p + 0x8) = nv;
                *(short *)(reset_p + 0xa) = i * 0x18 - 0x1c;
            }
        }
        ot_ofs = idx5 << 2;
        AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ot_ofs), (void *)(prim_ofs + ((int)row_base + i * 0x14)));
        i++;
    } while (i < 2);
    AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ot_ofs), kaze_registration_confirm_tpage_prims + idx * 8);
}

/* MATCH under gcc 2.7.2-psx -O2 (512B, byte-exact). Closes the 26/128 near-miss.
 * Three clusters, all found via cc1 -dS/-dl asm-diff triage:
 *  B (loop AddPrim setup order): move the loop increments (entry_base+=,q+=,i+=)
 *    AFTER the AddPrim call so gcc computes the arg0 OT-load + arg1 p BEFORE the
 *    post-increments, matching the target's [sll ot;lw a0;addu a1;addiu;addiu] order.
 *  C (tail tmp=base+idx): name the base in its OWN local computed first
 *    (`char *cb = kaze_balloon_editor_swatch_tiles; tmp = cb + arg0*12;`) so the symbol base
 *    materializes into v1 before the arg0*12 index chain, matching target.
 *  A (prologue if-block stores, value-vs-address reg swap): the two-statement
 *    in-place `a=base; a+=idx` coalesces the address into the BASE reg (v1),
 *    so the const store-value lands in v0 -- target has address in v0, value v1.
 *    Fix: split each block's base into its own scoped `cb` local AND pin the
 *    address `a` to $2 (register char *a asm("$2")). The pin lands the fresh
 *    address in v0 (reusing the dying offset reg like the target); the store
 *    value then naturally takes v1. Pin alone (no cb split) swaps base/offset;
 *    cb split alone leaves a coalesced with base -- both levers are required. */
extern u8 kaze_balloon_paint_color_r;
extern u8 kaze_balloon_paint_color_g;
extern u8 kaze_balloon_paint_color_b;
extern char kaze_balloon_editor_label_sprites[];
extern char kaze_balloon_editor_label_tpage_prims[];
extern char kaze_balloon_editor_preview_sprite[];
extern char kaze_balloon_editor_preview_tpage_prims[];
extern char kaze_balloon_editor_swatch_tiles[];

/**
 * kaze_menu_panel_grid_draw_a308  (menu, 0x8005a308)
 *
 * Draws an 8-sprite 2x4 menu panel grid at kaze_balloon_editor_label_sprites
 * (x=(i/4)*0xe0-0x90, y=(i%4)*0x18+8), an RGB-tinted prim from
 * kaze_balloon_paint_color_r..4c, and extra panel/TPAGE prims into the menu
 * OT.
 */

void kaze_menu_panel_grid_draw_a308(int arg0, int arg1)
{
    extern void AddPrim(void *, void *);
    extern char kaze_menu_gs_ot_buf0_org[];
    int i, q, amul, slot, ot_ofs;
    char *base, *entry_base, *p, *tmp;
    if (arg1 != 0) {
        register char *a asm("$2");
        { char *cb = kaze_balloon_editor_preview_sprite; a = cb + arg0 * 20; }
        *(short *)(a + 8) = 0x60;
        *(short *)(a + 10) = -0x58;
        { char *cb = kaze_balloon_editor_swatch_tiles; a = cb + arg0 * 12; }
        *(short *)(a + 8) = 0x68;
        *(short *)(a + 10) = -0x50;
    }
    i = 0;
    base = kaze_balloon_editor_label_sprites;
    entry_base = base;
    slot = arg0 * 5;
    amul = slot * 0x20;
    q = amul;
    do {
        if (arg1 != 0) {
            *(short *)(q + (int)base + 8) = (i / 4) * 0xe0 - 0x90;
            *(short *)(q + (int)base + 10) = (i % 4) * 0x18 + 8;
        }
        ot_ofs = slot << 2;
        p = (char *)(amul + (int)entry_base);
        AddPrim(*(void**)(kaze_menu_gs_ot_buf0_org + ot_ofs), p);
        entry_base += 0x14;
        q += 0x14;
        i += 1;
    } while (i < 8);
    AddPrim(*(void**)(kaze_menu_gs_ot_buf0_org + ot_ofs), kaze_balloon_editor_label_tpage_prims + arg0 * 8);
    {
        char *cb = kaze_balloon_editor_swatch_tiles;
        tmp = cb + arg0 * 12;
    }
    *(u8 *)(tmp + 4) = kaze_balloon_paint_color_r;
    *(u8 *)(tmp + 5) = kaze_balloon_paint_color_g;
    *(u8 *)(tmp + 6) = kaze_balloon_paint_color_b;
    AddPrim(*(void**)(kaze_menu_gs_ot_buf0_org + ot_ofs), kaze_balloon_editor_preview_sprite + ot_ofs);
    AddPrim(*(void**)(kaze_menu_gs_ot_buf0_org + ot_ofs), tmp);
    AddPrim(*(void**)(kaze_menu_gs_ot_buf0_org + ot_ofs), kaze_balloon_editor_preview_tpage_prims + arg0 * 8);
}

extern u8 kaze_balloon_color_edit_r[];
extern char kaze_color_edit_popup_window_sprite[];
extern char kaze_color_edit_popup_window_tpage_prims[];
extern char kaze_color_edit_channel_label_sprites[];
extern char kaze_color_edit_popup_cursor_tiles[];
extern void kaze_linef3_gradient_bar(int, int, int, int, int);

/**
 * kaze_menu_rgb_slider_panel_draw  (menu, 0x8005a508)
 *
 * Draws a 3-slider adjust panel: handle x =
 * kaze_balloon_color_edit_r[i]/2-0x4c, cursor box via
 * kaze_linef3_gradient_bar at row kaze_balloon_color_edit_channel, and a
 * preview prim whose r/g/b color bytes are copied from the three
 * kaze_balloon_color_edit_r..52 values.
 */

void kaze_menu_rgb_slider_panel_draw(int arg0)
{
    extern void AddPrim(void *, void *);
    extern char kaze_menu_gs_ot_buf0_org[];
    int i;
    int slider_ofs;
    int ot_ofs;
    int prim_ofs;
    char *base;
    char *preview_base;
    char *preview;
    int preview_ofs;
    u8 red;

    i = 0;
    slider_ofs = ((arg0 << 1) + arg0) << 4;
    do {
        *(u16 *)(kaze_color_edit_channel_label_x + slider_ofs) = (kaze_balloon_color_edit_r[i] >> 1) - 0x4C;
        i++;
        slider_ofs += 0x10;
    } while (i < 3);

    ot_ofs = (arg0 * 5) << 2;
    prim_ofs = ((arg0 << 1) + arg0) << 4;
    __asm__ volatile("" : : "r"(ot_ofs), "r"(prim_ofs));
    base = kaze_color_edit_channel_label_sprites;

    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + ot_ofs), prim_ofs + base);
    /* cc1 otherwise forms the second AddPrim a1 before loading a0. */
    __asm__ __volatile__(
        ".set\tnoreorder\n\t"
        "addiu\t$5,%1,0x10\n\t"
        "lw\t$4,kaze_menu_gs_ot_buf0_org(%0)\n\t"
        "jal\tAddPrim\n\t"
        "addu\t$5,%2,$5\n\t"
        ".set\treorder"
        :
        : "r"(ot_ofs), "r"(base), "r"(prim_ofs)
        : "$4", "$5", "memory");
    base += 0x20;
    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + ot_ofs), prim_ofs + base);

    kaze_linef3_gradient_bar(arg0, 0x58, kaze_balloon_color_edit_channel * 0x12 + 0x61, 0x88, 10);

    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + ot_ofs), kaze_color_edit_popup_window_sprite + ot_ofs);
    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + ot_ofs), kaze_color_edit_popup_window_tpage_prims + (arg0 << 3));

    preview_base = kaze_color_edit_popup_cursor_tiles;
    preview_ofs = arg0 << 4;
    red = kaze_balloon_color_edit_r[0];
    preview = preview_base + preview_ofs;
    preview[4] = red;
    preview[5] = kaze_balloon_color_edit_r[1];
    preview[6] = kaze_balloon_color_edit_r[2];
    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + ot_ofs), preview);
}

/**
 * kaze_menu_prim_pair_draw_a6a4  (menu, 0x8005a6a4)
 *
 * Adds one menu panel prim (kaze_balloon_paint_cursor_sprites+buf*0x10) and
 * its DR_TPAGE (kaze_balloon_paint_cursor_tpages) to the menu OT for the
 * current buffer.
 */

void kaze_menu_prim_pair_draw_a6a4(int buf)
{
    extern void AddPrim(int, char *);
    extern char kaze_balloon_paint_cursor_tpages[];
    extern char kaze_balloon_paint_cursor_sprites[];
    extern MenuTitlebarSlot kaze_menu_gs_ot_buf0_org[];
    AddPrim(kaze_menu_gs_ot_buf0_org[buf].ot, kaze_balloon_paint_cursor_sprites + buf * 0x10);
    AddPrim(kaze_menu_gs_ot_buf0_org[buf].ot, kaze_balloon_paint_cursor_tpages + buf * 8);
    return;
}

extern s8 kaze_task_name_letter0_table;

/**
 * kaze_task_name_grid_draw  (menu, 0x8005a71c)
 *
 * Draws the 5x3 task-name letter grid: reads 3-char codes per task from
 * kaze_task_name_letter0_table, maps code to glyph UV
 * ((c%8)*0x10-0x80,(c/8)*0x10-0x80) in SPRTs at
 * kaze_balloon_entry_grid_sprites, code 0xfe rendered as glyph 0x25; skips
 * codes >= 0x27.
 */

void kaze_task_name_grid_draw(int arg0, int arg1)
{
    extern char kaze_balloon_entry_grid_tpages;
    extern Prim kaze_balloon_entry_grid_sprites;
    extern Slot kaze_menu_gs_ot_buf0_org[];
    extern void AddPrim(void *, void *);
  register int page asm("$21");
  register int y asm("$22");
  unsigned int new_var;
  int new_var2;
  register s8 *chars asm("$23");
  int fp;
  int row;
  int new_var3;
  Prim *cbase;
  Prim *rowbase;
  int page15;
  page = arg0;
  row = 0;
  new_var3 = page;
  page15 = 5 * (3 * new_var3);
  cbase = &kaze_balloon_entry_grid_sprites;
  new_var2 = -0x42;
  rowbase = cbase;
  y = -0x1C;
  fp = 0;
  chars = &kaze_task_name_letter0_table;
  do
  {
    register int col asm("$20");
    register int coloff asm("$19");
    register int x asm("$18");
    register s8 *nm asm("$17");
    register int s0 asm("$16");
    Prim *rp;
    register int pageoff asm("$2");
    register int pg asm("$10");
col = 0;
    { pg = page15; asm volatile("":"=r"(pg):"0"(pg)); coloff = 0; x = new_var2; nm = chars; pageoff = pg * 0x10; }
    asm volatile("" : : "r"(fp));
    asm volatile("" : : "r"(fp));
    rp = (Prim *) (pageoff + ((int) rowbase));
    s0 = pageoff + fp;
    do
    {
      int c;
      c = *nm;
      if (c == (-2))
      {
        c = 0x25;
        goto draw;
      }
      if (((unsigned int) c) < 0x27U)
      {
        draw:
        {
          pg = arg1;
          asm volatile("":"=r"(pg):"0"(pg));
          if (pg != 0)
          {
            Prim *wp = (Prim *) (s0 + ((int) cbase));
            wp->unk8 = x;
            wp->unkA = y;
          }
          {
            Prim *wp = (Prim *) (s0 + ((int) cbase));
            wp->unkC = ((c - ((c / 8) * 8)) * 0x10) - 0x80;
            wp->unkD = ((c / 8) * 0x10) - 0x80;
          }
          AddPrim(kaze_menu_gs_ot_buf0_org[page].ot, ((char *) rp) + coloff);
        }

      }
      new_var = 0x10;
      s0 += new_var;
      coloff += new_var;
      x += 0xC;
      col += 1;
      nm += 1;
    }
    while (col < 3);
    rowbase = (Prim *) (((char *) rowbase) + 0x30);
    asm volatile("" : : "r"(rowbase));
    y += 0x18;
    fp += 0x30;
    row += 1;
    asm volatile("" : : "r"(row));
    chars += 3;
  }
  while (row < 5);
  AddPrim(kaze_menu_gs_ot_buf0_org[page].ot, (&kaze_balloon_entry_grid_tpages) + (page << 3)); asm volatile("":: "r"(page));
}

extern char kaze_task_glyph_dobjs[];
extern char kaze_task_glyph_coordinates[];
extern char kaze_task_glyph_rotation_y[];
extern char kaze_task_glyph_rotation_z[];
extern void GsMapModelingData(ulong *);
extern void GsInitCoordinate2(void *, void *);
extern void GsLinkObject4(int, void *, int);

/**
 * kaze_task_glyph_models_link  (menu, 0x8005a8e0)
 *
 * Maps the letter TMD at kaze_menu_task_model_ptr via GsMapModelingData,
 * links its kaze_task_glyph_count glyph objects into GsDOBJ2 slots
 * kaze_task_glyph_dobjs with GsCOORDINATE2s at kaze_task_glyph_coordinates,
 * and zeroes the rotation table kaze_task_glyph_rotation_x.
 */

void kaze_task_glyph_models_link(void)
{
    extern char kaze_task_glyph_rotation_x[];
    int zero_off;
    int obj_off;
    int *model;
    int i;
    int limit;
    char *obj;
    char *coord;
    volatile int pad[2];

    model = (int *)kaze_menu_task_model_ptr;
    i = 0;
    model++;
    GsMapModelingData((ulong *)model++);
    kaze_task_glyph_count = *model;
    model++;
    if (kaze_task_glyph_count > 0) {
        zero_off = 0;
        obj = kaze_task_glyph_dobjs;
        obj_off = 0;
        coord = kaze_task_glyph_coordinates;
        do {
            GsInitCoordinate2(0, coord);
            GsLinkObject4((int)model, obj, i);
            *(int *)(kaze_task_glyph_dobjs + obj_off) = 0;
            *(char **)(kaze_task_glyph_dobj_coord_ptr + obj_off) = coord;
            __asm__ volatile("" : : : "memory");
            *(short *)(kaze_task_glyph_rotation_x + zero_off) = 0;
            *(short *)(kaze_task_glyph_rotation_y + zero_off) = 0;
            *(short *)(kaze_task_glyph_rotation_z + zero_off) = 0;
            zero_off += 8;
            obj += 0x10;
            obj_off += 0x10;
            __asm__ volatile("" : : : "memory");
            limit = kaze_task_glyph_count;
            __asm__ volatile("" : "=r"(limit) : "0"(limit));
            i++;
            coord += 0x50;
        } while (i < limit);
    }
}

extern int kaze_task_carousel_previous_index;
extern int kaze_task_carousel_selected_index;
extern int kaze_task_carousel_visible_index;
extern int kaze_task_carousel_input_index;
extern int kaze_task_carousel_transition_ticks;
extern int kaze_task_carousel_transition_duration;
extern int kaze_task_carousel_exit_timer;
extern volatile unsigned int kaze_task_carousel_angle_ring[];
extern u16 kaze_task_name_letter_rotation_x[];

/**
 * kaze_task_carousel_state_init  (menu, 0x8005a9ec)
 *
 * Initializes the letter-carousel state: builds the 0x29-entry angle ring
 * kaze_task_carousel_angle_ring (step
 * kaze_task_carousel_angle_step=0x63e70), resets index/rotation vars
 * kaze_task_carousel_visible_index/518/520/524/528/52c/530/538, clears
 * rotation tables, calls kaze_task_carousel_constants_init.
 */

void kaze_task_carousel_state_init(void)
{
    extern u16 kaze_task_glyph_rotation_x[];
    register int c __asm__("$2");
    register int angle_accum __asm__("$4");
    int j;
    volatile u16 *entry_ptr;
    register volatile unsigned int *ring_ptr __asm__("$5");
    register int i __asm__("$6");
    register int inc __asm__("$7");
    register int base __asm__("$8");

    c = 0x63E70;
    i = 0;
    inc = 0x63E70;
    __asm__("" : "=r"(inc) : "0"(inc));
    base = 0x800;
    ring_ptr = kaze_task_carousel_angle_ring;
    angle_accum = 0;
    kaze_task_carousel_previous_index = -1;
    kaze_task_letter_count = 0;
    kaze_task_menu_camera_bob_phase = 0;
    kaze_task_carousel_selected_index = 0;
    kaze_task_carousel_visible_index = 0;
    kaze_task_carousel_input_index = 0;
    kaze_task_carousel_transition_ticks = 0;
    kaze_task_carousel_transition_duration = 0xC;
    kaze_task_carousel_exit_timer = 0;
    kaze_task_carousel_angle_step = c;
    do {
        *ring_ptr = (base - (angle_accum >> 0xC)) & 0xFFF;
        ring_ptr = ring_ptr + 1;
        i = i + 1;
        angle_accum = angle_accum + inc;
    } while (i < 0x29);
    entry_ptr = kaze_task_glyph_rotation_x;
    j = 0;
    do {
        entry_ptr[2] = 0;
        entry_ptr[1] = 0;
        *(volatile u16 *)((int)kaze_task_glyph_rotation_x + j) = 0;
        j = j + 8;
        entry_ptr = entry_ptr + 4;
    } while (j < 0x148);
    entry_ptr = kaze_task_name_letter_rotation_x;
    j = 0;
    do {
        entry_ptr[2] = 0;
        entry_ptr[1] = 0;
        *(volatile u16 *)((int)kaze_task_name_letter_rotation_x + j) = 0;
        j = j + 8;
        entry_ptr = entry_ptr + 4;
    } while (j < 0x18);
    kaze_task_carousel_constants_init();
}
