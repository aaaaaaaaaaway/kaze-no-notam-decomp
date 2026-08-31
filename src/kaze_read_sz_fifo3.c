/**
 * kaze_read_sz_fifo3  (library, 0x80071cf0)
 *
 * PsyQ ReadSZfifo3 (REG04.OBJ): stores the three GTE screen-Z FIFO values
 * via gte_stsz3.
 */


void kaze_read_sz_fifo3(long *sz0, long *sz1, long *sz2) {
    __asm__ volatile("swc2 $17, 0(%0)" :: "r"(sz0) : "memory");
    __asm__ volatile("swc2 $18, 0(%0)" :: "r"(sz1) : "memory");
    __asm__ volatile("swc2 $19, 0(%0)" :: "r"(sz2) : "memory");
}
