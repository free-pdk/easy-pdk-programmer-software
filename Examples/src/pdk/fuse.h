#ifndef __PDK_FUSE_H__
#define __PDK_FUSE_H__

#if !defined(__PDK_IO_H__)
#  error "You must #include "pdk/io.h" instead of "pdk/fuse.h" by itself."
#endif

// FUSE macros
#if defined(FUSE_WORD_ADDR) && defined(FUSE_RES_BITS_HIGH)
  #define PDK_SET_FUSE(f)  { __asm__(".area FUSE (ABS)\n.org ("_ASMD(FUSE_WORD_ADDR)"*2)\n.word ("_ASMD(FUSE_RES_BITS_HIGH)"|"_ASMD(f)")\n.area CODE\n"); }
#endif

#endif //__PDK_FUSE_H__
