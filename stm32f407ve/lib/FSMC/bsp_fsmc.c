#include "bsp_fsmc.h"

/*  SRAM handle struct  */
SRAM_HandleTypeDef hsram;

/*  Lcd struct  */
LCD_Dev Lcd_Dev; 

/*  red  */
uint16_t POINT_COLOR = 0x0000;  

/*  back color  */
uint16_t BACK_COLOR = 0x0000;  

/*  LCD screen orientation  */
uint8_t LCD_SCREEN = 1;

/*  LCD screen sampling orientation  */
uint8_t LCD_SCAN = 1;

/** 
 * @brief  Calculate pow.
 * @param  m Be calculate.
 * @param  n Calculate.
 */
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while (n--)result*=m;    
	return result;
}			 

/** 
 * @brief  Display number on screen.
 * @param  x X coordinate.
 * @param  y Y coordinate.
 * @param  num Enter num you need to show.
 */
void ILI9341_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size)
{
  uint8_t t, temp;
  uint8_t enshow = 0;
  for (t = 0; t < len; t ++)
  {
    temp = (num/LCD_Pow(10, len-t-1)) % 10;
    if (enshow == 0 && t < (len - 1))
    {
      if (temp == 0)
      {
        ILI9341_ShowChar(x+(size/2)*t, y, ' ', size, 0);
        continue;
      }
      else
      {
        enshow = 1;
      }
    }
    ILI9341_ShowChar(x+(size/2)*t, y, temp+'0', size, 0);
  }
}

/** 
 * @brief  Display circle point on screen to calibration.
 * @param  x X coordinate.
 */
void ILI9341_Touch_Point(uint16_t x, uint16_t y, uint16_t color)
{
  POINT_COLOR = color;

  ILI9341_DrawLine(x-12, y, x+13, y, color);
  ILI9341_DrawLine(x, y-12, x, y+13, color);
  ILI9341_Fast_DrawPoint(x+1, y+1, color);
  ILI9341_Fast_DrawPoint(x-1, y+1, color);
  ILI9341_Fast_DrawPoint(x+1, y-1, color);
  ILI9341_Fast_DrawPoint(x-1, y-1, color);
  ILI9341_DrawCircle(x, y, 6, color);
}

/** 
 * @brief  
 */
void ILI9341_Draw_ALLCircle(uint16_t x, uint16_t y, uint8_t r)
{
  int a, b;
  int di;
  a = 0; b = r;
  di = 3-(r<<1);
  ILI9341_Open_Window(0, 0, Lcd_Dev.width, Lcd_Dev.height);
  while (a <= b)
  {
    int i = a, p = b;
    while (i > 0)
    {
/*      ILI9341_Fast_DrawPoint(x + b, y + i, POINT_COLOR);
      ILI9341_Fast_DrawPoint(x + b, y + i, POINT_COLOR);*/
      ILI9341_Fast_DrawPoint(x + b, y - i, POINT_COLOR);
      ILI9341_Fast_DrawPoint(x - i, y + b, POINT_COLOR);
      i --;
    }
    while (p > 0)
    {
      ILI9341_Fast_DrawPoint(x-a, y-p, POINT_COLOR);
      ILI9341_Fast_DrawPoint(x-p, y-a, POINT_COLOR);
      ILI9341_Fast_DrawPoint(x+a, y-p, POINT_COLOR);
      ILI9341_Fast_DrawPoint(x-p, y+a, POINT_COLOR);
      ILI9341_Fast_DrawPoint(x+a, y+p, POINT_COLOR);
      ILI9341_Fast_DrawPoint(x+p, y+a, POINT_COLOR);
      p --;
    }
    a ++;
    if (di < 0)
    {
      di += 4*a+6;
    }
    else
    {
      di += 10+4*(a-b);
      b --;
    }
  }
  ILI9341_Fast_DrawPoint(x, y, POINT_COLOR);
}

void ILI9341_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
  int a, b;
  int di;
  a=0; b=r;    
  di = 3 - (r << 1);            
  ILI9341_Open_Window(0, 0, Lcd_Dev.width, Lcd_Dev.height);
  while(a<=b)
  {
    ILI9341_Fast_DrawPoint(x0+a, y0-b, color);             //5
    ILI9341_Fast_DrawPoint(x0+b, y0-a, color);             //0           
    ILI9341_Fast_DrawPoint(x0+b, y0+a, color);             //4               
    ILI9341_Fast_DrawPoint(x0+a, y0+b, color);             //6 
    ILI9341_Fast_DrawPoint(x0-a, y0+b, color);             //1       
    ILI9341_Fast_DrawPoint(x0-b, y0+a, color);             
    ILI9341_Fast_DrawPoint(x0-a, y0-b, color);             //2             
    ILI9341_Fast_DrawPoint(x0-b, y0-a, color);             //7                
    a++;
    if (di < 0)
    {
      di += 4*a+6;    
    }
    else
    {
      di += 10+4*(a-b);   
      b --;
    }                 
  }
}                    

void ILI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  uint16_t t; 
  int xerr=0,yerr=0,delta_x,delta_y,distance; 
  int incx,incy,uRow,uCol; 

  delta_x = x2 - x1;
  delta_y = y2 - y1;
  uRow = x1;
  uCol = y1;

  if (delta_x > 0)
  {
    incx = 1;
  }
  else if (delta_x == 0)
  {
    incx = 0;
  }
  else
  {
    incx = -1;
    delta_x = -delta_x;
  }

  if (delta_y > 0)
  {
    incy = 1;
  }
  else if (delta_y == 0)
  {
    incy = 0;
  }
  else
  {
    incy = -1;
    delta_y = -delta_y;
  }

  if (delta_x > delta_y)
  {
    distance = delta_x;
  }
  else
  {
    distance = delta_y;
  }

  for (t = 0; t <= distance; t ++)
  {
    ILI9341_Fast_DrawPoint(uRow, uCol, color);
    xerr += delta_x;
    yerr += delta_y;
    if (xerr > distance)
    {
      xerr -= distance;
      uRow += incx;
    }
    if (yerr > distance)
    {
      yerr -= distance;
      uCol += incy;
    }
  }
  
}

void ILI9341_Display_Whole_Color(void)
{
  if (Lcd_Dev.id == 0x9341)
  {
    ILI9341_Draw_Rec(0, 0, Lcd_Dev.width, Lcd_Dev.height);
  }
}

void ILI9341_Scan_Dir(uint8_t dir)
{
  uint16_t regval=0;
  uint16_t dirreg=0;

  if (Lcd_Dev.id == 0x9341 && Lcd_Dev.dir == 1)
  {
    switch (dir)
    {
      case 0: dir=6; break;
      case 1:dir=7; break;
      case 2:dir=4; break;
      case 3:dir=5; break;
      case 4:dir=1; break;
      case 5:dir=0; break;
      case 6:dir=3;break;
      case 7:dir=2;break;   
    }
  }
  if (Lcd_Dev.id == 0x9341)
  {
    switch (dir)
    {
      case L2R_U2D:
        regval|=(0<<7)|(0<<6)|(0<<5); 
        break;
      case L2R_D2U:
        regval|=(1<<7)|(0<<6)|(0<<5); 
        break;
      case R2L_U2D:
        regval|=(0<<7)|(1<<6)|(0<<5); 
        break;
      case R2L_D2U:
        regval|=(1<<7)|(1<<6)|(0<<5); 
        break;   
      case U2D_L2R:
        regval|=(0<<7)|(0<<6)|(1<<5); 
        break;
      case U2D_R2L:
        regval|=(0<<7)|(1<<6)|(1<<5); 
        break;
      case D2U_L2R:
        regval|=(1<<7)|(0<<6)|(1<<5); 
        break;
      case D2U_R2L:
        regval|=(1<<7)|(1<<6)|(1<<5); 
        break;   
    }
    dirreg=0X36;
    ILI9341_WriteReg(dirreg,regval);
  }
}

void ILI9341_Display_Dir(uint8_t dir)
{
  /*  Vertical screen  */
  if (dir == 0)
  {
    Lcd_Dev.dir = 0;
    Lcd_Dev.width = 240;
    Lcd_Dev.height = 320;

    /*  If check success, set x, y cmd  */
    if (Lcd_Dev.id == 0x9341)
    {
      Lcd_Dev.WraMCmd = 0x2c;
      Lcd_Dev.SetXCmd = 0x2a;
      Lcd_Dev.SetYCmd = 0x2b;
    }
  }
//  /*  Horizontal screen  */
  if (dir == 1)
  {
    Lcd_Dev.dir = 1;
    Lcd_Dev.width = 320;
    Lcd_Dev.height = 240;

    /*  If check success, set x, y cmd  */
    if (Lcd_Dev.id == 0x9341)
    {
      Lcd_Dev.WraMCmd = 0x2c;
      Lcd_Dev.SetXCmd = 0x2a;
      Lcd_Dev.SetYCmd = 0x2b;
    }
  }
  ILI9341_Scan_Dir(LCD_SCAN);
}

/** 
 * @brief  Write data to per reg.
 * @param  reg Write reg to per address.
 */
void FSMC_Write_Reg(uint16_t reg)
{
  reg = reg;
  LCD->LCD_REG = reg;
}

/** 
 * @brief  Write data to per arm.
 * @param  data Write data to per address.
 */
void FSMC_Write_Data(uint16_t data)
{
  data = data;
  LCD->LCD_RAM = data;
}

/** 
 * @brief  Read per data.
 * @retval  Return per data.
 */
uint16_t FSMC_Read_Data(void)
{
  __IO uint16_t ram;
  ram = LCD->LCD_RAM;
  return ram;
}

/** 
 * @brief  Write reg to lcd.
 * @param  LCD_Reg LCD reg address.
 * @param  LCD_RegValue Write value to LCD address.
 */
void ILI9341_WriteReg(__IO uint16_t LCD_Reg, __IO uint16_t LCD_RegValue)
{  
  LCD->LCD_REG = LCD_Reg;
  LCD->LCD_RAM = LCD_RegValue;
}     

/** 
 * @brief  Read reg to lcd.
 * @param  LCD_Reg LCD reg address.
 * @retval  Return per data.
 */
uint16_t ILI9341_ReadReg(__IO uint16_t LCD_Reg)
{                       
  FSMC_Write_Reg(LCD_Reg);
  return FSMC_Read_Data();
}   

void ILI9341_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p)
{         
  uint8_t x0 = x;
  width += x;
  height += y;
  while ((*p <= '~') && (*p >= ' '))
  {       
      if (x >= width)
      {
        x = x0;
        y += size;
      }
      if (y >= height)
      {
        break;
      }
      ILI9341_ShowChar(x, y, *p, size, 1);
      x += (size / 2) + (size / 8);
      p++;
  }  
}

/** 
 * @brief  Check lcd thether is LIL9341, if yes, start function
 * @param  x x address.
 * @param  y y address.
 * @param  num Input need display char.
 * @param  size font size.
 * @param  mode add(1) or no add(0).
 */
void ILI9341_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode)
{
  uint8_t temp, t1, t;
  uint16_t y0 = y;
  uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) *(size / 2);
  num = num - ' ';
  for (t = 0; t < csize; t ++)
  {
    if (size == 12)
    {
      temp = asc2_1206[num][t];
    }
    else if (size == 16)
    {
      temp = asc2_1608[num][t];
    }
    else if (size == 24)
    {
      temp = asc2_2412[num][t];
    }
    else
    {
      return;
    }
    for (t1 = 0; t1 < 8; t1 ++)
    {
      if (temp & 0x80)
      {
        ILI9341_Fast_DrawPoint(x, y, POINT_COLOR);
      }
      else if (mode == 0)
      {
        ILI9341_Fast_DrawPoint(x, y, BACK_COLOR);
      }
      temp <<= 1;
      y ++;
      if (y >= Lcd_Dev.height)
      {
        return;
      }
      if (y - y0 == size)
      {
        y = y0;
        x ++;
        if (x >= Lcd_Dev.width)
        {
          return;
          break;
        }
      }
    }
  }
}

/** 
 * @brief  Check lcd whether is LIL9341, if yes, display lcd.
 */
void ILI9341_Display(void)
{
  LCD_OFF;
  if (Lcd_Dev.id == 0x9341 || Lcd_Dev.id == 0x9300)
  {
    LCD_ON;
  }
}

/** 
 * @brief  Draw point.
 * @param  x x address.
 * @param  y y address.
 * @param  color point color.
 */
void ILI9341_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
  if (Lcd_Dev.id == 0x9341)
  {
    FSMC_Write_Reg(Lcd_Dev.SetXCmd);
    FSMC_Write_Data(x >> 8);
    FSMC_Write_Data(x & 0xff);
    FSMC_Write_Reg(Lcd_Dev.SetYCmd);
    FSMC_Write_Data(y >> 8);
    FSMC_Write_Data(y & 0xff);
//    FSMC_Write_Reg(Lcd_Dev.WraMCmd);
//    FSMC_Write_Data(color);
    LCD->LCD_REG = Lcd_Dev.WraMCmd;
    LCD->LCD_RAM = color;
  }
}

/** 
 * @brief  Config FSMC and TIM.
 */
void FSMC_Init(void)
{
  /*  FSMC handle struct  */
  FSMC_NORSRAM_TimingTypeDef WriteTiming;
  FSMC_NORSRAM_TimingTypeDef ReadTiming;

  /* Config SRAM handle struct  */
  hsram.Instance = FSMC_INSTANCE;
  hsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  hsram.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
  hsram.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  hsram.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;

  /* Config FSMC write TIM handle struct  */
  WriteTiming.AccessMode = FSMC_ACCESS_MODE_A;
  WriteTiming.AddressHoldTime = 0x00;
  WriteTiming.DataSetupTime = 8;
  WriteTiming.AddressSetupTime = 9;
  WriteTiming.BusTurnAroundDuration = 0x00;
  WriteTiming.CLKDivision = 0x00;
  WriteTiming.DataLatency = 0x00;

  /* Config FSMC read TIM handle struct  */
  ReadTiming.AccessMode = FSMC_ACCESS_MODE_A;
  ReadTiming.AddressHoldTime = 0x00;
  ReadTiming.DataSetupTime = 60;
  ReadTiming.AddressSetupTime = 0xf;
  ReadTiming.BusTurnAroundDuration = 0x00;
  ReadTiming.CLKDivision = 0x00;
  ReadTiming.DataLatency = 0x00;

  /*  Init FSMC and TIM  */
  HAL_SRAM_Init(&hsram, &ReadTiming, &WriteTiming);
}

/** 
 * @brief  Config FSMC gpio.
 * @param  hsram Input SRAM handle struct.
 */
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
  /*  Check the sram bank  */
  if (hsram->Instance == FSMC_INSTANCE)
  {
    /*  GPIO init struct  */
    GPIO_InitTypeDef GPIO_InitTypeDef = {0};

    /*  LCD FSMC 8080 init  */
    FSMC_ILI9341_CLK_Init();

    /** Config FSMC gpio.
     *  BK      --->      OPP
     *  else    --->      AFPP
     */
    GPIO_InitTypeDef.Pin = FSMC_PIN_BK;
    GPIO_InitTypeDef.Mode = GPIO_MODE_OUTPUT_PP;    
    GPIO_InitTypeDef.Pull = GPIO_PULLUP;
    GPIO_InitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;

    /*  Init gpio  */
    HAL_GPIO_Init(FSMC_PORT_BK, &GPIO_InitTypeDef);
    
    GPIO_InitTypeDef.Pin = FSMC_PIN_A18 | FSMC_PIN_NOE | FSMC_PIN_NWE | FSMC_PIN_NE1 |
    FSMC_PIN_D0 | FSMC_PIN_D1 | FSMC_PIN_D2 | FSMC_PIN_D3 | GPIO_PIN_2 | FSMC_PIN_D13 |
    FSMC_PIN_D14 | FSMC_PIN_D15;
    GPIO_InitTypeDef.Mode = GPIO_MODE_AF_PP;
    GPIO_InitTypeDef.Pull = GPIO_PULLUP;
    GPIO_InitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitTypeDef.Alternate = GPIO_AF12_FSMC;

    /*  Init gpio  */
    HAL_GPIO_Init(FSMC_PORT_A18, &GPIO_InitTypeDef);

    GPIO_InitTypeDef.Pin = FSMC_PIN_D4 | FSMC_PIN_D5 | FSMC_PIN_D6 | FSMC_PIN_D7 | 
    FSMC_PIN_D8 | FSMC_PIN_D9 | FSMC_PIN_D10 | FSMC_PIN_D11 | FSMC_PIN_D12; 
    GPIO_InitTypeDef.Mode = GPIO_MODE_AF_PP;
    GPIO_InitTypeDef.Pull = GPIO_PULLUP;
    GPIO_InitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitTypeDef.Alternate = GPIO_AF12_FSMC;

    /*  Init gpio */
    HAL_GPIO_Init(FSMC_PORT_D4, &GPIO_InitTypeDef);

    /*  FSMC LCD LIL9341 init  */
    ILI9341_Init();
  }
}

/** 
 * @brief  ILI9341 Open window.
 * @param  usX Set x.
 * @param  usY Set y.
 * @param  usWidth Set width.
 * @param  usHeight Set height.
 */
void ILI9341_Open_Window(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight)
{
  /*  Set ILI9341 x address  */
  FSMC_Write_Reg(Lcd_Dev.SetXCmd);
  FSMC_Write_Data(usX >> 8);
  FSMC_Write_Data(usX & 0xff);
//  FSMC_Write_Data(((usX + usWidth) & 0xff00) >> 8);
  FSMC_Write_Data((usX + usWidth) >> 8);
  FSMC_Write_Data((usX + usWidth) & 0xff);

  /*  Set ILI9341 y address  */
  FSMC_Write_Reg(Lcd_Dev.SetYCmd);
  FSMC_Write_Data(usY >> 8);
  FSMC_Write_Data(usY & 0xff);
  FSMC_Write_Data((usY + usHeight) >> 8);
  FSMC_Write_Data((usY + usHeight) & 0xff);
}

/** 
 * @brief  ILI9341 draw rec.
 * @param  X Set x.
 * @param  Y Set y.
 * @param  Width Set width.
 * @param  Height Set height.
 */
void ILI9341_Draw_Rec(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  uint32_t i;
  /*  Set window  */
  ILI9341_Open_Window(x, y, width, height);

  /*  Config window cmd  */
  FSMC_Write_Reg(Lcd_Dev.WraMCmd);

  for (i = 0; i < (width +1) * (height+1); i ++)
  {
//    FSMC_Write_Data(RGB888_TO_RGB565(255, 00, 00));
      FSMC_Write_Data(POINT_COLOR);
  }
}

//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_NOE;
//    HAL_GPIO_Init(FSMC_PORT_NOE, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_NWE;
//    HAL_GPIO_Init(FSMC_PORT_NWE, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_NE1;
//    HAL_GPIO_Init(FSMC_PORT_NE1, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D0;
//    HAL_GPIO_Init(FSMC_PORT_D0, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D1;
//    HAL_GPIO_Init(FSMC_PORT_D1, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D2;
//    HAL_GPIO_Init(FSMC_PORT_D2, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D3;
//    HAL_GPIO_Init(FSMC_PORT_D3, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D4;
//    HAL_GPIO_Init(FSMC_PORT_D4, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D5;
//    HAL_GPIO_Init(FSMC_PORT_D5, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D6;
//    HAL_GPIO_Init(FSMC_PORT_D6, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D7;
//    HAL_GPIO_Init(FSMC_PORT_D7, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D8;
//    HAL_GPIO_Init(FSMC_PORT_D8, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D9;
//    HAL_GPIO_Init(FSMC_PORT_D9, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D10;
//    HAL_GPIO_Init(FSMC_PORT_D10, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D11;
//    HAL_GPIO_Init(FSMC_PORT_D11, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D12;
//    HAL_GPIO_Init(FSMC_PORT_D12, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D13;
//    HAL_GPIO_Init(FSMC_PORT_D13, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D14;
//    HAL_GPIO_Init(FSMC_PORT_D14, &GPIO_InitTypeDef);
//
//    /*  Init gpio  */
//    GPIO_InitTypeDef.Pin = FSMC_PIN_D15;
//    HAL_GPIO_Init(FSMC_PORT_D15, &GPIO_InitTypeDef);
//
//    /*  Enable LCD BK  */
//    LCD_ON;

/** 
 * @brief  Set FSMC clk.
 */
void FSMC_ILI9341_CLK_Init(void)
{
  /*  Enable FSMC clk  */
  FSMC_CLK();
  FSMC_CLK_BK();
  FSMC_CLK_A18();
  FSMC_CLK_NOE();
  FSMC_CLK_NWE();
  FSMC_CLK_NE1();
  FSMC_CLK_D0();
  FSMC_CLK_D1();
  FSMC_CLK_D2();
  FSMC_CLK_D3();
  FSMC_CLK_D4();
  FSMC_CLK_D5();
  FSMC_CLK_D6();
  FSMC_CLK_D7();
  FSMC_CLK_D8();
  FSMC_CLK_D9();
  FSMC_CLK_D11();
  FSMC_CLK_D12();
  FSMC_CLK_D13();
  FSMC_CLK_D14();
  FSMC_CLK_D15();
}

/** 
 * @brief  Init ILI9341.
 */
void ILI9341_Init(void)
{
/*  Check lcd id  */
    HAL_Delay(50);
    ILI9341_WriteReg(0x0000, 0x0001);
    HAL_Delay(50);
    Lcd_Dev.id = FSMC_Read_Data();
    FSMC_Write_Reg(0xd3);
    Lcd_Dev.id = FSMC_Read_Data();
    Lcd_Dev.id = FSMC_Read_Data();
    Lcd_Dev.id = FSMC_Read_Data();
    Lcd_Dev.id <<= 8;
    Lcd_Dev.id |= FSMC_Read_Data();

    /*  Display lcd  */
    ILI9341_Display();

    /*  Reset variables and init lcd  */
    if (Lcd_Dev.id == 0x9341)
    {
      /*  Clear  */
      FSMC_Bank1E->BWTR[6] &= ~(0XF<<0);
      FSMC_Bank1E->BWTR[6] &= ~(0XF<<8);
      /*  Set  */
      FSMC_Bank1E->BWTR[6] |= 3 << 0;  
      FSMC_Bank1E->BWTR[6] |= 2 << 8; 

      /*  Start to init  */
      FSMC_Write_Reg(0xCF);  
      FSMC_Write_Data(0x00);
      FSMC_Write_Data(0xC1); 
      FSMC_Write_Data(0X30); 
      FSMC_Write_Reg(0xED);  
      FSMC_Write_Data(0x64); 
      FSMC_Write_Data(0x03); 
      FSMC_Write_Data(0X12); 
      FSMC_Write_Data(0X81); 
      FSMC_Write_Reg(0xE8);  
      FSMC_Write_Data(0x85); 
      FSMC_Write_Data(0x10); 
      FSMC_Write_Data(0x7A); 
      FSMC_Write_Reg(0xCB);  
      FSMC_Write_Data(0x39); 
      FSMC_Write_Data(0x2C); 
      FSMC_Write_Data(0x00); 
      FSMC_Write_Data(0x34); 
      FSMC_Write_Data(0x02); 
      FSMC_Write_Reg(0xF7);  
      FSMC_Write_Data(0x20); 
      FSMC_Write_Reg(0xEA);  
      FSMC_Write_Data(0x00); 
      FSMC_Write_Data(0x00); 
      FSMC_Write_Reg(0xC0);    //Power control 
      FSMC_Write_Data(0x1B);   //VRH[5:0] 
      FSMC_Write_Reg(0xC1);    //Power control 
      FSMC_Write_Data(0x01);   //SAP[2:0];BT[3:0] 
      FSMC_Write_Reg(0xC5);    //VCM control 
      FSMC_Write_Data(0x30);    //3F
      FSMC_Write_Data(0x30);    //3C
      FSMC_Write_Reg(0xC7);    //VCM control2 
      FSMC_Write_Data(0XB7); 
      FSMC_Write_Reg(0x36);    // Memory Access Control 
      FSMC_Write_Data(0x48); 
      FSMC_Write_Reg(0x3A);   
      FSMC_Write_Data(0x55); 
      FSMC_Write_Reg(0xB1);   
      FSMC_Write_Data(0x00);   
      FSMC_Write_Data(0x1A); 
      FSMC_Write_Reg(0xB6);    // Display Function Control 
      FSMC_Write_Data(0x0A); 
      FSMC_Write_Data(0xA2); 
      FSMC_Write_Reg(0xF2);    // 3Gamma Function Disable 
      FSMC_Write_Data(0x00); 
      FSMC_Write_Reg(0x26);    //Gamma curve selected 
      FSMC_Write_Data(0x01); 
      FSMC_Write_Reg(0xE0);    //Set Gamma 
      FSMC_Write_Data(0x0F); 
      FSMC_Write_Data(0x2A); 
      FSMC_Write_Data(0x28); 
      FSMC_Write_Data(0x08); 
      FSMC_Write_Data(0x0E); 
      FSMC_Write_Data(0x08); 
      FSMC_Write_Data(0x54); 
      FSMC_Write_Data(0XA9); 
      FSMC_Write_Data(0x43); 
      FSMC_Write_Data(0x0A); 
      FSMC_Write_Data(0x0F); 
      FSMC_Write_Data(0x00); 
      FSMC_Write_Data(0x00); 
      FSMC_Write_Data(0x00); 
      FSMC_Write_Data(0x00);      
      FSMC_Write_Reg(0XE1);    //Set Gamma 
      FSMC_Write_Data(0x00); 
      FSMC_Write_Data(0x15); 
      FSMC_Write_Data(0x17); 
      FSMC_Write_Data(0x07); 
      FSMC_Write_Data(0x11); 
      FSMC_Write_Data(0x06); 
      FSMC_Write_Data(0x2B); 
      FSMC_Write_Data(0x56); 
      FSMC_Write_Data(0x3C); 
      FSMC_Write_Data(0x05); 
      FSMC_Write_Data(0x10); 
      FSMC_Write_Data(0x0F); 
      FSMC_Write_Data(0x3F); 
      FSMC_Write_Data(0x3F); 
      FSMC_Write_Data(0x0F); 
      FSMC_Write_Reg(0x2B); 
      FSMC_Write_Data(0x00);
      FSMC_Write_Data(0x00);
      FSMC_Write_Data(0x01);
      FSMC_Write_Data(0x3f);
      FSMC_Write_Reg(0x2A); 
      FSMC_Write_Data(0x00);
      FSMC_Write_Data(0x00);
      FSMC_Write_Data(0x00);
      FSMC_Write_Data(0xef);   
      FSMC_Write_Reg(0x11); //Exit Sleep
      HAL_Delay(120);
      FSMC_Write_Reg(0x29); //display on  

      /*  Init success  */
      ILI9341_Display_Dir(LCD_SCREEN);
    }
}