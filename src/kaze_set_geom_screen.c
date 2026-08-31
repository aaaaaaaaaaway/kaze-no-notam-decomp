/**
 * kaze_set_geom_screen  (library, 0x80070f28)
 *
 * PsyQ SetGeomScreen (REG13.OBJ): loads the GTE projection-distance register
 * H.
 */

void kaze_set_geom_screen(int h) {
    __asm__ volatile("ctc2 %0, $26" :: "r"(h));
}
