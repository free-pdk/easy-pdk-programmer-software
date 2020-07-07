#ifndef __PDK_DEVICE_PERIPH_TIMER_2_H__
#define __PDK_DEVICE_PERIPH_TIMER_2_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/timer_2.h" by itself."
#endif

// __sfr definitions
__sfr __at(TM2C_ADDR)        _tm2c;
__sfr __at(TM2CT_ADDR)       _tm2ct;
__sfr __at(TM2S_ADDR)        _tm2s;
__sfr __at(TM2B_ADDR)        _tm2b;

#define TM2C                 _tm2c
#define TM2CT                _tm2ct
#define TM2S                 _tm2s
#define TM2B                 _tm2b

// TM2C (Timer2 Control) register definitions
#define TM2C_INVERT_OUT_BIT          0
#define TM2C_MODE_SEL_BIT            1
#define TM2C_OUTPUT_SEL_BIT0         2
#define TM2C_CLK_SRC_BIT0            4

#define TM2C_INVERT_OUT              (1 << TM2C_INVERT_OUT_BIT)

#define TM2C_MODE_PERIOD             0x00
#define TM2C_MODE_PWM                (1 << TM2C_MODE_SEL_BIT)

#define TM2C_OUT_DISABLE             0x00
#if defined(__PDK_HAS_PORTB)
  #define TM2C_OUT_PB2                 (1 << TM2C_OUTPUT_SEL_BIT0)
#endif
#define TM2C_OUT_PA3                 (2 << TM2C_OUTPUT_SEL_BIT0)
#if defined(__PDK_HAS_PORTB)
  #define TM2C_OUT_PB4                 (3 << TM2C_OUTPUT_SEL_BIT0)
#else
  #define TM2C_OUT_PA4                 (3 << TM2C_OUTPUT_SEL_BIT0)
#endif

#define TM2C_CLK_DISABLE             0x00
#define TM2C_CLK_SYSCLK              (1 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_IHRC                (2 << TM2C_CLK_SRC_BIT0)
#if defined(__PDK_HAS_EOSC)
  #define TM2C_CLK_EOSC                (3 << TM2C_CLK_SRC_BIT0)
#endif
#define TM2C_CLK_ILRC                (4 << TM2C_CLK_SRC_BIT0)
#if defined(__PDK_HAS_COMP)
  #define TM2C_CLK_COMPOUT             (5 << TM2C_CLK_SRC_BIT0)
#endif
//0x06 reserved
//0x07 reserved
#define TM2C_CLK_PA0_RISE            (8 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_PA0_FALL            (9 << TM2C_CLK_SRC_BIT0)
#if defined(__PDK_HAS_PORTB)
  #define TM2C_CLK_PB0_RISE            (10 << TM2C_CLK_SRC_BIT0)
  #define TM2C_CLK_PB0_FALL            (11 << TM2C_CLK_SRC_BIT0)
#endif
#define TM2C_CLK_PA4_RISE            (12 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_PA4_FALL            (13 << TM2C_CLK_SRC_BIT0)

// TM2S (Timer2 Scalar) register definitions
#define TM2S_SCALE_BIT0              0
#define TM2S_PRESCALE_BIT0           5
#define TM2S_PWM_RES_SEL_BIT         7

#define TM2S_SCALE_NONE              0x00
#define TM2S_SCALE_DIV2              (1 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV3              (2 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV4              (3 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV5              (4 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV6              (5 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV7              (6 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV8              (7 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV9              (8 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV10             (9 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV11             (10 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV12             (11 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV13             (12 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV14             (13 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV15             (14 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV16             (15 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV17             (16 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV18             (17 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV19             (18 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV20             (19 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV21             (20 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV22             (21 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV23             (22 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV24             (23 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV25             (24 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV26             (25 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV27             (26 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV28             (27 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV29             (28 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV30             (29 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV31             (30 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV32             (31 << TM2S_SCALE_BIT0)

#define TM2S_PRESCALE_NONE           0x00
#define TM2S_PRESCALE_DIV4           (1 << TM2S_PRESCALE_BIT0)
#define TM2S_PRESCALE_DIV16          (2 << TM2S_PRESCALE_BIT0)
#define TM2S_PRESCALE_DIV64          (3 << TM2S_PRESCALE_BIT0)

#define TM2S_PWM_RES_8BIT            0x00
#define TM2S_PWM_RES_6BIT            (1 << TM2S_PWM_RES_SEL_BIT)

#endif //__PDK_DEVICE_PERIPH_TIMER_2_H__
