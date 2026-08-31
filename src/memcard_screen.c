#include "kaze_syms.h"
extern int kaze_memcard_active_slot_index;
extern int kaze_memcard_intro_scroll_timer;
extern int kaze_memcard_menu_row;
extern int D_800AE090;
extern int kaze_menu_vcount_timestamp;
extern int kaze_memcard_state_machine(int);


extern int kaze_memcard_work_buffer;
extern int kaze_menu_ot_work_buf0[];
extern char kaze_menu_gs_ot_buf[];
extern int kaze_memcard_slot_request_state[];
extern char kaze_sound_mode_setting;
void kaze_memcard_screen_init();
extern void GsInitVcount(void);
extern int VSync(int);
extern void kaze_gs_clear_vcount(void);
extern int GsGetActiveBuff(void);
extern void kaze_gs_set_work_base(int);
extern void GsClearOt(int, int, void *);
extern int DrawSync(int);
extern int GsGetVcount(void);
extern void kaze_input_edge_detect(void);
extern void GsSwapDispBuff(void);
extern void GsDrawOt(void *);
extern void kaze_heap_free(int);
extern void kaze_xa_stream_enable_33b0(void);
extern void kaze_xa_stream_disable_33e8(void);

/**
 * kaze_memcard_screen_loop  (menu, 0x8005ff6c)
 *
 * Memory-card screen main loop: init kaze_memcard_screen_init, per-frame
 * packet area/OT clear/state draw (kaze_memcard_state_dispatch) until
 * kaze_memory_card_prompt_latch==99; busy states service card slots
 * (kaze_memcard_slot_poll_start/b2e4/bde4/b56c, alternating
 * kaze_memcard_active_slot_index) instead of input; frees
 * kaze_memcard_work_buffer and runs kaze_xa_stream_enable_33b0/3e8 per
 * kaze_sound_mode_setting.
 */

void kaze_memcard_screen_loop(void)
{
    extern unsigned int kaze_memory_card_prompt_latch;
  int active;
  int ot_index;
  int new_var;
  int state;
  char *ot_base;
  kaze_memcard_events_open();
  kaze_memcard_screen_init();
  GsInitVcount();
  VSync(0);
  kaze_gs_clear_vcount();
  if (kaze_memory_card_prompt_latch != 0x63)
  {
    ot_base = kaze_menu_gs_ot_buf;
    do
    {
      active = GsGetActiveBuff();
      ot_index = active * 4;
      kaze_gs_set_work_base(kaze_menu_ot_work_buf0[active]);
      ot_index = (ot_index + active) * 4;
      GsClearOt(0, 0, (void *) (ot_index + ((int) ot_base)));
      kaze_memcard_state_dispatch(active);
      DrawSync(0);
      kaze_menu_vcount_timestamp = GsGetVcount();
      VSync(0);
      kaze_gs_clear_vcount();
      if (kaze_memcard_busy_state_check() != 0)
      {
 do { kaze_memcard_slot_request_state[kaze_memcard_active_slot_index] |= 1; } while (0);
        if (kaze_memcard_active_slot_index)
        {
          kaze_memcard_slot_poll_start();
          state = kaze_memcard_state_machine(kaze_memcard_active_slot_index);
          kaze_input_edge_detect_memcard(kaze_memcard_active_slot_index, state);
        }
        else
        {
          kaze_memcard_slot_poll_start();
          state = kaze_memcard_state_machine(kaze_memcard_active_slot_index);
          kaze_input_edge_detect_memcard(kaze_memcard_active_slot_index, state);
        }
        if (kaze_memcard_result_decode(kaze_memcard_active_slot_index, state) != 0)
        {
          kaze_memcard_active_slot_index = (kaze_memcard_active_slot_index + 1) & 1;
        }
      }
      else
      {
        kaze_input_edge_detect();
      }
      GsSwapDispBuff();
      GsDrawOt((void *) ((active * 20) + ((int) ot_base)));
      kaze_xa_audio_manager_35a0(0xD, ((kaze_menu_vcount_timestamp << 1) / 0x20D) + 1);
    }
    while (kaze_memory_card_prompt_latch != 0x63);
  }
  kaze_heap_free(new_var = kaze_memcard_work_buffer);
  kaze_memcard_events_close();
  if (kaze_sound_mode_setting == 0)
  {
    goto enable;
  }
  if (kaze_sound_mode_setting == 1)
  {
    goto disable;
  }
  goto done;
  enable:
  kaze_xa_stream_enable_33b0();

  goto done;
  disable:
  kaze_xa_stream_disable_33e8();

  done:
  ;

  ;
}

extern int kaze_memcard_file_image;
extern int kaze_memcard_slot_index;
extern int kaze_heap_alloc(int, int);

/**
 * kaze_memcard_screen_init  (menu, 0x800601a0)
 *
 * Memcard screen init: allocates the 0x2000 work buffer
 * kaze_memcard_work_buffer (file image at +0x200), resets state
 * kaze_memory_card_prompt_latch/070/074/078 and sets an unnamed memcard-
 * status flag (between kaze_memcard_status_retry_count and
 * kaze_memcard_slot_poll_counter) to 1, then builds screen prims via
 * kaze_slot_overlay_header_sprites_init/61f78/620b4/62270.
 */

void kaze_memcard_screen_init(void)
{
    extern int kaze_memory_card_prompt_latch;
    int one;

    kaze_memcard_work_buffer = kaze_heap_alloc(0x2000, 1);
    kaze_memcard_file_image = kaze_memcard_work_buffer + 0x200;
    one = 1;
    kaze_memory_card_prompt_latch = 0;
    kaze_memcard_intro_scroll_timer = 0;
    kaze_memcard_menu_row = 0;
    kaze_memcard_slot_index = 0;
    D_800AE090 = one;
    kaze_backbuffer_copy_offscreen();
    kaze_slot_overlay_header_sprites_init();
    kaze_slot_overlay_glyph_sprites_init();
    kaze_slot_overlay_row_sprites_init();
    kaze_slot_overlay_panel_prims_init();
    kaze_linef3_init();
}
