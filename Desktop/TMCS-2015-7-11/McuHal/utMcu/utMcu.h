#ifndef _utMCU_H
#define _utMCU_H

#include "utMcuCfg.h"
#include "utEvtProc.h"

extern volatile uint32_t *GPIOX_LP;
extern INT16U GPIOX_LPIN;

extern volatile uint32_t *GPIOX_PT;
extern INT16U GPIOX_PPIN;

#if 0
extern GPIO_TypeDef *GPIOX_LP;
extern INT16U GPIOX_LPIN;

extern GPIO_TypeDef *GPIOX_PT;
extern INT16U GPIOX_PPIN;

#define __INLINE	__inline

static __INLINE void LAMP_ON1()
{
	GPIOX_LP->ODR &= ~GPIOX_LPIN;
}

static __INLINE void LAMP_OFF1()
{
	GPIOX_LP->ODR |= GPIOX_LPIN;
}

static __INLINE void LAMP_TOG1()
{
	GPIOX_LP->ODR ^= GPIOX_LPIN;
}

static __INLINE void PTPIN_LOW()
{
	GPIOX_PT->ODR &= ~GPIOX_PPIN;
}

static __INLINE void PTPIN_HIGH()
{
	GPIOX_PT->ODR |= GPIOX_PPIN;
}

static __INLINE void PTPIN_TOG()
{
	GPIOX_PT->ODR ^= GPIOX_PPIN;
}

#endif

//extern INT32U *ADDRX;

#if 0
#define LAMP_ON		LAMP_ON1()
#define LAMP_OFF	LAMP_OFF1()
#define LAMP_TOG	LAMP_TOG1()
#else
#define LAMP_ON		((*GPIOX_LP) |= GPIOX_LPIN)
#define LAMP_OFF	((*GPIOX_LP) &= ~GPIOX_LPIN)
#define LAMP_TOG	((*GPIOX_LP) ^= GPIOX_LPIN)
#endif

#if 0
#define TXP_HIGH	PTPIN_HIGH()
#define TXP_LOW		PTPIN_LOW()
#define TXP_TOG		PTPIN_TOG()
#else
#define TXP_HIGH	((*GPIOX_PT) |= GPIOX_PPIN)		//((GPIOC->ODR) |= BIT12)	//
#define TXP_LOW		((*GPIOX_PT) &= ~GPIOX_PPIN)	//((GPIOC->ODR) &= ~BIT12)	//
#define TXP_TOG		((*GPIOX_PT) ^= GPIOX_PPIN)
#endif


//##############################################################################
//STM32特别需要
//开关中断
//不能删除，汇编需要一个头函数
//##############################################################################
void    CPU_IntEn     (void);
void    CPU_IntDis    (void);
//##############################################################################


//##############################################################################
//定义CPU的中断开关及看门狗
//##############################################################################
#define utEnCpuInt		CPU_IntEn
#define utDisCpuInt		CPU_IntDis
#define KICK_AND_REOPEN_WDG	IWDG_ReloadCounter()
//##############################################################################

#if 0
//==============================================================================
//软件prt，跟CPU有关
//==============================================================================
#define IS_TXF		(PT_TIMER & TXF)
#define TXF_OFF 	(PT_TIMER &= ~TXF)

#define TXP_HIGH	(PT_PORT |= TXP)
#define TXP_LOW		(PT_PORT &= ~TXP)
#define TXP_TOG		(PT_PORT ^= TXP)
//==============================================================================


//==============================================================================
//系统工作指示灯
//==============================================================================
//基本操作
#define LRED_ON		(LP_PORT &= ~LRED)
#define LGRN_ON		(LP_PORT &= ~LGRN)
#define LYLW_ON		(LP_PORT &= ~(LRED+LGRN))
#define LRED_OFF	(LP_PORT |= LRED)
#define LGRN_OFF	(LP_PORT |= LGRN)
#define LYLW_OFF	(LP_PORT |= (LRED+LGRN))
#define LRED_CPL	(LP_PORT ^= LRED)
#define LGRN_CPL	(LP_PORT ^= LGRN)
#define LYLW_CPL	(LP_PORT ^= (LRED+LGRN))

//闪烁
#define LRED_TOG	{LGRN_OFF;LRED_CPL;}
#define LGRN_TOG	{LRED_OFF;LGRN_CPL;}
#define LYLW_TOG	{if((LP_PORT & LGRN) != (LP_PORT & LRED)) LAMP_OFF; LYLW_CPL;}		//先关掉处于同样电平
#define LREDGRN_TOG	{if((LP_PORT & LGRN) == (LP_PORT & LRED)) LRED_CPL; LYLW_CPL;}		//一样就取反一个

#if 0
//##############################################################################
#define LAMP_ON		(LRED_ON)
#define LAMP_OFF	(LRED_OFF)
#define LAMP_TOG	(LRED_CPL)	//(LRED_TOG) //注意，单灯只能用CPL，不能用TOG!
//##############################################################################
//==============================================================================
#endif
#endif

#define BELL_ON			(BLPORT |= BELL)
#define BELL_OFF		(BLPORT &= ~BELL)
#define BELL_TOG		(BLPORT ^= BELL)

#define FILT_ON			(FT_PORT |= FLIT)
#define FILT_OFF		(FT_PORT &= ~FLIT)


//定义延时，跟硬件有关
//void Delay_ns(void);
//#define Delay_ns()		cntDly++
//内嵌汇编
#define Delay_20ns    {__nop();}
#define Delay_40ns    {__nop();__nop();}
#define Delay_60ns    {__nop();__nop();__nop();}
#define Delay_80ns    {__nop();__nop();__nop();__nop();}
#define Delay_100ns   {__nop();__nop();__nop();__nop();__nop();}


//------------------------------------------------------------------------------
void utInitCpuClk(void);		//首先使系统时钟正常工作
void utInitPrt(void); 			//初始化prt
void utInitOsalClk(void);		//初始utOS时钟
void utDelay1us(INT8U x);
void utDelay1ms(INT8U x);
void utDelay1s(INT8U x);
void utPtb0(char c); 			//串口0输出一个字节
void utPtb1(char c); 			//串口1输出一个字节
void utPtb2(char c); 			//串口2输出一个字节
void utPtc1(char c);			//串口1输出一个字符(遇回车自动加上换行)
void pts1(char *str);			//串口1输出一个字符串
void ptn1(char *str);			//串口1输出一个字符串(遇回车自动加上换行)
void utInitInt(void);
void utInitWatchDog(void);
#endif
