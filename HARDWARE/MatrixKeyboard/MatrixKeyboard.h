/**
	******************************************************************************
	* @file    MatrixKeyboard.h
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
 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__MATRIXKEYBOARD_H__
#define	__MATRIXKEYBOARD_H__

/* Includes ------------------------------------------------------------------*/
#include    "sys.h"
#include	"delay.h"
#include    "stm32f10x_gpio.h"
#include    "stm32f10x_rcc.h"


/* Defines ------------------------------------------------------------------*/
#define	KB_COL1				PBout(0)		//列
#define	KB_COL2				PBout(1)
#define	KB_COL3				PBout(2)
#define	KB_COL4				PBout(3)
#define	KB_ROW1				PBout(4)		//行
#define	KB_ROW2				PBout(5)
#define	KB_ROW3				PBout(6)
#define	KB_ROW4				PBout(7)


/* Functions -----------------------------------------------------------------*/
void MatrixKeyboardInit(void);			//矩阵键盘IO初始化
uint8_t KBColScanInit(void);			//矩阵键盘	列扫描初始化 扫描哪一列有按键按下
uint8_t KBRowScanInit(void);			//矩阵键盘	行扫描初始化 扫面哪一行有按键按下
void KeyBoardTest(void);

 #endif
/**********************************END OF FILE**********************************/
