#include "types.h"
#include "kaze_syms.h"
extern char kaze_ending_balloon_model_path[];
extern char kaze_ending_full_tim_path[];
extern char kaze_ending_txp_path[];
extern char kaze_ending_txp2_path[];
extern int kaze_ending_ot_buffer_a;
extern int kaze_ending_ot_buffer_b;
extern int kaze_ending_packet_buffer_b;
extern char kaze_ending_backdrop_poly_r;
extern char kaze_ending_backdrop_poly_g;
extern char kaze_ending_backdrop_poly_b;
extern short kaze_ending_backdrop_poly_x0;
extern short kaze_ending_backdrop_poly_y0;
extern short kaze_ending_backdrop_poly_x1;
extern short kaze_ending_backdrop_poly_y1;
extern short kaze_ending_backdrop_poly_x2;
extern short kaze_ending_backdrop_poly_y2;
extern short kaze_ending_backdrop_poly_x3;
extern short kaze_ending_backdrop_poly_y3;
extern char kaze_ending_credit_title_cluts[];
extern char kaze_ending_credit_line_cluts[];
extern char kaze_ending_fade_polys[];
extern char kaze_ending_fade_tpage_prims[];
extern char kaze_ending_flame_polys[];
extern char kaze_ending_flame_poly_clut[];
extern char kaze_ending_flame_poly_tpage[];
extern char kaze_ending_flame_coord2[];
extern int kaze_ending_flame_pos_x;
extern int kaze_ending_flame_pos_y;
extern int kaze_ending_flame_pos_z;
extern short kaze_ending_flame_angle_y;
extern short kaze_ending_flame_angle_z;
extern int kaze_ending_screen_state;
extern int kaze_ending_credits_substate;
extern int kaze_ending_debug_camera_mode;
extern int kaze_ending_debug_toggle_flag;
extern int kaze_ending_fade_phase;
extern int kaze_ending_fade_level;
extern int kaze_ending_credits_frame_counter;
extern int kaze_ending_balloon_bob_phase;
extern int kaze_ending_balloon_model_ptr;
extern int kaze_ending_credit_page_texture_table[];
extern int kaze_ending_music_track_id;
extern unsigned int kaze_input_pad2_mask_current;
extern int kaze_input_pad1_hold_counter;
extern int kaze_input_pad2_hold_counter;
extern unsigned int kaze_input_pad1_mask_previous;
extern unsigned int kaze_input_pad2_mask_previous;
extern void GsInit3D(void);
extern void GsInitGraph(int, int, int, int, int);
extern void ResetGraph(int);
extern void SetPolyF4(void *);
extern char kaze_ending_backdrop_sprites[];
extern char kaze_ending_backdrop_tpage_prims[];
extern int kaze_ending_backdrop_scroll_x;
extern int kaze_ending_backdrop_scroll_y;
extern char kaze_ending_backdrop_poly[];
extern char kaze_ending_credit_title_sprites[];
extern char kaze_ending_credit_line_sprites[];
extern char kaze_ending_credit_line_tpage_prims[];
typedef struct {
    short x, y, w, h;
} RECT;

extern unsigned int kaze_input_current_a;
extern unsigned int kaze_input_current_b;
extern unsigned int kaze_input_current_pad2_mask;
extern unsigned int kaze_input_prev_pad2_mask;
extern unsigned int kaze_menu_input_replay_flags;
extern unsigned int kaze_pad2_edge_mask;

/**
 * kaze_pad_edge_latch_update  (core, 0x80062cb0)
 *
 * Pad edge/latch update: after kaze_pad_mask_update, copies current masks
 * kaze_input_current_a/0e98 to kaze_menu_input_replay_flags/0ea4 and
 * kaze_input_pad1_mask_previous/1540, derives new-press masks
 * kaze_pad1_edge_mask/4d54 vs previous kaze_input_current_b/1514, and hold
 * counters kaze_input_pad1_hold_counter/0f54.
 */

void kaze_pad_edge_latch_update(void)
{
    extern unsigned int kaze_pad1_edge_mask;
    kaze_pad_mask_update();
    kaze_menu_input_replay_flags = kaze_input_current_a;
    kaze_input_pad1_mask_previous = kaze_input_current_b;
    kaze_input_pad2_mask_current = kaze_input_current_pad2_mask;
    kaze_input_pad2_mask_previous = kaze_input_prev_pad2_mask;

    if (kaze_menu_input_replay_flags != kaze_input_pad1_mask_previous) {
        kaze_input_pad1_hold_counter = 0;
        kaze_pad1_edge_mask = kaze_menu_input_replay_flags & ~kaze_input_pad1_mask_previous;
    } else {
        kaze_pad1_edge_mask = 0;
        kaze_input_pad1_hold_counter = kaze_input_pad1_hold_counter + 1;
    }

    if (kaze_input_pad2_mask_current != kaze_input_pad2_mask_previous) {
        kaze_input_pad2_hold_counter = 0;
        kaze_pad2_edge_mask = kaze_input_pad2_mask_current & ~kaze_input_pad2_mask_previous;
    } else {
        kaze_pad2_edge_mask = 0;
        kaze_input_pad2_hold_counter = kaze_input_pad2_hold_counter + 1;
    }
}

extern void ResetCallback(void);
void kaze_ending_graphics_init();
void kaze_ending_state_reset();
extern int GsGetActiveBuff(void);
extern void kaze_gs_set_work_base(int);
extern void GsClearOt(int, int, int *);
void kaze_ending_state_dispatch();
extern void VSync(int);
extern void DrawSync(int);
extern void GsSwapDispBuff(void);
extern void GsSortClear(int, int, int, int *);
extern void GsDrawOt(int *);
void kaze_ending_buffers_free();

/**
 * kaze_ending_screen_loop  (menu, 0x80062da4)
 *
 * Ending/credits screen main loop: inits via
 * kaze_ending_graphics_init/kaze_ending_state_reset, then per frame sets GS
 * work base, clears/draws the OT (GsClearOt/GsSortClear/GsDrawOt),
 * dispatches kaze_ending_state_dispatch until kaze_ending_screen_state==2;
 * plays music kaze_ending_music_track_id via kaze_xa_audio_manager_35a0.
 */

void kaze_ending_screen_loop(void)
{
    extern int kaze_ending_ot_headers[];
    extern int kaze_ending_packet_buffer_a[];
    int i;
    int j;

    ResetCallback();
    kaze_ending_graphics_init();
    kaze_ending_state_reset();
    while (kaze_ending_screen_state != 2) {
        i = GsGetActiveBuff();
        kaze_gs_set_work_base(kaze_ending_packet_buffer_a[i]);
        j = i * 5;
        GsClearOt(0, 0, kaze_ending_ot_headers + j);
        kaze_ending_state_dispatch(i);
        VSync(0);
        DrawSync(0);
        kaze_pad_edge_latch_update();
        GsSwapDispBuff();
        GsSortClear(0, 0, 0, kaze_ending_ot_headers + j);
        GsDrawOt(kaze_ending_ot_headers + j);
        if (kaze_ending_music_track_id != -2) {
            kaze_xa_audio_manager_35a0(kaze_ending_music_track_id, 2);
        }
    }
    kaze_ending_buffers_free();
}

void kaze_ending_credits_update();

/**
 * kaze_ending_state_dispatch  (menu, 0x80062ec0)
 *
 * Ending screen state dispatcher on kaze_ending_screen_state: state 0 runs
 * the debug viewer kaze_ending_debug_viewer_update, state 1 runs the credits
 * sequence kaze_ending_credits_update.
 */

void kaze_ending_state_dispatch(void)
{
    switch (kaze_ending_screen_state) {
    case 0:
        kaze_ending_debug_viewer_update();
        break;
    case 1:
        kaze_ending_credits_update();
        break;
    }
}

extern int kaze_ending_credit_page_index;
void kaze_ending_textures_load();
void kaze_ending_balloon_model_load();
void kaze_ending_fade_poly_update();
extern void kaze_title_model_sort_dispatch(int, void *);
extern void kaze_ending_credit_prims_add(int);

/**
 * kaze_ending_credits_update  (menu, 0x80062f10)
 *
 * Credits sequence driver on substate kaze_ending_credits_substate: loads
 * END textures/balloon model
 * (kaze_ending_textures_load/kaze_ending_balloon_model_load), per-page
 * keyframe/credit setup
 * (kaze_ending_credit_text_layout/kaze_ending_keyframe_load), fade and flame
 * updates; exits to kaze_ending_screen_state=2 on pad bits 0x820 of
 * kaze_pad1_edge_mask.
 */

void kaze_ending_credits_update(int buf)
{
    extern int kaze_pad1_edge_mask;
    extern int kaze_ending_ot_headers;
    int saved_flag;

    kaze_ending_credits_frame_counter = kaze_ending_credits_frame_counter + 1;
    if (kaze_ending_credits_substate != 1) {
        if (kaze_ending_credits_substate == 2) {
            kaze_ending_credit_text_layout();
            kaze_ending_keyframe_load();
            kaze_ending_path_segment_setup();
            kaze_ending_credit_page_upload();
            kaze_ending_credits_substate = 3;
            kaze_ending_fade_phase = 0;
            kaze_ending_balloon_bob_phase = 0;
        }
    }
    else {
        kaze_ending_textures_load();
        kaze_ending_balloon_model_load();
        kaze_ending_music_track_id = 0xf;
        kaze_cd_setmode_vsync_341c();
        kaze_ending_credits_substate = 0;
    }
    switch (kaze_ending_credits_substate) {
    case 0:
        if (kaze_ending_credits_frame_counter == (kaze_ending_credits_frame_counter / 0x78) * 0x78) {
            kaze_ending_credits_substate = 1;
            kaze_ending_fade_phase = 1;
        }
        kaze_ending_fade_poly_update(buf);
        kaze_ending_screen_move_prim_add(buf, 0);
        if (kaze_ending_fade_phase == 4) {
            kaze_ending_credit_page_index = 0;
            kaze_ending_credits_substate = 2;
            kaze_ending_fade_phase = 0;
        }
        return;
    case 3:
        if (kaze_ending_credits_frame_counter == (kaze_ending_credits_frame_counter / 0x1e0) * 0x1e0) {
            kaze_ending_fade_phase = 1;
        }
        kaze_ending_fade_poly_update(buf);
        kaze_ending_balloon_path_step();
        kaze_ending_burner_light_update();
        kaze_ending_flame_quad_render(buf);
        kaze_title_model_sort_dispatch(buf, &kaze_ending_ot_headers + buf * 5);
        kaze_ending_credit_prims_add(buf);
        saved_flag = kaze_ending_fade_phase;
        if (kaze_ending_fade_phase == 4) {
            kaze_ending_credit_page_index = kaze_ending_credit_page_index + 1;
            kaze_ending_credits_substate = 2;
            if (kaze_ending_credit_page_index == (kaze_ending_credit_page_index / 0xc) * 0xc) {
                kaze_ending_keyframe_load();
                kaze_ending_path_segment_setup();
                kaze_ending_credit_page_index = 0;
                kaze_ending_credits_substate = saved_flag;
            }
            kaze_ending_fade_phase = 0;
        }
        return;
    case 4:
        kaze_ending_fade_poly_update(buf);
        kaze_ending_balloon_path_step();
        kaze_ending_burner_light_update();
        kaze_ending_flame_quad_render(buf);
        kaze_title_model_sort_dispatch(buf, &kaze_ending_ot_headers + buf * 5);
        kaze_ending_screen_move_prim_add(buf, 1);
        if ((kaze_pad1_edge_mask & 0x820) != 0) {
            kaze_ending_fade_phase = 1;
        }
        if (kaze_ending_fade_phase == 4) {
            kaze_ending_screen_state = 2;
            kaze_ending_fade_phase = 0;
            kaze_ending_credits_frame_counter = 0;
        }
        return;
    }
}

extern void AddPrim(void *, void *);

/**
 * kaze_ending_fade_poly_update  (menu, 0x80063258)
 *
 * Fade in/out for the ending screen: adds the POLY_F4 at
 * kaze_ending_fade_polys plus DR_TPAGE kaze_ending_fade_tpage_prims and
 * ramps level kaze_ending_fade_level by +/-5, switching phase flag
 * kaze_ending_fade_phase at 0/0xff.
 */

void kaze_ending_fade_poly_update(int arg0)
{
    extern volatile int kaze_ending_ot_base_ptrs[];
    int phase;
    int off;
    register int ot asm("$16");
    register char *base asm("$3");
    register char *poly asm("$5");
    register int idx asm("$5");

    phase = kaze_ending_fade_phase;
    switch (phase) {
    case 0:
        base = kaze_ending_fade_polys;
        idx = arg0 * 2;
        idx = idx + arg0;
        off = idx * 8;
        poly = base + off;
        ot = arg0 * 4;
        poly[4] = kaze_ending_fade_level;
        ot = ot + arg0;
        poly[5] = kaze_ending_fade_level;
        ot = ot * 4;
        poly[6] = kaze_ending_fade_level;
        AddPrim((void *)*(volatile int *)((char *)kaze_ending_ot_base_ptrs + ot), poly);
        AddPrim((void *)*(volatile int *)((char *)kaze_ending_ot_base_ptrs + ot), kaze_ending_fade_tpage_prims + arg0 * 8);
        kaze_ending_fade_level = kaze_ending_fade_level - 5;
        if (kaze_ending_fade_level < 0) {
            kaze_ending_fade_phase = 3;
            kaze_ending_fade_level = 0;
        }
        break;
    case 1:
        base = kaze_ending_fade_polys;
        idx = arg0 * 2;
        idx = idx + arg0;
        off = idx * 8;
        poly = base + off;
        ot = arg0 * 4;
        poly[4] = kaze_ending_fade_level;
        ot = ot + arg0;
        poly[5] = kaze_ending_fade_level;
        ot = ot * 4;
        poly[6] = kaze_ending_fade_level;
        AddPrim((void *)*(volatile int *)((char *)kaze_ending_ot_base_ptrs + ot), poly);
        AddPrim((void *)*(volatile int *)((char *)kaze_ending_ot_base_ptrs + ot), kaze_ending_fade_tpage_prims + arg0 * 8);
        kaze_ending_fade_level = kaze_ending_fade_level + 5;
        if (kaze_ending_fade_level >= 0x100) {
            kaze_ending_fade_phase = 4;
            kaze_ending_credits_frame_counter = 0;
            kaze_ending_fade_level = 0xff;
        }
        break;
    }
}

extern int kaze_heap_alloc(int, int);

/**
 * kaze_ending_graphics_init  (menu, 0x80063410)
 *
 * Ending screen graphics init: ResetGraph/GsInitGraph(320x240)/GsInit3D,
 * allocates two 24000-byte packet buffers (kaze_ending_packet_buffer_a/334)
 * and two 0x1000-entry OTs (kaze_ending_ot_buffer_a/32c), then camera
 * (kaze_ending_camera_init) and lighting (kaze_ending_lighting_init) init.
 */

void kaze_ending_graphics_init(void)
{
    extern int kaze_ending_packet_buffer_a;
    extern int kaze_ending_ot_base_ptrs;
    extern int kaze_ending_ot_headers;
    register int i __asm__("$4");
    register int ofs __asm__("$3");
    register int twelve __asm__("$5");
    register int *ptr __asm__("$16");
    int value;

    ResetGraph(0);
    GsInitGraph(0x140, 0xf0, 4, 1, 0);
    kaze_gs_def_disp_buff(0, 0, 0, 0xf0);
    GsInit3D();
    kaze_ending_packet_buffer_a = kaze_heap_alloc(0x5dc0, 1);
    kaze_ending_packet_buffer_b = kaze_heap_alloc(0x5dc0, 1);
    kaze_heap_alloc(0x1000, 4);

    /* cc1 schedules the kaze_ending_ot_buffer_a base load before the call arguments here. */
    __asm__ __volatile__(
        ".set\tnoreorder\n\t"
        "addiu\t$4,$zero,0x1000\n\t"
        "addiu\t$5,$zero,0x4\n\t"
        "lui\t$16,%%hi(kaze_ending_ot_buffer_a)\n\t"
        "addiu\t$16,$16,%%lo(kaze_ending_ot_buffer_a)\n\t"
        "jal\tkaze_heap_alloc\n\t"
        "sw\t$2,0($16)\n\t"
        "addu\t$4,$zero,$zero\n\t"
        "addiu\t$5,$zero,0xc\n\t"
        "addu\t$3,$zero,$zero\n\t"
        "sw\t$2,kaze_ending_ot_buffer_b\n\t"
        ".set\treorder"
        :
        :
        : "$2", "$3", "$4", "$5", "$16", "memory");
    do {
        *(int *)((int)&kaze_ending_ot_headers + ofs) = twelve;
        value = *ptr;
        ptr++;
        i++;
        *(int *)((int)&kaze_ending_ot_base_ptrs + ofs) = value;
        ofs += 0x14;
    } while (i < 2);
    kaze_vram_clear_all();
    kaze_ending_camera_init();
    kaze_ending_lighting_init();
}

/**
 * kaze_ending_buffers_free  (menu, 0x80063514)
 *
 * Frees the ending screen's packet/OT buffers
 * (kaze_ending_packet_buffer_a/334/328/32c) and texture/model allocations
 * kaze_ending_asset_load_buffer_ptr/754 via kaze_heap_free, then
 * kaze_xa_audio_stop_33a3c cleanup.
 */

void kaze_ending_buffers_free(void)
{
    extern void kaze_heap_free(int);
    extern int kaze_ending_asset_load_buffer_ptr;
    extern int kaze_ending_packet_buffer_a;
    kaze_heap_free(kaze_ending_packet_buffer_a);
    kaze_heap_free(kaze_ending_packet_buffer_b);
    kaze_heap_free(kaze_ending_ot_buffer_a);
    kaze_heap_free(kaze_ending_ot_buffer_b);
    kaze_heap_free(kaze_ending_asset_load_buffer_ptr);
    kaze_heap_free(kaze_ending_balloon_model_ptr);
    kaze_xa_audio_stop_33a3c();
}

extern void kaze_gs_model_pair_link_init_9604(int, int);

/**
 * kaze_ending_balloon_model_load  (menu, 0x80063594)
 *
 * Loads the balloon model 'BAL\\T\\BALL.MDL' into
 * kaze_ending_balloon_model_ptr via kaze_file_load_by_name and links its
 * TMD/texture sections through kaze_gs_model_pair_link_init_9604 for the
 * ending flythrough.
 */

void kaze_ending_balloon_model_load(void)
{
    extern int kaze_file_load_by_name(char *);
    int ptr;
    int a;
    int b;

    ptr = kaze_file_load_by_name(kaze_ending_balloon_model_path);
    a = *(int *)(ptr + 4);
    b = *(int *)(ptr + 8);
    kaze_ending_balloon_model_ptr = ptr;
    kaze_gs_model_pair_link_init_9604(ptr + a, ptr + b);
}

/**
 * kaze_ending_textures_load  (menu, 0x800635d8)
 *
 * Loads 'BAL\\T\\END.TXP' and uploads each TIM to VRAM
 * (kaze_tim_load_to_vram), then loads 'BAL\\T\\END2.TXP' and fills the
 * credit-page pointer table kaze_ending_credit_page_texture_table with its
 * sub-image offsets.
 */

void kaze_ending_textures_load(void)
{
    extern void kaze_heap_free(void *);
    extern void kaze_tim_load_to_vram(int, int, int, int, int);
    extern int *kaze_file_load_by_name(char *);
    extern int *kaze_ending_asset_load_buffer_ptr;
    volatile int frame_pad[4];
    int count;
    int i;
    int offset;
    int *base;
    int *loaded;
    int *out;

    kaze_ending_asset_load_buffer_ptr = kaze_file_load_by_name(kaze_ending_txp_path);
    count = kaze_ending_asset_load_buffer_ptr[0];
    i = 0;
    if (count > 0) {
        do {
            base = kaze_ending_asset_load_buffer_ptr;
            kaze_tim_load_to_vram((int)base + base[i + 1], -1, -1, -1, -1);
            i++;
        } while (i < count);
    }

    kaze_heap_free(kaze_ending_asset_load_buffer_ptr);
    count = (kaze_ending_asset_load_buffer_ptr = kaze_file_load_by_name(kaze_ending_txp2_path))[0];
    loaded = kaze_ending_asset_load_buffer_ptr;
    i = 0;
    if (count > 0) {
        out = kaze_ending_credit_page_texture_table;
        base = loaded;
        do {
            offset = base[1];
            base++;
            i++;
            *out = (int)loaded + offset;
            out++;
        } while (i < count);
    }
}

extern char kaze_ending_credit_title_tpage_prims[];
extern char kaze_ending_screen_move_prims[];
extern short kaze_ending_flame_angle_x;
extern void SetDrawMove(void *, RECT *, int, int);
extern void SetSprt(void *);
extern void SetPolyFT4(void *);
extern void SetSemiTrans(void *, int);
extern void SetShadeTex(void *, int);
extern u16 GetClut(int, int);
extern void GsInitCoordinate2(void *, void *);

/**
 * kaze_ending_prims_init  (menu, 0x800636d0)
 *
 * Builds the ending scene prims: DR_MOVE screen-copy blocks at
 * kaze_ending_screen_move_prims, backdrop SPRT/DR_TPAGE pairs, POLY_F4
 * kaze_ending_backdrop_poly, credit-logo and text SPRTs (CLUT 0x3c0), the
 * semi-trans flame POLY_FT4 at kaze_ending_flame_polys, and
 * GsInitCoordinate2.
 */

void kaze_ending_prims_init(void)
{
    extern void SetDrawTPage(void *, int, int, unsigned int);
    extern u16 GetTPage(int, int, int, int);
    RECT move_rect;
    u16 clut;
    int c160;

    {
        register int outer __asm__("$20");
        register char *base __asm__("$21");
        register short sx __asm__("$18");
        outer = 0;
        base = kaze_ending_screen_move_prims;
        sx = 0x140;
        do {
            register int i __asm__("$17");
            register short x __asm__("$22");
            register char *b __asm__("$19");
            i = 0;
            x = sx;
            b = base;
            do {
                register int s0 __asm__("$16");
                register char *pp __asm__("$4");
                register RECT *rp __asm__("$5");
                register int zz __asm__("$6");
                register int m __asm__("$7");
                int c;
                pp = (char *)(i * 0x30 + (int)b);
                rp = &move_rect;
                __asm__("" : "=r"(rp) : "0"(rp));
                zz = 0;
                __asm__("" : "=r"(zz) : "0"(zz));
                s0 = i + 1;
                m = (s0 & 1) * 0xf0;
                c = 0x140;
                move_rect.w = c;
                c = 0xF0;
                move_rect.x = x;
                move_rect.y = 0;
                move_rect.h = c;
                SetDrawMove(pp, rp, zz, m);
                i = s0;
            } while (i < 2);
            base += 0x18;
            outer += 1;
            sx += 0x140;
        } while (outer < 2);
    }

    {
        register int outer __asm__("$17");
        register int c80 __asm__("$19");
        register int ofs __asm__("$18");
        outer = 0;
        c80 = 0x80;
        ofs = 0;
        do {
            register char *p __asm__("$16");
            p = kaze_ending_backdrop_sprites + ofs;
            SetSprt(p);
            __asm__("addiu %0,$0,0xA0" : "=r"(c160) : "r"(p));
            p[4] = c80;
            p[5] = c80;
            p[6] = c80;
            p[0xc] = 0;
            p[0xd] = 0;
            *(short *)(p + 0x10) = c160;
            *(short *)(p + 0x12) = 0xf0;
            clut = GetTPage(2, 0, 0x140, 0x100);
            SetDrawTPage(kaze_ending_backdrop_tpage_prims + outer * 8, 0, 1, (unsigned int)clut);
            outer += 1;
            ofs += 0x14;
        } while (outer < 2);
    }

    SetPolyF4(kaze_ending_backdrop_poly);
    kaze_ending_backdrop_poly_y0 = -0x78;
    kaze_ending_backdrop_poly_y1 = -0x78;
    kaze_ending_backdrop_poly_r = 0;
    kaze_ending_backdrop_poly_g = 0;
    kaze_ending_backdrop_poly_b = 0;
    kaze_ending_backdrop_poly_x0 = 0;
    kaze_ending_backdrop_poly_x1 = c160;
    kaze_ending_backdrop_poly_x2 = 0;
    kaze_ending_backdrop_poly_y2 = 0x78;
    kaze_ending_backdrop_poly_x3 = c160;
    kaze_ending_backdrop_poly_y3 = 0x78;

    {
        register int outer __asm__("$20");
        char *tp;
        register int ofs __asm__("$21");
        register int c80 __asm__("$23");
        outer = 0;
        c80 = 0x80;
        tp = kaze_ending_credit_title_tpage_prims;
        ofs = 0;
        do {
            register int i __asm__("$17");
            int j;
            int oc;
            register char *p_00 __asm__("$19");
            register int idx __asm__("$16");
            i = 0;
            oc = ofs;
            p_00 = tp;
            idx = ofs;
            j = 0;
            do {
                char *q;
                {
                    register char *aa __asm__("$4");
                    aa = kaze_ending_credit_title_sprites + oc;
                    SetSprt((char *)(j + (int)aa));
                }
                q = (char *)(idx + (int)kaze_ending_credit_title_sprites);
                q[4] = c80;
                q[5] = c80;
                q[6] = c80;
                *(u16 *)(kaze_ending_credit_title_cluts + idx) = GetClut(0x3c0, 0x1fc);
                clut = GetTPage(0, 0, 0x200, 0x100);
                SetDrawTPage(p_00, 0, 0, (unsigned int)clut);
                p_00 += 0x10;
                idx += 0x28;
                i += 1;
                j += 0x28;
            } while (i < 2);
            tp += 8;
            outer += 1;
            ofs += 0x14;
        } while (outer < 2);
    }

    {
        register int outer __asm__("$20");
        register int c80b __asm__("$22");
        register int ofs __asm__("$21");
        outer = 0;
        c80b = 0x80;
        ofs = 0;
        do {
            register int i __asm__("$17");
            int j;
            int oc;
            register int o8 __asm__("$23");
            int k;
            register int idx __asm__("$16");
            i = 0;
            oc = ofs;
            __asm__("" : "=r"(oc) : "0"(oc));
            o8 = outer * 8;
            k = 0;
            idx = ofs;
            j = 0;
            do {
                char *q;
                {
                    register char *aa __asm__("$4");
                    aa = kaze_ending_credit_line_sprites + oc;
                    SetSprt((char *)(j + (int)aa));
                }
                q = (char *)(idx + (int)kaze_ending_credit_line_sprites);
                q[4] = c80b;
                q[5] = c80b;
                q[6] = c80b;
                *(short *)(q + 0x10) = c80b;
                *(short *)(q + 0x12) = 0x18;
                *(u16 *)(kaze_ending_credit_line_cluts + idx) = GetClut(0x3c0, 0x1fc);
                clut = GetTPage(0, 0, 0x200, 0x100);
                {
                    register int acc __asm__("$4");
                    acc = (int)kaze_ending_credit_line_tpage_prims;
                    acc = o8 + acc;
                    acc = k + acc;
                    SetDrawTPage((void *)acc, 0, 0, (unsigned int)clut);
                }
                k += 0x18;
                idx += 0x3c;
                i += 1;
                j += 0x3c;
            } while (i < 2);
            outer += 1;
            ofs += 0x14;
        } while (outer < 3);
    }

    {
        register int outer __asm__("$17");
        register int c10 __asm__("$19");
        register int ofs __asm__("$18");
        outer = 0;
        c10 = 0x10;
        ofs = 0;
        do {
            register char *p __asm__("$16");
            p = kaze_ending_flame_polys + ofs;
            SetPolyFT4(p);
            SetSemiTrans(p, 1);
            SetShadeTex(p, 1);
            p[0xc] = 0;
            p[0xd] = 0;
            p[0x14] = c10;
            p[0x15] = 0;
            p[0x1c] = 0;
            p[0x1d] = c10;
            p[0x24] = c10;
            p[0x25] = c10;
            *(u16 *)(kaze_ending_flame_poly_tpage + ofs) = GetTPage(0, 1, 0x280, 0x100);
            *(u16 *)(kaze_ending_flame_poly_clut + ofs) = GetClut(0x3c0, 0x1fb);
            ofs += 0x28;
            outer += 1;
        } while (outer < 2);
    }

    GsInitCoordinate2((void *)0, kaze_ending_flame_coord2);
    kaze_ending_flame_pos_x = 0;
    kaze_ending_flame_pos_y = 0;
    kaze_ending_flame_pos_z = 0;
    kaze_ending_flame_angle_x = 0;
    kaze_ending_flame_angle_y = 0;
    kaze_ending_flame_angle_z = 0;
}

/**
 * kaze_ending_fade_prims_init  (menu, 0x80063b1c)
 *
 * Initializes the double-buffered fade prims: semi-trans black POLY_F4 quads
 * at kaze_ending_fade_polys covering the screen and DR_TPAGE prims at
 * kaze_ending_fade_tpage_prims with blend mode from abr.
 */

void kaze_ending_fade_prims_init(int abr)
{
    extern void SetDrawTPage(void *, int, int, int);
    extern int GetTPage(int, int, int, int);
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
        SetDrawTPage(kaze_ending_fade_tpage_prims + i * 8, 0, 1, tpage & 0xFFFF);
        p = kaze_ending_fade_polys + prim_ofs;
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
 * kaze_ending_state_reset  (menu, 0x80063c2c)
 *
 * Resets the ending module state (kaze_ending_screen_state=1, fade 0xff,
 * counters/pages zeroed, music id -2), uploads 'BAL\\T\\FULL.TIM' to VRAM at
 * x=0x140, then builds prims via
 * kaze_ending_prims_init/kaze_ending_fade_prims_init.
 */

void kaze_ending_state_reset(void)
{
    extern int kaze_ending_asset_load_buffer_ptr;
    extern void kaze_heap_free(int arg0);
    extern void kaze_tim_load_to_vram(int arg0, int arg1, int arg2, int arg3, int arg4);
    extern int kaze_file_load_by_name(char *name);
    kaze_ending_screen_state = 1;
    kaze_ending_fade_level = 0xFF;
    kaze_ending_debug_camera_mode = 0;
    kaze_ending_credit_page_index = 0;
    kaze_ending_credits_substate = 0;
    kaze_ending_debug_toggle_flag = 0;
    kaze_ending_fade_phase = 0;
    kaze_ending_credits_frame_counter = 0;
    kaze_ending_balloon_bob_phase = 0;
    kaze_ending_music_track_id = -2;
    kaze_ending_asset_load_buffer_ptr = kaze_file_load_by_name(kaze_ending_full_tim_path);
    kaze_tim_load_to_vram(kaze_ending_asset_load_buffer_ptr, 0x140, 0, 0, 0);
    kaze_heap_free(kaze_ending_asset_load_buffer_ptr);
    kaze_ending_prims_init();
    kaze_ending_fade_prims_init(2);
    kaze_ending_backdrop_scroll_x = -0xA0;
    kaze_ending_backdrop_scroll_y = -0x78;
}
