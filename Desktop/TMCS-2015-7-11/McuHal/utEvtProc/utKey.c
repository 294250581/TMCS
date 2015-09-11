#include "utKey.h"

void swgInitKey()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//GPIOC->ODR &= ~(BIT0+BIT1+BIT2+BIT31+BIT4);	//���0
	
	(GPIOC->ODR) |= 0x1f;		//ȫ�����
	(GPIOC->ODR) &= ~(BIT0);	//��Ӧλ����

	//PC5-PC8���� Y
	GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_5+GPIO_Pin_6+GPIO_Pin_7+GPIO_Pin_8;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	//PC0-PC4��� X
	GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_0+GPIO_Pin_1+GPIO_Pin_2+GPIO_Pin_3+GPIO_Pin_4;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

INT32U KeyVal = 0;

INT8U getAsciiOfKey()
{
	switch(KeyVal)
	{
		case KEY_NUM0:
			return '0';
			
		case KEY_NUM1:
			return '1';
			
		case KEY_NUM2:
			return '2';
			
		case KEY_NUM3:
			return '3';
			
		case KEY_NUM4:
			return '4';
			
		case KEY_NUM5:
			return '5';
			
		case KEY_NUM6:
			return '6';
			
		case KEY_NUM7:
			return '7';
			
		case KEY_NUM8:
			return '8';
			
		case KEY_NUM9:
			return '9';

		case KEY_DOT:
			return '.';

		default:
			return 0;
	}
}

void checkKey(void)
{	
	static INT32U RowVal = 0x00000001;
	//static INT32U LineVal = 0x00000001;
	static INT32U KeyTimes=0;
	static INT8U fBkBlink = 0;
	static INT8U fUpDown = 0;
	INT8U dat;
	
	#define IS_KEYDN	((RowVal & (BIT5+BIT6+BIT7+BIT8)) != (BIT5+BIT6+BIT7+BIT8))
	
	enum
	{
		STKEY_IDLE,
		STKEY_DETDN,
		STKEY_CNT,
		STKEY_DETUP
	};
	static INT8U stKey=STKEY_IDLE;
	static INT8U cntBitXOut=0;
	
	//����
    RowVal = (((GPIOC->IDR)&(BIT5+BIT6+BIT7+BIT8)) | ((GPIOC->ODR)&(BIT0+BIT1+BIT2+BIT3+BIT4)));

	switch(stKey)
	{
		case STKEY_IDLE:				//����״̬
			if(IS_KEYDN)
			{
				//utPtn("find key\n");
				KeyTimes = 0;
				stKey = STKEY_DETDN;	//��ʱ20ms֮���ٲ⣬��������
				return;
			}
			
			//û�а�����ֱ�Ӻ���
			(GPIOC->ODR) |= 0x1f;		//ȫ�����
			(GPIOC->ODR) &= ~(BIT0<<cntBitXOut);	//��Ӧλ����
			if(++cntBitXOut==5)
				cntBitXOut = 0;
			break;

		case STKEY_DETDN:				//20ms֮����ȷ�ϰ���
			if(++KeyTimes>=20)	
			{
				if(IS_KEYDN)			//20ms֮�����ǵ�
				{
					KeyTimes = 0;
					KeyVal = RowVal;	//��ʱ��¼����
					BELL_ON;			//��ʼ��					
					fBkBlink = fBlink;	//��¼��ʼ�Ƿ���˸
					stKey = STKEY_CNT;	//�������㣬���밴����ʱ״̬
					return;
				}

				//�������ص�IDLE
				stKey = STKEY_IDLE;
			}

			//�����ȴ�20ms
			break;

		case STKEY_CNT:
		{
			if(!IS_KEYDN)				//�����������,ֻ����һ������
			{
				KeyTimes = 0;			
				stKey = STKEY_DETUP;	//20ms֮�����Ϊ��������Ҫ��������
				return;				
			}

			//�������ǰ���״̬,�Ҵ���1s������ѭ��
			if(++KeyTimes>=1000)
			{
				if(KeyVal == KEY_MENU)
				{
					BELL_OFF;													
					KeyTimes = 0;					//���̾͸�ֵΪ0
					stKey = STKEY_DETUP;
					KeyVal = KEY_MENU_LONG;			//��Ϊ����
					//osalSetEvent (EVENT_KEY);	
					utSetInputEvt (INPUT_KEY);
					return;
				}
				else if((KeyVal == KEY_UP) || (KeyVal == KEY_DOWN))
				{		
					#if 0
					if((MenuState == P1) || (MenuState == P2))
					{
						fUpDown = 1;
						if(KeyTimes%50 == 0)	//100msŪһ��
						{
							BELL_TOG;
							if(fBlink)
								BlinkOff();		
							//osalSetEvent (EVENT_KEY);
							utSetInputEvt (INPUT_KEY);
						}
					}
					#endif
				}

				//�����Ȳ��ܣ��ȴ����������˳�
				return;
			}		

			//�����ȴ�
		}
		break;

		case STKEY_DETUP:
		{
			if(++KeyTimes >= 20)	//��ʱ20ms�ټ��
			{
				if(IS_KEYDN)		//������ǰ����������ȸ�
				{
					KeyTimes = 0;
					return;
				}

				if(fUpDown)
				{
					fUpDown = 0;
					if(fBkBlink)
						BlinkOn();
				}
				
				//��ȫ����
				KeyTimes = 0;
				stKey = STKEY_IDLE;
				BELL_OFF;

				#if 0
				utPrt("key=%x\n",KeyVal);
				dat = getAsciiOfKey();
				if(dat)
					utPtc(dat);
				utPtn("\n");
				#endif
				
				utSetInputEvt (INPUT_KEY);
				return;
			}	

			//�����ȴ�20ms
		}
		break;

		default:	//����δ֪״̬
		{
			while(1)
			{
				LAMP_TOG;
				BELL_TOG;
				utDelay1ms(100);
			}
		}
		//break;
	}	
}


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
