#include "utKey.h"

void swgInitKey()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//GPIOC->ODR &= ~(BIT0+BIT1+BIT2+BIT31+BIT4);	//输出0
	
	(GPIOC->ODR) |= 0x1f;		//全输出高
	(GPIOC->ODR) &= ~(BIT0);	//相应位清零

	//PC5-PC8输入 Y
	GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_5+GPIO_Pin_6+GPIO_Pin_7+GPIO_Pin_8;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	//PC0-PC4输出 X
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
	
	//读入
    RowVal = (((GPIOC->IDR)&(BIT5+BIT6+BIT7+BIT8)) | ((GPIOC->ODR)&(BIT0+BIT1+BIT2+BIT3+BIT4)));

	switch(stKey)
	{
		case STKEY_IDLE:				//待机状态
			if(IS_KEYDN)
			{
				//utPtn("find key\n");
				KeyTimes = 0;
				stKey = STKEY_DETDN;	//延时20ms之后再测，计数清零
				return;
			}
			
			//没有按键，直接忽略
			(GPIOC->ODR) |= 0x1f;		//全输出高
			(GPIOC->ODR) &= ~(BIT0<<cntBitXOut);	//相应位清零
			if(++cntBitXOut==5)
				cntBitXOut = 0;
			break;

		case STKEY_DETDN:				//20ms之后再确认按下
			if(++KeyTimes>=20)	
			{
				if(IS_KEYDN)			//20ms之后仍是低
				{
					KeyTimes = 0;
					KeyVal = RowVal;	//此时记录按键
					BELL_ON;			//开始响					
					fBkBlink = fBlink;	//记录开始是否闪烁
					stKey = STKEY_CNT;	//计数清零，进入按键计时状态
					return;
				}

				//误动作，回到IDLE
				stKey = STKEY_IDLE;
			}

			//继续等待20ms
			break;

		case STKEY_CNT:
		{
			if(!IS_KEYDN)				//如果按键弹起,只有这一个出口
			{
				KeyTimes = 0;			
				stKey = STKEY_DETUP;	//20ms之后才认为结束，需要计数清零
				return;				
			}

			//按键仍是按着状态,且大于1s，继续循环
			if(++KeyTimes>=1000)
			{
				if(KeyVal == KEY_MENU)
				{
					BELL_OFF;													
					KeyTimes = 0;					//立刻就赋值为0
					stKey = STKEY_DETUP;
					KeyVal = KEY_MENU_LONG;			//换为长键
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
						if(KeyTimes%50 == 0)	//100ms弄一次
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

				//其它先不管，等待按键结束退出
				return;
			}		

			//继续等待
		}
		break;

		case STKEY_DETUP:
		{
			if(++KeyTimes >= 20)	//延时20ms再检测
			{
				if(IS_KEYDN)		//如果还是按键，继续等高
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
				
				//完全结束
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

			//继续等待20ms
		}
		break;

		default:	//其它未知状态
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
