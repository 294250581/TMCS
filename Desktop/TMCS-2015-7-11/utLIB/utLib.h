#ifndef _utLIB_H
#define _utLIB_H

#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "math.h"
#include "utLibCfg.h"


//##############################################################################
//����PRT��ʽ
//##############################################################################
#define PRT_SOFT	0
#define PRT_UART0	1
#define PRT_UART1	2
#define PRT_UART2	3
//##############################################################################


//##############################################################################
//����ģ��
//##############################################################################
typedef unsigned char  BOOLEAN; 		/*  ��������                    */
typedef unsigned char  INT8U;           /*  �޷���8λ���ͱ���           */
typedef signed   char  INT8S;           /*  �з���8λ���ͱ���           */
typedef unsigned short INT16U;          /*  �޷���16λ���ͱ���          */
typedef signed   short INT16S;          /*  �з���16λ���ͱ���          */
typedef unsigned long  INT32U;          /*  �޷���32λ���ͱ���          */
typedef volatile unsigned long  INT32UV;          /*  ��������Ҫ�Ż����޷���32λ���ͱ���          *///
typedef signed   long  INT32S;          /*  �з���32λ���ͱ���          */
typedef float          FP32;            /*  �����ȸ�������32λ���ȣ�    */
typedef double         FP64;            /*  ˫���ȸ�������64λ���ȣ�    */
//##############################################################################


//##############################################################################
//����һЩ���ú�
//##############################################################################
#define false   0
#define true    !false

#define UT_OK 		0
#define UT_ERR		1

//һ���ֵĸߵ��ֽڽ���
#define SWAP(x)   ( (((INT16U)(x)) << 8) | (((INT16U)(x)) >> 8) )

#define HWD(x)    ( (INT16U)(((INT32U)(x)) >> 16) )	//HIGH WORD
#define LWD(x)    ( (INT16U)(x) )

#define HBT(x)    ( (INT8U)(((INT16U)(x)) >> 8) )					//HIGH BYTE
#define LBT(x)    ( (INT8U)(x) )
	
#define	HHB(x)	  ( ((INT8U)(x)) >> 4 )								//HIGH HALF BYTE
#define	LHB(x)	  ( ((INT8U)(x)) & 0x0F )		
//##############################################################################



//##############################################################################
//λ����
//##############################################################################
#define BIT0                (0x01)
#define BIT1                (0x02)
#define BIT2                (0x04)
#define BIT3                (0x08)
#define BIT4                (0x10)
#define BIT5                (0x20)
#define BIT6                (0x40)
#define BIT7                (0x80)
#define BIT8                (0x0100)
#define BIT9                (0x0200)
#define BIT10               (0x0400)
#define BIT11               (0x0800)
#define BIT12               (0x1000)
#define BIT13               (0x2000)
#define BIT14               (0x4000)
#define BIT15               (0x8000)
#define BIT16				(0x00010000)
#define BIT17               (0x00020000)
#define BIT18               (0x00040000)
#define BIT19               (0x00080000)
#define BIT20               (0x00100000)
#define BIT21               (0x00200000)
#define BIT22               (0x00400000)
#define BIT23               (0x00800000)
#define BIT24               (0x01000000)
#define BIT25               (0x02000000)
#define BIT26               (0x04000000)
#define BIT27               (0x08000000)
#define BIT28               (0x10000000)
#define BIT29               (0x20000000)
#define BIT30               (0x40000000)
#define BIT31               (0x80000000)
//##############################################################################


//##############################################################################
//����1ms,10ms,100ms,1s֮������Ҫ�Ķ�ʱ��
//������������ʱ(cnt)��ֹ
//##############################################################################
typedef struct
{
	BIT_OF_CPU	cnt;
	BIT_OF_CPU	en: 1;
}TIMER_CNT;				//��ʱ���ṹ

void utInitTmr(void);	//��ʱ����ʼ��	
#define utStTmr(x)		{x.cnt=0; x.en = 1;}
#define utEdTmr(x)		{x.en=0;}
#define utIsAlvTmr(x)	(x.en)		//��ʱ�����ڼ���̬
#define utIsAlmTmr(x,y)	((++(x.cnt))==y)
#define utExecTmr(tmr,tout,proc)	\
if(utIsAlvTmr(tmr))					\
{									\
	if(utIsAlmTmr(tmr,tout))		\
	{								\
		utEdTmr (tmr);				\
		proc;						\
	}								\
}							
//##############################################################################
#endif
