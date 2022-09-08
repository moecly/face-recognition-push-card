#include "bsp_adc.h"

/*  ADC handle struct  */
ADC_HandleTypeDef hadc;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc;
// char buff[128];

/** 
 * @brief  Config ADC and init.
 */
void ADC_Init(void)
{
  /*  ADC mode struct  */
  ADC_MultiModeTypeDef multimode = {0};

  /*  ADC channel struct  */
  ADC_ChannelConfTypeDef sConfig = {0};

  /*  Config ADC init struct  */
  hadc.Instance = ADC_INSTANCE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.ScanConvMode = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.NbrOfConversion = 1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;

  /*  ADC GPIO init  */
  HAL_ADC_Init(&hadc);

  /*  Set ADC mode  */
  multimode.Mode = ADC_TRIPLEMODE_INTERL;
  multimode.DMAAccessMode = ADC_DMAACCESSMODE_2; 
  multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
  HAL_ADCEx_MultiModeConfigChannel(&hadc, &multimode);

  /*  Config ADC channel1  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
  
  /*  Config ADC channel4  */
  // sConfig.Channel = ADC_CHANNEL_3;
  // sConfig.Rank = 2;
  // sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  // HAL_ADC_ConfigChannel(&hadc, &sConfig);

  // /*  Config ADC channel7  */
  // sConfig.Channel = ADC_CHANNEL_4;
  // sConfig.Rank = 3;
  // sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  // HAL_ADC_ConfigChannel(&hadc, &sConfig);

  // /*  Config ADC channel tempsensor  */
  // sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  // sConfig.Rank = 4;
  // sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  // HAL_ADC_ConfigChannel(&hadc, &sConfig);

  /*  Enable ADC  */
  // HAL_ADC_Start(&hadc);
}

/** 
 * @brief  Config ADC and init.
 */
void ADC2_Init(void)
{
  /*  ADC channel struct  */
  ADC_ChannelConfTypeDef sConfig = {0};

  /*  Config ADC init struct  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.ScanConvMode = DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  // hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  // hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;

  /*  ADC GPIO init  */
  HAL_ADC_Init(&hadc2);

  /*  Config ADC channel1  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc2, &sConfig);
  
  /*  Enable ADC  */
  // HAL_ADC_Start(&hadc2);
}

/** 
 * @brief  Config ADC and init.
 */
void ADC3_Init(void)
{
  /*  ADC channel struct  */
  ADC_ChannelConfTypeDef sConfig = {0};

  /*  Config ADC init struct  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.ContinuousConvMode = ENABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.ScanConvMode = DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  // hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  // hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;

  /*  ADC GPIO init  */
  HAL_ADC_Init(&hadc3);

  /*  Config ADC channel1  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc3, &sConfig);
  
  /*  Enable ADC  */
  // HAL_ADC_Start(&hadc3);
}

/** 
 * @brief  Config and init ADC gpio.
 * @param  hadc Enter the ADC handle struct 
 *              you want to config.
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  /*  Check whether the name corresponds  */
  if (hadc->Instance == ADC_INSTANCE)
  {
    /*  Enable adc and gpio clk  */
    ADC_CLK();
    ADC_GPIO_CLK();
    ADC_DMA_CLK();

    /*  ADC gpio init struct  */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
  
    /*  Start to config gpio  */
    GPIO_InitStruct.Pin = GPIO_PIN_2/* | GPIO_PIN_3 | GPIO_PIN_4*/;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    /*  ADC gpio init  */
    HAL_GPIO_Init(ADC_PORT, &GPIO_InitStruct);

    /*  Enable ADC nvic  */
    // HAL_NVIC_SetPriority(ADC_IRQN, 0, 0);
    // HAL_NVIC_EnableIRQ(ADC_IRQN);

    /*  Config ADC DMA struct  */
    hdma_adc.Instance = ADC_DMA_STREAM;
    hdma_adc.Init.Channel = ADC_DMA_CHANNEL;
    hdma_adc.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_adc.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc.Init.Mode = DMA_CIRCULAR;
    hdma_adc.Init.PeriphBurst = DMA_PBURST_SINGLE; 
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
  
    /*  ADC DMA init  */
    // HAL_DMA_Init(hadc->DMA_Handle);
    HAL_DMA_Init(&hdma_adc);
  
    /*  Connecting ADC and DMA  */
    // __HAL_LINKDMA(hadc, DMA_Handle, *(hadc->DMA_Handle));
     __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

    /*  Start DMA transmit  */
    // HAL_ADC_Start_DMA(hadc, (uint32_t *) ADC_ConvertedValue, 1);
  } else if (hadc->Instance == ADC2)
  {
    /*  Enable adc and gpio clk  */
    __HAL_RCC_ADC2_CLK_ENABLE();
    ADC_GPIO_CLK();
    // ADC_DMA_CLK();

    /*  ADC gpio init struct  */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
  
    /*  Start to config gpio  */
    GPIO_InitStruct.Pin = GPIO_PIN_2/* | GPIO_PIN_3 | GPIO_PIN_4*/;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    /*  ADC gpio init  */
    HAL_GPIO_Init(ADC_PORT, &GPIO_InitStruct);
  } else if (hadc->Instance == ADC3)
  {
    /*  Enable adc and gpio clk  */
    __HAL_RCC_ADC3_CLK_ENABLE();
    ADC_GPIO_CLK();
    // ADC_DMA_CLK();

    /*  ADC gpio init struct  */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
  
    /*  Start to config gpio  */
    GPIO_InitStruct.Pin = GPIO_PIN_2/* | GPIO_PIN_3 | GPIO_PIN_4*/;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    /*  ADC gpio init  */
    HAL_GPIO_Init(ADC_PORT, &GPIO_InitStruct);
  }
}

// uint16_t getAdc(uint8_t ch)
// {
//   HAL_ADC_PollForConversion(&hadc, 100);
//   return HAL_ADC_GetValue(&hadc);
// }

// uint16_t getAdcAverage(uint8_t ch, uint8_t times)
// {
//   uint32_t temp_val = 0;
//   uint8_t t;
//   for (t = 0; t < times; t ++)
//   {
//     temp_val += getAdc(ch);
//     HAL_Delay(5);
//   }
//   return temp_val / times;
// }

// int getTemprate(void)
// {
//   uint32_t adcx;
//   int result;
//   double temprate;
//   adcx = getAdcAverage(ADC_CHANNEL_16, 10);
//   temprate = (float) adcx * (3.3 / 4096);
//   temprate = (temprate - 0.76) / 0.0025 + 25;
//   // temprate = (1.43-3.3/4095*adcx)/0.0043+25;
//   result = temprate *= 100;
//   return result;
// }