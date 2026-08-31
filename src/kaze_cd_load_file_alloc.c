/**
 * kaze_cd_load_file_alloc  (cd_stream, 0x80012524)
 *
 * Loads a whole CD file: appends ';1', resolves via the cached lookup
 * kaze_cd_file_lookup_cached, CdControl seek, heap-allocates a sector-
 * rounded buffer (kaze_heap_malloc), CdRead+CdReadSync with retry; returns
 * the buffer.
 */


#include "kaze_syms.h"
extern char kaze_cd_filename_version_suffix[];
extern void strcpy(char *, char *);
extern void strcat(char *, char *);
extern int kaze_cd_file_lookup_cached(int *, char *);
extern int CdControl(int, int *, int);
extern char *kaze_heap_malloc(int);
extern void kaze_heap_free(char *);
extern int CdRead(int, char *, int);
extern int CdReadSync(int, int);

#define LOAD_CD_RECORD_PTR(dst) __asm__("addiu %0,$sp,0x110" : "=r"(dst))

char *kaze_cd_load_file_alloc(char *name)
{
    int ok;
    int nsect;
    char *buf;
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
        goto no_buffer;
    }
    ok = CdControl(2, file, 0);
    buf = 0;
    if (ok != 0) {
        nsect = (file[1] + 0x7ff) / 0x800;
        buf = kaze_heap_malloc(nsect * 0x800);
        if (buf != 0) {
            ok = CdRead(nsect, buf, 0x80);
            if (ok != 0) {
                goto have_buffer;
            }
            kaze_heap_free(buf);
        }
no_buffer:
        buf = 0;
    }
have_buffer:
    if (buf == 0) {
        goto retry;
    }
    ok = CdReadSync(0, 0);
    if (ok == 0) {
        return buf;
    }
    kaze_heap_free(buf);
    goto retry;
}
