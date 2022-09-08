#ifndef __BSP_LED_H_
#define __BSP_LED_H_

#include "stm32f1xx.h"

#define LED_D1_ON HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
#define LED_D1_OFF HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
#define LED_D1_TogglePin HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
/*#define LED_D2_ON HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
#define LED_D2_OFF HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
#define LED_D2_TogglePin HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);*/

// 初始化LED灯对应的GPIO引脚
void LED_GPIO_Init(void);

#endif