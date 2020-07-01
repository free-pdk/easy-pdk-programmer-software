#ifndef __PDK_IO_PFS154_H__
#define __PDK_IO_PFS154_H__

#if !defined(__PDK_IO_H__)
#  error "You must #include "pdk/io.h" instead of "pdk/io_pfs154.h" by itself."
#endif

#ifndef __PDK__IO_XXX_H_
#  define __PDK_IO_XXX_H_ "io_pfs154.h"
#else
#  error "Attempt to include more than one "pdk/io_XXX.h" file."
#endif 

#if !defined __SDCC_pdk14
#  error "PFS154 needs the PDK14 backend. You must compile with the -mpdk14 option."
#endif

#define ILRC_FREQ           55000

// Factory Calibration address locations
#define FACTORY_IHRCR_ADDR  0x07ed
#define FACTORY_BGTR_ADDR   0x07ee


// FUSE definitions
#define FUSE_WORD_ADDR      0x07ff
#define FUSE_RES_BITS_HIGH  0x30FC // - - 1 1   B B 0 D   1 1 1 1   1 1 0 S
// Blank IC Values          0x3FFD // - - 1 1   1 1 1 1   1 1 1 1   1 1 1 1 (Security Off, Normal IO Drive, Fast Boot-up)
#define FUSE_SECURITY_ON    0x0000 //(S)
#define FUSE_SECURITY_OFF   0x0001
#define FUSE_IO_DRV_LOW     0x0000 //(D)
#define FUSE_IO_DRV_NORMAL  0x0100
#define FUSE_BOOTUP_SLOW    0x0000 //(B)
#define FUSE_BOOTUP_FAST    0x0C00


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
#define PWHG1CUBL_ADDR      0x2b
#define PWMG2C_ADDR         0x2c
#define PWMG2S_ADDR         0x2d
#define PWMG2DTH_ADDR       0x2e
#define PWMG2DTL_ADDR       0x2f
#define PWMG2CUBH_ADDR      0x30
#define PWHM2CUBL_ADDR      0x31
#define TM3C_ADDR           0x32
#define TM3CT_ADDR          0x33
#define TM3S_ADDR           0x34
#define TM3B_ADDR           0x35
//0x36
//0x37
//0x38
#define ILRCR_ADDR          0x39
//0x3a
//0x3b
//0x3c
//0x3d
//0x3e
//0x3f


// FLAG (ACC Status Flag) register definitions
#define FLAG_Z_BIT                   0
#define FLAG_C_BIT                   1
#define FLAG_AC_BIT                  2
#define FLAG_OV_BIT                  3

#define FLAG_Z                       (1 << FLAG_Z_BIT)
#define FLAG_C                       (1 << FLAG_C_BIT)
#define FLAG_AC                      (1 << FLAG_AC_BIT)
#define FLAG_OV                      (1 << FLAG_OV_BIT)


// CLKMD (Clock Mode) register definitions
#define CLKMD_PA5_PRSTB_BIT          0
#define CLKMD_WATCHDOG_ENABLE_BIT    1
#define CLKMD_ILRC_ENABLE_BIT        2
#define CLKMD_CLOCKTYPE_BIT          3
#define CLKMD_IHRC_ENABLE_BIT        4
#define CLKMD_SYSCLK_BIT0            5

#define CLKMD_ENABLE_PRSTB           (1 << CLKMD_PA5_PRSTB_BIT)
#define CLKMD_ENABLE_WATCHDOG        (1 << CLKMD_WATCHDOG_ENABLE_BIT)
#define CLKMD_ENABLE_ILRC            (1 << CLKMD_ILRC_ENABLE_BIT)
#define CLKMD_ENABLE_IHRC            (1 << CLKMD_IHRC_ENABLE_BIT)

#define CLKMD_MODE0(s)               (s << CLKMD_SYSCLK_BIT0)
#define CLKMD_IHRC_DIV4              CLKMD_MODE0(0)
#define CLKMD_IHRC_DIV2              CLKMD_MODE0(1)
#define CLKMD_EOSC_DIV4              CLKMD_MODE0(3)
#define CLKMD_EOSC_DIV2              CLKMD_MODE0(4)
#define CLKMD_EOSC                   CLKMD_MODE0(5)
#define CLKMD_ILRC_DIV4              CLKMD_MODE0(6)
#define CLKMD_ILRC                   CLKMD_MODE0(7)

#define CLKMD_MODE1(s)               ((s << CLKMD_SYSCLK_BIT0) & (1 << CLKMD_CLOCKTYPE_BIT))
#define CLKMD_IHRC_DIV16             CLKMD_MODE1(0)
#define CLKMD_IHRC_DIV8              CLKMD_MODE1(1)
#define CLKMD_ILRC_DIV16             CLKMD_MODE1(2)
#define CLKMD_IHRC_DIV32             CLKMD_MODE1(3)
#define CLKMD_IHRC_DIV64             CLKMD_MODE1(4)
#define CLKMD_EOSC_DIV8              CLKMD_MODE1(5)


// INTEN (Interrupt Enable) register definitions
#define INTEN_PA0_ENABLE_BIT         0
#define INTEN_PB0_ENABLE_BIT         1
#define INTEN_T16_ENABLE_BIT         2
#define INTEN_COMP_ENABLE_BIT        4
#define INTEN_PWMG_ENABLE_BIT        5
#define INTEN_TM2_ENABLE_BIT         6
#define INTEN_TM3_ENABLE_BIT         7

#define INTEN_PA0                    (1 << INTEN_PA0_ENABLE_BIT)
#define INTEN_PB0                    (1 << INTEN_PB0_ENABLE_BIT)
#define INTEN_T16                    (1 << INTEN_T16_ENABLE_BIT)
#define INTEN_COMP                   (1 << INTEN_COMP_ENABLE_BIT)
#define INTEN_PWMG                   (1 << INTEN_PWMG_ENABLE_BIT)
#define INTEN_TM2                    (1 << INTEN_TM2_ENABLE_BIT)
#define INTEN_TM3                    (1 << INTEN_TM3_ENABLE_BIT)


// INTRQ (Interrupt Request) register definitions
#define INTRQ_PA0_BIT                0
#define INTRQ_PB0_BIT                1
#define INTRQ_T16_BIT                2
#define INTRQ_COMP_BIT               4
#define INTRQ_PWMG_BI                5
#define INTRQ_TM2_BIT                6
#define INTRQ_TM3_BIT                7

#define INTRQ_PA0                    (1 << INTRQ_PA0_BIT)
#define INTRQ_PB0                    (1 << INTRQ_PB0_BIT)
#define INTRQ_T16                    (1 << INTRQ_T16_BIT)
#define INTRQ_COMP                   (1 << INTRQ_COMP_BIT)
#define INTRQ_PWMG                   (1 << INTRQ_PWMG_BIT)
#define INTRQ_TM2                    (1 << INTRQ_TM2_BIT)
#define INTRQ_TM3                    (1 << INTRQ_TM3_BIT)


// T16 (Timer16 Mode) register definitions
#define T16_INT_SRC_BIT0             0
#define T16_CLK_DIV_BIT0             3
#define T16_CLK_SRC_BIT0             5

#define T16_INTSRC_8BIT              0x00
#define T16_INTSRC_9BIT              (1 << T16_INT_SRC_BIT0)
#define T16_INTSRC_10BIT             (2 << T16_INT_SRC_BIT0)
#define T16_INTSRC_11BIT             (3 << T16_INT_SRC_BIT0)
#define T16_INTSRC_12BIT             (4 << T16_INT_SRC_BIT0)
#define T16_INTSRC_13BIT             (5 << T16_INT_SRC_BIT0)
#define T16_INTSRC_14BIT             (6 << T16_INT_SRC_BIT0)
#define T16_INTSRC_15BIT             (7 << T16_INT_SRC_BIT0)
#define T16_CLK_DIV1                 0x00
#define T16_CLK_DIV4                 (1 << T16_CLK_DIV_BIT0)
#define T16_CLK_DIV16                (2 << T16_CLK_DIV_BIT0)
#define T16_CLK_DIV64                (3 << T16_CLK_DIV_BIT0)
#define T16_CLK_DISABLE              0x00
#define T16_CLK_SYSCLK               (1 << T16_CLK_SRC_BIT0)
#define T16_CLK_PA4_FALL             (3 << T16_CLK_SRC_BIT0)
#define T16_CLK_IHRC                 (4 << T16_CLK_SRC_BIT0)
#define T16_CLK_EOSC                 (5 << T16_CLK_SRC_BIT0)
#define T16_CLK_ILRC                 (6 << T16_CLK_SRC_BIT0)
#define T16_CLK_PA0_FALL             (7 << T16_CLK_SRC_BIT0)


// EOSC (External Oscillator Setting) register definitions
#define EOSC_BG_LVR_SHUTDOWN_BIT     0
#define EOSC_CRYSTAL_SEL_BIT0        5
#define EOSC_ENABLE_BIT              7

#define EOSC_BG_LVR_SHUTDOWN         (1 << EOSC_BG_LVR_SHUTDOWN_BIT)
#define EOSC_32KHZ_CRYSTAL           (1 << EOSC_CRYSTAL_SEL_BIT0)
#define EOSC_1MHZ_CRYSTAL            (2 << EOSC_CRYSTAL_SEL_BIT0)
#define EOSC_4MHZ_CRYSTAL            (3 << EOSC_CRYSTAL_SEL_BIT0)
#define EOSC_ENABLE                  (1 << EOSC_BG_LVR_SHUTDOWN_BIT)


// INTEGS (Interrupt Edge Select) register definitions
#define INTEGS_PA0_EDGE_SEL_BIT0     0
#define INTEGS_PB0_EDGE_SEL_BIT0     2
#define INTEGS_T16_EDGE_SEL_BIT      4

#define INTEGS_PA0_BOTH              0x00
#define INTEGS_PA0_RISING            (1 << INTEGS_PA0_EDGE_SEL_BIT0)
#define INTEGS_PA0_FALLING           (2 << INTEGS_PA0_EDGE_SEL_BIT0)
#define INTEGS_PB0_BOTH              0x00
#define INTEGS_PB0_RISING            (1 << INTEGS_PB0_EDGE_SEL_BIT0)
#define INTEGS_PB0_FALLING           (2 << INTEGS_PB0_EDGE_SEL_BIT0)
#define INTEGS_T16_RISING            0x00
#define INTEGS_T16_FALLING           (0 << INTEGS_T16_EDGE_SEL_BIT)


// PADIE (Port A Digital Input Enable) register definitions
#define PADIE_PA0_BIT                0
#define PADIE_PA3_BIT                3
#define PADIE_PA4_BIT                4
#define PADIE_PA5_BIT                5
#define PADIE_PA6_BIT                6
#define PADIE_PA7_BIT                7

#define PADIE_PA0_WAKEUP_ENABLE      (1 << PADIE_PA0_BIT)
#define PADIE_PA3_WAKEUP_ENABLE      (1 << PADIE_PA3_BIT)
#define PADIE_PA4_WAKEUP_ENABLE      (1 << PADIE_PA4_BIT)
#define PADIE_PA5_WAKEUP_ENABLE      (1 << PADIE_PA5_BIT)
#define PADIE_PA6_WAKEUP_ENABLE      (1 << PADIE_PA6_BIT)
#define PADIE_PA7_WAKEUP_ENABLE      (1 << PADIE_PA7_BIT)


// PBDIE (Port B Digital Input Enable) register definitions
#define PADIE_PB0_BIT                0
#define PADIE_PB1_BIT                1
#define PADIE_PB2_BIT                2
#define PADIE_PB3_BIT                3
#define PADIE_PB4_BIT                4
#define PADIE_PB5_BIT                5
#define PADIE_PB6_BIT                6
#define PADIE_PB7_BIT                7

#define PADIE_PB0_WAKEUP_ENABLE      (1 << PADIE_PB0_BIT)
#define PADIE_PB1_WAKEUP_ENABLE      (1 << PADIE_PB1_BIT)
#define PADIE_PB2_WAKEUP_ENABLE      (1 << PADIE_PB2_BIT)
#define PADIE_PB3_WAKEUP_ENABLE      (1 << PADIE_PB3_BIT)
#define PADIE_PB4_WAKEUP_ENABLE      (1 << PADIE_PB4_BIT)
#define PADIE_PB5_WAKEUP_ENABLE      (1 << PADIE_PB5_BIT)
#define PADIE_PB6_WAKEUP_ENABLE      (1 << PADIE_PB6_BIT)
#define PADIE_PB7_WAKEUP_ENABLE      (1 << PADIE_PB7_BIT)


// MISC register definitions
#define MISC_WATCHDOG_BIT0           0
#define MISC_LVR_DISABLE_BIT         2
#define MISC_LCD_ENABLE_BIT          4
#define MISC_FAST_WAKEUP_ENABLE_BIT  5

#define MISC_WATCHDOG_8K_ILRC        0x00
#define MISC_WATCHDOG_16K_ILRC       (1 << MISC_WATCHDOG_BIT0)
#define MISC_WATCHDOG_64K_ILRC       (2 << MISC_WATCHDOG_BIT0)
#define MISC_WATCHDOG_256K_ILRC      (3 << MISC_WATCHDOG_BIT0)
#define MISC_LVR_DISABLE             (1 << MISC_LVR_DISABLE_BIT)
#define MISC_LCD_ENABLE              (1 << MISC_LCD_ENABLE_BIT)
#define MISC_FAST_WAKEUP_ENABLE      (1 << MISC_FAST_WAKEUP_ENABLE_BIT)


// MISC2 register definitions
#define MISC2_COMP_EDGE_SEL_BIT0     5

#define MISC2_COMP_EDGE_INT_BOTH     0x00
#define MISC2_COMP_EDGE_INT_RISE     (1 << MISC2_COMP_EDGE_SEL_BIT0)
#define MISC2_COMP_EDGE_INT_FALL     (2 << MISC2_COMP_EDGE_SEL_BIT0)
//#define MISC2_UNK_TM3                0x00
//#define MISC2_UNK_PWMG2              0x10
//#define MISC2_UNK_COMP               0x00
//#define MISC2_UNK_PWMG1              0x08

// MISCLVR register definitions
#define MISCLVR_LVR_BIT0             5

#define MISCLVR_4V                   0x00
#define MISCLVR_3V5                  (1 << MISCLVR_LVR_BIT0)
#define MISCLVR_3V                   (2 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V75                 (3 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V5                  (4 << MISCLVR_LVR_BIT0)
#define MISCLVR_1V8                  (5 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V2                  (6 << MISCLVR_LVR_BIT0)
#define MISCLVR_2V                   (7 << MISCLVR_LVR_BIT0)

// TM2C (Timer2 Control) register definitions
#define TM2C_INVERT_OUT_BIT          0
#define TM2C_MODE_SEL_BIT            1
#define TM2C_OUTPUT_SEL_BIT0         2
#define TM2C_CLK_SRC_BIT0            4

#define TM2C_INVERT_OUT              (1 << TM2C_INVERT_OUT_BIT)
#define TM2C_MODE_PERIOD             0x00
#define TM2C_MODE_PWM                (1 << TM2C_MODE_SEL_BIT)
#define TM2C_OUT_DISABLE             0x00
#define TM2C_OUT_PB2                 (1 << TM2C_OUTPUT_SEL_BIT0)
#define TM2C_OUT_PA3                 (2 << TM2C_OUTPUT_SEL_BIT0)
#define TM2C_OUT_PB4                 (3 << TM2C_OUTPUT_SEL_BIT0)
#define TM2C_CLK_DISABLE             0x00
#define TM2C_CLK_SYSCLK              (1 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_IHRC                (2 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_EOSC                (3 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_ILRC                (4 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_COMPOUT             (5 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_PA0_RISE            (8 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_PA0_FALL            (9 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_PB0_RISE            (10 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_PB0_FALL            (11 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_PA4_RISE            (12 << TM2C_CLK_SRC_BIT0)
#define TM2C_CLK_PA4_FALL            (13 << TM2C_CLK_SRC_BIT0)


// TM2S (Timer2 Scalar) register definitions
#define TM2S_SCALE_BIT0              0
#define TM2S_PRESCALE_BIT0           5
#define TM2S_PWM_RES_SEL_BIT         7

#define TM2S_SCALE_NONE              0x00
#define TM2S_SCALE_DIV2              (1 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV3              (2 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV4              (3 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV5              (4 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV6              (5 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV7              (6 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV8              (7 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV9              (8 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV10             (9 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV11             (10 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV12             (11 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV13             (12 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV14             (13 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV15             (14 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV16             (15 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV17             (16 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV18             (17 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV19             (18 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV20             (19 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV21             (20 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV22             (21 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV23             (22 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV24             (23 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV25             (24 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV26             (25 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV27             (26 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV28             (27 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV29             (28 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV30             (29 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV31             (30 << TM2S_SCALE_BIT0)
#define TM2S_SCALE_DIV32             (31 << TM2S_SCALE_BIT0)
#define TM2S_PRESCALE_NONE           0x00
#define TM2S_PRESCALE_DIV4           (1 << TM2S_PRESCALE_BIT0)
#define TM2S_PRESCALE_DIV16          (2 << TM2S_PRESCALE_BIT0)
#define TM2S_PRESCALE_DIV64          (3 << TM2S_PRESCALE_BIT0)
#define TM2S_PWM_RES_8BIT            0x00
#define TM2S_PWM_RES_6BIT            (1 << TM2S_PWM_RES_SEL_BIT)


// TM3C (Timer3 Control) register definitions
#define TM3C_INVERT_OUT_BIT          0
#define TM3C_MODE_SEL_BIT            1
#define TM3C_OUTPUT_SEL_BIT0         2
#define TM3C_CLK_SRC_BIT0            4

#define TM3C_INVERT_OUT              (1 << TM3C_INVERT_OUT_BIT)
#define TM3C_MODE_PERIOD             0x00
#define TM3C_MODE_PWM                (1 << TM3C_MODE_SEL_BIT)
#define TM3C_OUT_DISABLE             0x00
#define TM3C_OUT_PB2                 (1 << TM3C_OUTPUT_SEL_BIT0)
#define TM3C_OUT_PA3                 (2 << TM3C_OUTPUT_SEL_BIT0)
#define TM3C_OUT_PB4                 (3 << TM3C_OUTPUT_SEL_BIT0)
#define TM3C_CLK_DISABLE             0x00
#define TM3C_CLK_SYSCLK              (1 << TM3C_CLK_SRC_BIT0)
#define TM3C_CLK_IHRC                (2 << TM3C_CLK_SRC_BIT0)
#define TM3C_CLK_EOSC                (3 << TM3C_CLK_SRC_BIT0)
#define TM3C_CLK_ILRC                (4 << TM3C_CLK_SRC_BIT0)
#define TM3C_CLK_COMPOUT             (5 << TM3C_CLK_SRC_BIT0)
#define TM3C_CLK_PA0_RISE            (8 << TM3C_CLK_SRC_BIT0)
#define TM3C_CLK_PA0_FALL            (9 << TM3C_CLK_SRC_BIT0)
#define TM3C_CLK_PB0_RISE            (10 << TM3C_CLK_SRC_BIT0)
#define TM3C_CLK_PB0_FALL            (11 << TM3C_CLK_SRC_BIT0)
#define TM3C_CLK_PA4_RISE            (12 << TM3C_CLK_SRC_BIT0)
#define TM3C_CLK_PA4_FALL            (13 << TM3C_CLK_SRC_BIT0)


// TM3S (Timer3 Scalar) register definitions
#define TM3S_SCALE_BIT0              0
#define TM3S_PRESCALE_BIT0           5
#define TM3S_PWM_RES_SEL_BIT         7

#define TM3S_SCALE_NONE              0x00
#define TM3S_SCALE_DIV2              (1 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV3              (2 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV4              (3 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV5              (4 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV6              (5 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV7              (6 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV8              (7 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV9              (8 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV10             (9 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV11             (10 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV12             (11 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV13             (12 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV14             (13 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV15             (14 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV16             (15 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV17             (16 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV18             (17 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV19             (18 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV20             (19 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV21             (20 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV22             (21 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV23             (22 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV24             (23 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV25             (24 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV26             (25 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV27             (26 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV28             (27 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV29             (28 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV30             (29 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV31             (30 << TM3S_SCALE_BIT0)
#define TM3S_SCALE_DIV32             (31 << TM3S_SCALE_BIT0)
#define TM3S_PRESCALE_NONE           0x00
#define TM3S_PRESCALE_DIV4           (1 << TM3S_PRESCALE_BIT0)
#define TM3S_PRESCALE_DIV16          (2 << TM3S_PRESCALE_BIT0)
#define TM3S_PRESCALE_DIV64          (3 << TM3S_PRESCALE_BIT0)
#define TM3S_PWM_RES_8BIT            0x00
#define TM3S_PWM_RES_6BIT            (1 << TM3S_PWM_RES_SEL_BIT)


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
#define GPCC_COMP_MINUS_PB6          (4 << GPCC_COMP_MINUS_BIT0)
#define GPCC_COMP_MINUS_PB7          (5 << GPCC_COMP_MINUS_BIT0)
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
//#define GPCS_COMP_WAKEUP_ENABLE      0x40
#define GPCS_COMP_OUTPUT_PA0         (1 << GPCS_COMP_OUT_PA0_BIT)


// PWMG0C (PWMG0 Control) register definitions
#define PWMG0C_CLK_SRC_BIT           0
#define PWMG0C_OUT_PIN_SEL_BIT0      1
#define PWMG0C_CTR_RST_BIT           4
#define PWMG0C_INVERT_OUT_BIT        5
#define PWMG0C_STATUS_OUT_BIT        6
#define PWMG0C_ENABLE_BIT            7

#define PWMG0C_CLK_SYSCLK            0x00
#define PWMG0C_CLK_IHRC              (1 << PWMG0C_CLK_SRC_BIT)
#define PWMG0C_OUT_NONE              0x00
#define PWMG0C_OUT_PB5               (1 << PWMG0C_OUT_PIN_SEL_BIT0)
#define PWMG0C_OUT_PA0               (2 << PWMG0C_OUT_PIN_SEL_BIT0)
#define PWMG0C_OUT_PB4               (3 << PWMG0C_OUT_PIN_SEL_BIT0)
#define PWMG0C_RESET_COUNTER         (1 << PWMG0C_CTR_RST_BIT)
#define PWMG0C_INVERT_OUT            (1 << PWMG0C_INVERT_OUT_BIT)
#define PWMG0C_STATUS_OUT            (1 << PWMG0C_STATUS_OUT_BIT)
#define PWMG0C_ENABLE                (1 << PWMG0C_ENABLE_BIT)


// PWMG0S (PWMG0 Scalar) register definitions
#define PWMG0S_CLK_DIV_BIT0          0
#define PWMG0S_PRESCALE_BIT0         5
#define PWMG0S_INT_MODE_BIT          7

#define PWMG0S_SCALE_NONE            0x00
#define PWMG0S_SCALE_DIV2            (1 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV3            (2 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV4            (3 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV5            (4 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV6            (5 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV7            (6 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV8            (7 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV9            (8 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV10           (9 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV11           (10 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV12           (11 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV13           (12 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV14           (13 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV15           (14 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV16           (15 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV17           (16 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV18           (17 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV19           (18 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV20           (19 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV21           (20 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV22           (21 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV23           (22 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV24           (23 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV25           (24 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV26           (25 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV27           (26 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV28           (27 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV29           (28 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV30           (29 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV31           (30 << PWMG0S_SCALE_BIT0)
#define PWMG0S_SCALE_DIV32           (31 << PWMG0S_SCALE_BIT0)
#define PWMG0S_PRESCALE_NONE         0x00
#define PWMG0S_PRESCALE_DIV4         (1 << PWMG0S_PRESCALE_BIT0)
#define PWMG0S_PRESCALE_DIV16        (2 << PWMG0S_PRESCALE_BIT0)
#define PWMG0S_PRESCALE_DIV64        (3 << PWMG0S_PRESCALE_BIT0)
#define PWMG0_INT_AT_DUTY            0x00
#define PWMG0_INT_AT_0               (1 << PWMG0S_INT_MODE_BIT)


// PWMG1C (PWMG1 Control) register definitions
#define PWMG1C_CLK_SRC_BIT           0
#define PWMG1C_OUT_PIN_SEL_BIT0      1
#define PWMG1C_CTR_RST_BIT           4
#define PWMG1C_INVERT_OUT_BIT        5
#define PWMG1C_STATUS_OUT_BIT        6
#define PWMG1C_ENABLE_BIT            7

#define PWMG1C_CLK_SYSCLK            0x00
#define PWMG1C_CLK_IHRC              (1 << PWMG1C_CLK_SRC_BIT)
#define PWMG1C_OUT_NONE              0x00
#define PWMG1C_OUT_PB5               (1 << PWMG1C_OUT_PIN_SEL_BIT0)
#define PWMG1C_OUT_PA0               (2 << PWMG1C_OUT_PIN_SEL_BIT0)
#define PWMG1C_OUT_PB4               (3 << PWMG1C_OUT_PIN_SEL_BIT0)
#define PWMG1C_RESET_COUNTER         (1 << PWMG1C_CTR_RST_BIT)
#define PWMG1C_INVERT_OUT            (1 << PWMG1C_INVERT_OUT_BIT)
#define PWMG1C_STATUS_OUT            (1 << PWMG1C_STATUS_OUT_BIT)
#define PWMG1C_ENABLE                (1 << PWMG1C_ENABLE_BIT)


// PWMG1S (PWMG1 Scalar) register definitions
#define PWMG1S_CLK_DIV_BIT0          0
#define PWMG1S_PRESCALE_BIT0         5
#define PWMG1S_INT_MODE_BIT          7

#define PWMG1S_SCALE_NONE            0x00
#define PWMG1S_SCALE_DIV2            (1 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV3            (2 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV4            (3 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV5            (4 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV6            (5 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV7            (6 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV8            (7 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV9            (8 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV10           (9 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV11           (10 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV12           (11 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV13           (12 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV14           (13 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV15           (14 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV16           (15 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV17           (16 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV18           (17 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV19           (18 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV20           (19 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV21           (20 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV22           (21 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV23           (22 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV24           (23 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV25           (24 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV26           (25 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV27           (26 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV28           (27 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV29           (28 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV30           (29 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV31           (30 << PWMG1S_SCALE_BIT0)
#define PWMG1S_SCALE_DIV32           (31 << PWMG1S_SCALE_BIT0)
#define PWMG1S_PRESCALE_NONE         0x00
#define PWMG1S_PRESCALE_DIV4         (1 << PWMG1S_PRESCALE_BIT0)
#define PWMG1S_PRESCALE_DIV16        (2 << PWMG1S_PRESCALE_BIT0)
#define PWMG1S_PRESCALE_DIV64        (3 << PWMG1S_PRESCALE_BIT0)
#define PWMG1_INT_AT_DUTY            0x00
#define PWMG1_INT_AT_0               (1 << PWMG1S_INT_MODE_BIT)


// PWMG2C (PWMG2 Control) register definitions
#define PWMG2C_CLK_SRC_BIT           0
#define PWMG2C_OUT_PIN_SEL_BIT0      1
#define PWMG2C_CTR_RST_BIT           4
#define PWMG2C_INVERT_OUT_BIT        5
#define PWMG2C_STATUS_OUT_BIT        6
#define PWMG2C_ENABLE_BIT            7

#define PWMG2C_CLK_SYSCLK            0x00
#define PWMG2C_CLK_IHRC              (1 << PWMG2C_CLK_SRC_BIT)
#define PWMG2C_OUT_NONE              0x00
#define PWMG2C_OUT_PB5               (1 << PWMG2C_OUT_PIN_SEL_BIT0)
#define PWMG2C_OUT_PA0               (2 << PWMG2C_OUT_PIN_SEL_BIT0)
#define PWMG2C_OUT_PB4               (3 << PWMG2C_OUT_PIN_SEL_BIT0)
#define PWMG2C_RESET_COUNTER         (1 << PWMG2C_CTR_RST_BIT)
#define PWMG2C_INVERT_OUT            (1 << PWMG2C_INVERT_OUT_BIT)
#define PWMG2C_STATUS_OUT            (1 << PWMG2C_STATUS_OUT_BIT)
#define PWMG2C_ENABLE                (1 << PWMG2C_ENABLE_BIT)


// PWMG2S (PWMG2 Scalar) register definitions
#define PWMG2S_CLK_DIV_BIT0          0
#define PWMG2S_PRESCALE_BIT0         5
#define PWMG2S_INT_MODE_BIT          7

#define PWMG2S_SCALE_NONE            0x00
#define PWMG2S_SCALE_DIV2            (1 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV3            (2 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV4            (3 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV5            (4 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV6            (5 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV7            (6 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV8            (7 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV9            (8 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV10           (9 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV11           (10 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV12           (11 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV13           (12 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV14           (13 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV15           (14 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV16           (15 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV17           (16 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV18           (17 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV19           (18 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV20           (19 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV21           (20 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV22           (21 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV23           (22 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV24           (23 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV25           (24 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV26           (25 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV27           (26 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV28           (27 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV29           (28 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV30           (29 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV31           (30 << PWMG2S_SCALE_BIT0)
#define PWMG2S_SCALE_DIV32           (31 << PWMG2S_SCALE_BIT0)
#define PWMG2S_PRESCALE_NONE         0x00
#define PWMG2S_PRESCALE_DIV4         (1 << PWMG2S_PRESCALE_BIT0)
#define PWMG2S_PRESCALE_DIV16        (2 << PWMG2S_PRESCALE_BIT0)
#define PWMG2S_PRESCALE_DIV64        (3 << PWMG2S_PRESCALE_BIT0)
#define PWMG2_INT_AT_DUTY            0x00
#define PWMG2_INT_AT_0               (1 << PWMG2S_INT_MODE_BIT)

#endif //__PDK_IO_PFS154_H__
