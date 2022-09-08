#ifndef __BSP_KEY_H_
#define __BSP_KEY_H_

#include "stm32f1xx.h"
#include "bsp_led.h"

#define KEY_ON 1
#define KEY_OFF 0

// 初始化KEY对应的GPIO引脚
void KEY_GPIO_Init(void);

uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN);

#endif