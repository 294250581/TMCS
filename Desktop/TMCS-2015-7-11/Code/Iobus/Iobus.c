#include "Iobus.h"

GPIO_TypeDef* IOBUS_EN_PORT[BUSn] = {IOBUSA_EN_PORT, IOBUSB_EN_PORT};
GPIO_TypeDef* IOBUSA_PORT[SUBn]   = {IOBUSA_A0_PORT, IOBUSA_A1_PORT,
	                                 IOBUSA_A2_PORT, IOBUSA_A3_PORT};
GPIO_TypeDef* IOBUSB_PORT[SUBn]   = {IOBUSB_A0_PORT, IOBUSB_A1_PORT,
	                                 IOBUSB_A2_PORT, IOBUSB_A3_PORT};

const uint32_t IOBUS_EN_CLK[BUSn] = {IOBUSA_EN_CLK, IOBUSB_EN_CLK};
const uint32_t IOBUSA_CLK[SUBn]   = {IOBUSA_A0_CLK, IOBUSA_A1_CLK,
	                                 IOBUSA_A2_CLK, IOBUSA_A3_CLK};
const uint32_t IOBUSB_CLK[SUBn]   = {IOBUSB_A0_CLK, IOBUSB_A1_CLK,
	                                 IOBUSB_A2_CLK, IOBUSB_A3_CLK};

const uint16_t IOBUS_EN_PIN[BUSn] = {IOBUSA_EN_PIN, IOBUSB_EN_PIN};
const uint16_t IOBUSA_PIN[SUBn]   = {IOBUSA_A0_PIN, IOBUSA_A1_PIN,
	                                 IOBUSA_A2_PIN, IOBUSA_A3_PIN};
const uint16_t IOBUSB_PIN[SUBn]   = {IOBUSB_A0_PIN, IOBUSB_A1_PIN,
	                                 IOBUSB_A2_PIN, IOBUSB_A3_PIN};

const uint8_t  IOBUSA_PORTS[SUBn] = {IOBUSA_A0_PORTS, IOBUSA_A1_PORTS,
	                                 IOBUSA_A2_PORTS, IOBUSA_A3_PORTS};
const uint8_t  IOBUSA_PINS[SUBn]  = {IOBUSA_A0_PINS, IOBUSA_A1_PINS,
	                                 IOBUSA_A2_PINS, IOBUSA_A3_PINS};
const uint32_t IOBUSA_EXTI[SUBn]  = {IOBUSA_A0_EXTI, IOBUSA_A1_EXTI,
	                                 IOBUSA_A2_EXTI, IOBUSA_A3_EXTI};
IRQn_Type      IOBUSA_NVIC[SUBn]  = {IOBUSA_A0_NVIC, IOBUSA_A1_NVIC,
	                                 IOBUSA_A2_NVIC, IOBUSA_A3_NVIC};

const uint8_t  IOBUSB_PORTS[SUBn] = {IOBUSB_A0_PORTS, IOBUSB_A1_PORTS,
	                                 IOBUSB_A2_PORTS, IOBUSB_A3_PORTS};
const uint8_t  IOBUSB_PINS[SUBn]  = {IOBUSB_A0_PINS, IOBUSB_A1_PINS,
	                                 IOBUSB_A2_PINS, IOBUSB_A3_PINS};
const uint32_t IOBUSB_EXTI[SUBn]  = {IOBUSB_A0_EXTI, IOBUSB_A1_EXTI,
	                                 IOBUSB_A2_EXTI, IOBUSB_A3_EXTI};
IRQn_Type      IOBUSB_NVIC[SUBn]  = {IOBUSB_A0_NVIC, IOBUSB_A1_NVIC,
	                                 IOBUSB_A2_NVIC, IOBUSB_A3_NVIC};

IO_TCBTypedef IOBUSA_TCB[SUBn];
IO_TCBTypedef IOBUSB_TCB[SUBn];


/* init io tcb */
void iobus_tcb_init(void)
{
	INT8U SUB=0;

	for(SUB=0; SUB<4; SUB++)
	{
		IOBUSA_TCB[SUB].io_mode = IOBUS_INPUT;
		IOBUSA_TCB[SUB].io_int  = INT_OFF;
		IOBUSA_TCB[SUB].io_intmode = INT_FALLING;
		
		IOBUSB_TCB[SUB].io_mode = IOBUS_INPUT;
		IOBUSB_TCB[SUB].io_int  = INT_OFF;
		IOBUSB_TCB[SUB].io_intmode = INT_FALLING;
	}
}

/* Iobus en GPIO Init */
void iobus_en_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(IOBUS_EN_CLK[BUS_A] | IOBUS_EN_CLK[BUS_B], ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = IOBUS_EN_PIN[BUS_A];
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(IOBUS_EN_PORT[BUS_A], &GPIO_InitStructure);
	/* 74HC243配置成输入模式 */
	GPIO_SetBits(IOBUS_EN_PORT[BUS_A], IOBUS_EN_PIN[BUS_A]);

	GPIO_InitStructure.GPIO_Pin = IOBUS_EN_PIN[BUS_B];
	GPIO_Init(IOBUS_EN_PORT[BUS_B], &GPIO_InitStructure);
	/* 74HC243配置成输入模式 */
	GPIO_SetBits(IOBUS_EN_PORT[BUS_B], IOBUS_EN_PIN[BUS_B]);
}

/* Iobus sub GPIO Init */
void iobus_sub_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(IOBUSA_CLK[SUB_A0] | IOBUSA_CLK[SUB_A1] |
		                   IOBUSA_CLK[SUB_A2] | IOBUSA_CLK[SUB_A3], ENABLE);
	RCC_APB2PeriphClockCmd(IOBUSB_CLK[SUB_A0] | IOBUSB_CLK[SUB_A1] |
		                   IOBUSB_CLK[SUB_A2] | IOBUSB_CLK[SUB_A3], ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

	/* 所有端口初始化时设置成输入模式 */
	GPIO_InitStructure.GPIO_Pin = IOBUSA_PIN[SUB_A0];
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(IOBUSA_PORT[SUB_A0], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IOBUSA_PIN[SUB_A1];
	GPIO_Init(IOBUSA_PORT[SUB_A1], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IOBUSA_PIN[SUB_A2];
	GPIO_Init(IOBUSA_PORT[SUB_A2], &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = IOBUSA_PIN[SUB_A3];
	GPIO_Init(IOBUSA_PORT[SUB_A3], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IOBUSB_PIN[SUB_A0];
	GPIO_Init(IOBUSB_PORT[SUB_A0], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IOBUSB_PIN[SUB_A1];
	GPIO_Init(IOBUSB_PORT[SUB_A1], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IOBUSB_PIN[SUB_A2];
	GPIO_Init(IOBUSB_PORT[SUB_A2], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IOBUSB_PIN[SUB_A3];
	GPIO_Init(IOBUSB_PORT[SUB_A3], &GPIO_InitStructure);
}

/* Iobus NVIC Init */
void iobus_nvic_init(void)
{	
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_EXTILineConfig(IOBUSA_PORTS[SUB_A0], IOBUSA_PINS[SUB_A0]);
	EXTI_InitStructure.EXTI_Line = IOBUSA_EXTI[SUB_A0];
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);

	GPIO_EXTILineConfig(IOBUSB_PORTS[SUB_A0], IOBUSB_PINS[SUB_A0]);
	EXTI_InitStructure.EXTI_Line = IOBUSB_EXTI[SUB_A0];
	EXTI_Init(&EXTI_InitStructure);

	GPIO_EXTILineConfig(IOBUSB_PORTS[SUB_A1], IOBUSB_PINS[SUB_A1]);
	EXTI_InitStructure.EXTI_Line = IOBUSB_EXTI[SUB_A1];
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(IOBUSB_PORTS[SUB_A2], IOBUSB_PINS[SUB_A2]);
	EXTI_InitStructure.EXTI_Line = IOBUSB_EXTI[SUB_A2];
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = IOBUSA_NVIC[SUB_A0];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = IOBUSA_NVIC[SUB_A1];
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = IOBUSA_NVIC[SUB_A2];
	NVIC_Init(&NVIC_InitStructure);	

	NVIC_InitStructure.NVIC_IRQChannel = IOBUSA_NVIC[SUB_A3];
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = IOBUSB_NVIC[SUB_A0];
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = IOBUSB_NVIC[SUB_A1];
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = IOBUSB_NVIC[SUB_A2];
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = IOBUSB_NVIC[SUB_A3];
	NVIC_Init(&NVIC_InitStructure);
}

/* Iobus Init */
void iobus_init(void)
{
	iobus_en_init();
	iobus_sub_init();
	iobus_nvic_init();
	iobus_tcb_init();
}

void iobus_mode(IOBUS_Typedef iobus, IOBUS_STATE state)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	if(state == IOBUS_OUTPUT)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	else
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	switch(iobus)
	{
		case BUS_A:
			GPIO_InitStructure.GPIO_Pin = IOBUSA_PIN[SUB_A0];
			GPIO_Init(IOBUSA_PORT[SUB_A0], &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin = IOBUSA_PIN[SUB_A1];
			GPIO_Init(IOBUSA_PORT[SUB_A1], &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin = IOBUSA_PIN[SUB_A2];
			GPIO_Init(IOBUSA_PORT[SUB_A2], &GPIO_InitStructure);	

			GPIO_InitStructure.GPIO_Pin = IOBUSA_PIN[SUB_A3];
			GPIO_Init(IOBUSA_PORT[SUB_A3], &GPIO_InitStructure);
		break;

		case BUS_B:
			GPIO_InitStructure.GPIO_Pin = IOBUSB_PIN[SUB_A0];
			GPIO_Init(IOBUSB_PORT[SUB_A0], &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin = IOBUSB_PIN[SUB_A1];
			GPIO_Init(IOBUSB_PORT[SUB_A1], &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin = IOBUSB_PIN[SUB_A2];
			GPIO_Init(IOBUSB_PORT[SUB_A2], &GPIO_InitStructure);	

			GPIO_InitStructure.GPIO_Pin = IOBUSB_PIN[SUB_A3];
			GPIO_Init(IOBUSB_PORT[SUB_A3], &GPIO_InitStructure);
		break;
	}
}

/* set 243 A and B mode */
void iobus_set_mode(IOBUS_Typedef iobus, IOBUS_STATE state)
{
	/* 总线做输出 */
	if(state == IOBUS_OUTPUT)
	{
		iobus_mode(iobus, state);
		GPIO_ResetBits(IOBUS_EN_PORT[iobus], IOBUS_EN_PIN[iobus]);

		if(iobus == BUS_A)
		{
			iobus_set_tcbn(&IOBUSA_TCB[SUB_A0], TCB_IO_MODE, IOBUS_OUTPUT);
			iobus_set_tcbn(&IOBUSA_TCB[SUB_A1], TCB_IO_MODE, IOBUS_OUTPUT);	
			iobus_set_tcbn(&IOBUSA_TCB[SUB_A2], TCB_IO_MODE, IOBUS_OUTPUT);	
			iobus_set_tcbn(&IOBUSA_TCB[SUB_A3], TCB_IO_MODE, IOBUS_OUTPUT);	
		}
		if(iobus == BUS_B)
		{
			iobus_set_tcbn(&IOBUSB_TCB[SUB_A0], TCB_IO_MODE, IOBUS_OUTPUT);
			iobus_set_tcbn(&IOBUSB_TCB[SUB_A1], TCB_IO_MODE, IOBUS_OUTPUT);	
			iobus_set_tcbn(&IOBUSB_TCB[SUB_A2], TCB_IO_MODE, IOBUS_OUTPUT);	
			iobus_set_tcbn(&IOBUSB_TCB[SUB_A3], TCB_IO_MODE, IOBUS_OUTPUT);	
		}	
	}

	/* 总线做输入 */
	if(state == IOBUS_INPUT)
	{
		iobus_mode(iobus, state);
		GPIO_SetBits(IOBUS_EN_PORT[iobus], IOBUS_EN_PIN[iobus]);

		if(iobus == BUS_A)
		{
			iobus_set_tcbn(&IOBUSA_TCB[SUB_A0], TCB_IO_MODE, IOBUS_INPUT);
			iobus_set_tcbn(&IOBUSA_TCB[SUB_A1], TCB_IO_MODE, IOBUS_INPUT);	
			iobus_set_tcbn(&IOBUSA_TCB[SUB_A2], TCB_IO_MODE, IOBUS_INPUT);	
			iobus_set_tcbn(&IOBUSA_TCB[SUB_A3], TCB_IO_MODE, IOBUS_INPUT);	
		}	
		if(iobus == BUS_B)
		{
			iobus_set_tcbn(&IOBUSB_TCB[SUB_A0], TCB_IO_MODE, IOBUS_INPUT);
			iobus_set_tcbn(&IOBUSB_TCB[SUB_A1], TCB_IO_MODE, IOBUS_INPUT);	
			iobus_set_tcbn(&IOBUSB_TCB[SUB_A2], TCB_IO_MODE, IOBUS_INPUT);	
			iobus_set_tcbn(&IOBUSB_TCB[SUB_A3], TCB_IO_MODE, IOBUS_INPUT);	
		}
	}
}

/* set output */
void iobus_set_output(IOBUS_Typedef iobus, IOSUB_Typedef iosub, OUTPUT_Typedef output)
{
	void (*output_mode)(GPIO_TypeDef*, uint16_t) = NULL;
	if(output == OUTPUT_HIGH)
		output_mode = GPIO_SetBits;
	else
		output_mode = GPIO_ResetBits;

	switch(iobus)
	{
		case BUS_A:
			output_mode(IOBUSA_PORT[iosub], IOBUSA_PIN[iosub]);
		break;

		case BUS_B:
			output_mode(IOBUSB_PORT[iosub], IOBUSB_PIN[iosub]);
		break;
	}
}

/* get IO input */
uint8_t iobus_get_ioinput(IOBUS_Typedef iobus, IOSUB_Typedef iosub)
{
	uint8_t iostate;

	switch (iobus)
	{
		case BUS_A:
			iostate = GPIO_ReadInputDataBit(IOBUSA_PORT[iosub], IOBUSA_PIN[iosub]);
		break;

		case BUS_B:
			iostate = GPIO_ReadInputDataBit(IOBUSB_PORT[iosub], IOBUSB_PIN[iosub]);
		break;

		default:
		break;
	}

	return iostate;
}

/* get bus input */
uint8_t iobus_get_businput(IOBUS_Typedef iobus)
{
	uint8_t iostate;

	switch(iobus)
	{
		case BUS_A:
			iostate = (GPIO_ReadInputDataBit(IOBUSA_PORT[SUB_A0], IOBUSA_PIN[SUB_A0]) << 0) |
				      (GPIO_ReadInputDataBit(IOBUSA_PORT[SUB_A1], IOBUSA_PIN[SUB_A1]) << 1) |
				      (GPIO_ReadInputDataBit(IOBUSA_PORT[SUB_A2], IOBUSA_PIN[SUB_A2]) << 2) |
				      (GPIO_ReadInputDataBit(IOBUSA_PORT[SUB_A3], IOBUSA_PIN[SUB_A3]) << 3);		                            
		break;

		case BUS_B:
			iostate = (GPIO_ReadInputDataBit(IOBUSB_PORT[SUB_A0], IOBUSB_PIN[SUB_A0]) << 0) |
				      (GPIO_ReadInputDataBit(IOBUSB_PORT[SUB_A1], IOBUSB_PIN[SUB_A1]) << 1) |
				      (GPIO_ReadInputDataBit(IOBUSB_PORT[SUB_A2], IOBUSB_PIN[SUB_A2]) << 2) |
				      (GPIO_ReadInputDataBit(IOBUSB_PORT[SUB_A3], IOBUSB_PIN[SUB_A3]) << 3);		        
		break;

		default:
		break;
	}
	return iostate;
}

void iobus_set_tcbn(IO_TCBTypedef *tcb, IOBUS_TCB iobus_tcb, INT8U value)
{	
	switch(iobus_tcb)
	{
		case TCB_IO_MODE:
			utPrt("io_mode\r\n");
			tcb->io_mode = value;
		break;

		case TCB_IO_INT:
			utPrt("io_int\r\n");
			tcb->io_int = value;
		break;

		case TCB_IO_INTMODE:
			utPrt("io_intmode\r\n");
			tcb->io_intmode = value;
		break;
	}
}

void iobus_set_nvic(IOBUS_Typedef iobus, IOSUB_Typedef iosub, 
	                INT_Typedef ioint,   INTMODE_Typedef intmode)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	uint32_t Line;
	uint8_t  IRQChannel;
	EXTITrigger_TypeDef Trigger;

	switch(intmode)
	{
		case INT_RISING:
			Trigger = EXTI_Trigger_Rising;
		break;
		case INT_FALLING:
			Trigger = EXTI_Trigger_Falling;
		break;
		case INT_RANDF:
			Trigger = EXTI_Trigger_Rising_Falling;
		break;
	}
	
	if(iobus == BUS_A)
	{
		Line = IOBUSA_EXTI[iosub];
		IRQChannel = IOBUSA_NVIC[iosub];
		iobus_set_tcbn(&IOBUSA_TCB[iosub], TCB_IO_INT, ioint);
	}
	if(iobus == BUS_B)
	{
		Line = IOBUSB_EXTI[iosub];
		IRQChannel = IOBUSB_NVIC[iosub];
		iobus_set_tcbn(&IOBUSB_TCB[iosub], TCB_IO_INT, ioint);
	}
	
	EXTI_InitStructure.EXTI_Line = Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = Trigger;
	EXTI_InitStructure.EXTI_LineCmd = (FunctionalState)ioint;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = (FunctionalState)ioint;
	NVIC_Init(&NVIC_InitStructure);
}

void iobus_reload(void)
{
	if(IOBUSA_TCB[SUB_A0].io_mode == IOBUS_OUTPUT)
	{
		utPrt("ioa_mode == output\n");
		iobus_set_mode(BUS_A, IOBUS_OUTPUT);
	}
	if(IOBUSA_TCB[SUB_A0].io_mode == IOBUS_INPUT)
	{
		utPrt("ioa_mode == intput\n");
		iobus_set_mode(BUS_A, IOBUS_INPUT);
		if(IOBUSA_TCB[SUB_A0].io_int == INT_ON)
		{
			iobus_set_nvic(BUS_A,SUB_A0,INT_ON,IOBUSA_TCB[SUB_A0].io_intmode);
		}
	}
	
	if(IOBUSB_TCB[SUB_A0].io_mode == IOBUS_OUTPUT)
	{
		utPrt("iob_mode == output\n");
		iobus_set_mode(BUS_B, IOBUS_OUTPUT);
	}
	if(IOBUSB_TCB[SUB_A0].io_mode == IOBUS_INPUT)
	{
		utPrt("iob_mode == input\n");
		iobus_set_mode(BUS_B, IOBUS_INPUT);
		if(IOBUSB_TCB[SUB_A0].io_int == INT_ON)
		{
			iobus_set_nvic(BUS_B,SUB_A0,INT_ON,IOBUSB_TCB[SUB_A0].io_intmode);
		}
		if(IOBUSB_TCB[SUB_A1].io_int == INT_ON)
		{
			iobus_set_nvic(BUS_B,SUB_A1,INT_ON,IOBUSB_TCB[SUB_A1].io_intmode);
		}
		if(IOBUSB_TCB[SUB_A2].io_int == INT_ON)
		{
			iobus_set_nvic(BUS_B,SUB_A2,INT_ON,IOBUSB_TCB[SUB_A2].io_intmode);
		}
	}
}

