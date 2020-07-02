#ifndef __PDK_DEVICE_PERIPH_BANDGAP_H__
#define __PDK_DEVICE_PERIPH_BANDGAP_H__

#if !defined(__PDK_IO_H__)
#  error "You must #include "pdk/io.h" instead of "pdk/device/periph/bandgap.h" by itself."
#endif

// __sfr definitions
__sfr __at(BGTR_ADDR)         _bgtr;

#define BGTR                  _bgtr

#endif //__PDK_DEVICE_PERIPH_BANDGAP_H__
