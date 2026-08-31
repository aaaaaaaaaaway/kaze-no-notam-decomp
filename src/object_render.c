#include "types.h"
#include "kaze_syms.h"
extern int ScaleMatrix(int *, int *);
extern int kaze_draw_model(int, int, int, int, int, int, int, int);
extern int kaze_draw_model_flatlit_3dd2c(int, int, int, int, int);
extern int kaze_prim_buffer_append(int, int *, int);
extern int kaze_tmd_sort_const_color_opaque_8a38(int, int, int, int, int, int, int, int);
typedef struct 
{
  s16 vx;
  s16 vy;
  s16 vz;
  s16 pad;
} SVEC;

extern void AddPrim(void *, void *);
long kaze_gte_project_vertex();
extern s32 rcos(s32);
extern s32 rsin(s32);
extern char *kaze_state_base_ptr;
extern s32 kaze_burner_state;

/**
 * kaze_balloon_overlay_builder  (render, 0x8002bbbc)
 *
 * Per-frame balloon/wind overlay quad builder: computes corners from camera
 * yaw (+0x4a) and burner state, projects them via kaze_gte_project_vertex
 * into the +0x488d4 POLY_FT4, and AddPrims it.
 */

void kaze_balloon_overlay_builder(int ot)
{
  int y_off;
  int sum;
  s32 first_projection;
  int new_var;
  int *pburner = &kaze_burner_state;
  SVEC v[4];
  {
    int trig = rsin(0x1400 - (*((s16 *) (kaze_state_base_ptr + 0x4a))));
    int j3 = (*pburner) * (-0x19);
    u32 u2;
    if (j3 < 0)
    {
      j3 = j3 + 0xfff;
    }
    u2 = trig * (j3 >> 0xc);
    if (((int) u2) < 0)
    {
      u2 = u2 + 0xfff;
    }
    v[2].vx = (u16) (u2 >> 0xc);
    v[0].vx = v[2].vx;
  }
  {
    int trig = rcos(0x1400 - (*((s16 *) (kaze_state_base_ptr + 0x4a))));
    int j3 = (*pburner) * (-0x19);
    u32 u2;
    if (j3 < 0)
    {
      j3 = j3 + 0xfff;
    }
    u2 = trig * (j3 >> 0xc);
    if (((int) u2) < 0)
    {
      u2 = u2 + 0xfff;
    }
    v[2].vz = (u16) (u2 >> 0xc);
    v[0].vz = v[2].vz;
  }
  {
    int trig = rsin(0x1400 - (*((s16 *) (kaze_state_base_ptr + 0x4a))));
    int j3 = (*pburner) * 0x19;
    u32 u2;
    if (j3 < 0)
    {
      j3 = j3 + 0xfff;
    }
    u2 = trig * (j3 >> 0xc);
    if (((int) u2) < 0)
    {
      u2 = u2 + 0xfff;
    }
    v[3].vx = (u16) (u2 >> 0xc);
    v[1].vx = v[3].vx;
  }
  {
    int trig = rcos(0x1400 - (*((s16 *) (kaze_state_base_ptr + 0x4a))));
    int burner = *pburner;
    int raw_j3 = burner * 25;
    int j3 = raw_j3;
    u32 u2;
    u16 fourth_z;
    if (raw_j3 < 0)
    {
      j3 = raw_j3 + 0xfff;
    }
    u2 = trig * (j3 >> 0xc);
    if (((int) u2) < 0)
    {
      u2 = u2 + 0xfff;
    }
    fourth_z = (u16) (u2 >> 0xc);
    v[3].vz = fourth_z;
    v[1].vz = fourth_z;
    { register int remaining asm("$3") = 0x1000 - burner;
      register int quarter_raw asm("$2") = remaining * 25;
      register int scoped_raw_y asm("$3") = quarter_raw * 4;
      y_off = scoped_raw_y;
    if (scoped_raw_y < 0)
    {
      y_off = scoped_raw_y + 0xfff;
    }
    }
    { register int block_base asm("$4") = y_off >> 0xc;
    y_off = block_base;
    __asm__("" : "=r"(y_off) : "0"(y_off));
    v[1].vy = y_off;
    v[0].vy = y_off;
    y_off = raw_j3 * 4;
    if (y_off < 0)
    {
      y_off = y_off + 0xfff;
    }
    y_off = block_base + (y_off >> 0xc);
    v[3].vy = y_off;
    v[2].vy = v[3].vy;
    }
  }
  {
    int c;
    int second_result;
    c = 0x488d4;
    first_projection = kaze_gte_project_vertex((kaze_state_base_ptr + (((*((int *) (kaze_state_base_ptr + 0x98))) * 0x28) + c)) + 8, &v[0]);
    sum = first_projection;
    new_var = ((*((int *) (kaze_state_base_ptr + 0x98))) * 0x28) + c;
    second_result = kaze_gte_project_vertex((kaze_state_base_ptr + new_var) + 0x20, &v[3]);
    { void *third_vertex = &v[1];
    __asm__ volatile("" ::: "memory");
    sum = sum + second_result;
    sum = sum + (((u32) sum) >> 0x1f);
    { register int idx asm("$6") = *((int *) (kaze_state_base_ptr + 0x98));
      register int third_off asm("$3") = (idx * 0x28) + c;
      kaze_gte_project_vertex((kaze_state_base_ptr + third_off) + 0x10, third_vertex); }}
    kaze_gte_project_vertex((kaze_state_base_ptr + (((*((int *) (kaze_state_base_ptr + 0x98))) * 0x28) + c)) + 0x18, &v[2]);
    AddPrim((void *) (ot + ((sum >> 3) * 4)), (void *) (kaze_state_base_ptr + (((*((int *) (kaze_state_base_ptr + 0x98))) * 0x28) + c)));
  }
  return;
}

#include "gte.h"

/**
 * kaze_gte_project_vertex  (library, 0x8002bec8)
 *
 * GTE helper: loads one SVECTOR, runs rotate/translate/perspective, stores
 * packed screen XY into the caller's primitive slot, returns depth.
 */

long kaze_gte_project_vertex(long *sxy, short *v) {
    long *scratch = (long *)0x1F800200;
    __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(v));
    __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(v));
    gte_nop();
    gte_nop();
    gte_rtps();
    __asm__ volatile("swc2 $14, 0(%0)" :: "r"(sxy) : "memory");
    gte_stsz_div4(scratch);
    return *scratch;
}

extern int kaze_stage_mode;
extern int PushMatrix(void);
extern int PopMatrix(void);
extern int RotMatrix(int *, int *);
#define gte_SetRotMatrix(r0)                                                   \
    __asm__ volatile("lw $12, 0(%0)\n\t"                                       \
                     "lw $13, 4(%0)\n\t"                                       \
                     "ctc2 $12, $0\n\t"                                        \
                     "ctc2 $13, $1\n\t"                                        \
                     "lw $12, 8(%0)\n\t"                                       \
                     "lw $13, 12(%0)\n\t"                                      \
                     "lw $14, 16(%0)\n\t"                                      \
                     "ctc2 $12, $2\n\t"                                        \
                     "ctc2 $13, $3\n\t"                                        \
                     "ctc2 $14, $4"                                            \
                     :: "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0)                                                 \
    __asm__ volatile("lw $12, 20(%0)\n\t"                                      \
                     "lw $13, 24(%0)\n\t"                                      \
                     "ctc2 $12, $5\n\t"                                        \
                     "lw $14, 28(%0)\n\t"                                      \
                     "ctc2 $13, $6\n\t"                                        \
                     "ctc2 $14, $7"                                            \
                     :: "r"(r0) : "$12", "$13", "$14")
#define gte_SetLightMatrix(r0)                                                 \
    __asm__ volatile("lw $12, 0(%0)\n\t"                                       \
                     "lw $13, 4(%0)\n\t"                                       \
                     "ctc2 $12, $8\n\t"                                        \
                     "ctc2 $13, $9\n\t"                                        \
                     "lw $12, 8(%0)\n\t"                                       \
                     "lw $13, 12(%0)\n\t"                                      \
                     "lw $14, 16(%0)\n\t"                                      \
                     "ctc2 $12, $10\n\t"                                       \
                     "ctc2 $13, $11\n\t"                                       \
                     "ctc2 $14, $12"                                           \
                     :: "r"(r0) : "$12", "$13", "$14")
#define gte_ldclmv(p)                                                          \
    __asm__ volatile("lhu $12, 0(%0)\n\t"                                      \
                     "lhu $13, 6(%0)\n\t"                                      \
                     "lhu $14, 12(%0)\n\t"                                     \
                     "mtc2 $12, $9\n\t"                                        \
                     "mtc2 $13, $10\n\t"                                       \
                     "mtc2 $14, $11"                                           \
                     :: "r"(p) : "$12", "$13", "$14")
#define gte_rtir()                                                             \
    __asm__ volatile("nop\n\t"                                                 \
                     "nop\n\t"                                                 \
                     "cop2 0x49E012")
#define gte_stclmv(p)                                                          \
    __asm__ volatile("mfc2 $12, $9\n\t"                                        \
                     "mfc2 $13, $10\n\t"                                       \
                     "mfc2 $14, $11\n\t"                                       \
                     "sh $12, 0(%0)\n\t"                                       \
                     "sh $13, 6(%0)\n\t"                                       \
                     "sh $14, 12(%0)"                                          \
                     :: "r"(p) : "$12", "$13", "$14", "memory")
#define gte_ldv0(p)                                                            \
    __asm__ volatile("lwc2 $0, 0(%0)\n\t"                                      \
                     "lwc2 $1, 4(%0)"                                          \
                     :: "r"(p))
#define gte_rt()                                                               \
    __asm__ volatile("nop\n\t"                                                 \
                     "nop\n\t"                                                 \
                     "cop2 0x480012")
#define gte_stlvnl(p)                                                          \
    __asm__ volatile("swc2 $25, 0(%0)\n\t"                                     \
                     "swc2 $26, 4(%0)\n\t"                                     \
                     "swc2 $27, 8(%0)"                                         \
                     :: "r"(p) : "memory")
#define gte_rtps()                                                             \
    __asm__ volatile("nop\n\t"                                                 \
                     "nop\n\t"                                                 \
                     "cop2 0x180001")
#define gte_stsxy(p)                                                           \
    __asm__ volatile("swc2 $14, 0(%0)" :: "r"(p) : "memory")
#define gte_stszotz(p)                                                         \
    __asm__ volatile("mfc2 $12, $19\n\t"                                       \
                     "nop\n\t"                                                 \
                     "sra $12, $12, 2\n\t"                                     \
                     "sw $12, 0(%0)"                                           \
                     :: "r"(p) : "$12", "memory")

/**
 * kaze_mode_object_render  (render, 0x8002bf04)
 *
 * Renders the six +0x58e0c mode-object slots and projects the balloon
 * LINE_F2 endpoints (+0x489ac/+0x489b0) from submodel pointer groups.
 */

void kaze_mode_object_render(int ot)
{
    int *m;
    int *r0;
    int *r0_00;
    int *r0_01;
    int *p;
    int i;
    int a;
    u32 sum;
    int c;
    register int b asm("$2");
    register int off1 asm("$5");
    register int off2 asm("$6");

    m = (int *)0x1f800000;
    if ((u32)(kaze_stage_mode - 1) < 2) {
        *(int **)((char *)m + 0x90) = (int *)(kaze_state_base_ptr + 0x58e0c);
        r0_01 = (int *)0x1f800040;
        r0_00 = (int *)0x1f800002;
        r0 = (int *)0x1f800004;
        *(int *)((char *)m + 0xa4) = 0;
        do {
            if ((*(int **)((char *)m + 0x90))[1] != 0) {
                PushMatrix();
                RotMatrix((int *)(*(int *)((char *)m + 0x90) + 0x30), m);
                gte_SetRotMatrix(kaze_state_base_ptr + 0x481b8);
                gte_ldclmv(m);
                gte_rtir();
                gte_stclmv(r0_01);
                gte_ldclmv(r0_00);
                gte_rtir();
                gte_stclmv((char *)m + 0x42);
                gte_ldclmv(r0);
                gte_rtir();
                gte_stclmv((char *)m + 0x44);
                gte_SetRotMatrix(kaze_state_base_ptr + 0x28);
                gte_ldclmv(m);
                gte_rtir();
                gte_stclmv(m);
                gte_ldclmv(r0_00);
                gte_rtir();
                gte_stclmv(r0_00);
                gte_ldclmv(r0);
                { int _mt = r0; __asm__("" :: "r"(_mt) : "memory"); }
                gte_rtir();
                gte_stclmv(r0);
                PopMatrix();
                gte_ldv0((int *)(*(int *)((char *)m + 0x90) + 0x28));
                gte_rt();
                gte_stlvnl((char *)m + 0x14);
                PushMatrix();
                ScaleMatrix(m, (int *)(*(int *)((char *)m + 0x90) + 0x18));
                gte_SetRotMatrix(m);
                gte_SetTransMatrix(m);
                gte_SetLightMatrix(r0_01);
                p = *(int **)((char *)m + 0x90);
                if (p[0x13] & 0x10) {
                    *(int *)(kaze_state_base_ptr + 0x9c) = kaze_tmd_sort_const_color_opaque_8a38(
                        ((int *)(kaze_state_base_ptr + 0x589e8))[p[0]], p[0x12], ot,
                        *(int *)(kaze_state_base_ptr + 0x9c), 2, 0xff, 0, 0);
                    p = *(int **)((char *)m + 0x90);
                    p[0x13] = p[0x13] & ~0x10;
                }
                {
                    register int *pf asm("$2");
                    pf = *(int **)((char *)m + 0x90);
                    *(int *)(kaze_state_base_ptr + 0x9c) = kaze_draw_model_flatlit_3dd2c(
                        ((int *)(kaze_state_base_ptr + 0x589e8))[pf[0]], pf[0x12], ot,
                        *(int *)(kaze_state_base_ptr + 0x9c), 2);
                }
                if (kaze_stage_mode == 2) {
                    *(int *)(kaze_state_base_ptr + 0x9c) = kaze_draw_model_flatlit_3dd2c(
                        *(int *)(kaze_state_base_ptr + 0x58a00), 0, ot,
                        *(int *)(kaze_state_base_ptr + 0x9c), kaze_stage_mode);
                    *(int *)((char *)m + 0xa8) = 0;
                    do {
                        register int *sp2 asm("$8");
                        int va;
                        int vb;
                        int *sA;
                        int *sB;
                        off1 = 0x58a04;
                        off2 = 0x489ac;
                        {
                            register int *pp asm("$2");
                            pp = *(int **)((char *)m + 0x90);
                            va = kaze_state_base_ptr + pp[0] * 16 + off1;
                        }
                        {
                            register int ii asm("$2");
                            int *vp;
                            ii = *(int *)((char *)m + 0xa8);
                            vp = *(int **)(va + ii * 4);
                            sA = (int *)(kaze_state_base_ptr + off2);
                            gte_ldv0(vp);
                        }
                        gte_rtps();
                        gte_stsxy(sA);
                        sp2 = (int *)0x1f800200;
                        __asm__("" : "=r"(sp2) : "0"(sp2));
                        gte_stszotz(sp2);
                        i = *(int *)((char *)m + 0xa8);
                        vb = i * 4 + (int)kaze_state_base_ptr;
                        sB = (int *)(kaze_state_base_ptr + 0x489b0);
                        { register int ab asm("$3"); ab = 0x1f800000;
                          a = *(volatile int *)(ab + 0x200); }
                        gte_ldv0(*(int **)(vb + 0x58a64));
                        gte_rtps();
                        gte_stsxy(sB);
                        gte_stszotz(sp2);
                        b = 0x1f800000;
                        b = *(volatile int *)(b + 0x200);
                        sum = a + b;
                        c = sum + (sum >> 31);
                        *(int *)((char *)m + 0x94) = c >> 1;
                        kaze_prim_buffer_append(ot + (c >> 3) * 4,
                                                (int *)(kaze_state_base_ptr + 0x489a4), 3);
                        i = *(int *)((char *)m + 0xa8) + 1;
                        *(int *)((char *)m + 0xa8) = i;
                    } while (i < 4);
                }
                PopMatrix();
            }
            *(int *)((char *)m + 0xa4) = *(int *)((char *)m + 0xa4) + 1;
            *(int *)((char *)m + 0x90) = *(int *)((char *)m + 0x90) + 0x58;
        } while (*(int *)((char *)m + 0xa4) < 6);
    }
}

/**
 * kaze_marker_render  (render, 0x8002c4e8)
 *
 * Renders the 10 mode-0 marker slots with per-marker matrices and colors
 * using model asset slot 2 (+0x589f0).
 */

void kaze_marker_render(int ot)
{
    int *p;
    int *m;
    int *r0;
    int *r0_00;
    int *r0_01;
    int slot_g;
    int slot_b;
    int color_wrap;
    int red;
    int green;
    int blue;
    int model_asset;

    m = (int *)0x1f800000;
    *(int **)0x1f80008c = (int *)(kaze_state_base_ptr + 0x58a74);
    *(int *)0x1f8000a4 = 0;
    color_wrap = 0x200;
    r0_01 = (int *)0x1f800040;
    r0_00 = (int *)0x1f800002;
    r0 = (int *)0x1f800004;
    do {
        p = *(int **)((char *)m + 0x8c);
        if ((*p != 0) && (p[1] != 0)) {
            *(int *)((char *)m + 0x94) = p[0x13];
            slot_g = *(int *)((char *)m + 0x8c);
            *(int *)((char *)m + 0x98) = *(int *)(slot_g + 0x50);
            slot_b = *(int *)((char *)m + 0x8c);
            *(int *)((char *)m + 0x9c) = *(int *)(slot_b + 0x54);
            {
                int cc0 = *(int *)((char *)m + 0x94);
                if (cc0 > 0xff) *(int *)((char *)m + 0x94) = color_wrap - cc0;
            }
            {
                int cc1 = *(int *)((char *)m + 0x98);
                if (cc1 > 0xff) *(int *)((char *)m + 0x98) = color_wrap - cc1;
            }
            {
                int cc2 = *(int *)((char *)m + 0x9c);
                if (cc2 > 0xff) *(int *)((char *)m + 0x9c) = color_wrap - cc2;
            }
            PushMatrix();
            RotMatrix((int *)(*(int *)((char *)m + 0x8c) + 0x28), m);
            gte_SetRotMatrix(kaze_state_base_ptr + 0x481b8);
            gte_ldclmv(m);
            gte_rtir();
            gte_stclmv(r0_01);
            gte_ldclmv(r0_00);
            gte_rtir();
            gte_stclmv((char *)m + 0x42);
            gte_ldclmv(r0);
            gte_rtir();
            gte_stclmv((char *)m + 0x44);
            gte_SetRotMatrix(kaze_state_base_ptr + 0x28);
            gte_ldclmv(m);
            gte_rtir();
            gte_stclmv(m);
            gte_ldclmv(r0_00);
            gte_rtir();
            gte_stclmv(r0_00);
            gte_ldclmv(r0);
            gte_rtir();
            gte_stclmv(r0);
            PopMatrix();
            gte_ldv0((int *)(*(int *)((char *)m + 0x8c) + 0x18));
            gte_rt();
            gte_stlvnl((char *)m + 0x14);
            PushMatrix();
            gte_SetRotMatrix(m);
            gte_SetTransMatrix(m);
            gte_SetLightMatrix(r0_01);
            model_asset = *(int *)(kaze_state_base_ptr + 0x589f0);
            red = *(int *)((char *)m + 0x94);
            green = *(int *)((char *)m + 0x98);
            blue = *(int *)((char *)m + 0x9c);
            *(int *)(kaze_state_base_ptr + 0x9c) =
                kaze_draw_model(model_asset, 0, ot, *(int *)(kaze_state_base_ptr + 0x9c),
                                2, red, green, blue);
            PopMatrix();
        }
        *(int *)((char *)m + 0xa4) = *(int *)((char *)m + 0xa4) + 1;
        *(int *)((char *)m + 0x8c) = *(int *)((char *)m + 0x8c) + 0x5c;
    } while (*(int *)((char *)m + 0xa4) < 10);
}

extern int kaze_model_set_translation_component_348b0(int, int, int);

/**
 * kaze_dynobj_render  (render, 0x8002c924)
 *
 * Renders active +0x5901c dynamic objects, resolving model pointers from the
 * +0xecb28 table (one-based +0xecb24 alias form for remapped negative ids).
 */

void kaze_dynobj_render(int ot)
{
    int *p;
    int i;
    int *m;
    int *r0;
    int *r00;
    int *r01;
    int **pp;
    int t;
    int a;
    int b;

    t = 0x5901c;
    m = (int *)0x1f800004;
    r01 = (int *)0x1f800024;
    r00 = (int *)0x1f800006;
    r0 = (int *)0x1f800008;
    pp = (int **)0x1f800000;
    *(int *)0x1f800044 = 0;
    *pp = (int *)(kaze_state_base_ptr + t);
    do {
        p = *pp;
        if ((*p != 0) && (p[3] != 0) && (p[2] >= 0)) {
            i = *(s16 *)((char *)p + 0x42);
            if (i < 0) {
                a = (int)kaze_state_base_ptr + i * -0x80 + 0xe1e60;
                b = *(int *)(a + *(int *)(kaze_state_base_ptr + 0xecaf8) * 4) * 4;
                p[1] = *(int *)((int)kaze_state_base_ptr + b + 0xecb24);
            } else {
                p[1] = *(int *)(i * 4 + (int)kaze_state_base_ptr + 0xecb28);
            }
            PushMatrix();
            RotMatrix(*pp + 10, m);
            gte_SetRotMatrix(kaze_state_base_ptr + 0x481b8);
            gte_ldclmv(m);
            gte_rtir();
            gte_stclmv(r01);
            gte_ldclmv(r00);
            gte_rtir();
            gte_stclmv((char *)pp + 0x26);
            gte_ldclmv(r0);
            gte_rtir();
            gte_stclmv(pp + 10);
            gte_SetRotMatrix(kaze_state_base_ptr + 0x28);
            gte_ldclmv(m);
            gte_rtir();
            gte_stclmv(m);
            gte_ldclmv(r00);
            gte_rtir();
            gte_stclmv(r00);
            gte_ldclmv(r0);
            gte_rtir();
            gte_stclmv(r0);
            PopMatrix();
            gte_ldv0(*pp + 8);
            gte_rt();
            gte_stlvnl(pp + 6);
            PushMatrix();
            ScaleMatrix(m, *pp + 0xc);
            gte_SetRotMatrix(m);
            gte_SetTransMatrix(m);
            gte_SetLightMatrix(r01);
            p = *pp;
            if (p[2] != 0) {
                kaze_model_get_field_348ec(p[1], 0, p + 0x10);
                kaze_model_set_translation_component_348b0(
                    (*pp)[1], 0, *(u16 *)(kaze_state_base_ptr + 0x5b6dc));
                *(int *)(kaze_state_base_ptr + 0x9c) =
                    kaze_draw_model((*pp)[1], (*pp)[2], ot,
                                    *(int *)(kaze_state_base_ptr + 0x9c), 2,
                                    (*pp)[0x11], (*pp)[0x11], (*pp)[0x11]);
                kaze_model_set_translation_component_348b0(
                    (*pp)[1], 0, *(u16 *)((char *)*pp + 0x40));
            } else {
                *(int *)(kaze_state_base_ptr + 0x9c) =
                    kaze_draw_model_flatlit_3dd2c(p[1], p[2], ot,
                                                  *(int *)(kaze_state_base_ptr + 0x9c), 2);
            }
            PopMatrix();
        }
        *(int *)((char *)pp + 0x44) = *(int *)((char *)pp + 0x44) + 1;
        *pp = *pp + 0x1e;
    } while (*(int *)((char *)pp + 0x44) < 0x12);
}
