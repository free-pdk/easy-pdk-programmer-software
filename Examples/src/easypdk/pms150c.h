#ifndef __PMS150C_H__
#define __PMS150C_H__

#ifndef PMS150C
#define PMS150C
#endif
#if !defined __SDCC_pdk13
#error "PMS150C needs PDK13 backend. Compile with -mpdk13"
#endif

#include "pdkcommon.h"

//fuse definitions
#define FUSE_SECURITY_ON   0x0000 //(S)
#define FUSE_SECURITY_OFF  0x0001
#define FUSE_LVR_4V        0x0000 //(L)
#define FUSE_LVR_3V5       0x0004
#define FUSE_LVR_3V        0x0008
#define FUSE_LVR_2V75      0x000C
#define FUSE_LVR_2V5       0x0010
#define FUSE_LVR_1V8       0x0014
#define FUSE_LVR_2V2       0x0018
#define FUSE_LVR_2V        0x001C
#define FUSE_IO_DRV_LOW    0x0000 //(D)
#define FUSE_IO_DRV_NORMAL 0x0080
#define FUSE_BOOTUP_SLOW   0x0000 //(B)
#define FUSE_BOOTUP_FAST   0x0C00
#define FUSE_RES_BITS_HIGH 0x0260 // - - - 0   B B 1 0   D 1 1 L   L L 0 S
// Blank IC Values         0x0FFD // - - - 0   1 1 1 1   1 1 1 1   1 1 0 1 (Security Off, 2.0V LVR, Normal IO Drive, Fast Boot-up)
#define EASY_PDK_FUSE(f) { __asm__(".area FUSE (ABS)\n.org (0x3ff*2)\n.word ("_ASMD(FUSE_RES_BITS_HIGH)"|"_ASMD(f)")\n.area CODE\n"); }

//set calibration macros
#define EASY_PDK_CALIBRATE_IHRC EASY_PDK_CALIBRATE_IHRC_H8
#define EASY_PDK_CALIBRATE_ILRC EASY_PDK_CALIBRATE_ILRC_L8
#define EASY_PDK_CALIBRATE_BG   EASY_PDK_CALIBRATE_BG_B19
#define EASY_PDK_USE_FACTORY_BGTR() { __asm__("call #0x3f6\n mov "_ASMV(BGTR)",a\n"); }

#define ILRC_FREQ  59000

//IO register definitions
__sfr __at(0x00) _flag;
//0x01
__sfr __at(0x02) _sp;
__sfr __at(0x03) _clkmd;
__sfr __at(0x04) _inten;
__sfr __at(0x05) _intrq;
__sfr __at(0x06) _t16m;
//0x07
//0x08
__sfr __at(0x09) _tm2b;
__sfr __at(0x0a) _eoscr;
__sfr __at(0x0b) _ihrcr;
__sfr __at(0x0c) _integs;
__sfr __at(0x0d) _padier;
//0x0e
//0x0f
__sfr __at(0x10) _pa;
__sfr __at(0x11) _pac;
__sfr __at(0x12) _paph;
//0x13
//0x14
//0x15
//0x16
__sfr __at(0x17) _tm2s;
//0x18
__sfr __at(0x19) _bgtr;
__sfr __at(0x1a) _gpcc;
__sfr __at(0x1b) _misc;
__sfr __at(0x1c) _tm2c;
__sfr __at(0x1d) _tm2ct;
__sfr __at(0x1e) _gpcs;
__sfr __at(0x1f) _ilrcr;

//T16C register
__sfr16          _t16c;

#define SP        _sp
#define FLAG      _flag
#define CLKMD   _clkmd
#define INTEN   _inten
#define INTRQ   _intrq
#define T16M    _t16m
#define TM2B    _tm2b
#define EOSCR   _eoscr
#define IHRCR   _ihrcr
#define INTEGS  _integs
#define PADIER  _padier
#define PA      _pa
#define PAC     _pac
#define PAPH    _paph
#define TM2S    _tm2s
#define BGTR    _bgtr
#define GPCC    _gpcc
#define MISC    _misc
#define TM2C    _tm2c
#define TM2CT   _tm2ct
#define GPCS    _gpcs
#define ILRCR   _ilrcr
#define T16C    _t16c

//flag definitions
#define FLAG_ZF 0x01
#define FLAG_CF 0x02
#define FLAG_AC 0x04
#define FLAG_OV 0x08
#define FLAG_ZF_BIT 0
#define FLAG_CF_BIT 1
#define FLAG_AC_BIT 2
#define FLAG_OV_BIT 3

//clkmd definitions
#define CLKMD_ENABLE_PA5RST          0x01
#define CLKMD_ENABLE_WATCHDOG        0x02
#define CLKMD_ENABLE_ILRC            0x04
#define CLKMD_ENABLE_IHRC            0x10
#define CLKMD_IHRC_DIV2              0x20
#define CLKMD_IHRC_DIV4              0x00
#define CLKMD_IHRC_DIV8              0x28
#define CLKMD_IHRC_DIV16             0x08
#define CLKMD_IHRC_DIV32             0x68
#define CLKMD_IHRC_DIV64             0x88
#define CLKMD_ILRC                   0xe0
#define CLKMD_ILRC_DIV4              0xc0

//interrupt enable definitions
#define INTEN_PA0                    0x01
#define INTEN_T16                    0x04
#define INTEN_COMP                   0x10
#define INTEN_TM2                    0x40

//interrupt request definitions
#define INTRQ_PA0                    0x01
#define INTRQ_T16                    0x04
#define INTRQ_COMP                   0x10
#define INTRQ_TM2                    0x40

//tm16 definitions
#define T16_INTSRC_8BIT              0x00
#define T16_INTSRC_9BIT              0x01
#define T16_INTSRC_10BIT             0x02
#define T16_INTSRC_11BIT             0x03
#define T16_INTSRC_12BIT             0x04
#define T16_INTSRC_13BIT             0x05
#define T16_INTSRC_14BIT             0x06
#define T16_INTSRC_15BIT             0x07
#define T16_CLK_DIV1                 0x00
#define T16_CLK_DIV4                 0x08
#define T16_CLK_DIV16                0x10
#define T16_CLK_DIV64                0x18
#define T16_CLK_DISABLE              0x00
#define T16_CLK_SYSCLK               0x20
#define T16_CLK_PA4_FALL             0x60
#define T16_CLK_IHRC                 0x80
#define T16_CLK_ILRC                 0xC0
#define T16_CLK_PA0_FALL             0xE0

//eosc definitions
#define EOSC_LVD_BANDGAP_SHUTDOWN    0x01

//integs definitions
#define INTEGS_PA0_BOTH              0x00
#define INTEGS_PA0_RISING            0x01
#define INTEGS_PA0_FALLING           0x02
#define INTEGS_T16_RISING            0x00
#define INTEGS_T16_FALLING           0x10
#define INTEGS_COMP_BOTH             0x00
#define INTEGS_COMP_RISING           0x40
#define INTEGS_COMP_FALLING          0x80

//padie definitions
#define PADIE_PA0_WAKEUP_ENABLE      0x01
#define PADIE_PA3_WAKEUP_ENABLE      0x08
#define PADIE_PA4_WAKEUP_ENABLE      0x10
#define PADIE_PA5_WAKEUP_ENABLE      0x20
#define PADIE_PA6_WAKEUP_ENABLE      0x40
#define PADIE_PA7_WAKEUP_ENABLE      0x80

//misc definitions
#define MISC_WATCHDOG_8K_ILRC        0x00
#define MISC_WATCHDOG_16K_ILRC       0x01
#define MISC_WATCHDOG_64K_ILRC       0x02
#define MISC_WATCHDOG_256K_ILRC      0x03
#define MISC_LVR_DISABLE             0x04
#define MISC_FAST_WAKEUP_ENABLE      0x20

//tm2c definitions
#define TM2C_CLK_DISABLE             0x00
#define TM2C_CLK_SYSCLK              0x10
#define TM2C_CLK_IHRC                0x20
#define TM2C_CLK_ILRC                0x40
#define TM2C_CLK_COMPOUT             0x50
#define TM2C_CLK_PA0_RISE            0x80
#define TM2C_CLK_PA0_FALL            0x90
#define TM2C_CLK_PB0_RISE            0xA0
#define TM2C_CLK_PB0_FALL            0xB0
#define TM2C_CLK_PA4_RISE            0xC0
#define TM2C_CLK_PA4_FALL            0xD0
#define TM2C_OUT_DISABLE             0x00
#define TM2C_OUT_PA3                 0x08
#define TM2C_OUT_PA4                 0x0C
#define TM2C_MODE_PERIOD             0x00
#define TM2C_MODE_PWM                0x02
#define TM2C_INVERT_OUT              0x01

//tm2s definitions
#define TM2S_PWM_RES_8BIT            0x00
#define TM2S_PWM_RES_6BIT            0x80
#define TM2S_PRESCALE_NONE           0x00
#define TM2S_PRESCALE_DIV4           0x20
#define TM2S_PRESCALE_DIV16          0x40
#define TM2S_PRESCALE_DIV64          0x60
#define TM2S_SCALE_NONE              0x00
#define TM2S_SCALE_DIV2              0x01
#define TM2S_SCALE_DIV3              0x02
#define TM2S_SCALE_DIV4              0x03
#define TM2S_SCALE_DIV5              0x04
#define TM2S_SCALE_DIV6              0x05
#define TM2S_SCALE_DIV7              0x06
#define TM2S_SCALE_DIV8              0x07
#define TM2S_SCALE_DIV9              0x08
#define TM2S_SCALE_DIV10             0x09
#define TM2S_SCALE_DIV11             0x0A
#define TM2S_SCALE_DIV12             0x0B
#define TM2S_SCALE_DIV13             0x0C
#define TM2S_SCALE_DIV14             0x0D
#define TM2S_SCALE_DIV15             0x0E
#define TM2S_SCALE_DIV16             0x0F
#define TM2S_SCALE_DIV17             0x10
#define TM2S_SCALE_DIV18             0x11
#define TM2S_SCALE_DIV19             0x12
#define TM2S_SCALE_DIV20             0x13
#define TM2S_SCALE_DIV21             0x14
#define TM2S_SCALE_DIV22             0x15
#define TM2S_SCALE_DIV23             0x16
#define TM2S_SCALE_DIV24             0x17
#define TM2S_SCALE_DIV25             0x18
#define TM2S_SCALE_DIV26             0x19
#define TM2S_SCALE_DIV27             0x1A
#define TM2S_SCALE_DIV28             0x1B
#define TM2S_SCALE_DIV29             0x1C
#define TM2S_SCALE_DIV30             0x1D
#define TM2S_SCALE_DIV31             0x1E
#define TM2S_SCALE_DIV32             0x1F

//gpcc definitions
#define GPCC_COMP_PLUS_VINT_R        0x00
#define GPCC_COMP_PLUS_PA4           0x01
#define GPCC_COMP_MINUS_PA3          0x00
#define GPCC_COMP_MINUS_PA4          0x02
#define GPCC_COMP_MINUS_BANDGAP_1V2  0x04
#define GPCC_COMP_MINUS_VINT_R       0x06
#define GPCC_COMP_MINUS_PA6          0x08
#define GPCC_COMP_MINUS_PA7          0x0A
#define GPCC_COMP_OUT_INVERT         0x10
#define GPCC_COMP_OUT_TO_TM2CLK      0x20
#define GPCC_COMP_RESULT_NEGATIV     0x00
#define GPCC_COMP_RESULT_POSITIV     0x40
#define GPCC_COMP_ENABLE             0x80

//gpcs definitions
#define GPCS_COMP_CASE1              0x00
#define GPCS_COMP_CASE2              0x10
#define GPCS_COMP_CASE3              0x20
#define GPCS_COMP_CASE4              0x30
#define GPCS_COMP_WAKEUP_ENABLE      0x40
#define GPCS_COMP_OUTPUT_PA0         0x80

#endif //__PMS150C_H__
