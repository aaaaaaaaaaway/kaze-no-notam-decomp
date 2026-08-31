


/**
 * kaze_model_set_rotation_component_3489c  (render, 0x8003489c)
 *
 * Setter: writes arg2<<16 (fixed point) into the model draw struct rotation
 * array at arg0+8+idx*4 consumed by the model draw dispatch.
 */

void kaze_model_set_rotation_component_3489c(int arg0, int arg1, int arg2)
{
    *(int *)(arg0 + arg1 * 4 + 8) = arg2 << 16;
}


/**
 * kaze_model_set_translation_component_348b0  (render, 0x800348b0)
 *
 * Setter: writes arg2<<16 (fixed point) into the model draw struct
 * translation array at arg0+0x28+idx*4 consumed by the model draw dispatch.
 */

void kaze_model_set_translation_component_348b0(int arg0, int arg1, int arg2)
{
    *(int *)(arg0 + arg1 * 4 + 0x28) = arg2 << 16;
}
