/**
	******************************************************************************
	* @file    ws2812.h
	* @author  404Zen
	* @version V0.0.1
	* @date    2021.03.31
	* @brief   WS2812驱动
	******************************************************************************
	* @attention
	*
	* None.
	* 
	******************************************************************************
	*/
 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__WS2812_H__
#define	__WS2812_H__

/* Includes ------------------------------------------------------------------*/
#include    "stm32f10x_rcc.h"
#include    "stm32f10x_gpio.h"
#include    "stm32f10x_spi.h"
#include	"stm32f10x_dma.h"

/* Defines -------------------------------------------------------------------*/
#define	SPI_FOR_WS2812				SPI1

// #define	WS2812S		1				//T0H:250~550ns,T0L:700~1000ns;T1H:700~1000ns,T1L:250~550ns		Reset: >50us

#define	WS2812_T1						0xFC
#define	WS2812_T0						0xC0

/* Functions -----------------------------------------------------------------*/
void SPIInitForWS2812(void);
uint8_t	WS2812Reset(void);				//发送复位信号 刷新一帧显示
void WS2812SendFrame(void);				//WS2812发送一帧数据(DMA发送一帧数据)
void GRBDataInit(void);
uint8_t GRBTransToDataBuf(uint8_t *GRBBuf, uint16_t Points, uint8_t *DataBuf);

#endif
/**********************************END OF FILE**********************************/
