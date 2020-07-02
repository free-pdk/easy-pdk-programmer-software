#ifndef __PDK_DEVICE_PERIPH_ROP_H__
#define __PDK_DEVICE_PERIPH_ROP_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/rop.h" by itself."
#endif

// __sfr definitions
__sfr __at(ROP_ADDR)          _rop;

#define ROP                   _rop

// ROP register definitions

#if defined(ROP_PB0_PA4_SEL_BIT)
  #define ROP_INT_SRC_PB0              0x00
  #define ROP_INT_SRC_PA4              (1 << ROP_PB0_PA4_SEL_BIT)
#endif

#if defined(ROP_PA0_PB5_SEL_BIT)
  #define ROP_INT_SRC_PA0              0x00
  #define ROP_INT_SRC_PB5              (1 << ROP_PA0_PB5_SEL_BIT)
#endif

#if defined(ROP_TMX_BIT_SEL_BIT)
  #define ROP_TMX_6BIT                 0x00
  #define ROP_TMX_7BIT                 (1 << ROP_TMX_BIT_SEL_BIT)
#endif

#if defined(ROP_TMX_FREQ_SEL_BIT)
  #define ROP_TMX_16MHZ                0x00
  #define ROP_TMX_32MHZ                (1 << ROP_TMX_FREQ_SEL_BIT)
#endif

#if defined(ROP_PWM_SEL_BIT)
  #define ROP_PURE_PWM                 0x00
  #define ROP_GPC_PWM                  (1 << ROP_PWM_SEL_BIT)
#endif

#if defined(ROP_PWM_FREQ_SEL_BIT)
  #define ROP_PWM_16MHZ                0x00
  #define ROP_PWM_32MHZ                (1 << ROP_PWM_FREQ_SEL_BIT)
#endif

#if defined(ROP_TM2_PB2_PB0_SEL_BIT)
  #define ROP_TM2_PB2                  0x00
  #define ROP_TM2_PB0                  (1 << ROP_TM2_PB2_PB0_SEL_BIT)
#endif

#endif //__PDK_DEVICE_PERIPH_ROP_H__
