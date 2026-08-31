/**
 * kaze_st_set_mask  (library, 0x8007f940)
 *
 * PsyQ StSetMask (C_010.OBJ): stores the stream mask/start/rewind parameters
 * into kaze_cd_stream_mask, kaze_cd_stream_start and kaze_cd_stream_rewind.
 */


#include "kaze_syms.h"
extern int kaze_cd_stream_start;
extern int kaze_cd_stream_rewind;
extern int kaze_cd_stream_mask;
void kaze_st_set_mask(int a0, int a1, int a2)
{
    kaze_cd_stream_mask = a0;
    kaze_cd_stream_start = a1;
    kaze_cd_stream_rewind = a2;
}
