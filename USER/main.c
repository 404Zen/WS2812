/**
	******************************************************************************
	* @file    main.c
	* @author  404Zen
	* @version V1.0
	* @date    
	* @brief   
	******************************************************************************
	* @attention
	*
	******************************************************************************
	*/
#include	"includes.h"



int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();

	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//中断分组初始化

	//外设初始化
	uart_init(115200);										//串口初始化
	LEDInit();												//LED初始化
	KeyInit();												//按键初始化
	SPIInitForWS2812();

	delay_ms(100);

	WS2812Init();


	// WS2812DrawLine(6, 6, 1, 5, RGB_LIME);		//画竖线
	
	// WS2812DrawLine(0, 21, 6, 6, RGB_LIME);			//画横线

	// WS2812ShowChar(0, 0, '2', RGB_LIME);

	WS2812ShowString(0, 0, "23:45", RGB_WHITE);

	WS2812RefreshDisplay();
	

	printf("USART0 Init Done!\r\n");


	//系统初始化
	OSInit(&err);											//OS初始化
	OS_CRITICAL_ENTER();									//进入临界区

	StartTaskCreat();										//Start任务创建

	OS_CRITICAL_EXIT();										//退出临界区
	OSStart(&err);
	while (1)
	{
		;
	}
}


