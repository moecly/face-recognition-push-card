#include "bsp_key.h"

/** 
 * @brief  Enable key clk and config key gpio.
 */
void KEY_Init(void)
{
  /*  Enable key clk  */
  KEY1_CLK_ENABLE();
  KEY2_CLK_ENABLE();

  /*  Key gpio init struct  */
  GPIO_InitTypeDef GPIO_InitTypeDef = {0};

  /*  Config LED gpio struct  */
  GPIO_InitTypeDef.Pin = KEY1_PIN | KEY2_PIN;
  GPIO_InitTypeDef.Mode = GPIO_MODE_INPUT;
  GPIO_InitTypeDef.Pull = GPIO_PULLUP;
  GPIO_InitTypeDef.Speed = GPIO_SPEED_FREQ_LOW;

  /*  Init LED gpio  */
  HAL_GPIO_Init(KEY1_PORT, &GPIO_InitTypeDef);
}

/** 
 * @brief  Check key state.
 */
uint16_t Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN_x)
{
  if (HAL_GPIO_ReadPin(GPIOx, GPIO_PIN_x) == GPIO_PIN_RESET)
  {
    while (HAL_GPIO_ReadPin(GPIOx, GPIO_PIN_x) == GPIO_PIN_RESET);
    HAL_Delay(10);
    return KEY_ON;
  }
  else
  {
    return KEY_OFF;
  }
}