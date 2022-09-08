#ifndef __BSP_ADC_H__ 
#define __BSP_ADC_H__ 

/*  Include hal function  */
#include "stm32f4xx.h"

/*****************************************************************************/
#define ADC_PIN                     GPIO_PIN_5
#define ADC_PORT                    GPIOA
#define ADC_CLK                     __HAL_RCC_ADC1_CLK_ENABLE
#define ADC_GPIO_CLK                __HAL_RCC_GPIOA_CLK_ENABLE

#define ADC_INSTANCE                ADC1
#define ADC_CHANNEL                 ADC_CHANNEL_5
#define ADC_IRQN                    ADC_IRQn
#define ADC_IRQHANDLER              ADC_IRQHandler

#define ADC_DMAx                    DMA2
#define ADC_DMA_CLK                 __HAL_RCC_DMA2_CLK_ENABLE
#define ADC_DMA_CHANNEL             DMA_CHANNEL_0
#define ADC_DMA_STREAM              DMA2_Stream0
/*****************************************************************************/

// extern ADC_HandleTypeDef hadc;
// extern uint16_t ADC_ConvertedValue[4];
extern ADC_HandleTypeDef hadc;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
/*  Init ADC  */
void ADC_Init(void);
void ADC2_Init(void);
void ADC3_Init(void);
int getTemprate(void);


#endif