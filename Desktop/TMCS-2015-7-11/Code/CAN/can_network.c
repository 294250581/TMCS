#include "stm32f10x.h"
#include <stdio.h>
#include "can_network.h"

/* ����ȫ�ֱ��� */
CanTxMsg g_tCanTxMsg;	/* ���ڷ��� */
CanRxMsg g_tCanRxMsg;	/* ���ڽ��� */

/*
*********************************************************************************************************
*	�� �� ��: SendCanMsg
*	����˵��: ����һ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SendCanMsg(CanTxMsg *txmsg)
{
    CAN_Transmit(CAN1, txmsg);	
}

/*
*********************************************************************************************************
*	�� �� ��: can_Init
*	����˵��: ����CANӲ��
*	��    �Σ���
*	�� �� ֵ: ��
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

	GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);	// ʹ��CAN1����ӳ�� 
	
	/* ����CAN�źŽ�������: RX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//*GPIO����Ϊ��������ģʽ 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ����CAN�źŷ�������: TX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		// ����Ϊ����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	// ����GPIO����ٶ� 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	CAN_DeInit(CAN1);						// ��λCAN�Ĵ��� 
	CAN_StructInit(&CAN_InitStructure);		// ���CAN�����ṹ���ԱΪȱʡֵ 
	
	/*
		TTCM = time triggered communication mode
		ABOM = automatic bus-off management 
		AWUM = automatic wake-up mode
		NART = no automatic retransmission
		RFLM = receive FIFO locked mode 
		TXFP = transmit FIFO priority		
	*/
	CAN_InitStructure.CAN_TTCM = DISABLE;			// ��ֹʱ�䴥��ģʽ��������ʱ���), T  
	CAN_InitStructure.CAN_ABOM = DISABLE;			// ��ֹ�Զ����߹رչ��� 
	CAN_InitStructure.CAN_AWUM = DISABLE;			// ��ֹ�Զ�����ģʽ 
	CAN_InitStructure.CAN_NART = DISABLE;			// ��ֹ�ٲö�ʧ��������Զ��ش����� 
	CAN_InitStructure.CAN_RFLM = DISABLE;			// ��ֹ����FIFO����ģʽ 
	CAN_InitStructure.CAN_TXFP = DISABLE;			// ��ֹ����FIFO���ȼ� 

#if 1
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	// ����CANΪ��������ģʽ 
#else
	CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;	/* ����CANΪ���ع���ģʽ */
#endif
	
	/* 
		CAN ������ = RCC_APB1Periph_CAN / Prescaler / (SJW + BS1 + BS2);
		
		SJW = synchronisation_jump_width 
		BS = bit_segment
		
		�����У�����CAN������Ϊ500Kbps		
		CAN ������ = 360000000 / 3 / (1 + 6 + 5) / = 1MHz		
	*/
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
	CAN_InitStructure.CAN_Prescaler = 3;
	CAN_Init(CAN1, &CAN_InitStructure);

	// ����CAN�˲���0 
	CAN_FilterInitStructure.CAN_FilterNumber = 0;		                 // �˲�����ţ�0-13����14���˲��� 
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;		 // �˲���ģʽ������ID����ģʽ 
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	 // 32λ�˲� 
	CAN_FilterInitStructure.CAN_FilterIdHigh = can_parameter._t_std_id<<5;// �����ID�ĸ�16bit 
	CAN_FilterInitStructure.CAN_FilterIdLow =  CAN_ID_STD;               // �����ID�ĵ�16bit 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;				 // ID����ֵ��16bit 
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFF;				 // ID����ֵ��16bit 
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;		 // �˲�����FIFO 0 
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;				 // ʹ���˲��� 
	CAN_FilterInit(&CAN_FilterInitStructure);

	g_tCanTxMsg.StdId = can_parameter._t_std_id;
	g_tCanTxMsg.ExtId = 0x00;
	g_tCanTxMsg.IDE = CAN_ID_STD;
	g_tCanTxMsg.RTR = CAN_RTR_DATA;
	g_tCanTxMsg.DLC = 0;

	// ����CAN�ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// CAN FIFO0 ��Ϣ�����ж�ʹ�� 
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
*	�� �� ��: can_ISR
*	����˵��: CAN�жϷ������. ��������� stm32f10x_it.c�б�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void can_ISR(void)
{
	CAN_Receive(CAN1, CAN_FIFO0, &g_tCanRxMsg);
	can_state._t_can_receive = t_can_receive_true;
}

