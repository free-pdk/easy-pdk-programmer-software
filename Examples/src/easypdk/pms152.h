#ifndef __PMS152_H__
#define __PMS152_H__

#ifndef PMS152
#define PMS152
#endif
#if !defined __SDCC_pdk14
#error "PMS152 needs PDK14 backend. Compile with -mpdk14"
#endif

#include "pdkcommon.h"

//fuse definitions
#define FUSE_SECURITY_ON   0x0000 //(S)
#define FUSE_SECURITY_OFF  0x0001
#define FUSE_BOOTUP_SLOW   0x0000 //(B)
#define FUSE_BOOTUP_FAST   0x0C00
#define FUSE_RES_BITS_HIGH 0x11FC // - - 0 1   B B 0 1   1 1 1 1   1 1 0 S
// Blank IC Values         0x1FFD // - - 0 1   1 1 1 1   1 1 1 1   1 1 0 1 (Security Off, Fast Boot-up)
#define EASY_PDK_FUSE(f) { __asm__(".area FUSE (ABS)\n.org (0x4ff*2)\n.word ("_ASMD(FUSE_RES_BITS_HIGH)"|"_ASMD(f)")\n.area CODE\n"); }

//set calibration macros
#define EASY_PDK_CALIBRATE_IHRC(frequency,millivolt) EASY_PDK_CALIBRATE_RC_M( EASY_PDK_CALTYPE_IHRC, 0x0B, frequency, millivolt )
#define EASY_PDK_CALIBRATE_ILRC(frequency,millivolt) EASY_PDK_CALIBRATE_RC_M( EASY_PDK_CALTYPE_ILRC, 0x39, frequency, millivolt )
#define EASY_PDK_CALIBRATE_BG()                      EASY_PDK_CALIBRATE_BG_M( 0x1A, 0x18, 0x19 )
#define EASY_PDK_USE_FACTORY_BGTR() { __asm__("call #0x4f6\n mov "_ASMV(BGTR)",a\n"); }

#define ILRC_FREQ  55000

//IO register definitions
__sfr __at(0x00) _flag;
//0x01
__sfr __at(0x02) _sp;
__sfr __at(0x03) _clkmd;
__sfr __at(0x04) _inten;
__sfr __at(0x05) _intrq;
__sfr __at(0x06) _t16m;
//0x07
__sfr __at(0x08) _misc;
__sfr __at(0x09) _tm2b;
__sfr __at(0x0a) _eoscr;
__sfr __at(0x0b) _ihrcr;
__sfr __at(0x0c) _integs;
__sfr __at(0x0d) _padier;
__sfr __at(0x0e) _pbdier;
__sfr __at(0x0f) _misc2;
__sfr __at(0x10) _pa;
__sfr __at(0x11) _pac;
__sfr __at(0x12) _paph;
//0x13
__sfr __at(0x14) _pb;
__sfr __at(0x15) _pbc;
__sfr __at(0x16) _pbph;
__sfr __at(0x17) _tm2s;
__sfr __at(0x18) _gpcc;
__sfr __at(0x19) _gpcs;
__sfr __at(0x1a) _bgtr;
__sfr __at(0x1b) _misclvr;
__sfr __at(0x1c) _tm2c;
__sfr __at(0x1d) _tm2ct;
//0x1e
//0x1f
__sfr __at(0x20) _pwmg0c;
__sfr __at(0x21) _pwmgclk;
__sfr __at(0x22) _pwmg0dth;
__sfr __at(0x23) _pwmg0dtl;
__sfr __at(0x24) _pwmgcubh;
__sfr __at(0x25) _pwmgcubl;
__sfr __at(0x26) _pwmg1c;
//0x27
__sfr __at(0x28) _pwmg1dth;
__sfr __at(0x29) _pwmg1dtl;
//0x2a
//0x2b
__sfr __at(0x2c) _pwmg2c;
//0x2d
__sfr __at(0x2e) _pwmg2dth;
__sfr __at(0x2f) _pwmg2dtl;
//0x30
//0x31
//0x32
//0x33
//0x34
//0x35
//0x36
//0x37
//0x38
__sfr __at(0x39) _ilrcr;
__sfr __at(0x3a) _rop;
//0x3b
//0x3c
//0x3d
//0x3e
//0x3f

//T16C register
__sfr16          _t16c;

#define SP        _sp
#define FLAG      _flag
#define CLKMD     _clkmd
#define INTEN     _inten
#define INTRQ     _intrq
#define T16M      _t16m
#define TM2B      _tm2b
#define EOSCR     _eoscr
#define IHRCR     _ihrcr
#define INTEGS    _integs
#define PADIER    _padier
#define PBDIER    _pbdier
#define MISC2     _misc2
#define PA        _pa
#define PAC       _pac
#define PAPH      _paph
#define PB        _pb
#define PBC       _pbc
#define PBPH      _pbph
#define TM2S      _tm2s
#define GPCC      _gpcc
#define GPCS      _gpcs
#define BGTR      _bgtr
#define MISCLVR   _misclvr
#define TM2C      _tm2c
#define TM2CT     _tm2ct
#define PWMG0C    _pwmg0c
#define PWMGCLK   _pwmgclk
#define PWMG0DTH  _pwmg0dth
#define PWMG0DTL  _pwmg0dtl
#define PWMGCUBH  _pwmgcubh
#define PWMGCUBL  _pwmgcubl
#define PWMG1C    _pwmg1c
#define PWMG1DTH  _pwmg1dth
#define PWMG1DTL  _pwmg1dtl
#define PWMG2C    _pwmg2c
#define PWMG2DTH  _pwmg2dth
#define PWMG2DTL  _pwmg2dtl
#define ILRCR     _ilrcr
#define ROP       _rop
#define T16C      _t16c

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
#define CLKMD_ILRC_DIV16             0x48
#define CLKMD_EOSC                   0xa0
#define CLKMD_EOSC_DIV2              0x80
#define CLKMD_EOSC_DIV4              0x60
#define CLKMD_EOSC_DIV8              0xa8

//interrupt enable definitions
#define INTEN_PA0                    0x01
#define INTEN_PB5                    0x01
#define INTEN_PB0                    0x02
#define INTEN_PA4                    0x02
#define INTEN_T16                    0x04
#define INTEN_COMP                   0x10
#define INTEN_PWMG                   0x20
#define INTEN_TM2                    0x40

//interrupt request definitions
#define INTRQ_PA0                    0x01
#define INTRQ_PB5                    0x01
#define INTRQ_PB0                    0x02
#define INTRQ_PA4                    0x02
#define INTRQ_T16                    0x04
#define INTRQ_COMP                   0x10
#define INTRQ_PWMG                   0x20
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
#define T16_CLK_EOSC                 0xA0
#define T16_CLK_ILRC                 0xC0
#define T16_CLK_PA0_FALL             0xE0

//eosc definitions
#define EOSC_LVD_BANDGAP_SHUTDOWN    0x01
#define EOSC_EXT_32KHZ               0x20
#define EOSC_EXT_1MHZ                0x40
#define EOSC_EXT_4MHZ                0x60
#define EOSC_EXT_ENABLE              0x80

//integs definitions
#define INTEGS_PA0_BOTH              0x00
#define INTEGS_PA0_RISING            0x01
#define INTEGS_PA0_FALLING           0x02
#define INTEGS_PB5_BOTH              0x00
#define INTEGS_PB5_RISING            0x01
#define INTEGS_PB5_FALLING           0x02
#define INTEGS_PB0_BOTH              0x00
#define INTEGS_PB0_RISING            0x04
#define INTEGS_PB0_FALLING           0x08
#define INTEGS_PA4_BOTH              0x00
#define INTEGS_PA4_RISING            0x04
#define INTEGS_PA4_FALLING           0x08
#define INTEGS_T16_RISING            0x00
#define INTEGS_T16_FALLING           0x10

//padie definitions
#define PADIE_PA0_WAKEUP_ENABLE      0x01
#define PADIE_PA3_WAKEUP_ENABLE      0x08
#define PADIE_PA4_WAKEUP_ENABLE      0x10
#define PADIE_PA5_WAKEUP_ENABLE      0x20
#define PADIE_PA6_WAKEUP_ENABLE      0x40
#define PADIE_PA7_WAKEUP_ENABLE      0x80

//pbdie definitions
#define PBDIE_PB0_WAKEUP_ENABLE      0x01
#define PBDIE_PB1_WAKEUP_ENABLE      0x02
#define PBDIE_PB2_WAKEUP_ENABLE      0x04
#define PBDIE_PB3_WAKEUP_ENABLE      0x08
#define PBDIE_PB4_WAKEUP_ENABLE      0x10
#define PBDIE_PB5_WAKEUP_ENABLE      0x20
#define PBDIE_PB6_WAKEUP_ENABLE      0x40
#define PBDIE_PB7_WAKEUP_ENABLE      0x80

//misc definitions
#define MISC_WATCHDOG_8K_ILRC        0x00
#define MISC_WATCHDOG_16K_ILRC       0x01
#define MISC_WATCHDOG_64K_ILRC       0x02
#define MISC_WATCHDOG_256K_ILRC      0x03
#define MISC_LVR_DISABLE             0x04
#define MISC_FAST_WAKEUP_ENABLE      0x20

//misc2 definitions
#define MISC2_COMP_EDGE_INT_BOTH     0x00
#define MISC2_COMP_EDGE_INT_RISE     0x20
#define MISC2_COMP_EDGE_INT_FALL     0x40

//misc_lvr definitions
#define MISCLVR_1V8                  0x00
#define MISCLVR_1V9                  0x10
#define MISCLVR_2V                   0x20
#define MISCLVR_2V1                  0x30
#define MISCLVR_2V2                  0x40
#define MISCLVR_2V3                  0x50
#define MISCLVR_2V4                  0x60
#define MISCLVR_2V5                  0x70
#define MISCLVR_2V7                  0x80
#define MISCLVR_3V                   0x90
#define MISCLVR_3V15                 0xA0
#define MISCLVR_3V3                  0xB0
#define MISCLVR_3V5                  0xC0
#define MISCLVR_3V75                 0xD0
#define MISCLVR_4V                   0xE0
#define MISCLVR_4V5                  0xF0
#define MISCLVR_BANDGAP_ON           0x00
#define MISCLVR_BANDGAP_DIV4         0x01
#define MISCLVR_BANDGAP_DIV32        0x02
#define MISCLVR_BANDGAP_AUTO         0x03

//tm2c definitions
#define TM2C_CLK_DISABLE             0x00
#define TM2C_CLK_SYSCLK              0x10
#define TM2C_CLK_IHRC                0x20
#define TM2C_CLK_EOSC                0x30
#define TM2C_CLK_ILRC                0x40
#define TM2C_CLK_COMPOUT             0x50
#define TM2C_CLK_PA0_RISE            0x80
#define TM2C_CLK_PA0_FALL            0x90
#define TM2C_CLK_PB0_RISE            0xA0
#define TM2C_CLK_PB0_FALL            0xB0
#define TM2C_CLK_PA4_RISE            0xC0
#define TM2C_CLK_PA4_FALL            0xD0
#define TM2C_OUT_DISABLE             0x00
#define TM2C_OUT_PB2                 0x04
#define TM2C_OUT_PA3                 0x08
#define TM2C_OUT_PB4                 0x0C
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
#define GPCC_COMP_MINUS_PB6          0x08
#define GPCC_COMP_MINUS_PB7          0x0A
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

//pwmg0c definitions
#define PWMG0C_ENABLE                0x80
#define PWMG0C_OUT_STATUS            0x40
#define PWMG0C_OUT_INVERT            0x20
#define PWMG0C_OUT_PWMG0             0x00
#define PWMG0C_OUT_PWMG0_X_OR_PWMG1  0x10
#define PWMG0C_OUT_NONE              0x00
#define PWMG0C_OUT_PB5               0x02
#define PWMG0C_OUT_PA0               0x06
#define PWMG0C_OUT_PB4               0x08
#define PWMG0C_GEN_XOR               0x00
#define PWMG0C_GEN_OR                0x01

//pwmgclk definitions
#define PWMGCLK_PWMG_ENABLE          0x80
#define PWMGCLK_PWMG_DISABLE         0x00

#define PWMGCLK_PRESCALE_NONE        0x00
#define PWMGCLK_PRESCALE_DIV2        0x01
#define PWMGCLK_PRESCALE_DIV4        0x02
#define PWMGCLK_PRESCALE_DIV8        0x03
#define PWMGCLK_PRESCALE_DIV16       0x04
#define PWMGCLK_PRESCALE_DIV32       0x05
#define PWMGCLK_PRESCALE_DIV64       0x06
#define PWMGCLK_PRESCALE_DIV128      0x07
#define PWMGCLK_CLK_SYSCLK           0x00
#define PWMGCLK_CLK_IHRC             0x01

//pwmg1c definitions
#define PWMG1C_ENABLE                0x80
#define PWMG1C_OUT_STATUS            0x40
#define PWMG1C_OUT_INVERT            0x20
#define PWMG1C_OUT_PWMG1             0x00
#define PWMG1C_OUT_PWMG2             0x10
#define PWMG1C_OUT_NONE              0x00
#define PWMG1C_OUT_PB6               0x02
#define PWMG1C_OUT_PC3               0x04
#define PWMG1C_OUT_PA4               0x06
#define PWMG1C_OUT_PB7               0x08

//pwmg2c definitions
#define PWMG2C_ENABLE                0x80
#define PWMG2C_OUT_STATUS            0x40
#define PWMG2C_OUT_INVERT            0x20
#define PWMG2C_OUT_TOGGLE            0x10
#define PWMG2C_OUT_NONE              0x00
#define PWMG2C_OUT_PB3               0x02
#define PWMG2C_OUT_PC0               0x04
#define PWMG2C_OUT_PA3               0x06
#define PWMG2C_OUT_PB2               0x08
#define PWMG2C_OUT_PA5               0x0A
#define PWMG2C_OUT_PB5               0x0C

//rop definitions
#define ROP_INT_SRC_PB0              0x00
#define ROP_INT_SRC_PA4              0x01
#define ROP_INT_SRC_PA0              0x00
#define ROP_INT_SRC_PB5              0x02
#define ROP_TMX_6BIT                 0x00
#define ROP_TMX_7BIT                 0x10
#define ROP_TMX_16MHZ                0x00
#define ROP_TMX_32MHZ                0x20
#define ROP_PURE_PWM                 0x00
#define ROP_GPC_PWM                  0x40
#define ROP_PWM_16MHZ                0x00
#define ROP_PWM_32MHZ                0x80

#endif //__PMS152_H__
