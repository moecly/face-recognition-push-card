#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "stm32f4xx.h"

/*****************************************************************************/
/*  SPI config  */

/*  SPI instance  */
#define SPI_INSTANCE               SPI1

/*  SPI pin  */
#define SPI_PIN_SCK                GPIO_PIN_3    
#define SPI_PIN_MISO               GPIO_PIN_4
#define SPI_PIN_MOSI               GPIO_PIN_5

/*  SPI pin port  */
#define SPI_PORT_SCK               GPIOB
#define SPI_PORT_MISO              GPIOB
#define SPI_PORT_MOSI              GPIOB

/*  SPI address  */
#define SPI_ADDRESS1               0x5f

/*  SPI irqn  */
#define SPI_EV_IRQN                I2C1_EV_IRQn
#define SPI_ER_IRQN                I2C1_ER_IRQn

/*  SPI function  */
#define SPI_CLK_ENABLE()           __HAL_RCC_SPI1_CLK_ENABLE()
#define SPI_MISO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI_MOSI_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

/*  FLASH config  */

/*  FLASH pin  */
#define FLASH_PIN_CS               GPIO_PIN_0

/*  FLASH port  */
#define FLASH_PORT_CS              GPIOB

/*  Flash id  */
#define W25Q16                     0xef14

/*  FLASH function  */
#define FLASH_CS_HIGH              HAL_GPIO_WritePin(FLASH_PORT_CS, FLASH_PIN_CS, GPIO_PIN_SET);
#define FLASH_CS_LOW               HAL_GPIO_WritePin(FLASH_PORT_CS, FLASH_PIN_CS, GPIO_PIN_RESET);

/*  Flash cmd  */
#define W25X_WriteEnable           0x06 
#define W25X_WriteDisable          0x04 
#define W25X_ReadStatusReg         0x05 
#define W25X_WriteStatusReg        0x01 
#define W25X_ReadData              0x03 
#define W25X_FastReadData          0x0B 
#define W25X_FastReadDual          0x3B 
#define W25X_PageProgram           0x02 
#define W25X_BlockErase            0xD8 
#define W25X_SectorErase           0x20 
#define W25X_ChipErase             0xC7 
#define W25X_PowerDown             0xB9 
#define W25X_ReleasePowerDown      0xAB 
#define W25X_DeviceID              0xAB 
#define W25X_ManufactDeviceID      0x90 
#define W25X_JedecDeviceID         0x9F 
/*****************************************************************************/

extern uint16_t W25QXX_TYPE;
//const uint8_t TEXT_Buffer[]={"Explorer STM32F4 SPI TEST"};

void SPI_Init(void);  /*  SPI init  */
void SPI_SetSpeed(uint8_t SPI_BAUD);
uint16_t W25QXX_ReadID(void);
uint8_t SPI_ReadWriteByte(uint8_t data);
uint8_t W25QXX_ReadSR(void);
void W25QXX_WriteSR(uint8_t sr);
void W25QXX_Write_Enable(void);
void W25QXX_Write_Disable(void);
uint16_t W25QXX_ReadID(void);
void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data);
uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx);
void W25QXX_Read(uint8_t *str, uint32_t addr, uint16_t Transmitsize);
void W25QXX_Wait_Busy(void);
void W25QXX_Write_Page(uint8_t *str, uint32_t addr, uint16_t TransmitSize);
void W25QXX_Write(uint8_t *str, uint32_t addr, uint16_t TransmitSize);
void W25QXX_Write_NoCheck(uint8_t *str, uint32_t addr, uint16_t TransmitSize);
void W25QXX_ReadOneByte(uint8_t *str, uint32_t addr);
void W25QXX_Write_u32(uint32_t str, uint32_t addr, uint16_t TransmitSize);
void W25QXX_Read_u32(uint32_t *byte, uint32_t addr, uint16_t Transmitsize);
void W25QXX_Write_Int32(int32_t str, uint32_t addr, uint16_t TransmitSize);
void W25QXX_Read_Int32(int32_t *byte, uint32_t addr, uint16_t Transmitsize);

#endif 