#include "types.h"
#include "kaze_syms.h"
extern int kaze_menu_camera_trans_x;
extern int kaze_menu_camera_trans_y;
extern int kaze_menu_camera_trans_z;
extern int kaze_menu_camera_coord_word;
extern int kaze_menu_model1_coord_ptr;
extern int kaze_menu_model2_coord_ptr;
extern int RotTransPers(int, int, int, int);
extern int kaze_menu_camera_pos_z;
extern int kaze_menu_camera_scroll;
typedef struct { int w[4]; } Line16;
struct ot {
    int unk0;
    int unk4;
};

extern char kaze_menu_camera_matrix[];

/**
 * kaze_title_model_sort_primary  (render, 0x80049aac)
 *
 * RotMatrix/GsGetLws/GsSortObject4 sorts model kaze_menu_model1_gsobj using
 * rotation kaze_menu_camera_rot_x into the OT.
 */

void kaze_title_model_sort_primary(void *ot)
{
    extern void GsSortObject4(void *, void *, int, void *);
    extern void GsSetLsMatrix(void *);
    extern void GsSetLightMatrix(void *);
    extern void GsGetLws(int, void *, void *);
    extern void kaze_trans_matrix(void *, void *);
    extern void RotMatrix(void *, void *);
    extern char kaze_menu_camera_pos_x[];
    extern char kaze_menu_camera_rot_x[];
    char *m;
    int *p;

    m = kaze_menu_camera_matrix;
    RotMatrix(kaze_menu_camera_rot_x, m);
    kaze_trans_matrix(m, kaze_menu_camera_pos_x);
    p = &kaze_menu_model1_coord_ptr;
    GsGetLws(*p, (void *)0x1F800000, (void *)0x1F800020);
    GsSetLightMatrix((void *)0x1F800000);
    GsSetLsMatrix((void *)0x1F800020);
    GsSortObject4((void *)(p - 1), ot, 2, (void *)0x1F800000);
}

extern u16 kaze_menu_camera_rot_y;
extern u16 kaze_menu_camera_rot_z;
extern u16 kaze_menu_render_rot_x;
extern u16 kaze_menu_render_rot_y;
extern u16 kaze_menu_render_rot_z;
extern int kaze_menu_camera_coord_flags;

/**
 * kaze_title_model_sort_shadow  (render, 0x80049b44)
 *
 * Sorts the model-2 GsDOBJ2 structure (paired with kaze_menu_model1_gsobj,
 * just before kaze_menu_model2_coord_ptr) with a +0x180 Y offset into the
 * OT; light matrices from GsGetLws(kaze_menu_model2_coord_ptr).
 */

void kaze_title_model_sort_shadow(int ot)
{
    extern void GsSortObject4(int *, int, int, int);
    extern void GsSetLsMatrix(int);
    extern void GsSetLightMatrix(int);
    extern void GsGetLws(int, int, int);
    extern void kaze_trans_matrix(int *, int *);
    extern void RotMatrix(u16 *, int *);
    extern int kaze_menu_camera_pos_x;
    extern u16 kaze_menu_camera_rot_x;
    u16 *rot;
    int *coord;
    int *m;
    int *pos;
    int *obj;

    rot = &kaze_menu_render_rot_x;
    coord = &kaze_menu_camera_coord_word;
    m = coord + 1;
    *rot = kaze_menu_camera_rot_x;
    kaze_menu_render_rot_y = kaze_menu_camera_rot_y;
    kaze_menu_render_rot_z = kaze_menu_camera_rot_z;
    pos = &kaze_menu_camera_trans_x;
    *pos = kaze_menu_camera_pos_x;
    kaze_menu_camera_trans_y = kaze_menu_camera_scroll + 0x180;
    kaze_menu_camera_trans_z = kaze_menu_camera_pos_z;
    *coord = kaze_menu_camera_coord_flags;
    RotMatrix(rot, m);
    kaze_trans_matrix(m, pos);
    obj = &kaze_menu_model2_coord_ptr;
    GsGetLws(*obj, 0x1F800000, 0x1F800020);
    GsSetLightMatrix(0x1F800000);
    GsSetLsMatrix(0x1F800020);
    GsSortObject4(obj - 1, ot, 2, 0x1F800000);
}

extern int AddPrim(int, int);
extern int kaze_title_shadow_line_verts_a;
extern int kaze_title_shadow_line_verts_b;
extern Line16 kaze_title_shadow_line_prims[][4];

/**
 * kaze_title_shadow_line_project  (render, 0x80049c54)
 *
 * RotTransPers projects the shadow-line endpoints
 * (kaze_title_shadow_line_verts_a/15f0) and AddPrims into the OT bucket by
 * averaged Z.
 */

void kaze_title_shadow_line_project(int arg0, struct ot *arg1)
{
    extern void GsSetLsMatrix(int);
    extern void GsSetLightMatrix(int);
    extern void GsGetLws(int, int, int);
    register int *scratch asm("$16");
    int temp_s1;
    register int temp_s3 asm("$19");
    register int m1 asm("$20");
    int pp;
    int ff;
    register int coord asm("$23");
    int var_s2;
    int var_v0;
    int var_v0_2;
    int temp_v0;
    int temp_v1;

    scratch = (int *)0x1F800000;
    var_s2 = 0;
    coord = (int)&kaze_menu_camera_coord_flags;
    m1 = 0x1F800020;
    do {
        GsGetLws(coord, (int)scratch, m1);
        GsSetLightMatrix((int)scratch);
        GsSetLsMatrix(m1);
        temp_s3 = var_s2 * 8;
        temp_s1 = (int)&kaze_title_shadow_line_prims[arg0][var_s2];
        asm("" : "=r"(pp) : "0"(0x1F80004C));
        asm("" : "=r"(ff) : "0"(0x1F800050));
        var_v0 = RotTransPers(temp_s3 + (int)&kaze_title_shadow_line_verts_a, temp_s1 + 8, pp, ff);
        scratch[0x11] = var_v0;
        if (var_v0 < 0) {
            var_v0 += 3;
        }
        scratch[0x11] = var_v0 >> 2;
        GsGetLws(coord + 0x50, (int)scratch, m1);
        GsSetLightMatrix((int)scratch);
        GsSetLsMatrix(m1);
        var_v0_2 = RotTransPers(temp_s3 + (int)&kaze_title_shadow_line_verts_b, temp_s1 + 0xC, pp, ff);
        scratch[0x12] = var_v0_2;
        if (var_v0_2 < 0) {
            var_v0_2 += 3;
        }
        temp_v0 = var_v0_2 >> 2;
        scratch[0x12] = temp_v0;
        temp_v1 = (scratch[0x11] + temp_v0) / 2;
        scratch[0x10] = temp_v1;
        if ((u32)(temp_v1 - 1) < 0xFFF) {
            AddPrim(arg1->unk4 + (temp_v1 * 4), temp_s1);
        }
        var_s2 += 1;
    } while (var_s2 < 4);
}
