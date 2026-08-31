

/**
 * kaze_memcard_free_blocks  (library, 0x8004bb04)
 *
 * Sums file sizes in card directory kaze_memcard_directory_table and returns
 * free block count (15 - used).
 */



#include "types.h"
int kaze_memcard_free_blocks(int n)
{
    extern u8 kaze_memcard_directory_table;
    int i;
    int total;
    int adj;
    int pad[2];
    u8 *p;

    i = 0;
    total = 0;
    if (n > 0) {
        p = &kaze_memcard_directory_table;
        do {
            int t = *(int *)(p + 0x18);
            i++;
            total += t;
            p += 0x28;
        } while (i < n);
        adj = total;
    } else {
        adj = total;
    }
    if (total < 0) {
        adj = total + 0x1FFF;
    }
    total = adj >> 13;
    return 15 - total;
}

/**
 * kaze_memcard_find_file  (library, 0x8004bb58)
 *
 * Linear-searches the card directory kaze_memcard_directory_table for a
 * filename match; returns 1 if found.
 */

int kaze_memcard_find_file(int count, u8 *name)
{
    extern u8 kaze_memcard_directory_table[];
    int pad[2];
    int i;
    u8 *slot;
    int miss;
    u8 *p;
    u8 *q;
    u8 *limit;
    u8 n;
    int c;

    i = 0;
    if (count > 0) {
        slot = kaze_memcard_directory_table;
        do {
            miss = 0;
            p = slot;
            q = name;
            limit = slot + 0x15;
        top:
            n = *q;
            if (n == 0) goto after;
            c = *p;
            p++;
            if (n != c) goto miss_lbl;
            q++;
            if ((int)p < (int)limit) goto top;
        after:
            i++;
            if (miss) goto next;
            return 1;
        miss_lbl:
            miss++;
            goto after;
        next:
            slot += 0x28;
        } while (i < count);
    }
    return 0;
}
