/**
 * kaze_set_geom_offset  (library, 0x80070f08)
 *
 * PsyQ SetGeomOffset (REG12.OBJ): loads the GTE screen-offset registers
 * OFX/OFY (values <<16).
 */


void kaze_set_geom_offset(int ofx, int ofy) {
    ofx = ofx << 16;
    ofy = ofy << 16;
    __asm__ volatile("ctc2 %0, $24" :: "r"(ofx));
    __asm__ volatile("ctc2 %0, $25" :: "r"(ofy));
}
