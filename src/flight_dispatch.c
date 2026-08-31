#include "types.h"
#include "kaze_syms.h"

extern void kaze_sfx_voice_keyon_34108(int, int);

/**
 * kaze_failure_terminal_handler  (core, 0x8001e028)
 *
 * Failure handler: sets kaze_state_flags bit 0x00800000 and the post-event
 * sound latch before result audio.
 */

void kaze_failure_terminal_handler(void)
{
    extern int *kaze_state_base_ptr;
    if (kaze_state_base_ptr[0x12080] == 0) {
        kaze_state_base_ptr[0x12080] = 1;
        kaze_sfx_voice_keyon_34108(0x1a, 0);
    }
    kaze_state_base_ptr[5] |= 0x80800000;
}

/**
 * kaze_success_terminal_handler  (core, 0x8001e090)
 *
 * Full-route success handler: sets kaze_state_flags bit 0x00400000 and the
 * post-event sound latch before result audio.
 */

void kaze_success_terminal_handler(void)
{
    extern int *kaze_state_base_ptr;
    if (kaze_state_base_ptr[0x12080] == 0) {
        kaze_state_base_ptr[0x12080] = 1;
        kaze_sfx_voice_keyon_34108(0x1c, 0);
    }
    kaze_state_base_ptr[5] |= 0x80400000;
}

extern int SquareRoot0(int);
extern int ratan2(int, int);
extern int rsin(int);

/**
 * kaze_stage_score_compute  (objective, 0x8001e0f8)
 *
 * Score in GTE scratchpad: mode 1 = min marker-to-target distance
 * (+0x58a7c/84 vs +0x58e14/1c, cap 0xed8)*100/0x26; mode 0 = triangle area
 * of the three markers via ratan2/rsin; mode 2 = frames +0x48284/60 s.
 */

uint kaze_stage_score_compute(int mode)
{
    extern char *kaze_state_base_ptr;
  int angle1;
  int angle2;
  int dist_clamped;
  register int ang __asm__("$4");
  int unused_int0;
  int unused_int1;
  int c5;
  int unused_int2;
  int dxv;
  int value;
  int result;
  int cond;
  int *sp;
  int *cp = (int *) 0x1f800000;
  char *D;
__asm__ ("lui %0,0x1f80" : "=r"(sp));
  if (mode == 1)
  {
    goto mode1;
  }
  if (mode < 2)
  {
    if (mode == 0)
    {
      goto mode0;
    }
    goto ret;
  }
  if (mode == 2)
  {
    goto mode2;
  }
  goto ret;
  mode0:
  D = kaze_state_base_ptr;

  dxv = ((*((int *) (D + 0x58ad8))) - (*((int *) (D + 0x58a7c)))) / 0x26;
  sp[0] = dxv;
  cp[1] = ((*((int *) (D + 0x58ae0))) - (*((int *) (D + 0x58a84)))) / 0x26;
  cp[3] = ((*((int *) (D + 0x58b34))) - (*((int *) (D + 0x58a7c)))) / 0x26;
  {
    int *_p = (int *) D;
    cp[4] = (_p[90831] - _p[90785]) / 0x26;
  }
  cp[2] = SquareRoot0((dxv * dxv) + (cp[1] * cp[1]));
  {
    int cp5ret = SquareRoot0((cp[3] * cp[3]) + (cp[4] * cp[4]));
    int arg0 = cp[1];
    int arg1 = sp[0];
    cp[5] = cp5ret;
    angle1 = ratan2(arg0, arg1);
  }
  angle2 = ratan2(cp[4], cp[3]);
  angle1 = angle1 - angle2;
  if (angle1 < 0)
  {
    angle1 = -angle1;
  }
__asm__ ("");
  ang = angle1;
  cp[6] = ang;
  {

    register int rs __asm__("$2");
    rs = rsin(ang);
    ang = (cp[2] * rs) / 0x1000;
__asm__ ("" : : "r"(rs));
  }
  c5 = cp[5];
  if (ang < 0)
  {
    ang = -ang;
  }
__asm__ ("");
  {

    register int hh __asm__("$2");
    hh = ang * c5;
    hh = hh / 2;
    hh = __builtin_abs(hh);
    cp[7] = ang;
    value = hh / 10000;
  }
  cp[8] = value;
  if (value < 0)
  {
    goto zero;
  }
  result = 9999;
  cond = value < 10000;
  goto merge;
  mode1:
  cp[8] = 0xed8;

  cp[9] = 0;
  do
  {
    char *zc = kaze_state_base_ptr + (sp[9] * 0x5c);
    if (((*((int *) (zc + 0x58a74))) < 0) && ((sp[10] = 0, 0 < (*((int *) (kaze_state_base_ptr + 0x24))))))
    {
      do
      {

        char *Dl;
        int t0;
        register int i9b __asm__("$4");
__asm__("":"=r"(sp):"0"(sp));
        {
          int c9 = sp[9] * 0x5c;
          int Av;
          int c10;
          Dl = kaze_state_base_ptr;
__asm__("":"=r"(sp):"0"(sp));
          Av = *((int *) ((Dl + c9) + 0x58a7c));
          c10 = sp[10] * 0x58;
          t0 = Av - (*((int *) ((Dl + c10) + 0x58e14)));
        }
        i9b = sp[9];
        sp[0] = t0;
__asm__("":"=r"(sp):"0"(sp));
        {
          int c9b = i9b * 0x5c;
          sp[1] = (*((int *) ((Dl + c9b) + 0x58a84))) - (*((int *) ((Dl += sp[10] * 0x58) + 0x58e1c)));
        }
        if (__builtin_abs(sp[0]) < 0x76c)
        {
          if ((__builtin_abs(sp[1]) < 0x76c) && ((sp[2] = SquareRoot0((sp[0] * sp[0]) + (sp[1] * sp[1])), sp[2] < 0x76d)))
          {
            if (sp[2] < sp[8])
            {
              sp[8] = sp[2];
            }
            break;
          }
        }
        sp[10] = sp[10] + 1;
      }
      while (sp[10] < (*((int *) (kaze_state_base_ptr + 0x24))));
    }
    sp[9] = sp[9] + 1;
  }
  while (sp[9] < 3);
  if (0xed7 < sp[8])
  {
    sp[8] = 0xffffffff;
    goto ret;
  }
  if (sp[8] < 0)
  {
    goto selzero;
  }
  dist_clamped = 0x76c;
  if (sp[8] < 0x76d)
  {
    dist_clamped = sp[8];
  }
  goto seldone;
  selzero:
  dist_clamped = 0;

  seldone:
  value = (dist_clamped * 100) / 0x26;

  sp[8] = value;
  if (value < 0)
  {
    goto zero;
  }
  result = 9999;
__asm__ ("");
  cond = value < 10000;
  goto merge;
  mode2:
  value = (*((uint *) (kaze_state_base_ptr + 0x48284))) / 0x3c;

  cp[8] = value;
  if (value < 0)
  {
    goto zero;
  }
  result = 5999;
  cond = value < 6000;
  merge:
  if (cond)
  {
    result = value;
  }

  goto commit;
  zero:
  result = 0;

  commit:
  sp[8] = result;

  ret:
  return sp[8];

}

/**
 * kaze_flight_control_dispatch  (core, 0x8001e648)
 *
 * Per-frame control dispatcher on root+0x14 flags: 0x40000000 -> post-event
 * menu update, 0x80000000 -> result retry prompt (kaze_result_retry_prompt),
 * 0x200000 -> pause/exit prompt, else the burner target-gas law
 * kaze_target_gas_law.
 */

void kaze_flight_control_dispatch(void)
{
    extern char *kaze_state_base_ptr;
    int flags = *(int *)(kaze_state_base_ptr + 0x14);

    if (flags & 0x40000000) {
        kaze_post_event_menu_update();
    } else if (flags < 0) {
        kaze_result_retry_prompt();
    } else if (flags & 0x200000) {
        kaze_pause_exit_prompt();
    } else {
        kaze_target_gas_law();
    }
}
