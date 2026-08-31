/**
 * kaze_model_packet_table_build_97ec  (render, 0x800497ec)
 *
 * Walks 264 variable-length primitive records (stride 4 + len_byte*4) from
 * the model header field +0x10 and stores each record pointer into the table
 * at kaze_model_prim_ptr_table.
 */

#include "types.h"
#include "kaze_syms.h"
extern int kaze_model_prim_ptr_table[];

void kaze_model_packet_table_build_97ec(int arg0)
{
    int p;
    int i;
    int *tab;
    int q;

    p = *(int *)(arg0 + 0x10);
    i = 0;
    tab = kaze_model_prim_ptr_table;
    do {
        *tab = p;
        q = *(u8 *)(p + 1);
        i = i + 1;
        q = q * 4 + 4;
        p = p + q;
        tab = tab + 1;
    } while (i < 0x108);
}
