#ifndef __PDK_DEVICE_PMS131_H__
#define __PDK_DEVICE_PMS131_H__

#if !defined(__PDK_DEVICE_XXX_H__)
	#define __PDK_DEVICE_XXX_H__ "pms131.h"
#else
	#error "Attempt to include more than one "pdk/device/XXX.h" file."
#endif

#if !defined(__SDCC_pdk14)
	#error "PMS131 needs the PDK14 backend. You must compile with the -mpdk14 option."
#endif

#define ILRC_FREQ           37000

// Factory Calibration address locations
#define FACTORY_BGTR_ADDR   0x05f6


// FUSE definitions
#define FUSE_WORD_ADDR      0x05ff
#define FUSE_RES_BITS_HIGH  0x3C1C    // - - 1 1   1 1 0 0   L L L 1   1 1 0 S
// Blank IC Values          ???
#define FUSE_SECURITY_ON    0x0000    //(S)
#define FUSE_SECURITY_OFF   (1 << 0)
#define FUSE_LVR_4V1        0x0000    //(L)
#define FUSE_LVR_3V6        (1 << 5)
#define FUSE_LVR_3V1        (2 << 5)
#define FUSE_LVR_2V8        (3 << 5)
#define FUSE_LVR_2V5        (4 << 5)
#define FUSE_LVR_1V8        (5 << 5)
#define FUSE_LVR_2V2        (6 << 5)
#define FUSE_LVR_2V         (7 << 5)


// Register address definitions
#define FLAG_ADDR           0x00
//0x01
#define SP_ADDR             0x02
#define CLKMD_ADDR          0x03
#define INTEN_ADDR          0x04
#define INTRQ_ADDR          0x05
#define T16M_ADDR           0x06
//0x07
#define MULOP_ADDR          0x08
#define MULRH_ADDR          0x09
#define TM2B_ADDR           0x09
#define EOSCR_ADDR          0x0a
#define IHRCR_ADDR          0x0b
#define INTEGS_ADDR         0x0c
#define PADIER_ADDR         0x0d
#define PBDIER_ADDR         0x0e
//0x0f
#define PA_ADDR             0x10
#define PAC_ADDR            0x11
#define PAPH_ADDR           0x12
//0x13
#define PB_ADDR             0x14
#define PBC_ADDR            0x15
#define PBPH_ADDR           0x16
//0x17
//...
//0x1a
#define MISC_ADDR           0x1b
#define ADCRGC_ADDR         0x1c
//0x1d
//0x1e
//0x1f
#define ADCC_ADDR           0x20
#define ADCM_ADDR           0x21
#define ADCRH_ADDR          0x22
#define ADCRL_ADDR          0x23
#define TM3B_ADDR           0x23
//0x24
#define RESET_ADDR          0x26
//0x26
//...
//0x2d
#define TM3C_ADDR           0x2e
#define TM3CT_ADDR          0x2f
//0x30
//...
//0x35
#define BGTR_ADDR           0x36
#define TM2S_ADDR           0x37
//0x38
#define TM3S_ADDR           0x39
#define ROP_ADDR            0x3a
//0x3b
#define TM2C_ADDR           0x3c
#define TM2CT_ADDR          0x3d
//0x3e
//0x3f


// ROP register BIT definitions
#define ROP_PB0_PA4_SEL_BIT          0
#define ROP_PA0_PB5_SEL_BIT          1


// Peripheral definitions
#define __PDK_HAS_EOSC
#define __PDK_HAS_PORTB
#define __PDK_HAS_PB5_PA4_INT
#define __PDK_HAS_TM3
#define __PDK_HAS_ADC
#define __PDK_HAS_ADC_12BIT

#include "periph/accumulator.h"
#include "periph/multiplier.h"
#include "periph/stack.h"
#include "periph/clock.h"
#include "periph/external_oscillator.h"
#include "periph/interrupt.h"
#include "periph/port_a.h"
#include "periph/port_b.h"
#include "periph/timer_16.h"
#include "periph/timer_2.h"
#include "periph/timer_3.h"
#include "periph/bandgap.h"
#include "periph/adc.h"
#include "periph/misc.h"
#include "periph/rop.h"

// Override some defaults
// (not sure why these wouldn't be supported but they are not defined either in the .INC file or the datasheet)
#undef CLKMD_ILRC_DIV16
#undef TM2C_CLK_EOSC
#undef TM3C_CLK_EOSC

// Additional MISC register definitions
#define MISC_LVR_FAST_RECOVER_BIT    3
#define MISC_EC_DRIVE_BIT            6

#define MISC_LVR_NORMAL_RECOVER      0x00
#define MISC_LVR_FAST_RECOVER        (1 << MISC_LVR_RECOVER_BIT)

#define MISC_EC_DRIVE_HIGH           0x00
#define MISC_EC_DRIVE_LOW            (1 << MISC_EC_DRIVE_BIT)

#endif //__PDK_DEVICE_PMS152_H__
