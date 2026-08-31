#include "types.h"
#include "kaze_syms.h"
extern int kaze_title_vertex_remap_table_a[];
extern int kaze_title_vertex_remap_table_b[];
typedef struct {
    int a, b, c;
} Triple;
typedef struct {
    unsigned int tag;
    u8 r0, g0, b0, code;
    short x0, y0, x1, y1;
} LINE_F2;
typedef struct {
    u16 vx, vy, vz, pad;
} SVECTOR;

extern Triple kaze_model_vertex_ptr_table[];

/**
 * kaze_model_packet_triple_table_build_9828  (render, 0x80049828)
 *
 * For the same 264 model records, derives three base-relative pointers per
 * record (offsets at rec+0xa/+0xe/+0x12 scaled by 8) into the 12-byte-stride
 * table at kaze_model_vertex_ptr_table/2c/30.
 */

void kaze_model_packet_triple_table_build_9828(int *arg)
{
    int i;
    int j;
    int base;
    u8 *p;

    i = 0;
    j = 0;
    base = arg[0];
    p = (u8 *)arg[4];
    do {
        kaze_model_vertex_ptr_table[j].a = base + *(short *)(p + 10) * 8;
        kaze_model_vertex_ptr_table[j].b = base + *(u8 *)(p + 0xE) * 8;
        i++;
        kaze_model_vertex_ptr_table[j].c = base + *(u8 *)(p + 0x12) * 8;
        p += *(u8 *)(p + 1) * 4 + 4;
        j++;
    } while (i < 0x108);
}

extern void SetLineF2(LINE_F2 *);
extern void SetShadeTex(LINE_F2 *, int);
extern LINE_F2 kaze_title_shadow_line_prims[];
extern SVECTOR **kaze_menu_model1_data_ptr;
extern SVECTOR **kaze_menu_model2_data_ptr;
extern SVECTOR kaze_title_shadow_line_verts_a[4];
extern SVECTOR kaze_title_shadow_line_verts_b[4];

/**
 * kaze_title_shadow_line_setup  (render, 0x800498b8)
 *
 * Builds 8 LINE_F2 shadow-line primitives at
 * kaze_title_shadow_line_prims/3408, copying vertex coords from
 * kaze_title_vertex_remap_table_a/dc04 tables via
 * kaze_menu_model1_data_ptr/f78.
 */

void kaze_title_shadow_line_setup(void)
{
    int i;
    for (i = 0; i < 4; i++) {
        LINE_F2 *p = &kaze_title_shadow_line_prims[i];
        LINE_F2 *q = &kaze_title_shadow_line_prims[i + 4];
        SVECTOR *base;
        int *a1;
        SetLineF2(p);
        SetShadeTex(p, 1);
        p->r0 = 0x40;
        p->g0 = 0x40;
        p->b0 = 0x20;
        SetLineF2(q);
        SetShadeTex(q, 1);
        q->r0 = 0x40;
        q->g0 = 0x40;
        q->b0 = 0x20;
        base = *kaze_menu_model1_data_ptr;
        kaze_title_shadow_line_verts_a[i].vx = base[kaze_title_vertex_remap_table_a[i]].vx;
        kaze_title_shadow_line_verts_a[i].vy = base[kaze_title_vertex_remap_table_a[i]].vy;
        kaze_title_shadow_line_verts_a[i].vz = base[kaze_title_vertex_remap_table_a[i]].vz;
        do { a1 = &kaze_title_vertex_remap_table_b[i]; } while (0);
        base = *kaze_menu_model2_data_ptr;
        kaze_title_shadow_line_verts_b[i].vx = base[*a1].vx;
        kaze_title_shadow_line_verts_b[i].vy = base[*a1].vy;
        kaze_title_shadow_line_verts_b[i].vz = base[*a1].vz;
    }
}

extern void kaze_title_model_sort_primary(int);

/**
 * kaze_title_model_sort_dispatch  (render, 0x80049a60)
 *
 * Dispatches kaze_title_model_sort_primary/80049b44/80049c54 to sort title
 * models and shadow lines into GsOT arg1.
 */

void kaze_title_model_sort_dispatch(int arg0, int arg1)
{
    kaze_title_model_sort_primary(arg1);
    kaze_title_model_sort_shadow(arg1);
    kaze_title_shadow_line_project(arg0, arg1);
}
