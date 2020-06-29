/* USER CODE BEGIN Header */
/*
Copyright (C) 2019  freepdk  https://free-pdk.github.io

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_OUT_Pin GPIO_PIN_13
#define LED1_OUT_GPIO_Port GPIOC
#define LED2_OUT_Pin GPIO_PIN_14
#define LED2_OUT_GPIO_Port GPIOC
#define LED3_OUT_Pin GPIO_PIN_15
#define LED3_OUT_GPIO_Port GPIOC
#define DCDC15VOLT_ENABLE_OUT_Pin GPIO_PIN_2
#define DCDC15VOLT_ENABLE_OUT_GPIO_Port GPIOA
#define PWM_MINUS4VOLT_OUT_Pin GPIO_PIN_3
#define PWM_MINUS4VOLT_OUT_GPIO_Port GPIOA
#define USER_BTN_IN_Pin GPIO_PIN_15
#define USER_BTN_IN_GPIO_Port GPIOA
#define IC_IO_PA3_CLK_Pin GPIO_PIN_3
#define IC_IO_PA3_CLK_GPIO_Port GPIOB
#define IC_IO_PA4_Pin GPIO_PIN_4
#define IC_IO_PA4_GPIO_Port GPIOB
#define IC_IO_PA6_DAT_Pin GPIO_PIN_5
#define IC_IO_PA6_DAT_GPIO_Port GPIOB
#define IC_IO_PA0_UART1_TX_Pin GPIO_PIN_6
#define IC_IO_PA0_UART1_TX_GPIO_Port GPIOB
#define IC_IO_PA7_USART1_RX_Pin GPIO_PIN_7
#define IC_IO_PA7_USART1_RX_GPIO_Port GPIOB
#define HW_VARIANT_DET0_Pin GPIO_PIN_8
#define HW_VARIANT_DET0_GPIO_Port GPIOB
#define HW_VARIANT_DET1_Pin GPIO_PIN_9
#define HW_VARIANT_DET1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
