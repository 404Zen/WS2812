/**
	******************************************************************************
	* @file    task.h
	* @author  404Zen
	* @version V1.0
	* @date    2021-03-07
	* @brief   UCOSIII Task Headfile
	******************************************************************************
	* @attention
	*
	******************************************************************************
	*/



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TASK_H
#define __TASK_H

/* Includes ------------------------------------------------------------------*/
#include	"string.h"
// #include	"includes.h"
#include	"os.h"
#include	"os_app_hooks.h"

#include	"malloc.h"

//Peripheral
#include	"usart.h"
#include	"led.h"
#include	"oled.h"
#include	"key.h"
#include	"ws2812.h"


/* Task Config ---------------------------------------------------------------*/
//START任务
#define START_PRIO						3					//优先级
#define START_STACK_SIZE				512					//堆栈大小
static	OS_TCB	StartTaskTCB;								//任务控制块
static	CPU_STK	StartStack[START_STACK_SIZE];				//任务堆栈，堆栈大小为SIZE*4字节
static	void	StartTask(void *p_arg);						//任务函数

//BLINK任务
#define BLINK_PRIO						(OS_CFG_PRIO_MAX-3)	//优先级
#define BLINK_STACK_SIZE				128					//堆栈大小
static	OS_TCB	BlinkTaskTCB;								//任务控制块
static	CPU_STK	BlinkStack[BLINK_STACK_SIZE];				//任务堆栈
static	void	BlinkTask(void *p_arg);						//任务函数

//按键操作函数
#define	KEY_PRIO						4
#define	KEY_STACK_SIZE					128
static	OS_TCB	KeyTaskTCB;
static	CPU_STK	KeyStack[KEY_STACK_SIZE];
static	void	KeyTask(void *p_arg);


/* Message queue ------------------------------------------------------------ */
				

/* Timer -------------------------------------------------------------------- */
static	uint8_t	Timer1Status;					//定时器工作状态
static	OS_TMR	Timer1;							//定时器1
static	void	Timer1Callback(void *p_tmr, void *p_arg);	//定时器1回调函数


/* Exported functions ------------------------------------------------------- */
void StartTaskCreat(void);									//开始任务创建函数

#endif
/**********************************END OF FILE**********************************/
