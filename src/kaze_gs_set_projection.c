/**
 * kaze_gs_set_projection  (library, 0x8006d240)
 *
 * PsyQ GsSetProjection: forwards the projection distance to SetGeomScreen
 * (kaze_set_geom_screen).
 */

extern void kaze_set_geom_screen(void);

void kaze_gs_set_projection(void)
{
    kaze_set_geom_screen();
}
