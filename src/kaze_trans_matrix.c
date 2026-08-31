/**
 * kaze_trans_matrix  (library, 0x80070cb8)
 *
 * PsyQ TransMatrix (MTX_07.OBJ): copies a 3-word translation vector into
 * MATRIX +0x14/+0x18/+0x1c.
 */

/* PsyQ MTX_07.OBJ TransMatrix. Hand-assembled original (t-reg temps, base
   kept in $4, return-move as the LAST insn, unfilled jr delay slot).
   Matched lawfully at gcc 2.7.2-psx -O1 via register pins + a zero-code
   volatile launder:
   - pins $8/$9/$10 reproduce the hand-picked t-reg temps (gcc alone
     allocates $2/$3/$5);
   - pinning the base copy to $4 keeps stores on mtx_arg and defers the
     $2 <- $4 return move to the end (otherwise local-alloc ties the base
     pseudo to $2 and hoists the move to function entry);
   - the ""-template volatile asm pins the return value into $2 through a
     "0" tie, forcing reload to emit `addu $2,$4,$0` right before it while
     the volatile barrier keeps reorg/assembler from swapping anything into
     the jr delay slot, recovering the original's nop. */
int kaze_trans_matrix(int mtx_arg, int *vec)
{
    register int mtx __asm__("$4") = mtx_arg;
    register int a __asm__("$8") = vec[0];
    register int b __asm__("$9") = vec[1];
    register int c __asm__("$10") = vec[2];
    register int r __asm__("$2");
    *(int *)(mtx + 0x14) = a;
    *(int *)(mtx + 0x18) = b;
    *(int *)(mtx + 0x1c) = c;
    __asm__ volatile("" : "=r"(r) : "0"(mtx));
    return r;
}
