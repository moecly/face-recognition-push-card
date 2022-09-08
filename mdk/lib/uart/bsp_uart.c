#include "bsp_uart.h"

/*  UART handle struct  */
UART_HandleTypeDef huart;
UART_HandleTypeDef huart2;

/*  DMA handle struct  */
DMA_HandleTypeDef hdma_uart_tx;
DMA_HandleTypeDef hdma_uart_rx;
DMA_HandleTypeDef hdma_uart2_tx;
DMA_HandleTypeDef hdma_uart2_rx;

uint8_t uart_receive[DMA_MAX_LENGTH];
uint8_t uart_receive2[DMA_MAX_LENGTH];
uint8_t uart_data[128];

int _write(int fd, char *pBuffer, int size)
{
  while(__HAL_UART_GET_FLAG(&huart, UART_FLAG_TC) == 0);
  HAL_UART_Transmit(&huart, (uint8_t *)pBuffer, size, 0xfff);
  return size;
}

/** 
 * @brief  UART init.
 */
void UART_Init(void)
{

  /*  Init uart1  */
  huart.Instance = UART_INSTANCE;
  huart.Init.BaudRate = 115200;
  huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart.Init.Mode = UART_MODE_TX_RX;
  huart.Init.OverSampling = UART_OVERSAMPLING_16;
  huart.Init.Parity = UART_PARITY_NONE;
  huart.Init.StopBits = UART_STOPBITS_1;
  huart.Init.WordLength = UART_WORDLENGTH_8B;
  
  HAL_UART_Init(&huart);
  __HAL_UART_ENABLE_IT(&huart, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart, uart_receive, sizeof(uart_receive));

    /*  Init uart1  */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  
  HAL_UART_Init(&huart2);
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart2, uart_receive2, sizeof(uart_receive2));

}

/** 
 * @brief  Config and init uart gpio.
 * @param  huart Enter uart handle struct you need to set.
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  if(huart->Instance == UART_INSTANCE)
  {
    GPIO_InitTypeDef GPIO_InitTypeDef = {0};
    /** uart gpio configuration 
     *  PA9   --->  tx     
     *  PA10  --->  rx     
     *  txmode  --->  AF   
     *  rxmode  --->  RI     
     */
    UART_RX_CLK_ENABLE();
    UART_TX_CLK_ENABLE();
    UART_CLK_ENABLE();
    GPIO_InitTypeDef.Pin = UART_PIN_TX | UART_PIN_RX;
    GPIO_InitTypeDef.Mode = GPIO_MODE_AF_PP;
    GPIO_InitTypeDef.Pull = GPIO_PULLUP;
    GPIO_InitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitTypeDef.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(UART_PORT_TX, &GPIO_InitTypeDef);
    
    /*  Uart irq  */
    HAL_NVIC_SetPriority(UART_IRQN, 0, 0);
    HAL_NVIC_EnableIRQ(UART_IRQN);

    /*  Config uart tx dma  */
    UART_TX_DMA_ENABLE();
    hdma_uart_tx.Instance = UART_TX_DMA_INSTANCE;
    hdma_uart_tx.Init.Channel = UART_TX_DMA_ChANNEL;
    hdma_uart_tx.Init.Mode = DMA_NORMAL;
    hdma_uart_tx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart_tx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_uart_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    hdma_uart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_uart_tx.Init.Priority = DMA_PRIORITY_HIGH;

    /*  Init uart tx dma  */
    HAL_DMA_Init(&hdma_uart_tx);
    __HAL_LINKDMA(huart, hdmatx, hdma_uart_tx);
    // HAL_NVIC_SetPriority(UART_TX_DMA_IRQN, 0, 0);
    // HAL_NVIC_EnableIRQ(UART_TX_DMA_IRQN);

    /*  Config uart rx dma  */
    UART_RX_DMA_ENABLE();
    hdma_uart_rx.Instance = UART_RX_DMA_INSTANCE;
    hdma_uart_rx.Init.Channel = UART_RX_DMA_ChANNEL;
    hdma_uart_rx.Init.Mode = DMA_CIRCULAR;
    hdma_uart_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart_rx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_uart_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    hdma_uart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_uart_rx.Init.Priority = DMA_PRIORITY_HIGH;

    /*  Init uart tx dma  */
    HAL_DMA_Init(&hdma_uart_rx);
    __HAL_LINKDMA(huart, hdmarx, hdma_uart_rx);
    // HAL_NVIC_SetPriority(UART_RX_DMA_IRQN, 0, 0);
    // HAL_NVIC_EnableIRQ(UART_RX_DMA_IRQN);

  } else if (huart->Instance == USART2) {

    GPIO_InitTypeDef GPIO_InitTypeDef = {0};
    /** uart gpio configuration 
     *  PA2   --->  tx     
     *  PA3   --->  rx     
     *  txmode  --->  AF   
     *  rxmode  --->  RI     
     */
    UART_RX_CLK_ENABLE();
    UART_TX_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    GPIO_InitTypeDef.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitTypeDef.Mode = GPIO_MODE_AF_PP;
    GPIO_InitTypeDef.Pull = GPIO_PULLUP;
    GPIO_InitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitTypeDef.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(UART_PORT_TX, &GPIO_InitTypeDef);
    
    /*  Uart irq  */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    /*  Config uart tx dma  */
    __HAL_RCC_DMA1_CLK_ENABLE();
    hdma_uart2_tx.Instance = DMA1_Stream6;
    hdma_uart2_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_uart2_tx.Init.Mode = DMA_NORMAL;
    hdma_uart2_tx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart2_tx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_uart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart2_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    hdma_uart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_uart2_tx.Init.Priority = DMA_PRIORITY_HIGH;

    /*  Init uart tx dma  */
    HAL_DMA_Init(&hdma_uart2_tx);
    __HAL_LINKDMA(huart, hdmatx, hdma_uart2_tx);
    // HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

    /*  Config uart rx dma  */
    hdma_uart2_rx.Instance = DMA1_Stream5;
    hdma_uart2_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_uart2_rx.Init.Mode = DMA_CIRCULAR;
    hdma_uart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart2_rx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_uart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart2_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    hdma_uart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_uart2_rx.Init.Priority = DMA_PRIORITY_HIGH;

    /*  Init uart tx dma  */
    HAL_DMA_Init(&hdma_uart2_rx);
    __HAL_LINKDMA(huart, hdmarx, hdma_uart2_rx);
    // HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

  }
}

/** 
 * @brief  Send string to uart peripheral.
 */
void UART_SendString(UART_HandleTypeDef huart, uint8_t *str)
{
  unsigned int k = 0; 
  do
  {
    while(__HAL_UART_GET_FLAG(&huart, UART_FLAG_TC) == 0x00u);
    HAL_UART_Transmit(&huart, (uint8_t *) (str+k++), 1, 1000);
  } while(*(str+k) != '\0');
}

/** 
 * @brief  Send string to uart peripheral.
 */
// void UART2_SendString(uint8_t *str)
// {
//   unsigned int k = 0; 
//   do
//   {
//     while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == 0x00u);
//     HAL_UART_Transmit(&huart2, (uint8_t *) (str+k++), 1, 0xfff);
//   } while(*(str+k) != '\0');
// }


/** 
 * @brief  My printf function.
 */
void myPrintf(char str1[], char str2[])
{
  char str[80];
  strcat(str, str1);
  strcat(str, str2);
  // UART_SendString((uint8_t *) str);
}

int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart, (uint8_t *) &ch, 1, 1000);
  return ch;
}

void dmaRealTimeTransmitToEsp8266Face(UART_HandleTypeDef huart, UART_HandleTypeDef huart2, uint8_t uartData[]) 
{
  static int dmaDataTail = 0;
  static int dmaDataLength = 0;
  static int dmaDataHead = 0;
    // HAL_UART_IRQHandler(&huart);
  if (__HAL_UART_GET_FLAG(&huart, UART_FLAG_IDLE) != RESET) {
    // __HAL_UART_FLUSH_DRREGISTER(&huart);
    // huart.Instance->SR &= ~UART_FLAG_IDLE;
    // huart.Instance->SR |= UART_FLAG_IDLE;
    uint8_t temp;
    temp = USART1->SR;
    temp = USART1->DR;
    // uint8_t temp = huart.Instance->SR;
    // temp = huart.Instance->DR;
    dmaDataTail = hdma_uart_rx.Instance->NDTR;
    dmaDataTail = DMA_MAX_LENGTH - dmaDataTail;
    dmaDataLength = (dmaDataTail - dmaDataHead + DMA_MAX_LENGTH) % DMA_MAX_LENGTH;
    for (int i = 0; i < dmaDataLength; i ++) {
      int index = (dmaDataHead + i) % DMA_MAX_LENGTH;
      HAL_UART_Transmit(&huart2, &uart_receive[index], 1, 0xfff);
      // UART_SendString(huart2, &uartData[index]);
      // UART_SendString(&uart_receive[index]);
    }
    // 
    dmaDataHead = dmaDataTail;
  }

  if (__HAL_UART_GET_FLAG(&huart, UART_FLAG_RXNE) != RESET) {
    huart.Instance->SR &= ~UART_FLAG_RXNE;
  }
}

void dmaRealTimeTransmitToK210(UART_HandleTypeDef huart, UART_HandleTypeDef huart2, uint8_t uartData[])
{
  static int dmaDataTail = 0;
  static int dmaDataLength = 0;
  static int dmaDataHead = 0;
  // HAL_UART_IRQHandler(&huart);
  if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET) {
    // __HAL_UART_FLUSH_DRREGISTER(&huart2);
    uint8_t temp;
    temp = USART2->SR;
    temp = USART2->DR;
    dmaDataTail = hdma_uart2_rx.Instance->NDTR;
    dmaDataTail = DMA_MAX_LENGTH - dmaDataTail;
    dmaDataLength = (dmaDataTail - dmaDataHead + DMA_MAX_LENGTH) % DMA_MAX_LENGTH;
    // for (int i = 0; i < dmaDataLength2; i ++) {
    int index = (dmaDataHead) % DMA_MAX_LENGTH;
    if (uartData[index] == '^') {
      if (uartData[index+1] == 'R') {
        HAL_UART_Transmit(&huart, (uint8_t*) &"C", 1, 0xfff);
        // UART_SendString(huart, (uint8_t*) &"C");
      }
    }
    dmaDataHead = dmaDataTail;
  }
}