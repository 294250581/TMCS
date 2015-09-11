#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "utGlobals.h"
#include "stm32f10x.h"

#define SPEED_SAMPLING_TIME 9
extern int32_t hEncoder_Timer_Overflow;


void ENC_Init(void);
void TIM2_IRQHandler(void);
void ENC_Deal(void);
void ENC_Clean(void);

#endif


















