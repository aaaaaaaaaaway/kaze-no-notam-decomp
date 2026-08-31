/**
 * kaze_bios_a0_thunk  (library, 0x80080dcc)
 *
 * BIOS call thunk: jumps through the kernel A0 function-table gateway at
 * 0xa0.
 */

/* raw BIOS A0 syscall thunk (t2=0xA0 jump with t1=0x72 selector); asm-only function */
__asm__(
"	.text\n"
"	.globl kaze_bios_a0_thunk\n"
"	.ent kaze_bios_a0_thunk\n"
"kaze_bios_a0_thunk:\n"
"	.set noreorder\n"
"	addiu	$10, $zero, 0xA0\n"
"	jr	$10\n"
"	addiu	$9, $zero, 0x72\n"
"	.set reorder\n"
"	.end kaze_bios_a0_thunk\n"
);
