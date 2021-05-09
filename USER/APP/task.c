/**
	******************************************************************************
	* @file    task.c
	* @author  404Zen
	* @version V1.0
	* @date    2021-03-07
	* @brief   UCOSIII Task Headfile
	******************************************************************************
	* @attention
	*
	******************************************************************************
	*/
// #include "includes.h"
#include "task.h"
#include "stdlib.h"


//开始任务函数
void	StartTask(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();

	CPU_Init();

#if	OS_CFG_STAT_TASK_EN > 0u			//如果使能了统计任务
	OSStatTaskCPUUsageInit(&err);
#endif

#ifdef	CPU_CFG_INT_DIS_MEAS_EN			//如果使能了测量中断关闭时间
	CPU_IntDisMeasMaxCurReset();
#endif

#if OS_CFG_SCHED_ROUND_ROBIN_EN			//当使用时间片轮转的时候
	OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
#endif

#if OS_CFG_APP_HOOKS_EN					//使用钩子函数
	App_OS_SetAllHooks();
#endif

	

	OS_CRITICAL_ENTER();								//进入临界区


#if	1	//定时器1
	OSTmrCreate(	(OS_TMR*	)&Timer1,				//定时器1
					(CPU_CHAR*	)"Timer1",
					(OS_TICK	)0,						//初始化定时器时的延迟值
					(OS_TICK	)100,					//定时器周期 Ticks数 每个Tick 1ms
					(OS_OPT		)OS_OPT_TMR_PERIODIC,	//模式
					(OS_TMR_CALLBACK_PTR)Timer1Callback,//回调函数
					(void*		)0,						//回调函数参数
					(OS_ERR*	)&err);					//错误码
#endif


#if 1	//Blink任务
	OSTaskCreate(	(OS_TCB		*)&BlinkTaskTCB,	
					(CPU_CHAR	*)"Blink Task",	
					(OS_TASK_PTR )BlinkTask,
					(void		*)0,
					(OS_PRIO	 )BLINK_PRIO,
					(CPU_STK	*)&BlinkStack[0],
					(CPU_STK_SIZE)BLINK_STACK_SIZE/10,
					(CPU_STK_SIZE)BLINK_STACK_SIZE,
					(OS_MSG_QTY  )0,
					(OS_TICK	 )0,
					(void		*)0,
					(OS_OPT		 )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
					(OS_ERR		*)&err	);
#endif

	//按键扫描任务
	OSTaskCreate(	(OS_TCB		*)&KeyTaskTCB,	
					(CPU_CHAR	*)"Key Task",	
					(OS_TASK_PTR )KeyTask,
					(void		*)0,
					(OS_PRIO	 )KEY_PRIO,
					(CPU_STK	*)&KeyStack[0],
					(CPU_STK_SIZE)KEY_STACK_SIZE/10,
					(CPU_STK_SIZE)KEY_STACK_SIZE,
					(OS_MSG_QTY  )0,
					(OS_TICK	 )0,
					(void		*)0,
					(OS_OPT		 )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
					(OS_ERR		*)&err	);
	

	OSTmrStart(&Timer1, &err);

	OS_CRITICAL_EXIT();							//退出临界区


	// OS_TaskSuspend( (OS_TCB*)&StartTaskTCB, &err);	//挂起开始任务
	OSTaskDel((OS_TCB*)0, &err);				//删除开始任务
}


void	BlinkTask(void *p_arg)
{
	OS_ERR err;

	while (1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &err);
	}	
}

void	KeyTask(void *p_arg)
{
	OS_ERR	err;
	uint8_t KeyValue = 0;
	static uint8_t Counter = 0;

	while (1)
	{
		KeyValue = KeyScan(0);


		if(KEY_VALUE_0 == KeyValue)
		{
			Counter ++;
			printf("Key0 Press!\r\n");
			// GRBTransToDataBuf()
			WS2812SendFrame();
			
			//等待DMA传送完成
			while (1)
			{
				//DMA CH3 传输完成
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET)
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);
					break;
				}
			}
		}


		//调度其他任务
		OSTimeDlyHMSM(0, 0, 0, 10, OS_OPT_TIME_HMSM_STRICT, &err);
	}	
}


void StartTaskCreat(void)
{
	OS_ERR err;

	//创建开始任务
	OSTaskCreate(	(OS_TCB		*)&StartTaskTCB,			//任务控制块
					(CPU_CHAR	*)"Start Task",				//任务名字
					(OS_TASK_PTR )StartTask,				//任务函数
					(void		*)0,						//传递给任务函数的参数
					(OS_PRIO	 )START_PRIO,				//任务优先级
					(CPU_STK	*)&StartStack[0],			//任务堆栈的基地址
					(CPU_STK_SIZE)START_STACK_SIZE/10,		//堆栈深度限位
					(CPU_STK_SIZE)START_STACK_SIZE,			//堆栈大小
					(OS_MSG_QTY  )0,
					(OS_TICK	 )0,
					(void		*)0,						//用户补充的存储区
					(OS_OPT		 )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
					(OS_ERR		*)&err						//任务错误时的返回值
					);
}


//定时器1回调函数
void	Timer1Callback(void *p_tmr, void *p_arg)
{
	// printf("Timer1 Callback!\r\n");


	// WS2812SendFrame();
			
	// //等待DMA传送完成
	// while (1)
	// {
	// 	//DMA CH3 传输完成
	// 	if(DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET)
	// 	{
	// 		DMA_ClearFlag(DMA1_FLAG_TC3);
	// 		break;
	// 	}
	// }

	// WS2812Reset();
	// GradinetHSL();
}



/**********************************END OF FILE**********************************/
