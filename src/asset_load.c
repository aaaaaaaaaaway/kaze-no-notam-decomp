#include "types.h"
#include "kaze_syms.h"
extern char kaze_mapdata_file_path[];
extern char kaze_texdata_file_path[];
extern char kaze_rmode_file_path[];
extern char kaze_loading_screen_tim_fmt[];
extern char kaze_common_ssp_tim_path[];
extern char kaze_common_demotext_tim_path[];
extern char kaze_common_tex02_tim_path[];
extern char kaze_common_tex01_tim_path[];
extern char kaze_common_console_tim_path[];
extern char kaze_balmodel_file_path[];
extern char kaze_texture_tbl_file_path[];
extern unsigned int kaze_str_frame_cached_width;
extern unsigned int kaze_str_frame_cached_height;
extern int kaze_str_movie_end_flag;
extern int kaze_str_vlc_buffer_a;
extern int kaze_str_vlc_buffer_b;
extern int kaze_str_mdec_buffer_a;
extern int kaze_str_mdec_buffer_b;
extern int kaze_staged_stage_level;
extern int CdRead2(int);
extern void ClearOTagR(int, int);
extern void DecDCTvlc(int, int);
extern void DrawOTagEnv(int, int);
extern void PutDrawEnv(int);
extern void SetDefDrawEnv(void *, int, int, int, int);
extern void SetDispMask(int);
extern void SetDrawLoad(void *, void *);
extern void SetLineF4(void *);
extern void SetPolyF3(void *);
extern void SetPolyF4(void *);
extern void SetPolyG4(void *);
extern void SetSprt8(void *);
extern void SetTile1(void *);
extern void StFreeRing(int);
extern unsigned int StGetNext(unsigned int **, unsigned int **);
extern void TermPrim(int);
extern void exit(int);
extern int rand(void);
extern void srand(unsigned int);
extern void kaze_start_location_map_loop(int, int, int, int);
typedef struct {
    short x, y, w, h;
} RECT;
typedef struct { int a, b, c, d; } Blk10;
typedef struct
{
  int a;
  int b;
  int c;
  int d;
} Blk10__1;
typedef struct
{
  int a;
  int b;
} Blk8;
struct blk78
{
  int w[30];
};
struct blk28
{
  int w[10];
};
struct blk12c0
{
  int w[1200];
};
struct blkc80
{
  int w[800];
};
typedef struct { int w[15]; } Blk3C;
typedef struct { int w[20]; } Blk50;
typedef struct { int w[30]; } Blk78;
typedef struct { int w[24]; } Blk60;
typedef struct { int w[68]; } Blk110;
typedef struct { int w[18]; } Blk48;
typedef struct 
{
  int w[36];
} Blk90;
typedef struct 
{
  int w[12];
} Blk30;
typedef struct {
    short x;
    short y;
    short w;
    short h;
} RECT__1;
typedef struct { unsigned short f; } U16S;
typedef struct { short x, y, w, h; } SRECT;
typedef struct { int w0, w1, w2, w3; } W4;
typedef struct { int w0, w1, w2; } W3;
typedef struct { int w0, w1; } W2;

int kaze_str_next_frame_fetch();

/**
 * kaze_str_frame_vlc_decode  (cd_stream, 0x80013a2c)
 *
 * Fetches the next STR frame (kaze_str_next_frame_fetch), flips the VLC
 * double-buffer index at ctx+0x8, runs DecDCTvlc into the current buffer,
 * and StFreeRing's the sector.
 */

void kaze_str_frame_vlc_decode(int ctx)
{
    int i;
    int bs;
    unsigned int flag;

    i = 0x800000;
    for (;;) {
        bs = kaze_str_next_frame_fetch(ctx);
        if (bs != 0) goto found;
        if (--i == 0) goto out;
    }

found:
    flag = *(int *)(ctx + 8) == 0;
    *(unsigned int *)(ctx + 8) = flag;
    DecDCTvlc(bs, *(int *)(flag * 4 + ctx));
    StFreeRing(bs);
out:
    return;
}

extern unsigned int kaze_str_movie_stream_count;
extern void ClearImage(RECT *, int, int, int);

/**
 * kaze_str_next_frame_fetch  (cd_stream, 0x80013ab4)
 *
 * Polls StGetNext for the next STR frame; sets end flag
 * kaze_str_movie_end_flag at the last frame number
 * (kaze_str_movie_stream_count-1), clears the screen and updates cached
 * width/height (kaze_str_frame_cached_width/kaze_str_frame_cached_height) on
 * resolution change.
 */

int kaze_str_next_frame_fetch(int ctx)
{
    unsigned int status;
    int i;
    u16 t1;
    u16 t2;
    u16 w;
    u16 h;
    RECT rect;
    unsigned int *frame_data;
    unsigned int *ret;
    unsigned int *frame_hdr;

    i = 0x800000;
    do {
        status = StGetNext(&frame_data, &frame_hdr);
        i = i - 1;
        if (status == 0) {
            goto found;
        }
    } while (i != 0);
    return 0;

found:
    if (kaze_str_movie_stream_count - 1 <= frame_hdr[2]) {
        kaze_str_movie_end_flag = 1;
    }
    if ((kaze_str_frame_cached_width != *(u16 *)((int)frame_hdr + 0x10)) ||
        (kaze_str_frame_cached_height != *(u16 *)((int)frame_hdr + 0x12))) {
        rect.w = 0x140;
        rect.x = 0;
        rect.y = 0;
        rect.h = 0x1E0;
        ClearImage(&rect, 0, 0, 0);
        t1 = *(u16 *)((int)frame_hdr + 0x10);
        t2 = *(u16 *)((int)frame_hdr + 0x12);
        kaze_str_frame_cached_width = t1;
        kaze_str_frame_cached_height = t2;
    }
    w = kaze_str_frame_cached_width;
    h = kaze_str_frame_cached_height;
    ret = frame_data;
    *(u16 *)(ctx + 0x24) = w;
    *(u16 *)(ctx + 0x1C) = w;
    *(u16 *)(ctx + 0x26) = h;
    *(u16 *)(ctx + 0x1E) = h;
    *(u16 *)(ctx + 0x32) = h;
    return (int)ret;
}

extern int CdControl(u8, u8 *, u8 *);

/**
 * kaze_cd_stream_seek_start  (cd_stream, 0x80013bd4)
 *
 * Retries CdControl seek to the CdlLOC then starts continuous streaming with
 * CdRead2(0x1c0).
 */

void kaze_cd_stream_seek_start(u8 *arg0)
{
    while (CdControl(2, arg0, 0) == 0) {}
    while (CdRead2(0x1C0) == 0) {}
}

extern int kaze_heap_free_list_head[];
extern int kaze_str_stream_ring_buffer;

/**
 * kaze_str_playback_buffers_free  (cd_stream, 0x80013c20)
 *
 * Frees the five STR playback heap blocks (VLC buffers
 * kaze_str_vlc_buffer_a/c8, image buffers kaze_str_mdec_buffer_a/d4, ring
 * kaze_str_stream_ring_buffer) with inlined coalescing free.
 */

void kaze_str_playback_buffers_free(void)
{
    int *bp;
    int *p;
    int *next;
    int size;
    int psize;
    int ptr;

    ptr = kaze_str_vlc_buffer_a;
    if (ptr != 0) {
        bp = (int *)(ptr - 8);
        p = kaze_heap_free_list_head;
        while (!((p < bp) && (bp < (int *)*p))) {
            next = (int *)*p;
            if (next == kaze_heap_free_list_head) {
                break;
            }
            p = next;
        }
        size = bp[1];
        next = (int *)*p;
        if (bp + size * 2 == next) {
            bp[1] = size + next[1];
            *bp = *(int *)*p;
        } else {
            *bp = (int)next;
        }
        psize = p[1];
        if (p + psize * 2 == bp) {
            p[1] = psize + bp[1];
            *p = *bp;
        } else {
            *p = (int)bp;
        }
    }

    ptr = kaze_str_vlc_buffer_b;
    if (ptr != 0) {
        bp = (int *)(ptr - 8);
        p = kaze_heap_free_list_head;
        while (!((p < bp) && (bp < (int *)*p))) {
            next = (int *)*p;
            if (next == kaze_heap_free_list_head) {
                break;
            }
            p = next;
        }
        size = bp[1];
        next = (int *)*p;
        if (bp + size * 2 == next) {
            bp[1] = size + next[1];
            *bp = *(int *)*p;
        } else {
            *bp = (int)next;
        }
        psize = p[1];
        if (p + psize * 2 == bp) {
            p[1] = psize + bp[1];
            *p = *bp;
        } else {
            *p = (int)bp;
        }
    }

    ptr = kaze_str_mdec_buffer_a;
    if (ptr != 0) {
        bp = (int *)(ptr - 8);
        p = kaze_heap_free_list_head;
        while (!((p < bp) && (bp < (int *)*p))) {
            next = (int *)*p;
            if (next == kaze_heap_free_list_head) {
                break;
            }
            p = next;
        }
        size = bp[1];
        next = (int *)*p;
        if (bp + size * 2 == next) {
            bp[1] = size + next[1];
            *bp = *(int *)*p;
        } else {
            *bp = (int)next;
        }
        psize = p[1];
        if (p + psize * 2 == bp) {
            p[1] = psize + bp[1];
            *p = *bp;
        } else {
            *p = (int)bp;
        }
    }

    ptr = kaze_str_mdec_buffer_b;
    if (ptr != 0) {
        bp = (int *)(ptr - 8);
        p = kaze_heap_free_list_head;
        while (!((p < bp) && (bp < (int *)*p))) {
            next = (int *)*p;
            if (next == kaze_heap_free_list_head) {
                break;
            }
            p = next;
        }
        size = bp[1];
        next = (int *)*p;
        if (bp + size * 2 == next) {
            bp[1] = size + next[1];
            *bp = *(int *)*p;
        } else {
            *bp = (int)next;
        }
        psize = p[1];
        if (p + psize * 2 == bp) {
            p[1] = psize + bp[1];
            *p = *bp;
        } else {
            *p = (int)bp;
        }
    }

    ptr = kaze_str_stream_ring_buffer;
    if (ptr != 0) {
        bp = (int *)(ptr - 8);
        p = kaze_heap_free_list_head;
        while (!((p < bp) && (bp < (int *)*p))) {
            next = (int *)*p;
            if (next == kaze_heap_free_list_head) {
                break;
            }
            p = next;
        }
        size = bp[1];
        next = (int *)*p;
        if (bp + size * 2 == next) {
            bp[1] = size + next[1];
            *bp = *(int *)*p;
        } else {
            *bp = (int)next;
        }
        psize = p[1];
        if (p + psize * 2 == bp) {
            p[1] = psize + bp[1];
            *p = *bp;
        } else {
            *p = (int)bp;
        }
    }
}


/**
 * kaze_map_cdlfile_lookup  (cd_stream, 0x8001404c)
 *
 * Resolves CdlFILE metadata for \BAL\MAP\MAPDATA.DAT, TEXDATA.DAT, and
 * RMODE.DAT into live-root +0x48224/+0x4823c/+0x48254.
 */

void kaze_map_cdlfile_lookup(void)
{
    extern int kaze_state_base_ptr;
    while (kaze_cd_file_lookup_cached(kaze_state_base_ptr + 0x48224, kaze_mapdata_file_path) == 0) {}
    while (kaze_cd_file_lookup_cached(kaze_state_base_ptr + 0x4823C, kaze_texdata_file_path) == 0) {}
    while (kaze_cd_file_lookup_cached(kaze_state_base_ptr + 0x48254, kaze_rmode_file_path) == 0) {}
}

extern void SetDefDispEnv(void *, int, int, int, int);
extern void kaze_set_geom_screen(int);
void kaze_loading_screen_show();
void kaze_common_assets_load();
void kaze_asset_prim_master_init();
void kaze_env_prim_init();
void kaze_post_event_sprite_init();
void kaze_post_event_menu_prim_init();
void kaze_post_event_frame_prim_init();
void kaze_environment_lookup_init();
void kaze_post_event_split_prim_init();
void kaze_post_event_grid_prim_init();
void kaze_post_event_banner_prim_init();

/**
 * kaze_drawenv_init  (render, 0x800140e4)
 *
 * Initializes primary and secondary DRAWENV/DISPENV blocks (+0xbe..+0xc0,
 * +0x24136..+0x24138, draw/disp bases +0xa8/+0x104/+0x24120/+0x2417c).
 */

void kaze_drawenv_init(void)
{
    extern int VSync(int);
    extern int kaze_state_base_ptr;
    int h;
    unsigned int seed;

    h = 0xf0;
    SetDefDrawEnv((void *)(kaze_state_base_ptr + 0xa8), 0, 0, 0x140, h);
    SetDefDrawEnv((void *)(kaze_state_base_ptr + 0x24120), 0, 0x100, 0x140, h);
    SetDefDispEnv((void *)(kaze_state_base_ptr + 0x104), 0, 0x100, 0x140, h);
    SetDefDispEnv((void *)(kaze_state_base_ptr + 0x2417c), 0, 0, 0x140, h);

    {
        register int base __asm__("$2");
        base = kaze_state_base_ptr;
        *(char *)(base + 0x24137) = 0;
        *(char *)(base + 0xbf) = 0;
    }
    {
        register int base __asm__("$3");
        base = kaze_state_base_ptr;
        *(char *)(base + 0x24136) = 1;
        *(char *)(base + 0xbe) = 1;
    }
    {
        register int base __asm__("$2");
        base = kaze_state_base_ptr;
        *(char *)(base + 0x24138) = 0;
        *(char *)(base + 0xc0) = 0;
    }

    kaze_set_geom_offset(0xa0, 0x78);
    kaze_set_geom_screen(0xfa);
    kaze_loading_screen_show();
    kaze_common_assets_load();
    kaze_asset_prim_master_init();
    kaze_env_prim_init();
    kaze_post_event_sprite_init();
    kaze_post_event_menu_prim_init();
    kaze_post_event_frame_prim_init();
    kaze_environment_lookup_init();
    kaze_post_event_split_prim_init();
    kaze_post_event_grid_prim_init();
    kaze_post_event_banner_prim_init();
    seed = VSync(-1);
    srand(seed);
}

extern int sprintf(char *, char *, int);
extern void PutDispEnv(int);
extern void kaze_cd_load_tim_upload(char *, int, int, int, int);
extern void MoveImage(RECT *, int, int);

/**
 * kaze_loading_screen_show  (render, 0x80014260)
 *
 * Shows the stage loading screen: loads \BAL\M\LOAD%d.TIM selected by
 * staging field kaze_staged_stage_level, sets DISPENV/DRAWENV from live-root
 * +0x2417c/+0xa8, MoveImage's the second half, DrawSync/VSync,
 * SetDispMask(1).
 */

void kaze_loading_screen_show(void)
{
    extern void VSync(int);
    extern void DrawSync(int);
    extern int kaze_state_base_ptr;
    extern RECT D_80010278;
    RECT rect = D_80010278;
    char buf[64];

    sprintf(buf, kaze_loading_screen_tim_fmt, kaze_staged_stage_level);
    PutDispEnv(kaze_state_base_ptr + 0x2417C);
    PutDrawEnv(kaze_state_base_ptr + 0xA8);
    kaze_cd_load_tim_upload(buf, 0, 0, 0, 0);
    MoveImage(&rect, 0, 0x100);
    DrawSync(0);
    VSync(0);
    SetDispMask(1);
}

extern void kaze_cd_load_file_into(char *, u8 *, int);

/**
 * kaze_common_assets_load  (cd_stream, 0x8001432c)
 *
 * Loads shared assets: SSP.TIM, DEMOTEXT.TIM, TEX02/TEX01.TIM, CONSOLE.TIM
 * into VRAM via the TIM loader, plus BALMODEL.DAT into live-root +0x509e8
 * and TEXTURE.TBL into +0x5ae98.
 */

void kaze_common_assets_load(void)
{
    extern u8 *kaze_state_base_ptr;
    kaze_cd_load_tim_upload(kaze_common_ssp_tim_path, -1, -1, -1, -1);
    kaze_cd_load_tim_upload(kaze_common_demotext_tim_path, 0x380, 0x100, 0x140, 500);
    kaze_cd_load_tim_upload(kaze_common_tex02_tim_path, 0x240, 0x100, 0x140, 0x1F7);
    kaze_cd_load_tim_upload(kaze_common_tex01_tim_path, 0x200, 0x100, 0x140, 0x1F9);
    kaze_cd_load_tim_upload(kaze_common_console_tim_path, 0x180, 0x100, 0x140, 0x1FB);
    kaze_cd_load_file_into(kaze_balmodel_file_path, kaze_state_base_ptr + 0x509E8, 0x8000);
    kaze_cd_load_file_into(kaze_texture_tbl_file_path, kaze_state_base_ptr + 0x5AE98, 0x800);
    return;
}

extern void SetLineF2(void *);
extern void SetShadeTex(void *, int);
extern void SetSemiTrans(void *, int);
extern void SetDrawTPage(void *, int, int, unsigned int);
extern int GetTPage(int, int, int, int);
extern void kaze_tmd_model_relocate_3462c(void);
extern void kaze_model_set_translation_component_348b0(void *, int, unsigned int);

/**
 * kaze_asset_prim_master_init  (render, 0x8001442c)
 *
 * Master asset/primitive initializer: rebases BALMODEL submodel pointers
 * into +0x589e8.. slots from the +0x509ec offset array, assigns TPage/CLUTs,
 * builds balloon LINE_F2/POLY_FT4 overlays, seeds the +0xecb00 animation
 * ring and stage-flag transforms.
 */

void kaze_asset_prim_master_init(void)
{
    extern short GetClut(int, int);
    extern void SetPolyFT4(void *);
    extern char *kaze_state_base_ptr;
    u16 tex_attr;
    int reserved_i2;
    int ram_ptr;
    int prim_base;
    Blk10 *copy_src;
    Blk10 *copy_dst;
    Blk10 *copy_end;
    int sched_pad_a;
    uint sched_pad_b;
    int sched_pad_c;
    int pc;

    {
        register int c_slot asm("$19");
        register uint cnt asm("$18");
        register int c_ent asm("$17");
        register int c_val asm("$16");
        register int base asm("$3");
        int root;
        register unsigned int val asm("$4");
        cnt = 0;
        c_slot = 0x589e8;
        c_ent = 0x509ec;
        c_val = 0x509e8;
        __asm__("" : "=r"(c_slot) : "0"(c_slot));
        __asm__("" : "=r"(c_ent) : "0"(c_ent));
        __asm__("" : "=r"(c_val) : "0"(c_val));
        while (1) {
            root = (int)kaze_state_base_ptr;
            if (*(uint *)(root + 0x509e8) <= cnt) break;
            { int _s = cnt * 4; base = _s + root; }
            val = *(uint *)(base + c_ent);
            val = root + (((val >> 2) << 2) + c_val);
            *(uint *)(base + c_slot) = val;
            cnt = cnt + 1;
            kaze_tmd_model_relocate_3462c();
            if ((int)cnt >= 0xb) break;
        }
    }
    tex_attr = GetTPage(0, 0, 0x1c0, 0x100);
    kaze_model_set_rotation_component_3489c(*(void **)((int)kaze_state_base_ptr + 0x589ec), 0, (unsigned int)tex_attr);
    tex_attr = GetClut(0x140, 0x1fc);
    kaze_model_set_translation_component_348b0(*(void **)((int)kaze_state_base_ptr + 0x589ec), 0, (unsigned int)tex_attr);
    tex_attr = GetTPage(0, 0, 0x240, 0x100);
    kaze_model_set_rotation_component_3489c(*(void **)((int)kaze_state_base_ptr + 0x589f4), 0, (unsigned int)tex_attr);
    tex_attr = GetClut(0x140, 0x1f7);
    kaze_model_set_translation_component_348b0(*(void **)((int)kaze_state_base_ptr + 0x589f4), 0, (unsigned int)tex_attr);
    tex_attr = GetTPage(0, 0, 0x240, 0x100);
    kaze_model_set_rotation_component_3489c(*(void **)((int)kaze_state_base_ptr + 0x589f8), 0, (unsigned int)tex_attr);
    tex_attr = GetClut(0x140, 0x1f7);
    kaze_model_set_translation_component_348b0(*(void **)((int)kaze_state_base_ptr + 0x589f8), 0, (unsigned int)tex_attr);
    tex_attr = GetTPage(0, 0, 0x240, 0x100);
    kaze_model_set_rotation_component_3489c(*(void **)((int)kaze_state_base_ptr + 0x589fc), 0, (unsigned int)tex_attr);
    tex_attr = GetClut(0x140, 0x1f7);
    kaze_model_set_translation_component_348b0(*(void **)((int)kaze_state_base_ptr + 0x589fc), 0, (unsigned int)tex_attr);
    tex_attr = GetTPage(0, 0, 0x1c0, 0x100);
    kaze_model_set_rotation_component_3489c(*(void **)((int)kaze_state_base_ptr + 0x58a00), 0, (unsigned int)tex_attr);
    tex_attr = GetClut(0x140, 0x1fc);
    kaze_model_set_translation_component_348b0(*(void **)((int)kaze_state_base_ptr + 0x58a00), 0, (unsigned int)tex_attr);
    tex_attr = GetTPage(0, 0, 0x240, 0x100);
    kaze_model_set_rotation_component_3489c(*(void **)((int)kaze_state_base_ptr + 0x58a04), 0, (unsigned int)tex_attr);
    tex_attr = GetClut(0x150, 0x1f7);
    kaze_model_set_translation_component_348b0(*(void **)((int)kaze_state_base_ptr + 0x58a04), 0, (unsigned int)tex_attr);
    tex_attr = GetTPage(0, 0, 0x240, 0x100);
    kaze_model_set_rotation_component_3489c(*(void **)((int)kaze_state_base_ptr + 0x58a04), 1, (unsigned int)tex_attr);
    tex_attr = GetClut(0x160, 0x1f7);
    kaze_model_set_translation_component_348b0(*(void **)((int)kaze_state_base_ptr + 0x58a04), 1, (unsigned int)tex_attr);
    kaze_prim_color_table_copy(*(void **)((int)kaze_state_base_ptr + 0x589e8));
    {
    register int line_i asm("$18");
    register int line_p asm("$16");
    register int c96 asm("$19");
    register int c100 asm("$17");
    line_i = 0;
    c96 = 0x96;
    c100 = 100;
    {
    int init_base;
    register int c4096 asm("$4");
#define state_base_ref init_base
    line_p = 0x48924;
    state_base_ref = (int)kaze_state_base_ptr;
    ram_ptr = *(int *)((int)kaze_state_base_ptr + 0x589ec);
    prim_base = *(int *)(*(int *)((int)kaze_state_base_ptr + 0x589e8) + 0x1b8);
    *(int *)((int)kaze_state_base_ptr + 0x58a14) = prim_base + 0x418;
    *(int *)(state_base_ref + 0x58a18) = prim_base + 0x2b8;
    *(int *)(state_base_ref + 0x58a1c) = prim_base + 0x90;
    *(int *)(state_base_ref + 0x58a20) = prim_base + 0x208;
    prim_base = *(int *)(ram_ptr + 0x1b8);
    ram_ptr = *(int *)(state_base_ref + 0x589f4);
    *(int *)(state_base_ref + 0x58a24) = prim_base + 0x20;
    *(int *)(state_base_ref + 0x58a28) = prim_base + 0x28;
    *(int *)(state_base_ref + 0x58a2c) = prim_base + 0x30;
    *(int *)(state_base_ref + 0x58a30) = prim_base + 0x38;
    prim_base = *(int *)(ram_ptr + 0x1b8);
    ram_ptr = *(int *)(state_base_ref + 0x589f8);
    *(int *)(state_base_ref + 0x58a34) = prim_base + 0xd0;
    *(int *)(state_base_ref + 0x58a38) = prim_base + 0xe8;
    *(int *)(state_base_ref + 0x58a3c) = prim_base + 0xe0;
    *(int *)(state_base_ref + 0x58a40) = prim_base + 0xd8;
    prim_base = *(int *)(ram_ptr + 0x1b8);
    ram_ptr = *(int *)(state_base_ref + 0x589fc);
    *(int *)(state_base_ref + 0x58a44) = prim_base + 0x118;
    *(int *)(state_base_ref + 0x58a48) = prim_base + 0xe8;
    *(int *)(state_base_ref + 0x58a4c) = prim_base + 0x110;
    *(int *)(state_base_ref + 0x58a50) = prim_base + 0x88;
    prim_base = *(int *)(ram_ptr + 0x1b8);
    ram_ptr = *(int *)(state_base_ref + 0x58a00);
    *(int *)(state_base_ref + 0x58a54) = prim_base + 0x110;
    *(int *)(state_base_ref + 0x58a58) = prim_base + 0x130;
    *(int *)(state_base_ref + 0x58a5c) = prim_base + 0x108;
    *(int *)(state_base_ref + 0x58a60) = prim_base + 0x128;
    prim_base = *(int *)(ram_ptr + 0x1b8);
    c4096 = 0x1000;
    *(int *)(state_base_ref + 0x58a64) = prim_base + 0x20;
    *(int *)(state_base_ref + 0x58a68) = prim_base + 0x28;
    *(int *)(state_base_ref + 0x58a6c) = prim_base + 0x30;
    *(int *)(state_base_ref + 0x58a70) = prim_base + 0x38;
    *(int *)(state_base_ref + 0xed338) = 0x4000;
    *(int *)(state_base_ref + 0xed33c) = 0x4000;
    *(u16 *)(state_base_ref + 0xed35a) = 0x400;
    *(u16 *)(state_base_ref + 0xed360) = 0x800;
    *(u16 *)(state_base_ref + 0xed328) = 0;
    *(u16 *)(state_base_ref + 0xed32a) = 0;
    *(u16 *)(state_base_ref + 0xed32c) = 0;
    *(u16 *)(state_base_ref + 0xed330) = 0;
    *(u16 *)(state_base_ref + 0xed332) = 0;
    *(u16 *)(state_base_ref + 0xed334) = 0;
    *(int *)(state_base_ref + 0xed340) = c4096;
    *(u16 *)(state_base_ref + 0xed358) = 0;
    *(u16 *)(state_base_ref + 0xed35c) = 0;
    *(u16 *)(state_base_ref + 0xed362) = 0;
    *(short *)(state_base_ref + 0xed364) = -2048;
    *(int *)(state_base_ref + 0xed368) = c4096;
    *(int *)(state_base_ref + 0xed36c) = c4096;
    *(int *)(state_base_ref + 0xed370) = c4096;
#undef state_base_ref
    }
    do {
        SetLineF2((void *)((int)kaze_state_base_ptr + line_p));
        __asm__("":"=r"(line_p):"0"(line_p));
        { register int t asm("$3"); t = line_i * 0x10;
        *(u8 *)(t + (int)kaze_state_base_ptr + 0x48928) = c96;
        line_i = line_i + 1;
        *(u8 *)(t + (int)kaze_state_base_ptr + 0x48929) = c100;
        *(u8 *)(t + (int)kaze_state_base_ptr + 0x4892a) = 0x3c; }
        line_p = line_p + 0x10;
    } while (line_i < 4);
    }
    copy_dst = (Blk10 *)((int)kaze_state_base_ptr + 0x48964);
    copy_src = (Blk10 *)((int)kaze_state_base_ptr + 0x48924);
    copy_end = copy_src + 4;
    do {
        *copy_dst++ = *copy_src++;
    } while (copy_src != copy_end);
    SetLineF2((void *)((int)kaze_state_base_ptr + 0x489a4));
    pc = 0;
    *(u8 *)((int)kaze_state_base_ptr + 0x489a8) = 0x96;
    *(u8 *)((int)kaze_state_base_ptr + 0x489a9) = 100;
    *(u8 *)((int)kaze_state_base_ptr + 0x489aa) = 0x3c;
    do {
    register int t asm("$16");
        t = pc * 0x28 + 0x488d4;
        SetPolyFT4((void *)((int)kaze_state_base_ptr + t));
        SetShadeTex((void *)((int)kaze_state_base_ptr + t), 1);
        SetSemiTrans((void *)((int)kaze_state_base_ptr + t), 1);
        *(u8 *)(pc * 0x28 + (int)kaze_state_base_ptr + 0x488e0) = 0xc0;
        *(u8 *)(pc * 0x28 + (int)kaze_state_base_ptr + 0x488e1) = 0x60;
        *(u8 *)(pc * 0x28 + (int)kaze_state_base_ptr + 0x488e8) = 0xdf;
        *(u8 *)(pc * 0x28 + (int)kaze_state_base_ptr + 0x488e9) = 0x60;
        *(u8 *)(pc * 0x28 + (int)kaze_state_base_ptr + 0x488f0) = 0xc0;
        *(u8 *)(pc * 0x28 + (int)kaze_state_base_ptr + 0x488f1) = 0x7f;
        *(u8 *)(pc * 0x28 + (int)kaze_state_base_ptr + 0x488f8) = 0xdf;
        *(u8 *)(pc * 0x28 + (int)kaze_state_base_ptr + 0x488f9) = 0x7f;
        tex_attr = GetTPage(0, 1, 0x1c0, 0x100);
        { int p1 = (int)kaze_state_base_ptr; p1 += pc * 0x28; *(u16 *)(p1 + 0x488ea) = tex_attr; }
        tex_attr = GetClut(0x220, 0x1fb);
        ram_ptr = (int)kaze_state_base_ptr;
        { int p2 = ram_ptr; p2 += pc * 0x28; *(u16 *)(p2 + 0x488e2) = tex_attr; }
        pc = pc + 1;
    } while (pc < 2);
    *(u16 *)(ram_ptr + 0xecb02) = 0x8042;
    *(u16 *)(ram_ptr + 0xecb04) = 0x8084;
    *(u16 *)(ram_ptr + 0xecb06) = 0x80c6;
    *(u16 *)(ram_ptr + 0xecb08) = 0x8108;
    *(u16 *)(ram_ptr + 0xecb0a) = 0x814a;
    *(u16 *)(ram_ptr + 0xecb0c) = 0x818c;
    *(u16 *)(ram_ptr + 0xecb0e) = 0x81ce;
    *(u16 *)(ram_ptr + 0xecb10) = 0x8210;
    *(u16 *)(ram_ptr + 0xecb12) = 0x8252;
    *(u16 *)(ram_ptr + 0xecb14) = 0x8294;
    *(u16 *)(ram_ptr + 0xecb16) = 0x82d6;
    *(u16 *)(ram_ptr + 0xecb18) = 0x8318;
    *(u16 *)(ram_ptr + 0xecb1a) = 0x835a;
    *(u16 *)(ram_ptr + 0xecb1c) = 0x839c;
    *(u16 *)(ram_ptr + 0xecb1e) = 0x83de;
    *(u16 *)(ram_ptr + 0x5ae96) = 0x76c;
    *(u16 *)(ram_ptr + 0x5ae92) = 0x76c;
    *(short *)(ram_ptr + 0x5ae8e) = -9500;
    *(u16 *)(ram_ptr + 0xecb00) = 0;
    *(u16 *)(ram_ptr + 0xecb20) = 0;
    *(u16 *)(ram_ptr + 0x5ae90) = 0;
    *(u16 *)(ram_ptr + 0x5ae8c) = 0;
    *(u16 *)(ram_ptr + 0x5ae94) = 0x251c;
}

extern void SetSprt(void *);

/**
 * kaze_env_prim_init  (render, 0x80014ea8)
 *
 * One-time environment primitive initializer: sky/water POLY_G4 blocks,
 * horizon quads, DR_TPAGE arrays, buffer-1 clones, route/start marker SPRTs,
 * and packed-terrain CLUT/TPage lookups.
 */

void kaze_env_prim_init(void)
{
    extern short GetClut(int, int);
    extern char *kaze_state_base_ptr;
  u16 lookup;
  int c140;
  Blk10__1 *cs;
  Blk10__1 *cd;
  int cnt;
  Blk10__1 *ce;
  int o1;

  int ofs;
  cnt = 0;
  c140 = 0x140;
  o1 = 0;
  do
  {
    int p;

    register int offb __asm__("$16");

    register int offa __asm__("$17");
    offa = o1 + 0x48354;
    SetPolyG4(kaze_state_base_ptr + offa);
    offb = o1 + 0x48504;
    SetPolyG4(kaze_state_base_ptr + offb);
    SetSemiTrans(kaze_state_base_ptr + offa, 1);
    SetSemiTrans(kaze_state_base_ptr + offb, 1);
    cnt = cnt + 1;
    p = ((int) kaze_state_base_ptr) + o1;
    *((u16 *) (p + 0x4851c)) = 0;
    *((u16 *) (p + 0x4850c)) = 0;
    *((u16 *) (p + 0x4836c)) = 0;
    *((u16 *) (p + 0x4835c)) = 0;
    *((u16 *) (p + 0x48524)) = c140;
    *((u16 *) (p + 0x48514)) = c140;
    *((u16 *) (p + 0x48374)) = c140;
    *((u16 *) (p + 0x48364)) = c140;
    o1 = o1 + 0x24;
  }
  while (cnt < 6);
  cnt = 0;
  ofs = 0;
  do
  {
    int p;

    register int t0 __asm__("$16");
    {
      register int t1 __asm__("$4");
      t1 = 0x482f4;
      cnt = cnt + 1;
      t1 = ofs + t1;
      SetPolyF4(kaze_state_base_ptr + t1);
    }
    p = ((int) kaze_state_base_ptr) + ofs;
    *((u16 *) (p + 0x48304)) = 0;
    *((u16 *) (p + 0x482fc)) = 0;
    *((u16 *) (p + 0x48308)) = 0x140;
    *((u16 *) (p + 0x48300)) = 0x140;
    lookup = GetTPage(0, 1, 0x140, 0);
    t0 = 0x486b4;
    SetDrawTPage(kaze_state_base_ptr + (ofs + t0), 0, 1, (unsigned int) lookup);
    lookup = GetTPage(0, 2, 0x140, 0);
    SetDrawTPage(kaze_state_base_ptr + (ofs + 0x486b4) + 8, 0, 1, (unsigned int) lookup);
    lookup = GetTPage(0, 2, 0x140, 0);
    SetDrawTPage(kaze_state_base_ptr + (ofs + 0x486b4) + 0x10, 0, 1, (unsigned int) lookup);
    lookup = GetTPage(0, 1, 0x140, 0);
    t0 = 0x486e4;
    SetDrawTPage(kaze_state_base_ptr + (ofs + t0), 0, 1, (unsigned int) lookup);
    lookup = GetTPage(0, 2, 0x140, 0);
    SetDrawTPage(kaze_state_base_ptr + (ofs + 0x486e4) + 8, 0, 1, (unsigned int) lookup);
    lookup = GetTPage(0, 2, 0x140, 0);
    SetDrawTPage(kaze_state_base_ptr + (ofs + 0x486e4) + 0x10, 0, 1, (unsigned int) lookup);
    ofs = ofs + 0x18;
  }
  while (cnt < 2);
  cd = (Blk10__1 *) (kaze_state_base_ptr + 0x4842c);
  cs = (Blk10__1 *) (kaze_state_base_ptr + 0x48354);
  ce = cs + 13;
  do
  {
    *(cd++) = *(cs++);
  }
  while (cs != ce);
  *((Blk8 *) cd) = *((Blk8 *) cs);
  cd = (Blk10__1 *) (kaze_state_base_ptr + 0x485dc);
  cs = (Blk10__1 *) (kaze_state_base_ptr + 0x48504);
  ce = cs + 13;
  do
  {
    *(cd++) = *(cs++);
  }
  while (cs != ce);
  *((Blk8 *) cd) = *((Blk8 *) cs);
  cd = (Blk10__1 *) (kaze_state_base_ptr + 0x48324);
  cs = (Blk10__1 *) (kaze_state_base_ptr + 0x482f4);
  ce = cs + 3;
  do
  {
    *(cd++) = *(cs++);
  }
  while (cs != ce);
  cnt = 0;
  ofs = 0;
  do
  {
    int o;

    register int p __asm__("$3");
    int v;
    o = ofs + 0x48714;
    SetSprt(kaze_state_base_ptr + o);
    SetShadeTex(kaze_state_base_ptr + o, 1);
    p = (int) kaze_state_base_ptr;
    p = ofs + p;
    *((u16 *) (p + 0x48724)) = 0x80;
    *((u16 *) (p + 0x48726)) = 0x40;
    *((u8 *) (p + 0x48720)) = 0;
    v = ((cnt % 2) * 0x40) + (-0x80);
    {
      register int d __asm__("$6");
      d = (int) kaze_state_base_ptr;
      *((char *) ((ofs + d) + 0x48721)) = v;
    }
    lookup = GetClut(0x140, 0x1fa);
    cnt = cnt + 1;
    *((u16 *) ((((int) kaze_state_base_ptr) + ofs) + 0x48722)) = lookup;
    ofs = ofs + 0x14;
  }
  while (cnt < 4);
  lookup = GetTPage(1, 0, 0x240, 0x100);
  SetDrawTPage(kaze_state_base_ptr + 0x487b4, 0, 1, (unsigned int) lookup);
  lookup = GetTPage(1, 0, 0x240, 0x100);
  SetDrawTPage(kaze_state_base_ptr + 0x487bc, 0, 1, (unsigned int) lookup);
  cd = (Blk10__1 *) (kaze_state_base_ptr + 0x48764);
  cs = (Blk10__1 *) (kaze_state_base_ptr + 0x48714);
  ce = cs + 5;
  do
  {
    *(cd++) = *(cs++);
  }
  while (cs != ce);
  cnt = 0;
  do
  {
    lookup = GetClut(((cnt - ((cnt / 16) * 16)) * 0x10) + 0x140, (cnt / 16) + 0x180);
    {
      int _s = cnt * 2;
      *((u16 *) ((_s + ((int) kaze_state_base_ptr)) + 0x5b69c)) = lookup;
    }
    cnt = cnt + 1;
  }
  while (cnt < 0x20);
  lookup = GetTPage(0, 2, 0x140, 0);
  *((u16 *) (((int) kaze_state_base_ptr) + 0x5b698)) = lookup;
  lookup = GetTPage(0, 2, 0x180, 0);
  cnt = 0;
  *((u16 *) (((int) kaze_state_base_ptr) + 0x5b69a)) = lookup;
  lookup = GetClut(0x140, 0x1ff);
  *((u16 *) (((int) kaze_state_base_ptr) + 0x5b6dc)) = lookup;
  lookup = GetClut(0x140, 0x1ff);
  ofs = 0;
  *((u16 *) (((int) kaze_state_base_ptr) + 0x5b6de)) = lookup;
  do
  {
    int o;

    register int p __asm__("$5");

    register int p2 __asm__("$3");

    register int p3 __asm__("$3");

    register int tk __asm__("$3");
    register int clut_x __asm__("$4");
    o = 0x489b4;
    o = ofs + o;
    SetSprt(kaze_state_base_ptr + o);
    SetShadeTex(kaze_state_base_ptr + o, 1);
    SetSemiTrans(kaze_state_base_ptr + o, 0);
    clut_x = 0x200;
    p = (int) kaze_state_base_ptr;
    __asm__("" : "=r"(clut_x) : "0"(clut_x), "r"(p));
    p = ofs + p;
    *((u16 *) (p + 0x489bc)) = 0x60;
    *((u16 *) (p + 0x489be)) = 0x68;
    *((u8 *) (p + 0x489c0)) = 0;
    p3 = (int) kaze_state_base_ptr;
    p3 = ofs + p3;
    *((u8 *) (p3 + 0x489c1)) = 0xe0;
    p2 = (int) kaze_state_base_ptr;
    p2 = ofs + p2;
    *((u16 *) (p2 + 0x489c4)) = 0x80;
    *((u16 *) (p2 + 0x489c6)) = 0x20;
    lookup = GetClut(clut_x, 0x1f5);
    *((u16 *) ((((int) kaze_state_base_ptr) + ofs) + 0x489c2)) = lookup;
    lookup = GetTPage(0, 0, 0x380, 0x100);
    tk = 0x489dc;
    {
      register int t;
      t = cnt * 8;
      SetDrawTPage(kaze_state_base_ptr + (t + tk), 0, 1, (unsigned int) lookup);
    }
    ofs = ofs + 0x14;
    cnt = cnt + 1;
  }
  while (cnt < 2);
}

extern void SetTile(void *);

/**
 * kaze_post_event_sprite_init  (render, 0x80015660)
 *
 * Initializes slot 0 post-event sprite CLUT/TPage (+0x4a1a2/+0x4a1aa) and
 * overlay primitive fields (+0x4a288..).
 */

void kaze_post_event_sprite_init(void)
{
    extern int GetClut(int, int);
    extern unsigned long long SetPolyFT4(void *);
    extern char *kaze_state_base_ptr;
  int outer;
  int off1;
  int c7f;
  int acc1;
  int to1;
  int clut1;
  register int cval1 asm("$3");
  char *q1;
  char *ba;
  register char *bc asm("$5");
  int d8;
  int x2;
  int y2;
  int c2;
  char *m2;
  int off3;
  int acc3;
  int to3;
  register int cc3 asm("$2");
  unsigned char col3;
  char *p3;
  register char *q3 asm("$4");
  register char *r3 asm("$5");
  int rr3;
  int rr3b;
  char *bb;
  int off4;
  int off4b;
  int ff4;
  int r4;
  int r4c;
  outer = 0;
  c7f = 0x7F;
  acc1 = 0;
  off1 = 0;
  do
  {
    to1 = off1 + 0x4A194;
    SetPolyFT4(kaze_state_base_ptr + to1);
    SetSemiTrans(kaze_state_base_ptr + to1, 1);
    *((u8 *) ((off1 + ((int) kaze_state_base_ptr)) + 0x4A1A0)) = 0;
    *((u8 *) ((off1 + ((int) kaze_state_base_ptr)) + 0x4A1A1)) = 0;
    outer += 1;
    *((u8 *) ((off1 + ((int) kaze_state_base_ptr)) + 0x4A1A8)) = c7f;
    cval1 = (-0x80) - acc1;
    *((u8 *) ((off1 + ((int) kaze_state_base_ptr)) + 0x4A1A9)) = 0;
    *((u8 *) ((off1 + ((int) kaze_state_base_ptr)) + 0x4A1B0)) = 0;
    *((u8 *) ((off1 + ((int) kaze_state_base_ptr)) + 0x4A1B1)) = c7f;
    *((u8 *) ((off1 + ((int) kaze_state_base_ptr)) + 0x4A1B8)) = c7f;
    *((u8 *) ((off1 + ((int) kaze_state_base_ptr)) + 0x4A1B9)) = c7f;
    q1 = kaze_state_base_ptr + off1;
    *((u8 *) (q1 + 0x4A19A)) = cval1;
    *((u8 *) (q1 + 0x4A199)) = cval1;
    *((u8 *) (q1 + 0x4A198)) = cval1;
    *((u16 *) ((kaze_state_base_ptr + off1) + 0x4A1AA)) = GetTPage(1, 1, 0x180, 0x100);
    clut1 = GetClut(0x140, 0x1FD);
    acc1 += 0x30;
    ba = kaze_state_base_ptr;
    *((u16 *) ((ba + off1) + 0x4A1A2)) = clut1;
    off1 += 0x28;
  }
  while (outer < 3);
  *((struct blk78 *) (ba + 0x4A20C)) = *((struct blk78 *) (ba + 0x4A194));
  outer = 0;
  c2 = -0x3F16;
  m2 = kaze_state_base_ptr;
  do
  {
    d8 = outer / 8;
    x2 = ((outer - (d8 * 8)) << 13) - 0x7000;
    y2 = (d8 << 13) - 0x7000;
    asm volatile("sh %0,0x506d4(%1)" : : "r"(x2), "r"(m2));
    asm volatile("sh %0,0x506d8(%1)" : : "r"(y2), "r"(m2));
    asm volatile("sh %0,0x506d6(%1)" : : "r"(c2), "r"(m2));
    outer += 1;
    m2 += 8;
  }
  while (outer < 0x40);
  SetPolyFT4(kaze_state_base_ptr + 0x4A284);
  SetSemiTrans(kaze_state_base_ptr + 0x4A284, 1);
  *((u8 *) (((int) kaze_state_base_ptr) + 0x4A290)) = 0;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x4A291)) = 0x40;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x4A298)) = 0x1F;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x4A299)) = 0x40;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x4A2A0)) = 0;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x4A2A1)) = 0x5F;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x4A2A8)) = 0x1F;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x4A2A9)) = 0x5F;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x4A288)) = 0x80;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x4A289)) = 0x80;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x4A28A)) = 0x80;
  *((u16 *) (((int) kaze_state_base_ptr) + 0x4A29A)) = GetTPage(0, 1, 0x1C0, 0x140);
  *((u16 *) (((int) kaze_state_base_ptr) + 0x4A292)) = GetClut(0x140, 0x1FB);
  *((struct blk28 *) (kaze_state_base_ptr + 0x4A2AC)) = *((struct blk28 *) (kaze_state_base_ptr + 0x4A284));
  outer = 0;
  acc3 = 0;
  off3 = 0;
  do
  {
    to3 = off3 + 0x4A2D4;
    SetTile1(kaze_state_base_ptr + to3);
    SetSemiTrans(kaze_state_base_ptr + to3, 1);
    p3 = kaze_state_base_ptr + off3;
    q3 = kaze_state_base_ptr + off3;
    r3 = q3;
    q3 = kaze_state_base_ptr + off3;
    if (!(outer & 1))
    {
      cc3 = outer & 2;
      if (!cc3)
      {
        cc3 = 0xFF;
      }
      else
      {
        cc3 = 0x55;
      }
    }
    else
    {
      cc3 = 0x80;
    }
    *((u8 *) (p3 + 0x4A2DA)) = cc3;
    col3 = *((u8 *) (q3 + 0x4A2DA));
    *((u8 *) (q3 + 0x4A2DA)) = col3;
    *((u8 *) (r3 + 0x4A2D9)) = col3;
    *((u8 *) (p3 + 0x4A2D8)) = col3;
    rr3 = rand();
    *((short *) ((acc3 + ((int) kaze_state_base_ptr)) + 0x4FA54)) = rr3 % 0x1000;
    off3 += 0xC;
    outer += 1;
    rr3b = rand();
    bb = kaze_state_base_ptr;
    *((short *) ((acc3 + ((int) bb)) + 0x4FD74)) = (rr3b % 31768) + 0x3E8;
    acc3 += 2;
  }
  while (outer < 0x190);
  *((short *) (((int) bb) + 0x4FD74)) = 0;
  outer = 0;
  ff4 = 0xFF;
  do
  {
    off4 = outer * 0x10;
    SetLineF2(kaze_state_base_ptr + (off4 + 0x4C854));
    SetTile(kaze_state_base_ptr + (off4 + 0x4E154));
    *((u8 *) ((off4 + ((int) kaze_state_base_ptr)) + 0x4C858)) = ff4;
    *((u8 *) ((off4 + ((int) kaze_state_base_ptr)) + 0x4C859)) = ff4;
    *((u8 *) ((off4 + ((int) kaze_state_base_ptr)) + 0x4C85A)) = ff4;
    *((u8 *) ((off4 + ((int) kaze_state_base_ptr)) + 0x4E158)) = ff4;
    *((u8 *) ((off4 + ((int) kaze_state_base_ptr)) + 0x4E159)) = ff4;
    *((u8 *) ((off4 + ((int) kaze_state_base_ptr)) + 0x4E15A)) = ff4;
    off4b = outer * 8;
    r4 = rand();
    *((short *) ((off4b + ((int) kaze_state_base_ptr)) + 0x50094)) = (r4 % 12000) - 0x1770;
    outer += 1;
    *((short *) ((off4b + ((int) kaze_state_base_ptr)) + 0x50096)) = rand() - 0x7FFF;
    r4c = (rand() % 12000) - 0x1770;
    bc = kaze_state_base_ptr;
    *((short *) ((off4b + ((int) bc)) + 0x50098)) = r4c;
  }
  while (outer < 0xC8);
  *((struct blk12c0 *) (bc + 0x4B594)) = *((struct blk12c0 *) (bc + 0x4A2D4));
  *((struct blkc80 *) (kaze_state_base_ptr + 0x4D4D4)) = *((struct blkc80 *) (kaze_state_base_ptr + 0x4C854));
  *((struct blkc80 *) (kaze_state_base_ptr + 0x4EDD4)) = *((struct blkc80 *) (kaze_state_base_ptr + 0x4E154));
}

extern void SetLineF3(void *);

/**
 * kaze_post_event_menu_prim_init  (render, 0x80015eec)
 *
 * Post-event menu/confirmation/dialog SPRT, panel POLY_F4, icon/tile SPRT_8,
 * and marker primitive setup (+0x49194..+0x49724).
 */

void kaze_post_event_menu_prim_init(void)
{
    extern short GetClut();
    extern char *kaze_state_base_ptr;
    int outer;
    int tofs7;
    int tofs8;
    int dofs0;
    int ofs2;
    int half;
    char *p2;
    char *q2;
    char *w2;
    char *bsc;
    short clut2;
    int ofs3;
    char *q3;
    short clut3;
    int ofs4;
    char *p4;
    int dofs4;
    int i5;
    int cC0;
    int o3;
    int o2;
    int o1;
    int semi;
    char *pc0;
    char *pb0;
    char *pa0;
    char *pc1;
    char *pb1;
    char *pa1;
    char *pc2;
    char *pb2;
    char *pa2;
    int ofs6;
    char *qc0;
    char *qb0;
    char *qa0;
    char *qc1;
    char *qb1;
    char *qa1;
    char *qc2;
    char *qb2;
    char *qa2;
    char *q7;
    short clut7;
    char *q8;
    short clut8;
    int oa8;
    int ob8;
    int ofs9;
    int acc9;
    int c20;
    char *q9a;
    char *q9b;
    char *q9c;

    outer = 0;
    do {
        dofs0 = outer * 8;
        SetDrawTPage(kaze_state_base_ptr + (dofs0 + 0x48E1C), 0, 1, GetTPage(0, 2, 0x200, 0x100) & 0xFFFF);
        outer++;
    } while (outer < 2);

    outer = 0;
    do {
        ofs2 = outer * 0x14;
        SetSprt(kaze_state_base_ptr + (ofs2 + 0x4918C));
        {
        register int c160 asm("$4") = 0x160;
        half = outer / 2;
        p2 = (char *)(ofs2 + (int)kaze_state_base_ptr);
        *(short *)(p2 + 0x4919C) = 0x80;
        *(u8 *)(p2 + 0x49198) = (outer % 2) << 7;
        {
        register char *r199 asm("$5") = kaze_state_base_ptr;
        *(short *)(p2 + 0x4919E) = 0x10;
        *(u8 *)(ofs2 + (int)r199 + 0x49199) = half * 0x10 + 0x40;
        }
        *(short *)((int)kaze_state_base_ptr + ofs2 + 0x49194) = 0x60;
        clut2 = GetClut(c160, 0x1F9, p2);
        q2 = (char *)((int)kaze_state_base_ptr + ofs2);
        *(short *)(q2 + 0x4919A) = clut2;
        w2 = (char *)(outer * 4 + (int)kaze_state_base_ptr);
        *(int *)(w2 + 0x492A4) = 0x40;
        }
        outer++;
    } while (outer < 3);

    *(short *)((int)kaze_state_base_ptr + 0x49196) = 0x5C;
    *(short *)((int)kaze_state_base_ptr + 0x491AA) = 0x6D;
    *(short *)((int)kaze_state_base_ptr + 0x491BE) = 0x86;
    *(Blk3C *)(kaze_state_base_ptr + 0x491C8) = *(Blk3C *)(kaze_state_base_ptr + 0x4918C);

    outer = 0;
    do {
        ofs3 = outer * 0x14;
        SetSprt(kaze_state_base_ptr + (ofs3 + 0x49204));
        clut3 = GetClut(0x160, 0x1F9);
        {
            register char *dp asm("$6") = kaze_state_base_ptr;
            q3 = (char *)((int)dp + ofs3);
            *(short *)(q3 + 0x49212) = clut3;
        }
        outer++;
    } while (outer < 4);

    {
    register int c80 asm("$4") = 0x80;
    register short c40 asm("$3") = 0x40;
    register char *a2b asm("$6") = kaze_state_base_ptr;
    *(short *)(a2b + 0x49228) = c40;
    *(short *)(a2b + 0x4923C) = c40;
    *(u8 *)(a2b + 0x49210) = 0x80;
    {
    register char *r211 asm("$3") = kaze_state_base_ptr;
    *(short *)(a2b + 0x49216) = 0x10;
    *(short *)(a2b + 0x4922A) = 0x10;
    *(short *)(a2b + 0x4923E) = 0x10;
    *(short *)(a2b + 0x49252) = 0x10;
    *(short *)(a2b + 0x49214) = c80;
    *(short *)(a2b + 0x49250) = c80;
    *(u8 *)((int)r211 + 0x49211) = 0x50;
    }
    *(u8 *)((int)kaze_state_base_ptr + 0x49224) = 0;
    *(u8 *)((int)kaze_state_base_ptr + 0x49225) = 0x60;
    *(u8 *)((int)kaze_state_base_ptr + 0x49238) = 0x40;
    *(u8 *)((int)kaze_state_base_ptr + 0x49239) = 0x60;
    *(u8 *)((int)kaze_state_base_ptr + 0x4924C) = 0x80;
    *(u8 *)((int)kaze_state_base_ptr + 0x4924D) = 0x60;
    }
    SetShadeTex(kaze_state_base_ptr + 0x49204, 1);
    SetShadeTex(kaze_state_base_ptr + 0x49240, 1);
    *(int *)((int)kaze_state_base_ptr + 0x492B4) = 0x40;
    *(int *)((int)kaze_state_base_ptr + 0x492B0) = 0x40;
    *(Blk50 *)(kaze_state_base_ptr + 0x49254) = *(Blk50 *)(kaze_state_base_ptr + 0x49204);

    outer = 0;
    *(int *)((int)kaze_state_base_ptr + 0x48E0C) = 0;
    do {
        ofs4 = outer * 0x14;
        SetSprt(kaze_state_base_ptr + (ofs4 + 0x492C4));
        p4 = (char *)(ofs4 + (int)kaze_state_base_ptr);
        *(u8 *)(p4 + 0x492D0) = 0;
        {
        register char *rd1 asm("$3") = kaze_state_base_ptr;
        *(short *)(p4 + 0x492D4) = 0x80;
        *(short *)(p4 + 0x492D6) = 0x80;
        *(u8 *)(ofs4 + (int)rd1 + 0x492D1) = 0;
        }
        *(u8 *)(ofs4 + (int)kaze_state_base_ptr + 0x492C8) = 0x80;
        *(u8 *)(ofs4 + (int)kaze_state_base_ptr + 0x492C9) = 0x80;
        *(u8 *)(ofs4 + (int)kaze_state_base_ptr + 0x492CA) = 0x80;
        *(short *)((int)kaze_state_base_ptr + ofs4 + 0x492D2) = GetClut(0x140, 0x1FE);
        SetDrawTPage(kaze_state_base_ptr + (outer * 8 + 0x492EC), 0, 1, GetTPage(1, 2, 0x140, 0x100) & 0xFFFF);
        outer++;
    } while (outer < 2);

    cC0 = 0xC0;
    i5 = 0;
    do {
        o3 = i5 + 0x48E2C;
        SetPolyF4(kaze_state_base_ptr + o3);
        o2 = i5 + 0x48FDC;
        SetPolyF4(kaze_state_base_ptr + o2);
        o1 = i5 + 0x492FC;
        SetPolyF4(kaze_state_base_ptr + o1);
        semi = (u32)i5 < 0x18;
        SetSemiTrans(kaze_state_base_ptr + o3, semi);
        SetSemiTrans(kaze_state_base_ptr + o2, semi);
        SetSemiTrans(kaze_state_base_ptr + o1, semi);

        pc0 = (char *)((int)kaze_state_base_ptr + i5);
        pb0 = pc0;
        pa0 = pc0;
        if (i5 != 0)
            *(u8 *)(pc0 + 0x49300) = 8;
        else
            *(u8 *)((int)kaze_state_base_ptr + 0x49300) = cC0;
        *(u8 *)(pc0 + 0x48E30) = *(u8 *)(pb0 + 0x48FE0) = *(u8 *)(pa0 + 0x49300) = *(u8 *)(pa0 + 0x49300);

        pc1 = (char *)((int)kaze_state_base_ptr + i5);
        pb1 = pc1;
        pa1 = pc1;
        if (i5 != 0)
            *(u8 *)(pc1 + 0x49301) = 0x7B;
        else
            *(u8 *)((int)kaze_state_base_ptr + 0x49301) = cC0;
        *(u8 *)(pc1 + 0x48E31) = *(u8 *)(pb1 + 0x48FE1) = *(u8 *)(pa1 + 0x49301) = *(u8 *)(pa1 + 0x49301);

        pc2 = (char *)((int)kaze_state_base_ptr + i5);
        pb2 = pc2;
        pa2 = pc2;
        if (i5 != 0)
            *(u8 *)(pc2 + 0x49302) = 0x42;
        else
            *(u8 *)((int)kaze_state_base_ptr + 0x49302) = cC0;
        *(u8 *)(pc2 + 0x48E32) = *(u8 *)(pb2 + 0x48FE2) = *(u8 *)(pa2 + 0x49302) = *(u8 *)(pa2 + 0x49302);

        i5 += 0x18;
    } while (i5 < 0x78);

    outer = 0;
    do {
        ofs6 = outer * 0x18;
        SetLineF3(kaze_state_base_ptr + (ofs6 + 0x48F1C));
        SetLineF3(kaze_state_base_ptr + (ofs6 + 0x490CC));
        SetLineF3(kaze_state_base_ptr + (ofs6 + 0x493EC));

        qc0 = (char *)((int)kaze_state_base_ptr + ofs6);
        qb0 = qc0;
        qa0 = qc0;
        if (!(outer & 1))
            *(u8 *)(qc0 + 0x493F0) = 0x19;
        else
            *(u8 *)(qc0 + 0x493F0) = 0;
        *(u8 *)(qc0 + 0x48F20) = *(u8 *)(qb0 + 0x490D0) = *(u8 *)(qa0 + 0x493F0) = *(u8 *)(qa0 + 0x493F0);

        qc1 = (char *)((int)kaze_state_base_ptr + ofs6);
        qb1 = qc1;
        qa1 = qc1;
        if (outer & 1)
            *(u8 *)(qc1 + 0x493F1) = 0x4A;
        else
            *(u8 *)(qc1 + 0x493F1) = 0xAD;
        *(u8 *)(qc1 + 0x48F21) = *(u8 *)(qb1 + 0x490D1) = *(u8 *)(qa1 + 0x493F1) = *(u8 *)(qa1 + 0x493F1);

        qc2 = (char *)((int)kaze_state_base_ptr + ofs6);
        qb2 = qc2;
        qa2 = qc2;
        if (outer & 1)
            *(u8 *)(qc2 + 0x493F2) = 0x29;
        else
            *(u8 *)(qc2 + 0x493F2) = 8;
        *(u8 *)(qc2 + 0x48F22) = *(u8 *)(qb2 + 0x490D2) = *(u8 *)(qa2 + 0x493F2) = *(u8 *)(qa2 + 0x493F2);

        outer++;
    } while (outer < 4);

    *(Blk78 *)(kaze_state_base_ptr + 0x48EA4) = *(Blk78 *)(kaze_state_base_ptr + 0x48E2C);
    *(Blk60 *)(kaze_state_base_ptr + 0x48F7C) = *(Blk60 *)(kaze_state_base_ptr + 0x48F1C);
    *(Blk78 *)(kaze_state_base_ptr + 0x49054) = *(Blk78 *)(kaze_state_base_ptr + 0x48FDC);
    *(Blk60 *)(kaze_state_base_ptr + 0x4912C) = *(Blk60 *)(kaze_state_base_ptr + 0x490CC);
    *(Blk78 *)(kaze_state_base_ptr + 0x49374) = *(Blk78 *)(kaze_state_base_ptr + 0x492FC);
    *(Blk60 *)(kaze_state_base_ptr + 0x4944C) = *(Blk60 *)(kaze_state_base_ptr + 0x493EC);

    outer = 0;
    do {
        tofs7 = outer * 0x10;
        SetSprt8(kaze_state_base_ptr + (tofs7 + 0x494AC));
        SetSemiTrans(kaze_state_base_ptr + (tofs7 + 0x494AC), 1);
        clut7 = GetClut(0x140, 0x1F9);
        q7 = (char *)((int)kaze_state_base_ptr + tofs7);
        *(short *)(q7 + 0x494BA) = clut7;
        outer++;
    } while (outer < 0x11);

    SetDrawTPage(kaze_state_base_ptr + 0x49750, 0, 1, GetTPage(0, 1, 0x200, 0x100) & 0xFFFF);
    SetDrawTPage(kaze_state_base_ptr + 0x49758, 0, 1, GetTPage(0, 1, 0x200, 0x100) & 0xFFFF);

    *(Blk110 *)(kaze_state_base_ptr + 0x495BC) = *(Blk110 *)(kaze_state_base_ptr + 0x494AC);

    outer = 0;
    ob8 = 0;
    oa8 = 0;
    do {
        SetPolyF3(kaze_state_base_ptr + (oa8 + 0x49724));
        SetSemiTrans(kaze_state_base_ptr + (oa8 + 0x49724), 1);
        tofs8 = outer * 0x10;
        *(u8 *)(oa8 + (int)kaze_state_base_ptr + 0x49728) = 0;
        *(u8 *)(oa8 + (int)kaze_state_base_ptr + 0x49729) = 0;
        *(u8 *)(oa8 + (int)kaze_state_base_ptr + 0x4972A) = 0;
        SetLineF4(kaze_state_base_ptr + (ob8 + 0x496EC));
        SetSemiTrans(kaze_state_base_ptr + (ob8 + 0x496EC), 1);
        *(u8 *)(ob8 + (int)kaze_state_base_ptr + 0x496F0) = 0;
        *(u8 *)(ob8 + (int)kaze_state_base_ptr + 0x496F1) = 0;
        *(u8 *)(ob8 + (int)kaze_state_base_ptr + 0x496F2) = 0;
        SetSprt8(kaze_state_base_ptr + (tofs8 + 0x496CC));
        SetSemiTrans(kaze_state_base_ptr + (tofs8 + 0x496CC), 1);
        *(u8 *)(tofs8 + (int)kaze_state_base_ptr + 0x496D8) = 0;
        *(u8 *)(tofs8 + (int)kaze_state_base_ptr + 0x496D9) = 0x70;
        clut8 = GetClut(0x140, 0x1F9);
        q8 = (char *)((int)kaze_state_base_ptr + tofs8);
        *(short *)(q8 + 0x496DA) = clut8;
        oa8 += 0x14;
        ob8 += 0x1C;
        outer++;
    } while (outer < 2);

    outer = 0;
    c20 = 0x20;
    acc9 = 0x49760;
    ofs9 = 0;
    do {
        SetPolyF4(kaze_state_base_ptr + acc9);
        SetSemiTrans(kaze_state_base_ptr + acc9, 1);
        q9a = (char *)(ofs9 + (int)kaze_state_base_ptr);
        *(u8 *)(q9a + 0x49764) = c20;
        outer++;
        q9b = (char *)(ofs9 + (int)kaze_state_base_ptr);
        *(u8 *)(q9b + 0x49765) = c20;
        acc9 += 0x18;
        q9c = (char *)(ofs9 + (int)kaze_state_base_ptr);
        *(u8 *)(q9c + 0x49766) = c20;
        ofs9 += 0x18;
    } while (outer < 3);

    *(Blk48 *)(kaze_state_base_ptr + 0x497A8) = *(Blk48 *)(kaze_state_base_ptr + 0x49760);

    SetDrawTPage(kaze_state_base_ptr + 0x497F0, 0, 1, GetTPage(0, 2, 0x200, 0x100) & 0xFFFF);
    SetDrawTPage(kaze_state_base_ptr + 0x497F8, 0, 1, GetTPage(0, 2, 0x200, 0x100) & 0xFFFF);
}

extern void SetSprt16(void *);

/**
 * kaze_post_event_frame_prim_init  (render, 0x80016e54)
 *
 * Post-event SPRT_16/SPRT/POLY_FT4/SPRT_8/POLY_F4 frame and footer primitive
 * setup (+0x489ec..+0x48d84), including the HUD gauge POLY_F4 slots later
 * driven by the HUD renderer.
 */

void kaze_post_event_frame_prim_init(void)
{
    extern short GetClut();
    extern void SetPolyFT4(void *);
    extern char *kaze_state_base_ptr;
  short rect[4];
  int outer;
  int w1;
  int ofs1;
  char *p1;
  char *q1;
  int v3;
  int ofs3;
  int t3;
  int c12;
  char *p3;
  char *r3b;
  int v4;
  int ofs4;
  int m4;
  int of4;
  char *new_var;
  int c18;
  int c20;
  int c2e;
  int c36;
  char *sp8;
  short cl4;
  int a4;
  int v5;
  int f5;
  int ofs5;
  int m5;
  char *p5;
  char *r5;
  char *q5;
  char *a05;
  short cl5;
  int w6;
  int m6;
  int ofs6;
  char *p6;
  register Blk10__1 *cd1 asm("$6");
  register Blk10__1 *cs1 asm("$16");
  register Blk10__1 *ce1 asm("$7");
  Blk10__1 *cd2;
  Blk10__1 *cs2;
  Blk10__1 *ce2;
  Blk10__1 *dst3;
  Blk10__1 *src3;
  Blk10__1 *end3;
  {
    register char *r3 asm("$3");
    register int c48a3c asm("$2");
    outer = 0;
    w1 = 0x50;
    do
    {
      ofs1 = outer * 0x10;
      SetSprt16(kaze_state_base_ptr + (ofs1 + 0x489EC));
      p1 = (char *) (ofs1 + ((int) kaze_state_base_ptr));
      *((u8 *) (p1 + 0x489F8)) = 0;
      q1 = (char *) (ofs1 + ((int) kaze_state_base_ptr));
      *((short *) (p1 + 0x489F4)) = 0x118;
      *((short *) (p1 + 0x489F6)) = w1;
      *((u8 *) (q1 + 0x489F9)) = 0;
      {
        short cl = GetClut(0x140, 0x1F9, q1);
        r3 = kaze_state_base_ptr;
        new_var = r3;
        *((short *) ((new_var + ofs1) + 0x489FA)) = cl;
      }
      outer++;
      w1 += 0x12;
    }
    while (outer < 5);
    c48a3c = 0x48A3C;
    cd1 = (Blk10__1 *) (new_var + c48a3c);
    cs1 = (Blk10__1 *) (new_var + 0x489EC);
    ce1 = cs1 + 5;
    do
    {
      *(cd1++) = *(cs1++);
    }
    while (cs1 != ce1);
  }
  {
    register char *r4 asm("$4");
    register int half3 asm("$7");
    register int clx3 asm("$4");
    outer = 0;
    c12 = 0xC;
    v3 = 0;
    do
    {
      ofs3 = v3 + 0x48A8C;
      SetSprt(kaze_state_base_ptr + ofs3);
      SetShadeTex(kaze_state_base_ptr + ofs3, 1);
      SetSemiTrans(kaze_state_base_ptr + ofs3, 1);
      {
        int clx;
        __asm__("addiu %0,$0,336" : "=r"(clx));
        clx3 = clx;
      }
      half3 = outer / 2;
      __asm__("" :: "r"(half3));
      p3 = (char *) (v3 + ((int) kaze_state_base_ptr));
      *((u8 *) (p3 + 0x48A98)) = (outer % 2) * 0xC;
      r3b = (char *) (v3 + ((int) kaze_state_base_ptr));
      t3 = (half3 * 0xC) + 0x20;
      *((short *) (p3 + 0x48A9C)) = c12;
      *((short *) (p3 + 0x48A9E)) = c12;
      *((u8 *) (r3b + 0x48A99)) = t3;
      outer++;
      {
        short cl = GetClut(clx3, 0x1F9);
        r4 = kaze_state_base_ptr;
        *((short *) ((r4 + v3) + 0x48A9A)) = cl;
      }
      v3 += 0x14;
    }
    while (outer < 4);
    cd2 = (Blk10__1 *) (r4 + 0x48ADC);
    cs2 = (Blk10__1 *) (r4 + 0x48A8C);
    ce2 = cs2 + 5;
    do
    {
      *(cd2++) = *(cs2++);
    }
    while (cs2 != ce2);
  }
  outer = 0;
  c18 = 0x18;
  c20 = 0x20;
  c2e = 0x2E;
  c36 = 0x36;
  v4 = 0;
  do
  {
    ofs4 = v4 + 0x48BBC;
    SetPolyFT4(kaze_state_base_ptr + ofs4);
    SetShadeTex(kaze_state_base_ptr + ofs4, 1);
    SetSemiTrans(kaze_state_base_ptr + ofs4, 1);
    *((u8 *) ((v4 + ((int) kaze_state_base_ptr)) + 0x48BC8)) = c18;
    *((u8 *) ((v4 + ((int) kaze_state_base_ptr)) + 0x48BC9)) = c20;
    m4 = outer * 0x10;
    *((u8 *) ((v4 + ((int) kaze_state_base_ptr)) + 0x48BD0)) = c2e;
    *((u8 *) ((v4 + ((int) kaze_state_base_ptr)) + 0x48BD1)) = c20;
    *((u8 *) ((v4 + ((int) kaze_state_base_ptr)) + 0x48BD8)) = c18;
    *((u8 *) ((v4 + ((int) kaze_state_base_ptr)) + 0x48BD9)) = c36;
    *((u8 *) ((v4 + ((int) kaze_state_base_ptr)) + 0x48BE0)) = c2e;
    *((u8 *) ((v4 + ((int) kaze_state_base_ptr)) + 0x48BE1)) = c36;
    *((short *) ((((int) kaze_state_base_ptr) + v4) + 0x48BD2)) = GetTPage(0, 2, 0x200, 0x100);
    cl4 = GetClut(0x150, 0x1F9);
    of4 = 0x48B2C + m4;
    sp8 = (char *) (((int) kaze_state_base_ptr) + of4);
    *((short *) ((((int) kaze_state_base_ptr) + v4) + 0x48BCA)) = cl4;
    v4 += 0x28;
    SetSprt8(sp8);
    SetShadeTex(kaze_state_base_ptr + of4, 1);
    *((u8 *) ((m4 + ((int) kaze_state_base_ptr)) + 0x48B38)) = 0x40;
    *((u8 *) ((m4 + ((int) kaze_state_base_ptr)) + 0x48B39)) = 0x28;
    *((short *) ((((int) kaze_state_base_ptr) + m4) + 0x48B3A)) = GetClut(0x140, 0x1F9);
    outer++;
  }
  while (outer < 2);
  outer = 0;
  a4 = 0x48C44;
  do
  {
    outer++;
    SetPolyF4(kaze_state_base_ptr + a4);
    a4 += 0x18;
  }
  while (outer < 6);
  rect[0] = 0x160;
  rect[1] = 0x1F7;
  rect[2] = 0x10;
  rect[3] = 1;
  outer = 0;
  f5 = 0;
  v5 = 0;
  do
  {
    ofs5 = v5 + 0x48C1C;
    m5 = outer * 8;
    SetSprt(kaze_state_base_ptr + ofs5);
    SetShadeTex(kaze_state_base_ptr + ofs5, 1);
    SetSemiTrans(kaze_state_base_ptr + ofs5, 1);
    p5 = (char *) (v5 + ((int) kaze_state_base_ptr));
    *((u8 *) (p5 + 0x48C28)) = 0xC0;
    r5 = (char *) (v5 + ((int) kaze_state_base_ptr));
    *((short *) (p5 + 0x48C2C)) = 0x40;
    *((short *) (p5 + 0x48C2E)) = 0x40;
    *((u8 *) (r5 + 0x48C29)) = 0;
    q5 = (char *) (v5 + ((int) kaze_state_base_ptr));
    *((short *) (q5 + 0x48C24)) = 0x80;
    *((short *) (q5 + 0x48C26)) = 0x58;
    SetDrawTPage(kaze_state_base_ptr + (m5 + 0x48D74), 0, 1, GetTPage(0, 2, 0x140, 0) & 0xFFFF);
    SetDrawTPage(kaze_state_base_ptr + (m5 + 0x48D64), 0, 1, GetTPage(0, 0, 0x200, 0x100) & 0xFFFF);
    cl5 = GetClut(0x1B0, 0x1F9);
    a05 = kaze_state_base_ptr + (f5 + 0x48D84);
    *((short *) ((((int) kaze_state_base_ptr) + v5) + 0x48C2A)) = cl5;
    SetDrawLoad(a05, rect);
    f5 += 0x44;
    outer++;
    v5 += 0x14;
  }
  while (outer < 2);
  SetSemiTrans(kaze_state_base_ptr + 0x48C5C, 1);
  SetSemiTrans(kaze_state_base_ptr + 0x48CBC, 1);
  *((u8 *) (((int) kaze_state_base_ptr) + 0x48C60)) = 0x80;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x48C61)) = 0x80;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x48C62)) = 0x80;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x48CC0)) = 0x80;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x48CC1)) = 0x80;
  *((u8 *) (((int) kaze_state_base_ptr) + 0x48CC2)) = 0x80;
  dst3 = (Blk10__1 *) (((int) kaze_state_base_ptr) + 0x48CD4);
  src3 = (Blk10__1 *) (((int) kaze_state_base_ptr) + 0x48C44);
  end3 = src3 + 9;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C4C)) = 0x5C;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C4E)) = 0xD1;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C52)) = 0xD1;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C54)) = 0x5C;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C50)) = 0x124;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C56)) = 0xD5;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C5A)) = 0xD5;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C66)) = 0xD0;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C6A)) = 0xD0;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C64)) = 0x5C;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C6C)) = 0x5C;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C6E)) = 0xD6;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C72)) = 0xD6;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C58)) = 0x124;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C68)) = 0x124;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C70)) = 0x124;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C7C)) = 0xA4;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C7E)) = 0xC6;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C82)) = 0xC6;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C80)) = 0x124;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C84)) = 0xA4;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C96)) = 0xC8;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C9A)) = 0xC8;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C9E)) = 0xCC;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CA2)) = 0xCC;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CB6)) = 0xCE;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CBA)) = 0xCE;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C86)) = 0xCA;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CC6)) = 0xC7;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CCA)) = 0xC7;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C88)) = 0x124;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C8A)) = 0xCA;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C94)) = 0xA4;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C98)) = 0x124;
  *((short *) (((int) kaze_state_base_ptr) + 0x48C9C)) = 0xA4;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CA0)) = 0x124;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CAC)) = 0xA4;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CAE)) = 0xCA;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CB0)) = 0x124;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CB2)) = 0xCA;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CB4)) = 0xA4;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CB8)) = 0x124;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CC4)) = 0xA4;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CC8)) = 0x124;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CCC)) = 0xA4;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CCE)) = 0xCF;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CD0)) = 0x124;
  *((short *) (((int) kaze_state_base_ptr) + 0x48CD2)) = 0xCF;
  do
  {
    *(dst3++) = *(src3++);
  }
  while (src3 != end3);
  SetDrawTPage(kaze_state_base_ptr + 0x48C0C, 0, 1, GetTPage(0, 2, 0x200, 0x100) & 0xFFFF);
  outer = 0;
  w6 = 0x94;
  SetDrawTPage(kaze_state_base_ptr + 0x48C14, 0, 1, GetTPage(0, 2, 0x200, 0x100) & 0xFFFF);
  do
  {
    m6 = outer * 0x10;
    ofs6 = m6 + 0x48B4C;
    SetSprt8(kaze_state_base_ptr + ofs6);
    SetShadeTex(kaze_state_base_ptr + ofs6, 1);
    outer++;
    *((u8 *) ((m6 + ((int) kaze_state_base_ptr)) + 0x48B58)) = 0x48;
    *((u8 *) ((m6 + ((int) kaze_state_base_ptr)) + 0x48B59)) = 0x28;
    p6 = (char *) (m6 + ((int) kaze_state_base_ptr));
    *((short *) (p6 + 0x48B54)) = w6;
    *((short *) (p6 + 0x48B56)) = 0x58;
    *((short *) ((((int) kaze_state_base_ptr) + m6) + 0x48B5A)) = GetClut(0x140, 0x1F9);
    w6 += 8;
  }
  while (outer < 3);
  SetDrawTPage(kaze_state_base_ptr + 0x48BAC, 0, 1, GetTPage(0, 0, 0x200, 0x100) & 0xFFFF);
  SetDrawTPage(kaze_state_base_ptr + 0x48BB4, 0, 1, GetTPage(0, 0, 0x200, 0x100) & 0xFFFF);
  *((Blk30 *) (kaze_state_base_ptr + 0x48B7C)) = *((Blk30 *) (kaze_state_base_ptr + 0x48B4C));
}

/* MATCH under gcc 2.7.2-psx -O2 (648B, byte-exact).
 * Closes the 2/162 near-miss. Two levers, both discovered via cc1 -dS/-dg dumps:
 *  1) The reused constant 5 must emit (li $a1,5) BEFORE the reused 0x1a (li $a0,0x1a).
 *     Both are priority-1 scheduler leaves; final order = LUID = CSE chain order.
 *     A $4 register-asm on 0x1a (the old approach) pins its reload to absolute
 *     block-start (right after the LoadImage call, before base is even loaded),
 *     which forces it ahead of the 0x56/86 store -> wrong order and immovable.
 *     Instead: pin the reused 5 into $a1 (register short t5 asm("$5")) and
 *     assign it early (t5=5 right after base=), giving 5 the lowest LUID so it
 *     emits first; 0x1a stays a plain literal, CSE'd at its first use (late LUID)
 *     and allocated to $a0 naturally (5 already owns $a1).
 *  2) Dropping the $4 pin perturbs reload's frame-elim ordering of the two
 *     LoadImage arg addiu (a1=buf emitted before a0=&rect). Fix: pin the two
 *     args to $4/$5 in source order (register RECT__1 *pr asm("$4"); pb asm("$5"))
 *     so the arg-setup addiu order is fixed to a0-before-a1. */
extern RECT__1 kaze_env_lookup_vram_rect;
extern void LoadImage(RECT__1 *, unsigned char *);

/**
 * kaze_environment_lookup_init  (render, 0x80017a54)
 *
 * Initializes the environment color lookup value at +0x482b0 consumed by
 * kaze_environment_color_update.
 */

void kaze_environment_lookup_init(void)
{
    extern int kaze_state_base_ptr;
    unsigned short buf[256];
    RECT__1 rect;
    int i;
    int base;
    register short t5 asm("$5");

    rect = kaze_env_lookup_vram_rect;
    for (i = 0; i < 256; i++) {
        buf[i] = 0xffff;
    }
    {
        register RECT__1 *pr asm("$4") = &rect;
        register unsigned char *pb asm("$5") = (unsigned char *)buf;
        LoadImage(pr, pb);
    }

    base = kaze_state_base_ptr;
    t5 = 5;
    *(short *)(base + 0x482b0) = 0x56;
    *(short *)(base + 0x482b2) = 0x11;
    *(short *)(base + 0x482b4) = 0x35;
    *(short *)(base + 0x482b6) = 0xd0;
    *(short *)(base + 0x482b8) = 0xa3;
    *(short *)(base + 0x482ba) = 0x4d;
    *(short *)(base + 0x482bc) = 4;
    *(short *)(base + 0x482c4) = 0x28;
    *(short *)(base + 0x482c6) = 0x2d;
    *(short *)(base + 0x482c8) = 0x42;
    *(short *)(base + 0x482ca) = 0x5d;
    *(short *)(base + 0x482cc) = 100;
    *(short *)(base + 0x482ce) = 0x84;
    *(short *)(base + 0x482d2) = 0x50;
    *(short *)(base + 0x482d4) = 0x70;
    *(short *)(base + 0x482dc) = 0x14;
    *(short *)(base + 0x482de) = 0x19;
    *(short *)(base + 0x482e0) = 0x2e;
    *(short *)(base + 0x482e2) = 0x36;
    *(short *)(base + 0x482e4) = 0x3b;
    *(short *)(base + 0x482e6) = 0x51;
    *(short *)(base + 0x482e8) = 0x3e;
    *(short *)(base + 0x482ac) = 0;
    *(short *)(base + 0x482ae) = t5;
    *(short *)(base + 0x482be) = 0;
    *(short *)(base + 0x482c0) = t5;
    *(short *)(base + 0x482c2) = 0x1a;
    *(short *)(base + 0x482d0) = 0x49;
    *(short *)(base + 0x482d6) = 0;
    *(short *)(base + 0x482d8) = t5;
    *(short *)(base + 0x482da) = 0x1a;
    *(short *)(base + 0x482ea) = 0x40;
    *(short *)(base + 0x482ec) = 0x49;
    *(short *)(base + 0x482ee) = 0;
    *(short *)(base + 0x482f0) = t5;
    *(short *)(base + 0x482f2) = 0x1a;
}

/**
 * kaze_post_event_split_prim_init  (render, 0x80017cdc)
 *
 * Split post-event POLY_FT4/POLY_F4/DR_TPAGE setup (+0x487c4..+0x488c4).
 */

void kaze_post_event_split_prim_init(void)
{
    extern void SetPolyFT4(void *);
    extern char *kaze_state_base_ptr;
    int outer;
    int ofs;
    int tofs;
    int pofs;
    int page;
    int ofs2;
    int dofs;
    int tp;
    char *p;
    char *q;
    char *r;
    int ofs_b;
    char *p_b;
    char *q_b;
    char *r_b;

    outer = 0;
    do {
        ofs = outer * 0x50;
        tofs = outer * 0x28;
        pofs = ofs + 0x487C4;
        SetPolyFT4(kaze_state_base_ptr + pofs);
        SetPolyFT4(kaze_state_base_ptr + pofs + 0x28);
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487C8) = 0x70;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487C9) = 0x70;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487CA) = 0x70;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487F0) = 0x70;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487F1) = 0x70;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487F2) = 0x70;
        p = (char *)(ofs + (int)kaze_state_base_ptr);
        *(u8 *)(p + 0x487D0) = 0;
        q = (char *)(ofs + (int)kaze_state_base_ptr);
        {
            register short c134 asm("$4") = 0x134;
            *(short *)(p + 0x487CC) = 0xC;
            *(short *)(p + 0x487CE) = 8;
            *(short *)(p + 0x487D4) = 0xA0;
            *(short *)(p + 0x487D6) = 8;
            *(short *)(p + 0x487DC) = 0xC;
            *(short *)(p + 0x487DE) = 0xE8;
            *(short *)(p + 0x487E4) = 0xA0;
            *(short *)(p + 0x487E6) = 0xE8;
            *(short *)(p + 0x487F4) = 0xA0;
            *(short *)(p + 0x487F6) = 8;
            *(short *)(p + 0x487FC) = c134;
            *(short *)(p + 0x487FE) = 8;
            *(short *)(p + 0x48804) = 0xA0;
            *(short *)(p + 0x48806) = 0xE8;
            *(short *)(p + 0x4880C) = c134;
            *(short *)(p + 0x4880E) = 0xE8;
        }
        *(u8 *)(q + 0x487D1) = 0;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487D8) = 0xA0;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487D9) = 0;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487E0) = 0;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487E1) = 0xEF;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487E8) = 0xA0;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487E9) = 0xEF;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487F8) = 0x20;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x487F9) = 0;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x48800) = 0xBF;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x48801) = 0;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x48808) = 0x20;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x48809) = 0xEF;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x48810) = 0xBF;
        *(u8 *)(ofs + (int)kaze_state_base_ptr + 0x48811) = 0xEF;
        page = (outer & 1) << 7;
        *(u16 *)((int)kaze_state_base_ptr + tofs + 0x487DA) = GetTPage(2, 0, page, 0x100);
        *(u16 *)((int)kaze_state_base_ptr + tofs + 0x4882A) = GetTPage(2, 0, page, 0);
        outer++;
    } while (outer < 2);

    outer = 0;
    do {
        ofs_b = outer * 0x18;
        SetPolyF4(kaze_state_base_ptr + (ofs_b + 0x48864));
        *(u8 *)(ofs_b + (int)kaze_state_base_ptr + 0x48868) = 0;
        *(u8 *)(ofs_b + (int)kaze_state_base_ptr + 0x48869) = 0;
        *(u8 *)(ofs_b + (int)kaze_state_base_ptr + 0x4886A) = 0;
        ofs2 = ofs_b + 0x48894;
        q_b = kaze_state_base_ptr + ofs2;
        p_b = (char *)(ofs_b + (int)kaze_state_base_ptr);
        *(short *)(p_b + 0x4886C) = 0;
        *(short *)(p_b + 0x4886E) = 0;
        *(short *)(p_b + 0x48870) = 0x140;
        *(short *)(p_b + 0x48872) = 0;
        *(short *)(p_b + 0x48874) = 0;
        *(short *)(p_b + 0x48876) = 0xF0;
        *(short *)(p_b + 0x48878) = 0x140;
        *(short *)(p_b + 0x4887A) = 0xF0;
        SetPolyF4(q_b);
        SetSemiTrans(kaze_state_base_ptr + ofs2, 1);
        *(u8 *)(ofs_b + (int)kaze_state_base_ptr + 0x48898) = 0x80;
        *(u8 *)(ofs_b + (int)kaze_state_base_ptr + 0x48899) = 0x80;
        *(u8 *)(ofs_b + (int)kaze_state_base_ptr + 0x4889A) = 0x80;
        r_b = (char *)(ofs_b + (int)kaze_state_base_ptr);
        *(short *)(r_b + 0x4889C) = 0;
        *(short *)(r_b + 0x4889E) = 0;
        *(short *)(r_b + 0x488A0) = 0x140;
        *(short *)(r_b + 0x488A2) = 0;
        *(short *)(r_b + 0x488A4) = 0;
        *(short *)(r_b + 0x488A6) = 0xF0;
        *(short *)(r_b + 0x488A8) = 0x140;
        *(short *)(r_b + 0x488AA) = 0xF0;
        tp = GetTPage(0, 2, 0x140, 0);
        dofs = outer * 8;
        SetDrawTPage(kaze_state_base_ptr + (dofs + 0x488C4), 0, 1, tp & 0xFFFF);
        outer++;
    } while (outer < 2);
}

extern SRECT kaze_post_event_grid_area_rect1;
extern void SetDrawArea(void *, SRECT *);

/**
 * kaze_post_event_grid_prim_init  (render, 0x8001833c)
 *
 * Post-event grid POLY_FT4 slots, grid panel POLY_F4, and LINE_F2/LINE_F3
 * setup (+0x49804..+0x49b6e).
 */

void kaze_post_event_grid_prim_init(void)
{
    extern int GetClut(int, int);
    extern void SetPolyFT4(void *);
    extern SRECT D_80010278;
    extern char *kaze_state_base_ptr;
    SRECT r1;
    SRECT r2;
    int outer;
    int i;
    register int i2 asm("$16");
    int lofs;
    char *p3, *p4, *p5, *p6;
    char *q2, *q3, *q4, *q5;
    int lbase;
    register int var_v0 asm("$2");
    int var_v0_2;
    register int c7f asm("$23");
    register int c3f asm("$22");
    register int zr asm("$0");

    r1 = kaze_post_event_grid_area_rect1;
    r2 = D_80010278;
    c7f = 0x7F;
    c3f = 0x3F;
    outer = 0;
    do {
        i = 0;
        do {
            SetPolyFT4(kaze_state_base_ptr + (outer * 0x140 + 0x49800) + i * 0x28);
            *(u8 *)(i * 0x28 + (outer * 0x140 + (int)kaze_state_base_ptr) + 0x49804) = 0x40;
            *(u8 *)(i * 0x28 + (outer * 0x140 + (int)kaze_state_base_ptr) + 0x49805) = 0x40;
            *(u8 *)(i * 0x28 + (outer * 0x140 + (int)kaze_state_base_ptr) + 0x49806) = 0x40;
            ((U16S *)((int)kaze_state_base_ptr + (outer * 0x140 + i * 0x28) + 0x49816))->f = GetTPage(0, 0, 0x240, 0x100);
            ((U16S *)((int)kaze_state_base_ptr + (outer * 0x140 + i * 0x28) + 0x4980E))->f = GetClut(0x150, 0x1F7);
            i++;
        } while (i < 8);
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4980C) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4980D) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49814) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49815) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4981C) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4981D) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49824) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49825) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49834) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49835) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4983C) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4983D) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49844) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49845) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4984C) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4984D) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4985C) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4985D) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49864) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49865) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4986C) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4986D) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49874) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49875) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49884) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49885) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4988C) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4988D) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49894) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49895) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4989C) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4989D) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498AC) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498AD) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498B4) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498B5) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498BC) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498BD) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498C4) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498C5) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498D4) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498D5) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498DC) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498DD) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498E4) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498E5) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498EC) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498ED) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498FC) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x498FD) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49904) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49905) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4990C) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4990D) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49914) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49915) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49924) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49925) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4992C) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4992D) = 0;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49934) = 0x40;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x49935) = c3f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4993C) = c7f;
        *(u8 *)(outer * 0x140 + (int)kaze_state_base_ptr + 0x4993D) = c3f;
        SetPolyF4(kaze_state_base_ptr + (outer * 0x18 + 0x49A80));
        SetSemiTrans(kaze_state_base_ptr + (outer * 0x18 + 0x49A80), 1);
        *(u8 *)(outer * 0x18 + (int)kaze_state_base_ptr + 0x49A84) = 0x80;
        *(u8 *)(outer * 0x18 + (int)kaze_state_base_ptr + 0x49A85) = 0x80;
        *(u8 *)(outer * 0x18 + (int)kaze_state_base_ptr + 0x49A86) = 0x80;
        lofs = outer << 5;
        q2 = (char *)(outer * 0x18 + (int)kaze_state_base_ptr);
        *(short *)(q2 + 0x49A88) = 0x10;
        *(short *)(q2 + 0x49A8A) = 0xA4;
        *(short *)(q2 + 0x49A8E) = 0xA4;
        *(short *)(q2 + 0x49A8C) = 0x40;
        *(short *)(q2 + 0x49A90) = 0x10;
        *(short *)(q2 + 0x49A92) = 0xD4;
        *(short *)(q2 + 0x49A94) = 0x40;
        *(short *)(q2 + 0x49A96) = 0xD4;
        i2 = 0;
        do {
            SetLineF2(kaze_state_base_ptr + (lofs + 0x49AB0) + i2);
            SetSemiTrans(kaze_state_base_ptr + (lofs + 0x49AB0) + i2, 1);
            *(u8 *)(i2 + (lofs + (int)kaze_state_base_ptr) + 0x49AB4) = 0;
            { register int c40 asm("$3") = 0x40; *(u8 *)(i2 + (lofs + (int)kaze_state_base_ptr) + 0x49AB5) = c40; }
            *(u8 *)(i2 + (lofs + (int)kaze_state_base_ptr) + 0x49AB6) = 0;
            p3 = (char *)(i2 + (lofs + (int)kaze_state_base_ptr));
            if (i2 != 0) {
                *(short *)(p3 + 0x49AB8) = 0x28;
            } else {
                *(short *)(p3 + 0x49AB8) = 0x10;
            }
            /* zr==$zero so (zr+0xBC) assembles byte-identically to `li 188`,
               but its RTL src is (plus $0 188) not (const_int 188), so
               loop.c combine_movables won't merge it with store D's 0xBC and
               hoist both into a callee-saved reg. */
            { char *pb = (char *)(i2 + (lofs + (int)kaze_state_base_ptr)); *(short *)(pb + 0x49ABA) = (i2 != 0) ? 0xA4 : (zr + 0xBC); }
            p4 = (char *)(i2 + (lofs + (int)kaze_state_base_ptr));
            if (i2 != 0) {
                *(short *)(p4 + 0x49ABC) = 0x28;
            } else {
                *(short *)(p4 + 0x49ABC) = c3f;
            }
            { char *pd = (char *)(i2 + (lofs + (int)kaze_state_base_ptr)); *(short *)(pd + 0x49ABE) = (i2 != 0) ? 0xD3 : 0xBC; }
            i2 += 0x10;
        } while (i2 < 0x20);
        i = 0;
        do {
            SetLineF3(kaze_state_base_ptr + (outer * 0x90 + 0x49AF0) + i * 0x18);
            i++;
        } while (i < 6);
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49AF4) = 8;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49AF5) = 0x7B;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49AF6) = 0x42;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B0C) = 8;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B0D) = 0x7B;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B0E) = 0x42;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B24) = 0x19;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B25) = 0xAD;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B26) = 8;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B3C) = 0x19;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B3D) = 0xAD;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B3E) = 8;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B54) = 0;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B55) = 0x4A;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B56) = 0x29;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B6C) = 0;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B6D) = 0x4A;
        *(u8 *)(outer * 0x90 + (int)kaze_state_base_ptr + 0x49B6E) = 0x29;
        q3 = (char *)(outer * 0x90 + (int)kaze_state_base_ptr);
        *(short *)(q3 + 0x49AF8) = 0xE;
        *(short *)(q3 + 0x49AFA) = 0xD5;
        *(short *)(q3 + 0x49AFC) = 0xE;
        *(short *)(q3 + 0x49AFE) = 0xA2;
        *(short *)(q3 + 0x49B00) = 0x41;
        *(short *)(q3 + 0x49B02) = 0xA2;
        *(short *)(q3 + 0x49B10) = 0x41;
        *(short *)(q3 + 0x49B12) = 0xA2;
        *(short *)(q3 + 0x49B14) = 0x41;
        *(short *)(q3 + 0x49B16) = 0xD5;
        *(short *)(q3 + 0x49B18) = 0xE;
        *(short *)(q3 + 0x49B1A) = 0xD5;
        *(short *)(q3 + 0x49B28) = 0xD;
        *(short *)(q3 + 0x49B2A) = 0xD6;
        *(short *)(q3 + 0x49B2C) = 0xD;
        *(short *)(q3 + 0x49B2E) = 0xA1;
        *(short *)(q3 + 0x49B30) = 0x42;
        *(short *)(q3 + 0x49B32) = 0xA1;
        *(short *)(q3 + 0x49B40) = 0x40;
        *(short *)(q3 + 0x49B42) = 0xA3;
        *(short *)(q3 + 0x49B44) = 0x40;
        *(short *)(q3 + 0x49B46) = 0xD4;
        *(short *)(q3 + 0x49B48) = 0xF;
        *(short *)(q3 + 0x49B4A) = 0xD4;
        *(short *)(q3 + 0x49B58) = 0xF;
        *(short *)(q3 + 0x49B5A) = 0xD4;
        *(short *)(q3 + 0x49B5C) = 0xF;
        *(short *)(q3 + 0x49B5E) = 0xA3;
        *(short *)(q3 + 0x49B60) = 0x40;
        *(short *)(q3 + 0x49B62) = 0xA3;
        *(short *)(q3 + 0x49B70) = 0x42;
        *(short *)(q3 + 0x49B72) = 0xA1;
        *(short *)(q3 + 0x49B74) = 0x42;
        *(short *)(q3 + 0x49B76) = 0xD6;
        *(short *)(q3 + 0x49B78) = 0xD;
        *(short *)(q3 + 0x49B7A) = 0xD6;
        i = 0;
        do {
            SetSprt8(kaze_state_base_ptr + (outer * 0xA0 + 0x49C10) + i * 0x10);
            SetShadeTex(kaze_state_base_ptr + (outer * 0xA0 + 0x49C10) + i * 0x10, 1);
            ((U16S *)((int)kaze_state_base_ptr + (outer * 0xA0 + i * 0x10) + 0x49C1E))->f = GetClut(0x140, 0x1F9);
            i++;
        } while (i < 0xA);
        {
            register char *q4r asm("$8");
            register char *q4d asm("$3");
            q4r = (char *)(outer * 0xA0 + (int)kaze_state_base_ptr);
            *(short *)(q4r + 0x49C18) = 0x25;
            *(u8 *)(q4r + 0x49C1C) = 0x40;
            q4d = kaze_state_base_ptr;
            *(short *)(q4r + 0x49C1A) = 0xB9;
            *(u8 *)(outer * 0xA0 + (int)q4d + 0x49C1D) = 0x28;
        }
        SetDrawTPage(kaze_state_base_ptr + (outer * 0x10 + 0x49D50), 0, 1, GetTPage(0, 1, 0x200, 0x100) & 0xFFFF);
        SetDrawTPage(kaze_state_base_ptr + (outer * 0x10 + 0x49D50) + 8, 0, 1, GetTPage(0, 2, 0x200, 0x100) & 0xFFFF);
        r2.y = outer << 8;
        r1.y += outer << 8;
        SetDrawArea(kaze_state_base_ptr + (outer * 0x18 + 0x49D70), &r1);
        SetDrawArea(kaze_state_base_ptr + (outer * 0x18 + 0x49D70) + 0xC, &r2);
        i = 0;
        do {
            SetSprt(kaze_state_base_ptr + (outer * 0x78 + 0x49DA0) + i * 0x14);
            SetShadeTex(kaze_state_base_ptr + (outer * 0x78 + 0x49DA0) + i * 0x14, 1);
            q5 = (char *)(i * 0x14 + (outer * 0x78 + (int)kaze_state_base_ptr));
            *(short *)(q5 + 0x49DB0) = 0xC;
            *(short *)(q5 + 0x49DB2) = 0x10;
            ((U16S *)((int)kaze_state_base_ptr + (outer * 0x78 + i * 0x14) + 0x49DAE))->f = GetClut(0x170, 0x1F4);
            i++;
        } while (i < 6);
        SetDrawTPage(kaze_state_base_ptr + (outer * 8 + 0x49E90), 0, 1, GetTPage(0, 0, 0x380, 0x100) & 0xFFFF);
        outer++;
    } while (outer < 2);
}

/**
 * kaze_post_event_banner_prim_init  (render, 0x80019570)
 *
 * Post-event banner/score SPRT and draw TPage setup (+0x49ea0..+0x4a180)
 * plus large-panel/copy tables.
 */

void kaze_post_event_banner_prim_init(void)
{
    extern int GetClut(int, int);
    extern char *kaze_state_base_ptr;
    int outer;
    int o2;
    int o5;
    int t1;
    register int s1 asm("$16");
    char *p;
    char *q;
    short clut1;

    outer = 0;
    do {
        o2 = outer * 0x14;
        o5 = outer * 0x18;
        SetSprt(kaze_state_base_ptr + (o2 + 0x49EA0));
        {
            register char *pv asm("$2") = (char *)(o2 + (int)kaze_state_base_ptr);
            *(u8 *)(pv + 0x49EAC) = 0;
            q = (char *)(o2 + (int)kaze_state_base_ptr);
            *(u16 *)(pv + 0x49EB0) = 0x100;
            *(u16 *)(pv + 0x49EB2) = 0x20;
            *(u8 *)(q + 0x49EAD) = 0;
        }
        *(u8 *)(o2 + (int)kaze_state_base_ptr + 0x49EA4) = 0x80;
        *(u8 *)(o2 + (int)kaze_state_base_ptr + 0x49EA5) = 0x80;
        *(u8 *)(o2 + (int)kaze_state_base_ptr + 0x49EA6) = 0x80;
        clut1 = GetClut(0x140, 0x1F4);
        {
            register char *arg asm("$4") = kaze_state_base_ptr + (o2 + 0x49EC8);
            __asm__ volatile("" : "=r"(arg) : "0"(arg));
            *(u16 *)((int)kaze_state_base_ptr + o2 + 0x49EAE) = clut1;
            SetSprt(arg);
        }
        t1 = outer * 8;
        {
            register char *p2 asm("$2") = (char *)(o2 + (int)kaze_state_base_ptr);
            *(u8 *)(p2 + 0x49ED4) = 0;
            {
                register char *q2 asm("$3") = (char *)(o2 + (int)kaze_state_base_ptr);
                *(u16 *)(p2 + 0x49ED8) = 0x100;
                *(u16 *)(p2 + 0x49EDA) = 0x20;
                *(u8 *)(q2 + 0x49ED5) = 0x20;
            }
        }
        *(u8 *)(o2 + (int)kaze_state_base_ptr + 0x49ECC) = 0x80;
        *(u8 *)(o2 + (int)kaze_state_base_ptr + 0x49ECD) = 0x80;
        *(u8 *)(o2 + (int)kaze_state_base_ptr + 0x49ECE) = 0x80;
        clut1 = GetClut(0x150, 0x1F4);
        {
            register char *arg2 asm("$4") = kaze_state_base_ptr + (o2 + 0x49EF0);
            *(u16 *)((int)kaze_state_base_ptr + o2 + 0x49ED6) = clut1;
            SetSprt(arg2);
        }
        {
            register char *p3 asm("$2") = (char *)(o2 + (int)kaze_state_base_ptr);
            *(u8 *)(p3 + 0x49EFC) = 0;
            {
                register char *q3 asm("$3") = (char *)(o2 + (int)kaze_state_base_ptr);
                *(u16 *)(p3 + 0x49F00) = 0x80;
                *(u16 *)(p3 + 0x49F02) = 0x20;
                *(u8 *)(q3 + 0x49EFD) = 0xC0;
            }
        }
        *(u8 *)(o2 + (int)kaze_state_base_ptr + 0x49EF4) = 0x80;
        *(u8 *)(o2 + (int)kaze_state_base_ptr + 0x49EF5) = 0x80;
        *(u8 *)(o2 + (int)kaze_state_base_ptr + 0x49EF6) = 0x80;
        *(u16 *)((int)kaze_state_base_ptr + o2 + 0x49EFE) = GetClut(0x160, 0x1F4);
        SetDrawTPage(kaze_state_base_ptr + (t1 + 0x4A0F8), 0, 1, GetTPage(0, 0, 0x380, 0x100) & 0xFFFF);
        SetDrawTPage(kaze_state_base_ptr + (t1 + 0x4A108), 0, 1, GetTPage(0, 0, 0x380, 0x100) & 0xFFFF);
        SetDrawTPage(kaze_state_base_ptr + (t1 + 0x4A118), 0, 1, GetTPage(0, 0, 0x380, 0x100) & 0xFFFF);
        s1 = o5 + 0x4A128;
        {
            register char *poly asm("$4") = kaze_state_base_ptr + s1;
            SetPolyF4(poly);
        }
        {
            register char *semi asm("$4") = kaze_state_base_ptr + s1;
            SetSemiTrans(semi, 1);
        }
        s1 = o2 + 0x4A158;
        {
            register char *spr2 asm("$4") = kaze_state_base_ptr + s1;
            {
                register char *p0 asm("$2") = kaze_state_base_ptr + o5;
                *(u16 *)(p0 + 0x4A13C) = 0x140;
                *(u16 *)(p0 + 0x4A134) = 0x140;
                *(u16 *)(p0 + 0x4A136) = 0;
                *(u16 *)(p0 + 0x4A132) = 0;
                *(u16 *)(p0 + 0x4A138) = 0;
                *(u16 *)(p0 + 0x4A130) = 0;
                *(u16 *)(p0 + 0x4A13E) = 0xF0;
                *(u16 *)(p0 + 0x4A13A) = 0xF0;
            }
            SetSprt(spr2);
        }
        SetShadeTex(kaze_state_base_ptr + s1, 1);
        {
            register int clx2 asm("$4") = 0x1B0;
            register char *p4 asm("$3") = (char *)(o2 + (int)kaze_state_base_ptr);
            *(u8 *)(p4 + 0x4A164) = 0x80;
            {
                register char *q4 asm("$6") = (char *)(o2 + (int)kaze_state_base_ptr);
                *(short *)(p4 + 0x4A160) = -0x10;
                *(u16 *)(p4 + 0x4A162) = 0x18;
                *(u16 *)(p4 + 0x4A168) = 0x80;
                *(u16 *)(p4 + 0x4A16A) = 0x20;
                *(u8 *)(q4 + 0x4A165) = 0xE0;
            }
            clut1 = GetClut(clx2, 0x1F5);
        }
        *(u16 *)((int)kaze_state_base_ptr + o2 + 0x4A166) = clut1;
        SetDrawTPage(kaze_state_base_ptr + (t1 + 0x4A180), 0, 1, GetTPage(0, 2, 0x380, 0x100) & 0xFFFF);
        outer++;
    } while (outer < 2);

    {
        int o;
        register int clx asm("$17");
        register int c80 asm("$18");
        int cx;
        char *p;
        char *q;
        clx = 0x180;
        outer = 0;
        c80 = 0x80;
        o = 0;
        do {
            SetSprt(kaze_state_base_ptr + (o + 0x49F18));
            p = (char *)(o + (int)kaze_state_base_ptr);
            *(u16 *)(p + 0x49F28) = 0x40;
            *(u8 *)(p + 0x49F24) = outer << 6;
            q = (char *)(o + (int)kaze_state_base_ptr);
            *(u16 *)(p + 0x49F2A) = 0x10;
            *(u8 *)(q + 0x49F25) = c80;
            *(u8 *)(o + (int)kaze_state_base_ptr + 0x49F1C) = c80;
            cx = clx;
            *(u8 *)(o + (int)kaze_state_base_ptr + 0x49F1D) = c80;
            *(u8 *)(o + (int)kaze_state_base_ptr + 0x49F1E) = c80;
            clx += 0x10;
            *(u16 *)((int)kaze_state_base_ptr + o + 0x49F26) = GetClut(cx, 0x1F4);
            o += 0x14;
            outer++;
        } while (outer < 3);
    }

    {
        int o;
        register int c80 asm("$17");
        char *p;
        char *q;
        c80 = 0x80;
        o = 0;
        do {
            {
                register int arg asm("$4") = o + 0x49F90;
                SetSprt(kaze_state_base_ptr + arg);
            }
            q = (char *)(o + (int)kaze_state_base_ptr);
            if (o != 0) {
                *(u16 *)(q + 0x49FA0) = 0x40;
            } else {
                *(u16 *)(q + 0x49FA0) = c80;
            }
            p = (char *)(o + (int)kaze_state_base_ptr);
            *(u8 *)(p + 0x49F94) = c80;
            {
                register char *q95 asm("$4") = (char *)(o + (int)kaze_state_base_ptr);
            *(u16 *)(p + 0x49FA2) = 0x10;
                *(u8 *)(q95 + 0x49F95) = c80;
            }
            *(u8 *)(o + (int)kaze_state_base_ptr + 0x49F96) = c80;
            o += 0x14;
        } while (o < 0x3C);
    }

    {
        int o;
        char *p;
        register char *gb asm("$4");
        short clut;
        W4 *src;
        W4 *dst;
        W4 *end;
        register int c80 asm("$17");
        outer = 0;
        c80 = 0x80;
        o = 0;
        do {
            SetSprt(kaze_state_base_ptr + (o + 0x4A008));
            {
                register char *p4 asm("$5") = (char *)(o + (int)kaze_state_base_ptr);
                *(u8 *)(p4 + 0x4A00C) = c80;
                {
                    register char *q4 asm("$3") = (char *)(o + (int)kaze_state_base_ptr);
                *(u16 *)(p4 + 0x4A018) = 0xC;
                *(u16 *)(p4 + 0x4A01A) = 0x10;
                    *(u8 *)(q4 + 0x4A00D) = c80;
                }
            }
            *(u8 *)(o + (int)kaze_state_base_ptr + 0x4A00E) = c80;
            clut = GetClut(0x170, 0x1F4);
            gb = kaze_state_base_ptr;
            *(u16 *)((int)gb + o + 0x4A016) = clut;
            o += 0x14;
            outer++;
        } while (outer < 6);
        dst = (W4 *)(gb + 0x49F54);
        src = (W4 *)(gb + 0x49F18);
        end = (W4 *)((int)src + 0x30);
        do {
            *dst = *src;
            src++;
            dst++;
        } while (src != end);
        *(W3 *)dst = *(W3 *)src;
    }

    {
        W4 *src;
        W4 *dst;
        W4 *end;
        dst = (W4 *)(kaze_state_base_ptr + 0x49FCC);
        src = (W4 *)(kaze_state_base_ptr + 0x49F90);
        end = (W4 *)((int)src + 0x30);
        do {
            *dst = *src;
            src++;
            dst++;
        } while (src != end);
        *(W3 *)dst = *(W3 *)src;
    }

    {
        W4 *src;
        W4 *dst;
        W4 *end;
        dst = (W4 *)(kaze_state_base_ptr + 0x4A080);
        src = (W4 *)(kaze_state_base_ptr + 0x4A008);
        end = (W4 *)((int)src + 0x70);
        do {
            *dst = *src;
            src++;
            dst++;
        } while (src != end);
        *(W2 *)dst = *(W2 *)src;
    }
}

extern int kaze_input_current_a;
extern int kaze_input_current_pad2_mask;
extern int kaze_input_current_b;
extern int kaze_input_prev_pad2_mask;
extern int kaze_stage_mode;
extern int kaze_heap_alloc(int, int);
extern void GsInitVcount(void);
extern void kaze_play_sound_effect(int, int, int, int);
extern int GsGetVcount(void);
extern void kaze_gs_clear_vcount(void);
extern void kaze_seq_stop_track_34310(int);
extern void kaze_sfx_voice_keyoff_34170(int);
extern void kaze_heap_free(void);

/**
 * kaze_main_game_loop  (core, 0x80019e94)
 *
 * Main game loop. Allocates the 0xed430-byte live-root state block via
 * kaze_heap_alloc into kaze_state_base_ptr, resets input latches, runs the
 * double-buffered render loop (page index +0x98, primitive cursor +0x9c,
 * load metric +0xa0, vsync divisor +0xa4), and frees the root on exit. +0x0
 * is the loop-exit/next-mode code, +0x4 the frame-loop running flag.
 */

void kaze_main_game_loop(int demo_mode)
{
    extern int VSync(int);
    extern int DrawSync(int);
    extern int kaze_state_base_ptr;
    int env;
    int ot;
    int ot_00;
    int idx;
    int prim_load;
    register int cond asm("$2");
    long vsync_count;

    SetDispMask(0);
    kaze_state_base_ptr = kaze_heap_alloc(1, 0xED430);
    if (kaze_state_base_ptr == 0) {
        exit(0x2BD);
    }
    kaze_map_cdlfile_lookup();
    kaze_drawenv_init();
    kaze_stage_staging_copy();
    *(int *)kaze_state_base_ptr = 0;
    do {
        kaze_cd_setmode_vsync_341c();
        kaze_input_current_pad2_mask = 0;
        kaze_input_current_a = 0;
        kaze_input_prev_pad2_mask = -1;
        kaze_input_current_b = -1;
        if (demo_mode != 0) {
            *(short *)(kaze_state_base_ptr + 0x48208) = 0x140;
            *(short *)(kaze_state_base_ptr + 0x4820A) = 0xF0;
        } else {
            kaze_start_location_map_loop(kaze_state_base_ptr + 0x4120, kaze_state_base_ptr + 0x28198,
                                         kaze_state_base_ptr + 0x5B6E0, kaze_state_base_ptr + 0xE2EE0);
            kaze_clear_image_rect(0, 0, 0x140, 0x100);
            kaze_clear_image_rect(0, 0x100, 0x140, 0x100);
            *(short *)(kaze_state_base_ptr + 0x4820A) = 0;
            *(short *)(kaze_state_base_ptr + 0x48208) = 0;
        }
        kaze_flight_state_init(demo_mode);
        kaze_xa_track_seek_setup_3454(*(int *)(kaze_state_base_ptr + 0xECAE4));
        *(int *)(kaze_state_base_ptr + 0xA4) = 3;
        GsInitVcount();
        *(int *)(kaze_state_base_ptr + 8) = -1;
        *(int *)(kaze_state_base_ptr + 0x98) = 0;
        *(int *)(kaze_state_base_ptr + 4) = 1;
        do {
            idx = *(int *)(kaze_state_base_ptr + 0x98) ^ 1;
            {
                int m = idx * 0x24078 + 0xA8;
                __asm__("" : "=r"(m) : "0"(m));
                env = kaze_state_base_ptr + m;
            }
            ot_00 = env + 0x70;
            *(int *)(kaze_state_base_ptr + 0x98) = idx;
            *(int *)(kaze_state_base_ptr + 0x9C) = env + 0x4078;
            ClearOTagR(ot_00, 0x1000);
            ot = env + 0x4070;
            ClearOTagR(ot, 2);
            TermPrim(ot_00);
            TermPrim(ot);
            kaze_xa_audio_manager_35a0(*(int *)(kaze_state_base_ptr + 0xECAE4), *(int *)(kaze_state_base_ptr + 0xA4));
            kaze_play_sound_effect(0, *(short *)(kaze_state_base_ptr + 0xECAE8), 0, 0);
            kaze_play_sound_effect(0xB, *(short *)(kaze_state_base_ptr + 0xECAEC), 0, 0);
            if (kaze_stage_mode == 1) {
                kaze_play_sound_effect(7, *(short *)(kaze_state_base_ptr + 0xECAF0), 0, 0);
            }
            kaze_demo_input_playback();
            kaze_hud_status_update();
            kaze_flight_timer_gas_update();
            kaze_world_update_pass();
            kaze_flight_scene_render_dispatch(ot_00, ot);
            GsGetVcount();
            DrawSync(0);
            GsGetVcount();
            VSync(3);
            vsync_count = GsGetVcount();
            *(int *)(kaze_state_base_ptr + 0xA4) = (vsync_count + 0x83) / 0x107;
            kaze_gs_clear_vcount();
            kaze_pad_mask_update();
            {
                int t = *(int *)(kaze_state_base_ptr + 0x9C) + -0x4078;
                __asm__("" : "=r"(t) : "0"(t));
                prim_load = (t - env >> 2) * 400;
            }
            if (prim_load < 0) {
                prim_load = prim_load + 0x1FFFF;
            }
            *(int *)(kaze_state_base_ptr + 0xA0) = prim_load >> 0x11;
            PutDispEnv(env + 0x5C);
            DrawOTagEnv(env + 0x406C, env);
        } while (*(int *)(kaze_state_base_ptr + 4) != 0);
        DrawSync(0);
        kaze_xa_audio_stop_33a3c();
        kaze_seq_stop_track_34310(-1);
        kaze_burner_sound_toggle(0);
        kaze_sfx_voice_keyoff_34170(0x1E);
        {
            register int *root asm("$4") = (int *)kaze_state_base_ptr;
            __asm__("" : "=r"(root) : "0"(root));
            cond = *root;
        }
    } while (cond == 0);
    kaze_heap_free();
    kaze_state_base_ptr = 0;
}
