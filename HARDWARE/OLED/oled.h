/**
	******************************************************************************
	* @file    oled.h
	* @author  Zen
	* @version V1.0
	* @date    2020-06-14
	* @brief   OLED Driver Headfile
	******************************************************************************
	* @attention
	*
	******************************************************************************
	*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OLED_H
#define __OLED_H


/* Includes ------------------------------------------------------------------*/
// #include "../CMSIS/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
#include "iic.h"

#define	USE_SOFT_IIC		0

#define OLED_HARD_IIC		I2C2
#define	OLED_IIC_ADDR		0x78
#define	OLED_CMD_REG		0x00
#define	OLED_DATA_REG		0x40


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
#define	OLED_SCK_PIN		GPIO_Pin_10
#define	OLED_SCK_PORT		GPIOB
#define	OLED_SCK_PERIPH		RCC_APB2Periph_GPIOB

#define	OLED_SDA_PIN		GPIO_Pin_11
#define	OLED_SDA_PORT		GPIOB
#define	OLED_SDA_PERIPH		RCC_APB2Periph_GPIOB

#define	OLED_SCLK_Set()		GPIO_SetBits(OLED_SCK_PORT,OLED_SCK_PIN)			//（SCL）输出高
#define	OLED_SCLK_Clr()		GPIO_ResetBits(OLED_SCK_PORT,OLED_SCK_PIN)			//（SCL）输出低
#define	OLED_SDIN_Set()		GPIO_SetBits(OLED_SDA_PORT,OLED_SDA_PIN)			//（SDA）输出高
#define	OLED_SDIN_Clr()		GPIO_ResetBits(OLED_SDA_PORT,OLED_SDA_PIN)			//（SDA）输出高
#define	OLED_READ_SDIN()	GPIO_ReadInputDataBit(OLED_SDA_PORT, OLED_SDA_PIN)	//读取（SDA）电平

#define OLED_CMD  0						//写命令
#define OLED_DATA 1						//写数据

#define SIZE 		16		//显示字符的大小
#define Max_Column	128		//最大列数
#define Max_Row		64		//最大行数
#define X_WIDTH 	128		//X轴的宽度
#define Y_WIDTH 	64	    //Y轴的宽度					  				   
#define	IIC_ACK		0		//应答
#define	IIC_NO_ACK	1		//不应答

/* Exported functions ------------------------------------------------------- */
void OLED_WR_Byte(unsigned char dat,unsigned char cmd);		//OLED写字节函数
void OLED_Display_On(void);									//开显示函数
void OLED_Display_Off(void);								//关显示函数
void OLED_Init(void);										//OLED初始化函数
void OLED_Clear(void);  									//清屏函数
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr); 										//显示字符函数
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size2); 	//在指定的位置，显示一个指定数的长度大小函数
void OLED_ShowString(unsigned char x,unsigned char y, unsigned char *p);									//在指定位置开始显示字符串函数
void OLED_Set_Pos(unsigned char x, unsigned char y);														//画点函数
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no); 									//声明在指定位置显示汉字函数
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]); //显示图片函数
void OLED_Scroll(void);	//滚动函数

#endif
/**********************************END OF FILE**********************************/
