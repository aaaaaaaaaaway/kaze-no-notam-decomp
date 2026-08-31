/**
 * kaze_memcard_save_draw  (menu, 0x80061258)
 *
 * Save-flow draw: message id per phase kaze_memory_card_save_state (0xc
 * confirm, 8 busy, 3 overwrite confirm, 10 saved-ok, 0xd no space, 4 write
 * failed, 0 no card) via kaze_slot_overlay_add_status_strip.
 */


extern unsigned char kaze_memory_card_save_state;

extern void kaze_slot_overlay_add_header_prims(void);
extern void kaze_slot_overlay_add_rows(int);
extern void kaze_slot_overlay_add_status_strip(int, int, int);
extern void kaze_drmove_addprim_a0ec(int, int);

void kaze_memcard_save_draw(int arg)
{
    kaze_slot_overlay_add_header_prims();
    kaze_slot_overlay_add_rows(arg);
    switch (kaze_memory_card_save_state) {
    case 0:
        kaze_slot_overlay_add_status_strip(arg, 0xc, 1);
        break;
    case 3:
        kaze_slot_overlay_add_status_strip(arg, 3, 1);
        break;
    case 1:
    case 2:
    case 4:
    case 5:
        kaze_slot_overlay_add_status_strip(arg, 8, 1);
        break;
    case 6:
        kaze_slot_overlay_add_status_strip(arg, 10, 0);
        break;
    case 7:
        kaze_slot_overlay_add_status_strip(arg, 0xd, 0);
        break;
    case 8:
        kaze_slot_overlay_add_status_strip(arg, 4, 1);
        break;
    case 9:
        kaze_slot_overlay_add_status_strip(arg, 0, 0);
        break;
    }
    kaze_drmove_addprim_a0ec(arg, 0xfff);
}
