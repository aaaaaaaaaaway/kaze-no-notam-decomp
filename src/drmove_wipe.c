#include "types.h"
#include "kaze_syms.h"
extern char kaze_tim_load_error_msg[];
extern u8 kaze_drmove_wipe_height_table_0[];
extern u8 kaze_drmove_wipe_height_table_1[];
extern u8 kaze_drmove_wipe_height_table_2[];
extern u8 kaze_drmove_wipe_height_table_3[];
extern char kaze_fade_overlay_tpage_table[];
extern char kaze_drmove_wipe_grid_table[];
extern void SetPolyF4(void *);
extern void printf(char *);
typedef struct {
    short x, y, w, h;
} RECT;
typedef struct { s16 f0, f2, f4, f6; s32 f8, fc, f10; } DrawMoveArg;
typedef struct { s16 x, y, w, h; } RECT__1;
typedef struct {
    int prim;
    int pad[4];
} Slot;
typedef struct {
    short x;
    short y;
    short w;
    short h;
} RECT__2;
typedef struct {
    unsigned int pmode;
    short px;
    short py;
    short pw;
    short ph;
    unsigned char *pixel;
    short cx;
    short cy;
    short cw;
    short ch;
    unsigned char *clut;
} GsIMAGE;

/**
 * kaze_drmove_pair_setup_a064  (render, 0x8004a064)
 *
 * SetDrawMove sets two DR_MOVE frame-copy primitives at
 * kaze_drmove_pair_table for a 0x140x0xf0 rect at (x,y).
 */

void kaze_drmove_pair_setup_a064(short x, short y)
{
    extern void SetDrawMove(void *, RECT *, int, int);
    extern char kaze_drmove_pair_table[];
    int i;
    RECT rect;

    i = 0;
    rect.w = 0x140;
    rect.x = x;
    rect.y = y;
    rect.h = 0xF0;
    do {
        SetDrawMove(kaze_drmove_pair_table + i * 24, &rect, 0, ((i + 1) & 1) * 0xF0);
        i++;
    } while (i < 2);
}

/**
 * kaze_drmove_addprim_a0ec  (render, 0x8004a0ec)
 *
 * AddPrims a DR_MOVE from kaze_drmove_pair_table into OT bucket
 * (&kaze_menu_gs_ot_buf0_org)[idx*5].
 */

void kaze_drmove_addprim_a0ec(int idx, int n)
{
    extern void AddPrim(int, u8 *);
    extern u8 kaze_drmove_pair_table;
    extern int kaze_menu_gs_ot_buf0_org;
    AddPrim((&kaze_menu_gs_ot_buf0_org)[idx * 5] + n * 4, &kaze_drmove_pair_table + idx * 0x18);
}

/**
 * kaze_drmove_grid_setup  (render, 0x8004a140)
 *
 * Builds a 20x15 grid of DR_MOVE tile primitives at
 * kaze_drmove_wipe_grid_table for both frame buffers (screen wipe tiles).
 */

void kaze_drmove_grid_setup(void) {
    extern void SetDrawMove(void *, void *, s32, s32);
    DrawMoveArg buf;
    register s32 col   __asm__("$16");
    register s32 x     __asm__("$17");
    register s32 pofs  __asm__("$18");
    register s32 nxt   __asm__("$19");
    register s32 row   __asm__("$20");
    register s32 y     __asm__("$21");
    register s32 rofs  __asm__("$22");
    register s32 page  __asm__("$23");
    register s32 size __asm__("$9");
    register s32 base_page __asm__("$8");
    s32 page_ofs, base_row;
    char *dst;
    page = 0; size = 0x10; page_ofs = 0;
    do {
        row = 0; rofs = 0;
        do {
            col = 0; y = row * 0x10; base_page = page_ofs; base_row = rofs; pofs = 0; x = 0x280;
            do {
                dst = kaze_drmove_wipe_grid_table + base_row;
                __asm__ ("addu %0,%1,%0" : "=r"(dst) : "r"(base_page), "0"(dst));
                nxt = page + 1;
                buf.f0 = x; buf.f2 = y; buf.f4 = size; buf.f6 = size;
                buf.f8 = base_row; buf.fc = base_page; buf.f10 = size;
                SetDrawMove(dst + pofs, &buf, col * 0x10, y + (nxt & 1) * 0xF0);
                base_row = buf.f8; base_page = buf.fc; size = buf.f10;
                pofs += 0x18; col += 1; x += 0x10;
            } while (col < 0x14);
            row += 1; rofs += 0x1E0;
        } while (row < 0xF);
        page = nxt; page_ofs += 0x1C20;
    } while (page < 2);
}

/**
 * kaze_drmove_wipe_tiles  (render, 0x8004a260)
 *
 * Builds and AddPrims a wipe/dissolve grid of DR_MOVE tiles clipped by per-
 * column height tables
 * UNK_8008dd58/de84/dfb0/kaze_drmove_wipe_height_table_0.
 */

void kaze_drmove_wipe_tiles(int buf_arg, int counter, int mode_arg)
{
    extern void *kaze_menu_gs_ot_buf0_org[];
    extern void AddPrim(void *, void *);
    extern void SetDrawMove(void *, RECT__1 *, int, int);
    register int buf asm("$18") = buf_arg;
    register int mode asm("$20");
    register int col_x asm("$17");
    register int col_i asm("$19");
    register int row_off asm("$22");
    register int buf_off asm("$21");
    int row_y;
    register u8 *mask_ptr asm("$23");
    register char *s0 asm("$16");
    register int unused_i1 asm("$9");
    register u8 *init_mask asm("$9");
    register int outer_next asm("$9");
    register int outer_more asm("$2");
    u8 *height_tbl;
    unsigned int height_val;
    register int y asm("$7");
    int unused_i4;
    register int draw_x asm("$6");
    register int width asm("$2");
    register RECT__1 *rect asm("$5");
    register int l40_raw asm("$9");
    int init_row;
    RECT__1 wipe_rect;
    struct { int l58; int p0; int l50; int p1; int l48; int p2;
             int l40; int p3; int l38; int p4; u8 *l30; int p5; } v;

    /* Prologue emission-order fix (LAWFUL, zero-code launders only):
       target sets $s2=buf, $s4=mode, then v.l48, then v.l30, then $s6=row_off.
       Tie buf's use to mode so mode's arg-copy emits first; a pure-input barrier
       forces the v.l48 sll-chain to complete before the v.l30 lui/addiu;
       chained ties keep row_off=0 last. Matches prologue bytes exactly. */
    mode = mode_arg;
    __asm__("" : "=r"(buf) : "0"(buf), "r"(mode));
    init_row = buf * 0xe0;
    __asm__("" :: "r"(init_row));
    init_mask = kaze_drmove_wipe_height_table_0;
    __asm__("" : "=r"(init_mask) : "0"(init_mask), "r"(init_row));
    row_off = 0;
    __asm__("" : "=r"(row_off) : "0"(row_off), "r"(init_mask));
    v.l58 = counter;
    v.l50 = 0;
    v.l48 = init_row;
    v.l38 = 0;
    v.l30 = init_mask;
    { int _mt = mode; __asm__("" :: "r"(_mt) : "memory"); }
    /* Force counter to a stack home (target keeps it at sp+0x18, reloaded each
       inner iteration) instead of letting gcc borrow the $fp/row_y slot. */
    __asm__("" : "=m"(v.l58) : "m"(v.l58));
    do {
        col_i = 0;
        buf_off = 0;
        {
            register int raw asm("$9");
            raw = v.l50;
            __asm__("" : "=r"(raw) : "0"(raw));
            col_x = 0;
            row_y = raw * 0x10;
        }
        { int _mt = row_y; __asm__("" :: "r"(_mt) : "memory"); }
        {
            register int raw asm("$9");
            register int sum asm("$2");
            raw = v.l48;
            mask_ptr = v.l30;
            __asm__("" : "=r"(raw) : "0"(raw), "r"(mask_ptr));
            sum = raw + buf;
            __asm__("" : "=r"(sum) : "0"(sum));
            v.l40 = sum * 0x20;
        }
        do {
            switch (mode) {
            case 0:
                height_val = (unsigned int)*mask_ptr;
                break;
            case 1:
                height_tbl = kaze_drmove_wipe_height_table_1 + row_off;
                goto LAB_8004a360;
            case 2:
                height_tbl = kaze_drmove_wipe_height_table_2 + row_off;
                goto LAB_8004a360;
            case 3:
                height_tbl = kaze_drmove_wipe_height_table_3 + row_off;
LAB_8004a360:
                height_val = (unsigned int)(u8)height_tbl[col_i];
                break;
            default:
                height_val = 0;
                break;
            }
            {
                register int limit asm("$9");
                register int delta asm("$8");
                limit = v.l58;
                __asm__("" : "=r"(limit) : "0"(limit));
            if ((int)height_val < limit) {
                delta = limit - height_val;
                __asm__("" : "=r"(delta) : "0"(delta));
                if (delta < 8) {
                    register int rect_y asm("$3");
                    register int rect_x asm("$2");
                    register int l38_raw asm("$9");
                    draw_x = col_x + delta;
                    s0 = kaze_drmove_wipe_grid_table;
                    rect = &wipe_rect;
                    rect_y = row_y + delta;
                    __asm__("" : "=r"(rect_y) : "0"(rect_y));
                    y = rect_y + (buf + 1U & 1) * 0xf0;
                    rect_x = delta + 0x280;
                    __asm__("" : "=r"(rect_x) : "0"(rect_x));
                    l38_raw = v.l38;
                    __asm__("" : "=r"(l38_raw) : "0"(l38_raw));
                    rect_x = col_x + rect_x;
                    __asm__("" : "=r"(rect_x) : "0"(rect_x));
                    wipe_rect.y = rect_y;
                    rect_y = delta * 2;
                    __asm__("" : "=r"(rect_y) : "0"(rect_y));
                    wipe_rect.x = rect_x;
                    __asm__("" : "=m"(wipe_rect.x) : "m"(wipe_rect.x));
                    width = 0x10;
                    s0 += l38_raw;
                    l40_raw = v.l40;
                    __asm__("" : "=r"(l40_raw) : "0"(l40_raw));
                    width = width - rect_y;
                    goto LAB_8004a420;
                }
            } else {
                register int l38_raw asm("$9");
                register int else_x asm("$2");
                s0 = kaze_drmove_wipe_grid_table;
                rect = &wipe_rect;
                draw_x = col_x;
                __asm__("":"=r"(col_x):"0"(col_x));
                y = row_y + (buf + 1U & 1) * 0xf0;
                { int _mt = col_x; __asm__("" :: "r"(_mt) : "memory"); }
                l38_raw = *(volatile int *)&v.l38;
                __asm__ volatile("" : : "r"(l38_raw));
                else_x = col_x + 0x280;
                __asm__("" : : "r"(else_x));
                wipe_rect.x = else_x;
                __asm__("" : "=m"(wipe_rect.x) : "m"(wipe_rect.x));
                s0 += l38_raw;
                l40_raw = v.l40;
                __asm__("" : "=r"(l40_raw) : "0"(l40_raw));
                width = 0x10;
                wipe_rect.y = row_y;
LAB_8004a420:
                wipe_rect.w = width;
                wipe_rect.h = width;
                __asm__("" : "=m"(wipe_rect.w), "=m"(wipe_rect.h)
                           : "m"(wipe_rect.w), "m"(wipe_rect.h));
                s0 = (char *)((unsigned int)l40_raw + (unsigned int)s0);
                s0 += buf_off;
                SetDrawMove((void *)s0, rect, draw_x, y);
                {
                    register int off asm("$2");
                    off = buf * 0x14;
                    __asm__("" : "=r"(off) : "0"(off));
                    AddPrim(*(void **)((char *)kaze_menu_gs_ot_buf0_org + off), (void *)s0);
                }
            }
            }
            buf_off = buf_off + 0x18;
            col_x = col_x + 0x10;
            col_i = col_i + 1;
            mask_ptr = mask_ptr + 1;
        } while (col_i < 0x14);
        { register int raw asm("$9"); raw = v.l38; raw += 0x1e0; v.l38 = raw; }
        { register u8 *raw asm("$9"); raw = v.l30; raw += 0x14; v.l30 = raw; }
        __asm__ volatile("" : : : "memory");
        outer_next = v.l50;
        row_off = row_off + 0x14;
        outer_next = outer_next + 1;
        outer_more = outer_next < 0xf;
        v.l50 = outer_next;
    } while (outer_more);
}

extern int GetTPage(int, int, int, int);
extern void SetDrawTPage(void *, int, int, int);
extern void SetSemiTrans(void *, int);
extern void SetShadeTex(void *, int);

/**
 * kaze_semitrans_quad_setup  (render, 0x8004a4dc)
 *
 * SetPolyF4 semi-transparent quad plus DR_TPAGE at
 * kaze_fade_overlay_quad_table/7230 with GetTPage(abr) (fade overlay quad).
 */

void kaze_semitrans_quad_setup(int abr)
{
    extern char kaze_fade_overlay_quad_table[];
    int i;
    int prim_ofs;
    int x0;
    int y0;
    int x1;
    int y1;
    int idx;
    char *p;
    int tpage;

    i = 0;
    x0 = -0xA0;
    y0 = -0x78;
    x1 = 0xA0;
    y1 = 0x78;
    prim_ofs = 0;
    do {
        tpage = GetTPage(1, abr, 0, 0);
        SetDrawTPage(kaze_fade_overlay_tpage_table + i * 8, 0, 1, tpage & 0xFFFF);
        p = kaze_fade_overlay_quad_table + prim_ofs;
        SetPolyF4(p);
        SetSemiTrans(p, 1);
        SetShadeTex(p, 1);
        prim_ofs += 0x18;
        i++;
        idx = i < 2;
        *(short *)(p + 0x8) = x0;
        *(short *)(p + 0xA) = y0;
        *(short *)(p + 0xC) = x1;
        *(short *)(p + 0xE) = y0;
        *(short *)(p + 0x10) = x0;
        *(short *)(p + 0x12) = y1;
        *(short *)(p + 0x14) = x1;
        *(short *)(p + 0x16) = y1;
        p[4] = 0;
        p[5] = 0;
        p[6] = 0;
    } while (idx);
}

/**
 * kaze_quad_addprim_color  (render, 0x8004a5ec)
 *
 * Sets RGB of overlay quad kaze_fade_overlay_quad_table and AddPrims the
 * quad+tpage into the OT bucket.
 */

void kaze_quad_addprim_color(int idx, int color)
{
    extern int AddPrim(void *, void *);
    extern u8 kaze_fade_overlay_quad_table[];
    extern Slot kaze_menu_gs_ot_buf0_org[];
    u8 *base;
    u8 *p;

    base = kaze_fade_overlay_quad_table;
    p = base + idx * 0x18;
    p[4] = color;
    p[5] = color;
    p[6] = color;
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[idx].prim, p);
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[idx].prim, kaze_fade_overlay_tpage_table + idx * 8);
}

/**
 * kaze_quad_addprim_texpage  (render, 0x8004a67c)
 *
 * Sets quad color, GetTPage/SetDrawTPage, and AddPrims the quad+tpage into
 * the OT bucket.
 */

void kaze_quad_addprim_texpage(int idx, int r, int g, int b, int abr)
{
    extern void AddPrim(void *, void *);
    extern char kaze_menu_gs_ot_buf0_org[];
    extern u8 kaze_fade_overlay_quad_table[];
    int ot_ofs;
    u8 *base;
    u8 *p;
    char *tpage;
    register int t __asm__("$3");

    t = abr;
    base = kaze_fade_overlay_quad_table;
    p = base + idx * 0x18;
    p[5] = g;
    g = 0;
    p[6] = b;
    b = 0;
    p[4] = r;
    t = GetTPage(0, t, g, b);
    tpage = kaze_fade_overlay_tpage_table + idx * 8;
    SetDrawTPage(tpage, 0, 1, t & 0xFFFF);
    ot_ofs = idx * 4;
    ot_ofs += idx;
    ot_ofs *= 4;
    AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ot_ofs), p);
    AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ot_ofs), tpage);
}

extern void GsGetTimInfo(ulong *, GsIMAGE *);
extern void LoadImage(RECT__2 *, unsigned char *);
extern void DrawSync(int);

/**
 * kaze_tim_load_to_vram  (render, 0x8004a74c)
 *
 * Validates TIM magic 0x10, GsGetTimInfo then LoadImage of pixel data and
 * CLUT to VRAM at optional override coords.
 */

void kaze_tim_load_to_vram(int *tim, short px, short py, short cx, short cy)
{
    RECT__2 rect;
    GsIMAGE image;

    if (*tim != 0x10) {
        printf(kaze_tim_load_error_msg);
        return;
    }
    GsGetTimInfo((ulong *)(tim + 1), &image);
    if (px != -1) {
        image.px = px;
    }
    if (py != -1) {
        image.py = py;
    }
    rect.x = image.px;
    rect.y = image.py;
    rect.w = image.pw;
    rect.h = image.ph;
    LoadImage(&rect, image.pixel);
    DrawSync(0);
    if ((image.pmode & 8) != 0) {
        if (cx != -1) {
            image.cx = cx;
        }
        if (cy != -1) {
            image.cy = cy;
        }
        rect.x = image.cx;
        rect.y = image.cy;
        rect.w = image.cw;
        rect.h = image.ch;
        LoadImage(&rect, image.clut);
        DrawSync(0);
    }
}
