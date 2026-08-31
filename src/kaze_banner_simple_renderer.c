/**
 * kaze_banner_simple_renderer  (hud, 0x80032134)
 *
 * Trimmed banner renderer drawing a single SPRT (+0x49ec8) with x0 0x20 and
 * y0 from root +0x8, plus optional confirm box and background TPAGE.
 */

#include "types.h"
#include "kaze_syms.h"
extern char *kaze_state_base_ptr;

extern void AddPrim(void *, void *);

void kaze_banner_simple_renderer(void *ot_arg)
{
    char *base;
    register int row asm("$16");
    register void *ot asm("$17");
    int tmp;
    char *prim;
    register int color asm("$5");

    ot = ot_arg;
    row = *(int *)(kaze_state_base_ptr + 8);
    if (row >= 0) {
        tmp = 0xd;
        if (row < 0xe) {
            tmp = row;
        }
    } else {
        tmp = 0;
    }
    row = tmp;

    if (*(int *)(kaze_state_base_ptr + 8) == 0xe) {
        kaze_confirm_box_renderer(ot, 0xa0, 0xb8);
    }

    {
        register char *banner_base asm("$6");
        register int banner_ofs asm("$7");

        banner_base = kaze_state_base_ptr;
        banner_ofs = 0x49ec8;
        *(u16 *)(banner_base + *(int *)(banner_base + 0x98) * 0x14 + 0x49ed0) = 0x20;
        *(short *)(banner_base + *(int *)(banner_base + 0x98) * 0x14 + 0x49ed2) = 0x68 - ((0xd - row) << 4);
        AddPrim(ot, banner_base + (*(int *)(banner_base + 0x98) * 0x14 + banner_ofs));
    }
    AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x4a108));

    base = kaze_state_base_ptr;
    prim = base + *(int *)(base + 0x98) * 0x18;
    color = row << 4;
    *(u8 *)(prim + 0x4889a) = color;
    *(u8 *)(prim + 0x48899) = color;
    *(u8 *)(prim + 0x48898) = color;
    AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x18 + 0x48894));
    AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x488c4));
}
