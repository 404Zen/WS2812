/**
	******************************************************************************
	* @file    key.c
	* @author  404Zen
	* @version V0.0.1
	* @date    2021.03.13
	* @brief   Key Driver
	******************************************************************************
	* @attention
	*
	* None.
	* 
	******************************************************************************
	*/

 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_H__
#define __KEY_H__

/* Includes ------------------------------------------------------------------*/
#include    "sys.h"
#include	"delay.h"
#include    "stm32f10x_gpio.h"
#include    "stm32f10x_rcc.h"


/* Defines ------------------------------------------------------------------*/
#define	BOARD_FK103M5
// #define BOARD_BLUEPILL

#ifdef	BOARD_FK103M5

#define	KEY_VALUE_0			(0x30)

#define	KEY0_PERIPH			RCC_APB2Periph_GPIOA
#define KEY0_PORT			GPIOA
#define	KEY0_PIN			GPIO_Pin_15
#define KEY0 				GPIO_ReadInputDataBit(KEY0_PORT, KEY0_PIN)

#define	KEY_PERIPH			(KEY0_PERIPH)

#endif

/* Functions -----------------------------------------------------------------*/
void KeyInit(void);						//Key Initialization
uint8_t KeyScan(uint8_t ContinueMode);	//按键扫描函数

uint8_t ReadKEY0Level(void);			//读取按键电平


#endif		
/**********************************END OF FILE*********************************/
