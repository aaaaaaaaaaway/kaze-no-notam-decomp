#include "types.h"
#include "kaze_syms.h"
extern int kaze_grid_select_state;
extern int kaze_grid_select_pulse_phase;
extern int kaze_options_state;
extern int kaze_options_fade_counter;
extern int kaze_button_config_action_row;
extern int kaze_button_config_toggle_row;
extern int kaze_options_fade_mode;
extern int kaze_options_fade_reverse_flag;
extern int kaze_menu_screen_mode;
extern int kaze_menu_vcount_timestamp;

extern int kaze_input_edge;
extern int kaze_menu_variant_source;
extern void kaze_sfx_play_dispatch(int);

/**
 * kaze_grid_select_input  (menu, 0x80054f08)
 *
 * D-pad navigation over the 3x3 slot grid kaze_menu_variant_source (enabled
 * when kaze_grid_variant_saved_index==9); accept (0x20) exits with
 * kaze_menu_screen_mode=9 and slot+1, cancel (0x40) returns to dialog state
 * 2.
 */

void kaze_grid_select_input(void)
{
    extern unsigned char kaze_grid_variant_saved_index;
    int sfx;

    if (kaze_grid_variant_saved_index == 9) {
        if ((kaze_input_edge & 0x4000) && (kaze_menu_variant_source / 3 != 2)) {
            kaze_menu_variant_source = kaze_menu_variant_source + 3;
            kaze_sfx_play_dispatch(1);
        }
        if ((kaze_input_edge & 0x1000) && (kaze_menu_variant_source / 3 != 0)) {
            kaze_menu_variant_source = kaze_menu_variant_source - 3;
            kaze_sfx_play_dispatch(1);
        }
        if ((kaze_input_edge & 0x2000) && ((kaze_menu_variant_source / 3) * 3 != kaze_menu_variant_source - 2)) {
            kaze_menu_variant_source = kaze_menu_variant_source + 1;
            kaze_sfx_play_dispatch(1);
        }
        if ((kaze_input_edge & 0x8000) && (kaze_menu_variant_source != (kaze_menu_variant_source / 3) * 3)) {
            kaze_menu_variant_source = kaze_menu_variant_source - 1;
            kaze_sfx_play_dispatch(1);
        }
    }
    if (kaze_input_edge & 0x20) {
        kaze_grid_select_state = 99;
        kaze_menu_screen_mode = 9;
        kaze_menu_variant_source = kaze_menu_variant_source + 1;
        sfx = 2;
    } else {
        if (!(kaze_input_edge & 0x40)) {
            return;
        }
        kaze_grid_select_state = 2;
        kaze_task_menu_enter_init();
        sfx = 0x65;
    }
    kaze_sfx_play_dispatch(sfx);
}

extern void kaze_linef3_gradient_bar(int, int, int, int, int);
void kaze_grid_select_frame_prims_add();
void kaze_grid_select_pulse_draw();
extern void kaze_drmove_addprim_a0ec(int, int);

/**
 * kaze_grid_select_draw  (menu, 0x800550e8)
 *
 * Draws the selection ring at the current 3x3 cell (0x48/0x30 pitch), then
 * frame prims kaze_grid_select_frame_prims_add, cell prims
 * kaze_grid_select_pulse_draw, and fade overlay kaze_drmove_addprim_a0ec.
 */

void kaze_grid_select_draw(int arg)
{
    extern u8 kaze_grid_variant_saved_index;
    if ((kaze_grid_select_state != 99) && (kaze_grid_variant_saved_index == 9)) {
        kaze_linef3_gradient_bar(arg, (kaze_menu_variant_source % 3) * 0x48 + 0x43,
                                 (kaze_menu_variant_source / 3) * 0x30 + 0x4f, 0x2a, 0x2a);
    }
    kaze_grid_select_frame_prims_add(arg, 0, 0);
    kaze_grid_select_pulse_draw(arg);
    kaze_drmove_addprim_a0ec(arg, 0xfff);
}

extern int kaze_grid_select_prim_buffer_a[];
extern void SetSprt(void *);
extern void SetShadeTex(void *, int);

/**
 * kaze_grid_select_frame_sprites_init  (menu, 0x800551a4)
 *
 * Builds header/banner SPRTs plus DR_TPAGE inside each allocated buffer
 * kaze_grid_select_prim_buffer_a/6c (CLUTs 0x360/0x370 row 0xf6, tpage
 * 0x3c0).
 */

void kaze_grid_select_frame_sprites_init(void)
{
    extern void SetDrawTPage(void *, int, int, int);
    extern u_short GetTPage(int, int, int, int);
    extern u_short GetClut(int, int);
    int i;
    int *slot;
    int *base;
    u_short clut;
    u_short tpage;

    i = 0;
    do {
        base = kaze_grid_select_prim_buffer_a;
        slot = base + i;
        i++;
        SetSprt((void *)*slot);
        SetShadeTex((void *)*slot, 1);
        *(short *)(*slot + 8) = -0xa0;
        *(short *)(*slot + 10) = -0x78;
        *(short *)(*slot + 0x10) = 0x40;
        *(short *)(*slot + 0x12) = 0x40;
        *(unsigned char *)(*slot + 0xc) = 0;
        *(unsigned char *)(*slot + 0xd) = 0x80;
        clut = GetClut(0x360, 0xf6);
        *(u_short *)(*slot + 0xe) = clut;
        SetSprt((void *)(*slot + 0x14));
        SetShadeTex((void *)(*slot + 0x14), 1);
        *(short *)(*slot + 0x1c) = -0x60;
        *(short *)(*slot + 0x1e) = -0x58;
        *(short *)(*slot + 0x24) = 0x80;
        *(short *)(*slot + 0x26) = 0x20;
        *(unsigned char *)(*slot + 0x20) = 0x40;
        *(unsigned char *)(*slot + 0x21) = 0x80;
        clut = GetClut(0x370, 0xf6);
        *(u_short *)(*slot + 0x22) = clut;
        tpage = GetTPage(0, 0, 0x3c0, 0);
        SetDrawTPage((void *)(*slot + 0x28), 0, 1, tpage);
    } while (i < 2);
}

extern void SetTile(void *);
extern void SetSemiTrans(void *, int);

/**
 * kaze_grid_select_cell_sprites_init  (menu, 0x80055310)
 *
 * Builds nine 0x28x0x28 cell SPRTs per buffer, recolors the first
 * kaze_grid_variant_saved_index unlocked cells with CLUT (0x350,0xf6), and
 * nine semi-transparent shadow TILEs.
 */

void kaze_grid_select_cell_sprites_init(void)
{
    extern void SetDrawTPage(void *, int, int, unsigned int);
    extern int GetTPage(int, int, int, int);
    extern s16 GetClut(int, int);
    extern u8 kaze_grid_variant_saved_index;
    register int n asm("$22");
    int *p;
    register int i asm("$17");
    int width = 0x28;
    register u8 color asm("$21") = 0x80;
    int pad0;

    p = kaze_grid_select_prim_buffer_a;
    n = (char *)p - (char *)p;
outer_loop:
    {
        {
        int off;
        int s3;
        i = 0;
        s3 = 0x30;
        off = 0;
        __asm__ volatile ("" : "=m"(pad0));
        do {
            register int quotient asm("$5");
            register int rem asm("$6");
            SetSprt((void *)(*p + s3));
            SetShadeTex((void *)(*p + s3), 0);
            quotient = i / 3;
            rem = i - quotient * 3;
            *(s16 *)(off + *p + 0x38) = rem * 0x48 - 0x5C;
            *(s16 *)(off + *p + 0x3A) = quotient * 0x30 - 0x28;
            *(s16 *)(off + *p + 0x40) = width;
            *(s16 *)(off + *p + 0x42) = width;
            *(s8 *)(off + *p + 0x3C) = rem * 0x28;
            *(s8 *)(off + *p + 0x3D) = quotient * 0x28;
            s3 += 0x14;
            *(u8 *)(off + *p + 0x34) = color;
            *(u8 *)(off + *p + 0x35) = color;
            *(u8 *)(off + *p + 0x36) = color;
            i += 1;
            *(u16 *)(*p + off + 0x3E) = GetClut(0x340, 0xF6);
            off += 0x14;
        } while (i < 9);
        }
        {
        int off;
        int *q;
        int guard = kaze_grid_variant_saved_index;
        __asm__("" : "=r"(guard) : "0"(guard));
        if (guard > 0) {
            i = 0;
            q = p;
            off = 0;
            do {
                register int quotient asm("$6") = i / 3;
                int rem = quotient << 1;
                register int first_addr asm("$5") = off + *q;
                rem = rem + quotient;
                rem = i - rem;
                *(s8 *)(first_addr + 0x3C) = rem * 0x28 + 0x78;
                *(s8 *)(off + *q + 0x3D) = quotient * 0x28;
                {
                    u16 clut;
                    register int base asm("$4");
                    register int limit asm("$3");
                    clut = GetClut(0x350, 0xF6);
                    i += 1;
                    base = *q;
                    limit = kaze_grid_variant_saved_index;
                    *(u16 *)(base + off + 0x3E) = clut;
                    off += 0x14;
                    if (i >= limit) break;
                }
            } while (1);
        }
        }
        {
        register int off asm("$5");
        int s3;
        int *q;
        i = 0;
        q = p;
        s3 = 0xEC;
        do {
            int quotient;
            register int rem asm("$3");
            int tripled;
            register int first_base asm("$6");
            SetTile((void *)(*q + s3));
            SetSemiTrans((void *)(*q + s3), 1);
            off = i * 0x10;
            first_base = off + *q;
            quotient = i / 3;
            tripled = quotient * 3;
            rem = i - tripled;
            __asm__("" : : "r"(rem));
            *(s16 *)(first_base + 0xF4) = rem * 0x48 - 0x56;
            *(s16 *)(off + *q + 0xF6) = tripled * 0x10 - 0x22;
            *(s16 *)(off + *q + 0xF8) = width;
            *(s16 *)(off + *q + 0xFA) = width;
            *(u8 *)(off + *q + 0xF0) = 0;
            __asm__("" : : "r"(s3) : "memory");
            s3 += 0x10;
            *(u8 *)(off + *q + 0xF1) = 0;
            __asm__("" : : "r"(i) : "memory");
            i += 1;
            *(u8 *)(off + *q + 0xF2) = 0;
        } while (i < 9);
        }
        {
            u16 tpage = GetTPage(0, 0, 0x3C0, 0);
            register int draw_mode asm("$5") = 0;
            register int dither asm("$6") = 1;
            register int page_arg asm("$7") = tpage;
            int draw_base = *p;
            p += 1;
            __asm__("" : "=r"(n) : "0"(n), "r"(draw_mode), "r"(dither), "r"(page_arg));
            n += 4;
            SetDrawTPage((void *)(draw_base + 0xE4), draw_mode, dither, page_arg);
        }
    }
    if (n < 8) goto outer_loop;
}

extern void AddPrim(void *, void *);

/**
 * kaze_grid_select_frame_prims_add  (menu, 0x8005564c)
 *
 * Positions the two header sprites of buffer idx at
 * (x-0xa0,y-0x78)/(x-0x60,y-0x58) and adds them plus the DR_TPAGE to OT
 * (&kaze_menu_gs_ot_buf0_org)[buf*5].
 */

void kaze_grid_select_frame_prims_add(int idx, int x, int y)
{
    extern char kaze_menu_gs_ot_buf0_org[];
    int *p;
    int ot_ofs;

    p = kaze_grid_select_prim_buffer_a;
    ot_ofs = idx * 4;
    p = (int *)((int)p + ot_ofs);
    *(short *)(*p + 0x8) = x - 0xA0;
    *(short *)(*p + 0xA) = y - 0x78;
    *(short *)(*p + 0x1C) = x - 0x60;
    *(short *)(*p + 0x1E) = y - 0x58;

    ot_ofs = (ot_ofs + idx) * 4;
    AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ot_ofs), (void *)*p);
    AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ot_ofs), (void *)(*p + 0x14));
    AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ot_ofs), (void *)(*p + 0x28));
}

extern int rsin(int);
/*
 * OT-pointer access island.
 *
 * The original object keeps only the byte offset (buf * 0x14) in $s3 and
 * rebuilds the OT-table load `lui $at,%hi; addu $at,$at,$s3; lw,%lo($at)` at
 * every one of the three use-sites.  Plain C (`kaze_menu_gs_ot_buf0_org[buf * 5]`) lets
 * gcc GCSE-hoist &kaze_menu_gs_ot_buf0_org[buf*5] into a single saved pointer register
 * and collapse each site to `lw,0($s3)`, which is a different (though equal
 * size) instruction sequence.  This bounded island forces the re-emit so the
 * loop bodies schedule exactly like the target; the offset stays plain C.
 */
#define OT_PRIM(off) ({ void *__p; \
    __asm__ ("lw\t%0,kaze_menu_gs_ot_buf0_org(%1)" : "=r"(__p) : "r"(off)); __p; })

/**
 * kaze_grid_select_pulse_draw  (menu, 0x80055704)
 *
 * Pulses the selected cell tint with rsin phase kaze_grid_select_pulse_phase
 * (color channel picked by kaze_grid_variant_saved_index tier), then
 * AddPrims the 9 cell sprites and 9 shadow tiles.
 */

void kaze_grid_select_pulse_draw(int buf)
{
    extern int kaze_menu_gs_ot_buf0_org[];
    extern u8 kaze_grid_variant_saved_index;
    int phase;
    register int phase5 asm("$2");
    register int *cell_ptr asm("$4");
    register int k80 asm("$5");
    int i;
    int ofs;
    unsigned int dc;
    register int col asm("$6");
    int next_phase_clock;
    int *loopPi;
    int ot_ofs;
    void *primPtr;

    /* phase = rsin(kaze_grid_select_pulse_phase) / 0x20, spelled split so the +0x1f rounding
     * adjust straddles the kaze_grid_select_pulse_phase wrap update exactly as in the target. */
    phase = rsin(kaze_grid_select_pulse_phase);
    if (phase < 0) {
        phase += 0x1f;
    }
    dc = kaze_grid_variant_saved_index;
    next_phase_clock = (kaze_grid_select_pulse_phase + 0x20) & 0xfff;
    kaze_grid_select_pulse_phase = next_phase_clock;
    __asm__("" : "=r"(phase) : "0"(phase), "r"(next_phase_clock), "r"(dc), "m"(kaze_grid_select_pulse_phase));
    phase5 = phase >> 5;
    if (phase5 < 0) {
        phase5 = -phase5;
    }
    {
        register int base255 asm("$3");
        __asm__("addiu $3,$0,0xff" : "=r"(base255) : "r"(phase5));
        col = base255 - phase5;
    }

    if (dc < 3) {
        register int *gridBase __asm__("$2");
        cell_ptr = (k80 = 0x80, gridBase = kaze_grid_select_prim_buffer_a, gridBase + buf);
        __asm__("" : "=r"(cell_ptr) : "0"(cell_ptr));
        __asm__("" : "=r"(k80) : "0"(k80));
        *(char *)(kaze_menu_variant_source * 0x14 + *cell_ptr + 0x34) = k80;
        *(char *)(kaze_menu_variant_source * 0x14 + *cell_ptr + 0x35) = col;
    } else {
        if (dc < 6) {
            register int *gridBase __asm__("$2");
            cell_ptr = (gridBase = kaze_grid_select_prim_buffer_a, gridBase + buf);
            k80 = 0x80;
            __asm__("" : "=r"(k80) : "0"(k80));
        __asm__("" : "=r"(cell_ptr) : "0"(cell_ptr));
            *(char *)(kaze_menu_variant_source * 0x14 + *cell_ptr + 0x34) = k80;
            *(char *)(kaze_menu_variant_source * 0x14 + *cell_ptr + 0x35) = k80;
            *(char *)(kaze_menu_variant_source * 0x14 + *cell_ptr + 0x36) = col;
            goto LAB_800558e8;
        }
        phase5 = dc < 9;
        if (phase5 != 0) {
            register int *gridBase __asm__("$2");
            int drawBase;
            int drawBase2;
            cell_ptr = (int *)(buf * 4);
            gridBase = kaze_grid_select_prim_buffer_a;
            drawBase = kaze_menu_variant_source;
            __asm__("" : "=r"(gridBase) : "0"(gridBase), "r"(drawBase));
            cell_ptr = (int *)((int)cell_ptr + (int)gridBase);
            __asm__("" : "=r"(cell_ptr) : "0"(cell_ptr));
            *(char *)(drawBase * 0x14 + *cell_ptr + 0x34) = col;
            drawBase2 = kaze_menu_variant_source;
            __asm__ volatile("" :: "r"(drawBase2));
            k80 = 0x80;
            {
                int draw5 = drawBase2 * 5;
                int drawOfs2;
                int prim2;
                __asm__ volatile("" ::: "memory");
                prim2 = *cell_ptr;
                drawOfs2 = draw5 * 4;
                *(char *)(drawOfs2 + prim2 + 0x35) = k80;
            }
            __asm__("":"=r"(cell_ptr):"0"(cell_ptr));
        } else {
            goto LAB_800558e8;
        }
    }
    {
        register int draw3 __asm__("$3") = kaze_menu_variant_source;
        register int addr3 __asm__("$2");
        addr3 = draw3 * 5;
        draw3 = *cell_ptr;
        addr3 = addr3 * 4 + draw3;
        *(char *)(addr3 + 0x36) = k80;
    }

LAB_800558e8:
    i = 0;
    ot_ofs = buf * 0x14;
    loopPi = &kaze_grid_select_prim_buffer_a[buf];
    ofs = 0x30;
    do {
        i = i + 1;
        AddPrim(OT_PRIM(ot_ofs), (void *)(*loopPi + ofs));
        ofs = ofs + 0x14;
    } while (i < 9);

    i = 0;
    {
        int scaled4 = buf * 4;
        int otCalc = (scaled4 + buf) * 4;
        register int *gridBase __asm__("$2");
        int *loopCalc;
        __asm__("" : "=r"(otCalc) : "0"(otCalc));
        ot_ofs = otCalc;
        gridBase = kaze_grid_select_prim_buffer_a;
        loopCalc = (int *)((int)gridBase + scaled4);
        __asm__("" : "=r"(loopCalc) : "0"(loopCalc));
        loopPi = loopCalc;
    }
    ofs = 0xec;
    primPtr = (void *)(*loopPi + 0xe4);
    { int _mt = loopPi; __asm__("" :: "r"(_mt) : "memory"); }
    AddPrim(OT_PRIM(ot_ofs), primPtr);
    do {
        i = i + 1;
        primPtr = (void *)(*loopPi + ofs);
        AddPrim(OT_PRIM(ot_ofs), primPtr);
        ofs = ofs + 0x10;
    } while (i < 9);
}

extern void *kaze_menu_ot_work_buf0[];
extern char kaze_menu_gs_ot_buf[];
void kaze_options_screen_init();
extern void GsInitVcount(void);
extern int VSync(int);
extern void kaze_gs_clear_vcount(void);
extern int GsGetActiveBuff(void);
extern void kaze_gs_set_work_base(void *);
extern void GsClearOt(int, int, void *);
extern void DrawSync(int);
extern int GsGetVcount(void);
extern void kaze_input_edge_detect(void);
extern void GsSwapDispBuff(void);
extern void GsDrawOt(void *);

/**
 * kaze_options_screen_loop  (menu, 0x800559bc)
 *
 * Modal loop for the options/entry menu:
 * GsClearOt/kaze_options_state_dispatch/GsDrawOt with VSync until
 * kaze_options_state==99; sound tick kaze_xa_audio_manager_35a0 uses 0xc
 * during state 3, else 0xd.
 */

void kaze_options_screen_loop(void)
{
    int active;
    void **work_base;
    char *ot;
    int audio;

    kaze_options_screen_init();
    GsInitVcount();
    VSync(0);
    kaze_gs_clear_vcount();
    if (kaze_options_state != 99) {
        work_base = kaze_menu_ot_work_buf0;
        do {
            active = GsGetActiveBuff();
            kaze_gs_set_work_base(work_base[active]);
            ot = kaze_menu_gs_ot_buf + active * 20;
            GsClearOt(0, 0, ot);
            kaze_options_state_dispatch(active);
            DrawSync(0);
            kaze_menu_vcount_timestamp = GsGetVcount();
            VSync(0);
            kaze_gs_clear_vcount();
            kaze_input_edge_detect();
            GsSwapDispBuff();
            GsDrawOt(ot);
            audio = 13;
            if (kaze_options_state == 3) {
                audio = 12;
            }
            kaze_xa_audio_manager_35a0(audio, 1);
        } while (kaze_options_state != 99);
    }
}

extern int kaze_options_top_menu_row;
extern short kaze_button_config_column_flag;
extern short kaze_button_config_remap_armed_flag;

/**
 * kaze_options_screen_init  (menu, 0x80055ae0)
 *
 * Resets options-menu state vars
 * kaze_options_state..kaze_options_fade_reverse_flag and builds all sprite
 * sets (kaze_registration_sprites_init_all registration,
 * kaze_options_header_sprites_init/kaze_options_row_sprites_init
 * headers+rows, kaze_button_config_sprites_init key config,
 * kaze_sound_mode_sprites_init sound).
 */

void kaze_options_screen_init(void)
{
    int mode;
    int fade;

    mode = 2;
    kaze_options_fade_mode = mode;
    fade = 1;
    kaze_options_state = 0;
    kaze_options_fade_counter = 0;
    kaze_options_top_menu_row = 0;
    kaze_button_config_action_row = 0;
    kaze_button_config_toggle_row = 0;
    kaze_button_config_column_flag = 0;
    kaze_button_config_remap_armed_flag = 0;
    kaze_options_fade_reverse_flag = fade;
    kaze_backbuffer_copy_offscreen();
    kaze_registration_sprites_init_all();
    kaze_options_header_sprites_init();
    kaze_options_row_sprites_init();
    kaze_button_config_sprites_init();
    kaze_sound_mode_sprites_init();
    kaze_linef3_init();
}
