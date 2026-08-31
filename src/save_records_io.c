#include "types.h"
#include "kaze_syms.h"
extern char kaze_save_file_name[];
extern int kaze_task_record_index;
extern char kaze_slot_overlay_header_icon_sprites[];
extern char kaze_slot_overlay_header_bar_sprites[];
extern char kaze_slot_overlay_header_tpages[];
extern u8 kaze_slot_overlay_row_glow_rgb;
extern char kaze_slot_overlay_status_sprites[];
extern char kaze_slot_overlay_status_sprite_y0[];
extern char kaze_slot_overlay_status_sprite_v0[];
extern char kaze_slot_overlay_status_sprite_clut[];
extern char kaze_slot_overlay_status_sprite_h[];
extern char kaze_slot_overlay_status_tpage_lo[];
extern char kaze_slot_overlay_status_tpage_hi[];
extern char kaze_slot_overlay_status_tile_prims[];
extern char kaze_slot_overlay_status_border_light[];
extern char kaze_slot_overlay_status_border_dark[];
extern int kaze_memcard_file_count(int);
typedef struct {
    int prim;
    int pad[4];
} Slot;
typedef struct {
    char pad0[4];
    u8 unk4, unk5, unk6;
    char pad7[5];
    u8 unkC, unkD;
} Row;
typedef struct {
    unsigned char c[15];
} Blk;

extern void SetSprt(void *);
extern void SetShadeTex(void *, int);
extern u16 GetClut(int, int);
extern int GetTPage(int, int, int, int);
extern void SetDrawTPage(void *, int, int, unsigned int);
extern void SetTile(void *);
extern void SetLineF3(void *);

/**
 * kaze_slot_overlay_panel_prims_init  (menu, 0x80062270)
 *
 * Initializes the status panel prims: SPRT at
 * kaze_slot_overlay_status_sprites (CLUT 0x390), two DR_TPAGEs (0x240/0x280
 * pages), a TILE at kaze_slot_overlay_status_tile_prims, and LINE_F3 border
 * lines at kaze_slot_overlay_status_border_light/kaze_slot_overlay_status_bo
 * rder_dark.
 */

void kaze_slot_overlay_panel_prims_init(void)
{
    int outer;
    int line_ofs;
    int sprite_ofs;
    char *sprite;
    char *tile;
    char *line1;
    char *line2;

    outer = 0;
    do {
        sprite_ofs = outer * 0x14;
        line_ofs = outer * 0x18;
        sprite = (char *)(sprite_ofs + (int)kaze_slot_overlay_status_sprites);
        SetSprt(sprite);
        SetShadeTex(sprite, 1);
        *(short *)(sprite + 8) = -0x80;
        *(short *)(sprite + 0xA) = 0x48;
        *(short *)(sprite + 0x10) = 0x100;
        *(short *)(sprite + 0x12) = 0x20;
        *(char *)(sprite + 0xC) = 0;
        *(char *)(sprite + 0xD) = 0;
        *(u16 *)(kaze_slot_overlay_status_sprite_clut + sprite_ofs) = GetClut(0x390, 0xF4);
        SetDrawTPage(kaze_slot_overlay_status_tpage_lo + outer * 8, 0, 1, GetTPage(0, 0, 0x240, 0x100) & 0xFFFF);
        SetDrawTPage(kaze_slot_overlay_status_tpage_hi + outer * 8, 0, 1, GetTPage(0, 0, 0x280, 0x100) & 0xFFFF);
        tile = (char *)(outer * 0x10 + (int)kaze_slot_overlay_status_tile_prims);
        SetTile(tile);
        line1 = (char *)(line_ofs + (int)kaze_slot_overlay_status_border_light);
        *(short *)(tile + 8) = -0x80;
        *(short *)(tile + 0xA) = 0x48;
        *(short *)(tile + 0xC) = 0x100;
        *(short *)(tile + 0xE) = 0x20;
        *(char *)(tile + 4) = 0;
        *(char *)(tile + 5) = 0;
        *(char *)(tile + 6) = 0;
        SetLineF3(line1);
        line2 = (char *)(line_ofs + (int)kaze_slot_overlay_status_border_dark);
        SetLineF3(line2);
        outer++;
        *(short *)(line1 + 8) = -0x81;
        *(short *)(line1 + 0xA) = 0x68;
        *(short *)(line1 + 0xC) = -0x81;
        *(short *)(line1 + 0xE) = 0x47;
        *(short *)(line1 + 0x10) = 0x80;
        *(short *)(line1 + 0x12) = 0x47;
        *(short *)(line2 + 8) = 0x80;
        *(short *)(line2 + 0xC) = 0x80;
        *(short *)(line2 + 0xA) = 0x47;
        *(short *)(line2 + 0xE) = 0x68;
        *(short *)(line2 + 0x10) = -0x81;
        *(short *)(line2 + 0x12) = 0x68;
        *(char *)(line1 + 4) = 0xA0;
        *(char *)(line1 + 5) = 0xA0;
        *(char *)(line1 + 6) = 0xA0;
        *(char *)(line2 + 4) = 0x60;
        *(char *)(line2 + 5) = 0x60;
        *(char *)(line2 + 6) = 0x60;
    } while (outer < 2);
}

/**
 * kaze_slot_overlay_add_header_prims  (menu, 0x80062478)
 *
 * Adds the header SPRT pair (kaze_slot_overlay_header_icon_sprites/kaze_slot
 * _overlay_header_bar_sprites) and their DR_TPAGE
 * (kaze_slot_overlay_header_tpages) to the frame's ordering table
 * (&kaze_menu_gs_ot_buf0_org)[buf*5].
 */

void kaze_slot_overlay_add_header_prims(int idx)
{
    extern int AddPrim(void *, void *);
    extern Slot kaze_menu_gs_ot_buf0_org[];
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[idx].prim, kaze_slot_overlay_header_icon_sprites + idx * 0x14);
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[idx].prim, kaze_slot_overlay_header_bar_sprites + idx * 0x14);
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[idx].prim, kaze_slot_overlay_header_tpages + idx * 8);
}

extern char kaze_slot_overlay_glyph_sprites[];
extern char kaze_slot_overlay_glyph_tpages[];

/**
 * kaze_slot_overlay_add_glyph_prims  (menu, 0x80062508)
 *
 * Adds the two glyph SPRTs at kaze_slot_overlay_glyph_sprites and their
 * DR_TPAGE at kaze_slot_overlay_glyph_tpages to the ordering table
 * (&kaze_menu_gs_ot_buf0_org)[buf*5].
 */

void kaze_slot_overlay_add_glyph_prims(int idx)
{
    extern void AddPrim(void *, void *);
    extern char kaze_menu_gs_ot_buf0_org[];
    int i;
    int slot;
    int ofs20;
    int ofs40;
    char *p;

    i = 0;
    slot = idx * 5;
    ofs20 = slot * 4;
    ofs40 = slot * 8;
    p = kaze_slot_overlay_glyph_sprites;
    do {
        AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + ofs20), (void *)(ofs40 + (int)p));
        p += 0x14;
        i++;
    } while (i < 2);
    AddPrim((void *)*(int *)(kaze_menu_gs_ot_buf0_org + (idx * 5 * 4)), kaze_slot_overlay_glyph_tpages + (idx * 8));
}

extern u8 kaze_slot_overlay_row_highlight_sprites;
extern s32 kaze_memcard_slot_index;
extern char kaze_memcard_slot_status_flags[];
extern u8 kaze_slot_overlay_row_icon_sprites;
extern u8 kaze_slot_overlay_row_tpage_prims;

/**
 * kaze_slot_overlay_add_rows  (menu, 0x800625b8)
 *
 * Adds the two row sprite pairs (kaze_slot_overlay_row_icon_sprites/kaze_slo
 * t_overlay_row_highlight_sprites); dims rows whose
 * kaze_memcard_slot_status_flags flags lack bits 1/2/0x100 and pulses
 * brightness of the row selected by kaze_memcard_slot_index, plus DR_TPAGE
 * kaze_slot_overlay_row_tpage_prims.
 */

void kaze_slot_overlay_add_rows(int buf_arg) {
    extern char kaze_menu_gs_ot_buf0_org[];
    extern void AddPrim(void *, void *);
    register s32 row_ofs asm("$16");
    register s32 row_idx asm("$17");
    register u8 c80 asm("$18");
    register s32 flag_ofs asm("$19");
    register s32 cb_ofs asm("$20");
    register u8 *pair_ptr asm("$21");
    register s32 group_ofs asm("$22");
    register u8 c40 asm("$23");
    s32 buf;
    s32 prim_ofs;
    u32 flags;
    register Row *r asm("$3");
    Row *rd;
    int v0, v1;
    u8 c;
    register s32 p5 asm("$2");

    buf = buf_arg;
    row_idx = 0;
    c40 = 0x40;
    c80 = 0x80;
    p5 = buf * 5;
    prim_ofs = p5 << 2;
    pair_ptr = &kaze_slot_overlay_row_highlight_sprites;
    cb_ofs = 0;
    flag_ofs = 0;
    group_ofs = p5 << 3;
    row_ofs = group_ofs;
    do {
        asm volatile("" : "=r"(buf) : "0"(buf));
        flags = *(u32 *)(kaze_memcard_slot_status_flags + flag_ofs);
        if (flags & 1) goto bright;
        if (flags & 2) goto bright;
        if ((flags & 0x100) == 0) goto dim;
    bright:
        r = (Row *)((int)&kaze_slot_overlay_row_highlight_sprites + row_ofs);
        r->unkC = c40;
        r->unkD = 0;
        if (row_idx == kaze_memcard_slot_index) {
            c = *(&kaze_slot_overlay_row_glow_rgb + row_ofs);
            if (c < 0x71U) {
                *(&kaze_slot_overlay_row_glow_rgb + row_ofs) = c + 0x10;
                v0 = *(&kaze_slot_overlay_row_glow_rgb + 1 + row_ofs) + 0x10;
                v1 = *(&kaze_slot_overlay_row_glow_rgb + 2 + row_ofs) + 0x10;
                goto store12;
            }
            r->unk4 = c80;
            r->unk5 = c80;
            r->unk6 = c80;
        } else {
            c = *(&kaze_slot_overlay_row_glow_rgb + row_ofs);
            if (0x4f < c) {
                *(&kaze_slot_overlay_row_glow_rgb + row_ofs) = c - 0x10;
                v0 = *(&kaze_slot_overlay_row_glow_rgb + 1 + row_ofs) - 0x10;
                v1 = *(&kaze_slot_overlay_row_glow_rgb + 2 + row_ofs) - 0x10;
            store12:
                *(&kaze_slot_overlay_row_glow_rgb + 1 + row_ofs) = v0;
                *(&kaze_slot_overlay_row_glow_rgb + 2 + row_ofs) = v1;
            } else {
                r->unk4 = c40;
                r->unk5 = c40;
                r->unk6 = c40;
            }
        }
        goto tail;
    dim:
        rd = (Row *)((int)&kaze_slot_overlay_row_highlight_sprites + row_ofs);
        rd->unkC = c80;
        rd->unkD = 0;
        rd->unk4 = c80;
        rd->unk5 = c80;
        rd->unk6 = c80;
    tail:
        {
            register u8 *cb asm("$5") = &kaze_slot_overlay_row_icon_sprites;
            register u8 *a1a asm("$5");
            a1a = (u8 *)(group_ofs + (cb_ofs + (int)cb));
            cb_ofs = cb_ofs + 0x14;
            flag_ofs = flag_ofs + 4;
            row_ofs = row_ofs + 0x14;
            row_idx = row_idx + 1;
            AddPrim(*(void **)(&kaze_menu_gs_ot_buf0_org[prim_ofs]), a1a);
        }
        {
            u8 *p = (u8 *)(group_ofs + (int)pair_ptr);
            pair_ptr = pair_ptr + 0x14;
            AddPrim(*(void **)(&kaze_menu_gs_ot_buf0_org[prim_ofs]), p);
        }
        if (1 < row_idx) {
            s32 fpo = buf * 0x14;
            AddPrim(*(void **)(&kaze_menu_gs_ot_buf0_org[fpo]), &kaze_slot_overlay_row_tpage_prims + buf * 8);
            return;
        }
    } while (1);
}

extern Blk kaze_status_strip_glyph_table;

/**
 * kaze_slot_overlay_add_status_strip  (menu, 0x8006281c)
 *
 * Adds the status strip SPRT kaze_slot_overlay_status_sprites with message
 * glyph byte from the kaze_status_strip_glyph_table table (index 2 or 8),
 * picks DR_TPAGE kaze_slot_overlay_status_tpage_lo/240 by index, and adds
 * TILE/LINE_F3 frame prims.
 */

void kaze_slot_overlay_add_status_strip(int arg0, int arg1, int arg2)
{
    extern int AddPrim(void *, void *);
    extern Slot kaze_menu_gs_ot_buf0_org[];
    Blk local = kaze_status_strip_glyph_table;

    if (arg2 != 0) {
        *(short *)(kaze_slot_overlay_status_sprite_y0 + arg0 * 20) = 0x48;
        *(short *)(kaze_slot_overlay_status_sprite_h + arg0 * 20) = 0x20;
    } else {
        *(short *)(kaze_slot_overlay_status_sprite_y0 + arg0 * 20) = 0x50;
        *(short *)(kaze_slot_overlay_status_sprite_h + arg0 * 20) = 0x10;
    }
    kaze_slot_overlay_status_sprite_v0[arg0 * 20] = local.c[arg1];
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[arg0].prim, kaze_slot_overlay_status_sprites + arg0 * 20);
    if (arg1 < 4) {
        AddPrim((void *)kaze_menu_gs_ot_buf0_org[arg0].prim, kaze_slot_overlay_status_tpage_lo + arg0 * 8);
    } else {
        AddPrim((void *)kaze_menu_gs_ot_buf0_org[arg0].prim, kaze_slot_overlay_status_tpage_hi + arg0 * 8);
    }
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[arg0].prim, kaze_slot_overlay_status_tile_prims + arg0 * 16);
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[arg0].prim, kaze_slot_overlay_status_border_light + arg0 * 24);
    AddPrim((void *)kaze_menu_gs_ot_buf0_org[arg0].prim, kaze_slot_overlay_status_border_dark + arg0 * 24);
}

extern int kaze_memcard_work_buffer;
extern int kaze_heap_alloc(int, int);
int kaze_save_verify_unpack_records();
extern void kaze_memcard_free_blocks(int);
extern void kaze_heap_free(int);

/**
 * kaze_memcard_load_records  (menu, 0x800629e4)
 *
 * Memcard load: allocs a 0x2000 buffer (kaze_memcard_work_buffer, data at
 * +0x200), scans both card ports for file 'BISLPS-00912' (kaze_memcard_probe
 * _blocking/kaze_memcard_file_count/kaze_memcard_find_file), reads it and
 * verifies/unpacks records via kaze_save_verify_unpack_records.
 */

int kaze_memcard_load_records(void)
{
    extern int kaze_memcard_file_image;
    int slot;
    int count;

    kaze_memcard_events_open();
    kaze_memcard_work_buffer = kaze_heap_alloc(0x2000, 1);
    slot = 0;
    kaze_memcard_file_image = kaze_memcard_work_buffer + 0x200;
    do {
        if (kaze_memcard_probe_blocking(slot) >= 0) {
            count = kaze_memcard_file_count(slot);
            if (kaze_memcard_find_file(count, kaze_save_file_name) == 0) {
                goto no_file;
            }
            if (kaze_memcard_file_read(slot, kaze_save_file_name, kaze_memcard_work_buffer, 1) == 0) {
                if (kaze_save_verify_unpack_records() == 0) {
                    break;
                }
            }
            goto next_slot;
no_file:
            kaze_memcard_free_blocks(count);
        }
next_slot:
        slot++;
    } while (slot < 2);
    kaze_heap_free(kaze_memcard_work_buffer);
    kaze_memcard_events_close();
    return 0;
}

extern int kaze_save_slot_unpack_buffer[];

/**
 * kaze_save_verify_unpack_records  (menu, 0x80062adc)
 *
 * Verifies the 0x1dfc-byte checksum of the save image at
 * kaze_memcard_file_image and unpacks only the five 0x108-entry record
 * tables into the kaze_save_slot_unpack_buffer[] structures (stride 0x14)
 * plus kaze_task_record_index.
 */

int kaze_save_verify_unpack_records(void)
{
    extern u8 *kaze_memcard_file_image;
    register u8 *p asm("$3");
    register int checksum asm("$4");
    register unsigned int i asm("$6");

    p = kaze_memcard_file_image;
    checksum = 0;
    i = 0;
    do {
        i++;
        checksum += *p;
        p++;
    } while (i < 0x1DFC);

    if (checksum != *(int *)(kaze_memcard_file_image + 0x1DFC)) {
        return -1;
    }

    {
        register int outer asm("$6");
        register int base asm("$8");
        register int *record_ptrs asm("$7");

        outer = 0;
        base = 0;
        kaze_task_record_index = kaze_memcard_file_image[2];
        record_ptrs = kaze_save_slot_unpack_buffer;
        do {
            register int inner asm("$5");
            register int off asm("$4");
            register u8 *dst asm("$3");
            register int rec asm("$2");

            inner = 0;
            rec = *record_ptrs;
            off = base;
            dst = (u8 *)(rec + 6);
            do {
                register u8 *src asm("$2");

                src = kaze_memcard_file_image;
                src += off;
                dst[-2] = src[0x3B4];
                src = kaze_memcard_file_image;
                src += off;
                dst[-1] = src[0x3B5];
                inner++;
                src = kaze_memcard_file_image;
                src += off;
                dst[0] = src[0x3B6];
                off += 3;
                dst += 0x14;
            } while (inner < 0x108);
            base += 0x31C;
            outer++;
            record_ptrs++;
        } while (outer < 5);
    }

    return 0;
}
