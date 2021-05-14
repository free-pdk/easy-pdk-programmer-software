#ifndef __PMS171B_H__
#define __PMS171B_H__

#ifndef PMS171B
#define PMS171B
#endif
#if !defined __SDCC_pdk14
#error "PMS171B needs PDK14 backend. Compile with -mpdk14"
#endif

#include "pdkcommon.h"

//fuse definitions
#define FUSE_SECURITY_ON       0x0000
#define FUSE_SECURITY_OFF      0x0001 //(S)
#define FUSE_PB4PB5_DRV_NORMAL 0x0000 //(D)
#define FUSE_PB4PB5_DRV_STRONG 0x0100
#define FUSE_BOOTUP_SLOW       0x0000 //(B)
#define FUSE_BOOTUP_FAST       0x0600
#define FUSE_RES_BITS_HIGH     0x10FC // - - 0 1   B B 0 D   1 1 1 1   1 1 0 S
// Blank IC Values             0x1FFD // - - 0 1   1 1 1 1   1 1 1 1   1 1 0 1 (Security off, High PB4/PB5 Drive, Fast Boot-up)
#define EASY_PDK_FUSE(f) { __asm__(".area FUSE (ABS)\n.org (0x5ff*2)\n.word ("_ASMD(FUSE_RES_BITS_HIGH)"|"_ASMD(f)")\n.area CODE\n"); }

//set calibration macros
#define EASY_PDK_CALIBRATE_IHRC(frequency, millivolt)                          \
  EASY_PDK_CALIBRATE_RC_M(EASY_PDK_CALTYPE_IHRC, 0x0B, frequency, millivolt)
#define EASY_PDK_CALIBRATE_ILRC(frequency, millivolt)                          \
  EASY_PDK_CALIBRATE_RC_M(EASY_PDK_CALTYPE_ILRC, 0x39, frequency, millivolt)
#define EASY_PDK_CALIBRATE_BG() EASY_PDK_CALIBRATE_BG_M(0x1a, 0x18, 0x19)
#define EASY_PDK_USE_FACTORY_BGTR() { __asm__("call #0x5f6\n mov "_ASMV(BGTR)",a\n"); }

#define ILRC_FREQ  50000

#define EASY_PDK_INIT_SYSCLOCK_16MHZ()      {_misclvr=MISCLVR_4V5;_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_IHRC;}
#define EASY_PDK_INIT_SYSCLOCK_8MHZ()       {_misclvr=MISCLVR_3V;_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_IHRC_DIV2;}
#define EASY_PDK_INIT_SYSCLOCK_4MHZ()       {_misclvr=MISCLVR_2V2;_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_IHRC_DIV4;}
#define EASY_PDK_INIT_SYSCLOCK_2MHZ()       {_misclvr=MISCLVR_1V8;_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_IHRC_DIV8;}
#define EASY_PDK_INIT_SYSCLOCK_1MHZ()       {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_IHRC_DIV16;}
#define EASY_PDK_INIT_SYSCLOCK_500KHZ()     {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_IHRC_DIV32;}
#define EASY_PDK_INIT_SYSCLOCK_ILRC()       {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_ILRC;}
#define EASY_PDK_INIT_SYSCLOCK_ILRC_DIV4()  {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_ILRC_DIV4;}
#define EASY_PDK_INIT_SYSCLOCK_ILRC_DIV16() {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_ILRC_DIV16;}

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
__sfr __at(0x0e) _pbdier;
__sfr __at(0x0f) _misc2;
__sfr __at(0x10) _pa;
__sfr __at(0x11) _pac;
__sfr __at(0x12) _paph;
//0x13
__sfr __at(0x14) _pb;
__sfr __at(0x15) _pbc;
__sfr __at(0x16) _pbph;
__sfr __at(0x17) _misc;
__sfr __at(0x18) _gpcc;
__sfr __at(0x19) _gpcs;
__sfr __at(0x1a) _bgtr;
//0x1
__sfr __at(0x1c) _tm2c;
__sfr __at(0x1d) _tm2ct;
__sfr __at(0x1e) _tm2s;
//0x1f
//0x20
//0x21
//0x22
//0x23
//0x24
//0x25
//0x26
//0x27
//0x28
//0x29
//0x2a
//0x2b
//0x2c
//0x2d
//0x2e
//0x2f
//0x30
//0x31
__sfr __at(0x32) _tm3c;
__sfr __at(0x33) _tm3ct;
__sfr __at(0x34) _tm3s;
__sfr __at(0x35) _misclvr;
//0x36
//0x37
__sfr __at(0x38) _pbpl;
__sfr __at(0x39) _ilrcr;
__sfr __at(0x3a) _rop;
__sfr __at(0x3b) _adcc;
__sfr __at(0x3c) _adcm;
__sfr __at(0x3d) _adcrgc;
__sfr __at(0x3e) _adcr;
__sfr __at(0x3f) _tm3b;

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
#define PBPL      _pbpl
#define MISC      _misc
#define TM2S      _tm2s
#define GPCC      _gpcc
#define GPCS      _gpcs
#define BGTR      _bgtr
#define MISCLVR   _misclvr
#define TM2C      _tm2c
#define TM2CT     _tm2ct
#define ROP       _rop
#define TM3C      _tm3c
#define TM3CT     _tm3ct
#define TM3S      _tm3s
#define TM3B      _tm3b
#define ILRCR     _ilrcr
#define ADCC      _adcc
#define ADCM      _adcm
#define ADCRGC    _adcrgc
#define ADCR      _adcr
#define T16C      _t16c

//flag definitions
#define FLAG_ZF 0x01
#define FLAG_CF 0x02
#define FLAG_AC 0x04
#define FLAG_OV 0x08

//clkmd definitions
#define CLKMD_ENABLE_PRSTB           0x01
#define CLKMD_ENABLE_WATCHDOG        0x02
#define CLKMD_ENABLE_ILRC            0x04
#define CLKMD_ENABLE_IHRC            0x10
#define CLKMD_IHRC                   0x40
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
#define INTEN_ADC                    0x08
#define INTEN_COMP                   0x10
#define INTEN_TM2                    0x40
#define INTEN_TM3                    0x80

//interrupt request definitions
#define INTRQ_PA0                    0x01
#define INTRQ_PB5                    0x01
#define INTRQ_PB0                    0x02
#define INTRQ_PA4                    0x02
#define INTRQ_T16                    0x04
#define INTRQ_ADC                    0x08
#define INTRQ_COMP                   0x10
#define INTRQ_TM2                    0x40
#define INTRQ_TM3                    0x80

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

//adcc definitions
#define ADCC_ADC_ENABLE              0x80
#define ADCC_ADC_CONV_START          0x40
#define ADCC_ADC_CONV_COMPLETE       0x40
#define ADCC_CH_AD0_PB0              0x00
#define ADCC_CH_AD1_PB1              0x04
#define ADCC_CH_AD2_PB2              0x08
#define ADCC_CH_AD3_PB3              0x0C
#define ADCC_CH_AD4_PB4              0x10
#define ADCC_CH_AD5_PB5              0x14
#define ADCC_CH_AD6_PB6              0x18
#define ADCC_CH_AD7_PB7              0x1C
#define ADCC_CH_AD8_PA3              0x20
#define ADCC_CH_AD9_PA4              0x24
#define ADCC_CH_AD10_PA0             0x28
#define ADCC_CH_AD15_BANDGAP         0x3C

//adcm definitions
#define ADCM_CLK_SYSCLK              0x00
#define ADCM_CLK_SYSCLK_DIV2         0x02
#define ADCM_CLK_SYSCLK_DIV4         0x04
#define ADCM_CLK_SYSCLK_DIV8         0x06
#define ADCM_CLK_SYSCLK_DIV16        0x08
#define ADCM_CLK_SYSCLK_DIV32        0x0A
#define ADCM_CLK_SYSCLK_DIV64        0x0C
#define ADCM_CLK_SYSCLK_DIV128       0x0E

//misc definitions
#define MISC_WATCHDOG_8K_ILRC        0x00
#define MISC_WATCHDOG_16K_ILRC       0x01
#define MISC_WATCHDOG_64K_ILRC       0x02
#define MISC_WATCHDOG_256K_ILRC      0x03
#define MISC_LVR_DISABLE             0x04
#define MISC_FAST_WAKEUP_ENABLE      0x20
#define MISC_EC_DRIVE_HIGH           0x00
#define MISC_EC_DRIVE_LOW            0x40

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
#define MISCLVR_2V75                 0x80
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

//tm3c definitions
#define TM3C_CLK_DISABLE             0x00
#define TM3C_CLK_SYSCLK              0x10
#define TM3C_CLK_IHRC                0x20
#define TM3C_CLK_EOSC                0x30
#define TM3C_CLK_ILRC                0x40
#define TM3C_CLK_COMPOUT             0x50
#define TM3C_CLK_PA0_RISE            0x80
#define TM3C_CLK_PA0_FALL            0x90
#define TM3C_CLK_PB0_RISE            0xA0
#define TM3C_CLK_PB0_FALL            0xB0
#define TM3C_CLK_PA4_RISE            0xC0
#define TM3C_CLK_PA4_FALL            0xD0
#define TM3C_OUT_DISABLE             0x00
#define TM3C_OUT_PB5                 0x04
#define TM3C_OUT_PB6                 0x08
#define TM3C_OUT_PB7                 0x0C
#define TM3C_MODE_PERIOD             0x00
#define TM3C_MODE_PWM                0x02
#define TM3C_INVERT_OUT              0x01

//tm3s definitions
#define TM3S_PWM_RES_8BIT            0x00
#define TM3S_PWM_RES_6BIT            0x80
#define TM3S_PRESCALE_NONE           0x00
#define TM3S_PRESCALE_DIV4           0x20
#define TM3S_PRESCALE_DIV16          0x40
#define TM3S_PRESCALE_DIV64          0x60
#define TM3S_SCALE_NONE              0x00
#define TM3S_SCALE_DIV2              0x01
#define TM3S_SCALE_DIV3              0x02
#define TM3S_SCALE_DIV4              0x03
#define TM3S_SCALE_DIV5              0x04
#define TM3S_SCALE_DIV6              0x05
#define TM3S_SCALE_DIV7              0x06
#define TM3S_SCALE_DIV8              0x07
#define TM3S_SCALE_DIV9              0x08
#define TM3S_SCALE_DIV10             0x09
#define TM3S_SCALE_DIV11             0x0A
#define TM3S_SCALE_DIV12             0x0B
#define TM3S_SCALE_DIV13             0x0C
#define TM3S_SCALE_DIV14             0x0D
#define TM3S_SCALE_DIV15             0x0E
#define TM3S_SCALE_DIV16             0x0F
#define TM3S_SCALE_DIV17             0x10
#define TM3S_SCALE_DIV18             0x11
#define TM3S_SCALE_DIV19             0x12
#define TM3S_SCALE_DIV20             0x13
#define TM3S_SCALE_DIV21             0x14
#define TM3S_SCALE_DIV22             0x15
#define TM3S_SCALE_DIV23             0x16
#define TM3S_SCALE_DIV24             0x17
#define TM3S_SCALE_DIV25             0x18
#define TM3S_SCALE_DIV26             0x19
#define TM3S_SCALE_DIV27             0x1A
#define TM3S_SCALE_DIV28             0x1B
#define TM3S_SCALE_DIV29             0x1C
#define TM3S_SCALE_DIV30             0x1D
#define TM3S_SCALE_DIV31             0x1E
#define TM3S_SCALE_DIV32             0x1F

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

//rop definitions
#define ROP_INT_SRC_PB0              0x00
#define ROP_INT_SRC_PA4              0x01
#define ROP_INT_SRC_PA0              0x00
#define ROP_INT_SRC_PB5              0x02
#define ROP_TMX_6BIT                 0x00
#define ROP_TMX_7BIT                 0x20
#define ROP_TMX_16MHZ                0x00
#define ROP_TMX_32MHZ                0x80
#define ROP_GPC_PWM                  0x40
#define ROP_TM2_Out1_PB0             0x10
#define ROP_TM2_Out1_PB2             0x00

#endif //__PMS171B_H__
