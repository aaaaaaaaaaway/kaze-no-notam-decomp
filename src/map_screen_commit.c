#include "types.h"
#include "kaze_syms.h"
extern u8 kaze_map_bg_tile_present_table[];
extern u8 kaze_skip_start_x_tile_table[];
extern u8 kaze_skip_start_z_tile_table[];
extern u8 kaze_skip_start_wind_table[];
extern int D_800AAF0C;
extern int kaze_map_heap_ptr_alt;
extern int kaze_map_ot_heap_alloc_a;
extern int kaze_map_ot_heap_alloc_b;
extern int D_800B1A48;
extern int kaze_menu_vcount_timestamp;
extern int kaze_map_fade_counter;
extern int kaze_map_selected_wind;
extern int kaze_map_state;
extern int kaze_staged_start_wind_dir;
extern int kaze_staged_start_x;
extern int kaze_staged_start_y;
extern int kaze_staged_start_z;
extern void SetDispMask(int);
extern void SetPolyF4(void *);
typedef struct {
    void *ot;
    char pad[16];
} T;
typedef struct {
    void *ptr;
    int pad[4];
} Slot;
typedef struct {
    short x, y, w, h;
} RECT;

extern void AddPrim(void *, void *);

/**
 * kaze_map_tiles_draw  (map_screen, 0x8004ff38)
 *
 * AddPrims the map background tile SPRTs flagged present in the
 * kaze_map_bg_tile_present_table 8x10 table plus the tile DR_TPAGE
 * (+0x1ee8).
 */

void kaze_map_tiles_draw(int arg0)
{
    extern T kaze_map_screen_prim_ot_ptr[];
    extern char *kaze_map_screen_prim_buffer;
    int i, j, base, v, count;
    u8 *tiles;

    count = 0;
    j = 0;
    tiles = kaze_map_bg_tile_present_table;
    do {
        base = j * 10;
        i = 0;
        do {
            v = base + i;
            if (*(u8 *)(v + (int)tiles) != 0) {
                AddPrim(kaze_map_screen_prim_ot_ptr[arg0].ot,
                        kaze_map_screen_prim_buffer + (arg0 * 0x320 + 0x18A8) + count * 0x14);
                count += 1;
            }
            i += 1;
        } while (i < 0xA);
        j += 1;
    } while (j < 8);
    AddPrim(kaze_map_screen_prim_ot_ptr[arg0].ot, kaze_map_screen_prim_buffer + (arg0 * 8 + 0x1EE8));
}

extern void SetSemiTrans(void *, int);

/**
 * kaze_map_fade_quad_init  (map_screen, 0x80050074)
 *
 * Inits the full-screen fade POLY_F4 (semi-trans, -0xa0..0xa0 x -0x78..0x78)
 * at +0x1ef8 per buffer plus a DR_TPAGE using GetTPage(1,mode,0,0) as blend
 * rate.
 */

void kaze_map_fade_quad_init(int mode)
{
    extern void SetShadeTex(void *, int);
    extern void SetDrawTPage(void *, int, int, int);
    extern u16 GetTPage(int, int, int, int);
    extern int kaze_map_screen_prim_buffer;
    int i;
    int x0;
    int y0;
    int x1;
    int y1;
    int poly;
    int off;
    int tpage;
    int base;
    int base2;

    i = 0;
    x0 = -0xA0;
    y0 = -0x78;
    x1 = 0xA0;
    y1 = 0x78;
    poly = 0x1EF8;
    off = 0;
    tpage = 0x1F28;
    do {
        base = GetTPage(1, mode, 0, 0);
        SetDrawTPage((void *)(kaze_map_screen_prim_buffer + tpage), 0, 1, base & 0xFFFF);
        SetPolyF4((void *)(kaze_map_screen_prim_buffer + poly));
        SetSemiTrans((void *)(kaze_map_screen_prim_buffer + poly), 1);
        SetShadeTex((void *)(kaze_map_screen_prim_buffer + poly), 1);
        i++;
        base = off + kaze_map_screen_prim_buffer;
        *(u8 *)(base + 0x1EFC) = 0;
        base2 = kaze_map_screen_prim_buffer;
        tpage += 8;
        *(short *)(base + 0x1F00) = x0;
        *(short *)(base + 0x1F02) = y0;
        *(short *)(base + 0x1F04) = x1;
        *(short *)(base + 0x1F06) = y0;
        *(short *)(base + 0x1F08) = x0;
        *(short *)(base + 0x1F0A) = y1;
        *(short *)(base + 0x1F0C) = x1;
        *(short *)(base + 0x1F0E) = y1;
        *(u8 *)((char *)off + base2 + 0x1EFD) = 0;
        poly += 0x18;
        *(u8 *)(off + kaze_map_screen_prim_buffer + 0x1EFE) = 0;
        off += 0x18;
    } while (i < 2);
}

/**
 * kaze_map_fade_quad_draw  (map_screen, 0x800501c8)
 *
 * Draws the full-screen fade quad: writes RGB=val into the +0x1ef8 POLY_F4
 * of buffer idx and AddPrims it plus its TPAGE.
 */

void kaze_map_fade_quad_draw(int idx, u8 val)
{
    extern Slot kaze_map_screen_prim_ot_ptr[];
    extern int kaze_map_screen_prim_buffer;
    int i, j;

    i = idx * 0x18;
    *(u8 *)(i + kaze_map_screen_prim_buffer + 0x1efc) = val;
    *(u8 *)(i + kaze_map_screen_prim_buffer + 0x1efd) = val;
    *(u8 *)(i + kaze_map_screen_prim_buffer + 0x1efe) = val;
    i += 0x1ef8;
    AddPrim(kaze_map_screen_prim_ot_ptr[idx].ptr, (void *)(kaze_map_screen_prim_buffer + i));
    j = idx * 8 + 0x1f28;
    AddPrim(kaze_map_screen_prim_ot_ptr[idx].ptr, (void *)(kaze_map_screen_prim_buffer + j));
}

extern int kaze_map_cursor_x;
extern int kaze_map_cursor_z;

/**
 * kaze_start_location_commit  (map_screen, 0x80050280)
 *
 * Commits the selected start location into staged/live start coordinates
 * when the map screen exits at state 9.
 */

void kaze_start_location_commit(void)
{
    kaze_staged_start_y = -0x64;
    kaze_staged_start_wind_dir = kaze_map_selected_wind;
    kaze_staged_start_x = (kaze_map_cursor_x / 2) * 0xED8 + 0x76C;
    kaze_staged_start_z = (kaze_map_cursor_z / 2) * 0xED8 + 0x76C;
}

extern int kaze_menu_variant_source;
extern void kaze_play_sound_effect();
void kaze_static_map_screen_init();
extern void GsInitVcount();
extern void VSync();
extern void kaze_gs_clear_vcount();
extern int GsGetActiveBuff();
extern void kaze_gs_set_work_base();
extern void GsClearOt();
void kaze_static_map_screen_update();
extern int GsGetVcount();
extern void kaze_input_edge_detect();
extern void GsSwapDispBuff();
extern void GsDrawOt();
void kaze_static_map_screen_free();
extern void kaze_seq_stop_track_34310();

/**
 * kaze_start_location_skip_path  (map_screen, 0x8005031c)
 *
 * Alternate start path taken when kaze_staged_stage_variant is nonzero,
 * bypassing the interactive map screen.
 */

void kaze_start_location_skip_path(int base0, int base1)
{
    extern void DrawSync();
    extern int kaze_map_screen_ot_table[];
    int i;
    int *ot;
    int arg;
    int idx;

    kaze_play_sound_effect(10, 100, 0, 0);
    kaze_static_map_screen_init();
    GsInitVcount();
    VSync(0);
    kaze_gs_clear_vcount();
    while (kaze_map_state != 9) {
        i = GsGetActiveBuff();
        arg = base1;
        if (i == 0) {
            arg = base0;
        }
        kaze_gs_set_work_base(arg);
        ot = &kaze_map_screen_ot_table[i * 5];
        GsClearOt(0, 0, ot);
        kaze_static_map_screen_update(i);
        DrawSync(0);
        kaze_menu_vcount_timestamp = GsGetVcount();
        VSync(0);
        kaze_gs_clear_vcount();
        kaze_input_edge_detect();
        GsSwapDispBuff();
        GsDrawOt(ot);
    }
    kaze_static_map_screen_free();
    idx = (kaze_menu_variant_source - 1) * 3;
    {
        int b0, b1, b2, x, z;
        b0 = kaze_skip_start_x_tile_table[idx];
        x = b0 * 0xed8;
        b1 = kaze_skip_start_z_tile_table[idx];
        kaze_staged_start_x = x + 0x76c;
        z = b1 * 0xed8;
        b2 = kaze_skip_start_wind_table[idx];
        kaze_staged_start_z = z + 0x76c;
        kaze_staged_start_wind_dir = b2;
    }
    kaze_seq_stop_track_34310(10);
    return;
}

extern int kaze_input_current_c;
extern int kaze_menu_input_prev_mask;
extern int kaze_input_edge;
extern void ClearImage(RECT *, int, int, int);
extern int kaze_heap_alloc(int, int);
void kaze_static_map_bg_prim_init();

/**
 * kaze_static_map_screen_init  (map_screen, 0x800504e0)
 *
 * Static start-map screen init (kaze_start_location_skip_path skip path):
 * clears VRAM, allocates OT headers + 0x1fa8 prim buffer
 * kaze_map_screen_prim_buffer, zeroes input mask kaze_input_edge and
 * latches, sets kaze_map_state=4 fade 0xff, inits fade+bg prims.
 */

void kaze_static_map_screen_init(void)
{
    extern void DrawSync(int);
    extern int kaze_map_screen_prim_buffer;
    extern int kaze_map_screen_prim_ot_ptr;
    extern int kaze_map_screen_ot_table;
    RECT local;
    int *p;

    SetDispMask(0);
    local.w = 0x140;
    local.x = 0;
    local.y = 0;
    local.h = 0x1E0;
    ClearImage(&local, 0, 0, 0);
    DrawSync(0);
    SetDispMask(1);
    p = &kaze_map_ot_heap_alloc_a;
    *p = kaze_heap_alloc(0x10, 4);
    kaze_map_ot_heap_alloc_b = kaze_heap_alloc(0x10, 4);
    kaze_map_screen_ot_table = 4;
    D_800AAF0C = 4;
    kaze_map_heap_ptr_alt = kaze_map_ot_heap_alloc_b;
    kaze_map_screen_prim_ot_ptr = *p;
    kaze_map_screen_prim_buffer = kaze_heap_alloc(1, 0x1FA8);
    kaze_input_current_c = 0;
    kaze_menu_input_prev_mask = 0;
    kaze_input_edge = 0;
    D_800B1A48 = 0;
    kaze_map_state = 4;
    kaze_map_fade_counter = 0xFF;
    kaze_map_fade_quad_init(2);
    kaze_static_map_bg_prim_init();
}

void kaze_static_map_bg_draw();

/**
 * kaze_static_map_screen_update  (map_screen, 0x800505f0)
 *
 * Static start-map screen states: 4 fades in to 6, 6 waits for Circle
 * (kaze_input_edge & 0x20) then 5 fades out to exit state 9
 * (kaze_map_state); draws the background via kaze_static_map_bg_draw.
 */

void kaze_static_map_screen_update(int idx)
{
    extern int kaze_map_screen_prim_ot_ptr;
    extern u8 *kaze_map_screen_prim_buffer;
    register int c asm("$3");
    register int t asm("$5");
    int a;

    switch (kaze_map_state) {
    case 4:
        c = kaze_map_fade_counter;
        if (c >= 0) {
            t = idx * 0x18;
            a = idx * 4;
            *(u8 *)(t + (int)kaze_map_screen_prim_buffer + 0x1efc) = c;
            a = a + idx;
            *(u8 *)(t + (int)kaze_map_screen_prim_buffer + 0x1efd) = c;
            a = a * 4;
            *(u8 *)(t + (int)kaze_map_screen_prim_buffer + 0x1efe) = c;
            AddPrim(*(void **)((int)&kaze_map_screen_prim_ot_ptr + a), kaze_map_screen_prim_buffer + (t + 0x1ef8));
            AddPrim(*(void **)((int)&kaze_map_screen_prim_ot_ptr + a), kaze_map_screen_prim_buffer + (idx * 8 + 0x1f28));
            kaze_map_fade_counter = kaze_map_fade_counter - 8;
        } else {
            kaze_map_state = 6;
            kaze_map_fade_counter = 0;
        }
        break;
    case 6:
        if ((kaze_input_edge & 0x20) != 0) {
            kaze_map_state = 5;
        }
        break;
    case 5:
        c = kaze_map_fade_counter;
        if (c >= 0x100) {
            kaze_map_state = 9;
            return;
        }
        t = idx * 0x18;
        a = idx * 4;
        *(u8 *)(t + (int)kaze_map_screen_prim_buffer + 0x1efc) = c;
        a = a + idx;
        *(u8 *)(t + (int)kaze_map_screen_prim_buffer + 0x1efd) = c;
        a = a * 4;
        *(u8 *)(t + (int)kaze_map_screen_prim_buffer + 0x1efe) = c;
        AddPrim(*(void **)((int)&kaze_map_screen_prim_ot_ptr + a), kaze_map_screen_prim_buffer + (t + 0x1ef8));
        AddPrim(*(void **)((int)&kaze_map_screen_prim_ot_ptr + a), kaze_map_screen_prim_buffer + (idx * 8 + 0x1f28));
        kaze_map_fade_counter = kaze_map_fade_counter + 8;
        break;
    }
    kaze_static_map_bg_draw(idx);
}

extern void kaze_heap_free(int);

/**
 * kaze_static_map_screen_free  (map_screen, 0x80050820)
 *
 * Frees the static start-map screen OT buffers
 * kaze_map_ot_heap_alloc_a/kaze_map_ot_heap_alloc_b and the prim buffer
 * kaze_map_screen_prim_buffer via kaze_heap_free.
 */

void kaze_static_map_screen_free(void)
{
    extern int kaze_map_screen_prim_buffer;
    kaze_heap_free(kaze_map_ot_heap_alloc_a);
    kaze_heap_free(kaze_map_ot_heap_alloc_b);
    kaze_heap_free(kaze_map_screen_prim_buffer);
}

extern void SetSprt();
extern int GetClut();

/**
 * kaze_static_map_bg_prim_init  (map_screen, 0x80050868)
 *
 * Inits the full-screen background image as a 0x100+0x40 wide x 0xf0 tall
 * SPRT pair per buffer at +0x1f38 (cluts (0x140,499); TPages 0x2c0/0x340)
 * with DR_TPAGEs at +0x1f88.
 */

void kaze_static_map_bg_prim_init(void) {
    extern int GetTPage();
    extern void SetDrawTPage();
    extern void SetShadeTex();
    extern int kaze_map_screen_prim_buffer;
    int s0, s1, s2, s3;
    int m120, hf0;
    register char *p asm("$3");
    register char *q asm("$6");
    s3 = 0; m120 = -0x78; hf0 = 0xF0; s2 = 0x1F88; s1 = 0x1F38; s0 = 0;
    do {
        SetSprt(kaze_map_screen_prim_buffer + s1);
        SetShadeTex(kaze_map_screen_prim_buffer + s1, 1);
        p = (char *)(s0 + kaze_map_screen_prim_buffer);
        *(char *)(p + 0x1F44) = 0;
        q = (char *)(s0 + kaze_map_screen_prim_buffer);
        *(s16 *)(p + 0x1F40) = -0xA0;
        *(s16 *)(p + 0x1F42) = m120;
        *(s16 *)(p + 0x1F48) = 0x100;
        *(s16 *)(p + 0x1F4A) = hf0;
        *(char *)(q + 0x1F45) = 0;
        *(s16 *)(kaze_map_screen_prim_buffer + s0 + 0x1F46) = GetClut(0x140, 0x1F3, q);
        SetDrawTPage(kaze_map_screen_prim_buffer + s2, 0, 1, GetTPage(1, 0, 0x2C0, 0x100) & 0xFFFF);
        s3 += 1;
        SetSprt(kaze_map_screen_prim_buffer + s1 + 0x14);
        SetShadeTex(kaze_map_screen_prim_buffer + s1 + 0x14, 1);
        p = (char *)(s0 + kaze_map_screen_prim_buffer);
        *(char *)(p + 0x1F58) = 0;
        q = (char *)(s0 + kaze_map_screen_prim_buffer);
        *(s16 *)(p + 0x1F54) = 0x60;
        *(s16 *)(p + 0x1F56) = m120;
        *(s16 *)(p + 0x1F5C) = 0x40;
        *(s16 *)(p + 0x1F5E) = hf0;
        *(char *)(q + 0x1F59) = 0;
        *(s16 *)(kaze_map_screen_prim_buffer + s0 + 0x1F5A) = GetClut(0x140, 0x1F3, q);
        SetDrawTPage(kaze_map_screen_prim_buffer + s2 + 8, 0, 1, GetTPage(1, 0, 0x340, 0x100) & 0xFFFF);
        s1 += 0x28;
        s0 += 0x28;
        s2 += 0x10;
    } while (s3 < 2);
}

/**
 * kaze_static_map_bg_draw  (map_screen, 0x80050a3c)
 *
 * AddPrims the full-screen background SPRT pair (+0x1f38) and their
 * DR_TPAGEs (+0x1f88) for buffer idx.
 */

void kaze_static_map_bg_draw(int idx)
{
    extern char kaze_map_screen_prim_ot_ptr[];
    extern int kaze_map_screen_prim_buffer;
    int tmp;
    int ot_ofs;
    int bg_ofs;

    tmp = idx * 4;
    tmp += idx;
    ot_ofs = tmp * 4;
    bg_ofs = tmp * 8;
    bg_ofs += 0x1F38;
    AddPrim((void *)*(int *)(kaze_map_screen_prim_ot_ptr + ot_ofs), (void *)(kaze_map_screen_prim_buffer + bg_ofs));

    __asm__ volatile("" : "=r"(idx) : "0"(idx));
    idx <<= 4;
    idx += 0x1F88;
    AddPrim((void *)*(int *)(kaze_map_screen_prim_ot_ptr + ot_ofs), (void *)(kaze_map_screen_prim_buffer + idx));
    AddPrim((void *)*(int *)(kaze_map_screen_prim_ot_ptr + ot_ofs), (void *)(kaze_map_screen_prim_buffer + bg_ofs + 0x14));
    AddPrim((void *)*(int *)(kaze_map_screen_prim_ot_ptr + ot_ofs), (void *)(kaze_map_screen_prim_buffer + idx + 0x8));
}
