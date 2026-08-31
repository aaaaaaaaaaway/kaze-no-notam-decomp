/**
 * kaze_stage_completion_check  (objective, 0x8001d4a4)
 *
 * Per-mode completion test when root+0x14 terminal flags clear: modes 0/1
 * need >=3 completed +0x58a74 markers (bit 0x80000000), mode 2 scans
 * +0x58e58; score via kaze_stage_score_compute into +0xc, raises
 * success/failure handlers.
 */


#include "kaze_syms.h"
extern int kaze_mode_state;
extern int kaze_stage_score_compute(int);
extern int *kaze_state_base_ptr;
extern int kaze_stage_index;
extern int kaze_stage_mode;
extern int kaze_stage_variant;

extern int SquareRoot0(int);
extern int kaze_record_table_insert(int, int, int);

#define M(base, o) (*(int *)((char *)(base) + (o)))

void kaze_stage_completion_check(void)
{
    int *base;
    int s6v;
    int d2;
    int vA, vB, vC;

    base = kaze_state_base_ptr;
    if (base[5] & 0xc00000) {
        return;
    }
    s6v = kaze_stage_index + 1;
    s6v = s6v & -(s6v < 3);

    if (kaze_stage_variant == 0) {
        goto ModeDispatch;
    }
    {
        switch (kaze_stage_variant) {
        case 2: {
            int *b2;
            register char *p2 __asm__("$4");

            int mx, mz, rz;
            register int rx __asm__("$3");
            int t2, cl2;
            vA = 0;
            vB = 0;
            vC = 0;
            do {
                b2 = kaze_state_base_ptr;
                p2 = (char *)b2 + vC;
                if (M(p2, 0x58a74) < 0) {
                    mx = M(p2, 0x58a7c);
                    rx = M(b2, 0x58e14);
                    mz = M(p2, 0x58a84);
                    rz = M(b2, 0x58e1c);
                    mx -= rx;
                    rx = mz - rz;
                    t2 = mx >= 0 ? mx : -mx;
                    vA++;
                    if (t2 < 0x76c) {
                        t2 = rx >= 0 ? rx : -rx;
                        if (t2 < 0x76c) {
                            t2 = mx * mx;
                            rx *= rx;
                            d2 = SquareRoot0(t2 + rx);
                            if (d2 >= 0) { cl2 = 0x76c; if (d2 < 0x76d) cl2 = d2; } else { cl2 = 0; }
                            d2 = cl2;
                            d2 = d2 * 100 / 38;
                            if (d2 >= 0) { cl2 = 0x270f; if (d2 < 0x2710) cl2 = d2; } else { cl2 = 0; }
                            d2 = cl2;
                            if (d2 < 0x3e9) {
                                goto Lhit2;
                            }
                        }
                    }
                }
                vB++;
                vC += 0x5c;
            } while (vB < 10);
            if (vA >= 3) {
                int sc2 = kaze_stage_score_compute(kaze_stage_mode);
                kaze_state_base_ptr[3] = sc2;
                if ((unsigned int)sc2 < 0x3e9) {
                    if (sc2 < 0x3e9) {
                        kaze_success_terminal_handler();
                        kaze_state_base_ptr[4] = kaze_record_table_insert(s6v, 0, kaze_state_base_ptr[3]);
                        return;
                    }
                } else {
                    goto Lfail;
                }
            }
            break; }

        case 5: {
            int *b5;
            char *po, *pi;
            int off5, off5c, offi, bit;
            int mx, rx, mz, rz;
            int t5, cl5;
            vC = 0;
            off5 = 0;
            do {
                vA = 0;
                if (M((char *)kaze_state_base_ptr + off5, 0x58a74) < 0) {
                    off5c = off5;
                    offi = 0;
                    do {
                        b5 = kaze_state_base_ptr;
                        po = (char *)b5 + off5c;
                        __asm__("" :: "r"(b5));
                        pi = (char *)b5 + offi;
                        mx = M(po, 0x58a7c);
                        rx = M(pi, 0x58e14);
                        __asm__("");
                        mz = M(po, 0x58a84);
                        rz = M(pi, 0x58e1c);
                        mx -= rx;
                        mz -= rz;
                        t5 = mx >= 0 ? mx : -mx;
                        if (t5 < 0x76c) {
                            t5 = mz >= 0 ? mz : -mz;
                            if (t5 < 0x76c) {
                                t5 = mx * mx;
                                mz *= mz;
                                d2 = SquareRoot0(t5 + mz);
                                if (d2 >= 0) { cl5 = 0x76c; if (d2 < 0x76d) cl5 = d2; } else { cl5 = 0; }
                                d2 = cl5;
                                d2 = d2 * 100 / 38;
                                if (d2 >= 0) { cl5 = 0x270f; if (d2 < 0x2710) cl5 = d2; } else { cl5 = 0; }
                                d2 = cl5;
                                if (d2 < 0x7d1) {
                                    bit = 1 << vA;
                                    if ((vC & bit) == 0) {
                                        vC |= bit;
                                        break;
                                    }
                                }
                                kaze_failure_terminal_handler();
                            }
                        }
                        vA++;
                        offi += 0x58;
                    } while (vA < 3);
                    if (vA >= 3) {
                        kaze_failure_terminal_handler();
                    }
                }
                off5 += 0x5c;
            } while (off5 < 0x114);
            if ((vC & 7) == 7) {
                int sc5;
                kaze_success_terminal_handler();
                sc5 = kaze_stage_score_compute(kaze_stage_mode);
                kaze_state_base_ptr[3] = sc5;
                kaze_state_base_ptr[4] = kaze_record_table_insert(s6v, 0, sc5);
                return;
            }
            break; }

        case 9: {
            int *b9;
            register char *p9 __asm__("$4");

            int mz, rz;
            register int mx __asm__("$5");
            register int rx __asm__("$3");
            int t9, cl9;
            int score9;
            vA = 0;
            vB = 0;
            vC = 0;
            for (; vB < 3; vC += 0x5c, vB++) {
                b9 = kaze_state_base_ptr;
                p9 = (char *)b9 + vC;
                if (M(p9, 0x58a74) < 0) {
                    mx = M(p9, 0x58a7c);
                    rx = M(b9, 0x58e14);
                    mz = M(p9, 0x58a84);
                    rz = M(b9, 0x58e1c);
                    mx -= rx;
                    rx = mz - rz;
                    t9 = mx >= 0 ? mx : -mx;
                    if (t9 < 0x76c) {
                        t9 = rx >= 0 ? rx : -rx;
                        if (t9 < 0x76c) {
                            t9 = mx * mx;
                            rx *= rx;
                            d2 = SquareRoot0(t9 + rx);
                            if (d2 >= 0) { cl9 = 0x76c; if (d2 < 0x76d) cl9 = d2; } else { cl9 = 0; }
                            d2 = cl9;
                            d2 = d2 * 100 / 38;
                            if (d2 >= 0) { cl9 = 0x270f; if (d2 < 0x2710) cl9 = d2; } else { cl9 = 0; }
                            d2 = cl9;
                            if (d2 >= 0x3e9) {
                                kaze_failure_terminal_handler();
                                continue;
                            }
                            vA++;
                            continue;
                        }
                    }
                    kaze_failure_terminal_handler();
                    break;
                }
            }
            if (vA < 3) {
                break;
            }
            score9 = kaze_stage_score_compute(kaze_stage_mode);
            kaze_state_base_ptr[3] = score9;
            kaze_success_terminal_handler();
            kaze_state_base_ptr[4] = kaze_record_table_insert(s6v, 0, kaze_state_base_ptr[3]);
            return; }

        case 1:
        case 4: {
            int *b14;
            char *wp14;
            int d14;
            vA = 0;
            b14 = kaze_state_base_ptr;
            for (vB = 0; vB < 10; vB++) {
                wp14 = (char *)b14 + vB * 0x5c;
                if (*(int *)(wp14 + 0x58a74) & 0x80000000) {
                    vA++;
                }
            }
            if (vA >= 3) {
                vA = 0xbb8;
                if (kaze_stage_variant == 4) {
                    vA = 0x9c4;
                }
                d14 = kaze_stage_score_compute(kaze_stage_mode);
                kaze_state_base_ptr[3] = d14;
                if (d14 >= vA) {
                    kaze_success_terminal_handler();
                    kaze_state_base_ptr[4] = kaze_record_table_insert(s6v, 1, kaze_state_base_ptr[3]);
                    return;
                }
                goto Lfail;
            }
            break; }

        case 7: {
            char *p7;
            register int f1 __asm__("$3");
            register int f3 __asm__("$5");
            register int mx7 __asm__("$4");
            int mz7;

            vC = 0;
            d2 = 0;
            f1 = 0;
            f3 = 0;
            vA = 0;
            vB = 0;
            do {
                p7 = (char *)kaze_state_base_ptr + vB * 0x5c;
                if (M(p7, 0x58a74) < 0) {
                    mx7 = M(p7, 0x58a7c);
                    vA++;
                    if ((unsigned int)(mx7 + 0xfffddac7) > 0x3218e) {
                        mz7 = M(p7, 0x58a84);
                        if ((unsigned int)(mz7 + 0xfffddac7) > 0x3218e) {
                            if (mx7 <= 0x22538) {
                                if (mz7 <= 0x22538) {
                                    if (f3) {
                                        goto Lf7;
                                    }
                                    f3 = 1;
                                } else {
                                    if (d2) {
                                        goto Lf7;
                                    }
                                    d2 = 1;
                                }
                            } else {
                                if (mz7 <= 0x22538) {
                                    if (f1) {
                                        goto Lf7;
                                    }
                                    f1 = 1;
                                    __asm__("" :: "r"(mx7));
                                } else {
                                    if (vC) {
                                        goto Lf7;
                                    }
                                    vC = 1;
                                    __asm__("" :: "r"(mx7));
                                }
                            }
                        } else {
                            goto Lf7;
                        }
                    } else {
                        goto Lf7;
                    }
                }
                vB++;
                continue;
Lf7:
                kaze_failure_terminal_handler();
                return;
            } while (vB < 3);
            if (vA >= 3) {
                int sc7;
                kaze_success_terminal_handler();
                sc7 = kaze_stage_score_compute(kaze_stage_mode);
                kaze_state_base_ptr[3] = sc7;
                kaze_state_base_ptr[4] = kaze_record_table_insert(s6v, 1, sc7);
                return;
            }
            break; }

        case 3: {
            if ((unsigned int)((unsigned int)M(kaze_state_base_ptr, 0x48284) / 60) < 0x3d) {
                vA = 0;
                do {
                    char *p3 = (char *)kaze_state_base_ptr + vA * 0x58;
                    if (M(p3, 0x58e58) == 0x84000000) {
                        int sc3;
                        kaze_success_terminal_handler();
                        sc3 = kaze_stage_score_compute(2);
                        kaze_state_base_ptr[3] = sc3;
                        kaze_state_base_ptr[4] = kaze_record_table_insert(s6v, 2, sc3);
                    }
                    vA++;
                } while (vA < 6);
                return;
            }
            goto Lfail; }

        case 8: {
            char *p8;
            vA = 0;
            for (; vA < 6; vA++) {
                p8 = (char *)kaze_state_base_ptr + vA * 0x58;
                if (M(p8, 0x58e58) == 0x84000000 && M(p8, 0x58e0c) == 3) {
                    goto Lwin2;
                }
            }
            break; }

        case 6: {
            int *b6;

            vB = 0;
            vA = 0;
            for (; vA < 6; vA++) {
                char *p6a = (char *)kaze_state_base_ptr + vA * 0x58;
                if (M(p6a, 0x58e58) == 0x84000000) {
                    vB = 1;
                    break;
                }
            }
            if (vB) {
                goto Lwin2;
            }
            if (kaze_mode_state == 0) {
                vA = 0;
                for (; vA < 6; vA++) {
                    char *p6b = (char *)kaze_state_base_ptr + vA * 0x58;
                    if (M(p6b, 0x58e58) & 0x80000000) {
                        return;
                    }
                }
                vB = 0;
                b6 = kaze_state_base_ptr;
                for (vA = 0; vA < 3; vA++) {
                    char *p6 = (char *)b6 + vA * 0x5c;
                    if (M(p6, 0x58a74) == 0) {
                        vB++;
                    }
                }
                if (vB < 3) {
                    break;
                }
Lfail:
                kaze_failure_terminal_handler();
                return;
            }
            break; }
        }
        return;
    }

ModeDispatch:
    switch (kaze_stage_mode) {
    case 0: {
        char *wp0;

        vA = 0;
        for (vB = 0; vB < 10; vB++) {
            wp0 = (char *)base + vB * 0x5c;
            if (*(int *)(wp0 + 0x58a74) & 0x80000000) {
                vA++;
            }
        }
        if (vA >= 3) {
            int sc0;
            kaze_success_terminal_handler();
            sc0 = kaze_stage_score_compute(kaze_stage_mode);
            kaze_state_base_ptr[3] = sc0;
            kaze_state_base_ptr[4] = kaze_record_table_insert(s6v, 1, sc0);
            return;
        }
        break; }

    case 1: {
        char *wp1;
        int sc1;
        vA = 0;
        for (vB = 0; vB < 10; vB++) {
            wp1 = (char *)base + vB * 0x5c;
            if (*(int *)(wp1 + 0x58a74) & 0x80000000) {
                vA++;
            }
        }
        if (vA >= 3) {
            sc1 = kaze_stage_score_compute(kaze_stage_mode);
            kaze_state_base_ptr[3] = sc1;
            if (sc1 < 0) {
                goto Lfail;
            }
            kaze_success_terminal_handler();
            kaze_state_base_ptr[4] = kaze_record_table_insert(s6v, 0, kaze_state_base_ptr[3]);
            return;
        }
        break; }

    case 2:
        goto Lmode2;
    }
    return;

Lhit2:
    kaze_success_terminal_handler();
    kaze_state_base_ptr[3] = d2;
    kaze_state_base_ptr[4] = kaze_record_table_insert(s6v, 0, d2);
    return;

Lwin2: {
    int scw;
    kaze_success_terminal_handler();
    scw = kaze_stage_score_compute(2);
    kaze_state_base_ptr[3] = scw;
    kaze_state_base_ptr[4] = kaze_record_table_insert(s6v, 2, scw);
    return; }

Lmode2: {
    char *pm2;
    for (vA = 0; vA < 6; vA++) {
        pm2 = (char *)base + vA * 0x58;
        if (M(pm2, 0x58e58) == 0x84000000) {
            goto Lwin2;
        }
    }
    return; }
}
