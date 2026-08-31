# Kaze no Notam: Architecture Guide

Reader's guide to the decompiled source of *Kaze no Notam* (Artdink, 1997,
`SLPS_009.12`), a hot-air-balloon flight/navigation game. The matching
decompilation is complete: all 648 game functions are byte-matched
(`tools/progress.py`), the EXE rebuilds byte-identically
(sha1 `f8f44c2ddaf4969f4132fb17d1abab06a75c2def`), and each function lives in
`src/<kaze_name>.c`. The remaining 1,076 functions in the binary are PsyQ
SDK/library code left in `asm/`.

Curated names, categories, and one-line purposes for every game function are in
`docs/function-map.json` (categories: menu 249, render 129, library 123,
map_screen 38, sound 24, core 20, cd_stream 20, objective 17, flight 12,
hud 10, collision 6). Named live-state fields are laid out in
`include/kaze_live_root.h`. This document is the map between the two: what
each subsystem does, its key functions, its key state, and how it connects to
the rest.

## Memory model

Two state domains exist, and almost every function touches one or both:

- **Static BSS window** `0x800a7168..0x800b5700`, zeroed at startup. Holds
  input state, the flight-dynamics cluster (`kaze_wind_layer0_dir..0x800b17xx`), menu
  state machines, staging blocks, and PsyQ library internals.
- **The live root**: `kaze_main_game_loop` allocates a single
  **0xed430-byte** state/asset block per stage via
  `kaze_heap_alloc(1, 0xed430)` and stores the pointer in
  `kaze_state_base_ptr`. Nearly all in-flight gameplay,
  render, objective, and collision state is `base+offset` off this pointer.
  In a typical live session the root resolves to `0x800d0f50`. The block is
  freed on loop exit, so live-root offsets are only meaningful in flight.

Live-root header fields worth memorizing:

| offset | field | meaning |
| --- | --- | --- |
| `+0x00` | `kaze_state_mode` | loop-exit / next-mode code |
| `+0x04` | `kaze_state_phase` | frame-loop running flag |
| `+0x08` | `kaze_state_submode` | exit request (used by demo playback) |
| `+0x0c` | `kaze_state_result_score` | final score from `kaze_stage_completion_check` |
| `+0x10` | `kaze_state_next_route` | follow-up route from `kaze_record_table_insert` |
| `+0x14` | `kaze_state_flags` | gameplay state flags (see Objectives) |
| `+0x18` | `kaze_terrain_height_layer` | map tile height layer, seeds altitude |
| `+0x24` | `kaze_checkpoint_count` | objective count |
| `+0x68/+0x6c/+0x70` | `kaze_object_pos_x/y/z` | player position mirror |
| `+0x98` | `kaze_render_page_index` | double-buffer page (flipped per frame) |
| `+0x9c` | `kaze_render_primitive_cursor` | per-frame primitive cursor |
| `+0xa4` | `kaze_state_step_delta` | per-frame time step used by physics |

**Menu staging vs live state:** menu code writes a staging block
`kaze_staged_stage_index..kaze_staged_stage_variant` (`kaze_staged_stage_mode`,
staged start `kaze_staged_start_wind_dir..kaze_staged_start_z`);
`kaze_flight_state_init` / `kaze_stage_staging_copy` copy it into the live
block `kaze_stage_index..kaze_stage_variant` (`kaze_stage_mode`,
`kaze_stage_flags`) when a flight starts.

## Core: boot and main loop

Boot path: entry `start` zeroes BSS, then `kaze_system_boot_init`,
`kaze_input_light_defaults_init`, and `kaze_game_state_defaults_init` set up
pads and defaults.

`kaze_main_game_loop` (core) is the flight-session loop. It
allocates the 0xed430-byte live root, resets input latches, and runs the
double-buffered frame loop: flip `+0x98`, advance `+0x9c`, store a
primitive-load metric at `+0xa0`, recompute the VSync divisor at `+0xa4`.
Per frame it dispatches flight control (`kaze_flight_control_dispatch`),
camera (`kaze_camera_control_update`), the world update pass
(`kaze_world_update_pass`), and the render passes. On exit it frees the root
and returns `+0x0` as the next-mode code. Note for offset reading: the
decompiler types the root as `int *` in this function, so `base + 0x1048`
style expressions are *word* indices (byte offset = index * 4).

Terminal handlers: `kaze_success_terminal_handler` sets
`kaze_state_flags |= 0x80400000`; `kaze_failure_terminal_handler`
sets `0x80800000`. The meaningful terminal bits are
**`0x00400000` success** and **`0x00800000` failure**; completion guards test
`flags & 0x00c00000`. Both handlers set the one-shot
`kaze_post_event_sound_latch` (`base+0x48200`) before playing the result
jingle. `kaze_record_table_insert` files the score and
yields `kaze_state_next_route`.

Input: `kaze_input_poll` with button bits Triangle `0x10`,
Circle `0x20`, Cross `0x40`, Square `0x80`, Start `0x800`, Up `0x1000`,
Right `0x2000`, Down `0x4000`, Left `0x8000`. Edge detection in
`kaze_input_edge_detect`; the shared edge/current bitfield is
`kaze_input_edge`.

## Flight model

The flight-dynamics cluster is static BSS at `kaze_wind_layer0_dir..kaze_burner_state`:
position `kaze_pos_x/y/z` (`0xb16d8/dc/e0`), per-frame deltas
`kaze_delta_x/y/z` (`0xb16e8/ec/f0`), wind layers, heading, gas. Position is
mirrored into live root `+0x68/+0x6c/+0x70` each frame.

- `kaze_flight_state_init` initializes the flight state from
  the staging block: wind layer table, mode-0 marker constants (variant 1
  hardcodes markers at `(81700,0,142500)` and `(427500,0,423700)`), and
  effect/latch state.
- `kaze_wind_motion_update` is the wind-field integrator.
  Five altitude-layer wind entries at `kaze_wind_layer0_dir` (stride 8:
  `{angle, strength}`); the active layer index is
  **`layer = -kaze_pos_y / 0x76c - 3`, clamped 0..4**: the game's core
  altitude to wind law. On a layer change it seeds a decaying previous-wind
  vector (`0xb1748/4c`) from current motion, then sums previous wind +
  active-layer wind (scaled by step delta) + the extra-impulse vector
  (`kaze_extra_angle/strength`, `0xb1758/5c`), each term fixed-point `>>12`,
  into `delta_x/z`; recomputes `kaze_motion_angle` (`0xb1750`) and
  `kaze_horizontal_speed` (`0xb1754`). `delta_y` = `kaze_vertical_wind`
  (`0xb1768`, random jitter ±0x20) + `kaze_vertical_control` (`0xb1764`).
  Burner timer `kaze_impact_flash_timer` decays `-0x100*step_delta`, setting state flag
  `0x80` below `0x2000`.
- `kaze_object_physics_snapshot` applies the deltas to
  position, snapshots `kaze_prev_frame_pos_*` (`0xb1708..14`), clamps
  (`pos_x/z` in `[0,0x76bff]`, `pos_y` in `[-0x3a02,0]`; object pos `x/z` in
  `[0x7e2c,0x6edd4]`), writes the live-root mirrors and route offsets
  (`+0x88/+0x90`), and raises `flags |= 0x10000` when the object leaves the
  playfield and `0x8000` on large moves. It also runs the mode-1 endgame
  check (see Objectives).
- Gas/burner: Triangle is the climb/burner control. `kaze_gas` (`0xb1778`)
  drains continuously; `kaze_target_gas` (`0xb177c`) is recomputed by
  `kaze_target_gas_law` as
  `kaze_min_gas + kaze_terrain_height_layer * 0x76c0 + 0x28000`;
  `kaze_min_gas_init` seeds `kaze_min_gas` from per-mode
  tables at `kaze_min_gas_base_table..ac`; `kaze_gas_floor_reset` and
  `kaze_burner_input_update`/`kaze_vertical_control_update` drive
  `kaze_gas_floor` (`0xb1784`) and `kaze_burner_state` (`0xb178c`).
  `kaze_flight_timer_gas_update` advances timers
  (`base+0x48280` window, `+0x48284` total, displayed `/0x3c`) and decays
  `kaze_vertical_penalty` (`0xb1770`).
- Landing/contact: `kaze_landing_event_update` probes the
  previous-frame position (radius 0x32, then 300) against terrain and dynamic
  objects; on an object hit it caches the struck slot's spin and landing
  parameter into `kaze_extra_angle`/`kaze_extra_strength` (feeding straight
  back into the wind integrator's extra-impulse term) and calls
  `kaze_landing_goal_handler`, which can set terminal-ish
  state flags (`+0x14 |= 0x9000`).

## Menu system

The largest category (249 functions, `0x8004xxxx..0x80065xxx`). Flow: title →
select-mode → task/setup screens → start-location map → flight → post-event
result → records/ending.

- **Title/opening:** `kaze_title_screen_loop` with state
  helpers (`kaze_title_menu_substate`, `kaze_title_check_savedata`, ...);
  attract demo via `kaze_opening_demo_*` and `kaze_demo_attract_runner_34478`.
  Title state global: `kaze_title_menu_state`;
  start-flow state `kaze_title_start_flow_state`.
- **Setup menu (mode staging):** `kaze_setup_menu_loop`
  dispatches settings rows. Row 1 (`kaze_setup_menu_stage_row_input`)
  → Circle moves to row 2. Row 2
  (`kaze_settings_mode_row_handler`) edits the mode source
  `kaze_menu_mode_source` (Left decrements). Row 3
  (`kaze_setup_menu_option_row_29b4`) edits `kaze_menu_level_source`. Row 10 enters the
  task carousel. Circle walks rows `2 → 3 → 6 → 99` (row 99 = accepted,
  outer state 9). `kaze_settings_menu_dispatch` maps the
  source to the staged mode: `kaze_menu_mode_source == 0 → staged mode 1`,
  `== 1 → mode 0` (default; `kaze_setup_menu_init` sets `kaze_menu_mode_source = 1`),
  `== 2 → mode 2`. So one Left on row 2 is the decomp-backed route to mode 1.
- **Task-letter carousel:** `kaze_task_menu_state_machine`
  with the `kaze_task_carousel_*` family (rotate/select/letter-add/delete/
  layout/zoom). State globals: `kaze_task_menu_state`,
  carousel indices `kaze_task_carousel_visible_index`/`kaze_task_carousel_input_index`/`kaze_task_carousel_selected_index`,
  transition ticks/duration `kaze_task_carousel_transition_ticks`/`kaze_task_carousel_transition_duration`, exit timer `kaze_task_carousel_exit_timer`,
  `kaze_task_transition_counter`. Exit path through
  `kaze_task_carousel_exit_handler`.
- **Menu camera:** the 3D route/menu backdrops use a `GsCOORDINATE2` camera:
  `kaze_menu_camera_rot_x/y`, `kaze_menu_camera_pos_x/z`,
  scroll `kaze_menu_camera_scroll`, coord flags `kaze_menu_camera_coord_flags`.
- **Records / memory card / registration / ending:** records screens
  (`kaze_records_screen_frame` etc., sorted by `kaze_records_combined_sort`);
  memory card flows `kaze_memcard_screen_loop` → format/load/save flows, with
  save payload packing in `kaze_save_image_pack` and record verification in
  `kaze_save_verify_unpack_records` (state globals `kaze_memory_card_load_state`/`kaze_memory_card_save_state`);
  name registration `kaze_registration_*`; staff-roll/ending
  `kaze_ending_screen_loop` and the `kaze_ending_*` family.
- **Post-event (result) menu:** after a terminal, `kaze_post_event_menu_update`
  runs the result/exit UI using
  `kaze_post_event_menu_state` (`base+0x48e0c`) and
  `kaze_post_event_menu_choice` (`base+0x492bc`);
  `kaze_pause_exit_prompt` sets `kaze_exit_prompt_latch`
  (`base+0xecafc`).

## Start-location map screen

Before flight, the player picks a launch point on a 2D map.

- `kaze_start_location_map_loop` runs while
  `kaze_map_state` != 9. Work buffers are
  `kaze_map_screen_packet_area_a/b` (`base+0x4120`/`+0x28198`). Skipped
  entirely when staged variant `kaze_staged_stage_variant != 0`
  (`kaze_start_location_skip_path`).
- Cursor: `kaze_start_map_cursor_move` updates `kaze_map_cursor_x/z`.
  Each frame in state 2,
  `kaze_start_location_validity` recomputes
  `kaze_map_location_valid`: cursor must be in
  `0x14..0xeb` both axes; in mode-1 staging (`kaze_menu_mode_source == 0`) the start
  must be ≥ `0x20` half-cells from the per-stage target (static tables
  `kaze_start_location_target_x_table`/`kaze_start_location_target_z_table` indexed by
  `kaze_menu_stage_source`); and the terrain tile u32 at
  `(x/2)*4 + (z/2)*0x200` into the `base+0x5b6e0` packed tile grid must not
  have flags `0x0c000000`, its zone id (`>>8 & 0x3ff`) must not be in the
  per-stage exclusion lists (`kaze_start_zone_excl_table_mode1`/`kaze_start_zone_excl_table_mode0`/`kaze_start_zone_excl_table_mode2`),
  the `+0xe2ee0` zone-entry short must be 0, and the tile's top nibble
  (terrain type) must be < 3.
- Confirm: `kaze_map_cursor_confirm_input` (Circle, state 2 → 3) →
  `kaze_map_second_confirm_input` (Circle, state 3 → fade state 5) → state 9.
  On exit `kaze_start_location_commit` writes the staged
  start (`kaze_stage_start_x` `kaze_staged_start_x` etc.), later copied to live
  start fields `kaze_start_wind_dir..kaze_start_z`.
- The rest of the 38 map_screen functions are prim init/draw pairs
  (`kaze_map_panel_*`, `kaze_map_cursor_*`, `kaze_map_wind_overlay_*`,
  `kaze_map_tiles_draw`, fade quads) plus the static in-flight map variant
  (`kaze_static_map_screen_*`, shown when Square pauses flight).

## Objectives and completion

Three stage modes, selected by `kaze_stage_mode`, with
per-mode objective storage in the live root:

- **Mode 0** ("triangle"): marker table at `base+0x58a74`, 10 slots, stride
  **0x5c**. Per slot: word0 state (`0`/`1`/`0x80000000`/`0x81000000`),
  words 1..3 x/y/z, `+0x2c` heading accumulator (`&0x1ff`), `+0x4` on-screen
  flag, `+0x14/16/18` screen-relative delta shorts. Maintained by
  `kaze_marker_slot_update`; dropping markers uses
  `kaze_marker_slot_find_free`/`kaze_marker_drop`; proximity-attach to a
  dynamic object via `kaze_marker_attach_to_dynobj`.
- **Modes 1/2** (target / timed): shared object table at `base+0x58e0c`,
  6 slots, stride **0x58** (slot0 active `+0x58e0c`, x/z at
  `+0x58e14`/`+0x58e1c`). `kaze_mode1_slot_populate` fills
  it: mode 1 reads static target tables at `kaze_mode1_target_x_table`/`kaze_mode1_target_y_table`/`kaze_mode1_target_z_table`
  (stage 0 target `(241300, 0, 239400)`, stage 1 `(245100, 0, 247000)`,
  stage 2 `(233700, 0, 216600)`; variant 2 overrides slot 0 from
  `kaze_mode1_target_variant2_x`/`kaze_mode1_target_variant2_y`/`kaze_mode1_target_variant2_z` to `(427500, 0, 349600)`, variant 5 fills three
  slots); mode 2 procedurally places six objects (active pattern
  `3,4,5,3,4,5`) around the wind anchor with terrain-collision retry.
  Slot fields include scale, rotation, altitude layer
  (`(search_y-700)/0x76c`), collision angle/strength, and a move timer
  (`(rand%5+3)*0x3c`). `kaze_mode_object_update` is the
  per-frame slot state machine (`0x81000000` descend → `0x82000000` grow →
  `0x84000000` done).
- **Dynamic objects** (vehicles, scenery, goal receivers): 18-slot table at
  `base+0x5901c`, stride **0x78**. Spawned per stage by
  `kaze_dynamic_object_spawner` from placement tables
  `kaze_dynobj_placement_table_0` (stage 0) / `kaze_dynobj_placement_table_1` (stage 1) / `kaze_dynobj_placement_table_2`
  (stage 2), 0x40-byte records, type 0 terminates, via
  `kaze_dynamic_object_spawn_slot`. Route-following types
  (3/10/6) index the shared node table `kaze_route_node_next_table` (0x10-byte records).
  Slot layout: `+0x00` type (0 = inactive), `+0x10/14/18` world pos,
  `+0x28/2a/2c` rotation (u16, 0..0xfff), `+0x30/34/38` scale
  (0x1000 = 1.0), `+0x42` model/zone id (negative = remap through
  `+0xe1e60`), `+0x44` brightness, `+0x48/4c/50` spin terms, `+0x54..0x68`
  route/movement params, `+0x6c` landing param (copied into
  `kaze_extra_strength` on collision, feeding `kaze_landing_goal_handler`).
  Per-frame behaviors: `kaze_dynobj_update` plus the
  `kaze_dynobj_patrol_shuttle`/`route_follow_*`/`bob_anim`/`fall_respawn_anim`
  family.
- **Scoring:** `kaze_stage_completion_check` (the game's one
  real jumptable function) routes to `kaze_stage_score_compute`: mode 1 = min marker↔object distance (candidate iff
  `|dx|,|dz| < 0x76c` and `SquareRoot0(dx²+dz²) < 0x76d`; all-miss returns
  `0xffffffff`; else `score = (min*100)/0x26`, cap 9999); mode 0 = area of
  the triangle spanned by 3 markers, `score = area/10000`; mode 2 =
  `timer/0x3c` seconds, cap 5999. The mode-1 terminal path lives in
  `kaze_object_physics_snapshot`: with variant 0, mode 1, and no terminal
  bits set, touching ground (`pos_y >= 0`) or drifting > `0x76c0` from the
  target triggers scoring: a non-negative score gives success terminal +
  `kaze_record_table_insert`, a negative score gives failure terminal.
- Goal proximity is surfaced to the HUD as `kaze_goal_proximity_hud`
  (`base+0xecaf0`, reaches 100 inside the target radius).

## HUD

- `kaze_hud_gauge_renderer`: the in-flight panel. Gas gauge
  POLY_F4 bars (gas / target gas / min gas / gas floor; bar
  `Y = 0x124 - value/0xc80`, gas bar `/0x800` with a yellow→red color that
  turns white while `kaze_vertical_penalty != 0`); a wind compass with
  cardinal sprites and a needle from `kaze_view_heading` (`base+0x4a`,
  masked `0xfff`) and `kaze_balloon_angle`; and a **5-slot wind-layer panel**
  (arrow slot *i* shows layer *4−i*; the slot for the current altitude layer
  draws bright, 0x80 vs 0x60), which is the render-side proof of
  `layer = -kaze_pos_y/0x76c - 3`. The altitude marker parks offscreen below
  3 altitude cells.
- `kaze_hud_status_update`: per-frame HUD state, a
  target-proximity flag scanned over `kaze_checkpoint_count` slots within
  `0x76c1`, three gauge bars at `base+0xecae8/ec/f0` (altitude/wind/target),
  and the `kaze_hud_status` code at `base+0xecae4` (state machine values:
  `9` = terminal, `0xb` on gas/flag failure, `-1` in failure/terminal
  branches; a live success trace observed `2 -> 9`). Warning SFX voice
  `0x1e` keys on when `(kaze_stage_flags & 3) == 3`.
- `kaze_minimap_radar_builder`: the minimap overlay,
  page-indexed and double-buffered. World→minimap divisor is **0xed8 = 3800**
  (one map tile). Wind needle direction =
  `((0x1800 - kaze_wind_current_angle) & 0xfff) / 0x200`. Checkpoint dots
  snap to a mode object when within one tile. `kaze_stage_variant == 7`
  emits an extra sprite block. Marker dots via
  `kaze_minimap_marker_renderer`; helpers `kaze_radar_dot_colors_update`,
  `kaze_radar_range_delta`.
- HUD misc: `kaze_altitude_hud` (`base+0xecae8`), refresh counter
  `+0xecaf4`, and `kaze_hud_icon_index` (`+0xecaf8`), which **doubles as the
  terrain remap page index** used by collision (see below).

## Rendering pipeline (GsSort / TMD / GTE)

Frame structure: `kaze_drawenv_init` sets up the paired
DRAWENV/DISPENV at `base+0xa8`/`+0x104` (page 0) and `+0x24120`/`+0x2417c`
(page 1); `kaze_main_game_loop` flips `kaze_render_page_index` (`+0x98`) each
frame and every primitive table in the root is page-indexed off it.
`kaze_loading_screen_show` submits the envs.

- **World:** `kaze_terrain_tile_window_cull` fills the 16x16
  visible-tile cache at `base+0xec6e0`; terrain draws through the tile
  tpage/CLUT lookups (`+0x5b698`, 32-entry CLUT at `+0x5b69c`) and the
  `TEXTURE.TBL` UV table (`+0x5ae98`). The sky pass
  (`kaze_env_sky_render_pass`) and water pass
  (`kaze_env_water_render_pass`) are gated by state-flag bits
  `0x20000000`/`0x10000000` (above/below-horizon visibility computed by
  `kaze_render_vector_load`).
- **Models:** `kaze_draw_model` and
  `kaze_draw_model_flatlit_3dd2c` are the TMD model-draw
  dispatch feeding ~40 primitive rasterizer leaves
  (`0x8003xxxx..0x80048xxx`). Model sources: `BALMODEL.DAT` header/pointer
  table at `base+0x509e8`/`+0x589e8` (submodel pointers `+0x58a14`), terrain
  models from the `MAPDATA.DAT` header `+0x6b6e0` rebased into the
  `+0xecb28` pointer table. Dynamic objects render via `kaze_dynobj_render`,
  mode objects via `kaze_mode_object_render`.
- **Balloon:** `kaze_balloon_render` with rotation inputs
  `kaze_balloon_render_rot0/1`;
  `kaze_balloon_overlay_builder` rebuilds the balloon/wind
  overlay POLY_FT4 at `base+0x488d4` each frame from camera yaw and burner
  state through the GTE helper `kaze_gte_project_vertex`.
- **Environment/lighting:** `kaze_environment_color_update`
  computes light sin/cos (`+0x4826c/+0x48270`) and the base
  color triple (`+0x48288..8c`); `kaze_environment_poly_refresh`
  copies the
  three environment color sets into the POLY_G4/POLY_F4 blocks at
  `+0x48354`/`+0x48504`/`+0x482f4` each frame; `kaze_sun_angle_light_update`
  drives overlay colors; the light matrix lives at
  `+0x481b8`. Weather particles (`LINE_F2`/`TILE` tables at
  `+0x4c854`/`+0x4e154`, 200 slots stride 8 at `+0x50094`) are gated by
  `kaze_stage_flags & 7`; stage-flag effects render in
  `kaze_stage_flag_fx_render` with counters at
  `base+0xed34c`/`+0xed378`.
- **Post-event/UI primitive pools:** a family of init functions builds every
  2D primitive table in the root once per stage:
  `kaze_env_prim_init` (route/HUD sprites),
  `kaze_post_event_sprite_init`,
  `kaze_post_event_menu_prim_init`,
  `kaze_post_event_frame_prim_init`,
  `kaze_post_event_split_prim_init`,
  `kaze_post_event_grid_prim_init`,
  `kaze_post_event_banner_prim_init`, and
  `kaze_asset_prim_master_init` (balloon model lines and
  overlay). Their runtime consumers are the sort/draw functions
  (`kaze_postevent_bg_draw`, `kaze_postevent_prim_dispatch`,
  `kaze_score_banner_renderer`, `kaze_postevent_banner_addprim_32f98`,
  `kaze_screen_composite_draw`, ...). The full offset lists for these tables
  are provenance-only state, recorded in the research notes kept alongside
  the decompilation project rather than in this repository.

## Collision and zones

Zone/shape collision is table-driven from per-stage map data in the live root:

- `base+0xe2ee0`: per-zone-id table (up to 0x200 zones) of
  `{s16 primitive count, s16 shape start index}` pairs.
- `base+0xe36e0`: shape table, **12-byte records of 6 shorts**
  `{local x, local y, local z, extent_x, extent_y, extent_z}`.
  `kaze_collision_shape_test` tests one record: negative
  `extent_x` selects the sphere branch (radius `|extent_x|`, return
  `SquareRoot0(dx²+dy²+dz²) + ext_x`, ≤0 = inside); otherwise a yaw-rotated
  box test (at yaw `0x400`/`0xc00` the X and Z half-extents swap) with
  per-axis penetration computed in GTE scratchpad. The out-mask encodes hit
  direction (`0x77` = fully inside).
- `kaze_terrain_zone_collision_query`: the terrain query.
  Converts tile grid coords to world (`*0xed8 + 0x76c`), resolves remapped
  zone ids through the `+0xe1e60` remap table using the page index at
  `+0xecaf8`, falls back to the default shape at `+0x5ae8c`; returns 1 for a
  shape hit (mask set) or 2 for below-terrain (mask 4).
  `kaze_terrain_collision_neighbor_query` is the
  neighborhood variant.
- `kaze_dynamic_object_collision_query`: same shape tables
  applied to the 18-slot `+0x5901c` dynamic-object table via
  `kaze_object_shape_test`, after a coarse `0x2c89` axis
  distance cull. Negative slot model ids remap through `+0xe1e60` exactly
  like terrain.

Consumers: `kaze_landing_event_update` (flight), `kaze_start_location_validity`
(map screen), `kaze_mode1_slot_populate` (mode-2 placement retry), and
`kaze_mode_object_proximity_test` (objectives).

## CD streaming and asset loading

Low-level layer (`kaze_cd_*`, `0x80012xxx`): sector reads
(`kaze_cd_read_sectors`/`kaze_cd_read_at_loc`), a cached ISO directory tree
(`kaze_cd_file_cache_root_init`, `kaze_cd_dir_tree_build`,
`kaze_cd_file_lookup_cached`, `kaze_file_load_by_name`), whole-file loads
(`kaze_cd_load_file_alloc`/`kaze_cd_load_file_into`), and TIM texture upload
(`kaze_cd_load_tim_upload`). STR movie playback:
`kaze_str_movie_play` → `kaze_str_playback_buffers_init` /
`kaze_str_stream_start` / `kaze_str_frame_vlc_decode` /
`kaze_str_next_frame_fetch`.

Game data files: `\BAL\MAP\MAPDATA.DAT`, `\BAL\MAP\TEXDATA.DAT`,
`\BAL\MAP\RMODE.DAT`, `\BAL\M\BALMODEL.DAT`, `\BAL\M\TEXTURE.TBL`,
`BAL\K\BALL.PRM`, `BALL2.PRM`, `GM.MDL`, `GM.TXP`.

- `kaze_map_cdlfile_lookup` resolves the CdlFILE records for
  MAPDATA/TEXDATA/RMODE into `base+0x48224`/`+0x4823c`/`+0x48254`.
- `kaze_stage_map_cd_loader` is the per-stage loader:
  MAPDATA one `0x91000`-byte block at `stage_index*0x122` sectors; TEXDATA
  twelve `0x20`-sector chunks at
  `stage_index*0xd80 + (stage_level-1)*0x480 + stage_mode*0x180`; RMODE two
  `0x16`-sector chunks at `(stage_variant-1)*0x2c`. It then rebuilds the
  `+0xecb28` terrain/model pointer table from the `+0x6b6e0` MAPDATA header.
- `kaze_common_assets_load` loads the shared
  menu/model/texture assets (BALMODEL, TEXTURE.TBL, ...).

## Sound

`kaze_sound_system_init` brings up SPU/streams. Three lanes:

- **XA/CD audio (music):** `kaze_xa_audio_manager_35a0` with
  start/seek/stop/fade wrappers (`kaze_xa_audio_start_track_3384c`,
  `kaze_xa_audio_seek_resume_339a8`, `kaze_xa_audio_stop_33a3c`,
  `kaze_xa_audio_fade_in/out`, volume via `kaze_cd_audio_set_volume_33c58`),
  file table in `kaze_xa_file_table_init_331c`.
- **SEQ/VAB (jingles):** `kaze_seq_vab_bank_load_33d2c` /
  `kaze_seq_vab_bank_close_33f58` / `kaze_seq_stop_track_34310` /
  `kaze_seq_reset_track_volumes_34048`.
- **SFX:** `kaze_play_sound_effect` and voice
  keyon/keyoff (`kaze_sfx_voice_keyon_34108`/`keyoff`), dispatched from
  gameplay through `kaze_sfx_play_dispatch`/`kaze_sfx_stop_dispatch`.
  Gameplay hooks: `kaze_burner_sound_toggle` (modal latch at
  `base+0x481f8`), the HUD warning voice `0x1e`, and the terminal handlers'
  result jingle gated by `kaze_post_event_sound_latch` (`base+0x48200`).

The high-write BSS leaders around `0xb0d48/0xb2f5c` (CD streaming) and
`0xb1518/0xb2da8/0xb56f4` (Ss/SsVm voice manager) are PsyQ library internals,
not game state.

## Scripted demo playback

`kaze_demo_input_playback` drives the title/attract demo
through the same flight loop: gated by `kaze_state_flags & 0x00100000`, it
reads `{s32 timer, u32 command mask}` entries at `base+0xed388` (stride 8,
cursor at `+0xed42c`, `0xffffffff` terminates) and copies each command into
`kaze_input_current_a` per frame; the balloon flies itself with synthetic
input. Any real button press (mask `kaze_input_bind_pause` or Cross) requests exit via
root `+0x8`, ramps the fade at `+0xed428`, clears the loop-running flag
`+0x4`, and sets the exit code `+0x0`.

## Cross-subsystem cheat sheet

| fact | value |
| --- | --- |
| live root size / pointer | `0xed430` bytes, `kaze_state_base_ptr` |
| altitude → wind layer | `layer = -kaze_pos_y / 0x76c - 3`, clamp 0..4 |
| world tile size | `0xed8` (3800) world units; tile→world `*0xed8 + 0x76c` |
| success / failure flags | `kaze_state_flags & 0x00400000` / `0x00800000` (guard `0x00c00000`) |
| demo-playback flag | `kaze_state_flags & 0x00100000` |
| sky / water gates | `kaze_state_flags & 0x20000000` / `0x10000000` |
| mode-0 markers | `base+0x58a74`, 10 slots, stride 0x5c |
| mode-1/2 objects | `base+0x58e0c`, 6 slots, stride 0x58 |
| dynamic objects | `base+0x5901c`, 18 slots, stride 0x78 |
| zone tables | counts `+0xe2ee0`, 12-byte shapes `+0xe36e0`, remap `+0xe1e60` |
| mode staging | `kaze_menu_mode_source` source → staged `kaze_staged_stage_mode` → live `kaze_stage_mode` |
| mode-1 stage-0 target | `(241300, 0, 239400)` from `kaze_mode1_target_x_table` |

Deeper provenance for the live-root and BSS fields above (the RAM traces and
watch-file captures that confirmed each offset during play) was kept in a
research log alongside the decompilation project; it is not part of this
repository. The field layout itself is `include/kaze_live_root.h`; per-function
purposes are `docs/function-map.json`.
