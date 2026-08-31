
#include "types.h"

extern char *kaze_state_base_ptr;
extern void AddPrim(void *, void *);
void kaze_marker_brightness_anim();

/**
 * kaze_postevent_bg_draw  (render, 0x80029c14)
 *
 * Post-event background pass gated by the +0x48e0c phase counter; AddPrims
 * background POLY strips and the +0x48e1c TPAGE.
 */

void kaze_postevent_bg_draw(void *ot)
{
    char *p;
    char *p_00;
    int i;
    int unused0;

    p_00 = kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x3c + 0x4918c);
    p = kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x50 + 0x49204) + 0x14;
    if ((*(uint *)(kaze_state_base_ptr + 0x14) & 0x40000000) != 0) {
        if (6 < *(int *)(kaze_state_base_ptr + 0x48e0c)) {
            kaze_marker_brightness_anim(ot);
        }
        if (*(int *)(kaze_state_base_ptr + 0x48e0c) - 4U < 6) {
            if (*(int *)(kaze_state_base_ptr + 0x48e0c) == 6) {
                i = 0;
                do {
                    AddPrim(ot, p);
                    p = p + 0x14;
                    i = i + 1;
                } while (i < 2);
                AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x50 + 0x49204));
            }
            if (*(int *)(kaze_state_base_ptr + 0x48e0c) < 7) {
                i = 3;
                do {
                    AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x60 + 0x490cc) + i * 0x18);
                    i = i + -1;
                } while (-1 < i);
                i = 4;
                do {
                    AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x78 + 0x48fdc) + i * 0x18);
                    i = i + -1;
                } while (-1 < i);
            }
        }
        if (*(int *)(kaze_state_base_ptr + 0x48e0c) - 3U < 7) {
            i = 0;
            do {
                AddPrim(ot, p_00);
                i = i + 1;
                p_00 = p_00 + 0x14;
            } while (i < 3);
        }
        i = 3;
        if (*(int *)(kaze_state_base_ptr + 0x48e0c) < 10) {
            do {
                AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x60 + 0x48f1c) + i * 0x18);
                i = i + -1;
            } while (-1 < i);
            i = 4;
            do {
                AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x78 + 0x48e2c) + i * 0x18);
                i = i + -1;
            } while (-1 < i);
            AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x48e1c));
        }
    }
    return;
}

extern char *volatile D_8008D9D0_v __asm__("kaze_state_base_ptr");
extern int kaze_stage_mode;
extern int kaze_stage_variant;

/**
 * kaze_marker_brightness_anim  (render, 0x80029f2c)
 *
 * Advances the marker/effect brightness phase at +0x4974c.
 */

void kaze_marker_brightness_anim(void *arg0)
{
    int b;
    int pad;
    int cst;
    char *d1;
    char *d2;
    int inv;
    int mask;
    int i;
    int off;
    int base2;
    int ba;
    int bb;
    char *v0;
    char *v1;

    {
        register int c asm("$4");
        c = *(volatile int *)(kaze_state_base_ptr + 0x4974C);
        b = c;
        if (c >= 0xFF) {
            register int cm asm("$4");
            __asm__("" : "=r"(cm) : "0"(c));
            b = 0x1FE - cm;
        }
    }
    __asm__("lui %0,0x40" : "=r"(mask));
    d1 = D_8008D9D0_v;
    *(volatile int *)(d1 + 0x4974C) =
        (*(volatile int *)(d1 + 0x4974C) + 0x10) % 510;
    if (!(*(int *)(d1 + 0x14) & mask)) {
        inv = ~b;
        v0 = d1 + *(int *)(d1 + 0x98) * 0x110;
        *(s8 *)(v0 + 0x494B2) = inv;
        *(s8 *)(v0 + 0x494B1) = inv;
        *(s8 *)(v0 + 0x494B0) = inv;
        v1 = kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x10;
        *(s8 *)(v1 + 0x496D2) = inv;
        *(s8 *)(v1 + 0x496D1) = inv;
        *(s8 *)(v1 + 0x496D0) = inv;
        AddPrim(arg0,
                kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x110 + 0x494AC));
        AddPrim(arg0,
                kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x10 + 0x496CC));
    }
    __asm__ __volatile__("" : "=r"(inv));
    __asm__ __volatile__("" :: "r"(off));
    switch (kaze_stage_mode) {
    case 0:
        if (*(int *)(kaze_state_base_ptr + 0x14) & mask) {
            {
                char *b1p;
                b1p = kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x1C;
                *(s8 *)(b1p + 0x496F1) = b;
            }
            {
                char *b2p;
                b2p = kaze_state_base_ptr + *(int *)(kaze_state_base_ptr + 0x98) * 0x14;
                *(s8 *)(b2p + 0x49729) = ~b;
            }
            AddPrim(arg0,
                    kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x1C + 0x496EC));
            AddPrim(arg0,
                    kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x14 + 0x49724));
        }
        i = 0;
        bb = b;
        off = 0x10;
        base2 = 0;
        do {
            if (*(int *)(kaze_state_base_ptr + base2 + 0x58A74) < 0) {
                v0 = kaze_state_base_ptr + (off + *(int *)(kaze_state_base_ptr + 0x98) * 0x110);
                cst = 0x40000;
                __asm__("" : "=r"(cst) : "0"(cst));
                *(s8 *)(v0 + 0x494B2) = bb;
                *(s8 *)(v0 + 0x494B1) = bb;
                *(s8 *)(v0 + 0x494B0) = bb;
                cst |= 0x94AC;
                AddPrim(arg0,
                        kaze_state_base_ptr +
                            (*(int *)(kaze_state_base_ptr + 0x98) * 0x110 + cst) +
                            off);
            }
            off += 0x10;
            i += 1;
            base2 += 0x5C;
        } while (i < 3);
        break;
    case 1:
        if (*(int *)(kaze_state_base_ptr + 0x14) & mask) {
            i = 0;
            ba = b;
            off = 0x70;
            do {
                char *bp;
                bp = kaze_state_base_ptr + (off + *(int *)(kaze_state_base_ptr + 0x98) * 0x110);
                *(s8 *)(bp + 0x494B2) = ba;
                *(s8 *)(bp + 0x494B1) = ba;
                *(s8 *)(bp + 0x494B0) = ba;
                __asm__("lui %0,0x4" : "=r"(cst) : "r"(i));
                __asm__("ori %0,%0,0x94ac" : "=r"(cst) : "0"(cst));
                i += 1;
                AddPrim(arg0,
                        kaze_state_base_ptr +
                            (*(int *)(kaze_state_base_ptr + 0x98) * 0x110 + cst) +
                            off);
                off += 0x10;
            } while (i < 3);
        } else {
            i = 0;
            bb = b;
            off = 0x10;
            base2 = 0;
            do {
                if (*(int *)(kaze_state_base_ptr + base2 + 0x58A74) < 0) {
                    v0 = kaze_state_base_ptr +
                         (off + *(int *)(kaze_state_base_ptr + 0x98) * 0x110);
                    cst = 0x40000;
                    __asm__("" : "=r"(cst) : "0"(cst));
                    *(s8 *)(v0 + 0x494B2) = bb;
                    *(s8 *)(v0 + 0x494B1) = bb;
                    *(s8 *)(v0 + 0x494B0) = bb;
                    cst |= 0x94AC;
                    AddPrim(arg0,
                            kaze_state_base_ptr +
                                (*(int *)(kaze_state_base_ptr + 0x98) * 0x110 +
                                 cst) +
                                off);
                }
                off += 0x10;
                i += 1;
                base2 += 0x5C;
            } while (i < 3);
            i = 0;
            d2 = kaze_state_base_ptr;
            if (*(int *)(d2 + 0x24) > 0) {
                ba = b;
                off = 0x40;
                do {
                    char *bp2;
                    bp2 = d2 +
                         (off + *(int *)(d2 + 0x98) * 0x110);
                    *(s8 *)(bp2 + 0x494B2) = ba;
                    *(s8 *)(bp2 + 0x494B1) = ba;
                    *(s8 *)(bp2 + 0x494B0) = ba;
                    __asm__("lui %0,0x4" : "=r"(cst) : "r"(i));
                    __asm__("ori %0,%0,0x94ac" : "=r"(cst) : "0"(cst));
                    AddPrim(arg0,
                            kaze_state_base_ptr +
                                (*(int *)(kaze_state_base_ptr + 0x98) * 0x110 +
                                 cst) +
                                off);
                    i += 1;
                    off += 0x10;
                    d2 = kaze_state_base_ptr;
                } while (i < *(int *)(d2 + 0x24));
            }
        }
        break;
    case 2:
        i = 0;
        ba = b;
        off = 0xA0;
        do {
            char *bp3;
            bp3 = kaze_state_base_ptr + (off + *(int *)(kaze_state_base_ptr + 0x98) * 0x110);
            *(s8 *)(bp3 + 0x494B2) = ba;
            *(s8 *)(bp3 + 0x494B1) = ba;
            *(s8 *)(bp3 + 0x494B0) = ba;
            __asm__("lui %0,0x4" : "=r"(cst) : "r"(i));
            __asm__("ori %0,%0,0x94ac" : "=r"(cst) : "0"(cst));
            i += 1;
            AddPrim(arg0,
                    kaze_state_base_ptr +
                        (*(int *)(kaze_state_base_ptr + 0x98) * 0x110 + cst) +
                        off);
            off += 0x10;
        } while (i < 6);
        break;
    default:
        i = 0;
        break;
    }
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x49750));
    if ((kaze_stage_mode == 1) && (*(int *)(kaze_state_base_ptr + 0x14) & 0x400000)) {
        i = 0;
        {
            int ct1;
            ct1 = 0x49800;
            __asm__("" : "=r"(ct1) : "0"(ct1));
            off = 0;
            do {
                AddPrim(arg0,
                        kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x140 + ct1) +
                            off);
                i += 1;
                off += 0x28;
            } while (i < 4);
        }
    } else {
        if (kaze_stage_variant == 7) {
            i = 0;
            {
                int ct2;
                ct2 = 0x49760;
                __asm__("" : "=r"(ct2) : "0"(ct2));
                off = 0;
                do {
                    AddPrim(arg0,
                            kaze_state_base_ptr +
                                (*(int *)(kaze_state_base_ptr + 0x98) * 0x48 + ct2) + off);
                    i += 1;
                    off += 0x18;
                } while (i < 3);
            }
            AddPrim(arg0,
                    kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x497F0));
        }
        AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x14 + 0x492C4));
    }
    i = 3;
    __asm__ __volatile__("" :: "r"(i));
    {
        int ct3;
        ct3 = 0x493EC;
        __asm__("" : "=r"(ct3) : "0"(ct3));
        off = 0x48;
        do {
            AddPrim(arg0,
                    kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x60 + ct3) +
                        off);
            i -= 1;
            off -= 0x18;
        } while (i >= 0);
    }
    i = 4;
    __asm__ __volatile__("" :: "r"(i));
    {
        int ct4;
        ct4 = 0x492FC;
        __asm__("" : "=r"(ct4) : "0"(ct4));
        off = 0x60;
        do {
            AddPrim(arg0,
                    kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x78 + ct4) +
                        off);
            i -= 1;
            off -= 0x18;
        } while (i >= 0);
    }
    AddPrim(arg0, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x492EC));
    __asm__ __volatile__("" :: "m"(pad));
}
