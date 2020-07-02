#ifndef __PDK_DEVICE_PERIPH_PORT_B_H__
#define __PDK_DEVICE_PERIPH_PORT_B_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/port_b.h" by itself."
#endif

// __sfr definitions
__sfr __at(PBDIER_ADDR)      _pbdier;
__sfr __at(PB_ADDR)          _pb;
__sfr __at(PBC_ADDR)         _pbc;
__sfr __at(PBPH_ADDR)        _pbph;

#define PBDIER               _pbdier
#define PB                   _pb
#define PBC                  _pbc
#define PBPH                 _pbph

#if defined(PBPL_ADDR)
  __sfr __at(PBPL_ADDR)        _pbpl;
  #define PBPL                 _pbpl
#endif

#endif //__PDK_DEVICE_PERIPH_PORT_B_H__
