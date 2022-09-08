#include "bsp_led.h"

/** 
 * @brief  Enable led clk and config led gpio.
 */
void LED_Init(void)
{
  /*  Enable LED clk  */
  LED1_CLK_ENABLE();

  /*  LED gpio init struct  */
  GPIO_InitTypeDef GPIO_InitTypeDef = {0};

  /*  Config LED gpio struct  */
  GPIO_InitTypeDef.Pin = LED1_PIN | LED2_PIN;
  GPIO_InitTypeDef.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitTypeDef.Pull = GPIO_PULLUP;
  GPIO_InitTypeDef.Speed = GPIO_SPEED_FREQ_LOW;

  /*  Init LED gpio  */
  HAL_GPIO_Init(LED1_PORT, &GPIO_InitTypeDef);
}