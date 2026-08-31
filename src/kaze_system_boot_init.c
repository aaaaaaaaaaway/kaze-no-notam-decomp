/**
 * kaze_system_boot_init  (core, 0x80011e60)
 *
 * Boot init: ResetGraph/ResetCallback, CdInit, DecDCTReset, InitPAD (buffers
 * kaze_pad1_buffer/kaze_pad2_status_byte), _bu_init/_card_auto, then clears
 * VRAM and builds the CD file cache from kaze_cd_root_dir_name.
 */


#include "kaze_syms.h"
extern char kaze_cd_root_dir_name[];
extern void CdInit(void);
extern void CdSetDebug(int);
extern void DecDCTReset(int);
extern void InitPAD(char *, int, char *, int);
extern void ResetGraph(int);
extern void SetDispMask(int);
extern void SetGraphDebug(int);
extern void _bu_init(void);
extern void _card_auto(int);
extern char kaze_pad1_buffer[];

extern void ResetCallback(void);
extern void ChangeClearPAD(int);

void kaze_system_boot_init(void)
{
    ResetGraph(0);
    ResetCallback();
    kaze_set_video_mode(0);
    SetDispMask(0);
    CdInit();
    CdSetDebug(0);
    DecDCTReset(0);
    InitPAD(kaze_pad1_buffer, 0x22, kaze_pad1_buffer + 0x22, 0x22);
    kaze_init_card(1);
    kaze_start_card();
    ChangeClearPAD(0);
    _bu_init();
    _card_auto(0);
    SetGraphDebug(0);
    kaze_init_geom();
    kaze_vram_clear_all();
    kaze_cd_file_cache_root_init(kaze_cd_root_dir_name);
}
