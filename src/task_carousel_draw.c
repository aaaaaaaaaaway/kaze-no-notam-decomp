#include "kaze_syms.h"
extern int kaze_task_glyph_count;
extern int kaze_task_letter_count;
extern void exit(int);
typedef struct {
    short m[3][3];
    short pad;
    int t[3];
} MATRIX;
typedef struct {
    int attribute;
    int coord2;
    int tmd;
    int id;
} GsDOBJ2;

extern int kaze_menu_gs_ot_buf[];
extern void kaze_title_model_sort_dispatch(int, int *);
void kaze_task_carousel_glyph_render();
void kaze_task_name_letters_render();
extern void kaze_drmove_addprim_a0ec(int, int);

/**
 * kaze_task_carousel_frame_draw  (menu, 0x8005c76c)
 *
 * Task-carousel frame draw: sets base color (kaze_gs_light_set_color_9568),
 * clears the buffer OT (kaze_title_model_sort_dispatch), renders ring glyphs
 * (kaze_task_carousel_glyph_render) and entered letters
 * (kaze_task_name_letters_render), then finishes with
 * kaze_drmove_addprim_a0ec.
 */

void kaze_task_carousel_frame_draw(int buf)
{
    kaze_gs_light_set_color_9568(0, 0xFF, 0xFF, 0xFF);
    kaze_title_model_sort_dispatch(buf, kaze_menu_gs_ot_buf + buf * 5);
    kaze_task_carousel_glyph_render(buf);
    kaze_task_name_letters_render(buf);
    kaze_drmove_addprim_a0ec(buf, 0xFFF);
    return;
}

extern int kaze_task_menu_state;
extern GsDOBJ2 kaze_task_glyph_dobjs[];
extern int kaze_task_carousel_scatter_remaining;
extern int kaze_task_carousel_angle_ring;
extern short kaze_task_glyph_rotation_x[];
extern int kaze_task_carousel_glyph_pos_x[];
extern int kaze_task_carousel_selected_index;
extern int kaze_task_carousel_previous_index;
void kaze_task_carousel_glyph_highlight();
void kaze_task_letter_add_highlight();
extern void RotMatrix(short *, int);
extern void kaze_trans_matrix(int, int *);
extern void GsGetLws(int, MATRIX *, MATRIX *);
extern void GsSetLightMatrix(MATRIX *);
extern void GsSetLsMatrix(MATRIX *);
extern void GsSortObject4(GsDOBJ2 *, int *, int, int);

/**
 * kaze_task_carousel_glyph_render  (menu, 0x8005c7e0)
 *
 * Renders the carousel glyph 3D objects when kaze_task_menu_state is
 * 0x28/0x29/0x2b/0x2c: culls by ring angle window, applies highlight colors
 * (kaze_task_carousel_glyph_highlight/cc60), RotMatrix+translate, GsGetLws,
 * GsSortObject4 into the buffer OT.
 */

void kaze_task_carousel_glyph_render(int buf)
{
    int glyph_i;
    int *coord_ptr;
    int ang_ofs;
    int off20;
    GsDOBJ2 *objp;
    MATRIX MStack_68;
    MATRIX MStack_48;
    MATRIX *m48;
    int pad[2];

    if (((unsigned)(kaze_task_menu_state - 0x28) < 2) || ((unsigned)(kaze_task_menu_state - 0x2b) < 2)) {
        kaze_gs_light_set_color_9568(0, 0, 0x80, 0);
        objp = kaze_task_glyph_dobjs;
        glyph_i = 0;
        if (0 < kaze_task_glyph_count) {
            m48 = &MStack_48;
            off20 = buf * 20;
            coord_ptr = &objp->coord2;
            ang_ofs = 0;
            do {
                if ((((kaze_task_menu_state != 0x2b) || (0 < *(int *)((int)&kaze_task_carousel_scatter_remaining + ang_ofs))) &&
                     (299 < *(int *)((int)&kaze_task_carousel_angle_ring + ang_ofs) - 0x47cU)) &&
                    (299 < *(int *)((int)&kaze_task_carousel_angle_ring + ang_ofs) - 0xa58U)) {
                    if (glyph_i >= 0x27) {
                        kaze_gs_light_set_color_9568(0, 0xff, 0x80, 0);
                    } else {
                        kaze_task_carousel_glyph_highlight(glyph_i);
                        kaze_task_letter_add_highlight(glyph_i);
                    }
                    RotMatrix(&kaze_task_glyph_rotation_x[glyph_i * 4], *coord_ptr + 4);
                    kaze_trans_matrix(*coord_ptr + 4, &kaze_task_carousel_glyph_pos_x[glyph_i * 4]);
                    GsGetLws(*coord_ptr, &MStack_68, m48);
                    GsSetLightMatrix(&MStack_68);
                    GsSetLsMatrix(m48);
                    GsSortObject4(objp, (int *)((int)kaze_menu_gs_ot_buf + off20), 2, 0x1F800000);
                    if ((glyph_i == kaze_task_carousel_selected_index) || (glyph_i == kaze_task_carousel_previous_index)) {
                        kaze_gs_light_set_color_9568(0, 0, 0x80, 0);
                    }
                }
                ang_ofs = ang_ofs + 4;
                glyph_i = glyph_i + 1;
                coord_ptr = coord_ptr + 4;
                objp = objp + 1;
            } while (glyph_i < kaze_task_glyph_count);
        }
    }
    __asm__("" :: "m"(pad[0]));
}

extern int kaze_task_carousel_exit_timer;
extern int kaze_task_carousel_transition_ticks;
extern int kaze_task_carousel_transition_duration;
/*
 * kaze_task_carousel_glyph_highlight (0x8005c9f0), 624 B.
 *
 * Highlight colour ramp for the selected (kaze_task_carousel_selected_index) / previous
 * (kaze_task_carousel_previous_index) carousel glyph, interpolated over rotation progress
 * (transition_ticks kaze_task_carousel_transition_ticks / duration kaze_task_carousel_transition_duration), fed to the colour
 * setter kaze_gs_light_set_color_9568(light_index=0, r, g, b).
 *
 * 2.7.2-psx -O2.  Near miss: EXACT SIZE 624 B, 8/156 words differ.
 *
 * Structure levers (all byte-exact):
 *  - FOUR SEPARATE call sites (not a shared goto/LAB).  gcc's cross-jumping
 *    then merges the longest common tails exactly like the original: all four
 *    paths converge on ONE jal (CC48); the two transition==0 paths first
 *    converge on the shared `a3=0` at CC44.  This is what keeps the per-path
 *    `move a0,zero` clears from CSE-collapsing while still emitting a single
 *    jal -- the key fix over the previous shared-clear candidate (628 B/+1).
 *  - exit (kaze_task_carousel_exit_timer) is chain-laundered AFTER the `!=0` return test so gcc
 *    cannot prove it is 0 and reuse t0 for the zero args; the zero args then
 *    spell `$zero` (target) instead of `move a?,t0`.  Fixes 2 words.
 *  - block2's dur (kaze_task_carousel_transition_duration) is pinned $2(v0): otherwise the block2 q1b
 *    pin reserves v1 across tt's live range, spilling dur to a2 and cascading
 *    a whole register family through every division.  Fixes ~15 words.
 *  - b is spelled as an IN-PLACE shift on q5 (`q5 = q5>>12; b = q5 &
 *    0xff`) to reproduce `srl a1,a1 ; andi a3,a1` (reuse a1 in place); a fresh
 *    q5-into-a3 spelling defers the mask into the j delay slot instead.
 *  - r's shift is forced through a fresh v1($3) scratch to reproduce
 *    `srl v1,a2 ; andi a1,v1` rather than the economical in-place `srl a1,a2`.
 *  - the divisions go through the sibling CHECKED_DIV inline-asm island to
 *    emit the exact `break 0x1c00`/`break 0x1800` sequence under the project's
 *    maspsx --expand-li build (a plain `/` emits a bare div/mflo here).
 *
 * REMAINING WALL (all 8 words = 4 per block, both trans!=0 blocks):
 *   the per-path `move a0,zero` (light index 0) must sit at the TOP of the
 *   mask block (right after the last mflo, orig 0x100/0x220) so the trailing
 *   `andi a2` fills the `j` delay slot.  Ours schedules it at mask-pos 4
 *   (after srl a1,a1 / andi a3,a1 / srl v1,a2).  DIAG-VERIFIED (sched2 BLOCK3,
 *   gcc-2.7.2-psx-diag): the mask insns and the a0=0 arg-move share
 *   INSN_PRIORITY (24) and the winner is pure descending-LUID; the a0=0
 *   arg-move (insn 90, luid 16) OUT-ranks the mask insns (luid 12..15) so it
 *   is scheduled into a later mask position rather than block-top.  To win
 *   block-top it needs the LOWEST luid in the mask block = the earliest source
 *   position -- but any early placement of an explicit `a0=0` is
 *   GCSE-hoisted / cross-jump-merged / flow-deleted (verified: 4 clears drop
 *   to 2, size 616 B).  Late binding (arg-move) keeps 4 per-path clears but
 *   forces the high luid.  The two requirements are mutually exclusive under
 *   this compiler's optimisers.  Confirmed a hard floor by the regular
 *   decomp-permuter: 5600+ iterations, base score 120 (=8w), never below.
 *
 *   Root mechanism: the CHECKED_DIV VOLATILE asm island (mandatory under
 *   --expand-li to get the break codes) is a scheduling barrier that
 *   perturbs where gcc inserts the a0=0 arg-move.  Compiling the same body
 *   with plain `/` + maspsx --expand-div (non-barrier real div) schedules
 *   the a0=0 clear differently -- but the project build hardcodes
 *   --expand-li, so CHECKED_DIV (and its 8-word scheduling tax) is required.
 */

/**
 * kaze_task_carousel_glyph_highlight  (menu, 0x8005c9f0)
 *
 * Computes the highlight color ramp for the selected
 * (kaze_task_carousel_selected_index) and previous
 * (kaze_task_carousel_previous_index) glyphs, interpolating toward/away from
 * the 0xe0 highlight over rotation progress kaze_task_carousel_transition_ti
 * cks/kaze_task_carousel_transition_duration, via color setter
 * kaze_gs_light_set_color_9568.
 */

void kaze_task_carousel_glyph_highlight(int idx)
{
    register int exit __asm__("$8");
    register int num __asm__("$4");
    register int q1 __asm__("$6");
    register int q5 __asm__("$5");
    unsigned int r;
    unsigned int g;
    unsigned int b;

    exit = kaze_task_carousel_exit_timer;
    if (exit != 0) {
        return;
    }
    /* keep exit opaque so gcc does not reuse t0's known-0 for the zero args */
    __asm__("" : "=r"(exit) : "0"(exit));
    if (kaze_task_carousel_selected_index == idx) {
        if (kaze_task_carousel_transition_ticks != 0) {
            register int dur __asm__("$2") = kaze_task_carousel_transition_duration;
            register int d __asm__("$3") = dur - kaze_task_carousel_transition_ticks;
            register int s __asm__("$7") = 0xe0000;
            __asm__("" : "=r"(s) : "0"(s));
            num = d * s;
            q1 = num / dur;
            __asm__ volatile("" :: "r"(q1));
            q5 = exit / dur;
            __asm__ volatile("" :: "r"(q5));
            s = 0x60000;
            __asm__("" : "=r"(s) : "0"(s));
            d = d * s;
            dur = d / dur;
            __asm__ volatile("" :: "r"(dur));
            {
            register int za __asm__("$4") = 0;
            __asm__("" : "=r"(za) : "0"(za));
            q5 = (int)((unsigned int)q5 >> 0xc);
            b = q5 & 0xff;
            { register int t3 __asm__("$3") = (int)((unsigned int)q1 >> 0xc); r = t3 & 0xff; }
            g = (dur >> 0xc) - 0x80U & 0xff;
            kaze_gs_light_set_color_9568(za, r, g, b);
            }
            return;
        }
        kaze_gs_light_set_color_9568(0, 0xe0, 0xe0, 0);
        return;
    }
    if (kaze_task_carousel_previous_index != idx) {
        return;
    }
    if (kaze_task_carousel_transition_ticks != 0) {
        register int dur __asm__("$2") = kaze_task_carousel_transition_duration;
        register int d __asm__("$4") = dur - kaze_task_carousel_transition_ticks;
        register int s __asm__("$7") = -0xe0000;
        register int q1b __asm__("$3");
        __asm__("" : "=r"(s) : "0"(s));
        q1b = d * s;
        q1b = q1b / dur;
        __asm__ volatile("" :: "r"(q1b));
        q5 = exit / dur;
        __asm__ volatile("" :: "r"(q5));
        s = -0x60000;
        __asm__("" : "=r"(s) : "0"(s));
        d = d * s;
        dur = d / dur;
        __asm__ volatile("" :: "r"(dur));
        {
        register int za __asm__("$4") = 0;
        __asm__("" : "=r"(za) : "0"(za));
        q5 = (int)((unsigned int)q5 >> 0xc);
        b = q5 & 0xff;
        { register int t3 __asm__("$3") = q1b >> 0xc; r = t3 - 0x20U & 0xff; }
        g = (dur >> 0xc) - 0x20U & 0xff;
        kaze_gs_light_set_color_9568(za, r, g, b);
        }
        return;
    }
    kaze_gs_light_set_color_9568(0, 0, 0x80, 0);
    return;
}

/**
 * kaze_task_letter_add_highlight  (menu, 0x8005cc60)
 *
 * During the add-letter animation (kaze_task_carousel_exit_timer) ramps the
 * selected glyph color from highlight toward red as the letter flies to the
 * name row.
 */

void kaze_task_letter_add_highlight(int idx)
{
    int base;
    int scale;

    if (kaze_task_carousel_exit_timer != 0 && kaze_task_carousel_selected_index == idx) {
        base = 0x1E - kaze_task_carousel_exit_timer;
        scale = -0xE0000;
        kaze_gs_light_set_color_9568(0, 0xE0, (((base * scale) / 0x1E >> 12) - 0x20) & 0xFF, 0);
    }
}

extern GsDOBJ2 kaze_task_name_letter_dobjs[];
extern short kaze_task_name_letter_rotation_x[];
extern int kaze_task_name_letter_pos_x[];

/**
 * kaze_task_name_letters_render  (menu, 0x8005cce4)
 *
 * Renders up to 3 entered task-name letter models (GsDOBJ2s
 * kaze_task_name_letter_dobjs, rotations kaze_task_name_letter_rotation_x,
 * positions kaze_task_name_letter_pos_x) with
 * RotMatrix/GsGetLws/GsSortObject4 in red (0xe0,0,0).
 */

void kaze_task_name_letters_render(int buf)
{
    int i;
    GsDOBJ2 *objp;
    MATRIX M58;
    MATRIX M38;

    kaze_gs_light_set_color_9568(0, 0xE0, 0, 0);
    i = 0;
    objp = kaze_task_name_letter_dobjs;
    do {
        if (i < kaze_task_letter_count) {
            RotMatrix(&kaze_task_name_letter_rotation_x[i * 4], objp->coord2 + 4);
            kaze_trans_matrix(objp->coord2 + 4, &kaze_task_name_letter_pos_x[i * 4]);
            GsGetLws(objp->coord2, &M58, &M38);
            GsSetLightMatrix(&M58);
            GsSetLsMatrix(&M38);
            GsSortObject4(objp, kaze_menu_gs_ot_buf + buf * 5, 2, 0x1F800000);
            objp = objp + 1;
        }
        i = i + 1;
    } while (i < 3);
}
