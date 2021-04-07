/**
	******************************************************************************
	* @file    iic.c
	* @author  404Zen
	* @version V0.0.1
	* @date    2021.03.25
	* @brief   STM32硬件IIC驱动
	******************************************************************************
	* @attention
	*
	*	IIC1 PB6-SCL1, PB7-SDA.
	*	IIC2 PB10-SCL2,PB11-SDA2
	* 
	******************************************************************************
	*/

/* Includes ------------------------------------------------------------------*/
#include	"iic.h"

#if SYSTEM_SUPPORT_OS
#include "includes.h"							//UCOS
#endif

/**
	* @brief  IIC及对应IO初始化
	* @param  None.
	* @note   None. 
	* @retval None
	*/
void IICInit(I2C_TypeDef* I2Cx)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	// NVIC_InitTypeDef	NVIC_InitStructure;
	I2C_InitTypeDef		I2C_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟

	if(I2C1 == I2Cx)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);//使能IIC1时钟
		//IO口初始化	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;			//复用开漏输出
		// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		// GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	else if(I2C2 == I2Cx)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);//使能IIC2时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		
	}		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;			//复用开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	

#if	0
	//中断配置
	// I2C1_EV_IRQn                = 31,     I2C1 Event Interrupt
 	// I2C1_ER_IRQn                = 32,     I2C1 Error Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//IIC中断优先级设为最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;				//IIC模式 
	I2C_InitStructure.I2C_ClockSpeed = STM32_IIC_SPEED;		//速率<= 400000
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		//SCL时钟线占空比 L:H = 2:1 or 16:9
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;				//IIC应答使能
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;		//7位地址模式
	I2C_InitStructure.I2C_OwnAddress1 = STM32_IIC_ADDR;		//本机IIC地址，不能与总线上其他地址相同
	I2C_Init(I2Cx, &I2C_InitStructure);

	I2C_Cmd(I2Cx, ENABLE);
}


/**
	* @brief  IIC发送一字节数据
	* @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
	* @param  SlaveAddr：从机地址
	* @param  RegAddr:寄存器地址
	* @param  Data：要写入的数据
	* @note   None. 
	* @retval None
	*/
void IICSendByte(I2C_TypeDef* I2Cx,	uint8_t SlaveAddr, uint8_t RegAddr, uint8_t Data)
{
	//发送开始信号
	I2C_GenerateSTART(I2Cx, ENABLE);		
	//等待I2C发送起始信号完成	EV5
	while (!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT))	;

	//发送从机地址,写入状态
	I2C_Send7bitAddress(I2Cx,SlaveAddr, I2C_Direction_Transmitter);
	//等待主机发送期间地址，等待从机应答	EV6
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))	;

	//发送寄存器地址
	I2C_SendData(I2Cx,RegAddr);
	//等待数据发送完成	EV8_2
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))	;

	//发送数据
	I2C_SendData(I2Cx,Data);
	//等待数据发送完成	EV8_2
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))	;

	//发送结束信号
	I2C_GenerateSTOP(I2Cx,ENABLE);	
}

/**
	* @brief  IIC读取一字节数据
	* @param  I2Cx:where x can be 1 or 2 to select the I2C peripheral.
	* @param  SlaveAddr:从机地址
	* @param  RegAddr:寄存器地址
	* @note   None. 
	* @retval 读到的数据
	*/
uint8_t IICReadByte(I2C_TypeDef* I2Cx, uint8_t SlaveAddr, uint8_t RegAddr)
{
	uint8_t Temp = 0;

	//等待IIC空闲
	while (I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY))	;

	//设置IIC设备地址 ↓↓↓
	I2C_GenerateSTART(I2Cx, ENABLE);		//发送开始信号
	//等待I2C发送起始信号完成	EV5
	while (!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT))	;

	//发送从机地址,写入状态
	I2C_Send7bitAddress(I2Cx,SlaveAddr, I2C_Direction_Transmitter);
	//等待主机发送期间地址，等待从机应答	EV6
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))	;

	I2C_Cmd(I2Cx,ENABLE);		//?

	//发送寄存器地址
	I2C_SendData(I2Cx,RegAddr);
	//等待数据发送完成
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))	;
	//地址设置完成 ↑↑↑

	I2C_GenerateSTART(I2Cx,ENABLE);
	//等待事件完成	EV5
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT))	;

	//发送从机地址
	I2C_Send7bitAddress(I2Cx,SlaveAddr, I2C_Direction_Receiver)	;
	//等待事件完成	EV6
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))	;

	//最后一个数据时关闭应答
	I2C_AcknowledgeConfig(I2Cx,DISABLE);
	//最后一个数据时发送停止信号
	I2C_GenerateSTOP(I2Cx,ENABLE);

	Temp = I2C_ReceiveData(I2Cx);

	return Temp;
}

/**
	* @brief  IIC发送多字节数据
	* @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
	* @param  SlaveAddr：从机地址
	* @param  RegAddr:寄存器地址
	* @param  DataPtr：要写入的数据指针
	* @param  Len：要写入的数据长度
	* @note   None. 
	* @retval None
	*/
void	IICSendBuffer(I2C_TypeDef* I2Cx, uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *DataPtr, uint16_t Len)
{
	//发送开始信号
	I2C_GenerateSTART(I2Cx, ENABLE);		
	//等待I2C发送起始信号完成	EV5
	while (!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT))	;

	//发送器件地址
	I2C_Send7bitAddress(I2Cx,SlaveAddr,I2C_Direction_Transmitter);
	//等待时间完成 EV6
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))	;

	//发送寄存器地址
	I2C_SendData(I2Cx,RegAddr);
	//等待数据发送完成	EV8_2
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))	;

	//循环发送数据
	while(Len--)
	{
		//发送数据
		I2C_SendData(I2Cx,*DataPtr);
		DataPtr++;
		//等待数据发送完成	EV8_2
		while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))	;
	}

	//发送停止信号
	I2C_GenerateSTOP(I2Cx,ENABLE);
}

/**
	* @brief  IIC读取多字节数据
	* @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
	* @param  SlaveAddr：从机地址
	* @param  RegAddr:寄存器地址
	* @param  DataPtr：数据指针
	* @param  Len：要读取的数据长度
	* @note   None. 
	* @retval None
	*/
void	IICReadBuffer(I2C_TypeDef* I2Cx, uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *DataPtr, uint16_t Len)
{
	//等待IIC空闲
	while (I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY))	;

	/*** 设置IIC设备地址 ↓↓↓ ***/
	//发送开始信号
	I2C_GenerateSTART(I2Cx, ENABLE);		
	//等待I2C发送起始信号完成	EV5
	while (!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT))	;

	//发送从机地址,写入状态
	I2C_Send7bitAddress(I2Cx,SlaveAddr, I2C_Direction_Transmitter);
	//等待主机发送期间地址，等待从机应答	EV6
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))	;

	I2C_Cmd(I2Cx,ENABLE);		//?

	//发送寄存器地址
	I2C_SendData(I2Cx,RegAddr);
	//等待数据发送完成	EV8_2
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))	;
	/*** 地址设置完成 ↑↑↑ ***/

	I2C_GenerateSTART(I2Cx, ENABLE);		
	//等待I2C发送起始信号完成	EV5
	while (!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT))	;

	//发送从机地址
	I2C_Send7bitAddress(I2Cx,SlaveAddr, I2C_Direction_Receiver)	;
	//等待事件完成	EV6
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))	;

	while (Len)
	{
		if(Len <= 1)
		{
			//最后一个数据时关闭应答
			I2C_AcknowledgeConfig(I2Cx,DISABLE);
			//最后一个数据时发送停止信号
			I2C_GenerateSTOP(I2Cx,ENABLE);
		}
		
		//一字节数据接收完成	EV7
		if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			*DataPtr = I2C_ReceiveData(I2Cx);
			DataPtr++;
			Len--;
		}
	}

	//使能IIC应答
	I2C_AcknowledgeConfig(I2Cx,ENABLE);
}
 /**********************************END OF FILE**********************************/
