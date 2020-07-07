#ifndef __PDK_DEVICE_PERIPH_INTERRUPT_H__
#define __PDK_DEVICE_PERIPH_INTERRUPT_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/deviceo.h" instead of "pdk/device/periph/interrupt.h" by itself."
#endif

// __sfr definitions
__sfr __at(INTEN_ADDR)        _inten;
__sfr __at(INTRQ_ADDR)        _intrq;
__sfr __at(INTEGS_ADDR)       _integs;

#define INTEN                 _inten
#define INTRQ                 _intrq
#define INTEGS                _integs


// INTEN (Interrupt Enable) register definitions
#define INTEN_PA0_ENABLE_BIT         0
#define INTEN_T16_ENABLE_BIT         2
#define INTEN_TM2_ENABLE_BIT         6

#define INTEN_PA0                    (1 << INTEN_PA0_ENABLE_BIT)
#define INTEN_T16                    (1 << INTEN_T16_ENABLE_BIT)
#define INTEN_TM2                    (1 << INTEN_TM2_ENABLE_BIT)

#if defined(__PDK_HAS_PORTB)
  #define INTEN_PB0_ENABLE_BIT         1
  #define INTEN_PB0                    (1 << INTEN_PB0_ENABLE_BIT)
#endif
#if defined(__PDK_HAS_PB5_PA4_INT)
  #define INTEN_PB5_ENABLE_BIT         0
  #define INTEN_PA4_ENABLE_BIT         1

  #define INTEN_PB5                    (1 << INTEN_PB5_ENABLE_BIT)
  #define INTEN_PA4                    (1 << INTEN_PA4_ENABLE_BIT)
#endif
#if defined(__PDK_HAS_ADC)
  #define INTEN_ADC_ENABLE_BIT         3
  #define INTEN_ADC                    (1 << INTEN_ADC_ENABLE_BIT)
#endif
#if defined(__PDK_HAS_COMP)
  #define INTEN_COMP_ENABLE_BIT        4
  #define INTEN_COMP                   (1 << INTEN_COMP_ENABLE_BIT)
#endif
#if defined(__PDK_HAS_PWMG)
  #define INTEN_PWMG_ENABLE_BIT        5
  #define INTEN_PWMG                   (1 << INTEN_PWMG_ENABLE_BIT)
#endif
#if defined(__PDK_HAS_TM3)
  #define INTEN_TM3_ENABLE_BIT         7
  #define INTEN_TM3                    (1 << INTEN_TM3_ENABLE_BIT)
#endif

// INTRQ (Interrupt Request) register definitions
#define INTRQ_PA0_BIT                0
#define INTRQ_T16_BIT                2
#define INTRQ_TM2_BIT                6

#define INTRQ_PA0                    (1 << INTRQ_PA0_BIT)
#define INTRQ_T16                    (1 << INTRQ_T16_BIT)
#define INTRQ_TM2                    (1 << INTRQ_TM2_BIT)

#if defined(__PDK_HAS_PORTB)
  #define INTRQ_PB0_BIT                1
  #define INTRQ_PB0                    (1 << INTRQ_PB0_BIT)
#endif
#if defined(__PDK_HAS_PB5_PA4_INT)
  #define INTRQ_PB5_BIT                0
  #define INTRQ_PA4_BIT                1

  #define INTRQ_PB5                    (1 << INTRQ_PB5_BIT)
  #define INTRQ_PA4                    (1 << INTRQ_PA4_BIT)
#endif
#if defined(__PDK_HAS_ADC)
  #define INTRQ_ADC_BIT                3
  #define INTRQ_ADC                    (1 << INTRQ_ADC_BIT)
#endif
#if defined(__PDK_HAS_COMP)
  #define INTRQ_COMP_BIT               4
  #define INTRQ_COMP                   (1 << INTRQ_COMP_BIT)
#endif
#if defined(__PDK_HAS_PWMG)
  #define INTRQ_PWMG_BIT               5
  #define INTRQ_PWMG                   (1 << INTRQ_PWMG_BIT)
#endif
#if defined(__PDK_HAS_TM3)
  #define INTRQ_TM3_BIT                7
  #define INTRQ_TM3                    (1 << INTRQ_TM3_BIT)
#endif

// INTEGS (Interrupt Edge Select) register definitions
#define INTEGS_PA0_EDGE_SEL_BIT0     0
#define INTEGS_T16_EDGE_SEL_BIT      4

#define INTEGS_PA0_BOTH              0x00
#define INTEGS_PA0_RISING            (1 << INTEGS_PA0_EDGE_SEL_BIT0)
#define INTEGS_PA0_FALLING           (2 << INTEGS_PA0_EDGE_SEL_BIT0)

#define INTEGS_T16_RISING            0x00
#define INTEGS_T16_FALLING           (1 << INTEGS_T16_EDGE_SEL_BIT)

#if defined(__PDK_HAS_PORTB)
  #define INTEGS_PB0_EDGE_SEL_BIT0     2
  #define INTEGS_PB0_BOTH              0x00
  #define INTEGS_PB0_RISING            (1 << INTEGS_PB0_EDGE_SEL_BIT0)
  #define INTEGS_PB0_FALLING           (2 << INTEGS_PB0_EDGE_SEL_BIT0)
#endif
#if defined(__PDK_HAS_PB5_PA4_INT)
  #define INTEGS_PB5_EDGE_SEL_BIT0     0
  #define INTEGS_PA4_EDGE_SEL_BIT0     2

  #define INTEGS_PB5_BOTH              0x00
  #define INTEGS_PB5_RISING            (1 << INTEGS_PB5_EDGE_SEL_BIT0)
  #define INTEGS_PB5_FALLING           (2 << INTEGS_PB5_EDGE_SEL_BIT0)
  #define INTEGS_PA4_BOTH              0x00
  #define INTEGS_PA4_RISING            (1 << INTEGS_PA4_EDGE_SEL_BIT0)
  #define INTEGS_PA4_FALLING           (2 << INTEGS_PA4_EDGE_SEL_BIT0)
#endif
#if defined(__PDK_HAS_COMP_INT_EDGE)
  #define INTEGS_COMP_EDGE_SEL_BIT0     6
  #define INTEGS_COMP_BOTH              0x00
  #define INTEGS_COMP_RISING            (1 << INTEGS_COMP_EDGE_SEL_BIT0)
  #define INTEGS_COMP_FALLING           (2 << INTEGS_COMP_EDGE_SEL_BIT0)
#endif

#endif //__PDK_DEVICE_PERIPH_INTERRUPT_H__
