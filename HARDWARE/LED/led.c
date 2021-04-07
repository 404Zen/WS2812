/**
  ******************************************************************************
  * @file    led.c
  * @author  404Zen
  * @version V0.0.1
  * @date    2021.03.13
  * @brief   LED Driver
  ******************************************************************************
  * @attention
  *
  * None.
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "led.h"


/**
  * @brief  LED GPIO Initial.
  * @param  None.
  * @note   None. 
  * @retval None
  */
void LEDInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(LED_PERIPH, ENABLE);				//使能端口时钟

	GPIO_InitStructure.GPIO_Pin = LED0_PIN;				 	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(LED0_PORT, &GPIO_InitStructure);				//初始化GPIO
	GPIO_SetBits(LED0_PORT,LED0_PIN);						//输出高，关闭LED
}

/**********************************END OF FILE*********************************/
