#include "types.h"
#include "kaze_syms.h"
extern int kaze_ending_balloon_bob_phase;
extern int kaze_ending_balloon_bob_offset_y;
extern int kaze_ending_path_start_x;
extern int kaze_ending_path_start_y;
extern int kaze_ending_path_start_z;
extern int kaze_ending_path_end_x;
extern int kaze_ending_path_end_y;
extern int kaze_ending_path_end_z;
extern int kaze_ending_path_delta_x;
extern int kaze_ending_path_delta_y;
extern int kaze_ending_path_delta_z;
extern int kaze_ending_path_error_x;
extern int kaze_ending_path_error_y;
extern int kaze_ending_path_error_z;
extern int kaze_ending_path_step_x;
extern int kaze_ending_path_step_y;
extern int kaze_ending_path_step_z;
extern int kaze_ending_path_progress;
extern int kaze_ending_path_speed;
extern int kaze_ending_path_dominant_axis;
extern int kaze_menu_camera_pos_z;
extern int kaze_menu_camera_scroll;

extern void AddPrim(void *, void *);
extern int kaze_ending_ot_base_ptrs[];
extern u8 kaze_ending_backdrop_sprites[], kaze_ending_backdrop_tpage_prims[], kaze_ending_credit_title_sprites[], kaze_ending_credit_title_tpage_prims[];
extern u8 kaze_ending_credit_line_sprites[], kaze_ending_credit_line_tpage_prims[], kaze_ending_backdrop_poly[], kaze_ending_path_keyframe_table[];
extern int kaze_ending_backdrop_scroll_x, kaze_ending_backdrop_scroll_y, kaze_ending_credit_page_index;

/**
 * kaze_ending_credit_prims_add  (menu, 0x80064354)
 *
 * Adds credit-page prims: backdrop SPRT kaze_ending_backdrop_sprites at
 * scroll pos kaze_ending_backdrop_scroll_x/408, then logo sprites and text-
 * line sprites per the page's logo and text-line counts, read from each
 * kaze_ending_path_keyframe_table record (stride 0x1c, the text-line count
 * 0xc bytes into the record), plus POLY_F4 kaze_ending_backdrop_poly.
 */

void kaze_ending_credit_prims_add(int buf)
{
    int i, s1, lot, v404, v6f0;
    int *tbl;
    u8 *prim;
    /*
     * MATCH 0/134, byte-exact 536B (closed from 21).
     * Frame: original reserves a 0x40 frame (16 dead bytes above the arg area);
     * the zero-emit "=m"(pad) island at the END reserves them and lets sched2
     * interleave the prologue reg-saves among the initial computation.
     * FINISH (21 -> 0), three levers:
     *  1. STRENGTH-REDUCED IV (21->11): the two AddPrim loops used an explicit
     *     byte-offset counter (s1=0; s1+=0x14) whose `move s1,zero` init sorted
     *     BEFORE the LICM-hoisted param*0x28/*0x10 shifts. Replacing s1 with the
     *     giv expression `i * 0x14` lets loop.c generate the counter update at a
     *     high UID, materialising the invariant shifts first (target order).
     *  2. PRIM-BASE EARLY (11->4): writing `prim = kaze_ending_backdrop_sprites; prim += s1;`
     *     (base referenced before the s1 arithmetic) emits the kaze_ending_backdrop_sprites
     *     lui/addiu first, matching the target prologue.
     *  3. CONST-LOAD ORDER (4->0): the kaze_ending_backdrop_scroll_x (sh-store value) and kaze_ending_credit_page_index
     *     (tbl base) const-loads landed in swapped emission order because the tbl
     *     computation was source-first (kaze_ending_credit_page_index lower LUID + longer dependent
     *     chain). Reading BOTH into scalar temps at the top in target order
     *     (v404=kaze_ending_backdrop_scroll_x then v6f0=kaze_ending_credit_page_index), and deferring the tbl multiply
     *     to use v6f0, forces kaze_ending_backdrop_scroll_x to load first while both stay early. */
    int pad[4];

    v404 = kaze_ending_backdrop_scroll_x;
    v6f0 = kaze_ending_credit_page_index;
    prim = kaze_ending_backdrop_sprites;
    s1 = buf * 0x14;
    prim += s1;
    *(short *)(prim + 8) = (short)v404;
    *(short *)(prim + 0xA) = (short)kaze_ending_backdrop_scroll_y;
    tbl = (int *)(kaze_ending_path_keyframe_table + v6f0 * 0x1c);

    AddPrim((void *)(*(int *)(s1 + (char *)kaze_ending_ot_base_ptrs) + 0x3ffc), prim);
    AddPrim((void *)(*(int *)(s1 + (char *)kaze_ending_ot_base_ptrs) + 0x3ffc), kaze_ending_backdrop_tpage_prims + buf * 8);

    i = 0;
    if (0 < tbl[0]) {
        lot = buf * 0x14;
        do {
            AddPrim((void *)*(int *)(lot + (char *)kaze_ending_ot_base_ptrs), kaze_ending_credit_title_sprites + buf * 0x28 + i * 0x14);
            AddPrim((void *)*(int *)(lot + (char *)kaze_ending_ot_base_ptrs), kaze_ending_credit_title_tpage_prims + buf * 0x10 + i * 8);
            i++;
        } while (i < tbl[0]);
    }
    i = 0;
    if (0 < tbl[3]) {
        lot = buf * 0x14;
        do {
            AddPrim((void *)*(int *)(lot + (char *)kaze_ending_ot_base_ptrs), kaze_ending_credit_line_sprites + buf * 0x3c + i * 0x14);
            AddPrim((void *)*(int *)(lot + (char *)kaze_ending_ot_base_ptrs), kaze_ending_credit_line_tpage_prims + buf * 0x18 + i * 8);
            i++;
        } while (i < tbl[3]);
    }
    AddPrim((void *)*(int *)(buf * 0x14 + (char *)kaze_ending_ot_base_ptrs), kaze_ending_backdrop_poly);
    __asm__ __volatile__("" : "=m"(pad));
}

/**
 * kaze_ending_path_segment_setup  (menu, 0x8006456c)
 *
 * Sets up a balloon flythrough segment: start kaze_ending_path_start_x..
 * into kaze_menu_camera_pos_x/e4/e8, computes per-axis deltas/step signs
 * (kaze_ending_path_delta_x..73c, speed kaze_ending_path_speed) and dominant
 * axis selector kaze_ending_path_dominant_axis for the DDA stepper.
 */

void kaze_ending_path_segment_setup(void)
{
    extern int kaze_menu_camera_pos_x;
    register int x asm("$2");
    register int y asm("$4");
    register int end_x asm("$3");
    register int z asm("$5");
    register int dx asm("$6");
    int dy;
    int dz;
    int abs_x;

    x = kaze_ending_path_start_x;
    y = kaze_ending_path_start_y;
    end_x = kaze_ending_path_end_x;
    z = kaze_ending_path_start_z;
    __asm__("" : "=r"(x) : "0"(x));
    __asm__("" : "=r"(y) : "0"(y));
    __asm__("" : "=r"(end_x) : "0"(end_x));
    __asm__("" : "=r"(z) : "0"(z));
    dx = end_x - x;

    kaze_menu_camera_pos_x = x;
    dy = kaze_ending_path_end_y - y;
    dz = kaze_ending_path_end_z - z;
    kaze_ending_path_progress = 0;
    kaze_ending_path_error_x = 0;
    kaze_ending_path_error_y = 0;
    kaze_ending_path_error_z = 0;
    kaze_menu_camera_scroll = y;
    kaze_menu_camera_pos_z = z;
    kaze_ending_path_delta_x = dx;
    kaze_ending_path_delta_y = dy;
    kaze_ending_path_delta_z = dz;

    if (dx < 0) {
        end_x = -dx;
        kaze_ending_path_delta_x = end_x;
        kaze_ending_path_step_x = -kaze_ending_path_speed;
    } else {
        kaze_ending_path_step_x = kaze_ending_path_speed;
    }

    if (kaze_ending_path_delta_y < 0) {
        kaze_ending_path_step_y = -kaze_ending_path_speed;
        kaze_ending_path_delta_y = -kaze_ending_path_delta_y;
    } else {
        kaze_ending_path_step_y = kaze_ending_path_speed;
    }

    if (kaze_ending_path_delta_z < 0) {
        kaze_ending_path_step_z = -kaze_ending_path_speed;
        kaze_ending_path_delta_z = -kaze_ending_path_delta_z;
    } else {
        kaze_ending_path_step_z = kaze_ending_path_speed;
    }

    abs_x = kaze_ending_path_delta_x;
    if ((kaze_ending_path_delta_y < abs_x) && (kaze_ending_path_delta_z < abs_x)) {
        kaze_ending_path_dominant_axis = 0;
    } else {
        if (kaze_ending_path_delta_y <= kaze_ending_path_delta_z) {
            kaze_ending_path_dominant_axis = 2;
        } else {
            kaze_ending_path_dominant_axis = 1;
        }
    }
}

extern int rcos(int);
extern int kaze_menu_camera_coord_flags;

/**
 * kaze_ending_balloon_path_step  (menu, 0x8006471c)
 *
 * DDA step of the ending balloon along the current segment: advances
 * kaze_menu_camera_pos_x/e4/e8 by the dominant-axis error accumulators
 * (kaze_ending_path_error_x/72c/730) plus an rcos bobbing offset
 * kaze_ending_balloon_bob_offset_y on Y; clears
 * kaze_menu_camera_coord_flags.
 */

void kaze_ending_balloon_path_step(void) {
    extern int kaze_menu_camera_pos_x[3];
    int bob;
    int pos;

    kaze_ending_balloon_bob_phase = kaze_ending_balloon_bob_phase + 0xc;
    if (kaze_ending_balloon_bob_phase >= 0x1001) {
        kaze_ending_balloon_bob_phase = 0;
    }
    bob = rcos(kaze_ending_balloon_bob_phase);
    if (bob < 0) {
        bob = bob + 0x7ff;
    }
    kaze_ending_balloon_bob_offset_y = bob >> 0xb;
    switch (kaze_ending_path_dominant_axis) {
    case 0:
        pos = kaze_ending_path_progress;
        if (pos >= kaze_ending_path_delta_x) goto Lblock27;
        kaze_menu_camera_pos_x[0] = kaze_menu_camera_pos_x[0] + kaze_ending_path_step_x;
        kaze_ending_path_error_y = kaze_ending_path_error_y + kaze_ending_path_delta_y;
        if (kaze_ending_path_error_y >= kaze_ending_path_delta_x) {
            kaze_menu_camera_pos_x[1] = kaze_menu_camera_pos_x[1] + kaze_ending_path_step_y;
            kaze_ending_path_error_y = kaze_ending_path_error_y - kaze_ending_path_delta_x;
        }
        kaze_ending_path_error_z = kaze_ending_path_error_z + kaze_ending_path_delta_z;
        if (kaze_ending_path_error_z >= kaze_ending_path_delta_x) {
            kaze_menu_camera_pos_x[2] = kaze_menu_camera_pos_x[2] + kaze_ending_path_step_z;
            kaze_ending_path_error_z = kaze_ending_path_error_z - kaze_ending_path_delta_x;
        }
        goto Lblock26;
    case 1:
        if (kaze_ending_path_progress < kaze_ending_path_delta_y) {
            kaze_menu_camera_pos_x[1] = kaze_menu_camera_pos_x[1] + kaze_ending_path_step_y;
            kaze_ending_path_error_x = kaze_ending_path_error_x + kaze_ending_path_delta_x;
            if (kaze_ending_path_error_x >= kaze_ending_path_delta_y) {
                kaze_menu_camera_pos_x[0] = kaze_menu_camera_pos_x[0] + kaze_ending_path_step_x;
                kaze_ending_path_error_x = kaze_ending_path_error_x - kaze_ending_path_delta_y;
            }
            kaze_ending_path_error_z = kaze_ending_path_error_z + kaze_ending_path_delta_z;
            if (kaze_ending_path_error_z >= kaze_ending_path_delta_y) {
                kaze_menu_camera_pos_x[2] = kaze_menu_camera_pos_x[2] + kaze_ending_path_step_z;
                kaze_ending_path_error_z = kaze_ending_path_error_z - kaze_ending_path_delta_y;
            }
            kaze_ending_path_progress = kaze_ending_path_progress + kaze_ending_path_speed;
        }
        goto Lend;
    case 2:
        pos = kaze_ending_path_progress;
        if (pos >= kaze_ending_path_delta_z) goto Lblock27;
        kaze_menu_camera_pos_x[2] = kaze_menu_camera_pos_x[2] + kaze_ending_path_step_z;
        kaze_ending_path_error_x = kaze_ending_path_error_x + kaze_ending_path_delta_x;
        if (kaze_ending_path_error_x >= kaze_ending_path_delta_z) {
            kaze_menu_camera_pos_x[0] = kaze_menu_camera_pos_x[0] + kaze_ending_path_step_x;
            kaze_ending_path_error_x = kaze_ending_path_error_x - kaze_ending_path_delta_z;
        }
        kaze_ending_path_error_y = kaze_ending_path_error_y + kaze_ending_path_delta_y;
        if (kaze_ending_path_error_y >= kaze_ending_path_delta_z) {
            kaze_menu_camera_pos_x[1] = kaze_menu_camera_pos_x[1] + kaze_ending_path_step_y;
            kaze_ending_path_error_y = kaze_ending_path_error_y - kaze_ending_path_delta_z;
        }
        goto Lblock26;
    default:
        goto Lend;
    }
Lblock26:
    kaze_ending_path_progress = pos + kaze_ending_path_speed;
Lblock27:
    kaze_menu_camera_pos_x[1] = kaze_menu_camera_pos_x[1] + kaze_ending_balloon_bob_offset_y;
Lend:
    kaze_menu_camera_coord_flags = 0;
}
