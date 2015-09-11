#include "utMCU.h"
#include "stm32f10x_iwdg.h"

GPIO_TypeDef *GPIOX; 

volatile uint32_t *GPIOX_LP;
INT16U GPIOX_LPIN;

volatile uint32_t *GPIOX_PT;
INT16U GPIOX_PPIN;

//------------------------------------------------------------------------------
//校正时钟
void utDelay1us(INT8U x)
{
	INT8U i;
	
	while(x--)
	{
		i = 6;
		while(i--);
	}
}

//------------------------------------------------------------------------------
void utDelay1ms(INT8U x)
{
	INT8U i;
	
	while(x--)
	{
		i = 20;
		while(i--)
			utDelay1us(50);
	}
}

//------------------------------------------------------------------------------
void utDelay1s(INT8U x)
{	
	while(x--)
	{
		utDelay1ms(250);
		utDelay1ms(250);
		utDelay1ms(250);
		utDelay1ms(250);
	}
}

//------------------------------------------------------------------------------
//(1)首先初始化CPU时钟
void utInitLamp(void);		//放在初始化cpu后
void utInitCpuClk(void)
{ 
	ErrorStatus HSEStartUpStatus;

	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	#if 1
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON); 

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if(HSEStartUpStatus == SUCCESS)
	#endif  	
	
	{
	/* Enable Prefetch Buffer */
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

	/* Flash 2 wait state */
	FLASH_SetLatency(FLASH_Latency_2);
		
	/* HCLK = SYSCLK */
	RCC_HCLKConfig(RCC_SYSCLK_Div1); 

	/* PCLK2 = HCLK */
	RCC_PCLK2Config(RCC_HCLK_Div1); 

	/* PCLK1 = HCLK/2 */
	RCC_PCLK1Config(RCC_HCLK_Div2);

	//PCLK2 1/4, ADC时钟不能超过14M，每次转换时间不高于1us
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   				

	#if 1
	/* PLLCLK = 8MHz * 9 = 72 MHz */
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	#else
	//内部高速晶振,4M*16=64,最大只能64M
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
	#endif

	/* Enable PLL */ 
	RCC_PLLCmd(ENABLE);

	//等待PLL时钟稳定
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

	//选择时钟源为PLL
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	//等待时钟源稳定
	while(RCC_GetSYSCLKSource() != 0x08)
	{
	}

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	}
	
	/* 呼吸灯配置 */
	utInitLamp();
}

//------------------------------------------------------------------------------
//(2)初始化系统指示灯，并校正Delay
void utInitLamp()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	#if 0
	GPIOX = GPIOD;
	GPIOX_LPIN = BIT1;
	#else
	GPIOX = LP_PORT;
	GPIOX_LPIN = LP_PIN;	
	#endif
	
	GPIOX_LP = &(GPIOX->ODR);
	GPIO_InitStructure.GPIO_Pin =	GPIOX_LPIN;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_Init(GPIOX, &GPIO_InitStructure);	
}

//------------------------------------------------------------------------------
//(3)其次初始化prt时钟，包括选择哪种prt方式
//------------------------------------------------------------------------------
void utInitPrt(void) 		//使用定时器2作为prt时钟
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	TIM_TimeBaseStructure.TIM_Period = 156;	
	TIM_TimeBaseStructure.TIM_Prescaler = 0x00; 	  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	GPIOX = PT_PORT;
	GPIOX_PPIN = PT_PIN;
	
	utpPtb = utPtbx;	
	GPIOX_PT = &(GPIOX->ODR);
	GPIO_SetBits(GPIOX,GPIOX_PPIN);	 	
	GPIO_InitStructure.GPIO_Pin =	GPIOX_PPIN;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_Init(GPIOX, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOX,GPIOX_PPIN);	
}


//------------------------------------------------------------------------------
//(4)初始化1ms发生一次的系统时钟
//中断中占用了几个周期，TMR1不能自动装载
//#define TMR_BASE ((INT16U)(-1995)) 
void utInitOsalClk(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick->LOAD = 9000;
	SysTick->VAL = (u32)0x00000000;
	SysTick->CTRL |= (u32)0x00000001;
	SysTick->CTRL |= (u32)0x00000002;
}

//------------------------------------------------------------------------------
//(5)初始化看门狗
void utInitWatchDog(void)
{
	/* IWDG timeout equal to 350ms (the timeout may varies due to LSI frequency
	   dispersion) -------------------------------------------------------------*/
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* IWDG counter clock: 32KHz(LSI) / 32 = 1KHz */
	//使用独立的内部时钟
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	
	/* Set counter reload value to 200 */
	//200ms中断一次
	IWDG_SetReload(200);
	
	/* Reload IWDG counter */
	//重新装载看门狗
	IWDG_ReloadCounter();
	
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}

//------------------------------------------------------------------------------
//osal中断，定时器0用中断1
//在SysTickHandler()中更新时钟

