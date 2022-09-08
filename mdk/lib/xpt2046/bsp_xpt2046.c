#include "bsp_xpt2046.h"
#include "math.h"

/*  Display message  */
const uint8_t TP_REMIND_MSG_TBL[] = "Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";

/*  Flash size  */
uint32_t FLASH_SIZE = 16 * 1024 * 1024;

/*  Font size  */
uint32_t POINT_SIZE = 0;

/*  XPT2046 state  */
typedef enum
{
  XPT2046_STATE_RELEASE = 0,
  XPT2046_STATE_WAITING,
  XPT2046_STATE_PRESSED,
} Touch_State;

/*  LCD size  */
uint8_t CMD_RDX=0XD0;
uint8_t CMD_RDY=0X90;

/*  XPT parameter and function  */
_m_xpt_dev xpt_dev = {
  XPT2046_Touch,
  XPT2046_Scan,
  XPT2046_ADjust,
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  0,
  0,
  0,
  0,
  0,
  0
};

/** 
 * @brief  Init XPT2046
 */
void XPT2046_Init(void)
{
  XPT2046_MspInit();
}

/** 
 * @brief  XPT2046 gpio init.
 */
void XPT2046_MspInit(void)
{
  /*  Enable clk  */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*  XPT2046 struct  */
  GPIO_InitTypeDef GPIO_InitTypeDef = {0};

  GPIO_InitTypeDef.Pin = XPT2046_PIN_CS | XPT2046_PIN_MOSI | XPT2046_PIN_SCK;
  GPIO_InitTypeDef.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitTypeDef.Pull = GPIO_PULLUP;
  GPIO_InitTypeDef.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  /*  Init XPT2046  */
  HAL_GPIO_Init(XPT2046_PORT_CS, &GPIO_InitTypeDef);

  GPIO_InitTypeDef.Pin = XPT2046_PIN_MISO;
  GPIO_InitTypeDef.Mode = GPIO_MODE_INPUT;
  GPIO_InitTypeDef.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(XPT2046_PORT_MISO, &GPIO_InitTypeDef);

  GPIO_InitTypeDef.Pin = XPT2046_PIN_PEN;
  GPIO_InitTypeDef.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(XPT2046_PORT_PEN, &GPIO_InitTypeDef);
}

/** 
 * @brief  XPT2046 delay us.
 * @param  Count Delay Count us.
 */
void XPT2046_DelayUs(__IO uint32_t Count)
{
  __IO uint32_t i;

  for (i = 0; i < Count; i ++)
  {
    __IO uint8_t j = 12;
    while (j --);
  }
}

/** 
 * @brief  Send cmd to XPT2046
 * @param  cmd Need send cmd.
 */
void XPT2046_SendCmd(uint8_t cmd)
{
  uint8_t count;
  for (count = 0; count < 8; count ++)
  {
    if (cmd & 0x80)
    {
      XPT2046_MOSI_HIGH();
    }
    else
    {
      XPT2046_MOSI_LOW();
    }
    cmd <<= 1;
    XPT2046_SCK_LOW();
    XPT2046_DelayUs(5);
    XPT2046_SCK_HIGH();
  }
}

/** 
 * @brief  Receive data from XPT2046.
 * @param  cmd Send a command to receive the required value.
 * @retval  Return 12 bit data.
 */
uint16_t XPT2046_ReceiveData(uint8_t cmd)
{
  uint8_t i = 0;
  uint16_t data = 0;
  XPT2046_SCK_LOW();
  XPT2046_MOSI_LOW();
  XPT2046_CS_ENABLE();
  XPT2046_SendCmd(cmd);
  XPT2046_DelayUs(5);
  XPT2046_SCK_LOW();
  XPT2046_DelayUs(5);
  XPT2046_SCK_HIGH();
  XPT2046_DelayUs(5);
  XPT2046_SCK_LOW();
  for (i = 0; i < 16; i ++)
  {
    data <<= 1;

    XPT2046_SCK_LOW();
    XPT2046_DelayUs(5);
    XPT2046_SCK_HIGH();
    XPT2046_DelayUs(5);
    if (XPT2046_MISO_READ() == GPIO_PIN_SET)
    {
      data ++;
    }
  }
  data >>= 4;
  XPT2046_CS_DISABLE();
  return data;
}

/** 
 * @brief  XPT2046 state.
 * @retval  XPT2046 state.
 */
uint8_t XPT2046_Touch_Detect(void)
{
  uint8_t result = 0;
  static Touch_State touch_state;
  uint8_t i = 0;
  switch (touch_state)
  {
    case XPT2046_STATE_RELEASE:
      if (XPT2046_PENIRQ_READ() == 0)
      {
        touch_state = XPT2046_STATE_WAITING;
        result = XPT2046_NOT_PRESSED;
      }
      else
      {
        touch_state = XPT2046_STATE_PRESSED;
        result = XPT2046_NOT_PRESSED;
      }
    break;

    case XPT2046_STATE_WAITING:
      if (XPT2046_PENIRQ_READ() == 0)
      {
        i ++;
        if (i > 5)
        {
          touch_state = XPT2046_STATE_PRESSED;
          result = XPT2046_PRESSED;
          i = 0;
        }
        else
        {
          touch_state = XPT2046_STATE_WAITING;
          result = XPT2046_NOT_PRESSED;
        }
      }
      else
      {
        i = 0;
        touch_state = XPT2046_STATE_RELEASE;
      }
    break;

    case XPT2046_STATE_PRESSED:
      if (XPT2046_PENIRQ_READ() == 0)
      {
        touch_state = XPT2046_STATE_PRESSED;
        result = XPT2046_PRESSED;
      }
      else
      {
        touch_state = XPT2046_STATE_RELEASE;
        result = XPT2046_NOT_PRESSED;
      }
    break;
  }

  return result;
}

/** 
 * @brief  Read XPT2046 coordinates.
 * @param  x XPT2046 x coordinate.
 * @param  y XPT2046 y coordinate.
 * @retval  OK;
 */
uint8_t XPT2046_Read_XY1(uint16_t *x, uint16_t *y)
{
  uint16_t temp1, temp2;
  temp1 = XPT2046_ReceiveData(CMD_RDX);
  temp2 = XPT2046_ReceiveData(CMD_RDY);
  *x = temp1;
  *y = temp2;
  return 1;
}

/** 
 * @brief  The second read XPT2046 coordinates to get accuracy coordinate.
 * @param  x To receive XPT2046 x coordinate.
 * @param  y To receive XPT2046 y coordinate.
 * @retval OK(1) / OFF(0)
 */
uint8_t XPT2046_Read_XY2(uint16_t *x, uint16_t *y)
{
  uint16_t x1, x2;
  uint16_t y1, y2;
  uint8_t flag;
  flag = XPT2046_Read_XY1(&x1, &y1);
  if (flag == 0)
  {
    return 0;
  }
  flag = XPT2046_Read_XY1(&x2, &y2);
  if (flag == 0)
  {
    return 0;
  }
  if (((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE)) && ((y2 <= y1 && y1 < y2 + ERR_RANGE) || (y1 <= y2 && y2 < y1 + ERR_RANGE)))
  {
    *x = (x1 + x2) / 2;
    *y = (y1 + y2) / 2;
    return 1;
  }
  else return 0;
}

/** 
 * @brief  Save adc sampling data.
 */
void XPT2046_Save_ADjdata(void)
{
  uint32_t temp = 0;
//  temp = xpt_dev.xfac * 100000000;
//  W25QXX_Write_Int32(temp, FLASH_SIZE-(4096*1)-250, 4);
//  temp = xpt_dev.yfac * 100000000;
//  W25QXX_Write_Int32(temp, FLASH_SIZE-(4096*2)-250, 4);
  W25QXX_Write_u32((xpt_dev.xoff), FLASH_SIZE-(4096*8)-777, 2);
  W25QXX_Write_u32((xpt_dev.yoff), FLASH_SIZE-(4096*9)-777, 2);
  W25QXX_Write_u32((xpt_dev.TouchType), FLASH_SIZE-(4096*10)-77, 1);
  temp = 0x0a;
  W25QXX_Write_u32(temp, FLASH_SIZE-(4096*11)-77, 1);
}

/** 
 * @brief  Get adc sampling data from flash.
 * @retval  If without data, return OFF(0), else return ON(1).
 */
uint8_t XPT2046_Get_ADjdata(void)
{
  uint32_t temp = 0;
  W25QXX_Read_u32((uint32_t *)&temp, FLASH_SIZE-(4096*11)-77, 1);
  if (temp == 0x0a)
  {
    POINT_COLOR = WHITE;
    ILI9341_Display_Whole_Color();
//    W25QXX_Read_Int32((int32_t*)&temp, FLASH_SIZE-(4096*1)-250, 4);
//    xpt_dev.xfac = (float)temp/100000000;
//    xpt_dev.xfac = 0.000034;
//    W25QXX_Read_Int32((int32_t*)&temp, FLASH_SIZE-(4096*2)-250, 4);
//    xpt_dev.yfac = (float)temp/100000000;
//    xpt_dev.yfac = 0.0000234;

/*    xpt_dev.xfac = -0.089342691;
    xpt_dev.yfac = -0.0648298189;
    xpt_dev.xoff = 338;
    xpt_dev.yoff = 248;
    xpt_dev.TouchType = 1;*/

/*    xpt_dev.xfac = -0.0902643427;
    xpt_dev.yfac = -0.0655093342;
    xpt_dev.xoff = 332;
    xpt_dev.yoff = 259;
    xpt_dev.TouchType = 1;*/

    xpt_dev.xfac = -0.090644218;
    xpt_dev.yfac = -0.0647458732;
    xpt_dev.xoff = 338;
    xpt_dev.yoff = 252;
    xpt_dev.TouchType = 1;

//    W25QXX_Read_u32(&temp, FLASH_SIZE-(4096*8)-777, 2);
//    xpt_dev.xoff = temp;
//    W25QXX_Read_u32(&temp, FLASH_SIZE-(4096*9)-777, 2);
//    xpt_dev.yoff = temp;
//    W25QXX_Read_u32(&temp, FLASH_SIZE-(4096*10)-77, 1);
//    xpt_dev.TouchType = temp;
    if (xpt_dev.TouchType)
    {
      CMD_RDX = 0x90;
      CMD_RDY = 0xd0;
    }
    else
    {
      CMD_RDX = 0xd0;
      CMD_RDY = 0x90;
    }
    return 1;
  }
  else
  {
    return 0;
  }
}

/** 
 * @brief  CHeck whether the screen is pressed.
 * @param  tp if tp = 1, calibration the screen.
 *            else Get x, y coordinate.
 * @retval  The screen state.
 */
uint8_t XPT2046_Scan(uint8_t tp)
{
  if (XPT2046_PENIRQ_READ() == 0)
  {
    LED1_OFF;
    LED2_ON;
    if (tp)
    {
//      xpt_dev.x[0] = XPT2046_ReceiveData(CMD_RDX);
//      xpt_dev.y[0] = XPT2046_ReceiveData(CMD_RDY);
        XPT2046_Read_XY2(&xpt_dev.x[0], &xpt_dev.y[0]);
    }
    else
    {
//      xpt_dev.x[0] = xpt_dev.xfac * XPT2046_ReceiveData(CMD_RDX) + xpt_dev.xoff;
//      xpt_dev.y[0] = xpt_dev.yfac * XPT2046_ReceiveData(CMD_RDY) + xpt_dev.yoff;
        if (XPT2046_Read_XY2(&xpt_dev.x[0], &xpt_dev.y[0]))
        {
          xpt_dev.x[0] = xpt_dev.xfac * xpt_dev.x[0] + xpt_dev.xoff;
          xpt_dev.y[0] = xpt_dev.yfac * xpt_dev.y[0] + xpt_dev.yoff;
        }
    }
    if ((xpt_dev.sta & TP_PRES_DOWN) == 0)
    {
      xpt_dev.sta=TP_PRES_DOWN | TP_CATH_PRES;
      xpt_dev.x[4] = xpt_dev.x[0];
      xpt_dev.y[4] = xpt_dev.y[0];
    }
  }
  else
  {
    if (xpt_dev.sta & TP_PRES_DOWN)
    {
      xpt_dev.sta &= ~(1 << 7);
    }
    else
    {
      xpt_dev.x[0] = 0xffff;
      xpt_dev.y[0] = 0xffff;
      xpt_dev.y[4] = 0;
      xpt_dev.x[4] = 0;
    }
  }
  return xpt_dev.sta & TP_PRES_DOWN;
}

/** 
 * @brief  Display adc smapling data.
 */
void XPT2046_Adj_Info_Show(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t fac)
{    
  POINT_COLOR=YELLOW;
  ILI9341_ShowString(40,160,Lcd_Dev.width,Lcd_Dev.height,16,(uint8_t *)"x1:");
  ILI9341_ShowString(40+80,160,Lcd_Dev.width,Lcd_Dev.height,16,(uint8_t *)"y1:");
  ILI9341_ShowString(40,180,Lcd_Dev.width,Lcd_Dev.height,16,(uint8_t *)"x2:");
  ILI9341_ShowString(40+80,180,Lcd_Dev.width,Lcd_Dev.height,16,(uint8_t *)"y2:");
  ILI9341_ShowString(40,200,Lcd_Dev.width,Lcd_Dev.height,16,(uint8_t *)"x3:");
  ILI9341_ShowString(40+80,200,Lcd_Dev.width,Lcd_Dev.height,16,(uint8_t *)"y3:");
  ILI9341_ShowString(40,220,Lcd_Dev.width,Lcd_Dev.height,16,(uint8_t *)"x4:");
  ILI9341_ShowString(40+80,220,Lcd_Dev.width,Lcd_Dev.height,16,(uint8_t *)"y4:");  
  ILI9341_ShowString(145+60,160,Lcd_Dev.width,Lcd_Dev.height,16,(uint8_t *)"fac is:");     
  ILI9341_ShowNum(40+24,160,x0,4,16);
  ILI9341_ShowNum(40+24+80,160,y0,4,16);
  ILI9341_ShowNum(40+24,180,x1,4,16);
  ILI9341_ShowNum(40+24+80,180,y1,4,16);
  ILI9341_ShowNum(40+24,200,x2,4,16);  
  ILI9341_ShowNum(40+24+80,200,y2,4,16);
  ILI9341_ShowNum(40+24,220,x3,4,16);  
  ILI9341_ShowNum(40+24+80,220,y3,4,16);
  ILI9341_ShowNum(145+100,160,fac,4,16);
}

/** 
 * @brief  Start to adc smapling data.
 */
void XPT2046_ADjust(void)
{
  uint16_t pos_temp[4][2];
  uint8_t cnt = 0;  
  uint16_t d1, d2;
  uint32_t tem1, tem2;
  double fac;   
  uint16_t outtime = 0;

  /*  Clear screen  */
  POINT_COLOR = BLACK;
  ILI9341_Display_Whole_Color();

  /*  Display message  */
  POINT_COLOR = YELLOW;
  ILI9341_ShowString(20, 50, (Lcd_Dev.width)-70, (Lcd_Dev.height), 16, (uint8_t *)TP_REMIND_MSG_TBL);
  ILI9341_Touch_Point(20, 20, BLUE);
  xpt_dev.sta = 0;
  xpt_dev.xfac = 0;
  while (1)
  {
    xpt_dev.scan(1);
    if ((xpt_dev.sta & 0xc0) == TP_CATH_PRES)
    {
      outtime = 0;
      xpt_dev.sta &= ~(1 << 6);
      pos_temp[cnt][0] = xpt_dev.x[0];
      pos_temp[cnt][1] = xpt_dev.y[0];
      cnt ++;
      switch (cnt)
      {
        case 1:
          /*  Clear flag1 color  */
          ILI9341_Touch_Point(20, 20, BLACK);
          ILI9341_Touch_Point(Lcd_Dev.width-20, 20, BLUE);
        break;

        case 2:
          /*  Clear flag2 color  */
          ILI9341_Touch_Point(Lcd_Dev.width-20, 20, BLACK);
          ILI9341_Touch_Point(20, Lcd_Dev.height - 20, BLUE);
        break;

        case 3:
          /*  Clear flag3 color  */
          ILI9341_Touch_Point(20, Lcd_Dev.height - 20, BLACK);
          ILI9341_Touch_Point(Lcd_Dev.width-20, Lcd_Dev.height - 20, BLUE);
        break;

        case 4:
          tem1 = abs(pos_temp[0][0] - pos_temp[1][0]);
          tem2 = abs(pos_temp[0][1] - pos_temp[1][1]);
          tem1 *= tem1;
          tem2 *= tem2;
          d1 = sqrt(tem1 + tem2);

          tem1 = abs(pos_temp[2][0] - pos_temp[3][0]);
          tem2 = abs(pos_temp[2][1] - pos_temp[3][1]);
          tem1 *= tem1;
          tem2 *= tem2;
          d2 = sqrt(tem1 + tem2);
          fac = (float)(d1 / d2);
          if (fac < 0.95 || fac > 1.05 || d1 == 0 || d2 == 0)
          {
            cnt = 0;
            ILI9341_Touch_Point(Lcd_Dev.width-20, Lcd_Dev.height-20, BLACK);
            ILI9341_Touch_Point(20, 20, BLUE);
            XPT2046_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);
            continue;
          }
          tem1 = abs(pos_temp[0][0] - pos_temp[2][0]);
          tem2 = abs(pos_temp[0][1] - pos_temp[2][1]);
          tem1 *= tem1;
          tem2 *= tem2;
          d1 = sqrt(tem1 + tem2);

          tem1 = abs(pos_temp[1][0] - pos_temp[3][0]);
          tem2 = abs(pos_temp[1][1] - pos_temp[3][1]);
          tem1 *= tem1;
          tem2 *= tem2;
          d2 = sqrt(tem1 + tem2);
          fac = (float)(d1 / d2);
          if (fac < 0.95 || fac > 1.05)
          {
            cnt = 0;
            ILI9341_Touch_Point(Lcd_Dev.width-20, Lcd_Dev.height-20, BLACK);
            ILI9341_Touch_Point(20, 20, BLUE);
            XPT2046_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);
            continue;
          }
          tem1 = abs(pos_temp[1][0] - pos_temp[2][0]);
          tem2 = abs(pos_temp[1][1] - pos_temp[2][1]);
          tem1 *= tem1;
          tem2 *= tem2;
          d1 = sqrt(tem1 + tem2);

          tem1 = abs(pos_temp[0][0] - pos_temp[3][0]);
          tem2 = abs(pos_temp[0][1] - pos_temp[3][1]);
          tem1 *= tem1;
          tem2 *= tem2;
          d2 = sqrt(tem1 + tem2);
          fac = (float)(d1 / d2);
          if (fac < 0.95 || fac > 1.05)
          {
            cnt = 0;
            ILI9341_Touch_Point(Lcd_Dev.width-20, Lcd_Dev.height-20, BLACK);
            ILI9341_Touch_Point(20, 20, BLUE);
            XPT2046_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);
            continue;
          }
          xpt_dev.xfac=(float)(Lcd_Dev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);
					xpt_dev.xoff=(Lcd_Dev.width-xpt_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;
					xpt_dev.yfac=(float)(Lcd_Dev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);
					xpt_dev.yoff=(Lcd_Dev.height-xpt_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;
          POINT_COLOR = YELLOW;
          if (abs(xpt_dev.xfac) > 2 || abs(xpt_dev.yfac) > 2)
          {
            cnt = 0;
            ILI9341_Touch_Point(Lcd_Dev.width-20, Lcd_Dev.height - 20, BLACK);
            ILI9341_Touch_Point(20, 20, BLUE);
            ILI9341_ShowString(40, 26, Lcd_Dev.width, Lcd_Dev.height, 16, (uint8_t *)"TP Need readjust");
            xpt_dev.TouchType = ! xpt_dev.TouchType;
            if (xpt_dev.TouchType)
            {
              CMD_RDX = 0x90;
              CMD_RDY = 0xD0;
            }
            else
            {
              CMD_RDX = 0xD0;
              CMD_RDY = 0x90;
            }
            continue;
          }
          POINT_COLOR = YELLOW;
          ILI9341_Display_Whole_Color();
          POINT_COLOR = BLUE;
          ILI9341_ShowString(35+20, 110-30, Lcd_Dev.width, Lcd_Dev.height, 24, (uint8_t *)"Touch Screen ");
          ILI9341_ShowString(35+20, 110+10, Lcd_Dev.width, Lcd_Dev.height, 24, (uint8_t *)"ADjust OK!");
          XPT2046_Save_ADjdata();
          return;
      }
    }
    HAL_Delay(5);
    outtime ++;
    if (outtime > 1000)
    {
      POINT_COLOR = BLACK;
      ILI9341_Display_Whole_Color();
      POINT_COLOR = BLUE;
      ILI9341_ShowString(35+20, 110-30, Lcd_Dev.width, Lcd_Dev.height, 24, (uint8_t *)"Touch timeout");
      break;
    }
  }
}

/** 
 * @brief  Drwa point at pressed.
 * @param  x To get from screen scan.
 * @param  y To get from screen scan.
 * @param  color Draw color.
 */
void XPT2046_Draw_Big_Point(uint16_t x, uint16_t y, uint16_t color)
{
  POINT_COLOR = color;
  if (POINT_SIZE == 4)
  {
    ILI9341_Draw_ALLCircle(x, y, 4);
    /*
    ILI9341_Open_Window(Lcd_Dev.width, Lcd_Dev.height, 0, 0);
    ILI9341_Fast_DrawPoint(x, y, color);
    ILI9341_Fast_DrawPoint(x+1, y, color);
    ILI9341_Fast_DrawPoint(x+2, y, color);
    ILI9341_Fast_DrawPoint(x+3, y, color);
    ILI9341_Fast_DrawPoint(x+4, y, color);
    ILI9341_Fast_DrawPoint(x, y+1, color);
    ILI9341_Fast_DrawPoint(x, y+2, color);
    ILI9341_Fast_DrawPoint(x, y+3, color);
    ILI9341_Fast_DrawPoint(x, y+4, color);
    ILI9341_Fast_DrawPoint(x+1, y+1, color);
    ILI9341_Fast_DrawPoint(x+2, y+2, color);
    ILI9341_Fast_DrawPoint(x+3, y+3, color);
    ILI9341_Fast_DrawPoint(x+4, y+4, color);

    ILI9341_Fast_DrawPoint(x+1, y+2, color);
    ILI9341_Fast_DrawPoint(x+1, y+3, color);
    ILI9341_Fast_DrawPoint(x+1, y+4, color);
    ILI9341_Fast_DrawPoint(x-1, y-2, color);
    ILI9341_Fast_DrawPoint(x-1, y-3, color);
    ILI9341_Fast_DrawPoint(x-1, y-4, color);

    ILI9341_Fast_DrawPoint(x+2, y+2, color);
    ILI9341_Fast_DrawPoint(x+2, y+3, color);
    ILI9341_Fast_DrawPoint(x+2, y+4, color);
    ILI9341_Fast_DrawPoint(x-2, y-2, color);
    ILI9341_Fast_DrawPoint(x-2, y-3, color);
    ILI9341_Fast_DrawPoint(x-2, y-4, color);

    ILI9341_Fast_DrawPoint(x+3, y+2, color);
    ILI9341_Fast_DrawPoint(x+3, y+3, color);
    ILI9341_Fast_DrawPoint(x+3, y+4, color);
    ILI9341_Fast_DrawPoint(x-3, y-2, color);
    ILI9341_Fast_DrawPoint(x-3, y-3, color);
    ILI9341_Fast_DrawPoint(x-3, y-4, color);

    ILI9341_Fast_DrawPoint(x+4, y+2, color);
    ILI9341_Fast_DrawPoint(x+4, y+3, color);
    ILI9341_Fast_DrawPoint(x+4, y+4, color);
    ILI9341_Fast_DrawPoint(x-4, y-2, color);
    ILI9341_Fast_DrawPoint(x-4, y-3, color);
    ILI9341_Fast_DrawPoint(x-4, y-4, color);

    ILI9341_Fast_DrawPoint(x-1, y, color);
    ILI9341_Fast_DrawPoint(x-2, y, color);
    ILI9341_Fast_DrawPoint(x-3, y, color);
    ILI9341_Fast_DrawPoint(x-4, y, color);
    ILI9341_Fast_DrawPoint(x-1, y-1, color);
    ILI9341_Fast_DrawPoint(x-2, y-2, color);
    ILI9341_Fast_DrawPoint(x-3, y-3, color);
    ILI9341_Fast_DrawPoint(x-4, y-4, color);
    ILI9341_Fast_DrawPoint(x, y-1, color);
    ILI9341_Fast_DrawPoint(x, y-2, color);
    ILI9341_Fast_DrawPoint(x, y-3, color);
    ILI9341_Fast_DrawPoint(x, y-4, color);*/
  }
  else if (POINT_SIZE == 5)
  {
        ILI9341_Draw_ALLCircle(x, y, 5);
    /*
    ILI9341_Open_Window(Lcd_Dev.width, Lcd_Dev.height, 0, 0);
    ILI9341_Fast_DrawPoint(x, y, color);
    ILI9341_Fast_DrawPoint(x+1, y, color);
    ILI9341_Fast_DrawPoint(x+2, y, color);
    ILI9341_Fast_DrawPoint(x+3, y, color);
    ILI9341_Fast_DrawPoint(x+4, y, color);
    ILI9341_Fast_DrawPoint(x+5, y, color);
    ILI9341_Fast_DrawPoint(x, y+1, color);
    ILI9341_Fast_DrawPoint(x, y+2, color);
    ILI9341_Fast_DrawPoint(x, y+3, color);
    ILI9341_Fast_DrawPoint(x, y+4, color);
    ILI9341_Fast_DrawPoint(x, y+5, color);
    ILI9341_Fast_DrawPoint(x+1, y+1, color);
    ILI9341_Fast_DrawPoint(x+2, y+2, color);
    ILI9341_Fast_DrawPoint(x+3, y+3, color);
    ILI9341_Fast_DrawPoint(x+4, y+4, color);
    ILI9341_Fast_DrawPoint(x+5, y+5, color);

    ILI9341_Fast_DrawPoint(x+1, y+2, color);
    ILI9341_Fast_DrawPoint(x+1, y+3, color);
    ILI9341_Fast_DrawPoint(x+1, y+4, color);
    ILI9341_Fast_DrawPoint(x+1, y+5, color);
    ILI9341_Fast_DrawPoint(x-1, y-2, color);
    ILI9341_Fast_DrawPoint(x-1, y-3, color);
    ILI9341_Fast_DrawPoint(x-1, y-4, color);
    ILI9341_Fast_DrawPoint(x-1, y-5, color);

    ILI9341_Fast_DrawPoint(x+2, y+2, color);
    ILI9341_Fast_DrawPoint(x+2, y+3, color);
    ILI9341_Fast_DrawPoint(x+2, y+4, color);
    ILI9341_Fast_DrawPoint(x+2, y+5, color);
    ILI9341_Fast_DrawPoint(x-2, y-2, color);
    ILI9341_Fast_DrawPoint(x-2, y-3, color);
    ILI9341_Fast_DrawPoint(x-2, y-4, color);
    ILI9341_Fast_DrawPoint(x-2, y-5, color);

    ILI9341_Fast_DrawPoint(x+3, y+2, color);
    ILI9341_Fast_DrawPoint(x+3, y+3, color);
    ILI9341_Fast_DrawPoint(x+3, y+4, color);
    ILI9341_Fast_DrawPoint(x+3, y+5, color);
    ILI9341_Fast_DrawPoint(x-3, y-2, color);
    ILI9341_Fast_DrawPoint(x-3, y-3, color);
    ILI9341_Fast_DrawPoint(x-3, y-4, color);
    ILI9341_Fast_DrawPoint(x-3, y-5, color);

    ILI9341_Fast_DrawPoint(x+4, y+2, color);
    ILI9341_Fast_DrawPoint(x+4, y+3, color);
    ILI9341_Fast_DrawPoint(x+4, y+4, color);
    ILI9341_Fast_DrawPoint(x+4, y+5, color);
    ILI9341_Fast_DrawPoint(x-4, y-2, color);
    ILI9341_Fast_DrawPoint(x-4, y-3, color);
    ILI9341_Fast_DrawPoint(x-4, y-4, color);
    ILI9341_Fast_DrawPoint(x-4, y-5, color);

    ILI9341_Fast_DrawPoint(x-1, y, color);
    ILI9341_Fast_DrawPoint(x-2, y, color);
    ILI9341_Fast_DrawPoint(x-3, y, color);
    ILI9341_Fast_DrawPoint(x-4, y, color);
    ILI9341_Fast_DrawPoint(x-5, y, color);
    ILI9341_Fast_DrawPoint(x-1, y-1, color);
    ILI9341_Fast_DrawPoint(x-2, y-2, color);
    ILI9341_Fast_DrawPoint(x-3, y-3, color);
    ILI9341_Fast_DrawPoint(x-4, y-4, color);
    ILI9341_Fast_DrawPoint(x-5, y-5, color);
    ILI9341_Fast_DrawPoint(x, y-1, color);
    ILI9341_Fast_DrawPoint(x, y-2, color);
    ILI9341_Fast_DrawPoint(x, y-3, color);
    ILI9341_Fast_DrawPoint(x, y-4, color);
    ILI9341_Fast_DrawPoint(x, y-5, color);*/
  }
  else if (POINT_SIZE == 6)
  {
    ILI9341_Draw_ALLCircle(x, y, 6);
    /*
    ILI9341_Open_Window(Lcd_Dev.width, Lcd_Dev.height, 0, 0);
    ILI9341_Fast_DrawPoint(x, y, color);
    ILI9341_Fast_DrawPoint(x+1, y, color);
    ILI9341_Fast_DrawPoint(x+2, y, color);
    ILI9341_Fast_DrawPoint(x+3, y, color);
    ILI9341_Fast_DrawPoint(x+4, y, color);
    ILI9341_Fast_DrawPoint(x+5, y, color);
    ILI9341_Fast_DrawPoint(x+6, y, color);
    ILI9341_Fast_DrawPoint(x, y+1, color);
    ILI9341_Fast_DrawPoint(x, y+2, color);
    ILI9341_Fast_DrawPoint(x, y+3, color);
    ILI9341_Fast_DrawPoint(x, y+4, color);
    ILI9341_Fast_DrawPoint(x, y+5, color);
    ILI9341_Fast_DrawPoint(x, y+6, color);
    ILI9341_Fast_DrawPoint(x+1, y+1, color);
    ILI9341_Fast_DrawPoint(x+2, y+2, color);
    ILI9341_Fast_DrawPoint(x+3, y+3, color);
    ILI9341_Fast_DrawPoint(x+4, y+4, color);
    ILI9341_Fast_DrawPoint(x+5, y+5, color);
    ILI9341_Fast_DrawPoint(x+6, y+6, color);

    ILI9341_Fast_DrawPoint(x+1, y+2, color);
    ILI9341_Fast_DrawPoint(x+1, y+3, color);
    ILI9341_Fast_DrawPoint(x+1, y+4, color);
    ILI9341_Fast_DrawPoint(x+1, y+5, color);
    ILI9341_Fast_DrawPoint(x+1, y+6, color);
    ILI9341_Fast_DrawPoint(x-1, y-2, color);
    ILI9341_Fast_DrawPoint(x-1, y-3, color);
    ILI9341_Fast_DrawPoint(x-1, y-4, color);
    ILI9341_Fast_DrawPoint(x-1, y-5, color);
    ILI9341_Fast_DrawPoint(x-1, y-6, color);

    ILI9341_Fast_DrawPoint(x+2, y+2, color);
    ILI9341_Fast_DrawPoint(x+2, y+3, color);
    ILI9341_Fast_DrawPoint(x+2, y+4, color);
    ILI9341_Fast_DrawPoint(x+2, y+5, color);
    ILI9341_Fast_DrawPoint(x+2, y+6, color);
    ILI9341_Fast_DrawPoint(x-2, y-2, color);
    ILI9341_Fast_DrawPoint(x-2, y-3, color);
    ILI9341_Fast_DrawPoint(x-2, y-4, color);
    ILI9341_Fast_DrawPoint(x-2, y-5, color);
    ILI9341_Fast_DrawPoint(x-2, y-6, color);

    ILI9341_Fast_DrawPoint(x+3, y+2, color);
    ILI9341_Fast_DrawPoint(x+3, y+3, color);
    ILI9341_Fast_DrawPoint(x+3, y+4, color);
    ILI9341_Fast_DrawPoint(x+3, y+5, color);
    ILI9341_Fast_DrawPoint(x+3, y+6, color);
    ILI9341_Fast_DrawPoint(x-3, y-2, color);
    ILI9341_Fast_DrawPoint(x-3, y-3, color);
    ILI9341_Fast_DrawPoint(x-3, y-4, color);
    ILI9341_Fast_DrawPoint(x-3, y-5, color);
    ILI9341_Fast_DrawPoint(x-3, y-6, color);

    ILI9341_Fast_DrawPoint(x+4, y+2, color);
    ILI9341_Fast_DrawPoint(x+4, y+3, color);
    ILI9341_Fast_DrawPoint(x+4, y+4, color);
    ILI9341_Fast_DrawPoint(x+4, y+5, color);
    ILI9341_Fast_DrawPoint(x+4, y+6, color);
    ILI9341_Fast_DrawPoint(x+4, y+6, color);
    ILI9341_Fast_DrawPoint(x-4, y-2, color);
    ILI9341_Fast_DrawPoint(x-4, y-3, color);
    ILI9341_Fast_DrawPoint(x-4, y-4, color);
    ILI9341_Fast_DrawPoint(x-4, y-5, color);
    ILI9341_Fast_DrawPoint(x-4, y-6, color);

    ILI9341_Fast_DrawPoint(x-1, y, color);
    ILI9341_Fast_DrawPoint(x-2, y, color);
    ILI9341_Fast_DrawPoint(x-3, y, color);
    ILI9341_Fast_DrawPoint(x-4, y, color);
    ILI9341_Fast_DrawPoint(x-5, y, color);
    ILI9341_Fast_DrawPoint(x-6, y, color);
    ILI9341_Fast_DrawPoint(x-1, y-1, color);
    ILI9341_Fast_DrawPoint(x-2, y-2, color);
    ILI9341_Fast_DrawPoint(x-3, y-3, color);
    ILI9341_Fast_DrawPoint(x-4, y-4, color);
    ILI9341_Fast_DrawPoint(x-5, y-5, color);
    ILI9341_Fast_DrawPoint(x-6, y-6, color);
    ILI9341_Fast_DrawPoint(x, y-1, color);
    ILI9341_Fast_DrawPoint(x, y-2, color);
    ILI9341_Fast_DrawPoint(x, y-3, color);
    ILI9341_Fast_DrawPoint(x, y-4, color);
    ILI9341_Fast_DrawPoint(x, y-5, color);
    ILI9341_Fast_DrawPoint(x, y-6, color);*/
  }
  else if (POINT_SIZE == 3)
  {
    ILI9341_Draw_ALLCircle(x, y, 3);
/*
    ILI9341_Open_Window(Lcd_Dev.width, Lcd_Dev.height, 0, 0);
    ILI9341_Fast_DrawPoint(x, y, color);
    ILI9341_Fast_DrawPoint(x+1, y, color);
    ILI9341_Fast_DrawPoint(x+2, y, color);
    ILI9341_Fast_DrawPoint(x+3, y, color);
    ILI9341_Fast_DrawPoint(x, y+1, color);
    ILI9341_Fast_DrawPoint(x, y+2, color);
    ILI9341_Fast_DrawPoint(x, y+3, color);
    ILI9341_Fast_DrawPoint(x+1, y+1, color);
    ILI9341_Fast_DrawPoint(x+2, y+2, color);
    ILI9341_Fast_DrawPoint(x+3, y+3, color);

    ILI9341_Fast_DrawPoint(x+1, y+2, color);
    ILI9341_Fast_DrawPoint(x+1, y+3, color);
    ILI9341_Fast_DrawPoint(x-1, y-2, color);
    ILI9341_Fast_DrawPoint(x-1, y-3, color);

    ILI9341_Fast_DrawPoint(x+2, y+2, color);
    ILI9341_Fast_DrawPoint(x+2, y+3, color);
    ILI9341_Fast_DrawPoint(x-2, y-2, color);
    ILI9341_Fast_DrawPoint(x-2, y-3, color);

    ILI9341_Fast_DrawPoint(x+3, y+2, color);
    ILI9341_Fast_DrawPoint(x+3, y+3, color);
    ILI9341_Fast_DrawPoint(x-3, y-2, color);
    ILI9341_Fast_DrawPoint(x-3, y-3, color);

    ILI9341_Fast_DrawPoint(x-1, y, color);
    ILI9341_Fast_DrawPoint(x-2, y, color);
    ILI9341_Fast_DrawPoint(x-3, y, color);
    ILI9341_Fast_DrawPoint(x-1, y-1, color);
    ILI9341_Fast_DrawPoint(x-2, y-2, color);
    ILI9341_Fast_DrawPoint(x-3, y-3, color);
    ILI9341_Fast_DrawPoint(x, y-1, color);
    ILI9341_Fast_DrawPoint(x, y-2, color);
    ILI9341_Fast_DrawPoint(x, y-3, color);*/
  }
  else if (POINT_SIZE == 2)
  {
    ILI9341_Draw_ALLCircle(x, y, 2);
/*    ILI9341_Open_Window(Lcd_Dev.width, Lcd_Dev.height, 0, 0);
    ILI9341_Fast_DrawPoint(x, y, color);
    ILI9341_Fast_DrawPoint(x+1, y, color);
    ILI9341_Fast_DrawPoint(x+2, y, color);
    ILI9341_Fast_DrawPoint(x, y+1, color);
    ILI9341_Fast_DrawPoint(x, y+2, color);
    ILI9341_Fast_DrawPoint(x+1, y+1, color);
    ILI9341_Fast_DrawPoint(x+2, y+2, color);

    ILI9341_Fast_DrawPoint(x+1, y+2, color);
    ILI9341_Fast_DrawPoint(x-1, y-2, color);

    ILI9341_Fast_DrawPoint(x+2, y+2, color);
    ILI9341_Fast_DrawPoint(x-2, y-2, color);

    ILI9341_Fast_DrawPoint(x+3, y+2, color);
    ILI9341_Fast_DrawPoint(x-3, y-2, color);

    ILI9341_Fast_DrawPoint(x-1, y, color);
    ILI9341_Fast_DrawPoint(x-2, y, color);
    ILI9341_Fast_DrawPoint(x-1, y-1, color);
    ILI9341_Fast_DrawPoint(x-2, y-2, color);
    ILI9341_Fast_DrawPoint(x, y-1, color);
    ILI9341_Fast_DrawPoint(x, y-2, color);*/
  }
    else if (POINT_SIZE == 1)
  {
/*    ILI9341_Open_Window(Lcd_Dev.width, Lcd_Dev.height, 0, 0);
    ILI9341_Fast_DrawPoint(x, y, color);
    ILI9341_Fast_DrawPoint(x+1, y, color);
//    ILI9341_Fast_DrawPoint(x+2, y, color);
    ILI9341_Fast_DrawPoint(x, y+1, color);
//    ILI9341_Fast_DrawPoint(x, y+2, color);
    ILI9341_Fast_DrawPoint(x+1, y+1, color);
//    ILI9341_Fast_DrawPoint(x+2, y+2, color);

//    ILI9341_Fast_DrawPoint(x+1, y+2, color);
//    ILI9341_Fast_DrawPoint(x-1, y-2, color);
//
//    ILI9341_Fast_DrawPoint(x+2, y+2, color);
//    ILI9341_Fast_DrawPoint(x-2, y-2, color);
//
//    ILI9341_Fast_DrawPoint(x+3, y+2, color);
//    ILI9341_Fast_DrawPoint(x-3, y-2, color);

    ILI9341_Fast_DrawPoint(x-1, y, color);
//    ILI9341_Fast_DrawPoint(x-2, y, color);
    ILI9341_Fast_DrawPoint(x-1, y-1, color);
//    ILI9341_Fast_DrawPoint(x-2, y-2, color);
    ILI9341_Fast_DrawPoint(x, y-1, color);
//    ILI9341_Fast_DrawPoint(x, y-2, color);*/
    ILI9341_Draw_ALLCircle(x, y, 1);
  }
/*  else
  {
    ILI9341_Open_Window(Lcd_Dev.width, Lcd_Dev.height, 0, 0);
    ILI9341_Fast_DrawPoint(x, y, color);
    ILI9341_Fast_DrawPoint(x+1, y, color);
    ILI9341_Fast_DrawPoint(x+2, y, color);
    ILI9341_Fast_DrawPoint(x+3, y, color);
    ILI9341_Fast_DrawPoint(x+4, y, color);
    ILI9341_Fast_DrawPoint(x+5, y, color);
    ILI9341_Fast_DrawPoint(x+6, y, color);
    ILI9341_Fast_DrawPoint(x+7, y, color);
    ILI9341_Fast_DrawPoint(x+8, y, color);
    ILI9341_Fast_DrawPoint(x, y+1, color);
    ILI9341_Fast_DrawPoint(x, y+2, color);
    ILI9341_Fast_DrawPoint(x, y+3, color);
    ILI9341_Fast_DrawPoint(x, y+4, color);
    ILI9341_Fast_DrawPoint(x, y+5, color);
    ILI9341_Fast_DrawPoint(x, y+6, color);
    ILI9341_Fast_DrawPoint(x, y+7, color);
    ILI9341_Fast_DrawPoint(x, y+8, color);
    ILI9341_Fast_DrawPoint(x+1, y+1, color);
    ILI9341_Fast_DrawPoint(x+2, y+2, color);
    ILI9341_Fast_DrawPoint(x+3, y+3, color);
    ILI9341_Fast_DrawPoint(x+4, y+4, color);
    ILI9341_Fast_DrawPoint(x+5, y+5, color);
    ILI9341_Fast_DrawPoint(x+6, y+6, color);
    ILI9341_Fast_DrawPoint(x+7, y+7, color);
    ILI9341_Fast_DrawPoint(x+8, y+8, color);
  }*/
}

/** 
 * @brief  XPT2046 draw display init.
 */
void XPT2046_Draw_Dralog(void)
{
  POINT_COLOR = WHITE;
  ILI9341_Display_Whole_Color();
  POINT_COLOR = BLACK;
  ILI9341_Draw_Rec(Lcd_Dev.width-320, 0, 50, 40);
  POINT_COLOR = YELLOW;
  ILI9341_Draw_Rec(Lcd_Dev.width-320, 40, 50, 40);
  POINT_COLOR = BLUE;
  ILI9341_Draw_Rec(Lcd_Dev.width-320, 80, 50, 40);
  POINT_COLOR = GREEN;
  ILI9341_Draw_Rec(Lcd_Dev.width-320, 120, 50, 40);
  POINT_COLOR = WHITE;
  ILI9341_Draw_Rec(Lcd_Dev.width-320, 160, 50, 40);
  POINT_COLOR = GRAY;
  ILI9341_Draw_Rec(Lcd_Dev.width-320, 200, 50, 40);
  POINT_COLOR = BLACK;
  ILI9341_Draw_ALLCircle(Lcd_Dev.width-250, 20, 6);
  ILI9341_Draw_ALLCircle(Lcd_Dev.width-250, 60, 7);
  ILI9341_Draw_ALLCircle(Lcd_Dev.width-250, 100, 8);
  ILI9341_Draw_ALLCircle(Lcd_Dev.width-250, 140, 9);
  ILI9341_Draw_ALLCircle(Lcd_Dev.width-250, 180, 10);
  ILI9341_Draw_ALLCircle(Lcd_Dev.width-250, 220, 11);
  POINT_COLOR = BLACK;
  POINT_SIZE = 1;
}

/** 
 * @brief  XPT2046 draw init.
 */
void XPT2046_Draw(void)
{
  uint8_t key;
  uint8_t key2;
  uint16_t temp;
  while (1)
  {
    key = Key_Scan(KEY1_PORT, KEY1_PIN);
    key2 = Key_Scan(KEY2_PORT, KEY2_PIN);
    xpt_dev.scan(0);
    if (xpt_dev.sta & TP_PRES_DOWN)
    {
      if ((xpt_dev.x[0] < Lcd_Dev.width) && (xpt_dev.y[0] < Lcd_Dev.height))
      {
//        if (((xpt_dev.x[0] > (Lcd_Dev.width-50)) && (xpt_dev.y[0] < 24)))
//        {
//          XPT2046_Draw_Dralog();
//        }
        if ((xpt_dev.x[0] < Lcd_Dev.width-270) && (xpt_dev.y[0] < 40) && 
        xpt_dev.x[0] > Lcd_Dev.width-320 && xpt_dev.y[0] > 0)
        {
          while ((xpt_dev.x[0] < Lcd_Dev.width-270) && (xpt_dev.y[0] < 40) && 
          xpt_dev.x[0] > Lcd_Dev.width-320 && xpt_dev.y[0] > 0)
          {
            xpt_dev.scan(0);
            POINT_COLOR = WHITE;
            ILI9341_Draw_Rec(Lcd_Dev.width-320, 0, 50, 40);
          }
          POINT_COLOR = BLACK;
          ILI9341_Draw_Rec(Lcd_Dev.width-320, 0, 50, 40);
        }
        else if ((xpt_dev.x[0] < Lcd_Dev.width-270) && (xpt_dev.y[0] < 80) && 
        xpt_dev.x[0] > Lcd_Dev.width-320 && xpt_dev.y[0] > 40)
        {
          while ((xpt_dev.x[0] < Lcd_Dev.width-270) && (xpt_dev.y[0] < 80) && 
          xpt_dev.x[0] > Lcd_Dev.width-320 && xpt_dev.y[0] > 40)
          {
            xpt_dev.scan(0);
            POINT_COLOR = WHITE;
            ILI9341_Draw_Rec(Lcd_Dev.width-320, 40, 50, 40);
          }
          POINT_COLOR = YELLOW;
          ILI9341_Draw_Rec(Lcd_Dev.width-320, 40, 50, 40);
        }
        else if ((xpt_dev.x[0] < Lcd_Dev.width-270) && (xpt_dev.y[0] < 120) && 
        xpt_dev.x[0] > Lcd_Dev.width-320 && xpt_dev.y[0] > 80)
        {
          while ((xpt_dev.x[0] < Lcd_Dev.width-270) && (xpt_dev.y[0] < 120) && 
          xpt_dev.x[0] > Lcd_Dev.width-320 && xpt_dev.y[0] > 80)
          {
            xpt_dev.scan(0);
            POINT_COLOR = WHITE;
            ILI9341_Draw_Rec(Lcd_Dev.width-320, 80, 50, 40);
          }
          POINT_COLOR = BLUE;
          ILI9341_Draw_Rec(Lcd_Dev.width-320, 80, 50, 40);
        }
        else if ((xpt_dev.x[0] < Lcd_Dev.width-270) && (xpt_dev.y[0] < 160) && 
        xpt_dev.x[0] > Lcd_Dev.width-320 && xpt_dev.y[0] > 120)
        {
          while ((xpt_dev.x[0] < Lcd_Dev.width-270) && (xpt_dev.y[0] < 160) && 
          xpt_dev.x[0] > Lcd_Dev.width-320 && xpt_dev.y[0] > 120)
          {
            xpt_dev.scan(0);
            POINT_COLOR = WHITE;
            ILI9341_Draw_Rec(Lcd_Dev.width-320, 120, 50, 40);
          } 
          POINT_COLOR = GREEN;
          ILI9341_Draw_Rec(Lcd_Dev.width-320, 120, 50, 40);
        }
        else if ((xpt_dev.x[0] < Lcd_Dev.width-270) && (xpt_dev.y[0] < 200) && 
        xpt_dev.x[0] > Lcd_Dev.width-320 && xpt_dev.y[0] > 160)
        {
          while ((xpt_dev.x[0] < Lcd_Dev.width-270) && (xpt_dev.y[0] < 200) && 
          xpt_dev.x[0] > Lcd_Dev.width-320 && xpt_dev.y[0] > 160)
          {
            xpt_dev.scan(0);
            POINT_COLOR = WHITE;
            ILI9341_Draw_Rec(Lcd_Dev.width-320, 160, 50, 40);
          }
          POINT_COLOR = WHITE;
        }
        else if ((xpt_dev.x[0] < Lcd_Dev.width-270) && (xpt_dev.y[0] < 240) && 
        xpt_dev.x[0] > Lcd_Dev.width-320 && xpt_dev.y[0] > 200)
        {
          while ((xpt_dev.x[0] < Lcd_Dev.width-270) && (xpt_dev.y[0] < 240) && 
          xpt_dev.x[0] > Lcd_Dev.width-320 && xpt_dev.y[0] > 200)
          {
            xpt_dev.scan(0);
            POINT_COLOR = WHITE;
            ILI9341_Draw_Rec(Lcd_Dev.width-320, 200, 50, 40);
          }
          POINT_COLOR = GRAY;
          ILI9341_Draw_Rec(Lcd_Dev.width-320, 200, 50, 40);
        }
        else if ((xpt_dev.x[0] < Lcd_Dev.width-220) && xpt_dev.x[0] > Lcd_Dev.width-280
        && (xpt_dev.y[0] < 40) && (xpt_dev.y[0] > 0))
        {
          while ((xpt_dev.x[0] < Lcd_Dev.width-220) && xpt_dev.x[0] > Lcd_Dev.width-280
          && (xpt_dev.y[0] < 40) && (xpt_dev.y[0] > 0))
          {
            xpt_dev.scan(0);
            temp = POINT_COLOR;
            POINT_COLOR = WHITE;
            ILI9341_Draw_Rec(Lcd_Dev.width-269, 0, Lcd_Dev.width-280, 39);
            POINT_COLOR = BLACK;
            ILI9341_Draw_ALLCircle(Lcd_Dev.width-250, 20, 6);
            POINT_COLOR = temp;
            POINT_SIZE = 1;
          }
        }
        else if ((xpt_dev.x[0] < Lcd_Dev.width-220) && xpt_dev.x[0] > Lcd_Dev.width-280
        && (xpt_dev.y[0] < 80) && (xpt_dev.y[0] > 40))
        {
          while ((xpt_dev.x[0] < Lcd_Dev.width-220) && xpt_dev.x[0] > Lcd_Dev.width-280
          && (xpt_dev.y[0] < 80) && (xpt_dev.y[0] > 40))
          {
            xpt_dev.scan(0);
            temp = POINT_COLOR;
            POINT_COLOR = WHITE;
            ILI9341_Draw_Rec(Lcd_Dev.width-269, 40, Lcd_Dev.width-280, 39);
            POINT_COLOR = BLACK;
            ILI9341_Draw_ALLCircle(Lcd_Dev.width-250, 60, 7);
            POINT_COLOR = temp;
            POINT_SIZE = 2;
          }
        }
        else if ((xpt_dev.x[0] < Lcd_Dev.width-220) && xpt_dev.x[0] > Lcd_Dev.width-280
        && (xpt_dev.y[0] < 120) && (xpt_dev.y[0] > 80))
        {
          while ((xpt_dev.x[0] < Lcd_Dev.width-220) && xpt_dev.x[0] > Lcd_Dev.width-280
          && (xpt_dev.y[0] < 120) && (xpt_dev.y[0] > 80))
          {
            xpt_dev.scan(0);
            temp = POINT_COLOR;
            POINT_COLOR = WHITE;
            ILI9341_Draw_Rec(Lcd_Dev.width-269, 80, Lcd_Dev.width-280, 39);
            POINT_COLOR = BLACK;
            ILI9341_Draw_ALLCircle(Lcd_Dev.width-250, 100, 8);
            POINT_COLOR = temp;
            POINT_SIZE = 3;
          }
        }
        else if ((xpt_dev.x[0] < Lcd_Dev.width-220) && xpt_dev.x[0] > Lcd_Dev.width-280
        && (xpt_dev.y[0] < 160) && (xpt_dev.y[0] > 120))
        {
          while ((xpt_dev.x[0] < Lcd_Dev.width-220) && xpt_dev.x[0] > Lcd_Dev.width-280
          && (xpt_dev.y[0] < 160) && (xpt_dev.y[0] > 120))
          {
            xpt_dev.scan(0);
            temp = POINT_COLOR;
            POINT_COLOR = WHITE;
            ILI9341_Draw_Rec(Lcd_Dev.width-269, 120, Lcd_Dev.width-280, 39);
            POINT_COLOR = BLACK;
            ILI9341_Draw_ALLCircle(Lcd_Dev.width-250, 140, 9);
            POINT_COLOR = temp;
            POINT_SIZE = 4;
          }
        }
        else if ((xpt_dev.x[0] < Lcd_Dev.width-220) && xpt_dev.x[0] > Lcd_Dev.width-280
        && (xpt_dev.y[0] < 200) && (xpt_dev.y[0] > 160))
        {
          while ((xpt_dev.x[0] < Lcd_Dev.width-220) && xpt_dev.x[0] > Lcd_Dev.width-280
          && (xpt_dev.y[0] < 200) && (xpt_dev.y[0] > 160))
          {
            xpt_dev.scan(0);
            temp = POINT_COLOR;
            POINT_COLOR = WHITE;
            ILI9341_Draw_Rec(Lcd_Dev.width-269, 160, Lcd_Dev.width-280, 39);
            POINT_COLOR = BLACK;
            ILI9341_Draw_ALLCircle(Lcd_Dev.width-250, 180, 10);
            POINT_COLOR = temp;
            POINT_SIZE = 5;
          }
        }
        else if ((xpt_dev.x[0] < Lcd_Dev.width-220) && xpt_dev.x[0] > Lcd_Dev.width-280
        && (xpt_dev.y[0] < 240) && (xpt_dev.y[0] > 200))
        {
          while ((xpt_dev.x[0] < Lcd_Dev.width-220) && xpt_dev.x[0] > Lcd_Dev.width-280
          && (xpt_dev.y[0] < 240) && (xpt_dev.y[0] > 200))
          {
            xpt_dev.scan(0);
            temp = POINT_COLOR;
            POINT_COLOR = WHITE;
            ILI9341_Draw_Rec(Lcd_Dev.width-269, 200, Lcd_Dev.width-280, 39);
            POINT_COLOR = BLACK;
            ILI9341_Draw_ALLCircle(Lcd_Dev.width-250, 220, 11);
            POINT_COLOR = temp;
            POINT_SIZE = 6;
          }
        }
        else
        {
//            ILI9341_Display_Whole_Color();
          XPT2046_Draw_Big_Point(xpt_dev.x[0], xpt_dev.y[0], POINT_COLOR);
        }
      }
    }
    else
    {
//      XPT2046_DelayUs(1);
      LED1_ON;
      LED2_OFF;
    }
    if (key2 == KEY_ON)
    {
      POINT_COLOR = BLACK;
      xpt_dev.adjust();
      XPT2046_Save_ADjdata();
      XPT2046_Draw_Dralog();
    }
    if (key == KEY_ON)
    {
      XPT2046_Draw_Dralog();
    }
  }
}

/** 
 * @brief  Check XPT2046 state.
 */
void XPT2046_Touch(void)
{
  if (!XPT2046_Get_ADjdata())
  {
    XPT2046_ADjust();
//    XPT2046_Save_ADjdata();
    XPT2046_Get_ADjdata();
  }
  if (XPT2046_Get_ADjdata())
  {
    POINT_COLOR = BLUE;
    ILI9341_ShowString(110, 110, Lcd_Dev.width, Lcd_Dev.height, 24, (uint8_t *)"Android");
    HAL_Delay(50);
    XPT2046_Draw_Dralog();
    XPT2046_Draw();
  }
  else
  {
    ILI9341_ShowString(35+20, 110-70, Lcd_Dev.width, Lcd_Dev.height, 24, (uint8_t *)"ADJ failed");
  }
/*  if (XPT2046_Touch_Detect() == XPT2046_PRESSED)
  {
    LED1_OFF;
    LED2_ON;
    printf("Touch down !\n");
    xpt_dev.x[0] = XPT2046_ReceiveData(0xd0);
    xpt_dev.y[0] = XPT2046_ReceiveData(0x90);
    printf("x = %x, y = %x\n", xpt_dev.x[0], xpt_dev.y[0]);
    HAL_Delay(10);
  }*/
}