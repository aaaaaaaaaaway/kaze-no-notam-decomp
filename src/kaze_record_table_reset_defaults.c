/**
 * kaze_record_table_reset_defaults  (core, 0x8005fea8)
 *
 * Resets the 3x3x6 record table kaze_record_table_a/kaze_record_table_score
 * from the ROM default table at kaze_record_default_initial0_table (name
 * chars + value per entry).
 */

#include "types.h"
#include "kaze_syms.h"
extern u8 kaze_record_default_initial0_table;
extern u8 kaze_record_default_initial1_table;
extern u8 kaze_record_default_initial2_table;
extern int kaze_record_default_value_table;
extern u8 kaze_record_table_a;
extern u8 kaze_record_table_b;
extern u8 kaze_record_table_c;
extern int kaze_record_table_score;

void kaze_record_table_reset_defaults(void)
{
    u8 a, b, c;
    int d;
    int i4, i6, i7, i8, i9, i10, i11, i12, i13;

    i11 = 0;
    i13 = 0;
    i12 = 0;
    do {
        i10 = 0;
        i9 = 0;
        do {
            i8 = 0;
            i7 = i9;
            i6 = i11 << 3;
            do {
                a = (&kaze_record_default_initial0_table)[i6];
                b = (&kaze_record_default_initial1_table)[i6];
                c = (&kaze_record_default_initial2_table)[i6];
                d = *(int *)((int)&kaze_record_default_value_table + i6);
                i6 = i6 + 8;
                i11 = i11 + 1;
                i4 = i7 + i12;
                i8 = i8 + 1;
                (&kaze_record_table_a)[i4] = a;
                (&kaze_record_table_b)[i4] = b;
                (&kaze_record_table_c)[i4] = c;
                *(int *)((int)&kaze_record_table_score + i4) = d;
                i7 = i7 + 8;
            } while (i8 < 6);
            i10 = i10 + 1;
            i9 = i9 + 0x30;
        } while (i10 < 3);
        i13 = i13 + 1;
        i12 = i12 + 0x90;
    } while (i13 < 3);
}
