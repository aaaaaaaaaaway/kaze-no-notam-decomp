#include "types.h"
#include "kaze_syms.h"
extern int kaze_extra_angle;
extern int kaze_extra_strength;
extern int kaze_gas;
extern int kaze_landing_lockout;
extern int kaze_vertical_control;
extern int kaze_vertical_penalty;
extern int kaze_burner_hold_charge;
extern int kaze_pos_y;
typedef struct { int x, y, z, pad; } Vec4;

extern int SquareRoot0(int);

/**
 * kaze_trail_particle_update  (render, 0x80029230)
 *
 * Updates the 128-slot trail/smoke particle pool at +0x5988c (stride 0x2c):
 * advances life 0x20/tick, expires past 0x1000, computes visibility and
 * camera-relative short deltas from root +0x68/+0x6c/+0x70 within 0x76c0.
 */

void kaze_trail_particle_update(void)
{
    extern int kaze_state_base_ptr;
    register int *active __asm__("$20");
    register int *visible __asm__("$16");
    register int i __asm__("$21");
    register u32 half __asm__("$23");
    register u32 maxu __asm__("$22");
    register int dx __asm__("$18");
    register int dz __asm__("$17");
    register int dy __asm__("$19");
    register int root __asm__("$6");
    register int base_ofs __asm__("$3");
    register int root0 __asm__("$2");
    int life;
    u32 dist;
    register u32 vis_result __asm__("$2");

    base_ofs = 0x5988C;
    root0 = kaze_state_base_ptr;
    active = (int *)(root0 + base_ofs);
    if ((*(u32 *)(root0 + 0x14) & 0xC0000000) == 0) {
        i = 0;
        half = 0x8000;
        maxu = 0xFFFF;
        visible = active + 1;
        do {
            if (*active != 0) {
                root = kaze_state_base_ptr;
                life = visible[9] + *(int *)(root + 0xA4) * 0x20;
                visible[9] = life;
                if (life > 0x1000) {
                    visible[9] = 0;
                    *active = 0;
                } else {
                    {
                        register int cam_x __asm__("$3");
                        register int part_x __asm__("$2");
                        register int part_y __asm__("$5");
                        register int part_z __asm__("$4");
                        register int cam_y __asm__("$3");
                        register int cam_z __asm__("$2");

                        cam_x = *(int *)(root + 0x68);
                        part_x = visible[1];
                        part_y = visible[2];
                        __asm__ volatile("" : : "r"(part_y) : "memory");
                        part_z = visible[3];
                        dx = cam_x - part_x;
                        cam_y = *(int *)(root + 0x6C);
                        __asm__ volatile("" : : "r"(cam_y) : "memory");
                        cam_z = *(int *)(root + 0x70);
                        dz = part_z - cam_z;
                        dy = part_y - cam_y;
                    }
                    if (dx + half <= maxu) {
                        if ((dz + half <= maxu) &&
                            (dy + half <= maxu)) {
                            {
                                register int xsq __asm__("$2");
                                register int zsq __asm__("$3");

                                xsq = dx * dx;
                                zsq = dz * dz;
                                dist = SquareRoot0(xsq + zsq);
                            }
                            if (0x76C0 < (int)dist) {
                                goto not_visible;
                            }
                            vis_result = dist;
                            *(s16 *)(visible + 5) = dx;
                            *(s16 *)((char *)visible + 0x16) = dy;
                            *(s16 *)(visible + 6) = dz;
                        } else {
                            vis_result = 0xFFFFFFFF;
                        }
                    } else {
not_visible:
                        vis_result = 0xFFFFFFFF;
                    }
                    *visible = ~vis_result >> 0x1F;
                }
            }
            i++;
            visible += 0xB;
            active += 0xB;
        } while (i < 0x80);
    }
}

/**
 * kaze_trail_particle_spawn  (render, 0x800293bc)
 *
 * Spawns one +0x5988c trail particle at position pos, reusing the first free
 * slot or the oldest (max life word 10); called from the balloon physics
 * snapshot, marker update, and mode-object update.
 */

void kaze_trail_particle_spawn(int *pos)
{
    extern int kaze_state_base_ptr;
    register int *src __asm__("$8");
    register int base_ofs __asm__("$3");
    register int age __asm__("$7");
    register int best __asm__("$5");
    register int i __asm__("$4");
    register int *slot __asm__("$6");
    register int root __asm__("$2");
    register int cur_age __asm__("$3");
    register int final_base __asm__("$4");
    register int final_root __asm__("$3");
    register int offset __asm__("$2");
    int x;
    int y;
    int z;
    int w;

    __asm__ volatile("addu %0,%1,$zero" : "=r"(src) : "r"(pos));
    base_ofs = 0x5988c;
    age = 0;
    best = 0x7f;
    root = kaze_state_base_ptr;
    i = 0;
    slot = (int *)(root + base_ofs);
loop:
    do {
        if (slot[0] == 0) {
            best = i;
            goto found;
        }
        cur_age = slot[10];
        if (age < cur_age) {
            age = cur_age;
            best = i;
        }
        i++;
        slot += 0xb;
        if (i < 0x80) {
            goto loop;
        }
    } while (0);

found:
    if (best < 0x80) {
        final_base = 0x5988c;
        offset = best * 0x2c;
        final_root = kaze_state_base_ptr;
        offset = offset + final_base;
        slot = (int *)(final_root + offset);
        slot[0] = 1;
        x = src[0];
        y = src[1];
        z = src[2];
        w = src[3];
        slot[2] = x;
        slot[3] = y;
        slot[4] = z;
        slot[5] = w;
        __asm__ volatile("" ::: "memory");
        slot[10] = 0;
    }
}

extern Vec4 kaze_prev_frame_pos_x;
/* kaze_prev_frame_pos {x,y,z,aux} */
extern int kaze_pos_x;
/* kaze_pos_x */
/* kaze_pos_y */
extern int kaze_pos_z;
/* kaze_pos_z */
extern int kaze_impact_flash_timer;
/* kaze_extra_angle */
/* kaze_extra_strength */
extern int kaze_stage_mode;
/* kaze_stage_mode */
extern int kaze_dynamic_object_collision_query(int *, int, int *);
int kaze_landing_goal_handler();
extern void kaze_play_sound_effect(int, int, int, int);

/**
 * kaze_landing_event_update  (flight, 0x8002948c)
 *
 * Flight/landing event path touching wind/vertical fields and dynamic-object
 * extra state (+0x59088).
 */

void kaze_landing_event_update(void)
{
    extern char *kaze_state_base_ptr;
    int hit_index;
    int slot_i;
    int best_slot;
    int best_score;
    int score;
    register int off asm("$16");
    int *marker;
    int * obj;
    Vec4 query_pos;
    int query_out[2];

    if ((*(int *)(kaze_state_base_ptr + 0x14) & 0xc2001000) != 0) {
        return;
    }
    query_pos.x = kaze_prev_frame_pos_x.x;
    query_pos.y = -kaze_prev_frame_pos_x.y;
    query_pos.z = kaze_prev_frame_pos_x.z;
    hit_index = kaze_dynamic_object_collision_query((int *)&query_pos, 0x32, query_out);
    if (-1 < hit_index) {
        marker = (int *)(kaze_state_base_ptr + 0x5988c);
        best_score = 0;
        best_slot = 0x7f;
        slot_i = 0;
loop_c:
        if (*marker == 0) goto brk_c;
        __asm__("":"=r"(marker):"0"(marker));
        score = marker[10];
        if (best_score < score) {
            best_score = score;
            best_slot = slot_i;
        }
        slot_i = slot_i + 1;
        marker = marker + 0xb;
        if (slot_i < 0x80) {
            goto loop_c;
        }
post_c:
        if (best_slot < 0x80) {
            marker = (int *)(kaze_state_base_ptr + 0x5988c) + best_slot * 0xb;
            *marker = 1;
            *(Vec4 *)(marker + 2) = kaze_prev_frame_pos_x;
            marker[10] = 0;
        }
        { int _s = hit_index * 0x78; kaze_landing_goal_handler(query_out[0], *(int *)(kaze_state_base_ptr + _s + 0x59088)); }
        obj = (int *)kaze_state_base_ptr + hit_index * 0x1e;
        goto LAB_800297f0;
brk_c:
        best_slot = slot_i;
        goto post_c;
    }
    goto Lmode2;
brk_a:
    best_slot = slot_i;
    goto post_a;
Lmode2:
    if ((kaze_stage_mode == 2) &&
        (hit_index = kaze_mode_object_proximity_test((int *)&query_pos, 0x32, query_out, -1), -1 < hit_index)) {
        marker = (int *)(kaze_state_base_ptr + 0x5988c);
        best_score = 0;
        best_slot = 0x7f;
        slot_i = 0;
        { int _s = hit_index * 0x58; *(int *)(kaze_state_base_ptr + _s + 0x58e60) = 1; }
loop_a:
        if (*marker == 0) goto brk_a;
        score = marker[10];
        if (best_score < score) {
            best_score = score;
            { int _mt = score; __asm__("" :: "r"(_mt) : "memory"); }
            best_slot = slot_i;
        }
        slot_i = slot_i + 1;
        marker = marker + 0xb;
        if (slot_i < 0x80) {
            goto loop_a;
        }
post_a:
        if (best_slot < 0x80) {
            marker = (int *)(kaze_state_base_ptr + 0x5988c) + best_slot * 0xb;
            *marker = 1;
            *(Vec4 *)(marker + 2) = kaze_prev_frame_pos_x;
            marker[10] = 0;
        }
        off = hit_index * 0x58;
        kaze_landing_goal_handler(query_out[0], *(int *)(kaze_state_base_ptr + off + 0x58e4c));
        goto Ltail;
    }
    else {
        query_pos.x = kaze_pos_x;
        query_pos.y = -kaze_pos_y + 0x12c;
        query_pos.z = kaze_pos_z;
        hit_index = kaze_dynamic_object_collision_query((int *)&query_pos, 0x12c, query_out);
        if (-1 < hit_index) {
            kaze_impact_flash_timer = 0x4000;
            kaze_play_sound_effect(8, 100, 1, 1);
            obj = (int *)kaze_state_base_ptr + hit_index * 0x1e;
LAB_800297f0:
            kaze_extra_angle = obj[0x1641a];
            kaze_extra_strength = obj[0x16422];
            __asm__("" :: "r"(obj) : "memory");
            if (obj[0x16407] == 2) {
                obj[0x1641a] = obj[0x1641a] + 0x800U & 0xfff;
                obj[0x16423] = obj[0x16423] ^ 1;
            }
            return;
        }
        if (kaze_stage_mode != 2) {
            return;
        }
        hit_index = kaze_mode_object_proximity_test((int *)&query_pos, 0x12c, query_out, -1);
        if (hit_index < 0) {
            return;
        }
        {
        int _a0 = 8, _a1 = 100, _a2 = 1;
        __asm__("" : "=r"(_a0), "=r"(_a1), "=r"(_a2) : "0"(_a0), "1"(_a1), "2"(_a2));
        off = hit_index * 0x58;
        *(int *)(kaze_state_base_ptr + off + 0x58e60) = 1;
        kaze_impact_flash_timer = 0x4000;
        kaze_play_sound_effect(_a0, _a1, _a2, 1);
        }
        kaze_vertical_control_update(query_out[0], *(int *)(kaze_state_base_ptr + off + 0x58e4c));
    }
Ltail:
    {
        int _a = *(int *)(kaze_state_base_ptr + off + 0x58e48);
        int _b = *(int *)(kaze_state_base_ptr + off + 0x58e4c);
        kaze_extra_angle = _a;
        kaze_extra_strength = _b;
    }
    return;
}

extern int kaze_burner_state;

/**
 * kaze_landing_goal_handler  (flight, 0x80029964)
 *
 * Landing/goal handler: plays landing audio and either raises terminal state
 * flags (+0x14 |= 0x9000) or triggers the landing effect with the struck
 * object's landing parameter.
 */

int kaze_landing_goal_handler(int arg0, int arg1)
{
    extern char *kaze_state_base_ptr;
    int flags;

    kaze_play_sound_effect(9, 100, 1, 1);

    if (kaze_gas > 0) {
        goto landing_effect;
    }
    if (kaze_landing_lockout <= 0) {
        goto terminal;
    }

landing_effect:
    kaze_impact_flash_timer = 0x4000;
    kaze_vertical_control_update(arg0, arg1);

    if (kaze_vertical_control == 0) {
        kaze_vertical_control = -0x28;
    }

    __asm__ volatile("" ::: "memory");
    if (kaze_landing_lockout != 0) {
        goto zero_return;
    }

    {
        int t;

        t = 0xb4;
        kaze_landing_lockout = t;
        t = 0x3e80;
        kaze_vertical_penalty = t;
    }
    goto zero_return;

terminal:
    kaze_burner_state = 0;
    kaze_burner_hold_charge = 0;
    flags = *(int *)(kaze_state_base_ptr + 0x14);
    flags &= -5;
    flags &= -0x21;
    flags &= -3;
    *(int *)(kaze_state_base_ptr + 0x14) = flags | 0x9000;
    return 1;

zero_return:
    return 0;
}

extern void AddPrim(void *, void *);

/**
 * kaze_flight_scene_render_dispatch  (render, 0x80029a70)
 *
 * Main flight-scene render dispatcher: demo overlay, result banners by flags
 * 0x400000/0x800000/0x2000000, post-event bg/prims, balloon, mode objects,
 * markers, dynamic objects, fx, trail particles, terrain+water, sky.
 */

void kaze_flight_scene_render_dispatch(int ot, void *ctx)
{
    extern char *kaze_state_base_ptr;
    unsigned int flags;
    char *base;

    if ((*(unsigned int *)(kaze_state_base_ptr + 0x14) & 0x100000) != 0) {
        kaze_postevent_banner_addprim_32f98();
    }
    base = kaze_state_base_ptr;
    if (*(int *)(base + 8) < 2) {
        goto normal_render;
    }
    if ((*(unsigned int *)(base + 0x14) & 0xC000000) == 0) {
        goto normal_render;
    }
    kaze_screen_composite_draw(ctx);
    kaze_prim_ot_link_insert_345bc(ot + 0x3FFC, (int)ctx + 4, ctx);
    return;

normal_render:
        flags = *(unsigned int *)(kaze_state_base_ptr + 0x14);
        if ((flags & 0x400000) != 0) {
            kaze_score_banner_renderer(ot);
            goto banners_done;
        }
        if ((flags & 0x800000) != 0) {
            kaze_banner_simple_renderer(ot);
            goto banners_done;
        }
        if ((flags & 0x2000000) != 0) {
            kaze_post_event_sprite_anim(ot);
        }
banners_done:
        kaze_postevent_bg_draw(ot);
        kaze_postevent_prim_dispatch(ot);
        kaze_balloon_render(ot);
        kaze_mode_object_render(ot);
        kaze_marker_render(ot);
        kaze_dynobj_render(ot);
        kaze_stage_flag_fx_render(ot);
        kaze_trail_particle_render(ot);
        kaze_terrain_render_setup(ot, ctx);
        kaze_env_sky_render_pass(ctx);
        AddPrim(ctx, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x44 + 0x48D84));
        kaze_prim_ot_link_insert_345bc(ot + 0x3FFC, (int)ctx + 4, ctx);
        kaze_draw_area_anim(ot + 0x3FFC);
}
