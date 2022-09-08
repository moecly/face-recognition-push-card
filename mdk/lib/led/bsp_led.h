#ifndef __BSP_LED_H__
#define __BSP_LED_H__

/*  To use hal function  */
#include "stm32f4xx.h"

/*****************************************************************************/
/*  Led pin  */
#define LED1_PIN                     GPIO_PIN_6
#define LED2_PIN                     GPIO_PIN_7
/*  Led port  */
#define LED1_PORT                    GPIOA
#define LED2_PORT                    GPIOA
/*  Led function  */
#define LED1_CLK_ENABLE              __HAL_RCC_GPIOA_CLK_ENABLE
#define LED1_ON                      HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET)
#define LED1_OFF                     HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_SET)

#define LED2_CLK_ENABLE              __HAL_RCC_GPIOA_CLK_ENABLE
#define LED2_ON                      HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET)
#define LED2_OFF                     HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET)
/*****************************************************************************/

void LED_Init(void);  /*  LED init  */

#endif