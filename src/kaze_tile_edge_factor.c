/**
 * kaze_tile_edge_factor  (render, 0x8002f9a4)
 *
 * Computes a 0..0xed sub-tile edge factor from balloon position in its tile
 * (pos%0xed8>>4), mirrored/halved by direction-mask bits (X:
 * 0x80/0x10/0x40/0x20, Z: 8/1/4/2), returns the max; for terrain seam
 * blending.
 */


#include "types.h"
struct KazeState {
    char pad0[0x68];
    int x;
    char pad1[4];
    int z;
};

extern struct KazeState *kaze_state_base_ptr;

int kaze_tile_edge_factor(uint flags)
{
    int x;
    int z;
    register uint zflags __asm__("$3");
    register int ret __asm__("$2");

    x = kaze_state_base_ptr->x % 0xED8 >> 4;
    z = kaze_state_base_ptr->z % 0xED8 >> 4;
    __asm__("" : "=r"(z) : "0"(z));
    zflags = flags;
    if ((flags & 0x80) != 0) {
        if ((flags & 0x10) != 0) {
            x = 0xED - x;
        }
        if ((flags & 0x40) != 0) {
            x >>= 1;
        }
        if ((flags & 0x20) != 0) {
            x += 0x76;
        }
    } else {
        x = 0;
    }
    if ((zflags & 8) != 0) {
        if ((zflags & 1) != 0) {
            z = 0xED - z;
        }
        if ((zflags & 4) != 0) {
            z >>= 1;
        }
        ret = zflags & 2;
        if (ret != 0) {
            z += 0x76;
        }
    } else {
        z = 0;
    }
    ret = z;
    if (ret <= x) {
        ret = x;
    }
    return ret;
}
