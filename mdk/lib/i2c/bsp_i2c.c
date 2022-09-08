#include "bsp_i2c.h"
#include "bsp_uart.h"
#include "stdio.h"

I2C_HandleTypeDef hi2c;

/** 
 * @brief  to currect i2c.
 */
void I2C_Init(void)
{
  /*  Config I2C handle struct  */
  hi2c.Instance = I2C_INSTANCE;
  hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c.Init.ClockSpeed = 400000;
  hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c.Init.DutyCycle = I2C_DUTYCYCLE_16_9;
  hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  hi2c.Init.OwnAddress1 = I2C_ADDRESS1;
  hi2c.Init.OwnAddress2 = 0;

  /*  Init I2C  */
  HAL_I2C_Init(&hi2c);
}

/** 
 * @brief  to config i2c gpio.
 * @param  hi2c i2c handle struct.
 */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->Instance == I2C1)
  {
    /*  Start I2C and gpio clk  */
    I2C_CLK_ENABLE();
    I2C_SCL_CLK_ENABLE();
    I2C_SDA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitTypeDef = {0};
    /**  I2C gpio configuration
     *  PB6  --->  SCL
     *  PB7  --->  SDA
     *  SCL  --->  AF_OD
     *  SDA  --->   RISING
     */
    GPIO_InitTypeDef.Pin = I2C_PIN_SCL;
    GPIO_InitTypeDef.Mode = GPIO_MODE_AF_OD; 
    GPIO_InitTypeDef.Pull = GPIO_PULLUP;
    GPIO_InitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_SCL_PORT, &GPIO_InitTypeDef);

    GPIO_InitTypeDef.Pin = I2C_PIN_SDA;
    GPIO_InitTypeDef.Mode = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(I2C_SDA_PORT, &GPIO_InitTypeDef);

    HAL_NVIC_SetPriority( I2C_EV_IRQN, 0, 0 );
    HAL_NVIC_EnableIRQ(I2C_EV_IRQN);
  }
}

/** 
 * @brief  i2c Page write to eeprom.
 * @param  str transmit string.
 * @param  WriteAddr memory address.
 * @param  size size of string.
 * @retval  return HAL_OK\HAL_ERROR.
 */
uint16_t I2C_Mem_Write(uint8_t *str, uint8_t WriteAddr, uint16_t size)
{
  if (HAL_I2C_Mem_Write(&hi2c, EEPROM_ADDRESS_WRITE, WriteAddr, I2C_MEMADD_SIZE_8BIT, str, size, 0xfff) == HAL_OK)
  {
    /*  if write success  */
    return HAL_OK;  
  }
  else
  {
    /*  if write failed  */
    return HAL_ERROR;
  }
}

/** 
 * @brief  i2c Page read to eeprom.
 * @param  str retval string.
 * @param  WriteAddr memory address.
 * @param  size size of string.
 * @retval  return HAL_OK\HAL_ERROR.
 */
uint16_t I2C_Mem_Read(uint8_t *str, uint8_t ReadAddr, uint16_t size)
{
  if (HAL_I2C_Mem_Read(&hi2c, EEPROM_ADDRESS_WRITE, ReadAddr, I2C_MEMADD_SIZE_8BIT, str, size, 0xfff) == HAL_OK)
  {
    /*  If write success  */
    return HAL_OK;  
  }
  else
  {
    /*  If write failed  */
    return I2C_TIMEOUT_UserCallback(0);
  }
}

/** 
 * @brief  Write data to EEPROM.
 * @param  str Input you need char write to EEPROM.
 * @param  addr Input EEPROM address.
 * @param  size String length.
 * @retval  Return HAL_OK/HAL_ERROR;
 */
uint16_t I2C_EE_Write(uint8_t *str, uint8_t addr, uint16_t size)
{
  uint32_t Flag_Timeout;
  uint16_t TxSize = 0;

  /*  Enable I2C peripheral  */
  hi2c.Instance->CR1 |= I2C_CR1_PE;

  /*  Send start flag  */
  hi2c.Instance->CR1 |= I2C_CR1_START;

  /*  Send peripheral address  */
  hi2c.Instance->DR |= EEPROM_ADDRESS_WRITE;

  /*  Wait until TXE flag is set  */
  Flag_Timeout = I2C_FLAG_TIMEOUT;
  while ((hi2c.Instance->CR1) << 1 == SET)
  {
    Flag_Timeout --;
    if (Flag_Timeout == 0u)
    {
      return I2C_TIMEOUT_UserCallback(0);
    }
  }

  /*  Send memory address  */
  hi2c.Instance->DR = addr;

  /* Wait I2C_SR1_AF RESET */
  Flag_Timeout = I2C_FLAG_TIMEOUT;
  while ((hi2c.Instance->SR1) << 10 == SET)
  {
    Flag_Timeout --;
    if (Flag_Timeout == 0u)
    {
      return I2C_TIMEOUT_UserCallback(0);
    }
  }

  /*  Clear AF flag  */
  hi2c.Instance->SR1 &= ~I2C_SR1_AF;

  /*  Wait I2C_SR1_ADDR SET  */
  Flag_Timeout = I2C_FLAG_TIMEOUT;
  while ((hi2c.Instance->SR1) << 1 == SET)
  {
    Flag_Timeout --;
    if (Flag_Timeout == 0u)
    {
      return I2C_TIMEOUT_UserCallback(0);
    }
  }

  /*  Clear ADDR flag  */
  hi2c.Instance->SR1 &= ~I2C_SR1_ADDR;

  /*  Start to write data to EEPROM  */
  while (size > 0u)
  {
    /*  Wait I2C_SR1_TXE SET  */
    Flag_Timeout = I2C_FLAG_TIMEOUT;
    while ((hi2c.Instance->SR1) << 7 == SET)
    {
      Flag_Timeout --;
      if (Flag_Timeout == 0u)
      {
        return I2C_TIMEOUT_UserCallback(0);
      }
    }

    /*  Write data to DR  */
    hi2c.Instance->DR = *(str + TxSize);
    TxSize ++;
    size --;

    /*  Wait I2C_SR1_BTF SET  */
    Flag_Timeout = I2C_FLAG_TIMEOUT;
    while ((hi2c.Instance->SR1) << 2 == SET)
    {
      Flag_Timeout --;
      if (Flag_Timeout == 0u)
      {
        return I2C_TIMEOUT_UserCallback(0);
      }
    }

    /*  Write data to DR  */
    if (size > 0u)
    {
      hi2c.Instance->DR = *(str + TxSize);
      TxSize ++;
      size --;
    }

    /*  Wait I2C_SR1_BTF SET  */
    Flag_Timeout = I2C_FLAG_TIMEOUT;
    while ((hi2c.Instance->SR1) << 2 == SET)
    {
      Flag_Timeout --;
      if (Flag_Timeout == 0u)
      {
        return I2C_TIMEOUT_UserCallback(0);
      }
    }

    /* Wait I2C_SR1_AF RESET */
    Flag_Timeout = I2C_FLAG_TIMEOUT;
    while ((hi2c.Instance->SR1) << 10 == SET)
    {
      Flag_Timeout --;
      if (Flag_Timeout == 0u)
      {
        return I2C_TIMEOUT_UserCallback(0);
      }
    }

    /*  Clear AF flag  */
    hi2c.Instance->SR1 &= ~I2C_SR1_AF;
  }

  /*  Set I2C flag stop  */
  hi2c.Instance->CR1 |= I2C_CR1_STOP;

  /*  Return status  */
  return HAL_OK;
}

uint16_t I2C_EE_Read(uint8_t *str, uint8_t addr, uint16_t size)
{
  uint32_t Flag_Timeout;
  uint16_t TxSize = 0;

  /*  Enable I2C peripheral  */
  hi2c.Instance->CR1 |= I2C_CR1_PE;

  /*  Send start flag  */
  hi2c.Instance->CR1 |= I2C_CR1_START;

  /*  Send peripheral address  */
  hi2c.Instance->DR |= EEPROM_ADDRESS_WRITE;

  /*  Send ACK flag enable  */
  hi2c.Instance->CR1 |= I2C_CR1_ACK;

  /*  Wait until TXE flag is set  */
  Flag_Timeout = I2C_FLAG_TIMEOUT;
  while ((hi2c.Instance->CR1) << 1 == SET)
  {
    Flag_Timeout --;
    if (Flag_Timeout == 0u)
    {
      return I2C_TIMEOUT_UserCallback(2);
    }
  }

  /*  Send memory address  */
  hi2c.Instance->DR = addr;

  /* Wait I2C_SR1_AF RESET */
  Flag_Timeout = I2C_FLAG_TIMEOUT;
  while ((hi2c.Instance->SR1) << 10 == SET)
  {
    Flag_Timeout --;
    if (Flag_Timeout == 0u)
    {
      return I2C_TIMEOUT_UserCallback(3);
    }
  }

  /*  Wait I2C_SR1_ADDR SET  */
  Flag_Timeout = I2C_FLAG_TIMEOUT;
  while ((hi2c.Instance->SR1) << 1 == SET)
  {
    Flag_Timeout --;
    if (Flag_Timeout == 0u)
    {
      return I2C_TIMEOUT_UserCallback(4);
    }
  }

  /*  Clear ADDR flag  */
  hi2c.Instance->SR1 &= ~I2C_SR1_ADDR;

  /*  Config ADDR and ack  */
  if (size == 0u)
  {
    hi2c.Instance->CR1 |= I2C_CR1_STOP;
  }
  else if (size == 1u)
  {
    /*  Send ACK flag disable  */
    hi2c.Instance->CR1 &= ~I2C_CR1_ACK;
  }

  /*  Start to receive string  */
  while (size > 0u)
  {
    if (size < 3u)
    {
      /*  Two bytes  */
      if (size == 2u)
      {
        /*  Wait until BTF flag is set  */
        Flag_Timeout = I2C_FLAG_TIMEOUT;
        while ((hi2c.Instance->SR1 & I2C_SR1_BTF) != I2C_SR1_BTF)
        {
          Flag_Timeout --;
          /*  If timeout  */
          if (Flag_Timeout == 0u)
          {
            return I2C_TIMEOUT_UserCallback(5);
          }
        }
        
        /*  Read data from EEPROM  */
        *(str + TxSize) = hi2c.Instance->DR;

        /*  Update  */
        TxSize ++;
        size --;

        /*  Read data from EEPROM  */
        *(str + TxSize) = hi2c.Instance->DR;

        /*  Update  */
        TxSize ++;
        size --;
      }

      /*  One byte  */
      else if (size == 1u)
      {
        /*  Wait until RXNE flag is set  */
        Flag_Timeout = I2C_FLAG_TIMEOUT;
        while ((hi2c.Instance->SR1 & I2C_SR1_RXNE) != I2C_SR1_RXNE)
        {
          /*  Wait until BTF flag is set  */
          Flag_Timeout = I2C_FLAG_TIMEOUT;
          Flag_Timeout --;
          /*  If timeout  */
          if (Flag_Timeout == 0u)
          {
            return I2C_TIMEOUT_UserCallback(6);
          }
        }
        TxSize ++;
        size --;
      }
    }

    /*  else  */
    else
    {
      /*  Wait until BTF flag is set  */
      Flag_Timeout = I2C_FLAG_TIMEOUT;
      while ((hi2c.Instance->SR1 << 2) != SET)
      {
        Flag_Timeout --;
        /*  If timeout  */
        if (Flag_Timeout == 0u)
        {
          return I2C_TIMEOUT_UserCallback(8);
        }
      }

      /*  Read data from EEPROM  */
      *(str + TxSize) = hi2c.Instance->DR;

      /*  Update  */
      TxSize ++;
      size --;

      /*  Read data from EEPROM  */
      *(str + TxSize) = hi2c.Instance->DR;

      /*  Update  */
      TxSize ++;
      size --;
    }

    /*  Wait until BTF flag is set  */
    Flag_Timeout = I2C_FLAG_TIMEOUT;
    while ((hi2c.Instance->SR1 & I2C_SR1_BTF) != I2C_SR1_BTF)
    {
      Flag_Timeout --;
      /*  If timeout  */
      if (Flag_Timeout == 0u)
      {
        return I2C_TIMEOUT_UserCallback(9);
      }
    }
  }
  /*  Set I2C flag stop  */
  hi2c.Instance->CR1 |= I2C_CR1_STOP;

  /*  Return status  */
  return HAL_OK;
}

/** 
 * @brief  Return error code to quick find error.
 * @param  ErrorCode To printf error code.
 */
uint32_t I2C_TIMEOUT_UserCallback(uint8_t ErrorCode)
{
  /*  Printf error warnning  */
  printf("Error! Timeout! ErrorCode = %d\n", ErrorCode);

  return HAL_ERROR;
}