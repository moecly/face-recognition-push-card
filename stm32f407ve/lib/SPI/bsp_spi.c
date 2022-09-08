#include "bsp_spi.h"

/*  SPI handle struct  */
SPI_HandleTypeDef hspi;

//const uint8_t TEXT_Buffer[]={"Explorer STM32F4 SPI TEST"};

uint16_t W25QXX_TYPE = 0;
uint8_t W25QXX_BUFFER[4096];
uint8_t SendBuff;

/** 
 * @brief  Init SPI to use. 
 */
void SPI_Init(void)
{
  hspi.Instance = SPI1;
  hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi.Init.CRCPolynomial = 7;
  hspi.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi.Init.Direction = SPI_DIRECTION_2LINES;
  hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi.Init.Mode = SPI_MODE_MASTER;
  hspi.Init.NSS = SPI_NSS_SOFT;
  hspi.Init.TIMode = SPI_TIMODE_DISABLE;

  /*  SPI Init  */
  HAL_SPI_Init(&hspi);
  FLASH_CS_HIGH;

  /**  Must first config  */
  __HAL_SPI_ENABLE(&hspi);

  /*  Enable and read id  */
  W25QXX_Write_Enable();
  SPI_SetSpeed(SPI_BAUDRATEPRESCALER_2);
  SPI_ReadWriteByte(0xff);

  /*  Init SPI SCK  */
  W25QXX_TYPE = W25QXX_ReadID();
}

/** 
 * @brief  SPI gpio init.
 * @param  hspi SPI_HandleTypeDef struct. 
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  /*  Check the instance whether is spi  */
  if (hspi->Instance == SPI_INSTANCE)
  {
    GPIO_InitTypeDef GPIO_InitTypeDef = {0};
    /*  Config SPI and gpio clk  */
    SPI_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    SPI_MISO_CLK_ENABLE();
    SPI_MOSI_CLK_ENABLE();

    /** Config SPI gpio 
     *  SCK       --->     PA5
     *  MISO      --->     PA8
     *  MOSI      --->     PA7
     *  SCK_MODE  --->     AF_PP
     *  MOSI      --->     AF_PP
     *  MISO      --->     INPUT_UP
     */
    /*  Config SPI SCK  */
    GPIO_InitTypeDef.Pin = SPI_PIN_SCK | SPI_PIN_MISO | SPI_PIN_MOSI;
    GPIO_InitTypeDef.Mode = GPIO_MODE_AF_PP;
    GPIO_InitTypeDef.Pull = GPIO_PULLUP;
    GPIO_InitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitTypeDef.Alternate = GPIO_AF5_SPI1;

    HAL_GPIO_Init(SPI_PORT_SCK, &GPIO_InitTypeDef);

    /*  Config FLASH CS  */
    GPIO_InitTypeDef.Pin = FLASH_PIN_CS;
    GPIO_InitTypeDef.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitTypeDef.Pull = GPIO_PULLUP;
    GPIO_InitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;

    /*  Init Flash CS  */
    HAL_GPIO_Init(FLASH_PORT_CS, &GPIO_InitTypeDef);
    
  }
}



/**
  * @brief  Transmits a Data through the SPIx/I2Sx peripheral.
  * @param  SPIx: To select the SPIx/I2Sx peripheral, where x can be: 1, 2, 3, 4, 5 or 6 
  *         in SPI mode or 2 or 3 in I2S mode or I2Sxext for I2S full duplex mode.     
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data)
{
//  /* Check the parameters */
//  assert_param(IS_SPI_ALL_PERIPH_EXT(SPIx));
  
  /* Write in the DR register the data to be sent */
  SPIx->DR = Data;
}

/**
  * @brief  Returns the most recent received data by the SPIx/I2Sx peripheral. 
  * @param  SPIx: To select the SPIx/I2Sx peripheral, where x can be: 1, 2, 3, 4, 5 or 6 
  *         in SPI mode or 2 or 3 in I2S mode or I2Sxext for I2S full duplex mode. 
  * @retval The value of the received data.
  */
uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx)
{
//  /* Check the parameters */
//  assert_param(IS_SPI_ALL_PERIPH_EXT(SPIx));
  
  /* Return the data in the DR register */
  return SPIx->DR;
}

/** 
 * @brief  Set SPI speed from reg.
 * @param  SPI_BAUD Enter the baud rate you need to set.
 */
void SPI_SetSpeed(uint8_t SPI_BAUD)
{
  /*  Clear SPI baud  */
  hspi.Instance->CR1 &= 0xffc7;

  /*  Set SPI baud  */
  hspi.Instance->CR1 |= SPI_BAUD;
}

/** 
 * @brief  Get spi flag state.
 * @param  SPIx SPI_TypeDef struct.
 * @param  SPI_I2S_FLAG SPI flag.
 * @retval  SPI flag state.
 */
FlagStatus SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG)
{
  FlagStatus bitstatus = RESET;
//  /* Check the parameters */
//  assert_param(IS_SPI_ALL_PERIPH_EXT(SPIx));
//  assert_param(IS_SPI_I2S_GET_FLAG(SPI_I2S_FLAG));
  
  /* Check the status of the specified SPI flag */
  if ((SPIx->SR & SPI_I2S_FLAG) != (uint16_t)RESET)
  {
    /* SPI_I2S_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* SPI_I2S_FLAG is reset */
    bitstatus = RESET;
  }
  /* Return the SPI_I2S_FLAG status */
  return  bitstatus;
}

/** 
 * @brief  Send data to SPI.
 * @param  data Enter the data you want to get.
 * @retval  Return read data.
 */
uint8_t SPI_ReadWriteByte(uint8_t data)
{
  /*  Wait SPI TXE flag  */
  while (SPI_I2S_GetFlagStatus(hspi.Instance, SPI_FLAG_TXE) == RESET);
  SPI_I2S_SendData(hspi.Instance, data);

  /*  Wait SPI RXNE flag  */
  while (SPI_I2S_GetFlagStatus(hspi.Instance, SPI_FLAG_RXNE) == RESET);

  /*  Return read data  */
  return SPI_I2S_ReceiveData(hspi.Instance);
}

/** 
 * @brief  Read sr from W25QXX.
 * @retval  Return the data read.
 */
uint8_t W25QXX_ReadSR(void)
{
  uint8_t byte = 0;
  FLASH_CS_LOW;
  SPI_ReadWriteByte(W25X_ReadStatusReg);
  byte = SPI_ReadWriteByte(0xff);
  FLASH_CS_HIGH;
  return byte;
}

/** 
 * @brief  Write data to W25QXX.
 * @param  sr Enter the date you want to get.
 */
void W25QXX_WriteSR(uint8_t sr)
{
  FLASH_CS_LOW;
  SPI_ReadWriteByte(W25X_WriteStatusReg);
  SPI_ReadWriteByte(sr);
  FLASH_CS_HIGH;
}

/** 
 * @brief  W25QXX write enable.
 */
void W25QXX_Write_Enable(void)
{
  FLASH_CS_LOW;
  SPI_ReadWriteByte(W25X_WriteEnable);
  FLASH_CS_HIGH;
}

/** 
 * @brief  W25QXX read enable.
 */
void W25QXX_Write_Disable(void)
{
  FLASH_CS_LOW;
  SPI_ReadWriteByte(W25X_WriteDisable);
  FLASH_CS_HIGH;
}

/** 
 * @brief  Erase chip from W25QXX.
 */
void W25QXX_Erase_Chip(void)
{
  W25QXX_Wait_Busy();
  W25QXX_Write_Enable();
  FLASH_CS_LOW;
  SPI_ReadWriteByte(W25X_ChipErase);
  W25QXX_Wait_Busy();
  FLASH_CS_HIGH;
}

/** 
 * @brief  Erase specified area.
 * @param  Enter the addr you want to erase.
 */
void W25QXX_Erase_Sector(uint32_t addr)
{
  W25QXX_Write_Enable();
  W25QXX_Wait_Busy();
  FLASH_CS_LOW;
  SPI_ReadWriteByte(W25X_SectorErase);
  SPI_ReadWriteByte((uint8_t)(addr >> 16));
  SPI_ReadWriteByte((uint8_t)(addr >> 8));
  SPI_ReadWriteByte((uint8_t)(addr >> 0));
  FLASH_CS_LOW;
  W25QXX_Wait_Busy();
}

/** 
 * @brief  Read id from W25QXX.
 * @retval  Return id from this funtion.
 */
uint16_t W25QXX_ReadID(void)
{
  uint16_t id = 0;
  FLASH_CS_LOW;
  SPI_ReadWriteByte(0x90);
  SPI_ReadWriteByte(0x00);
  SPI_ReadWriteByte(0x00);
  SPI_ReadWriteByte(0x00);  

  id |= SPI_ReadWriteByte(0xff) << 8;
  id |= SPI_ReadWriteByte(0xff);
  FLASH_CS_HIGH;

  return id;
}

/** 
 * @brief  Wait busy flag from W25QXX.
 */
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR() & 0x01) == 0x01);  
}  

/** 
 * @brief  Read one byte from W25QXX.
 * @param  byte Return data to this variable.
 * @param  addr Enter the addr you want to visit.
 */
void W25QXX_ReadOneByte(uint8_t *byte, uint32_t addr)
{
  /*  Flash cs set low  */
  FLASH_CS_LOW;

  /*  Send flash location address  */
  SPI_ReadWriteByte(W25X_ReadData);
  SPI_ReadWriteByte((uint8_t)(addr >> 16));
  SPI_ReadWriteByte((uint8_t)(addr >> 8));
  SPI_ReadWriteByte((uint8_t)(addr >> 0));

  /*  Start to read data  */
  *byte = SPI_ReadWriteByte(0xff);

  /*  Flash cs set high  */
  FLASH_CS_HIGH;
}

/** 
 * @brief  Read 32 bits from W25QXX.
 * @param  byte Return data to this variable.
 * @param  addr Enter the addr you want to visit.
 * @param  Transmitsize The length of byte variable.
 */
void W25QXX_Read_u32(uint32_t *str, uint32_t addr, uint16_t Transmitsize)
{
  uint8_t temp;
  uint16_t i;
  for (i = 0; i < Transmitsize; i ++)
  {
//    W25QXX_Read(&temp, addr, 4);
    W25QXX_ReadOneByte(&temp, addr);
    *str |= temp;
    if (i + 1 != Transmitsize)
    {
     *str <<= 8;
     addr ++;
    }
  }
}

/** 
 * @brief  Read 32 bits from W25QXX.
 * @param  byte Return data to this variable.
 * @param  addr Enter the addr you want to visit.
 * @param  Transmitsize The length of byte variable.
 */
void W25QXX_Read_Int32(int32_t *str, uint32_t addr, uint16_t Transmitsize)
{
  uint8_t temp;
  uint16_t i;
  for (i = 0; i < Transmitsize; i ++)
  {
    W25QXX_ReadOneByte(&temp, addr);
    *str |= temp;
    if (i + 1 != Transmitsize)
    {
     *str <<= 8;
     addr ++;
    }
  }
}

/** 
 * @brief  Read 8 bits from W25QXX.
 * @param  byte Return data to this variable.
 * @param  addr Enter the addr you want to visit.
 * @param  Transmitsize The length of byte variable.
 */
void W25QXX_Read(uint8_t *str, uint32_t addr, uint16_t Transmitsize)
{
  uint16_t i;

  /*  Flash cs set low  */
  FLASH_CS_LOW;

  /*  Send flash location address  */
  SPI_ReadWriteByte(W25X_ReadData);
  SPI_ReadWriteByte((uint8_t)(addr >> 16));
  SPI_ReadWriteByte((uint8_t)(addr >> 8));
  SPI_ReadWriteByte((uint8_t)(addr >> 0));

  /*  Start to read data  */
  for (i = 0; i < Transmitsize; i ++)
  {
    str[i] = SPI_ReadWriteByte(0xff);
  }

  /*  Flash cs set high  */
  FLASH_CS_HIGH;
}

/** 
 * @brief  Write 8 bits to W25QXX.
 * @param  str Variables to be transferred.
 * @param  addr Enter the addr you want to visit.
 * @param  Transmitsize The length of byte variable.
 */
void W25QXX_Write_Page(uint8_t *str, uint32_t addr, uint16_t TransmitSize)
{
  uint16_t i;

  /*  Flash cs set low  */
  W25QXX_Write_Enable();
  FLASH_CS_LOW;

  /*  Send flash location address  */
  SPI_ReadWriteByte(W25X_PageProgram);
  SPI_ReadWriteByte((uint8_t)(addr >> 16));
  SPI_ReadWriteByte((uint8_t)(addr >> 8));
  SPI_ReadWriteByte((uint8_t)(addr >> 0));

  /*  Start to read data  */
  for (i = 0; i < TransmitSize; i ++)
  {
    SPI_ReadWriteByte(str[i]);
  }

  /*  Flash cs set high  */
  FLASH_CS_HIGH;
  W25QXX_Wait_Busy();
}

/** 
 * @brief  Write to W25QXX.
 * @param  str Variables to be transferred.
 * @param  addr Enter the addr you want to visit.
 * @param  Transmitsize The length of byte variable.
 */
void W25QXX_Write_NoCheck(uint8_t *str, uint32_t addr, uint16_t TransmitSize)
{
  uint16_t PageNum;

  /*  No write data  */
  PageNum = 256 - (addr % 256);

  if (TransmitSize <= PageNum)
  {
    PageNum = TransmitSize;
  }
  while (1)
  {
    W25QXX_Write_Page(str, addr, PageNum);
    if (TransmitSize == PageNum)
    {
      break;
    }
    else
    {
      str += PageNum;
      addr += PageNum;
      TransmitSize -= PageNum;
      if (TransmitSize <= 256)
      {
        PageNum = TransmitSize;
      }
      else
      {
        PageNum = 256;
      }
    }
  }
}

/** 
 * @brief  Write 32 bits to W25QXX.
 * @param  str Veriables to be transferred.
 * @param  addr Enter the addr you want to visit.
 * @param  Transmitsize The length of byte variable.
 */
void W25QXX_Write_Int32(int32_t str, uint32_t addr, uint16_t TransmitSize)
{
  uint16_t size = TransmitSize;
  uint8_t a;
  uint8_t b[2];
  uint8_t c[4];
  uint16_t i;
  if (size == 1)
  {
    for (i = 0; i < size; i ++)
    {
      a = str >> ((size - i - 1) * 8);
      W25QXX_Write(&a, addr, 1);
      addr += 8;
    }
  }
  else if (size == 2)
  {
    b[0] = str >> 8;
    b[1] = str;
    W25QXX_Write(b, addr, 2);
  }
  else if (size == 4)
  {
    c[0] = str >> 24;
    c[1] = str >> 16;
    c[2] = str >> 8;
    c[3] = str;
    W25QXX_Write(c, addr, 4);
  }
}

/** 
 * @brief  Write 32 bits to W25QXX.
 * @param  str Veriables to be transferred.
 * @param  addr Enter the addr you want to visit.
 * @param  Transmitsize The length of byte variable.
 */
void W25QXX_Write_u32(uint32_t str, uint32_t addr, uint16_t TransmitSize)
{
  uint16_t size = TransmitSize;
  uint8_t a;
  uint8_t b[2];
  uint8_t c[4];
  uint16_t i;
  if (size == 1)
  {
    for (i = 0; i < size; i ++)
    {
      a = str >> ((size - i - 1) * 8);
      W25QXX_Write(&a, addr, 1);
      addr += 8;
    }
  }
  else if (size == 2)
  {
    b[0] = str >> 8;
    b[1] = str;
    W25QXX_Write(b, addr, 2);
  }
  else if (size == 4)
  {
    c[0] = str >> 24;
    c[1] = str >> 16;
    c[2] = str >> 8;
    c[3] = str;
    W25QXX_Write(c, addr, 4);
  }
}

/** 
 * @brief  Write to W25QXX.
 * @param  str Veriables to be transferred.
 * @param  addr Enter the addr you want to visit.
 * @param  Transmitsize The length of byte variable.
 */
void W25QXX_Write(uint8_t *str, uint32_t addr, uint16_t TransmitSize)
{
  uint8_t *buf;
  uint32_t secpos;
  uint16_t secoff;
  uint16_t secremain;
  uint16_t i;

  secpos = addr / 4096;
  secoff = addr % 4096;
  secremain = 4096 - secoff;
  buf = W25QXX_BUFFER;

  if (TransmitSize <= secremain)
  {
    secremain = TransmitSize;
  }
  while (1)
  {
    W25QXX_Read(buf, secpos * 4096, 4096);
    for (i = 0; i < secremain; i ++)
    {
      if (buf[i + secoff] != 0xff)
      {
        break;
      }
    }
    if (i < secremain)
    {
      W25QXX_Erase_Sector(secpos);
      for (i = 0; i < secremain; i ++)
      {
        buf[i + secoff] = str[i];
      }
      W25QXX_Write_NoCheck(buf, secpos * 4096, 4096);
    }
    else
    {
      W25QXX_Write_NoCheck(str, addr, secremain);
    }

    if (secremain == TransmitSize)
    {
      break;
    }
    else
    {
      str += secremain;
      addr += secremain;
      secpos ++;
      secoff = 0;
      TransmitSize -= secremain;
      if (TransmitSize <= 4096)
      {
        secremain = TransmitSize;
      }
      else
      {
        secremain = 4096;
      }
    }
  }
}
