#include "kaze_syms.h"
extern int kaze_task_record_index;
extern int kaze_task_glyph_count;
extern int kaze_task_letter_count;
extern char kaze_task_name_letter_dobj_coord_ptr[];
extern short kaze_task_name_letter_rotation_y[];
extern short kaze_task_name_letter_rotation_z[];
extern char kaze_task_name_letter_pos_y[];
extern char kaze_task_name_letter_pos_z[];
extern int kaze_task_name_letter_slot1_x;
extern int kaze_task_name_letter_slot2_x;
extern int kaze_gs_light0_dir_y;
extern int kaze_gs_light0_dir_z;
extern int kaze_menu_task_model_ptr;
extern int kaze_task_carousel_transition_amplitude;
extern int kaze_task_transition_counter;
extern void GsSetAmbient(int, int, int);
extern int kaze_balloon_view_zoom_dir_counter;
extern int kaze_menu_camera_pos_z;
extern int kaze_menu_camera_scroll;
extern void kaze_gs_view_set_pose_94c8(int, int, int, int, int, int);


extern void GsInitCoordinate2(void *, void *);
extern void GsLinkObject4(int, void *, int);
extern signed char kaze_task_name_letter0_table[];
extern char kaze_task_name_letter_coordinates[];
extern char kaze_task_name_letter_dobjs[];
extern short kaze_task_glyph_rotation_x[];
extern short kaze_task_glyph_rotation_y[];
extern short kaze_task_glyph_rotation_z[];
extern short kaze_task_name_letter_rotation_x[];
extern int kaze_task_name_letter_pos_x;

/**
 * kaze_task_name_letter_models_link  (menu, 0x8005abb4)
 *
 * Links up to 3 letter models for the current task name (chars
 * kaze_task_name_letter0_table[kaze_task_record_index*3]) into GsDOBJ2 slots
 * kaze_task_name_letter_dobjs with coords kaze_task_name_letter_coordinates,
 * positioned x=-0x140/0/0x140, y=-0x260, z=-0x800.
 */

void kaze_task_name_letter_models_link(void)
{
    short glyph_coord;
    int slot_off;
    int slot_idx;
    unsigned int n;
    int i;
    signed char *base;

    i = 0;
    base = kaze_task_name_letter0_table;
    do {
        n = (unsigned int)*(base + kaze_task_record_index * 3 + i);
        if (0x26 < n) {
            return;
        }
        GsInitCoordinate2((void *)0x0, (void *)(kaze_task_name_letter_coordinates + kaze_task_letter_count * 0x50));
        GsLinkObject4(kaze_menu_task_model_ptr + 0xc, (void *)(kaze_task_name_letter_dobjs + kaze_task_letter_count * 0x10), n);
        slot_idx = kaze_task_letter_count;
        slot_off = slot_idx * 0x10;
        *(char **)(kaze_task_name_letter_dobj_coord_ptr + slot_off) = kaze_task_name_letter_coordinates + slot_idx * 0x50;
        glyph_coord = kaze_task_glyph_rotation_x[n * 4];
        *(int *)(kaze_task_name_letter_dobjs + slot_off) = 0;
        kaze_task_name_letter_rotation_x[slot_idx * 4] = glyph_coord;
        __asm__ volatile("" : : : "memory");
        kaze_task_name_letter_rotation_y[slot_idx * 4] = kaze_task_glyph_rotation_y[n * 4];
        __asm__ volatile("" : : : "memory");
        kaze_task_name_letter_rotation_z[slot_idx * 4] = kaze_task_glyph_rotation_z[n * 4];
        if (slot_idx == 1) goto set68;
        if (slot_idx >= 2) goto ge2;
        if (slot_idx == 0) goto set58;
        goto join;
    ge2:
        if (slot_idx == 2) goto set78;
        goto join;
    set58:
        kaze_task_name_letter_pos_x = 0xfffffec0;
        goto join;
    set68:
        kaze_task_name_letter_slot1_x = 0;
        goto join;
    set78:
        kaze_task_name_letter_slot2_x = 0x140;
    join:
        i = i + 1;
        {
            register int tidx __asm__("$4");
            tidx = kaze_task_letter_count * 0x10;
            *(int *)(kaze_task_name_letter_pos_y + tidx) = 0xfffffda0;
            *(int *)(kaze_task_name_letter_pos_z + tidx) = 0xfffff800;
        }
        kaze_task_letter_count = kaze_task_letter_count + 1;
    } while (i < 3);
}

extern int rsin(int);
extern int rcos(int);
extern void GsSetFlatLight(int, void *);
extern int kaze_task_menu_state;
extern int kaze_task_carousel_angle_ring;
extern int kaze_task_carousel_transition_ticks;
extern int kaze_task_carousel_transition_duration;
extern int kaze_task_glyph_coordinates;
extern int kaze_task_carousel_glyph_pos_x;
extern int kaze_task_carousel_glyph_pos_y[];
extern int kaze_task_carousel_glyph_pos_z;
extern int kaze_gs_light0_dir_x[];
extern short kaze_menu_camera_rot_x;
extern int kaze_menu_camera_pos_x;
extern int kaze_menu_camera_coord_flags;

/**
 * kaze_task_carousel_intro_anim  (menu, 0x8005ada8)
 *
 * Carousel intro/zoom transition: ramps camera fields
 * kaze_menu_camera_pos_x/e4/e8 and view via kaze_gs_view_set_pose_94c8 over
 * 0x40 frames of kaze_task_transition_counter, drops ring glyphs into place
 * from rsin/rcos of kaze_task_carousel_angle_ring, ramps flat light/ambient,
 * then sets task-menu state kaze_task_menu_state=0x28.
 */

void kaze_task_carousel_intro_anim(void)
{
    int glyph_i;
    int amb_scaled;
    int frame_left;
    int ang_ofs;
    int cc_ofs;
    int amb;

    register int dnum __asm__("$4");
    register int dden __asm__("$2");
    register int res __asm__("$2");
    short *pRot;
    int pad;

    __asm__("" : : "m"(pad));
    frame_left = 0x40 - kaze_task_transition_counter;
    pRot = &kaze_menu_camera_rot_x;
    if (0 < *pRot) {
        *pRot = frame_left * 8;
    }
    if (*pRot < 0) {
        *pRot = frame_left * -0x200 / 0x40;
    }
    if ((kaze_balloon_view_zoom_dir_counter & 1) != 0) {
        kaze_menu_camera_pos_x = 0;
        kaze_menu_camera_scroll = frame_left * 0xe0 / 0x40 + -0x180;
        kaze_menu_camera_pos_z = frame_left * 6;
    } else {
        kaze_menu_camera_scroll = frame_left * 6 + -0x180;
        kaze_menu_camera_pos_x = 0;
        kaze_menu_camera_pos_z = frame_left * 0x10;
    }
    __asm__("" : : : "memory");
    kaze_menu_camera_coord_flags = 0;
    kaze_gs_view_set_pose_94c8((frame_left * 2 + -0x80) * 0x10000 >> 0x10, 0, 0, 0,
                               frame_left * 0xb + -0x2c0, frame_left * 0x32 + -0xe00);
    glyph_i = 0;
    if (0 < kaze_task_glyph_count) {
        int *pada = kaze_task_carousel_glyph_pos_y;
        int da_ofs;
        cc_ofs = 0;
        ang_ofs = 0;
        da_ofs = 0;
        do {
            int *pang = (int *)((int)&kaze_task_carousel_angle_ring + ang_ofs);
            {
                register int trig_prod __asm__("$2");
                trig_prod = rsin(*pang) * 0x600;
                if (trig_prod < 0) {
                    trig_prod = trig_prod + 0xfff;
                }
                *(int *)((int)&kaze_task_carousel_glyph_pos_x + da_ofs) = trig_prod >> 0xc;
            }
            {
                register int trig_prod __asm__("$2");
                trig_prod = rcos(*pang) * 0x600;
                if (trig_prod < 0) {
                    trig_prod = trig_prod + 0xfff;
                }
                *(int *)((int)&kaze_task_carousel_glyph_pos_z + da_ofs) = trig_prod >> 0xc;
            }
            *(int *)((int)&kaze_task_carousel_glyph_pos_y + da_ofs) = frame_left * -0x400 / 0x40;
            if (glyph_i == 0) {
                dnum = kaze_task_carousel_transition_ticks << 10;
                dden = kaze_task_carousel_transition_duration;
                dnum = dnum / dden;
                res = rcos(dnum);
                pada[0] = pada[0] + kaze_task_carousel_transition_amplitude * res / 0x1000;
            }
            *(int *)((int)&kaze_task_glyph_coordinates + cc_ofs) = 0;
            cc_ofs = cc_ofs + 0x50;
            ang_ofs = ang_ofs + 4;
            da_ofs = da_ofs + 0x10;
        } while (++glyph_i < kaze_task_glyph_count);
    }
    {
        int *pflat = &kaze_gs_light0_dir_y;
        int lv = frame_left * -0x1000 / 0x40 + 0x1000;
        *pflat = lv;
        kaze_gs_light0_dir_z = lv;
        amb = frame_left * 0x14 + 0x300;
        GsSetFlatLight(0, (void *)(pflat - 1));
    }
    amb_scaled = amb * 0x10000 >> 0x10;
    GsSetAmbient(amb_scaled, amb_scaled, amb_scaled);
    if (kaze_task_transition_counter < 0x40) {
        kaze_task_transition_counter = kaze_task_transition_counter + 1;
        return;
    }
    kaze_task_menu_state = 0x28;
    kaze_task_transition_counter = 0;
}

/**
 * kaze_task_carousel_update  (menu, 0x8005b124)
 *
 * Per-frame letter-carousel update dispatcher: rotate input, rotation
 * interp, ring layout, confirm input, add-letter anim, delete input, delete
 * anim, and camera bob
 * (kaze_task_carousel_rotate_input/b628/c4c4/b7b0/b898/bc94/bd68/c6c0).
 */

void kaze_task_carousel_update(void)
{
    kaze_task_carousel_rotate_input();
    kaze_task_carousel_rotate_interp();
    kaze_task_carousel_layout_update();
    kaze_task_carousel_select_input();
    kaze_task_letter_add_anim_commit();
    kaze_task_letter_delete_input();
    kaze_task_letter_delete_anim();
    kaze_task_menu_camera_bob();
}
