/**
	******************************************************************************
	* @file    ws2812.c
	* @author  404Zen
	* @version V0.0.1
	* @date    2021.03.31
	* @brief   WS2812驱动
	******************************************************************************
	* @attention
	*
	*	PA5	-	SCK1		实际上无意义
	*	PA7	-	MOSI1	
	* 
	******************************************************************************
	*/

/* Includes ------------------------------------------------------------------*/
#include	"ws2812.h"


#define	WS2812_POINTS		8
#define	WS2812_BUF_SIZE		(WS2812_POINTS*24)
static	uint8_t	WS2812DataBuf[WS2812_BUF_SIZE];			//SPI DMA缓存，每Byte数据相当于WS2812 1bit数据，所以要传送 GRB 24bit,一个像素点需要24Byte缓存 加上复位的1Byte数据
static	uint8_t WS2812GRBBuf[WS2812_POINTS*3];			//WS2812 GRB数据缓存



/**
	* @brief  SPI Initial For WS2812
	* @param  None.
	* @note   None. 
	* @retval None
	*/
void SPIInitForWS2812(void)
{

	GPIO_InitTypeDef	GPIO_InitStructure;
	SPI_InitTypeDef		SPI_InitStructure;
	DMA_InitTypeDef		DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);	//使能 SPI1 GPIOA 时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);							//使能DMA传输


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;					//单工模式，只发不收
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;								//SPI主机
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;							//8bit数据模式
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;									//CPOL = 0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;								//CPHA = 0
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;									//NSS 由软件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;			//SPI速率  SPI1时钟来源于PCLK2 64MHz/8 = 8MHz
	// SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;							//MSB 
	SPI_InitStructure.SPI_CRCPolynomial = 7;									//CRC校验多项式
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);

	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;				//外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)WS2812DataBuf;				//内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;							//数据传输方向，内存到外设
	DMA_InitStructure.DMA_BufferSize = (WS2812_BUF_SIZE);						//缓存大小，1次需要传输的数据量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设数据宽度8bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;			//内存数据宽度8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								//Normal:只发一次，Cycle：循环发送
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//DMA优先级中
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//非内存到内存传输
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);								//DMA1_CH3 初始化
	// SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);							//SPI1 DMA发送使能
	// DMA_Cmd(DMA1_Channel3, ENABLE);
}

/**
	* @brief  GRBDataInit
	* @param  None.
	* @note   None. 
	* @retval None
	*/
void GRBDataInit(void)
{
	static uint8_t Color = 0;
	static uint8_t Cycle = 0;
	uint8_t i = 0;

	if(Color >= 255)
	{
		Color = 0;
		Cycle++;			//颜色变换
	}
	else
	{
		Color++;
	}

	if(Cycle >= 3)
	{
		Cycle = 0;
	}

	for (i = 0; i < WS2812_POINTS; i++)
	{
		WS2812GRBBuf[(i * 3 + Cycle)] = Color;				
		WS2812GRBBuf[(i * 3 + ((Cycle+1)%3))] = 0x00;		
		WS2812GRBBuf[(i * 3 + ((Cycle+2)%3))] = 0x00;
	}	
}

/**
	* @brief  WS2812 发送一帧数据，启动DMA传输
	* @param  None.
	* @note   None. 
	* @retval None.
	*/
void WS2812SendFrame(void)
{
	GRBTransToDataBuf(WS2812GRBBuf, WS2812_POINTS, WS2812DataBuf);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);							//SPI1 DMA发送使能
	DMA_Cmd(DMA1_Channel3, DISABLE);		//关闭DMA1 CH4
	DMA_SetCurrDataCounter(DMA1_Channel3, WS2812_BUF_SIZE);
	DMA_Cmd(DMA1_Channel3, ENABLE);			//关闭DMA1 CH4
}


/**
	* @brief  WS2812 发送复位信号
	* @param  None.
	* @note   None. 
	* @retval 0:Timeout	1:Success
	*/
uint8_t	WS2812Reset(void)
{
	uint8_t RetryCounter = 0;
	uint8_t i = 0;

	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);							//SPI1 DMA发送失能
	// SPI_I2S_SendData(SPI_FOR_WS2812,	0xF0);

	for (i = 0; i < 50; i++)
	{
		//等待发送缓存空
		while (SPI_I2S_GetFlagStatus(SPI_FOR_WS2812, SPI_I2S_FLAG_TXE) == RESET)
		{
			RetryCounter++;
			if(RetryCounter >= 200)
				return 0;
		}
		SPI_I2S_SendData(SPI_FOR_WS2812,	0x00);
	}

	return 1;
}

/**
	* @brief  GRB数据缓存转换到SPI发送数据缓存
	* @param  *GRBBuf:GRB数据缓存地址
	* @param  Points:像素点数量，不超过1024点
	* @param  *DataBuf:SPI发送数据缓存地址
	* @note   None. 
	* @retval None.
	*/
uint8_t GRBTransToDataBuf(uint8_t *GRBBuf, uint16_t Points, uint8_t *DataBuf)
{
	static uint16_t i = 0;
	uint8_t j = 0;
	uint8_t Temp = 0;

	i = (Points * 3);

	//多于1024个点或超过每一帧的像素点
	if((Points > 1024) || (Points > WS2812_POINTS))
		return 0;
	
	while (i--)
	{
		Temp = *GRBBuf;

		for (j = 0; j < 8; j++)
		{
			if(Temp & 0x80)
			{
				*DataBuf = WS2812_T1;
			}
			else
			{
				*DataBuf = WS2812_T0;
			}
			DataBuf++;
			Temp <<= 1;
		}
		GRBBuf++;
	}

	//剩下的像素点填0
	if(Points < WS2812_POINTS)
	{
		i = ((WS2812_POINTS - Points) * 3);

		while (i--)
		{
			for (j = 0; j < 8; j++)
			{
				*DataBuf = WS2812_T0;
				DataBuf++;
			}
			GRBBuf++;
		}	
	}

	return 1;
}

