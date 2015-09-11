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
//STM32�ر���Ҫ
//�����ж�
//����ɾ���������Ҫһ��ͷ����
//##############################################################################
void    CPU_IntEn     (void);
void    CPU_IntDis    (void);
//##############################################################################


//##############################################################################
//����CPU���жϿ��ؼ����Ź�
//##############################################################################
#define utEnCpuInt		CPU_IntEn
#define utDisCpuInt		CPU_IntDis
#define KICK_AND_REOPEN_WDG	IWDG_ReloadCounter()
//##############################################################################

#if 0
//==============================================================================
//���prt����CPU�й�
//==============================================================================
#define IS_TXF		(PT_TIMER & TXF)
#define TXF_OFF 	(PT_TIMER &= ~TXF)

#define TXP_HIGH	(PT_PORT |= TXP)
#define TXP_LOW		(PT_PORT &= ~TXP)
#define TXP_TOG		(PT_PORT ^= TXP)
//==============================================================================


//==============================================================================
//ϵͳ����ָʾ��
//==============================================================================
//��������
#define LRED_ON		(LP_PORT &= ~LRED)
#define LGRN_ON		(LP_PORT &= ~LGRN)
#define LYLW_ON		(LP_PORT &= ~(LRED+LGRN))
#define LRED_OFF	(LP_PORT |= LRED)
#define LGRN_OFF	(LP_PORT |= LGRN)
#define LYLW_OFF	(LP_PORT |= (LRED+LGRN))
#define LRED_CPL	(LP_PORT ^= LRED)
#define LGRN_CPL	(LP_PORT ^= LGRN)
#define LYLW_CPL	(LP_PORT ^= (LRED+LGRN))

//��˸
#define LRED_TOG	{LGRN_OFF;LRED_CPL;}
#define LGRN_TOG	{LRED_OFF;LGRN_CPL;}
#define LYLW_TOG	{if((LP_PORT & LGRN) != (LP_PORT & LRED)) LAMP_OFF; LYLW_CPL;}		//�ȹص�����ͬ����ƽ
#define LREDGRN_TOG	{if((LP_PORT & LGRN) == (LP_PORT & LRED)) LRED_CPL; LYLW_CPL;}		//һ����ȡ��һ��

#if 0
//##############################################################################
#define LAMP_ON		(LRED_ON)
#define LAMP_OFF	(LRED_OFF)
#define LAMP_TOG	(LRED_CPL)	//(LRED_TOG) //ע�⣬����ֻ����CPL��������TOG!
//##############################################################################
//==============================================================================
#endif
#endif

#define BELL_ON			(BLPORT |= BELL)
#define BELL_OFF		(BLPORT &= ~BELL)
#define BELL_TOG		(BLPORT ^= BELL)

#define FILT_ON			(FT_PORT |= FLIT)
#define FILT_OFF		(FT_PORT &= ~FLIT)


//������ʱ����Ӳ���й�
//void Delay_ns(void);
//#define Delay_ns()		cntDly++
//��Ƕ���
#define Delay_20ns    {__nop();}
#define Delay_40ns    {__nop();__nop();}
#define Delay_60ns    {__nop();__nop();__nop();}
#define Delay_80ns    {__nop();__nop();__nop();__nop();}
#define Delay_100ns   {__nop();__nop();__nop();__nop();__nop();}


//------------------------------------------------------------------------------
void utInitCpuClk(void);		//����ʹϵͳʱ����������
void utInitPrt(void); 			//��ʼ��prt
void utInitOsalClk(void);		//��ʼutOSʱ��
void utDelay1us(INT8U x);
void utDelay1ms(INT8U x);
void utDelay1s(INT8U x);
void utPtb0(char c); 			//����0���һ���ֽ�
void utPtb1(char c); 			//����1���һ���ֽ�
void utPtb2(char c); 			//����2���һ���ֽ�
void utPtc1(char c);			//����1���һ���ַ�(���س��Զ����ϻ���)
void pts1(char *str);			//����1���һ���ַ���
void ptn1(char *str);			//����1���һ���ַ���(���س��Զ����ϻ���)
void utInitInt(void);
void utInitWatchDog(void);
#endif
