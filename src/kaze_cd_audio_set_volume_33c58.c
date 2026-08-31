/**
 * kaze_cd_audio_set_volume_33c58  (sound, 0x80033c58)
 *
 * Sets CD-audio mix volume: clamps param to 0..0x80, writes the CdlATV L/R
 * fields +0x14af8/+0x14afa, mirrors to +0x14b0c, and applies via CdMix
 * (zeroed when mute flag +0x14afc set).
 */


#include "kaze_syms.h"
extern void CdMix(void *);
extern int kaze_sound_state_base_ptr;

void kaze_cd_audio_set_volume_33c58(int vol)
{
    int base0;
    register int base1 asm("$3");
    register int clipped asm("$3");
    register int cdvol asm("$4");
    register int byte asm("$5");
    register int mixoff asm("$2");

    if (vol < 0) {
        goto clip_low;
    }
    clipped = 0x80;
    if (vol >= 0x81) {
        goto clip_done;
    }
    clipped = vol;
    goto clip_done;
clip_low:
    clipped = 0;
clip_done:

    asm("addu %0,%1,$zero" : "=r"(cdvol) : "r"(clipped));
    base0 = kaze_sound_state_base_ptr;
    asm("addu %0,%1,$zero" : "=r"(byte) : "r"(cdvol));
    *(unsigned char *)(kaze_sound_state_base_ptr + 0x14AFA) = byte;
    *(unsigned char *)(base0 + 0x14AF8) = byte;
    base1 = kaze_sound_state_base_ptr;
    *(int *)(base0 + 0x14B0C) = cdvol;
    if (*(int *)(base1 + 0x14AFC) != 0) {
        mixoff = 0x10000;
        *(unsigned char *)(base1 + 0x14AFB) = 0;
        *(unsigned char *)(base1 + 0x14AF9) = 0;
    } else {
        mixoff = 0x10000;
        *(unsigned char *)(base1 + 0x14AFB) = byte;
        *(unsigned char *)(base1 + 0x14AF9) = byte;
    }
    cdvol = kaze_sound_state_base_ptr;
    mixoff |= 0x4AF8;
    CdMix((void *)(cdvol + mixoff));
}
