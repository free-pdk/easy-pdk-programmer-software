#ifndef __PDK_DEVICE_PERIPH_MULTIPLIER_H__
#define __PDK_DEVICE_PERIPH_MULTIPLIER_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/multiplier.h" by itself."
#endif

// __sfr definitions
__sfr __at(MULOP_ADDR)        _mulop;
__sfr __at(MULRH_ADDR)        _mulrh;

#define MULOP                 _mulop
#define MULRH                 _mulrh

#endif //__PDK_DEVICE_PERIPH_MULTIPLIER_H__
