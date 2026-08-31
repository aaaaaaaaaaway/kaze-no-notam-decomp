/**
 * kaze_burner_sound_toggle  (sound, 0x8001fef4)
 *
 * Latches the burner loop sound on/off via the flag at live-root +0x481f8,
 * calling kaze_sfx_voice_keyon_34108(0,0) to start and
 * kaze_sfx_voice_keyoff_34170(0) to stop.
 */


extern char *kaze_state_base_ptr;
extern void kaze_sfx_voice_keyon_34108(int, int);
extern void kaze_sfx_voice_keyoff_34170(int);

void kaze_burner_sound_toggle(int enable)
{
    if (enable != 0) {
        if (*(int *)(kaze_state_base_ptr + 0x481F8) == 0) {
            kaze_sfx_voice_keyon_34108(0, 0);
            *(int *)(kaze_state_base_ptr + 0x481F8) = 1;
        }
    } else {
        if (*(int *)(kaze_state_base_ptr + 0x481F8) != 0) {
            kaze_sfx_voice_keyoff_34170(0);
            *(int *)(kaze_state_base_ptr + 0x481F8) = 0;
        }
    }
}
