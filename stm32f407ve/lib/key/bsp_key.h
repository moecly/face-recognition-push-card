#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/*  To use hal function  */
#include "stm32f4xx.h"

/*****************************************************************************/
/*  Key pin  */
#define KEY1_PIN                     GPIO_PIN_3
#define KEY2_PIN                     GPIO_PIN_4

/*  Key port  */
#define KEY1_PORT                    GPIOE
#define KEY2_PORT                    GPIOE

/*  Key status  */
#define KEY_ON                      1
#define KEY_OFF                     0

/*  Key function  */
#define KEY1_CLK_ENABLE              __HAL_RCC_GPIOE_CLK_ENABLE
#define KEY2_CLK_ENABLE              __HAL_RCC_GPIOE_CLK_ENABLE
/*****************************************************************************/

void KEY_Init(void);  /*  KEY init  */
uint16_t Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN_x);

#endif