/**
 * kaze_read_sz_fifo4  (library, 0x800745f0)
 *
 * PsyQ ReadSZfifo4 (REG05.OBJ): stores the four GTE screen-Z FIFO values via
 * gte_stsz4.
 */

void kaze_read_sz_fifo4(long *sz0, long *sz1, long *sz2, long *sz3) {
    __asm__ volatile("swc2 $16, 0(%0)" :: "r"(sz0) : "memory");
    __asm__ volatile("swc2 $17, 0(%0)" :: "r"(sz1) : "memory");
    __asm__ volatile("swc2 $18, 0(%0)" :: "r"(sz2) : "memory");
    __asm__ volatile("swc2 $19, 0(%0)" :: "r"(sz3) : "memory");
}
