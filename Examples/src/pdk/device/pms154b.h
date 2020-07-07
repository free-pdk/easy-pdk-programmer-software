#ifndef __PDK_DEVICE_PMS154B_H__
#define __PDK_DEVICE_PMS154B_H__

#if !defined(__PDK_DEVICE_XXX_H__)
	#define __PDK_DEVICE_XXX_H__ "pms154b.h"
#else
	#error "Attempt to include more than one "pdk/device/XXX.h" file."
#endif

#if !defined(__SDCC_pdk14)
	#error "PMS154B needs the PDK14 backend. You must compile with the -mpdk14 option."
#endif

#define ILRC_FREQ           70000

// Factory Calibration address locations
#define FACTORY_BGTR_ADDR   0x07f6


// FUSE definitions
#define FUSE_WORD_ADDR      0x07ff
#define FUSE_RES_BITS_HIGH  0x1040    // - - 0 1   B B 0 D   0 1 0 L   L L 0 S
// Blank IC Values          ???
#define FUSE_SECURITY_ON    0x0000    //(S)
#define FUSE_SECURITY_OFF   (1 << 0)
#define FUSE_LVR_4V         0x0000    //(L)
#define FUSE_LVR_3V5        (1 << 2)
#define FUSE_LVR_3V         (2 << 2)
#define FUSE_LVR_2V75       (3 << 2)
#define FUSE_LVR_2V5        (4 << 2)
#define FUSE_LVR_1V8        (5 << 2)
#define FUSE_LVR_2V2        (6 << 2)
#define FUSE_LVR_2V         (7 << 2)
#define FUSE_IO_DRV_LOW     0x0000    //(D)
#define FUSE_IO_DRV_NORMAL  (1 << 8)
#define FUSE_BOOTUP_SLOW    0x0000    //(B)
#define FUSE_BOOTUP_FAST    (3 << 10)


// Register address definitions
#define FLAG_ADDR           0x00
//0x01
#define SP_ADDR             0x02
#define CLKMD_ADDR          0x03
#define INTEN_ADDR          0x04
#define INTRQ_ADDR          0x05
#define T16M_ADDR           0x06
//0x07
#define MISC_ADDR           0x08
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
#define TM2S_ADDR           0x17
#define GPCC_ADDR           0x18
#define GPCS_ADDR           0x19
#define BGTR_ADDR           0x1a
//0x1b
#define TM2C_ADDR           0x1c
#define TM2CT_ADDR          0x1d
//0x1e
//0x1f
#define PWMG0C_ADDR         0x20
#define PWMG0S_ADDR         0x21
#define PWMG0DTH_ADDR       0x22
#define PWMG0DTL_ADDR       0x23
#define PWMG0CUBH_ADDR      0x24
#define PWMG0CUBL_ADDR      0x25
//0x26
//...
//0x31
#define TM3C_ADDR           0x32
#define TM3CT_ADDR          0x33
#define TM3S_ADDR           0x34
#define TM3B_ADDR           0x35
//0x36
//0x37
//0x38
#define ILRCR_ADDR          0x39
//0x3a
//...
//0x3f


// Peripheral definitions
#define __PDK_HAS_COMP
#define __PDK_HAS_EOSC
#define __PDK_HAS_PORTB
#define __PDK_HAS_TM3
#define __PDK_HAS_PWMG
#define __PDK_HAS_LCD

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
#include "periph/pwmg_0.h"
#include "periph/misc.h"

#endif //__PDK_DEVICE_PMS154B_H__
