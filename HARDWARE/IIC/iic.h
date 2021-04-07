/**
	******************************************************************************
	* @file    iic.h
	* @author  404Zen
	* @version V0.0.1
	* @date    2021.03.25
	* @brief   STM32硬件IIC驱动
	******************************************************************************
	* @attention
	*
	* IIC1 PB6-SCL1,PB7-SDA1.
	* IIC2 PB10-SCL2,PB11-SDA2.
	* 
	******************************************************************************
	*/
 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__IIC_H__
#define	__IIC_H__

/* Includes ------------------------------------------------------------------*/
#include    "sys.h"
#include	"delay.h"
#include    "stm32f10x_gpio.h"
#include    "stm32f10x_rcc.h"
#include	"stm32f10x_i2c.h"


/* Defines ------------------------------------------------------------------*/
#define	STM32_IIC_SPEED			100000	//该值必须小于 400000,即最大传输速度400KHz		在UCOSIII中 200KHz以上速度会导致STM32的硬件IIC卡死在等待应答信号，后续改用中断方式？
#define	STM32_IIC_ADDR			0x32	//STM32 IIC 地址，不能与总线上其他地址相同

/* Functions -----------------------------------------------------------------*/
void	IICInit(I2C_TypeDef* I2Cx);					//IIC及对应IO初始化 非Remap
void	IICSendByte(I2C_TypeDef* I2Cx, uint8_t SlaveAddr, uint8_t RegAddr, uint8_t Data);	//IIC发送一字节数据
uint8_t IICReadByte(I2C_TypeDef* I2Cx, uint8_t SlaveAddr, uint8_t RegAddr);					//IIC读取一字节数据
void	IICSendBuffer(I2C_TypeDef* I2Cx, uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *DataPtr, uint16_t Len);
void	IICReadBuffer(I2C_TypeDef* I2Cx, uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *DataPtr, uint16_t Len);
 #endif
 /**********************************END OF FILE**********************************/
