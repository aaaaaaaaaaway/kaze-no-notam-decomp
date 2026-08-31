#include "types.h"
#include "kaze_syms.h"
extern unsigned char kaze_ending_text_glyph_width_table;
extern unsigned char kaze_ending_text_glyph_height_table;
extern short kaze_ending_text_line_width_table;
extern short D_8008E704;
extern int kaze_ending_light0_dir_x;
extern int kaze_ending_light0_dir_y;
extern int kaze_ending_light0_dir_z;
extern u8 kaze_ending_light0_color_r;
extern u8 kaze_ending_light0_color_g;
extern u8 kaze_ending_light0_color_b;
extern int kaze_ending_camera_matrix;
extern int kaze_ending_camera_pos_x;
extern int kaze_ending_camera_pos_y;
extern int kaze_ending_camera_pos_z;
extern int kaze_ending_credit_title_pos[];
extern int kaze_ending_credit_line_cursor[];
extern int kaze_ending_debug_camera_mode;
extern int kaze_ending_path_start_x;
extern int kaze_ending_path_start_y;
extern int kaze_ending_path_start_z;
extern int kaze_ending_path_end_x;
extern int kaze_ending_path_end_y;
extern int kaze_ending_path_end_z;
extern int kaze_ending_path_speed;
extern void D_dead_call(void);
extern void GsSetAmbient(int, int, int);
extern void RotMatrixYXZ(void *, void *);
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern char kaze_ending_credit_title_sprites[];
extern char kaze_ending_credit_line_sprites[];
extern int kaze_menu_camera_pos_z;
extern int kaze_menu_camera_scroll;
typedef struct {
    int path0;
    int path1;
    int path2;
    int path3;
    int path4;
    int path5;
    s16 cam_x;
    s16 cam_y;
    s16 cam_z;
    s16 pad1;
    int rot_x;
    int rot_y;
    int rot_z;
    int light_x;
    int light_y;
    int light_z;
    u8 light_r;
    u8 light_g;
    u8 light_b;
    u8 pad2;
    u16 ambient_r;
    u16 ambient_g;
    u16 ambient_b;
    u16 pad3;
    int speed;
} EndingKeyframe;

#define MEM_BARRIER() __asm__ __volatile__("" : : : "memory")
extern int kaze_ending_credit_page_index;
extern EndingKeyframe kaze_ending_keyframe_table[];
extern s16 D_800AE3C4;
extern int kaze_ending_coord2_buffer;
extern int kaze_ending_camera_view2;
extern void kaze_trans_matrix(void *, void *);
extern void RotTrans(void *, void *, long *);
extern void GsSetFlatLight(int, void *);
extern void GsSetView2(void *);

/**
 * kaze_ending_keyframe_load  (menu, 0x80064ab4)
 *
 * Loads ending keyframe kaze_ending_credit_page_index from the 0x48-stride
 * table at kaze_ending_keyframe_table: segment start/end positions, camera
 * angles/pos, light vector/color, ambient and speed; rebuilds camera
 * matrices and calls GsSetAmbient/GsSetFlatLight/GsSetView2.
 */

void kaze_ending_keyframe_load(void)
{
    extern s16 kaze_ending_camera_angle_z;
    extern s16 kaze_ending_camera_angle_y;
    extern s16 kaze_ending_camera_angle_x;
    long scratch[2];
    register EndingKeyframe *kf asm("$16") = &kaze_ending_keyframe_table[kaze_ending_credit_page_index];
    register int path0 asm("$3");
    register int path1 asm("$5");
    register int path2 asm("$6");
    register int path3 asm("$7");
    register int path4 asm("$8");
    register int path5 asm("$9");
    register int *cam_pos asm("$18");
    register int *light asm("$19");
    register void *matrix asm("$17");
    register s16 *rot asm("$4");
    int light_z;

    __asm__ __volatile__(
        "lw\t%0,0(%6)\n\t"
        "lw\t%1,4(%6)\n\t"
        "lw\t%2,8(%6)\n\t"
        "lw\t%3,12(%6)\n\t"
        "lw\t%4,16(%6)\n\t"
        "lw\t%5,20(%6)"
        : "=r"(path0), "=r"(path1), "=r"(path2),
          "=r"(path3), "=r"(path4), "=r"(path5)
        : "r"(kf));

    cam_pos = &kaze_ending_camera_pos_x;
    *cam_pos = kf->cam_x;
    MEM_BARRIER();
    kaze_ending_camera_pos_y = kf->cam_y;
    MEM_BARRIER();
    kaze_ending_camera_pos_z = kf->cam_z;
    MEM_BARRIER();
    rot = &kaze_ending_camera_angle_x;
    *rot = kf->rot_x;
    MEM_BARRIER();
    kaze_ending_camera_angle_y = kf->rot_y;
    MEM_BARRIER();
    kaze_ending_camera_angle_z = kf->rot_z;
    MEM_BARRIER();
    light = &kaze_ending_light0_dir_x;
    *light = kf->light_x;
    MEM_BARRIER();
    kaze_ending_light0_dir_y = kf->light_y;
    MEM_BARRIER();
    light_z = kf->light_z;
    kaze_ending_path_start_x = path0;
    kaze_ending_path_start_y = path1;
    kaze_ending_path_start_z = path2;
    kaze_ending_path_end_x = path3;
    kaze_ending_path_end_y = path4;
    kaze_ending_path_end_z = path5;
    kaze_ending_light0_dir_z = light_z;
    MEM_BARRIER();
    kaze_ending_light0_color_r = kf->light_r;
    MEM_BARRIER();
    kaze_ending_light0_color_g = kf->light_g;
    MEM_BARRIER();
    matrix = &kaze_ending_camera_matrix;
    kaze_ending_light0_color_b = kf->light_b;
    MEM_BARRIER();
    kaze_ending_path_speed = kf->speed;

    RotMatrixYXZ(rot, matrix);
    kaze_trans_matrix(matrix, cam_pos);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    RotTrans(&D_800AE3C4, cam_pos, scratch);
    kaze_trans_matrix(matrix, cam_pos);
    kaze_ending_coord2_buffer = 0;
    MEM_BARRIER();
    GsSetAmbient(kf->ambient_r, kf->ambient_g, kf->ambient_b);
    GsSetFlatLight(0, light);
    GsSetView2(&kaze_ending_camera_view2);
}

extern char kaze_ending_path_keyframe_table[];

/**
 * kaze_ending_credit_text_layout  (menu, 0x80064c80)
 *
 * Lays out credit text for page kaze_ending_credit_page_index: centers title
 * sprites and per-line glyph sprites using widths/UVs from the
 * kaze_ending_text_glyph_width_table glyph table and line indices from the
 * kaze_ending_path_keyframe_table page table (CLUT row from entry field
 * +0x10).
 */

void kaze_ending_credit_text_layout(void)
{
    int j4_floor;
    int unused_int0;
    int row_idx;
    int unused_int1;
    int *entry;
    int c1;
    int stage;

    stage = kaze_ending_credit_page_index;
    __asm__ volatile("" : : "r"(stage) : "memory");
    { int _s = stage * 0x1c; entry = (int *)(kaze_ending_path_keyframe_table + _s); }
    c1 = 0;
    if (*entry == 1) {
        int gi2;
        int dim3;
        char *text_ptr;
        register int *q49c asm("$7");
        { int *qraw = kaze_ending_credit_title_pos;
          __asm__ volatile("" : "=r"(q49c) : "0"(qraw));
          __asm__("" : : "r"(qraw)); }
        text_ptr = kaze_ending_credit_title_sprites;
        __asm__ volatile("" : : "r"(text_ptr));
        {
            int dim1;
            register int raw6 asm("$2");
            dim1 = entry[1];
            kaze_ending_credit_line_cursor[0] = 0x10;
            raw6 = dim1 * 6;
            dim3 = entry[3];
            __asm__ volatile("" : "=r"(raw6) : "0"(raw6), "r"(dim3));
            gi2 = raw6;
        }
        __asm__ ("" : : "r"(gi2));
        kaze_ending_credit_line_cursor[1] = (dim3 * -0x1c) / 2;
        { int e0 = (0xa0 - *(short *)((char *)&kaze_ending_text_line_width_table + gi2)) / 2;
          kaze_ending_credit_title_pos[1] = kaze_ending_credit_line_cursor[1] + -0x14;
          q49c[0] = e0; }
        do {
      char * pp;
            pp = &kaze_ending_credit_title_sprites[c1 * 0x28];
            pp[0xc] = (&kaze_ending_text_glyph_width_table)[gi2];
            pp[0xd] = (&kaze_ending_text_glyph_height_table)[gi2];
            *(short *)(pp + 8) = (short)q49c[0];
            *(short *)(pp + 10) = (short)q49c[1];
            { short *_p = (short *)pp; _p[8] = *(short *)((char *)&kaze_ending_text_line_width_table + gi2); }
            { short *_p = (short *)pp; _p[9] = *(short *)((char *)&D_8008E704 + gi2); }
            c1 = c1 + 1;
        } while (c1 < 2);
        if (0 < entry[3]) {
            int outer9;
            int sv;
            int seedoff;
            char *q4ac;
            int *pw;
            int *q554b;
            outer9 = 0;
            q4ac = kaze_ending_credit_line_sprites;
            q554b = kaze_ending_credit_line_cursor;
            sv = 0;
            pw = entry;
            seedoff = 0;
            do {
                int inner;
                int off;
                seedoff = outer9 * 0x14;
                inner = 0;
                off = seedoff;
                do {
                    int j4;
                    int shifted;
		          char * pB;
                    register int firstv asm("$2");
                    firstv = entry[outer9 + 4];
                    if (firstv < 0) {
                        firstv = firstv + 7;
                    }
                    pB = (char *)(off + (int)q4ac);
                    shifted = (firstv >> 3) << 7;
                    __asm__ volatile("" : "=r"(firstv) : "0"(shifted));
                    pB[0xc] = (char)firstv;
                    { int _mt = firstv; __asm__("" :: "r"(_mt) : "memory"); }
                    j4 = entry[outer9 + 4];
                    j4_floor = j4;
                    if (j4 < 0) {
                        j4_floor = j4 + 7;
                    }
                    pB[0xd] = (char)((j4 - ((j4_floor >> 3) << 3)) * 0x18 + 0x3c);
                    *(short *)(pB + 8) = (short)q554b[0];
                    { int y = q554b[1];
                      __asm__ volatile("" : : "r"(y));
                      inner = inner + 1;
                      *(short *)(pB + 0xa) = (short)(y + sv); }
                    off = off + 0x3c;
                } while (inner < 2);
                sv = sv + 0x1c;
                pw = pw + 1;
                outer9 = outer9 + 1;
            } while (outer9 < entry[3]);
        }
    }
    else {
        register char * base42c asm("$16");
        register char *base4ac asm("$25");
        int *p554;
        int *p49c;
        volatile int * pw;
        register int cnt asm("$10");
        int seed9;
        register int seed58 asm("$14");
        register int seedA0 asm("$15");
        cnt = 0;
        if (0 < *entry) {
            p554 = kaze_ending_credit_line_cursor;
            p49c = kaze_ending_credit_title_pos;
            __asm__("" :: "r"(p49c) : "memory");
            base42c = kaze_ending_credit_title_sprites;
            __asm__("" :: "r"(base42c) : "memory");
            base4ac = kaze_ending_credit_line_sprites;
            pw = entry;
            seed9 = 0;
            seedA0 = -0x37;
            seed58 = -0x23;
            do {
                int inner11;
                int inner40;
                int inner60;
                int goff;
                int dim1;
                register int raw_goff asm("$2");
                inner11 = 0;
                inner60 = seed9;
                inner40 = seed9;
                dim1 = pw[1];
                p554[0] = 0x10;
                p554[1] = seed58;
                raw_goff = dim1 * 6;
                goff = raw_goff;
                { int _mt = goff; __asm__("" :: "r"(_mt) : "memory"); }
                p49c[0] = (0xa0 - *(short *)((char *)&kaze_ending_text_line_width_table + raw_goff)) / 2;
                p49c[1] = seedA0;
                do {
          char * pA;
                    pA = &kaze_ending_credit_title_sprites[inner40];
                    { int _mt = inner11; __asm__("" :: "r"(_mt) : "memory"); }
                    pA[0xc] = (&kaze_ending_text_glyph_width_table)[goff];
                    pA[0xd] = (&kaze_ending_text_glyph_height_table)[goff];
                    *(short *)(pA + 8) = (short)p49c[0];
                    { short *_p = (short *)pA; _p[5] = (short)p49c[1]; }
                    { short *_p = (short *)pA; _p[8] = *(short *)((char *)&kaze_ending_text_line_width_table + goff); }
                    { short *_p = (short *)pA; _p[9] = *(short *)((char *)&D_8008E704 + goff); }
                    {
                        int k6;
                        int t2;
            char * pB;
                        row_idx = pw[4];
                        if (row_idx < 0) {
                            row_idx = row_idx + 7;
                        }
                        pB = (char *)(inner60 + (int)base4ac);
                        pB[0xc] = (char)((row_idx >> 3) << 7);
                        k6 = pw[4];
                        t2 = k6;
                        if (k6 < 0) {
                            t2 = k6 + 7;
                        }
                        pB[0xd] = (char)((k6 - ((t2 >> 3) << 3)) * 0x18 + 0x3c);
                        *(short *)(pB + 8) = (short)p554[0];
                        inner60 = inner60 + 0x3c;
                        inner40 = inner40 + 0x28;
                        inner11 = inner11 + 1;
                        *(short *)(pB + 0xa) = (short)p554[1];
                    }
                } while (inner11 < 2);
                pw = pw + 1;
                seed9 = seed9 + 0x14;
                seedA0 = seedA0 + 0x3c;
                seed58 = seed58 + 0x3c;
                cnt = cnt + 1;
            } while (cnt < *entry);
        }
    }
    return;
    D_dead_call();
}

extern s16 kaze_ending_camera_roll;
extern volatile int kaze_menu_input_replay_flags;
extern int kaze_gs_view_coord2;
extern u16 kaze_menu_camera_rot_y;
extern int kaze_menu_camera_pos_x;
extern int kaze_menu_camera_coord_flags;
extern int kaze_pad1_edge_mask;
extern void RotMatrix(void *, void *);

/**
 * kaze_ending_debug_camera_control  (menu, 0x8006502c)
 *
 * Hidden ending viewer control: select (bit 0x100 of kaze_pad1_edge_mask)
 * cycles mode kaze_ending_debug_camera_mode; modes adjust camera angles/pos
 * (kaze_ending_camera_angle_x/3b4), balloon pos kaze_menu_camera_pos_x.., or
 * light vector/color kaze_ending_light0_dir_x.. from pad mask
 * kaze_menu_input_replay_flags.
 */

void kaze_ending_debug_camera_control(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    extern u16 kaze_ending_camera_angle_z;
    extern u16 kaze_ending_camera_angle_y;
    extern u16 kaze_ending_camera_angle_x;
    long sp10[1];
    int mode;
    int mask;
    int m;
    u8 *p;
    u8 v;
    u16 *pu;
    int *pi;
    int *pm;
    register int *pm0 asm("$16");
    register int *pm1 asm("$17");

    if (kaze_pad1_edge_mask & 0x100) {
        mode = kaze_ending_debug_camera_mode + 1;
        kaze_ending_debug_camera_mode = mode;
        if (!(mode & 3)) {
            kaze_ending_debug_camera_mode = 0;
        }
    } else {
        if (kaze_pad1_edge_mask & 0x800) {
            kaze_ending_camera_reset_defaults();
            pu = &kaze_ending_camera_angle_x;
            pm0 = &kaze_ending_camera_matrix;
            __asm__ volatile("" : "=r"(pm0) : "0"(pm0));
            pm = pm0;
        } else {
            pu = &kaze_ending_camera_angle_x;
            pm1 = &kaze_ending_camera_matrix;
            __asm__ volatile("" : "=r"(pm1) : "0"(pm1));
            pm = pm1;
            *pu = arg0;
            kaze_ending_camera_angle_y = arg1;
            kaze_ending_camera_angle_z = arg2;
            {
                register int *pi0 asm("$16");
                pi0 = &kaze_ending_camera_pos_x;
                __asm__ volatile("" : "=r"(pi0) : "0"(pi0));
                *pi0 = arg3;
            }
            kaze_ending_camera_pos_y = arg4;
            kaze_ending_camera_pos_z = arg5;
        }
        RotMatrix(pu, pm);
        kaze_trans_matrix(&kaze_ending_camera_matrix, &kaze_ending_camera_pos_x);
        kaze_gs_view_coord2 = 0;
    }
    switch (kaze_ending_debug_camera_mode) {
    case 0:
        if (kaze_menu_input_replay_flags & 0x1000) {
            u16 *r = &kaze_ending_camera_angle_x;
            *r += 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x4000) {
            u16 *r = &kaze_ending_camera_angle_x;
            *r -= 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x2000) {
            u16 *r = &kaze_ending_camera_angle_y;
            *r += 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x8000) {
            u16 *r = &kaze_ending_camera_angle_y;
            *r -= 0x20;
        }
        if (kaze_menu_input_replay_flags & 1) {
            u16 *r = &kaze_ending_camera_angle_z;
            *r += 0x20;
        }
        if (kaze_menu_input_replay_flags & 2) {
            u16 *r = &kaze_ending_camera_angle_z;
            *r -= 0x20;
        }
        m = kaze_menu_input_replay_flags;
        if (m & 0x20) {
            kaze_ending_camera_roll = 0x20;
        } else if (m & 0x80) {
            kaze_ending_camera_roll = -0x20;
        } else {
            kaze_ending_camera_roll = 0;
        }
        RotMatrixYXZ(&kaze_ending_camera_angle_x, &kaze_ending_camera_matrix);
        kaze_trans_matrix(&kaze_ending_camera_matrix, &kaze_ending_camera_pos_x);
        SetRotMatrix(&kaze_ending_camera_matrix);
        SetTransMatrix(&kaze_ending_camera_matrix);
        RotTrans(&D_800AE3C4, &kaze_ending_camera_pos_x, sp10);
        kaze_trans_matrix(&kaze_ending_camera_matrix, &kaze_ending_camera_pos_x);
        kaze_ending_coord2_buffer = 0;
        break;
    case 1:
        if (kaze_menu_input_replay_flags & 0x2000) {
            int *r = &kaze_ending_camera_pos_x;
            *r += 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x8000) {
            int *r = &kaze_ending_camera_pos_x;
            *r -= 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x1000) {
            int *r = &kaze_ending_camera_pos_y;
            *r -= 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x4000) {
            int *r = &kaze_ending_camera_pos_y;
            *r += 0x20;
        }
        if (kaze_menu_input_replay_flags & 1) {
            int *r = &kaze_ending_camera_pos_z;
            *r += 0x20;
        }
        if (kaze_menu_input_replay_flags & 2) {
            int *r = &kaze_ending_camera_pos_z;
            *r -= 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x10) {
            u16 *r = &kaze_ending_camera_angle_x;
            *r += 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x40) {
            u16 *r = &kaze_ending_camera_angle_x;
            *r -= 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x20) {
            u16 *r = &kaze_ending_camera_angle_y;
            *r += 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x80) {
            u16 *r = &kaze_ending_camera_angle_y;
            *r -= 0x20;
        }
        RotMatrix(&kaze_ending_camera_angle_x, &kaze_ending_camera_matrix);
        kaze_trans_matrix(&kaze_ending_camera_matrix, &kaze_ending_camera_pos_x);
        kaze_ending_coord2_buffer = 0;
        break;
    case 2:
        if (kaze_menu_input_replay_flags & 0x1000) {
            int *r = &kaze_menu_camera_scroll;
            *r -= 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x4000) {
            int *r = &kaze_menu_camera_scroll;
            *r += 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x8000) {
            int *r = &kaze_menu_camera_pos_x;
            *r -= 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x2000) {
            int *r = &kaze_menu_camera_pos_x;
            *r += 0x20;
        }
        if (kaze_menu_input_replay_flags & 8) {
            int *r = &kaze_menu_camera_pos_z;
            *r -= 0x20;
        }
        if (kaze_menu_input_replay_flags & 2) {
            int *r = &kaze_menu_camera_pos_z;
            *r += 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x20) {
            u16 *r = &kaze_menu_camera_rot_y;
            *r -= 0x20;
        }
        if (kaze_menu_input_replay_flags & 0x80) {
            u16 *r = &kaze_menu_camera_rot_y;
            *r += 0x20;
        }
        kaze_menu_camera_coord_flags = 0;
        break;
    case 3:
        m = kaze_menu_input_replay_flags;
        if (m & 0x80) {
            if (m & 0x1000) {
                u8 *pb = &kaze_ending_light0_color_r;
                if (*pb < 0xFF) {
                    *pb = *pb + 1;
                }
            }
            if (kaze_menu_input_replay_flags & 0x4000) {
                p = &kaze_ending_light0_color_r;
                goto shared_dec;
            }
        } else if (m & 0x10) {
            if (m & 0x1000) {
                u8 *pb = &kaze_ending_light0_color_g;
                if (*pb < 0xFF) {
                    *pb = *pb + 1;
                }
            }
            if (kaze_menu_input_replay_flags & 0x4000) {
                p = &kaze_ending_light0_color_g;
                goto shared_dec;
            }
        } else {
            if (m & 0x20) {
                    if (m & 0x1000) {
                        u8 *pb = &kaze_ending_light0_color_b;
                        if (*pb < 0xFF) {
                            *pb = *pb + 1;
                        }
                    }
                if (kaze_menu_input_replay_flags & 0x4000) {
                    p = &kaze_ending_light0_color_b;
                shared_dec:
                    v = *p;
                    if (v != 0) {
                        *p = v - 1;
                    }
                }
            } else {
                if (m & 0x1000) {
                    int *r = &kaze_ending_light0_dir_y;
                    *r += 0x20;
                }
                if (kaze_menu_input_replay_flags & 0x4000) {
                    int *r = &kaze_ending_light0_dir_y;
                    *r -= 0x20;
                }
                if (kaze_menu_input_replay_flags & 0x2000) {
                    int *r = &kaze_ending_light0_dir_x;
                    *r -= 0x20;
                }
                if (kaze_menu_input_replay_flags & 0x8000) {
                    int *r = &kaze_ending_light0_dir_x;
                    *r += 0x20;
                }
                if (kaze_menu_input_replay_flags & 8) {
                    int *r = &kaze_ending_light0_dir_z;
                    *r += 0x20;
                }
                if (kaze_menu_input_replay_flags & 2) {
                    int *r = &kaze_ending_light0_dir_z;
                    *r -= 0x20;
                }
            }
        }
        break;
    }
    GsSetFlatLight(0, &kaze_ending_light0_dir_x);
    GsSetView2(&kaze_ending_camera_view2);
}
