#ifndef __PDK_DEVICE_PFS172_H__
#define __PDK_DEVICE_PFS172_H__

#if !defined(__PDK_DEVICE_XXX_H__)
	#define __PDK_DEVICE_XXX_H__ "pfs172.h"
#else
	#error "Attempt to include more than one "pdk/device/XXX.h" file."
#endif

#if !defined(__SDCC_pdk14)
	#error "PFS172 needs the PDK14 backend. You must compile with the -mpdk14 option."
#endif

#define ILRC_FREQ           55000

// Factory Calibration address locations
#define FACTORY_IHRCR_ADDR  0x07ed
#define FACTORY_BGTR_ADDR   0x07ee


// FUSE definitions
#define FUSE_WORD_ADDR      0x07ff
#define FUSE_RES_BITS_HIGH  0x017C    // - - B B   0 0 0 1   D 1 1 1   1 1 0 S => 0x017C
#define FUSE_SECURITY_ON    0x0000    //(S)
#define FUSE_SECURITY_OFF   (1 << 0)
#define FUSE_PB4_PB7_NORMAL 0x0000    //(D)
#define FUSE_PB4_PB7_STRONG (1 << 7)
#define FUSE_BOOTUP_SLOW    0x0000    //(B)
#define FUSE_BOOTUP_FAST    (3 << 12)


// Register address definitions
#define FLAG_ADDR           0x00
//0x01
#define SP_ADDR             0x02
#define CLKMD_ADDR          0x03
#define INTEN_ADDR          0x04
#define INTRQ_ADDR          0x05
#define T16M_ADDR           0x06
//0x07
//0x08
//0x09
#define EOSCR_ADDR          0x0a
#define IHRCR_ADDR          0x0b
#define INTEGS_ADDR         0x0c
#define PADIER_ADDR         0x0d
#define PBDIER_ADDR         0x0e
//0x0f
#define PA_ADDR             0x10
#define PAC_ADDR            0x11
#define PAPH_ADDR           0x12
#define PAPL_ADDR           0x13
//0x14
#define PB_ADDR             0x15
#define PBC_ADDR            0x16
#define PBPH_ADDR           0x17
#define PBPL_ADDR           0x18
//0x19
//...
//0x1f
#define ADCC_ADDR           0x20
#define ADCM_ADDR           0x21
#define ADCR_ADDR           0x22
//0x23
//0x24
//0x25
#define MISC_ADDR           0x26
#define MISC2_ADDR          0x27
#define MISCLVR_ADDR        0x28
//0x2a
#define GPCC_ADDR           0x2b
#define GPCS_ADDR           0x2c
//0x2d
//0x2e
//0x2f
#define TM2C_ADDR           0x30
#define TM2CT_ADDR          0x31
#define TM2S_ADDR           0x32
#define TM2B_ADDR           0x33
#define TM3C_ADDR           0x34
#define TM3CT_ADDR          0x35
#define TM3S_ADDR           0x36
#define TM3B_ADDR           0x37
//0x38
//0x3a
#define ILRCR_ADDR          0x3b
#define BGTR_ADDR           0x3c
#define ROP_ADDR            0x3d
//0x3e
//0x3f


// ROP register BIT definitions
#define ROP_PB0_PA4_SEL_BIT          0
#define ROP_PA0_PB5_SEL_BIT          1
#define ROP_TMX_BIT_SEL_BIT          4
#define ROP_TMX_FREQ_SEL_BIT         5
#define ROP_PWM_SEL_BIT              6


// Peripheral definitions
#define __PDK_HAS_COMP
#define __PDK_HAS_EOSC
#define __PDK_HAS_PORTB
#define __PDK_HAS_PB5_PA4_INT
#define __PDK_HAS_TM3
#define __PDK_HAS_ADC

#include "periph/accumulator.h"
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
#include "periph/comparator.h"
#include "periph/adc.h"
#include "periph/misc.h"
#include "periph/misc2.h"
#include "periph/misclvr.h"
#include "periph/rop.h"

// Additional MISC register definitions
// TODO: verify these... they are in the PFS172.INC file but not the datasheet
#define MISC_EC_DRIVE_BIT            6

#define MISC_EC_DRIVE_HIGH           0x00
#define MISC_EC_DRIVE_LOW            (1 << MISC_EC_DRIVE_BIT)

// Additional MISC2 register definitions
#define MISC2_PA6_TO_PA7_BIT         1
#define MISC2_PB3_TO_PB4_BIT         2

#define MISC2_PA6_TO_PA7             (1 << MISC2_PA6_TO_PA7_BIT)
#define MISC2_PB3_TO_PB4             (1 << MISC2_PB3_TO_PB4_BIT)

#endif //__PDK_DEVICE_PFS172_H__
