#include "types.h"
#include "kaze_syms.h"
extern char kaze_opening_demo_model_path[];
extern char kaze_opening_demo_txp_path[];
extern u8 kaze_start_zone_excl_table_mode1[];
extern u8 kaze_start_zone_excl_table_mode2[];
extern u8 kaze_start_zone_excl_table_mode0[];
extern int D_8008DBDC;
extern unsigned char kaze_opening_camera_keyframe_alt_table;
extern s32 kaze_start_location_target_x_table[];
extern s32 kaze_start_location_target_z_table[];
extern u8 kaze_title_next_state;
extern int kaze_title_idle_timer;
extern int kaze_title_vsync_frame_delay;
extern int kaze_title_blink_phase;
extern u8 kaze_title_press_start_cycle_count;
extern int kaze_title_xa_track_id;
extern u8 kaze_title_start_skip_flag;
extern u8 kaze_opening_model_loaded_flag;
extern int kaze_opening_model_file_ptr;
extern int kaze_opening_model_data_ptr;
extern int kaze_opening_model_link_handle;
extern int kaze_opening_model_coord2;
extern int kaze_opening_model_matrix;
extern u16 kaze_opening_model_rot_y;
extern u16 kaze_opening_model_rot_z;
extern int kaze_opening_model_pos_x;
extern int kaze_opening_model_pos_y;
extern int kaze_opening_model_pos_z;
extern int kaze_opening_camera_keyframe_frames;
extern int kaze_opening_camera_keyframe_index;
extern int D_800AAF0C;
extern int kaze_map_heap_ptr_alt;
extern int kaze_map_ot_heap_alloc_a;
extern int kaze_map_ot_heap_alloc_b;
extern int kaze_map_wind_overlay_phase;
extern int kaze_map_wind_overlay_angle;
extern int kaze_map_target_marker_pulse_phase;
extern int kaze_map_scroll_arrow_pulse;
extern int kaze_gs_view_pos_x;
extern int kaze_gs_view_pos_z;
extern int kaze_gs_view_matrix;
extern int D_800B1A48;
extern int kaze_menu_vcount_timestamp;
extern int kaze_map_fade_counter;
extern int kaze_map_feedback_state;
extern int kaze_map_intro_counter;
extern int kaze_map_location_valid;
extern int kaze_map_selected_wind;
extern int kaze_map_state;
extern int kaze_staged_stage_variant;
extern void SetDispMask(int);
extern void SetSprt8(void *);
extern void exit(int);
extern int kaze_menu_camera_pos_z;
extern int kaze_menu_camera_scroll;
typedef struct {
    int ot;
    int pad[4];
} MenuTitlebarSlot;
typedef struct {
    int prim;
    int pad[4];
} Slot20;
typedef struct {
    char pad[20];
} OT20;
typedef struct {
    short x;
    short y;
    short w;
    short h;
} RECT;

/**
 * kaze_pressstart_sprite_setup  (render, 0x8004c51c)
 *
 * SetSprt/GetClut/GetTPage builds the press-start SPRT+DR_TPAGE at
 * kaze_title_pressstart_sprite_buffer for both buffers.
 */

void kaze_pressstart_sprite_setup(void)
{
    extern int kaze_title_pressstart_sprite_buffer[];
    extern void SetDrawTPage(void *prim, int dfe, int dtd, unsigned int tpage);
    extern u16 GetTPage(int tp, int abr, int x, int y);
    extern u16 GetClut(int x, int y);
    extern void SetShadeTex(void *prim, int on);
    extern void SetSprt(void *sprt);
    u16 attr;
    int *sprt_ptr;
    int i;
    int tmp;

    i = 0;
    tmp = 0x80;
    sprt_ptr = kaze_title_pressstart_sprite_buffer;
    do {
        i = i + 1;
        SetSprt((void *)*sprt_ptr);
        SetShadeTex((void *)*sprt_ptr, 1);
        *(short *)(*sprt_ptr + 8) = -0x40;
        *(short *)(*sprt_ptr + 10) = -0x28;
        *(short *)(*sprt_ptr + 0x10) = tmp;
        *(short *)(*sprt_ptr + 0x12) = 0x50;
        *(u8 *)(*sprt_ptr + 0xC) = tmp;
        *(u8 *)(*sprt_ptr + 0xD) = 0xA0;
        attr = GetClut(0x3C0, 0x1FD);
        *(u16 *)(*sprt_ptr + 0xE) = attr;
        attr = GetTPage(0, 0, 0x3C0, 0x100);
        SetDrawTPage((void *)(*sprt_ptr + 0x14), 0, 1, attr);
        sprt_ptr = sprt_ptr + 1;
    } while (i < 2);
}

/**
 * kaze_pressstart_sprite_addprim  (render, 0x8004c604)
 *
 * AddPrims the press-start SPRT and its DR_TPAGE
 * (kaze_title_pressstart_sprite_buffer[+0/+0x14]) into the OT.
 */

void kaze_pressstart_sprite_addprim(int idx)
{
    extern void AddPrim(int, char *);
    extern char *kaze_title_pressstart_sprite_buffer[];
    extern MenuTitlebarSlot kaze_menu_gs_ot_buf0_org[];
    AddPrim(kaze_menu_gs_ot_buf0_org[idx].ot, kaze_title_pressstart_sprite_buffer[idx]);
    AddPrim(kaze_menu_gs_ot_buf0_org[idx].ot, kaze_title_pressstart_sprite_buffer[idx] + 0x14);
    return;
}

void kaze_title_state2_init();
void kaze_title_state2_update();
void kaze_title_state2_advance();
void kaze_title_state2_fade_draw();

/**
 * kaze_title_state2_substate  (menu, 0x8004c678)
 *
 * Title state-2 substate machine (kaze_title_start_flow_state):
 * kaze_title_state2_init/c7ac/c948 plus render kaze_title_state2_fade_draw.
 */

void kaze_title_state2_substate(int arg0)
{
    extern u8 kaze_title_start_flow_state;
    switch (kaze_title_start_flow_state) {
    case 0:
        kaze_title_state2_init();
        break;
    case 1:
        kaze_title_state2_update();
        break;
    case 0xff:
        kaze_title_state2_advance();
        break;
    }
    if (kaze_title_start_flow_state == 1) {
        kaze_title_state2_fade_draw(arg0);
    }
}

void kaze_title_sprite_setup_b();
extern void kaze_drmove_pair_setup_a064(int, int);

/**
 * kaze_title_state2_init  (menu, 0x8004c720)
 *
 * Inits state-2 fade vars, calls kaze_title_sprite_setup_b sprite setup and
 * kaze_drmove_pair_setup_a064 buffer-copy, VSyncs.
 */

void kaze_title_state2_init(void)
{
    extern void VSync(int);
    extern s16 kaze_start_map_transition_alpha;
    extern u8 kaze_title_fade_started_flag;
    extern u8 kaze_title_clear_color_r;
    extern u8 kaze_title_clear_color_g;
    extern u8 kaze_title_clear_color_b;
    extern u8 kaze_title_start_flow_state;
    kaze_title_start_flow_state = 1;
    D_8008DBDC = 0;
    kaze_title_idle_timer = 0;
    kaze_title_vsync_frame_delay = 0;
    kaze_title_blink_phase = 0;
    kaze_title_clear_color_b = 0;
    kaze_title_clear_color_g = 0;
    kaze_title_clear_color_r = 0;
    kaze_title_fade_started_flag = 0;
    kaze_start_map_transition_alpha = 0xFF;
    kaze_title_sprite_setup_b();
    kaze_drmove_pair_setup_a064(0x2C0, 0);
    VSync(0x1E);
}

/**
 * kaze_title_state2_update  (menu, 0x8004c7ac)
 *
 * State-2 timer/START(0x800) handling and fade sequencing via
 * kaze_title_idle_timer/4c/54; sets exit kaze_title_start_skip_flag.
 */

void kaze_title_state2_update(void)
{
    extern int kaze_input_current_c;
    extern int kaze_input_edge;
    extern s16 kaze_start_map_transition_alpha;
    extern u8 kaze_title_fade_blend_mode;
    extern u8 kaze_title_fade_started_flag;
    extern u8 kaze_title_clear_color_b;
    extern u8 kaze_title_clear_color_g;
    extern u8 kaze_title_clear_color_r;
    extern u8 kaze_title_start_flow_state;
    if ((kaze_title_idle_timer >= 0x259) && (kaze_title_fade_started_flag == 0)) {
        u8 flag;

        flag = kaze_title_press_start_cycle_count;
        kaze_title_fade_started_flag = 1;
        kaze_title_fade_blend_mode = 2;
        kaze_start_map_transition_alpha = 0;
        if (flag != 0) {
            kaze_title_next_state = 0xff;
        } else {
            kaze_title_next_state = 3;
        }
    }
    if (((kaze_input_edge & 0x800) != 0) && (kaze_title_fade_started_flag == 0)) {
        kaze_title_fade_started_flag = 1;
        kaze_title_fade_blend_mode = 2;
        kaze_start_map_transition_alpha = 0;
        kaze_title_next_state = 0xff;
        kaze_title_start_skip_flag = 1;
    }
    if ((kaze_title_fade_started_flag == 1) && (kaze_start_map_transition_alpha >= 0x100)) {
        kaze_title_start_flow_state = 0xff;
        kaze_title_clear_color_r = 0;
        kaze_title_clear_color_g = 0;
        kaze_title_clear_color_b = 0;
    }
    if (kaze_input_current_c != 0) {
        kaze_title_idle_timer = 0;
    } else {
        kaze_title_idle_timer = kaze_title_idle_timer + 1;
    }
    kaze_title_blink_phase = (kaze_title_blink_phase + 1) % 0x5a;
}

extern u8 kaze_title_screen_state;

/**
 * kaze_title_state2_advance  (menu, 0x8004c948)
 *
 * Advances title state kaze_title_screen_state=kaze_title_next_state and
 * increments kaze_title_press_start_cycle_count.
 */

void kaze_title_state2_advance(void)
{
    extern u8 kaze_title_start_flow_state;
    u8 n = kaze_title_press_start_cycle_count;

    kaze_title_start_flow_state = 0;
    kaze_title_screen_state = kaze_title_next_state;
    kaze_title_press_start_cycle_count = n + 1;
}

void kaze_title_sprite_blink_addprim();
extern void kaze_drmove_addprim_a0ec(int, int);

/**
 * kaze_title_state2_fade_draw  (menu, 0x8004c97c)
 *
 * Fades sprite alpha kaze_start_map_transition_alpha and AddPrims
 * (kaze_quad_addprim_texpage/a0ec) the title logo for state 2.
 */

void kaze_title_state2_fade_draw(int arg)
{
    extern u8 kaze_title_fade_started_flag;
    extern u8 kaze_title_fade_blend_mode;
    extern u16 kaze_start_map_transition_alpha;
    u16 a;

    if (kaze_start_map_transition_alpha < 0x100) {
        a = kaze_start_map_transition_alpha & 0xff;
        kaze_quad_addprim_texpage(arg, a, a, a, kaze_title_fade_blend_mode);
        if (kaze_title_fade_started_flag == 0) {
            kaze_start_map_transition_alpha -= 0x10;
        } else {
            kaze_start_map_transition_alpha += 0x10;
        }
    } else {
        kaze_title_sprite_blink_addprim(arg);
    }
    kaze_drmove_addprim_a0ec(arg, 0xfff);
}

/**
 * kaze_title_sprite_setup_b  (render, 0x8004ca34)
 *
 * SetSprt/GetClut/GetTPage builds a second SPRT+DR_TPAGE (offset
 * +0x1c/+0x30) at kaze_title_pressstart_sprite_buffer.
 */

void kaze_title_sprite_setup_b(void)
{
    extern int kaze_title_pressstart_sprite_buffer[];
    extern void SetDrawTPage(void *prim, int dfe, int dtd, unsigned int tpage);
    extern u16 GetTPage(int tp, int abr, int x, int y);
    extern u16 GetClut(int x, int y);
    extern void SetShadeTex(void *prim, int on);
    extern void SetSprt(void *sprt);
    u16 clut;
    int *slot;
    int i;

    i = 0;
    slot = kaze_title_pressstart_sprite_buffer;
    do {
        i = i + 1;
        SetSprt((void *)(*slot + 0x1C));
        SetShadeTex((void *)(*slot + 0x1C), 1);
        *(short *)(*slot + 0x24) = -0x28;
        *(short *)(*slot + 0x26) = 0x28;
        *(short *)(*slot + 0x2C) = 0x50;
        *(short *)(*slot + 0x2E) = 0x10;
        *(u8 *)(*slot + 0x28) = 0;
        *(u8 *)(*slot + 0x29) = 0x90;
        clut = GetClut(0x3C0, 0x1FF);
        *(u16 *)(*slot + 0x2A) = clut;
        clut = GetTPage(0, 0, 0x3C0, 0x100);
        SetDrawTPage((void *)(*slot + 0x30), 0, 1, clut);
        slot = slot + 1;
    } while (i < 2);
}

/**
 * kaze_title_sprite_blink_addprim  (render, 0x8004cb1c)
 *
 * AddPrims the +0x1c/+0x30 sprite while blink phase kaze_title_blink_phase <
 * 0x3c.
 */

void kaze_title_sprite_blink_addprim(int i)
{
    extern void AddPrim(void *, void *);
    extern int kaze_title_pressstart_sprite_buffer[];
    extern Slot20 kaze_menu_gs_ot_buf0_org[];
    if (kaze_title_blink_phase < 0x3C) {
        AddPrim((void *)kaze_menu_gs_ot_buf0_org[i].prim, (void *)(kaze_title_pressstart_sprite_buffer[i] + 0x1C));
        AddPrim((void *)kaze_menu_gs_ot_buf0_org[i].prim, (void *)(kaze_title_pressstart_sprite_buffer[i] + 0x30));
    }
}

void kaze_opening_demo_init();
void kaze_opening_demo_update();
void kaze_opening_demo_exit();
void kaze_opening_demo_draw();

/**
 * kaze_opening_state_dispatch  (menu, 0x8004cb9c)
 *
 * Opening/demo state-3 substate machine: kaze_opening_demo_init/cea8/cd54
 * plus render kaze_opening_demo_draw.
 */

void kaze_opening_state_dispatch(int arg)
{
    extern unsigned char kaze_title_start_flow_state;
    register int saved_arg __asm__("$16");
    int state;

    saved_arg = arg;
    state = kaze_title_start_flow_state;
    if (state == 1) {
        goto update;
    }
    if (state >= 2) {
        goto high_state;
    }
    if (state == 0) {
        goto init;
    }
    goto done;

high_state:
    if (state == 0xff) {
        goto exit;
    }
    goto done;

init:
    kaze_opening_demo_init();

update:
    kaze_opening_demo_update();

    goto done;

exit:
    kaze_opening_demo_exit();

done:
    if (kaze_title_start_flow_state == 1) {
        kaze_opening_demo_draw(saved_arg);
    }
}

extern s16 kaze_menu_camera_rot_x;
extern s16 kaze_menu_camera_rot_y;
extern s16 kaze_menu_camera_rot_z;
extern int kaze_menu_camera_pos_x;
extern int kaze_file_load_by_name(char *);
void kaze_opening_model_link();

/**
 * kaze_opening_demo_init  (menu, 0x8004cc3c)
 *
 * Loads BAL\K\OPN.MDL/OPN.TXP opening model+texture, resets camera/altitude
 * vars, starts music (kaze_cd_setmode_vsync_341c).
 */

void kaze_opening_demo_init(void)
{
    extern void VSync(int);
    extern u8 kaze_title_fade_started_flag;
    extern s16 kaze_start_map_transition_alpha;
    extern u8 kaze_title_clear_color_b;
    extern u8 kaze_title_clear_color_g;
    extern u8 kaze_title_clear_color_r;
    extern u8 kaze_title_start_flow_state;
    if (kaze_opening_model_loaded_flag == 0) {
        kaze_opening_model_file_ptr = kaze_file_load_by_name(kaze_opening_demo_model_path);
        kaze_opening_model_link();
        kaze_txp_archive_load_vram(kaze_opening_demo_txp_path);
        kaze_opening_model_loaded_flag = 1;
    } else {
        VSync(0x1E);
    }
    kaze_title_start_flow_state = 1;
    kaze_title_vsync_frame_delay = 2;
    kaze_title_clear_color_r = 0x80;
    kaze_title_clear_color_g = 0x80;
    kaze_title_clear_color_b = 0xFF;
    kaze_start_map_transition_alpha = 0xFF;
    D_8008DBDC = 0;
    kaze_title_idle_timer = 0;
    kaze_opening_camera_keyframe_index = 0;
    kaze_title_fade_started_flag = 0;
    kaze_menu_camera_rot_x = 0;
    kaze_menu_camera_rot_z = 0;
    kaze_menu_camera_rot_y = 0;
    kaze_menu_camera_pos_x = 0;
    kaze_menu_camera_scroll = 0;
    kaze_menu_camera_pos_z = 0;
    kaze_title_xa_track_id = 0xE;
    kaze_cd_setmode_vsync_341c();
}

void kaze_opening_camera_keyframe();
void kaze_opening_camera_view();

/**
 * kaze_opening_demo_update  (menu, 0x8004cd54)
 *
 * Opening cinematic update: altitude kaze_menu_camera_scroll end and START|X
 * (0x840) skip, camera kaze_opening_camera_keyframe/d2f4, fade
 * kaze_start_map_transition_alpha.
 */

void kaze_opening_demo_update(void)
{
    extern unsigned char kaze_title_clear_color_r;
    extern unsigned char kaze_title_clear_color_g;
    extern unsigned char kaze_title_clear_color_b;
    extern unsigned char kaze_title_start_flow_state;
    extern short kaze_start_map_transition_alpha;
    extern unsigned char kaze_title_fade_blend_mode;
    extern unsigned char kaze_title_fade_started_flag;
    extern int kaze_input_edge;
    if (kaze_menu_camera_scroll < -20000 && kaze_title_fade_started_flag == 0) {
        kaze_title_fade_started_flag = 1;
        kaze_title_fade_blend_mode = 1;
        kaze_start_map_transition_alpha = 0;
        kaze_title_xa_track_id = -1;
    }
    if ((kaze_input_edge & 0x840) != 0 && kaze_title_fade_started_flag == 0 && kaze_start_map_transition_alpha < 0) {
        kaze_title_fade_started_flag = 1;
        kaze_title_fade_blend_mode = 2;
        kaze_start_map_transition_alpha = 0;
        kaze_title_xa_track_id = -1;
    }
    kaze_opening_camera_keyframe();
    kaze_opening_camera_view();
    if (kaze_title_fade_started_flag == 1 && kaze_start_map_transition_alpha >= 0x100) {
        kaze_title_start_flow_state = 0xff;
        if (kaze_title_fade_blend_mode == 1) {
            kaze_title_clear_color_b = 0xff;
            kaze_title_clear_color_g = 0xff;
            kaze_title_clear_color_r = 0xff;
        } else {
            kaze_title_clear_color_b = 0;
            kaze_title_clear_color_g = 0;
            kaze_title_clear_color_r = 0;
        }
    }
}

/**
 * kaze_opening_demo_exit  (menu, 0x8004cea8)
 *
 * Exits the opening cinematic to state 2 and stops music
 * (kaze_xa_audio_stop_33a3c).
 */

void kaze_opening_demo_exit(void)
{
    extern u8 kaze_title_start_flow_state;
    kaze_title_screen_state = 2;
    kaze_title_start_flow_state = 0;
    kaze_title_xa_track_id = -2;
    kaze_xa_audio_stop_33a3c();
}

void kaze_opening_model_sort();
extern void kaze_title_model_sort_dispatch(int, void *);

/**
 * kaze_opening_demo_draw  (menu, 0x8004cee8)
 *
 * Fades the opening overlay sprite (kaze_start_map_transition_alpha) and
 * sorts the opening model via
 * kaze_opening_model_sort/kaze_title_model_sort_dispatch.
 */

void kaze_opening_demo_draw(int idx)
{
    extern char kaze_menu_gs_ot_buf[];
    extern unsigned short kaze_start_map_transition_alpha;
    extern unsigned char kaze_title_fade_blend_mode;
    extern unsigned char kaze_title_fade_started_flag;
    unsigned int alpha;

    alpha = kaze_start_map_transition_alpha;
    if (alpha < 0x100) {
        alpha &= 0xFF;
        kaze_quad_addprim_texpage(idx, alpha, alpha, alpha, kaze_title_fade_blend_mode);
        if (kaze_title_fade_started_flag == 0) {
            kaze_start_map_transition_alpha = kaze_start_map_transition_alpha - 8;
        } else {
            kaze_start_map_transition_alpha = kaze_start_map_transition_alpha + 8;
        }
    }

    kaze_opening_model_sort(idx);
    kaze_title_model_sort_dispatch(idx, kaze_menu_gs_ot_buf + idx * 0x14);
}

extern void GsMapModelingData(int *arg0);
extern void GsInitCoordinate2(int arg0, int *arg1);
extern void GsLinkObject4(int *arg0, int *arg1, int arg2);
extern void kaze_gs_model_pair_link_init_9604(int arg0, int arg1);

/**
 * kaze_opening_model_link  (render, 0x8004cf9c)
 *
 * GsMapModelingData/GsInitCoordinate2/GsLinkObject4 sets up the opening
 * balloon model kaze_opening_model_file_ptr for rendering.
 */

void kaze_opening_model_link(void)
{
    extern u16 kaze_opening_model_rot_x;
    extern int *kaze_opening_model_coord_ptr;
    int *p;
    int *q;

    p = (int *)(kaze_opening_model_file_ptr + *(int *)(kaze_opening_model_file_ptr + 4));
    kaze_opening_model_data_ptr = (int)p;
    p++;
    GsMapModelingData(p++);
    p++;
    GsInitCoordinate2(0, &kaze_opening_model_coord2);
    q = &kaze_opening_model_link_handle;
    GsLinkObject4(p, q, 0);
    *q = 0;
    kaze_opening_model_coord_ptr = &kaze_opening_model_coord2;
    kaze_opening_model_rot_x = 0;
    kaze_opening_model_rot_z = 0;
    kaze_opening_model_rot_y = 0;
    kaze_opening_model_pos_x = 0;
    kaze_opening_model_pos_y = 0;
    kaze_opening_model_pos_z = 0;
    kaze_gs_model_pair_link_init_9604(kaze_opening_model_file_ptr + *(int *)(kaze_opening_model_file_ptr + 8),
                                      kaze_opening_model_file_ptr + *(int *)(kaze_opening_model_file_ptr + 0xC));
}

extern void RotMatrix(int *, int *);
extern void GsGetLws(int, void *, void *);
extern void GsSetLightMatrix(void *);
extern void GsSetLsMatrix(void *);
extern void GsSortObject4(int *, OT20 *, int, void *);

/**
 * kaze_opening_model_sort  (render, 0x8004d078)
 *
 * RotMatrix/GsGetLws/GsSortObject4 sorts the opening model
 * kaze_opening_model_link_handle into the frame OT.
 */

void kaze_opening_model_sort(int arg)
{
    extern void kaze_trans_matrix(int *, int *);
    extern OT20 kaze_menu_gs_ot_buf[];
    extern int kaze_opening_model_coord_ptr;
    extern int kaze_opening_model_rot_x;
    int *m;
    int *h;

    m = &kaze_opening_model_matrix;
    RotMatrix(&kaze_opening_model_rot_x, m);
    kaze_trans_matrix(m, &kaze_opening_model_pos_x);
    h = &kaze_opening_model_coord_ptr;
    GsGetLws(*h, (void *)0x1f800000, (void *)0x1f800020);
    GsSetLightMatrix((void *)0x1f800000);
    GsSetLsMatrix((void *)0x1f800020);
    GsSortObject4(h - 1, &kaze_menu_gs_ot_buf[arg], 2, (void *)0x1f800000);
}

/*
 * kaze_opening_camera_keyframe (0x8004d124)
 *
 * Advances the opening-cinematic camera along the keyframe table at
 * kaze_opening_camera_keyframe_alt_table (16-byte records: int altitude;
 * u16 yaw; u16 pitch; s16 dx; s16 dy; s16 dz) driven by the descending
 * altitude kaze_menu_camera_scroll. When the current keyframe's altitude
 * threshold is reached it computes the per-frame position/angle deltas
 * toward the next keyframe and advances the keyframe index
 * kaze_opening_camera_keyframe_index.
 *
 * Match notes (gcc 2.7.2-psx -O2):
 *   - The index/altitude temporaries are pinned to the caller-save registers
 *     the original used ($v1/$a0/$t0/$t1/$t2) so the stack-frame adjust falls
 *     into the skip-branch delay slot and the two 16*index offsets stay live
 *     without extra copies.
 *   - The yaw/pitch "current" operands are pinned so temp_a1/temp_a3 land in
 *     $a1/$a3 and the freed altitude register $a0 is reused for yaw_cur.
 *   - One zero-code tied barrier keeps the "next" yaw load ahead of the "cur"
 *     yaw load: old gcc otherwise schedules the two $at-serialized halfword
 *     loads in the opposite order. It emits no instructions.
 */
extern int kaze_menu_camera_coord_flags;

/**
 * kaze_opening_camera_keyframe  (menu, 0x8004d124)
 *
 * Advances the opening camera keyframe from table
 * kaze_opening_camera_keyframe_alt_table by altitude
 * kaze_menu_camera_scroll, computing per-frame position/angle deltas.
 */

void kaze_opening_camera_keyframe(void)
{
    extern short kaze_opening_camera_dz_delta;
    extern short kaze_opening_camera_dy_delta;
    extern short kaze_opening_camera_dx_delta;
    extern short kaze_opening_camera_pitch_delta;
    extern short kaze_opening_camera_yaw_delta;
    extern unsigned char kaze_opening_camera_keyframe_dz_table;
    extern unsigned char kaze_opening_camera_keyframe_dy_table;
    extern unsigned char kaze_opening_camera_keyframe_dx_table;
    extern unsigned char kaze_opening_camera_keyframe_pitch_table;
    extern unsigned char kaze_opening_camera_keyframe_yaw_table;
    register unsigned short yaw_cur asm("$4");
    register unsigned short pitch_cur asm("$6");
    register unsigned short yaw_next asm("$5");
    register int index asm("$3");
    register int alt asm("$4");
    register int next_ofs asm("$8");
    register int cur_ofs asm("$9");
    register int next_index asm("$10");
    short yaw_delta;
    unsigned short pitch_next;
    short pitch_delta;
    int frac;

    index = kaze_opening_camera_keyframe_index;
    alt = kaze_menu_camera_scroll;
    cur_ofs = index * 0x10;
    if (*(int *)((&kaze_opening_camera_keyframe_alt_table) + cur_ofs) >= alt) {
        next_index = index + 1;
        next_ofs = next_index * 0x10;
        frac = alt - *(int *)((&kaze_opening_camera_keyframe_alt_table) + next_ofs);
        if (frac < 0) {
            frac += 0xF;
        }
        yaw_next = *(unsigned short *)((&kaze_opening_camera_keyframe_yaw_table) + next_ofs);
        __asm__ __volatile__("" : : "r"(yaw_next)); /* keep next before cur load */
        yaw_cur = *(unsigned short *)((&kaze_opening_camera_keyframe_yaw_table) + cur_ofs);
        yaw_delta = yaw_next - yaw_cur;
        pitch_next = *(unsigned short *)((&kaze_opening_camera_keyframe_pitch_table) + next_ofs);
        kaze_opening_camera_keyframe_frames = frac >> 4;
        pitch_cur = *(unsigned short *)((&kaze_opening_camera_keyframe_pitch_table) + cur_ofs);
        pitch_delta = pitch_next - pitch_cur;
        kaze_opening_camera_dx_delta = *(short *)((&kaze_opening_camera_keyframe_dx_table) + next_ofs) - *(short *)((&kaze_opening_camera_keyframe_dx_table) + cur_ofs);
        kaze_opening_camera_dy_delta = *(short *)((&kaze_opening_camera_keyframe_dy_table) + next_ofs) - *(short *)((&kaze_opening_camera_keyframe_dy_table) + cur_ofs);
        kaze_opening_camera_dz_delta = *(short *)((&kaze_opening_camera_keyframe_dz_table) + next_ofs) - *(short *)((&kaze_opening_camera_keyframe_dz_table) + cur_ofs);
        kaze_opening_camera_yaw_delta = yaw_delta;
        kaze_opening_camera_pitch_delta = pitch_delta;
        if (yaw_delta >= 0x800) {
            kaze_opening_camera_yaw_delta = yaw_delta - 0x1000;
        }
        if (kaze_opening_camera_yaw_delta < -0x7FF) {
            kaze_opening_camera_yaw_delta += 0x1000;
        }
        if (pitch_delta >= 0x800) {
            kaze_opening_camera_pitch_delta = pitch_delta - 0x1000;
        }
        if (kaze_opening_camera_pitch_delta < -0x7FF) {
            kaze_opening_camera_pitch_delta += 0x1000;
        }
        kaze_opening_camera_keyframe_index = next_index;
        kaze_title_idle_timer = 0;
    }
    {
        int *p = &kaze_menu_camera_scroll;
        kaze_menu_camera_coord_flags = 0;
        *p -= 0x10;
    }
}

extern int kaze_gs_view_pos_y[2];
extern int kaze_gs_view_coord2;
extern s16 kaze_gs_view_rot_x[2];
extern s16 kaze_gs_view_rot_y;
extern int kaze_menu_gs_view2;
extern int rcos(int);
extern int RotMatrixZYX(void *, void *);
extern int GsSetView2(void *);

/**
 * kaze_opening_camera_view  (render, 0x8004d2f4)
 *
 * Interpolates opening camera position/angles with rsin/rcos, builds view
 * matrix via RotMatrixZYX and GsSetView2.
 */

void kaze_opening_camera_view(void)
{
    extern int kaze_trans_matrix(void *, void *);
    extern int rsin(int);
    extern s16 kaze_opening_camera_dz_delta;
    extern s16 kaze_opening_camera_dy_delta;
    extern s16 kaze_opening_camera_dx_delta;
    extern u16 kaze_opening_camera_pitch_delta;
    extern u16 kaze_opening_camera_yaw_delta;
    extern u8 kaze_opening_camera_keyframe_dz_table;
    extern u8 kaze_opening_camera_keyframe_dy_table;
    extern u8 kaze_opening_camera_keyframe_dx_table;
    extern u8 kaze_opening_camera_keyframe_pitch_table;
    extern u8 kaze_opening_camera_keyframe_yaw_table;
  s16 round_bias;
  int prod;
  int kf_ofs;
  int sum_tmp;
  int unused0;
  uint yaw_masked;
  int amp_x;
  int scratch;
  int res;
  int q0;
  int q1;
  int q2;
  int fin;
  int half;
  int hv;
  int idx1;
  int raw;
  int *pb;
  int *pc;
  int idx2;
  u16 yaw;
  int *ps;
  ps = (int *) 0x1f800000;
  if (kaze_title_idle_timer < kaze_opening_camera_keyframe_frames)
  {
    scratch = (kaze_title_idle_timer << 0xb) / kaze_opening_camera_keyframe_frames;
    if (scratch < 0x400)
    {
      scratch += 0x400;
      res = rsin(scratch);
      amp_x = ((s16) kaze_opening_camera_yaw_delta) / 2;
      prod = amp_x * res;
      kf_ofs = (kaze_opening_camera_keyframe_index - 1) * 0x10;
      if (prod < 0)
      {
        prod += 0xfff;
      }
      kaze_gs_view_rot_x[0] = (*((s16 *) ((&kaze_opening_camera_keyframe_yaw_table) + kf_ofs))) + (amp_x - (prod >> 0xc));
      res = rsin(scratch);
      half = ((s16) kaze_opening_camera_pitch_delta) / 2;
      prod = half * res;
      kf_ofs = (kaze_opening_camera_keyframe_index + -1) * 0x10;
      if (prod < 0)
      {
        prod += 0xfff;
      }
      res = prod >> 0xc;
      prod = *((u16 *) ((&kaze_opening_camera_keyframe_pitch_table) + kf_ofs));
      res = half - res;
    }
    else
    {
      scratch -= 0x400;
      res = rsin(scratch);
      amp_x = ((s16) kaze_opening_camera_yaw_delta) / 2;
      prod = amp_x * res;
      kf_ofs = (kaze_opening_camera_keyframe_index - 1) * 0x10;
      if (prod < 0)
      {
        prod += 0xfff;
      }
      kaze_gs_view_rot_x[0] = (*((s16 *) ((&kaze_opening_camera_keyframe_yaw_table) + kf_ofs))) + (amp_x + (prod >> 0xc));
      res = rsin(scratch);
      half = ((s16) kaze_opening_camera_pitch_delta) / 2;
      prod = half * res;
      kf_ofs = (kaze_opening_camera_keyframe_index - 1) * 0x10;
      if (prod < 0)
      {
        prod += 0xfff;
      }
      res = prod >> 0xc;
      prod = *((u16 *) ((&kaze_opening_camera_keyframe_pitch_table) + kf_ofs));
      sum_tmp = half + res;
      res = sum_tmp;
    }
    prod += res;
    kaze_gs_view_rot_y = prod;
__asm__ volatile("");
    yaw_masked = (*((u16 *) kaze_gs_view_rot_x)) & 0xfff;
    *((u16 *) kaze_gs_view_rot_x) = yaw_masked;
    ((u16 *) kaze_gs_view_rot_x)[1] = ((u16 *) kaze_gs_view_rot_x)[1] & 0xfff;
    prod = rsin(yaw_masked);
    prod = prod * (-0xc00);
    pb = kaze_gs_view_pos_y;
    if (prod < 0)
    {
      prod += 0xfff;
    }
    pb[0] = kaze_menu_camera_scroll - (prod >> 0xc);
    prod = rcos(kaze_gs_view_rot_x[0]);
    prod = prod * (-0xc00);
    if (prod < 0)
    {
      prod += 0xfff;
    }
    scratch = prod >> 0xc;
    res = rsin(kaze_gs_view_rot_x[1]);
    raw = scratch * res;
    if (raw < 0)
    {
      raw += 0xfff;
    }
    kaze_gs_view_pos_x = raw >> 0xc;
    res = rcos(kaze_gs_view_rot_x[1]);
    hv = scratch * res;
    if (hv < 0)
    {
      if (kaze_opening_camera_yaw_delta)
      {
        hv += 0xfff;
      }
      else
      {
        hv += 0xfff;
      }
    }
    q0 = (kaze_opening_camera_dx_delta * kaze_title_idle_timer) / kaze_opening_camera_keyframe_frames;
    q1 = (kaze_opening_camera_dy_delta * kaze_title_idle_timer) / kaze_opening_camera_keyframe_frames;
    q2 = (kaze_opening_camera_dz_delta * kaze_title_idle_timer) / kaze_opening_camera_keyframe_frames;
    res = hv >> 0xc;
    pb[1] = res;
    kaze_title_idle_timer = kaze_title_idle_timer + 1;
    idx1 = (kaze_opening_camera_keyframe_index - 1) * 0x10;
    ps[0] = kaze_gs_view_pos_x + ((*((s16 *) ((&kaze_opening_camera_keyframe_dx_table) + idx1))) + q0);
    ps[1] = pb[0] + ((*((s16 *) ((&kaze_opening_camera_keyframe_dy_table) + idx1))) + q1);
    ps[2] = pb[1] + ((*((s16 *) ((&kaze_opening_camera_keyframe_dz_table) + idx1))) + q2);
  }
  else
  {
    yaw = *((u16 *) ((&kaze_opening_camera_keyframe_yaw_table) + (kaze_opening_camera_keyframe_index * 0x10)));
    *((u16 *) kaze_gs_view_rot_x) = yaw;
    ((u16 *) kaze_gs_view_rot_x)[1] = *((u16 *) ((&kaze_opening_camera_keyframe_pitch_table) + (kaze_opening_camera_keyframe_index * 0x10)));
    res = rcos((s16) yaw);
    res = res * (-0xc00);
    if (res < 0)
    {
      res += 0xfff;
    }
    scratch = res >> 0xc;
    prod = rsin(kaze_gs_view_rot_x[0]);
    prod = prod * (-0xc00);
    pc = kaze_gs_view_pos_y;
    if (prod < 0)
    {
      prod += 0xfff;
    }
    pc[0] = kaze_menu_camera_scroll - (prod >> 0xc);
    res = rsin(kaze_gs_view_rot_x[1]);
    raw = scratch * res;
    if (raw < 0)
    {
      raw += 0xfff;
    }
    kaze_gs_view_pos_x = raw >> 0xc;
    res = rcos(kaze_gs_view_rot_x[1]);
    raw = scratch * res;
    if (raw < 0)
    {
      round_bias = 0xfff;
      raw += round_bias;
    }
    fin = raw >> 0xc;
    kaze_gs_view_pos_z = fin;
    idx2 = kaze_opening_camera_keyframe_index * 0x10;
    ps[0] = kaze_gs_view_pos_x + (*((s16 *) ((&kaze_opening_camera_keyframe_dx_table) + idx2)));
    *((int *) 0x1f800004) = pc[0] + (*((s16 *) ((&kaze_opening_camera_keyframe_dy_table) + idx2)));
    *((int *) 0x1f800008) = fin + (*((s16 *) ((&kaze_opening_camera_keyframe_dz_table) + idx2)));
  }
  RotMatrixZYX(kaze_gs_view_rot_x, &kaze_gs_view_matrix);
  kaze_trans_matrix(&kaze_gs_view_matrix, ps);
  kaze_gs_view_coord2 = 0;
  GsSetView2(&kaze_menu_gs_view2);
  return;
}

extern void kaze_play_sound_effect(int, int, int, int);
void kaze_start_map_screen_init();
extern void GsInitVcount(void);
extern void kaze_gs_clear_vcount(void);
extern int GsGetActiveBuff(void);
extern void kaze_gs_set_work_base(int);
extern void GsClearOt(int, int, void *);
void kaze_start_map_cursor_move();
int kaze_start_location_validity();
void kaze_map_cursor_confirm_input();
void kaze_map_panel_scroll_update();
void kaze_map_cursor_state_draw();
void kaze_map_screen_state_renderers();
void kaze_map_fade_in_state();
void kaze_map_second_confirm_input();
void kaze_map_confirm_state_draw();
void kaze_map_fade_out_state();
extern int GsGetVcount(void);
extern void kaze_input_edge_detect(void);
extern void GsSwapDispBuff(void);
extern void GsSortClear(int, int, int, void *);
extern void GsDrawOt(void *);
void kaze_map_screen_free_buffers();
extern void kaze_seq_stop_track_34310(int);

/**
 * kaze_start_location_map_loop  (map_screen, 0x8004d904)
 *
 * Start-location map screen loop: runs while kaze_map_state != 9, recomputes
 * kaze_map_location_valid each state-2 frame via
 * kaze_start_location_validity(terrain tiles, zone table), commits the start
 * via kaze_start_location_commit on exit; skipped when
 * kaze_staged_stage_variant is nonzero (kaze_start_location_skip_path path).
 * Work buffers at bytes +0x4120/+0x28198.
 */

void kaze_start_location_map_loop(int work_a, int work_b, int valid_a, int valid_b)
{
    extern int DrawSync(int);
    extern int VSync(int);
    extern char kaze_map_screen_ot_table[];
    int active;
    int ot_index;
    int work;
    void *ot;
    /*
     * The four incoming arguments are held in call-saved registers across the
     * whole map loop: work bases in s2/s3, start-location args in s5/s6, and
     * the hoisted OT base &kaze_map_screen_ot_table in s4 (between them).  Plain C gives the
     * OT base a lower saved register than the work-base params because it is
     * referenced more often inside the loop, rotating s2/s3/s4.  Pinning the
     * four params to their target saved registers, in argument order, is the
     * old-gcc idiom that reproduces the exact prologue copy order and leaves
     * s4 free for the OT base.  The body is otherwise plain C.
     */
    register int wa asm("$18") = work_a;   /* $s2 */
    register int wb asm("$19") = work_b;   /* $s3 */
    register int va asm("$21") = valid_a;  /* $s5 */
    register int vb asm("$22") = valid_b;  /* $s6 */

    if (kaze_staged_stage_variant != 0) {
        kaze_start_location_skip_path();
    } else {
        kaze_play_sound_effect(10, 100, 0, 0);
        kaze_start_map_screen_init();
        GsInitVcount();
        VSync(0);
        kaze_gs_clear_vcount();
        while (kaze_map_state != 9) {
            active = GsGetActiveBuff();
            work = wb;
            if (active == 0) {
                work = wa;
            }
            kaze_gs_set_work_base(work);
            ot_index = active * 4;
            ot_index = (ot_index + active) * 4;
            GsClearOt(0, 0, (void *)(ot_index + (int)kaze_map_screen_ot_table));
            switch (kaze_map_state) {
            case 1:
                kaze_map_screen_state_renderers(active);
                kaze_map_fade_in_state(active);
                break;
            case 2:
                kaze_start_map_cursor_move();
                kaze_map_location_valid = kaze_start_location_validity(va, vb);
                kaze_map_cursor_confirm_input();
                kaze_map_panel_scroll_update(active);
                kaze_map_cursor_state_draw(active);
                break;
            case 3:
                kaze_map_second_confirm_input();
                kaze_map_confirm_state_draw(active);
                break;
            case 5:
                kaze_map_fade_out_state(active);
                break;
            }
            DrawSync(0);
            kaze_menu_vcount_timestamp = GsGetVcount();
            VSync(0);
            kaze_gs_clear_vcount();
            kaze_input_edge_detect();
            GsSwapDispBuff();
            ot = (void *)(active * 20 + (int)kaze_map_screen_ot_table);
            GsSortClear(0x10, 0x40, 0x10, ot);
            GsDrawOt(ot);
        }
        kaze_map_screen_free_buffers();
        kaze_start_location_commit();
        kaze_seq_stop_track_34310(10);
    }
}

extern int kaze_map_screen_prim_ot_ptr;
extern int kaze_map_screen_prim_buffer;
extern int kaze_menu_input_prev_mask;
extern void ClearImage(RECT *, unsigned char, unsigned char, unsigned char);
extern int kaze_heap_alloc(int, int);
extern int kaze_rand_range(int, int);
void kaze_map_panel_prim_init();
void kaze_map_cursor_prim_init();
void kaze_map_target_marker_prim_init();
void kaze_map_scroll_arrow_prim_init();
void kaze_map_banner_prim_init();
void kaze_map_wind_overlay_prim_init();

/**
 * kaze_start_map_screen_init  (map_screen, 0x8004db48)
 *
 * Clears the screen, allocates buffers, randomizes start (kaze_rand_range),
 * sets cursor kaze_map_cursor_screen_x/30/34/38, and runs map sub-inits.
 */

void kaze_start_map_screen_init(void)
{
    extern void DrawSync(int);
    extern int kaze_input_current_c;
    extern int kaze_input_edge;
    extern int kaze_map_cursor_z;
    extern int kaze_map_cursor_x;
    extern int kaze_map_cursor_screen_y;
    extern int kaze_map_cursor_screen_x;
    extern int kaze_map_screen_ot_table;
    RECT rect;
    int *map_heap_ptr;
    int map_heap;
    int map_heap_alt;

    SetDispMask(0);
    rect.w = 0x140;
    rect.x = 0;
    rect.y = 0;
    rect.h = 0x1E0;
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
    SetDispMask(1);
    map_heap_ptr = &kaze_map_ot_heap_alloc_a;
    *map_heap_ptr = kaze_heap_alloc(0x10, 4);
    map_heap_alt = kaze_heap_alloc(0x10, 4);
    kaze_map_ot_heap_alloc_b = map_heap_alt;
    map_heap = *map_heap_ptr;
    kaze_map_screen_ot_table = 4;
    D_800AAF0C = 4;
    kaze_map_heap_ptr_alt = map_heap_alt;
    kaze_map_screen_prim_ot_ptr = map_heap;
    kaze_map_screen_prim_buffer = kaze_heap_alloc(1, 0x1FA8);
    kaze_input_current_c = 0;
    kaze_menu_input_prev_mask = 0;
    kaze_input_edge = 0;
    D_800B1A48 = 0;
    kaze_map_selected_wind = kaze_rand_range(0, 7);
    kaze_map_cursor_screen_x = 0xA0;
    kaze_map_cursor_screen_y = 0x78;
    kaze_map_cursor_x = 0x80;
    kaze_map_cursor_z = 0x58;
    kaze_map_state = 1;
    kaze_map_wind_overlay_phase = 0;
    kaze_map_wind_overlay_angle = 0;
    kaze_map_target_marker_pulse_phase = 0;
    kaze_map_scroll_arrow_pulse = 0;
    kaze_map_fade_counter = 0xFF;
    kaze_map_intro_counter = 0;
    kaze_map_feedback_state = 0;
    kaze_map_panel_prim_init();
    kaze_map_cursor_prim_init();
    kaze_map_target_marker_prim_init();
    kaze_map_scroll_arrow_prim_init();
    kaze_map_banner_prim_init();
    kaze_map_wind_overlay_prim_init();
    kaze_map_tile_prim_init();
    kaze_map_fade_quad_init(2);
}

/**
 * kaze_map_screen_state_renderers  (map_screen, 0x8004dd00)
 *
 * First of the map-screen state renderers; kaze_map_screen_state_renderers/k
 * aze_start_map_cursor_move/kaze_map_cursor_confirm_input/kaze_map_panel_scr
 * oll_update/kaze_map_second_confirm_input/kaze_map_fade_in_state/kaze_map_c
 * ursor_state_draw/kaze_map_confirm_state_draw/kaze_map_fade_out_state
 * render the map screen states (cursor, confirmation, fade) inside the map
 * loop; kaze_map_screen_free_buffers finalizes and
 * kaze_input_edge_detect/kaze_gs_set_work_base handle input/packet areas.
 */

void kaze_map_screen_state_renderers(int idx)
{
    extern int rsin(int arg0);
    int shake_y;
    int prim_off;
    int shake_prim_off;

    if (kaze_map_intro_counter == 0x40) {
        goto plain;
    }
    if (kaze_map_intro_counter < 0x41) {
        goto shake;
    }
    kaze_map_state = 2;
plain:
    prim_off = idx * 0x14 + kaze_map_screen_prim_buffer;
    *(short *)(prim_off + 8) = -0x80;
    *(short *)(prim_off + 10) = -0x58;
    *(short *)(prim_off + 0x10) = 0x100;
    *(short *)(prim_off + 0x12) = 0xA0;
    goto done;
shake:
    shake_y = rsin(kaze_map_intro_counter << 4);
    if (shake_y < 0) {
        shake_y += 0xF;
    }
    shake_prim_off = idx * 0x14 + kaze_map_screen_prim_buffer;
    *(short *)(shake_prim_off + 8) = -0x80;
    *(short *)(shake_prim_off + 10) = (shake_y >> 4) - 0x158;
    *(short *)(shake_prim_off + 0x10) = 0x100;
    *(short *)(shake_prim_off + 0x12) = 0x100;
done:
    kaze_map_intro_counter = kaze_map_intro_counter + 1;
}

extern u32 kaze_input_hold_timer;

/**
 * kaze_start_map_cursor_move  (map_screen, 0x8004dde8)
 *
 * Moves the start-location map cursor/box
 * (kaze_map_cursor_screen_x/30/34/38) per D-pad masks
 * kaze_input_edge/kaze_input_current_c with bounds clamping.
 */

void kaze_start_map_cursor_move(void)
{
    extern s32 kaze_input_current_c;
    extern s32 kaze_input_edge;
    extern s32 kaze_map_cursor_z;
    extern s32 kaze_map_cursor_x;
    extern s32 kaze_map_cursor_screen_y;
    extern s32 kaze_map_cursor_screen_x;
  register int t asm("$2");
  register int w asm("$3");

  if ((kaze_input_edge & 0x1000) != 0) goto s1_fine;
  if ((kaze_input_current_c & 0x1000) == 0) goto s2;
  if (!(kaze_input_hold_timer - 0x10 < 0x10)) goto s1_coarse;
s1_fine:
  w = kaze_map_cursor_screen_y;
  if (w >= 0x23) t = w - 2; else t = 0x20;
  kaze_map_cursor_screen_y = t;
  __asm__ __volatile__("");
  w = kaze_map_cursor_z;
  if (w < 3) goto s1_zero;
  t = w - 2;
  kaze_map_cursor_z = t;
  goto s2;
s1_coarse:
  if (kaze_input_hold_timer < 0x20) goto s2;
  w = kaze_map_cursor_screen_y;
  if (w >= 0x25) t = w - 4; else t = 0x20;
  kaze_map_cursor_screen_y = t;
  __asm__ __volatile__("");
  w = kaze_map_cursor_z;
  if (w < 5) goto s1_zero;
  t = w - 4;
  kaze_map_cursor_z = t;
  goto s2;
s1_zero:
  kaze_map_cursor_z = 0;
s2:
  if ((kaze_input_edge & 0x4000) != 0) goto s2_fine;
  if ((kaze_input_current_c & 0x4000) == 0) goto s3;
  if (!(kaze_input_hold_timer - 0x10 < 0x10)) goto s2_coarse;
s2_fine:
  w = kaze_map_cursor_screen_y;
  if (w < 0xbc) t = w + 2; else t = 0xbe;
  kaze_map_cursor_screen_y = t;
  __asm__ __volatile__("");
  w = kaze_map_cursor_z;
  if (w < 0xfc) t = w + 2; else t = 0xfe;
  kaze_map_cursor_z = t;
  goto s3;
s2_coarse:
  if (kaze_input_hold_timer < 0x20) goto s3;
  w = kaze_map_cursor_screen_y;
  if (w < 0xba) t = w + 4; else t = 0xbe;
  kaze_map_cursor_screen_y = t;
  __asm__ __volatile__("");
  w = kaze_map_cursor_z;
  if (w < 0xfa) t = w + 4; else t = 0xfe;
  kaze_map_cursor_z = t;
  goto s3;
s3:
  if ((kaze_input_edge & 0x8000) != 0) goto s3_fine;
  if ((kaze_input_current_c & 0x8000) == 0) goto s4;
  if (!(kaze_input_hold_timer - 0x10 < 0x10)) goto s3_coarse;
s3_fine:
  w = kaze_map_cursor_screen_x;
  if (w >= 0x23) t = w - 2; else t = 0x20;
  kaze_map_cursor_screen_x = t;
  __asm__ __volatile__("");
  w = kaze_map_cursor_x;
  if (w < 3) goto s3_zero;
  t = w - 2;
  kaze_map_cursor_x = t;
  goto s4;
s3_coarse:
  if (kaze_input_hold_timer < 0x20) goto s4;
  w = kaze_map_cursor_screen_x;
  if (w >= 0x25) t = w - 4; else t = 0x20;
  kaze_map_cursor_screen_x = t;
  __asm__ __volatile__("");
  w = kaze_map_cursor_x;
  if (w < 5) goto s3_zero;
  t = w - 4;
  kaze_map_cursor_x = t;
  goto s4;
s3_zero:
  kaze_map_cursor_x = 0;
s4:
  if ((kaze_input_edge & 0x2000) != 0) goto s4_fine;
  if ((kaze_input_current_c & 0x2000) == 0) return;
  if (!(kaze_input_hold_timer - 0x10 < 0x10)) goto s4_coarse;
s4_fine:
  w = kaze_map_cursor_screen_x;
  if (w < 0x11c) t = w + 2; else t = 0x11e;
  kaze_map_cursor_screen_x = t;
  __asm__ __volatile__("");
  w = kaze_map_cursor_x;
  if (w < 0xfc) t = w + 2; else t = 0xfe;
  kaze_map_cursor_x = t;
  return;
s4_coarse:
  if (kaze_input_hold_timer < 0x20) return;
  w = kaze_map_cursor_screen_x;
  if (w < 0x11a) t = w + 4; else t = 0x11e;
  kaze_map_cursor_screen_x = t;
  __asm__ __volatile__("");
  w = kaze_map_cursor_x;
  if (w < 0xfa) t = w + 4; else t = 0xfe;
  kaze_map_cursor_x = t;
  return;
}

extern s32 kaze_menu_mode_source;
extern s32 kaze_menu_stage_source;
extern s32 SquareRoot0(s32);

/**
 * kaze_start_location_validity  (map_screen, 0x8004e198)
 *
 * Start-location validity rule: cursor in 0x14..0xeb both axes; in mode-1
 * staging (kaze_menu_mode_source==0) at least 0x20 half-cells from the per-
 * stage target
 * (kaze_start_location_target_x_table/kaze_start_location_target_z_table
 * indexed by kaze_menu_stage_source); terrain tile must lack flags
 * 0x0c000000, zone id not in per-stage exclusion lists (kaze_start_zone_excl
 * _table_mode1/kaze_start_zone_excl_table_mode0/kaze_start_zone_excl_table_m
 * ode2), zero +0xe2ee0 zone entry, terrain type nibble < 3.
 */

int kaze_start_location_validity(s32 tiles, s32 zone_table)
{
    extern s32 kaze_map_cursor_z;
    extern s32 kaze_map_cursor_x;
    s32 dx;
    s32 dz;
    s32 zone_id;
    s32 dist;
    u32 idx;
    u32 tile;
    u32 excl_id;
    u32 terrain_type;

    if (0x13 < kaze_map_cursor_x) {
        if (kaze_map_cursor_z >= 0x14) {
            if (kaze_map_cursor_x < 0xEC) {
                if (kaze_map_cursor_z < 0xEC) {
                    if (kaze_menu_mode_source == 0) {
                        dx = kaze_map_cursor_x / 2 - kaze_start_location_target_x_table[kaze_menu_stage_source];
                        dz = kaze_map_cursor_z / 2 - kaze_start_location_target_z_table[kaze_menu_stage_source];
                        dist = SquareRoot0(dx * dx + dz * dz);
                        if (dist < 0x20) {
                            return 0;
                        }
                    }
                    tile = *(u32 *)(kaze_map_cursor_z / 2 * 0x200 + tiles + kaze_map_cursor_x / 2 * 4);
                    if ((tile & 0xC000000) == 0) {
                        terrain_type = tile >> 0x1c;
                        zone_id = (s32)(tile >> 8) & 0x3ff;
                        if (zone_id != 0) {
                            goto check_zone;
                        }
                        return 1;
check_zone:
                        if (kaze_menu_stage_source == 1) {
                            goto mode_one;
                        }
                        if (kaze_menu_stage_source < 2) {
                            if (kaze_menu_stage_source == 0) {
                                goto mode_zero;
                            }
                            goto zone_table_check;
                        }
                        if (kaze_menu_stage_source == 2) {
                            goto mode_two;
                        }
                        goto zone_table_check;
mode_zero:
                        idx = 0;
                        do {
                            excl_id = (u32)kaze_start_zone_excl_table_mode0[idx];
                            if (excl_id == (u32)zone_id) {
                                return 0;
                            }
                            idx = idx + 1;
                        } while (idx < 9);
                        goto zone_table_check;
mode_one:
                        idx = 0;
                        do {
                            excl_id = (u32)kaze_start_zone_excl_table_mode1[idx];
                            if (excl_id == (u32)zone_id) {
                                return 0;
                            }
                            idx = idx + 1;
                        } while (idx < 7);
                        goto zone_table_check;
mode_two:
                        idx = 0;
                        do {
                            excl_id = (u32)kaze_start_zone_excl_table_mode2[idx];
                            if (excl_id == (u32)zone_id) {
                                return 0;
                            }
                            idx = idx + 1;
                        } while (idx < 0xD);
zone_table_check:
                        if (*(short *)((zone_id << 2) + zone_table - 4) == 0) {
                            goto valid_zone;
                        }
                        goto invalid;
                    }
                }
            }
        }
    }

invalid:
    return 0;
valid_zone:
    return (s32)terrain_type < 3;
}

extern void kaze_sfx_play_dispatch(int);

/**
 * kaze_map_cursor_confirm_input  (map_screen, 0x8004e3c0)
 *
 * Map-screen cursor-state input: dpad/Cross clears banner index
 * kaze_map_feedback_state; Circle checks validity kaze_map_location_valid -
 * invalid shows banner 2 + error sound, valid sets map state
 * kaze_map_state=3 and banner 1 (first confirm).
 */

void kaze_map_cursor_confirm_input(void)
{
    extern volatile int kaze_input_edge;
    if (kaze_input_edge & 0xF040) {
        kaze_map_feedback_state = 0;
    }
    if (kaze_input_edge & 0x20) {
        if (kaze_map_location_valid != 0) {
            kaze_map_state = 3;
            kaze_map_feedback_state = 1;
            kaze_sfx_play_dispatch(0x88);
        } else {
            kaze_map_feedback_state = 2;
            kaze_sfx_play_dispatch(5);
        }
    }
}

/**
 * kaze_map_panel_scroll_update  (map_screen, 0x8004e450)
 *
 * Updates the 256x160 map panel SPRT v-texture offset
 * (kaze_map_screen_prim_buffer + buf*0x14 +0xc/+0xd) for vertical map
 * scrolling from cursor-y kaze_map_cursor_screen_y edges (0x20/0xbe) and
 * scroll kaze_map_cursor_z; otherwise copies the other buffer.
 */

void kaze_map_panel_scroll_update(int buf)
{
    extern int kaze_map_cursor_z;
    extern int kaze_map_cursor_screen_y;
    int other_off;
    int cur_off;

    if (kaze_map_cursor_screen_y == 0x20) {
        if (kaze_map_cursor_z < 0x60) {
            *(unsigned char *)(buf * 0x14 + kaze_map_screen_prim_buffer + 0xc) = 0;
            *(char *)(buf * 0x14 + kaze_map_screen_prim_buffer + 0xd) = (char)kaze_map_cursor_z;
        } else {
            *(unsigned char *)(buf * 0x14 + kaze_map_screen_prim_buffer + 0xc) = 0;
            *(char *)(buf * 0x14 + kaze_map_screen_prim_buffer + 0xd) = 0x60;
        }
    } else if (kaze_map_cursor_screen_y == 0xbe) {
        if (kaze_map_cursor_z >= 0xa0) {
            *(unsigned char *)(buf * 0x14 + kaze_map_screen_prim_buffer + 0xc) = 0;
            *(char *)(buf * 0x14 + kaze_map_screen_prim_buffer + 0xd) = (char)kaze_map_cursor_z + 0x62;
        } else {
            *(unsigned char *)(buf * 0x14 + kaze_map_screen_prim_buffer + 0xc) = 0;
            *(unsigned char *)(buf * 0x14 + kaze_map_screen_prim_buffer + 0xd) = 0;
        }
    } else {
        cur_off = buf * 0x14;
        other_off = ((buf + 1U) & 1) * 0x14;
        *(unsigned char *)(kaze_map_screen_prim_buffer + cur_off + 0xc) =
            *(unsigned char *)(kaze_map_screen_prim_buffer + other_off + 0xc);
        *(unsigned char *)(kaze_map_screen_prim_buffer + cur_off + 0xd) =
            *(unsigned char *)(kaze_map_screen_prim_buffer + other_off + 0xd);
    }
}

/**
 * kaze_map_second_confirm_input  (map_screen, 0x8004e5bc)
 *
 * Map-screen second-confirm input: Circle advances map state kaze_map_state
 * to 5 (commit path); Cross cancels back to cursor state 2 and clears banner
 * kaze_map_feedback_state; plays confirm/cancel sounds via
 * kaze_sfx_play_dispatch.
 */

void kaze_map_second_confirm_input(void)
{
    extern int kaze_input_edge;
    if ((kaze_input_edge & 0x20) != 0) {
        kaze_map_state = 5;
        kaze_sfx_play_dispatch(2);
    }
    if ((kaze_input_edge & 0x40) != 0) {
        kaze_map_state = 2;
        kaze_map_feedback_state = 0;
        kaze_sfx_play_dispatch(0x65);
    }
}

/**
 * kaze_map_fade_in_state  (map_screen, 0x8004e628)
 *
 * Map-screen fade-in state renderer: steps fade level kaze_map_fade_counter
 * down by 7 and draws the fade quad (kaze_map_fade_quad_draw), frame lines,
 * map tiles, and map panel.
 */

void kaze_map_fade_in_state(int idx)
{
    if (kaze_map_fade_counter >= 0) {
        kaze_map_fade_quad_draw(idx, kaze_map_fade_counter & 0xFF);
        kaze_map_fade_counter = kaze_map_fade_counter - 7;
    } else {
        kaze_map_fade_counter = 0;
    }
    kaze_map_frame_lines_draw(idx);
    kaze_map_tiles_draw(idx);
    kaze_map_panel_draw(idx);
    return;
}

/**
 * kaze_map_cursor_state_draw  (map_screen, 0x8004e6a0)
 *
 * Map-screen cursor-state draw pass: cursor sprite, scroll arrows, message
 * banner (index kaze_map_feedback_state), frame lines, map tiles, wind-
 * direction overlay, and map panel.
 */

void kaze_map_cursor_state_draw(int arg0)
{
    kaze_map_cursor_draw();
    kaze_map_scroll_arrow_draw(arg0);
    kaze_map_banner_draw(arg0, kaze_map_feedback_state);
    kaze_map_frame_lines_draw(arg0);
    kaze_map_tiles_draw(arg0);
    kaze_map_wind_overlay_anim(arg0);
    kaze_map_panel_draw(arg0);
}

/**
 * kaze_map_confirm_state_draw  (map_screen, 0x8004e700)
 *
 * Map-screen confirm-state draw pass: pulsing target marker instead of the
 * cursor, plus message banner, frame lines, map tiles, wind overlay, and map
 * panel.
 */

void kaze_map_confirm_state_draw(int idx)
{
    kaze_map_target_marker_draw();
    kaze_map_banner_draw(idx, kaze_map_feedback_state);
    kaze_map_frame_lines_draw(idx);
    kaze_map_tiles_draw(idx);
    kaze_map_wind_overlay_anim(idx);
    kaze_map_panel_draw(idx);
}

/**
 * kaze_map_fade_out_state  (map_screen, 0x8004e758)
 *
 * Map-screen fade-out state: draws the fade quad with rising
 * kaze_map_fade_counter (+8/frame) and sets map state kaze_map_state=9
 * (exit) once past 0xff; also draws frame lines, tiles, and panel.
 */

void kaze_map_fade_out_state(int idx)
{
    kaze_map_fade_quad_draw(idx, kaze_map_fade_counter & 0xFF);
    kaze_map_fade_counter = kaze_map_fade_counter + 8;
    if (kaze_map_fade_counter > 0xFF) {
        kaze_map_state = 9;
    }
    kaze_map_frame_lines_draw(idx);
    kaze_map_tiles_draw(idx);
    kaze_map_panel_draw(idx);
    return;
}

extern void kaze_heap_free(int);

/**
 * kaze_map_screen_free_buffers  (map_screen, 0x8004e7cc)
 *
 * Map-screen teardown: frees the two OT buffers
 * kaze_map_ot_heap_alloc_a/kaze_map_ot_heap_alloc_b and the primitive buffer
 * kaze_map_screen_prim_buffer via kaze_heap_free, then nulls
 * kaze_map_screen_prim_buffer.
 */

void kaze_map_screen_free_buffers(void)
{
    kaze_heap_free(kaze_map_ot_heap_alloc_a);
    kaze_heap_free(kaze_map_ot_heap_alloc_b);
    kaze_heap_free(kaze_map_screen_prim_buffer);
    kaze_map_screen_prim_buffer = 0;
}

extern volatile int tail_D_8008E20C asm("kaze_map_screen_prim_buffer");
extern void SetLineF3(void *);

/**
 * kaze_map_panel_prim_init  (map_screen, 0x8004e81c)
 *
 * Inits per-buffer map primitives in kaze_map_screen_prim_buffer: 256x160
 * map panel SPRT at (-0x80,-0x58) with clut(0x140,499), DR_TPAGE, and two
 * colored LINE_F3 frame lines.
 */

void kaze_map_panel_prim_init(void)
{
    extern void SetDrawTPage(void *, int, int, unsigned int);
    extern u16 GetTPage(int, int, int, int);
    extern u16 GetClut(int, int);
    extern void SetShadeTex(void *, int);
    extern void SetSprt(void *);
  int clut_base;
  int prim_off;
  u16 tex16;
  int underline_off;
  int sprt_off;
  int tpage_off;
  int line1_off;
  int line2_off;
  int buf_i;
  register int c160 asm("$22");
  int cm89;
  int c96;
  buf_i = 0;
  __asm__("li %0,160" : "=r"(c160));
  __asm__("li %0,-89" : "=r"(cm89));
  __asm__("li %0,96" : "=r"(c96));
  { int t_i8 = 0x68; line2_off = t_i8; }
  { int _mt = line2_off; __asm__("" :: "r"(_mt)); }
  line1_off = 0x38;
  underline_off = 0;
  tpage_off = 0x28;
  sprt_off = 0;
  do {
    SetSprt((void *)(kaze_map_screen_prim_buffer + sprt_off));
    do {
      register int early_base asm("$3");
      int clut_addr;
      register int tail_base asm("$2");
      register int tail_128 asm("$3");
      SetShadeTex((void *)(kaze_map_screen_prim_buffer + sprt_off), 1);
      early_base = kaze_map_screen_prim_buffer;
      prim_off = sprt_off + early_base;
      *(u8 *)(prim_off + 0xc) = 0;
      clut_base = kaze_map_screen_prim_buffer;
      *(s16 *)(prim_off + 8) = -0x80;
      *(s16 *)(prim_off + 10) = -0x58;
      *(s16 *)(prim_off + 0x10) = 0x100;
      *(s16 *)(prim_off + 0x12) = c160;
      clut_addr = sprt_off + clut_base;
      *(u8 *)(clut_addr + 0xd) = 0;
      tex16 = GetClut(0x140, 499);
      *(u16 *)(kaze_map_screen_prim_buffer + sprt_off + 0xe) = tex16;
      tex16 = GetTPage(1, 0, 0x300, 0x100);
      SetDrawTPage((void *)(kaze_map_screen_prim_buffer + tpage_off), 0, 1, (unsigned int)tex16);
      SetLineF3((void *)(kaze_map_screen_prim_buffer + line1_off));
      SetLineF3((void *)(kaze_map_screen_prim_buffer + line2_off));
      buf_i = buf_i + 1;
      tail_base = kaze_map_screen_prim_buffer;
      tail_base = underline_off + tail_base;
      tail_128 = 0x80;
      *(u8 *)(tail_base + 0x3c) = c160;
      {
        register int tail_store_base asm("$4");
        tail_store_base = tail_D_8008E20C;
      *(s16 *)(tail_base + 0x40) = -0x81;
      *(s16 *)(tail_base + 0x42) = 0x48;
      *(s16 *)(tail_base + 0x44) = -0x81;
      *(s16 *)(tail_base + 0x76) = 0x48;
      *(s16 *)(tail_base + 0x78) = -0x81;
      *(s16 *)(tail_base + 0x46) = cm89;
      *(s16 *)(tail_base + 0x48) = tail_128;
      *(s16 *)(tail_base + 0x4a) = cm89;
      *(s16 *)(tail_base + 0x70) = tail_128;
      *(s16 *)(tail_base + 0x72) = cm89;
      *(s16 *)(tail_base + 0x74) = tail_128;
      *(s16 *)(tail_base + 0x7a) = 0x48;
        tail_store_base = underline_off + tail_store_base;
        { u8 *_p = (u8 *)tail_store_base; _p[61] = c160; }
      }
      sprt_off = sprt_off + 0x14;
      *(u8 *)(underline_off + kaze_map_screen_prim_buffer + 0x3e) = c160;
      tpage_off = tpage_off + 8;
      *(u8 *)(underline_off + kaze_map_screen_prim_buffer + 0x6c) = c96;
      line1_off = line1_off + 0x18;
      *(u8 *)(underline_off + kaze_map_screen_prim_buffer + 0x6d) = c96;
      line2_off = line2_off + 0x18;
      *(u8 *)(underline_off + kaze_map_screen_prim_buffer + 0x6e) = c96;
      underline_off = underline_off + 0x18;
    } while (0);
  } while (buf_i < 2);
  __asm__("");
  __asm__("");
  __asm__("");
  __asm__("" :: "r"(buf_i));
  __asm__("" :: "r"(buf_i));
}

extern void SetSprt16(void *);

/**
 * kaze_map_cursor_prim_init  (map_screen, 0x8004ea38)
 *
 * Inits the map cursor SPRT_16 pair per buffer (valid/invalid variants,
 * tiles (0xc0,0x60)/(0xc0,0x70), clut(0x160,0x1f1)) plus DR_TPAGE at
 * +0x98..+0xd8 of kaze_map_screen_prim_buffer.
 */

void kaze_map_cursor_prim_init(void)
{
    extern void SetDrawTPage(void *, int, int, unsigned int);
    extern u_short GetTPage(int, int, int, int);
    extern u_short GetClut(int, int);
    extern void SetShadeTex(void *, int);
    u_short clut;
    int slot;
    int pair_ofs;
    int sprt_ofs;
    int dr_ofs;
    int tile_u;
    int i;

    i = 0;
    tile_u = 0xC0;
    dr_ofs = 0xD8;
    sprt_ofs = 0x98;
    do {
        SetSprt16((void *)(kaze_map_screen_prim_buffer + sprt_ofs));
        SetShadeTex((void *)(kaze_map_screen_prim_buffer + sprt_ofs), 1);
        pair_ofs = i * 0x20;
        *(char *)(pair_ofs + kaze_map_screen_prim_buffer + 0xA4) = tile_u;
        *(char *)(pair_ofs + kaze_map_screen_prim_buffer + 0xA5) = 0x60;
        clut = GetClut(0x160, 0x1F1);
        i++;
        slot = kaze_map_screen_prim_buffer + sprt_ofs;
        *(u_short *)(kaze_map_screen_prim_buffer + pair_ofs + 0xA6) = clut;
        SetSprt16((void *)(slot + 0x10));
        SetShadeTex((void *)(kaze_map_screen_prim_buffer + sprt_ofs + 0x10), 1);
        *(char *)(pair_ofs + kaze_map_screen_prim_buffer + 0xB4) = tile_u;
        *(char *)(pair_ofs + kaze_map_screen_prim_buffer + 0xB5) = 0x70;
        clut = GetClut(0x160, 0x1F1);
        sprt_ofs += 0x20;
        *(u_short *)(kaze_map_screen_prim_buffer + pair_ofs + 0xB6) = clut;
        clut = GetTPage(0, 0, 0x2C0, 0x100);
        SetDrawTPage((void *)(kaze_map_screen_prim_buffer + dr_ofs), 0, 1, (unsigned int)clut);
        dr_ofs += 8;
    } while (i < 2);
}

extern void SetSemiTrans(void *, int);

/**
 * kaze_map_target_marker_prim_init  (map_screen, 0x8004eb98)
 *
 * Inits the semi-transparent 8x8 target-marker SPRT_8 (tile 0xd0/0x70,
 * clut(0x180,0x1f1)) plus DR_TPAGE per buffer at +0xe8/+0x108 of
 * kaze_map_screen_prim_buffer.
 */

void kaze_map_target_marker_prim_init(void)
{
    extern void SetDrawTPage(void *, int, int, int);
    extern int GetTPage(int, int, int, int);
    extern int GetClut(int, int);
    extern void SetShadeTex(void *, int);
    int i;
    int sprt_ofs;
    int dr_ofs;
    int slot_ofs;
    int tpage;
    int clut_x;
    int clut_y;

    i = 0;
    dr_ofs = 0x108;
    sprt_ofs = 0xE8;
    do {
        slot_ofs = i << 4;
        i++;
        SetSprt8((void *)(kaze_map_screen_prim_buffer + sprt_ofs));
        SetShadeTex((void *)(kaze_map_screen_prim_buffer + sprt_ofs), 0);
        SetSemiTrans((void *)(kaze_map_screen_prim_buffer + sprt_ofs), 1);
        clut_x = 0x180;
        clut_y = 0x1F1;
        *(u8 *)(slot_ofs + kaze_map_screen_prim_buffer + 0xF4) = 0xD0;
        *(u8 *)(slot_ofs + kaze_map_screen_prim_buffer + 0xF5) = 0x70;
        *(u16 *)(kaze_map_screen_prim_buffer + slot_ofs + 0xF6) = GetClut(clut_x, clut_y);
        sprt_ofs += 0x10;
        tpage = GetTPage(0, 1, 0x2C0, 0x100);
        SetDrawTPage((void *)(kaze_map_screen_prim_buffer + dr_ofs), 0, 1, tpage & 0xFFFF);
        dr_ofs += 8;
    } while (i < 2);
}

/**
 * kaze_map_scroll_arrow_prim_init  (map_screen, 0x8004eca0)
 *
 * Inits the two blinking scroll-arrow SPRT_8 icons per buffer (+0x118, tile
 * v 0x60/0x68, clut(0x170,0x1f1)) with up/down positions near the panel edge
 * and a DR_TPAGE at +0x158.
 */

void kaze_map_scroll_arrow_prim_init(void)
{
    extern void SetDrawTPage(void *, int, int, int);
    extern int GetTPage(int, int, int, int);
    extern int GetClut(int, int);
    extern void SetShadeTex(void *, int);
    u16 tpage;
    register int inner __asm__("$17");
    register int sprt_ofs __asm__("$16");
    register int v __asm__("$18");
    register int clut_ofs __asm__("$19");
    register int buf_ofs __asm__("$20");
    register int prim_ofs __asm__("$21");
    register int dr_ofs __asm__("$22");
    register int outer __asm__("$23");
    int neg4;
    register int base_after __asm__("$8");
    register int outer_base __asm__("$3");

    outer = 0;
    neg4 = -4;
    dr_ofs = 0x158;
    buf_ofs = 0;
    prim_ofs = 0x118;
    do {
        inner = 0;
        clut_ofs = buf_ofs;
        v = 0x60;
        do {
            register int clut_x __asm__("$4");
            register int clut_y __asm__("$5");
            register int color __asm__("$3");

            sprt_ofs = inner * 0x10;
            inner++;
            SetSprt8((void *)(kaze_map_screen_prim_buffer + prim_ofs + sprt_ofs));
            SetShadeTex((void *)(kaze_map_screen_prim_buffer + prim_ofs + sprt_ofs), 0);
            clut_x = 0x170;
            __asm__ volatile("" : "=r"(clut_x) : "0"(clut_x));
            color = 0xd0;
            *(u8 *)(sprt_ofs + (buf_ofs + kaze_map_screen_prim_buffer) + 0x124) = color;
            sprt_ofs = sprt_ofs + (buf_ofs + kaze_map_screen_prim_buffer);
            clut_y = 0x1f1;
            *(char *)(sprt_ofs + 0x125) = v;
            tpage = GetClut(clut_x, clut_y);
            base_after = kaze_map_screen_prim_buffer;
            v += 8;
            *(u16 *)(base_after + clut_ofs + 0x126) = tpage;
            clut_ofs += 0x10;
        } while (inner < 2);
        {
            register int tpage_mode __asm__("$4") = 0;
            register int tpage_abr __asm__("$5") = 0;
            register int tpage_x __asm__("$6") = 0x2c0;
            register int tpage_y __asm__("$7") = 0x100;

            __asm__ volatile("" : "=r"(tpage_mode) : "0"(tpage_mode));
            __asm__ volatile("" : "=r"(tpage_abr) : "0"(tpage_abr));
            __asm__ volatile("" : "=r"(tpage_x) : "0"(tpage_x));
            __asm__ volatile("" : "=r"(tpage_y) : "0"(tpage_y));
            outer_base = buf_ofs + base_after;
            *(short *)(outer_base + 0x122) = -0x60;
            *(short *)(outer_base + 0x120) = neg4;
            *(short *)(outer_base + 0x130) = neg4;
            *(short *)(outer_base + 0x132) = 0x48;
            tpage = GetTPage(tpage_mode, tpage_abr, tpage_x, tpage_y);
        }
        {
            register int draw_zero __asm__("$5") = 0;
            register int draw_one __asm__("$6") = 1;
            register int draw_tpage __asm__("$7") = tpage;

            __asm__ volatile("" : "=r"(draw_zero) : "0"(draw_zero));
            __asm__ volatile("" : "=r"(draw_one) : "0"(draw_one));
            __asm__ volatile("" : "=r"(draw_tpage) : "0"(draw_tpage));
            buf_ofs += 0x20;
            prim_ofs += 0x20;
            outer++;
            SetDrawTPage((void *)(kaze_map_screen_prim_buffer + dr_ofs), draw_zero, draw_one, draw_tpage);
        }
        dr_ofs += 8;
    } while (outer < 2);
}

/**
 * kaze_map_banner_prim_init  (map_screen, 0x8004ee10)
 *
 * Inits the 6-variant 256x16 message banner SPRTs per buffer (+0x168,
 * v0=i*16, y 0x50, clut(0x140,0x1f1)) plus DR_TPAGE and two LINE_F3
 * underline frames.
 */

void kaze_map_banner_prim_init(void)
{
    extern void SetDrawTPage(void *, int, int, unsigned int);
    extern u16 GetTPage(int, int, int, int);
    extern u16 GetClut(int, int);
    extern void SetShadeTex(void *, int);
    extern void SetSprt(void *);
  register int prim_off_v1 asm("$2");
  int iBase;
  int iDat;
  u16 tex16;
  register int prim_off asm("$6");
  int v0;
  register int gc_a0 asm("$4");
  register int gc_a1 asm("$5");
  int var_off;
  int variant_i;
  int clut_off;
  int underline_off;
  int banner_off;
  int sprt_off;
  register int c60 asm("$22");
  int tpage_off;
  int line1_off;
  int buf_i;
  int line2_off;
  short *new_var;
  c60 = 0x60;
  buf_i = 0;
  line2_off = 0x298;
  line1_off = 0x268;
  underline_off = 0;
  tpage_off = 600;
  sprt_off = 0x168;
  banner_off = 0;
  do
  {
    variant_i = 0;
    clut_off = banner_off;
    var_off = 0;
    do
    {
      SetSprt((void *) ((kaze_map_screen_prim_buffer + sprt_off) + var_off));
      SetShadeTex((void *) ((kaze_map_screen_prim_buffer + sprt_off) + var_off), 1);
      gc_a0 = 0x140;
      asm("" : "=r"(gc_a0) : "0"(gc_a0));
      gc_a1 = 0x1f1;
      asm("" : "=r"(gc_a1) : "0"(gc_a1));
      v0 = variant_i << 4;
      variant_i = variant_i + 1;
      prim_off = banner_off + kaze_map_screen_prim_buffer;
      prim_off = var_off + prim_off;
      new_var = (short *) (prim_off + 0x170);
      *new_var = -0x80;
      *((short *) (prim_off + 0x172)) = 0x50;
      *((short *) (prim_off + 0x178)) = 0x100;
      *((u8 *) (prim_off + 0x174)) = 0;
      prim_off_v1 = banner_off + kaze_map_screen_prim_buffer;
      *((short *) (prim_off + 0x17a)) = 0x10;
      prim_off_v1 = var_off + prim_off_v1;
      *((char *) (prim_off_v1 + 0x175)) = (char) v0;
      tex16 = GetClut(gc_a0, gc_a1);
      var_off = var_off + 0x14;
      *((u16 *) ((kaze_map_screen_prim_buffer + clut_off) + 0x176)) = tex16;
      clut_off = clut_off + 0x14;
    }
    while (variant_i < 6);
    tex16 = GetTPage(0, 0, 0x2c0, 0x100);
    SetDrawTPage((void *) (kaze_map_screen_prim_buffer + tpage_off), 0, 1, (unsigned int) tex16);
    SetLineF3((void *) (kaze_map_screen_prim_buffer + line1_off));
    sprt_off = sprt_off + 0x78;
    SetLineF3((void *) (kaze_map_screen_prim_buffer + line2_off));
    banner_off = banner_off + 0x78;
    iBase = underline_off + kaze_map_screen_prim_buffer;
    *((short *) (iBase + 0x270)) = -0x81;
    *((short *) (iBase + 0x274)) = -0x81;
    *((short *) (iBase + 0x276)) = 0x4f;
    asm("" : "=r"(c60) : "0"(c60));
    *((u8 *) (iBase + 0x26c)) = 0xa0;
    iDat = kaze_map_screen_prim_buffer;
    *((short *) (iBase + 0x27a)) = 0x4f;
    *((short *) (iBase + 0x2a2)) = 0x4f;
    *((short *) (iBase + 0x272)) = c60;
    *((short *) (iBase + 0x278)) = 0x80;
    *((short *) (iBase + 0x2a0)) = 0x80;
    *((short *) (iBase + 0x2a4)) = 0x80;
    *((short *) (iBase + 0x2a6)) = c60;
    *((short *) (iBase + 0x2a8)) = -0x81;
    *((short *) (iBase + 0x2aa)) = c60;
    *((u8 *) ((underline_off + iDat) + 0x26d)) = 0xa0;
    buf_i = buf_i + 1;
    *((u8 *) ((underline_off + kaze_map_screen_prim_buffer) + 0x26e)) = 0xa0;
    tpage_off += 8;
    *((u8 *) ((underline_off + kaze_map_screen_prim_buffer) + 0x29c)) = c60;
    *((u8 *) ((underline_off + kaze_map_screen_prim_buffer) + 0x29d)) = c60;
    line2_off = line2_off + 0x18;
    *((u8 *) ((underline_off + kaze_map_screen_prim_buffer) + 0x29e)) = c60;
    line1_off = line1_off + 0x18;
    underline_off = underline_off + 0x18;
  }
  while (buf_i < 2);
}

/*
 * kaze_map_wind_overlay_prim_init (0x8004f08c) -- MATCH, byte-exact (584B, 0 diff).
 *
 * Inits the 7x10 grid of semi-transparent POLY_FT4 wind-direction arrow quads
 * per buffer for the map wind overlay. Two passes (buf_i<2), 7 rows
 * (row_i<7), 10 cols (col_i<10) per row.
 *
 * The last residual was the 0x2d4 byte-store address chain: the target emits
 * addend-first in-place adds `addu $3,$19,$3 / $3,$18,$3 / $3,$17,$3` (base $3
 * threaded as rt) while a plain expression emits accumulator-first
 * `addu $3,$3,$19 ...`. Root mechanism (see docs/matching-knowledge.jsonl):
 *   - Operand order = PLUS-rtx operand order (mips.md addu %0,%z1,%2). cse/combine
 *     never swap two REGs; expand_binop (optabs.c:411) swaps when target==op1.
 *   - A reassigned in-place chain (`bb=addend+bb`, or left-assoc reusing base's
 *     pseudo) gets COALESCED by local-alloc into one pseudo that is both dest and
 *     op0 -> accumulator-first, but lands on base's reg $3 + schedules before the
 *     sh (correct reg/schedule).  A right-nest `k+(j+(i+base))` keeps distinct
 *     fresh pseudos -> addend-first survives, but they float to $v0 / after-sh.
 * FIX (the a308 two-lever recipe, adapted): give the base its own scoped local
 * `cb` PINNED to $3, PIN the right-nested addend-first result `a` to $3 too
 * (adjacent live ranges: cb dies exactly as a is born), and PIN the sh-address
 * result `sh16` to $16 so it reuses tex_off's dying reg like the target
 * (`addu $16,$3,$16`). The pins stop local-alloc from coalescing the chain into
 * an op0-first in-place form while still forcing base+result onto $3 and the
 * sh-addr onto $16 -- reproducing addend-first-in-place-on-$3-before-sh exactly.
 *   - Other reconstruction notes: kaze_map_screen_prim_buffer is a plain int global reloaded per
 *     store (aliasing), reproducing the per-store `lw kaze_map_screen_prim_buffer`. The target
 *     keeps all 9 saved regs busy (the row/col offsets and middle copies,
 *     prim_base gcc-picks-$30), forcing the address recompute per store. c7f pins the
 *     shared 0x7f to $v1 (materialised once, reused across 0x2e5/0x2ed).
 */
extern void SetPolyFT4(void *);

/**
 * kaze_map_wind_overlay_prim_init  (map_screen, 0x8004f08c)
 *
 * Inits the 7x10 grid of semi-transparent POLY_FT4 wind-direction arrow
 * quads per buffer (+0x2c8, clut(0x150,0x1f1), tpage(0x2c0,0x100)) used by
 * the map wind overlay.
 */

void kaze_map_wind_overlay_prim_init(void)
{
    extern int GetTPage(int, int, int, int);
    extern int GetClut(int, int);
    extern void SetShadeTex(void *, int);
    register int row_off asm("$18");
    register int row_i asm("$22");
    register int tex_base asm("$23");
    int prim_base;
    int buf_i;
    u16 tex16;

    prim_base = 0x2c8;
    tex_base = 0;
    buf_i = 0;
    do {
        row_i = 0;
        row_off = 0;
        do {
            register int col_off asm("$17");
            register int tex_row_off asm("$19");
            register int prim_row_off asm("$20");
            register int col_i asm("$21");
            register int c7f asm("$3");
            int tex_off;

            col_i = 0;
            tex_row_off = tex_base;
            prim_row_off = prim_base;
            col_off = 0;
            do {
                SetPolyFT4((void *)(kaze_map_screen_prim_buffer + prim_row_off + row_off + col_off));
                SetShadeTex((void *)(kaze_map_screen_prim_buffer + prim_row_off + row_off + col_off), 0);
                SetSemiTrans((void *)(kaze_map_screen_prim_buffer + prim_row_off + row_off + col_off), 1);
                tex16 = GetClut(0x150, 0x1f1);
                tex_off = col_off + row_off + tex_row_off;
                *(u16 *)(kaze_map_screen_prim_buffer + tex_off + 0x2d6) = tex16;
                tex16 = GetTPage(0, 1, 0x2c0, 0x100);
                {
                    register int cb asm("$3") = kaze_map_screen_prim_buffer;
                    register int a asm("$3");
                    register int sh16 asm("$16");
                    sh16 = cb + tex_off;
                    *(u16 *)(sh16 + 0x2de) = tex16;
                    a = col_off + (row_off + (tex_row_off + cb));
                    *(u8 *)(a + 0x2d4) = 0x40;
                }
                *(u8 *)(col_off + (row_off + (tex_row_off + kaze_map_screen_prim_buffer)) + 0x2d5) = 0x60;
                *(u8 *)(col_off + (row_off + (tex_row_off + kaze_map_screen_prim_buffer)) + 0x2dc) = 0x5f;
                *(u8 *)(col_off + (row_off + (tex_row_off + kaze_map_screen_prim_buffer)) + 0x2dd) = 0x60;
                *(u8 *)(col_off + (row_off + (tex_row_off + kaze_map_screen_prim_buffer)) + 0x2e4) = 0x40;
                c7f = 0x7f;
                *(u8 *)(col_off + (row_off + (tex_row_off + kaze_map_screen_prim_buffer)) + 0x2e5) = c7f;
                *(u8 *)(col_off + (row_off + (tex_row_off + kaze_map_screen_prim_buffer)) + 0x2ec) = 0x5f;
                col_i = col_i + 1;
                *(u8 *)(col_off + (row_off + (tex_row_off + kaze_map_screen_prim_buffer)) + 0x2ed) = c7f;
                col_off = col_off + 0x28;
            } while (col_i < 10);
            row_i = row_i + 1;
            row_off = row_off + 400;
        } while (row_i < 7);
        prim_base = prim_base + 0xaf0;
        tex_base = tex_base + 0xaf0;
        buf_i = buf_i + 1;
    } while (buf_i < 2);
}
