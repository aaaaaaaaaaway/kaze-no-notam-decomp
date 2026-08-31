#include "types.h"
#include "kaze_syms.h"
extern char kaze_balloon_prm_alt_path[];
extern char kaze_balloon_prm_path[];
extern int kaze_task_record_index;
extern int kaze_main_menu_neighbor_up_table[];
extern int kaze_main_menu_neighbor_down_table[];
extern int kaze_main_menu_neighbor_left_table[];
extern int kaze_main_menu_neighbor_right_table[];
extern int kaze_menu_model_ptr;
extern int kaze_main_menu_substate;
extern int kaze_main_menu_prev_item;
extern int kaze_main_menu_panel_zoom_phase;
extern int kaze_main_menu_state_timer;
extern int D_800AAF88;
extern char kaze_main_menu_titlebar_sprite[];
extern char kaze_main_menu_titlebar_clut[];
extern char kaze_main_menu_titlebar_tpage[];
extern char kaze_main_menu_icon_clut;
extern char kaze_main_menu_panel_clut[];
extern char kaze_main_menu_panel_tpage[];
extern char kaze_main_menu_label_clut;
extern int kaze_menu_screen_mode;
extern int kaze_menu_vcount_timestamp;
extern u8 kaze_balloon_color_table_g[];
extern u8 kaze_balloon_color_table_b[];
typedef struct MenuIconPrim {
    char pad0[8];
    short x;
    short y;
    char pad1[0x44];
} MenuIconPrim;

extern u8 kaze_balloon_color_table_r[];

/**
 * kaze_balloon_color_apply  (menu, 0x800513a0)
 *
 * Applies the selected balloon color scheme: copies 264 RGB triples from PRM
 * entry (&kaze_save_slot_unpack_buffer)[kaze_task_record_index] (stride
 * 0x14) into the kaze_balloon_color_table_r color array.
 */

void kaze_balloon_color_apply(void)
{
    extern int kaze_save_slot_unpack_buffer[];
    int i;
    int off;
    int base;

    base = kaze_save_slot_unpack_buffer[kaze_task_record_index];
    i = 0;
    off = 6;
    do {
        kaze_balloon_color_table_r[i] = *(u8 *)(base + off - 2);
        kaze_balloon_color_table_g[i] = *(u8 *)(base + off - 1);
        kaze_balloon_color_table_b[i] = *(u8 *)(base + off);
        i += 4;
        off += 0x14;
    } while (i < 0x420);
}

extern int kaze_heap_free(int);

/**
 * kaze_menu_model_free  (menu, 0x8005140c)
 *
 * Frees the menu model buffer kaze_menu_model_ptr (GM.MDL) via
 * kaze_heap_free.
 */

void kaze_menu_model_free(void)
{
    kaze_heap_free(kaze_menu_model_ptr);
}

extern int kaze_input_current_c;
extern void kaze_input_edge_detect(void);
extern int kaze_file_load_by_name(char *);

/**
 * kaze_balloon_prm_load  (menu, 0x80051434)
 *
 * Loads the balloon design pack BAL\K\BALL.PRM (BALL2.PRM when
 * (kaze_input_current_c & 0xf)==0xf), builds the entry pointer table at
 * kaze_save_slot_unpack_buffer, then calls kaze_record_table_reset_defaults.
 */

void kaze_balloon_prm_load(void)
{
    extern void VSync(int);
    extern int kaze_save_slot_unpack_buffer;
    int pad[2];
    int mask;
    int cur;
    char *name;
    int *base;
    int count;
    register int i asm("$3");
    register int *out asm("$5");
    register int *p asm("$4");
    int off;

    VSync(0);
    kaze_input_edge_detect();
    cur = kaze_input_current_c;
    mask = 0xF;
    cur &= mask;
    __asm__ volatile("" :: "r"(cur), "r"(mask));
    name = kaze_balloon_prm_path;
    if (cur == mask) {
        name = kaze_balloon_prm_alt_path;
    }
    base = (int *)kaze_file_load_by_name(name);
    count = *base;
    i = 0;
    if (count > 0) {
        out = &kaze_save_slot_unpack_buffer;
        p = base;
        do {
            off = p[1];
            p++;
            i++;
            *out = (int)base + off;
            out++;
        } while (i < count);
    }
    kaze_record_table_reset_defaults();
}

extern int kaze_menu_ot_work_buf0[];
extern char kaze_menu_gs_ot_buf[];
void kaze_main_menu_init();
extern void GsInitVcount(void);
extern void kaze_gs_clear_vcount(void);
extern int GsGetActiveBuff(void);
extern void kaze_gs_set_work_base(int);
extern void GsClearOt(int, int, void *);
void kaze_main_menu_intro_scroll();
void kaze_main_menu_select_input();
void kaze_main_menu_draw();
extern int DrawSync(int);
extern int GsGetVcount(void);
extern void GsSwapDispBuff(void);
extern void GsDrawOt(void *);

/**
 * kaze_main_menu_loop  (menu, 0x800514d4)
 *
 * Main menu top screen loop (menu mode 0): own VSync/GsOT render loop over
 * sub-state kaze_main_menu_substate (0 intro scroll, 1 item select), exits
 * at 3; keeps BGM 0xd playing via kaze_xa_audio_manager_35a0.
 */

void kaze_main_menu_loop(void)
{
    extern int VSync(int);
    int active;
    int ot_index;
    int state;
    int *work_bases;
    char *ot_base;

    kaze_main_menu_init();
    GsInitVcount();
    VSync(0);
    kaze_gs_clear_vcount();

    if (kaze_main_menu_substate != 3) {
        work_bases = kaze_menu_ot_work_buf0;
        ot_base = kaze_menu_gs_ot_buf;
        do {
        active = GsGetActiveBuff();
        ot_index = active * 4;
        kaze_gs_set_work_base(work_bases[active]);
        ot_index = (ot_index + active) * 4;
        GsClearOt(0, 0, (void *)(ot_index + (int)ot_base));

        state = kaze_main_menu_substate;
        if (state == 0) {
            goto intro_scroll;
        }
        if (state == 1) {
            goto select_input;
        }
        goto after_menu_draw;

intro_scroll:
        kaze_main_menu_intro_scroll(active);
        goto draw_menu;

select_input:
        kaze_main_menu_select_input();

draw_menu:
        kaze_main_menu_draw(active);

after_menu_draw:
        DrawSync(0);
        kaze_menu_vcount_timestamp = GsGetVcount();
        VSync(0);
        kaze_gs_clear_vcount();
        kaze_input_edge_detect();
        GsSwapDispBuff();
        GsDrawOt((void *)(active * 20 + (int)ot_base));
        kaze_xa_audio_manager_35a0(0xD, 1);
        } while (kaze_main_menu_substate != 3);
    }
}

extern int kaze_main_menu_selected_item;
extern int kaze_main_menu_icon_pulse_phase;
void kaze_main_menu_titlebar_prim_init();
void kaze_main_menu_panel_prim_init();
void kaze_main_menu_icon_prim_init();
void kaze_main_menu_label_prim_init();

/**
 * kaze_main_menu_init  (menu, 0x80051630)
 *
 * Main menu init: zeroes sub-state/selection/animation vars from
 * kaze_main_menu_substate through kaze_main_menu_state_timer, sets an
 * unnamed menu-init flag just before kaze_main_menu_titlebar_sprite to 2,
 * then inits the title-bar, item-panel, item-icon, and item-label
 * primitives.
 */

void kaze_main_menu_init(void)
{
    kaze_main_menu_substate = 0;
    kaze_main_menu_prev_item = 0;
    kaze_main_menu_selected_item = 0;
    kaze_main_menu_panel_zoom_phase = 0;
    kaze_main_menu_icon_pulse_phase = 0;
    kaze_main_menu_state_timer = 0;
    D_800AAF88 = 2;
    kaze_backbuffer_copy_offscreen();
    kaze_drmove_grid_setup();
    kaze_main_menu_titlebar_prim_init();
    kaze_main_menu_panel_prim_init();
    kaze_main_menu_icon_prim_init();
    kaze_main_menu_label_prim_init();
}

extern int kaze_input_edge;

/**
 * kaze_main_menu_intro_scroll  (menu, 0x800516b4)
 *
 * Main menu intro state: draws the background at scroll
 * kaze_main_menu_state_timer (0..0x2a); any input (kaze_input_edge) or
 * completion advances to select state kaze_main_menu_substate=1.
 */

void kaze_main_menu_intro_scroll(int arg)
{
    kaze_drmove_wipe_tiles(arg, kaze_main_menu_state_timer, 0);
    if (kaze_main_menu_state_timer >= 0x2A || kaze_input_edge != 0) {
        kaze_main_menu_substate = 1;
        kaze_main_menu_state_timer = 0;
        D_800AAF88 = 0;
    } else {
        kaze_main_menu_state_timer = kaze_main_menu_state_timer + 1;
    }
}

extern void kaze_sfx_play_dispatch(int);

/**
 * kaze_main_menu_select_input  (menu, 0x80051734)
 *
 * Main menu select input: idle timeout 0xe10 exits to attract
 * (kaze_menu_screen_mode=8); dpad nav via tables
 * kaze_main_menu_neighbor_up_table/238/248/258; Circle maps items 0..3 to
 * menu modes 1/2/5/4; selection change arms the zoom animation via
 * kaze_main_menu_panel_zoom_phase.
 */

void kaze_main_menu_select_input(void)
{
    int item;

    if (kaze_main_menu_state_timer >= 0xE11) {
        kaze_menu_screen_mode = 8;
        kaze_main_menu_substate = 3;
        return;
    }

    if (kaze_input_current_c != 0) {
        kaze_main_menu_state_timer = 0;
    } else {
        kaze_main_menu_state_timer = kaze_main_menu_state_timer + 1;
    }

    if ((kaze_input_edge & 0x20) != 0) {
        item = kaze_main_menu_selected_item;
        if (item == 1) {
            goto accept_mode_2;
        }
        if (item < 2) {
            if (item == 0) {
                goto accept_start;
            }
            goto set_state_3;
        }
        if (item == 2) {
            goto accept_records;
        }
        if (item == 3) {
            goto accept_mode_4;
        }
        goto set_state_3;

accept_start:
        kaze_menu_screen_mode = 1;
        goto set_state_3;

accept_mode_2:
        kaze_menu_screen_mode = 2;
        goto set_state_3;

accept_records:
        kaze_menu_screen_mode = 5;
        goto set_state_3;

accept_mode_4:
        kaze_menu_screen_mode = 4;

set_state_3:
        kaze_main_menu_substate = 3;
        kaze_sfx_play_dispatch(2);
        return;
    }

    item = kaze_main_menu_selected_item;
    if ((kaze_input_edge & 0x1000) != 0) {
        item = kaze_main_menu_neighbor_up_table[item];
    }
    if ((kaze_input_edge & 0x4000) != 0) {
        item = kaze_main_menu_neighbor_down_table[item];
    }
    if ((kaze_input_edge & 0x8000) != 0) {
        item = kaze_main_menu_neighbor_left_table[item];
    }
    if ((kaze_input_edge & 0x2000) != 0) {
        item = kaze_main_menu_neighbor_right_table[item];
    }
    if (item != kaze_main_menu_selected_item) {
        kaze_main_menu_prev_item = kaze_main_menu_selected_item;
        kaze_main_menu_selected_item = item;
        kaze_main_menu_panel_zoom_phase = 0x400;
        kaze_sfx_play_dispatch(1);
    }
}

extern void kaze_drmove_addprim_a0ec(int, int);

/**
 * kaze_main_menu_draw  (menu, 0x800518e0)
 *
 * Main menu draw pass: title bar, selected item panel zoom animation, item
 * icon highlights, selected item label, then the menu background.
 */

void kaze_main_menu_draw(int idx)
{
    kaze_main_menu_titlebar_draw();
    kaze_main_menu_panel_zoom_anim(idx, kaze_main_menu_selected_item);
    kaze_main_menu_icon_highlight_draw(idx);
    kaze_main_menu_label_draw(idx, kaze_main_menu_selected_item);
    kaze_drmove_addprim_a0ec(idx, 0xFFF);
}

/**
 * kaze_main_menu_titlebar_prim_init  (menu, 0x8005193c)
 *
 * Inits the main-menu title-bar SPRT per buffer (0xa0x0x10 at (-0x50,-0x58),
 * clut(0x390,0xf0)) plus DR_TPAGE at
 * kaze_main_menu_titlebar_sprite/kaze_main_menu_titlebar_tpage.
 */

void kaze_main_menu_titlebar_prim_init(void)
{
    extern void SetDrawTPage(void *prim, int dfe, int dtd, unsigned int tpage);
    extern u16 GetTPage(int tp, int abr, int x, int y);
    extern u16 GetClut(int x, int y);
    extern void SetShadeTex(void *prim, int on);
    extern void SetSprt(void *sprt);
    u16 clut;
    int offset;
    int buf_i;
    char *p;

    buf_i = 0;
    offset = 0;
    do {
        p = kaze_main_menu_titlebar_sprite;
        p += offset;
        SetSprt(p);
        SetShadeTex(p, 1);
        *(short *)(p + 8) = -0x50;
        *(short *)(p + 0xA) = -0x58;
        *(short *)(p + 0x10) = 0xA0;
        *(short *)(p + 0x12) = 0x10;
        p[0xC] = 0;
        p[0xD] = 0x80;
        clut = GetClut(0x390, 0xF0);
        *(u16 *)(kaze_main_menu_titlebar_clut + offset) = clut;
        clut = GetTPage(0, 0, 0x180, 0x100);
        SetDrawTPage(kaze_main_menu_titlebar_tpage + buf_i * 8, 0, 1, clut);
        buf_i = buf_i + 1;
        offset = offset + 0x14;
    } while (buf_i < 2);
}

extern char kaze_main_menu_panel_polys[];
extern void SetPolyFT4(void *);

/**
 * kaze_main_menu_panel_prim_init  (menu, 0x80051a1c)
 *
 * Inits the four 128x128 POLY_FT4 menu item preview panels at
 * kaze_main_menu_panel_polys (tpages 0x140+i*0x20, cluts 0x340+i*0x10).
 */

void kaze_main_menu_panel_prim_init(void)
{
    extern void SetShadeTex(void *, int);
    extern int GetClut(int, int);
    extern int GetTPage(int, int, int, int);
    register int i asm("$18");
    register int v asm("$3");
    register int x asm("$19");
    register int clut_x asm("$20");
    register int ofs asm("$17");
    register int c0 asm("$2");
    int c1;
    char *p;
    register int hi_y asm("$21");
    register int tp asm("$4");
    register int abr asm("$5");
    int t;

    i = 0;
    hi_y = 0x7F;
    clut_x = 0x340;
    x = 0x140;
    ofs = 0;
    do {
        p = kaze_main_menu_panel_polys + ofs;
        SetPolyFT4(p);
        SetShadeTex(p, 0);
        v = i << 7;
        if (v < 0) {
            c0 = v + 0xFF;
        } else {
            c0 = v;
        }
        tp = 0;
        abr = 0;
        asm volatile("" : : "r"(tp), "r"(abr));
        c0 = v - ((c0 >> 8) << 8);
        c1 = c0 + 0x7F;
        p[0xC] = c0;
        p[0xD] = 0;
        p[0x14] = c1;
        p[0x15] = 0;
        p[0x1C] = c0;
        p[0x1D] = hi_y;
        p[0x24] = c1;
        p[0x25] = hi_y;
        t = GetTPage(tp, abr, x, 0x100);
        *(short *)(kaze_main_menu_panel_tpage + ofs) = t;
        t = GetClut(clut_x, 0xF0);
        clut_x += 0x10;
        x += 0x20;
        *(short *)(kaze_main_menu_panel_clut + ofs) = t;
        asm volatile("" : : : "memory");
        i++;
        ofs += 0x28;
    } while (i < 4);
}

extern char kaze_main_menu_icon_sprites;
extern char D_800AAFC4_left __asm__("kaze_main_menu_icon_sprites");
extern char kaze_main_menu_icon_tpage;

/**
 * kaze_main_menu_icon_prim_init  (menu, 0x80051b28)
 *
 * Inits the four 64x64 menu item icon SPRTs per buffer at
 * kaze_main_menu_icon_sprites (screen corners (-0xa0/0x60, -0x78/0x38),
 * clut(0x380,0xf0)) plus DR_TPAGE.
 */

void kaze_main_menu_icon_prim_init(void)
{
    extern void SetDrawTPage(char *, int, int, unsigned int);
    extern u16 GetTPage(int, int, int, int);
    extern u16 GetClut(int, int);
    extern void SetShadeTex(char *, int);
    extern void SetSprt(char *);
    u16 clut;
    int inner_offset;
    char *scan;
    register char *entry __asm__("$2");
    int inner;
    int outer_offset;
    int outer;
    int shade;
    char *base;
    volatile MenuIconPrim *left;
    volatile MenuIconPrim *right;

    outer = 0;
    shade = 0x80;
    base = &kaze_main_menu_icon_sprites;
    {
        register char *seed __asm__("$8");

        seed = &kaze_main_menu_icon_sprites;
        __asm__("" : "=r"(seed) : "0"(seed));
        right = (volatile MenuIconPrim *)(seed + 0x3c);
    }
    left = (volatile MenuIconPrim *)&D_800AAFC4_left;
    outer_offset = 0;
    do {
        inner = 0;
        inner_offset = outer_offset;
        scan = base;
        do {
            SetSprt((char *)(outer_offset + (int)scan));
            SetShadeTex((char *)(outer_offset + (int)scan), 0);
            {
                register int clut_x __asm__("$4");
                register int clut_y __asm__("$5");
                register char *entry_base __asm__("$8");
                register int size __asm__("$8");
                register int u __asm__("$3");

                clut_x = 0x380;
                clut_y = 0xF0;
                entry_base = &kaze_main_menu_icon_sprites;
                __asm__ volatile("" : "=r"(entry_base) : "0"(entry_base));
                entry = (char *)(inner_offset + (int)entry_base);
                size = 0x40;
                __asm__ volatile("" : "=r"(size) : "0"(size));
                *(short *)(entry + 0x10) = size;
                __asm__ volatile("" ::: "memory");
                u = inner << 6;
                *(short *)(entry + 0x12) = size;
                *(char *)(entry + 0x0c) = u;
                *(char *)(entry + 0x0d) = shade;
                *(char *)(entry + 0x04) = shade;
                *(char *)(entry + 0x05) = shade;
                *(char *)(entry + 0x06) = shade;
                clut = GetClut(clut_x, clut_y);
            }
            *(volatile u16 *)((int)&kaze_main_menu_icon_clut + inner_offset) = clut;
            inner_offset += 0x14;
            inner++;
            scan += 0x14;
        } while (inner < 4);
        clut = GetTPage(0, 0, 0x140, 0x100);
        SetDrawTPage(&kaze_main_menu_icon_tpage + outer * 8, 0, 1, clut);

        {
            register int left_x __asm__("$5");
            register int y_top __asm__("$3");
            register int right_x __asm__("$4");
            register char *base2 __asm__("$8");
            char *p;

            left_x = -0xa0;
            y_top = -0x78;
            base2 = &kaze_main_menu_icon_sprites;
            p = base2 + 0x14;
            p = (char *)(outer_offset + (int)p);
            right_x = 0x60;
            left->x = left_x;
            left->y = y_top;
            *(short *)(p + 0x08) = right_x;
            *(short *)(p + 0x0a) = y_top;
            p = base2 + 0x28;
            p = (char *)(outer_offset + (int)p);
            y_top = 0x38;
            *(short *)(p + 0x08) = left_x;
            *(short *)(p + 0x0a) = y_top;
            right->x = right_x;
            right->y = y_top;
        }

        right++;
        left++;
        outer++;
        outer_offset += 0x50;
    } while (outer < 2);
}

#define V0_BARRIER(x) \
    ({ register int value __asm__("$2"); value = (x); \
       __asm__ volatile("" : "=r"(value) : "0"(value)); value; })
extern char kaze_main_menu_label_sprites;
extern char kaze_main_menu_label_tpage;

/**
 * kaze_main_menu_label_prim_init  (menu, 0x80051cc0)
 *
 * Inits the four menu item label SPRTs per buffer at
 * kaze_main_menu_label_sprites (0xa0x0x10 at y 0x44, v0=-0x70+i*0x10,
 * clut(0x390,0xf0)) plus DR_TPAGE.
 */

void kaze_main_menu_label_prim_init(void)
{
    extern void SetDrawTPage(char *, int, int, unsigned int);
    extern u16 GetTPage(int, int, int, int);
    extern u16 GetClut(int, int);
    extern void SetShadeTex(char *, int);
    extern void SetSprt(char *);
    u16 clut;
    int inner_offset;
    char *scan;
    register char *entry __asm__("$3");
    int v;
    int inner;
    int outer_offset;
    int outer;
    char *base;

    outer = 0;
    base = &kaze_main_menu_label_sprites;
    outer_offset = 0;
    do {
        inner = 0;
        v = -0x70;
        inner_offset = outer_offset;
        scan = base;
        do {
            SetSprt((char *)(outer_offset + (int)scan));
            SetShadeTex((char *)(outer_offset + (int)scan), 1);
            {
                register int clut_x __asm__("$4");
                register int clut_y __asm__("$5");

                clut_x = 0x390;
                clut_y = 0xF0;
                entry = (char *)(inner_offset + (int)base);
                __asm__ volatile("" : "=r"(entry) : "0"(entry));
                *(short *)(entry + 8) = V0_BARRIER(-0x50);
                *(short *)(entry + 10) = V0_BARRIER(0x44);
                *(short *)(entry + 0x10) = V0_BARRIER(0xA0);
                *(short *)(entry + 0x12) = V0_BARRIER(0x10);
                *(char *)(entry + 0xC) = 0;
                *(char *)(entry + 0xD) = v;
                clut = GetClut(clut_x, clut_y);
            }
            v += 0x10;
            *(volatile u16 *)((int)&kaze_main_menu_label_clut + inner_offset) = clut;
            inner_offset += 0x14;
            inner++;
            scan += 0x14;
        } while (inner < 4);
        clut = GetTPage(0, 0, 0x180, 0x100);
        SetDrawTPage(&kaze_main_menu_label_tpage + outer * 8, 0, 1, clut);
        outer++;
        outer_offset += 0x50;
    } while (outer < 2);
}
