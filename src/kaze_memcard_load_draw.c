/**
 * kaze_memcard_load_draw  (menu, 0x80061184)
 *
 * Load-flow draw: message id per phase kaze_memory_card_load_state (0xb
 * confirm, 8 busy, 10 done, 6 no save file, 7 retry confirm, 0xe read error,
 * 0 no card) via kaze_slot_overlay_add_status_strip.
 */


extern unsigned char kaze_memory_card_load_state;

extern void kaze_slot_overlay_add_header_prims(void);
extern void kaze_slot_overlay_add_rows(int);
extern void kaze_slot_overlay_add_status_strip(int, int, int);
extern void kaze_drmove_addprim_a0ec(int, int);

void kaze_memcard_load_draw(int arg)
{
    kaze_slot_overlay_add_header_prims();
    kaze_slot_overlay_add_rows(arg);
    switch (kaze_memory_card_load_state) {
    case 0:
        kaze_slot_overlay_add_status_strip(arg, 0xb, 1);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        kaze_slot_overlay_add_status_strip(arg, 8, 1);
        break;
    case 5:
        kaze_slot_overlay_add_status_strip(arg, 10, 0);
        break;
    case 6:
        kaze_slot_overlay_add_status_strip(arg, 6, 0);
        break;
    case 7:
        kaze_slot_overlay_add_status_strip(arg, 7, 1);
        break;
    case 8:
        kaze_slot_overlay_add_status_strip(arg, 0xe, 0);
        break;
    case 9:
        kaze_slot_overlay_add_status_strip(arg, 0, 0);
        break;
    }
    kaze_drmove_addprim_a0ec(arg, 0xfff);
}
