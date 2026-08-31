#include "types.h"
#include "kaze_syms.h"
extern int kaze_gas;
extern int kaze_vertical_penalty;
extern int kaze_wind_layer0_strength;
extern int rand(void);
extern int D_800B17D4;
extern s32 D_800B33B0;
extern int kaze_pos_y;

extern volatile char *D_8008D9D0_v __asm__("kaze_state_base_ptr");
extern s32 kaze_input_action_button_mask;
extern void kaze_play_sound_effect(int, int, int, int);
#define W(o)  (*(s32 *)(kaze_state_base_ptr + (o)))
#define HU(o) (*(u16 *)(kaze_state_base_ptr + (o)))
#define HS(o) (*(s16 *)(kaze_state_base_ptr + (o)))
#define BW(b, o)  (*(s32 *)((b) + (o)))
#define BHU(b, o) (*(u16 *)((b) + (o)))
#define BHS(b, o) (*(s16 *)((b) + (o)))

/**
 * kaze_post_event_menu_update  (menu, 0x80022684)
 *
 * Post-result/exit UI state machine consuming the post-event menu state at
 * base+0x48e0c.
 */

void kaze_post_event_menu_update(void)
{
    extern void kaze_sfx_voice_keyon_34108(int, int);
    extern void kaze_sfx_voice_keyoff_34170(int);
    extern s32 kaze_input_current_b;
    extern s32 kaze_input_current_a;
    extern char *kaze_state_base_ptr;
    s32 temp_v1;
    u16 temp_v0;
    u16 temp_v0_2;
    u16 temp_v0_5;
    u16 temp_v0_6;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_7;
    s32 temp_v0_8;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_v0_9;
    u16 temp_v0_10;
    u16 temp_v0_11;
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_3;
    s16 var_v1_4;
    register int var_a0 asm("$4");
    char frame_pad[0x20];

    if (W(0x481F8) != 0) {
        kaze_sfx_voice_keyoff_34170(0);
        W(0x481F8) = 0;
    }
    temp_v1 = W(0x48E0C);
    if ((u32)(temp_v1 - 1) < 2U) {
        if (temp_v1 == 1) {
            HU(0x48E10) = HU(0x48E10) + W(0xA4) * 0xC;
            temp_v0 = HU(0x48E12) + W(0xA4) * 0xA;
            HU(0x48E12) = temp_v0;
            if (HS(0x48E10) >= 0x6E && (s16)temp_v0 >= 0x40) {
                W(0x48E0C) = 3;
            }
        } else {
            HU(0x48E10) = HU(0x48E10) - W(0xA4) * 0xC;
            temp_v0_2 = HU(0x48E12) - W(0xA4) * 0xA;
            HU(0x48E12) = temp_v0_2;
            if (HS(0x48E10) <= 0 && (temp_v0_2 << 0x10) <= 0) {
                __asm__ volatile("");
                W(0x48E0C) = 0;
                W(0x14) &= 0xBFFFFFFF;
            }
        }
        {
        register char *clamp_a0 asm("$4") = kaze_state_base_ptr;
        temp_v0_3 = BHS(clamp_a0, 0x48E10);
        var_v1 = temp_v0_3;
        if (temp_v0_3 >= 0) {
            if (temp_v0_3 >= 0x6F) {
                var_v1 = 0x6E;
            }
        } else {
            var_v1 = 0;
        }
        {
        register char *clamp_a1 asm("$5") = D_8008D9D0_v;
        BHU(clamp_a0, 0x48E10) = var_v1;
        temp_v0_4 = BHS(clamp_a1, 0x48E12);
        var_v1_2 = temp_v0_4;
        if (temp_v0_4 >= 0) {
            if (temp_v0_4 >= 0x41) {
                var_v1_2 = 0x40;
            }
        } else {
            var_v1_2 = 0;
        }
        BHU(clamp_a1, 0x48E12) = var_v1_2;
        }
        }
        return;
    }
    if ((u32)(temp_v1 - 4) < 2U) {
        if (temp_v1 == 4) {
            HU(0x48E14) = HU(0x48E14) + W(0xA4) * 0xC;
            temp_v0_5 = HU(0x48E16) + W(0xA4) * 0xA;
            HU(0x48E16) = temp_v0_5;
            if (HS(0x48E14) >= 0x60 && (s16)temp_v0_5 >= 0x20) {
                W(0x48E0C) = 6;
            }
        } else {
            HU(0x48E14) = HU(0x48E14) - W(0xA4) * 0xC;
            temp_v0_6 = HU(0x48E16) - W(0xA4) * 0xA;
            HU(0x48E16) = temp_v0_6;
            if (HS(0x48E14) <= 0 && (temp_v0_6 << 0x10) <= 0) {
                W(0x48E0C) = 3;
            }
        }
        {
        register char *clamp2_a0 asm("$4") = kaze_state_base_ptr;
        temp_v0_7 = BHS(clamp2_a0, 0x48E14);
        var_v1_3 = temp_v0_7;
        if (temp_v0_7 >= 0) {
            if (temp_v0_7 >= 0x61) {
                var_v1_3 = 0x60;
            }
        } else {
            var_v1_3 = 0;
        }
        {
        register char *clamp2_a1 asm("$5") = D_8008D9D0_v;
        BHU(clamp2_a0, 0x48E14) = var_v1_3;
        temp_v0_8 = BHS(clamp2_a1, 0x48E16);
        var_v1_4 = temp_v0_8;
        if (temp_v0_8 >= 0) {
            if (temp_v0_8 >= 0x21) {
                var_v1_4 = 0x20;
            }
        } else {
            var_v1_4 = 0;
        }
        BHU(clamp2_a1, 0x48E16) = var_v1_4;
        }
        }
        return;
    }
    if (temp_v1 == 7) {
        goto block_41;
    }
    if ((u32)(temp_v1 - 9) < 2U) {
        goto check10;
    }
    if (temp_v1 != 12) {
        goto cases;
    }
check10:
    if (temp_v1 != 0xA) {
        goto block_40;
    }
block_41:
    temp_v0_11 = HU(0x492C2) + (W(0xA4) << 5);
    HU(0x492C2) = temp_v0_11;
    if ((s16)temp_v0_11 >= 0x38) {
        HU(0x492C2) = 0x38;
        W(0x48E0C) = W(0x48E0C) + 1;
    }
    return;
block_40:
    temp_v0_10 = HU(0x492C2) + (W(0xA4) << 5);
    HU(0x492C2) = temp_v0_10;
    if ((s16)temp_v0_10 >= 0x138) {
        if (W(0x48E0C) == 9) {
            W(0x48E0C) = 3;
            return;
        }
        W(0x48E0C) = 0;
        W(0x14) &= 0xBFFFFFFF;
    }
    return;
cases:
    if (temp_v1 == 3) {
        if (((kaze_input_current_a & D_800B33B0) && ((kaze_input_current_b & D_800B33B0) == 0)) ||
            ((kaze_input_current_a & 0x40) && !(kaze_input_current_b & 0x40))) {
            W(0x48E0C) = 2;
            kaze_sfx_voice_keyon_34108(1, 1);
            return;
        }
        if ((kaze_input_current_a & 0x20) && !(kaze_input_current_b & 0x20)) {
            register int choice_one asm("$6") = 1;
            temp_a0 = W(0x492BC);
            if (temp_a0 == choice_one) {
                goto pchoice1;
            }
            if (temp_a0 < 2) {
                if (temp_a0 == 0) {
                    goto pchoice0;
                }
                return;
            }
            if (temp_a0 == 2) {
                goto pchoice2;
            }
            return;
pchoice0:
            var_a0 = 0x24;
            W(0x48E0C) = 7;
            HU(0x492C0) = 0x60;
            HS(0x492C2) = -0xC8;
            kaze_sfx_voice_keyon_34108(var_a0, 1);
            return;
pchoice1:
            var_a0 = 0x24;
            HU(0x48E18) = 0xA0;
            HU(0x48E1A) = 0x96;
            W(0x48E0C) = 4;
            W(0x492B8) = choice_one;
            W(0x492B4) = 0x40;
            W(0x492B0) = 0x40;
            kaze_sfx_voice_keyon_34108(var_a0, 1);
            return;
pchoice2:
            var_a0 = 0x24;
            HU(0x48E18) = 0xA0;
            HU(0x48E1A) = 0xB2;
            W(0x48E0C) = 4;
            W(0x492B8) = choice_one;
            W(0x492B4) = 0x40;
            W(0x492B0) = 0x40;
            kaze_sfx_voice_keyon_34108(var_a0, 1);
            return;
        }
        if ((kaze_input_current_a & 0x1000) && !(kaze_input_current_b & 0x1000)) {
            W(0x492BC) = W(0x492BC) - 1;
            kaze_play_sound_effect(1, 0x64, 1, 1);
        }
        if ((kaze_input_current_a & 0x4000) && !(kaze_input_current_b & 0x4000)) {
            W(0x492BC) = W(0x492BC) + 1;
            kaze_play_sound_effect(1, 0x64, 1, 1);
        }
        temp_v0_9 = W(0x492BC);
        if (temp_v0_9 < 0) {
            W(0x492BC) = 2;
            return;
        }
        if (temp_v0_9 >= 3) {
            W(0x492BC) = 0;
            return;
        }
    } else if (temp_v1 == 6) {
        if (((kaze_input_current_a & D_800B33B0) && ((kaze_input_current_b & D_800B33B0) == 0)) ||
            ((kaze_input_current_a & 0x40) && !(kaze_input_current_b & 0x40))) {
            W(0x492B8) = 1;
            W(0x48E0C) = 5;
            kaze_sfx_voice_keyon_34108(1, 1);
            return;
        }
        if ((kaze_input_current_a & 0x20) && !(kaze_input_current_b & 0x20)) {
            if (W(0x492B8) != 0) {
                W(0x48E0C) = 5;
                kaze_sfx_voice_keyon_34108(1, 1);
                return;
            }
            temp_a1 = W(0x14) & 0xBFFFFFFF;
            W(0x14) = temp_a1;
            if (W(0x492BC) == 1) {
                W(0x14) = temp_a1 | 0x84000000;
                kaze_play_sound_effect(2, 0x64, 1, 1);
                return;
            }
            W(0x14) = temp_a1 | 0x88000000;
            kaze_sfx_voice_keyon_34108(3, 1);
            return;
        }
        if (((kaze_input_current_a & 0x8000) && ((kaze_input_current_b & 0x8000) == 0)) ||
            ((kaze_input_current_a & 0x2000) && !(kaze_input_current_b & 0x2000))) {
            W(0x492B8) = (W(0x492B8) == 0);
            kaze_play_sound_effect(1, 0x64, 1, 1);
            return;
        }
    } else if (temp_v1 == 8) {
        if (((kaze_input_current_a & 0x20) && !(kaze_input_current_b & 0x20)) ||
            ((kaze_input_current_a & 0x40) && !(kaze_input_current_b & 0x40)) ||
            ((kaze_input_current_a & D_800B33B0) && ((kaze_input_current_b & D_800B33B0) == 0))) {
            W(0x48E0C) = 9;
            kaze_sfx_voice_keyon_34108(1, 1);
            return;
        }
    } else if (temp_v1 == 11) {
        if (((kaze_input_current_a & kaze_input_action_button_mask) && ((kaze_input_current_b & kaze_input_action_button_mask) == 0)) ||
            ((kaze_input_current_a & 0x40) && !(kaze_input_current_b & 0x40)) ||
            ((kaze_input_current_a & D_800B33B0) && ((kaze_input_current_b & D_800B33B0) == 0)) ||
            ((kaze_input_current_a & 0x20) && !(kaze_input_current_b & 0x20))) {
            W(0x48E0C) = 12;
            kaze_sfx_voice_keyon_34108(1, 1);
            return;
        }
    }
    return;
}

extern int kaze_stage_variant;
extern int kaze_input_bind_pause;

/**
 * kaze_result_retry_prompt  (menu, 0x80023220)
 *
 * Post-result continue/retry state machine on root+0x8: Circle/Cross opens
 * the retry prompt (selection word 0x124ae) or fast-exits in demo;
 * Left/Right toggles, Circle confirms flag 0x4000000 (retry) or 0x8000000
 * (exit).
 */

void kaze_result_retry_prompt(void)
{
    extern void kaze_sfx_voice_keyon_34108(int, int);
    extern void kaze_sfx_voice_keyoff_34170(int);
    extern int kaze_input_current_b;
    extern int kaze_input_current_a;
    extern int *kaze_state_base_ptr;
    int substate;
    unsigned int flags;

    if (kaze_state_base_ptr[0x1207e] != 0) {
        kaze_sfx_voice_keyoff_34170(0);
        kaze_state_base_ptr[0x1207e] = 0;
    }
    flags = kaze_state_base_ptr[5];
    if ((flags & 0xc000000) != 0) {
        int n = kaze_state_base_ptr[2];
        kaze_state_base_ptr[2] = n + 1;
        if (0x13 < n + 1) {
            if ((kaze_state_base_ptr[5] & 0x8000000) != 0) {
                kaze_state_base_ptr[0] = 1;
            }
            kaze_state_base_ptr[1] = 0;
        }
    } else {
        if ((flags & 0xc00000) != 0) {
            substate = kaze_state_base_ptr[2];
            if (substate < 0xd) {
                kaze_state_base_ptr[2] = substate + 1;
            } else if (substate == 0xd) {
                if ((((kaze_input_current_a & 0x20) != 0) && ((kaze_input_current_b & 0x20) == 0)) ||
                    (((kaze_input_current_a & 0x40) != 0) && ((kaze_input_current_b & 0x40) == 0))) {
                    if ((D_800B17D4 == 1) ||
                        ((kaze_stage_variant != 0) && ((kaze_state_base_ptr[5] & 0x400000) != 0))) {
                        kaze_state_base_ptr[2] = 2;
                        kaze_state_base_ptr[5] = kaze_state_base_ptr[5] | 0x8000000;
                        kaze_sfx_voice_keyon_34108(3, 1);
                    } else {
                        kaze_state_base_ptr[2] = 0xe;
                        kaze_state_base_ptr[0x124ae] = 0;
                        kaze_state_base_ptr[0x124ad] = 0x40;
                        kaze_state_base_ptr[0x124ac] = 0x40;
                        kaze_sfx_voice_keyon_34108(0x24, 1);
                    }
                }
            } else if (substate == 0xe) {
                if (((kaze_input_current_a & 0x20) != 0) && ((kaze_input_current_b & 0x20) == 0)) {
                    if (kaze_state_base_ptr[0x124ae] != 0) {
                        kaze_state_base_ptr[5] = flags | 0x8000000;
                        kaze_sfx_voice_keyon_34108(3, 1);
                    } else {
                        kaze_state_base_ptr[5] = flags | 0x4000000;
                        kaze_play_sound_effect(2, 100, 1, 1);
                    }
                    kaze_state_base_ptr[2] = 2;
                } else {
                    __asm__ volatile("" ::: "memory");
                    if ((((kaze_input_current_a & 0x8000) != 0) && ((kaze_input_current_b & 0x8000) == 0)) ||
                        (((kaze_input_current_a & 0x2000) != 0) && ((kaze_input_current_b & 0x2000) == 0))) {
                        kaze_state_base_ptr[0x124ae] = (unsigned int)(kaze_state_base_ptr[0x124ae] == 0);
                        kaze_play_sound_effect(1, 100, 1, 1);
                    }
                }
            }
        } else {
            if (((kaze_input_current_a & kaze_input_bind_pause) != 0) && ((kaze_input_current_b & kaze_input_bind_pause) == 0)) {
                kaze_state_base_ptr[5] = flags & 0x7fffffff;
                kaze_state_base_ptr[0x3b2bf] = 0;
            }
        }
    }
}

extern int kaze_pos_x;
extern int kaze_stage_mode;
extern int kaze_stage_flags;
extern int SquareRoot0(int);

/**
 * kaze_hud_status_update  (hud, 0x800235a8)
 *
 * Updates HUD status: +0xecae4 code (0xb no-gas, 9 terminal, -1 demo), 0x1e
 * loop-sound latch +0x481fc; every 15 ticks altitude +0xecae8 (-y/0x96),
 * wind meter +0xecaec, goal-proximity +0xecaf0 (100 near target).
 */

void kaze_hud_status_update(void)
{
    extern int kaze_sfx_voice_keyoff_34170(int);
    extern int kaze_sfx_voice_keyon_34108(int, int);
    extern char *kaze_state_base_ptr;
  char *temp_a0;
  char *temp_a0_2;
  int *pWind;
  int var_s4;
  int var_s3;
  char *new_var2;
  int var_s1;
  int var_s0;
  int var_v1;
  int temp_v0;
  register int var_a0 asm("$4");
  int temp_v1;
  int temp_v1_2;
  s32 new_var;
  int var_layer;
  temp_a0 = kaze_state_base_ptr;
  var_s4 = 0;
  var_s3 = 0;
  if ((!((*((s32 *) (((char *) temp_a0) + 0x14))) & 0xC00000)) && (kaze_stage_mode == 1))
  {
    var_s1 = 0;
    if (var_s4 < (*((s32 *) (((char *) temp_a0) + 0x24))))
    {
      pWind = &kaze_pos_x;
      var_s0 = 0;
      loop_4:
      asm("" : : "r"(pWind));

      temp_a0_2 = temp_a0 + var_s0;
      var_v1 = pWind[0] - (*((s32 *) (((char *) temp_a0_2) + 0x58E14)));
      new_var = *(volatile s32 *)(((char *) temp_a0_2) + 0x58E1C);
      asm volatile("" : : : "memory");
      temp_v0 = pWind[2];
      if (var_v1 < 0)
      {
        var_v1 = -var_v1;
      }
      temp_v0 = temp_v0 - new_var;
      var_a0 = temp_v0;
      if (temp_v0 < 0)
      {
        var_a0 = -var_a0;
      }
      if (((var_v1 >= 0x76C1) || (var_a0 >= 0x76C1)) ||
          (SquareRoot0(({ temp_v0 = var_v1 * var_v1;
                         var_v1 = var_a0 * var_a0;
                         temp_v0 + var_v1; })) >= 0x76C1))
      {
        temp_a0 = kaze_state_base_ptr;
        {
        int loop_bound = *((s32 *) (((char *) temp_a0) + 0x24));
        asm volatile("" : : : "memory");
        var_s1 += 1;
        var_s0 += 0x58;
        if (var_s1 < loop_bound)
        {
          goto loop_4;
        }
        }
      }
      else
      {
        goto wind_success;
      }
    }
  }
  status_check:
  if ((kaze_stage_flags & 3) == 3)
  {
    var_s3 = 1;
  }
  temp_a0 = kaze_state_base_ptr;
  temp_v1 = *((s32 *) (((char *) temp_a0) + 0x14));
  if (!(temp_v1 & 0x400000)) goto check_800000;
  {
    register int status_value asm("$2") = 9;
    *((s32 *) (((char *) temp_a0) + 0xECAE4)) = status_value;
    goto status_done;
  }

  wind_success:
  var_s4 = 1;
  goto status_check;

  check_800000:
  if (!(temp_v1 & 0x800000)) goto other_status;
  {
    register int status_value asm("$2") = 9;
    *((s32 *) (((char *) temp_a0) + 0xECAE4)) = status_value;
    goto status_done;
  }

  other_status:
  if (((temp_v1 & 0x02008000) == 0x8000) || (kaze_gas == 0))
  {
    *((s32 *) (((char *) temp_a0) + 0xECAE4)) = 0xB;
  }
  else
  {
    s32 current_status;
    current_status = *((s32 *) (((char *) temp_a0) + 0xECAE4));
    temp_v1_2 = *((s32 *) (((char *) temp_a0) + 0xECAE0));
    if (current_status != temp_v1_2)
    {
      *((s32 *) (((char *) temp_a0) + 0xECAE4)) = temp_v1_2;
    }
  }
  goto status_done;

  status_done:
  if (((*((s32 *) (((char *) kaze_state_base_ptr) + 0x14))) & 0x100000) && ((*((s32 *) (((char *) kaze_state_base_ptr) + 8))) == 1))
  {
    *((s32 *) (((char *) kaze_state_base_ptr) + 0xECAE4)) = -1;
    var_s3 = 0;
  }
  if ((*((s32 *) (((char *) kaze_state_base_ptr) + 0x14))) & 0x0C000000)
  {
    *((s32 *) (((char *) kaze_state_base_ptr) + 0xECAE4)) = -1;
    var_s3 = 0;
  }
  if (var_s3 != 0)
  {
    if ((*((s32 *) (((char *) kaze_state_base_ptr) + 0x481FC))) == 0)
    {
      kaze_sfx_voice_keyon_34108(0x1E, 0);
      *((s32 *) (((char *) kaze_state_base_ptr) + 0x481FC)) = 1;
    }
  }
  else
    if ((*((s32 *) (((char *) kaze_state_base_ptr) + 0x481FC))) != 0)
  {
    kaze_sfx_voice_keyoff_34170(0x1E);
    *((s32 *) (((char *) kaze_state_base_ptr) + 0x481FC)) = 0;
  }
  {
  s32 tick = (*((s32 *) (((char *) kaze_state_base_ptr) + 0xECAF4))) + (*((s32 *) (((char *) kaze_state_base_ptr) + 0xA4)));
  *((s32 *) (((char *) kaze_state_base_ptr) + 0xECAF4)) = tick;
  if (tick >= 0xF)
  {
    *((s32 *) (((char *) kaze_state_base_ptr) + 0xECAF4)) = 0;
    if ((*((s32 *) (((char *) kaze_state_base_ptr) + 0x14))) & 0x0CC00000)
    {
      *((s32 *) (((char *) kaze_state_base_ptr) + 0xECAE8)) = (*((s32 *) (((char *) kaze_state_base_ptr) + 0xECAE8))) - ((*((s32 *) (((char *) kaze_state_base_ptr) + 0xA4))) * 4);
    }
    else
    {
      *((s32 *) (((char *) kaze_state_base_ptr) + 0xECAE8)) = (-(*((s32 *) (((char *) kaze_state_base_ptr) + 0x6C)))) / 150;
    }
    new_var2 = kaze_state_base_ptr;
    temp_v1 = *((s32 *) (((char *) new_var2) + 0xECAE8));
    if (temp_v1 >= 0)
    {
      var_a0 = 0x5A;
      if (temp_v1 < 0x5B)
      {
        var_a0 = temp_v1;
      }
    }
    else
    {
      var_a0 = 0;
    }
    *((s32 *) (((char *) new_var2) + 0xECAE8)) = var_a0;
    var_layer = ((-kaze_pos_y) / 0x76C) - 3;
    asm("" : "=r"(var_layer) : "0"(var_layer));
    if (((&kaze_wind_layer0_strength)[var_layer * 2] == 0x50) && (!((*((s32 *) (((char *) kaze_state_base_ptr) + 0x14))) & 0x0CC00000)))
    {
      *((s32 *) (((char *) kaze_state_base_ptr) + 0xECAEC)) = (*((0, (s32 *) (((char *) kaze_state_base_ptr) + 0xECAEC)))) + ((*((s32 *) (((char *) kaze_state_base_ptr) + 0xA4))) * 4);
    }
    else
    {
      *((s32 *) (((char *) kaze_state_base_ptr) + 0xECAEC)) = (*((s32 *) (((char *) kaze_state_base_ptr) + 0xECAEC))) - ((*((s32 *) (((char *) kaze_state_base_ptr) + 0xA4))) * 4);
    }
    new_var2 = kaze_state_base_ptr;
    asm("" : "=r"(new_var2) : "0"(new_var2));
    temp_v1 = *((s32 *) (((char *) new_var2) + 0xECAEC));
    if (temp_v1 >= 0)
    {
      var_a0 = 0x64;
      if (temp_v1 < 0x65)
      {
        var_a0 = temp_v1;
      }
    }
    else
    {
      var_a0 = 0;
    }
    *((s32 *) (((char *) new_var2) + 0xECAEC)) = var_a0;
    {
      register char *ecaf0_root asm("$2");
      register int ecaf0_scale asm("$3");
      if (var_s4 != 0)
      {
        ecaf0_root = kaze_state_base_ptr;
        ecaf0_scale = (*((s32 *) (((char *) ecaf0_root) + 0xA4))) * 4;
        var_a0 = (*((s32 *) (((char *) ecaf0_root) + 0xECAF0))) + ecaf0_scale;
      }
      else
      {
        ecaf0_root = kaze_state_base_ptr;
        ecaf0_scale = (*((s32 *) (((char *) ecaf0_root) + 0xA4))) * 4;
        var_a0 = (*((s32 *) (((char *) ecaf0_root) + 0xECAF0))) - ecaf0_scale;
      }
      *((s32 *) (((char *) ecaf0_root) + 0xECAF0)) = var_a0;
    }
    new_var2 = kaze_state_base_ptr;
    asm("" : "=r"(new_var2) : "0"(new_var2));
    temp_v1 = *((s32 *) (((char *) new_var2) + 0xECAF0));
    if (temp_v1 >= 0)
    {
      var_a0 = 0x64;
      if (temp_v1 < 0x65)
      {
        var_a0 = temp_v1;
      }
    }
    else
    {
      var_a0 = 0;
    }
    *((s32 *) (((char *) new_var2) + 0xECAF0)) = var_a0;
  }
  }
}

extern int kaze_burner_state;
void kaze_wind_layers_reroll();

/**
 * kaze_flight_timer_gas_update  (flight, 0x80023b44)
 *
 * In-flight timers/gas: time +0x48284 (cap 359999), wind timer +0x48280,
 * drains gas kaze_gas (-0x10/-0x18 with burner ramp; bonus
 * kaze_vertical_penalty first), clamps 0..0x64000, +0xecaf8 page mod 0x1e,
 * wind re-roll per 0x4b0.
 */

void kaze_flight_timer_gas_update(void)
{
    extern char *kaze_state_base_ptr;
    unsigned int *wind_timer_ptr;
    register char *state __asm__("$5");
    register char *copy __asm__("$4");
    int clamped_gas;
    register unsigned int clamped_time __asm__("$7");
    unsigned int limit;
    register int limit2 __asm__("$2");
    int *pgas;
    int gasv;

    state = kaze_state_base_ptr;
    if ((*(unsigned int *)(state + 0x14) & 0xc0001000) == 0) {
        if ((*(unsigned int *)(state + 0x14) & 0x200000) != 0) {
            *(int *)(state + 0x48284) =
                *(int *)(state + 0x48284) + *(int *)(state + 0xa4);
            *(int *)(state + 0x48280) =
                *(int *)(state + 0x48280) + *(int *)(state + 0xa4);
            limit = 359999;
            clamped_time = 359999;
            if (*(unsigned int *)(state + 0x48284) <= limit) {
                clamped_time = *(unsigned int *)(state + 0x48284);
            }
            *(unsigned int *)(state + 0x48284) = clamped_time;
            copy = state;
            if ((*(unsigned int *)(copy + 0x14) & 0x200000) != 0) {
                int *pvp = &kaze_vertical_penalty;
                int vp = *pvp;
                if (vp != 0) {
                    if (vp < *(int *)(copy + 0xa4) * 300) {
                        *pvp = 0;
                        kaze_gas = kaze_gas - vp;
                    } else {
                        kaze_gas = kaze_gas + *(int *)(copy + 0xa4) * -300;
                        *pvp = vp + *(int *)(copy + 0xa4) * -300;
                    }
                } else {
                    if (kaze_burner_state != 0) {
                        kaze_gas = kaze_gas + *(int *)(copy + 0xa4) * -0x18;
                    } else {
                        kaze_gas = kaze_gas + *(int *)(state + 0xa4) * -0x10;
                    }
                }
            }
        }
        pgas = &kaze_gas;
        gasv = *pgas;
        if (gasv >= 0) {
            limit2 = 0x64000;
            clamped_gas = 0x64000;
            if (gasv <= limit2) {
                clamped_gas = gasv;
            }
        } else {
            clamped_gas = 0;
        }
        state = kaze_state_base_ptr;
        wind_timer_ptr = (unsigned int *)(state + 0x48280);
        *pgas = clamped_gas;
        *(int *)(state + 0xecaf8) = (*(int *)(state + 0xecaf8) + 1) % 0x1e;
        if ((0x4af < *wind_timer_ptr) && ((*(unsigned int *)(state + 0x14) & 0x80) != 0)) {
            *(int *)(state + 0x48280) = 0;
            kaze_wind_layers_reroll();
        }
    }
    return;
}

extern int kaze_wind_layer0_dir;
extern int kaze_pos_z;

/**
 * kaze_wind_layers_reroll  (flight, 0x80023de4)
 *
 * Re-randomizes the five wind layers at kaze_wind_layer0_dir (stride 8):
 * random-walk direction &7, random speed 0x50/0x28/0x14 at +4; outside the
 * 0x7e2d..0x6efd8 play region forces one layer to point back toward map
 * center.
 */

void kaze_wind_layers_reroll(void)
{
    extern char *kaze_state_base_ptr;
    register int target __asm__("$18");
    int r;
    int r2;
    int val;
    int m;
    int sval;
    register int idx __asm__("$16");
    register int *p __asm__("$17");
    register int *base __asm__("$19");
    int active;
    int j;

    target = 0;
    active = 0;
    if (((*(unsigned int *)(kaze_state_base_ptr + 0x14) & 0x8000) != 0) &&
        (0x66fa7 < (unsigned int)(kaze_pos_x - 0x7e2d) ||
         0x66fa6 < (unsigned int)(kaze_pos_z - 0x7e2d))) {
        active = 1;
        if (kaze_pos_x < 0x26f71) {
            if (kaze_pos_z < 0x26f71) {
                target = 3;
            } else {
                target = 2;
                if (0x4fc8f < kaze_pos_z) {
                    target = 1;
                }
            }
        } else if (0x4fc8f < kaze_pos_x) {
            if (kaze_pos_z < 0x26f71) {
                target = 5;
            } else {
                target = 6;
                if (0x4fc8f < kaze_pos_z) {
                    target = 7;
                }
            }
        } else if (kaze_pos_z < 0x7e2d) {
            target = 4;
        }
    }
    idx = 0;
    base = &kaze_wind_layer0_dir;
    p = base;
    do {
        switch (rand() % 3) {
        case 0:
            val = *(int *)((int)&kaze_wind_layer0_dir + idx) + 1;
            *p = val;
            break;
        case 1:
            val = *(int *)((int)&kaze_wind_layer0_dir + idx) + -1;
            *p = val;
            break;
        }
        *(unsigned int *)((int)&kaze_wind_layer0_dir + idx) =
            *(unsigned int *)((int)&kaze_wind_layer0_dir + idx) & 7;
        r = rand();
        sval = r;
        if (r < 0) {
            sval = r + 7;
        }
        m = r + (sval >> 3) * -8;
        if (m == 0) {
            m = 0x50;
        } else {
            if (m < 0) goto s14;
            if (m < 3) { m = 0x28; goto sdone; }
        s14:
            m = 0x14;
        sdone: ;
        }
        *(int *)((int)&kaze_wind_layer0_strength + idx) = m;
        p += 2;
        idx += 8;
    } while ((int)p < (int)base + 0x28);
    j = 0;
    if (active) {
        do {
            if (*(int *)((int)&kaze_wind_layer0_dir + j) == target) {
                return;
            }
            j += 8;
        } while (j < 0x28);
        r2 = rand();
        (&kaze_wind_layer0_dir)[(r2 % 5) * 2] = target;
    }
}

#include "gte.h"

/**
 * kaze_world_update_pass  (core, 0x800240a4)
 *
 * Per-frame world update umbrella: loads GTE matrices from the camera matrix
 * at root+0x28, then render-vector load, post-event menu anim, HUD gauges,
 * dynamic/mode-object and marker updates, cloth anim, landing events.
 */

void kaze_world_update_pass(void)
{
    extern int kaze_state_base_ptr;
    int base = kaze_state_base_ptr;

    if ((*(u32 *)(base + 0x14) & 0xc000000) == 0 || *(int *)(base + 8) < 2) {
        int r0 = base + 0x28;

        gte_ld_rotm(r0);
        gte_ld_transv(r0);
        kaze_render_vector_load();
        kaze_postevent_menu_anim();
        kaze_hud_gauge_renderer();
        kaze_dynobj_update();
        kaze_mode_object_update();
        kaze_marker_slot_update();
        kaze_stage_flag_cloth_anim();
        kaze_trail_particle_update();
        kaze_landing_event_update();
    }
}
