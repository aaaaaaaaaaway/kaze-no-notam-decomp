#include "types.h"
#include "kaze_syms.h"
extern int kaze_heap_free_list_head_size;
extern int kaze_heap_alloc_direction_flag;
extern int kaze_str_movie_end_flag;
extern int kaze_str_vlc_buffer_a;
extern int kaze_str_vlc_buffer_index;
extern int kaze_str_mdec_buffer_index;
extern int kaze_str_display_page_index;
extern void DecDCTReset(int);
extern void DecDCTin(u_long *, int);
extern void DecDCTout(u_long *, int);
extern void DecDCToutCallback(void *);
extern void SetDispMask(int);
extern void StSetRing(u_long *, int);
extern void StSetStream(int, int, int, void *, void *);
typedef struct {
    u_char minute, second, sector, track;
} CdlLOC;
typedef struct {
    CdlLOC pos;
    unsigned int size;
    char name[24];
} CdlFILE;
typedef struct {
    short x, y, w, h;
} RECT;
typedef struct {
    RECT disp;
    RECT screen;
    u_char isinter;
    u_char isrgb24;
    u_char pad0;
    u_char pad1;
} DISPENV;
typedef struct {
    short left, right;
} SndVolume;

/**
 * kaze_heap_init  (library, 0x80012ec0)
 *
 * Initializes the K&R-style free-list heap: aligns start to 8, sets head
 * kaze_heap_free_list_head with one free block covering size bytes (unit = 8
 * bytes), clears kaze_heap_free_list_head_size.
 */

void kaze_heap_init(int start, int size)
{
    extern int *kaze_heap_free_list_head;
    int **p = &kaze_heap_free_list_head;

    *p = (int *)(((u32)(start + 7) >> 3) << 3);
    kaze_heap_free_list_head_size = 0;
    *kaze_heap_free_list_head = (int)p;
    kaze_heap_free_list_head[1] = (u32)(size - ((int)*p - start)) >> 3;
}

/**
 * kaze_heap_malloc  (library, 0x80012f10)
 *
 * First-fit free-list allocator over the kaze_heap_free_list_head heap in
 * 8-byte units; kaze_heap_alloc_direction_flag selects front- or back-of-
 * block split. Underlies kaze_heap_alloc.
 */

int *kaze_heap_malloc(int size)
{
    extern int *kaze_heap_free_list_head;
    int *prev;
    int *cur;
    int n;
    int mode;
    int avail;

    prev = (int *)&kaze_heap_free_list_head;
    n = ((uint)(size + 7) >> 3) + 1;
    cur = (int *)*prev;
    if (cur != (int *)&kaze_heap_free_list_head) {
        mode = kaze_heap_alloc_direction_flag;
        do {
            avail = cur[1];
            if (n <= avail) {
                if (avail == n) {
                    *prev = *cur;
                } else {
                    if (mode != 0) {
                        cur[1] = avail - n;
                        cur = cur + (avail - n) * 2;
                    } else {
                        *prev = (int)(cur + n * 2);
                        cur[n * 2] = *cur;
                        *(int *)(*prev + 4) = cur[1] - n;
                    }
                    cur[1] = n;
                }
                return cur + 2;
            }
            prev = cur;
            cur = (int *)*cur;
        } while (cur != (int *)&kaze_heap_free_list_head);
    }
    return (int *)0;
}

/**
 * kaze_heap_free  (library, 0x80012fd0)
 *
 * Frees a block back into the kaze_heap_free_list_head free list, coalescing
 * with adjacent free neighbors.
 */

void kaze_heap_free(int ptr)
{
    extern int kaze_heap_free_list_head[];
    int *bp;
    int *p;
    int *next;
    int size;
    int psize;

    if (ptr != 0) {
        bp = (int *)(ptr - 8);
        p = kaze_heap_free_list_head;
        while (!((p < bp) && (bp < (int *)*p))) {
            next = (int *)*p;
            if (next == kaze_heap_free_list_head) {
                break;
            }
            p = next;
        }

        size = bp[1];
        next = (int *)*p;
        if (bp + size * 2 == next) {
            bp[1] = size + next[1];
            *bp = *(int *)*p;
        } else {
            *bp = (int)next;
        }

        psize = p[1];
        if (p + psize * 2 == bp) {
            p[1] = psize + bp[1];
            *p = *bp;
        } else {
            *p = (int)bp;
        }
    }
}

/**
 * kaze_heap_alloc  (library, 0x800130a0)
 *
 * Heap allocator used for the live-root block (called with element count 1
 * and size 0xed430 by the main loop).
 */

int *kaze_heap_alloc(int count, int size)
{
    extern void bzero(u8 *, int);
    extern int *kaze_heap_free_list_head;
    int block_size;
    int need;
    int *prev;
    int *node;
    int *res;
    int mode;

    prev = (int *)&kaze_heap_free_list_head;
    need = ((count * size + 7U) >> 3) + 1;
    node = kaze_heap_free_list_head;
    if (node != (int *)&kaze_heap_free_list_head) {
        mode = kaze_heap_alloc_direction_flag;
        do {
            block_size = node[1];
            if (need <= block_size) {
                if (block_size == need) {
                    *prev = *node;
                } else {
                    if (mode != 0) {
                        node[1] = block_size - need;
                        node = node + (block_size - need) * 2;
                    } else {
                        *prev = (int)(node + need * 2);
                        node[need * 2] = *node;
                        *(int *)(*prev + 4) = node[1] - need;
                    }
                    node[1] = need;
                }
                res = node + 2;
                goto done;
            }
            prev = node;
            node = (int *)*node;
        } while (node != (int *)&kaze_heap_free_list_head);
    }
    res = 0;
done:
    if (res != 0) {
        bzero((u8 *)res, count * size);
    }
    return res;
}

/* kaze_str_movie_play (0x8001319c), 560 bytes, gcc 2.7.2-psx -O2. EXACT MATCH.
 * Closing levers (32 -> 0 words):
 *  - Cluster 1 (callback ptr): 0x800138d8 stream callback is kaze_str_stream_start
 *    (0x80013784) + 0x154; pass (void*)((char*)kaze_str_stream_start + 0x154) to get
 *    the %hi/%lo symbol+offset reloc (lui/addiu) not the raw-constant lui/ori.
 *  - Cluster 2a (slot addr): scoped `register int *cb asm("$3")=(int*)((char*)base+0xc)`
 *    (fresh base ptr in v1) + offset-first int arith `slot=(int*)((kaze_str_mdec_buffer_index<<2)+(int)cb)`
 *    so PLUS op0 is idx*4 => addend-first `addu a0,v0,v1`.
 *  - SINGLE-TEMP SERIALIZATION (the finishing lever, 32 -> 12): the original threads
 *    BOTH the a-raw and b-raw HImode loads through the SAME register a2 ($6). Pinning
 *    braw to $6 too (`register int braw asm("$6")`) makes the b-raw `lh a2,50(s0)`
 *    schedule early (killing the load-delay nop) and reuse a2, matching target exactly.
 *  - Cluster 2c (kaze_str_display_page_index==0)*0xf0: launder the boolean and pin it to $3/v1
 *    (`register int yb asm("$3")=(...); asm("":"=r"(yb):"0"(yb))`). Stripping the {0,1}
 *    range info + a caller-saved pin makes gcc synth-mult by 0xf0 as sll4/subu/sll4
 *    in v1 (was the sltiu/negu/andi boolean combine); the $3 pin keeps it caller-saved
 *    (the documented "launder forces callee-saved" dead-end was the MISSING pin).
 *  - a/b register naming (12 -> 0): split b's live range -- `register int be asm("$3")`
 *    holds the -1/+14 raw intermediate in v1, `register int b asm("$2")` (v0) holds the
 *    final (be>>4)+1; and spell the scale as `b << 8` (NOT `b * 0x100`) so gcc keeps
 *    the shift on b (staying in v0) instead of reassociating it onto a (a * (b<<8)>>1
 *    => sll v0,v0,8; mult a1,v0). `b * 0x100` reassociated to sll a1 => 2-word miss.
 */
extern int kaze_str_movie_stream_count;
extern int kaze_input_current_a;
extern CdlFILE *CdSearchFile(CdlFILE *, char *);
extern void SsGetSerialVol(int, SndVolume *);
extern void SsSetSerialVol(int, int, int);
void kaze_str_playback_buffers_init();
void kaze_str_stream_start();
extern void VSync(int);
extern void SetDefDispEnv(DISPENV *, int, int, int, int);
extern void PutDispEnv(DISPENV *);
extern void CdControlB(int, u_char *, u_char *);

/**
 * kaze_str_movie_play  (cd_stream, 0x8001319c)
 *
 * Plays an STR movie: CdSearchFile, optional SsSetSerialVol, allocates
 * buffers, streams and DecDCTin/DecDCTout each frame with VSync/PutDispEnv
 * page flip until Start (0x800 in kaze_input_current_a) or end flag
 * kaze_str_movie_end_flag.
 */

void kaze_str_movie_play(char *name, int last_frame, void *x, int y, u_short vol)
{
    int *base;
    int *slot;
    int a;
    register int b asm("$2");
    SndVolume svol[2];
    DISPENV disp;
    CdlFILE file;

    if (CdSearchFile(&file, name) != 0) {
        if ((short)vol >= 0) {
            SsGetSerialVol(0, svol);
            SsSetSerialVol(0, (short)vol, (short)vol);
        }
        kaze_str_movie_stream_count = last_frame + 1;
        base = &kaze_str_vlc_buffer_a;
        kaze_str_playback_buffers_init(base, x, y, x, y + 0xf0);
        kaze_str_stream_start(&file, (void *)((char *)kaze_str_stream_start + 0x154));
        kaze_str_frame_vlc_decode(base);
        do {
            DecDCTin((u_long *)base[kaze_str_vlc_buffer_index], 2);
            {
                register int *cb asm("$3") = (int *)((char *)base + 0xc);
                slot = (int *)((kaze_str_mdec_buffer_index << 2) + (int)cb);
            }
            {
                register int araw asm("$6") = *(short *)((char *)base + 0x30);
                a = araw - 1;
                if (a < 0) a = araw + 0xe;
            }
            a = (a >> 4) + 1;
            {
                register int braw asm("$6") = *(short *)((char *)base + 0x32);
                register int be asm("$3") = braw - 1;
                if (be < 0) be = braw + 0xe;
                b = (be >> 4) + 1;
            }
            DecDCTout((u_long *)*slot, a * (b << 8) >> 1);
            kaze_str_frame_vlc_decode(base);
            VSync(0);
            {
                register int yb asm("$3") = (kaze_str_display_page_index == 0);
                __asm__("" : "=r"(yb) : "0"(yb));
                SetDefDispEnv(&disp, 0, yb * 0xf0, 0x140, 0xf0);
            }
            PutDispEnv(&disp);
            SetDispMask(1);
            kaze_pad_mask_update();
        } while ((kaze_input_current_a & 0x800) == 0 && kaze_str_movie_end_flag == 0);
        DecDCToutCallback(0);
        kaze_st_unset_ring();
        CdControlB(9, 0, 0);
        kaze_str_playback_buffers_free();
        if ((int)((unsigned int)vol << 0x10) >= 0) {
            SsSetSerialVol(0, svol[0].left, svol[0].right);
        }
    }
}

/**
 * kaze_str_playback_buffers_init  (cd_stream, 0x800133cc)
 *
 * Allocates and zeroes the STR playback context: two 0x28000-byte VLC
 * buffers and two 0x3c00-byte MDEC image buffers from the
 * kaze_heap_free_list_head heap, plus display x/y/size fields and slice
 * width 0x10 in the context struct.
 */

void kaze_str_playback_buffers_init(int *ctx, int x, int y, int width, int height)
{
    extern void bzero(u8 *, int);
    extern int kaze_heap_free_list_head;
    int *ring;
    register int *node       __asm__("$4");
    register int *prev       __asm__("$5");
    register int need_size   __asm__("$6");
    register int free_mode   __asm__("$7");
    register int split_step  __asm__("$8");
    register int *head       __asm__("$9");
    register int *split      __asm__("$3");
    register int split_step2 __asm__("$9");
    register int *head2      __asm__("$8");
    int size;

    /* ---- block 1: VLC buffer 0x28000 (code 0x5001) -> ctx[0] ---- */
    node = (int *)kaze_heap_free_list_head;
    prev = &kaze_heap_free_list_head;
    need_size = 0x5001;
    if (node != &kaze_heap_free_list_head) {
        free_mode = kaze_heap_alloc_direction_flag;
        split_step = 0x28008;
        head = &kaze_heap_free_list_head;
        do {
            size = node[1];
            if (size >= need_size) {
                if (size == need_size) {
                    *prev = *node;
                } else {
                    if (free_mode != 0) {
                        node[1] = size - need_size;
                        node = node + (size - need_size) * 2;
                    } else {
                        {
                            register int *split_node __asm__("$3") = (int *)((u8 *)node + split_step);
                            __asm__("" : "=r"(split_node) : "0"(split_node));
                            *prev = (int)split_node;
                            split_node[0] = node[0];
                            ((int *)*(volatile int *)prev)[1] = node[1] - need_size;
                        }
                    }
                    node[1] = need_size;
                }
                ring = node + 2;
                goto done1;
            }
            prev = node;
            node = (int *)*node;
        } while (node != head);
    }
    ring = 0;
done1:
    if (ring != 0) {
        bzero((u8 *)ring, 0x28000);
    }
    *ctx = (int)ring;

    /* ---- block 2: VLC buffer 0x28000 (code 0x5001) -> ctx[1], ctx[2]=0 ---- */
    node = (int *)kaze_heap_free_list_head;
    prev = &kaze_heap_free_list_head;
    need_size = 0x5001;
    if (node != &kaze_heap_free_list_head) {
        free_mode = kaze_heap_alloc_direction_flag;
        split_step = 0x28008;
        head = &kaze_heap_free_list_head;
        do {
            size = node[1];
            if (size >= need_size) {
                if (size == need_size) {
                    *prev = *node;
                } else {
                    if (free_mode != 0) {
                        node[1] = size - need_size;
                        node = node + (size - need_size) * 2;
                    } else {
                        {
                            register int *split_node __asm__("$3") = (int *)((u8 *)node + split_step);
                            __asm__("" : "=r"(split_node) : "0"(split_node));
                            *prev = (int)split_node;
                            split_node[0] = node[0];
                            ((int *)*(volatile int *)prev)[1] = node[1] - need_size;
                        }
                    }
                    node[1] = need_size;
                }
                ring = node + 2;
                goto done2;
            }
            prev = node;
            node = (int *)*node;
        } while (node != head);
    }
    ring = 0;
done2:
    if (ring != 0) {
        bzero((u8 *)ring, 0x28000);
    }
    ctx[1] = (int)ring;
    ctx[2] = 0;

    /* ---- block 3: MDEC image 0x3c00 (code 0x781) -> ctx[3] ---- */
    prev = &kaze_heap_free_list_head;
    __asm__ volatile("" : "=r"(prev) : "0"(prev) : "memory");
    node = (int *)kaze_heap_free_list_head;
    __asm__ volatile("" : "=r"(node) : "0"(node) : "memory");
    need_size = 0x781;
    if (node != prev) {
        free_mode = kaze_heap_alloc_direction_flag;
        split_step2 = 15368;
        head2 = prev;
        do {
            size = node[1];
            if (size >= need_size) {
                if (size == need_size) {
                    *prev = *node;
                } else {
                    if (free_mode != 0) {
                        node[1] = size - need_size;
                        node = node + (size - need_size) * 2;
                    } else {
                        {
                            register int *split_node __asm__("$3") = (int *)((u8 *)node + split_step2);
                            __asm__("" : "=r"(split_node) : "0"(split_node));
                            *prev = (int)split_node;
                            split_node[0] = node[0];
                            ((int *)*(volatile int *)prev)[1] = node[1] - need_size;
                        }
                    }
                    node[1] = need_size;
                }
                ring = node + 2;
                goto done3;
            }
            prev = node;
            node = (int *)*node;
        } while (node != head2);
    }
    ring = 0;
done3:
    if (ring != 0) {
        bzero((u8 *)ring, 0x3c00);
    }
    prev = &kaze_heap_free_list_head;
    ctx[3] = (int)ring;
    __asm__ volatile("" : "=r"(prev) : "0"(prev) : "memory");

    /* ---- block 4: MDEC image 0x3c00 (code 0x781) -> ctx[4], ctx[5]=0 ---- */
    node = (int *)kaze_heap_free_list_head;
    __asm__ volatile("" : "=r"(node) : "0"(node) : "memory");
    need_size = 0x781;
    if (node != prev) {
        free_mode = kaze_heap_alloc_direction_flag;
        split_step2 = 15368;
        head2 = prev;
        do {
            size = node[1];
            if (size >= need_size) {
                if (size == need_size) {
                    *prev = *node;
                } else {
                    if (free_mode != 0) {
                        node[1] = size - need_size;
                        node = node + (size - need_size) * 2;
                    } else {
                        {
                            register int *split_node __asm__("$3") = (int *)((u8 *)node + split_step2);
                            __asm__("" : "=r"(split_node) : "0"(split_node));
                            *prev = (int)split_node;
                            split_node[0] = node[0];
                            ((int *)*(volatile int *)prev)[1] = node[1] - need_size;
                        }
                    }
                    node[1] = need_size;
                }
                ring = node + 2;
                goto done4;
            }
            prev = node;
            node = (int *)*node;
        } while (node != head2);
    }
    ring = 0;
done4:
    if (ring != 0) {
        bzero((u8 *)ring, 0x3c00);
    }
    ctx[4] = (int)ring;
    ctx[5] = 0;
    *(short *)(ctx + 6) = x;
    *(short *)((int)ctx + 0x1a) = y;
    *(short *)(ctx + 8) = width;
    *(short *)((int)ctx + 0x22) = height;
    ctx[10] = 0;
    *(short *)(ctx + 0xb) = x;
    *(short *)((int)ctx + 0x2e) = y;
    *(short *)(ctx + 0xc) = 0x10;
    ctx[0xd] = 0;
    return;
}

extern u_long *kaze_str_stream_ring_buffer;

/**
 * kaze_str_stream_start  (cd_stream, 0x80013784)
 *
 * Starts CD movie streaming: allocates the 0x10000-byte ring
 * kaze_str_stream_ring_buffer, DecDCTReset+DecDCToutCallback,
 * StSetRing/StSetStream with frame count kaze_str_movie_stream_count,
 * seeks/starts CdRead2, clears end flag kaze_str_movie_end_flag.
 */

void kaze_str_stream_start(void *loc, void *callback)
{
    extern void bzero(u_char *, int);
    extern int kaze_heap_free_list_head;
    register void *loc_reg __asm__("$18");
    register void *callback_reg __asm__("$17");
    register u_long *ring __asm__("$16");
    register int *node __asm__("$4") = (int *)kaze_heap_free_list_head;
    register int *prev __asm__("$5") = &kaze_heap_free_list_head;
    register int need_size __asm__("$6") = 0x2001;
    register int free_mode __asm__("$7");
    register int split_step __asm__("$8");
    register int *head __asm__("$9");
    int size;
    register int *split __asm__("$3");

    loc_reg = loc;
    callback_reg = callback;
    if (node != &kaze_heap_free_list_head) {
        free_mode = kaze_heap_alloc_direction_flag;
        split_step = 0x10008;
        head = &kaze_heap_free_list_head;
        do {
            size = node[1];
            if (size >= need_size) {
                if (size == need_size) {
                    *prev = *node;
                } else {
                    if (free_mode != 0) {
                        node[1] = size - need_size;
                        node = node + (size - need_size) * 2;
                    } else {
                        split = (int *)((char *)node + split_step);
                        __asm__("" : "=r"(split) : "0"(split));
                        *prev = (int)split;
                        *split = *node;
                        split = (int *)*prev;
                        split[1] = node[1] - need_size;
                    }
                    node[1] = need_size;
                }
                ring = (u_long *)(node + 2);
                goto found_ring;
            }
            prev = node;
            node = (int *)*node;
        } while (node != head);
    }
    ring = 0;

found_ring:
    if (ring != 0) {
        bzero((u_char *)ring, 0x10000);
    }
    kaze_str_stream_ring_buffer = ring;
    DecDCTReset(1);
    DecDCToutCallback(callback_reg);
    StSetRing(kaze_str_stream_ring_buffer, 0x20);
    StSetStream(0, 1, kaze_str_movie_stream_count, 0, 0);
    kaze_cd_stream_seek_start(loc_reg);
    kaze_str_movie_end_flag = 0;
}
