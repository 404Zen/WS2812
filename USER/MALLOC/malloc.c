/**
	******************************************************************************
	* @file    malloc.c
	* @author  404Zen
	* @version V1.0
	* @date    2021-03-23
	* @brief   Malloc for STM32F10x
	******************************************************************************
	* @attention
	*
	******************************************************************************
	*/

/* Includes ------------------------------------------------------------------*/
#include	"malloc.h"


//内存池
__align(32)	uint8_t	membase[MEM_MAX_SIZE];					//SRAM内存池
//内存管理表
uint16_t	memmapbase[MEM_ALLOC_TABLE_SIZE];				//内存池MAP
//内存管理参数
const	uint32_t	memtblsize = MEM_ALLOC_TABLE_SIZE;
const	uint32_t	memblksize = MEM_BLOCK_SIZE;
const	uint32_t	memsize = MEM_MAX_SIZE;

//内存管理控制器
struct _m_malloc_dev	malloc_dev = 
{
	my_mem_init,			
	my_mem_perused,
	membase,
	memmapbase,
	0,
};

void mymemcpy(void *des, void *src, uint32_t n)
{
	uint8_t *xdes = des;
	uint8_t	*xsrc = src;

	while (n--)
	{
		*xdes++ = *xsrc++;
	}
	
}

void mymemset(void *s, uint8_t c, uint32_t count)
{
	u8 *xs = s;
	while (count--)
	{
		*xs++ = c;
	}
	
}

void my_mem_init(void)
{
	mymemset(malloc_dev.memmap, 0, memtblsize*2);		//内存状态表数据清零
	mymemset(malloc_dev.membase, 0, memsize);			//内存池数据清零
	malloc_dev.memrdy = 1;
}

uint8_t	my_mem_perused(void)
{
	uint32_t	used = 0;
	uint32_t	i;

	for (i = 0; i < memtblsize; i++)
	{
		if(malloc_dev.memmap[i])
			used++;
	}

	return (used*100)/(memtblsize);
}

uint32_t my_mem_malloc(uint32_t size)
{
	signed long offset = 0;
	uint32_t	nmemb;
	uint32_t	cmemb = 0;
	uint32_t	i;

	if(!malloc_dev.memrdy)
		malloc_dev.init();
	
	if(size == 0)
		return 0xFFFFFFFF;
	
	nmemb = size/memblksize;
	if(size % memblksize)
		nmemb++;
	
	for (offset = memtblsize - 1; offset >= 0; offset--)
	{
		if(!malloc_dev.memmap[offset])
			cmemb++;
		else
			cmemb = 0;
		
		if(cmemb == nmemb)
		{
			for (i = 0; i < nmemb; i++)
			{
				malloc_dev.memmap[offset + i] = nmemb;
			}

			return	(offset*memblksize);
		}
	}
	return	0xFFFFFFFF;
}

uint8_t my_mem_free(uint32_t offset)
{
	int i;

	if(!malloc_dev.memrdy)
	{
		malloc_dev.init();
		return 1;
	}

	if(offset < memsize)
	{
		int index = offset/memblksize;
		int nmemb = malloc_dev.memmap[index];
		for (i = 0; i < nmemb; i++)
		{
			malloc_dev.memmap[index+i] = 0;
		}
		return 0;
	}
	else
		return 2;
}

void myfree(void *ptr)
{
	uint32_t offset;
	
	if(ptr == NULL)
		return	;
	
	offset = (uint32_t)ptr - (uint32_t)malloc_dev.membase;
	my_mem_free(offset);
}

void *mymalloc(uint32_t size)
{
	uint32_t offset;

	offset = my_mem_malloc(size);
	if(offset == 0xFFFFFFFF)
		return NULL;
	else
		return	(void*)((uint32_t)malloc_dev.membase+offset);
}


void *myrealloc(void *ptr, uint32_t size)
{
	uint32_t offset;

	offset = my_mem_malloc(size);
	if(offset == 0xFFFFFFFF)
		return NULL;
	else
	{
		mymemcpy((void*)((uint32_t)malloc_dev.membase+offset), ptr, size);
		myfree(ptr);
		return (void*)((uint32_t)malloc_dev.membase+offset);
	}
}
