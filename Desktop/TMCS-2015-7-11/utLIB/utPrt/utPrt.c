//==============================================================================
//C文件
//与硬件无关的常用库函数
#include "utPrt.h"

//==============================================================================
//内部符号预定义
#define PT_TIMER	(TIM3->SR)
#define TXF			TIM_IT_Update	//定义用于prt定时的溢出标志位

#define IS_TXF		(PT_TIMER & TXF)
#define TXF_OFF 	(PT_TIMER &= ~TXF)

//==============================================================================
//内部调用变量

//==============================================================================
//内部调用函数定义
static char hex2cha(char hex);

//==============================================================================
//外部调用函数
void (*utpPtb)(char c);		//通用，挂接指针后就用某个具体ptb


//##############################################################################
//prt模块
//##############################################################################
//------------------------------------------------------------------------------
void utPtbx(char c) 			//输出一个字节,万能通用
{	
	INT8U i;

	utEnterCrt;
	
	while (!IS_TXF);		//同步	
	TXF_OFF; 				//清除先前的标志位
	//(1)至少多了一个bit,宁慢不快，主要是等待最后一位的问题，连续时一位也不会多

	//起始位
	while (!IS_TXF);		//同步
	TXP_LOW;
	TXF_OFF; 				//清除标志位

	//8位数据
	for (i = 0; i < 8; i++)
	{
		while (!IS_TXF);	//同步

		#if 1
		if (c & BIT0)		//先传低位
			TXP_HIGH; 		//不能加分号
		else
			TXP_LOW;		//不能加分号
		#endif
		
		c >>= 1;			//右移一位

		TXF_OFF; 			//清除标志位
	}

	//终止位
	while (!IS_TXF);
	TXP_HIGH; 				//结束位
	TXF_OFF; 				//清除标志位

	//如果(1)处不等待，此处就要等待

	utExitCrt;
}

//------------------------------------------------------------------------------
void utPtb(char c)			//输出一个字符
{
	(*utpPtb)(c);
}


//------------------------------------------------------------------------------
void utPtc(char c)			//输出一个字符
{
	if(c == '\n')			//如果是'n'先加一个'r'
	{
		(*utpPtb)('\r');		
	}

	(*utpPtb)(c);	
}

//------------------------------------------------------------------------------
void utPts(char *str)
{
	char *p = str;
	
	while(*p)
	{
		(*utpPtb)(*p);
		p++;
	}
}

//------------------------------------------------------------------------------
void utPtn(char *str)
{
	char *p = str;
	
	while(*p)
	{
		utPtc(*p);
		p++;
	}
}

//------------------------------------------------------------------------------
char hex2cha(char hex)
{
	if(hex>9)
		return (char)(hex-10+'A');
	else
		return (char)(hex+'0');
}

//------------------------------------------------------------------------------
void utPrt(char *fmt, ...)
{
    char *s;
    INT16U d;
    char buf[5];
    va_list ap;
	
    va_start(ap, fmt); 
    while (*fmt) 
	{
		if (*fmt != '%') 
		{
			utPtc(*fmt++);
			continue;
		}
        switch (*++fmt) 
		{
            case 's':
                s = va_arg(ap, char*);
                for ( ; *s; s++) 
				{
                    utPtc(*s);
                }
                break;

			#if 0
            case 'c':
                s = va_arg(ap, INT8U*);
                utPtb(*s);
                break;
			#endif

			#if 1
            case 'c':
                d = va_arg(ap, int);
                utPtb(((INT8U)d));
                break;
			#endif
			
							
            case 'd':
                d = va_arg(ap, int);	//WORD
				if(d>99)
				{
					buf[4] = 0;
					buf[3] = '0'+d%10;
					d /= 10;
					buf[2] = '0'+d%10;
					d /= 10;
					buf[1] = '0'+d%10;
					d /= 10;
					buf[0] = '0'+d%10;
				}
				else
				{
					buf[2] = 0;
					buf[1] = '0'+d%10;
					d /= 10;
					buf[0] = '0'+d%10;
				}
					
                for (s = buf; *s; s++) 
				{
                    utPtc(*s);
                }
                break;

			case 'x':
				d = va_arg(ap, int);	//WORD
				if(d & 0xff00)
				{
					buf[4] = 0;
					buf[3] = hex2cha(d&0xf);
					d >>= 4;
					buf[2] = hex2cha(d&0xf);
					d >>= 4;
					buf[1] = hex2cha(d&0xf);
					d >>= 4;
					buf[0] = hex2cha(d&0xf);
				}
				else
				{
					buf[2] = 0;
					buf[1] = hex2cha(d&0xf);
					d >>= 4;
					buf[0] = hex2cha(d&0xf);
				}

                for (s = buf; *s; s++) 
				{
                    utPtc(*s);
                }				
				break;
				
            /* Add other specifiers here... */              
            default:  
                utPtc(*fmt);
                break;
        }
        fmt++;
    }
    va_end(ap);
}

void utPrtwrg(INT8U x)
{
	while(1)
	{	
		utPrt("wrg%s\n",x);
		utDelay1ms(50);	
	}
}

void utCalClk1(void)
{
	while (1)
	{
		//起始位
		while (!IS_TXF);	//同步
		TXF_OFF;			//清除标志位
		TXP_LOW;

		//起始位
		while (!IS_TXF);	//同步
		TXF_OFF;			//清除标志位
		TXP_HIGH;
	}
}

void utCalClk10(void)
{
	while(1)
	{
		utPtbx(0x00);			//如果带校验的话正好10位		
	}
}

void utTestPrt(void)
{
	while (1)
	{
		LAMP_TOG;
		utPtn("0123456789ABCDEF\n");
		utDelay1ms(200);		
	}
}
//##############################################################################


//==============================================================================
//内部调用函数

//==============================================================================
//end of the file


//------------------------------------------------------------------------------  




