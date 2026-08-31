#include "types.h"
#include "kaze_syms.h"
extern char *kaze_xa_filename_table[];
extern int kaze_xa_file_index_table;
extern char kaze_xa_track_default_volume_table[];
extern int kaze_xa_track_length_table[];
typedef struct {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
} CdlLOC;
typedef struct {
    CdlLOC pos;
    unsigned int size;
    char name[16];
} CdlFILE;


/**
 * kaze_xa_file_table_init_331c  (sound, 0x8003331c)
 *
 * Builds the XA/streamed-audio CD file-location table at
 * kaze_xa_file_loc_table by resolving \BAL\STR\BALXA01.XA entries
 * (kaze_cd_file_lookup_cached) into packed 24-bit CdlLOC fields.
 */

void kaze_xa_file_table_init_331c(void)
{
    extern CdlLOC kaze_xa_file_loc_table;
    CdlFILE file;
    char **table;
    CdlLOC *p;
    unsigned int i;

    table = kaze_xa_filename_table;
    p = &kaze_xa_file_loc_table;
    i = 0;
    do {
        if (kaze_cd_file_lookup_cached(&file, table[*(int *)((char *)&kaze_xa_file_index_table + i)]) != 0) {
            *p = file.pos;
        }
        i += 0x10;
        p += 4;
    } while (i < 0x100);
}

extern int kaze_sound_state_base_ptr;

/**
 * kaze_xa_stream_enable_33b0  (sound, 0x800333b0)
 *
 * Enables CD/XA streaming: sets flag kaze_sound_state_base_ptr+0x14afc=1 and
 * calls PsyQ stream helper kaze_ss_set_stereo.
 */

void kaze_xa_stream_enable_33b0(void)
{
    *(int *)(kaze_sound_state_base_ptr + 0x14AFC) = 1;
    kaze_ss_set_stereo();
}

/**
 * kaze_xa_stream_disable_33e8  (sound, 0x800333e8)
 *
 * Disables CD/XA streaming: clears flag kaze_sound_state_base_ptr+0x14afc=0
 * and calls PsyQ stream helper kaze_ss_set_mono.
 */

void kaze_xa_stream_disable_33e8(void)
{
    *(int *)(kaze_sound_state_base_ptr + 0x14AFC) = 0;
    kaze_ss_set_mono();
}

extern int VSync(int);

/**
 * kaze_cd_setmode_vsync_341c  (sound, 0x8003341c)
 *
 * Issues a blocking CdControlB mode command (0x0e, param 200) followed by
 * VSync(5) during CD/XA audio setup.
 */

void kaze_cd_setmode_vsync_341c(void)
{
    extern int CdControlB(int, u8 *, u8 *);
    u8 param[8];

    param[0] = 200;
    CdControlB(0xE, param, (u8 *)0);
    VSync(5);
}

extern int CdPosToInt(CdlLOC *);

/**
 * kaze_xa_track_seek_setup_3454  (sound, 0x80033454)
 *
 * Resolves an XA track's CD location from kaze_xa_file_loc_table
 * (CdPosToInt), stores start/end sectors at +0x14b00..+0x14b08, and issues
 * CdControlB read/seek (0x0d/0x15) after kaze_cd_audio_set_volume_33c58
 * mutes.
 */

void kaze_xa_track_seek_setup_3454(int track)
{
    extern void CdControlB(int, u8 *, u8 *);
    extern CdlLOC kaze_xa_file_loc_table[];
    register int idx __asm__("$16");
    register int loc_off __asm__("$17");
    register int base __asm__("$2");
    register int post_base __asm__("$4");
    int pos;

    if (track != -1) {
        loc_off = 0x14AF4;
        base = kaze_sound_state_base_ptr;
        idx = track << 4;
        *(CdlLOC *)(base + loc_off) = *(CdlLOC *)(kaze_xa_file_loc_table + track * 4);
        post_base = kaze_sound_state_base_ptr;
        pos = CdPosToInt((CdlLOC *)(post_base + loc_off));
        post_base = kaze_sound_state_base_ptr;
        *(int *)(post_base + 0x14B00) = pos;
        *(int *)(post_base + 0x14B04) = *(int *)(post_base + 0x14B00);
        *(int *)(post_base + 0x14B08) = *(int *)(post_base + 0x14B00) + *(int *)(kaze_xa_track_length_table + track * 4);
        if (*(u8 *)(post_base + 0x14AF1) != kaze_xa_track_default_volume_table[idx]) {
            kaze_cd_audio_set_volume_33c58(0);
            *(u8 *)(kaze_sound_state_base_ptr + 0x14AF1) = *(int *)(kaze_xa_track_default_volume_table + idx);
            CdControlB(0xD, (u8 *)(kaze_sound_state_base_ptr + 0x14AF0), 0);
        }
        CdControlB(0x15, (u8 *)(kaze_sound_state_base_ptr + loc_off), 0);
    }
}

extern void CdControlF(int, int);

/**
 * kaze_xa_audio_manager_35a0  (sound, 0x800335a0)
 *
 * Per-frame XA audio manager keyed on current track +0x14b18: stops
 * (CdControlF pause) on -1, fades/polls the active track
 * (kaze_xa_audio_fade_in_33bac/kaze_xa_audio_loop_poll_33690), starts a new
 * track (kaze_xa_audio_start_track_3384c), or queues via
 * kaze_xa_audio_fade_out_33ae8.
 */

void kaze_xa_audio_manager_35a0(int arg0, int arg1)
{
    if (*(int *)(kaze_sound_state_base_ptr + 0x14B18) != -1 && *(int *)(kaze_sound_state_base_ptr + 0x14B18) != arg0) {
        kaze_xa_audio_fade_out_33ae8(arg1);
    } else if (arg0 == -1) {
        if (*(int *)(kaze_sound_state_base_ptr + 0x14B10) != 0) {
            CdControlF(9, 0);
            *(int *)(kaze_sound_state_base_ptr + 0x14B10) = 0;
            *(int *)(kaze_sound_state_base_ptr + 0x14B14) = 0;
        }
    } else if (arg0 == *(int *)(kaze_sound_state_base_ptr + 0x14B18)) {
        kaze_xa_audio_fade_in_33bac(arg1);
        kaze_xa_audio_loop_poll_33690(arg1);
    } else {
        kaze_xa_audio_start_track_3384c();
    }
}
