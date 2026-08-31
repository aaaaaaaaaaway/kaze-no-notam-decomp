#include "types.h"
#include "kaze_syms.h"
extern int kaze_main_menu_prev_item;
extern int kaze_main_menu_panel_zoom_phase;
extern char kaze_main_menu_titlebar_sprite[];
extern char kaze_main_menu_titlebar_tpage[];
extern u8 kaze_main_menu_icon_shade;
extern u8 kaze_main_menu_panel_color_r[];
extern u8 kaze_main_menu_panel_color_g[];
extern u8 kaze_main_menu_panel_color_b[];
extern void GsSortPoly(void *, void *, int);
typedef struct {
    int ot;
    int pad[4];
} MenuTitlebarSlot;
typedef struct {
    char pad[4];
    u8 unk4;
    u8 unk5;
    u8 unk6;
} Row;
typedef struct {
    int prim;
    int pad[4];
} Slot;

/**
 * kaze_main_menu_titlebar_draw  (menu, 0x80051df8)
 *
 * AddPrims the main-menu title-bar SPRT and its DR_TPAGE into the GsOT prim
 * list (&kaze_menu_gs_ot_buf0_org)[buf*5].
 */

void kaze_main_menu_titlebar_draw(int buf)
{
    extern void AddPrim(int, char *);
    extern MenuTitlebarSlot kaze_menu_gs_ot_buf0_org[];
    AddPrim(kaze_menu_gs_ot_buf0_org[buf].ot, kaze_main_menu_titlebar_sprite + buf * 0x14);
    AddPrim(kaze_menu_gs_ot_buf0_org[buf].ot, kaze_main_menu_titlebar_tpage + buf * 8);
    return;
}

extern u8 kaze_main_menu_panel_polys[];
extern int kaze_menu_gs_ot_buf[];

/**
 * kaze_main_menu_panel_zoom_anim  (menu, 0x80051e6c)
 *
 * Animates the selected item panel: phase kaze_main_menu_panel_zoom_phase
 * 0x400..0xbff shrinks the previous item then grows the new one with rsin
 * width and brightness ramps, else sorts the resting 128x120 quad via
 * GsSortPoly.
 */

void kaze_main_menu_panel_zoom_anim(int buf, int item)
{
    extern int rsin(int);
    int sin_abs;
    register int amp __asm__("$9");
    register int phase_off __asm__("$2");
    int cm68;
    register int oth __asm__("$5");
    int a2v;
    int idxv;
    register int *ot __asm__("$5");
    int pad[5];

    if ((unsigned int)(kaze_main_menu_panel_zoom_phase - 0x400) < 0x400) {
        sin_abs = rsin(kaze_main_menu_panel_zoom_phase);
        if (sin_abs < 0) {
            sin_abs = -sin_abs;
        }
        __asm__("" : "=r"(sin_abs) : "0"(sin_abs));
        if (sin_abs < 0) {
            sin_abs = sin_abs + 0x1f;
        }
        amp = sin_abs >> 5;
        phase_off = kaze_main_menu_panel_zoom_phase - 0x400;
        if (phase_off < 0) {
            phase_off = kaze_main_menu_panel_zoom_phase - 0x3f1;
        }
        {
            register int shade __asm__("$4");
            {
                register int sh4 __asm__("$7");
                sh4 = phase_off >> 4;
                shade = sh4 - 0x80;
            }
            oth = buf * 20;
            __asm__("move %0,$0" : "=r"(a2v));
            kaze_main_menu_panel_color_r[kaze_main_menu_prev_item * 0x28] = shade;
            ot = (int *)((char *)kaze_menu_gs_ot_buf + oth);
            kaze_main_menu_panel_color_g[kaze_main_menu_prev_item * 0x28] = shade;
            cm68 = -0x44;
            kaze_main_menu_panel_color_b[kaze_main_menu_prev_item * 0x28] = shade;
        }
        idxv = kaze_main_menu_prev_item;
        __asm__ volatile("" ::: "memory");
        {
            register u8 *pv __asm__("$4");
            register u8 *pvb __asm__("$2");
            pvb = kaze_main_menu_panel_polys;
            __asm__("" : "=r"(pvb) : "0"(pvb));
            pv = (u8 *)(idxv * 0x28 + (int)pvb);
            __asm__("" : "=r"(amp) : "0"(amp), "r"(pv));
            {
            register int half_w __asm__("$2");
            register int t0vp __asm__("$8");
            half_w = (0x80 - amp) / 2;
            t0vp = half_w - 0x40;
            {
                register int amp_adj __asm__("$3");
                amp_adj = amp - 0x40;
                half_w = half_w + amp_adj;
            }
            *(s16 *)(pv + 0x8) = t0vp;
            *(s16 *)(pv + 0xa) = cm68;
            *(s16 *)(pv + 0x10) = half_w;
            *(s16 *)(pv + 0x12) = cm68;
            *(s16 *)(pv + 0x18) = t0vp;
            *(s16 *)(pv + 0x1a) = 0x3c;
            *(s16 *)(pv + 0x20) = half_w;
            *(s16 *)(pv + 0x22) = 0x3c;
            GsSortPoly(pv, ot, a2v);
            }
        }
        kaze_main_menu_panel_zoom_phase = kaze_main_menu_panel_zoom_phase + 0x40;
        return;
    }
    if ((unsigned int)(kaze_main_menu_panel_zoom_phase - 0x800) < 0x400) {
        register int sh4v __asm__("$7");
        sin_abs = rsin(kaze_main_menu_panel_zoom_phase);
        if (sin_abs < 0) {
            sin_abs = -sin_abs;
        }
        __asm__("" : "=r"(sin_abs) : "0"(sin_abs));
        if (sin_abs < 0) {
            sin_abs = sin_abs + 0x1f;
        }
        amp = sin_abs >> 5;
        phase_off = kaze_main_menu_panel_zoom_phase - 0x800;
        if (phase_off < 0) {
            phase_off = kaze_main_menu_panel_zoom_phase - 0x7f1;
        }
        sh4v = phase_off >> 4;
        {
            register u8 *pv __asm__("$4");
            {
                register u8 *pvb __asm__("$2");
                pvb = kaze_main_menu_panel_polys;
                __asm__("" : "=r"(pvb) : "0"(pvb));
                pv = (u8 *)(item * 0x28 + (int)pvb);
            }
            ot = kaze_menu_gs_ot_buf + buf * 5;
            __asm__("move %0,$0" : "=r"(a2v));
            {
                register int shade __asm__("$2");
                shade = sh4v + 0x40;
                pv[0x4] = shade;
                pv[0x5] = shade;
                pv[0x6] = shade;
            }
            __asm__ volatile("" ::: "memory");
            {
                register int half_w __asm__("$2");
                register int t0vp __asm__("$8");
                half_w = (0x80 - amp) / 2;
                t0vp = half_w - 0x40;
                cm68 = -0x44;
                {
                    register int amp_adj __asm__("$3");
                    amp_adj = amp - 0x40;
                    half_w = half_w + amp_adj;
                }
                *(s16 *)(pv + 0x8) = t0vp;
                *(s16 *)(pv + 0xa) = cm68;
                *(s16 *)(pv + 0x10) = half_w;
                *(s16 *)(pv + 0x12) = cm68;
                *(s16 *)(pv + 0x18) = t0vp;
                *(s16 *)(pv + 0x1a) = 0x3c;
                *(s16 *)(pv + 0x20) = half_w;
                *(s16 *)(pv + 0x22) = 0x3c;
                GsSortPoly(pv, ot, a2v);
            }
        }
        kaze_main_menu_panel_zoom_phase = kaze_main_menu_panel_zoom_phase + 0x40;
        return;
    }
    oth = buf * 4;
    __asm__("" : "=r"(item) : "0"(item), "r"(oth));
    {
        register u8 *pv __asm__("$4");
        register int cm64 __asm__("$7");
        int c128;
        {
            register u8 *pvb __asm__("$2");
            pvb = kaze_main_menu_panel_polys;
            __asm__("" : "=r"(pvb) : "0"(pvb));
            pv = (u8 *)(item * 0x28 + (int)pvb);
        }
        ot = kaze_menu_gs_ot_buf + (oth + buf);
        __asm__("move %0,$0" : "=r"(a2v));
        c128 = 0x80;
        __asm__("li %0,-64" : "=r"(cm64) : "r"(c128));
        pv[0x4] = c128;
        pv[0x5] = c128;
        pv[0x6] = c128;
        *(s16 *)(pv + 0xa) = -0x44;
        *(s16 *)(pv + 0x12) = -0x44;
        *(s16 *)(pv + 0x8) = cm64;
        *(s16 *)(pv + 0x10) = 0x40;
        *(s16 *)(pv + 0x18) = cm64;
        *(s16 *)(pv + 0x1a) = 0x3c;
        *(s16 *)(pv + 0x20) = 0x40;
        *(s16 *)(pv + 0x22) = 0x3c;
        GsSortPoly(pv, ot, a2v);
    }
    if (kaze_main_menu_panel_zoom_phase != 0) {
        kaze_main_menu_panel_zoom_phase = 0;
    }
}

extern s32 kaze_main_menu_selected_item;
extern s32 kaze_main_menu_icon_pulse_phase;
extern u8 kaze_main_menu_icon_sprites;
extern char kaze_main_menu_icon_tpage[];

/**
 * kaze_main_menu_icon_highlight_draw  (menu, 0x80052140)
 *
 * Menu item icon highlight pass: the selected icon
 * (kaze_main_menu_selected_item) pulses via
 * rsin(kaze_main_menu_icon_pulse_phase), the others step back toward 0x80;
 * AddPrims all four icons plus TPAGE.
 */

void kaze_main_menu_icon_highlight_draw(s32 arg0) {
    extern char kaze_menu_gs_ot_buf0_org[];
    extern s32 rsin(s32);
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
    register int cc asm("$2");
    u8 c;
    void *temp_a1;
    void *prim_ptr;

    arg = arg0;
    i = 0;
    row = (char *)&kaze_main_menu_icon_sprites;
    b3ac = (u8 *)row;
    arg5 = arg * 5;
    base60 = arg5 * 0x10;
    off = base60;
    do {
        asm volatile("" : "=r"(arg) : "0"(arg));
        if (i == kaze_main_menu_selected_item) {
            cc = rsin(kaze_main_menu_icon_pulse_phase);
            if (cc < 0) {
                cc += 0x3F;
            }
            cc = cc >> 6;
            if (cc < 0) {
                cc = -cc;
            }
            cc += 0x40;
            *(&kaze_main_menu_icon_shade + off) = cc;
            *(&kaze_main_menu_icon_shade + 1 + off) = cc;
            *(&kaze_main_menu_icon_shade + 2 + off) = cc;
            prim_ofs = arg5 * 4;
        } else {
            c = *(&kaze_main_menu_icon_shade + off);
            if (c >= 0x89U) {
                *(&kaze_main_menu_icon_shade + off) = c - 8;
                v0 = *(&kaze_main_menu_icon_shade + 1 + off) - 8;
                v1 = *(&kaze_main_menu_icon_shade + 2 + off) - 8;
                goto store12;
            }
            if (c < 0x78U) {
                *(&kaze_main_menu_icon_shade + off) = c + 8;
                v0 = *(&kaze_main_menu_icon_shade + 1 + off) + 8;
                v1 = *(&kaze_main_menu_icon_shade + 2 + off) + 8;
            store12:
                *(&kaze_main_menu_icon_shade + 1 + off) = v0;
                *(&kaze_main_menu_icon_shade + 2 + off) = v1;
                prim_ofs = arg5 * 4;
            } else {
                r = (Row *)(off + (int)b3ac);
                asm volatile("" :: "r"(r));
                cc = 0x80;
                r->unk4 = cc;
                r->unk5 = cc;
                r->unk6 = cc;
                asm volatile("" ::: "memory");
                prim_ofs = arg5 * 4;
            }
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
    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + prim_ofs), (void *)(cc + (int)&kaze_main_menu_icon_tpage));
    kaze_main_menu_icon_pulse_phase = (kaze_main_menu_icon_pulse_phase + 0x20) & 0xFFF;
}

extern char kaze_main_menu_label_sprites[];
extern char kaze_main_menu_label_tpage[];

/**
 * kaze_main_menu_label_draw  (menu, 0x80052350)
 *
 * AddPrims the item label SPRT for selection sub
 * (kaze_main_menu_label_sprites + buf*0x50 + sel*0x14) plus its DR_TPAGE.
 */

void kaze_main_menu_label_draw(int idx, int sub)
{
    extern int AddPrim(void *, void *);
    extern Slot kaze_menu_gs_ot_buf0_org[];
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[idx].prim, sub * 0x14 + (kaze_main_menu_label_sprites + idx * 0x50));
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[idx].prim, kaze_main_menu_label_tpage + idx * 8);
}
