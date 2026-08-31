#include "types.h"
#include "kaze_syms.h"
extern int D_8008DBDC;
extern u8 kaze_title_screen_cleanup_flag;
extern int kaze_memcard_load_records(void);

extern u8 kaze_title_start_flow_state;

/**
 * kaze_title_check_savedata  (menu, 0x8004c460)
 *
 * Checks save presence via
 * kaze_memcard_load_records/kaze_title_screen_cleanup_flag, sets substate
 * kaze_title_start_flow_state, and builds the press-start sprite.
 */

void kaze_title_check_savedata(void)
{
    int r;
    int neg;

    r = kaze_memcard_load_records();
    neg = -1;
    D_8008DBDC = 0;
    if ((r == neg) && (kaze_title_screen_cleanup_flag == 0)) {
        r = 1;
    } else {
        r = 0xff;
    }
    kaze_title_start_flow_state = r;
    kaze_pressstart_sprite_setup();
}

extern int kaze_input_edge;

/**
 * kaze_title_wait_input  (menu, 0x8004c4bc)
 *
 * If any button was pressed (kaze_input_edge) advance title substate
 * kaze_title_start_flow_state to 0xff.
 */

void kaze_title_wait_input(void)
{
    if (kaze_input_edge != 0) {
        kaze_title_start_flow_state = 0xFF;
    }
}

extern u8 kaze_title_screen_state;

/**
 * kaze_title_goto_state2  (menu, 0x8004c4e0)
 *
 * Sets title substate 0 and advances state kaze_title_screen_state=2.
 */

void kaze_title_goto_state2(void)
{
    int two;

    two = 2;
    kaze_title_start_flow_state = 0;
    kaze_title_screen_state = two;
}

/**
 * kaze_title_pressstart_draw  (menu, 0x8004c4fc)
 *
 * Thin wrapper calling kaze_pressstart_sprite_addprim to AddPrim the press-
 * start sprite.
 */

void kaze_title_pressstart_draw(void)
{
    kaze_pressstart_sprite_addprim();
}
