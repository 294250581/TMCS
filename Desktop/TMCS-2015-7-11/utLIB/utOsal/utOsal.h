#ifndef _OSAL_H
#define _OSAL_H

#include "utGlobals.h"

#define EVENT 	BIT_OF_CPU
#define UT_FLAG	BIT_OF_CPU

extern BIT_OF_CPU stInt;
extern BIT_OF_CPU ut1ms;
#define utEnInt		{utEnCpuInt();stInt=1;}		//ʹ���ж�,������stIntҪ��¼!!!
#define utDisInt	{utDisCpuInt();stInt=0;} 	//��ֹ�ж�,������stIntҪ��¼!!!
#define utEnterCrt	{utDisCpuInt();}			//�����ٽ���
#define utExitCrt	{if(stInt) utEnCpuInt();}	//�˳��ٽ�����������ǰ��������Ƿ�Ҫ���ж�

/*******************************/
#define STLINE	((INT16U)(__LINE__))

#define	utBeginTask				\
	static INT16U st=0; 		\
								\
	switch(st)					\
	{							\
		case 0: 				

#define utWait(proc)			\
			st = STLINE;		\
								\
		case STLINE: 			\
			if(!(proc)) 		\
			{					\
				return false;	\
			}			

#define utEndTask				\
			st = 0;				\
			return true;		\
	}							\
	return false;	
/*******************************/




void utInitSys(void);			//ʹ����osϵͳ��ʼ��
void utStartOS(void);			//��ʼ��CPU����Ȩ��������ϵͳ�������ٷ���
void utOsalTimerUpdate(void);	//��Ҫ���ϸ��1ms��ʱ���ж��е���

void ut1msProc(void);			//���Ӻ���
void ut10msProc(void);			//���Ӻ���
void ut100msProc(void);			//���Ӻ���
void ut1sProc(void);			//���Ӻ���

//������������¼�������
void utSetInputEvt(EVENT x);
void utClrInputEvt(EVENT x);	
BOOLEAN utIsInputEvt(EVENT x);
void utSetOutputEvt(EVENT x);
void utClrOutputEvt(EVENT x);	
BOOLEAN utIsOutputEvt(EVENT x);

//�����־�����ã�ȡ�����ж�
void utSetFlag(UT_FLAG x);
void utClrFlag(UT_FLAG x);	
BOOLEAN utIsFlag(UT_FLAG x);
INT8U utSleep(INT32U time);

#endif
