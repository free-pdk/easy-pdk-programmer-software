#ifndef __PDK_DEVICE_PERIPH_ACCUMULATOR_H__
#define __PDK_DEVICE_PERIPH_ACCUMULATOR_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/accumulator.h" by itself."
#endif

// __sfr definitions
__sfr __at(FLAG_ADDR)         _flag;

#define FLAG                  _flag

// FLAG (ACC Status Flag) register definitions
#define FLAG_Z_BIT                   0
#define FLAG_C_BIT                   1
#define FLAG_AC_BIT                  2
#define FLAG_OV_BIT                  3

#define FLAG_Z                       (1 << FLAG_Z_BIT)
#define FLAG_C                       (1 << FLAG_C_BIT)
#define FLAG_AC                      (1 << FLAG_AC_BIT)
#define FLAG_OV                      (1 << FLAG_OV_BIT)

#endif //__PDK_DEVICE_PERIPH_ACCUMULATOR_H__
