/**
	******************************************************************************
	* @file    malloc.h
	* @author  404Zen
	* @version V1.0
	* @date    2021-03-23
	* @brief   Malloc for STM32F10x
	******************************************************************************
	* @attention
	*
	******************************************************************************
	*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MALLOC_H
#define __MALLOC_H

/* Includes ------------------------------------------------------------------*/
#include	"stm32f10x.h"


#ifndef	NULL
#define	NULL	0
#endif

#define	SRAMBANK				1								//支持的SRAM块

#define	MEM_BLOCK_SIZE			32								//内存块大小	32字节
#define	MEM_MAX_SIZE			40*1024							//最大管理内存	40K
#define	MEM_ALLOC_TABLE_SIZE	MEM_MAX_SIZE/MEM_BLOCK_SIZE		//内存表大小

//内存管理控制器
struct _m_malloc_dev
{
	void	(*init)(void);				//初始化
	uint8_t	(*perused)(void);			//内存使用率
	uint8_t	*membase;						//内存池
	uint16_t	*memmap;					//内存管理状态表
	uint8_t	memrdy;							//内存管理是否就绪
};

extern	struct	_m_malloc_dev	malloc_dev;	//


void mymemset(void *s, uint8_t c, uint32_t count);			//设置内存
void mymemcpy(void *des, void *src, uint32_t n);			//复制内存
void mymemset(void *s, uint8_t c, uint32_t count);
void my_mem_init(void);										//内存管理初始化函数
uint8_t	my_mem_perused(void);
uint8_t my_mem_free(uint32_t offset);
uint32_t my_mem_malloc(uint32_t size);

void myfree(void *ptr);
void *mymalloc(uint32_t size);
void *myrealloc(void *ptr, uint32_t size);

#endif
