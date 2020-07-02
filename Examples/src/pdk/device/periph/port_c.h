#ifndef __PDK_DEVICE_PERIPH_PORT_C_H__
#define __PDK_DEVICE_PERIPH_PORT_C_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/port_c.h" by itself."
#endif

// __sfr definitions
__sfr __at(PCDIER_ADDR)      _pcdier;
__sfr __at(PC_ADDR)          _pc;
__sfr __at(PCC_ADDR)         _pcc;
__sfr __at(PCPH_ADDR)        _pcph;

#define PCDIER               _pcdier
#define PC                   _pc
#define PCC                  _pcc
#define PCPH                 _pcph

#if defined(PCPL_ADDR)
  __sfr __at(PCPL_ADDR)        _pcpl;
  #define PCPL                 _pcpl
#endif

#endif //__PDK_DEVICE_PERIPH_PORT_C_H__
