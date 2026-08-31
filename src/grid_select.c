#include "types.h"
#include "kaze_syms.h"
extern int kaze_menu_level_source;
extern int kaze_menu_flags_source;
extern u8 kaze_variant_stage_source_table;
extern u8 kaze_variant_flags_source_table;
extern u8 kaze_variant_level_source_table;
extern u8 kaze_variant_weather_source_table;
extern int kaze_grid_select_prim_buffer_b;
extern u8 kaze_setup_menu_stage_panel_shade;
extern u8 kaze_setup_menu_level_row_shade;
extern u8 kaze_setup_menu_flags_row_shade;
extern u8 kaze_title_weather_rows_tint;
extern char kaze_title_bg_sprt_prims[];
extern char kaze_title_bg_tpage_prims[];
extern int kaze_grid_select_state;
extern int kaze_grid_select_fade_counter;
extern int kaze_grid_select_pulse_phase;
extern int kaze_grid_select_reentry_flag;
extern int kaze_menu_vcount_timestamp;
typedef struct {
    char pad[4];
    u8 unk4;
    u8 unk5;
    u8 unk6;
} Row;
typedef struct { char pad[4]; u8 unk4, unk5, unk6; } Row__1;
typedef struct {
    void *ob;
    char pad[16];
} Slot20;

extern void AddPrim(void *, void *);
extern u8 kaze_setup_menu_stage_panel_sprites;
extern u8 kaze_setup_menu_stage_panel_tpage;

/**
 * kaze_title_rows3_highlight_draw  (menu, 0x80053fc4)
 *
 * Fades 3 menu-row sprites at kaze_setup_menu_stage_panel_sprites toward
 * bright when row==kaze_menu_stage_source, dims others, then AddPrims the
 * rows plus trailer prim kaze_setup_menu_stage_panel_tpage.
 */

void kaze_title_rows3_highlight_draw(s32 arg0) {
    extern s32 kaze_menu_stage_source;
    extern char kaze_menu_gs_ot_buf0_org[];
    register s32 off asm("$16");
    s32 prim_ofs;
    void *prim_ptr;
    void *temp_a1;
    register s32 i asm("$18");
    register char *row asm("$19");
    s32 arg;
    s32 base60;
    register u8 *b3ac asm("$22");
    Row *r;
    int v0;
    int v1;
    int cc;
    u8 c;

    arg = arg0;
    i = 0;
    row = (char *)&kaze_setup_menu_stage_panel_sprites;
    b3ac = (u8 *)row;
    base60 = arg * 0x3C;
    off = base60;
    do {
        asm volatile("" : "=r"(arg) : "0"(arg));
        if (kaze_menu_stage_source == i) {
            c = *(&kaze_setup_menu_stage_panel_shade + off);
            if (c < 0x71U) {
                *(&kaze_setup_menu_stage_panel_shade + off) = c + 0x10;
                v0 = *(&kaze_setup_menu_stage_panel_shade + 1 + off) + 0x10;
                v1 = *(&kaze_setup_menu_stage_panel_shade + 2 + off) + 0x10;
                goto store12;
            }
            r = (Row *)(off + (int)b3ac);
            cc = 0x80;
            goto storeclamp;
        }
        c = *(&kaze_setup_menu_stage_panel_shade + off);
        if (c >= 0x50U) {
            *(&kaze_setup_menu_stage_panel_shade + off) = c - 0x10;
            v0 = *(&kaze_setup_menu_stage_panel_shade + 1 + off) - 0x10;
            v1 = *(&kaze_setup_menu_stage_panel_shade + 2 + off) - 0x10;
        store12:
            *(&kaze_setup_menu_stage_panel_shade + 1 + off) = v0;
            *(&kaze_setup_menu_stage_panel_shade + 2 + off) = v1;
        } else {
            r = (Row *)(off + (int)b3ac);
            cc = 0x40;
        storeclamp:
            r->unk4 = cc;
            r->unk5 = cc;
            r->unk6 = cc;
        }
        prim_ofs = arg * 0x14;
        prim_ptr = *(void **)(kaze_menu_gs_ot_buf0_org + prim_ofs);
        asm volatile("" :: "r"(prim_ptr));
        temp_a1 = (void *)(base60 + (int)row);
        row += 0x14;
        off += 0x14;
        i++;
        AddPrim(prim_ptr, temp_a1);
    } while (i < 3);
    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + prim_ofs), (void *)(arg * 8 + (int)&kaze_setup_menu_stage_panel_tpage));
}

extern u8 kaze_task_grid_available_flags, kaze_setup_menu_mode_panel_sprites, kaze_setup_menu_mode_panel_shade, kaze_setup_menu_mode_icon_sprites, kaze_setup_menu_mode_icon_tpage;
extern s32 kaze_menu_stage_source, kaze_menu_mode_source;

/**
 * kaze_settings_mode_rows_draw  (menu, 0x80054174)
 *
 * Draws the 3 settings rows keyed to cursor kaze_menu_mode_source; adds a
 * marker prim per row when flag kaze_task_grid_available_flags[row +
 * kaze_menu_stage_source*3] is set, using the same fade-highlight scheme.
 */

void kaze_settings_mode_rows_draw(s32 arg0) {
    extern char kaze_menu_gs_ot_buf0_org[];
    register s32 off asm("$16");
    register s32 i asm("$17");
    register char *row asm("$18");
    register s32 rofs asm("$20");
    register u8 *b3ac asm("$23");
    s32 base60, prim_ofs; u8 *tbl; s32 sel;
    void *prim_ptr, *temp_a1; Row__1 *r; int v0, v1, cc; u8 c;
    i = 0; prim_ofs = arg0 * 0x14; row = (char *)&kaze_setup_menu_mode_panel_sprites; b3ac = (u8 *)row;
    rofs = 0; base60 = arg0 * 0x3C; off = base60;
    do {
        sel = kaze_menu_stage_source; asm volatile("" : "=r"(sel) : "0"(sel));
        tbl = &kaze_task_grid_available_flags; asm volatile("" : "=r"(tbl) : "0"(tbl));
        if (*(sel * 3 + tbl + i) != 0) {
            void *pp = *(void **)(kaze_menu_gs_ot_buf0_org + prim_ofs);
            register s32 a1v asm("$5");
            asm volatile("" :: "r"(pp));
            a1v = (int)&kaze_setup_menu_mode_icon_sprites;
            a1v = rofs + a1v;
            asm volatile("" : "=r"(a1v) : "0"(a1v));
            AddPrim(pp, (void *)(base60 + a1v));
        }
        if (kaze_menu_mode_source == i) {
            c = *(&kaze_setup_menu_mode_panel_shade + off);
            if (c < 0x71U) { *(&kaze_setup_menu_mode_panel_shade + off) = c + 0x10; v0 = *(&kaze_setup_menu_mode_panel_shade+1+off)+0x10; v1 = *(&kaze_setup_menu_mode_panel_shade+2+off)+0x10; goto s12; }
            r = (Row__1 *)(off + (int)b3ac); cc = 0x80; goto sc;
        }
        c = *(&kaze_setup_menu_mode_panel_shade + off);
        if (c >= 0x50U) { *(&kaze_setup_menu_mode_panel_shade+off) = c-0x10; v0 = *(&kaze_setup_menu_mode_panel_shade+1+off)-0x10; v1 = *(&kaze_setup_menu_mode_panel_shade+2+off)-0x10;
        s12: *(&kaze_setup_menu_mode_panel_shade+1+off) = v0; *(&kaze_setup_menu_mode_panel_shade+2+off) = v1; }
        else { r = (Row__1 *)(off + (int)b3ac); cc = 0x40; sc: r->unk4 = cc; r->unk5 = cc; r->unk6 = cc; }
        prim_ptr = *(void **)(kaze_menu_gs_ot_buf0_org + prim_ofs);
        asm volatile("" :: "r"(prim_ptr));
        temp_a1 = (void *)(base60 + (int)row);
        row += 0x14; rofs += 0x14; off += 0x14; i++;
        AddPrim(prim_ptr, temp_a1);
    } while (i < 3);
    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + (arg0 * 0x14)), (void *)(arg0 * 8 + (int)&kaze_setup_menu_mode_icon_tpage));
}

extern s32 kaze_title_menu_state;
extern char kaze_setup_menu_level_row_sprites[], kaze_setup_menu_flags_row_sprites[], kaze_setup_menu_weather_row_sprites[], kaze_title_rows_tpage_prims[];

/**
 * kaze_title_row_groups_fade_draw  (menu, 0x80054390)
 *
 * Fades the 3/4/10-entry title row groups toward selected tint based on
 * title state kaze_title_menu_state (3..6), then adds the entries chosen by
 * kaze_menu_level_source/kaze_menu_flags_source/kaze_menu_weather_source.
 */

void kaze_title_row_groups_fade_draw(int buf) {
    extern char kaze_menu_gs_ot_buf0_org[];
    extern s32 kaze_menu_level_source, kaze_menu_flags_source, kaze_menu_weather_source;
    register int row_ofs asm("$4");
    int g;
    int b;
    register int i asm("$5");
    Row__1 *r;
    register Row__1 *rbase asm("$8");
    register int c0 asm("$7"), c8 asm("$6");
    u8 c;
    int c3;
    int prim_ofs;

    i = 0;
    rbase = (Row__1 *)&kaze_setup_menu_level_row_sprites;
    c0 = 0xc0;
    c8 = 0x80;
    { register int mul15 __asm__("$2") = (buf * 16) - buf;
    __asm__("" :: "r"(mul15));
    row_ofs = mul15 * 4;
    }
    do {
        __asm__("addiu %0,$0,3" : "=r"(c3) : "r"(i));
        if ((kaze_title_menu_state == c3) ||
            (kaze_title_menu_state == ({ int _t = 6; __asm__("" : "=r"(_t) : "0"(_t), "r"(i)); _t; }))) {
            c = *(&kaze_setup_menu_level_row_shade + row_ofs);
            if (c < 0xb1) {
                *(&kaze_setup_menu_level_row_shade + row_ofs) = c + 0x10;
                g = *(&kaze_setup_menu_level_row_shade + 1 + row_ofs) + 0x10;
                b = *(&kaze_setup_menu_level_row_shade + 2 + row_ofs) - 0x10;
                goto LAB_80054494;
            }
            r = (Row__1 *)(row_ofs + (int)rbase);
            r->unk4 = c0;
            r->unk5 = c0;
            r->unk6 = ({ int _q; __asm__("addiu %0,$0,0x40" : "=r"(_q) : "r"(i)); _q; });
        } else {
            c = *(&kaze_setup_menu_level_row_shade + row_ofs);
            if (0x8f < c) {
                *(&kaze_setup_menu_level_row_shade + row_ofs) = c - 0x10;
                g = *(&kaze_setup_menu_level_row_shade + 1 + row_ofs) - 0x10;
                b = *(&kaze_setup_menu_level_row_shade + 2 + row_ofs) + 0x10;
            LAB_80054494:
                *(&kaze_setup_menu_level_row_shade + 1 + row_ofs) = g;
                *(&kaze_setup_menu_level_row_shade + 2 + row_ofs) = b;
            } else {
                register Row__1 *rv0 asm("$2");
                rv0 = (Row__1 *)(row_ofs + (int)rbase);
                rv0->unk4 = c8;
                rv0->unk5 = c8;
                rv0->unk6 = c8;
            }
        }
        i = i + 1;
        row_ofs = row_ofs + 0x14;
    } while (i < 3);
    i = 0;
    rbase = (Row__1 *)&kaze_setup_menu_flags_row_sprites;
    c0 = 0xc0;
    c8 = 0x80;
    { register int mul5 __asm__("$2") = (buf * 4) + buf;
    __asm__("" :: "r"(mul5));
    row_ofs = mul5 * 16;
    }
    do {
        __asm__("addiu %0,$0,4" : "=r"(c3) : "r"(i));
        if ((kaze_title_menu_state == c3) ||
            (kaze_title_menu_state == ({ int _t = 6; __asm__("" : "=r"(_t) : "0"(_t), "r"(i)); _t; }))) {
            c = *(&kaze_setup_menu_flags_row_shade + row_ofs);
            if (c < 0xb1) {
                *(&kaze_setup_menu_flags_row_shade + row_ofs) = c + 0x10;
                g = *(&kaze_setup_menu_flags_row_shade + 1 + row_ofs) + 0x10;
                b = *(&kaze_setup_menu_flags_row_shade + 2 + row_ofs) - 0x10;
                goto LAB_800545bc;
            }
            r = (Row__1 *)(row_ofs + (int)rbase);
            r->unk4 = c0;
            r->unk5 = c0;
            r->unk6 = ({ int _q; __asm__("addiu %0,$0,0x40" : "=r"(_q) : "r"(i)); _q; });
        } else {
            c = *(&kaze_setup_menu_flags_row_shade + row_ofs);
            if (0x8f < c) {
                *(&kaze_setup_menu_flags_row_shade + row_ofs) = c - 0x10;
                g = *(&kaze_setup_menu_flags_row_shade + 1 + row_ofs) - 0x10;
                b = *(&kaze_setup_menu_flags_row_shade + 2 + row_ofs) + 0x10;
            LAB_800545bc:
                *(&kaze_setup_menu_flags_row_shade + 1 + row_ofs) = g;
                *(&kaze_setup_menu_flags_row_shade + 2 + row_ofs) = b;
            } else {
                register Row__1 *rv0 asm("$2");
                rv0 = (Row__1 *)(row_ofs + (int)rbase);
                rv0->unk4 = c8;
                rv0->unk5 = c8;
                rv0->unk6 = c8;
            }
        }
        i = i + 1;
        row_ofs = row_ofs + 0x14;
    } while (i < 4);
    i = 0;
    rbase = (Row__1 *)&kaze_setup_menu_weather_row_sprites;
    c0 = 0xc0;
    c8 = 0x80;
    { register int mul25 __asm__("$2");
    mul25 = (((buf * 2) + buf) * 8) + buf;
    __asm__("" :: "r"(mul25));
    row_ofs = mul25 * 8;
    }
    do {
        if (!(kaze_title_menu_state - 5U < 2))
            goto third_inactive;
        c = *(&kaze_title_weather_rows_tint + row_ofs);
        if (!(c < 0xb1))
            goto third_active_sat;
        *(&kaze_title_weather_rows_tint + row_ofs) = c + 0x10;
        g = *(&kaze_title_weather_rows_tint + 1 + row_ofs) + 0x10;
        b = *(&kaze_title_weather_rows_tint + 2 + row_ofs) - 0x10;
        goto third_shared_store;
third_active_sat:
        r = (Row__1 *)(row_ofs + (int)rbase);
        r->unk4 = c0;
        r->unk5 = c0;
        r->unk6 = ({ int _q; __asm__("addiu %0,$0,0x40" : "=r"(_q) : "r"(i)); _q; });
        goto third_increment;
third_inactive:
        c = *(&kaze_title_weather_rows_tint + row_ofs);
        if (c < 0x90)
            goto third_inactive_sat;
        *(&kaze_title_weather_rows_tint + row_ofs) = c - 0x10;
        g = *(&kaze_title_weather_rows_tint + 1 + row_ofs) - 0x10;
        b = *(&kaze_title_weather_rows_tint + 2 + row_ofs) + 0x10;
third_shared_store:
        *(&kaze_title_weather_rows_tint + 1 + row_ofs) = g;
        *(&kaze_title_weather_rows_tint + 2 + row_ofs) = b;
        goto third_increment;
third_inactive_sat:
        {
            register Row__1 *rv0 asm("$2");
            rv0 = (Row__1 *)(row_ofs + (int)rbase);
            rv0->unk4 = c8;
            rv0->unk5 = c8;
            rv0->unk6 = c8;
        }
third_increment:
        i = i + 1;
        row_ofs = row_ofs + 0x14;
    } while (i < 0xa);
    {
    register int off60 asm("$3");
    off60 = buf * 60;
    prim_ofs = buf * 0x14;
    {
        int idx = kaze_menu_level_source;
        AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + prim_ofs),
                (void *)(((int)&kaze_setup_menu_level_row_sprites + off60) + ((idx * 5) * 4)));
    }
    {
        int idx = kaze_menu_flags_source;
        AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + prim_ofs),
                (void *)(((int)&kaze_setup_menu_flags_row_sprites + buf * 0x50) + ((idx * 5) * 4)));
    }
    {
        int idx = kaze_menu_weather_source;
        register int off200 asm("$2");
        off200 = buf * 200;
        AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + prim_ofs),
                (void *)(((int)&kaze_setup_menu_weather_row_sprites + off200) + ((idx * 5) * 4)));
    }
    AddPrim(*(void **)(kaze_menu_gs_ot_buf0_org + prim_ofs),
            (void *)(buf * 8 + (int)&kaze_title_rows_tpage_prims));
    }
}

extern u8 kaze_title_icon_row1_sprites[];
extern u8 kaze_title_icon_row2_sprites[];
extern u8 kaze_title_icon_rows_tpage_prims[];
extern u8 kaze_setup_row1_flash_level;
extern u8 kaze_setup_row2_flash_level;

/**
 * kaze_title_icon_rows_draw  (menu, 0x80054830)
 *
 * Decays icon brightness bytes
 * kaze_setup_row1_flash_level/kaze_setup_row2_flash_level toward 0x80,
 * applies them to both SPRT_16 icon rows, and AddPrims them plus tpage prim
 * kaze_title_icon_rows_tpage_prims.
 */

void kaze_title_icon_rows_draw(int arg0)
{
    extern Slot20 kaze_menu_gs_ot_buf0_org[];
    u8 *p;
    int i7;
    int i8;
    int arg;
    register int i4 asm("$19");
    register u8 *pu5 asm("$20");
    register u8 *pu6 asm("$22");
    register u8 *pb3 asm("$17");
    register u8 *pb2 asm("$16");

    arg = arg0;
    i8 = 0;
    pu5 = kaze_title_icon_row2_sprites;
    pu6 = kaze_title_icon_row1_sprites;
    pb3 = &kaze_setup_row2_flash_level;
    pb2 = &kaze_setup_row1_flash_level;
    i7 = arg * 0x30;
    i4 = i7;
loop:
    {
        register unsigned int v asm("$3");
        register int reset asm("$6");
        register int dec asm("$2");

        v = *pb2;
        reset = 0x80;
        if (v >= 0x8cU) {
            dec = v - 0xc;
            *pb2 = dec;
        } else {
            *pb2 = reset;
        }
        v = *pb3;
        reset = 0x80;
        if (v >= 0x8cU) {
            dec = v - 0xc;
            *pb3 = dec;
        } else {
            *pb3 = reset;
        }
    }
    {
        register u8 *base asm("$6");
        register u8 *q asm("$3");

        base = kaze_title_icon_row1_sprites;
        q = base + i4;
        p = (u8 *)(i7 + (int)pu6);
        pu6 += 0x10;
        q[4] = *pb2;
        base = kaze_title_icon_row2_sprites;
        q[5] = *pb2;
        i8 = i8 + 1;
        q[6] = *pb2;
        q = base + i4;
        q[4] = *pb3;
        pb2 = pb2 + 1;
        q[5] = *pb3;
        q[6] = *pb3;
        i4 = i4 + 0x10;
    }
    pb3 = pb3 + 1;
    AddPrim(kaze_menu_gs_ot_buf0_org[arg].ob, p);
    p = (u8 *)(i7 + (int)pu5);
    pu5 += 0x10;
    AddPrim(kaze_menu_gs_ot_buf0_org[arg].ob, p);
    if (i8 < 3) {
        goto loop;
    }
    AddPrim(kaze_menu_gs_ot_buf0_org[arg].ob, kaze_title_icon_rows_tpage_prims + arg * 8);
}

/**
 * kaze_title_bg_prims_add  (menu, 0x800549cc)
 *
 * Adds the title background SPRT/DR_TPAGE prims
 * (kaze_title_bg_sprt_prims/914/8d8/91c) at the OT tail
 * ((&kaze_menu_gs_ot_buf0_org)[buf*5]+0x3ffc) so they draw behind
 * everything.
 */

void kaze_title_bg_prims_add(int idx)
{
    extern char kaze_menu_gs_ot_buf0_org[];
    register int idx_save __asm__("$19");
    register int sprt_ofs __asm__("$16");
    register int ot_ofs __asm__("$20");
    register char *sprt __asm__("$18");
    register char *tpage __asm__("$17");

    idx_save = idx;
    sprt_ofs = idx_save << 2;
    sprt_ofs += idx_save;
    ot_ofs = sprt_ofs << 2;
    sprt_ofs <<= 3;
    sprt = kaze_title_bg_sprt_prims;
    AddPrim((void *)(*(int *)(kaze_menu_gs_ot_buf0_org + ot_ofs) + 0x3FFC), sprt + sprt_ofs);

    idx_save <<= 4;
    tpage = kaze_title_bg_tpage_prims;
    AddPrim((void *)(*(int *)(kaze_menu_gs_ot_buf0_org + ot_ofs) + 0x3FFC), tpage + idx_save);

    sprt += 0x14;
    AddPrim((void *)(*(int *)(kaze_menu_gs_ot_buf0_org + ot_ofs) + 0x3FFC), sprt + sprt_ofs);

    tpage += 0x8;
    AddPrim((void *)(*(int *)(kaze_menu_gs_ot_buf0_org + ot_ofs) + 0x3FFC), tpage + idx_save);
}

extern int kaze_menu_ot_work_buf0[];
extern int kaze_menu_gs_ot_buf[];
void kaze_grid_select_init();
extern void GsInitVcount(void);
extern void VSync(int);
extern void kaze_gs_clear_vcount(void);
extern int GsGetActiveBuff(void);
extern void kaze_gs_set_work_base(int);
extern void GsClearOt(int, int, int *);
void kaze_grid_select_state_step();
extern void DrawSync(int);
extern int GsGetVcount(void);
extern void kaze_input_edge_detect(void);
extern void GsSwapDispBuff(void);
extern void GsDrawOt(int *);
void kaze_grid_select_teardown();

/**
 * kaze_grid_select_screen_loop  (menu, 0x80054a9c)
 *
 * Modal loop for the 3x3 grid-select screen:
 * GsClearOt/kaze_grid_select_state_step/GsDrawOt with VSync until state
 * kaze_grid_select_state==99, then teardown kaze_grid_select_teardown; keeps
 * sound tick kaze_xa_audio_manager_35a0(0xd).
 */

void kaze_grid_select_screen_loop(void)
{
    int i;
    int j;

    kaze_grid_select_init();
    GsInitVcount();
    VSync(0);
    kaze_gs_clear_vcount();
    while (kaze_grid_select_state != 99) {
        i = GsGetActiveBuff();
        kaze_gs_set_work_base(kaze_menu_ot_work_buf0[i]);
        j = i * 5;
        GsClearOt(0, 0, kaze_menu_gs_ot_buf + j);
        kaze_grid_select_state_step(i);
        DrawSync(0);
        kaze_menu_vcount_timestamp = GsGetVcount();
        VSync(0);
        kaze_gs_clear_vcount();
        kaze_input_edge_detect();
        GsSwapDispBuff();
        GsDrawOt(kaze_menu_gs_ot_buf + j);
        kaze_xa_audio_manager_35a0(0xD, 1);
    }
    kaze_grid_select_teardown();
}

extern int kaze_heap_alloc(int arg0, int arg1);
extern int kaze_grid_select_prim_buffer_a;
extern u8 kaze_grid_variant_saved_index;

/**
 * kaze_grid_select_init  (menu, 0x80054bb4)
 *
 * Allocates two 0x17c prim buffers into kaze_grid_select_prim_buffer_a/6c,
 * clamps selection kaze_menu_variant_source from save byte
 * kaze_grid_variant_saved_index, seeds state kaze_grid_select_state from
 * kaze_grid_select_reentry_flag, and builds sprites.
 */

void kaze_grid_select_init(void)
{
    extern unsigned int kaze_menu_variant_source;
    unsigned int v;

    kaze_grid_select_prim_buffer_a = kaze_heap_alloc(1, 0x17C);
    kaze_grid_select_prim_buffer_b = kaze_heap_alloc(1, 0x17C);
    v = kaze_grid_variant_saved_index;
    if (v < 9) {
        kaze_menu_variant_source = v;
    } else {
        kaze_menu_variant_source = 0;
    }
    if (kaze_grid_select_reentry_flag != 0) {
        kaze_grid_select_state = 1;
    } else {
        kaze_grid_select_state = 0;
    }
    kaze_grid_select_fade_counter = 0;
    kaze_grid_select_pulse_phase = 0;
    kaze_backbuffer_copy_offscreen();
    kaze_grid_select_frame_sprites_init();
    kaze_grid_select_cell_sprites_init();
    kaze_linef3_init();
    kaze_task_menu_prim_init();
    kaze_task_menu_enter_init();
}

extern void kaze_heap_free(int);

/**
 * kaze_grid_select_teardown  (menu, 0x80054c8c)
 *
 * Frees the two prim buffers and maps chosen slot kaze_menu_variant_source
 * to title-menu row values kaze_menu_stage_source/kaze_menu_flags_source/kaz
 * e_menu_level_source/kaze_menu_weather_source via ROM tables
 * kaze_variant_stage_source_table/2f.
 */

void kaze_grid_select_teardown(void)
{
    extern int kaze_menu_weather_source;
    extern int kaze_menu_stage_source;
    extern int kaze_menu_variant_source;
    int *p = &kaze_grid_select_prim_buffer_a;
    int i;
    u8 a, b, c, d;

    kaze_heap_free(*p);
    kaze_heap_free(kaze_grid_select_prim_buffer_b);
    i = kaze_menu_variant_source - 1;
    a = (&kaze_variant_stage_source_table)[i * 3];
    b = (&kaze_variant_flags_source_table)[i * 3];
    c = (&kaze_variant_level_source_table)[i * 3];
    d = (&kaze_variant_weather_source_table)[i];
    *p = 0;
    kaze_grid_select_prim_buffer_b = 0;
    kaze_menu_stage_source = a;
    kaze_menu_flags_source = b;
    kaze_menu_level_source = c;
    kaze_menu_weather_source = d;
}

int kaze_grid_select_fade_step();
extern void kaze_task_menu_state_machine(int arg0, int arg1, int arg2);
extern void kaze_grid_select_confirm_result(int arg0);
extern void kaze_grid_select_draw(int arg0);

/**
 * kaze_grid_select_state_step  (menu, 0x80054d44)
 *
 * Per-frame state machine for kaze_grid_select_state: 0 intro fade + dialog,
 * 1 fade-in, 2 confirm dialog (kaze_task_menu_state_machine), 10 grid input
 * kaze_grid_select_input, then draw kaze_grid_select_draw.
 */

void kaze_grid_select_state_step(int buf)
{
    switch (kaze_grid_select_state) {
    case 0:
        if (kaze_grid_select_fade_step(buf) != 0) {
            kaze_grid_select_state = 2;
        }
        kaze_task_menu_state_machine(buf, 1, 1);
        break;
    case 1:
        if (kaze_grid_select_fade_step(buf) != 0) {
            kaze_grid_select_state = 0xA;
        }
        kaze_grid_select_draw(buf);
        break;
    case 2:
        kaze_grid_select_confirm_result(buf);
        break;
    case 0xA:
        kaze_grid_select_input();
        kaze_grid_select_draw(buf);
        break;
    }
}

extern int kaze_input_edge;

/**
 * kaze_grid_select_fade_step  (menu, 0x80054e20)
 *
 * Advances the screen fade counter kaze_grid_select_fade_counter to 0x2a via
 * kaze_drmove_wipe_tiles; any new input kaze_input_edge skips it. Returns 1
 * when finished.
 */

int kaze_grid_select_fade_step(int arg0)
{
    int ret;

    kaze_drmove_wipe_tiles(arg0, kaze_grid_select_fade_counter, 2);
    if ((kaze_grid_select_fade_counter >= 0x2a) || (kaze_input_edge != 0)) {
        kaze_grid_select_fade_counter = 0;
        ret = 1;
    } else {
        kaze_grid_select_fade_counter = kaze_grid_select_fade_counter + 1;
        ret = 0;
    }
    return ret;
}
