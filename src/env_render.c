#include "types.h"
#include "kaze_syms.h"
extern int ScaleMatrix(int *, int *);
extern int kaze_draw_model(int, int, int, int, int, int, int, int);
typedef struct { char b[8]; } rot8;

extern int kaze_stage_flags;
extern int PushMatrix(void);
extern int PopMatrix(void);
extern int ReadRotMatrix(int *);
extern int rsin(int);
extern int rcos(int);
#define gte_ldv0(x) __asm__ volatile("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" :: "r"(x))
#define gte_rt() __asm__ volatile("nop\n\tnop\n\tcop2 0x480012")
#define gte_rtps_b() __asm__ volatile("cop2 0x180001")
#define gte_stsxy(x) __asm__ volatile("swc2 $14, 0(%0)" :: "r"(x) : "memory")
#define gte_stszotz(x) __asm__ volatile("mfc2 $12, $19\n\tnop\n\tsra $12, $12, 2\n\tsw $12, 0(%0)" :: "r"(x) : "$12", "memory")
#define gte_stlvnl(p) __asm__ volatile("swc2 $25, 0(%0)\n\tswc2 $26, 4(%0)\n\tswc2 $27, 8(%0)" :: "r"(p) : "memory")
#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0)\n\tlw $13, 4(%0)\n\tctc2 $12, $0\n\tctc2 $13, $1\n\tlw $12, 8(%0)\n\tlw $13, 12(%0)\n\tlw $14, 16(%0)\n\tctc2 $12, $2\n\tctc2 $13, $3\n\tctc2 $14, $4" :: "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0)\n\tlw $13, 24(%0)\n\tctc2 $12, $5\n\tlw $14, 28(%0)\n\tctc2 $13, $6\n\tctc2 $14, $7" :: "r"(r0) : "$12", "$13", "$14")
#define gte_SetLightMatrix(r0) __asm__ volatile("lw $12, 0(%0)\n\tlw $13, 4(%0)\n\tctc2 $12, $8\n\tctc2 $13, $9\n\tlw $12, 8(%0)\n\tlw $13, 12(%0)\n\tlw $14, 16(%0)\n\tctc2 $12, $10\n\tctc2 $13, $11\n\tctc2 $14, $12" :: "r"(r0) : "$12", "$13", "$14")
#define gte_ldclmv(p) __asm__ volatile("lhu $12, 0(%0)\n\tlhu $13, 6(%0)\n\tlhu $14, 12(%0)\n\tmtc2 $12, $9\n\tmtc2 $13, $10\n\tmtc2 $14, $11" :: "r"(p) : "$12", "$13", "$14")
#define gte_rtir() __asm__ volatile("nop\n\tnop\n\tcop2 0x49E012")
#define gte_stclmv(p) __asm__ volatile("mfc2 $12, $9\n\tmfc2 $13, $10\n\tmfc2 $14, $11\n\tsh $12, 0(%0)\n\tsh $13, 6(%0)\n\tsh $14, 12(%0)" :: "r"(p) : "$12", "$13", "$14", "memory")

/**
 * kaze_stage_flag_fx_render  (render, 0x8002ce48)
 *
 * Stage-flag/weather effect renderer: two effect groups gated by the
 * +0xed34c/+0xed378 counters using transforms at +0xed328..+0xed360 and
 * model slots 8-10.
 */

void kaze_stage_flag_fx_render(int ot)
{
    extern int AddPrim(void *, void *);
    extern int RotMatrix(int *, int *);
    extern char *kaze_state_base_ptr;
    char *base;
    int **c;
    int idx;
    register int sz asm("$4");
    register int raw asm("$3");
    short *pkt;
    short sv;
    void *ldp;
    int *pflags;
    int flags;

    pflags = &kaze_stage_flags;
    c = (int **)0x1F800000;
    if ((*pflags & 7) != 0) {
        PushMatrix();
        ReadRotMatrix((int *)0x1F80002C);
        *(s16 *)0x1F80004C = 0;
        sv = *(int *)(kaze_state_base_ptr + 0x6C);
        *(s16 *)0x1F800050 = 0;
        *(s16 *)0x1F80004E = -sv;
        gte_ldv0((void *)0x1F80004C);
        gte_rt();
        gte_stlvnl((void *)0x1F800040);
        gte_SetRotMatrix((int *)0x1F80002C);
        gte_SetTransMatrix((int *)0x1F80002C);
        {
            register char *b asm("$4");
            b = kaze_state_base_ptr;
            c[0] = (int *)(b + 0x50094);
            c[1] = (int *)(b + (*(int *)(b + 0x98) * 0xC80 + 0x4E154));
            flags = *pflags;
            c[2] = (int *)(b + (*(int *)(b + 0x98) * 0xC80 + 0x4C854));
        }
        if ((flags & 5) == 5) {
            c[0x1e] = 0;
            do {
                gte_ldv0((short *)c[0]);
                c[0] = (int *)((short *)c[0] + 4);
                c[0x1e] = (int *)((int)c[0x1e] + 1);
                gte_rtps_b();
                gte_stsxy((short *)c[1] + 4);
                gte_stszotz((int *)&c[0x1d]);
                raw = (int)c[0x1d];
                sz = raw >> 2;
                c[0x1d] = (int *)sz;
                if (sz >= 0) {
                    if (sz < 0x65) {
                        pkt = (short *)c[1];
                        sv = (short)(sz / 0x14);
                        pkt[7] = sv;
                        pkt[6] = sv;
                    } else {
                        register short *epkt asm("$2");
                        epkt = (short *)c[1];
                        epkt[7] = 1;
                        epkt[6] = 1;
                    }
                    AddPrim((void *)(ot + (int)c[0x1d] * 4), c[1]);
                }
                c[1] = (int *)((short *)c[1] + 8);
            } while ((int)c[0x1e] < 200);
        } else if ((flags & 3) == 3) {
            c[0x1e] = 0;
            ldp = (void *)0x1F80004C;
            do {
                *(rot8 *)((char *)c + 0x4C) = *(rot8 *)c[0];
                gte_ldv0(ldp);
                c[0] = (int *)((short *)c[0] + 4);
                c[0x1e] = (int *)((int)c[0x1e] + 1);
                gte_rtps_b();
                *(s16 *)((char *)c + 0x4E) = *(s16 *)((char *)c + 0x4E) + 300;
                gte_stsxy((short *)c[2] + 4);
                gte_stszotz((int *)&c[0x1d]);
                gte_ldv0(ldp);
                idx = (int)c[0x1d];
                c[0x1d] = (int *)(idx >> 2);
                if ((idx >> 2) > 0) {
                    gte_rtps_b();
                    gte_stsxy((short *)c[2] + 6);
                    AddPrim((void *)(ot + (int)c[0x1d] * 4), c[2]);
                }
                c[2] = (int *)((short *)c[2] + 8);
            } while ((int)c[0x1e] < 200);
        }
        PopMatrix();
    }
    if (*(int *)(kaze_state_base_ptr + 0xED34C) != 0) {
        c[0x1e] = 0;
        do {
            *(s16 *)(kaze_state_base_ptr + 0xED330) = (s16)rsin((int)c[0x1e] << 9);
            *(s16 *)(kaze_state_base_ptr + 0xED334) = (s16)rcos((int)c[0x1e] << 9);
            PushMatrix();
            RotMatrix((int *)(kaze_state_base_ptr + 0xED328), (int *)&c[3]);
            gte_SetRotMatrix((int *)(kaze_state_base_ptr + 0x481B8));
            gte_ldclmv((int *)&c[3]);
            gte_rtir();
            gte_stclmv((int *)&c[0xb]);
            gte_ldclmv((int *)((char *)c + 0xe));
            gte_rtir();
            gte_stclmv((int *)((char *)c + 0x2e));
            gte_ldclmv((int *)&c[4]);
            gte_rtir();
            gte_stclmv((int *)&c[0xc]);
            gte_SetRotMatrix((int *)(kaze_state_base_ptr + 0x28));
            gte_ldclmv((int *)&c[3]);
            gte_rtir();
            gte_stclmv((int *)&c[3]);
            gte_ldclmv((int *)((char *)c + 0xe));
            gte_rtir();
            gte_stclmv((int *)((char *)c + 0xe));
            gte_ldclmv((int *)&c[4]);
            gte_rtir();
            gte_stclmv((int *)&c[4]);
            PopMatrix();
            gte_ldv0((int *)(kaze_state_base_ptr + 0xED330));
            gte_rt();
            gte_stlvnl((int *)&c[8]);
            PushMatrix();
            ScaleMatrix((int *)&c[3], (int *)(kaze_state_base_ptr + 0xED338));
            gte_SetRotMatrix((int *)&c[3]);
            gte_SetTransMatrix((int *)&c[3]);
            gte_SetLightMatrix((int *)&c[0xb]);
            base = kaze_state_base_ptr;
            sz = kaze_draw_model(*(int *)(base + 0x58A08), 0x50000100,
                ot, *(int *)(base + 0x9C), 2, *(int *)(base + 0xED34C),
                *(int *)(base + 0xED34C), *(int *)(base + 0xED34C));
            base = kaze_state_base_ptr;
            *(int *)(base + 0x9C) = sz;
            PopMatrix();
            idx = (int)c[0x1e];
            c[0x1e] = (int *)(idx + 1);
        } while ((idx + 1) < 8);
    }
    if (*(int *)(kaze_state_base_ptr + 0xED378) != 0) {
        PushMatrix();
        RotMatrix((int *)(kaze_state_base_ptr + 0xED358), (int *)&c[3]);
        gte_SetRotMatrix((int *)(kaze_state_base_ptr + 0x28));
        gte_ldclmv((int *)&c[3]);
        gte_rtir();
        gte_stclmv((int *)&c[3]);
        gte_ldclmv((int *)((char *)c + 0xe));
        gte_rtir();
        gte_stclmv((int *)((char *)c + 0xe));
        gte_ldclmv((int *)&c[4]);
        gte_rtir();
        gte_stclmv((int *)&c[4]);
        PopMatrix();
        gte_ldv0((int *)(kaze_state_base_ptr + 0xED360));
        gte_rt();
        gte_stlvnl((int *)&c[8]);
        PushMatrix();
        gte_SetRotMatrix((int *)&c[3]);
        gte_SetTransMatrix((int *)&c[3]);
        base = kaze_state_base_ptr;
        sz = kaze_draw_model(*(int *)(base + 0x58A0C), 0x50000100,
            ot, *(int *)(base + 0x9C), 2, *(int *)(base + 0xED378),
            *(int *)(base + 0xED378), *(int *)(base + 0xED378));
        base = kaze_state_base_ptr;
        *(int *)(base + 0x9C) = sz;
        sz = kaze_draw_model(*(int *)(base + 0x58A10), 0x60000100,
            ot, *(int *)(base + 0x9C), 2, *(int *)(base + 0xED378),
            *(int *)(base + 0xED378), *(int *)(base + 0xED378));
        base = kaze_state_base_ptr;
        *(int *)(base + 0x9C) = sz;
        PopMatrix();
    }
}

extern int SetPolyFT4(int);
extern int SetSemiTrans(int, int);
extern int GetTPage(int, int, int, int);
extern int GetClut(int, int);
void kaze_object_vertex_anim();
#undef gte_SetRotMatrix
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
#undef gte_SetTransMatrix
#define gte_SetTransMatrix(r0)                                                 \
    __asm__ volatile("lw $12, 20(%0)\n\t"                                      \
                     "lw $13, 24(%0)\n\t"                                      \
                     "ctc2 $12, $5\n\t"                                        \
                     "lw $14, 28(%0)\n\t"                                      \
                     "ctc2 $13, $6\n\t"                                        \
                     "ctc2 $14, $7"                                            \
                     :: "r"(r0) : "$12", "$13", "$14")
#undef gte_ldclmv
#define gte_ldclmv(p)                                                          \
    __asm__ volatile("lhu $12, 0(%0)\n\t"                                      \
                     "lhu $13, 6(%0)\n\t"                                      \
                     "lhu $14, 12(%0)\n\t"                                     \
                     "mtc2 $12, $9\n\t"                                        \
                     "mtc2 $13, $10\n\t"                                       \
                     "mtc2 $14, $11"                                           \
                     :: "r"(p) : "$12", "$13", "$14")
#undef gte_rtir
#define gte_rtir()                                                             \
    __asm__ volatile("nop\n\t"                                                 \
                     "nop\n\t"                                                 \
                     "cop2 0x49E012")
#undef gte_stclmv
#define gte_stclmv(p)                                                          \
    __asm__ volatile("mfc2 $12, $9\n\t"                                        \
                     "mfc2 $13, $10\n\t"                                       \
                     "mfc2 $14, $11\n\t"                                       \
                     "sh $12, 0(%0)\n\t"                                       \
                     "sh $13, 6(%0)\n\t"                                       \
                     "sh $14, 12(%0)"                                          \
                     :: "r"(p) : "$12", "$13", "$14", "memory")
#undef gte_ldv0
#define gte_ldv0(p)                                                            \
    __asm__ volatile("lwc2 $0, 0(%0)\n\t"                                      \
                     "lwc2 $1, 4(%0)"                                          \
                     :: "r"(p))
#undef gte_rt
#define gte_rt()                                                               \
    __asm__ volatile("nop\n\t"                                                 \
                     "nop\n\t"                                                 \
                     "cop2 0x480012")
#define gte_rtv0()                                                             \
    __asm__ volatile("nop\n\t"                                                 \
                     "nop\n\t"                                                 \
                     "cop2 0x486012")
#undef gte_stlvnl
#define gte_stlvnl(p)                                                          \
    __asm__ volatile("swc2 $25, 0(%0)\n\t"                                     \
                     "swc2 $26, 4(%0)\n\t"                                     \
                     "swc2 $27, 8(%0)"                                         \
                     :: "r"(p) : "memory")

/**
 * kaze_trail_particle_render  (render, 0x8002d840)
 *
 * Renders the 128 +0x5988c trail particles as semi-trans POLY_FT4
 * billboards: TPage(0x200,0x100)/CLUT(0x1c0,0x1f9), yaw/pitch billboard
 * matrix from root+0x48/+0x4a, color from ambient +0x481d8..e0 clamped
 * 0x30..0xff.
 */

void kaze_trail_particle_render(int ot)
{
    extern int RotMatrix(char *, char *);
    extern char *kaze_state_base_ptr;
    int col;
    int n;
    int e;
    int *particle;
    char *pM;
    char *pL;
    char *m;
    char *r0;

    SetPolyFT4(0x1f8000b4);
    SetSemiTrans(0x1f8000b4, 1);
    *(u8 *)0x1f8000c0 = 0x60;
    *(u8 *)0x1f8000c1 = 0x20;
    *(u8 *)0x1f8000c8 = 0x7f;
    *(u8 *)0x1f8000c9 = 0x20;
    *(u8 *)0x1f8000d0 = 0x60;
    *(u8 *)0x1f8000d1 = 0x3f;
    *(u8 *)0x1f8000d8 = 0x7f;
    *(u8 *)0x1f8000d9 = 0x3f;
    *(u16 *)0x1f8000ca = GetTPage(0, 1, 0x200, 0x100);
    *(u16 *)0x1f8000c2 = GetClut(0x1c0, 0x1f9);
    *(s16 *)0x1f800054 = 0;
    *(s16 *)0x1f800056 = -0x60;
    *(s16 *)0x1f800058 = -0x60;
    *(s16 *)0x1f80005c = 0;
    *(s16 *)0x1f80005e = -0x60;
    *(s16 *)0x1f800060 = 0x60;
    *(s16 *)0x1f800064 = 0;
    *(s16 *)0x1f800066 = 0x60;
    *(s16 *)0x1f800068 = -0x60;
    *(s16 *)0x1f80006c = 0;
    *(s16 *)0x1f80006e = 0x60;
    *(s16 *)0x1f800070 = 0x60;
    *(s16 *)0x1f80004c = 0;
    *(int *)0x1f800040 = (int)(kaze_state_base_ptr + 0x5988c);
    *(u16 *)0x1f80004e = *(u16 *)(kaze_state_base_ptr + 0x4a);
    *(u16 *)0x1f800050 = *(u16 *)(kaze_state_base_ptr + 0x48);
    n = *(int *)(kaze_state_base_ptr + 0x481d8) * 0x7f;
    m = (char *)0x1f800000;
    if (n < 0) {
        n = n + 0xfff;
    }
    col = n >> 0xc;
    *(int *)(m + 0xe0) = col;
    if (0x2f < col) {
        e = 0xff;
        if (col < 0x100) {
            e = col;
        }
    } else {
        e = 0x30;
    }
    *(int *)(m + 0xe0) = e;
    n = *(int *)(kaze_state_base_ptr + 0x481dc) * 0x7f;
    if (n < 0) {
        n = n + 0xfff;
    }
    col = n >> 0xc;
    *(int *)(m + 0xe4) = col;
    if (0x2f < col) {
        e = 0xff;
        if (col < 0x100) {
            e = col;
        }
    } else {
        e = 0x30;
    }
    *(int *)(m + 0xe4) = e;
    n = *(int *)(kaze_state_base_ptr + 0x481e0) * 0x7f;
    if (n < 0) {
        n = n + 0xfff;
    }
    col = n >> 0xc;
    *(int *)(m + 0xe8) = col;
    if (0x2f < col) {
        e = 0xff;
        if (col < 0x100) {
            e = col;
        }
    } else {
        e = 0x30;
    }
    *(int *)(m + 0xe8) = e;
    PushMatrix();
    pM = m + 0x20;
    RotMatrix(m + 0x4c, pM);
    gte_SetRotMatrix(pM);
    gte_ldv0(m + 0x54);
    gte_rtv0();
    gte_stlvnl(m + 0x74);
    gte_SetRotMatrix(pM);
    gte_ldv0(m + 0x5c);
    gte_rtv0();
    gte_stlvnl(m + 0x84);
    gte_SetRotMatrix(pM);
    gte_ldv0(m + 0x64);
    gte_rtv0();
    gte_stlvnl(m + 0x94);
    gte_SetRotMatrix(pM);
    gte_ldv0(m + 0x6c);
    gte_rtv0();
    gte_stlvnl(m + 0xa4);
    r0 = m + 2;
    PopMatrix();
    pL = m + 4;
    *(int *)(m + 0xf4) = 0;
    do {
        particle = *(int **)(m + 0x40);
        if ((*particle != 0) && (particle[1] != 0)) {
            PushMatrix();
            RotMatrix((char *)(*(int *)(m + 0x40) + 0x20), m);
            gte_SetRotMatrix(kaze_state_base_ptr + 0x28);
            gte_ldclmv(m);
            gte_rtir();
            gte_stclmv(m);
            gte_ldclmv(r0);
            gte_rtir();
            gte_stclmv(r0);
            gte_ldclmv(pL);
            gte_rtir();
            gte_stclmv(pL);
            PopMatrix();
            gte_ldv0(*(int *)(m + 0x40) + 0x18);
            gte_rt();
            gte_stlvnl(m + 0x14);
            PushMatrix();
            gte_SetRotMatrix(m);
            gte_SetTransMatrix(m);
            kaze_object_vertex_anim(ot, m, *(int *)(*(int *)(m + 0x40) + 0x28));
            PopMatrix();
        }
        *(int *)(m + 0xf4) = *(int *)(m + 0xf4) + 1;
        *(int *)(m + 0x40) = *(int *)(m + 0x40) + 0x2c;
    } while (*(int *)(m + 0xf4) < 0x80);
}

#include "gte.h"
void kaze_prim_buffer_append();

/**
 * kaze_object_vertex_anim  (render, 0x8002de14)
 *
 * Per-object procedural vertex animation using a phase counter in the object
 * struct (+0xec) to index and write animated vertices.
 */

void kaze_object_vertex_anim(int ot, int obj, int phase)
{
    extern int kaze_state_base_ptr;
    int state_base;
    int scratch;
    int phase_base;
    int amp;
    long *r0;
    int vtx_base_ofs;
    int wobble_raw;
    int e0, e4, e8;
    int iA, iB, iC;
    int iAddr, iOff, iCnt;
    int iRs, t15, iv6c;
    int t16;
    int iOffc;
    register int z asm("$2");

    e0 = *(int *)(obj + 0xe0);
    iA = phase * e0;
    if (iA < 0) iA = iA + 0xfff;
    e4 = *(int *)(obj + 0xe4);
    *(char *)(obj + 0xb8) = (char)e0 - (char)(iA >> 0xc);
    iB = phase * e4;
    if (iB < 0) iB = iB + 0xfff;
    e8 = *(int *)(obj + 0xe8);
    *(char *)(obj + 0xb9) = (char)e4 - (char)(iB >> 0xc);
    iC = phase * e8;
    if (iC < 0) iC = iC + 0xfff;
    phase_base = phase * 5;
    vtx_base_ofs = 0x508dc;
    r0 = (long *)(obj + 0xdc);
    *(char *)(obj + 0xba) = (char)e8 - (char)(iC >> 0xc);
    *(int *)(obj + 0xec) = 0;
LAB_outer:
    {
        scratch = phase * -0x10;
        if (scratch < 0) scratch = scratch + 0xff;
        *(short *)(obj + 0x46) = (short)(scratch >> 8);
        z = rsin(*(int *)(obj + 0xec) << 9);
        t15 = phase_base << 4;
        t15 -= phase_base;
        wobble_raw = t15 << 1;
        iv6c = wobble_raw;
        iRs = z;
        if (wobble_raw < 0) iv6c = wobble_raw + 0xfff;
        __asm__("" :: "r"(phase_base));
        amp = (iv6c >> 0xc) + 100;
        scratch = iRs * amp;
        if (scratch < 0) scratch = scratch + 0xfff;
        *(short *)(obj + 0x44) = (short)(scratch >> 0xc);
        scratch = rcos(*(int *)(obj + 0xec) << 9);
        __asm__("" :: "r"(wobble_raw));
        scratch = scratch * amp;
        if (scratch < 0) scratch = scratch + 0xfff;
        state_base = kaze_state_base_ptr;
        *(short *)(obj + 0x48) = (short)(scratch >> 0xc);
        *(int *)(obj + 0xf0) = 0;
        do {
            {
                int f0v = *(int *)(obj + 0xf0);
                __asm__("" :: "r"(f0v) : "memory");
                iAddr = state_base + (f0v * 8 + *(int *)(obj + 0xec) * 0x20);
                t16 = f0v * 0x10;
                *(short *)(iAddr + 0x508dc) =
                    *(short *)(obj + 0x44) + (short)*(int *)(obj + t16 + 0x7c);
            }
            {
                int f0v = *(int *)(obj + 0xf0);
                __asm__("" :: "r"(f0v) : "memory");
                iAddr = state_base + (f0v * 8 + *(int *)(obj + 0xec) * 0x20);
                t16 = f0v * 0x10;
                *(short *)(iAddr + 0x508de) =
                    *(short *)(obj + 0x46) + (short)*(int *)(obj + t16 + 0x78);
            }
            {
                int f0v = *(int *)(obj + 0xf0);
                __asm__("" :: "r"(f0v) : "memory");
                iAddr = state_base + (f0v * 8 + *(int *)(obj + 0xec) * 0x20);
                t16 = f0v * 0x10;
                *(short *)(iAddr + 0x508e0) =
                    *(short *)(obj + 0x48) + (short)*(int *)(obj + t16 + 0x74);
            }
            iCnt = *(int *)(obj + 0xf0) + 1;
            *(int *)(obj + 0xf0) = iCnt;
        } while (iCnt < 4);
        *(int *)(obj + 0xf0) = 0;
        do {
            {
                int addr = kaze_state_base_ptr + (*(int *)(obj + 0xec) * 0x20 + vtx_base_ofs) + *(int *)(obj + 0xf0) * 8;
                iOff = *(int *)(obj + 0xf0) * 8;
                __asm__ volatile("lwc2 $0, 0(%0)" :: "r"(addr));
                __asm__ volatile("lwc2 $1, 4(%0)" :: "r"(addr));
                __asm__ volatile("nop");
                __asm__ volatile("nop");
                gte_rtps();
                iOffc = iOff + 0xbc;
                __asm__ volatile("swc2 $14, 0(%0)" :: "r"(obj + iOffc) : "memory");
                __asm__ volatile(
                    "mfc2 $12, $19\n\t"
                    "nop\n\t"
                    "sra $12, $12, 2\n\t"
                    "sw $12, 0(%0)"
                    :: "r"(r0) : "$12", "memory");
            }
            iCnt = *(int *)(obj + 0xf0) + 1;
            *(int *)(obj + 0xf0) = iCnt;
        } while (iCnt < 4);
        kaze_prim_buffer_append(ot + (*(int *)(obj + 0xdc) >> 2) * 4, obj + 0xb4, 9);
        iCnt = *(int *)(obj + 0xec) + 1;
        *(int *)(obj + 0xec) = iCnt;
    }
    if (iCnt < 8) goto LAB_outer;
}

/*
 * Appends `count` words to the primitive buffer, chaining an OT entry.
 * De-islanded: the original reserves an unused 8-byte stack frame
 * (`addiu sp,-8` at position 0 / `addiu sp,8` in the epilogue) even though the
 * body never touches the stack.  A zero-code frame-forcer `int pad[2];
 * __asm__("" :: "m"(pad[0]))` reserves the 8 bytes; with the register pins
 * below gcc emits the sp-adjust at the true prologue position (without the
 * pins it schedules into the body at +0x38).  The remaining empty asm
 * statements are zero-code reload barriers pinning the per-iteration
 * kaze_state_base_ptr reloads; all are lawful (island_audit clean).
 */

/**
 * kaze_prim_buffer_append  (render, 0x8002e108)
 *
 * Appends a count-word GPU packet into the frame primitive buffer at live-
 * root +0x9c (cursor advance), writing the length/link header and re-linking
 * the caller's tag ot to the new packet.
 */

void kaze_prim_buffer_append(unsigned int *ot, unsigned int *src, unsigned int count)
{
    extern volatile int kaze_state_base_ptr;
    register unsigned int *ot_save __asm__("$8");
    register unsigned int scratch __asm__("$7");
    register unsigned int work __asm__("$3");
    register unsigned int *base0 __asm__("$4");
    register unsigned int idx __asm__("$4");
    register unsigned int *pre_cursor __asm__("$2");
    unsigned int old_tag;
    int pad[2];

    ot_save = ot;
    __asm__("" :: "m"(pad[0]));

    scratch = 0xFFFFFF;
    work = count << 24;
    old_tag = *ot_save;
    base0 = (unsigned int *)kaze_state_base_ptr;
    old_tag = old_tag & scratch;
    scratch = base0[0x9c / 4];
    work = work | old_tag;
    *(unsigned int *)scratch = work;
    work = kaze_state_base_ptr;
    src = src + 1;
    pre_cursor = (unsigned int *)((unsigned int *)work)[0x9c / 4];
    idx = 0;
    if ((((unsigned int *)work)[0x9c / 4] = (unsigned int)(pre_cursor + 1), count != 0)) {
        do {
            register unsigned int *loop_base __asm__("$2");
            register unsigned int *loop_cursor __asm__("$3");
            register unsigned int value __asm__("$2");

            loop_base = (unsigned int *)kaze_state_base_ptr;
            __asm__ volatile("" : "=r"(loop_base) : "0"(loop_base));
            loop_cursor = (unsigned int *)loop_base[0x9c / 4];
            value = *src;
            __asm__ volatile("" : "=r"(value) : "0"(value));
            *loop_cursor = value;
            loop_cursor = (unsigned int *)kaze_state_base_ptr;
            idx++;
            pre_cursor = (unsigned int *)loop_cursor[0x9c / 4];
            pre_cursor = pre_cursor + 1;
            loop_cursor[0x9c / 4] = (unsigned int)pre_cursor;
            src++;
        } while (idx < count);
    }
    pre_cursor = (unsigned int *)(scratch & 0xFFFFFF);
    *ot_save = (unsigned int)pre_cursor;

}

/**
 * kaze_env_sky_render_pass  (render, 0x8002e1b8)
 *
 * Above-horizon sky render pass gated by state flag 0x20000000: sky gradient
 * POLY_G4 layers (+0x48354) with per-layer draw flags, DR_TPAGEs (+0x486b4),
 * horizon fill, route/start sprite ribbon culling, and particles.
 */

void kaze_env_sky_render_pass(void *arg0)
{
    extern void AddPrim(void *, void *);
    extern char *kaze_state_base_ptr;
    int idx;
    char *s5;
    char *s4;
    char *s3;
    char *s0;
    void *ot;
    int i;
    int a1v;
    int v;

    idx = *(int *)(kaze_state_base_ptr + 0x98);
    ot = (char *)arg0 + 4;
    s5 = kaze_state_base_ptr + (idx * 0x78 + 0x4A194);
    s3 = s5 + 0x50;
    s4 = kaze_state_base_ptr + (idx * 0x12C0 + 0x4A2D4);
    if (*(int *)(kaze_state_base_ptr + 0x14) & 0x20000000) {
        a1v = idx * 0xD8;
        if (*(u8 *)(kaze_state_base_ptr + a1v + 0x4841E) != 0) {
            AddPrim(ot, kaze_state_base_ptr + (a1v + 0x48354) + 0x90);
            AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0xD8 + 0x48354) + 0xB4);
            AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x18 + 0x486B4) + 0x10);
        }
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0xD8 + 0x48354));
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0xD8 + 0x48354) + 0x24);
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x18 + 0x486B4));
        a1v = *(int *)(kaze_state_base_ptr + 0x98) * 0xD8;
        if (*(u8 *)(kaze_state_base_ptr + a1v + 0x483D6) != 0) {
            AddPrim(ot, kaze_state_base_ptr + (a1v + 0x48354) + 0x48);
            AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0xD8 + 0x48354) + 0x6C);
            AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x18 + 0x486B4) + 0x8);
        }
        i = 2;
        if (kaze_stage_flags & 1) {
            kaze_sky_cloud_quads_render(ot);
        } else {
            s0 = s5 + 0x6D;
            do {
                if (*(u8 *)(s0 - 0x10) != *(u8 *)(s0)) {
                    AddPrim(ot, s3);
                }
                i -= 1;
                s0 -= 0x28;
                s3 -= 0x28;
            } while (i >= 0);
            a1v = *(int *)(kaze_state_base_ptr + 0x98) * 0x50;
            v = *(s16 *)(kaze_state_base_ptr + a1v + 0x4871E);
            if (v + 0x40 >= 0 && v < 0xF0) {
                AddPrim(ot, kaze_state_base_ptr + (a1v + 0x48714));
                AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x50 + 0x48714) + 0x14);
                AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x50 + 0x48714) + 0x28);
                AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x50 + 0x48714) + 0x3C);
                AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x8 + 0x487B4));
            }
            if (*(int *)(kaze_state_base_ptr + 0x4827C) != 0) {
                AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x28 + 0x4A284));
                i = 0;
                do {
                    AddPrim(ot, s4);
                    i += 1;
                    s4 += 0xC;
                } while (i < 0x190);
            }
        }
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x30 + 0x482F4));
    }
}
