#ifndef _CAN_NETWORK_H
#define _CAN_NETWORK_H
#include "stm32f10x_can.h"
#include "Can_configure.h"
#include "Can_receive.h"

extern CanTxMsg g_tCanTxMsg;	/* ���ڷ��� */
extern CanRxMsg g_tCanRxMsg;	/* ���ڽ��� */

void SendCanMsg(CanTxMsg *txmsg);
void can_Init(void);
void can_ISR(void);
void can_set_baudrate(u16 baudrate);

#endif


