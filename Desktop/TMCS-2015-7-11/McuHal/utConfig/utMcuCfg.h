#ifndef _utMCUCFG_H
#define _utMCUCFG_H

#include "utGlobals.h"
//#include "stm32f10x.h"
#include "stm32f10x.h"

//##############################################################################
//�������prt�Ĺܽ�
#if 1	//�����
#define PT_PORT		GPIOA
#define PT_PIN 		BIT8
#else	//PingBan
#define PT_PORT		GPIOD
#define PT_PIN 		BIT2
#endif
//##############################################################################


//##############################################################################
//��������ָʾ������LAMP
//##############################################################################
#define LP_PORT		GPIOC
#define LP_PIN		BIT15

#if 0
//#define LP_DIR		P2DIR
#define LRED		BIT9
#define LGRN		BIT4
#endif
//##############################################################################

#define BLPORT			(GPIOB->ODR)
#define BELL			BIT8

#define FT_PORT			(GPIOB->ODR)
#define FLIT			BIT2

#endif
