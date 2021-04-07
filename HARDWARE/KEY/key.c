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
#include "key.h"


/**
	* @brief  KEY GPIO Initial.
	* @param  None.
	* @note   None. 
	* @retval None
	*/
void KeyInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(KEY_PERIPH, ENABLE);				//使能端口时钟

	GPIO_InitStructure.GPIO_Pin = KEY0_PIN;				 	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(KEY0_PORT, &GPIO_InitStructure);				//初始化GPIO
	GPIO_SetBits(KEY0_PORT,KEY0_PIN);						//输出高，关闭LED
}

/**
	* @brief  按键扫描函数.
	* @param  ContinueMode：0：不支持连按， 1：支持连按.
	* @note   None. 
	* @retval None
	*/
uint8_t KeyScan(uint8_t ContinueMode)
{
	static uint8_t	KeyUpFlag = 1;		//按键松开标志
	
	if(ContinueMode)
		KeyUpFlag = 1;					//连按模式，每次进入函数都将按键松开标志置位
	
	if(KeyUpFlag && (KEY0 == 0))
	{
		delay_ms(10);					//软件消抖
		KeyUpFlag = 0;

		if(KEY0 == 0)
			return	KEY_VALUE_0;
		else
			return	0;
	}
	else if (KEY0 == 1)
	{
		KeyUpFlag = 1;		
	}
	
	return 0;
}


/**
	* @brief  Read KEY0 Level
	* @param  None.
	* @note   None. 
	* @retval Key Level,High(1) or Low(0)
	*/
uint8_t ReadKEY0Level(void)
{
	return	GPIO_ReadInputDataBit(KEY0_PORT, KEY0_PIN);
}


/**********************************END OF FILE*********************************/

