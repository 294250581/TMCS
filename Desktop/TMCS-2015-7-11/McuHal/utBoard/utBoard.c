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
/* ���ڷ����ض��� */
int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(u8)ch);
	/* ע��˴���USART_FLAG_TXE��־λ */
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}
#endif



//------------------------------------------------------------------------------
void utInitUart3()
{
	#if 1
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;						//���崮�ڳ�ʼ���ṹ��
	//NVIC_InitTypeDef NVIC_InitStructure;	
	#endif

	/* ��������*/
	//Ĭ�ϵĶ˿ڲ�����ӳ��

	#if 1	//����3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			//������ܲ�д!!!!
	
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200; 				//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //����RTSCTSӲ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ��ͽ���

	USART_Init(USART3, &USART_InitStructure);					//��ʼ������1
	USART_ClearITPendingBit(USART3, USART_IT_RXNE); 			//�ѵ������ͽ����ж�?
	USART_ClearITPendingBit(USART3, USART_IT_TXE);				//�ѵ������ͽ����ж�?
	USART_ClearITPendingBit(USART3, USART_IT_TC);				//�ѵ������ͽ����ж�?
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);									//ʹ�ܴ���1 

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
//���豸�Ĺ�����IO��ʼ����������ʱ����ʱ��
void utInitIO(void)	
{
}

//##############################################################################
//��ʼ���ж�����
//##############################################################################
void utInitInt(void)
{
	#if 0
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0); 		//��Ȼ����
	#else
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000); 	//IAPģʽ����
	#endif
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		//�����ȼ�������Ϊ1λ���ȼ���3λ�����ȼ�
}
//##############################################################################

void utInitIO(void);
void utInitUart(void);

//ϵͳָʾ�ơ�prt��İ��ӳ�ʼ��
char str[100];
void utInitBoard()
{	
	utInitVariable();
	utInitIO();
	test();
}


