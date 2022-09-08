#include "bsp_key.h"

void KEY_GPIO_Init(void)
{
	GPIO_InitTypeDef KEY_GPIO_Init;
	__HAL_RCC_GPIOA_CLK_ENABLE();

	KEY_GPIO_Init.Mode = GPIO_MODE_INPUT;
	KEY_GPIO_Init.Pull = GPIO_PULLUP;
	KEY_GPIO_Init.Pin = GPIO_PIN_8;
	KEY_GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOA, &KEY_GPIO_Init);
}

uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
	{
//		while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET);
//		HAL_Delay(10);
		return KEY_ON;
	}
	else
	{
		return KEY_OFF;
	}
}