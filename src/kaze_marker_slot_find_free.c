/**
 * kaze_marker_slot_find_free  (objective, 0x800218ec)
 *
 * Returns the index of the first free slot (state word 0) in the 10-slot
 * mode-0 marker table at +0x58a74 (stride 0x5c), or -1 when full.
 */


extern int kaze_state_base_ptr;

int kaze_marker_slot_find_free(void)
{
    int slot;
    int i = 0;
    int p = kaze_state_base_ptr;

    for (; i < 10; i++, p += 0x5C) {
        if (*(int *)(p + 0x58A74) == 0) {
            slot = i;
            goto check;
        }
    }
    slot = i;
check:
    i = slot < 10;
    if (i == 0) {
        slot = -1;
    }
    return slot;
}
