//==============================================================================
//ͷ�ļ�
#ifndef _UT_PRT_H
#define _UT_PRT_H

//==============================================================================
//˵��:ռ��TIM2!
//==============================================================================

//==============================================================================
//����ȫ�ֶ�������ļ�
#include "utGlobals.h"

extern void (*utpPtb)(char c);		//ͨ�ã��ҽ�ָ������ĳ������ptb

#define sprt(format,  args...)	{sprintf(str, format, ##args);utPtn(str);}

void utPtbx(char c);		//����prt
void utPtb(char c);			//ͨ�ã�������#define utPtb	(*utpPtb)��Ч�ʸ�һЩ
void utPtc(char c);			//ͨ��
void utPts(char *str);		//ͨ��
void utPtn(char *str);		//ͨ��
void utPrt(char *fmt, ...);	//ͨ��
void utPrtwrg(INT8U x);

void utCalClk1(void);
void utCalClk10(void);
void utTestPrt(void);


//==============================================================================
//�ⲿ����Ԥ����

//==============================================================================
//�ⲿ���ñ���

//==============================================================================
//ʹ�õ��ⲿ����

//==============================================================================
//�ⲿ���ú���

#endif
//==============================================================================
//end of the file
