#include "types.h"
#include "kaze_syms.h"
extern int kaze_demo_attract_mode_pool[];
extern int kaze_demo_attract_variant_pool[];
extern int rand(void);
extern int D_800B17D4;

extern int kaze_staged_stage_index[];

/**
 * kaze_demo_attract_runner_34478  (core, 0x80034478)
 *
 * Attract/demo mode runner: randomizes the menu staging block
 * kaze_staged_stage_index..kaze_staged_stage_variant (mode/level/variant,
 * timers 0x3ae94) and repeatedly runs the main loop kaze_main_game_loop(1)
 * until kaze_title_screen_loop breaks.
 */

int kaze_demo_attract_runner_34478(void)
{
    int r;
    int q;

    while (1) {
        if (kaze_title_screen_loop() != 0) {
            break;
        }
        kaze_staged_stage_index[0] = 0;
        kaze_staged_stage_index[1] = 0;
        kaze_staged_stage_index[2] = kaze_demo_attract_mode_pool[rand() % 3];
        {
            int raw;
            int div;

            raw = rand();
            div = raw;
            if (raw < 0) {
                div = raw + 3;
            }
            kaze_staged_stage_index[3] = kaze_demo_attract_variant_pool[raw - ((div >> 2) << 2)];
        }
        kaze_staged_stage_index[5] = 0;
        r = rand();
        q = r;
        if (r < 0) {
            q = r + 7;
        }
        kaze_staged_stage_index[6] = r - ((q >> 3) << 3);
        kaze_staged_stage_index[11] = 0;
        kaze_staged_stage_index[8] = 0;
        kaze_staged_stage_index[7] = 0x3AE94;
        kaze_staged_stage_index[9] = 0x3AE94;
        kaze_main_game_loop(1);
    }
    return 1;
}

/**
 * kaze_state_oneshot_trigger_34580  (core, 0x80034580)
 *
 * One-shot state transition: when an unnamed flight-state flag reaches 1, it
 * invokes kaze_ending_screen_loop and advances that same flag to 2.
 */

void kaze_state_oneshot_trigger_34580(void)
{
    if (D_800B17D4 == 1) {
        kaze_ending_screen_loop();
        D_800B17D4 = 2;
    }
}

/**
 * kaze_prim_ot_link_insert_345bc  (library, 0x800345bc)
 *
 * Ordering-table/primitive-list splice helper: rewrites 24-bit next-pointers
 * to insert p1's primitive into the chain terminated at p3 (0xffffff
 * terminator).
 */

void kaze_prim_ot_link_insert_345bc(u32 *p1, u32 p2, u32 *p3)
{
    u32 old;

    old = *p1;
    *p1 = p2 & 0xffffff;
    while ((*p3 & 0xffffff) != 0xffffff) {
        p3 = (u32 *)((*p3 & 0xffffff) | 0x80000000);
    }
    *p3 = (*p3 & 0xff000000) | (old & 0xffffff);
}

/**
 * kaze_tmd_model_relocate_3462c  (render, 0x8003462c)
 *
 * Relocates a loaded TMD/model in place: verifies the -1 magic, sets the
 * relocated bit, and rebases 20 submodel pointer triples plus the
 * vertex/normal arrays by the model base address.
 */

void kaze_tmd_model_relocate_3462c(int *base)
{
    register int *orig __asm__("$7");
    int i;
    int count;
    int scratch[4];

    /* Empty frame anchor: matches the original 0x10 stack frame without code. */
    __asm__ volatile("" : "=m"(scratch));
    orig = base;
    if (base != 0 && *base == -1 && (base[1] & 1) == 0) {
        base[1] |= 1;
        {
            register int *triples __asm__("$5");
            register int loop_i __asm__("$6");

            base = base + 0x1A;
            loop_i = 0;
            triples = base + 3;
            do {
                if (*base != 0) {
                    register int left __asm__("$2");
                    register int right __asm__("$3");

                    left = triples[-2];
                    right = *triples;
                    left += (int)orig;
                    triples[-2] = left;
                    left = triples[-1];
                    right += (int)orig;
                    *triples = right;
                    left += (int)orig;
                    triples[-1] = left;
                }
                loop_i++;
                triples += 4;
                base += 4;
            } while (loop_i < 0x14);
        }

        {
            register int relocated __asm__("$2");
            register int *rebased __asm__("$3");

            relocated = *base;
            /* Tied no-code barriers hold the first list preheader order. */
            __asm__ volatile("" : "=r"(relocated) : "0"(relocated));
            i = 0;
            relocated += (int)orig;
            __asm__ volatile("" : "=r"(relocated) : "0"(relocated));
            count = base[1];
            rebased = (int *)relocated;
            *base = (int)rebased;
            if (count > 0) {
                do {
                    *rebased += (int)orig;
                    i++;
                    rebased++;
                } while (i < count);
            }
        }

        base += 2;
        {
            register int relocated __asm__("$2");
            register int *rebased __asm__("$3");

            relocated = *base + (int)orig;
            i = 0;
            count = base[1];
            rebased = (int *)relocated;
            *base = (int)rebased;
            if (count > 0) {
                do {
                    *rebased += (int)orig;
                    i++;
                    rebased++;
                } while (i < count);
            }
        }

        base += 2;
        *base += (int)orig;
        base += 2;
        *base += (int)orig;
    }
}
