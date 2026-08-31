/**
 * kaze_stage_staging_copy  (core, 0x8001a2c4)
 *
 * Copies the menu staging block
 * kaze_staged_stage_index..kaze_staged_stage_variant into the live stage
 * block kaze_stage_index..kaze_stage_variant during flight/map
 * initialization.
 */


#include "kaze_syms.h"
extern int kaze_staged_stage_mode;
extern int kaze_staged_stage_variant;
typedef struct {
    int a, b, c, d;
} Blk;

extern int kaze_state_base_ptr;
extern Blk kaze_staged_stage_index[];
extern Blk kaze_stage_index[];

void kaze_stage_staging_copy(void)
{
    Blk *dst;
    Blk *src;
    Blk *end;

    *(int *)(kaze_state_base_ptr + 0x24) = 1;
    switch (kaze_staged_stage_variant) {
    case 2:
        kaze_staged_stage_mode = 1;
        break;
    case 5:
        kaze_staged_stage_mode = 1;
        *(int *)(kaze_state_base_ptr + 0x24) = 3;
        break;
    case 9:
        kaze_staged_stage_mode = 1;
        break;
    case 1:
    case 4:
    case 7:
        kaze_staged_stage_mode = 0;
        break;
    case 3:
    case 6:
    case 8:
        kaze_staged_stage_mode = 2;
        break;
    }
    dst = kaze_stage_index;
    src = kaze_staged_stage_index;
    end = src + 3;
    do {
        *dst++ = *src++;
    } while (src != end);
    kaze_stage_map_cd_loader();
    kaze_lighting_env_frame_update();
}
