/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "utGlobals.h"
#include "Can_network.h"
#include "Iobus.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if 0
void PendSV_Handler(void)
{
}
#endif
/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if 1
void SysTick_Handler(void)
{
	utOsalTimerUpdate();
}
#endif
/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

#ifndef STM32F10X_CL
/*******************************************************************************
* Function Name  : USB_HP_CAN1_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts requests
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	can_ISR();
}
#endif /* STM32F10X_CL */

#if defined(STM32F10X_HD) || defined(STM32F10X_XL) 
/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles SDIO global interrupt request.
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SDIO_IRQHandler(void)
{ 
}
#endif /* STM32F10X_HD | STM32F10X_XL*/

#ifdef STM32F10X_CL
/*******************************************************************************
* Function Name  : OTG_FS_IRQHandler
* Description    : This function handles USB-On-The-Go FS global interrupt request.
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void OTG_FS_IRQHandler(void)
{

}
#endif /* STM32F10X_CL */

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/*******************************************************************************
* Function Name  : PPP_IRQHandler
* Description    : This function handles PPP interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*void PPP_IRQHandler(void)
{
}*/
void EXTI0_IRQHandler(void)
{
	if( EXTI_GetITStatus(EXTI_Line0) != RESET )
	{
		utPrt("MotorX Flag\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI1_IRQHandler(void)
{
	if( EXTI_GetITStatus(EXTI_Line1) != RESET )
	{
		utPrt("MotorY Flag\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}

void EXTI2_IRQHandler(void)
{	
	if( EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		utPrt("MotorZ Flag\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line3) != RESET)
	{
		if(IOBUSA_TCB[SUB_A0].io_int == INT_ON)
			utPrt("IOBUSA_A0 INT\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line4) != RESET)
	{
		if(IOBUSB_TCB[SUB_A2].io_int == INT_ON)
			utPrt("IOBUSB_A2 INT\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line6) != RESET)
	{
		if(IOBUSB_TCB[SUB_A0].io_int == INT_ON)
			utPrt("IOBUSB_A0 INT\n");
	}

	if(EXTI_GetFlagStatus(EXTI_Line5) != RESET)
	{
		if(IOBUSB_TCB[SUB_A1].io_int == INT_ON)
			utPrt("IOBUSB_A1 INT\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line5);
	EXTI_ClearITPendingBit(EXTI_Line6);
}

void EXTI15_10_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line14);
}
