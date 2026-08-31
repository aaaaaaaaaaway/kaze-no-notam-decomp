#include "types.h"
#include "kaze_syms.h"
extern s16 kaze_memcard_device_path_tail;
extern void _card_clear(int);
extern void _card_info(int);
extern void _card_load(int);
extern int close(int);
extern int firstfile(char *, char *);
extern int nextfile(char *);
extern int open(char *, int);
extern int read(int, int, int);
extern int write(int, void *, int);
extern int kaze_memcard_wait_result_a(void);
typedef struct {
    char b[6];
} Name6;
typedef struct {
    char c[7];
} S7;

/**
 * kaze_memcard_probe_blocking  (library, 0x8004b6f4)
 *
 * Blocking _card_info/_card_load probe using kaze_memcard_wait_result_a
 * event wait; returns memcard status code.
 */

int kaze_memcard_probe_blocking(int slot)
{
    register int slot_save __asm__("$18") = slot;
    register int port __asm__("$16");
    register int ret __asm__("$17");
    register int status __asm__("$3");
    register int out __asm__("$2");

    kaze_memcard_clear_events_a();
    port = slot_save << 4;
    _card_info(port);
    status = kaze_memcard_wait_result_a();

    if (status == 1) {
        goto err3_plain;
    }
    if (status < 2) {
        ret = 0;
        if (status == 0) {
            goto load_phase;
        }
        out = -3;
        goto done;
    }
    if (status == 2) {
        goto err1;
    }
    if (status != 3) {
        out = -3;
        goto done;
    }
    ret = 1;
    kaze_memcard_clear_events_b();
    _card_clear(port);
    kaze_memcard_wait_result_b();

load_phase:
    kaze_memcard_clear_events_a();
    _card_load(slot_save << 4);
    status = kaze_memcard_wait_result_a();
    if (status == 1) {
        goto err3_plain;
    }
    if (status < 2) {
        if (status == 0) {
            goto ret_saved;
        }
        out = -3;
        goto done;
    }
    if (status == 2) {
        goto err1;
    }
    if (status == 3) {
        goto err2;
    }
    out = -3;
    goto done;
ret_saved:
    out = ret;
    goto done;
err2:
    out = -2;
    goto done;
err1:
    out = -1;
    goto done;
err3_plain:
    out = -3;
done:
    return out;
}

extern void format(Name6 *);

/**
 * kaze_memcard_format  (library, 0x8004b7fc)
 *
 * Builds the device name from kaze_memcard_device_path and format()s the
 * memory card, waiting for the result event.
 */

int kaze_memcard_format(int arg)
{
    extern Name6 kaze_memcard_device_path;
    Name6 buf;
    int r;

    buf = kaze_memcard_device_path;
    if (arg != 0) {
        buf.b[2] = 0x31;
    }
    kaze_memcard_clear_events_b();
    format(&buf);
    r = kaze_memcard_wait_result_b();
    if (r != 0) {
        if (r == 2) {
            r = -1;
        } else {
            r = -2;
        }
    }
    return r;
}

extern char *strcat(char *, char *);

/**
 * kaze_memcard_file_read  (library, 0x8004b888)
 *
 * open()/read()s a save file (blocks<<13 bytes) from the memory-card device
 * path kaze_memcard_device_path.
 */

unsigned int kaze_memcard_file_read(int slot, char *name, int dst, int blocks)
{
    extern int kaze_memcard_device_path;
    register unsigned int slot_save __asm__("$16") = slot;
    register int fd __asm__("$17");
    register int byte_count __asm__("$18");
    register int dst_save __asm__("$19");
    int nread;
    unsigned int ret;
    int tail;
    register char *pathp __asm__("$17");
    register char *callp __asm__("$4");
    char path[32];

    __asm__ volatile("" : "=r"(slot_save) : "0"(slot_save));
    *(int *)path = kaze_memcard_device_path;
    tail = kaze_memcard_device_path_tail;
    *(s16 *)(path + 4) = tail;
    __asm__ volatile("" : : : "memory");
    pathp = path;
    __asm__ volatile("" : "=r"(callp) : "0"(pathp));
    dst_save = dst;
    __asm__ volatile("" : : "r"(dst_save));
    strcat(callp, (byte_count = blocks, name));
    if (slot_save != 0) {
        path[2] = 0x31;
    }
    fd = open(pathp, 1);
    if (fd != -1) {
        byte_count <<= 13;
        nread = read(fd, dst_save, byte_count);
        ret = 0xfffffffe;
        if (nread == byte_count) {
            ret = 0;
        }
        close(fd);
    } else {
        __asm__ volatile("addiu %0,$0,-1" : "=r"(ret));
    }
    return ret;
}

/**
 * kaze_memcard_file_write  (library, 0x8004b954)
 *
 * Optionally creates then open()/write()s a save file (blocks<<13 bytes) to
 * the memory-card device.
 */

uint kaze_memcard_file_write(int slot, char *name, void *src, int blocks, int create)
{
    extern int kaze_memcard_device_path;
    register uint slot_save __asm__("$16") = slot;
    register int fd __asm__("$17");
    register int block_count __asm__("$18");
    register int create_save __asm__("$19") = create;
    register void *src_save __asm__("$20");
    int prefix_tail;
    int nwritten;
    register char *pathp __asm__("$17");
    register char *cat_arg __asm__("$4");
    char path[32];

    __asm__ volatile("" : "=r"(create_save) : "0"(create_save));
    *(int *)path = kaze_memcard_device_path;
    prefix_tail = kaze_memcard_device_path_tail;
    *(s16 *)(path + 4) = prefix_tail;
    __asm__ volatile("" : : : "memory");
    pathp = path;
    cat_arg = pathp;
    src_save = src;
    __asm__ volatile("" : : "r"(cat_arg), "r"(src_save));
    strcat(cat_arg, (block_count = blocks, name));
    if (slot_save != 0) {
        path[2] = 0x31;
    }
    if (create_save != 0) {
        fd = open(pathp, (block_count << 16) | 0x200);
        if (fd == -1) {
            return 0xfffffffd;
        }
        close(fd);
    }
    fd = open(path, 2);
    if (fd != -1) {
        block_count <<= 13;
        nwritten = write(fd, src_save, block_count);
        slot_save = 0xfffffffe;
        __asm__ volatile("" : : "r"(slot_save));
        slot_save = (-(uint)(nwritten != block_count)) & slot_save;
        close(fd);
    } else {
        __asm__ volatile("li %0,-1" : "=r"(slot_save));
    }
    return slot_save;
}

extern S7 kaze_memcard_file_glob_pattern;
extern char kaze_memcard_directory_table;

/**
 * kaze_memcard_file_count  (library, 0x8004ba58)
 *
 * firstfile/nextfile enumerates matching card files into
 * kaze_memcard_directory_table and returns the count.
 */

int kaze_memcard_file_count(int arg)
{
    S7 name;
    int count;
    char *p;
    char *ret;

    name = kaze_memcard_file_glob_pattern;
    count = 0;
    if (arg != 0) {
        name.c[2] = 0x31;
    }
    p = &kaze_memcard_directory_table;
    ret = (char *)firstfile(name.c, p);
    if (ret == p) {
        do {
            count++;
            p += 0x28;
            ret = (char *)nextfile(p);
        } while (ret == p);
    }
    return count;
}
