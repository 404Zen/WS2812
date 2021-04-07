/**
  ******************************************************************************
  * @file    led.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H__
#define __LED_H__

/* Includes ------------------------------------------------------------------*/
#include    "sys.h"
#include    "stm32f10x_gpio.h"
#include    "stm32f10x_rcc.h"


#define	BOARD_FK103M5
// #define BOARD_BLUEPILL

#ifdef	BOARD_FK103M5

#define LED0 				PCout(13)	
#define	LED0_PERIPH			RCC_APB2Periph_GPIOC
#define LED0_PORT			GPIOC
#define	LED0_PIN			GPIO_Pin_13

#define	LED_PERIPH			(LED0_PERIPH)

#endif

#ifdef	BOARD_BLUEPILL

#define LED_PERIPH			(LED0_PERIPH)

#define LED0 				PCout(13)	
#define	LED0_PERIPH			RCC_APB2Periph_GPIOC
#define LED0_PORT			GPIOC
#define	LED0_PIN			GPIO_Pin_13

#endif

/* Functions ******************************************************************/
void LEDInit(void);						//LED GPIO Initialization

		 					
#endif		
/**********************************END OF FILE*********************************/
