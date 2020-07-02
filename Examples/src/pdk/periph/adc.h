#ifndef __PDK_PERIPH_ADC_H__
#define __PDK_PERIPH_ADC_H__

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
#define ADCC_CH_AD11_PC1             (11 << ADCC_CHANNEL_SEL_BIT0)
#define ADCC_CH_AD12_PC2             (12 << ADCC_CHANNEL_SEL_BIT0)
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

// ADCRGC (ADC Regulator Control) register definitions
#define ADCRGC_CLK_SRC_BIT           7

#define ADCRGC_ADC_REF_VDD           0x00
#define ADCRGC_ADC_REF_PB1           (1 << ADCRGC_CLK_SRC_BIT)

#endif //__PDK_PERIPH_ADC_H__
