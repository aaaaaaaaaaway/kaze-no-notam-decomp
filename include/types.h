#ifndef TYPES_H
#define TYPES_H

/* Fixed-width and PsyQ-style scalar types shared by all matched sources.
 * These are the exact definitions the per-function files carried locally;
 * consolidating them here is codegen-neutral. */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;

typedef unsigned char uchar;
typedef unsigned char u_char;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned short u_short;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long u_long;

#endif /* TYPES_H */
