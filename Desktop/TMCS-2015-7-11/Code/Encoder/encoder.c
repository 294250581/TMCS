#include "encoder.h"

#define ENCODER_TIMER TIM2
#define ENCODER_PPR	  (uint16_t)(2500)
#define SPEED_BUFFER_SIZE 8

#define COUNTER_RESET (uint16_t)0
#define ICx_FILTER    (uint8_t)6

#define TIMx_PRE_EMPTION_PRIORITY 1
#define TIMx_SUB_PRIORITY         0

#define SPEED_SAMPLING_FREQ (uint16_t)(2000/(SPEED_SAMPLING_TIME+1))

int32_t hEncoder_Timer_Overflow = 0;

void ENC_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef       TIM_ICInitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	/* Encoder A and B */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_DeInit(ENCODER_TIMER);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = 0x00;
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(ENCODER_TIMER, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(ENCODER_TIMER, TIM_EncoderMode_TI12,
		                       TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = ICx_FILTER;
	TIM_ICInit(ENCODER_TIMER, &TIM_ICInitStructure);

	TIM_ClearFlag(ENCODER_TIMER, TIM_FLAG_Update);
	TIM_ITConfig(ENCODER_TIMER, TIM_IT_Update, DISABLE);
	
	TIM2->CNT = 0;	

	TIM_Cmd(ENCODER_TIMER,ENABLE);
}

void ENC_Deal(void)
{
	int x;
	x = TIM2->CNT;
	utPrt("ENC = %d\n", x);
}

void ENC_Clean(void)
{
	TIM2->CNT = 0;
	utPrt("ENC clear = %d\n", TIM2->CNT);
}



