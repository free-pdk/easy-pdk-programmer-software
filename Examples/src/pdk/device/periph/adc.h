#ifndef __PDK_DEVICE_PERIPH_ADC_H__
#define __PDK_DEVICE_PERIPH_ADC_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" instead of "pdk/device/periph/adc.h" by itself."
#endif

// __sfr definitions
__sfr __at(ADCC_ADDR)         _adcc;
__sfr __at(ADCM_ADDR)         _adcm;

#define ADCC                  _adcc
#define ADCM                  _adcm

#if defined(__PDK_HAS_ADC_12BIT)
  __sfr __at(ADCRH_ADDR)        _adcrh;
  __sfr __at(ADCRL_ADDR)        _adcrl;
  #define ADCRH                 _adcrh
  #define ADCRL                 _adcrl
#else
  __sfr __at(ADCR_ADDR)         _adcr;
  #define ADCR                  _adcr
#endif

#if defined(ADCRGC_ADDR)
  __sfr __at(ADCRGC_ADDR)       _adcrgc;
  #define ADCRGC                _adcrgc
#endif

// ADCC (ADC Control) register definitions
#define ADCC_CHANNEL_SEL_BIT0        2
#define ADCC_PROCESS_CONTROL_BIT     6
#define ADCC_ENABLE_BIT              7

#define ADCC_CH_AD0_PB0              0x00
#define ADCC_CH_AD1_PB1              (1 << ADCC_CHANNEL_SEL_BIT0)
#define ADCC_CH_AD2_PB2              (2 << ADCC_CHANNEL_SEL_BIT0)
#define ADCC_CH_AD3_PB3              (3 << ADCC_CHANNEL_SEL_BIT0)
#define ADCC_CH_AD4_PB4              (4 << ADCC_CHANNEL_SEL_BIT0)
#define ADCC_CH_AD5_PB5              (5 << ADCC_CHANNEL_SEL_BIT0)
#define ADCC_CH_AD6_PB6              (6 << ADCC_CHANNEL_SEL_BIT0)
#define ADCC_CH_AD7_PB7              (7 << ADCC_CHANNEL_SEL_BIT0)
#define ADCC_CH_AD8_PA3              (8 << ADCC_CHANNEL_SEL_BIT0)
#define ADCC_CH_AD9_PA4              (9 << ADCC_CHANNEL_SEL_BIT0)
#define ADCC_CH_AD10_PA0             (10 << ADCC_CHANNEL_SEL_BIT0)
#if defined(__PDK_HAS_PORTC)
  #define ADCC_CH_AD11_PC1             (11 << ADCC_CHANNEL_SEL_BIT0)
  #define ADCC_CH_AD12_PC2             (12 << ADCC_CHANNEL_SEL_BIT0)
#endif
#define ADCC_CH_AD16_BANDGAP         (15 << ADCC_CHANNEL_SEL_BIT0)

#define ADCC_START_ADC_CONV          (1 << ADCC_PROCESS_CONTROL_BIT)
#define ADCC_IS_ADC_CONV_READY       (1 << ADCC_PROCESS_CONTROL_BIT)

#define ADCC_ADC_ENABLE              (1 << ADCC_ENABLE_BIT)


// ADCM (ADC Mode) register definitions
#define ADCM_CLK_SRC_BIT0            1

#define ADCM_CLK_SYSCLK              0x00
#define ADCM_CLK_SYSCLK_DIV2         (1 << ADCM_CLK_SRC_BIT0)
#define ADCM_CLK_SYSCLK_DIV4         (2 << ADCM_CLK_SRC_BIT0)
#define ADCM_CLK_SYSCLK_DIV8         (3 << ADCM_CLK_SRC_BIT0)
#define ADCM_CLK_SYSCLK_DIV16        (4 << ADCM_CLK_SRC_BIT0)
#define ADCM_CLK_SYSCLK_DIV32        (5 << ADCM_CLK_SRC_BIT0)
#define ADCM_CLK_SYSCLK_DIV64        (6 << ADCM_CLK_SRC_BIT0)
#define ADCM_CLK_SYSCLK_DIV128       (7 << ADCM_CLK_SRC_BIT0)

#if defined(__PDK_HAS_ADC_12BIT)
  #define ADCM_BIT_RES_BIT0            5
  #define ADCM_8BIT_RES                0x00
  #define ADCM_9BIT_RES                (1 << ADCM_BIT_RES_BIT0)
  #define ADCM_10BIT_RES               (2 << ADCM_BIT_RES_BIT0)
  #define ADCM_11BIT_RES               (3 << ADCM_BIT_RES_BIT0)
  #define ADCM_12BIT_RES               (4 << ADCM_BIT_RES_BIT0)
#endif

// ADCRGC (ADC Regulator Control) register definitions
#if defined(ADCRGC_ADDR)
  // Might need to change the definition this one uses depending on similarity/difference to other devices
  #if defined(__PDK_HAS_ADC_12BIT)
    #define ADCRGC_AD16_SEL_BIT          4
    #define ADCRGC_ADC_REF_BIT0          5

    #define ADCRGC_AD16_BANDGAP          0x00
    #define ADCRGC_AD16_QTR_VDD          (1 << ADCRGC_AD16_SEL_BIT)

    #define ADCRGC_ADC_REF_VDD           0x00
    #define ADCRGC_ADC_REF_2V            (1 << ADCRGC_ADC_REF_BIT0)
    #define ADCRGC_ADC_REF_3V            (2 << ADCRGC_ADC_REF_BIT0)
    #define ADCRGC_ADC_REF_4V            (3 << ADCRGC_ADC_REF_BIT0)
    #define ADCRGC_ADC_REF_PB1           (4 << ADCRGC_ADC_REF_BIT0)
    #define ADCRGC_ADC_REF_BG            (5 << ADCRGC_ADC_REF_BIT0)
  #else
    #define ADCRGC_ADC_REF_BIT           7

    #define ADCRGC_ADC_REF_VDD           0x00
    #define ADCRGC_ADC_REF_PB1           (1 << ADCRGC_ADC_REF_BIT)
  #endif
#endif

#endif //__PDK_DEVICE_PERIPH_ADC_H__
