/**
 * kaze_memcard_busy_state_check  (menu, 0x80060234)
 *
 * Returns 1 when the memcard screen state kaze_memory_card_prompt_latch is
 * 10 or 0xc, i.e. frames that must service memory-card events instead of
 * polling controller input.
 */


extern unsigned int kaze_memory_card_prompt_latch;

int kaze_memcard_busy_state_check(void)
{
    switch (kaze_memory_card_prompt_latch) {
    case 10:
    case 12:
        return 1;
    case 0:
    case 1:
    case 0x1e:
    default:
        return 0;
    }
}
