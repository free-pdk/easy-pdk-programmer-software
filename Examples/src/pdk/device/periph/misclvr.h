#ifndef __PDK_DEVICE_PERIPH_MISCLVR_H__
#define __PDK_DEVICE_PERIPH_MISCLVR_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/misclvr.h" by itself."
#endif

// __sfr definitions
__sfr __at(MISCLVR_ADDR)      _misclvr;

#define MISCLVR               _misclvr

// MISCLVR register definitions
#define MISCLVR_BANDGAP_DIV_BIT0     0
#define MISCLVR_LVR_BIT0             4

#define MISCLVR_BANDGAP_ON           0x00
#define MISCLVR_BANDGAP_DIV4         (1 << MISCLVR_BANDGAP_DIV_BIT0)
#define MISCLVR_BANDGAP_DIV32        (2 << MISCLVR_BANDGAP_DIV_BIT0)
#define MISCLVR_BANDGAP_AUTO         (3 << MISCLVR_BANDGAP_DIV_BIT0)

#define MISCLVR_1V8                  0x00
#define MISCLVR_1V9                  (1 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V                   (2 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V1                  (3 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V2                  (4 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V3                  (5 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V4                  (6 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V5                  (7 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V7                  (8 << MISCLVR_LVR_BIT0)
#define MISCLVR_3V                   (9 << MISCLVR_LVR_BIT0)
#define MISCLVR_3V15                 (10 << MISCLVR_LVR_BIT0)
#define MISCLVR_3V3                  (11 << MISCLVR_LVR_BIT0)
#define MISCLVR_3V5                  (12 << MISCLVR_LVR_BIT0)
#define MISCLVR_3V75                 (13 << MISCLVR_LVR_BIT0)
#define MISCLVR_4V                   (14 << MISCLVR_LVR_BIT0)
#define MISCLVR_4V5                  (15 << MISCLVR_LVR_BIT0)

#endif //__PDK_DEVICE_PERIPH_MISCLVR_H__
