#include "utTest.h"

#if 1
void utPtb1(char c) 						//����1���һ���ֽ�
{
	USART1->DR = c;
	while(!(USART1->SR & BIT7));			//�ȴ�����ձ�־λ,д�����ݺ��λ���Ϊ0
}

//------------------------------------------------------------------------------
void utPtb3(char c) 						//����2���һ���ֽ�
{
	USART3->DR = c;
	while(!(USART3->SR & BIT7));			//�ȴ�����ձ�־λ,д�����ݺ��λ���Ϊ0
}

void test(void)
{}
#else
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void utPtb1(char c) 						//����1���һ���ֽ�
{
	USART1->DR = c;
	while(!(USART1->SR & BIT7));			//�ȴ�����ձ�־λ,д�����ݺ��λ���Ϊ0
}

//------------------------------------------------------------------------------
void utPtb2(char c) 						//����2���һ���ֽ�
{
	USART2->DR = c;
	while(!(USART2->SR & BIT7));			//�ȴ�����ձ�־λ,д�����ݺ��λ���Ϊ0
}

//------------------------------------------------------------------------------
void utPtb3(char c) 						//����3���һ���ֽ�
{
	USART3->DR = c;
	while(!(USART3->SR & BIT7));			//�ȴ�����ձ�־λ,д�����ݺ��λ���Ϊ0
}

//------------------------------------------------------------------------------
void utPtc1(char c)							//���һ���ַ�
{
	if(c == '\n')							//�����'n'�ȼ�һ��'r'
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
		#if 0	//����
		sprintf(tx1Buf,"0123456789__%05d\n",++datx1);
		tx1Len = strlen(tx1Buf);

		fSendOver = 0;

		//���ó��ȣ���ʹ��
		DMA1_Channel4->CNDTR = tx1Len;
		DMA_Cmd(DMA1_Channel4, ENABLE);
		
		while(!fSendOver);
		fSendOver=0;
		
		DMA_Cmd(DMA1_Channel4, DISABLE);
		
		utDelay1ms(2);
		USART1->CR1 |= BIT9;		//1:��У������������
		utPtb1(0xff);		
		USART1->CR1 &= ~BIT9;		//�Ļ�żУ��
		
		utDelay1ms (80);
		LAMP_TOG;
		#endif

		#if 1	//���ղ���
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

			//���ô������ݳ���
			DMA1_Channel5->CNDTR = LEN_UART1;
			//��DMA
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
		utDelay1ms (2);				//ֻҪ����ms���ӳ٣��Ͳ����ǰ����ַ�����һ��
		//�˴�1ms��usb2msʱ��У��ᵥ��
		//�˴�2ms��usb2ms��ȷʵû�ٳ���oddУ���ǰ����������һ������,
		USART1->CR1 |= BIT9;		//1:��У������������
		utPtb1(0x00);		
		USART1->CR1 &= ~BIT9;		//�Ļ�żУ��
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
		
		dtPC.fl = 1234.5;//datx++;	//��������Ÿ������һ��
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
		
		dtPC.fl = 5678.9;//datx++;	//��������Ÿ������һ��
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

	#if 0	//��д������ԣ�������Ӧ�ú�д���һ���������������
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
		utDAC8560Write(0xFFFF);	//��ߵ�ѹ��һ��
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
	USART_InitTypeDef USART_InitStructure;						//���崮�ڳ�ʼ���ṹ��
	//NVIC_InitTypeDef NVIC_InitStructure;	
	#endif

	/* ��������*/
	//Ĭ�ϵĶ˿ڲ�����ӳ��

	#if 1	//����1

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			//������ܲ�д!!!!
	
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600; 				//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //����RTSCTSӲ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ��ͽ���

	USART_Init(USART1, &USART_InitStructure);					//��ʼ������1
	USART_ClearITPendingBit(USART1, USART_IT_RXNE); 			//�ѵ������ͽ����ж�?
	USART_ClearITPendingBit(USART1, USART_IT_TXE);				//�ѵ������ͽ����ж�?
	USART_ClearITPendingBit(USART1, USART_IT_TC);				//�ѵ������ͽ����ж�?
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);									//ʹ�ܴ���1 

	#if 0
	//��������1�ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	#endif
	#endif

	#if 0	//����2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			//������ܲ�д!!!!

	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	#if 0	//����2 RTS��CTS
	//RTS OUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//CTS IN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	#endif
	
	USART_InitStructure.USART_BaudRate = 19200; 				//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;		//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //����RTSCTSӲ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ��ͽ���
	//USART_InitStructure.USART_Clock = USART_Clock_Disable;	//����ʱ�ӽ�ֹ
	//USART_InitStructure.USART_CPOL = USART_CPOL_Low;			//ʱ���½�����Ч
	//USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;		//�����ڵڶ���ʱ���ز�׽
	//USART_InitStructure.USART_LastBit = USART_LastBit_Disable;	//�������λ��ʱ�����岻�����SCLK����
	
	USART_Init(USART2, &USART_InitStructure);					//��ʼ������2
	USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
	USART_ClearITPendingBit(USART2, USART_IT_TXE); 
	USART_ClearITPendingBit(USART2, USART_IT_TC);				//�ҿ����������б�־
	
	USART_ITConfig(USART2, USART_IT_TC, DISABLE);				//���÷����ж�
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//�����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	USART_Cmd(USART2, ENABLE);									//ʹ�ܴ���2 

	//��������2�ж�
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


