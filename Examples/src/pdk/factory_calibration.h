#ifndef __PDK_FACTORY_CALIBRATION_H__
#define __PDK_FACTORY_CALIBRATION_H__

#if !defined(__PDK_IO_H__)
#  error "You must #include "pdk/io.h" instead of "pdk/factory_calibration.h" by itself."
#endif

// Factory Calibration macros
#if defined(FACTORY_IHRCR_ADDR)
  #define PDK_USE_FACTORY_IHRCR_16MHZ() { __asm__("call #("_STR_DATA(FACTORY_IHRCR_ADDR)")\n mov "_STR_VAR(IHRCR)",a\n"); }
#endif
#if defined(FACTORY_BGTR_ADDR)
  #define PDK_USE_FACTORY_BGTR() { __asm__("call #("_STR_DATA(FACTORY_BGTR_ADDR)")\n mov "_STR_VAR(BGTR)",a\n"); }
#endif

#endif //__PDK_FACTORY_CALIBRATION_H__
