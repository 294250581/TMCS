#ifndef _OSAL_H
#define _OSAL_H

#include "utGlobals.h"

#define EVENT 	BIT_OF_CPU
#define UT_FLAG	BIT_OF_CPU

extern BIT_OF_CPU stInt;
extern BIT_OF_CPU ut1ms;
#define utEnInt		{utEnCpuInt();stInt=1;}		//使能中断,别忘了stInt要记录!!!
#define utDisInt	{utDisCpuInt();stInt=0;} 	//禁止中断,别忘了stInt要记录!!!
#define utEnterCrt	{utDisCpuInt();}			//进入临界区
#define utExitCrt	{if(stInt) utEnCpuInt();}	//退出临界区，根据先前情况决定是否要打开中断

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




void utInitSys(void);			//使整个os系统初始化
void utStartOS(void);			//开始将CPU控制权交给操作系统，不会再返回
void utOsalTimerUpdate(void);	//需要在严格的1ms定时器中断中调用

void ut1msProc(void);			//钩子函数
void ut10msProc(void);			//钩子函数
void ut100msProc(void);			//钩子函数
void ut1sProc(void);			//钩子函数

//各类输入输出事件的设置
void utSetInputEvt(EVENT x);
void utClrInputEvt(EVENT x);	
BOOLEAN utIsInputEvt(EVENT x);
void utSetOutputEvt(EVENT x);
void utClrOutputEvt(EVENT x);	
BOOLEAN utIsOutputEvt(EVENT x);

//各类标志的设置，取消，判断
void utSetFlag(UT_FLAG x);
void utClrFlag(UT_FLAG x);	
BOOLEAN utIsFlag(UT_FLAG x);
INT8U utSleep(INT32U time);

#endif
