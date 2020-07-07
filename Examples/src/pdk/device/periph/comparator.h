#ifndef __PDK_DEVICE_PERIPH_COMPARATOR_H__
#define __PDK_DEVICE_PERIPH_COMPARATOR_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/comparator.h" by itself."
#endif

// __sfr definitions
__sfr __at(GPCC_ADDR)         _gpcc;
__sfr __at(GPCS_ADDR)         _gpcs;

#define GPCC                  _gpcc
#define GPCS                  _gpcs

// GPCC (Comparator Control) register definitions
#define GPCC_COMP_PLUS_BIT           0
#define GPCC_COMP_MINUS_BIT0         1
#define GPCC_COMP_INVERT_OUT_BIT     4
#define GPCC_COMP_OUT_TO_TM2CLK_BIT  5
#define GPCC_COMP_RESULT_BIT         6
#define GPCC_COMP_ENABLE_BIT         7

#define GPCC_COMP_PLUS_VINT_R        0x00
#define GPCC_COMP_PLUS_PA4           (1 << GPCC_COMP_PLUS_BIT)

#define GPCC_COMP_MINUS_PA3          0x00
#define GPCC_COMP_MINUS_PA4          (1 << GPCC_COMP_MINUS_BIT0)
#define GPCC_COMP_MINUS_BANDGAP_1V2  (2 << GPCC_COMP_MINUS_BIT0)
#define GPCC_COMP_MINUS_VINT_R       (3 << GPCC_COMP_MINUS_BIT0)
#if defined(__PDK_HAS_PORTB)
  #define GPCC_COMP_MINUS_PB6          (4 << GPCC_COMP_MINUS_BIT0)
  #define GPCC_COMP_MINUS_PB7          (5 << GPCC_COMP_MINUS_BIT0)
#else
  #define GPCC_COMP_MINUS_PA6          (4 << GPCC_COMP_MINUS_BIT0)
  #define GPCC_COMP_MINUS_PA7          (5 << GPCC_COMP_MINUS_BIT0)
#endif

#define GPCC_COMP_OUT_INVERT         (1 << GPCC_COMP_INVERT_OUT_BIT)
#define GPCC_COMP_OUT_TO_TM2CLK      (1 << GPCC_COMP_OUT_TO_TM2CLK_BIT)

#define GPCC_COMP_RESULT_NEGATIV     0x00
#define GPCC_COMP_RESULT_POSITIV     (1 << GPCC_COMP_RESULT_BIT)
#define GPCC_COMP_ENABLE             (1 << GPCC_COMP_ENABLE_BIT)

// GPCS (Comparator Selection) register definitions
#define GPCS_COMP_VOLTAGE_LVL_BIT0   0
#define GPCS_COMP_RANGE_SEL_BIT0     4
#define GPCS_COMP_OUT_PA0_BIT        7

#define GPCS_COMP_RANGE1             0x00
#define GPCS_COMP_RANGE2             (1 << GPCS_COMP_RANGE_SEL_BIT0)
#define GPCS_COMP_RANGE3             (2 << GPCS_COMP_RANGE_SEL_BIT0)
#define GPCS_COMP_RANGE4             (3 << GPCS_COMP_RANGE_SEL_BIT0)

#define GPCS_COMP_OUTPUT_PA0         (1 << GPCS_COMP_OUT_PA0_BIT)

#if defined(__PDK_HAS_COMP_WAKEUP)
  #define GPCS_COMP_WAKEUP_ENABLE_BIT  6
  #define GPCS_COMP_WAKEUP_ENABLE      (1 << GPCS_COMP_WAKEUP_ENABLE_BIT)
#endif

#endif //__PDK_DEVICE_PERIPH_COMPARATOR_H__
