#include "utGlobals.h"
#include "Encoder.h"
#include "dSPIN.h"
#include "motor.h"
#include "Can_network.h"
#include "Flash.h"
#include "Iobus.h"

#define motor_device
#define can_device

void xxxx(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

int main(void)
{
	utDisInt;
	utInitInt();	
	utInitSys();
	utEnInt;
	/* Can bus init */
	can_Init();
	/* iobus init */
	iobus_init();

	/* 步进电机初始化 */
#if defined(motor_device)

	/* L6470端口及SPI初始化 */
	dSPIN_Peripherals_Init();
	
	/* L6470复位 */
	dSPIN_Reset_And_Standby(dSPIN_Device_All);

	/* L6470中断初始化 */
	dSPIN_Interrupt_Channel_Config();

	/* 电机参数初始化 */
	motor_init_set();
	
	ENC_Init();
	
#endif

	utPrt("********************************\r\n");
	utPrt("CPU: STM32F103RCT6 at 72 MHz	\r\n");
	utPrt("Board: TMCS    Board (Version2) \r\n");
	utPrt("Editor: Tsui					\r\n");
	utPrt("Compant: TMCS					\r\n");
	utPrt("Shell: Shell Version1			\r\n");
	utPrt("Compile Date: %s	   \r\n",__DATE__);
	utPrt("Compile TIME: %s	   \r\n",__TIME__);
	utPrt("********************************\r\n");

	/* set can baud rate at 250Kbit */
	can_set_baudrate(can_parameter._t_bit_rate);

#if 0
	g_tCanTxMsg.DLC 	= 3;
	g_tCanTxMsg.Data[0] = 0x11;
	g_tCanTxMsg.Data[1] = 0x22;
	g_tCanTxMsg.Data[2] = 0x33;
	SendCanMsg(&g_tCanTxMsg); 
#endif

	DetectCal();
	iobus_reload();

	utDelay1s(1);
	ENC_Clean();

	/* Start os */
	utPrt("Start utOs......\n");

	utStartOS();	
}
