#include "types.h"
#include "kaze_syms.h"
extern u8 kaze_save_icon_clut[];
extern u8 kaze_save_icon_frame0[];
extern u8 kaze_save_icon_frame1[];
extern u8 kaze_save_icon_frame2[];
extern u8 kaze_save_icon_frame0_rare[];
extern int kaze_task_record_index;
extern char kaze_task_name_string[];
extern char kaze_slot_overlay_header_icon_sprites[];
extern char kaze_slot_overlay_header_bar_sprites[];
extern char kaze_slot_overlay_header_tpages[];
extern int D_800B17D4;
struct RecPtr { int p; };
struct DB2 { int v; };

extern int kaze_memcard_slot_index;
extern int kaze_memcard_slot_request_state[];
extern void kaze_slot_overlay_add_header_prims(void);
extern void kaze_slot_overlay_add_rows(int);
extern void kaze_slot_overlay_add_status_strip(int, int, int);
extern void kaze_drmove_addprim_a0ec(int, int);

/**
 * kaze_slot_select_overlay_draw  (menu, 0x80061330)
 *
 * Per-frame draw of the two-slot select overlay: adds header prims
 * (kaze_slot_overlay_add_header_prims), the two rows with highlight at index
 * kaze_memcard_slot_index and status flags kaze_memcard_slot_status_flags,
 * and a status strip chosen by kaze_memcard_slot_request_state[sel] bit 2.
 */

void kaze_slot_select_overlay_draw(int arg)
{
    kaze_slot_overlay_add_header_prims();
    kaze_slot_overlay_add_rows(arg);
    if (kaze_memcard_slot_request_state[kaze_memcard_slot_index] & 4) {
        kaze_slot_overlay_add_status_strip(arg, 8, 0);
    } else {
        kaze_slot_overlay_add_status_strip(arg, 2, 1);
    }
    kaze_drmove_addprim_a0ec(arg, 0xFFF);
}

extern u8 *kaze_memcard_file_image;
extern u8 kaze_sound_mode_setting;
extern u8 kaze_grid_variant_saved_index;
extern u8 kaze_settings_toggle_a;
extern u8 kaze_settings_toggle_b;
extern u8 kaze_record_table_a;
extern u8 kaze_record_table_b;
extern u8 kaze_record_table_c;

/**
 * kaze_save_verify_unpack_full  (menu, 0x800613b4)
 *
 * Verifies the 0x1dfc-byte checksum of the save image at
 * kaze_memcard_file_image, then unpacks options
 * (kaze_task_record_index/dc14/dc1c/dc1d), five record tables into
 * kaze_save_slot_unpack_buffer[], the 3x3x6 grid at kaze_record_table_a, and
 * bit flags to kaze_task_grid_available_flags.
 */

int kaze_save_verify_unpack_full(void)
{
    extern u8 kaze_task_grid_available_flags;
    extern u8 kaze_record_table_score;
    extern struct RecPtr kaze_save_slot_unpack_buffer[];
    extern u8 kaze_task_name_letter0_table;
    extern u8 kaze_button_action_map;
    register u8 *p asm("$3");
    register int checksum asm("$4");
    register unsigned int i asm("$7");

    p = kaze_memcard_file_image;
    checksum = 0;
    i = 0;
    do {
        i++;
        checksum += *p;
        p++;
    } while (i < 0x1DFC);

    if (checksum != *(int *)(kaze_memcard_file_image + 0x1DFC)) {
        return -1;
    }

    {
        register int outer asm("$7");
        register int aoff asm("$6");
        register u8 *dbe4p asm("$8");

        kaze_task_record_index = kaze_memcard_file_image[2];
        kaze_sound_mode_setting = kaze_memcard_file_image[3];
        outer = 0;
        kaze_grid_variant_saved_index = kaze_memcard_file_image[0xA];
        do {
            (&kaze_button_action_map)[outer] = (kaze_memcard_file_image + outer)[4];
            outer++;
            aoff = 0;
        } while (outer < 5);

        {
            register u8 *q asm("$3");
            q = kaze_memcard_file_image;
            outer = 0;
            asm("" :: "r"(outer));
            kaze_settings_toggle_a = q[9] & 1;
            dbe4p = &kaze_task_name_letter0_table;
            kaze_settings_toggle_b = (q[9] >> 1) & 1;
        }
        do {
            register int cnt asm("$5");
            register int off asm("$4");
            register u8 *dst asm("$3");

            cnt = 0;
            off = aoff;
            dst = dbe4p;
            do {
                register u8 *src asm("$2");
                src = kaze_memcard_file_image;
                src += off;
                *dst = (src + cnt)[0x3B1];
                cnt++;
                dst++;
            } while (cnt < 3);

            cnt = 0;
            {
                register int rec asm("$2");
                rec = kaze_save_slot_unpack_buffer[outer].p;
                dst = (u8 *)(rec + 6);
            }
            off = aoff;
            do {
                register u8 *src asm("$2");
                src = kaze_memcard_file_image;
                src += off;
                dst[-2] = src[0x3B4];
                src = kaze_memcard_file_image;
                src += off;
                dst[-1] = src[0x3B5];
                cnt++;
                src = kaze_memcard_file_image;
                src += off;
                dst[0] = src[0x3B6];
                off += 3;
                dst += 0x14;
            } while (cnt < 0x108);
            aoff += 0x31C;
            outer++;
            dbe4p += 3;
        } while (outer < 5);
    }

    {
        register int outer asm("$7");
        register int t3 asm("$11");

        outer = 0;
        t3 = 0;
        do {
            register int mid asm("$5");
            register int t1 asm("$9");
            register int t2 asm("$10");

            mid = 0;
            t2 = t3;
            t1 = 0;
            do {
                register int cnt asm("$6");
                register int off asm("$4");
                register int idx asm("$3");

                cnt = 0;
                off = t1;
                for (; cnt < 6; cnt++) {
                    register u8 *src asm("$2");
                    idx = off + t2;
                    src = kaze_memcard_file_image;
                    (&kaze_record_table_a)[idx] = (src + idx)[0x200];
                    src = kaze_memcard_file_image;
                    (&kaze_record_table_b)[idx] = (src + idx)[0x201];
                    src = kaze_memcard_file_image;
                    (&kaze_record_table_c)[idx] = (src + idx)[0x202];
                    {
                        register u8 *srcw asm("$8");
                        srcw = kaze_memcard_file_image;
                        *(int *)(&kaze_record_table_score + idx) = *(int *)((srcw + idx) + 0x204);
                    }
                    off += 8;
                }
                mid++;
                t1 += 0x30;
            } while (mid < 3);
            outer++;
            t3 += 0x90;
        } while (outer < 3);
    }

    {
        register int mask asm("$5");
        register int outer asm("$7");
        register int one asm("$9");
        register u8 *pv asm("$6");

        mask = 1;
        outer = 0;
        one = 1;
        D_800B17D4 = (*(u16 *)kaze_memcard_file_image >> 9) & 3;
        pv = &kaze_task_grid_available_flags;
        do {
            register u8 *pw asm("$4");
            register u8 *bound asm("$8");

            pw = pv;
            bound = pv + 3;
            do {
                register unsigned int mm asm("$3");
                mm = mask & 0xFFFF;
                if ((mask & *(u16 *)kaze_memcard_file_image) == mm) {
                    *pw = one;
                } else {
                    *pw = 0;
                }
                pw++;
                mask <<= 1;
            } while ((int)pw < (int)bound);
            outer++;
            pv += 3;
        } while (outer < 3);
    }

    return 0;
}

extern u8 *kaze_memcard_work_buffer;
extern char *strcpy(char *, const char *);

/**
 * kaze_save_image_pack  (menu, 0x800616ac)
 *
 * Builds the memcard save image at kaze_memcard_work_buffer: 'SC'
 * header+title strcpy, icon data from
 * kaze_save_icon_frame0/kaze_save_icon_frame1 (variant by
 * kaze_rand_range()==0x32), packs options/records
 * (kaze_save_slot_unpack_buffer, kaze_record_table_a) and writes checksum at
 * +0x1dfc.
 */

void kaze_save_image_pack(void)
{
    extern int kaze_record_table_score;
    extern struct DB2 kaze_save_slot_unpack_buffer[];
    extern u8 kaze_task_grid_available_flags[];
    extern u8 kaze_task_name_letter0_table[];
    extern u8 kaze_button_action_map[];
    u8 *pack;
    u8 *save;
    int unused_int0;
    int grp_idx;
    int unused_int1;
    int unused_int2;
    int unused_int3;
    int unused_int4;
    int rec_idx;
    u8 *unused_ptr0;
    u8 *unused_ptr1;
    char *unused_cptr0;
    char *unused_cptr1;
    u16 *flag_word_ptr;
    u16 unused_u16;
    u16 flag_bits;
    u32 sum;
    u8 *p;

    pack = kaze_memcard_work_buffer;
    pack[0] = 0x53;
    pack[1] = 0x43;
    pack[2] = 0x13;
    pack[3] = 1;
    strcpy((char *)(pack + 4), kaze_task_name_string);
    __builtin_memcpy(pack + 0x60, kaze_save_icon_clut, 0x20);

    if (kaze_rand_range(0, 0x32) == 0x32) {
        __builtin_memcpy(pack + 0x80, kaze_save_icon_frame1, 0x80);
        __builtin_memcpy(pack + 0x100, kaze_save_icon_frame2, 0x80);
        __builtin_memcpy(pack + 0x180, kaze_save_icon_frame0_rare, 0x80);
    } else {
        __builtin_memcpy(pack + 0x80, kaze_save_icon_frame0, 0x80);
        __builtin_memcpy(pack + 0x100, kaze_save_icon_frame1, 0x80);
        __builtin_memcpy(pack + 0x180, kaze_save_icon_frame2, 0x80);
    }

    kaze_memcard_file_image[2] = (u8)kaze_task_record_index;
    kaze_memcard_file_image[3] = kaze_sound_mode_setting;
    kaze_memcard_file_image[0xa] = kaze_grid_variant_saved_index;

    {
        register int dc14i asm("$8");
        dc14i = 0;
        do {
            {
                register u8 dc14v asm("$3");
                dc14v = kaze_button_action_map[dc14i];
                (kaze_memcard_file_image + dc14i)[4] = dc14v;
            }
            dc14i = dc14i + 1;
        } while (dc14i < 5);
    }

    {
        register int grp_idx asm("$8");
        register int rBase asm("$7");
        register u8 *rFlag asm("$9");
        grp_idx = 0;
        rBase = 0;
        kaze_memcard_file_image[9] = kaze_settings_toggle_a;
        kaze_memcard_file_image[9] = kaze_memcard_file_image[9] | (kaze_settings_toggle_b << 1);
        rFlag = kaze_task_name_letter0_table;
        do {
            {
                register int c asm("$6");
                register int bc asm("$5");
                register u8 *pv asm("$4");
                c = 0;
                bc = rBase;
                pv = rFlag;
                do {
                    (kaze_memcard_file_image + bc + c)[0x3b1] = *pv;
                    c = c + 1;
                    pv = pv + 1;
                } while (c < 3);
            }
            {
                register int c asm("$6");
                register int off asm("$5");
                register u8 *src asm("$4");
                register int tmpv asm("$2");
                c = 0;
                tmpv = kaze_save_slot_unpack_buffer[grp_idx].v;
                src = (u8 *)(tmpv + 6);
                off = rBase;
                do {
                    c = c + 1;
                    (kaze_memcard_file_image + off)[0x3b4] = src[-2];
                    (kaze_memcard_file_image + off)[0x3b5] = src[-1];
                    (kaze_memcard_file_image + off)[0x3b6] = src[0];
                    src = src + 0x14;
                    off = off + 3;
                } while (c < 0x108);
            }
            rBase = rBase + 0x31c;
            grp_idx = grp_idx + 1;
            rFlag = rFlag + 3;
        } while (grp_idx < 5);
    }

    {
        register int outer asm("$8");
        register int base asm("$11");
        register int baseCopy asm("$10");
        outer = 0;
        base = 0;
        do {
            register int row asm("$6");
            register int rowOfs asm("$9");
            row = 0;
            baseCopy = base;
            rowOfs = 0;
            do {
                register int rank asm("$7");
                register int recOfs asm("$5");
                rank = 0;
                recOfs = rowOfs;
                do {
                    rec_idx = recOfs + baseCopy;
                    (kaze_memcard_file_image + rec_idx)[0x200] = (&kaze_record_table_a)[rec_idx];
                    (kaze_memcard_file_image + rec_idx)[0x201] = (&kaze_record_table_b)[rec_idx];
                    (kaze_memcard_file_image + rec_idx)[0x202] = (&kaze_record_table_c)[rec_idx];
                    flag_word_ptr = (u16 *)kaze_memcard_file_image;
                    *(int *)(kaze_memcard_file_image + rec_idx + 0x204) = *(int *)((int)&kaze_record_table_score + rec_idx);
                    recOfs = recOfs + 8;
                    rank = rank + 1;
                } while (rank < 6);
                row = row + 1;
                rowOfs = rowOfs + 0x30;
            } while (row < 3);
            outer = outer + 1;
            base = base + 0x90;
        } while (outer < 3);
    }

    {
        register u16 rBit asm("$6");
        register int rOuter asm("$8");
        register u16 *rFlagWord asm("$5");
        register char *rBase asm("$7");
        rBit = 1;
        rOuter = 0;
        rFlagWord = flag_word_ptr;
        rBase = (char *)kaze_task_grid_available_flags;
        {
            int term;
            flag_bits = *rFlagWord & 0xf9ff;
            term = (D_800B17D4 & 3) << 9;
            *rFlagWord = flag_bits;
            __asm__ volatile("" :: "m"(*rFlagWord));
            *rFlagWord = flag_bits | (u16)term;
        }
        do {
            register char *rCur asm("$4");
            register char *rEnd asm("$9");
            rCur = rBase;
            rEnd = rBase + 3;
            do {
                if (*rCur != '\0') {
                    *rFlagWord = *rFlagWord | rBit;
                } else {
                    *rFlagWord = *rFlagWord & ~rBit;
                }
                rCur = rCur + 1;
                rBit = rBit << 1;
            } while ((int)rCur < (int)rEnd);
            rOuter = rOuter + 1;
            rBase = rBase + 3;
        } while (rOuter < 3);
    }

    {
        register u8 *rP asm("$4");
        register u8 *rSave asm("$5");
        register u32 rCount asm("$8");
        rP = kaze_memcard_file_image;
        rCount = 0;
        rSave = rP;
        *(u32 *)(rP + 0x1dfc) = 0;
        do {
            rCount = rCount + 1;
            *(u32 *)(rSave + 0x1dfc) = *(u32 *)(rSave + 0x1dfc) + *rP;
            rP = rP + 1;
        } while (rCount < 0x1dfc);
    }
}

extern int GetClut(int, int);
extern int GetTPage(int, int, int, int);
extern void SetSprt(void *);
extern void SetShadeTex(void *, int);
extern void SetDrawTPage(void *, int, int, int);

/**
 * kaze_slot_overlay_header_sprites_init  (menu, 0x80061e38)
 *
 * Initializes the double-buffered header SPRT pairs at
 * kaze_slot_overlay_header_icon_sprites/kaze_slot_overlay_header_bar_sprites
 * (CLUTs 0x340/0x370 at y 0xf4) plus DR_TPAGE prims at
 * kaze_slot_overlay_header_tpages for the slot-select overlay.
 */

void kaze_slot_overlay_header_sprites_init(void)
{
    int i;
    int off;
    int size;
    char *p;
    int clut;

    i = 0;
    size = 0x40;
    off = 0;
    do {
        p = kaze_slot_overlay_header_icon_sprites;
        p = off + p;
        SetSprt(p);
        SetShadeTex(p, 1);
        *(short *)(p + 0x08) = -0xA0;
        *(short *)(p + 0x0A) = -0x78;
        *(short *)(p + 0x10) = size;
        *(short *)(p + 0x12) = size;
        p[0x0C] = 0;
        p[0x0D] = 0;
        clut = GetClut(0x340, 0xF4);
        *(short *)(kaze_slot_overlay_header_icon_sprites + off + 0x0E) = clut;

        p = kaze_slot_overlay_header_bar_sprites;
        p = off + p;
        SetSprt(p);
        SetShadeTex(p, 1);
        *(short *)(p + 0x08) = -0x60;
        *(short *)(p + 0x0A) = -0x58;
        *(short *)(p + 0x10) = 0xC0;
        *(short *)(p + 0x12) = 0x20;
        p[0x0C] = 0;
        p[0x0D] = size;
        clut = GetClut(0x370, 0xF4);
        *(short *)(kaze_slot_overlay_header_bar_sprites + off + 0x0E) = clut;

        clut = GetTPage(0, 0, 0x240, 0x100);
        SetDrawTPage(kaze_slot_overlay_header_tpages + i * 8, 0, 1, clut & 0xFFFF);
        i++;
        off += 0x14;
    } while (i < 2);
}
