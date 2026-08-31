#include "types.h"
#include "kaze_syms.h"
extern char kaze_seq_vh_filename_fmt[];
extern char kaze_seq_vb_filename_fmt[];
extern char kaze_seq_sep_filename_fmt[];
extern char kaze_seq_vab_name_table[];
extern char kaze_seq_sep_name_table[];
extern char kaze_seq_id_table[];
extern char kaze_seq_vab_id_table[];
extern int kaze_sfx_note_table[];
extern void SsSepClose(int);
extern s16 SsSepOpen(void *, s16, s16);
extern void SsSepPlay(s16, s16, int, s16);
extern void SsUtAllKeyOff(int);
extern void SsVabClose(int);
extern s16 SsVabOpenHeadSticky(void *, int, unsigned int);
extern int SsVabTransBody(u8 *, s16);
extern void SsVabTransCompleted(int);
extern void SsVoKeyOff(int, int);
extern void SsVoKeyOn(unsigned int, int, int, int);

/* "%s.VH" */
/* "%s.VB" */
/* "%s.SEP" */
/* VH/VB name ptr, stride 0x10 */
/* SEP name ptr */
/* seq id */
/* vab id */
void kaze_seq_vab_bank_close_33f58();
extern void kaze_cd_load_file_into(char *, void *, int);
extern char *kaze_cd_load_file_alloc(char *);
extern void kaze_heap_free(void *);
void kaze_seq_reset_track_volumes_34048();
extern void sprintf(char *, char *, char *);
extern void bzero(void *, int);

/**
 * kaze_seq_vab_bank_load_33d2c  (sound, 0x80033d2c)
 *
 * Loads a per-map SEQ/VAB sound bank from BAL\MAP\BALOON tables: opens VH
 * (SsVabOpenHeadSticky), transfers VB body (SsVabTransBody), opens the SEP
 * sequence (SsSepOpen), storing handles at +0x14ac4/+0x14ac6.
 */

void kaze_seq_vab_bank_load_33d2c(int bank)
{
    extern int kaze_sound_state_base_ptr;
    int base;
    s16 handle;
    u16 trans_status;
    u8 *vb_buf;
    register int tbl_ofs __asm__("$17"); /* bank*0x10 lives in $s1 across all table reads */
    char filename[256];

    if (*(int *)(kaze_sound_state_base_ptr + 0x14AC0) != bank) {
        tbl_ofs = bank * 0x10;
        kaze_seq_vab_bank_close_33f58();
        sprintf(filename, kaze_seq_vh_filename_fmt, *(char **)(kaze_seq_vab_name_table + tbl_ofs));
        kaze_cd_load_file_into(filename, (void *)(kaze_sound_state_base_ptr + 0xAC0), 0);
        handle = SsVabOpenHeadSticky((u8 *)(kaze_sound_state_base_ptr + 0xAC0), -1,
                                    *(unsigned int *)(kaze_seq_vab_id_table + tbl_ofs));
        *(s16 *)(kaze_sound_state_base_ptr + 0x14AC4) = handle;
        if (handle != -1) {
            sprintf(filename, kaze_seq_vb_filename_fmt, *(char **)(kaze_seq_vab_name_table + tbl_ofs));
            vb_buf = (u8 *)kaze_cd_load_file_alloc(filename);
            trans_status = (u16)SsVabTransBody(vb_buf, *(s16 *)(kaze_sound_state_base_ptr + 0x14AC4));
            if (-1 < (int)((unsigned int)trans_status << 0x10)) {
                SsVabTransCompleted(1);
            }
            kaze_heap_free(vb_buf);
            base = kaze_sound_state_base_ptr;
            if (*(s16 *)(kaze_sound_state_base_ptr + 0x14AC4) != -1) {
                *(int *)(kaze_sound_state_base_ptr + 0x14AC0) = bank;
                if (*(char **)(kaze_seq_sep_name_table + tbl_ofs) != 0) {
                    bzero((void *)(base + 0xAAC0), 0xA000);
                    sprintf(filename, kaze_seq_sep_filename_fmt, *(char **)(kaze_seq_sep_name_table + tbl_ofs));
                    kaze_cd_load_file_into(filename, (void *)(kaze_sound_state_base_ptr + 0xAAC0), 0);
                    handle = SsSepOpen((void *)(kaze_sound_state_base_ptr + 0xAAC0),
                                      *(s16 *)(kaze_sound_state_base_ptr + 0x14AC4),
                                      *(s16 *)(kaze_seq_id_table + tbl_ofs));
                    *(s16 *)(kaze_sound_state_base_ptr + 0x14AC6) = handle;
                    if (handle != -1) {
                        *(int *)(kaze_sound_state_base_ptr + 0x14AC8) = *(int *)(kaze_seq_id_table + tbl_ofs);
                        kaze_seq_reset_track_volumes_34048();
                    }
                }
            }
        }
    }
}

void kaze_seq_stop_track_34310();

/**
 * kaze_seq_vab_bank_close_33f58  (sound, 0x80033f58)
 *
 * Closes the current SEQ/VAB sound bank: SsSepClose the SEP handle +0x14ac6,
 * SsUtAllKeyOff, SsVabClose the VAB handle +0x14ac4, and resets bank id
 * +0x14ac0.
 */

void kaze_seq_vab_bank_close_33f58(void)
{
    extern int kaze_sound_state_base_ptr;
    if (*(s16 *)(kaze_sound_state_base_ptr + 0x14AC6) != -1) {
        kaze_seq_stop_track_34310(-1);
        SsSepClose(*(s16 *)(kaze_sound_state_base_ptr + 0x14AC6));
        *(s16 *)(kaze_sound_state_base_ptr + 0x14AC6) = -1;
        *(int *)(kaze_sound_state_base_ptr + 0x14AC8) = 0;
    }
    if (*(s16 *)(kaze_sound_state_base_ptr + 0x14AC4) != -1) {
        SsUtAllKeyOff(0);
        SsVabClose(*(s16 *)(kaze_sound_state_base_ptr + 0x14AC4));
        *(s16 *)(kaze_sound_state_base_ptr + 0x14AC4) = -1;
        *(int *)(kaze_sound_state_base_ptr + 0x14AC0) = -1;
    }
}

/**
 * kaze_seq_reset_track_volumes_34048  (sound, 0x80034048)
 *
 * Resets all SEP sequence track volumes to 0 (SsSepSetVol loop over +0x14ac8
 * tracks) and clears the per-track volume cache at +0x14acc.
 */

void kaze_seq_reset_track_volumes_34048(void)
{
    extern void SsSepSetVol(s16, s16, int, int);
    extern int kaze_sound_state_base_ptr;
    int t;
    int b;
    int c;
    int i;
    int pad[2];

    b = kaze_sound_state_base_ptr;
    if (*(s16 *)(b + 0x14ac6) != -1) {
        i = 0;
        if (0 < *(int *)(b + 0x14ac8)) {
            do {
                SsSepSetVol(*(s16 *)(b + 0x14ac6), (s16)i, 0, 0);
                t = i * 2;
                c = kaze_sound_state_base_ptr;
                i = i + 1;
                *(u16 *)(t + c + 0x14acc) = 0;
                b = c;
            } while (i < *(int *)(c + 0x14ac8));
        }
    }
}

void kaze_sfx_voice_keyoff_34170();

/**
 * kaze_sfx_voice_keyon_34108  (sound, 0x80034108)
 *
 * Triggers a one-shot SFX voice via SsVoKeyOn on the current VAB (+0x14ac4)
 * and note table kaze_sfx_note_table; optionally keys off first
 * (kaze_sfx_voice_keyoff_34170).
 */

void kaze_sfx_voice_keyon_34108(unsigned int num, int stop)
{
    extern char *kaze_sound_state_base_ptr;
    if (stop != 0) {
        kaze_sfx_voice_keyoff_34170();
    }
    SsVoKeyOn((*(s16 *)(kaze_sound_state_base_ptr + 0x14ac4) << 8) | num, kaze_sfx_note_table[num], 100, 100);
}

/**
 * kaze_sfx_voice_keyoff_34170  (sound, 0x80034170)
 *
 * Keys off an SFX voice via SsVoKeyOff on the current VAB handle +0x14ac4
 * and note table kaze_sfx_note_table.
 */

void kaze_sfx_voice_keyoff_34170(u32 arg0)
{
    extern int kaze_sound_state_base_ptr;
    SsVoKeyOff((*(short *)(kaze_sound_state_base_ptr + 0x14AC4) << 8) | arg0, kaze_sfx_note_table[arg0]);
}

/**
 * kaze_play_sound_effect  (sound, 0x800341b8)
 *
 * Plays a sound effect: sets SEP track volume (SsSepSetVol, cached at
 * +0x14acc) and starts/replays the track (SsSepStop/SsSepPlay) on the
 * current SEP handle +0x14ac6. Anchor: plays a sound effect.
 */

void kaze_play_sound_effect(short id, int vol_arg, short pitch, int restart_arg)
{
    extern void SsSepStop(s16, s16);
    extern void SsSepSetVol(s16, s16, s16, s16);
    extern int kaze_sound_state_base_ptr;
    register int restart __asm__("$17");
    register short id_reg __asm__("$16");
    register short pitch_reg __asm__("$18");
    register int vol __asm__("$8");
    register int clipped __asm__("$6");
    int base;
    int sid;
    int set_base;

    restart = restart_arg;
    base = kaze_sound_state_base_ptr;
    id_reg = id;
    pitch_reg = pitch;
    vol = vol_arg;
    if ((*(short *)(kaze_sound_state_base_ptr + 0x14ac6) != -1) &&
        (sid = (int)id, sid >= 0) &&
        (sid < *(int *)(kaze_sound_state_base_ptr + 0x14ac8))) {
        if (*(short *)(sid * 2 + kaze_sound_state_base_ptr + 0x14acc) != (short)vol_arg) {
            if ((short)vol < 0) {
                goto low_volume;
            }
            clipped = vol;
            if ((short)vol < 0x80) {
                goto set_volume;
            }
            clipped = 0x7f;
            goto set_volume;
low_volume:
            clipped = 0;
set_volume:
            vol = clipped;
            set_base = kaze_sound_state_base_ptr;
            *(short *)(id_reg * 2 + set_base + 0x14acc) = vol;
            SsSepSetVol(*(short *)(set_base + 0x14ac6), id_reg, vol, vol);
        }
        if (restart != 0) {
            SsSepStop(*(short *)(kaze_sound_state_base_ptr + 0x14ac6), id_reg);
        }
        SsSepPlay(*(short *)(kaze_sound_state_base_ptr + 0x14ac6), id_reg, 1, pitch_reg);
    }
}

/**
 * kaze_seq_stop_track_34310  (sound, 0x80034310)
 *
 * Stops SEP sequence track(s): for param<0 stops all +0x14ac8 tracks, else
 * the given track, zeroing volume cache +0x14acc and calling
 * SsSepSetVol(0)/SsSepStop.
 */

void kaze_seq_stop_track_34310(int id)
{
    extern void SsSepStop(s16, int);
    extern void SsSepSetVol(s16, int, int, int);
    extern int kaze_sound_state_base_ptr;
    int pad;
    register int base __asm__("$8");
    register int neg __asm__("$2");
    register int id_tmp __asm__("$2");
    register int id_reg __asm__("$16");
    register int vol_off __asm__("$17");
    register int vol_base __asm__("$18");
    register int loop_base __asm__("$7");
    int shifted;
    int limit;
    s16 track;

    base = kaze_sound_state_base_ptr;
    neg = -1;
    /* Non-emitting stack/base/neg operands shape frame size and early liveness. */
    __asm__("" : : "m"(pad), "r"(base), "r"(neg));
    if (*(s16 *)(base + 0x14AC6) != neg) {
        loop_base = base;
        /* Non-emitting loop-base barrier tests target's first branch delay slot. */
        __asm__("" : : "r"(loop_base));
        id_tmp = id << 16;
        id_reg = id_tmp >> 16;
        if (id_reg < 0) {
            if (*(int *)(base + 0x14AC8) > 0) {
                vol_off = 0;
                vol_base = 0x14ACC;
                /* Non-emitting counter barrier prevents folding the target preheader shift. */
                __asm__("" : "=r"(vol_off) : "0"(vol_off));
                shifted = vol_off << 16;
                do {
                    register int vol_arg __asm__("$5");
                    register int zero_arg __asm__("$6");

                    shifted = shifted >> 16;
                    vol_arg = shifted;
                    zero_arg = 0;
                    /* Non-emitting arg barrier keeps call arguments live before the store. */
                    __asm__("" : : "r"(vol_arg), "r"(zero_arg));
                    *(s16 *)(loop_base + vol_off * 2 + vol_base) = 0;
                    track = *(s16 *)(loop_base + 0x14AC6);
                    SsSepSetVol(track, vol_arg, zero_arg, 0);
                    SsSepStop(*(s16 *)(kaze_sound_state_base_ptr + 0x14AC6), shifted);
                    loop_base = kaze_sound_state_base_ptr;
                    limit = *(int *)(loop_base + 0x14AC8);
                    /*
                     * O2 otherwise hoists vol_off++ ahead of this load, adding
                     * a nop before the loop slt. This zero-code barrier keeps
                     * the original load-delay-slot schedule.
                     */
                    __asm__ __volatile__("" : : "r"(limit));
                    vol_off++;
                    shifted = vol_off << 16;
                } while (vol_off < limit);
            }
        } else {
            register int arg_id __asm__("$5");
            register int zero_arg __asm__("$6");

            arg_id = id_reg;
            zero_arg = 0;
            /* Non-emitting arg barrier tests target's positive-path arg setup. */
            __asm__("" : : "r"(arg_id), "r"(zero_arg));
            id_tmp = id_reg << 1;
            id_tmp = id_tmp + base;
            *(s16 *)(id_tmp + 0x14ACC) = 0;
            SsSepSetVol(*(s16 *)(base + 0x14AC6), arg_id, zero_arg, 0);
            arg_id = id_reg;
            SsSepStop(*(s16 *)(kaze_sound_state_base_ptr + 0x14AC6), arg_id);
        }
    }
}
