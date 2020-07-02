#ifndef __PDK_PERIPH_PWMG0_H__
#define __PDK_PERIPH_PWMG0_H__

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

#endif //__PDK_PERIPH_PWMG0_H__
