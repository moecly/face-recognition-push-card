#ifndef __BSP_XPT2046_H__
#define __BSP_XPT2046_H__

#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_uart.h"
#include "stdio.h"
#include "bsp_fsmc.h"
#include "stdlib.h"
#include "bsp_spi.h"
#include "bsp_key.h"

/*****************************************************************************/
/*  XPT2046 cmd  */
#define XPT2046_CHANNEL_X                        
#define XPT2046_CHANNEL_Y                        

#define XPT2046_FLASH_ADJUST_ADDR                77

#define ERR_RANGE                                50

#define TP_PRES_DOWN                             0x80
#define TP_CATH_PRES                             0x40

#define XPT2046_NOT_PRESSED                      0
#define XPT2046_PRESSED                          1

#define XPT2046_PIN_CS                           GPIO_PIN_12
#define XPT2046_PIN_SCK                          GPIO_PIN_13
#define XPT2046_PIN_MISO                         GPIO_PIN_14
#define XPT2046_PIN_MOSI                         GPIO_PIN_15
#define XPT2046_PIN_PEN                          GPIO_PIN_5

#define XPT2046_PORT_CS                          GPIOB
#define XPT2046_PORT_SCK                         GPIOB
#define XPT2046_PORT_MISO                        GPIOB
#define XPT2046_PORT_MOSI                        GPIOB
#define XPT2046_PORT_PEN                         GPIOC

/*  XPT2046 function  */
#define XPT2046_CS_ENABLE()                      HAL_GPIO_WritePin(XPT2046_PORT_CS, XPT2046_PIN_CS, GPIO_PIN_RESET)
#define XPT2046_CS_DISABLE()                     HAL_GPIO_WritePin(XPT2046_PORT_CS, XPT2046_PIN_CS, GPIO_PIN_SET)

#define XPT2046_SCK_HIGH()                       HAL_GPIO_WritePin(XPT2046_PORT_SCK, XPT2046_PIN_SCK, GPIO_PIN_SET)
#define XPT2046_SCK_LOW()                        HAL_GPIO_WritePin(XPT2046_PORT_SCK, XPT2046_PIN_SCK, GPIO_PIN_RESET)

#define XPT2046_MOSI_HIGH()                      HAL_GPIO_WritePin(XPT2046_PORT_MOSI, XPT2046_PIN_MOSI, GPIO_PIN_SET)
#define XPT2046_MOSI_LOW()                       HAL_GPIO_WritePin(XPT2046_PORT_MOSI, XPT2046_PIN_MOSI, GPIO_PIN_RESET)

#define XPT2046_PENIRQ_READ()                    HAL_GPIO_ReadPin(XPT2046_PORT_PEN, XPT2046_PIN_PEN)
#define XPT2046_MISO_READ()                      HAL_GPIO_ReadPin(XPT2046_PORT_MISO, XPT2046_PIN_MISO)

/*****************************************************************************/

/*
typedef struct
{
  uint16_t x;
  uint16_t y;
  uint16_t pre_x;
  uint16_t pre_y;
} XPT2046_Coordinate;

typedef struct
{
  float dX_X,
        dX_Y,
        dX,
        dY_X,
        dY_Y,
        dY;
} XPT2046_TouchPara;
*/

typedef struct
{
  void (*init)(void);
  uint8_t (*scan)(uint8_t);
  void (*adjust)(void);
  uint16_t x[5];
  uint16_t y[5];
  uint8_t sta;
  float xfac;
  float yfac;
  short xoff;
  short yoff;
  uint8_t TouchType;
} _m_xpt_dev;

extern _m_xpt_dev xpt_dev;
extern uint32_t FLASH_SIZE;
extern uint8_t CMD_RDX;
extern uint8_t CMD_RDY;

void XPT2046_Init(void);
void XPT2046_MspInit(void);
void XPT2046_SendCmd(uint8_t cmd);
uint16_t XPT2046_ReceiveData(uint8_t cmd);
void XPT2046_DelayUs(__IO uint32_t Count);
uint8_t XPT2046_Touch_Detect(void);
void XPT2046_Touch(void);
void XPT2046_ADjust(void);
uint8_t XPT2046_Scan(uint8_t tp);
void ILI9341_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size);
void XPT2046_Adj_Info_Show(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t fac);
void XPT2046_Save_ADjdata(void);
uint8_t XPT2046_Get_ADjdata(void);
void XPT2046_Draw(void);

#endif