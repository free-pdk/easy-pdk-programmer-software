#ifndef __PDK_DEVICE_PERIPH_PORT_A_H__
#define __PDK_DEVICE_PERIPH_PORT_A_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/port_a.h" by itself."
#endif

// __sfr definitions
__sfr __at(PADIER_ADDR)      _padier;
__sfr __at(PA_ADDR)          _pa;
__sfr __at(PAC_ADDR)         _pac;
__sfr __at(PAPH_ADDR)        _paph;

#define PADIER               _padier
#define PA                   _pa
#define PAC                  _pac
#define PAPH                 _paph

// PIN definitions (also useful for PORTB/PORTC)
#define PIN0	               (1)
#define PIN1	               (1<<1)
#define PIN2	               (1<<2)
#define PIN3	               (1<<3)
#define PIN4	               (1<<4)
#define PIN5	               (1<<5)
#define PIN6	               (1<<6)
#define PIN7	               (1<<7)

#endif //__PDK_DEVICE_PERIPH_PORT_A_H__
