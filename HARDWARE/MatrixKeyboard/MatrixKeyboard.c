/**
	******************************************************************************
	* @file    MatrixKeyboard.c
	* @author  404Zen
	* @version V0.0.1
	* @date    2021.03.30
	* @brief   矩阵键盘驱动
	******************************************************************************
	* @attention
	*
	* None.
	* 
	******************************************************************************
	*/

/* Includes ------------------------------------------------------------------*/
#include	"MatrixKeyboard.h"
#include	"stdio.h"
#include	"stdlib.h"


/**
	* @brief  矩阵键盘IO初始化
	* @param  None.
	* @note   None. 
	* @retval None
	*/
void MatrixKeyboardInit(void)
{
	// GPIO_InitTypeDef	GPIO_InitStructure;

	// GPIOB->CRL = 0x77777777;
	// GPIOB->ODR = 0x000000FF;
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_Init(GPIOB, &GPIO_InitStructure);
}



/**
	* @brief  矩阵键盘	列扫描初始化 扫描哪一列有按键按下
	* @param  None.
	* @note   None. 
	* @retval None
	*/
uint8_t KBColScanInit(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	KB_ROW1 = 1; KB_ROW2 = 1; KB_ROW3 = 1; KB_ROW4 = 1;		//行输出高电平

	return (uint8_t)(GPIOB->IDR & 0x0000000F);
}

/**
	* @brief  矩阵键盘	行扫描初始化 扫描哪一行有按键按下
	* @param  None.
	* @note   None. 
	* @retval None
	*/
uint8_t KBRowScanInit(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// KB_ROW1 = 1; KB_ROW2 = 1; KB_ROW3 = 1; KB_ROW4 = 1;		//行输出高电平
	KB_COL1 = 1; KB_COL2 = 1; KB_COL3 = 1; KB_COL4;

	return (uint8_t)(GPIOB->IDR & 0x000000F0);
}



void KeyBoardTest(void)
{
	// uint8_t *pbuf;
	uint8_t KBValue = 0;
//	static uint8_t Status = 0;

	// pbuf = malloc(20);

	while (1)
	{
		KBValue = KBColScanInit();
		delay_ms(50);
		KBValue |= KBRowScanInit();

		// sprintf((char*)pbuf, "Key Value is:%02X", KBValue);
		// OLED_ShowString(0, 2, pbuf);
		if(KBValue != 0)
		{
			printf("Key Value is:%02X\r\n", KBValue);
			KBValue = 0;
		}
	}

	
	// while (1)
	// {
	// 	KBValue = GPIOB->IDR;

	// 	if(KBValue != 0)
	// 	{
	// 		sprintf(pbuf, "Key Value is:%02X", KBValue);
	// 		OLED_ShowString(0, 2, pbuf);
	// 	}
	// 	delay_ms(500);
	// }
}

/**********************************END OF FILE*********************************/
