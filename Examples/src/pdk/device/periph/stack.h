#ifndef __PDK_DEVICE_PERIPH_STACK_H__
#define __PDK_DEVICE_PERIPH_STACK_H__

#if !defined(__PDK_IO_H__)
#  error "You must #include "pdk/io.h" instead of "pdk/device/periph/stack.h" by itself."
#endif

// __sfr definitions
__sfr __at(SP_ADDR)           _sp;

#define SP                    _sp

#endif //__PDK_DEVICE_PERIPH_STACK_H__
