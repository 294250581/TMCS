#include "utBoard.h"

void swgInitD2(void);

void inituartIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

void USART1_Config(USART_TypeDef* USARTx,INT32U bandrate)
{	
    USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    USART_InitStructure.USART_BaudRate = bandrate;					
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			
    USART_InitStructure.USART_Parity = USART_Parity_No;				
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					
    USART_Init(USARTx, &USART_InitStructure);							

	USART_ClearFlag(USARTx,USART_FLAG_TC);
    USART_Cmd(USARTx, ENABLE);	
}

void utInitDebug(void) 		
{		
	inituartIO();
	USART1_Config(USART1,115200);	
}

#if 0
#pragma import(__use_no_semihosting)
struct __FILE  
{  
	int handle;  
};  
FILE __stdout;  

_sys_exit(int x)  
{  
	x = x;  
}
#endif

#if 1
/* 串口发送重定向 */
int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(u8)ch);
	/* 注意此处的USART_FLAG_TXE标志位 */
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}
#endif



//------------------------------------------------------------------------------
void utInitUart3()
{
	#if 1
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;						//定义串口初始化结构体
	//NVIC_InitTypeDef NVIC_InitStructure;	
	#endif

	/* 串口设置*/
	//默认的端口不用重映射

	#if 1	//串口3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			//这个不能不写!!!!
	
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200; 				//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;		//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //禁用RTSCTS硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送接收

	USART_Init(USART3, &USART_InitStructure);					//初始化串口1
	USART_ClearITPendingBit(USART3, USART_IT_RXNE); 			//难道上来就进了中断?
	USART_ClearITPendingBit(USART3, USART_IT_TXE);				//难道上来就进了中断?
	USART_ClearITPendingBit(USART3, USART_IT_TC);				//难道上来就进了中断?
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);									//使能串口1 

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

	utEnInt;
	while(1)
	{
		//utPtn("here1\n");
		//utPtn1("55hahha\n");
		//utPtn("here2\n");
		utPtb('a');
		utPtb1('a');
		LAMP_TOG;

		utDelay1ms(200);
	}
	#endif
}


//##############################################################################
//------------------------------------------------------------------------------
//各设备的功能性IO初始化，不做长时间延时用
void utInitIO(void)	
{
}

//##############################################################################
//初始化中断向量
//##############################################################################
void utInitInt(void)
{
	#if 0
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0); 		//自然启动
	#else
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000); 	//IAP模式启动
	#endif
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		//将优先级组配置为1位优先级和3位子优先级
}
//##############################################################################

void utInitIO(void);
void utInitUart(void);

//系统指示灯、prt外的板子初始化
char str[100];
void utInitBoard()
{	
	utInitVariable();
	utInitIO();
	test();
}


