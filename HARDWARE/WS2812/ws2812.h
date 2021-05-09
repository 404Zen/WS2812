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

/* 平台相关定------------------------------------------------------------------*/
#define WS2812_DMA(x)	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, (x))		//WS2812 DMA发送使能/失能
// #define	WS2812_DISABLE_DMA	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE)	//WS2812 DMA发送失能


/* Defines -------------------------------------------------------------------*/
#define WS2812_ROW			8							//行数
#define	WS2812_COL			32							//列数
#define	WS2812_POINTS		(WS2812_ROW*WS2812_COL)		//屏幕的像素点数 如果帧率要 > 30fps Max Ponit is 1024 

#define	WS2812_BUF_SIZE		(WS2812_POINTS*24)
static	uint8_t	WS2812DataBuf[WS2812_BUF_SIZE];			//SPI DMA缓存，每Byte数据相当于WS2812 1bit数据，所以要传送 GRB 24bit,一个像素点需要24Byte缓存 加上复位的1Byte数据
static	uint8_t WS2812GRBBuf[WS2812_POINTS*3];			//WS2812 GRB数据缓存

#define WS2812_BRIGHTNESS_CONTROL			1			//亮度控制使能
#define	BRIGHTNESS_CONTROL					10			//0~100  亮度控制  0到100%

#if WS2812_BRIGHTNESS_CONTROL == 1
	#define	WS2812_BRIGHT					BRIGHTNESS_CONTROL
#else
	#define	WS2812_BRIGHT					100
#endif

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
void WS2812Init(void);					//WS2812初始化
uint8_t	WS2812Reset(void);				//发送复位信号 刷新一帧显示
void WS2812SendFrame(void);				//WS2812发送一帧数据(DMA发送一帧数据)
void WS2812ClearGRBBuf(void);			//WS2812清除GRB缓存
void WS2812ClearDisplay(void);			//清屏
void WS2812RefreshDisplay(void);		//刷新显示
void GRBDataInit(void);
uint8_t GRBTransToDataBuf(uint8_t *GRBBuf, uint16_t Points, uint8_t *DataBuf);
void GradinetHSL(void);
void RGB2HSL(uint32_t RGBColor, float *H, float *S, float *L);
uint32_t HSL2RGB(float *H, float *S, float *L);
float Hue2RGB(float Var1, float Var2, float VarHue);
uint8_t WS2812DrawLine(uint8_t StartX, uint8_t EndX, uint8_t StartY, uint8_t EndY, uint32_t Color);
uint8_t	WS2812ShowChar(uint8_t X, uint8_t Y, uint8_t Char, uint32_t Color);
uint8_t WS2812ShowString(uint8_t X, uint8_t Y, uint8_t *Str, uint32_t Color);
#endif
/**********************************END OF FILE**********************************/
