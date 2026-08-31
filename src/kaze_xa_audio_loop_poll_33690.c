/**
 * kaze_xa_audio_loop_poll_33690  (sound, 0x80033690)
 *
 * XA audio playback state machine: every 0x1f frames polls CdSync/CdLastCom
 * and re-issues seek/read/loop commands (0x11/0x15/0x1b) to keep the
 * streamed track between +0x14b04 and +0x14b08.
 */

#include "types.h"
#include "kaze_syms.h"
extern int CdLastCom(void);
extern int CdSync(int, void *);
extern int kaze_sound_state_base_ptr;

extern int CdPosToInt(void *);
extern void CdControlF(int, void *);

void kaze_xa_audio_loop_poll_33690(int arg0)
{
    int base;
    int sync;
    register int dispatch __asm__("$3");
    register int com __asm__("$4");
    int timer;
    register int should_seek __asm__("$4");
    void *param;
    u8 status[8];

    base = kaze_sound_state_base_ptr;
    timer = *(int *)(base + 0x14b1c);
    timer = timer + arg0;
    *(int *)(base + 0x14b1c) = timer;
    if (timer < 0x1f) {
        return;
    }
    *(int *)(base + 0x14b1c) = 0;
    sync = CdSync(1, status);
    dispatch = CdLastCom();
    if (sync == 2) {
        goto sync_complete;
    }
    if (sync != 5) {
        return;
    }
    switch (dispatch) {
    case 1:
    case 0x11:
        com = 1;
        param = 0;
        goto control_call;
    case 0xd:
    {
        register int ofs __asm__("$2");
        register int b __asm__("$5");
        ofs = 0x14af0;
        b = kaze_sound_state_base_ptr;
        param = (void *)(b + ofs);
        com = 0xd;
        goto control_call;
    }
    case 0x15:
    case 0x1b:
        goto seek_return;
    default:
        return;
    }

sync_complete:
    switch (dispatch) {
    case 0x15:
        kaze_cd_audio_set_volume_33c58(*(int *)(kaze_sound_state_base_ptr + 0x14b0c));
        com = 0x1b;
        goto control_zero;
    case 0x1b:
        if ((status[0] & 0x40) != 0) {
            return;
        }
        *(int *)(kaze_sound_state_base_ptr + 0x14b14) = 1;
        com = 0x11;
        param = 0;
        goto control_call;
    case 1:
        if ((status[0] & 0x80) == 0) {
            goto seek_return;
        }
        com = 0x11;
        goto control_zero;
    case 0x11:
        CdPosToInt(status + 5);
        __asm__ __volatile__(
            ".set\tnoat\n\t"
            ".set\tnoreorder\n\t"
            "lw\t$3,kaze_sound_state_base_ptr\n\t"
            "lui\t$1,0x1\n\t"
            "addu\t$1,$3,$1\n\t"
            "lw\t$4,0x4b08($1)\n\t"
            "lui\t$1,0x1\n\t"
            "addu\t$1,$3,$1\n\t"
            "sw\t$2,0x4b04($1)\n\t"
            "slt\t$4,$4,$2\n\t"
            ".set\treorder\n\t"
            ".set\tat"
            : "=r"(should_seek)
            :
            : "$1", "$2", "$3", "memory");
        if (!should_seek) {
            com = 0x11;
            goto control_zero;
        }
        com = 0x11;
        goto seek_return;
    case 0xd:
        goto seek_return;
    default:
        return;
    }

seek_return:
    kaze_xa_audio_seek_resume_339a8();
    return;

control_zero:
    __asm__ __volatile__("" ::: "memory");
    param = 0;
control_call:
    CdControlF(com, param);
}
