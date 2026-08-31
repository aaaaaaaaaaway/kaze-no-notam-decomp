/**
 * kaze_screen_composite_draw  (render, 0x8003180c)
 *
 * Appends the two full-screen POLY_FT4 overlay halves (+0x487c4/+0x487ec)
 * and a black fill POLY_F4 for screen composite/transitions.
 */


extern char *kaze_state_base_ptr;

extern void AddPrim(void *, void *);

void kaze_screen_composite_draw(void *arg)
{
    int off = 0x487C4;

    AddPrim(arg, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 80 + off));
    AddPrim(arg, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 80 + off) + 0x28);
    AddPrim(arg, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 24 + 0x48864));
}
