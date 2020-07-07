#ifndef __PDK_DEVICE_PERIPH_MISC_H__
#define __PDK_DEVICE_PERIPH_MISC_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/misc.h" by itself."
#endif

// __sfr definitions
__sfr __at(MISC_ADDR)         _misc;

#define MISC                  _misc

// MISC register definitions
#define MISC_WATCHDOG_BIT0           0
#define MISC_LVR_DISABLE_BIT         2
#define MISC_FAST_WAKEUP_ENABLE_BIT  5

#define MISC_WATCHDOG_8K_ILRC        0x00
#define MISC_WATCHDOG_16K_ILRC       (1 << MISC_WATCHDOG_BIT0)
#define MISC_WATCHDOG_64K_ILRC       (2 << MISC_WATCHDOG_BIT0)
#define MISC_WATCHDOG_256K_ILRC      (3 << MISC_WATCHDOG_BIT0)

#define MISC_LVR_DISABLE             (1 << MISC_LVR_DISABLE_BIT)
#define MISC_FAST_WAKEUP_ENABLE      (1 << MISC_FAST_WAKEUP_ENABLE_BIT)

#if defined(__PDK_HAS_LCD)
  #define MISC_LCD_ENABLE_BIT          4
  #define MISC_LCD_ENABLE              (1 << MISC_LCD_ENABLE_BIT)
#endif

#endif //__PDK_DEVICE_PERIPH_MISC_H__
