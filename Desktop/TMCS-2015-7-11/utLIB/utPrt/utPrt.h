//==============================================================================
//头文件
#ifndef _UT_PRT_H
#define _UT_PRT_H

//==============================================================================
//说明:占用TIM2!
//==============================================================================

//==============================================================================
//包含全局定义变量文件
#include "utGlobals.h"

extern void (*utpPtb)(char c);		//通用，挂接指针后就用某个具体ptb

#define sprt(format,  args...)	{sprintf(str, format, ##args);utPtn(str);}

void utPtbx(char c);		//软件prt
void utPtb(char c);			//通用，还可以#define utPtb	(*utpPtb)，效率高一些
void utPtc(char c);			//通用
void utPts(char *str);		//通用
void utPtn(char *str);		//通用
void utPrt(char *fmt, ...);	//通用
void utPrtwrg(INT8U x);

void utCalClk1(void);
void utCalClk10(void);
void utTestPrt(void);


//==============================================================================
//外部符号预定义

//==============================================================================
//外部调用变量

//==============================================================================
//使用的外部变量

//==============================================================================
//外部调用函数

#endif
//==============================================================================
//end of the file

