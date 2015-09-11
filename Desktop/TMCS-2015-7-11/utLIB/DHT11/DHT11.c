#include "DHT11.h"

#define Delay_us	Delay_1us

#if 1
INT16U temperature=0,humidity=0;

enum
{
	stDHT11_IDLE = 0,
	stDHT11_START,
	stDHT11_CONVERT,
	stDHT11_OK,
};

void Sensor_IO_IN()			//��©����
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_4;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Sensor_IO_OUT()		//���,�͵�ƽ
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_4;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}	 

#define DQ_PORT	(GPIOA->ODR)
#define DQ_PORT_IN	(GPIOA->IDR)
#define P_DQ	BIT4

#define DQ_HIGH		(DQ_PORT |= P_DQ)
#define DQ_LOW		(DQ_PORT &= ~P_DQ)
#define IS_DQ_HIGH	(DQ_PORT_IN & P_DQ)
#define IS_DQ_LOW	(!(DQ_PORT_IN & P_DQ))


INT8U get_DHT11_value()
{
	static INT8U stDHT11 = stDHT11_IDLE;
	static INT8U cnt10ms = 0;

	INT16U bit;
	INT8U p,time_count;
	INT8U fWrg=0;
	
	switch(stDHT11)
	{
		case stDHT11_IDLE:
			temperature=0x0000;
			humidity=0x0000;
			stDHT11 = stDHT11_START;
			break;

		case stDHT11_START:
			Sensor_IO_OUT();
			DQ_LOW; 			//���������ߴ���18ms���Ϳ�ʼ�ź�
			cnt10ms = 0;		//��ʼ��ʱ
			stDHT11 = stDHT11_CONVERT;
			break;

		case stDHT11_CONVERT:
			if(++cnt10ms==3)	//30ms
			{
				bit=0x8000;
				p = 0;
				fWrg = 0;

				DQ_HIGH;		//�ͷ������ߣ����ڼ��͵�ƽ��Ӧ���ź�
				
				Delay_us(30);	// ��ʱ20-40us���ȴ�һ��ʱ�����Ӧ���źţ�Ӧ���ź��Ǵӻ�����������80us
				
				//��Ӧ���ź�,�˳�
				Sensor_IO_IN();
				if(IS_DQ_HIGH) 
				{	
					stDHT11 = stDHT11_IDLE;
					break;
				}
				
				//�ȴ���ƽ���
				time_count=0;
				while(IS_DQ_LOW)
				{		
					Delay_us(9);
					if(++time_count==12)	//����120us��ʱ����
					{
						fWrg = 1;
						break;
					}
				}
				
				//�ȴ���ƽ���
				time_count=0;
				while(IS_DQ_HIGH)
				{		
					Delay_us(9);
					if(++time_count==12) 	//����120us��ʱ����
					{
						fWrg = 1;
						break;
					}	
				}
				
				//���ݿ�ʼ
				while(p!=2)
				{		
					//�͵�ƽ���50us,�ȴ���ƽ���
					time_count=0;
					while(IS_DQ_LOW)
					{		
						Delay_us(9);
						if(++time_count==10)//����100us��ʱ����
						{
							fWrg = 1;
							break;
						}
					}	
					
					time_count=0;
					do
					{		 
						Delay_us(9);
						time_count++;
						if(time_count==10)	 //����100us��ʱ����
						{
							fWrg = 1;
							break;
						}
					}while(IS_DQ_HIGH);
				
					if(time_count>=5)	//���ߵ�ƽ����50us,˵������λΪ1,����Ϊ0
					{		
						if(p==0)	   humidity|=bit;
						if(p==1)	   temperature|=bit;
					}
					   
					bit>>=1;	
					
					//1�ֽڽ���
					if(bit==0x0000)
					{		
						p++;
						bit=0x8000;
					}	
				}

				if(fWrg)
					stDHT11 = stDHT11_IDLE;
				else
					stDHT11 = stDHT11_OK;
			}
			break;

		case stDHT11_OK:
			if(++cnt10ms==120)
				stDHT11 = stDHT11_IDLE;
			break;
	}

	return stDHT11;
}

#if 0
void testDHT11()
{
	unsigned int wendu=0,shidu=0;
    unsigned char TPtemp[20]={0};
    unsigned char HMtemp[20]={0};
	INT8U cnt10ms=0;

	while(1)
	{
		Delay_ms(10);
		if(get_DHT11_value()==stDHT11_OK)
		{
			if(++cnt10ms!=100)
				continue;

			//1s��ʾһ��
			wendu=temperature>>8;
			shidu=humidity>>8;	
			sprintf(TPtemp,"%d",wendu);
			sprintf(HMtemp,"%d",shidu);
			
			USART1_SendString("TP1:") ;
			USART1_SendString(TPtemp);
			USART1_SendString(";") ;
			
			USART1_SendString("HM1:") ;
			USART1_SendString(HMtemp);
			USART1_SendString(";") ;
			
			USART1_SendString("CO1:0") ;
			USART1_SendString(";\r\n") ;
		}
		else
			cnt10ms= 0;
	}
}

int  main(void)
{   
	Init();
	testDHT11();	
	while(1);
}
#endif

#endif
