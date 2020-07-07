#ifndef __PDK_DEVICE_PERIPH_PWMG_BASIC_H__
#define __PDK_DEVICE_PERIPH_PWMG_BASIC_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/pwmg_basic.h" by itself."
#endif

// __sfr definitions
__sfr __at(PWMGCLK_ADDR)      _pwmgclk;
__sfr __at(PWMGCUBH_ADDR)     _pwmgcubh;
__sfr __at(PWMGCUBL_ADDR)     _pwmgcubl;

__sfr __at(PWMG0C_ADDR)       _pwmg0c;
__sfr __at(PWMG0DTH_ADDR)     _pwmg0dth;
__sfr __at(PWMG0DTL_ADDR)     _pwmg0dtl;

__sfr __at(PWMG1C_ADDR)       _pwmg1c;
__sfr __at(PWMG1DTH_ADDR)     _pwmg1dth;
__sfr __at(PWMG1DTL_ADDR)     _pwmg1dtl;

__sfr __at(PWMG2C_ADDR)       _pwmg2c;
__sfr __at(PWMG2DTH_ADDR)     _pwmg2dth;
__sfr __at(PWMG2DTL_ADDR)     _pwmg2dtl;

#define PWMGCLK               _pwmgclk
#define PWMGCUBH              _pwmgcubh
#define PWMGCUBL              _pwmgcubl

#define PWMG0C                _pwmg0c
#define PWMG0DTH              _pwmg0dth
#define PWMG0DTL              _pwmg0dtl

#define PWMG1C                _pwmg1c
#define PWMG1DTH              _pwmg1dth
#define PWMG1DTL              _pwmg1dtl

#define PWMG2C                _pwmg2c
#define PWMG2DTH              _pwmg2dth
#define PWMG2DTL              _pwmg2dtl

// PWMGCLK (PWMG Clock) register definitions
#define PWMGCLK_CLK_SRC_BIT          0
#define PWMGCLK_PRESCALE_BIT0        4
#define PWMGCLK_ENABLE_BIT           7

#define PWMGCLK_CLK_SYSCLK           0x00
#define PWMGCLK_CLK_IHRC             (1 << PWMGCLK_CLK_SRC_BIT)

#define PWMGCLK_PRESCALE_NONE        0x00
#define PWMGCLK_PRESCALE_DIV2        (1 << PWMGCLK_PRESCALE_BIT0)
#define PWMGCLK_PRESCALE_DIV4        (2 << PWMGCLK_PRESCALE_BIT0)
#define PWMGCLK_PRESCALE_DIV8        (3 << PWMGCLK_PRESCALE_BIT0)
#define PWMGCLK_PRESCALE_DIV16       (4 << PWMGCLK_PRESCALE_BIT0)
#define PWMGCLK_PRESCALE_DIV32       (5 << PWMGCLK_PRESCALE_BIT0)
#define PWMGCLK_PRESCALE_DIV64       (6 << PWMGCLK_PRESCALE_BIT0)
#define PWMGCLK_PRESCALE_DIV128      (7 << PWMGCLK_PRESCALE_BIT0)

#define PWMGCLK_PWMG_ENABLE          (1 << PWMGCLK_ENABLE_BIT)

// PWMG0C (PWMG0 Control) register definitions
#define PWMG0C_X_OR_BIT              0
#define PWMG0C_OUT_PIN_SEL_BIT0      1
#define PWMG0C_X_OR_ENABLE_BIT       4
#define PWMG0C_INVERT_OUT_BIT        5
#define PWMG0C_STATUS_OUT_BIT        6

#define PWMG0C_GEN_XOR               0x00
#define PWMG0C_GEN_OR                (1 << PWMG0C_X_OR_BIT)

#define PWMG0C_OUT_NONE              0x00
#define PWMG0C_OUT_PB5               (1 << PWMG0C_OUT_PIN_SEL_BIT0)
#if defined(__PDK_HAS_PORTC)
  #define PWMG0C_OUT_PC2               (2 << PWMG0C_OUT_PIN_SEL_BIT0)
#endif
#define PWMG0C_OUT_PA0               (3 << PWMG0C_OUT_PIN_SEL_BIT0)
#define PWMG0C_OUT_PB4               (4 << PWMG0C_OUT_PIN_SEL_BIT0)
#define PWMG0C_OUT_PB6               (5 << PWMG0C_OUT_PIN_SEL_BIT0)
//0x06 reserved
//0x07 reserved

#define PWMG0C_OUT_PWMG0             0x00
#define PWMG0C_OUT_PWMG0_X_OR_PWMG1  (1 << PWMG0C_X_OR_ENABLE_BIT)

#define PWMG0C_INVERT_OUT            (1 << PWMG0C_INVERT_OUT_BIT)
#define PWMG0C_STATUS_OUT            (1 << PWMG0C_STATUS_OUT_BIT)

// PWMG1C (PWMG1 Control) register definitions
#define PWMG1C_OUT_PIN_SEL_BIT0      1
#define PWMG1C_PWMG1_2_SEL_BIT       4
#define PWMG1C_INVERT_OUT_BIT        5
#define PWMG1C_STATUS_OUT_BIT        6

#define PWMG1C_OUT_NONE              0x00
#define PWMG1C_OUT_PB6               (1 << PWMG1C_OUT_PIN_SEL_BIT0)
#if defined(__PDK_HAS_PORTC)
  #define PWMG1C_OUT_PC3               (2 << PWMG1C_OUT_PIN_SEL_BIT0)
#endif
#define PWMG1C_OUT_PA4               (3 << PWMG1C_OUT_PIN_SEL_BIT0)
#define PWMG1C_OUT_PB7               (4 << PWMG1C_OUT_PIN_SEL_BIT0)
//0x05 reserved
//0x06 reserved
//0x07 reserved

#define PWMG1C_OUT_PWMG1             0x00
#define PWMG1C_OUT_PWMG2             (1 << PWMG1C_PWMG1_2_SEL_BIT)

#define PWMG1C_INVERT_OUT            (1 << PWMG1C_INVERT_OUT_BIT)
#define PWMG1C_STATUS_OUT            (1 << PWMG1C_STATUS_OUT_BIT)

// PWMG2C (PWMG2 Control) register definitions
#define PWMG2C_OUT_PIN_SEL_BIT0      1
#define PWMG2C_PWMG2_DIV2_SEL_BIT    4
#define PWMG2C_INVERT_OUT_BIT        5
#define PWMG2C_STATUS_OUT_BIT        6

#define PWMG2C_GEN_XOR               0x00
#define PWMG2C_GEN_OR                (1 << PWMG2C_X_OR_BIT)

#define PWMG2C_OUT_NONE              0x00
#define PWMG2C_OUT_PB3               (1 << PWMG2C_OUT_PIN_SEL_BIT0)
#if defined(__PDK_HAS_PORTC)
  #define PWMG2C_OUT_PC0               (2 << PWMG2C_OUT_PIN_SEL_BIT0)
#endif
#define PWMG2C_OUT_PA3               (3 << PWMG2C_OUT_PIN_SEL_BIT0)
#define PWMG2C_OUT_PB2               (4 << PWMG2C_OUT_PIN_SEL_BIT0)
#define PWMG2C_OUT_PA5               (5 << PWMG2C_OUT_PIN_SEL_BIT0)
#define PWMG2C_OUT_PB5               (6 << PWMG2C_OUT_PIN_SEL_BIT0)
//0x07 reserved

#define PWMG2C_OUT_PWMG2             0x00
#define PWMG2C_OUT_PWMG2_DIV2        (1 << PWMG2C_PWMG2_DIV2_SEL_BIT)

#define PWMG2C_INVERT_OUT            (1 << PWMG2C_INVERT_OUT_BIT)
#define PWMG2C_STATUS_OUT            (1 << PWMG2C_STATUS_OUT_BIT)

#endif //__PDK_DEVICE_PERIPH_PWMG_BASIC_H__
