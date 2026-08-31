#include "types.h"
#include "kaze_syms.h"
extern int kaze_task_record_index;
extern u8 kaze_task_name_letter1_table;
extern u8 kaze_task_name_letter2_table;
extern int kaze_records_screen_prim_buffer_b;
extern short kaze_balloon_name_row_sprites_x[];
extern int kaze_task_glyph_count;
extern int kaze_records_tab_slide_timer;
extern int kaze_records_row_slide_timer;
extern int kaze_records_cursor_move_timer;
extern int kaze_records_cursor_tab;
extern int kaze_records_cursor_row;
extern signed char kaze_records_working_table_name1;
extern signed char kaze_records_working_table_name2;
extern unsigned char kaze_records_working_table_stage_tag[];
extern int kaze_gs_light0_dir_y;
extern int kaze_gs_light0_dir_z;
extern int kaze_staged_stage_variant;
extern int kaze_task_carousel_transition_amplitude;
extern int kaze_task_transition_counter;
extern void GsSetAmbient(int, int, int);
extern int D_800B17D4;
extern int kaze_menu_camera_pos_z;
extern int kaze_menu_camera_scroll;
extern void kaze_gs_view_set_pose_94c8(int, int, int, int, int, int);
typedef struct { char b[8]; } B8;
typedef struct {
    int prim;
    int pad[4];
} Slot;
typedef struct {
    int w0;
    int w1;
} Rec;
typedef struct RecordEntry { u8 a, b, c, pad; u32 score; } RecordEntry;

extern int kaze_task_menu_state;

/**
 * kaze_task_name_empty_check  (menu, 0x8005d1a0)
 *
 * At state 0x15 checks the selected task's 3-char name at
 * kaze_task_name_letter0_table+kaze_task_record_index*3: all 0xfe (empty)
 * goes to state 2 (name entry), otherwise state 99 (accept/exit).
 */

void kaze_task_name_empty_check(void)
{
    extern s8 kaze_task_name_letter0_table[];
    int count;
    int target;
    s8 *base;
    s8 *p;
    s8 *end;

    count = 0;
    if (kaze_task_menu_state == 0x15) {
        target = -2;
        base = kaze_task_name_letter0_table;
        p = base + kaze_task_record_index * 3;
        end = p + 3;
        do {
            if (*p == target) {
                count = count + 1;
            }
            p = p + 1;
        } while ((int)p < (int)end);
        if (count == 3) {
            kaze_task_menu_state = 2;
            kaze_task_transition_counter = 0;
        } else {
            kaze_task_menu_state = 0x63;
        }
    }
}

extern short kaze_balloon_entry_grid_sprite_x;
extern short kaze_balloon_entry_grid_sprite_y;
extern int kaze_task_carousel_angle_ring[];
extern int kaze_task_carousel_glyph_pos_x;
extern int kaze_task_carousel_glyph_pos_z;
extern int rsin(int);
extern int rcos(int);

/**
 * kaze_task_list_slide_out_anim  (menu, 0x8005d230)
 *
 * State-2 transition: slides the task-row and letter-grid sprites off-screen
 * over 0x20 frames of kaze_task_transition_counter, then sets state 0x29,
 * links the name letter models (kaze_task_name_letter_models_link), lays out
 * ring glyph x/z, and converts 0xfe name chars to 0xff.
 */

void kaze_task_list_slide_out_anim(int task_idx)
{
    extern signed char kaze_task_name_letter0_table[];
    extern int kaze_task_carousel_glyph_pos_y;
    int i;
    int offset;
    int counter;
    int val;
    int *angles;
    int voff;
    signed char *flags;
    int pad[2];

    i = 4;
    offset = task_idx * 100 + 0x50;
    counter = kaze_task_transition_counter;
    do {
        *(short *)((char *)kaze_balloon_name_row_sprites_x + offset) = -0x90 - counter * 8;
        i--;
        offset -= 0x14;
    } while (i >= 0);

    {
        int pf0;
        register int sub __asm__("$10");
        register int base __asm__("$9");
        register int c8 __asm__("$12");
        register int cc __asm__("$2");

        i = 0;
        pf0 = task_idx * 0xf0;
        sub = -0x1c;
        cc = kaze_task_transition_counter;
        __asm__("" : "=r"(cc) : "0"(cc));
        base = 0;
        c8 = cc * 8;
        do {
            register int j __asm__("$6");
            register int t0;
            register int subc __asm__("$7");
            register int inner __asm__("$5");
            register int iv5 __asm__("$4");

            j = 0;
            t0 = c8 - 0x5e;
            subc = sub;
            inner = 0;
            iv5 = base;
            do {
                register int v __asm__("$2");
                register int addr __asm__("$3");
                v = inner - t0;
                inner += 0xc;
                addr = iv5 + pf0;
                __asm__("" : "=r"(addr) : "0"(addr));
                j++;
                *(short *)((char *)&kaze_balloon_entry_grid_sprite_x + addr) = v - 0xa0;
                *(short *)((char *)&kaze_balloon_entry_grid_sprite_y + addr) = subc;
                iv5 += 0x10;
            } while (j < 3);
            sub += 0x18;
            i++;
            base += 0x30;
        } while (i < 5);
    }

    if (kaze_task_transition_counter < 0x20) {
        kaze_task_transition_counter = kaze_task_transition_counter + 1;
        return;
    }

    kaze_task_menu_state = 0x29;
    kaze_task_transition_counter = 0;
    i = 0;
    kaze_task_name_letter_models_link();
    if (kaze_task_glyph_count > 0) {
        angles = kaze_task_carousel_angle_ring;
        voff = 0;
        do {
            val = rsin(*angles);
            val = val * 0x600;
            if (val < 0) val += 0xfff;
            *(int *)((char *)&kaze_task_carousel_glyph_pos_x + voff) = val >> 0xc;
            val = rcos(*angles);
            val = val * 0x600;
            if (val < 0) val += 0xfff;
            angles++;
            *(int *)((char *)&kaze_task_carousel_glyph_pos_z + voff) = val >> 0xc;
            *(int *)((char *)&kaze_task_carousel_glyph_pos_y + voff) = 0;
            voff += 0x10;
            i++;
        } while (i < kaze_task_glyph_count);
    }
    i = 0;
    flags = kaze_task_name_letter0_table;
    do {
        signed char *slot = (signed char *)(kaze_task_record_index * 3 + (int)flags) + i;
        if (*slot == -2) *slot = -1;
        i++;
    } while (i < 3);
    __asm__ volatile("" : "=m"(pad));
}

/* MATCH under gcc 2.7.2-psx -O2 (616B, byte-exact). LAWFUL: pure C, no asm.
 * The variable division (ticks<<10)/duration uses native C `/`, which maspsx
 * --expand-div (adopted build-wide) emits as the checked SDIV ladder byte-for-byte.
 * Structural levers (pure C):
 *  - amb computed as its own variable BEFORE GsSetFlatLight so it homes in a
 *    callee-saved reg and the +0x300 lands in the jal delay slot.
 *  - trailing update written condition-inverted so block order matches. */
extern void GsSetFlatLight(int, void *);
extern int kaze_task_carousel_transition_ticks;
extern int kaze_task_carousel_transition_duration;
extern int kaze_task_glyph_coordinates[];
extern int kaze_gs_light0_dir_x[];
extern int kaze_menu_camera_pos_x;
extern int kaze_menu_camera_coord_flags;

/**
 * kaze_task_carousel_zoom_anim  (menu, 0x8005d474)
 *
 * State-0x29 zoom-in: ramps camera kaze_menu_camera_pos_x/e4/e8 and view
 * kaze_gs_view_set_pose_94c8 over 0x40 frames, lowers glyphs vertically into
 * the ring, ramps flat light kaze_gs_light0_dir_x and ambient, then enters
 * carousel state 0x28.
 */

void kaze_task_carousel_zoom_anim(void)
{
    extern int kaze_task_carousel_glyph_pos_y[];
    int i;
    int remaining;
    int off_a;
    int off_b;
    int res;
    int amb;
    int pad;

    (void)&pad;
    remaining = 0x40 - kaze_task_transition_counter;
    kaze_menu_camera_pos_x = remaining * 5;
    kaze_menu_camera_scroll = remaining * 0x120 / 0x40 + -0x180;
    kaze_menu_camera_pos_z = remaining * 0x18;
    kaze_menu_camera_coord_flags = 0;
    kaze_gs_view_set_pose_94c8((remaining * 2 + -0x80) * 0x10000 >> 0x10, 0, 0, 0,
                               remaining * 0xb + -0x2c0, remaining * 0x32 + -0xe00);
    i = 0;
    if (0 < kaze_task_glyph_count) {
        int *pada = kaze_task_carousel_glyph_pos_y;
        off_b = 0;
        off_a = 0;
        do {
            *(int *)((int)&kaze_task_carousel_glyph_pos_y + off_a) = remaining * -0x400 / 0x40;
            if (i == 0) {
                res = rcos((kaze_task_carousel_transition_ticks << 10) / kaze_task_carousel_transition_duration);
                pada[0] = pada[0] + kaze_task_carousel_transition_amplitude * res / 0x1000;
            }
            *(int *)((int)&kaze_task_glyph_coordinates + off_b) = 0;
            off_b = off_b + 0x50;
            off_a = off_a + 0x10;
            i = i + 1;
        } while (i < kaze_task_glyph_count);
    }
    {
        int *pflat = &kaze_gs_light0_dir_y;
        int lv = remaining * -0x1000 / 0x40 + 0x1000;
        *pflat = lv;
        kaze_gs_light0_dir_z = lv;
        amb = remaining * 0x14 + 0x300;
        GsSetFlatLight(0, (void *)(pflat - 1));
    }
    i = amb * 0x10000 >> 0x10;
    GsSetAmbient(i, i, i);
    if (kaze_task_transition_counter < 0x40) {
        kaze_task_transition_counter = kaze_task_transition_counter + 1;
        return;
    }
    kaze_task_menu_state = 0x28;
    kaze_task_transition_counter = 0;
}

extern u16 kaze_menu_camera_rot_y;
extern int kaze_menu_gs_ot_buf[];
extern void kaze_linef3_gradient_bar(int, int, int, int, int);
extern void kaze_title_model_sort_dispatch(int, int *);
extern void kaze_drmove_addprim_a0ec(int, int);

/**
 * kaze_task_list_draw  (menu, 0x8005d6dc)
 *
 * State-0 task-list draw: cursor box via kaze_linef3_gradient_bar at row
 * kaze_task_record_index, background page via
 * kaze_title_cursor_frame_draw_3e00/kaze_grid_select_frame_prims_add, name
 * grid kaze_task_name_grid_draw(,1) and row sprites
 * kaze_task_menu_row_sprites_draw(,1), kaze_balloon_switch_swing_anim, yaw
 * spin, OT clear + finish.
 */

void kaze_task_list_draw(int arg0, int arg1)
{
    u16 *p;
    kaze_linef3_gradient_bar(arg0, 0x17, kaze_task_record_index * 0x18 + 0x5B, 0x72, 0x12);
    if (arg1 == 0) {
        kaze_title_cursor_frame_draw_3e00(arg0, 0, 0);
    } else {
        kaze_grid_select_frame_prims_add(arg0, 0, 0);
    }
    kaze_task_name_grid_draw(arg0, 1);
    kaze_task_menu_row_sprites_draw(arg0, 1);
    kaze_balloon_switch_swing_anim();
    p = &kaze_menu_camera_rot_y;
    kaze_menu_camera_coord_flags = 0;
    *p = (*p + 8) & 0xFFF;
    kaze_title_model_sort_dispatch(arg0, kaze_menu_gs_ot_buf + arg0 * 5);
    kaze_drmove_addprim_a0ec(arg0, 0xFFF);
}

/**
 * kaze_task_list_slide_draw  (menu, 0x8005d7cc)
 *
 * State-2 draw variant of the task list: passes vertical slide offset
 * kaze_task_transition_counter*-2 to the background draw
 * (kaze_title_cursor_frame_draw_3e00/kaze_grid_select_frame_prims_add) and
 * draws grid/rows without repositioning while the slide-out animation runs.
 */

void kaze_task_list_slide_draw(int page, int mode)
{
    u16 *p;

    if (mode == 0) {
        if (kaze_task_transition_counter == 0) {
            kaze_title_cursor_frame_draw_3e00(page, 0, -0x40);
        } else {
            kaze_title_cursor_frame_draw_3e00(page, 0, -(kaze_task_transition_counter << 1));
        }
    } else {
        if (kaze_task_transition_counter == 0) {
            kaze_grid_select_frame_prims_add(page, 0, -0x40);
        } else {
            kaze_grid_select_frame_prims_add(page, 0, -(kaze_task_transition_counter << 1));
        }
    }
    kaze_task_name_grid_draw(page, 0);
    kaze_task_menu_row_sprites_draw(page, 0);
    kaze_balloon_switch_swing_anim();
    p = &kaze_menu_camera_rot_y;
    kaze_menu_camera_coord_flags = 0;
    *p = (*p + 8) & 0xFFF;
    kaze_title_model_sort_dispatch(page, kaze_menu_gs_ot_buf + page * 5);
    kaze_drmove_addprim_a0ec(page, 0xFFF);
}

void kaze_records_cursor_input();
void kaze_records_draw_dispatch();
void kaze_records_screen_free();

/**
 * kaze_records_screen_frame  (menu, 0x8005d8d4)
 *
 * Records-screen frame driver: at state kaze_records_screen_state==10 runs
 * cursor input kaze_records_cursor_input (unless disabled) and draw
 * kaze_records_draw_dispatch; returns true at state 99 after freeing the
 * prim buffers (kaze_records_screen_free).
 */

int kaze_records_screen_frame(int arg0, int arg1)
{
    extern int kaze_records_screen_state;
    if (kaze_records_screen_state == 10) {
        if (arg1 == 0) {
            kaze_records_cursor_input();
        }
        kaze_records_draw_dispatch(arg0);
    }
    if (kaze_records_screen_state == 0x63) {
        kaze_records_screen_free();
        return 1;
    }
    return 0;
}

extern int kaze_heap_alloc(int, int);
void kaze_records_header_prim_init();
void kaze_records_tab_prim_init();
void kaze_records_rank_prim_init();
void kaze_records_text_prim_init();

/**
 * kaze_records_screen_init  (menu, 0x8005d940)
 *
 * Records screen init: allocates two 0x628 prim buffers
 * kaze_records_screen_prim_buffer_a/kaze_records_screen_prim_buffer_b,
 * copies the 3x3x6 record table kaze_record_table_a into working buffer
 * kaze_records_working_table with a source-stage tag byte, resets cursor
 * state (kaze_records_screen_state=10), builds prims.
 */

void kaze_records_screen_init(void)
{
    extern u8 kaze_records_working_table[];
    extern u8 kaze_records_screen_state[];
    extern u8 kaze_record_table_a[];
    extern int kaze_records_screen_prim_buffer_a;
    int source_stage;
    int stage_ofs;
    int row;
    int row_ofs;
    int rank;
    int rec_ofs;
    int ofs;
    int b0;
    int b1;
    int b2;
    int packed;

    kaze_records_screen_prim_buffer_a = kaze_heap_alloc(1, 0x628);
    kaze_records_screen_prim_buffer_b = kaze_heap_alloc(1, 0x628);
    source_stage = 0;
    stage_ofs = 0;
    do {
        row = 0;
        row_ofs = 0;
        do {
            rank = 0;
            rec_ofs = row_ofs;
            do {
                ofs = rec_ofs + stage_ofs;
                b0 = kaze_record_table_a[ofs];
                b1 = kaze_record_table_a[ofs + 1];
                b2 = kaze_record_table_a[ofs + 2];
                packed = *(int *)(kaze_record_table_a + ofs + 4);
                __asm__ volatile("addiu %0,%1,1" : "=r"(rank) : "r"(rank));
                kaze_records_working_table[ofs + 3] = source_stage;
                kaze_records_working_table[ofs] = b0;
                kaze_records_working_table[ofs + 1] = b1;
                kaze_records_working_table[ofs + 2] = b2;
                *(int *)(kaze_records_working_table + ofs + 4) = packed;
                rec_ofs += 8;
            } while (rank < 6);
            row++;
            row_ofs += 0x30;
        } while (row < 3);
        source_stage++;
        stage_ofs += 0x90;
    } while (source_stage < 3);

    *(int *)(kaze_records_screen_state + 0x00) = 10;
    *(int *)(kaze_records_screen_state + 0x04) = 0;
    kaze_records_screen_state[0x08] = 0;
    kaze_records_screen_state[0x0C] = 0;
    *(int *)(kaze_records_screen_state + 0x10) = 0;
    *(int *)(kaze_records_screen_state + 0x14) = 0;
    *(int *)(kaze_records_screen_state + 0x18) = 0;
    *(int *)(kaze_records_screen_state + 0x1C) = 0;
    *(int *)(kaze_records_screen_state + 0x20) = 0;
    *(int *)(kaze_records_screen_state + 0x24) = 2;
    kaze_records_header_prim_init();
    kaze_records_tab_prim_init();
    kaze_records_rank_prim_init();
    kaze_records_text_prim_init();
}

extern void kaze_heap_free(int);

/**
 * kaze_records_screen_free  (menu, 0x8005dab0)
 *
 * Frees the two records-screen prim buffers
 * kaze_records_screen_prim_buffer_a/kaze_records_screen_prim_buffer_b via
 * heap free kaze_heap_free.
 */

void kaze_records_screen_free(void)
{
    extern int kaze_records_screen_prim_buffer_a;
    int *p = &kaze_records_screen_prim_buffer_a;

    kaze_heap_free(*p);
    kaze_heap_free(kaze_records_screen_prim_buffer_b);
    *p = 0;
    kaze_records_screen_prim_buffer_b = 0;
}

extern int kaze_input_edge;
void kaze_records_combined_sort();
extern void kaze_sfx_play_dispatch(int);

/**
 * kaze_records_cursor_input  (menu, 0x8005db00)
 *
 * Records cursor input from kaze_input_edge: Down/Up move category row
 * kaze_records_cursor_row (0..2), Right/Left move stage tab
 * kaze_records_cursor_tab (0..3) with slide timers; any d-pad on tab 3 re-
 * sorts the combined table (kaze_records_combined_sort); Cross exits (state
 * 99). Sounds 0x68/0x65.
 */

void kaze_records_cursor_input(void)
{
    extern s8 kaze_records_row_slide_dir;
    extern s8 kaze_records_tab_slide_dir;
    extern int kaze_records_screen_state;
  if ((kaze_input_edge & 0x4000) && (kaze_records_cursor_row < 2)) {
    kaze_records_cursor_row += 1;
    kaze_records_row_slide_dir = 1;
    kaze_records_row_slide_timer = 0xC;
    kaze_records_cursor_move_timer = 0xF;
    kaze_sfx_play_dispatch(0x68);
  }
  if ((kaze_input_edge & 0x1000) && (kaze_records_cursor_row > 0)) {
    kaze_records_cursor_row -= 1;
    kaze_records_row_slide_timer = 0xC;
    kaze_records_row_slide_dir = 0;
    kaze_records_cursor_move_timer = 0xF;
    kaze_sfx_play_dispatch(0x68);
  }
  if ((kaze_input_edge & 0x2000) && (kaze_records_cursor_tab < 3)) {
    kaze_records_cursor_tab += 1;
    kaze_records_tab_slide_dir = 1;
    kaze_records_tab_slide_timer = 0xC;
    kaze_records_cursor_move_timer = 0xF;
    kaze_sfx_play_dispatch(0x68);
  }
  if ((kaze_input_edge & 0x8000) && (kaze_records_cursor_tab > 0)) {
    kaze_records_cursor_tab -= 1;
    kaze_records_tab_slide_timer = 0xC;
    kaze_records_tab_slide_dir = 0;
    kaze_records_cursor_move_timer = 0xF;
    kaze_sfx_play_dispatch(0x68);
  }
  if ((kaze_input_edge & 0xF000) && (kaze_records_cursor_tab == 3)) {
    kaze_records_combined_sort(kaze_records_cursor_row);
  }
  if (kaze_input_edge & 0x40) {
    kaze_records_screen_state = 0x63;
    kaze_sfx_play_dispatch(0x65);
  }
}

void kaze_records_entry_glyph_update();
void kaze_records_header_draw();
void kaze_records_rank_labels_draw();
void kaze_records_entries_draw();
void kaze_records_cursor_tab_draw();

/**
 * kaze_records_draw_dispatch  (menu, 0x8005dcf4)
 *
 * Records draw dispatcher: ticks the roll-in timer
 * kaze_records_cursor_move_timer then runs entry glyph update
 * kaze_records_entry_glyph_update, header kaze_records_header_draw, rank
 * labels kaze_records_rank_labels_draw, entries kaze_records_entries_draw,
 * cursor/tabs kaze_records_cursor_tab_draw, and kaze_drmove_addprim_a0ec.
 */

void kaze_records_draw_dispatch(int page)
{
    if (kaze_records_cursor_move_timer != 0) {
        kaze_records_cursor_move_timer = kaze_records_cursor_move_timer + -1;
    }
    kaze_records_entry_glyph_update(page);
    kaze_records_header_draw(page);
    kaze_records_rank_labels_draw(page);
    kaze_records_entries_draw(page);
    kaze_records_cursor_tab_draw(page);
    kaze_drmove_addprim_a0ec(page, 0xFFF);
    return;
}

extern int kaze_record_table_score;

/**
 * kaze_records_entry_glyph_update  (menu, 0x8005dd64)
 *
 * Updates glyph UVs for the 6 visible record entries: 3-char name glyphs
 * plus the value from
 * kaze_record_table_score/kaze_records_working_table_score formatted per
 * category row (row0/1 hundreds+fraction digits via
 * kaze_uint_to_decimal_string, row2 mm:ss capped 99:59).
 */

void kaze_records_entry_glyph_update(int page)
{
    extern signed char kaze_record_table_c;
    extern signed char kaze_record_table_b;
    extern signed char kaze_record_table_a;
    extern int kaze_records_working_table_score;
    extern signed char kaze_records_working_table;
    extern int kaze_records_screen_prim_buffer_a[];
    int val;
    int a;
    int b;
    int c;
    int t;
    int gv;
    int tl;
    int cnt;
    int ofsv;
    uint g;
    int ofs;
    register int d8 __asm__("$4");
    int d4;
    int base;
    int rowA;
    int rowB;
    u8 *pA;
    register u8 *pB __asm__("$7");
    register u8 *pC __asm__("$6");
    int entry_i;
    int entry_ofs;
    int *entry_ptr;
    int sym304;
    int c120;
    int c32;
    u8 *plocal;
    u8 digit_buf[8];

    entry_i = 0;
    sym304 = (int)kaze_records_screen_prim_buffer_a;
    entry_ptr = (int *)(page * 4 + sym304);
    c120 = 120;
    __asm__("" : "=r"(c120) : "0"(c120));
    plocal = digit_buf;
    __asm__("" : "=r"(plocal) : "0"(plocal));
    __asm__("" :: "r"(plocal));
    __asm__("" :: "r"(plocal));
    __asm__("" :: "r"(plocal));
    __asm__("" :: "r"(plocal));
    __asm__("" :: "r"(plocal));
    c32 = 32;
    __asm__("" : "=r"(c32) : "0"(c32));
    __asm__("" :: "r"(c32));
    __asm__("" :: "r"(c32));
    entry_ofs = 0;
L_outer:
    {
        ofs = entry_i * 8;
        d4 = kaze_records_cursor_tab;
        if (d4 == 3) {
            d8 = kaze_records_cursor_row;
            ofs += d8 * 0x30;
            a = (&kaze_records_working_table)[ofs];
            b = (&kaze_records_working_table_name1)[ofs];
            c = (&kaze_records_working_table_name2)[ofs];
            val = *(int *)((char *)&kaze_records_working_table_score + ofs);
        } else {
            d8 = kaze_records_cursor_row;
            ofs += d8 * 0x30;
            ofs += d4 * 0x90;
            a = (&kaze_record_table_a)[ofs];
            b = (&kaze_record_table_b)[ofs];
            c = (&kaze_record_table_c)[ofs];
            val = *(int *)((char *)&kaze_record_table_score + ofs);
        }
        if (a != -1) {
            int dst;
            t = a;
            __asm__("" : "=r"(t) : "0"(t));
            dst = entry_ofs + *entry_ptr;
            if (a < 0) {
                t = a + 15;
            }
            t = (t >> 4) << 4;
            *(char *)(dst + 0x26c) = (a - t) * 8;
            *(char *)(entry_ofs + *entry_ptr + 0x26d) = t;
        } else {
            *(char *)(entry_ofs + *entry_ptr + 0x26c) = c120;
            *(char *)(entry_ofs + *entry_ptr + 0x26d) = c32;
        }
        if (b != -1) {
            int dst;
            t = b;
            __asm__("" : "=r"(t) : "0"(t));
            dst = entry_ofs + *entry_ptr;
            if (b < 0) {
                t = b + 15;
            }
            t = (t >> 4) << 4;
            *(char *)(dst + 0x280) = (b - t) * 8;
            *(char *)(entry_ofs + *entry_ptr + 0x281) = t;
        } else {
            *(char *)(entry_ofs + *entry_ptr + 0x280) = c120;
            __asm__ volatile("" ::: "memory");
            *(char *)(entry_ofs + *entry_ptr + 0x281) = 32;
        }
        if (c != -1) {
            int dst;
            t = c;
            __asm__("" : "=r"(t) : "0"(t));
            dst = entry_ofs + *entry_ptr;
            if (c < 0) {
                t = c + 15;
            }
            t = (t >> 4) << 4;
            *(char *)(dst + 0x294) = (c - t) * 8;
            *(char *)(entry_ofs + *entry_ptr + 0x295) = t;
        } else {
            *(char *)(entry_ofs + *entry_ptr + 0x294) = c120;
            __asm__ volatile("" ::: "memory");
            *(char *)(entry_ofs + *entry_ptr + 0x295) = 32;
        }
        if (kaze_records_cursor_row == 1) goto Ld1;
        if (kaze_records_cursor_row < 2) {
            if (kaze_records_cursor_row == 0) goto Ld0;
        } else {
            if (kaze_records_cursor_row == 2) goto Ld2;
        }
        goto Lnext;
Ld0:
        {
            {
                if (9999 < val) {
                    val = 9999;
                }
                gv = val / 100;
                kaze_uint_to_decimal_string(gv, 2, 1, digit_buf);
                cnt = 3;
                rowA = entry_ofs;
                pA = plocal;
                ofsv = 0x3c;
                do {
                    if (gv != 0 && (g = *pA) != c32) {
                        int dst;
                        gv = g - 0x16;
                        dst = ofsv + (rowA + *entry_ptr);
                        tl = gv;
                        if (gv < 0) {
                            tl = g - 7;
                        }
                        tl = tl >> 4;
                        tl = tl << 4;
                        *(char *)(dst + 0x26c) = (gv - tl) * 8;
                        *(char *)(ofsv + (rowA + *entry_ptr) + 0x26d) = tl;
                    } else {
                        *(char *)(ofsv + (rowA + *entry_ptr) + 0x26c) = c120;
                        __asm__ volatile("" ::: "memory");
                        *(char *)(ofsv + (rowA + *entry_ptr) + 0x26d) = c32;
                    }
                    ofsv += 0x14;
                    cnt += 1;
                    pA += 1;
                } while (cnt < 5);
                if (gv != 0) {
                    register int c56 __asm__("$2") = 0x38;
                    int o5 = cnt * 0x14;
                    __asm__ volatile("" ::: "memory");
                    *(char *)(o5 + (entry_ofs + *entry_ptr) + 0x26c) = c56;
                    *(char *)(o5 + (entry_ofs + *entry_ptr) + 0x26d) = 0x20;
                } else {
                    __asm__ volatile("" ::: "memory");
                    *(char *)(cnt * 0x14 + (entry_ofs + *entry_ptr) + 0x26c) = c120;
                    __asm__ volatile("" ::: "memory");
                    *(char *)(cnt * 0x14 + (entry_ofs + *entry_ptr) + 0x26d) = 0x20;
                }
                gv = val / 100;
                kaze_uint_to_decimal_string(val - gv * 100, 2, 1, digit_buf);
                cnt += 1;
                if (cnt < 8) {
                    rowA = entry_ofs;
                    pA = plocal;
                    ofsv = cnt * 0x14;
                    do {
                        g = *pA;
                        if (g != c32) {
                            int dst;
                            gv = g - 0x16;
                            dst = ofsv + (rowA + *entry_ptr);
                            tl = gv;
                            if (gv < 0) {
                                tl = g - 7;
                            }
                            tl = tl >> 4;
                            tl = tl << 4;
                            *(char *)(dst + 0x26c) = (gv - tl) * 8;
                            *(char *)(ofsv + (rowA + *entry_ptr) + 0x26d) = tl;
                        } else {
                            *(char *)(ofsv + (rowA + *entry_ptr) + 0x26c) = c120;
                            __asm__ volatile("" ::: "memory");
                            *(char *)(ofsv + (rowA + *entry_ptr) + 0x26d) = c32;
                        }
                        ofsv += 0x14;
                        cnt += 1;
                        pA += 1;
                    } while (cnt < 8);
                }
            }
        }
        goto Lnext;
Ld1:
        {
            if (9999 < val) {
                val = 9999;
            }
            kaze_uint_to_decimal_string(val / 100, 2, 1, digit_buf);
            cnt = 3;
            rowA = entry_ofs;
            pA = plocal;
            ofsv = 0x3c;
            do {
                g = *pA;
                if (g != c32) {
                    int dst;
                    gv = g - 0x16;
                    dst = ofsv + (rowA + *entry_ptr);
                    tl = gv;
                    if (gv < 0) {
                        tl = g - 7;
                    }
                    tl = tl >> 4;
                    tl = tl << 4;
                    *(char *)(dst + 0x26c) = (gv - tl) * 8;
                    *(char *)(ofsv + (rowA + *entry_ptr) + 0x26d) = tl;
                } else {
                    *(char *)(ofsv + (rowA + *entry_ptr) + 0x26c) = c120;
                    __asm__ volatile("" ::: "memory");
                    *(char *)(ofsv + (rowA + *entry_ptr) + 0x26d) = c32;
                }
                ofsv += 0x14;
                cnt += 1;
                pA += 1;
            } while (cnt < 5);
            *(char *)(cnt * 0x14 + (entry_ofs + *entry_ptr) + 0x26c) = 0x20;
            *(char *)(cnt * 0x14 + (entry_ofs + *entry_ptr) + 0x26d) = 0x20;
            gv = val / 100;
            kaze_uint_to_decimal_string(val - gv * 100, 2, 0, digit_buf);
            cnt += 1;
            if (cnt < 8) {
                rowB = entry_ofs;
                pB = plocal;
                ofsv = cnt * 0x14;
                do {
                    int dst;
                    g = *pB;
                    gv = g - 0x16;
                    dst = ofsv + (rowB + *entry_ptr);
                    tl = gv;
                    if (gv < 0) {
                        tl = g - 7;
                    }
                    cnt += 1;
                    tl = tl >> 4;
                    tl = tl << 4;
                    *(char *)(dst + 0x26c) = (gv - tl) * 8;
                    pB += 1;
                    *(char *)(ofsv + (rowB + *entry_ptr) + 0x26d) = tl;
                    ofsv += 0x14;
                } while (cnt < 8);
            }
        }
        goto Lnext;
Ld2:
        {
            if (5999 < val) {
                val = 5999;
            }
            kaze_uint_to_decimal_string(val / 0x3c, 2, 0, digit_buf);
            cnt = 3;
            rowB = entry_ofs;
            pC = plocal;
            ofsv = 0x3c;
            do {
                int dst;
                g = *pC;
                gv = g - 0x16;
                dst = ofsv + (rowB + *entry_ptr);
                tl = gv;
                if (gv < 0) {
                    tl = g - 7;
                }
                cnt += 1;
                tl = tl >> 4;
                tl = tl << 4;
                *(char *)(dst + 0x26c) = (gv - tl) * 8;
                pC += 1;
                *(char *)(ofsv + (rowB + *entry_ptr) + 0x26d) = tl;
                ofsv += 0x14;
            } while (cnt < 5);
            {
                register int dsep __asm__("$3");
                dsep = cnt * 0x14 + (entry_ofs + *entry_ptr);
                *(char *)(dsep + 0x26c) = 0x40;
                *(char *)(cnt * 0x14 + (entry_ofs + *entry_ptr) + 0x26d) = 0x20;
            }
            gv = val / 0x3c;
            kaze_uint_to_decimal_string(val - gv * 0x3c, 2, 0, digit_buf);
            cnt += 1;
            if (cnt < 8) {
                rowB = entry_ofs;
                pB = plocal;
                ofsv = cnt * 0x14;
                do {
                    int dst;
                    g = *pB;
                    gv = g - 0x16;
                    dst = ofsv + (rowB + *entry_ptr);
                    tl = gv;
                    if (gv < 0) {
                        tl = g - 7;
                    }
                    tl = (tl >> 4) << 4;
                    *(char *)(dst + 0x26c) = (gv - tl) * 8;
                    *(char *)(ofsv + (rowB + *entry_ptr) + 0x26d) = tl;
                    __asm__ volatile("" :: "r"(pB), "r"(cnt), "r"(ofsv), "r"(gv), "r"(c32));
                    ofsv += 0x14;
                    cnt += 1;
                    pB += 1;
                } while (cnt < 8);
            }
        }
Lnext:
        entry_i += 1;
        entry_ofs += 0xa0;
    }
    if (entry_i < 6) goto L_outer;
    return;
}

extern void SetSprt(void *);
extern void SetShadeTex(void *, int);
extern void SetDrawTPage(void *, int, int, unsigned int);

/**
 * kaze_records_header_prim_init  (menu, 0x8005e5cc)
 *
 * One-time init of the records-screen header SPRTs and DR_TPAGE inside both
 * kaze_records_screen_prim_buffer_a prim buffers (CLUTs at 0x390/0x3a0 x
 * 0xf5, TPage 0x2c0).
 */

void kaze_records_header_prim_init(void)
{
    extern u16 GetTPage(int, int, int, int);
    extern u16 GetClut(int, int);
    extern int kaze_records_screen_prim_buffer_a[];
    int i;
    int *base;
    int ofs;
    int c40;
    int c80;
    int *p;

    i = 0;
    c40 = 0x40;
    c80 = 0x80;
    do {
        base = kaze_records_screen_prim_buffer_a;
        ofs = i * 4;
        p = (int *)(ofs + (int)base);
        i = i + 1;
        SetSprt((void *)*p);
        SetShadeTex((void *)*p, 1);
        *(s16 *)(*p + 8) = -0xa0;
        *(s16 *)(*p + 10) = -0x78;
        *(s16 *)(*p + 0x10) = c40;
        *(s16 *)(*p + 0x12) = c40;
        *(u8 *)(*p + 0xc) = c40;
        *(u8 *)(*p + 0xd) = c80;
        *(u16 *)(*p + 0xe) = GetClut(0x390, 0xf5);
        SetSprt((void *)(*p + 0x14));
        SetShadeTex((void *)(*p + 0x14), 1);
        *(s16 *)(*p + 0x1c) = -0x60;
        *(s16 *)(*p + 0x1e) = -0x58;
        *(s16 *)(*p + 0x24) = c80;
        *(s16 *)(*p + 0x26) = 0x20;
        *(u8 *)(*p + 0x20) = 0;
        *(u8 *)(*p + 0x21) = 0xc0;
        *(u16 *)(*p + 0x22) = GetClut(0x3a0, 0xf5);
        SetDrawTPage((void *)(*p + 0x28), 0, 1, GetTPage(0, 0, 0x2c0, 0x100));
    } while (i < 2);
}

extern B8 kaze_records_tab_underline_w_table;
extern B8 kaze_records_tab_underline_h_table;
extern void SetTile(void *);
extern void SetSemiTrans(void *, int);

/**
 * kaze_records_tab_prim_init  (menu, 0x8005e738)
 *
 * Inits the records tab/label prims in both buffers: 4 stage-tab SPRTs
 * (+0x4c), 3 category-row SPRTs (+0xa4), a semi-trans cursor TILE (+0xe8),
 * and 4 underline TILEs (+0x220) sized from the entries in
 * kaze_records_tab_underline_w_table and kaze_records_tab_underline_h_table.
 */

void kaze_records_tab_prim_init(void)
{
    extern u16 GetTPage(int, int, int, int);
    extern u16 GetClut(int, int);
    extern int kaze_records_screen_prim_buffer_a[];
    union { B8 h[2]; u16 s[8]; } tab;
    int n;
    int *p;
    u16 clut;
    int k28;

    tab.h[0] = kaze_records_tab_underline_w_table;
    tab.h[1] = kaze_records_tab_underline_h_table;

    __asm__("addiu %0,$0,0x28" : "=r"(k28));
    p = kaze_records_screen_prim_buffer_a;
    n = 0;
    do {
        int i = 0;
        int c1 = 0;
        int a1 = 0x4c;
        int o1 = 0;
        int c2, a2, o2, s5;
        int a3, o3;
        int *q, *r;

        SetSprt((void *)(*p + 0x30));
        SetShadeTex((void *)(*p + 0x30), 1);
        *(s16 *)(*p + 0x40) = k28;
        *(s16 *)(*p + 0x42) = k28;
        *(u16 *)(*p + 0x3e) = GetClut(0x340, 0xf5);
        SetDrawTPage((void *)(*p + 0x44), 0, 1, GetTPage(0, 0, 0x2c0, 0x100));
        do {
            SetSprt((void *)(*p + a1));
            SetShadeTex((void *)(*p + a1), 1);
            *(s16 *)(o1 + *p + 0x54) = -0xa0;
            *(s16 *)(o1 + *p + 0x56) = -0x78;
            i += 1;
            *(s16 *)(o1 + *p + 0x5c) = k28;
            *(s16 *)(o1 + *p + 0x5e) = k28;
            *(s8 *)(o1 + *p + 0x58) = 0;
            *(s8 *)(o1 + *p + 0x59) = c1;
            clut = GetClut(0x350, 0xf5);
            a1 += 0x14;
            c1 += 0x28;
            *(u16 *)(*p + o1 + 0x5a) = clut;
            o1 += 0x14;
        } while (i < 4);
        SetDrawTPage((void *)(*p + 0x9c), 0, 1, GetTPage(0, 0, 0x2c0, 0x100));
        i = 0;
        q = p;
        s5 = 0x360;
        c2 = 0;
        a2 = 0xa4;
        o2 = 0;
        do {
            SetSprt((void *)(*q + a2));
            SetShadeTex((void *)(*q + a2), 1);
            i += 1;
            *(s16 *)(o2 + *q + 0xac) = -0xa0;
            *(s16 *)(o2 + *q + 0xae) = -0x78;
            *(s16 *)(o2 + *q + 0xb4) = k28;
            *(s16 *)(o2 + *q + 0xb6) = k28;
            *(s8 *)(o2 + *q + 0xb0) = k28;
            *(s8 *)(o2 + *q + 0xb1) = c2;
            clut = GetClut(s5, 0xf5);
            s5 += 0x10;
            a2 += 0x14;
            c2 += 0x28;
            *(u16 *)(*q + o2 + 0xb2) = clut;
            o2 += 0x14;
        } while (i < 3);
        SetDrawTPage((void *)(*p + 0xe0), 0, 1, GetTPage(0, 0, 0x2c0, 0x100));
        SetTile((void *)(*p + 0xe8));
        SetSemiTrans((void *)(*p + 0xe8), 1);
        *(s16 *)(*p + 0xf0) = 0x1c;
        *(s16 *)(*p + 0xf2) = -0x20;
        *(s16 *)(*p + 0xf4) = 0x78;
        i = 0;
        *(s16 *)(*p + 0xf6) = 0x78;
        r = p;
        *(s8 *)(*p + 0xec) = 0;
        a3 = 0x220;
        *(s8 *)(*p + 0xed) = 0;
        *(s8 *)(*p + 0xee) = 0x20;
        do {
            SetTile((void *)(*r + a3));
            SetSemiTrans((void *)(*r + a3), 1);
            o3 = i * 0x10;
            *(s16 *)(o3 + *r + 0x22c) = tab.s[i];
            *(s16 *)(o3 + *r + 0x22e) = tab.s[i + 4];
            *(s8 *)(o3 + *r + 0x224) = 0;
            a3 += 0x10;
            *(s8 *)(o3 + *r + 0x225) = 0;
            i += 1;
            *(s8 *)(o3 + *r + 0x226) = 0;
        } while (i < 4);
        n += 4;
        *(s16 *)(*p + 0x248) = 0x94;
        *(s16 *)(*p + 0x24a) = -0x18;
        *(s16 *)(*p + 0x258) = 0x24;
        *(s16 *)(*p + 0x25a) = 0x58;
        p += 1;
    } while (n < 8);
}

extern void SetSprt16(void *);

/**
 * kaze_records_rank_prim_init  (menu, 0x8005eb54)
 *
 * Inits 3 columns x 6 rows of SPRT_16 rank/label sprites
 * (+0xf8/+0x158/+0x1b8) in both records buffers with CLUTs 0x3e0/0x3b0/0x3c0
 * and a DR_TPAGE.
 */

void kaze_records_rank_prim_init(void)
{
    extern int GetTPage(int, int, int, int);
    extern s16 GetClut(int, int);
    extern int kaze_records_screen_prim_buffer_a[];
    int n;
    int *p;
    int i;
    int off;
    int y;
    int s4;
    int s5;
    int s6;

    p = kaze_records_screen_prim_buffer_a;
    n = 0;
    do {
        i = 0;
        s6 = 0x1B8;
        s5 = 0x158;
        y = -0x19;
        s4 = 0xF8;
        do {
            SetSprt16((void *)(*p + s4));
            SetShadeTex((void *)(*p + s4), 1);
            off = i << 4;
            *(s16 *)(off + *p + 0x100) = 0x1C;
            *(s16 *)(off + *p + 0x102) = y;
            *(s8 *)(off + *p + 0x104) = ((i % 4) << 4) + 0x40;
            *(s8 *)(off + *p + 0x105) = ((i / 4) << 4) - 0x20;
            *(s16 *)(*p + off + 0x106) = GetClut(0x3E0, 0xF5);
            s4 += 0x10;
            SetSprt16((void *)(*p + s5));
            SetShadeTex((void *)(*p + s5), 1);
            *(s16 *)(off + *p + 0x160) = 0x76;
            *(s16 *)(off + *p + 0x162) = y;
            i += 1;
            *(s16 *)(*p + off + 0x166) = GetClut(0x3B0, 0xF5);
            s5 += 0x10;
            SetSprt16((void *)(*p + s6));
            SetShadeTex((void *)(*p + s6), 1);
            *(s16 *)(off + *p + 0x1C0) = 0x84;
            *(s16 *)(off + *p + 0x1C2) = y;
            *(s16 *)(*p + off + 0x1C6) = GetClut(0x3C0, 0xF5);
            s6 += 0x10;
            y += 0x12;
        } while (i < 6);
        SetDrawTPage((void *)(*p + 0x218), 0, 1, GetTPage(0, 0, 0x2C0, 0x100) & 0xFFFF);
        p += 1;
        n += 4;
    } while (n < 8);
}

/**
 * kaze_records_text_prim_init  (menu, 0x8005ed68)
 *
 * Inits the 6 rows x 8 entry-text SPRTs (+0x260, 8x16 glyph cells, CLUT
 * 0x3d0, name chars then value digits) plus the final DR_TPAGE in both
 * records buffers.
 */

void kaze_records_text_prim_init(void)
{
    extern int GetTPage(int, int, int, int);
    extern u16 GetClut(int, int);
    extern int kaze_records_screen_prim_buffer_a[];
    u16 clut;
    int row_count;
    int y;
    int buffer_count;
    int prim_copy;
    register int *p __asm__("$17");

    p = kaze_records_screen_prim_buffer_a;
    buffer_count = 0;
    do {
        register int prim_base __asm__("$23");
        register int row_base __asm__("$20");

        row_count = 0;
        __asm__("addiu %0,$0,-25" : "=r"(y));
        prim_base = 0x260;
        row_base = 0;
        do {
            int i;
            register int off __asm__("$16");
            register int x __asm__("$19");

            i = 0;
            {
                register int clut_off __asm__("$21");

                clut_off = row_base;
                x = 0x2a;
                off = 0;
	                do {
	                register int clut_arg __asm__("$4");
	                register int clut_y __asm__("$5");
	                SetSprt((void *)(*p + prim_base + off));
	                SetShadeTex((void *)(*p + prim_base + off), 1);
                { int first_base = *p;
                __asm__("" : "=r"(off), "=r"(i)
                        : "0"(off), "1"(i), "r"(x), "r"(first_base));
                i += 1;
                *(s16 *)(off + (row_base + first_base) + 0x268) = x;
                }
	                clut_arg = 0x3d0;
                *(s16 *)(off + (row_base + *p) + 0x26a) = (u16)y;
                { register int width __asm__("$2") = 8;
                *(s16 *)(off + (row_base + *p) + 0x270) = width;
                }
                { register int height __asm__("$2") = 0x10;
                *(s16 *)(off + (row_base + *p) + 0x272) = height;
                }
                { register int clear0_base __asm__("$2") = *p;
                clut_y = 0xf5;
                clear0_base = row_base + clear0_base;
                clear0_base = off + clear0_base;
                *(s8 *)(clear0_base + 0x26c) = 0;
                }
                __asm__ volatile("" : "=r"(off) : "0"(off));
                { register int clear1_base __asm__("$2") = *p;
                x += 9;
                clear1_base = row_base + clear1_base;
                clear1_base = off + clear1_base;
                *(s8 *)(clear1_base + 0x26d) = 0;
                }
                off += 0x14;
	                clut = GetClut(clut_arg, clut_y);
                *(u16 *)(*p + clut_off + 0x26e) = clut;
                    clut_off += 0x14;
                } while (i < 3);
            }
            if (i < 8) {
                int row_copy;
                register int clut_off2 __asm__("$22");
                register int x2 __asm__("$21");
                int off2;
                register int scaled __asm__("$3");
                register int x_scaled __asm__("$2");

                scaled = i << 2;
                row_copy = row_base;
                prim_copy = prim_base;
                scaled += i;
                scaled <<= 2;
                clut_off2 = scaled + row_base;
                x_scaled = i * 9;
                x2 = x_scaled + 0x2e;
                off2 = scaled;
                do {
                    register int clut_arg2 __asm__("$4");
                    SetSprt((void *)(*p + prim_copy + off2));
                    __asm__("":"=r"(prim_copy):"0"(prim_copy));
                    SetShadeTex((void *)(*p + prim_copy + off2), 1);
                    { int first_base = *p;
                    __asm__("" : "=r"(off2), "=r"(i)
                            : "0"(off2), "1"(i), "r"(x2), "r"(first_base));
                    i += 1;
                    *(s16 *)(off2 + (row_copy + first_base) + 0x268) = x2;
                    }
                    clut_arg2 = 0x3d0;
                    *(s16 *)(off2 + (row_copy + *p) + 0x26a) = (u16)y;
                    { register int width_base __asm__("$3") = *p;
                    register int width2 __asm__("$2") = 8;
                    width_base = row_copy + width_base;
                    width_base = off2 + width_base;
                    *(s16 *)(width_base + 0x270) = width2;
                    }
                    { int height_base = row_copy + *p;
                    *(s16 *)(off2 + height_base + 0x272) = 0x10;
                    }
                    *(s8 *)(off2 + (row_copy + *p) + 0x26c) = 0;
                    x2 += 9;
                    *(s8 *)(off2 + (row_copy + *p) + 0x26d) = 0;
                    off2 += 0x14;
                    clut = GetClut(clut_arg2, 0xf5);
                    *(u16 *)(*p + clut_off2 + 0x26e) = clut;
                    clut_off2 += 0x14;
                } while (i < 8);
            }
            y += 0x12;
            prim_base += 0xa0;
            row_count += 1;
            row_base += 0xa0;
        } while (row_count < 6);
        clut = GetTPage(0, 0, 0x300, 0x100);
        SetDrawTPage((void *)(*p + 0x620), 0, 1, clut);
        buffer_count += 4;
        p += 1;
    } while (buffer_count < 8);
}

/**
 * kaze_records_header_draw  (menu, 0x8005f02c)
 *
 * AddPrims the three records-screen header prims (+0x0/+0x14/+0x28 of the
 * buffer) to the OT (&kaze_menu_gs_ot_buf0_org)[buf*5].
 */

void kaze_records_header_draw(int idx)
{
    extern int AddPrim(void *, void *);
    extern int kaze_records_screen_prim_buffer_a[];
    extern Slot kaze_menu_gs_ot_buf0_org[];
    int *p;

    p = &kaze_records_screen_prim_buffer_a[idx];
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[idx].prim, (void *)*p);
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[idx].prim, (void *)(*p + 0x14));
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[idx].prim, (void *)(*p + 0x28));
}

/**
 * kaze_records_cursor_tab_draw  (menu, 0x8005f0b8)
 *
 * Animates the records cursor cell slide (rsin easing on timers
 * kaze_records_tab_slide_timer/9c with directions
 * kaze_records_tab_slide_dir/94), positions the cursor tile, stage tabs and
 * category labels relative to selection kaze_records_cursor_tab/adea8, and
 * AddPrims them.
 */

void kaze_records_cursor_tab_draw(int page)
{
    extern void AddPrim(void *, void *);
    extern u8 kaze_records_row_slide_dir;
    extern u8 kaze_records_tab_slide_dir;
    extern char kaze_menu_gs_ot_buf0_org[];
    extern int kaze_records_screen_prim_buffer_a[];
    register int p asm("$23") = page;
    int cursor_x;
    int cursor_y;
    int row_ofs;
    int *rec_ptr;
    int i;
    int prim_ofs;
    short row_y;
    int ofs5;

    if (kaze_records_tab_slide_timer != 0) {
        int t = kaze_records_tab_slide_timer - 1;
        int q;
        register int d __asm__("$3");
        int base;
        int n;
        kaze_records_tab_slide_timer = t;
        q = ((0xc - t) << 10) / 12;
        if (kaze_records_tab_slide_dir != 0) {
            q = rsin(q);
            d = kaze_records_cursor_tab;
            q = q * 0x28;
            d = d - 1;
            base = d * 0x28;
            if (q < 0) {
                q += 0xfff;
            }
            n = (q >> 12) + 0xc;
            cursor_x = base + n;
        } else {
            q = rsin(q);
            d = kaze_records_cursor_tab;
            q = q * 0x28;
            d = d + 1;
            base = d * 0x28;
            if (q < 0) {
                q += 0xfff;
            }
            n = (q >> 12) - 0xc;
            cursor_x = base - n;
        }
    } else {
        cursor_x = kaze_records_cursor_tab * 0x28 + 0xc;
    }

    if (kaze_records_row_slide_timer != 0) {
        int t = kaze_records_row_slide_timer - 1;
        int q;
        register int d __asm__("$3");
        int base;
        int n;
        kaze_records_row_slide_timer = t;
        q = ((0xc - t) << 10) / 12;
        if (kaze_records_row_slide_dir != 0) {
            q = rsin(q);
            d = kaze_records_cursor_row;
            q = q * 0x28;
            d = d - 1;
            base = d * 0x28;
            if (q < 0) {
                q += 0xfff;
            }
            n = (q >> 12) + 0x58;
            cursor_y = base + n;
        } else {
            q = rsin(q);
            d = kaze_records_cursor_row;
            q = q * 0x28;
            d = d + 1;
            base = d * 0x28;
            if (q < 0) {
                q += 0xfff;
            }
            n = (q >> 12) - 0x58;
            cursor_y = base - n;
        }
    } else {
        cursor_y = kaze_records_cursor_row * 0x28 + 0x58;
    }

    i = 0;
    {
        int *base = kaze_records_screen_prim_buffer_a;
        int p4 = p * 4;
        int *tv = (int *)((char *)base + p4);
        int ov = (p4 + p) * 4;
        rec_ptr = tv;
        ofs5 = ov;
        __asm__("" :: "r"(tv), "r"(ov));
    }
    prim_ofs = 0x4c;
    row_y = -0x94;
    row_ofs = 0;

    *(short *)(*rec_ptr + 0x38) = cursor_x - 0xa0;
    {
        int q = *rec_ptr;
        int sel1 = kaze_records_cursor_tab;
        *(short *)(q + 0x3a) = cursor_y - 0x78;
        *(u8 *)(*rec_ptr + 0x3c) = sel1 * 0x28 + 0x50;
        *(u8 *)(*rec_ptr + 0x3d) = kaze_records_cursor_row * 0x28;
    }

    AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ofs5), (void *)(*rec_ptr + 0x30));
    AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ofs5), (void *)(*rec_ptr + 0x44));
    do {
        __asm__("" : "=r"(cursor_y) : "0"(cursor_y));
        i = i + 1;
        *(short *)(row_ofs + *rec_ptr + 0x54) = row_y;
        row_y = row_y + 0x28;
        *(short *)(row_ofs + *rec_ptr + 0x56) = cursor_y - 0x78;
        row_ofs = row_ofs + 0x14;
        AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ofs5), (void *)(*rec_ptr + prim_ofs));
        prim_ofs = prim_ofs + 0x14;
    } while (i < 4);

    i = 0;
    {
        int *tv = kaze_records_screen_prim_buffer_a + p;
        int ov = p * 0x14;
        rec_ptr = tv;
        ofs5 = ov;
        __asm__("" :: "r"(tv), "r"(ov));
    }
    prim_ofs = 0xa4;
    row_y = -0x20;
    row_ofs = 0;
    AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ofs5), (void *)(*rec_ptr + 0x9c));
    do {
        __asm__("" : "=r"(cursor_x) : "0"(cursor_x) : "memory");
        *(short *)(row_ofs + *rec_ptr + 0xac) = cursor_x - 0xa0;
        i = i + 1;
        *(short *)(row_ofs + *rec_ptr + 0xae) = row_y;
        row_y = row_y + 0x28;
        row_ofs = row_ofs + 0x14;
        AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ofs5), (void *)(*rec_ptr + prim_ofs));
        prim_ofs = prim_ofs + 0x14;
    } while (i < 3);

    i = 0;
    {
        register int p3_work asm("$3") = p * 4;
        int tofs3 = (p3_work + p) * 4;
        int ofs3 = tofs3;
        register int *p3 asm("$16");
        register int *p3_sym asm("$2");
        int cur3;
        __asm__ __volatile__("" :: "r"(tofs3) : "memory");
        p3_sym = kaze_records_screen_prim_buffer_a;
        p3_work = (int)p3_sym + p3_work;
        __asm__("" : "=r"(p3_work) : "0"(p3_work));
        p3 = (int *)p3_work;
        __asm__("" :: "r"(p3));
        cur3 = 0x220;
        __asm__("" :: "r"(p3_work));
        AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ofs3), (void *)(*p3 + 0xe8));
        *(short *)(*p3 + 0x228) = cursor_x - 0x98;
        *(short *)(*p3 + 0x22a) = -0x18;
        *(short *)(*p3 + 0x238) = -0x8c;
        *(short *)(*p3 + 0x23a) = cursor_y - 0x70;
        do {
            i = i + 1;
            AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ofs3), (void *)(*p3 + cur3));
            cur3 = cur3 + 0x10;
        } while (i < 4);
    }

    {
        int p4 = p * 4;
        register int ofs __asm__("$3") = (p4 + p) * 4;
        int *tp = (int *)((char *)kaze_records_screen_prim_buffer_a + p4);
        AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ofs), (void *)(*tp + 0xe0));
    }
}

/**
 * kaze_records_rank_labels_draw  (menu, 0x8005f558)
 *
 * Draws the rank-number and label SPRT_16 columns with roll-in y offsets
 * from timer kaze_records_cursor_move_timer; on the combined tab
 * (kaze_records_cursor_tab==3) tints each entry by its source-stage tag byte
 * kaze_records_working_table_stage_tag.
 */

void kaze_records_rank_labels_draw(int page)
{
    extern void AddPrim(void *, void *);
    extern int kaze_menu_gs_ot_buf0_org[];
    extern int kaze_records_screen_prim_buffer_a[];
  int prim_ofs;
  register int role8 asm("$21");
  int * p;
  register int i asm("$18");
  register int field_ofs asm("$16");
  int new_var;
  int t;
  int q;
  s16 false_val;
  new_var = page;
  t = 0xF - kaze_records_cursor_move_timer;
  if (kaze_records_cursor_tab == 3)
  {
  int boff;
    q = t / 3;
    __asm__("":"=r"(q):"0"(q));
    i = 0;
    __asm__("":"=r"(i):"0"(i));
    { int _mt = i; __asm__("" :: "r"(_mt) : "memory"); }
    { register int *base asm("$3") = kaze_records_screen_prim_buffer_a; p = base + new_var; }
    boff = new_var * 20;
    prim_ofs = 0x1B8;
    field_ofs = 0;
    role8 = 0;
    false_val = -0x19;
    do
    {
      __asm__("":"=r"(t):"0"(t));
    if ((kaze_records_cursor_move_timer != 0) && (q < i))
      {
        { int _s = t * 6; *((s16 *) (((*p) + field_ofs) + 0x1C2)) = (_s) - 0x19; }
      }
      else
      {
        *((s16 *) (((*p) + field_ofs) + 0x1C2)) = false_val;
      }
      false_val += 0x12;
      i += 1;
      {
        int _s = kaze_records_cursor_row * 0x30;
    register int glyph_index asm("$2");
        glyph_index = role8 + _s;
        __asm__("":"=r"(glyph_index):"0"(glyph_index));
        *((s8 *) ((field_ofs + (*p)) + 0x1C4)) = kaze_records_working_table_stage_tag[glyph_index] << 4;
      }
      role8 += 8;
      {
        register int opacity asm("$2") = 0xF0;
        *((unsigned char *) ((field_ofs + (*p)) + 0x1C5)) = opacity;
      }
      field_ofs += 0x10;
      AddPrim((void *) *(int *)((char *)kaze_menu_gs_ot_buf0_org + boff), (void *) ((*p) + prim_ofs));
      __asm__("" :: "r"(prim_ofs) : "memory");
      prim_ofs += 0x10;
    }
    while (i < 6);
    __asm__("":: "r"(i));
  }
  {
  int q2 = t / 3;
  i = 0;
  { register int *base asm("$3") = kaze_records_screen_prim_buffer_a; p = base + new_var; }
  q = new_var * 20;
  role8 = 0x158;
  field_ofs = 0;
  false_val = -0x19;
  do
  {
    __asm__("":"=r"(t):"0"(t));
    if ((kaze_records_cursor_move_timer != 0) && (q2 < i))
    {
      { int _s = t * 6; *((s16 *) (((*p) + field_ofs) + 0x162)) = (_s) - 0x19; }
    }
    else
    {
      *((s16 *) (((*p) + field_ofs) + 0x162)) = false_val;
      { int _mt = field_ofs; __asm__("" :: "r"(_mt) : "memory"); }
    }
    false_val += 0x12;
    i += 1;
    __asm__("":"=r"(i):"0"(i));
    *((s8 *) ((field_ofs + (*p)) + 0x164)) = kaze_records_cursor_row << 4;
    {
      register int opacity asm("$2") = 0xE0;
      *((unsigned char *) ((field_ofs + (*p)) + 0x165)) = opacity;
    }
    field_ofs += 0x10;
    AddPrim((void *) *(int *)((char *)kaze_menu_gs_ot_buf0_org + q), (void *) ((*p) + role8));
    role8 += 0x10;
  }
  while (i < 6);
  __asm__("":"=r"(i):"0"(i));
  __asm__("":"=r"(i):"0"(i));
  __asm__("":: "r"(i));
  }
  {
 int p4;
  int * p3;
  int field3;
  int q3 = t / 3;
  i = 0;
  p4 = new_var * 4;
  { register int *base asm("$2") = kaze_records_screen_prim_buffer_a; p3 = (int *)((char *)base + p4); }
  role8 = 0xF8;
  field3 = 0;
  false_val = -0x19;
  __asm__("":"=r"(false_val):"0"(false_val));
  do
  {
    __asm__("":"=r"(t):"0"(t));
    if ((kaze_records_cursor_move_timer != 0) && (q3 < i))
    {
      { int _s = t * 6; *((s16 *) (((*p3) + field3) + 0x102)) = (_s) - 0x19; }
    }
    else
    {
      *((s16 *) (((*p3) + field3) + 0x102)) = false_val;
    }
    field3 += 0x10;
    __asm__("" :: "r"(field3) : "memory");
    false_val += 0x12;
    i += 1;
    __asm__("":"=r"(new_var):"0"(new_var));
    { int _mt = p4; __asm__("" :: "r"(_mt) : "memory"); }
    { register int ot_sum asm("$2") = p4 + new_var; field_ofs = ot_sum << 2; }
    AddPrim((void *) *(int *)((char *)kaze_menu_gs_ot_buf0_org + field_ofs), (void *) ((*p3) + role8));
    role8 += 0x10;
  }
  while (i < 6);
  __asm__("":: "r"(i));
  AddPrim((void *) *(int *)((char *)kaze_menu_gs_ot_buf0_org + field_ofs), (void *) (kaze_records_screen_prim_buffer_a[new_var] + 0x218));
  }
}

/**
 * kaze_records_entries_draw  (menu, 0x8005f8b4)
 *
 * Draws the 6 rows x 8 record entry text SPRTs with the same roll-in
 * y-offset animation, then the text DR_TPAGE (+0x620).
 */

void kaze_records_entries_draw(int page)
{
    extern void AddPrim(void *, void *);
    extern char kaze_menu_gs_ot_buf0_org[];
    extern int kaze_records_screen_prim_buffer_a[];
    register int s5 __asm__("$21");
    register int false_val __asm__("$23");
    register int prim_ofs __asm__("$22");
    register int i __asm__("$20");
    register int field_ofs __asm__("$16");
    register int inner_ofs __asm__("$17");
    register int *p __asm__("$18");
    register int page20 __asm__("$19");
    int row_ofs;
    int which;

    {
        int magic = 0x55555556;
        int fifteen = 0xF;
        int dv = kaze_records_cursor_move_timer;
        s5 = fifteen - dv;
        __asm__("" : "=r"(s5) : "0"(s5), "r"(fifteen), "r"(dv));
        which = s5 / 3;
        i = 0;
        false_val = -0x19;
        prim_ofs = 0x260;
        row_ofs = 0;
        (void)magic;
    }
    do {
        int page4;
        int cond;

        cond = which < i;
        {
            register int eb __asm__("$2");
            eb = (int)kaze_records_screen_prim_buffer_a;
            __asm__("sll %0, %1, 2" : "=r"(page4) : "r"(page), "r"(eb));
            p = (int *)(page4 + eb);
        }
        inner_ofs = 0;
        field_ofs = row_ofs;
        do {
            if (kaze_records_cursor_move_timer != 0 && cond) {
                *(s16 *)(*p + field_ofs + 0x26A) = s5 * 6 - 0x19;
            } else {
                *(s16 *)(*p + field_ofs + 0x26A) = false_val;
            }
            __asm__("" : "=r"(page4) : "0"(page4));
            page20 = (page4 + page) * 4;
            field_ofs += 0x14;
            AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + page20),
                    (void *)(*p + prim_ofs + inner_ofs));
            inner_ofs += 0x14;
        } while (inner_ofs < 0xA0);
        page20 = (page4 + page) * 4;
        false_val += 0x12;
        prim_ofs += 0xA0;
        i += 1;
        row_ofs += 0xA0;
    } while (i < 6);
    AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + page20),
            (void *)(kaze_records_screen_prim_buffer_a[page] + 0x620));
}

/**
 * kaze_records_combined_sort  (menu, 0x8005fa48)
 *
 * Selection-sorts the merged 18-entry (3 stages x 6) working record buffer
 * kaze_records_working_table/kaze_records_working_table_score for category
 * row cat_arg - descending for category 1, ascending otherwise - used by the
 * combined tab.
 */

void kaze_records_combined_sort(int cat_arg)
{
    extern char kaze_records_working_table_score[];
    extern char kaze_records_working_table[];
    register int cat asm("$15");
    register int cmp_cat asm("$19");
    register int init_cat asm("$20");
    register int one asm("$21");
    int cat_ofs;
    int cat2;
    register int stage_ofs asm("$16");
    int scan_cat_ofs;
    register int bi asm("$9");
    register int bc asm("$10");
    register int bj asm("$11");
    register int i asm("$13");
    int j;
    int row_ofs;
    int scan_ofs;
    int k;
    int l;
    int kbase;
    int koff;
    int lofs;
    int off0;
    int off;
    int base;
    int srcpart;
    int srcpart2;
    int better;
    u32 best, v;
    Rec *dst;
    Rec *src;
    Rec tmp;

    cat = cat_arg;
    cmp_cat = cat;
    init_cat = cat;
    i = 0;
    cat_ofs = cat * 0x30;
    one = 1;
    cat2 = cat * 2;
    stage_ofs = 0;
loop_i:
    j = 0;
    scan_cat_ofs = (cat2 + cat) * 0x10;
    row_ofs = 0;
    scan_ofs = cat_ofs;
loop_j:
    off0 = scan_ofs + stage_ofs;
    best = *(u32 *)&kaze_records_working_table_score[off0];
    bi = i;
    bc = init_cat;
    bj = j;
    __asm__ ("" : : "r" (bj));
    if (i < 3) {
        k = i;
        __asm__ ("" : "=r" (i) : "0" (i));
        kbase = i * 0x90;
        do {
            l = j & -(i == k);
            if (l < 6) {
                koff = kbase;
                lofs = l * 8 + scan_cat_ofs;
                do {
                    off = lofs + koff;
                    if (cmp_cat == one) {
                        v = *(u32 *)&kaze_records_working_table_score[off];
                        better = best < v;
                    } else {
                        v = *(u32 *)&kaze_records_working_table_score[off];
                        better = v < best;
                    }
                    if (better) {
                        best = v;
                        bi = k;
                        bc = cat;
                        bj = l;
                    }
                    l += 1;
                    lofs += 8;
                } while (l < 6);
            }
            k += 1;
            kbase += 0x90;
        } while (k < 3);
    }
    base = (int)kaze_records_working_table;
    dst = (Rec *)(row_ofs + (cat_ofs + (stage_ofs + base)));
    tmp = *dst;
    srcpart = bi * 0x90 + base;
    srcpart2 = bc * 0x30 + srcpart;
    {
        register int bj8 asm("$3");
        bj8 = bj * 8;
        src = (Rec *)(bj8 + srcpart2);
    }
    *dst = *src;
    *src = tmp;
    row_ofs += 8;
    j += 1;
    scan_ofs += 8;
    if (j < 6)
        goto loop_j;
    i += 1;
    stage_ofs += 0x90;
    if (i < 3)
        goto loop_i;
}

extern u8 kaze_grid_variant_saved_index;
extern u8 kaze_task_grid_available_flags;

/**
 * kaze_record_table_insert  (core, 0x8005fc1c)
 *
 * Inserts a flight result into the persistent record table for
 * stage/category: ranks value in the 6-entry kaze_record_table_score list
 * (desc for category 1), shifts lower entries, stores the current task's
 * 3-char name (kaze_task_name_letter0_table) and value, flags 1st place in
 * kaze_task_grid_available_flags; skipped when kaze_staged_stage_variant
 * scripted-start is set.
 */

int kaze_record_table_insert(int arg0, int arg1, u32 arg2)
{
    extern u8 kaze_task_name_letter0_table;
    extern u8 kaze_record_table_c;
    extern u8 kaze_record_table_b;
    extern u8 kaze_record_table_a;
  register int temp_t0 __asm__("$8");
  register int temp_t1 __asm__("$9");
  register int temp_v1 __asm__("$3");
  int var_a0;
  int var_a1;
  int temp_calc;
  int var_v0;
  int var_a3;
  int new_var;
  int final_ofs;
  u8 temp_u8;
  if (kaze_staged_stage_variant != 0)
  {
    if (kaze_staged_stage_variant == 9)
    {
      D_800B17D4 = 1;
    }
    var_v0 = -1;
    if (kaze_grid_variant_saved_index != 9)
    {
      kaze_grid_variant_saved_index = (u8) kaze_staged_stage_variant;
    }
    return var_v0;
  }
  var_a3 = 0;
  if (arg1 == 1)
  {
    int table_ofs = arg0 * 0x90;
    int search_ofs = 0x30;
    loop_7:
    if ((*((u32 *) (((int) (&kaze_record_table_score)) + (search_ofs + table_ofs)))) < arg2)
    {
      goto ranked;
    }

    var_a3 += 1;
    __asm__("":"=r"(var_a3):"0"(var_a3));
    search_ofs += 8;
    if (var_a3 < 6)
    {
      goto loop_7;
    }
  }
  else
  {
    int table_ofs = arg0 * 0x90;
    int search_ofs = arg1 * 0x30;
    loop_11:
    if (arg2 < (*((u32 *) (((int) (&kaze_record_table_score)) + (search_ofs + table_ofs)))))
    {
      goto ranked;
    }

    var_a3 += 1;
    search_ofs += 8;
    if (var_a3 < 6)
    {
      goto loop_11;
    }
  }
  ranked:
  if (var_a3 == 6)
  {
    return -1;
  }

  if (var_a3 < 5)
  {
    var_a1 = 5;
    temp_calc = (arg1 * 2) + arg1;
    temp_t1 = temp_calc * 0x10;
    temp_calc = (arg0 * 8) + arg0;
    temp_t0 = temp_calc * 0x10;
    shift_loop:
      var_a0 = (var_a1 * 8) + temp_t1;
      var_a1 -= 1;
      { int _mt = var_a1; __asm__("" :: "r"(_mt) : "memory"); }
      temp_v1 = (var_a1 * 8) + temp_t1;
      temp_v1 += temp_t0;
      var_a0 += temp_t0;
      ((RecordEntry *)((char *)&kaze_record_table_a + var_a0))->a = ((RecordEntry *)((char *)&kaze_record_table_a + temp_v1))->a;
      ((RecordEntry *)((char *)&kaze_record_table_a + var_a0))->b = ((RecordEntry *)((char *)&kaze_record_table_a + temp_v1))->b;
      ((RecordEntry *)((char *)&kaze_record_table_a + var_a0))->c = ((RecordEntry *)((char *)&kaze_record_table_a + temp_v1))->c;
      new_var = 8;
      ((RecordEntry *)((char *)&kaze_record_table_a + var_a0))->score = ((RecordEntry *)((char *)&kaze_record_table_a + temp_v1))->score;
      if (var_a3 < var_a1)
      {
        goto shift_loop;
      }
  }
  final_ofs = ((var_a3 * 8) + (arg1 * 0x30)) + (arg0 * 0x90);
  (&kaze_record_table_a)[final_ofs] = (&kaze_task_name_letter0_table)[kaze_task_record_index * 3];
  (&kaze_record_table_b)[final_ofs] = (&kaze_task_name_letter1_table)[kaze_task_record_index * 3];
  temp_u8 = (&kaze_task_name_letter2_table)[kaze_task_record_index * 3];
  *((int *) (((int) (&kaze_record_table_score)) + final_ofs)) = arg2;
  (&kaze_record_table_c)[final_ofs] = temp_u8;
  __asm__("" :: "r"(temp_u8) : "memory");
  if (var_a3 == 0)
  {
    {
      u8 *_flags = &kaze_task_grid_available_flags;
      int _s = arg0 * 3;
      __asm__("" :: "r"(_flags));
      *(_flags + _s + arg1) = 1;
    }
  }
  return var_a3;
}
