#include "types.h"
#include "kaze_syms.h"
extern char kaze_tim_load_error_msg[];
extern int kaze_linef3_gradient_bar_phase;
extern void printf(char *);
extern int rand(void);
typedef struct {
    short x;
    short y;
    short w;
    short h;
} RECT;
typedef struct {
    char pad[24];
} LINE_F3;
typedef struct {
  char pad[4];
  unsigned char r0, g0, b0, pad2;
  short x0, y0, x1, y1, x2, y2;
  char pad3[4];
} LINE_F3__1;
typedef struct {
    uint pmode;
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

extern int GsGetActiveBuff(void);
extern void MoveImage(RECT *, int, int);
extern void DrawSync(int);

/**
 * kaze_backbuffer_copy_offscreen  (render, 0x8004a904)
 *
 * GsGetActiveBuff + MoveImage copies the active 0x140x0xf0 back buffer to
 * offscreen VRAM (0x280,0).
 */

void kaze_backbuffer_copy_offscreen(void)
{
    int buf;
    RECT r;

    buf = GsGetActiveBuff();
    r.x = 0;
    r.y = buf * 0xF0;
    r.w = 0x140;
    r.h = 0xF0;
    MoveImage(&r, 0x280, 0);
    DrawSync(0);
}

extern void SetLineF3(LINE_F3 *);
extern void SetShadeTex(LINE_F3 *, int);

/**
 * kaze_linef3_init  (render, 0x8004a960)
 *
 * SetLineF3/SetShadeTex initializes 4 LINE_F3 gradient bars at
 * kaze_linef3_gradient_bar_table_a/7310; resets phase
 * kaze_linef3_gradient_bar_phase.
 */

void kaze_linef3_init(void)
{
    extern LINE_F3 kaze_linef3_gradient_bar_table_a[];
    extern LINE_F3 kaze_linef3_gradient_bar_table_b[];
    int i;
    LINE_F3 *q;
    LINE_F3 *p;

    i = 0;
    q = kaze_linef3_gradient_bar_table_b;
    p = kaze_linef3_gradient_bar_table_a;
    do {
        SetLineF3(p);
        SetShadeTex(p, 0);
        SetLineF3(q);
        SetShadeTex(q, 0);
        q++;
        i++;
        p++;
    } while (i < 2);
    kaze_linef3_gradient_bar_phase = 0;
}

extern char kaze_menu_gs_ot_buf0_org[];
extern int rsin(int);
extern void AddPrim(void *, void *);
/*
 * Fills the two LINE_F3__1 halves of an rsin-animated gradient bar and AddPrims
 * them.  De-islanded: the former 4-insn coord island is replaced by scoped
 * register pins (x0/y0/x1 -> $t1/$t0/$a3) plus a zero-code reader-anchor
 * `asm("" :: "r"(x0),"r"(y0),"r"(x1))` that reproduces the island's
 * scheduling-barrier effect (without it gcc floats the first two coord
 * stores ahead of the h/phase/right-pointer block, an 8-word swap).
 * The single-insn `mult`/`mfhi` forms remain: each /0x11 divide must be split
 * so the colour stores interleave into the multiply latency (leaving the abs
 * branch delay slot as a nop) and both divides share one 0x78787879 magic
 * hoisted to $s4 -- native `/0x11` cannot reproduce that store interleave.
 * Declaration and statement order below are load-bearing for register alloc.
 */

/**
 * kaze_linef3_gradient_bar  (render, 0x8004a9e8)
 *
 * Fills a LINE_F3 gradient bar's vertices, sets rsin-animated color from
 * phase kaze_linef3_gradient_bar_phase, and AddPrims it.
 */

void kaze_linef3_gradient_bar(int page, int x, int y, int w, volatile unsigned int h)
{
    extern LINE_F3__1 kaze_linef3_gradient_bar_table_b[];
    extern LINE_F3__1 kaze_linef3_gradient_bar_table_a[];
  int magic;
  register int off asm("$17");
  LINE_F3__1 *left;
  register int hi asm("$10");
  register int q asm("$3");
  LINE_F3__1 *right;
  register int s0 asm("$16");
  int phase;
  int color, r, sign;
  register int x0 asm("$9");
  register int y0 asm("$8");
  register int x1 asm("$7");
  short y1;
  magic = 0x78787879;
  off = page * 0x18;
  left = kaze_linef3_gradient_bar_table_a;
  left = (LINE_F3__1 *)((char *)left + off);
  x0 = x - 0xA0;
  y0 = y - 0x78;
  x1 = w + x - 0xA1;
  __asm__("" :: "r"(x0), "r"(y0), "r"(x1));
  y1 = (h + y) - 0x79;
  right = (LINE_F3__1 *)((char *)kaze_linef3_gradient_bar_table_b + off);
  phase = kaze_linef3_gradient_bar_phase;
  left->x0 = x0;
  left->y0 = y0;
  left->x1 = x1;
  left->y1 = y0;
  left->x2 = x1;
  left->y2 = y1;
  right->x0 = x1;
  right->y0 = y1;
  right->x1 = x0;
  right->y1 = y1;
  right->x2 = x0;
  right->y2 = y0;
  r = rsin(phase);
  __asm__ volatile("mult %0,%1" : : "r"(r), "r"(magic) : "hi", "lo");
  s0 = 0x80;
  sign = r >> 31;
  left->g0 = 0;
  left->b0 = s0;
  __asm__ volatile("mfhi %0" : "=r"(hi));
  q = (hi >> 3) - sign;
  if (q < 0) q = -q;
  left->r0 = q;
  r = rsin(kaze_linef3_gradient_bar_phase);
  __asm__ volatile("mult %0,%1" : : "r"(r), "r"(magic) : "hi", "lo");
  sign = r >> 31;
  right->b0 = s0;
  right->g0 = 0;
  s0 = (page * 5) * 4;
  __asm__ volatile("mfhi %0" : "=r"(hi));
  q = (hi >> 3) - sign;
  if (q < 0) q = -q;
  right->r0 = q;
  AddPrim(*((void **) (kaze_menu_gs_ot_buf0_org + s0)), left);
  AddPrim(*((void **) (kaze_menu_gs_ot_buf0_org + s0)), right);
  kaze_linef3_gradient_bar_phase = (kaze_linef3_gradient_bar_phase + 0x20) & 0xFFF;
}

/**
 * kaze_rand_range  (library, 0x8004ab6c)
 *
 * Returns a random integer between the two given bounds, inclusive, computed
 * as (rand()*rand()) mod the span plus the lower bound.
 */

int kaze_rand_range(int lo, int hi)
{
    int r1;
    int r2;
    int range;

    range = (hi - lo) + 1;
    if (range != 0) {
        r1 = rand();
        r2 = rand();
        range = (r1 * r2) % range + lo;
    }
    return range;
}

/**
 * kaze_uint_to_decimal_string  (library, 0x8004ac04)
 *
 * Converts a uint to a fixed-width decimal ASCII string with optional
 * leading-zero-to-space padding.
 */

void kaze_uint_to_decimal_string(u32 val, int len, int flag, char *buf)
{
    int zeros;
    int i;
    u32 q, r;
    char *end;
    char space;
    int j;

    zeros = 0;
    buf[len] = 0;
    i = len - 1;
    if (i >= 0) {
        do {
            r = val % 10;
            q = val / 10;
            val = q;
            if (r != 0) {
                zeros = 0;
            } else {
                zeros++;
            }
            buf[i] = r + 0x30;
            i--;
        } while (i >= 0);
    }
    if (flag != 0 && zeros > 0) {
        space = 0x20;
        j = len - 1;
        end = (char *)(j + (int)buf);
        q = zeros + (u32)buf;
    top:
        if (buf == end) {
            goto out;
        }
        *buf = space;
        buf++;
        if ((int)buf < (int)q) {
            goto top;
        }
    out:;
    }
}

extern unsigned int kaze_input_current_a;
extern unsigned int kaze_input_current_b;
extern unsigned int kaze_input_current_c;
extern unsigned int kaze_menu_input_prev_mask;
extern unsigned int kaze_input_edge;
extern int kaze_input_hold_timer;

/**
 * kaze_input_edge_detect  (core, 0x8004acac)
 *
 * Polls the pad (kaze_pad_mask_update), latches
 * kaze_input_current_a/kaze_input_current_b, computes newly-pressed mask
 * kaze_input_edge and hold counter kaze_input_hold_timer.
 */

void kaze_input_edge_detect(void)
{
    kaze_pad_mask_update();
    kaze_input_current_c = kaze_input_current_a;
    kaze_menu_input_prev_mask = kaze_input_current_b;
    if (kaze_input_current_a != kaze_input_current_b) {
        kaze_input_hold_timer = 0;
        kaze_input_edge = kaze_input_current_a & ~kaze_input_current_b;
    } else {
        kaze_input_edge = 0;
        kaze_input_hold_timer = kaze_input_hold_timer + 1;
    }
}

extern char *strcat(char *, char *);

/**
 * kaze_file_load_by_name  (cd_stream, 0x8004ad2c)
 *
 * Prepends drive prefix kaze_file_path_drive_prefix to a path and loads that
 * file from CD via kaze_cd_load_file_alloc.
 */

void kaze_file_load_by_name(char *name)
{
    extern void kaze_cd_load_file_alloc(u16 *);
    extern u16 kaze_file_path_drive_prefix;
    u16 buf[64];

    if (*(u8 *)name != '\\') {
        buf[0] = kaze_file_path_drive_prefix;
    }
    strcat((char *)buf, name);
    kaze_cd_load_file_alloc(buf);
}

extern void kaze_cd_load_tim_upload(void *, int, int, int, int);

/**
 * kaze_tim_display_by_name  (render, 0x8004ad78)
 *
 * Loads a TIM file by name (prefix kaze_file_path_drive_prefix) and displays
 * it at default position via kaze_cd_load_tim_upload.
 */

void kaze_tim_display_by_name(char *name)
{
    extern u16 kaze_file_path_drive_prefix;
    u16 buf[64];

    if (*(u8 *)name != '\\') {
        buf[0] = kaze_file_path_drive_prefix;
    }
    strcat((char *)buf, name);
    kaze_cd_load_tim_upload(buf, -1, -1, -1, -1);
}

extern void GsGetTimInfo(ulong *, GsIMAGE *);
extern void LoadImage(RECT *, unsigned char *);
extern void kaze_heap_free(void *);

/**
 * kaze_txp_archive_load_vram  (render, 0x8004add8)
 *
 * Loads a TXP archive by name via kaze_cd_load_file_alloc and LoadImage-
 * uploads every TIM entry to VRAM, then frees it.
 */

void kaze_txp_archive_load_vram(char *name)
{
    extern void *kaze_cd_load_file_alloc(void *);
    extern short kaze_file_path_drive_prefix;
    register int *archive __asm__("$18");
    register int i __asm__("$16");
    register int *entry __asm__("$17");
    register int count __asm__("$20");
    register RECT *rectp __asm__("$19");
    short path[64];
    RECT rect;
    GsIMAGE image;
    volatile int pad[2];

    if (*name != '\\') {
        path[0] = kaze_file_path_drive_prefix;
    }
    strcat((char *)path, name);
    archive = kaze_cd_load_file_alloc(path);
    __asm__ volatile("" : : "r"(archive));
    count = *archive;
    i = 0;
    if (count > 0) {
        rectp = &rect;
        entry = archive;
        do {
            if (*(int *)((int)archive + entry[1]) != 0x10) {
                printf(kaze_tim_load_error_msg);
            } else {
                GsGetTimInfo((ulong *)((int *)((int)archive + entry[1]) + 1), &image);
                rect.x = image.px;
                rect.y = image.py;
                rect.w = image.pw;
                rect.h = image.ph;
                LoadImage(rectp, image.pixel);
                DrawSync(0);
                if ((image.pmode & 8) != 0) {
                    rect.x = image.cx;
                    rect.y = image.cy;
                    rect.w = image.cw;
                    rect.h = image.ch;
                    LoadImage(rectp, image.clut);
                    DrawSync(0);
                }
            }
            entry++;
            i++;
        } while (i < count);
    }
    kaze_heap_free(archive);
}

extern void kaze_sfx_voice_keyon_34108(int voice, int arg1);
extern void kaze_play_sound_effect(short id, int arg1, int arg2, int arg3);

/**
 * kaze_sfx_play_dispatch  (sound, 0x8004af3c)
 *
 * Plays a sound effect: kaze_play_sound_effect for id<100, else
 * kaze_sfx_voice_keyon_34108(id-100).
 */

void kaze_sfx_play_dispatch(int id) {
    if (id >= 0x64) {
        kaze_sfx_voice_keyon_34108(id - 0x64, 1);
    } else {
        kaze_play_sound_effect((short)id, 0x64, 1, 1);
    }
}

extern void kaze_sfx_voice_keyoff_34170(int voice);
extern void kaze_seq_stop_track_34310(short id);

/**
 * kaze_sfx_stop_dispatch  (sound, 0x8004af88)
 *
 * Stops/updates a sound effect: kaze_seq_stop_track_34310 for id<100, else
 * kaze_sfx_voice_keyoff_34170(id-100).
 */

void kaze_sfx_stop_dispatch(int id) {
    if (id >= 0x64) {
        kaze_sfx_voice_keyoff_34170(id - 0x64);
    } else {
        kaze_seq_stop_track_34310((short)id);
    }
}
