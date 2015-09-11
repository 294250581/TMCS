#include "utTest.h"

#if 1
void utPtb1(char c) 						//串口1输出一个字节
{
	USART1->DR = c;
	while(!(USART1->SR & BIT7));			//等待传输空标志位,写入数据后该位会变为0
}

//------------------------------------------------------------------------------
void utPtb3(char c) 						//串口2输出一个字节
{
	USART3->DR = c;
	while(!(USART3->SR & BIT7));			//等待传输空标志位,写入数据后该位会变为0
}

void test(void)
{}
#else
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void utPtb1(char c) 						//串口1输出一个字节
{
	USART1->DR = c;
	while(!(USART1->SR & BIT7));			//等待传输空标志位,写入数据后该位会变为0
}

//------------------------------------------------------------------------------
void utPtb2(char c) 						//串口2输出一个字节
{
	USART2->DR = c;
	while(!(USART2->SR & BIT7));			//等待传输空标志位,写入数据后该位会变为0
}

//------------------------------------------------------------------------------
void utPtb3(char c) 						//串口3输出一个字节
{
	USART3->DR = c;
	while(!(USART3->SR & BIT7));			//等待传输空标志位,写入数据后该位会变为0
}

//------------------------------------------------------------------------------
void utPtc1(char c)							//输出一个字符
{
	if(c == '\n')							//如果是'n'先加一个'r'
	{
		utPtb1('\r');		
	}

	utPtb1(c);	
}

//------------------------------------------------------------------------------
void utPts1(char *str)
{
	char *p = str;
	
	while(*p)
	{
		utPtb1(*p);
		p++;
	}
}

//------------------------------------------------------------------------------
void utPtn1(char *str)
{
	char *p = str;
	
	while(*p)
	{
		utPtc1(*p);
		p++;
	}
}

char utScb1(void)
{
	char dat;
	
	if((USART1->SR & USART_FLAG_RXNE))
	{
		dat = (char)(USART1->DR);
		utPtb1(dat);
		return dat;
	}

	return 0;
}

char utGtb1(void)
{
	char dat;

	while(!(USART1->SR & USART_FLAG_RXNE));
	dat = (char)(USART1->DR);
	utPtb1(dat);
	return dat;
}

void utGtn1(char *str)
{
	INT8U i=0;
	char dat;

	do
	{
		dat=utGtb1();
		str[i++] = dat;
	}while(dat!='\r');
	
	str[i++] = '\n';
	utPtb1('\n');
	
	str[i] = 0;
}

void testUart()
{
	while(1)
		utGtb1();
}


void testCom2()
{
	#if 0
	int i;
	while(1)
	{
		sprintf(str,"%04X",dat++);
		for(i=0;i<4;i++)
			utPtb1(str[i]);

		LAMP_TOG;
		utDelay1ms (50);
	}
	#endif
}

void testDMAUart()
{
#if 0
	static INT16U datx1 = 0;
	INT8U temp,i;
	
	utInitUart ();
	utEnInt;
	while(1)
	{
		#if 0	//发送
		sprintf(tx1Buf,"0123456789__%05d\n",++datx1);
		tx1Len = strlen(tx1Buf);

		fSendOver = 0;

		//配置长度，后使能
		DMA1_Channel4->CNDTR = tx1Len;
		DMA_Cmd(DMA1_Channel4, ENABLE);
		
		while(!fSendOver);
		fSendOver=0;
		
		DMA_Cmd(DMA1_Channel4, DISABLE);
		
		utDelay1ms(2);
		USART1->CR1 |= BIT9;		//1:奇校验来触发错误
		utPtb1(0xff);		
		USART1->CR1 &= ~BIT9;		//改回偶校验
		
		utDelay1ms (80);
		LAMP_TOG;
		#endif

		#if 1	//接收测试
		if(fRcvOver)
		{
			fRcvOver = 0;

			temp = (USART1->SR);
			temp = (USART1->DR);

			DMA_Cmd(DMA1_Channel5,DISABLE);
			temp = LEN_UART1 - DMA_GetCurrDataCounter(DMA1_Channel5);
			if(temp)
			{
				utPrt("new%d: ",temp);
				for (i = 0;i < temp;i++)
				{
					utPtb(rx1Buf[i]);
				}
			}

			//设置传输数据长度
			DMA1_Channel5->CNDTR = LEN_UART1;
			//打开DMA
			DMA_Cmd(DMA1_Channel5,ENABLE);
		}
		#endif

		utDelay1ms(1);
		LAMP_TOG;
	}
#endif
}

void testCom4()
{
	static INT16U datx1=0;

	LAMP_ON;
	
	while(1)
	{
		sprintf(str,"__%05d\n",++datx1);
		utPtn1("0123456789");
		utPts1(str);

		#if 1
		utDelay1ms (2);				//只要有两ms的延迟，就不会和前面的字符连到一块
		//此处1ms，usb2ms时会校验会单独
		//此处2ms，usb2ms后确实没再出现odd校验跟前面数据连在一起的情况,
		USART1->CR1 |= BIT9;		//1:奇校验来触发错误
		utPtb1(0x00);		
		USART1->CR1 &= ~BIT9;		//改回偶校验
		#endif
		
		utDelay1ms (80);
		LAMP_TOG;
	}
}


long datx = 1000;
void testCom1()
{
	int i;

	//LAMP_OFF;

	utPrt("reset\n\n");

	while(1)
	{
		utEnterCrt;
		#if 0
		for(i=0;i<4;i++)
			utPtb1(0x55);
		#else
		
		dtPC.fl = 1234.5;//datx++;	//搞上这个才跟老马的一样
		for(i=0;i<4;i++)
			utPtb1(dtPC.bt[i]);

		if(datx>=20000)
			datx = 1000;
		
		#endif
		utExitCrt;

		utDelay1ms(100);
		LAMP_TOG;

		utEnterCrt;
		#if 0
		for(i=0;i<4;i++)
			utPtb1(0x55);
		#else
		
		dtPC.fl = 5678.9;//datx++;	//搞上这个才跟老马的一样
		for(i=0;i<4;i++)
			utPtb1(dtPC.bt[i]);

		if(datx>=20000)
			datx = 1000;
		
		#endif
		utExitCrt;

		utDelay1ms(100);
		LAMP_TOG;		
	}
}

#if 0
void testRelay2()
{
#if 0
	char dat;

	while(1)
	{
		dat = utScb1();
		
		if(dat=='1')
		{
			FILT_ON;
		}
		else if(dat=='2')
		{
			FILT_OFF;
		}

		utTwk(200);
	}
#endif	
}
#endif

void testIO()
{
	char dat;

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_PD01, ENABLE);
	AFIO->MAPR |= BIT26; 	

	//START IN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	//CTRL_OUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	

	//ctl0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	//purge
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//run
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  
	GPIO_Init(GPIOD, &GPIO_InitStructure);	


	#if 0
	while(1)
	{
		dat = utScb1();
		
		if(dat=='1')
		{
			(GPIOA->ODR)|=BIT14;
			if((GPIOA->IDR)&BIT15)
				utPtb1('+');
			else
				utPtb1('-');
		}
		else if(dat=='2')
		{
			(GPIOA->ODR)&=~BIT14;
		}

		utTwk(200);
	}	
	#else
	while(1)
	{
		(GPIOA->ODR)^=BIT14;
		(GPIOC->ODR)^=BIT9;
		(GPIOD->ODR)^=BIT0;
		(GPIOC->ODR)^=BIT15;		
		utDelay1ms(250);
		utDelay1ms(250);
		LAMP_TOG;
		utDelay1ms(250);
		utDelay1ms(250);
		LAMP_TOG;
	}
	#endif
}


void testLCD()
{
	utPtn("init dis\n");
	VSTM0802AInit();

	#if 0	//先写后读测试，读出的应该和写入的一样，否则就有问题
	SetCursorPos(0,0);
	ramptc (0xff);
	utPrt("\nget = %x\n\n",ReadData (0, 0));
	#endif

	
	utPtn("init dis ok\n");
	utDelay1ms (200);
	//showTest();

	showInit();
	utDelay1s(2);
	while(1);

	#if 0
	showHeating();
	utDelay1s(2);

	showCheckHome();
	utDelay1s(2);

	showMain();
	utDelay1s(2);

	showDAOut();
	utDelay1s(2);

	showWaveCal();
	utDelay1s(2);
	#endif
}

void testMotor(void)
{
	#if 1
	int j;

	#if 0
	for(j=0;j<50;j++)
	{
		utPtb('&');
		LAMP_TOG;
		utDelay1ms(100);
	}
	#endif

	swgInitMotor();

	utPtn("neg\n");
	for(j=0;j<900;j++)			
	{
		motorInv();				
		utDelay1ms (2);
	}

	for(j=0;j<10;j++)
	{
		utDelay1ms (200);
		LAMP_TOG;
	}

	utPtn("pos\n");

	for(j=0;j<900;j++)			
	{
		motor();				
		utDelay1ms (2);
	}
	#endif
}

void testADS1110(void)
{
	iicinit();
	while(1)
	{
		utPrt("ad0=%x\n",ReadDataCfgReg(0x91,0x01));
		utPrt("ad1=%x\n\n",ReadDataCfgReg(0x93,0x01));	
		utDelay1ms(200);
		LAMP_TOG;
	}
}

void testStore()
{
#if 0
	char dat;
	int i;

	while(1)
	{
		dat = utScb1();
		
		if(dat=='1')
		{
			for(i=0;i<10;i++)
				strctStore.bsicAir[i] = 1.0*i;
			storeData ();
		}
		else if(dat=='2')
		{
			for(i=0;i<10;i++)
				strctStore.bsicAir[i] = 55.55;
			storeData ();
		}

		utTwk(200);
	}
#endif
}

void testDAC8560(void)
{
	swgInitDAC8560();
	utDelay1ms (200);
	while(1)
	{
		utDAC8560Write(0xFFFF);	//最高电压的一半
		utDelay1ms (200);
	}
}

#if 0
void testKey()
{
	INT8U cnt1ms=0;

	swgInitBell();
	swgInitKey();
	while(1)
	{
		checkKey ();
		utDelay1ms (1);
		if(++cnt1ms>=200)
		{
			cnt1ms = 0;
			LAMP_TOG;
		}
	}
}
#endif

void testRelay()
{
	int i;
	
	swgInitRelay ();


	#if 0
	for(i=0;i<50;i++)
	{
		LAMP_TOG;
		utDelay1ms(100);
	}

	FILT_ON;
	#else

	while(1)
	{
		dat = utScb1();
		
		if(dat=='1')
		{
			FILT_ON;
		}
		else if(dat=='2')
		{
			FILT_OFF;
		}

		utTwk(200);
	}
	
	#endif
}

void utInitTestUart()
{
	#if 1
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;						//定义串口初始化结构体
	//NVIC_InitTypeDef NVIC_InitStructure;	
	#endif

	/* 串口设置*/
	//默认的端口不用重映射

	#if 1	//串口1

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			//这个不能不写!!!!
	
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600; 				//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;		//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //禁用RTSCTS硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送接收

	USART_Init(USART1, &USART_InitStructure);					//初始化串口1
	USART_ClearITPendingBit(USART1, USART_IT_RXNE); 			//难道上来就进了中断?
	USART_ClearITPendingBit(USART1, USART_IT_TXE);				//难道上来就进了中断?
	USART_ClearITPendingBit(USART1, USART_IT_TC);				//难道上来就进了中断?
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);									//使能串口1 

	#if 0
	//开启串口1中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	#endif
	#endif

	#if 0	//串口2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			//这个不能不写!!!!

	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	#if 0	//串口2 RTS，CTS
	//RTS OUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//CTS IN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	#endif
	
	USART_InitStructure.USART_BaudRate = 19200; 				//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;		//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //禁用RTSCTS硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送接收
	//USART_InitStructure.USART_Clock = USART_Clock_Disable;	//串口时钟禁止
	//USART_InitStructure.USART_CPOL = USART_CPOL_Low;			//时钟下降沿有效
	//USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;		//数据在第二个时钟沿捕捉
	//USART_InitStructure.USART_LastBit = USART_LastBit_Disable;	//最后数据位的时钟脉冲不输出到SCLK引脚
	
	USART_Init(USART2, &USART_InitStructure);					//初始化串口2
	USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
	USART_ClearITPendingBit(USART2, USART_IT_TXE); 
	USART_ClearITPendingBit(USART2, USART_IT_TC);				//我靠，上来就有标志
	
	USART_ITConfig(USART2, USART_IT_TC, DISABLE);				//禁用发送中断
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	USART_Cmd(USART2, ENABLE);									//使能串口2 

	//开启串口2中断
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	#endif


	#if 0
	utPtn("here\n");

	//utEnInt;
	while(1)
	{
		utPtb('a');
		utPtb1('a');
		LAMP_TOG;

		utDelay1ms(200);
	}
	#endif
}


void test(void)
{
	utInitTestUart();
	
	testKey();
	//testIO();
	//testDAC8560();
	//initTFT100 ();
	
	//testADS1110();
	//testCom3();
	//testCom3();
	//testUart();
	//testCom2();
	//testStore();
	//testRelay();
	//testCom1();
	//testLCD();
	//testMotor ();
	//testKey ();
	//swgInitRelay();
	//testDAC8560();
	//bgStepMotor();
	//bgCal656nm ();
	while(1);
}
#endif


