/**
 * kaze_memcard_format_draw  (menu, 0x800610c8)
 *
 * Format-flow draw: message id per phase kaze_memcard_format_phase (1
 * confirm, 8 busy, 10 formatted-ok, 5 failed, 0 no card) rendered via
 * kaze_slot_overlay_add_status_strip with confirm-cursor flag.
 */


extern unsigned char kaze_memcard_format_phase;

extern void kaze_slot_overlay_add_header_prims(void);
extern void kaze_slot_overlay_add_rows(int);
extern void kaze_slot_overlay_add_status_strip(int, int, int);
extern void kaze_drmove_addprim_a0ec(int, int);

void kaze_memcard_format_draw(int arg)
{
    kaze_slot_overlay_add_header_prims();
    kaze_slot_overlay_add_rows(arg);
    switch (kaze_memcard_format_phase) {
    case 0:
        kaze_slot_overlay_add_status_strip(arg, 1, 1);
        break;
    case 1:
    case 2:
    case 3:
        kaze_slot_overlay_add_status_strip(arg, 8, 1);
        break;
    case 4:
        kaze_slot_overlay_add_status_strip(arg, 10, 0);
        break;
    case 5:
        kaze_slot_overlay_add_status_strip(arg, 0, 1);
        break;
    case 6:
        kaze_slot_overlay_add_status_strip(arg, 5, 0);
        break;
    }
    kaze_drmove_addprim_a0ec(arg, 0xfff);
}
