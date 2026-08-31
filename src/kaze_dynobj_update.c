/**
 * kaze_dynobj_update  (objective, 0x80026f64)
 *
 * Per-frame update over the 18-slot +0x5901c dynamic-object table:
 * dispatches type handlers, then writes distance-based render code (+0x8),
 * in-range flag (+0xc), and brightness (+0x44).
 */

#include "types.h"
#include "kaze_syms.h"
extern int kaze_radar_range_delta(int *, int *);
extern int kaze_state_base_ptr;
void kaze_dynobj_update(void)
{
  int range;
  int brightness;
  int table_offset;
  int *slot;
  register uint *slot_shadow asm("$17");
  register int i asm("$18");
  table_offset = 0x5901c;
  slot = (int *) (kaze_state_base_ptr + table_offset);
  if (((*((uint *) (kaze_state_base_ptr + 0x14))) & 0xc0001000) == 0)
  {
    i = 0;
    ;
    do
    {
      if ((*slot) != 0)
      {
        switch (*slot)
        {
          case 2:
            kaze_dynobj_patrol_shuttle(slot);
            break;

          case 3:
            kaze_dynobj_route_follow_smooth(slot);
            break;

          case 4:
            kaze_dynobj_fall_respawn_anim(slot);
            break;

          case 5:
            kaze_dynobj_bob_anim(slot);
            break;

          case 6:
            kaze_dynobj_route_follow_direct(slot);
            break;

          case 10:
            kaze_dynobj_state_timer(slot);

        }

        if ((*slot) != 0)
        {
          range = kaze_radar_range_delta(slot + 4, slot + 8);
          if (range < 0)
          {
            ((uint *) (slot + 2))[1] = 0;
          }
          else
          {
            ((uint *) (slot + 2))[1] = 1;
            if (range >= 0x67e8)
            {
              range = 0x80 - ((range + (-0x67e8)) / 0x1e);
              if (range >= 0)
              {
                brightness = 0x80;
                if (range < 0x81)
                {
                  brightness = range;
                }
              }
              else
              {
                brightness = 0;
              }
              ((uint *) (slot + 2))[0xf] = brightness;
              *((uint *) (slot + 2)) = 0x60000040;
            }
            else
              if (range >= 0x5910)
            {
              range = 0x80 - ((range + (-0x5910)) / 0x1e);
              if (range >= 0x20)
              {
                brightness = 0x80;
                if (range < 0x81)
                {
                  brightness = range;
                }
              }
              else
              {
                brightness = 0x20;
              }
              ((uint *) (slot + 2))[0xf] = brightness;
              *((uint *) (slot + 2)) = 1;
            }
            else
            {
              *((uint *) (slot + 2)) = 0;
            }
          }
        }
      }
      i = i + 1;
      slot_shadow = slot_shadow + 0x1e;
      slot = slot + 0x1e;
    }
    while (i < 0x12);
  }
  return;
}
