#include "types.h"
#include "kaze_syms.h"
extern char kaze_cd_filename_version_suffix[];
extern int strcmp(char *, char *);
extern int strlen(char *);
extern int strncmp(char *, char *, int);
typedef struct {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
} CdlLOC;
typedef struct {
    CdlLOC pos;
    unsigned int size;
    char name[16];
} CdlFILE;
typedef struct {
    char b[4];
} U32;
typedef struct {
    CdlLOC pos;
    u32 size;
    char name[16];
} CdlFILE__1;
typedef struct CdCacheNode {
    struct CdCacheNode *child;
    CdlFILE file;
} CdCacheNode;
typedef struct {
    short x;
    short y;
    short w;
    short h;
} RECT;
typedef struct {
    uint pmode;
    short px;
    short py;
    short pw;
    short ph;
    unsigned char *pixel;
    short cx;
    short cy;
    short cw;
    short ch;
    unsigned char *clut;
} GsIMAGE;

extern void strcat(char *, char *);
CdlFILE__1 * kaze_cd_file_lookup_cached();
#define LOAD_CD_RECORD_PTR(dst) __asm__("addiu %0,$sp,0x110" : "=r"(dst))

/**
 * kaze_cd_load_file_into  (cd_stream, 0x80012704)
 *
 * Loads a CD file into a caller buffer with an optional byte cap: cached
 * path lookup, CdControl seek, CdRead/CdReadSync retry loop; returns the
 * sector-rounded byte count or retries on failure.
 */

int kaze_cd_load_file_into(char *name, char *buf, int max_size)
{
    extern int CdReadSync(int, int);
    extern int CdRead(int, char *, int);
    extern int CdControl(int, int *, int);
    extern void strcpy(char *, char *);
    int ok;
    int read_size;
    int nsect;
    char path[256];
    int file[6];

retry:
    strcpy(path, name);
    strcat(path, kaze_cd_filename_version_suffix);
    {
        register int *filep __asm__("$4");
        LOAD_CD_RECORD_PTR(filep);
        ok = kaze_cd_file_lookup_cached(filep, path);
    }
    if (ok == 0) {
        goto failed;
    }
    ok = CdControl(2, file, 0);
    read_size = -1;
    if (ok != 0) {
        if (max_size > 0) {
            __asm__ volatile("");
            if ((unsigned int)max_size < (unsigned int)file[1]) {
                goto use_max_size;
            }
            read_size = file[1];
            goto have_size;
use_max_size:
            read_size = max_size;
        } else {
            read_size = file[1];
        }
have_size:
        nsect = read_size + 0x7ff;
        if (nsect < 0) {
            nsect = read_size + 0xffe;
        }
        nsect = nsect >> 11;
        read_size = nsect * 0x800;
        ok = CdRead(nsect, buf, 0x80);
        if (ok != 0) {
            goto have_read;
        }
failed:
        read_size = -1;
    }
have_read:
    if (read_size < 0) {
        goto retry;
    }
    ok = CdReadSync(0, 0);
    if (ok != 0) {
        goto retry;
    }
    return read_size;
}

void kaze_cd_dir_tree_build();

/**
 * kaze_cd_file_cache_root_init  (cd_stream, 0x80012808)
 *
 * CdSearchFile on the root path, allocates the 0x1c-byte root record into
 * kaze_cd_root_cache_node storing CdlLOC pos/size and name, then recursively
 * builds the directory cache tree via kaze_cd_dir_tree_build.
 */

void kaze_cd_file_cache_root_init(char *name)
{
    extern char *strcpy(char *, char *);
    extern int kaze_heap_alloc(int, int);
    extern CdlFILE *CdSearchFile(CdlFILE *fp, char *name);
    extern int kaze_cd_root_cache_node;
    CdlFILE file;
    int v;

    if (CdSearchFile(&file, name) != 0) {
        v = kaze_heap_alloc(2, 0x1C);
        kaze_cd_root_cache_node = v;
        if (v != 0) {
            *(CdlLOC *)(v + 4) = file.pos;
            *(unsigned int *)(kaze_cd_root_cache_node + 8) = file.size;
            strcpy((char *)(kaze_cd_root_cache_node + 0xC), name);
            kaze_cd_dir_tree_build(kaze_cd_root_cache_node);
        }
    }
}

/* 4-byte block with alignment 1: struct assignment emits lwl/lwr + swl/swr */
extern void *kaze_heap_malloc(int size);
int kaze_cd_read_sectors();
extern CdlLOC *CdIntToPos(int sector, CdlLOC *pos);

/**
 * kaze_cd_dir_tree_build  (cd_stream, 0x8001289c)
 *
 * Recursive ISO9660 directory parser: reads sectors via
 * kaze_cd_read_sectors, allocates 0x1c-byte records, converts extents with
 * CdIntToPos, copies names, recurses into subdirectories to build the
 * kaze_cd_root_cache_node file cache.
 */

void kaze_cd_dir_tree_build(int *dir)
{
    extern char *strcpy(char *dst, char *src);
    extern void kaze_heap_free(void *ptr);
    extern void *kaze_heap_alloc(int count, int size);
    u8 rec_len;
    int buf;
    int cur;
    u8 *rec;
    int name;
    uint ofs;
    uint limit;
    u8 scratch[24];

    kaze_heap_alloc_dir_toggle();
    buf = (int)kaze_heap_malloc(dir[2]);
    kaze_heap_alloc_dir_toggle();
    do {
    } while (kaze_cd_read_sectors((CdlLOC *)(dir + 1), (void *)buf, dir[2]) < 0);

    *(int *)scratch = 0;
    /* zero-instruction barrier: keeps the count init store ahead of the
       dir[2] condition load so reorg thread-fills the branch delay with
       the loop-counter zeroing instead of the store */
    __asm__ volatile("");
    if (dir[2] != 0) {
        ofs = 0;
        limit = dir[2];
        do {
            rec = (u8 *)(buf + ofs);
            rec_len = *rec;
            while (rec_len != 0) {
                if (1 < rec[0x21]) {
                    *(int *)scratch = *(int *)scratch + 1;
                }
                rec = rec + *rec;
                rec_len = *rec;
            }
            ofs = ofs + 0x800;
        } while (ofs < limit);
    }

    cur = (int)kaze_heap_alloc(*(int *)scratch + 1, 0x1c);
    *dir = cur;
    ofs = 0;
    if (dir[2] != 0) {
        do {
            rec = (u8 *)(buf + ofs);
            if (*rec != 0) {
                name = cur + 0xc;
                do {
                    if (1 < rec[0x21]) {
                        *(U32 *)scratch = *(U32 *)(rec + 2);
                        CdIntToPos(*(int *)scratch, (CdlLOC *)(cur + 4));
                        *(U32 *)(name - 4) = *(U32 *)(rec + 10);
                        strcpy((char *)name, (char *)(rec + 0x21));
                        if ((rec[0x19] & 2) != 0) {
                            kaze_cd_dir_tree_build((int *)cur);
                        }
                        name = name + 0x1c;
                        cur = cur + 0x1c;
                    }
                    rec = rec + *rec;
                } while (*rec != 0);
            }
            ofs = ofs + 0x800;
        } while (ofs < (uint)dir[2]);
    }
    kaze_heap_free((void *)buf);
}

/**
 * kaze_cd_file_lookup_cached  (cd_stream, 0x80012a80)
 *
 * Path lookup in the cached CD directory tree at kaze_cd_root_cache_node,
 * walking backslash-separated components and filling a CdlFILE; falls back
 * to CdSearchFile when the cache is absent.
 */

CdlFILE__1 *kaze_cd_file_lookup_cached(CdlFILE__1 *out, char *path)
{
    extern CdlFILE__1 *CdSearchFile(CdlFILE__1 *, char *);
    extern CdCacheNode *kaze_cd_root_cache_node;
    CdCacheNode *node;
    CdCacheNode *child;
    char *name;
    int len;

    node = kaze_cd_root_cache_node;
    if (node == 0) {
        return CdSearchFile(out, path);
    }
    goto check_node;

hit:
    {
        register int w0 __asm__("$2") = *(int *)&node->file.pos;
        register int w1 __asm__("$3") = node->file.size;
        register int w2 __asm__("$4") = *(int *)(node->file.name + 0x0);
        register int w3 __asm__("$5") = *(int *)(node->file.name + 0x4);

        *(int *)&out->pos = w0;
        out->size = w1;
        *(int *)(out->name + 0x0) = w2;
        *(int *)(out->name + 0x4) = w3;

        w0 = *(int *)(node->file.name + 0x8);
        w1 = *(int *)(node->file.name + 0xC);
        *(int *)(out->name + 0x8) = w0;
        *(int *)(out->name + 0xC) = w1;
        __asm__("" ::: "memory");
        return out;
    }

check_node:
    if (node->file.name[0] == 0) {
        return 0;
    }
    do {
        name = node->file.name;
        if (strcmp(path, name) == 0) {
            goto hit;
        }
        child = node->child;
        if (child != 0) {
            len = strlen(name);
            if (strncmp(path, name, len) == 0 && path[len] == '\\') {
                path += len + 1;
                node = (CdCacheNode *)((char *)child - 0x1C);
            }
        }
        node++;
    } while (node->file.name[0] != 0);
    return 0;
}

u32 kaze_cd_read_at_loc();

/**
 * kaze_cd_read_sectors  (cd_stream, 0x80012bb8)
 *
 * Blocking CD sector read into a destination buffer (retried until non-
 * negative), used by the stage map loader with the +0x5b6e0 staging buffer.
 */

int kaze_cd_read_sectors(void) {
    extern int CdReadSync(int mode, unsigned char *result);
    int ret;

    ret = kaze_cd_read_at_loc();
    if (ret < 0) {
        return -1;
    }
    if (CdReadSync(0, (unsigned char *)0) != 0) {
        return -1;
    }
    return ret;
}

/**
 * kaze_cd_read_at_loc  (cd_stream, 0x80012c08)
 *
 * Reads a file whose CdlFILE header is hdr into dst: CdControl seek then
 * CdRead of the sector-rounded size (capped by size); returns bytes or -1.
 */

u32 kaze_cd_read_at_loc(u8 *hdr, void *dst, u32 size)
{
    extern int CdRead(int, void *, int);
    extern int CdControl(int, u8 *, int);
    register u8 *file __asm__("$3");
    register void *out __asm__("$18");
    int limit;
    register int n __asm__("$2");
    register int bytes __asm__("$16");
    register int sectors __asm__("$17");
    int cmd;
    int ok;

    __asm__("" : "=r"(file), "=r"(out), "=r"(n) : "0"(hdr), "1"(dst), "2"(size));
    if (n > 0) {
        limit = *(u32 *)(file + 4);
        bytes = n;
        if ((u32)bytes >= (u32)limit) {
            bytes = limit;
        }
    } else {
        bytes = *(u32 *)(file + 4);
    }

    n = bytes + 0x7ff;
    cmd = 2;
    if (n < 0) {
        n = bytes + 0xffe;
    }

    ok = CdControl(cmd, file, (sectors = n >> 11, 0));
    if (ok == 0) {
        return 0xffffffff;
    }
    ok = CdRead(sectors, out, 0x80);
    if (ok == 0) {
        return 0xffffffff;
    }
    return bytes;
}

extern int *kaze_cd_load_file_alloc(int);
void kaze_cd_read_postprocess();

/**
 * kaze_cd_load_tim_upload  (cd_stream, 0x80012cb8)
 *
 * Loads a file via kaze_cd_load_file_alloc until the first word equals the
 * TIM magic 0x10, then uploads it through the TIM post-process
 * kaze_cd_read_postprocess and frees the staging buffer.
 */

void kaze_cd_load_tim_upload(int a, int b, int c, int d, int e)
{
    extern void kaze_heap_free(int *);
    int *p;

    while (1) {
        do {
            p = kaze_cd_load_file_alloc(a);
        } while (p == 0);
        if (*p == 0x10) {
            break;
        }
        kaze_heap_free(p);
    }
    kaze_cd_read_postprocess(p, b, c, d, e, 1);
    kaze_heap_free(p);
}

extern void GsGetTimInfo(ulong *, GsIMAGE *);
extern void LoadImage(RECT *, unsigned char *);
extern void DrawSync(int);

/**
 * kaze_cd_read_postprocess  (cd_stream, 0x80012d78)
 *
 * Post-read processing/upload step invoked after each staged CD chunk read
 * by kaze_stage_map_cd_loader.
 */

void kaze_cd_read_postprocess(int *tim, int px, int py, int cx, int cy, int sync)
{
    RECT rect;
    GsIMAGE image;

    if (*tim == 0x10) {
        GsGetTimInfo((ulong *)(tim + 1), &image);
        if (px >= 0 && py >= 0) {
            image.px = px;
            image.py = py;
        }
        rect.x = image.px;
        rect.y = image.py;
        rect.w = image.pw;
        rect.h = image.ph;
        LoadImage(&rect, image.pixel);
        if (sync != 0) {
            DrawSync(0);
        }
        if ((image.pmode & 8) != 0) {
            if (cx >= 0 && cy >= 0) {
                image.cx = cx;
                image.cy = cy;
            }
            rect.x = image.cx;
            rect.y = image.cy;
            rect.w = image.cw;
            rect.h = image.ch;
            LoadImage(&rect, image.clut);
            if (sync != 0) {
                DrawSync(0);
            }
        }
    }
}
