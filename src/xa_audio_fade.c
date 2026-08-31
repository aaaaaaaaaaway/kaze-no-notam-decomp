#include "types.h"
#include "kaze_syms.h"
extern char kaze_xa_track_default_volume_table[];
extern int kaze_xa_track_length_table[];
typedef struct {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
} CdlLOC;

extern int kaze_sound_state_base_ptr;
extern CdlLOC kaze_xa_file_loc_table[];
extern int CdPosToInt(CdlLOC *);
void kaze_xa_audio_seek_resume_339a8();

/**
 * kaze_xa_audio_start_track_3384c  (sound, 0x8003384c)
 *
 * Starts an XA audio track: sets current track +0x14b18, resolves its CD
 * start/end sectors, and issues CdControlF read (0x0d) after muting via
 * kaze_cd_audio_set_volume_33c58.
 */

void kaze_xa_audio_start_track_3384c(int track)
{
    extern void CdControlF(int, u8 *);
    register int track_save __asm__("$16") = track;
    register int idx __asm__("$17");
    register int *base_v0 __asm__("$2");
    register int *base_v1 __asm__("$3");
    int sector;
    int len;

    if (track_save != -1) {
        idx = track_save << 4;
        base_v0 = (int *)(kaze_sound_state_base_ptr + 0x14af4);
        *(CdlLOC *)base_v0 = *(CdlLOC *)(kaze_xa_file_loc_table + track_save * 4);
        sector = CdPosToInt((CdlLOC *)(kaze_sound_state_base_ptr + 0x14af4));
        base_v1 = (int *)kaze_sound_state_base_ptr;
        *(int *)((char *)base_v1 + 0x14b00) = sector;
        *(int *)((char *)base_v1 + 0x14b04) = sector;
        len = kaze_xa_track_length_table[track_save * 4];
        *(int *)((char *)base_v1 + 0x14b18) = track_save;
        *(int *)((char *)base_v1 + 0x14b1c) = 0x1e;
        *(int *)((char *)base_v1 + 0x14b08) = *(int *)((char *)base_v1 + 0x14b00) + len;
        if (*(u8 *)((char *)base_v1 + 0x14af1) != kaze_xa_track_default_volume_table[idx]) {
            kaze_cd_audio_set_volume_33c58(0);
            *(u8 *)(kaze_sound_state_base_ptr + 0x14af1) = *(int *)(kaze_xa_track_default_volume_table + idx);
            CdControlF(0xd, (u8 *)(kaze_sound_state_base_ptr + 0x14af0));
        } else {
            kaze_xa_audio_seek_resume_339a8();
        }
    }
}

/**
 * kaze_xa_audio_seek_resume_339a8  (sound, 0x800339a8)
 *
 * Seeks/resumes the active XA track: sets target volume +0x14b0c=0x7f, seeks
 * (CdControlF 0x15) to +0x14b00, and sets the playing flag +0x14b10=1.
 */

void kaze_xa_audio_seek_resume_339a8(void)
{
    extern void CdControlF(u8, u8 *);
    int base;
    int base2;

    base = kaze_sound_state_base_ptr;
    if (*(int *)(base + 0x14B18) != -1) {
        *(int *)(base + 0x14B0C) = 0x7F;
        *(int *)(base + 0x14B04) = *(int *)(base + 0x14B00);
        CdControlF(0x15, (u8 *)(base + 0x14AF4));
        base2 = kaze_sound_state_base_ptr;
        *(int *)(base2 + 0x14B1C) = 0;
        *(int *)(base2 + 0x14B10) = 1;
    }
}

extern void CdControl(int, int, int);

/**
 * kaze_xa_audio_stop_33a3c  (sound, 0x80033a3c)
 *
 * Stops XA audio playback: CdControl pause/standby (0x09/0x08), clears
 * volume/playing flags +0x14b0c/+0x14b10/+0x14b14, and resets current track
 * +0x14b18 to -1.
 */

void kaze_xa_audio_stop_33a3c(void)
{
    int neg1 = -1;

    if (*(int *)(kaze_sound_state_base_ptr + 0x14b18) != neg1) {
        CdControl(9, 0, 0);
        CdControl(8, 0, 0);
        *(int *)(kaze_sound_state_base_ptr + 0x14b0c) = 0;
        *(int *)(kaze_sound_state_base_ptr + 0x14b14) = 0;
        *(int *)(kaze_sound_state_base_ptr + 0x14b10) = 0;
    }
    *(int *)(kaze_sound_state_base_ptr + 0x14b18) = neg1;
    *(u8 *)(kaze_sound_state_base_ptr + 0x14af1) = 0xff;
}

/**
 * kaze_xa_audio_fade_out_33ae8  (sound, 0x80033ae8)
 *
 * XA audio fade-out step: decreases the CD-audio volume +0x14b0c toward 0
 * (clamped 0..0x7f) via kaze_cd_audio_set_volume_33c58; releases the track
 * when it reaches 0.
 */

void kaze_xa_audio_fade_out_33ae8(int arg)
{
    int b2;
    int b3;
    int b4;
    int v;
    int vol;

    vol = *(int *)(kaze_sound_state_base_ptr + 0x14b0c);
    if (0 < vol) {
        vol = vol - 5;
        arg = vol - arg;
        *(int *)(kaze_sound_state_base_ptr + 0x14b0c) = arg;
        b2 = kaze_sound_state_base_ptr;
        if (arg >= 0) {
            v = 0x7f;
            if (arg < 0x80) {
                v = arg;
            }
        } else {
            v = 0;
        }
        b3 = kaze_sound_state_base_ptr;
        *(int *)(b2 + 0x14b0c) = v;
        kaze_cd_audio_set_volume_33c58(*(int *)(b3 + 0x14b0c));
    } else {
        kaze_cd_audio_set_volume_33c58(0);
        b4 = kaze_sound_state_base_ptr;
        *(int *)(b4 + 0x14b18) = -1;
        *(int *)(b4 + 0x14b14) = 0;
    }
}

/**
 * kaze_xa_audio_fade_in_33bac  (sound, 0x80033bac)
 *
 * XA audio fade-in step: increases the CD-audio volume +0x14b0c toward 0x7f
 * (clamped) and applies it via kaze_cd_audio_set_volume_33c58 while the play
 * flag +0x14b14 is set.
 */

void kaze_xa_audio_fade_in_33bac(int arg)
{
    int v;
    int t;
    int root;
    int u;
    int sum;

    if (*(int *)(kaze_sound_state_base_ptr + 0x14b14) != 0) {
        u = *(int *)(kaze_sound_state_base_ptr + 0x14b0c);
        if (u < 0x7f) {
            t = u + 5;
            sum = t + arg;
            *(int *)(kaze_sound_state_base_ptr + 0x14b0c) = sum;
            root = kaze_sound_state_base_ptr;
            if (sum >= 0) {
                v = 0x7f;
                if (sum < 0x80) {
                    v = sum;
                }
            } else {
                v = 0;
            }
            *(int *)(root + 0x14b0c) = v;
        }
        kaze_cd_audio_set_volume_33c58(*(int *)(kaze_sound_state_base_ptr + 0x14b0c));
    }
}
