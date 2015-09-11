#ifndef __IOBUS_H__
#define __IOBUS_H__
#include "utGlobals.h"

#define BUSn 2
#define SUBn 4

typedef struct _io_tcb
{
	INT8U io_mode;    /* input or output */
	INT8U io_int;     /* interrupt on or off */
	INT8U io_intmode; /* int mode <Rising Falling Randf> */
}IO_TCBTypedef;

typedef enum
{
	TCB_IO_MODE     = 0,
	TCB_IO_INT      = 1,
	TCB_IO_INTMODE  = 2,
	TCB_IO_CURSTATE = 3,
}IOBUS_TCB;

#define TCBn 4;

typedef enum
{
	IOBUS_OUTPUT = 0,
	IOBUS_INPUT  = 1,
}IOBUS_STATE;

typedef enum
{
	BUS_A = 0,
	BUS_B = 1,
}IOBUS_Typedef;

typedef enum
{
	SUB_A0 = 0,
	SUB_A1 = 1,
	SUB_A2 = 2,
	SUB_A3 = 3,
}IOSUB_Typedef;

typedef enum
{
	OUTPUT_LOW  = 0,	
	OUTPUT_HIGH = 1,
}OUTPUT_Typedef;

typedef enum
{
	INPUT_LOW  = 0,	
	INPUT_HIGH = 1,
}INPUT_Typedef;

typedef enum
{
	INT_ON  = 1,
	INT_OFF = 0,
}INT_Typedef;

typedef enum
{
	INT_RISING = 0,
	INT_FALLING= 1,
	INT_RANDF  = 2,
}INTMODE_Typedef;

#define IOBUSA_EN_CLK		RCC_APB2Periph_GPIOB
#define IOBUSA_EN_PIN       GPIO_Pin_7
#define IOBUSA_EN_PORT      GPIOB

#define IOBUSB_EN_CLK       RCC_APB2Periph_GPIOB   
#define IOBUSB_EN_PIN       GPIO_Pin_13
#define IOBUSB_EN_PORT      GPIOB

/* IOBUSA */
#define IOBUSA_A0_CLK       RCC_APB2Periph_GPIOB
#define IOBUSA_A0_PIN       GPIO_Pin_3
#define IOBUSA_A0_PORT      GPIOB
#define IOBUSA_A0_PORTS     GPIO_PortSourceGPIOB
#define IOBUSA_A0_PINS      GPIO_PinSource3
#define IOBUSA_A0_EXTI      EXTI_Line3
#define IOBUSA_A0_NVIC     	EXTI3_IRQn

#define IOBUSA_A1_CLK       RCC_APB2Periph_GPIOB
#define IOBUSA_A1_PIN       GPIO_Pin_0
#define IOBUSA_A1_PORT      GPIOB
#define IOBUSA_A1_PORTS     GPIO_PortSourceGPIOB
#define IOBUSA_A1_PINS      GPIO_PinSource0
#define IOBUSA_A1_EXTI      EXTI_Line0
#define IOBUSA_A1_NVIC      EXTI0_IRQn

#define IOBUSA_A2_CLK       RCC_APB2Periph_GPIOB
#define IOBUSA_A2_PIN       GPIO_Pin_1
#define IOBUSA_A2_PORT      GPIOB
#define IOBUSA_A2_PORTS     GPIO_PortSourceGPIOB
#define IOBUSA_A2_PINS      GPIO_PinSource1
#define IOBUSA_A2_EXTI      EXTI_Line1
#define IOBUSA_A2_NVIC      EXTI1_IRQn

#define IOBUSA_A3_CLK       RCC_APB2Periph_GPIOA
#define IOBUSA_A3_PIN       GPIO_Pin_4
#define IOBUSA_A3_PORT      GPIOA
#define IOBUSA_A3_PORTS     GPIO_PortSourceGPIOA
#define IOBUSA_A3_PINS      GPIO_PinSource4
#define IOBUSA_A3_EXTI      EXTI_Line4
#define IOBUSA_A3_NVIC      EXTI4_IRQn

/* IOBUSB */
#define IOBUSB_A0_CLK       RCC_APB2Periph_GPIOB
#define IOBUSB_A0_PIN       GPIO_Pin_6
#define IOBUSB_A0_PORT      GPIOB
#define IOBUSB_A0_PORTS     GPIO_PortSourceGPIOB
#define IOBUSB_A0_PINS      GPIO_PinSource6
#define IOBUSB_A0_EXTI      EXTI_Line6
#define IOBUSB_A0_NVIC      EXTI9_5_IRQn

#define IOBUSB_A1_CLK       RCC_APB2Periph_GPIOB
#define IOBUSB_A1_PIN       GPIO_Pin_5
#define IOBUSB_A1_PORT      GPIOB
#define IOBUSB_A1_PORTS     GPIO_PortSourceGPIOB
#define IOBUSB_A1_PINS      GPIO_PinSource5
#define IOBUSB_A1_EXTI      EXTI_Line5
#define IOBUSB_A1_NVIC      EXTI9_5_IRQn

#define IOBUSB_A2_CLK       RCC_APB2Periph_GPIOB
#define IOBUSB_A2_PIN       GPIO_Pin_4
#define IOBUSB_A2_PORT      GPIOB
#define IOBUSB_A2_PORTS     GPIO_PortSourceGPIOB
#define IOBUSB_A2_PINS      GPIO_PinSource4
#define IOBUSB_A2_EXTI      EXTI_Line4
#define IOBUSB_A2_NVIC      EXTI4_IRQn

#define IOBUSB_A3_CLK       RCC_APB2Periph_GPIOD
#define IOBUSB_A3_PIN       GPIO_Pin_2
#define IOBUSB_A3_PORT      GPIOD
#define IOBUSB_A3_PORTS     GPIO_PortSourceGPIOD
#define IOBUSB_A3_PINS      GPIO_PinSource2
#define IOBUSB_A3_EXTI      EXTI_Line2
#define IOBUSB_A3_NVIC      EXTI2_IRQn

extern IO_TCBTypedef IOBUSA_TCB[SUBn];
extern IO_TCBTypedef IOBUSB_TCB[SUBn];

void iobus_reload(void);
void iobus_init(void);
void iobus_set_mode(IOBUS_Typedef iobus, IOBUS_STATE state);
void iobus_set_output(IOBUS_Typedef iobus, IOSUB_Typedef iosub, OUTPUT_Typedef output);
uint8_t iobus_get_ioinput(IOBUS_Typedef iobus, IOSUB_Typedef iosub);
uint8_t iobus_get_businput(IOBUS_Typedef iobus);
void iobus_set_tcbn(IO_TCBTypedef *tcb, IOBUS_TCB iobus_tcb, INT8U value);
void iobus_set_nvic(IOBUS_Typedef iobus, IOSUB_Typedef iosub, 
	                INT_Typedef ioint,   INTMODE_Typedef intmode);

#endif

