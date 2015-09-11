//==============================================================================
//C�ļ�
//��Ӳ���޹صĳ��ÿ⺯��
#include "utPrt.h"

//==============================================================================
//�ڲ�����Ԥ����
#define PT_TIMER	(TIM3->SR)
#define TXF			TIM_IT_Update	//��������prt��ʱ�������־λ

#define IS_TXF		(PT_TIMER & TXF)
#define TXF_OFF 	(PT_TIMER &= ~TXF)

//==============================================================================
//�ڲ����ñ���

//==============================================================================
//�ڲ����ú�������
static char hex2cha(char hex);

//==============================================================================
//�ⲿ���ú���
void (*utpPtb)(char c);		//ͨ�ã��ҽ�ָ������ĳ������ptb


//##############################################################################
//prtģ��
//##############################################################################
//------------------------------------------------------------------------------
void utPtbx(char c) 			//���һ���ֽ�,����ͨ��
{	
	INT8U i;

	utEnterCrt;
	
	while (!IS_TXF);		//ͬ��	
	TXF_OFF; 				//�����ǰ�ı�־λ
	//(1)���ٶ���һ��bit,�������죬��Ҫ�ǵȴ����һλ�����⣬����ʱһλҲ�����

	//��ʼλ
	while (!IS_TXF);		//ͬ��
	TXP_LOW;
	TXF_OFF; 				//�����־λ

	//8λ����
	for (i = 0; i < 8; i++)
	{
		while (!IS_TXF);	//ͬ��

		#if 1
		if (c & BIT0)		//�ȴ���λ
			TXP_HIGH; 		//���ܼӷֺ�
		else
			TXP_LOW;		//���ܼӷֺ�
		#endif
		
		c >>= 1;			//����һλ

		TXF_OFF; 			//�����־λ
	}

	//��ֹλ
	while (!IS_TXF);
	TXP_HIGH; 				//����λ
	TXF_OFF; 				//�����־λ

	//���(1)�����ȴ����˴���Ҫ�ȴ�

	utExitCrt;
}

//------------------------------------------------------------------------------
void utPtb(char c)			//���һ���ַ�
{
	(*utpPtb)(c);
}


//------------------------------------------------------------------------------
void utPtc(char c)			//���һ���ַ�
{
	if(c == '\n')			//�����'n'�ȼ�һ��'r'
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
		//��ʼλ
		while (!IS_TXF);	//ͬ��
		TXF_OFF;			//�����־λ
		TXP_LOW;

		//��ʼλ
		while (!IS_TXF);	//ͬ��
		TXF_OFF;			//�����־λ
		TXP_HIGH;
	}
}

void utCalClk10(void)
{
	while(1)
	{
		utPtbx(0x00);			//�����У��Ļ�����10λ		
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
//�ڲ����ú���

//==============================================================================
//end of the file


//------------------------------------------------------------------------------  




