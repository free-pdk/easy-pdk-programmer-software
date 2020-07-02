#ifndef __PDK_DEVICE_PERIPH_MISCLVR_BASIC_H__
#define __PDK_DEVICE_PERIPH_MISCLVR_BASIC_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/misclvr_basic.h" by itself."
#endif

// __sfr definitions
__sfr __at(MISCLVR_ADDR)      _misclvr;

#define MISCLVR               _misclvr

// MISCLVR register definitions
#define MISCLVR_LVR_BIT0             5

#define MISCLVR_4V                   0x00
#define MISCLVR_3V5                  (1 << MISCLVR_LVR_BIT0)
#define MISCLVR_3V                   (2 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V75                 (3 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V5                  (4 << MISCLVR_LVR_BIT0)
#define MISCLVR_1V8                  (5 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V2                  (6 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V                   (7 << MISCLVR_LVR_BIT0)

#endif //__PDK_DEVICE_PERIPH_MISCLVR_BASIC_H__
