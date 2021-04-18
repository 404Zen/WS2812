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

#define	WS2812_T1						0xFC	//6bit H(750ns) 2bit L(250ns)
#define	WS2812_T0						0xC0	//2bit H(250ns)	6bit L(750ns)

//RGB Color define	(32bit 低24位有效)
#define RGB_BLACK				0x00000000		//黑色 	H:0 S:0	L:0
#define	RGB_WHITE				0x00FFFFFF		//白色	H:0	S:0	L:1
#define	RGB_RED					0x00FF0000
#define RGB_LIME				0x0000FF00
#define RGB_BLUE				0x000000FF
#define	RGB_YELLOW				0x00FFFF00
#define	RGB_CYAN				0x0000FFFF
#define	RGB_MAGENTA				0x00FF00FF
#define	RGB_SILVER				0x00BFBFBF
#define	RGB_GRAY				0x00808080
#define	RGB_MAROON				0x00800000
#define	RGB_OLIVE				0x00808000
#define	RGB_GREEN				0x00008000
#define	RGB_PURPLE				0x00800080
#define	RGB_TEAL				0x00008080
#define	RGB_NAVY				0x00000080


/* Functions -----------------------------------------------------------------*/
void SPIInitForWS2812(void);
uint8_t	WS2812Reset(void);				//发送复位信号 刷新一帧显示
void WS2812SendFrame(void);				//WS2812发送一帧数据(DMA发送一帧数据)
void GRBDataInit(void);
uint8_t GRBTransToDataBuf(uint8_t *GRBBuf, uint16_t Points, uint8_t *DataBuf);
void GradinetHSL(void);
void RGB2HSL(uint32_t RGBColor, float *H, float *S, float *L);
uint32_t HSL2RGB(float *H, float *S, float *L);
float Hue2RGB(float Var1, float Var2, float VarHue);
#endif
/**********************************END OF FILE**********************************/
