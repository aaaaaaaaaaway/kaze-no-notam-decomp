#include "types.h"
#include "kaze_syms.h"
extern void CdMix(void *);
extern void SpuInitHot(void);
extern void SsSetMVol(int, int);
extern void SsSetSerialAttr(char, char, char);
extern void SsSetTableSize(char *, int, int);
extern void SsSetTickMode(int);
extern void SsUtSetReverbDepth(int, int);
extern void SsUtSetReverbType(int);
extern void exit(int);

extern char *kaze_state_base_ptr;
extern void AddPrim(void *, void *);

/**
 * kaze_post_event_sprite_anim  (render, 0x80032300)
 *
 * Advances the post-event sprite animation counter at +0x4a190.
 */

void kaze_post_event_sprite_anim(void *ot_arg)
{
    register void *ot asm("$16");
    register char *base asm("$7");
    register int phase asm("$3");
    register int magic asm("$4");

    ot = ot_arg;
    {
        register char *seed asm("$7");

        seed = kaze_state_base_ptr;
        __asm__("" : "=r"(seed) : "0"(seed));
        base = seed;
    }
    magic = 0x51EB0000;
    __asm__ volatile("" : : "r"(magic), "r"(base));
    __asm__ volatile("" : "=r"(ot) : "0"(ot));
    magic |= 0x851F;
    phase = *(int *)(base + 0x4A190);
    {
        register int delta asm("$2");

        delta = *(int *)(base + 0xA4);
        phase += delta;
    }
    phase = phase % 100;
    *(int *)(base + 0x4A190) = phase;
    __asm__ volatile("" : "=r"(phase) : "0"(phase) : "memory");
    if (phase < 0x3C) {
        {
            register int idx asm("$2");
            register int page_base asm("$4");

            idx = *(int *)(base + 0x98);
            page_base = (idx << 2) + idx;
            page_base <<= 2;
            page_base += (int)base;
            {
                register int raw asm("$2");
                register int half asm("$3");

                raw = *(unsigned short *)(page_base + 0x49F00) << 16;
                half = raw >> 16;
                raw = (unsigned int)raw >> 31;
                half += raw;
                half >>= 1;
                raw = 0xA0;
                raw -= half;
                *(short *)(page_base + 0x49EF8) = raw;
            }
        }

        {
            register int idx asm("$2");
            register int page_base asm("$4");

            idx = *(int *)(base + 0x98);
            page_base = (idx << 2) + idx;
            page_base <<= 2;
            page_base += (int)base;
            {
                register int raw asm("$2");
                register int half asm("$3");

                raw = *(unsigned short *)(page_base + 0x49F02) << 16;
                half = raw >> 16;
                raw = (unsigned int)raw >> 31;
                half += raw;
                half >>= 1;
                raw = 0x78;
                raw -= half;
                *(short *)(page_base + 0x49EFA) = raw;
            }
        }

        AddPrim(ot, base + (*(int *)(base + 0x98) * 0x14 + 0x49EF0));
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x4A118));
    }
}

extern void kaze_panel_frame_vertices(int, int, int, int, int, int);

/**
 * kaze_confirm_box_renderer  (menu, 0x80032474)
 *
 * Draws the confirm/dialog box at a caller position: confirmation SPRT group
 * (+0x49204..+0x49248), background POLY strips, and the +0x48e1c TPAGE.
 */

void kaze_confirm_box_renderer(void *ot, short x, short y)
{
    char *g;
    int hpage;
    int t;
    int *pv;
    int step_val;
    int cur_val;
    int loop_i;
    int unused_slot;
    int a1v;
    int iInc;
    char *pa;
    u8 byte_val;

    g = kaze_state_base_ptr;
    t = *(int *)(g + 0x98) * 0x50;
    *(short *)(g + t + 0x49248) = x - 0x40;
    t = *(int *)(g + 0x98) * 0x50;
    *(short *)(g + t + 0x4924a) = y - 0x11;
    t = *(int *)(g + 0x98) * 0x50;
    *(short *)(g + t + 0x49220) = x - 0x40;
    t = *(int *)(g + 0x98) * 0x50;
    *(short *)(g + t + 0x49234) = x;
    hpage = *(int *)(g + 0x98);
    t = hpage * 0x50;
    *(short *)(g + t + 0x49236) = y;
    *(short *)(g + t + 0x49222) = y;
    loop_i = 0;
    do {
        __asm__("" : "=r"(loop_i) : "0"(loop_i));
        pv = (int *)(loop_i * 4 + (int)kaze_state_base_ptr + 0x492b0);
        step_val = *(int *)((int)kaze_state_base_ptr + 0xa4);
        cur_val = *pv;
        if (*(int *)((int)kaze_state_base_ptr + 0x492b8) == loop_i) {
            *pv = cur_val + step_val * 6;
        } else {
            *pv = cur_val - step_val * 6;
        }
        pv = (int *)(loop_i * 4 + (int)kaze_state_base_ptr + 0x492b0);
        step_val = *pv;
        if (step_val >= 0x40) {
            a1v = 0x80;
            if (step_val < 0x81) {
                a1v = step_val;
            }
        } else {
            a1v = 0x40;
        }
        iInc = loop_i + 1;
        *pv = a1v;
        byte_val = *(u8 *)(loop_i * 4 + (int)kaze_state_base_ptr + 0x492b0);
        pa = kaze_state_base_ptr + (iInc * 0x14 + *(int *)(kaze_state_base_ptr + 0x98) * 0x50);
        *(u8 *)(pa + 0x4920a) = byte_val;
        *(u8 *)(pa + 0x49209) = byte_val;
        *(u8 *)(pa + 0x49208) = byte_val;
        loop_i = iInc;
    } while (loop_i < 2);
    AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x50 + 0x49204) + 0x3c);
    AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x50 + 0x49204) + 0x14);
    AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x50 + 0x49204) + 0x28);
    kaze_panel_frame_vertices(
        (int)(kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x78 + 0x48fdc)),
        (int)(kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x60 + 0x490cc)),
        (int)(short)(x - 0x30), (int)(short)(y - 0x10), 0x60, 0x20);
    loop_i = 3;
    do {
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x60 + 0x490cc) + loop_i * 0x18);
        loop_i = loop_i - 1;
    } while (-1 < loop_i);
    loop_i = 4;
    do {
        AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 0x78 + 0x48fdc) + loop_i * 0x18);
        loop_i = loop_i - 1;
    } while (-1 < loop_i);
    AddPrim(ot, kaze_state_base_ptr + (*(int *)(kaze_state_base_ptr + 0x98) * 8 + 0x48e1c));
}

extern u16 GetClut(int, int);

/**
 * kaze_hud_two_digit_counter_284c  (hud, 0x8003284c)
 *
 * Renders a two-digit numeric readout (value/100 or /60 for time) as LINE_F2
 * digit primitives plus tile markers with GetClut; a format-mode argument
 * selects distance/altitude vs time formatting.
 */

void kaze_hud_two_digit_counter_284c(int panel, int x, int y, int mode, int value)
{
  u16 clut;
  int major;
  int divisor;
  int minor;

  register int minor_digit __asm__("$7");
  int cnt;

  register int q __asm__("$6");
  int t;
  int base;
   
  int vp3;
   
   
   ;
  vp3 = y;
  base = panel;
  switch (mode)
  {
    case 0:
    {
      u16 *pc0;
      int vw0;

      int magic0;
      int q10_0;
      int i710_0;
      union { long long ll; int w[2]; } uq0, ui0, um0;

      u16  p3c  ;
      int c40_0; 
       
       ;
       
      divisor = 10;
      minor = value / 100;
      major = minor;
      minor = value - (major * 100);
      if (major >= 0)
      {
        t = 99;
        if (major < 100)
        {
          t = major;
        }
      }
      else
      {
        t = 0;
      }
      major = t;
       ;
      cnt = 0;

      magic0 = 1717986919;
      __asm__("" : : "r"(magic0), "r"(minor)) ;

      do
      {
        vw0 = x  + (cnt * 0xc);
        pc0 = (u16 *) (base + ((cnt * 0x14) + 0xe));
__asm__("" : : "r"(cnt)  );
        q = major / divisor;
        minor_digit = minor / divisor;

        uq0.ll = (long long)q * (long long)magic0;
        q10_0 = (uq0.w[0] >> 2) - (q >> 31);
        ui0.ll = (long long)minor_digit * (long long)magic0;
        i710_0 = (ui0.w[0] >> 2) - (minor_digit >> 31);

        pc0[-3] = vw0 + 0x1e;
        pc0[0x1b] = vw0 + 0x42;
        __asm__("li %0,0x40" : "=r"(c40_0) : "m"(pc0[-3]), "m"(pc0[0x1b]) : "$8") ;
         ;
        __asm__("lhu %0,%1" : "=r,r"(  p3c  ) : "r,m"((u16)(    vp3  )), "m,m"(    pc0[-3]  ), "m,m"(    pc0[0x1b]  ), "m,m"(    *((u8 *) (((int) pc0) + 0x3b))  ), "m,m"(    *((u8 *) (((int) pc0) - 1))  ))  ;
         ;
         ;
        *((u8 *) (((int) pc0) + 0x3b)) = c40_0 ;
        *((u8 *) (((int) pc0) - 1)) = c40_0 ;
        pc0[0x1f] = 0xc ;
        pc0[1] = 0xc ;
        pc0[0x1c] = p3c;
        pc0[-2] = p3c;

        *((char *) (pc0 - 1)) = ((q + (q10_0 * (-10))) * 0xc) + 0xc;
        *((char *) (pc0 + 0x1d)) = ((minor_digit + (i710_0 * (-10))) * 0xc) + 0xc;

        clut = GetClut(0x170, 500);
        pc0[0x1e] = clut;
        *pc0 = clut;

        um0.ll = (long long)divisor * (long long)magic0;
        divisor = (um0.w[0] >> 2) - (divisor >> 31);

        cnt = cnt + 1;
      }
      while (cnt < 2);
       ;
      *((u16 *) (panel + 0x30)) = x  + 0x36;
      *((u16 *) (panel + 0x32)) = p3c;
      *((u8 *) (panel + 0x35)) = c40_0 ;
      *((s16 *) (panel + 0x38)) = 0xc ;
      *((u8 *) (panel + 0x34)) = 0x84;
      *((u16 *) (panel + 0x36)) = GetClut(0x170, 500);
      *((u16 *) (panel + 0x6c)) = x  + 0x5a;
      *((u8 *) (panel + 0x70)) = 0xe0;
      *((u8 *) (panel + 0x71)) = 0x60;
      *((u16 *) (panel + 0x6e)) = p3c;
      *((s16 *) (panel + 0x74)) = 0x18;
      __asm__("" : : "r"(p3c)) ;
      *((u16 *) (panel + 0x72)) = GetClut(0x210, 500);
      if ((*((char *) (panel + 0xc))) == 0xc)
      {
        *((u8 *) (panel + 0xc)) = 0;
      }
       ;
      break;
    }

    case 1:
    {
      int uv1;
      int uvnext;
      int magic;
      int q10;
      int i710;
      union { long long ll; int w[2]; } uq, ui, um;
      register int sp __asm__("$29"); 
      u16 *cur1;
      u16  p3c  ;
       
       ;
      divisor = 10;
      minor = value / 100;
      major = minor;
      minor = value - (100 * major);
      if (major >= 0)
      {
        t = 99;
        if (major < 100)
        {
          t = major;
        }
      }
      else
      {
        t = 0;
      }
      major = t;
       ;
      cnt = 0;
      magic = 1717986919;
      cur1 = (u16 *)(base + 14) ;
      __asm__("lw %0,16($sp)" : "=r"(uv1) : "r"(cur1)) ;
      do
      {
         ;
__asm__("" : : "r"(cnt)  );
        q = major / divisor;
        minor_digit = minor / divisor;
        uq.ll = (long long)q * (long long)magic;
        q10 = (uq.w[0] >> 2) - (q >> 31);
        ui.ll = (long long)minor_digit * (long long)magic;
        i710 = (ui.w[0] >> 2) - (minor_digit >> 31);
        __asm__("addiu %0,%1,12" : "=r"(uvnext) : "r"(uv1)) ;
        cur1[-3] = uvnext;
        cur1[0x1b] = uv1 + 0x3c;
         ;
        __asm__("lhu %0,%1" : "=r,r"(  p3c  ) : "r,m"((u16)(    vp3  )), "m,m"(    cur1[-3]  ), "m,m"(    cur1[0x1b]  ), "m,m"(    *(u8 *)(((int) cur1) + 0x3b)  ), "m,m"(    *(u8 *)(((int) cur1) - 1)  ))  ;
         ;
        *((u8 *) (((int) cur1) + 0x3b)) = 0x40 ;
        *((u8 *) (((int) cur1) - 1)) = 0x40 ;
        cur1[0x1f] = 0xc;
        cur1[1] = 0xc;
        cur1[0x1c] = p3c;
        cur1[-2] = p3c;
        *((char *) (cur1 - 1)) = ((q + (q10 * (-10))) * 0xc) + 0xc;
        *((char *) (cur1 + 0x1d)) = ((minor_digit + (i710 * (-10))) * 0xc) + 0xc;
        clut = GetClut(0x170, 500);
        __asm__("move %0,%1" : "=r"(uv1) : "r"(uvnext)) ;
        cur1[0x1e] = clut;
        *cur1 = clut;
        um.ll = (long long)divisor * (long long)magic;
        divisor = (um.w[0] >> 2) - (divisor >> 31);
        cnt = cnt + 1;
         ;
        __asm__("" : "=r"(cur1) : "0"(cur1)); cur1 = (u16 *)((int)cur1 + 20) ;
      }
      while (cnt < 2);
      *((u16 *) (panel + 0x30)) = x  + 0x24;
      *((u16 *) (panel + 0x32)) = p3c;
      *((u8 *) (panel + 0x35)) = 0x60;
      *((s16 *) (panel + 0x38)) = 0x18;
      *((u8 *) (panel + 0x34)) = 0xb0;
      *((u16 *) (panel + 0x36)) = GetClut(0x210, 500);
      *((u16 *) (panel + 0x6c)) = x  + 0x54;
      *((u16 *) (panel + 0x6e)) = p3c;
      *((u8 *) (panel + 0x70)) = 0x98;
      *((u8 *) (panel + 0x71)) = 0x60;
      *((s16 *) (panel + 0x74)) = 0x18;
      *((u16 *) (panel + 0x72)) = GetClut(0x210, 500);
      if (((*((char *) (panel + 0xc))) == 0xc) && ((*((u8 *) (panel + 0xc)) = 0, (*((char *) (panel + 0x20))) == 0xc)))
      {
        *((u8 *) (panel + 0x20)) = 0;
        *((u8 *) (panel + 0x34)) = 0x80;
      }
      if ((*((char *) (panel + 0x48))) == 0xc)
      {
        *((u8 *) (panel + 0x48)) = 0;
      }
       ;
      break;
    }

    case 2:
    {
      u16 *pc2;
      int vw2;

      int magic2;
      int q10_2;
      int i710_2;
      union { long long ll; int w[2]; } uq2, ui2, um2;

      u16  p3c  ;
      int c40_2; 
       
       ;
       
      divisor = 10;
      minor = value / 0x3c;
      major = minor;
      minor = value - (major * 0x3c);
      if (major >= 0)
      {
        t = 99;
        if (major < 100)
        {
          t = major;
        }
      }
      else
      {
        t = 0;
      }
      major = t;
       ;
      cnt = 0;

      magic2 = 1717986919;
      __asm__("" : : "r"(magic2)) ;

      do
      {
        vw2 = x  + (cnt * 0xc);
        pc2 = (u16 *) (base + ((cnt * 0x14) + 0xe));
__asm__("" : : "r"(cnt)  );
        q = major / divisor;
        minor_digit = minor / divisor;

        uq2.ll = (long long)q * (long long)magic2;
        q10_2 = (uq2.w[0] >> 2) - (q >> 31);
        ui2.ll = (long long)minor_digit * (long long)magic2;
        i710_2 = (ui2.w[0] >> 2) - (minor_digit >> 31);

        pc2[-3] = vw2 + 0x1e;
        pc2[0x1b] = vw2 + 0x42;
        __asm__("li %0,0x40" : "=r"(c40_2) : "m"(pc2[-3]), "m"(pc2[0x1b]) : "$8") ;
         ;
        __asm__("lhu %0,%1" : "=r,r"(  p3c  ) : "r,m"((u16)(    vp3  )), "m,m"(    pc2[-3]  ), "m,m"(    pc2[0x1b]  ), "m,m"(    *((u8 *) (((int) pc2) + 0x3b))  ), "m,m"(    *((u8 *) (((int) pc2) - 1))  ))  ;
         ;
         ;
        *((u8 *) (((int) pc2) + 0x3b)) = c40_2 ;
        *((u8 *) (((int) pc2) - 1)) = c40_2 ;
        pc2[0x1f] = 0xc ;
        pc2[1] = 0xc ;
        pc2[0x1c] = p3c;
        pc2[-2] = p3c;

        *((char *) (pc2 - 1)) = ((q + (q10_2 * (-10))) * 0xc) + 0xc;
        *((char *) (pc2 + 0x1d)) = ((minor_digit + (i710_2 * (-10))) * 0xc) + 0xc;

        clut = GetClut(0x170, 500);
        pc2[0x1e] = clut;
        *pc2 = clut;

        um2.ll = (long long)divisor * (long long)magic2;
        divisor = (um2.w[0] >> 2) - (divisor >> 31);

        cnt = cnt + 1;
      }
      while (cnt < 2);
       ;
      *((u16 *) (panel + 0x30)) = x  + 0x36;
      *((u16 *) (panel + 0x6c)) = x  + 0x5a;
      *((u8 *) (panel + 0x34)) = 0x9c;
      *((u8 *) (panel + 0x70)) = 0x90;
      *((u16 *) (panel + 0x6e)) = p3c;
      *((u16 *) (panel + 0x32)) = p3c;
      *((u8 *) (panel + 0x71)) = c40_2 ;  ; *((u8 *) (panel + 0x35)) = c40_2  ;
      *((s16 *) (panel + 0x74)) = 0xc ;
      *((s16 *) (panel + 0x38)) = 0xc ;
       ;
       ;
      clut = GetClut(0x170, 500);
      *((u16 *) (panel + 0x72)) = clut;
      *((u16 *) (panel + 0x36)) = clut;
       ;
      break;
    }

  }

}

/**
 * kaze_postevent_banner_addprim_32f98  (render, 0x80032f98)
 *
 * AddPrims the post-event banner/overlay primitives at live-root
 * +0x4a128/+0x4a158/+0x4a180 for the current page (+0x98), tinting the
 * +0x4a128 fill by the fade value +0xed428.
 */

void kaze_postevent_banner_addprim_32f98(void *arg)
{
    char *base;
    int page;
    int alpha;
    int poly_ofs;
    char *poly;
    u8 fade;

    base = kaze_state_base_ptr;
    alpha = *(int *)(base + 0xED428);
    if (alpha > 0) {
        page = *(int *)(base + 0x98);
        poly_ofs = ((page << 1) + page) << 3;
        poly = base + poly_ofs;
        fade = alpha;
        *(u8 *)(poly + 0x4A12E) = fade;
        *(u8 *)(poly + 0x4A12D) = fade;
        *(u8 *)(poly + 0x4A12C) = fade;
        AddPrim(arg, kaze_state_base_ptr + ((((*(int *)(kaze_state_base_ptr + 0x98) << 1) + *(int *)(kaze_state_base_ptr + 0x98)) << 3) + 0x4A128));
    }

    AddPrim(arg, kaze_state_base_ptr + (((*(int *)(kaze_state_base_ptr + 0x98) * 5) << 2) + 0x4A158));
    AddPrim(arg, kaze_state_base_ptr + ((*(int *)(kaze_state_base_ptr + 0x98) << 3) + 0x4A180));
}

extern char *kaze_sound_state_base_ptr;
extern void *kaze_heap_alloc(int, int);
extern void SsSetSerialVol(char, int, int);
extern void kaze_xa_stream_enable_33b0(void);

/**
 * kaze_sound_system_init  (sound, 0x800330a0)
 *
 * Sound subsystem init: allocates the 0x14b20 sound work area into
 * kaze_sound_state_base_ptr, configures serial/tick/reverb (SpuInitHot,
 * SsUtSetReverb), zeroes CD-audio state, CdMix; calls
 * kaze_xa_stream_enable_33b0/kaze_xa_file_table_init_331c.
 */

void kaze_sound_system_init(void)
{
    register char *p asm("$2");
    register char *q asm("$3");
    char *r;
    int i;

    kaze_sound_state_base_ptr = (char *)kaze_heap_alloc(1, 0x14b20);
    if (kaze_sound_state_base_ptr == 0) {
        exit(0x2bd);
    }
    kaze_ss_init();
    SsSetSerialAttr(0, 0, 1);
    SsSetSerialVol(0, 0x7f, 0x7f);
    SsSetTableSize(kaze_sound_state_base_ptr, 1, 0x10);
    SsSetTickMode(2);
    SpuInitHot();
    SsSetMVol(0x40, 0x40);
    SsUtSetReverbType(4);
    kaze_ss_ut_reverb_on();
    SsUtSetReverbDepth(0x10, 0x10);

    p = kaze_sound_state_base_ptr;
    p[0x14afa] = -0x80;
    p[0x14af8] = -0x80;
    q = kaze_sound_state_base_ptr;
    *(int *)(p + 0x14aec) = 0;
    *(int *)(p + 0x14b1c) = 0;
    *(int *)(p + 0x14b0c) = 0;
    *(int *)(p + 0x14b14) = 0;
    *(int *)(p + 0x14b10) = 0;
    *(int *)(p + 0x14b08) = 0;
    *(int *)(p + 0x14b04) = 0;
    *(int *)(p + 0x14b00) = 0;
    q[0x14afb] = 0;
    q[0x14af9] = 0;
    CdMix((void *)(kaze_sound_state_base_ptr + 0x14af8));

    i = 0;
    kaze_sound_state_base_ptr[0x14af0] = 1;
    kaze_sound_state_base_ptr[0x14af1] = -1;
    r = kaze_sound_state_base_ptr;
    *(short *)(r + 0x14ac4) = -1;
    *(short *)(r + 0x14ac6) = -1;
    *(int *)(r + 0x14ac0) = -1;
    *(int *)(r + 0x14b18) = -1;
    *(int *)(r + 0x14ac8) = 0;
    do {
        *(short *)(r + i * 2 + 0x14acc) = 0;
        i++;
    } while (i < 0x10);
    kaze_xa_stream_enable_33b0();
    kaze_xa_file_table_init_331c();
}
