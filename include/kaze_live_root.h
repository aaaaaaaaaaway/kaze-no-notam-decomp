#ifndef KAZE_LIVE_ROOT_H
#define KAZE_LIVE_ROOT_H

#include "types.h"

/* KazeLiveRoot: the 0xed430-byte live game-state block allocated by
 * kaze_main_game_loop and pointed to by kaze_state_base_ptr.
 * REFERENCE ONLY: matched sources access this block through
 * char*-plus-offset arithmetic (a matching constraint); this struct lays
 * out the same fields by name for readers and for new (non-matching)
 * code. Gaps between known fields are pads. */

typedef struct KazeLiveRoot {
    s32 kaze_state_mode; /* +0x000000 */
    s32 kaze_state_phase; /* +0x000004 */
    s32 kaze_state_submode; /* +0x000008 */
    s32 kaze_state_result_score; /* +0x00000c */
    s32 kaze_state_next_route; /* +0x000010 */
    u32 kaze_state_flags; /* +0x000014 */
    s32 kaze_terrain_height_layer; /* +0x000018 */
    s32 kaze_root_init_latch; /* +0x00001c */
    s32 kaze_wind_transition_latch; /* +0x000020 */
    s32 kaze_checkpoint_count; /* +0x000024 */
    s16 kaze_root_rotation_matrix_00; /* +0x000028 */
    s16 kaze_root_rotation_matrix_01; /* +0x00002a */
    s16 kaze_root_rotation_matrix_02; /* +0x00002c */
    s16 kaze_root_rotation_matrix_10; /* +0x00002e */
    s16 kaze_root_rotation_matrix_11; /* +0x000030 */
    s16 kaze_root_rotation_matrix_12; /* +0x000032 */
    s16 kaze_root_rotation_matrix_20; /* +0x000034 */
    s16 kaze_root_rotation_matrix_21; /* +0x000036 */
    s16 kaze_root_rotation_matrix_22; /* +0x000038 */
    u8 _pad0[0xe];
    u16 kaze_view_pitch; /* +0x000048 */
    u16 kaze_view_heading; /* +0x00004a */
    u16 kaze_view_roll; /* +0x00004c */
    u8 _pad1[0x2];
    s32 kaze_route_anchor_x; /* +0x000050 */
    s32 kaze_route_anchor_y; /* +0x000054 */
    s32 kaze_route_anchor_z; /* +0x000058 */
    u8 _pad2[0x4];
    s32 kaze_model_route_index; /* +0x000060 */
    u8 _pad3[0x4];
    s32 kaze_object_pos_x; /* +0x000068 */
    s32 kaze_object_pos_y; /* +0x00006c */
    s32 kaze_object_pos_z; /* +0x000070 */
    u8 _pad4[0x4];
    s32 kaze_object_prev_pos_x; /* +0x000078 */
    s32 kaze_object_prev_pos_y; /* +0x00007c */
    s32 kaze_object_prev_pos_z; /* +0x000080 */
    u8 _pad5[0x4];
    s32 kaze_object_route_offset_x; /* +0x000088 */
    u8 _pad6[0x4];
    s32 kaze_object_route_offset_z; /* +0x000090 */
    u8 _pad7[0x4];
    s32 kaze_render_page_index; /* +0x000098 */
    s32 kaze_render_primitive_cursor; /* +0x00009c */
    s32 kaze_render_load_metric; /* +0x0000a0 */
    s32 kaze_state_step_delta; /* +0x0000a4 */
    u32 kaze_draw_env0_base; /* +0x0000a8 */
    u8 _pad8[0x12];
    u8 kaze_draw_env0_dtd; /* +0x0000be */
    u8 kaze_draw_env0_dfe; /* +0x0000bf */
    u8 kaze_draw_env0_isbg; /* +0x0000c0 */
    u8 _pad9[0x43];
    u32 kaze_disp_env0_base; /* +0x000104 */
    u8 _pad10[0x4018];
    u32 kaze_map_screen_packet_area_a; /* +0x004120 */
    u8 _pad11[0x1fffc];
    u32 kaze_draw_env1_base; /* +0x024120 */
    u8 _pad12[0x12];
    u8 kaze_draw_env1_dtd; /* +0x024136 */
    u8 kaze_draw_env1_dfe; /* +0x024137 */
    u8 kaze_draw_env1_isbg; /* +0x024138 */
    u8 _pad13[0x43];
    u32 kaze_disp_env1_base; /* +0x02417c */
    u8 _pad14[0x4018];
    u32 kaze_map_screen_packet_area_b; /* +0x028198 */
    u8 _pad15[0x1fffc];
    s16 kaze_color_matrix_00; /* +0x048198 */
    s16 kaze_color_matrix_01; /* +0x04819a */
    s16 kaze_color_matrix_02; /* +0x04819c */
    s16 kaze_color_matrix_10; /* +0x04819e */
    s16 kaze_color_matrix_11; /* +0x0481a0 */
    s16 kaze_color_matrix_12; /* +0x0481a2 */
    s16 kaze_color_matrix_20; /* +0x0481a4 */
    s16 kaze_color_matrix_21; /* +0x0481a6 */
    s16 kaze_color_matrix_22; /* +0x0481a8 */
    u8 _pad16[0xe];
    s16 kaze_light_matrix_00; /* +0x0481b8 */
    s16 kaze_light_matrix_01; /* +0x0481ba */
    s16 kaze_light_matrix_02; /* +0x0481bc */
    s16 kaze_light_matrix_10; /* +0x0481be */
    s16 kaze_light_matrix_11; /* +0x0481c0 */
    s16 kaze_light_matrix_12; /* +0x0481c2 */
    s16 kaze_light_matrix_20; /* +0x0481c4 */
    s16 kaze_light_matrix_21; /* +0x0481c6 */
    s16 kaze_light_matrix_22; /* +0x0481c8 */
    u8 _pad17[0xe];
    s32 kaze_environment_normal_color_r; /* +0x0481d8 */
    s32 kaze_environment_normal_color_g; /* +0x0481dc */
    s32 kaze_environment_normal_color_b; /* +0x0481e0 */
    u8 _pad18[0x4];
    s32 kaze_environment_light_r; /* +0x0481e8 */
    s32 kaze_environment_light_g; /* +0x0481ec */
    s32 kaze_environment_light_b; /* +0x0481f0 */
    u8 _pad19[0x4];
    s32 kaze_modal_sound_latch; /* +0x0481f8 */
    s32 kaze_sound_goal_loop_active; /* +0x0481fc */
    s32 kaze_post_event_sound_latch; /* +0x048200 */
    s16 kaze_draw_area_origin_x; /* +0x048204 */
    s16 kaze_draw_area_origin_y; /* +0x048206 */
    s16 kaze_draw_area_width; /* +0x048208 */
    s16 kaze_draw_area_height; /* +0x04820a */
    u32 kaze_draw_area_prim; /* +0x04820c */
    u8 _pad20[0x14];
    u32 kaze_mapdata_cdl_file; /* +0x048224 */
    u32 kaze_mapdata_cdl_size; /* +0x048228 */
    u32 kaze_mapdata_cdl_name0; /* +0x04822c */
    u32 kaze_mapdata_cdl_name1; /* +0x048230 */
    u32 kaze_mapdata_cdl_name2; /* +0x048234 */
    u32 kaze_mapdata_cdl_name3; /* +0x048238 */
    u32 kaze_texdata_cdl_file; /* +0x04823c */
    u32 kaze_texdata_cdl_size; /* +0x048240 */
    u32 kaze_texdata_cdl_name0; /* +0x048244 */
    u32 kaze_texdata_cdl_name1; /* +0x048248 */
    u32 kaze_texdata_cdl_name2; /* +0x04824c */
    u32 kaze_texdata_cdl_name3; /* +0x048250 */
    u32 kaze_rmode_cdl_file; /* +0x048254 */
    u32 kaze_rmode_cdl_size; /* +0x048258 */
    u32 kaze_rmode_cdl_name0; /* +0x04825c */
    u32 kaze_rmode_cdl_name1; /* +0x048260 */
    u32 kaze_rmode_cdl_name2; /* +0x048264 */
    u32 kaze_rmode_cdl_name3; /* +0x048268 */
    s32 kaze_environment_light_sin; /* +0x04826c */
    s32 kaze_environment_light_cos; /* +0x048270 */
    u8 _pad21[0x8];
    s32 kaze_environment_effect_intensity; /* +0x04827c */
    s32 kaze_timer_window; /* +0x048280 */
    s32 kaze_timer_total; /* +0x048284 */
    u16 kaze_environment_base_color_r; /* +0x048288 */
    u16 kaze_environment_base_color_g; /* +0x04828a */
    u16 kaze_environment_base_color_b; /* +0x04828c */
    u16 kaze_environment_color0_r; /* +0x04828e */
    u16 kaze_environment_color0_g; /* +0x048290 */
    u16 kaze_environment_color0_b; /* +0x048292 */
    u16 kaze_environment_color1_r; /* +0x048294 */
    u16 kaze_environment_color1_g; /* +0x048296 */
    u16 kaze_environment_color1_b; /* +0x048298 */
    u16 kaze_environment_color2_r; /* +0x04829a */
    u16 kaze_environment_color2_g; /* +0x04829c */
    u16 kaze_environment_color2_b; /* +0x04829e */
    u8 _pad22[0x10];
    u16 kaze_environment_color_lookup_variant1_b; /* +0x0482b0 */
    u8 _pad23[0x42];
    u32 kaze_environment_poly_f4_slot0_tag; /* +0x0482f4 */
    u8 kaze_environment_poly_f4_color_r; /* +0x0482f8 */
    u8 _pad24[0x13];
    u32 kaze_env_water_horizon_fill_poly; /* +0x04830c */
    u8 _pad25[0x14];
    u32 kaze_environment_poly_f4_copy_slot0_tag; /* +0x048324 */
    u8 _pad26[0x2c];
    u32 kaze_environment_poly_g4_set0_slot0_tag; /* +0x048354 */
    u8 _pad27[0x20];
    u32 kaze_env_sky_polyg4_1; /* +0x048378 */
    u8 _pad28[0x20];
    u32 kaze_env_sky_polyg4_2; /* +0x04839c */
    u8 _pad29[0x20];
    u32 kaze_env_sky_polyg4_3; /* +0x0483c0 */
    u8 _pad30[0x12];
    u8 kaze_env_sky_layer1_draw_flag; /* +0x0483d6 */
    u8 _pad31[0xd];
    u32 kaze_env_sky_polyg4_4; /* +0x0483e4 */
    u8 _pad32[0x20];
    u32 kaze_env_sky_polyg4_5; /* +0x048408 */
    u8 _pad33[0x12];
    u8 kaze_env_sky_layer2_draw_flag; /* +0x04841e */
    u8 _pad34[0x5];
    u32 kaze_environment_poly_g4_set0_copy_end; /* +0x048424 */
    u8 _pad35[0x4];
    u32 kaze_environment_poly_g4_set0_copy_slot0_tag; /* +0x04842c */
    u8 _pad36[0xd4];
    u32 kaze_environment_poly_g4_set1_slot0_tag; /* +0x048504 */
    u8 _pad37[0x20];
    u32 kaze_env_water_polyg4_1; /* +0x048528 */
    u8 _pad38[0x20];
    u32 kaze_env_water_polyg4_2; /* +0x04854c */
    u8 _pad39[0x20];
    u32 kaze_env_water_polyg4_3; /* +0x048570 */
    u8 _pad40[0x2];
    u8 kaze_env_water_layer1_draw_flag; /* +0x048576 */
    u8 _pad41[0x41];
    u32 kaze_env_water_polyg4_5; /* +0x0485b8 */
    u8 _pad42[0x18];
    u32 kaze_environment_poly_g4_set1_copy_end; /* +0x0485d4 */
    u8 _pad43[0x4];
    u32 kaze_environment_poly_g4_set1_copy_slot0_tag; /* +0x0485dc */
    u8 _pad44[0xd4];
    u32 kaze_env_sky_tpage_0; /* +0x0486b4 */
    u8 _pad45[0x4];
    u32 kaze_env_sky_tpage_1; /* +0x0486bc */
    u8 _pad46[0x4];
    u32 kaze_env_sky_tpage_2; /* +0x0486c4 */
    u8 _pad47[0x1c];
    u32 kaze_env_water_tpage_0; /* +0x0486e4 */
    u8 _pad48[0x4];
    u32 kaze_env_water_tpage_1; /* +0x0486ec */
    u8 _pad49[0x4];
    u32 kaze_env_water_tpage_2; /* +0x0486f4 */
    u8 _pad50[0x1c];
    u32 kaze_route_sprite_slot0_tag; /* +0x048714 */
    u8 _pad51[0x4];
    s16 kaze_route_sprite0_x0; /* +0x04871c */
    s16 kaze_route_sprite0_y0; /* +0x04871e */
    u8 _pad52[0x1];
    u8 kaze_route_sprite_slot0_uv_v; /* +0x048721 */
    u16 kaze_route_sprite_slot0_clut; /* +0x048722 */
    u8 _pad53[0x4];
    u32 kaze_route_sprite1; /* +0x048728 */
    u8 _pad54[0x10];
    u32 kaze_route_sprite2; /* +0x04873c */
    u8 _pad55[0x10];
    u32 kaze_route_sprite3; /* +0x048750 */
    u8 _pad56[0x10];
    u32 kaze_route_sprite_copy_slot0_tag; /* +0x048764 */
    u8 _pad57[0x4c];
    u32 kaze_route_sprite_draw_tpage0_tag; /* +0x0487b4 */
    u8 _pad58[0x4];
    u32 kaze_route_sprite_draw_tpage1_tag; /* +0x0487bc */
    u8 _pad59[0x4];
    u32 kaze_post_event_split_polyft4_slot0_tag; /* +0x0487c4 */
    u8 kaze_post_event_split_polyft4_slot0_a_color_r; /* +0x0487c8 */
    u8 kaze_post_event_split_polyft4_slot0_a_color_g; /* +0x0487c9 */
    u8 kaze_post_event_split_polyft4_slot0_a_color_b; /* +0x0487ca */
    u8 _pad60[0xd];
    u8 kaze_post_event_split_polyft4_slot0_a_uv0_u; /* +0x0487d8 */
    u8 kaze_post_event_split_polyft4_slot0_a_uv0_v; /* +0x0487d9 */
    u16 kaze_post_event_split_polyft4_slot0_tpage_a; /* +0x0487da */
    u8 _pad61[0x4];
    u8 kaze_post_event_split_polyft4_slot0_a_uv1_u; /* +0x0487e0 */
    u8 kaze_post_event_split_polyft4_slot0_a_uv1_v; /* +0x0487e1 */
    u8 _pad62[0x6];
    u8 kaze_post_event_split_polyft4_slot0_a_uv2_u; /* +0x0487e8 */
    u8 kaze_post_event_split_polyft4_slot0_a_uv2_v; /* +0x0487e9 */
    u8 _pad63[0x2];
    u32 kaze_screen_overlay_ft4_right; /* +0x0487ec */
    u8 kaze_post_event_split_polyft4_slot0_b_color_r; /* +0x0487f0 */
    u8 kaze_post_event_split_polyft4_slot0_b_color_g; /* +0x0487f1 */
    u8 kaze_post_event_split_polyft4_slot0_b_color_b; /* +0x0487f2 */
    u8 _pad64[0x5];
    u8 kaze_post_event_split_polyft4_slot0_b_uv0_u; /* +0x0487f8 */
    u8 kaze_post_event_split_polyft4_slot0_b_uv0_v; /* +0x0487f9 */
    u8 _pad65[0x6];
    u8 kaze_post_event_split_polyft4_slot0_b_uv1_u; /* +0x048800 */
    u8 kaze_post_event_split_polyft4_slot0_b_uv1_v; /* +0x048801 */
    u8 _pad66[0x6];
    u8 kaze_post_event_split_polyft4_slot0_b_uv2_u; /* +0x048808 */
    u8 kaze_post_event_split_polyft4_slot0_b_uv2_v; /* +0x048809 */
    u8 _pad67[0x6];
    u8 kaze_post_event_split_polyft4_slot0_b_uv3_u; /* +0x048810 */
    u8 kaze_post_event_split_polyft4_slot0_b_uv3_v; /* +0x048811 */
    u8 _pad68[0x18];
    u16 kaze_post_event_split_polyft4_slot0_tpage_b; /* +0x04882a */
    u8 _pad69[0x38];
    u32 kaze_post_event_split_polyf4_backdrop_slot0_tag; /* +0x048864 */
    u8 kaze_post_event_split_polyf4_backdrop_slot0_color_r; /* +0x048868 */
    u8 kaze_post_event_split_polyf4_backdrop_slot0_color_g; /* +0x048869 */
    u8 kaze_post_event_split_polyf4_backdrop_slot0_color_b; /* +0x04886a */
    u8 _pad70[0x29];
    u32 kaze_post_event_split_polyf4_overlay_slot0_tag; /* +0x048894 */
    u8 kaze_post_event_split_polyf4_overlay_slot0_color_r; /* +0x048898 */
    u8 kaze_post_event_split_polyf4_overlay_slot0_color_g; /* +0x048899 */
    u8 kaze_post_event_split_polyf4_overlay_slot0_color_b; /* +0x04889a */
    u8 _pad71[0x29];
    u32 kaze_post_event_split_draw_tpage0_tag; /* +0x0488c4 */
    u8 _pad72[0xc];
    u32 kaze_balloon_overlay_ft4_tag; /* +0x0488d4 */
    u8 _pad73[0x4];
    s16 kaze_balloon_overlay_v0_x; /* +0x0488dc */
    s16 kaze_balloon_overlay_v0_y; /* +0x0488de */
    u8 kaze_balloon_overlay_slot0_uv0_u; /* +0x0488e0 */
    u8 kaze_balloon_overlay_slot0_uv0_v; /* +0x0488e1 */
    u16 kaze_balloon_overlay_slot0_clut; /* +0x0488e2 */
    s16 kaze_balloon_overlay_v1_x; /* +0x0488e4 */
    s16 kaze_balloon_overlay_v1_y; /* +0x0488e6 */
    u8 kaze_balloon_overlay_slot0_uv1_u; /* +0x0488e8 */
    u8 kaze_balloon_overlay_slot0_uv1_v; /* +0x0488e9 */
    u16 kaze_balloon_overlay_slot0_tpage; /* +0x0488ea */
    s16 kaze_balloon_overlay_v2_x; /* +0x0488ec */
    s16 kaze_balloon_overlay_v2_y; /* +0x0488ee */
    u8 kaze_balloon_overlay_slot0_uv2_u; /* +0x0488f0 */
    u8 kaze_balloon_overlay_slot0_uv2_v; /* +0x0488f1 */
    u8 _pad74[0x2];
    s16 kaze_balloon_overlay_v3_x; /* +0x0488f4 */
    s16 kaze_balloon_overlay_v3_y; /* +0x0488f6 */
    u8 kaze_balloon_overlay_slot0_uv3_u; /* +0x0488f8 */
    u8 kaze_balloon_overlay_slot0_uv3_v; /* +0x0488f9 */
    u8 _pad75[0x2a];
    u32 kaze_balloon_model_line_source_slot0_tag; /* +0x048924 */
    u8 kaze_balloon_model_line_source_color_r; /* +0x048928 */
    u8 kaze_balloon_model_line_source_color_g; /* +0x048929 */
    u8 kaze_balloon_model_line_source_color_b; /* +0x04892a */
    u8 _pad76[0x39];
    u32 kaze_balloon_model_line_copy_slot0_tag; /* +0x048964 */
    u8 _pad77[0x3c];
    u32 kaze_balloon_model_line_slot0_tag; /* +0x0489a4 */
    u8 kaze_balloon_model_line_color_r; /* +0x0489a8 */
    u8 kaze_balloon_model_line_color_g; /* +0x0489a9 */
    u8 kaze_balloon_model_line_color_b; /* +0x0489aa */
    u8 _pad78[0x1];
    u32 kaze_balloon_linef2_xy0; /* +0x0489ac */
    u32 kaze_balloon_linef2_xy1; /* +0x0489b0 */
    u32 kaze_postevent_icon_sprt; /* +0x0489b4 */
    u8 _pad79[0x9];
    u8 kaze_hud_sprite_slot0_uv_v; /* +0x0489c1 */
    u16 kaze_hud_sprite_slot0_clut; /* +0x0489c2 */
    u8 _pad80[0x18];
    u32 kaze_hud_sprite_draw_tpage_slot0_tag; /* +0x0489dc */
    u8 _pad81[0xc];
    u32 kaze_post_event_sprt16_slot0_tag; /* +0x0489ec */
    u8 kaze_hud_wind_layer_arrow0_r; /* +0x0489f0 */
    u8 kaze_hud_wind_layer_arrow0_g; /* +0x0489f1 */
    u8 kaze_hud_wind_layer_arrow0_b; /* +0x0489f2 */
    u8 _pad82[0x5];
    u8 kaze_hud_wind_layer_arrow0_u; /* +0x0489f8 */
    u8 kaze_hud_wind_layer_arrow0_v; /* +0x0489f9 */
    u16 kaze_post_event_sprt16_slot0_clut; /* +0x0489fa */
    u8 _pad83[0x90];
    u32 kaze_postevent_sprt_grp_a8c; /* +0x048a8c */
    u8 _pad84[0x4];
    s16 kaze_hud_compass_cardinal0_x; /* +0x048a94 */
    s16 kaze_hud_compass_cardinal0_y; /* +0x048a96 */
    u8 _pad85[0x2];
    u16 kaze_post_event_grid_sprt_slot0_clut; /* +0x048a9a */
    u8 _pad86[0x90];
    u32 kaze_post_event_status_sprt8_slot0_tag; /* +0x048b2c */
    u8 _pad87[0x4];
    s16 kaze_hud_altitude_marker_x; /* +0x048b34 */
    s16 kaze_hud_altitude_marker_y; /* +0x048b36 */
    u8 kaze_post_event_status_sprt8_slot0_uv_u; /* +0x048b38 */
    u8 kaze_post_event_status_sprt8_slot0_uv_v; /* +0x048b39 */
    u16 kaze_post_event_status_sprt8_slot0_clut; /* +0x048b3a */
    u8 _pad88[0x10];
    u32 kaze_post_event_footer_sprt8_slot0_tag; /* +0x048b4c */
    u8 _pad89[0x8];
    u8 kaze_post_event_footer_sprt8_slot0_uv_u; /* +0x048b58 */
    u8 kaze_post_event_footer_sprt8_slot0_uv_v; /* +0x048b59 */
    u16 kaze_post_event_footer_sprt8_slot0_clut; /* +0x048b5a */
    u8 _pad90[0x20];
    u32 kaze_post_event_footer_sprt8_copy_slot0_tag; /* +0x048b7c */
    u8 _pad91[0x2c];
    u32 kaze_post_event_footer_draw_tpage0_tag; /* +0x048bac */
    u8 _pad92[0x4];
    u32 kaze_post_event_footer_draw_tpage1_tag; /* +0x048bb4 */
    u8 _pad93[0x4];
    u32 kaze_postevent_polyft4_bbc; /* +0x048bbc */
    u8 _pad94[0x4];
    s16 kaze_hud_heading_needle_x0; /* +0x048bc4 */
    s16 kaze_hud_heading_needle_y0; /* +0x048bc6 */
    u8 kaze_post_event_texture_polyft4_slot0_uv0_u; /* +0x048bc8 */
    u8 kaze_post_event_texture_polyft4_slot0_uv0_v; /* +0x048bc9 */
    u16 kaze_post_event_texture_polyft4_slot0_clut; /* +0x048bca */
    s16 kaze_hud_heading_needle_x1; /* +0x048bcc */
    s16 kaze_hud_heading_needle_y1; /* +0x048bce */
    u8 kaze_post_event_texture_polyft4_slot0_uv1_u; /* +0x048bd0 */
    u8 kaze_post_event_texture_polyft4_slot0_uv1_v; /* +0x048bd1 */
    u16 kaze_post_event_texture_polyft4_slot0_tpage; /* +0x048bd2 */
    s16 kaze_hud_heading_needle_x2; /* +0x048bd4 */
    s16 kaze_hud_heading_needle_y2; /* +0x048bd6 */
    u8 kaze_post_event_texture_polyft4_slot0_uv2_u; /* +0x048bd8 */
    u8 kaze_post_event_texture_polyft4_slot0_uv2_v; /* +0x048bd9 */
    u8 _pad95[0x2];
    s16 kaze_hud_heading_needle_x3; /* +0x048bdc */
    s16 kaze_hud_heading_needle_y3; /* +0x048bde */
    u8 kaze_post_event_texture_polyft4_slot0_uv3_u; /* +0x048be0 */
    u8 kaze_post_event_texture_polyft4_slot0_uv3_v; /* +0x048be1 */
    u8 _pad96[0x2a];
    u32 kaze_post_event_frame_draw_tpage0_tag; /* +0x048c0c */
    u8 _pad97[0x4];
    u32 kaze_post_event_frame_draw_tpage1_tag; /* +0x048c14 */
    u8 _pad98[0x4];
    u32 kaze_postevent_sprt_c1c; /* +0x048c1c */
    u8 _pad99[0xa];
    u16 kaze_post_event_load_sprt_slot0_clut; /* +0x048c2a */
    u8 _pad100[0x18];
    u32 kaze_post_event_frame_poly_slot0_tag; /* +0x048c44 */
    u8 kaze_post_event_frame_poly_slot0_color_r_dynamic; /* +0x048c48 */
    u8 kaze_post_event_frame_poly_slot0_color_g_dynamic; /* +0x048c49 */
    u8 kaze_post_event_frame_poly_slot0_color_b_dynamic; /* +0x048c4a */
    u8 _pad101[0x3];
    s16 kaze_post_event_frame_poly_slot0_y0; /* +0x048c4e */
    u8 _pad102[0xc];
    u32 kaze_post_event_frame_poly_slot1_tag; /* +0x048c5c */
    u8 kaze_post_event_frame_poly_slot1_color_r; /* +0x048c60 */
    u8 kaze_post_event_frame_poly_slot1_color_g; /* +0x048c61 */
    u8 kaze_post_event_frame_poly_slot1_color_b; /* +0x048c62 */
    u8 _pad103[0x15];
    u8 kaze_post_event_frame_poly_slot2_color_r; /* +0x048c78 */
    u8 kaze_post_event_frame_poly_slot2_color_g; /* +0x048c79 */
    u8 kaze_post_event_frame_poly_slot2_color_b; /* +0x048c7a */
    u8 _pad104[0x15];
    u8 kaze_post_event_frame_poly_slot3_color_r; /* +0x048c90 */
    u8 kaze_post_event_frame_poly_slot3_color_g; /* +0x048c91 */
    u8 kaze_post_event_frame_poly_slot3_color_b; /* +0x048c92 */
    u8 _pad105[0x15];
    u8 kaze_post_event_frame_poly_slot4_color_r; /* +0x048ca8 */
    u8 kaze_post_event_frame_poly_slot4_color_g; /* +0x048ca9 */
    u8 kaze_post_event_frame_poly_slot4_color_b; /* +0x048caa */
    u8 _pad106[0x11];
    u32 kaze_post_event_frame_poly_slot5_tag; /* +0x048cbc */
    u8 kaze_post_event_frame_poly_slot5_color_r; /* +0x048cc0 */
    u8 kaze_post_event_frame_poly_slot5_color_g; /* +0x048cc1 */
    u8 kaze_post_event_frame_poly_slot5_color_b; /* +0x048cc2 */
    u8 _pad107[0x11];
    u32 kaze_post_event_frame_poly_copy_slot0_tag; /* +0x048cd4 */
    u8 _pad108[0x8c];
    u32 kaze_post_event_load_draw_tpage1_tag; /* +0x048d64 */
    u8 _pad109[0xc];
    u32 kaze_post_event_load_draw_tpage0_tag; /* +0x048d74 */
    u8 _pad110[0xc];
    u32 kaze_post_event_load_draw_load_slot0_tag; /* +0x048d84 */
    u8 _pad111[0xc];
    s16 kaze_trace_graph_vtx_buf; /* +0x048d94 */
    u8 _pad112[0x76];
    s32 kaze_post_event_menu_state; /* +0x048e0c */
    u16 kaze_post_event_menu_panel_width; /* +0x048e10 */
    u16 kaze_post_event_menu_panel_height; /* +0x048e12 */
    u16 kaze_post_event_confirm_panel_width; /* +0x048e14 */
    u16 kaze_post_event_confirm_panel_height; /* +0x048e16 */
    u16 kaze_post_event_confirm_panel_x; /* +0x048e18 */
    u16 kaze_post_event_confirm_panel_y; /* +0x048e1a */
    u32 kaze_postevent_bg_tpage; /* +0x048e1c */
    u8 _pad113[0xc];
    u32 kaze_post_event_panel_poly_group0_slot0_tag; /* +0x048e2c */
    u8 _pad114[0x6c];
    u32 kaze_post_event_panel_poly_group0_copy_end; /* +0x048e9c */
    u8 _pad115[0x4];
    u32 kaze_post_event_panel_poly_group0_copy_slot0_tag; /* +0x048ea4 */
    u8 _pad116[0x74];
    u32 kaze_post_event_linef3_group0_slot0_tag; /* +0x048f1c */
    u8 _pad117[0x5c];
    u32 kaze_post_event_linef3_group0_copy_slot0_tag; /* +0x048f7c */
    u8 _pad118[0x5c];
    u32 kaze_post_event_panel_poly_group1_slot0_tag; /* +0x048fdc */
    u8 _pad119[0x6c];
    u32 kaze_post_event_panel_poly_group1_copy_end; /* +0x04904c */
    u8 _pad120[0x4];
    u32 kaze_post_event_panel_poly_group1_copy_slot0_tag; /* +0x049054 */
    u8 _pad121[0x74];
    u32 kaze_post_event_linef3_group1_slot0_tag; /* +0x0490cc */
    u8 _pad122[0x5c];
    u32 kaze_post_event_linef3_group1_copy_slot0_tag; /* +0x04912c */
    u8 _pad123[0x5c];
    u32 kaze_post_event_menu_sprt_slot0_tag; /* +0x04918c */
    u8 _pad124[0x4];
    s16 kaze_post_event_menu_sprt_slot0_x; /* +0x049194 */
    u8 _pad125[0x4];
    u16 kaze_post_event_menu_sprt_slot0_clut; /* +0x04919a */
    u8 _pad126[0x68];
    u32 kaze_post_event_confirm_sprt_group0_slot0_tag; /* +0x049204 */
    u8 _pad127[0x4];
    s16 kaze_postevent_confirm_sprt0_x; /* +0x04920c */
    u8 _pad128[0x4];
    u16 kaze_post_event_confirm_sprt_slot0_clut; /* +0x049212 */
    u8 _pad129[0x4];
    u32 kaze_postevent_confirm_sprt1; /* +0x049218 */
    u8 _pad130[0x8];
    u8 kaze_post_event_confirm_sprt_slot1_uv_u; /* +0x049224 */
    u8 kaze_post_event_confirm_sprt_slot1_uv_v; /* +0x049225 */
    u8 _pad131[0x6];
    u32 kaze_postevent_confirm_sprt2; /* +0x04922c */
    u8 _pad132[0x8];
    u8 kaze_post_event_confirm_sprt_slot2_uv_u; /* +0x049238 */
    u8 kaze_post_event_confirm_sprt_slot2_uv_v; /* +0x049239 */
    u8 _pad133[0x6];
    u32 kaze_post_event_confirm_sprt_group0_slot3_tag; /* +0x049240 */
    u8 _pad134[0x4];
    s16 kaze_postevent_confirm_sprt3_x; /* +0x049248 */
    u8 _pad135[0x2];
    u8 kaze_post_event_confirm_sprt_slot3_uv_u; /* +0x04924c */
    u8 kaze_post_event_confirm_sprt_slot3_uv_v; /* +0x04924d */
    u8 _pad136[0x6];
    u32 kaze_post_event_confirm_sprt_group1_slot0_tag; /* +0x049254 */
    u8 _pad137[0x4c];
    s32 kaze_post_event_menu_option0_brightness; /* +0x0492a4 */
    s32 kaze_post_event_menu_option1_brightness; /* +0x0492a8 */
    s32 kaze_post_event_menu_option2_brightness; /* +0x0492ac */
    s32 kaze_post_event_confirm_option0_brightness; /* +0x0492b0 */
    s32 kaze_post_event_confirm_option1_brightness; /* +0x0492b4 */
    s32 kaze_post_event_confirm_choice; /* +0x0492b8 */
    s32 kaze_post_event_menu_choice; /* +0x0492bc */
    u16 kaze_post_event_confirm_panel_origin_x; /* +0x0492c0 */
    u16 kaze_post_event_confirm_panel_origin_y; /* +0x0492c2 */
    u32 kaze_post_event_dialog_sprt_slot0_tag; /* +0x0492c4 */
    u8 kaze_post_event_dialog_sprt_slot0_color_r; /* +0x0492c8 */
    u8 kaze_post_event_dialog_sprt_slot0_color_g; /* +0x0492c9 */
    u8 kaze_post_event_dialog_sprt_slot0_color_b; /* +0x0492ca */
    u8 _pad138[0x1];
    s16 kaze_radar_center_sprt_x; /* +0x0492cc */
    u8 _pad139[0x4];
    u16 kaze_post_event_dialog_sprt_slot0_clut; /* +0x0492d2 */
    u8 _pad140[0x18];
    u32 kaze_post_event_dialog_draw_tpage_slot0_tag; /* +0x0492ec */
    u8 _pad141[0xc];
    u32 kaze_post_event_panel_poly_group2_slot0_tag; /* +0x0492fc */
    u8 kaze_post_event_panel_poly_slot0_color_r; /* +0x049300 */
    u8 kaze_post_event_panel_poly_slot0_color_g; /* +0x049301 */
    u8 kaze_post_event_panel_poly_slot0_color_b; /* +0x049302 */
    u8 _pad142[0x69];
    u32 kaze_post_event_panel_poly_group2_copy_end; /* +0x04936c */
    u8 _pad143[0x4];
    u32 kaze_post_event_panel_poly_group2_copy_slot0_tag; /* +0x049374 */
    u8 _pad144[0x74];
    u32 kaze_post_event_linef3_group2_slot0_tag; /* +0x0493ec */
    u8 _pad145[0x5c];
    u32 kaze_post_event_linef3_group2_copy_slot0_tag; /* +0x04944c */
    u8 _pad146[0x5c];
    u32 kaze_post_event_icon_sprt8_slot0_tag; /* +0x0494ac */
    u8 _pad147[0x4];
    s16 kaze_radar_dot_vtx0_x; /* +0x0494b4 */
    u8 _pad148[0x4];
    u16 kaze_post_event_icon_sprt8_slot0_clut; /* +0x0494ba */
    u8 _pad149[0x8];
    s16 kaze_radar_dot_vtx1_x; /* +0x0494c4 */
    u8 _pad150[0xf6];
    u32 kaze_post_event_icon_sprt8_copy_slot0_tag; /* +0x0495bc */
    u8 _pad151[0x10c];
    u32 kaze_post_event_marker_sprt8_slot0_tag; /* +0x0496cc */
    u8 _pad152[0x4];
    s16 kaze_radar_needle_vtx_x; /* +0x0496d4 */
    u8 _pad153[0x2];
    u8 kaze_post_event_marker_sprt8_slot0_uv_u; /* +0x0496d8 */
    u8 kaze_post_event_marker_sprt8_slot0_uv_v; /* +0x0496d9 */
    u16 kaze_post_event_tile_sprt8_slot0_clut; /* +0x0496da */
    u8 _pad154[0x10];
    u32 kaze_post_event_marker_linef4_slot0_tag; /* +0x0496ec */
    u8 kaze_post_event_marker_linef4_slot0_color_r; /* +0x0496f0 */
    u8 kaze_post_event_marker_linef4_slot0_color_g; /* +0x0496f1 */
    u8 kaze_post_event_marker_linef4_slot0_color_b; /* +0x0496f2 */
    u8 _pad155[0x1];
    s16 kaze_route_minimap_poly_slot0_x0; /* +0x0496f4 */
    u8 _pad156[0x2e];
    u32 kaze_post_event_marker_polyf3_slot0_tag; /* +0x049724 */
    u8 kaze_post_event_marker_polyf3_slot0_color_r; /* +0x049728 */
    u8 kaze_post_event_marker_polyf3_slot0_color_g; /* +0x049729 */
    u8 kaze_post_event_marker_polyf3_slot0_color_b; /* +0x04972a */
    u8 _pad157[0x21];
    s32 kaze_marker_effect_brightness_phase; /* +0x04974c */
    u32 kaze_post_event_icon_sprt8_draw_tpage0_tag; /* +0x049750 */
    u8 _pad158[0x4];
    u32 kaze_post_event_icon_sprt8_draw_tpage1_tag; /* +0x049758 */
    u8 _pad159[0x4];
    u32 kaze_post_event_marker_polyf4_slot0_tag; /* +0x049760 */
    u8 kaze_post_event_marker_polyf4_slot0_color_r; /* +0x049764 */
    u8 kaze_post_event_marker_polyf4_slot0_color_g; /* +0x049765 */
    u8 kaze_post_event_marker_polyf4_slot0_color_b; /* +0x049766 */
    u8 _pad160[0x39];
    u32 kaze_post_event_marker_polyf4_copy_end; /* +0x0497a0 */
    u8 _pad161[0x4];
    u32 kaze_post_event_marker_polyf4_copy_slot0_tag; /* +0x0497a8 */
    u8 _pad162[0x44];
    u32 kaze_post_event_marker_polyf4_draw_tpage0_tag; /* +0x0497f0 */
    u8 _pad163[0x4];
    u32 kaze_post_event_marker_polyf4_draw_tpage1_tag; /* +0x0497f8 */
    u8 _pad164[0x4];
    u32 kaze_grid_polyft4_base; /* +0x049800 */
    u8 kaze_post_event_grid_polyft4_slot0_color_r; /* +0x049804 */
    u8 kaze_post_event_grid_polyft4_slot0_color_g; /* +0x049805 */
    u8 kaze_post_event_grid_polyft4_slot0_color_b; /* +0x049806 */
    u8 _pad165[0x6];
    u8 kaze_post_event_grid_polyft4_slot0_uv0_v; /* +0x04980d */
    u16 kaze_post_event_grid_polyft4_slot0_clut; /* +0x04980e */
    u8 _pad166[0x4];
    u8 kaze_post_event_grid_polyft4_slot0_uv1_u; /* +0x049814 */
    u8 kaze_post_event_grid_polyft4_slot0_uv1_v; /* +0x049815 */
    u16 kaze_post_event_grid_polyft4_slot0_tpage; /* +0x049816 */
    u8 _pad167[0x4];
    u8 kaze_post_event_grid_polyft4_slot0_uv2_u; /* +0x04981c */
    u8 kaze_post_event_grid_polyft4_slot0_uv2_v; /* +0x04981d */
    u8 _pad168[0x6];
    u8 kaze_post_event_grid_polyft4_slot0_uv3_u; /* +0x049824 */
    u8 kaze_post_event_grid_polyft4_slot0_uv3_v; /* +0x049825 */
    u8 _pad169[0xe];
    u8 kaze_post_event_grid_polyft4_slot1_uv0_u; /* +0x049834 */
    u8 kaze_post_event_grid_polyft4_slot1_uv0_v; /* +0x049835 */
    u8 _pad170[0x6];
    u8 kaze_post_event_grid_polyft4_slot1_uv1_u; /* +0x04983c */
    u8 kaze_post_event_grid_polyft4_slot1_uv1_v; /* +0x04983d */
    u8 _pad171[0x6];
    u8 kaze_post_event_grid_polyft4_slot1_uv2_u; /* +0x049844 */
    u8 kaze_post_event_grid_polyft4_slot1_uv2_v; /* +0x049845 */
    u8 _pad172[0x6];
    u8 kaze_post_event_grid_polyft4_slot1_uv3_u; /* +0x04984c */
    u8 kaze_post_event_grid_polyft4_slot1_uv3_v; /* +0x04984d */
    u8 _pad173[0xe];
    u8 kaze_post_event_grid_polyft4_slot2_uv0_u; /* +0x04985c */
    u8 kaze_post_event_grid_polyft4_slot2_uv0_v; /* +0x04985d */
    u8 _pad174[0x6];
    u8 kaze_post_event_grid_polyft4_slot2_uv1_u; /* +0x049864 */
    u8 kaze_post_event_grid_polyft4_slot2_uv1_v; /* +0x049865 */
    u8 _pad175[0x6];
    u8 kaze_post_event_grid_polyft4_slot2_uv2_u; /* +0x04986c */
    u8 kaze_post_event_grid_polyft4_slot2_uv2_v; /* +0x04986d */
    u8 _pad176[0x6];
    u8 kaze_post_event_grid_polyft4_slot2_uv3_u; /* +0x049874 */
    u8 kaze_post_event_grid_polyft4_slot2_uv3_v; /* +0x049875 */
    u8 _pad177[0xe];
    u8 kaze_post_event_grid_polyft4_slot3_uv0_u; /* +0x049884 */
    u8 kaze_post_event_grid_polyft4_slot3_uv0_v; /* +0x049885 */
    u8 _pad178[0x6];
    u8 kaze_post_event_grid_polyft4_slot3_uv1_u; /* +0x04988c */
    u8 kaze_post_event_grid_polyft4_slot3_uv1_v; /* +0x04988d */
    u8 _pad179[0x6];
    u8 kaze_post_event_grid_polyft4_slot3_uv2_u; /* +0x049894 */
    u8 kaze_post_event_grid_polyft4_slot3_uv2_v; /* +0x049895 */
    u8 _pad180[0x6];
    u8 kaze_post_event_grid_polyft4_slot3_uv3_u; /* +0x04989c */
    u8 kaze_post_event_grid_polyft4_slot3_uv3_v; /* +0x04989d */
    u8 _pad181[0xe];
    u8 kaze_post_event_grid_polyft4_slot4_uv0_u; /* +0x0498ac */
    u8 kaze_post_event_grid_polyft4_slot4_uv0_v; /* +0x0498ad */
    u8 _pad182[0x6];
    u8 kaze_post_event_grid_polyft4_slot4_uv1_u; /* +0x0498b4 */
    u8 kaze_post_event_grid_polyft4_slot4_uv1_v; /* +0x0498b5 */
    u8 _pad183[0x6];
    u8 kaze_post_event_grid_polyft4_slot4_uv2_u; /* +0x0498bc */
    u8 kaze_post_event_grid_polyft4_slot4_uv2_v; /* +0x0498bd */
    u8 _pad184[0x6];
    u8 kaze_post_event_grid_polyft4_slot4_uv3_u; /* +0x0498c4 */
    u8 kaze_post_event_grid_polyft4_slot4_uv3_v; /* +0x0498c5 */
    u8 _pad185[0xe];
    u8 kaze_post_event_grid_polyft4_slot5_uv0_u; /* +0x0498d4 */
    u8 kaze_post_event_grid_polyft4_slot5_uv0_v; /* +0x0498d5 */
    u8 _pad186[0x6];
    u8 kaze_post_event_grid_polyft4_slot5_uv1_u; /* +0x0498dc */
    u8 kaze_post_event_grid_polyft4_slot5_uv1_v; /* +0x0498dd */
    u8 _pad187[0x6];
    u8 kaze_post_event_grid_polyft4_slot5_uv2_u; /* +0x0498e4 */
    u8 kaze_post_event_grid_polyft4_slot5_uv2_v; /* +0x0498e5 */
    u8 _pad188[0x6];
    u8 kaze_post_event_grid_polyft4_slot5_uv3_u; /* +0x0498ec */
    u8 kaze_post_event_grid_polyft4_slot5_uv3_v; /* +0x0498ed */
    u8 _pad189[0xe];
    u8 kaze_post_event_grid_polyft4_slot6_uv0_u; /* +0x0498fc */
    u8 kaze_post_event_grid_polyft4_slot6_uv0_v; /* +0x0498fd */
    u8 _pad190[0x6];
    u8 kaze_post_event_grid_polyft4_slot6_uv1_u; /* +0x049904 */
    u8 kaze_post_event_grid_polyft4_slot6_uv1_v; /* +0x049905 */
    u8 _pad191[0x6];
    u8 kaze_post_event_grid_polyft4_slot6_uv2_u; /* +0x04990c */
    u8 kaze_post_event_grid_polyft4_slot6_uv2_v; /* +0x04990d */
    u8 _pad192[0x6];
    u8 kaze_post_event_grid_polyft4_slot6_uv3_u; /* +0x049914 */
    u8 kaze_post_event_grid_polyft4_slot6_uv3_v; /* +0x049915 */
    u8 _pad193[0xe];
    u8 kaze_post_event_grid_polyft4_slot7_uv0_u; /* +0x049924 */
    u8 kaze_post_event_grid_polyft4_slot7_uv0_v; /* +0x049925 */
    u8 _pad194[0x6];
    u8 kaze_post_event_grid_polyft4_slot7_uv1_u; /* +0x04992c */
    u8 kaze_post_event_grid_polyft4_slot7_uv1_v; /* +0x04992d */
    u8 _pad195[0x6];
    u8 kaze_post_event_grid_polyft4_slot7_uv2_u; /* +0x049934 */
    u8 kaze_post_event_grid_polyft4_slot7_uv2_v; /* +0x049935 */
    u8 _pad196[0x6];
    u8 kaze_post_event_grid_polyft4_slot7_uv3_u; /* +0x04993c */
    u8 kaze_post_event_grid_polyft4_slot7_uv3_v; /* +0x04993d */
    u8 _pad197[0x142];
    u32 kaze_minimap_panel_polyf4; /* +0x049a80 */
    u8 kaze_post_event_grid_panel_polyf4_slot0_color_r; /* +0x049a84 */
    u8 kaze_post_event_grid_panel_polyf4_slot0_color_g; /* +0x049a85 */
    u8 kaze_post_event_grid_panel_polyf4_slot0_color_b; /* +0x049a86 */
    u8 _pad198[0x29];
    u32 kaze_post_event_grid_linef2_slot0_tag; /* +0x049ab0 */
    u8 kaze_post_event_grid_linef2_slot0_color_r; /* +0x049ab4 */
    u8 kaze_post_event_grid_linef2_slot0_color_g; /* +0x049ab5 */
    u8 kaze_post_event_grid_linef2_slot0_color_b; /* +0x049ab6 */
    u8 _pad199[0x3];
    u16 kaze_post_event_grid_linef2_slot0_y0; /* +0x049aba */
    u8 _pad200[0x2];
    u16 kaze_post_event_grid_linef2_slot0_y1; /* +0x049abe */
    u32 kaze_minimap_panel_linef2_2; /* +0x049ac0 */
    u8 _pad201[0x2c];
    u32 kaze_post_event_grid_linef3_slot0_tag; /* +0x049af0 */
    u8 kaze_post_event_grid_linef3_slot0_color_r; /* +0x049af4 */
    u8 kaze_post_event_grid_linef3_slot0_color_g; /* +0x049af5 */
    u8 kaze_post_event_grid_linef3_slot0_color_b; /* +0x049af6 */
    u8 _pad202[0x11];
    u32 kaze_minimap_frame_linef3_1; /* +0x049b08 */
    u8 kaze_post_event_grid_linef3_slot1_color_r; /* +0x049b0c */
    u8 kaze_post_event_grid_linef3_slot1_color_g; /* +0x049b0d */
    u8 kaze_post_event_grid_linef3_slot1_color_b; /* +0x049b0e */
    u8 _pad203[0x11];
    u32 kaze_minimap_frame_linef3_2; /* +0x049b20 */
    u8 kaze_post_event_grid_linef3_slot2_color_r; /* +0x049b24 */
    u8 kaze_post_event_grid_linef3_slot2_color_g; /* +0x049b25 */
    u8 kaze_post_event_grid_linef3_slot2_color_b; /* +0x049b26 */
    u8 _pad204[0x11];
    u32 kaze_minimap_frame_linef3_3; /* +0x049b38 */
    u8 kaze_post_event_grid_linef3_slot3_color_r; /* +0x049b3c */
    u8 kaze_post_event_grid_linef3_slot3_color_g; /* +0x049b3d */
    u8 kaze_post_event_grid_linef3_slot3_color_b; /* +0x049b3e */
    u8 _pad205[0x11];
    u32 kaze_minimap_frame_linef3_4; /* +0x049b50 */
    u8 kaze_post_event_grid_linef3_slot4_color_r; /* +0x049b54 */
    u8 kaze_post_event_grid_linef3_slot4_color_g; /* +0x049b55 */
    u8 kaze_post_event_grid_linef3_slot4_color_b; /* +0x049b56 */
    u8 _pad206[0x11];
    u32 kaze_minimap_frame_linef3_5; /* +0x049b68 */
    u8 kaze_post_event_grid_linef3_slot5_color_r; /* +0x049b6c */
    u8 kaze_post_event_grid_linef3_slot5_color_g; /* +0x049b6d */
    u8 kaze_post_event_grid_linef3_slot5_color_b; /* +0x049b6e */
    u8 _pad207[0xa1];
    u32 kaze_minimap_marker_sprt8_base; /* +0x049c10 */
    u8 _pad208[0x4];
    s16 kaze_minimap_marker_sprt_x; /* +0x049c18 */
    s16 kaze_minimap_marker_sprt_y; /* +0x049c1a */
    u8 _pad209[0x2];
    u16 kaze_post_event_large_sprt8_slot0_clut; /* +0x049c1e */
    u8 _pad210[0x130];
    u32 kaze_minimap_tpage_1; /* +0x049d50 */
    u8 _pad211[0x4];
    u32 kaze_minimap_tpage_2; /* +0x049d58 */
    u8 _pad212[0x14];
    u32 kaze_minimap_drawarea_1; /* +0x049d70 */
    u8 _pad213[0x8];
    u32 kaze_minimap_drawarea_2; /* +0x049d7c */
    u8 _pad214[0x20];
    u32 kaze_minimap_label_sprt_base; /* +0x049da0 */
    u8 _pad215[0xa];
    u16 kaze_post_event_large_sprt_slot0_clut; /* +0x049dae */
    u8 _pad216[0xe0];
    u32 kaze_minimap_tpage_end; /* +0x049e90 */
    u8 _pad217[0xc];
    u32 kaze_post_event_banner_sprt_slot0_base_tag; /* +0x049ea0 */
    u8 kaze_post_event_banner_sprt_slot0_base_color_r; /* +0x049ea4 */
    u8 kaze_post_event_banner_sprt_slot0_base_color_g; /* +0x049ea5 */
    u8 kaze_post_event_banner_sprt_slot0_base_color_b; /* +0x049ea6 */
    u8 _pad218[0x1];
    s16 kaze_score_banner_icon_sprt_x; /* +0x049ea8 */
    u8 _pad219[0x4];
    u16 kaze_post_event_banner_sprt_slot0_clut; /* +0x049eae */
    u8 _pad220[0x18];
    u32 kaze_post_event_banner_sprt_slot1_base_tag; /* +0x049ec8 */
    u8 kaze_post_event_banner_sprt_slot1_base_color_r; /* +0x049ecc */
    u8 kaze_post_event_banner_sprt_slot1_base_color_g; /* +0x049ecd */
    u8 kaze_post_event_banner_sprt_slot1_base_color_b; /* +0x049ece */
    u8 _pad221[0x1];
    s16 kaze_banner2_sprt_x; /* +0x049ed0 */
    u8 _pad222[0x4];
    u16 kaze_post_event_banner_sprt_slot1_clut; /* +0x049ed6 */
    u8 _pad223[0x18];
    u32 kaze_post_event_banner_sprt_slot2_base_tag; /* +0x049ef0 */
    u8 kaze_post_event_banner_sprt_slot2_base_color_r; /* +0x049ef4 */
    u8 kaze_post_event_banner_sprt_slot2_base_color_g; /* +0x049ef5 */
    u8 kaze_post_event_banner_sprt_slot2_base_color_b; /* +0x049ef6 */
    u8 _pad224[0x7];
    u16 kaze_post_event_banner_sprt_slot2_clut; /* +0x049efe */
    u8 _pad225[0x18];
    u32 kaze_post_event_score_sprt_slot0_tag; /* +0x049f18 */
    u8 kaze_post_event_score_sprt_slot0_color_r; /* +0x049f1c */
    u8 kaze_post_event_score_sprt_slot0_color_g; /* +0x049f1d */
    u8 kaze_post_event_score_sprt_slot0_color_b; /* +0x049f1e */
    u8 _pad226[0x7];
    u16 kaze_post_event_score_sprt_slot0_clut; /* +0x049f26 */
    u8 _pad227[0x4];
    u32 kaze_score_label_sprt1; /* +0x049f2c */
    u8 _pad228[0x10];
    u32 kaze_score_label_sprt2; /* +0x049f40 */
    u8 _pad229[0x4c];
    u32 kaze_post_event_score_sprt_group1_slot0_tag; /* +0x049f90 */
    u8 _pad230[0x8];
    u8 kaze_score_digit0_sprt_u0; /* +0x049f9c */
    u8 kaze_score_digit0_sprt_v0; /* +0x049f9d */
    u16 kaze_score_digit0_sprt_clut; /* +0x049f9e */
    u16 kaze_post_event_score_sprt_group1_slot0_width; /* +0x049fa0 */
    u8 _pad231[0x2];
    u32 kaze_score_digit1_sprt; /* +0x049fa4 */
    u8 _pad232[0x9];
    u8 kaze_score_digit1_sprt_v0; /* +0x049fb1 */
    u16 kaze_score_digit1_sprt_clut; /* +0x049fb2 */
    u8 _pad233[0x4];
    u32 kaze_score_digit2_sprt; /* +0x049fb8 */
    u8 _pad234[0x4];
    u32 kaze_post_event_score_sprt_group1_copy_source_boundary; /* +0x049fc0 */
    u8 _pad235[0x1];
    u8 kaze_score_digit2_sprt_v0; /* +0x049fc5 */
    u16 kaze_score_digit2_sprt_clut; /* +0x049fc6 */
    u8 _pad236[0x4];
    u32 kaze_post_event_score_sprt_group1_copy_slot0_tag; /* +0x049fcc */
    u8 _pad237[0x38];
    u32 kaze_post_event_large_panel_sprt_slot0_tag; /* +0x04a008 */
    u8 kaze_post_event_large_panel_sprt_slot0_color_r; /* +0x04a00c */
    u8 kaze_post_event_large_panel_sprt_slot0_color_g; /* +0x04a00d */
    u8 kaze_post_event_large_panel_sprt_slot0_color_b; /* +0x04a00e */
    u8 _pad238[0x7];
    u16 kaze_post_event_large_panel_sprt_slot0_clut; /* +0x04a016 */
    u8 _pad239[0x60];
    u32 kaze_post_event_large_panel_sprt_copy_source_boundary; /* +0x04a078 */
    u8 _pad240[0x4];
    u32 kaze_post_event_large_panel_sprt_copy_slot0_tag; /* +0x04a080 */
    u8 _pad241[0x74];
    u32 kaze_post_event_banner_draw_tpage0_tag; /* +0x04a0f8 */
    u8 _pad242[0xc];
    u32 kaze_post_event_banner_draw_tpage1_tag; /* +0x04a108 */
    u8 _pad243[0xc];
    u32 kaze_post_event_banner_draw_tpage2_tag; /* +0x04a118 */
    u8 _pad244[0xc];
    u32 kaze_post_event_banner_polyf4_slot0_tag; /* +0x04a128 */
    u8 _pad245[0x2c];
    u32 kaze_post_event_banner_sprt_slot0_tag; /* +0x04a158 */
    u8 _pad246[0xa];
    u16 kaze_post_event_banner_sprt_overlay_clut; /* +0x04a166 */
    u8 _pad247[0x18];
    u32 kaze_post_event_banner_overlay_draw_tpage_tag; /* +0x04a180 */
    u8 _pad248[0xc];
    s32 kaze_post_event_sprite_anim_counter; /* +0x04a190 */
    u32 kaze_post_event_sprite_polyft4_slot0_tag; /* +0x04a194 */
    u8 _pad249[0x8];
    u8 kaze_post_event_sprite_slot0_uv0_u; /* +0x04a1a0 */
    u8 kaze_post_event_sprite_slot0_uv0_v; /* +0x04a1a1 */
    u16 kaze_post_event_sprite_slot0_clut; /* +0x04a1a2 */
    u8 _pad250[0x4];
    u8 kaze_post_event_sprite_slot0_uv1_u; /* +0x04a1a8 */
    u8 kaze_post_event_sprite_slot0_uv1_v; /* +0x04a1a9 */
    u16 kaze_post_event_sprite_slot0_tpage; /* +0x04a1aa */
    u8 _pad251[0x4];
    u8 kaze_post_event_sprite_slot0_uv2_u; /* +0x04a1b0 */
    u8 kaze_post_event_sprite_slot0_uv2_v; /* +0x04a1b1 */
    u8 _pad252[0x6];
    u8 kaze_post_event_sprite_slot0_uv3_u; /* +0x04a1b8 */
    u8 kaze_post_event_sprite_slot0_uv3_v; /* +0x04a1b9 */
    u8 _pad253[0xca];
    u32 kaze_post_event_overlay_polyft4_slot0_tag; /* +0x04a284 */
    u8 kaze_post_event_overlay_color_r; /* +0x04a288 */
    u8 kaze_post_event_overlay_color_g; /* +0x04a289 */
    u8 kaze_post_event_overlay_color_b; /* +0x04a28a */
    u8 _pad254[0x5];
    u8 kaze_post_event_overlay_uv0_u; /* +0x04a290 */
    u8 kaze_post_event_overlay_uv0_v; /* +0x04a291 */
    u16 kaze_post_event_overlay_clut; /* +0x04a292 */
    u8 _pad255[0x4];
    u8 kaze_post_event_overlay_uv1_u; /* +0x04a298 */
    u8 kaze_post_event_overlay_uv1_v; /* +0x04a299 */
    u16 kaze_post_event_overlay_tpage; /* +0x04a29a */
    u8 _pad256[0x4];
    u8 kaze_post_event_overlay_uv2_u; /* +0x04a2a0 */
    u8 kaze_post_event_overlay_uv2_v; /* +0x04a2a1 */
    u8 _pad257[0x2];
    u32 kaze_post_event_overlay_xy3_word; /* +0x04a2a4 */
    u8 kaze_post_event_overlay_uv3_u; /* +0x04a2a8 */
    u8 kaze_post_event_overlay_uv3_v; /* +0x04a2a9 */
    u8 _pad258[0x2];
    u32 kaze_post_event_overlay_copy_slot0_tag; /* +0x04a2ac */
    u8 _pad259[0x2];
    u8 kaze_post_event_overlay2_color_b; /* +0x04a2b2 */
    u8 _pad260[0xe];
    u8 kaze_post_event_overlay2_uv1_v; /* +0x04a2c1 */
    u8 _pad261[0x6];
    u8 kaze_post_event_overlay2_uv2_u; /* +0x04a2c8 */
    u8 _pad262[0x7];
    u8 kaze_post_event_overlay2_uv3_u; /* +0x04a2d0 */
    u8 kaze_post_event_overlay2_uv3_v; /* +0x04a2d1 */
    u8 _pad263[0x2];
    u32 kaze_post_event_tile1_slot0_tag; /* +0x04a2d4 */
    u8 _pad264[0x257c];
    u32 kaze_weather_particle_linef2_slot0_tag; /* +0x04c854 */
    u8 kaze_weather_particle_linef2_slot0_color_r; /* +0x04c858 */
    u8 kaze_weather_particle_linef2_slot0_color_g; /* +0x04c859 */
    u8 kaze_weather_particle_linef2_slot0_color_b; /* +0x04c85a */
    u8 _pad265[0xc79];
    u32 kaze_weather_particle_linef2_copy_slot0_tag; /* +0x04d4d4 */
    u8 _pad266[0xc7c];
    u32 kaze_weather_particle_tile_slot0_tag; /* +0x04e154 */
    u8 kaze_weather_particle_tile_slot0_color_r; /* +0x04e158 */
    u8 kaze_weather_particle_tile_slot0_color_g; /* +0x04e159 */
    u8 kaze_weather_particle_tile_slot0_color_b; /* +0x04e15a */
    u8 _pad267[0xc79];
    u32 kaze_weather_particle_tile_copy_slot0_tag; /* +0x04edd4 */
    u8 _pad268[0xc7c];
    s16 kaze_post_event_tile_angle_slot0; /* +0x04fa54 */
    u8 _pad269[0x31e];
    s16 kaze_post_event_tile_distance_slot0; /* +0x04fd74 */
    u8 _pad270[0x31e];
    s16 kaze_weather_particle_slot0_x; /* +0x050094 */
    s16 kaze_weather_particle_slot0_y; /* +0x050096 */
    s16 kaze_weather_particle_slot0_z; /* +0x050098 */
    u8 _pad271[0x83a];
    s16 kaze_billboard_render_rot_x; /* +0x0508d4 */
    s16 kaze_billboard_render_rot_y; /* +0x0508d6 */
    s16 kaze_billboard_render_rot_z; /* +0x0508d8 */
    u8 _pad272[0x102];
    u32 kaze_map_tile_gpu_code; /* +0x0509dc */
    s16 kaze_route_render_vector_x; /* +0x0509e0 */
    s16 kaze_route_render_vector_y; /* +0x0509e2 */
    s16 kaze_route_render_vector_z; /* +0x0509e4 */
    u8 _pad273[0x2];
    u32 kaze_balmodel_dat_header; /* +0x0509e8 */
    u32 kaze_balmodel_entry_offsets; /* +0x0509ec */
    u8 _pad274[0x7ff8];
    u32 kaze_model_asset_slot0_ptr; /* +0x0589e8 */
    u32 kaze_model_asset_slot1_ptr; /* +0x0589ec */
    u32 kaze_model_asset_ptr2; /* +0x0589f0 */
    u32 kaze_model_asset_slot3_ptr; /* +0x0589f4 */
    u32 kaze_model_asset_slot4_ptr; /* +0x0589f8 */
    u32 kaze_model_asset_slot5_ptr; /* +0x0589fc */
    u32 kaze_model_asset_slot6_ptr; /* +0x058a00 */
    u32 kaze_model_asset_slot7_ptr; /* +0x058a04 */
    u32 kaze_model_asset_ptr8; /* +0x058a08 */
    u32 kaze_model_asset_ptr9; /* +0x058a0c */
    u32 kaze_model_asset_ptr10; /* +0x058a10 */
    u32 kaze_balmodel_part0_ptr; /* +0x058a14 */
    u32 kaze_balmodel_part1_ptr; /* +0x058a18 */
    u32 kaze_balmodel_part2_ptr; /* +0x058a1c */
    u32 kaze_balmodel_part3_ptr; /* +0x058a20 */
    u32 kaze_balmodel_submodel_ptr_m1_0; /* +0x058a24 */
    u8 _pad275[0x3c];
    u32 kaze_balmodel_submodel_ptr_m5_0; /* +0x058a64 */
    u8 _pad276[0xc];
    s32 kaze_mode0_marker_slot0_active; /* +0x058a74 */
    u32 kaze_marker_slot0_onscreen; /* +0x058a78 */
    s32 kaze_mode0_marker_slot0_x; /* +0x058a7c */
    s32 kaze_mode0_marker_slot0_y; /* +0x058a80 */
    s32 kaze_mode0_marker_slot0_z; /* +0x058a84 */
    u8 _pad277[0x48];
    s32 kaze_mode0_marker_slot1_active; /* +0x058ad0 */
    u8 _pad278[0x4];
    s32 kaze_mode0_marker_slot1_x; /* +0x058ad8 */
    s32 kaze_mode0_marker_slot1_y; /* +0x058adc */
    s32 kaze_mode0_marker_slot1_z; /* +0x058ae0 */
    u8 _pad279[0x48];
    s32 kaze_mode0_marker_slot2_active; /* +0x058b2c */
    u8 _pad280[0x4];
    s32 kaze_mode0_marker_slot2_x; /* +0x058b34 */
    s32 kaze_mode0_marker_slot2_y; /* +0x058b38 */
    s32 kaze_mode0_marker_slot2_z; /* +0x058b3c */
    u8 _pad281[0x48];
    s32 kaze_mode0_marker_slot3_active; /* +0x058b88 */
    u8 _pad282[0x4];
    s32 kaze_mode0_marker_slot3_x; /* +0x058b90 */
    s32 kaze_mode0_marker_slot3_y; /* +0x058b94 */
    s32 kaze_mode0_marker_slot3_z; /* +0x058b98 */
    u8 _pad283[0x48];
    s32 kaze_mode0_marker_slot4_active; /* +0x058be4 */
    u8 _pad284[0x4];
    s32 kaze_mode0_marker_slot4_x; /* +0x058bec */
    s32 kaze_mode0_marker_slot4_y; /* +0x058bf0 */
    s32 kaze_mode0_marker_slot4_z; /* +0x058bf4 */
    u8 _pad285[0x48];
    s32 kaze_mode0_marker_slot5_active; /* +0x058c40 */
    u8 _pad286[0x4];
    s32 kaze_mode0_marker_slot5_x; /* +0x058c48 */
    s32 kaze_mode0_marker_slot5_y; /* +0x058c4c */
    s32 kaze_mode0_marker_slot5_z; /* +0x058c50 */
    u8 _pad287[0x48];
    s32 kaze_mode0_marker_slot6_active; /* +0x058c9c */
    u8 _pad288[0x4];
    s32 kaze_mode0_marker_slot6_x; /* +0x058ca4 */
    s32 kaze_mode0_marker_slot6_y; /* +0x058ca8 */
    s32 kaze_mode0_marker_slot6_z; /* +0x058cac */
    u8 _pad289[0x48];
    s32 kaze_mode0_marker_slot7_active; /* +0x058cf8 */
    u8 _pad290[0x4];
    s32 kaze_mode0_marker_slot7_x; /* +0x058d00 */
    s32 kaze_mode0_marker_slot7_y; /* +0x058d04 */
    s32 kaze_mode0_marker_slot7_z; /* +0x058d08 */
    u8 _pad291[0x48];
    s32 kaze_mode0_marker_slot8_active; /* +0x058d54 */
    u8 _pad292[0x4];
    s32 kaze_mode0_marker_slot8_x; /* +0x058d5c */
    s32 kaze_mode0_marker_slot8_y; /* +0x058d60 */
    s32 kaze_mode0_marker_slot8_z; /* +0x058d64 */
    u8 _pad293[0x48];
    s32 kaze_mode0_marker_slot9_active; /* +0x058db0 */
    u8 _pad294[0x4];
    s32 kaze_mode0_marker_slot9_x; /* +0x058db8 */
    s32 kaze_mode0_marker_slot9_y; /* +0x058dbc */
    s32 kaze_mode0_marker_slot9_z; /* +0x058dc0 */
    u8 _pad295[0x48];
    s32 kaze_mode_object_slot0_active; /* +0x058e0c */
    s32 kaze_mode_object_slot0_state; /* +0x058e10 */
    s32 kaze_mode_object_slot0_x; /* +0x058e14 */
    s32 kaze_mode_object_slot0_y; /* +0x058e18 */
    s32 kaze_mode_object_slot0_z; /* +0x058e1c */
    u8 _pad296[0x28];
    u32 kaze_mode_object_slot0_collision_angle; /* +0x058e48 */
    s32 kaze_mode_object_slot0_collision_strength; /* +0x058e4c */
    s32 kaze_mode_object_slot0_move_timer; /* +0x058e50 */
    u8 _pad297[0x4];
    u32 kaze_mode_object_slot0_flags; /* +0x058e58 */
    s32 kaze_mode_object_slot0_timer; /* +0x058e5c */
    s32 kaze_mode_object_slot0_trigger; /* +0x058e60 */
    s32 kaze_mode_object_slot1_active; /* +0x058e64 */
    s32 kaze_mode_object_slot1_state; /* +0x058e68 */
    s32 kaze_mode_object_slot1_x; /* +0x058e6c */
    s32 kaze_mode_object_slot1_y; /* +0x058e70 */
    s32 kaze_mode_object_slot1_z; /* +0x058e74 */
    u8 _pad298[0x28];
    u32 kaze_mode_object_slot1_collision_angle; /* +0x058ea0 */
    s32 kaze_mode_object_slot1_collision_strength; /* +0x058ea4 */
    s32 kaze_mode_object_slot1_move_timer; /* +0x058ea8 */
    u8 _pad299[0x4];
    u32 kaze_mode_object_slot1_flags; /* +0x058eb0 */
    s32 kaze_mode_object_slot1_timer; /* +0x058eb4 */
    s32 kaze_mode_object_slot1_trigger; /* +0x058eb8 */
    s32 kaze_mode_object_slot2_active; /* +0x058ebc */
    s32 kaze_mode_object_slot2_state; /* +0x058ec0 */
    s32 kaze_mode_object_slot2_x; /* +0x058ec4 */
    s32 kaze_mode_object_slot2_y; /* +0x058ec8 */
    s32 kaze_mode_object_slot2_z; /* +0x058ecc */
    u8 _pad300[0x28];
    u32 kaze_mode_object_slot2_collision_angle; /* +0x058ef8 */
    s32 kaze_mode_object_slot2_collision_strength; /* +0x058efc */
    s32 kaze_mode_object_slot2_move_timer; /* +0x058f00 */
    u8 _pad301[0x4];
    u32 kaze_mode_object_slot2_flags; /* +0x058f08 */
    s32 kaze_mode_object_slot2_timer; /* +0x058f0c */
    s32 kaze_mode_object_slot2_trigger; /* +0x058f10 */
    s32 kaze_mode_object_slot3_active; /* +0x058f14 */
    s32 kaze_mode_object_slot3_state; /* +0x058f18 */
    s32 kaze_mode_object_slot3_x; /* +0x058f1c */
    s32 kaze_mode_object_slot3_y; /* +0x058f20 */
    s32 kaze_mode_object_slot3_z; /* +0x058f24 */
    u8 _pad302[0x28];
    u32 kaze_mode_object_slot3_collision_angle; /* +0x058f50 */
    s32 kaze_mode_object_slot3_collision_strength; /* +0x058f54 */
    s32 kaze_mode_object_slot3_move_timer; /* +0x058f58 */
    u8 _pad303[0x4];
    u32 kaze_mode_object_slot3_flags; /* +0x058f60 */
    s32 kaze_mode_object_slot3_timer; /* +0x058f64 */
    s32 kaze_mode_object_slot3_trigger; /* +0x058f68 */
    s32 kaze_mode_object_slot4_active; /* +0x058f6c */
    s32 kaze_mode_object_slot4_state; /* +0x058f70 */
    s32 kaze_mode_object_slot4_x; /* +0x058f74 */
    s32 kaze_mode_object_slot4_y; /* +0x058f78 */
    s32 kaze_mode_object_slot4_z; /* +0x058f7c */
    u8 _pad304[0x28];
    u32 kaze_mode_object_slot4_collision_angle; /* +0x058fa8 */
    s32 kaze_mode_object_slot4_collision_strength; /* +0x058fac */
    s32 kaze_mode_object_slot4_move_timer; /* +0x058fb0 */
    u8 _pad305[0x4];
    u32 kaze_mode_object_slot4_flags; /* +0x058fb8 */
    s32 kaze_mode_object_slot4_timer; /* +0x058fbc */
    s32 kaze_mode_object_slot4_trigger; /* +0x058fc0 */
    s32 kaze_mode_object_slot5_active; /* +0x058fc4 */
    s32 kaze_mode_object_slot5_state; /* +0x058fc8 */
    s32 kaze_mode_object_slot5_x; /* +0x058fcc */
    s32 kaze_mode_object_slot5_y; /* +0x058fd0 */
    s32 kaze_mode_object_slot5_z; /* +0x058fd4 */
    u8 _pad306[0x28];
    u32 kaze_mode_object_slot5_collision_angle; /* +0x059000 */
    s32 kaze_mode_object_slot5_collision_strength; /* +0x059004 */
    s32 kaze_mode_object_slot5_move_timer; /* +0x059008 */
    u8 _pad307[0x4];
    u32 kaze_mode_object_slot5_flags; /* +0x059010 */
    s32 kaze_mode_object_slot5_timer; /* +0x059014 */
    s32 kaze_mode_object_slot5_trigger; /* +0x059018 */
    s32 kaze_dynamic_object_slot0_type; /* +0x05901c */
    u8 _pad308[0x4];
    u32 kaze_dynobj_slot0_render_code; /* +0x059024 */
    u32 kaze_dynobj_slot0_in_range_flag; /* +0x059028 */
    u8 _pad309[0x5c];
    u32 kaze_dynobj_slot0_land_param; /* +0x059088 */
    u8 _pad310[0x800];
    s32 kaze_transient_effect_slot0_active; /* +0x05988c */
    s32 kaze_transient_effect_slot0_visible; /* +0x059890 */
    s32 kaze_transient_effect_slot0_world_x; /* +0x059894 */
    s32 kaze_transient_effect_slot0_world_y; /* +0x059898 */
    s32 kaze_transient_effect_slot0_world_z; /* +0x05989c */
    s32 kaze_transient_effect_slot0_payload_w; /* +0x0598a0 */
    s16 kaze_transient_effect_slot0_relative_x; /* +0x0598a4 */
    s16 kaze_transient_effect_slot0_relative_y; /* +0x0598a6 */
    s16 kaze_transient_effect_slot0_relative_z; /* +0x0598a8 */
    u8 _pad311[0x2];
    s16 kaze_transient_effect_slot0_rot_x; /* +0x0598ac */
    s16 kaze_transient_effect_slot0_rot_y; /* +0x0598ae */
    s16 kaze_transient_effect_slot0_rot_z; /* +0x0598b0 */
    u8 _pad312[0x2];
    s32 kaze_transient_effect_slot0_age; /* +0x0598b4 */
    u8 _pad313[0x15d4];
    s16 kaze_default_collision_shape_x; /* +0x05ae8c */
    s16 kaze_default_collision_shape_y; /* +0x05ae8e */
    s16 kaze_default_collision_shape_z; /* +0x05ae90 */
    s16 kaze_default_collision_shape_extent_x; /* +0x05ae92 */
    s16 kaze_default_collision_shape_extent_y; /* +0x05ae94 */
    s16 kaze_default_collision_shape_extent_z; /* +0x05ae96 */
    u16 kaze_texture_tbl_tile_uv_slot0; /* +0x05ae98 */
    u8 _pad314[0x7fe];
    u16 kaze_terrain_tile_tpage0; /* +0x05b698 */
    u16 kaze_terrain_tile_tpage1; /* +0x05b69a */
    u16 kaze_terrain_tile_clut_slot0; /* +0x05b69c */
    u8 _pad315[0x3e];
    u16 kaze_model_texture_clut0; /* +0x05b6dc */
    u16 kaze_model_texture_clut1; /* +0x05b6de */
    u32 kaze_terrain_tilemap_packed_slot0; /* +0x05b6e0 */
    u8 _pad316[0xfffc];
    u32 kaze_terrain_model_data_header; /* +0x06b6e0 */
    u32 kaze_mapdata_entry_offsets; /* +0x06b6e4 */
    u8 _pad317[0x76778];
    u32 kaze_terrain_model_remap_table_slot0; /* +0x0e1e60 */
    u8 _pad318[0x107c];
    s16 kaze_zone_collision_slot0_count; /* +0x0e2ee0 */
    s16 kaze_zone_collision_slot0_shape_index; /* +0x0e2ee2 */
    u8 _pad319[0x7fc];
    s16 kaze_zone_collision_shape0_x; /* +0x0e36e0 */
    s16 kaze_zone_collision_shape0_y; /* +0x0e36e2 */
    s16 kaze_zone_collision_shape0_z; /* +0x0e36e4 */
    s16 kaze_zone_collision_shape0_extent_x; /* +0x0e36e6 */
    s16 kaze_zone_collision_shape0_extent_y; /* +0x0e36e8 */
    s16 kaze_zone_collision_shape0_extent_z; /* +0x0e36ea */
    u8 _pad320[0x8ff4];
    u32 kaze_visible_terrain_tile_cache_slot0; /* +0x0ec6e0 */
    u8 _pad321[0x3fc];
    s32 kaze_hud_status_source; /* +0x0ecae0 */
    s32 kaze_hud_status; /* +0x0ecae4 */
    s32 kaze_altitude_hud; /* +0x0ecae8 */
    s32 kaze_wind_hud; /* +0x0ecaec */
    s32 kaze_checkpoint_hud; /* +0x0ecaf0 */
    s32 kaze_hud_refresh_counter; /* +0x0ecaf4 */
    s32 kaze_hud_icon_index; /* +0x0ecaf8 */
    s32 kaze_exit_prompt_latch; /* +0x0ecafc */
    u16 kaze_prim_anim_scroll_table; /* +0x0ecb00 */
    u16 kaze_hud_history_slot1; /* +0x0ecb02 */
    u8 _pad322[0x1a];
    u16 kaze_prim_anim_scroll_table_e15; /* +0x0ecb1e */
    u8 _pad323[0x8];
    u32 kaze_terrain_model_ptr_table_slot0; /* +0x0ecb28 */
    u8 _pad324[0x7fc];
    s16 kaze_stage_flag_fx_rot_x; /* +0x0ed328 */
    u8 _pad325[0x6];
    s16 kaze_stage_flag_effect_vector_x; /* +0x0ed330 */
    s16 kaze_stage_flag_effect_vector_y; /* +0x0ed332 */
    s16 kaze_stage_flag_effect_vector_z; /* +0x0ed334 */
    u8 _pad326[0x2];
    s32 kaze_stage_flag_fx_scale_x; /* +0x0ed338 */
    u8 _pad327[0xc];
    s32 kaze_stage_flag_20000000_effect_angle; /* +0x0ed348 */
    s32 kaze_stage_flag_20000000_effect_alpha; /* +0x0ed34c */
    s32 kaze_stage_flag_20000000_effect_timer; /* +0x0ed350 */
    u8 _pad328[0x4];
    s16 kaze_stage_flag_fx2_rot_x; /* +0x0ed358 */
    u8 _pad329[0x6];
    s16 kaze_stage_flag_fx2_pos_x; /* +0x0ed360 */
    u8 _pad330[0x16];
    s32 kaze_stage_flag_10000000_effect_alpha; /* +0x0ed378 */
    s32 kaze_stage_flag_10000000_effect_timer; /* +0x0ed37c */
    u8 _pad331[0x8];
    s32 kaze_scripted_input_slot0_timer; /* +0x0ed388 */
    u32 kaze_scripted_input_slot0_command; /* +0x0ed38c */
    u8 _pad332[0x30];
    s32 kaze_stage_flag_variant_effect0_alpha; /* +0x0ed3c0 */
    u8 _pad333[0x18];
    s32 kaze_stage_flag_variant_effect1_x; /* +0x0ed3dc */
    u8 _pad334[0x20];
    s32 kaze_stage_flag_variant_effect2_alpha; /* +0x0ed400 */
    u8 _pad335[0x24];
    s32 kaze_stage_flag_post_event_banner_alpha; /* +0x0ed428 */
    s32 kaze_scripted_input_cursor; /* +0x0ed42c */
} KazeLiveRoot; /* sizeof == 0xed430 */

#endif /* KAZE_LIVE_ROOT_H */
