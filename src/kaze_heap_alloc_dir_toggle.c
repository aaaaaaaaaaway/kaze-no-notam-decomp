/**
 * kaze_heap_alloc_dir_toggle  (library, 0x80012ea0)
 *
 * Toggles the heap allocation-direction flag kaze_heap_alloc_direction_flag
 * (front vs back split in the free-list allocator).
 */


#include "kaze_syms.h"
extern int kaze_heap_alloc_direction_flag;
void kaze_heap_alloc_dir_toggle(void)
{
    kaze_heap_alloc_direction_flag = (kaze_heap_alloc_direction_flag == 0);
}
