#ifndef __BSP_FSMC_H__
#define __BSP_FSMC_H__


#include "font_12_16_24.h"
#include "stm32f4xx.h"
#include "bsp_led.h"

typedef struct  
{                        
  uint16_t width;  /*  LCD width  */
  uint16_t height;  /*  LCD height  */
  uint16_t id;  /*  LCD ID  */
  uint8_t dir;  /*  LCD mode  */
  uint16_t  WraMCmd;  /*  Write cmd  */
  uint16_t  SetXCmd;  /*  Set x  */
  uint16_t  SetYCmd;  /*  Set y  */
} LCD_Dev;     

typedef struct
{
  uint16_t LCD_REG;  /*  LCD reg address  */
  uint16_t LCD_RAM;  /*  LCD ram address  */
} LCD_Address;

/*****************************************************************************/
/*  LCD CMD  */
#define LCD_CMD_SetCoordinateX        0x2a
#define LCD_CMD_SetCorrdinateY        0x2b
#define LCD_CMD_Pixel                 0x2c

/*  LCD COLOR  */
#define WHITE                         0xFFFF
#define BLACK                         0x0000    
#define BLUE                          0x001F  
#define BRED                          0XF81F
#define GRED                          0XFFE0
#define GBLUE                         0X07FF
#define RED                           0xF800
#define MAGENTA                       0xF81F
//#define GREEN                         0x07E0
#define GREEN                         0x7ff
#define CYAN                          0x7FFF
#define YELLOW                        0xFFE0
#define BROWN                         0XBC40
#define BRRED                         0XFC07 
//#define GRAY                          0X8430 
#define GRAY                          0X7777

/*  LCD SCAN  */
#define L2R_U2D                       0 
#define L2R_D2U                       1 
#define R2L_U2D                       2 
#define R2L_D2U                       3 

#define U2D_L2R                       4 
#define U2D_R2L                       5 
#define D2U_L2R                       6 
#define D2U_R2L                       7

/*  LCD BK CONTROL  */
#define LCD_ON                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)
#define LCD_OFF                       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define LCD_BASE                      ((uint32_t)(0x60000000 | 0x00007FFFE))
#define LCD                           ((LCD_Address *) LCD_BASE)
#define RGB888_TO_RGB565(R, G, B)     (uint16_t)((R & 0x1f) << 11 | (G & 0x3f) << 5 | (B & 0x1f) << 0)

/*  FSMC CLK  */
#define FSMC_CLK                      __HAL_RCC_FSMC_CLK_ENABLE

/*  FSMC ADDR  */
#define FSMC_CMD_ADDR                 (__IO uint16_t *)(0x60000000)
#define FSMC_DATA_ADDR                (__IO uint16_t *)(0x60080000)

/*  FSMC INSTANCE  */
#define FSMC_INSTANCE                 FSMC_NORSRAM_BANK1

/*  FSMC EXTENDED  */
#define FSMC_EXTENDED                 FSMC_NORSRAM_EXTENDED_DEVICE

/*  FSMC GPIO  */
#define FSMC_PIN_BK                   GPIO_PIN_1
#define FSMC_PORT_BK                  GPIOB
#define FSMC_CLK_BK                   __HAL_RCC_GPIOB_CLK_ENABLE

#define FSMC_PIN_A18                  GPIO_PIN_13
#define FSMC_PORT_A18                 GPIOD
#define FSMC_CLK_A18                  __HAL_RCC_GPIOD_CLK_ENABLE

#define FSMC_PIN_NOE                  GPIO_PIN_4
#define FSMC_PORT_NOE                 GPIOD
#define FSMC_CLK_NOE                  __HAL_RCC_GPIOD_CLK_ENABLE

#define FSMC_PIN_NWE                  GPIO_PIN_5
#define FSMC_PORT_NWE                 GPIOD
#define FSMC_CLK_NWE                  __HAL_RCC_GPIOD_CLK_ENABLE

#define FSMC_PIN_NE1                  GPIO_PIN_7
#define FSMC_PORT_NE1                 GPIOD
#define FSMC_CLK_NE1                  __HAL_RCC_GPIOD_CLK_ENABLE

#define FSMC_PIN_D0                   GPIO_PIN_14
#define FSMC_PORT_D0                  GPIOD
#define FSMC_CLK_D0                   __HAL_RCC_GPIOD_CLK_ENABLE

#define FSMC_PIN_D1                   GPIO_PIN_15
#define FSMC_PORT_D1                  GPIOD
#define FSMC_CLK_D1                   __HAL_RCC_GPIOD_CLK_ENABLE

#define FSMC_PIN_D2                   GPIO_PIN_0
#define FSMC_PORT_D2                  GPIOD
#define FSMC_CLK_D2                   __HAL_RCC_GPIOD_CLK_ENABLE

#define FSMC_PIN_D3                   GPIO_PIN_1
#define FSMC_PORT_D3                  GPIOD
#define FSMC_CLK_D3                   __HAL_RCC_GPIOD_CLK_ENABLE

#define FSMC_PIN_D4                   GPIO_PIN_7
#define FSMC_PORT_D4                  GPIOE
#define FSMC_CLK_D4                   __HAL_RCC_GPIOE_CLK_ENABLE

#define FSMC_PIN_D5                   GPIO_PIN_8
#define FSMC_PORT_D5                  GPIOE
#define FSMC_CLK_D5                   __HAL_RCC_GPIOE_CLK_ENABLE

#define FSMC_PIN_D6                   GPIO_PIN_9
#define FSMC_PORT_D6                  GPIOE
#define FSMC_CLK_D6                   __HAL_RCC_GPIOE_CLK_ENABLE

#define FSMC_PIN_D7                   GPIO_PIN_10
#define FSMC_PORT_D7                  GPIOE
#define FSMC_CLK_D7                   __HAL_RCC_GPIOE_CLK_ENABLE

#define FSMC_PIN_D8                   GPIO_PIN_11
#define FSMC_PORT_D8                  GPIOE
#define FSMC_CLK_D8                   __HAL_RCC_GPIOE_CLK_ENABLE

#define FSMC_PIN_D9                   GPIO_PIN_12
#define FSMC_PORT_D9                  GPIOE
#define FSMC_CLK_D9                   __HAL_RCC_GPIOE_CLK_ENABLE

#define FSMC_PIN_D10                  GPIO_PIN_13
#define FSMC_PORT_D10                 GPIOE
#define FSMC_CLK_D10                  __HAL_RCC_GPIOE_CLK_ENABLE

#define FSMC_PIN_D11                  GPIO_PIN_14
#define FSMC_PORT_D11                 GPIOE
#define FSMC_CLK_D11                  __HAL_RCC_GPIOE_CLK_ENABLE

#define FSMC_PIN_D12                  GPIO_PIN_15
#define FSMC_PORT_D12                 GPIOE
#define FSMC_CLK_D12                  __HAL_RCC_GPIOE_CLK_ENABLE

#define FSMC_PIN_D13                  GPIO_PIN_8
#define FSMC_PORT_D13                 GPIOD
#define FSMC_CLK_D13                  __HAL_RCC_GPIOD_CLK_ENABLE

#define FSMC_PIN_D14                  GPIO_PIN_9
#define FSMC_PORT_D14                 GPIOD
#define FSMC_CLK_D14                  __HAL_RCC_GPIOD_CLK_ENABLE

#define FSMC_PIN_D15                  GPIO_PIN_10
#define FSMC_PORT_D15                 GPIOD
#define FSMC_CLK_D15                  __HAL_RCC_GPIOD_CLK_ENABLE
/*****************************************************************************/

extern LCD_Dev Lcd_Dev;  /*  Lcd struct  */
extern uint16_t  POINT_COLOR;  /*  red  */
extern uint16_t  BACK_COLOR;  /*  back color  */
extern uint8_t LCD_SCAN;
extern uint8_t LCD_SCREEN;
extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
extern const unsigned char asc2_2412[95][36];

void FSMC_Init(void);  /*  Init fsmc  */
void ILI9341_Init(void);  /*  LCD Init  */
void FSMC_Write_Reg(uint16_t reg);  /*  FSMC write reg  */
void FSMC_Write_Data(uint16_t data);  /*  FSMC write data  */
uint16_t FSMC_Read_Data(void);  /*  FSMC read data  */
void ILI9341_WriteReg(__IO uint16_t LCD_Reg, __IO uint16_t LCD_RegValue);  /*  LCD write reg  */
uint16_t ILI9341_ReadReg(__IO uint16_t LCD_Reg);  /*  LCD read reg  */
void ILI9341_Display(void);  /*  display lcd  */
void FSMC_ILI9341_CLK_Init(void);  /*  FSMC LCD init  */
void ILI9341_Open_Window(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight);  /*  Set LCD ILI9341 window  */
void ILI9341_Draw_Rec(uint16_t x, uint16_t y, uint16_t width, uint16_t height);  /*  Draw a rec  */
void ILI9341_Display_Dir(uint8_t dir);
void ILI9341_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void ILI9341_Scan_Dir(uint8_t dir);
void ILI9341_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode);
void ILI9341_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p);
void ILI9341_Display_Whole_Color(void);
void XPT2046_ADjust(void);
void ILI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ILI9341_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
void ILI9341_Touch_Point(uint16_t x, uint16_t y, uint16_t color);
void ILI9341_DrawNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size);
void ILI9341_Draw_ALLCircle(uint16_t x, uint16_t y, uint8_t r);

#endif 