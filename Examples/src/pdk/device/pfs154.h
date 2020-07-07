#ifndef __PDK_DEVICE_PFS154_H__
#define __PDK_DEVICE_PFS154_H__

#if !defined(__PDK_DEVICE_XXX_H__)
	#define __PDK_DEVICE_XXX_H__ "pfs154.h"
#else
	#error "Attempt to include more than one "pdk/device/XXX.h" file."
#endif

#if !defined(__SDCC_pdk14)
	#error "PFS154 needs the PDK14 backend. You must compile with the -mpdk14 option."
#endif

#define ILRC_FREQ           55000

// Factory Calibration address locations
#define FACTORY_IHRCR_ADDR  0x07ed
#define FACTORY_BGTR_ADDR   0x07ee


// FUSE definitions
#define FUSE_WORD_ADDR      0x07ff
#define FUSE_RES_BITS_HIGH  0x30FC // - - 1 1   B B 0 D   1 1 1 1   1 1 0 S
// Blank IC Values          0x3FFD // - - 1 1   1 1 1 1   1 1 1 1   1 1 1 1 (Security Off, Normal IO Drive, Fast Boot-up)
#define FUSE_SECURITY_ON    0x0000    //(S)
#define FUSE_SECURITY_OFF   (1 << 0)
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
#define MISC2_ADDR          0x0f
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
#define MISCLVR_ADDR        0x1b
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
#define PWMG1C_ADDR         0x26
#define PWMG1S_ADDR         0x27
#define PWMG1DTH_ADDR       0x28
#define PWMG1DTL_ADDR       0x29
#define PWMG1CUBH_ADDR      0x2a
#define PWMG1CUBL_ADDR      0x2b
#define PWMG2C_ADDR         0x2c
#define PWMG2S_ADDR         0x2d
#define PWMG2DTH_ADDR       0x2e
#define PWMG2DTL_ADDR       0x2f
#define PWMG2CUBH_ADDR      0x30
#define PWMG2CUBL_ADDR      0x31
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
#include "periph/pwmg_1.h"
#include "periph/pwmg_2.h"
#include "periph/misc.h"
#include "periph/misc2.h"
#include "periph/misclvr_basic.h"

// Additional MISC2 register definitions
#define MISC2_COMP_PWMG1_BIT0        3
#define MISC2_TM3_PWMG2_BIT0         4

#endif //__PDK_DEVICE_PFS154_H__
