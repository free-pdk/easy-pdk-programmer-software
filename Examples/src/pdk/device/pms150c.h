#ifndef __PDK_DEVICE_PMS150C_H__
#define __PDK_DEVICE_PMS150C_H__

#if !defined(__PDK_DEVICE_XXX_H__)
	#define __PDK_DEVICE_XXX_H__ "pms150c.h"
#else
	#error "Attempt to include more than one "pdk/device/XXX.h" file."
#endif

#if !defined(__SDCC_pdk13)
	#error "PMS150C/PMS15A needs the PDK13 backend. You must compile with the -mpdk13 option."
#endif

#define ILRC_FREQ           59000

// Factory Calibration address locations
#define FACTORY_BGTR_ADDR   0x03f6


// FUSE definitions
#define FUSE_WORD_ADDR      0x03ff
#define FUSE_RES_BITS_HIGH  0x0260    // - - - 0   B B 1 0   D 1 1 L   L L 0 S
// Blank IC Values          0x0FFD    // - - - 0   1 1 1 1   1 1 1 1   1 1 0 1 (Security Off, 2.0V LVR, Normal IO Drive, Fast Boot-up)
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
#define FUSE_IO_DRV_NORMAL  (1 << 7)
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
//0x08
#define TM2B_ADDR           0x09
#define EOSCR_ADDR          0x0a
#define IHRCR_ADDR          0x0b
#define INTEGS_ADDR         0x0c
#define PADIER_ADDR         0x0d
//0x0e
//0x0f
#define PA_ADDR             0x10
#define PAC_ADDR            0x11
#define PAPH_ADDR           0x12
//0x13
//...
//0x16
#define TM2S_ADDR           0x17
//0x18
#define BGTR_ADDR           0x19
#define GPCC_ADDR           0x1a
#define MISC_ADDR           0x1b
#define TM2C_ADDR           0x1c
#define TM2CT_ADDR          0x1d
#define GPCS_ADDR           0x1e
#define ILRCR_ADDR          0x1f


// Peripheral definitions
#define __PDK_HAS_COMP
#define __PDK_HAS_COMP_WAKEUP
#define __PDK_HAS_COMP_INT_EDGE

#include "periph/accumulator.h"
#include "periph/stack.h"
#include "periph/clock.h"
#include "periph/external_oscillator.h"
#include "periph/interrupt.h"
#include "periph/port_a.h"
#include "periph/timer_16.h"
#include "periph/timer_2.h"
#include "periph/bandgap.h"
#include "periph/comparator.h"
#include "periph/misc.h"

#endif //__PDK_DEVICE_PMS150C_H__
