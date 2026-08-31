#include "types.h"
#include "kaze_syms.h"
extern int kaze_live_stage_level;
extern void SetBackColor(int, int, int);
extern void SetFarColor(int, int, int);
extern void SetLightMatrix(void *);
extern int rand(void);
extern int kaze_cd_read_sectors(void *, void *, int);
typedef struct { int w[6]; } CdFileInfo;
typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} SVECTOR;
typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 cd;
} CVECTOR;
typedef struct { char b[4]; } U4;
typedef struct { short h[3]; } U6;
typedef struct { int w[54]; } Blk54;
typedef struct { int w[12]; } Blk12;

extern int kaze_stage_index;
extern int kaze_stage_mode;
extern int kaze_stage_variant;
extern int CdPosToInt(void *);
extern void CdIntToPos(int, void *);
extern void kaze_tmd_model_relocate_3462c(uint);
extern void bzero(void *, int);

/**
 * kaze_stage_map_cd_loader  (cd_stream, 0x8001a3bc)
 *
 * Per-stage CD map loader: MAPDATA.DAT one 0x91000-byte block at
 * +stage_index*0x122 sectors, TEXDATA.DAT twelve 0x20-sector chunks at
 * stage_index*0xd80+(stage_level-1)*0x480+stage_mode*0x180, RMODE.DAT two
 * 0x16-sector chunks at (stage_variant-1)*0x2c; rebuilds the +0xecb28
 * terrain/model pointer table from the +0x6b6e0 MAPDATA header.
 */

void kaze_stage_map_cd_loader(void)
{
    extern char *kaze_state_base_ptr;
    int sector_pos;
    int status;
    uint model_i;
    CdFileInfo cd_pos;
    int pad[2];
    int i;
    int *variant_ptr;
    int base;
    int n;
    int t;

    __asm__("" :: "m"(pad[0]));
    i = 0;
    sector_pos = CdPosToInt((void *)((int)kaze_state_base_ptr + 0x4823c));
    sector_pos = sector_pos + kaze_stage_index * 0xd80 + (kaze_live_stage_level + -1) * 0x480 + kaze_stage_mode * 0x180;
    for (; i < 0xc; sector_pos += 0x20, i++) {
        cd_pos = *(CdFileInfo *)((int)kaze_state_base_ptr + 0x4823c);
        CdIntToPos(sector_pos, &cd_pos);
        do {
            status = kaze_cd_read_sectors(&cd_pos, (void *)((int)kaze_state_base_ptr + 0x5b6e0), 0x10000);
        } while (status < 0);
        kaze_cd_read_postprocess((void *)((int)kaze_state_base_ptr + 0x5b6e0), -1, -1, -1, -1, 1);
    }
    variant_ptr = &kaze_stage_variant;
    if (*variant_ptr != 0) {
        status = 0x48254;
        i = 0;
        sector_pos = CdPosToInt((void *)((int)kaze_state_base_ptr + status));
        sector_pos = sector_pos + (*variant_ptr + -1) * 0x2c;
        for (; i < 2; sector_pos += 0x16, i++) {
            cd_pos = *(CdFileInfo *)((int)kaze_state_base_ptr + 0x48254);
            CdIntToPos(sector_pos, &cd_pos);
            do {
                status = kaze_cd_read_sectors(&cd_pos, (void *)((int)kaze_state_base_ptr + 0x5b6e0), 0xb000);
            } while (status < 0);
            kaze_cd_read_postprocess((void *)((int)kaze_state_base_ptr + 0x5b6e0), -1, -1, -1, -1, 1);
        }
        do { do { do {
            __asm__ volatile("" : : "r"(i));
            __asm__ volatile("" : : "r"(i));
            __asm__ volatile("" : : "r"(i));
            __asm__ volatile("" : : "r"(i));
            __asm__ volatile("" : : "r"(i));
            __asm__ volatile("" : : "r"(i));
        } while (0); } while (0); } while (0);
    }
    cd_pos = *(CdFileInfo *)((int)kaze_state_base_ptr + 0x48224);
    sector_pos = CdPosToInt((void *)((int)kaze_state_base_ptr + 0x48224));
    CdIntToPos(sector_pos + kaze_stage_index * 0x122, &cd_pos);
    do {
        sector_pos = kaze_cd_read_sectors(&cd_pos, (void *)((int)kaze_state_base_ptr + 0x5b6e0), 0x91000);
    } while (sector_pos < 0);
    {
        register int a0v __asm__("$4") = 0xe0000;
        register int basev __asm__("$2") = (int)kaze_state_base_ptr;
        __asm__("" : "=r"(a0v) : "0"(a0v), "r"(basev));
        a0v |= 0xcb28;
        bzero((void *)(basev + a0v), 0x800);
    }
    base = (int)kaze_state_base_ptr;
    model_i = 0;
    t = *(int *)(base + 0x6b6e0);
    if (t != 0) {
        do {
            if ((int)model_i >= 0x200) {
                return;
            }
            kaze_tmd_model_relocate_3462c(
                ((uint *)(base + 0xecb28))[model_i] =
                    base + (n = ((((uint *)(base + 0x6b6e4))[model_i] >> 2) << 2) + 0x6b6e0));
            model_i = model_i + 1;
            base = (int)kaze_state_base_ptr;
        } while (model_i < *(uint *)(base + 0x6b6e0));
    }
    return;
}

extern SVECTOR kaze_default_normal_vector;
void kaze_light_matrix_defaults();
void kaze_environment_color_update();
void kaze_sun_angle_light_update();
void kaze_environment_poly_refresh();
extern void SetColorMatrix(void *);
extern void NormalColor(SVECTOR *, CVECTOR *);

/**
 * kaze_lighting_env_frame_update  (render, 0x8001a7b4)
 *
 * Per-frame lighting pass: SetBackColor/SetFarColor, light-matrix defaults,
 * environment color update, sun-angle update, env poly refresh,
 * SetColorMatrix/SetLightMatrix (+0x48198/+0x481b8), NormalColor into
 * +0x481d8..e0.
 */

void kaze_lighting_env_frame_update(void)
{
    extern char *kaze_state_base_ptr;
    SVECTOR vec;
    CVECTOR color[2];
    char *base;

    vec = kaze_default_normal_vector;
    SetBackColor(0x30, 0x30, 0x30);
    SetFarColor(0xFF, 0xFF, 0xFF);
    kaze_light_matrix_defaults();
    kaze_environment_color_update();
    kaze_sun_angle_light_update();
    kaze_environment_poly_refresh();
    SetColorMatrix(kaze_state_base_ptr + 0x48198);
    SetLightMatrix(kaze_state_base_ptr + 0x481B8);
    NormalColor(&vec, color);
    base = kaze_state_base_ptr;
    *(int *)(kaze_state_base_ptr + 0x481D8) = color[0].r << 4;
    *(int *)(base + 0x481DC) = color[0].g << 4;
    *(int *)(base + 0x481E0) = color[0].b << 4;
}

/**
 * kaze_light_matrix_defaults  (render, 0x8001a8bc)
 *
 * Writes the default light matrix at live-root +0x481b8 (rows
 * 0xf000,0xf000,0 / 0x1000,0x1000,0 / 0,0x1000,0) and the identity color
 * matrix at +0x48198.
 */

void kaze_light_matrix_defaults(void)
{
    extern int kaze_state_base_ptr;
    int p = kaze_state_base_ptr;
    *(s16 *)(p + 0x481B8) = -0x1000;
    *(s16 *)(p + 0x481BA) = -0x1000;
    *(s16 *)(p + 0x481BC) = 0;
    *(s16 *)(p + 0x481BE) = 0x1000;
    *(s16 *)(p + 0x481C0) = 0x1000;
    *(s16 *)(p + 0x481C2) = 0;
    *(s16 *)(p + 0x481C4) = 0;
    *(s16 *)(p + 0x481C6) = 0x1000;
    *(s16 *)(p + 0x481C8) = 0;
    *(s16 *)(p + 0x48198) = 0x1000;
    *(s16 *)(p + 0x4819E) = 0x1000;
    *(s16 *)(p + 0x481A4) = 0x1000;
    *(s16 *)(p + 0x4819A) = 0;
    *(s16 *)(p + 0x481A0) = 0;
    *(s16 *)(p + 0x481A6) = 0;
    *(s16 *)(p + 0x4819C) = 0;
    *(s16 *)(p + 0x481A2) = 0;
    *(s16 *)(p + 0x481A8) = 0;
}

extern int kaze_stage_flags;
extern int rsin(int);
extern int rcos(int);

/**
 * kaze_environment_color_update  (render, 0x8001a9ac)
 *
 * Environment/sky color source: writes the environment color sets
 * (+0x4828e..+0x4829e), effect intensity (+0x4827c), light matrix terms
 * (+0x481b8..), and byte-splices the base RGB triple at +0x48288..+0x4828c
 * from the +0x482ac/+0x482b0 lookup.
 */

void kaze_environment_color_update(void){
    extern char *kaze_state_base_ptr;
    char *base; char *base2; char *lut; int idx; int t; s16 sv; int v;
    if (kaze_live_stage_level == 1) goto L1;
    if (kaze_live_stage_level < 2) goto Ldef;
    if (kaze_live_stage_level == 2) goto L2;
    if (kaze_live_stage_level == 3) goto L3;
    goto Ldef;
L1:
    *(int *)(kaze_state_base_ptr + 0x4826C) = rsin(0x600);
    t = rcos(0x600); base = kaze_state_base_ptr;
    *(int *)(base + 0x48270) = t; *(int *)(base + 0x4827C) = 0;
    t = kaze_stage_flags & 1; sv = 0x1000; if (t != 0) sv = 0x800;
    idx = 1; if (t != 0) idx = 5;
    v = sv;
    *(s16 *)(base + 0x481A4) = v; *(s16 *)(base + 0x4819E) = v; *(s16 *)(base + 0x48198) = v;
    v = 0xFF;
    *(s16 *)(base + 0x48292) = v; *(s16 *)(base + 0x48290) = v; *(s16 *)(base + 0x4828E) = v;
    v = 0x200; *(int *)(base + 0x481E8) = v;
    v = 0x80; *(int *)(base + 0x481EC) = v; *(int *)(base + 0x481F0) = v;
    *(s16 *)(base + 0x48298) = 0; *(s16 *)(base + 0x48296) = 0; *(s16 *)(base + 0x48294) = 0;
    v = 0x10000000; goto ae3c;
L2:
    *(int *)(kaze_state_base_ptr + 0x4826C) = rsin(0xB00);
    t = rcos(0xB00); base = kaze_state_base_ptr;
    *(int *)(base + 0x48270) = t; *(int *)(base + 0x4827C) = 0; idx = 2;
    if ((kaze_stage_flags & 1) != 0) { *(s16 *)(base + 0x48198) = 0xD80; v = 0x900; }
    else { *(s16 *)(base + 0x48198) = 0xF00; v = 0x800; }
    *(s16 *)(base + 0x481A4) = v; *(s16 *)(base + 0x4819E) = v; base = kaze_state_base_ptr;
    v = 0xFF;
    *(s16 *)(base + 0x48292) = v; *(s16 *)(base + 0x48290) = v; *(s16 *)(base + 0x4828E) = v;
    v = 0xC0;
    *(s16 *)(base + 0x4829E) = v; *(s16 *)(base + 0x4829C) = v;
    v = 0x400; *(int *)(base + 0x481E8) = v;
    v = 0x80; *(int *)(base + 0x481EC) = v; *(int *)(base + 0x481F0) = v;
    *(s16 *)(base + 0x48298) = 0; *(s16 *)(base + 0x48296) = 0; *(s16 *)(base + 0x48294) = 0;
    v = 0x20000000; goto ae60;
L3:
    *(int *)(kaze_state_base_ptr + 0x4826C) = rsin(0);
    v = rcos(0);
    __asm__("" : "=r"(v) : "0"(v));
    base = kaze_state_base_ptr; idx = 3;
    *(int *)(base + 0x48270) = v;
    v = 0xFF;
    *(int *)(base + 0x4827C) = v;
    __asm__ volatile("" : "=r"(v) : "0"(v));
    v = 0xFF;
    *(s16 *)(base + 0x48298) = v; *(s16 *)(base + 0x48296) = v; *(s16 *)(base + 0x48294) = v;
    v = 0x1F;
    *(s16 *)(base + 0x48292) = v; *(s16 *)(base + 0x48290) = v; *(s16 *)(base + 0x4828E) = v;
    v = 0x400; *(int *)(base + 0x481E8) = v;
    v = 0x80; *(int *)(base + 0x481EC) = v; *(int *)(base + 0x481F0) = v;
    *(s16 *)(base + 0x481A4) = 0; *(s16 *)(base + 0x4819E) = 0; *(s16 *)(base + 0x48198) = 0;
    v = 0x20000000; goto ae3c;
Ldef:
    *(int *)(kaze_state_base_ptr + 0x4826C) = rsin(0x400);
    t = rcos(0x400); base = kaze_state_base_ptr;
    *(int *)(base + 0x48270) = t; *(int *)(base + 0x4827C) = 0;
    t = kaze_stage_flags & 1; sv = 0x800; if (t != 0) sv = 0x400; idx = t << 2;
    v = sv;
    *(s16 *)(base + 0x481A4) = v; *(s16 *)(base + 0x4819E) = v; *(s16 *)(base + 0x48198) = v;
    v = 0xFF;
    *(s16 *)(base + 0x48298) = v; *(s16 *)(base + 0x48296) = v; *(s16 *)(base + 0x48294) = v;
    *(s16 *)(base + 0x48292) = v; *(s16 *)(base + 0x48290) = v; *(s16 *)(base + 0x4828E) = v;
    v = 0x400; *(int *)(base + 0x481E8) = v;
    v = 0x80; *(int *)(base + 0x481EC) = v; *(int *)(base + 0x481F0) = v;
    v = 0x10000000;
ae3c:
    *(s16 *)(base + 0x4829E) = 0; *(s16 *)(base + 0x4829C) = 0; *(s16 *)(base + 0x4829A) = 0;
ae60:
    *(int *)(base + 0x509DC) = v;
    base2 = kaze_state_base_ptr;
    *(s16 *)(base2 + 0x481BC) = -(*(int *)(base2 + 0x48270) / 2);
    *(s16 *)(base2 + 0x481BA) = *(int *)(base2 + 0x48270);
    *(s16 *)(base2 + 0x481B8) = -*(int *)(base2 + 0x4826C);
    lut = (char *)(idx * 6 + (int)base2);
    *(U6 *)(base2 + 0x48288) = *(U6 *)(lut + 0x482AC);
}

/**
 * kaze_sun_angle_light_update  (render, 0x8001af38)
 *
 * Sun direction from stage level kaze_live_stage_level (rsin/rcos of
 * level<<10 into light matrix +0x481be/+0x481c2, factors +0x48274/78),
 * stage-3 ambient boosts, random UV/brightness bytes for +0x4a288..+0x4a2d1
 * sprites.
 */

void kaze_sun_angle_light_update(void)
{
    extern unsigned char *kaze_state_base_ptr;
    unsigned char *p1, *p2, *p4, *p5, *p6, *p7;
    register unsigned char *p3 asm("$6");
    int lvl;
    int ang;
    short s;
    int t;
    int r, q, a0v;
    int hi;
    unsigned char b1, b2, b6, b7, b8;
    int ei;

    lvl = kaze_live_stage_level;
    ang = lvl << 10;
    *(short *)(kaze_state_base_ptr + 0x481c0) = -0x2710;
    s = rsin(ang);
    p1 = kaze_state_base_ptr;
    t = s * 32000;
    *(short *)(p1 + 0x481be) = s;
    if (t < 0) t += 0xfff;
    *(int *)(p1 + 0x48274) = t >> 12;
    s = rcos(ang);
    p2 = kaze_state_base_ptr;
    t = s * 32000;
    *(short *)(p2 + 0x481c2) = s;
    if (t < 0) t += 0xfff;
    *(int *)(p2 + 0x48278) = t >> 12;
    if (lvl == 3) {
        if (kaze_stage_flags == 0) {
            *(short *)(p2 + 0x4819a) = 0x200;
            *(short *)(p2 + 0x481a0) = 0x200;
            *(short *)(p2 + 0x481a6) = 0x500;
        } else {
            *(short *)(p2 + 0x4819a) = 0x100;
            *(short *)(p2 + 0x481a0) = 0x100;
            *(short *)(p2 + 0x481a6) = 0x280;
        }
    } else {
        *(short *)(p2 + 0x481a6) = 0;
        *(short *)(p2 + 0x481a0) = 0;
        *(short *)(p2 + 0x4819a) = 0;
    }
    r = rand() % 0xe;
    p3 = kaze_state_base_ptr;
    a0v = r;
    if (r < 0) a0v = r + 7;
    q = a0v >> 3;
    b1 = (r - q * 8) << 5;
    b2 = b1 | 0x1f;
    p3[0x4a2c8] = b1;
    p3[0x4a2b8] = b1;
    p3[0x4a2a0] = b1;
    p3[0x4a290] = b1;
    p4 = kaze_state_base_ptr;
    hi = q << 5;
    p4[0x4a2d0] = b2;
    p4[0x4a2c0] = b2;
    p4[0x4a2a8] = b2;
    p4[0x4a298] = b2;
    p5 = kaze_state_base_ptr;
    b6 = hi + 0x40;
    p5[0x4a2c1] = b6;
    p5[0x4a2b9] = b6;
    p5[0x4a299] = b6;
    p5[0x4a291] = b6;
    p6 = kaze_state_base_ptr;
    b7 = hi + 0x5f;
    p6[0x4a2d1] = b7;
    p6[0x4a2c9] = b7;
    p6[0x4a2a9] = b7;
    p6[0x4a2a1] = b7;
    __asm__ volatile("" :: "r"(hi));
    p7 = kaze_state_base_ptr;
    ei = *(int *)(p7 + 0x4827c);
    b8 = (unsigned int)(ei + ((unsigned int)ei >> 31)) >> 1;
    p7[0x4a2b2] = b8;
    p7[0x4a2b1] = b8;
    p7[0x4a2b0] = b8;
    p7[0x4a28a] = b8;
    p7[0x4a289] = b8;
    p7[0x4a288] = b8;
}

#define SPLOAD(pp, off) do { \
    (pp) = (char *)0x1f800000; \
    __asm__("" : "=r"(pp) : "0"(pp)); \
    (pp) = *(char **)((pp) + (off)); \
} while (0)
#define SPSTORE(val, off) (*(char **)(0x1f800000 + (off)) = (val))

/**
 * kaze_environment_poly_refresh  (render, 0x8001b288)
 *
 * Per-frame refresh of the environment POLY_G4 blocks
 * (+0x482f4/+0x48354/+0x48504) from the environment color sets via GTE
 * scratchpad pointers.
 */

void kaze_environment_poly_refresh(void)
{
    extern char *kaze_state_base_ptr;
    char *a0;

    { register char *r2 asm("$2"); register char *r5 asm("$5");
    r2 = kaze_state_base_ptr + 0x48354;
    a0 = kaze_state_base_ptr + 0x48504;
    r5 = kaze_state_base_ptr + 0x482F4;
    SPSTORE(r2, 0x48);
    SPSTORE(a0, 0x4c);
    SPSTORE(r5, 0x50); }
    *(u8*)(kaze_state_base_ptr + 0x482F8) = *(u16*)(kaze_state_base_ptr + 0x48288);
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); SPLOAD(p, 0x50); *(u8*)(p + 5) = *(u16*)(bp + 0x4828A); }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); SPLOAD(p, 0x50); *(u8*)(p + 6) = *(u16*)(bp + 0x4828C); }
    { char *p; SPLOAD(p, 0x50); *(u8*)(p + 0x1C) = 0; }
    { char *p; SPLOAD(p, 0x50); *(u8*)(p + 0x1D) = 0; }
    { char *p; SPLOAD(p, 0x50); *(u8*)(p + 0x1E) = 0; }
    { char *p; SPLOAD(p, 0x48);
    *(u8*)(p + 0x9C) = 0;
    *(u8*)(p + 0x94) = 0;
    *(u8*)(p + 0x54) = 0;
    *(u8*)(p + 0x4C) = 0;
    *(u8*)(p + 0xC) = 0;
    *(u8*)(p + 0x4) = 0;
    }
    { char *p; SPLOAD(p, 0x48);
    *(u8*)(p + 0x9D) = 0;
    *(u8*)(p + 0x95) = 0;
    *(u8*)(p + 0x55) = 0;
    *(u8*)(p + 0x4D) = 0;
    *(u8*)(p + 0xD) = 0;
    *(u8*)(p + 0x5) = 0;
    }
    { char *p; SPLOAD(p, 0x48);
    *(u8*)(p + 0x9E) = 0;
    *(u8*)(p + 0x96) = 0;
    *(u8*)(p + 0x56) = 0;
    *(u8*)(p + 0x4E) = 0;
    *(u8*)(p + 0xE) = 0;
    *(u8*)(p + 0x6) = 0;
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x48); c = *(u8*)(bp + 0x4828E);
    *(u8*)(p + 0x40) = c; 
    *(u8*)(p + 0x38) = c; 
    *(u8*)(p + 0x30) = c; 
    *(u8*)(p + 0x28) = c; 
    *(u8*)(p + 0x1C) = c; 
    *(u8*)(p + 0x14) = c; 
    }
    { char *p; u16 u = *(u16*)(kaze_state_base_ptr + 0x48294); int sh = (int)u << 16; int s = sh >> 16; unsigned int t = s + ((unsigned)sh >> 31); u8 h;
    SPLOAD(p, 0x48); h = t >> 1;
    *(u8*)(p + 0x88) = h; 
    *(u8*)(p + 0x80) = h; 
    *(u8*)(p + 0x78) = h; 
    *(u8*)(p + 0x70) = h; 
    *(u8*)(p + 0x64) = h; 
    *(u8*)(p + 0x5C) = h; 
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x48); c = *(u8*)(bp + 0x4829A);
    *(u8*)(p + 0xD0) = c; 
    *(u8*)(p + 0xC8) = c; 
    *(u8*)(p + 0xC0) = c; 
    *(u8*)(p + 0xB8) = c; 
    *(u8*)(p + 0xAC) = c; 
    *(u8*)(p + 0xA4) = c; 
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x48); c = *(u8*)(bp + 0x48290);
    *(u8*)(p + 0x41) = c; 
    *(u8*)(p + 0x39) = c; 
    *(u8*)(p + 0x31) = c; 
    *(u8*)(p + 0x29) = c; 
    *(u8*)(p + 0x1D) = c; 
    *(u8*)(p + 0x15) = c; 
    }
    { char *p; u16 u = *(u16*)(kaze_state_base_ptr + 0x48296); int sh = (int)u << 16; int s = sh >> 16; unsigned int t = s + ((unsigned)sh >> 31); u8 h;
    SPLOAD(p, 0x48); h = t >> 1;
    *(u8*)(p + 0x89) = h; 
    *(u8*)(p + 0x81) = h; 
    *(u8*)(p + 0x79) = h; 
    *(u8*)(p + 0x71) = h; 
    *(u8*)(p + 0x65) = h; 
    *(u8*)(p + 0x5D) = h; 
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x48); c = *(u8*)(bp + 0x4829C);
    *(u8*)(p + 0xD1) = c; 
    *(u8*)(p + 0xC9) = c; 
    *(u8*)(p + 0xC1) = c; 
    *(u8*)(p + 0xB9) = c; 
    *(u8*)(p + 0xAD) = c; 
    *(u8*)(p + 0xA5) = c; 
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x48); c = *(u8*)(bp + 0x48292);
    *(u8*)(p + 0x42) = c; 
    *(u8*)(p + 0x3A) = c; 
    *(u8*)(p + 0x32) = c; 
    *(u8*)(p + 0x2A) = c; 
    *(u8*)(p + 0x1E) = c; 
    *(u8*)(p + 0x16) = c; 
    }
    { char *p; u16 u = *(u16*)(kaze_state_base_ptr + 0x48298); int sh = (int)u << 16; int s = sh >> 16; unsigned int t = s + ((unsigned)sh >> 31); u8 h;
    SPLOAD(p, 0x48); h = t >> 1;
    *(u8*)(p + 0x8A) = h; 
    *(u8*)(p + 0x82) = h; 
    *(u8*)(p + 0x7A) = h; 
    *(u8*)(p + 0x72) = h; 
    *(u8*)(p + 0x66) = h; 
    *(u8*)(p + 0x5E) = h; 
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x48); c = *(u8*)(bp + 0x4829E);
    *(u8*)(p + 0xD2) = c; 
    *(u8*)(p + 0xCA) = c; 
    *(u8*)(p + 0xC2) = c; 
    *(u8*)(p + 0xBA) = c; 
    *(u8*)(p + 0xAE) = c; 
    *(u8*)(p + 0xA6) = c; 
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x4c); c = *(u8*)(bp + 0x4828E);
    *(u8*)(p + 0xC) = c; 
    *(u8*)(p + 0x4) = c; 
    }
    { char *p; u16 u = *(u16*)(kaze_state_base_ptr + 0x4828E); int sh = (int)u << 16; int s = sh >> 16; unsigned int t = s + ((unsigned)sh >> 31); u8 h;
    SPLOAD(p, 0x4c); h = t >> 1;
    *(u8*)(p + 0x30) = h; 
    *(u8*)(p + 0x28) = h; 
    *(u8*)(p + 0x1C) = h; 
    *(u8*)(p + 0x14) = h; 
    }
    { char *p; SPLOAD(p, 0x4c);
    *(u8*)(p + 0x40) = 0;
    *(u8*)(p + 0x38) = 0;
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x4c); c = *(u8*)(bp + 0x48294);
    *(u8*)(p + 0x54) = c; 
    *(u8*)(p + 0x4C) = c; 
    }
    { char *p; u16 u = *(u16*)(kaze_state_base_ptr + 0x48294); int sh = (int)u << 16; int s = sh >> 16; unsigned int t = s + ((unsigned)sh >> 31); u8 h;
    SPLOAD(p, 0x4c); h = t >> 1;
    *(u8*)(p + 0x78) = h; 
    *(u8*)(p + 0x70) = h; 
    *(u8*)(p + 0x64) = h; 
    *(u8*)(p + 0x5C) = h; 
    }
    { char *p; SPLOAD(p, 0x4c);
    *(u8*)(p + 0x88) = 0;
    *(u8*)(p + 0x80) = 0;
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x4c); c = *(u8*)(bp + 0x4829A);
    *(u8*)(p + 0x9C) = c; 
    *(u8*)(p + 0x94) = c; 
    }
    { char *p; u16 u = *(u16*)(kaze_state_base_ptr + 0x4829A); int sh = (int)u << 16; int s = sh >> 16; unsigned int t = s + ((unsigned)sh >> 31); u8 h;
    SPLOAD(p, 0x4c); h = t >> 1;
    *(u8*)(p + 0xC0) = h; 
    *(u8*)(p + 0xB8) = h; 
    *(u8*)(p + 0xAC) = h; 
    *(u8*)(p + 0xA4) = h; 
    }
    { char *p; SPLOAD(p, 0x4c);
    *(u8*)(p + 0xD0) = 0;
    *(u8*)(p + 0xC8) = 0;
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x4c); c = *(u8*)(bp + 0x48290);
    *(u8*)(p + 0xD) = c; 
    *(u8*)(p + 0x5) = c; 
    }
    { char *p; u16 u = *(u16*)(kaze_state_base_ptr + 0x48290); int sh = (int)u << 16; int s = sh >> 16; unsigned int t = s + ((unsigned)sh >> 31); u8 h;
    SPLOAD(p, 0x4c); h = t >> 1;
    *(u8*)(p + 0x31) = h; 
    *(u8*)(p + 0x29) = h; 
    *(u8*)(p + 0x1D) = h; 
    *(u8*)(p + 0x15) = h; 
    }
    { char *p; SPLOAD(p, 0x4c);
    *(u8*)(p + 0x41) = 0;
    *(u8*)(p + 0x39) = 0;
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x4c); c = *(u8*)(bp + 0x48296);
    *(u8*)(p + 0x55) = c; 
    *(u8*)(p + 0x4D) = c; 
    }
    { char *p; u16 u = *(u16*)(kaze_state_base_ptr + 0x48296); int sh = (int)u << 16; int s = sh >> 16; unsigned int t = s + ((unsigned)sh >> 31); u8 h;
    SPLOAD(p, 0x4c); h = t >> 1;
    *(u8*)(p + 0x79) = h; 
    *(u8*)(p + 0x71) = h; 
    *(u8*)(p + 0x65) = h; 
    *(u8*)(p + 0x5D) = h; 
    }
    { char *p; SPLOAD(p, 0x4c);
    *(u8*)(p + 0x89) = 0;
    *(u8*)(p + 0x81) = 0;
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x4c); c = *(u8*)(bp + 0x4829C);
    *(u8*)(p + 0x9D) = c; 
    *(u8*)(p + 0x95) = c; 
    }
    { char *p; u16 u = *(u16*)(kaze_state_base_ptr + 0x4829C); int sh = (int)u << 16; int s = sh >> 16; unsigned int t = s + ((unsigned)sh >> 31); u8 h;
    SPLOAD(p, 0x4c); h = t >> 1;
    *(u8*)(p + 0xC1) = h; 
    *(u8*)(p + 0xB9) = h; 
    *(u8*)(p + 0xAD) = h; 
    *(u8*)(p + 0xA5) = h; 
    }
    { char *p; SPLOAD(p, 0x4c);
    *(u8*)(p + 0xD1) = 0;
    *(u8*)(p + 0xC9) = 0;
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x4c); c = *(u8*)(bp + 0x48292);
    *(u8*)(p + 0xE) = c; 
    *(u8*)(p + 0x6) = c; 
    }
    { char *p; u16 u = *(u16*)(kaze_state_base_ptr + 0x48292); int sh = (int)u << 16; int s = sh >> 16; unsigned int t = s + ((unsigned)sh >> 31); u8 h;
    SPLOAD(p, 0x4c); h = t >> 1;
    *(u8*)(p + 0x32) = h; 
    *(u8*)(p + 0x2A) = h; 
    *(u8*)(p + 0x1E) = h; 
    *(u8*)(p + 0x16) = h; 
    }
    { char *p; SPLOAD(p, 0x4c);
    *(u8*)(p + 0x42) = 0;
    *(u8*)(p + 0x3A) = 0;
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x4c); c = *(u8*)(bp + 0x48298);
    *(u8*)(p + 0x56) = c; 
    *(u8*)(p + 0x4E) = c; 
    }
    { char *p; u16 u = *(u16*)(kaze_state_base_ptr + 0x48298); int sh = (int)u << 16; int s = sh >> 16; unsigned int t = s + ((unsigned)sh >> 31); u8 h;
    SPLOAD(p, 0x4c); h = t >> 1;
    *(u8*)(p + 0x7A) = h; 
    *(u8*)(p + 0x72) = h; 
    *(u8*)(p + 0x66) = h; 
    *(u8*)(p + 0x5E) = h; 
    }
    { char *p; SPLOAD(p, 0x4c);
    *(u8*)(p + 0x8A) = 0;
    *(u8*)(p + 0x82) = 0;
    }
    { char *bp = kaze_state_base_ptr; register char *p __asm__("$3"); u8 c; SPLOAD(p, 0x4c); c = *(u8*)(bp + 0x4829E);
    *(u8*)(p + 0x9E) = c; 
    *(u8*)(p + 0x96) = c; 
    }
    { char *p; u16 u = *(u16*)(kaze_state_base_ptr + 0x4829E); int sh = (int)u << 16; int s = sh >> 16; unsigned int t = s + ((unsigned)sh >> 31); u8 h;
    SPLOAD(p, 0x4c); h = t >> 1;
    *(u8*)(p + 0xC2) = h; 
    *(u8*)(p + 0xBA) = h; 
    *(u8*)(p + 0xAE) = h; 
    *(u8*)(p + 0xA6) = h; 
    }
    { register int copy1_off __asm__("$3");
    { char *p; SPLOAD(p, 0x4c);
    copy1_off = 0x40000;
    __asm__("" :: "r"(copy1_off));
    *(u8*)(p + 0xD2) = 0;
    *(u8*)(p + 0xCA) = 0;
    }
    copy1_off |= 0x842c;
    *(Blk54*)(kaze_state_base_ptr + copy1_off) = *(Blk54*)(kaze_state_base_ptr + 0x48354);
    }
    *(Blk54*)(kaze_state_base_ptr + 0x485DC) = *(Blk54*)(kaze_state_base_ptr + 0x48504);
    *(Blk12*)(kaze_state_base_ptr + 0x48324) = *(Blk12*)(kaze_state_base_ptr + 0x482F4);
}
