#include "types.h"
#include "kaze_syms.h"
extern char kaze_title_txp_path[];
extern int kaze_task_record_index;
extern u8 kaze_title_screen_cleanup_flag;
extern int D_8008E0E8;
extern int kaze_memcard_slot0_event_a;
extern int kaze_memcard_slot0_event_b;
extern int kaze_memcard_slot0_event_c;
extern int kaze_memcard_slot0_event_d;
extern int kaze_memcard_slot1_event_a;
extern int kaze_memcard_slot1_event_b;
extern int kaze_memcard_slot1_event_c;
extern int kaze_memcard_slot1_event_d;
extern int kaze_memcard_wait_timeout_count;
extern int kaze_title_vsync_frame_delay;
extern u8 kaze_title_press_start_cycle_count;
extern int kaze_title_xa_track_id;
extern u8 kaze_title_start_skip_flag;
extern int kaze_title_model_buffer_ptrs[];
extern u8 kaze_opening_model_loaded_flag;
extern int kaze_opening_model_file_ptr;
extern int kaze_gs_view_pos_x;
extern int kaze_gs_view_pos_z;
extern int kaze_gs_light0_dir_y;
extern int kaze_gs_light0_dir_z;
extern int D_800B1A48;
extern int kaze_menu_vcount_timestamp;
extern void GsSetAmbient(int, int, int);
typedef struct KazeTitleModelCopyBlock {
    int w0;
    int w1;
    int w2;
    int w3;
} KazeTitleModelCopyBlock;

/**
 * kaze_memcard_wait_result_a  (library, 0x8004bbdc)
 *
 * Spins on TestEvent(kaze_memcard_slot0_event_a..94) returning result code
 * 0-3 with timeout counter kaze_memcard_wait_timeout_count.
 */

int kaze_memcard_wait_result_a(void)
{
    extern int TestEvent(int);
    int one = 1;

    kaze_memcard_wait_timeout_count = 0;
    for (;;) {
        if (TestEvent(kaze_memcard_slot0_event_a) == one) {
            return 0;
        }
        if (TestEvent(kaze_memcard_slot0_event_b) == one) {
            return 1;
        }
        if (TestEvent(kaze_memcard_slot0_event_c) == one) {
            return 2;
        }
        if (TestEvent(kaze_memcard_slot0_event_d) == one) {
            return 3;
        }
        kaze_memcard_wait_timeout_count++;
    }
}

/**
 * kaze_memcard_clear_events_a  (library, 0x8004bc88)
 *
 * TestEvent-drains the 4 primary memcard events
 * kaze_memcard_slot0_event_a..94.
 */

void kaze_memcard_clear_events_a(void)
{
    extern void TestEvent(int);
    TestEvent(kaze_memcard_slot0_event_a);
    TestEvent(kaze_memcard_slot0_event_b);
    TestEvent(kaze_memcard_slot0_event_c);
    TestEvent(kaze_memcard_slot0_event_d);
}

/**
 * kaze_memcard_wait_result_b  (library, 0x8004bce0)
 *
 * Spins on TestEvent(kaze_memcard_slot1_event_a..a4) returning result code
 * 0-3 for the second event bank.
 */

int kaze_memcard_wait_result_b(void)
{
    extern int TestEvent(int);
    int one = 1;

    kaze_memcard_wait_timeout_count = 0;
    for (;;) {
        if (TestEvent(kaze_memcard_slot1_event_a) == one) {
            return 0;
        }
        if (TestEvent(kaze_memcard_slot1_event_b) == one) {
            return 1;
        }
        if (TestEvent(kaze_memcard_slot1_event_c) == one) {
            return 2;
        }
        if (TestEvent(kaze_memcard_slot1_event_d) == one) {
            return 3;
        }
        kaze_memcard_wait_timeout_count++;
    }
}

/**
 * kaze_memcard_clear_events_b  (library, 0x8004bd8c)
 *
 * TestEvent-drains the 4 secondary memcard events
 * kaze_memcard_slot1_event_a..a4.
 */

void kaze_memcard_clear_events_b(void)
{
    extern void TestEvent(int);
    TestEvent(kaze_memcard_slot1_event_a);
    TestEvent(kaze_memcard_slot1_event_b);
    TestEvent(kaze_memcard_slot1_event_c);
    TestEvent(kaze_memcard_slot1_event_d);
}

extern unsigned int kaze_input_current_a;
extern unsigned int kaze_input_current_b;
extern unsigned int kaze_input_current_c;
extern unsigned int kaze_menu_input_prev_mask;
extern unsigned int kaze_input_edge;
extern int kaze_input_hold_timer;

/**
 * kaze_input_edge_detect_memcard  (core, 0x8004bde4)
 *
 * Input edge detection (like kaze_input_edge_detect) that overrides with a
 * memcard confirm case; sets kaze_input_edge/kaze_input_hold_timer.
 */

void kaze_input_edge_detect_memcard(int mode, unsigned int buttons)
{
    kaze_pad_mask_update();
    kaze_input_current_c = kaze_input_current_a;
    kaze_menu_input_prev_mask = kaze_input_current_b;

    if (mode == 0) {
        if (buttons & 0x100) {
            buttons &= -0x101;
            if (buttons == 2) {
                kaze_input_current_c = kaze_input_current_b;
            }
        }
    }

    if (kaze_input_current_c != kaze_menu_input_prev_mask) {
        kaze_input_hold_timer = 0;
        kaze_input_edge = kaze_input_current_c & ~kaze_menu_input_prev_mask;
    } else {
        kaze_input_edge = 0;
        kaze_input_hold_timer = kaze_input_hold_timer + 1;
    }
}

extern u8 kaze_title_clear_color_r;
extern u8 kaze_title_clear_color_g;
extern u8 kaze_title_clear_color_b;
extern char kaze_menu_gs_ot_buf[];
extern int kaze_menu_ot_work_buf0[];
void kaze_title_screen_init();
void kaze_title_model_buffers_alloc();
extern void GsInitVcount(void);
extern int VSync(int);
extern void kaze_gs_clear_vcount(void);
extern int GsGetActiveBuff(void);
extern void kaze_gs_set_work_base(int);
extern void GsClearOt(int, int, void *);
void kaze_title_state_render_dispatch();
extern int DrawSync(int);
extern int GsGetVcount(void);
extern void kaze_input_edge_detect(void);
extern void GsSwapDispBuff(void);
extern void GsSortClear(int, int, int, void *);
extern void GsDrawOt(void *);
void kaze_title_model_buffers_free();
void kaze_title_screen_cleanup();

/**
 * kaze_title_screen_loop  (menu, 0x8004beb4)
 *
 * Title/opening screen main loop: init, per-frame
 * GsClearOt/GsSwapDispBuff/GsDrawOt, dispatch
 * kaze_title_state_render_dispatch by state kaze_title_screen_state until
 * kaze_title_screen_state==-1.
 */

u8 kaze_title_screen_loop(void)
{
    extern u8 kaze_title_screen_state;
    int active;
    void *ot;

    kaze_gs_graphics_init_9170();
    kaze_title_screen_init();
    kaze_title_model_buffers_alloc();
    GsInitVcount();
    VSync(0);
    kaze_gs_clear_vcount();

    while (kaze_title_screen_state != 0xFF) {
        active = GsGetActiveBuff();
        kaze_gs_set_work_base(kaze_menu_ot_work_buf0[active]);
        ot = kaze_menu_gs_ot_buf + active * 0x14;
        GsClearOt(0, 0, ot);
        kaze_title_state_render_dispatch(active);
        DrawSync(0);
        kaze_menu_vcount_timestamp = GsGetVcount();
        VSync(kaze_title_vsync_frame_delay);
        kaze_gs_clear_vcount();
        kaze_input_edge_detect();
        GsSwapDispBuff();
        GsSortClear(kaze_title_clear_color_r, kaze_title_clear_color_g, kaze_title_clear_color_b, ot);
        GsDrawOt(ot);
        if (kaze_title_xa_track_id != -2) {
            kaze_xa_audio_manager_35a0(kaze_title_xa_track_id, 2);
        }
    }

    kaze_balloon_color_apply();
    kaze_title_model_buffers_free();
    kaze_title_screen_cleanup();
    kaze_gs_graphics_free_9270();
    return kaze_title_start_skip_flag;
}

extern int kaze_title_pressstart_sprite_buffer;
extern u8 kaze_title_start_flow_state;
extern u8 kaze_title_fade_blend_mode;
extern int kaze_gs_light0_dir_x;
extern int kaze_gs_view_pos_y;
extern int kaze_heap_alloc(int, int);
extern void kaze_semitrans_quad_setup(int);
extern void GsSetFlatLight(int, int *);

/**
 * kaze_title_screen_init  (menu, 0x8004c034)
 *
 * Allocates buffers (kaze_heap_alloc), loads BAL\K\TITLE.TXP, sets flat
 * light/ambient and initial title state kaze_title_screen_state=1.
 */

void kaze_title_screen_init(void)
{
    extern u8 kaze_title_screen_state;
    int *p;

    kaze_title_pressstart_sprite_buffer = kaze_heap_alloc(1, 0x38);
    D_8008E0E8 = kaze_heap_alloc(1, 0x38);
    kaze_txp_archive_load_vram(kaze_title_txp_path);
    kaze_title_screen_state = 1;
    kaze_title_xa_track_id = -2;
    kaze_title_start_skip_flag = 0;
    D_800B1A48 = 0;
    kaze_title_start_flow_state = 0;
    kaze_title_vsync_frame_delay = 0;
    kaze_title_clear_color_b = 0;
    kaze_title_clear_color_g = 0;
    kaze_title_clear_color_r = 0;
    kaze_title_press_start_cycle_count = 0;
    kaze_title_fade_blend_mode = 2;
    kaze_opening_model_loaded_flag = 0;
    kaze_semitrans_quad_setup(0);
    p = &kaze_gs_light0_dir_x;
    *p = 0x400;
    kaze_gs_light0_dir_y = 0x1000;
    kaze_gs_light0_dir_z = 0x400;
    GsSetFlatLight(0, p);
    GsSetAmbient(0x800, 0x800, 0x800);
    kaze_gs_view_pos_x = 0;
    kaze_gs_view_pos_y = 0;
    kaze_gs_view_pos_z = -0xC00;
}

extern void kaze_heap_free(int);

/**
 * kaze_title_screen_cleanup  (menu, 0x8004c150)
 *
 * Frees title buffers kaze_title_pressstart_sprite_buffer/e8 and optional
 * kaze_opening_model_file_ptr model; sets kaze_title_screen_cleanup_flag=1.
 */

void kaze_title_screen_cleanup(void)
{
    int *p;

    if (kaze_opening_model_loaded_flag != 0) {
        kaze_heap_free(kaze_opening_model_file_ptr);
    }
    p = &D_8008E0E8;
    kaze_heap_free(*p);
    kaze_heap_free(kaze_title_pressstart_sprite_buffer);
    kaze_title_pressstart_sprite_buffer = 0;
    *p = 0;
    kaze_title_screen_cleanup_flag = 1;
    return;
}

void kaze_title_menu_substate();

/**
 * kaze_title_state_render_dispatch  (menu, 0x8004c1c8)
 *
 * Dispatches per-state render kaze_title_menu_substate/c678/cb9c by title
 * state kaze_title_screen_state (1/2/3).
 */

void kaze_title_state_render_dispatch(void)
{
    extern unsigned char kaze_title_screen_state;
    switch (kaze_title_screen_state) {
    case 1:
        kaze_title_menu_substate();
        break;
    case 2:
        kaze_title_state2_substate();
        break;
    case 3:
        kaze_opening_state_dispatch();
        break;
    }
}

extern int kaze_save_slot_unpack_buffer[];

/**
 * kaze_title_model_buffers_alloc  (menu, 0x8004c248)
 *
 * Allocates and copies 5 model/OT buffers (0x14a0 bytes) from
 * kaze_save_slot_unpack_buffer into kaze_title_model_buffer_ptrs for the
 * title.
 */

void kaze_title_model_buffers_alloc(void)
{
    int i;
    int *srcs;
    int *dsts;
    int dst_raw;
    KazeTitleModelCopyBlock *dst;
    KazeTitleModelCopyBlock *src;
    KazeTitleModelCopyBlock *end;

    i = 0;
    srcs = kaze_save_slot_unpack_buffer;
    dsts = kaze_title_model_buffer_ptrs;
    do {
        dst_raw = kaze_heap_alloc(1, 0x14A0);
        *dsts = dst_raw;
        src = (KazeTitleModelCopyBlock *)*srcs;
        dst = (KazeTitleModelCopyBlock *)dst_raw;
        end = (KazeTitleModelCopyBlock *)((int)src + 0x14A0);
        do {
            *dst = *src;
            src++;
            dst++;
        } while (src != end);
        srcs++;
        i++;
        dsts++;
    } while (i < 5);
}

/**
 * kaze_title_model_buffers_free  (menu, 0x8004c2e8)
 *
 * Copies the 5 title model buffers back to kaze_save_slot_unpack_buffer and
 * frees kaze_title_model_buffer_ptrs entries.
 */

void kaze_title_model_buffers_free(void)
{
    extern unsigned char kaze_game_menu_entered_flag;
    int *buffers;
    int *originals;
    int *end_buffers;
    KazeTitleModelCopyBlock *src;
    KazeTitleModelCopyBlock *dst;
    KazeTitleModelCopyBlock *end;

    buffers = kaze_title_model_buffer_ptrs;
    originals = kaze_save_slot_unpack_buffer;
    end_buffers = buffers + 5;
    do {
        src = (KazeTitleModelCopyBlock *)*buffers;
        dst = (KazeTitleModelCopyBlock *)*originals;
        end = (KazeTitleModelCopyBlock *)((int)src + 0x14A0);
        do {
            *dst = *src;
            src++;
            dst++;
        } while (src != end);
        kaze_heap_free(*buffers);
        buffers++;
        originals++;
    } while ((int)buffers < (int)end_buffers);

    if (kaze_game_menu_entered_flag == 0) {
        kaze_task_record_index = 0;
    }
}

/**
 * kaze_title_menu_substate  (menu, 0x8004c39c)
 *
 * Title state-1 substate machine (kaze_title_start_flow_state):
 * kaze_title_check_savedata/c4bc/c4e0/c4fc for press-start handling.
 */

void kaze_title_menu_substate(int arg)
{
    extern u8 kaze_game_menu_entered_flag;
    if (kaze_game_menu_entered_flag != 0) {
        kaze_title_goto_state2();
    } else {
        switch (kaze_title_start_flow_state) {
        case 0:
            kaze_title_check_savedata();
            break;
        case 1:
            kaze_title_wait_input();
            break;
        case 0xff:
            kaze_title_goto_state2();
            break;
        }
        if (kaze_title_start_flow_state == 1) {
            kaze_title_pressstart_draw(arg);
        }
    }
}
