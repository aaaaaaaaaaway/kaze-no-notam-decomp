#include "types.h"
#include "kaze_syms.h"
extern int kaze_gas;
extern int kaze_gas_floor;
extern int kaze_min_gas;
extern int kaze_target_gas;
extern int kaze_vertical_penalty;
extern int kaze_balloon_angle;
extern int kaze_pos_y;
extern void kaze_minimap_radar_builder(int, int);
typedef struct 
{
  int dir;
  int spd;
} WL;

#include "gte.h"
extern char *kaze_state_base_ptr;
extern int PushMatrix(void);
extern int PopMatrix(void);
extern int ReadRotMatrix(void *);
extern int rsin(int);
extern int rcos(int);
#define G kaze_state_base_ptr
#define SPSTORE_W(val, off) (*(int *)(0x1F800000 + (off)) = (int)(val))
#define SPLOAD_W(dst, off)                                                     \
    do { int _b; __asm__ volatile("li %0,0x1f800000" : "=r"(_b));                        \
         (dst) = *(int *)(_b + (off)); } while (0)
#define SPLOAD_P(dst, off)                                                     \
    do { int _b; __asm__ volatile("li %0,0x1f800000" : "=r"(_b));                        \
         (dst) = *(char **)(_b + (off)); } while (0)
#define SPLOAD_LH(dst, off)                                                    \
    do { int _b; __asm__ volatile("li %0,0x1f800000" : "=r"(_b));                        \
         (dst) = *(s16 *)(_b + (off)); } while (0)
#define SPLOAD_LHU(dst, off)                                                   \
    do { int _b; __asm__ volatile("li %0,0x1f800000" : "=r"(_b));                        \
         (dst) = *(u16 *)(_b + (off)); } while (0)
#define LDV01(p) do { \
    __asm__ volatile("lwc2 $0,0(%0)" :: "r"(p)); \
    __asm__ volatile("lwc2 $1,4(%0)" :: "r"(p)); } while (0)
#define LD6V(p) do {                                                           \
    register char *_b __asm__("$9") = (char *)(p);                             \
    __asm__ volatile("lwc2 $0,0(%0)" :: "r"(_b));                              \
    __asm__ volatile("lwc2 $1,4(%0)" :: "r"(_b));                              \
    __asm__ volatile("lwc2 $2,8(%0)" :: "r"(_b));                              \
    __asm__ volatile("lwc2 $3,0xc(%0)" :: "r"(_b));                            \
    __asm__ volatile("lwc2 $4,0x10(%0)" :: "r"(_b));                           \
    __asm__ volatile("lwc2 $5,0x14(%0)" :: "r"(_b)); } while (0)
#define MVMVA() gte_rtv0tr()
#define RTPT() do { gte_nop(); gte_nop(); gte_rtpt(); } while (0)
#define RTPS() gte_rtps()
#define RTPS_2NOP() do { gte_nop(); gte_nop(); gte_rtps(); } while (0)
#define STMAC3(p) do {                                                         \
    register char *_b __asm__("$9") = (char *)(p);                             \
    __asm__ volatile("swc2 $25,0(%0)" :: "r"(_b) : "memory");                  \
    __asm__ volatile("swc2 $26,4(%0)" :: "r"(_b) : "memory");                  \
    __asm__ volatile("swc2 $27,8(%0)" :: "r"(_b) : "memory"); } while (0)
#define STSXY3(p) do {                                                         \
    register char *_b __asm__("$9") = (char *)(p);                             \
    __asm__ volatile("swc2 $12,0(%0)" :: "r"(_b) : "memory");                  \
    __asm__ volatile("swc2 $13,4(%0)" :: "r"(_b) : "memory");                  \
    __asm__ volatile("swc2 $14,8(%0)" :: "r"(_b) : "memory"); } while (0)
#define STSXY1(p) __asm__ volatile("swc2 $14,0(%0)" :: "r"(p) : "memory")
#define SETROT(r0) do {                                                        \
    register int _c12 __asm__("$12");                                          \
    register int _c13 __asm__("$13");                                          \
    register int _c14 __asm__("$14");                                          \
    __asm__ volatile("lw %0,0(%1)"  : "=r"(_c12) : "r"(r0));                   \
    __asm__ volatile("lw %0,4(%1)"  : "=r"(_c13) : "r"(r0));                   \
    __asm__ volatile("ctc2 %0,$0" :: "r"(_c12));                               \
    __asm__ volatile("ctc2 %0,$1" :: "r"(_c13));                               \
    __asm__ volatile("lw %0,8(%1)"  : "=r"(_c12) : "r"(r0));                   \
    __asm__ volatile("lw %0,12(%1)" : "=r"(_c13) : "r"(r0));                   \
    __asm__ volatile("lw %0,16(%1)" : "=r"(_c14) : "r"(r0));                   \
    __asm__ volatile("ctc2 %0,$2" :: "r"(_c12));                               \
    __asm__ volatile("ctc2 %0,$3" :: "r"(_c13));                               \
    __asm__ volatile("ctc2 %0,$4" :: "r"(_c14)); } while (0)
#define SETTRANS(r0) do {                                                      \
    register int _c12 __asm__("$12");                                          \
    register int _c13 __asm__("$13");                                          \
    register int _c14 __asm__("$14");                                          \
    __asm__ volatile("lw %0,20(%1)" : "=r"(_c12) : "r"(r0));                   \
    __asm__ volatile("lw %0,24(%1)" : "=r"(_c13) : "r"(r0));                   \
    __asm__ volatile("ctc2 %0,$5" :: "r"(_c12));                               \
    __asm__ volatile("lw %0,28(%1)" : "=r"(_c14) : "r"(r0));                   \
    __asm__ volatile("ctc2 %0,$6" :: "r"(_c13));                               \
    __asm__ volatile("ctc2 %0,$7" :: "r"(_c14)); } while (0)
#define CHECKED_DIV_STEP(dst, num, den)                                        \
    __asm__ volatile(                                                          \
        ".set\tnoreorder\n\t"                                                  \
        ".set\tnoat\n\t"                                                       \
        "nop\n\t"                                                              \
        "div $zero,%1,%2\n\t"                                                  \
        "bnez %2,1f\n\t"                                                       \
        "nop\n\t"                                                              \
        "break 0x1c00\n"                                                       \
        "1:\n\t"                                                               \
        "addiu $1,$zero,-1\n\t"                                                \
        "bne %2,$1,2f\n\t"                                                     \
        "lui $1,0x8000\n\t"                                                    \
        "bne %1,$1,2f\n\t"                                                     \
        "nop\n\t"                                                              \
        "break 0x1800\n"                                                       \
        "2:\n\t"                                                               \
        ".set\tat\n\t"                                                         \
        ".set\treorder\n\t"                                                    \
        "mflo %0\n\t"                                                           \
        "nop"                                                              \
        : "=r"(dst)                                                            \
        : "r"(num), "r"(den)                                                   \
        : "$1")

/**
 * kaze_render_vector_load  (render, 0x80024174)
 *
 * Loads the route/view render SVECTOR at +0x509e0 into the GTE.
 */

void kaze_render_vector_load(void)
{
    char *m = (char *)0x1F800000;
    char *g7;

    PushMatrix();
    {
        int t6c = *(int *)(G + 0x6C);
        *(s16 *)(G + 0x509E0) = 0;
        *(s16 *)(G + 0x509E4) = 0;
        *(s16 *)(G + 0x509E2) = -t6c;
    }
    ReadRotMatrix((void *)0x1F800000);
    { char *hp0 = G + 0x509E0; LDV01(hp0); }
    __asm__ volatile("");
    *(void **)0x1F800048 = G + (*(int *)(G + 0x98) * 0xD8 + 0x48354);
    *(void **)0x1F80004C = G + (*(int *)(G + 0x98) * 0xD8 + 0x48504);
    MVMVA();
    {
        int _idx3 = *(int *)(G + 0x98);
        int _off3 = _idx3 * 0x30 + 0x482F4;
        register char *hp1 __asm__("$9");
        __asm__("" :: "r"(hp1), "r"(_idx3));
        hp1 = (char *)0x1F800014;
        __asm__("" : "=r"(hp1) : "0"(hp1), "r"(_off3));
        *(void **)0x1F800050 = G + _off3;
        __asm__ volatile("swc2 $25,0(%0)" :: "r"(hp1) : "memory");
        __asm__ volatile("swc2 $26,4(%0)" :: "r"(hp1) : "memory");
        __asm__ volatile("swc2 $27,8(%0)" :: "r"(hp1) : "memory");
    }
    SETROT(m);
    SETTRANS(m);

    {
        int p;
        int a0v;
        int a1v;
        int t;
        char *d;
        int rs = rsin(*(s16 *)(G + 0x4A)) * 0xED8;
        d = G;
        SPSTORE_W(-rs, 0x68);
        p = rcos(*(s16 *)(d + 0x4A)) * 475;
        __asm__ volatile("li %0,0x1f800000" : "=r"(a0v));
        a0v = *(int *)((char *)a0v + 0x68);
        a1v = p << 3;
        *(int *)0x1F80006C = a1v;
        t = a0v / 512; *(s16 *)0x1F800020 = t;
        t = a1v / 512; *(s16 *)0x1F800024 = t;
        t = a0v * 3; if (t < 0) t += 0xFFF; *(s16 *)0x1F800028 = t >> 12;
        t = (p << 4) + a1v; if (t < 0) t += 0xFFF; *(s16 *)0x1F80002C = t >> 12;
        t = a0v; if (t < 0) t += 0xFFF; *(s16 *)0x1F800030 = t >> 12;
        t = a1v; if (t < 0) t += 0xFFF; t >>= 12;
        do {
            register char *_b6 __asm__("$9") = (char *)0x1F800020;
            *(s16 *)0x1F800034 = t;
            *(s16 *)0x1F800032 = 0;
            *(s16 *)0x1F80002A = 0;
            *(s16 *)0x1F800022 = 0;
            __asm__ volatile("lwc2 $0,0(%0)" :: "r"(_b6));
            __asm__ volatile("lwc2 $1,4(%0)" :: "r"(_b6));
            __asm__ volatile("lwc2 $2,8(%0)" :: "r"(_b6));
            __asm__ volatile("lwc2 $3,0xc(%0)" :: "r"(_b6));
            __asm__ volatile("lwc2 $4,0x10(%0)" :: "r"(_b6));
            __asm__ volatile("lwc2 $5,0x14(%0)" :: "r"(_b6));
        } while (0);
    }
    RTPT();
    { char *hp3 = (char *)0x1F800038; STSXY3(hp3); }

    {
        char *g = G;
        int f = *(int *)(g + 0x14) & 0xCFFFFFFF;
        int t;
        *(int *)(g + 0x14) = f;
        SPLOAD_LH(t, 0x3A);
        if (t < 0) {
            char *p;
            *(int *)(g + 0x14) = f | 0x10000000;
            SPLOAD_P(p, 0x50);
            *(s16 *)(p + 0x26) = 0;
            *(s16 *)(p + 0x22) = 0;
            *(s16 *)(p + 0x16) = 0;
            *(s16 *)(p + 0x12) = 0;
            *(s16 *)(p + 0xE) = 0;
            *(s16 *)(p + 0xA) = 0;
            goto block19;
        }
        if (t >= 0xF1) {
            char *p;
            int t2;
            *(int *)(g + 0x14) = f | 0x20000000;
            SPLOAD_P(p, 0x50);
            *(s16 *)(p + 0xE) = 0;
            *(s16 *)(p + 0xA) = 0;
            {
            s16 t2c;
            __asm__ volatile("li %0,0x1f800000" : "=r"(t2));
            t2 = *(s16 *)((char *)t2 + 0x3A);
            t2c = t2;
            if (t2 >= 0xF1) {
                int c = 0xF0;
                char *q;
                SPLOAD_P(q, 0x50);
                *(s16 *)(q + 0x2E) = c;
                *(s16 *)(q + 0x2A) = c;
                *(s16 *)(q + 0x26) = c;
                *(s16 *)(q + 0x22) = c;
                *(s16 *)(q + 0x16) = c;
                *(s16 *)(q + 0x12) = c;
            } else {
                char *q;
                __asm__ volatile("li %0,0x1f800000" : "=r"(q));
                q = *(char **)(q + 0x50);
                *(s16 *)(q + 0x2E) = t2c;
                *(s16 *)(q + 0x2A) = t2c;
                *(s16 *)(q + 0x26) = t2c;
                *(s16 *)(q + 0x22) = t2c;
                *(s16 *)(q + 0x16) = t2c;
                *(s16 *)(q + 0x12) = t2c;
            }
            }
        } else {
            char *p;
            char *q;
            int u;
            *(int *)(g + 0x14) = f | 0x30000000;
            SPLOAD_P(p, 0x50);
            *(s16 *)(p + 0xE) = 0;
            *(s16 *)(p + 0xA) = 0;
            __asm__ volatile("li %0,0x1f800000" : "=r"(q));
            q = *(char **)(q + 0x50);
            SPLOAD_LHU(u, 0x3A);
            *(s16 *)(q + 0x26) = u;
            *(s16 *)(q + 0x22) = u;
            *(s16 *)(q + 0x16) = u;
            *(s16 *)(q + 0x12) = u;
block19:;
            __asm__ volatile("li %0,0x1f800000" : "=r"(q));
            q = *(char **)(q + 0x50);
            *(s16 *)(q + 0x2E) = 0xF0;
            *(s16 *)(q + 0x2A) = 0xF0;
        }
    }

    /* phase5: map/objective marker vertex setup (D held in a1, no calls) */
    {
    char *g = G;
    { int off;
      int a;
      off = *(int *)(g + 0x98) * 0x50;
      __asm__("");
      a = (*(u16 *)(g + 0x4A)) & 0x1FF;
      __asm__("" : "=r"(a) : "0"(a));
      *(s16 *)(g + off + 0x4871C) = (a >> 1) - 0x100; }
    { char *b = g + *(int *)(g + 0x98) * 0x50;
      *(s16 *)(b + 0x48730) = *(u16 *)(b + 0x4871C) + 0x80; }
    { char *b = g + *(int *)(g + 0x98) * 0x50;
      *(s16 *)(b + 0x48744) = *(u16 *)(b + 0x48730) + 0x80; }
    { char *b = g + *(int *)(g + 0x98) * 0x50;
      *(s16 *)(b + 0x48758) = *(u16 *)(b + 0x48744) + 0x80; }
    { s16 v = *(u16 *)(m + 0x3A) - 0x50;
      char *b = g + *(int *)(g + 0x98) * 0x50;
      *(s16 *)(b + 0x4875A) = v;
      *(s16 *)(b + 0x48746) = v;
      *(s16 *)(b + 0x48732) = v;
      *(s16 *)(b + 0x4871E) = v; }
    { char *b = g + *(int *)(g + 0x98) * 0x50;
      if (*(s16 *)(b + 0x4871C) < -0x80)
          *(s16 *)(b + 0x4871C) = *(u16 *)(b + 0x48758) + 0x80; }
    }

    /* phase6: light color fills (s0-based) */
    { s16 v = *(u16 *)(m + 0x3A) - 0x90;
      char *b = *(char **)(m + 0x48);
      *(s16 *)(b + 0xA2) = v; *(s16 *)(b + 0x9A) = v; *(s16 *)(b + 0x5A) = v;
      *(s16 *)(b + 0x52) = v; *(s16 *)(b + 0x12) = v; *(s16 *)(b + 0xA) = v; }
    { s16 v = *(u16 *)(m + 0x3A) - 0x10;
      char *b = *(char **)(m + 0x48);
      *(s16 *)(b + 0xC6) = v; *(s16 *)(b + 0xBE) = v; *(s16 *)(b + 0xB2) = v;
      *(s16 *)(b + 0xAA) = v; *(s16 *)(b + 0x7E) = v; *(s16 *)(b + 0x76) = v;
      *(s16 *)(b + 0x6A) = v; *(s16 *)(b + 0x62) = v; *(s16 *)(b + 0x36) = v;
      *(s16 *)(b + 0x2E) = v; *(s16 *)(b + 0x22) = v; *(s16 *)(b + 0x1A) = v; }
    { char *b1 = *(char **)(m + 0x4C);
      u16 v = *(u16 *)(m + 0x3A);
      char *b0 = *(char **)(m + 0x48);
      *(u16 *)(b1 + 0xA2) = v; *(u16 *)(b1 + 0x9A) = v; *(u16 *)(b1 + 0x5A) = v;
      *(u16 *)(b1 + 0x52) = v; *(u16 *)(b1 + 0x12) = v; *(u16 *)(b1 + 0xA) = v;
      *(u16 *)(b0 + 0xD6) = v; *(u16 *)(b0 + 0xCE) = v; *(u16 *)(b0 + 0x8E) = v;
      *(u16 *)(b0 + 0x86) = v; *(u16 *)(b0 + 0x46) = v; *(u16 *)(b0 + 0x3E) = v; }
    { char *b = *(char **)(m + 0x4C);
      u16 v = *(u16 *)(m + 0x3E);
      g7 = G;
      *(u16 *)(b + 0xC6) = v; *(u16 *)(b + 0xBE) = v; *(u16 *)(b + 0xB2) = v;
      *(u16 *)(b + 0xAA) = v; *(u16 *)(b + 0x7E) = v; *(u16 *)(b + 0x76) = v;
      *(u16 *)(b + 0x6A) = v; *(u16 *)(b + 0x62) = v; *(u16 *)(b + 0x36) = v;
      *(u16 *)(b + 0x2E) = v; *(u16 *)(b + 0x22) = v; *(u16 *)(b + 0x1A) = v; }
    { char *b = *(char **)(m + 0x4C);
      u16 v = *(u16 *)(m + 0x42);
      *(u16 *)(b + 0xD6) = v; *(u16 *)(b + 0xCE) = v; *(u16 *)(b + 0x8E) = v;
      *(u16 *)(b + 0x86) = v; *(u16 *)(b + 0x46) = v; *(u16 *)(b + 0x3E) = v; }

    /* phase7 + phase8 loop */
    *(int *)(m + 0x6C) = 0x7FBC;
    *(int *)(m + 0x68) = 0x7FBC;
    *(int *)(m + 0x64) = 0;
    *(int *)(m + 0x44) = (int)(g7 + (*(int *)(g7 + 0x98) * 0x78 + 0x4A194));
    if (*(int *)(m + 0x64) < 3) {
        do {
            {
                int t = *(int *)(G + 0x4826C) * *(int *)(m + 0x68);
                if (t < 0) t += 0xFFF;
                *(s16 *)(m + 0x20) = -(t >> 12);
            }
            {
                int t = *(int *)(G + 0x48270) * *(int *)(m + 0x6C);
                if (t < 0) t += 0xFFF;
                *(s16 *)(m + 0x22) = t >> 12;
            }
            {
                int t = *(int *)(G + 0x48270) * *(int *)(m + 0x68);
                if (t < 0) t += 0x1FFF;
                *(s16 *)(m + 0x24) = -(t >> 13);
            }
            __asm__ volatile("");
            { char *t4 = m + 0x20;
              register char *hp4 __asm__("$2");
              __asm__("" :: "r"(t4));
              hp4 = t4; LDV01(hp4); }
            {
                char *b = *(char **)(m + 0x44);
                u8 c = *(u8 *)(b + 0xD) + 0x7F;
                *(u8 *)(b + 0x25) = c;
                *(u8 *)(b + 0x1D) = c;
            }
            RTPS();
            { char *hp5 = m + 0x3C; STSXY1(hp5); }
            {
                char *b = *(char **)(m + 0x44);
                s16 v = (*(u16 *)(m + 0x3C) - 0x20) - (*(int *)(m + 0x64) << 5);
                *(s16 *)(b + 0x18) = v; *(s16 *)(b + 0x8) = v;
            }
            {
                char *b = *(char **)(m + 0x44);
                s16 v = (*(u16 *)(m + 0x3C) + 0x20) + (*(int *)(m + 0x64) << 5);
                *(s16 *)(b + 0x20) = v; *(s16 *)(b + 0x10) = v;
            }
            {
                char *b = *(char **)(m + 0x44);
                s16 v = (*(u16 *)(m + 0x3E) - 0x20) - (*(int *)(m + 0x64) << 5);
                *(s16 *)(b + 0x12) = v; *(s16 *)(b + 0xA) = v;
            }
            {
                char *b = *(char **)(m + 0x44);
                s16 v = (*(u16 *)(m + 0x3E) + 0x20) + (*(int *)(m + 0x64) << 5);
                *(s16 *)(b + 0x22) = v; *(s16 *)(b + 0x1A) = v;
            }
            {
                register int w __asm__("$2");
                register int cur __asm__("$7");
                register int t0 __asm__("$8");
                char *b;
                w = *(u16 *)(m + 0x3A) << 16;
                cur = w >> 16;
                b = *(char **)(m + 0x44);
                t0 = *(s16 *)(b + 0x1A);
                if (cur < t0) {
                    register int a2v __asm__("$6");
                    a2v = *(s16 *)(b + 0xA);
                    if (cur < a2v) {
                        u8 c = *(u8 *)(b + 0xD);
                        *(u8 *)(b + 0x25) = c; *(u8 *)(b + 0x1D) = c;
                    } else {
                        register int d __asm__("$2");
                        int e;
                        int base;
                        int q;
                        d = cur - a2v;
                        e = *(u8 *)(b + 0x1D);
                        base = *(u8 *)(b + 0xD);
                        { register int hn __asm__("$3");
                          register int hd __asm__("$2");
                          hn = (e - base) * d;
                          __asm__("");
                          hd = t0 - a2v;
                          q = hn / hd; }
                        __asm__("" :: "r"(t0));
                        {
                            u8 c = base + q;
                            *(u8 *)(b + 0x25) = c; *(u8 *)(b + 0x1D) = c;
                        }
                        {
                            char *b2 = *(char **)(m + 0x44);
                            u16 vv = *(u16 *)(m + 0x3A);
                            *(s16 *)(b2 + 0x22) = vv; *(s16 *)(b2 + 0x1A) = vv;
                        }
                    }
                } else if (t0 < 0 || *(s16 *)(b + 0xA) >= 0xF1 ||
                           *(s16 *)(b + 0x10) < 0 || *(s16 *)(b + 8) >= 0x141) {
                    char *b3 = *(char **)(m + 0x44);
                    u8 c = *(u8 *)(b3 + 0xD);
                    *(u8 *)(b3 + 0x25) = c; *(u8 *)(b3 + 0x1D) = c;
                }
            }
            __asm__("");
            *(int *)(m + 0x64) += 1;
            *(int *)(m + 0x68) -= 0xFA0;
            *(int *)(m + 0x6C) -= 0x1770;
            *(int *)(m + 0x44) += 0x28;
        } while (*(int *)(m + 0x64) < 3);
    }

    PopMatrix();
    if (*(int *)(G + 0x4827C) != 0 && (*(int *)(G + 0x14) & 0x20000000)) {
        *(int *)(m + 0x64) = 0;
        { int k = 0x4A2D4;
          char *g5 = G;
          *(int *)(m + 0x54) = (int)(g5 + (*(int *)(g5 + 0x98) * 0x12C0 + k)); }
        *(int *)(m + 0x58) = (int)(G + 0x4FA54);
        *(int *)(m + 0x5C) = (int)(G + 0x4FD74);
        if (*(int *)(m + 0x64) < 0x190) {
            do {
                {
                    int t = rsin(*(u16 *)*(u16 **)(m + 0x58)) * *(u16 *)*(u16 **)(m + 0x5C);
                    if (t < 0) t += 0xFFF;
                    *(s16 *)(m + 0x20) = t >> 12;
                }
                {
                    int t = rcos(*(u16 *)*(u16 **)(m + 0x58)) * *(u16 *)*(u16 **)(m + 0x5C);
                    if (t < 0) t += 0xFFF;
                    *(s16 *)(m + 0x24) = t >> 12;
                }
                {
                    int t = rcos((u16)*(u16 *)*(u16 **)(m + 0x5C) >> 5) * -0x7FFF;
                    if (t < 0) t += 0xFFF;
                    *(s16 *)(m + 0x22) = t >> 12;
                }
                __asm__ volatile("");
                { register char *hp6 __asm__("$2") = m + 0x20; LDV01(hp6); }
                *(int *)(m + 0x58) += 2;
                RTPS();
                *(int *)(m + 0x5C) += 2;
                { char *hp7 = *(char **)(m + 0x54) + 8; STSXY1(hp7); }
                *(int *)(m + 0x64) += 1;
                *(int *)(m + 0x54) += 0xC;
            } while (*(int *)(m + 0x64) < 0x190);
        }
        *(s16 *)(m + 0x20) = *(int *)(G + 0x48274);
        *(s16 *)(m + 0x24) = *(int *)(G + 0x48278);
        *(s16 *)(m + 0x22) = -0x2710;
        { char *hp8 = m + 0x20; LDV01(hp8); }
        RTPS_2NOP();
        { int k9 = 0x4A284;
      int i9 = *(int *)(G + 0x98);
      char *hp9 = G + (i9 * 0x28 + k9) + 8; STSXY1(hp9); }
        {
        char *g = G;
        { char *b = g + *(int *)(g + 0x98) * 0x28;
          u16 v = *(u16 *)(b + 0x4A28C) - 0x10;
          *(u16 *)(b + 0x4A28C) = v; *(u16 *)(b + 0x4A29C) = v; }
        { char *b = g + *(int *)(g + 0x98) * 0x28;
          u16 v = *(u16 *)(b + 0x4A28E) - 0x10;
          *(u16 *)(b + 0x4A28E) = v; *(u16 *)(b + 0x4A296) = v; }
        { char *b = g + *(int *)(g + 0x98) * 0x28;
          s16 v = *(u16 *)(b + 0x4A28C) + 0x20;
          *(s16 *)(b + 0x4A2A4) = v; *(s16 *)(b + 0x4A294) = v; }
        { char *b = g + *(int *)(g + 0x98) * 0x28;
          s16 v = *(u16 *)(b + 0x4A28E) + 0x20;
          *(s16 *)(b + 0x4A2A6) = v; *(s16 *)(b + 0x4A29E) = v; }
        }
    }
    { int pad[2]; __asm__("" :: "m"(pad[0])); }
}

extern void kaze_panel_frame_vertices(void *, void *, int, int, int, int);

/**
 * kaze_postevent_menu_anim  (menu, 0x80024e38)
 *
 * Post-event menu update/draw gated by state flag 0x40000000 and the
 * +0x48e0c phase counter; animates confirmation brightness ramps and
 * positions the confirm SPRT group from +0x48e18 anchors.
 */

void kaze_postevent_menu_anim(void)
{
  u8 bright;
  u16 anchor_y;
  int clamp_val;
  register int first_val asm("$5");
  int *counter_ptr;
  int ramp_val;
  register char *sprite_ptr asm("$7");
  register int first_i asm("$8");
  int second_i;
  register int second_off asm("$6");
  char *s1;
  char *s0;
  char *s2;

  s1 = kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x4918c);
  s0 = kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x50 + 0x49204);
  if ((*(uint *)(kaze_state_base_ptr + 0x14) & 0x40000000) != 0) {
    if (6 < *(int *)(kaze_state_base_ptr + 0x48e0c)) {
      kaze_minimap_radar_builder((int)*(short *)(kaze_state_base_ptr + 0x492c0),
                                 (int)*(short *)(kaze_state_base_ptr + 0x492c2));
    }
    if (*(int *)(kaze_state_base_ptr + 0x48e0c) - 4U < 6) {
      if (*(int *)(kaze_state_base_ptr + 0x48e0c) == 6) {
        char *pd = kaze_state_base_ptr;
        char *b50;
        register int first_off asm("$6");
        register int first_base asm("$9");
        first_i = 0;
        first_base = 0x492b0;
        b50 = pd + *(int *)(pd + 0x98) * 0x50;
        *(short *)(b50 + 0x4920c) =
             *(short *)(pd + 0x48e18) + -0x40;
        b50 = pd + *(int *)(pd + 0x98) * 0x50;
        *(short *)(b50 + 0x4920e) =
             *(short *)(pd + 0x48e1a) + -0x11;
        b50 = pd + *(int *)(pd + 0x98) * 0x50;
        *(short *)(b50 + 0x49220) =
             *(short *)(pd + 0x48e18) + -0x40;
        sprite_ptr = s0 + 0x18;
        b50 = pd + *(int *)(pd + 0x98) * 0x50;
        *(u16 *)(b50 + 0x49234) =
             *(u16 *)(pd + 0x48e18);
        first_off = 0;
        anchor_y = *(u16 *)(pd + 0x48e1a);
        b50 = pd + *(int *)(pd + 0x98) * 0x50;
        *(u16 *)(b50 + 0x49236) = anchor_y;
        *(u16 *)(b50 + 0x49222) = anchor_y;
        do {
          counter_ptr = (int *)(first_off + (int)kaze_state_base_ptr + first_base);
          ramp_val = *(int *)(kaze_state_base_ptr + 0xa4);
          first_val = *counter_ptr;
          if (*(int *)(kaze_state_base_ptr + 0x492b8) == first_i) {
            ramp_val = first_val + ramp_val * 6;
          }
          else {
            ramp_val = first_val - ramp_val * 6;
          }
          s2 = kaze_state_base_ptr;
          *counter_ptr = ramp_val;
          {
          register int *clamp_addr asm("$5");
          {
          register int clamp_mid asm("$2");
          clamp_mid = first_off + (int)s2;
          clamp_addr = (int *)(clamp_mid + first_base);
          }
          counter_ptr = clamp_addr;
          clamp_val = *clamp_addr;
          if (clamp_val < 0x40) {
            goto first_clamp_low;
          }
          if (clamp_val < 0x81) {
            ramp_val = clamp_val;
            goto first_clamp_done;
          }
          ramp_val = 0x80;
          goto first_clamp_done;
first_clamp_low:
          ramp_val = 0x40;
first_clamp_done:
          s2 = kaze_state_base_ptr;
          first_i = first_i + 1;
          *clamp_addr = ramp_val;
          }
          bright = *(u8 *)(first_off + (int)s2 + first_base);
          first_off = first_off + 4;
          sprite_ptr[2] = bright;
          sprite_ptr[1] = bright;
          *sprite_ptr = bright;
          sprite_ptr = sprite_ptr + 0x14;
        } while (first_i < 2);
      }
      kaze_panel_frame_vertices(
                kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x78 + 0x48fdc),
                kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x60 + 0x490cc),
                (int)(((uint)*(u16 *)(kaze_state_base_ptr + 0x48e18) -
                      ((int)*(short *)(kaze_state_base_ptr + 0x48e14) / 2)) * 0x10000) >> 0x10,
                (int)(((uint)*(u16 *)(kaze_state_base_ptr + 0x48e1a) -
                      ((int)*(short *)(kaze_state_base_ptr + 0x48e16) / 2)) * 0x10000) >> 0x10,
                (int)*(short *)(kaze_state_base_ptr + 0x48e14),
                (int)*(short *)(kaze_state_base_ptr + 0x48e16));
    }
    second_i = 0;
    if (*(int *)(kaze_state_base_ptr + 0x48e0c) - 3U < 7) {
      register int second_base asm("$9");
      second_base = 0x492a4;
      sprite_ptr = s1 + 4;
      second_off = 0;
      do {
        register int second_val asm("$5");
        counter_ptr = (int *)(second_off + (int)kaze_state_base_ptr + second_base);
        ramp_val = *(int *)(kaze_state_base_ptr + 0xa4);
        second_val = *counter_ptr;
        if (*(int *)(kaze_state_base_ptr + 0x492bc) == second_i) {
          ramp_val = second_val + ramp_val * 6;
        }
        else {
          ramp_val = second_val - ramp_val * 6;
        }
        s2 = kaze_state_base_ptr;
        *counter_ptr = ramp_val;
        {
        register int *clamp_addr2 asm("$5");
        register int second_clamp asm("$4");
        {
        register int clamp_mid2 asm("$2");
        clamp_mid2 = second_off + (int)s2;
        clamp_addr2 = (int *)(clamp_mid2 + second_base);
        }
        counter_ptr = clamp_addr2;
        clamp_val = *clamp_addr2;
        if (clamp_val < 0x40) {
          goto second_clamp_low;
        }
        if (clamp_val < 0x81) {
          second_clamp = clamp_val;
          goto second_clamp_done;
        }
        second_clamp = 0x80;
        goto second_clamp_done;
second_clamp_low:
        second_clamp = 0x40;
second_clamp_done:
        s2 = kaze_state_base_ptr;
        second_i = second_i + 1;
        *clamp_addr2 = second_clamp;
        }
        bright = *(u8 *)(second_off + (int)s2 + second_base);
        second_off = second_off + 4;
        sprite_ptr[2] = bright;
        sprite_ptr[1] = bright;
        *sprite_ptr = bright;
        sprite_ptr = sprite_ptr + 0x14;
      } while (second_i < 3);
    }
    kaze_panel_frame_vertices(
              kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x78 + 0x48e2c),
              kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x60 + 0x48f1c),
              (0xa0 - ((int)*(short *)(kaze_state_base_ptr + 0x48e10) / 2)) * 0x10000 >> 0x10,
              (0x78 - ((int)*(short *)(kaze_state_base_ptr + 0x48e12) / 2)) * 0x10000 >> 0x10,
              (int)*(short *)(kaze_state_base_ptr + 0x48e10),
              (int)*(short *)(kaze_state_base_ptr + 0x48e12));
  }
  return;
}

extern WL kaze_wind_layer0_dir[];

/**
 * kaze_hud_gauge_renderer  (hud, 0x8002536c)
 *
 * In-flight HUD renderer: gas/target/min/floor gauge bars (POLY_F4 slots at
 * +0x48c44/+0x48c74/+0x48c8c/+0x48ca4), wind compass cardinals (+0x48a94),
 * heading needle (+0x48bc4..), 5-slot wind-layer arrow panel (+0x489f0,
 * current layer bright; layer = -pos_y/0x76c - 3), and altitude marker
 * (+0x48b34).
 */

void kaze_hud_gauge_renderer(void)
{
  register char *panel asm("$21");
  int new_var;
  register int *wp asm("$20");
  register int hb asm("$17");
  int i;
  int a;
  int m;
  int tn;
  int r;
  {
    register char *bd asm("$4");
    register int ck asm("$5");
    register int pk asm("$2");
    bd = kaze_state_base_ptr;
    ck = 0x489ec;
    pk = ((*((int *) (bd + 0x98))) * 0x50) + ck;
    panel = bd + pk;
  }
  {
    register int i2 asm("$18");
    register int angle asm("$19");
    register char *b2 asm("$5");
    i = 0;
    i2 = 0;
    angle = 0x1000;
    do
    {
      a = (angle - (*((s16 *) (kaze_state_base_ptr + 0x4a)))) + 0x800;
      {
        register char *qa asm("$5");
        int r;
        int t;
        r = rsin(a);
        qa = kaze_state_base_ptr;
        t = (*((int *) (qa + 0x98))) * 0x50;
        qa = qa + (i2 + t);
        *((s16 *) (qa + 0x48a94)) = ((r * 0x12) / 0x1000) + 0x9b;
      }
      {
        register char *qb asm("$4");
        register int u asm("$2");
        int r;
        int t;
        int v;
        r = rcos(a);
        v = r * 0x12;
        b2 = kaze_state_base_ptr;
        t = (*((int *) (b2 + 0x98))) * 0x50;
        u = i2 + t;
        qb = b2 + u;
        *((s16 *) (qb + 0x48a96)) = (v / 0x1000) + 0x23;
      }
      i2 += 0x14;
      i++;
      angle += 0x400;
    }
    while (i < 4);
    {
      register int th asm("$2");
      register int wv asm("$4");
      th = 0x2000;
      wp = &kaze_balloon_angle;
      th -= *((s16 *) (b2 + 0x4a));
      wv = *wp;
      hb = th - wv;
    }
  }
  {
    register char *b3 asm("$4");
    a = hb + 0x1600;
    {
      register char *qn asm("$3");
      r = rsin(a);
      b3 = kaze_state_base_ptr;
      tn = (*((int *) (b3 + 0x98))) * 0x28;
      qn = b3 + tn;
      if (r < 0)
      {
        r += 0xff;
      }
      *((s16 *) (qn + 0x48bc4)) = 0xa0 - (r >> 8);
    }
    {
      register char *qn asm("$3");
      r = rcos(a);
      b3 = kaze_state_base_ptr;
      tn = (*((int *) (b3 + 0x98))) * 0x28;
      qn = b3 + tn;
      if (r < 0)
      {
        r += 0xff;
      }
      *((s16 *) (qn + 0x48bc6)) = 0x28 - (r >> 8);
    }
    a = hb + 0xa00;
    {
      register char *qn asm("$3");
      r = rsin(a);
      b3 = kaze_state_base_ptr;
      tn = (*((int *) (b3 + 0x98))) * 0x28;
      qn = b3 + tn;
      if (r < 0)
      {
        r += 0xff;
      }
      *((s16 *) (qn + 0x48bcc)) = 0xa0 - (r >> 8);
    }
    {
      register char *qn asm("$3");
      r = rcos(a);
      b3 = kaze_state_base_ptr;
      tn = (*((int *) (b3 + 0x98))) * 0x28;
      qn = b3 + tn;
      if (r < 0)
      {
        r += 0xff;
      }
      *((s16 *) (qn + 0x48bce)) = 0x28 - (r >> 8);
    }
    a = hb + 0x1200;
    {
      register char *qn asm("$3");
      r = rsin(a);
      b3 = kaze_state_base_ptr;
      tn = (*((int *) (b3 + 0x98))) * 0x28;
      qn = b3 + tn;
      if (r < 0)
      {
        r += 0xff;
      }
      *((s16 *) (qn + 0x48bd4)) = 0xa0 - (r >> 8);
    }
    {
      register char *qn asm("$3");
      r = rcos(a);
      b3 = kaze_state_base_ptr;
      tn = (*((int *) (b3 + 0x98))) * 0x28;
      qn = b3 + tn;
      if (r < 0)
      {
        r += 0xff;
      }
      *((s16 *) (qn + 0x48bd6)) = 0x28 - (r >> 8);
    }
    a = hb + 0xe00;
    {
      register char *qn asm("$3");
      r = rsin(a);
      b3 = kaze_state_base_ptr;
      tn = (*((int *) (b3 + 0x98))) * 0x28;
      qn = b3 + tn;
      if (r < 0)
      {
        r += 0xff;
      }
      *((s16 *) (qn + 0x48bdc)) = 0xa0 - (r >> 8);
    }
    {
      r = rcos(a);
      b3 = kaze_state_base_ptr;
      tn = (*((int *) (b3 + 0x98))) * 0x28;
      b3 = b3 + tn;
      if (r < 0)
      {
        r += 0xff;
      }
      *((s16 *) (b3 + 0x48bde)) = 0x28 - (r >> 8);
    }
  }
  {
    char *arrows;
    register int *pp asm("$10");
    int four;
    arrows = panel + 4;
    i = 0;
    four = 4;
    pp = wp - 0x21;
    for (; i < 5; i++)
    {
      u8 *p = (u8 *) (arrows + (i * 0x10));
      a = (((((*((s16 *) (kaze_state_base_ptr + 0x4a))) + (kaze_wind_layer0_dir[four - i].dir * 0x200)) + 0x80) & 0xfff) / 0x100) & 0xf;
      p[8] = (a % 8) << 4;
      p[9] = (a / 8) << 4;
      a = ((-(*pp)) / 0x76c) - 3;
      if (i == (four - a))
      {
        p[2] = 0x80;
        p[1] = 0x80;
        p[0] = 0x80;
      }
      else
      {
        p[2] = 0x60;
        p[1] = 0x60;
        p[0] = 0x60;
      }
    }

    asm("" : : "r"(panel));
  }
  {
    register char *b asm("$7");
    register int *gp asm("$8");
    b = kaze_state_base_ptr;
    gp = &kaze_gas;
    {
      register char *qg1 asm("$3");
      register int t asm("$2");
      int sa;
      t = (*((int *) (b + 0x98))) * 0x90;
      qg1 = b + t;
      sa = 0x124 - ((*gp) / 0x800);
      asm("");
      *((s16 *) (qg1 + 0x48c54)) = sa;
      *((s16 *) (qg1 + 0x48c4c)) = sa;
    }
    {
      register int vv asm("$2");
      register int pv asm("$4");
      char *qg;
      int t;
      {
        register int sb_ asm("$3");
        vv = kaze_target_gas;
        sb_ = 0x124 - (vv / 0xc80);
        pv = *((int *) (b + 0x98));
        asm("");
        t = pv * 0x90;
        qg = b + t;
        *((s16 *) (qg + 0x48c84)) = sb_;
        *((s16 *) (qg + 0x48c7c)) = sb_;
      }
      {
        register int sc_ asm("$3");
        vv = kaze_min_gas;
        sc_ = 0x124 - (vv / 0xc80);
        pv = *((int *) (b + 0x98));
        asm("");
        t = pv * 0x90;
        qg = b + t;
        *((s16 *) (qg + 0x48c9c)) = sc_;
        *((s16 *) (qg + 0x48c94)) = sc_;
      }
      {
        int sd_;
        vv = kaze_gas_floor;
        sd_ = 0x124 - (vv / 0xc80);
        pv = *((int *) (b + 0x98));
        asm("");
        t = pv * 0x90;
        qg = b + t;
        *((s16 *) (qg + 0x48cb4)) = sd_;
        *((s16 *) (qg + 0x48cac)) = sd_;
      }
    }
    if (kaze_vertical_penalty != 0)
    {
      *((u8 *) ((((*((int *) (b + 0x98))) * 0x90) + b) + 0x48c48)) = 0xff;
      *((u8 *) ((((*((int *) (kaze_state_base_ptr + 0x98))) * 0x90) + kaze_state_base_ptr) + 0x48c49)) = 0xff;
      *((u8 *) ((((*((int *) (kaze_state_base_ptr + 0x98))) * 0x90) + kaze_state_base_ptr) + 0x48c4a)) = 0xff;
    }
    else
    {
      *((u8 *) ((((*((int *) (b + 0x98))) * 0x90) + b) + 0x48c48)) = 0xff;
      {
        register int q2 asm("$3");
        register int tv asm("$2");
        register char *pb asm("$4");
        register int gv asm("$5");
        gv = *gp;
        pb = kaze_state_base_ptr;
        tv = (*((int *) (pb + 0x98))) * 0x90;
        q2 = tv + ((int) pb);
        if (gv < 0)
        {
          gv += 0x7ff;
        }
        {
          register int sv asm("$2");
          sv = gv >> 11;
          *((u8 *) (q2 + 0x48c49)) = sv;
        }
      }
      *((u8 *) ((((*((int *) (kaze_state_base_ptr + 0x98))) * 0x90) + kaze_state_base_ptr) + 0x48c4a)) = 0;
    }
  }
  *((u8 *) ((((*((int *) (kaze_state_base_ptr + 0x98))) * 0x90) + kaze_state_base_ptr) + 0x48c78)) = 0x80;
  *((u8 *) ((((*((int *) (kaze_state_base_ptr + 0x98))) * 0x90) + kaze_state_base_ptr) + 0x48c79)) = 0;
  *((u8 *) ((((*((int *) (kaze_state_base_ptr + 0x98))) * 0x90) + kaze_state_base_ptr) + 0x48c7a)) = 0;
  *((u8 *) ((((*((int *) (kaze_state_base_ptr + 0x98))) * 0x90) + kaze_state_base_ptr) + 0x48c90)) = 0;
  *((u8 *) ((((*((int *) (kaze_state_base_ptr + 0x98))) * 0x90) + kaze_state_base_ptr) + 0x48c91)) = 0x80;
  *((u8 *) ((((*((int *) (kaze_state_base_ptr + 0x98))) * 0x90) + kaze_state_base_ptr) + 0x48c92)) = 0x80;
  *((u8 *) ((((*((int *) (kaze_state_base_ptr + 0x98))) * 0x90) + kaze_state_base_ptr) + 0x48ca8)) = 0;
  *((u8 *) ((((*((int *) (kaze_state_base_ptr + 0x98))) * 0x90) + kaze_state_base_ptr) + 0x48ca9)) = 0x80;
  *((u8 *) ((((*((int *) (kaze_state_base_ptr + 0x98))) * 0x90) + kaze_state_base_ptr) + 0x48caa)) = 0;
  m = -kaze_pos_y;
  if ((m / 0x76c) < 3)
  {
    register char *qm asm("$2");
    register char *bt asm("$4");
    int t;
    bt = kaze_state_base_ptr;
    t = (*((int *) (bt + 0x98))) * 0x10;
    qm = bt + t;
    *((s16 *) (qm + 0x48b34)) = 0x258;
    t = (*((int *) (bt + 0x98))) * 0x10;
    bt = bt + t;
    *((s16 *) (bt + 0x48b36)) = 0;
  }
  else
  {
    char *qe;
    char *be;
    int t;
    int dv;
    a = m - 0x1644;
    dv = (a * 0x12) / 0x76c;
    new_var = 0x12a;
    be = kaze_state_base_ptr;
    t = (*((int *) (be + 0x98))) * 0x10;
    qe = be + t;
    *((s16 *) (qe + 0x48b34)) = new_var;
    t = (*((int *) (be + 0x98))) * 0x10;
    be = be + t;
    *((s16 *) (be + 0x48b36)) = 0xa6 - dv;
  }
}
