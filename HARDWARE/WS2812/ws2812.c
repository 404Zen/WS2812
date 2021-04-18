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
	*	PA7	-	MOSI1	- WS2812 DATA IN
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

	//GPIO初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	//SPI初始化
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;					//单工模式，只发不收
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;								//SPI主机
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;							//8bit数据模式
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;									//CPOL = 0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;								//CPHA = 0
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;									//NSS 由软件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;			//SPI速率  SPI1时钟来源于PCLK2 64MHz/8 = 8MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;							//MSB 
	SPI_InitStructure.SPI_CRCPolynomial = 7;									//CRC校验多项式
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);


	//DMA初始化
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
	GRBTransToDataBuf(WS2812GRBBuf, WS2812_POINTS, WS2812DataBuf);				//将GRB数据转换成SPI发送数据
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

	//复位约需要50uS(WS2812S),8MHz情况下发送50个字节约50uS
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

/**
	* @brief  HSL渐变色测试
	* @param  None.
	* @note   None. 
	* @retval None.
	*/
void GradinetHSL(void)
{
	static uint8_t	InitFlag = 0;
	static uint32_t StartColor = RGB_BLUE, EndColor = RGB_YELLOW;
	static float StartH, StartS, StartL;
	static float EndH, EndS, EndL;

	uint32_t Color = 0;
	static uint8_t nSteps = 100;
	static float HueStep = 0;
	static uint8_t i = 0, j = 0, Dir = 0;
	static float Hue, Stauration, Lightness;

	if(0 == InitFlag)
	{
		InitFlag = 1;
		
		RGB2HSL(StartColor, &StartH, &StartS, &StartL);
		RGB2HSL(EndColor, &EndH, &EndS, &EndL);
		
		Hue = StartH;
		Stauration = StartS;
		Lightness = StartL;
		HueStep = (EndH - StartH)/(float)nSteps;
	}
	
	//只改变色相
	Color = HSL2RGB(&Hue, &Stauration, &Lightness);

	if(0 == Dir)
	{
		Hue += HueStep;
		i++;
	}
	else
	{
		Hue -= HueStep;
		i++;
	}
	

	if(i == nSteps)
	{
		Dir = !Dir;
		i = 0;
		// Hue = StartH;
	}

	//填入WS2812缓存
	for (j = 0; j < WS2812_POINTS; j++)
	{
		WS2812GRBBuf[j * 3] = (uint8_t)((Color & 0x0000FF00) >> 8);			//Green	
		WS2812GRBBuf[j * 3 + 1] = (uint8_t)((Color & 0x00FF0000) >> 16);	//Red
		WS2812GRBBuf[j * 3 + 2] = (uint8_t)(Color & 0x000000FF);			//Blue
	}
}

/**
	* @brief  RGB转HSL
	* @param  RGBColor：RGB颜色
	* @param  *H : Hue
	* @param  *S : Staturation
	* @param  *L : Lightness
	* @note   None. 
	* @retval None.
	*/
void RGB2HSL(uint32_t RGBColor, float *H, float *S, float *L)
{
	float R, G, B;
	float Max, Min, DeltaR, DeltaG, DeltaB, DeltaMax;

	R = (float)(((RGBColor & 0x00FF0000) >> 16) / 255.0) ;
	G = (float)(((RGBColor & 0x0000FF00) >> 8) / 255.0);
	B = (float)((RGBColor & 0x000000FF) / 255.0);

	//求最小值
	Min = R < G ? R : G;
	Min = Min < B ? Min : B;
	//求最大值
	Max = R > G ? R : G;
	Max = Max > B ? Max : B;
	
	DeltaMax = Max - Min;			//Delta RGB Value

	*L = (float)((Max + Min) / 2.0);//亮度值

	if(DeltaMax == 0)				//灰度，没有色彩的值
	{
		*H = 0;						//色相
		*S = 0;						//饱和度
	}
	else
	{
		//饱和度
		if(*L < 0.5)
			*S = DeltaMax / (Max + Min);
		else
			*S = DeltaMax / (2 - Max - Min);

		DeltaR = ((Max - R) / 6.0 + (DeltaMax / 2.0)) / DeltaMax;
		DeltaG = ((Max - G) / 6.0 + (DeltaMax / 2.0)) / DeltaMax;
		DeltaB = ((Max - B) / 6.0 + (DeltaMax / 2.0)) / DeltaMax;

		//色相
		if(R == Max)
			*H = DeltaB - DeltaG;
		else if(G == Max)
			*H = (1.0 / 3.0) + DeltaR - DeltaB;
		else if(B == Max)
			*H = (2.0 / 3.0) + DeltaG - DeltaR;
		
		if (*H < 0)
			*H += 1;
		else if(*H > 1)
			*H -= 1;
	}
}

/**
	* @brief  HSL转RGB
	* @param  *H : Hue
	* @param  *S : Staturation
	* @param  *L : Lightness
	* @note   None. 
	* @retval RGB Color.
	*/
uint32_t HSL2RGB(float *H, float *S, float *L)
{
	float R, G, B;
	float Var1, Var2;
	uint32_t Color = 0;

	if(0 == S)			//饱和度为0
	{
		R = *L * 255;
		G = *L * 255;
		B = *L * 255;
	}
	else
	{
		if((*L) < 0.5)
			Var2 = (*L) * (1 + (*S));
		else
			Var2 = ((*L) + (*S)) - ((*S) * (*L));

		Var1 = 2.0 * (*L) - Var2;

		R = 255.0 * Hue2RGB(Var1, Var2, ((*H)+(1.0/3.0)));
		G = 255.0 * Hue2RGB(Var1, Var2, (*H));
		B = 255.0 * Hue2RGB(Var1, Var2, ((*H)-(1.0/3.0)));
 	}

	Color = ((((uint32_t)R) << 16 ) | (((uint32_t)G) << 8) | ((uint32_t)B)) & 0x00FFFFFF;
	return  Color;
}

/**
	* @brief  色相在RGB中的占比
	* @param  Var1
	* @param  Var2
	* @param  VarHue
	* @note   None. 
	* @retval RGB Color.
	*/
float Hue2RGB(float Var1, float Var2, float VarHue)
{
	if(VarHue < 0)
		VarHue += 1;
	else if(VarHue > 1)
		VarHue -= 1;

	if((6.0 * VarHue) < 1)
		return (Var1 + (Var2 - Var1) * 6.0 * VarHue);
	
	if((2.0 * VarHue) < 1)
		return (Var2);
	
	if((3.0 * VarHue) < 2)
		return (Var1 + (Var2 - Var1) * ((2.0/3.0)-VarHue) * 6.0);
	
	return (Var1);
}


