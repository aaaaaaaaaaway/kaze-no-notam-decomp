/**
 * kaze_task_menu_prim_init  (menu, 0x8005cdfc)
 *
 * Task-menu primitive init wrapper: kaze_balloon_select_row_sprites_init
 * builds the 5 task-row SPRTs/TPAGEs at kaze_balloon_name_row_sprites and
 * kaze_balloon_entry_grid_sprites_init builds the 5x3 name letter-grid
 * SPRT_16s at kaze_balloon_entry_grid_sprites.
 */


#include "kaze_syms.h"
extern int kaze_balloon_entry_grid_sprites_init(void);

void kaze_task_menu_prim_init(void)
{
    kaze_balloon_select_row_sprites_init();
    kaze_balloon_entry_grid_sprites_init();
}
