#include "types.h"
#include "kaze_syms.h"
extern int kaze_memcard_poll_retry_count;
extern int kaze_memcard_slot0_event_a;
extern int kaze_memcard_slot0_event_b;
extern int kaze_memcard_slot0_event_c;
extern int kaze_memcard_slot0_event_d;
extern void _card_clear(int);
extern void _card_info(int);
extern void _card_load(int);
extern int kaze_memcard_wait_result_b(void);

extern uint kaze_memcard_slot_request_state;

/**
 * kaze_memcard_slot_poll_start  (library, 0x8004b21c)
 *
 * Issues _card_info/_card_load/_card_clear for a slot depending on state
 * bits in kaze_memcard_slot_request_state.
 */

int kaze_memcard_slot_poll_start(int slot)
{
    uint *p;
    uint *base;
    uint f;
    uint nf;

    base = &kaze_memcard_slot_request_state;
    p = (uint *)((int)base + slot * 4);
    f = *p;
    if ((f & 0x700) != 0) {
        return -1;
    }
    if ((f & 2) != 0) {
        kaze_memcard_clear_events_b();
        _card_clear(slot << 4);
        return 0;
    }
    if ((f & 4) != 0) {
        kaze_memcard_clear_events_a();
        _card_load(slot << 4);
        nf = *p | 0x400;
    } else {
        __asm__ volatile("");
        if ((f & 1) == 0) {
            return 0;
        }
        kaze_memcard_clear_events_a();
        _card_info(slot << 4);
        nf = *p | 0x100;
    }
    *p = nf;
    return 0;
}

extern uint kaze_memcard_slot1_request_state;
extern int TestEvent(int);

/**
 * kaze_memcard_state_machine  (library, 0x8004b2e4)
 *
 * Polls memcard events (TestEvent) and advances slot state
 * kaze_memcard_slot_request_state with retry counter
 * kaze_memcard_poll_retry_count.
 */

uint kaze_memcard_state_machine(int slot)
{
  register uint status asm("$16");
  int evt;
  uint *slot_ptr;
  uint slot_state;
  uint new_var;
  uint clear_mask;
  uint *q;
  uint *r;
  uint flags_val;
  if ((kaze_memcard_slot_request_state == 0) && (kaze_memcard_slot1_request_state == 0))
    return 0xfffffffe;
  {
    evt = TestEvent(kaze_memcard_slot0_event_a);
    if (evt == 1)
    {
      status = 0;
      goto commit_check;
    }
    evt = TestEvent(kaze_memcard_slot0_event_b);
    if (evt == 1)
    {
      status = 1;
      goto commit_check;
    }
    evt = TestEvent(kaze_memcard_slot0_event_c);
    if (evt == 1)
    {
      status = 2;
      goto commit_check;
    }
    status = 0xffffffff;
    evt = TestEvent(kaze_memcard_slot0_event_d);
    if (evt == 1)
    {
      status = 3;
    }
    commit_check:
    if (status != 0xffffffff)
    {
      {
        uint *cb2 = &kaze_memcard_slot_request_state;
        slot_ptr = (uint *) ((&(*cb2)) + slot);
      }
      slot_state = *slot_ptr;
      kaze_memcard_poll_retry_count = 0;
      if ((slot_state & 0x100) != 0)
      {
        uint masked1;
        uint ta = slot_state & 0xfffffffe;
        *slot_ptr = (masked1 = ta & 0xfffffeff);
        if ((status == 3) || ((status == 0) && ((slot_state & 0x10) != 0)))
        {
          uint newf;
          uint tb = masked1 & 0xffffffee;
          newf = tb | 2;
          *slot_ptr = newf;
          if ((newf & 0x700) == 0)
          {
            if ((newf & 2) != 0)
            {
              kaze_memcard_clear_events_b();
              _card_clear(slot << 4);
            }
            else
              if ((newf & 4) != 0)
            {
              kaze_memcard_clear_events_a();
              _card_load(slot << 4);
              *slot_ptr = (*slot_ptr) | 0x400;
            }
          }
          status = kaze_memcard_wait_result_b();
          status = status | 0x200;
          { register uint *cb asm("$2"); cb = &kaze_memcard_slot_request_state; q = cb + slot; }
          flags_val = *q;
          clear_mask = 0xfffffffd;
        }
        else
        {
          status = status | 0x100;
          { register uint *cb asm("$2"); cb = &kaze_memcard_slot_request_state; q = cb + slot; }
          flags_val = *q;
          clear_mask = 0xffffffee;
        }
        *q = flags_val & clear_mask;
      }
      else
        if ((slot_state & 0x400) != 0)
      {
        uint tc = slot_state & 0xfffffffb;
        status = status | 0x400;
        *slot_ptr = tc & 0xfffffbff;
      }
    }
    else
    {
      if (kaze_memcard_poll_retry_count < 0x79)
      {
        goto do_incr;
      }
      {
        uint *cb = &kaze_memcard_slot_request_state;
        r = (uint *) ((&(*cb)) + slot);
      }
      slot_state = *r;
      new_var = slot_state;
      kaze_memcard_poll_retry_count = 0;
      status = 2;
      clear_mask = 0x402;
      if ((new_var & 0x100) != 0)
      {
        uint td = new_var & 0xfffffffe;
        td = td & 0xfffffeff;
        *r = td & 0xffffffee;
        status = 0x102;
      }
      else
        if ((new_var & 0x400) != 0)
      {
        uint te = new_var & 0xfffffffb;
        *r = te & 0xfffffbff;
        status = clear_mask;
      }
    }

    kaze_memcard_clear_events_a();
  }
  return status;
  do_incr:
  kaze_memcard_poll_retry_count = kaze_memcard_poll_retry_count + 1;

  return status;
}
