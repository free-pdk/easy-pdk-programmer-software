#ifndef __PDK_FACTORY_CALIBRATION_H__
#define __PDK_FACTORY_CALIBRATION_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/factory_calibration.h" by itself."
#endif

typedef unsigned char (*getfactorycalibration_funcptr)(void);

// Factory Calibration macros
#if defined(FACTORY_IHRCR_ADDR)
  #define GET_FACTORY_IHRCR ((getfactorycalibration_funcptr)FACTORY_IHRCR_ADDR)
  #define PDK_USE_FACTORY_IHRCR_16MHZ() \
    IHRCR = GET_FACTORY_IHRCR()
#endif

#if defined(FACTORY_BGTR_ADDR)
  #define GET_FACTORY_BGTR ((getfactorycalibration_funcptr)FACTORY_BGTR_ADDR)
  #define PDK_USE_FACTORY_BGTR() \
    BGTR = GET_FACTORY_BGTR()
#endif

#endif //__PDK_FACTORY_CALIBRATION_H__
