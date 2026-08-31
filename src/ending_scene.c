#include "types.h"
#include "kaze_syms.h"
extern int kaze_ending_scene_transition_counter;
extern int kaze_ending_scene_phase_index;
extern int kaze_ending_light0_dir_x;
extern int kaze_ending_light0_dir_y;
extern int kaze_ending_light0_dir_z;
extern u8 kaze_ending_light0_color_r;
extern u8 kaze_ending_light0_color_g;
extern u8 kaze_ending_light0_color_b;
extern int kaze_ending_light1_pos_x;
extern int kaze_ending_light1_pos_y;
extern int kaze_ending_light1_pos_z;
extern u8 kaze_ending_light1_color_r;
extern u8 kaze_ending_light1_color_g;
extern u8 kaze_ending_light1_color_b;
extern char *kaze_ending_coord2_ptr;
extern int kaze_ending_camera_pos_x;
extern int kaze_ending_camera_pos_y;
extern int kaze_ending_camera_pos_z;
extern short D_800AE3C6;
extern char kaze_ending_flame_poly_verts[];
extern int kaze_ending_flame_matrix;
extern int kaze_ending_flame_pos_x;
extern int kaze_ending_flame_pos_y;
extern int kaze_ending_flame_pos_z;
extern s16 kaze_ending_flame_quad_v0_y;
extern s16 kaze_ending_flame_quad_v0_z;
extern s16 kaze_ending_flame_quad_v1_x;
extern s16 kaze_ending_flame_quad_v1_y;
extern s16 kaze_ending_flame_quad_v1_z;
extern volatile s16 kaze_ending_flame_quad_v2_x;
extern s16 kaze_ending_flame_quad_v2_y;
extern s16 kaze_ending_flame_quad_v2_z;
extern s16 kaze_ending_flame_quad_v3_x;
extern s16 kaze_ending_flame_quad_v3_y;
extern s16 kaze_ending_flame_quad_v3_z;
extern int D_800AE74C;
extern int kaze_ending_credit_page_texture_table[];
extern void GsSetAmbient(int, int, int);
extern void GsSetLightMode(int);
extern void RotAverageNclip4(s16 *, s16 *, s16 *, s16 *, int *, int *, int *, int *, int *, int *, int *);
extern int kaze_menu_camera_pos_z;
extern int kaze_menu_camera_scroll;
typedef struct {
    int m[8];
} Block32;

extern void GsInitCoordinate2(int arg0, void *arg1);
extern void GsSetView2(void *arg0);
extern void kaze_gs_set_projection(int arg0);
extern short kaze_ending_camera_angle_x;
extern short kaze_ending_camera_angle_y;
extern short kaze_ending_camera_angle_z;
extern short D_800AE3C4;
extern short kaze_ending_camera_roll;
extern char kaze_ending_coord2_buffer[];
extern Block32 kaze_ending_camera_view2;
extern Block32 kaze_gs_view2_template;

/**
 * kaze_ending_camera_init  (menu, 0x80063d08)
 *
 * Initializes the ending camera: zeroes angle/pos vectors
 * kaze_ending_camera_angle_x/3b4/3c4 (z=-0xbb8), GsInitCoordinate2 on
 * kaze_ending_coord2_buffer, copies the GsVIEW2 template from
 * kaze_gs_view2_template and calls GsSetView2 plus projection 0x180 via
 * kaze_gs_set_projection.
 */

void kaze_ending_camera_init(void)
{
    kaze_ending_camera_pos_x = 0;
    kaze_ending_camera_pos_y = 0;
    kaze_ending_camera_pos_z = -0xBB8;
    kaze_ending_camera_angle_x = 0;
    kaze_ending_camera_angle_y = 0;
    kaze_ending_camera_angle_z = 0;
    D_800AE3C4 = 0;
    D_800AE3C6 = 0;
    kaze_ending_camera_roll = 0;
    GsInitCoordinate2(0, kaze_ending_coord2_buffer);
    kaze_ending_camera_view2 = kaze_gs_view2_template;
    kaze_ending_coord2_ptr = kaze_ending_coord2_buffer;
    GsSetView2(&kaze_ending_camera_view2);
    kaze_gs_set_projection(0x180);
}

extern void GsSetFlatLight(int, void *);

/**
 * kaze_ending_lighting_init  (menu, 0x80063df0)
 *
 * Sets up ending scene lighting: flat light 0 at kaze_ending_light0_dir_x
 * (dir 0,1000,1000, white), zeroed light 1 at kaze_ending_light1_pos_x,
 * GsSetAmbient(0x400) and GsSetLightMode(0).
 */

void kaze_ending_lighting_init(void)
{
    int *p = &kaze_ending_light0_dir_x;

    *p = 0;
    kaze_ending_light0_dir_y = 1000;
    kaze_ending_light0_dir_z = 1000;
    kaze_ending_light0_color_r = 0xff;
    kaze_ending_light0_color_g = 0xff;
    kaze_ending_light0_color_b = 0xff;
    GsSetFlatLight(0, p);
    kaze_ending_light1_pos_x = 0;
    kaze_ending_light1_pos_y = 0;
    kaze_ending_light1_pos_z = 0;
    kaze_ending_light1_color_r = 0;
    kaze_ending_light1_color_g = 0;
    kaze_ending_light1_color_b = 0;
    GsSetAmbient(0x400, 0x400, 0x400);
    GsSetLightMode(0);
}

extern int kaze_menu_camera_pos_x;
extern int kaze_menu_input_replay_flags;

/**
 * kaze_ending_burner_light_update  (menu, 0x80063e94)
 *
 * Burner flame effect during the ending: on pad bit 0x10 of
 * kaze_menu_input_replay_flags ramps kaze_ending_scene_transition_counter
 * and flickers light 1 color/position (light follows balloon
 * kaze_menu_camera_pos_x/e4/e8) and flame quad verts
 * kaze_ending_flame_quad_v0_x..; decays when released.
 */

void kaze_ending_burner_light_update(void)
{
    extern s16 kaze_ending_flame_quad_v0_x;
    int *light;
    int vx;
    s16 *pbc;
    int flag;
    int old;
    register int phase asm("$4");

    light = &kaze_ending_light1_pos_x;
    vx = kaze_menu_camera_pos_x;
    __asm__("" : : "r"(vx));
    flag = kaze_menu_input_replay_flags & 0x10;
    __asm__("" : : "r"(flag));
    *light = vx;
    kaze_ending_light1_pos_y = kaze_menu_camera_scroll + 0x1a4;
    kaze_ending_light1_pos_z = kaze_menu_camera_pos_z;
    if (flag != 0) {
        if (kaze_ending_scene_transition_counter < 0x28) {
            kaze_ending_scene_transition_counter = kaze_ending_scene_transition_counter + 1;
            D_800AE74C = -kaze_ending_scene_transition_counter / 20;
        }
        old = kaze_ending_scene_phase_index;
        kaze_ending_scene_phase_index = old + 1;
        if (old >= 5) {
            kaze_ending_scene_phase_index = 0;
        }
        kaze_ending_light1_color_b = 0;
        kaze_ending_light1_color_r = 0x45 - kaze_ending_scene_phase_index * 3;
        kaze_ending_light1_color_g = 0x25 - kaze_ending_scene_phase_index * 2;
        GsSetFlatLight(1, light);
        kaze_ending_flame_quad_v0_z = 0;
        kaze_ending_flame_quad_v1_z = 0;
        kaze_ending_flame_quad_v2_y = 0;
        kaze_ending_flame_quad_v2_z = 0;
        kaze_ending_flame_quad_v3_y = 0;
        kaze_ending_flame_quad_v3_z = 0;
        phase = kaze_ending_scene_phase_index;
        kaze_ending_flame_quad_v0_x = phase - 0x19;
        kaze_ending_flame_quad_v0_y = phase * 5 - 0x5a;
        kaze_ending_flame_quad_v1_x = 0x19 - phase;
        kaze_ending_flame_quad_v1_y = kaze_ending_flame_quad_v0_y;
        kaze_ending_flame_quad_v2_x = kaze_ending_flame_quad_v0_x;
        kaze_ending_flame_quad_v3_x = kaze_ending_flame_quad_v1_x;
    } else {
        if (kaze_ending_scene_transition_counter > 0) {
            kaze_ending_scene_transition_counter = kaze_ending_scene_transition_counter - 1;
            D_800AE74C = -kaze_ending_scene_transition_counter / 20;
        }
        if (kaze_ending_light1_color_r > 2) {
            kaze_ending_light1_color_r = kaze_ending_light1_color_r - 2;
        }
        if (kaze_ending_light1_color_g != 0) {
            kaze_ending_light1_color_g = kaze_ending_light1_color_g - 1;
        }
        GsSetFlatLight(1, light);
        pbc = &kaze_ending_flame_quad_v0_x;
        if (*pbc < 0) {
            s16 cc = kaze_ending_flame_quad_v2_x;
            *pbc = *pbc + 1;
            kaze_ending_flame_quad_v1_x = kaze_ending_flame_quad_v1_x - 1;
            kaze_ending_flame_quad_v2_x = cc;
        }
        if (kaze_ending_flame_quad_v0_y < 0) {
            kaze_ending_flame_quad_v0_y = kaze_ending_flame_quad_v0_y + 3;
            kaze_ending_flame_quad_v1_y = kaze_ending_flame_quad_v1_y + 3;
        }
    }
}

extern s16 kaze_ending_flame_angle_x;
extern int kaze_ending_ot_base_ptrs[];
extern void RotMatrix(s16 *, int *);
extern void kaze_trans_matrix(int *, int *);
extern void GsGetLws(int, int *, int *);
extern void GsSetLightMatrix(int *);
extern void GsSetLsMatrix(int *);
extern void AddPrim(void *, void *);

/**
 * kaze_ending_flame_quad_render  (menu, 0x80064134)
 *
 * Renders the burner flame POLY_FT4: positions its coordinate at balloon pos
 * (Y+0x1d6), builds light/screen matrices via GsGetLws, RotAverageNclip4
 * transforms the quad kaze_ending_flame_quad_v0_x.. and AddPrims it at the
 * computed OT depth.
 */

void kaze_ending_flame_quad_render(int idx)
{
    extern s16 kaze_ending_flame_quad_v0_x[];
    int m68[8];
    int m48[8];
    int flag;
    int otz;
    long long dmy;
    s16 *sv;
    int *mtx;
    int *tvec;

    sv = &kaze_ending_flame_angle_x;
    mtx = &kaze_ending_flame_matrix;
    tvec = &kaze_ending_flame_pos_x;
    *tvec = kaze_menu_camera_pos_x;
    kaze_ending_flame_pos_y = kaze_menu_camera_scroll + 0x1d6;
    kaze_ending_flame_pos_z = kaze_menu_camera_pos_z;
    RotMatrix(sv, mtx);
    kaze_trans_matrix(mtx, tvec);
    GsGetLws((int)mtx - 4, m68, m48);
    GsSetLightMatrix(m68);
    GsSetLsMatrix(m48);
    RotAverageNclip4(kaze_ending_flame_quad_v0_x,
                     (s16 *)((int)kaze_ending_flame_quad_v0_x + 8),
                     (s16 *)((int)kaze_ending_flame_quad_v0_x + 0x10),
                     (s16 *)((int)kaze_ending_flame_quad_v0_x + 0x18),
                     (int *)(idx * 0x28 + (int)kaze_ending_flame_poly_verts),
                     (int *)(idx * 0x28 + ((int)kaze_ending_flame_poly_verts + 8)),
                     (int *)(idx * 0x28 + ((int)kaze_ending_flame_poly_verts + 0x10)),
                     (int *)(idx * 0x28 + ((int)kaze_ending_flame_poly_verts + 0x18)),
                     &flag, &otz, (int *)&dmy);
    otz = otz / 4;
    if (otz - 1U < 0xfff) {
        AddPrim((void *)(kaze_ending_ot_base_ptrs[idx * 5] + otz * 4),
                (void *)(idx * 0x28 + ((int)kaze_ending_flame_poly_verts - 8)));
    }
    *(int *)((int)mtx - 4) = 0;
}

extern int kaze_ending_credit_page_index;
extern void kaze_tim_load_to_vram(int, int, int, int, int);

/**
 * kaze_ending_credit_page_upload  (menu, 0x800642b0)
 *
 * Uploads the current credit page texture
 * (&kaze_ending_credit_page_texture_table)[kaze_ending_credit_page_index] to
 * VRAM at (0x140,0x100) via kaze_tim_load_to_vram.
 */

void kaze_ending_credit_page_upload(void)
{
    kaze_tim_load_to_vram(kaze_ending_credit_page_texture_table[kaze_ending_credit_page_index], 0x140, 0x100, 0, 0);
}

extern u8 kaze_ending_screen_move_prims[];

/**
 * kaze_ending_screen_move_prim_add  (menu, 0x800642f4)
 *
 * Adds one DR_MOVE screen-copy prim from kaze_ending_screen_move_prims
 * (selected by buffer and arg1) at the far end (+0x3ffc) of the ending OT.
 */

void kaze_ending_screen_move_prim_add(int arg0, int arg1)
{
    AddPrim((void *)(kaze_ending_ot_base_ptrs[arg0 * 5] + 0x3ffc), arg1 * 0x18 + (kaze_ending_screen_move_prims + arg0 * 0x30));
}
