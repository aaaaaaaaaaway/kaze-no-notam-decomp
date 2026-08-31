#include "types.h"
#include "kaze_syms.h"
extern int kaze_mode_state;
extern int ScaleMatrix(int *, int *);
extern int kaze_balloon_scale_x;
extern int kaze_balloon_render_rot0;
extern int kaze_pos_y;
extern int kaze_draw_model_flatlit_3dd2c(int, int, int, int, int);
extern int kaze_gte_project_vertex(void *, int, int);
struct Marker {
    char pad[0x49C2C];
    u8 c2c;
    u8 c2d;
};
struct MarkerS {
    char pad[0x49C18];
    s16 s18;
    s16 s1a;
};

extern char *kaze_state_base_ptr;
extern int kaze_stage_variant;
extern void AddPrim(void *, void *);
void kaze_minimap_marker_renderer();

/**
 * kaze_postevent_prim_dispatch  (render, 0x8002a78c)
 *
 * Post-event primitive dispatcher gated by +0x14 flags; AddPrims the post-
 * event SPRT/POLY groups and delegates to the minimap renderer when the live
 * stage mode is 2.
 */

void kaze_postevent_prim_dispatch(void *ot)
{
    extern unsigned int kaze_stage_mode;
    int temp_v1;
    int var_s1;
    int var_s2;
    char *var_s4;
    int pad;
    int k48;

    temp_v1 = *(int *)(kaze_state_base_ptr + 0x98);
    var_s4 = kaze_state_base_ptr + (temp_v1 * 0x50 + 0x489ec);
    if ((*(uint *)(kaze_state_base_ptr + 0x14) & 0x2000000) == 0) {
        if (*(int *)(kaze_state_base_ptr + 0xecafc) != 0) {
            AddPrim(ot, kaze_state_base_ptr + (temp_v1 * 0x14 + 0x489b4));
            AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x489dc));
        }
        if ((*(uint *)(kaze_state_base_ptr + 0x14) & 0x100) != 0) {
            if ((kaze_stage_mode < 2) || (kaze_stage_variant == 6)) {
                if (0 < kaze_mode_state) {
                    var_s1 = 0;
                    do {
                        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x30 + 0x48b4c) + var_s1 * 0x10);
                        var_s1 += 1;
                    } while (var_s1 < kaze_mode_state);
                }
                AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x48bac));
            }
            AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x14 + 0x48c1c));
            AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x48d64));
        }
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x90 + 0x48c44));
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x90 + 0x48c44) + 0x18);
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x48d74));
        var_s1 = 0;
        __asm__("lui %0,0x4" : "=r"(k48));
        __asm__("ori %0,%1,0x8a8c" : "=r"(k48) : "0"(k48));
        var_s2 = 0;
        do {
            __asm__("" :: "r"(var_s2));
            AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x50 + k48) + var_s2);
            var_s1 += 1;
            var_s2 += 0x14;
        } while (var_s1 < 4);
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x28 + 0x48bbc));
        var_s1 = 0;
        do {
            __asm__("" :: "r"(var_s4), "r"(var_s4), "r"(var_s4));
            AddPrim(ot, var_s4);
            var_s1 += 1;
            var_s4 += 0x10;
        } while (var_s1 < 5);
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x10 + 0x48b2c));
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x48c0c));
        if (kaze_stage_mode == 2) {
            kaze_minimap_marker_renderer(ot);
        }
        __asm__ volatile("" :: "m"(pad));
    }
}

extern int SquareRoot0(int);
extern int ratan2(int, int);
extern int rcos(int);
extern int rsin(int);
extern void kaze_hud_two_digit_counter_284c(void *, int, int, int, int);

/**
 * kaze_minimap_marker_renderer  (hud, 0x8002ab28)
 *
 * Minimap object-marker renderer: polar-projects each +0x58e14 object around
 * the balloon into SPRT_8 markers (+0x49c10..), draws frame lines, panel,
 * draw-areas, TPages, and label SPRTs.
 */

void kaze_minimap_marker_renderer(void *arg0)
{
    extern int kaze_stage_mode;
    extern int kaze_pos_x[];
    int sp18;
    int i;
    int i2c;
    int i10;
    int i9;
    int off2;
    struct MarkerS *mp;
    int mt;
    int dz;
    int dist;
    int ang;
    int ang2;
    int val;

    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x90 + 0x49AF0));
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x90 + 0x49AF0) + 0x18);
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x90 + 0x49AF0) + 0x30);
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x90 + 0x49AF0) + 0x48);
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x90 + 0x49AF0) + 0x60);
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x90 + 0x49AF0) + 0x78);
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x18 + 0x49D70) + 0xC);
    sp18 = 0x1000 - *(s16 *)(kaze_state_base_ptr + 0x4A);
    i = 0;
    do {
        i2c = i * 0x58;
        i9 = i * 0x10;
        i10 = i * 0x10 + 0x10;
        if ((*(u32 *)(kaze_state_base_ptr + i2c + 0x58E58) & 0x84000000) == 0) {
            ang = (*(int *)(kaze_state_base_ptr + i2c + 0x58E14) - kaze_pos_x[0]) / 0x76c;
            dz = (*(int *)(kaze_state_base_ptr + i2c + 0x58E1C) - kaze_pos_x[2]) / 0x76c;
            dist = SquareRoot0(dz * dz + ang * ang);
            ang = ratan2(dz, ang);
            ang = 0x1000 - ang;
            ang2 = sp18 + ang;
            {
                int val1;
                val1 = rcos(ang2) * dist;
                mt = i10 + *(int *)(kaze_state_base_ptr + 0x98) * 0xA0;
                mp = (struct MarkerS *)(kaze_state_base_ptr + mt);
                if (val1 < 0) {
                    val1 += 0xFFF;
                }
                mp->s18 = 0x25 - (s16)(val1 >> 0xC);
            }
            {
                int val2;
                register struct MarkerS *mp2 asm("$3");
                val2 = rsin(ang2) * dist;
                mt = i10 + *(int *)(kaze_state_base_ptr + 0x98) * 0xA0;
                mp2 = (struct MarkerS *)(kaze_state_base_ptr + mt);
                if (val2 < 0) {
                    val2 += 0xFFF;
                }
                mp2->s1a = (s16)(val2 >> 0xC) + 0xB9;
            }
            ang2 = -kaze_pos_x[1] / 0x76c;
            dz = -*(int *)(kaze_state_base_ptr + i2c + 0x58E18) / 0x76c;
            if (ang2 == dz) {
                ((struct Marker *)(*(int *)(kaze_state_base_ptr + 0x98) * 0xA0 + kaze_state_base_ptr + i9))->c2c = 0x40;
                ((struct Marker *)(*(int *)(kaze_state_base_ptr + 0x98) * 0xA0 + kaze_state_base_ptr + i9))->c2d = 0x30;
            } else if (dz < ang2) {
                ((struct Marker *)(*(int *)(kaze_state_base_ptr + 0x98) * 0xA0 + kaze_state_base_ptr + i9))->c2c = 0x40;
                ((struct Marker *)(*(int *)(kaze_state_base_ptr + 0x98) * 0xA0 + kaze_state_base_ptr + i9))->c2d = 0x38;
            } else {
                ((struct Marker *)(*(int *)(kaze_state_base_ptr + 0x98) * 0xA0 + kaze_state_base_ptr + i9))->c2c = 0x48;
                ((struct Marker *)(*(int *)(kaze_state_base_ptr + 0x98) * 0xA0 + kaze_state_base_ptr + i9))->c2d = 0x30;
            }
            mt = *(int *)(kaze_state_base_ptr + 0x98) * 0xA0 + 0x49C10;
            AddPrim(arg0, kaze_state_base_ptr + mt + i10);
        }
        i += 1;
    } while (i < 6);
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0xA0 + 0x49C10));
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x20 + 0x49AB0));
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x20 + 0x49AB0) + 0x10);
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x10 + 0x49D50));
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x18 + 0x49D70));
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x18 + 0x49A80));
    i = 0;
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x10 + 0x49D50) + 8);
    kaze_hud_two_digit_counter_284c(
        kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x78 + 0x49DA0), -0x10, 0x20, kaze_stage_mode,
        *(u32 *)(kaze_state_base_ptr + 0x48284) / 0x3C);
    do {
        off2 = i * 0x14;
        AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x78 + 0x49DA0) + off2);
        i += 1;
    } while (i < 6);
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x49E90));
}

extern int kaze_pos_z;
extern u16 kaze_balloon_sway_offset_y;
extern int kaze_burner_state;
extern volatile int D_800B178C_v __asm__("kaze_burner_state");
extern int PushMatrix(void);
extern int PopMatrix(void);
extern int RotMatrix(int *, int *);
extern int SetColorMatrix(int *);
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
#define gte_rt()                                                              \
    __asm__ volatile("nop\n\t"                                                 \
                     "nop\n\t"                                                 \
                     "cop2 0x480012")
#define gte_stlvnl(p)                                                          \
    __asm__ volatile("swc2 $25, 0(%0)\n\t"                                     \
                     "swc2 $26, 4(%0)\n\t"                                     \
                     "swc2 $27, 8(%0)"                                         \
                     :: "r"(p) : "memory")

/**
 * kaze_balloon_render  (render, 0x8002b1f0)
 *
 * Balloon body renderer: builds rotation/scale/light matrices and draws
 * model slots +0x589e8/+0x589ec with submodel vertex groups
 * +0x58a14/+0x58a24.
 */

void kaze_balloon_render(int ot)
{
    extern int kaze_pos_x;
    char *sp;
    char *sp2;
    int dx;
    int dy;
    int dz;
    int t;
    int n;
    int r;

    {
        char *base = kaze_state_base_ptr;
        __asm__ volatile("" :: "r"(base), "r"(ot));
        if (*(int *)(base + 0x14) & 0x100) {
            return;
        }
    }
    sp = (char *)0x1F800000;
    sp2 = (char *)0x1F800080;
    dx = *(int *)(kaze_state_base_ptr + 0x68) - kaze_pos_x;
    dy = kaze_pos_y - *(int *)(kaze_state_base_ptr + 0x6C);
    dz = kaze_pos_z - *(int *)(kaze_state_base_ptr + 0x70);
    if ((u32)(dx + 0x8000) <= 0xFFFF &&
        (u32)(dz + 0x8000) <= 0xFFFF &&
        (u32)(dy + 0x8000) <= 0xFFFF) {
        register int dx2 asm("$2");
        register int dz2 asm("$3");
        dx2 = dx * dx;
        dz2 = dz * dz;
        n = SquareRoot0(dx2 + dz2);
        if (n < 0x76C1) {
            *(s16 *)(sp2 + 0x00) = dx;
            *(s16 *)(sp2 + 0x02) = dy;
            *(s16 *)(sp2 + 0x04) = dz;
            r = n;
            goto test;
        }
    }
    r = -1;
test:
    if (r < 0) {
        return;
    }
    if (!(*(int *)(kaze_state_base_ptr + 0x14) & 0x1000)) {
        *(u16 *)(sp + 0x82) += kaze_balloon_sway_offset_y;
    }
    PushMatrix();
    *(s16 *)(kaze_state_base_ptr + 0x481C6) = 0x1000;
    RotMatrix(&kaze_balloon_render_rot0, (int *)sp);
    gte_SetRotMatrix(kaze_state_base_ptr + 0x481b8);
    gte_ldclmv(sp);
    gte_rtir();
    gte_stclmv(sp + 0x40);
    gte_ldclmv(sp + 0x02);
    gte_rtir();
    gte_stclmv(sp + 0x42);
    gte_ldclmv(sp + 0x04);
    gte_rtir();
    gte_stclmv(sp + 0x44);
    gte_SetRotMatrix(kaze_state_base_ptr + 0x28);
    gte_ldclmv(sp);
    gte_rtir();
    gte_stclmv(sp);
    gte_ldclmv(sp + 0x02);
    gte_rtir();
    gte_stclmv(sp + 0x02);
    gte_ldclmv(sp + 0x04);
    gte_rtir();
    gte_stclmv(sp + 0x04);
    *(s16 *)(kaze_state_base_ptr + 0x481C6) = -0x1000;
    RotMatrix((int *)((char *)&kaze_balloon_render_rot0 + 8), (int *)(sp + 0x20));
    gte_SetRotMatrix(kaze_state_base_ptr + 0x481b8);
    gte_ldclmv(sp + 0x20);
    gte_rtir();
    gte_stclmv(sp + 0x60);
    gte_ldclmv(sp + 0x22);
    gte_rtir();
    gte_stclmv(sp + 0x62);
    gte_ldclmv(sp + 0x24);
    gte_rtir();
    gte_stclmv(sp + 0x64);
    gte_SetRotMatrix(kaze_state_base_ptr + 0x28);
    gte_ldclmv(sp + 0x20);
    gte_rtir();
    gte_stclmv(sp + 0x20);
    gte_ldclmv(sp + 0x22);
    gte_rtir();
    gte_stclmv(sp + 0x22);
    gte_ldclmv(sp + 0x24);
    gte_rtir();
    gte_stclmv(sp + 0x24);
    PopMatrix();
    gte_ldv0(sp + 0x80);
    gte_rt();
    gte_stlvnl(sp + 0x14);
    gte_stlvnl(sp + 0x34);
    PushMatrix();
    if (kaze_burner_state != 0) {
        { register int tv asm("$2");
          tv = *(int *)(kaze_state_base_ptr + 0x481E8) * kaze_burner_state;
          if (tv < 0) tv += 0xFFF;
          *(s16 *)(kaze_state_base_ptr + 0x4819C) = tv >> 12; }
        { register int tv asm("$2");
          tv = *(int *)(kaze_state_base_ptr + 0x481EC) * D_800B178C_v;
          if (tv < 0) tv += 0xFFF;
          *(s16 *)(kaze_state_base_ptr + 0x481A2) = tv >> 12; }
        { register int tv asm("$2");
          tv = *(int *)(kaze_state_base_ptr + 0x481F0) * D_800B178C_v;
          if (tv < 0) tv += 0xFFF;
          *(s16 *)(kaze_state_base_ptr + 0x481A8) = tv >> 12; }
        SetColorMatrix((int *)(kaze_state_base_ptr + 0x48198));
    }
    ScaleMatrix((int *)sp, &kaze_balloon_scale_x);
    gte_SetRotMatrix(sp);
    gte_SetTransMatrix(sp);
    gte_SetLightMatrix(sp + 0x40);
    *(int *)(kaze_state_base_ptr + 0x9c) = kaze_draw_model_flatlit_3dd2c(
        *(int *)(kaze_state_base_ptr + 0x589e8), 0, ot,
        *(int *)(kaze_state_base_ptr + 0x9c), 2);
    *(int *)(sp + 0xA4) = 0;
    do {
        int idx;
        char *field;
        idx = *(int *)(sp + 0xA4);
        field = (char *)(idx * 4);
        field += (int)kaze_state_base_ptr;
        __asm__("" : : "r"(field));
        idx <<= 4;
        *(int *)(sp + *(int *)(sp + 0xA4) * 4 + 0x94) = kaze_gte_project_vertex(
            kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x40 + 0x48924) +
                idx + 8,
            *(int *)(field + 0x58A14), idx);
        *(int *)(sp + 0xA4) = *(int *)(sp + 0xA4) + 1;
    } while (*(int *)(sp + 0xA4) < 4);
    if (kaze_burner_state != 0) {
        kaze_balloon_overlay_builder(ot);
    }
    gte_SetRotMatrix(sp + 0x20);
    gte_SetTransMatrix(sp + 0x20);
    gte_SetLightMatrix(sp + 0x60);
    *(int *)(kaze_state_base_ptr + 0x9c) = kaze_draw_model_flatlit_3dd2c(
        *(int *)(kaze_state_base_ptr + 0x589ec), 0, ot,
        *(int *)(kaze_state_base_ptr + 0x9c), 2);
    { int *_p = (int *)sp; _p[41] = 0; }
    do {
        int idx;
        char *field;
        register char *root asm("$5");
        register int *acc asm("$4");
        register int old asm("$3");
        int ret;
        idx = *(int *)(sp + 0xA4);
        field = (char *)(idx * 4);
        field += (int)kaze_state_base_ptr;
        __asm__("" : : "r"(field));
        idx <<= 4;
        ret = kaze_gte_project_vertex(
            kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x40 + 0x48924) +
                idx + 0xC,
            *(int *)(field + 0x58A24), idx);
        acc = (int *)(*(int *)(sp + 0xA4) * 4);
        acc = (int *)((char *)acc + (int)sp);
        old = acc[0x25];
        __asm__ volatile("" : : : "memory");
        root = kaze_state_base_ptr;
        acc[0x25] = old + ret;
        __asm__("":"=r"(sp):"0"(sp));
        t = *(int *)(sp + *(int *)(sp + 0xA4) * 4 + 0x94);
        AddPrim((char *)ot + ((t + (int)((u32)t >> 31)) >> 3 << 2),
                root + (*(int *)(root + 0x98) * 0x40 + 0x48924) +
                    *(int *)(sp + 0xA4) * 0x10);
        *(int *)(sp + 0xA4) = *(int *)(sp + 0xA4) + 1;
    } while (*(int *)(sp + 0xA4) < 4);
    if (kaze_burner_state != 0) {
        *(s16 *)(kaze_state_base_ptr + 0x481A8) = 0;
        *(s16 *)(kaze_state_base_ptr + 0x481A2) = 0;
        *(s16 *)(kaze_state_base_ptr + 0x4819C) = 0;
        SetColorMatrix((int *)(kaze_state_base_ptr + 0x48198));
    }
    PopMatrix();
}
