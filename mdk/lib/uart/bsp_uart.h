#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"

/*****************************************************************************/
/*  UART gpio */
#define UART_PIN_TX                  GPIO_PIN_9
#define UART_PIN_RX                  GPIO_PIN_10
#define UART_PORT_TX                 GPIOA
#define UART_PORT_RX                 GPIOA

/*  uart instance  */
#define UART_INSTANCE                USART1

/*  uart function  */
#define UART_CLK_ENABLE()            __HAL_RCC_USART1_CLK_ENABLE()
#define UART_TX_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART_RX_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART_TX_DMA_ENABLE()         __HAL_RCC_DMA2_CLK_ENABLE()
#define UART_RX_DMA_ENABLE()         __HAL_RCC_DMA2_CLK_ENABLE()

/*  uart irqn  */
#define UART_IRQN                    USART1_IRQn

/*  Uart dma  */
#define UART_TX_DMA_INSTANCE         DMA2_Stream7
#define UART_RX_DMA_INSTANCE         DMA2_Stream5
#define UART_TX_DMA_ChANNEL          DMA_CHANNEL_4
#define UART_RX_DMA_ChANNEL          DMA_CHANNEL_4
#define UART_RX_DMA_IRQN             DMA2_Stream5_IRQn
#define UART_TX_DMA_IRQN             DMA2_Stream7_IRQn

#define DMA_MAX_LENGTH               128
/*****************************************************************************/

extern UART_HandleTypeDef huart;
extern UART_HandleTypeDef huart2;
extern uint8_t uart_receive[DMA_MAX_LENGTH];
extern uint8_t uart_receive2[DMA_MAX_LENGTH];

// void UART2_SendString(uint8_t *str);
void UART_Init(void);  /*  uart init to currect use  */
int _write(int fd, char *pBuffer, int size);  /*  to use printf  */
void UART_SendString(UART_HandleTypeDef huart, uint8_t *str);
void myPrintf(char str1[], char str2[]);
void dmaRealTimeTransmitToEsp8266Face(UART_HandleTypeDef huart, UART_HandleTypeDef haurt2, uint8_t uartData[]);
void dmaRealTimeTransmitToK210(UART_HandleTypeDef huart, UART_HandleTypeDef huart2, uint8_t uartData[]);

#endif
