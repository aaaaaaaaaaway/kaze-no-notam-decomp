#include "kaze_syms.h"
extern char kaze_menu_model_pack_path[];
extern char kaze_menu_model_txp_path[];
extern int kaze_menu_fade_transition_state;
extern int kaze_menu_fade_transition_alpha;
extern int kaze_menu_model_ptr;
extern int D_800B169C;
extern int D_800B16A0;
extern int D_800B194C;
extern int kaze_menu_vcount_timestamp;
extern int kaze_menu_balloon_model_ptr;
extern int kaze_menu_task_model_ptr;


extern int kaze_menu_ot_work_buf0[];
extern char kaze_menu_gs_ot_buf[];
extern void GsInitVcount(void);
extern int VSync(int);
extern void kaze_gs_clear_vcount(void);
extern int GsGetActiveBuff(void);
extern void kaze_gs_set_work_base(int);
extern void GsClearOt(int, int, void *);
extern void kaze_drmove_addprim_a0ec(int, int);
extern int DrawSync(int);
extern int GsGetVcount(void);
extern void GsSwapDispBuff(void);
extern void GsSortClear(int, int, int, void *);
extern void GsDrawOt(void *);

/**
 * kaze_menu_fade_transition  (menu, 0x80050d0c)
 *
 * Menu fade with its own VSync/GsOT loop: param 0 fades in
 * (kaze_menu_fade_transition_alpha 0xff down past -0x10), param 1 fades out
 * (0 up past 0x10f) via the kaze_quad_addprim_color overlay, silencing BGM
 * (kaze_xa_audio_manager_35a0(-1,1)).
 */

void kaze_menu_fade_transition(int fade_out)
{
    int active;
    int ot_index;
    int state;
    char *ot_base;

    if (fade_out == 0) {
        kaze_menu_fade_transition_state = 0;
        kaze_menu_fade_transition_alpha = 0xff;
    } else {
        kaze_menu_fade_transition_state = 1;
        kaze_menu_fade_transition_alpha = 0;
    }
    GsInitVcount();
    VSync(0);
    kaze_gs_clear_vcount();

    if (kaze_menu_fade_transition_state != 9) {
        ot_base = kaze_menu_gs_ot_buf;
        do {
            active = GsGetActiveBuff();
            ot_index = active * 4;
            kaze_gs_set_work_base(kaze_menu_ot_work_buf0[active]);
            ot_index = (ot_index + active) * 4;
            GsClearOt(0, 0, (void *)(ot_index + (int)ot_base));

            state = kaze_menu_fade_transition_state;
            if (state == 0) goto block0;
            if (state == 1) goto block1;
            goto after;

block0:
            kaze_drmove_addprim_a0ec(active, 0xfff);
            if (kaze_menu_fade_transition_alpha > 0) {
                kaze_quad_addprim_color(active, kaze_menu_fade_transition_alpha & 0xff);
            } else if (kaze_menu_fade_transition_alpha < -0x10) {
                kaze_menu_fade_transition_state = 9;
            }
            kaze_menu_fade_transition_alpha = kaze_menu_fade_transition_alpha - 0x10;
            goto after;

block1:
            if (kaze_menu_fade_transition_alpha < 0xff) {
                kaze_drmove_addprim_a0ec(active, 0xfff);
                kaze_quad_addprim_color(active, kaze_menu_fade_transition_alpha & 0xff);
            } else if (0x10f < kaze_menu_fade_transition_alpha) {
                kaze_menu_fade_transition_state = 9;
            }
            kaze_menu_fade_transition_alpha = kaze_menu_fade_transition_alpha + 0x10;

after:
            DrawSync(0);
            kaze_menu_vcount_timestamp = GsGetVcount();
            VSync(0);
            kaze_gs_clear_vcount();
            GsSwapDispBuff();
            GsSortClear(0, 0, 0, (void *)(active * 20 + (int)ot_base));
            GsDrawOt((void *)(active * 20 + (int)ot_base));
            if (kaze_menu_fade_transition_state == 1) {
                kaze_xa_audio_manager_35a0(-1, 1);
            }
        } while (kaze_menu_fade_transition_state != 9);
    }
}

extern int kaze_menu_mode_preview_tim_table;
extern int kaze_file_load_by_name(char *);
extern void kaze_gs_model_pair_link_init_9604(void);

/**
 * kaze_menu_model_load  (menu, 0x80050f48)
 *
 * Loads the menu model pack BAL\K\GM.MDL, resolves six section pointers
 * (kaze_menu_balloon_model_ptr, kaze_menu_mode_preview_tim_table,
 * kaze_menu_task_model_ptr, and three further unnamed model-pack pointer
 * slots), then loads BAL\K\GM.TXP textures.
 */

void kaze_menu_model_load(void)
{
    int base;
    register int off4 asm("$4");
    register int off8 asm("$5");
    register int off10 asm("$3");
    register int offc asm("$6");
    register int off14 asm("$3");
    int off18;

    base = kaze_file_load_by_name(kaze_menu_model_pack_path);
    off4 = *(int *)(base + 4);
    off8 = *(int *)(base + 8);
    off10 = *(int *)(base + 0x10);
    offc = *(int *)(base + 0xc);
    kaze_menu_model_ptr = base;
    off4 = base + off4;
    off10 = base + off10;
    kaze_menu_mode_preview_tim_table = off10;
    off14 = *(int *)(base + 0x14);
    off8 = base + off8;
    off14 = base + off14;
    D_800B169C = off14;
    off18 = *(int *)(base + 0x18);
    offc = base + offc;
    kaze_menu_balloon_model_ptr = off4;
    D_800B194C = off8;
    kaze_menu_task_model_ptr = offc;
    D_800B16A0 = base + off18;
    kaze_gs_model_pair_link_init_9604();
    kaze_task_glyph_models_link();
    kaze_txp_archive_load_vram(kaze_menu_model_txp_path);
}
