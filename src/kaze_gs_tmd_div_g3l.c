/**
 * kaze_gs_tmd_div_g3l  (library, 0x800732f4)
 *
 * PsyQ GsTMDdivG3L (DVG3_00.OBJ): active-subdivision renderer for gouraud-
 * shaded TMD triangles, emitting divided packets into the work area.
 */

#include "types.h"
#include "kaze_syms.h"
extern int kaze_gs_tmd_semi_flag;
extern void *RCpolyG3A(void *, void *, int, void *);
extern int RotAverageNclipColorDpq3_1( void *, void *, void *, void *, void *, void *, void *, long *, long *, long *, void *, void *, void *, u16 *, long *);
extern void kaze_read_sz_fifo3(long *, long *, long *);
typedef struct {
    u8 hdr[0x13];
    u8 rgbc_cd;
    u32 tag;
} WorkHdr;

void *kaze_gs_tmd_div_g3l(u8 *poly, u8 *verts, u8 *normals, void *packet,
                          u32 count, u32 shift, int ot_ctx, u8 *work)
{
    u16 otz;
    long flag;
    u8 *next;
    u8 *v0 = work + 0x18;
    u8 *v1 = work + 0x30;
    u8 *v2 = work + 0x48;
    u32 i;

    *(u8 **)(work + 0xa8) = v0;
    *(u8 **)(work + 0xac) = v1;
    *(u8 **)(work + 0xb0) = v2;
    next = work + 0x60;
    i = 0;
    if (i < count) {
        do {
            u8 *p = poly + 3;
            __builtin_memcpy(v0, (u8 *)((*(u16 *)(p + 7) << 3) + (u32)verts), 8);
            __builtin_memcpy(v1, (u8 *)((*(u16 *)(p + 11) << 3) + (u32)verts), 8);
            __builtin_memcpy(v2, (u8 *)((*(u16 *)(p + 15) << 3) + (u32)verts), 8);
            if (RotAverageNclipColorDpq3_1(
                    v0, v1, v2,
                    normals + (*(u16 *)(p + 5) << 3),
                    normals + (*(u16 *)(p + 9) << 3),
                    normals + (*(u16 *)(p + 13) << 3),
                    poly + 4,
                    (long *)(v0 + 0x10), (long *)(v1 + 0x10),
                    (long *)(v2 + 0x10),
                    v0 + 0xc, v1 + 0xc, v2 + 0xc,
                    &otz, &flag) > 0) {
                u32 depth;
                int c60;
                kaze_read_sz_fifo3((long *)(v0 + 0x14),
                                   (long *)(v1 + 0x14),
                                   (long *)(v2 + 0x14));
                c60 = kaze_gs_tmd_semi_flag;
                depth = ((u32)otz - *(int *)(ot_ctx + 8)) >> shift;
                ((WorkHdr *)work)->tag = *(int *)(ot_ctx + 4) + depth * 4;
                ((WorkHdr *)work)->rgbc_cd = *p | (u8)(c60 << 1);
                packet = RCpolyG3A(packet, work, 0, next);
            }
            poly += 0x14;
            i++;
        } while (i < count);
    }
    return packet;
}
