#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

#include "stm32f4xx.h"

/*  I2C config  */
/*****************************************************************************/
/*  I2C pin  */
#define I2C_PIN_SCL                GPIO_PIN_6    
#define I2C_PIN_SDA                GPIO_PIN_7    

/*  I2C pin port  */
#define I2C_SCL_PORT               GPIOB
#define I2C_SDA_PORT               GPIOB

/*  I2C instance  */
#define I2C_INSTANCE               I2C1

/*  I2C address  */
#define I2C_ADDRESS1               0x5f

/*  I2C irqn  */
#define I2C_EV_IRQN                I2C1_EV_IRQn
#define I2C_ER_IRQN                I2C1_ER_IRQn

/*  I2C function  */
#define I2C_CLK_ENABLE()           __HAL_RCC_I2C1_CLK_ENABLE()
#define I2C_SCL_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C_SDA_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()

/*  I2C flag timeout  */
#define I2C_FLAG_TIMEOUT           ((uint32_t)0x1000)
#define I2C_LONG_FLAG_TIMEOUT      ((uint32_t)(10 * I2C_FLAG_TIMEOUT))

/*  EEPROM config  */
/*****************************************************************************/
/*  EEPROM address  */
#define EEPROM_ADDRESS_WRITE       0xa0
#define EEPROM_ADDRESS_READ        0xa1

/*  EEPROM address size  */
#define EEPROM_SIZE                8
/*****************************************************************************/

extern I2C_HandleTypeDef hi2c;

void I2C_Init(void);  /*  To currect use  */
uint16_t I2C_Mem_Write(uint8_t *str, uint8_t WriteAddr, uint16_t size);  /*  Write string to eeprom  */
uint16_t I2C_Mem_Read(uint8_t *str, uint8_t WriteAddr, uint16_t size);  /*  Read string to eeprom  */
uint32_t I2C_TIMEOUT_UserCallback(uint8_t ErrorCode);  /*  Return error code to user to quick find error  */
uint16_t I2C_EE_Write(uint8_t *str, uint8_t addr, uint16_t size);  /*  Write to EEPROM  */
uint16_t I2C_EE_Read(uint8_t *str, uint8_t addr, uint16_t size);  /*  Read to EEPROM  */

#endif