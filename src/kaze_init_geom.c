/**
 * kaze_init_geom  (library, 0x80070580)
 *
 * PsyQ InitGeom: patches the GTE, enables COP2 in the status register, and
 * loads default GTE registers (ZSF3/ZSF4, H=1000, DQA/DQB, OFX/OFY=0).
 */


#include "kaze_syms.h"
extern void _patch_gte(void);
void kaze_init_geom(void) {
    /*
     * PsyQ InitGeom preserves the caller return address in a global while
     * patching the GTE, so a normal C call would add a nonmatching stack save.
     */
    __asm__ volatile(
        ".set noreorder\n"
        ".set noat\n"
        "lui $at, %%hi(kaze_init_geom_patch_gte_ra_save)\n"
        "sw $ra, %%lo(kaze_init_geom_patch_gte_ra_save)($at)\n"
        "jal _patch_gte\n"
        "lui $ra, %%hi(kaze_init_geom_patch_gte_ra_save)\n"
        "lw $ra, %%lo(kaze_init_geom_patch_gte_ra_save)($ra)\n"
        "nop\n"
        ".set at\n"
        ".set reorder\n"
        ::: "memory");

    /* Enable COP2 and load the default GTE geometry registers. */
    __asm__ volatile(
        ".set noreorder\n"
        "mfc0 $v0, $12\n"
        "lui $v1, 0x4000\n"
        "or $v0, $v0, $v1\n"
        "mtc0 $v0, $12\n"
        "nop\n"
        "addiu $t0, $zero, 0x155\n"
        "ctc2 $t0, $29\n"
        "nop\n"
        "addiu $t0, $zero, 0x100\n"
        "ctc2 $t0, $30\n"
        "nop\n"
        "addiu $t0, $zero, 0x3E8\n"
        "ctc2 $t0, $26\n"
        "nop\n"
        "addiu $t0, $zero, -0x1062\n"
        "ctc2 $t0, $27\n"
        "nop\n"
        "lui $t0, 0x140\n"
        "ctc2 $t0, $28\n"
        "nop\n"
        "ctc2 $zero, $24\n"
        "ctc2 $zero, $25\n"
        "nop\n"
        ".set noreorder\n"
        ::: "$2", "$3", "$8", "memory");
}
