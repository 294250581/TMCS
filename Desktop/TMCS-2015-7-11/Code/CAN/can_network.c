#include "stm32f10x.h"
#include <stdio.h>
#include "can_network.h"

/* 定义全局变量 */
CanTxMsg g_tCanTxMsg;	/* 用于发送 */
CanRxMsg g_tCanRxMsg;	/* 用于接收 */

/*
*********************************************************************************************************
*	函 数 名: SendCanMsg
*	功能说明: 发送一包数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SendCanMsg(CanTxMsg *txmsg)
{
    CAN_Transmit(CAN1, txmsg);	
}

/*
*********************************************************************************************************
*	函 数 名: can_Init
*	功能说明: 配置CAN硬件
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
CAN_InitTypeDef CAN_InitStructure;

void can_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);	// 使能CAN1的重映射 
	
	/* 配置CAN信号接收引脚: RX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//*GPIO配置为上拉输入模式 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* 配置CAN信号发送引脚: TX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		// 配置为复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	// 设置GPIO最大速度 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	CAN_DeInit(CAN1);						// 复位CAN寄存器 
	CAN_StructInit(&CAN_InitStructure);		// 填充CAN参数结构体成员为缺省值 
	
	/*
		TTCM = time triggered communication mode
		ABOM = automatic bus-off management 
		AWUM = automatic wake-up mode
		NART = no automatic retransmission
		RFLM = receive FIFO locked mode 
		TXFP = transmit FIFO priority		
	*/
	CAN_InitStructure.CAN_TTCM = DISABLE;			// 禁止时间触发模式（不生成时间戳), T  
	CAN_InitStructure.CAN_ABOM = DISABLE;			// 禁止自动总线关闭管理 
	CAN_InitStructure.CAN_AWUM = DISABLE;			// 禁止自动唤醒模式 
	CAN_InitStructure.CAN_NART = DISABLE;			// 禁止仲裁丢失或出错后的自动重传功能 
	CAN_InitStructure.CAN_RFLM = DISABLE;			// 禁止接收FIFO加锁模式 
	CAN_InitStructure.CAN_TXFP = DISABLE;			// 禁止传输FIFO优先级 

#if 1
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	// 设置CAN为正常工作模式 
#else
	CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;	/* 设置CAN为环回工作模式 */
#endif
	
	/* 
		CAN 波特率 = RCC_APB1Periph_CAN / Prescaler / (SJW + BS1 + BS2);
		
		SJW = synchronisation_jump_width 
		BS = bit_segment
		
		本例中，设置CAN波特率为500Kbps		
		CAN 波特率 = 360000000 / 3 / (1 + 6 + 5) / = 1MHz		
	*/
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
	CAN_InitStructure.CAN_Prescaler = 3;
	CAN_Init(CAN1, &CAN_InitStructure);

	// 设置CAN滤波器0 
	CAN_FilterInitStructure.CAN_FilterNumber = 0;		                 // 滤波器序号，0-13，共14个滤波器 
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;		 // 滤波器模式，设置ID掩码模式 
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	 // 32位滤波 
	CAN_FilterInitStructure.CAN_FilterIdHigh = can_parameter._t_std_id<<5;// 掩码后ID的高16bit 
	CAN_FilterInitStructure.CAN_FilterIdLow =  CAN_ID_STD;               // 掩码后ID的低16bit 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;				 // ID掩码值高16bit 
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFF;				 // ID掩码值低16bit 
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;		 // 滤波器绑定FIFO 0 
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;				 // 使能滤波器 
	CAN_FilterInit(&CAN_FilterInitStructure);

	g_tCanTxMsg.StdId = can_parameter._t_std_id;
	g_tCanTxMsg.ExtId = 0x00;
	g_tCanTxMsg.IDE = CAN_ID_STD;
	g_tCanTxMsg.RTR = CAN_RTR_DATA;
	g_tCanTxMsg.DLC = 0;

	// 配置CAN中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// CAN FIFO0 消息接收中断使能 
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}  

void can_set_baudrate(u16 baudrate)
{
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

	switch(baudrate)
	{
		case t_bit_1Mbps:
			CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
			CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
			CAN_InitStructure.CAN_Prescaler = 3;
		break;

		case t_bit_800kbps:
			CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
			CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
			CAN_InitStructure.CAN_Prescaler = 3;
		break;

		case t_bit_500kbps:
			CAN_InitStructure.CAN_BS1 = CAN_BS1_15tq;
			CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
			CAN_InitStructure.CAN_Prescaler = 3;
		break;

		case t_bit_250kbps:
			CAN_InitStructure.CAN_BS1 = CAN_BS1_15tq;
			CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
			CAN_InitStructure.CAN_Prescaler = 6;
		break;
	}
	CAN_Init(CAN1, &CAN_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: can_ISR
*	功能说明: CAN中断服务程序. 这个函数在 stm32f10x_it.c中被调用
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/ 
void can_ISR(void)
{
	CAN_Receive(CAN1, CAN_FIFO0, &g_tCanRxMsg);
	can_state._t_can_receive = t_can_receive_true;
}

