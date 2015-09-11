/*****************************************************
FileName: dSPIN.c
Author:   Tsui
Company:  SnowFlake
Version:  1.0
Date:     2015-1-4
Description: dSPIN.c
	<!	dSPIN.c提供了L6470所有的设置和命令,如果没有
	特殊问题,不能修改!>
	L6470管脚初始化
	FLAG管脚中断初始化
	L6470命令函数
Function List: 
*****************************************************/
#include "dSPIN.h"


/* Initializes GPIOs
   *param: None
   *retval: None
*/
void dSPIN_Peripherals_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	/* 打开SPI时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /***********************************************************/

	/* SPI */
    GPIO_InitStructure.GPIO_Pin = dSPIN_SCK_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(dSPIN_SCK_Port, &GPIO_InitStructure);  

    GPIO_InitStructure.GPIO_Pin = dSPIN_MOSI_Pin;
    GPIO_Init(dSPIN_MOSI_Port, &GPIO_InitStructure);  

    GPIO_InitStructure.GPIO_Pin = dSPIN_MISO_Pin;
    GPIO_Init(dSPIN_MISO_Port, &GPIO_InitStructure);  
	
    /***********************************************************/

    /***********************************************************/
	
	/* CS(低电平有效) */
	GPIO_InitStructure.GPIO_Pin = dSPIN_nSSx_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(dSPIN_nSSx_Port, &GPIO_InitStructure);
	GPIO_SetBits(dSPIN_nSSx_Port, dSPIN_nSSx_Pin);

	GPIO_InitStructure.GPIO_Pin = dSPIN_nSSy_Pin;
	GPIO_Init(dSPIN_nSSy_Port, &GPIO_InitStructure);
	GPIO_SetBits(dSPIN_nSSy_Port, dSPIN_nSSy_Pin);

	GPIO_InitStructure.GPIO_Pin = dSPIN_nSSz_Pin;
	GPIO_Init(dSPIN_nSSz_Port, &GPIO_InitStructure);
	GPIO_SetBits(dSPIN_nSSz_Port, dSPIN_nSSz_Pin);

    /***********************************************************/

    /***********************************************************/
	
	/* STBY(低电平有效) */
	GPIO_InitStructure.GPIO_Pin = dSPIN_STBYx_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(dSPIN_STBYx_Port, &GPIO_InitStructure);
	GPIO_SetBits(dSPIN_STBYx_Port, dSPIN_STBYx_Pin);

	GPIO_InitStructure.GPIO_Pin = dSPIN_STBYy_Pin;
	GPIO_Init(dSPIN_STBYy_Port, &GPIO_InitStructure);
	GPIO_SetBits(dSPIN_STBYy_Port, dSPIN_STBYy_Pin);

	GPIO_InitStructure.GPIO_Pin = dSPIN_STBYz_Pin;
	GPIO_Init(dSPIN_STBYz_Port, &GPIO_InitStructure);
	GPIO_SetBits(dSPIN_STBYz_Port, dSPIN_STBYz_Pin);
	
    /***********************************************************/

    /***********************************************************/
	
    /* BSYN */
	GPIO_InitStructure.GPIO_Pin = dSPIN_BUSYx_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(dSPIN_BUSYx_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = dSPIN_BUSYy_Pin;
	GPIO_Init(dSPIN_BUSYy_Port, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = dSPIN_BUSYz_Pin;
	GPIO_Init(dSPIN_BUSYz_Port, &GPIO_InitStructure);	
		
    /***********************************************************/

    /***********************************************************/
	
    /* FLAG */
    GPIO_InitStructure.GPIO_Pin = dSPIN_FLAGx_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(dSPIN_FLAGx_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = dSPIN_FLAGy_Pin;
    GPIO_Init(dSPIN_FLAGy_Port, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = dSPIN_FLAGz_Pin;
    GPIO_Init(dSPIN_FLAGz_Port, &GPIO_InitStructure);
		
    /***********************************************************/
    
    /* SPI Initialize */
    SPI_Cmd(SPI1, DISABLE);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
}

/* Resets DSPIN and puts it into standby mode 
   *param: target device
   *retval: None
*/
void dSPIN_Reset_And_Standby(dSPIN_Device_TypeDef Device)
{
	switch(Device)
	{
		case dSPIN_Device_X:
			GPIO_ResetBits(dSPIN_STBYx_Port, dSPIN_STBYx_Pin);
			utDelay1ms(200);
			GPIO_SetBits(dSPIN_STBYx_Port, dSPIN_STBYx_Pin);
			break;
			
		case dSPIN_Device_Y:
			GPIO_ResetBits(dSPIN_STBYy_Port, dSPIN_STBYy_Pin);
			utDelay1ms(200);
			GPIO_SetBits(dSPIN_STBYy_Port, dSPIN_STBYy_Pin);
			break;

		case dSPIN_Device_Z:
			GPIO_ResetBits(dSPIN_STBYz_Port, dSPIN_STBYz_Pin);
			utDelay1ms(200);
			GPIO_SetBits(dSPIN_STBYz_Port, dSPIN_STBYz_Pin);
			break;

		case dSPIN_Device_All:
			GPIO_ResetBits(dSPIN_STBYx_Port, dSPIN_STBYx_Pin);
			GPIO_ResetBits(dSPIN_STBYy_Port, dSPIN_STBYy_Pin);
			GPIO_ResetBits(dSPIN_STBYz_Port, dSPIN_STBYz_Pin);
			utDelay1ms(200);
			GPIO_SetBits(dSPIN_STBYx_Port, dSPIN_STBYx_Pin);
			GPIO_SetBits(dSPIN_STBYy_Port, dSPIN_STBYy_Pin);
			GPIO_SetBits(dSPIN_STBYz_Port, dSPIN_STBYz_Pin);
			break;

		default:
			break;
	}
}

/* Interrupt channel configuration and enable 
   *param: None
   *retval: None
*/
void dSPIN_Interrupt_Channel_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_EXTILineConfig(dSPIN_FLAGx_PortSource, dSPIN_FLAGx_PinSource);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	GPIO_EXTILineConfig(dSPIN_FLAGy_PortSource, dSPIN_FLAGy_PinSource);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_Init(&EXTI_InitStructure);

	GPIO_EXTILineConfig(dSPIN_FLAGz_PortSource, dSPIN_FLAGz_PinSource);
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

/* Fills-in dSPN configuration structure with default values 
   *param: dSPIN_RegsStruct structure address(pointer to struct)
   *retval: None
*/
void dSPIN_Regs_Struct_Reset(dSPIN_RegsStruct_TypeDef* dSPIN_RegsStruct)
{
    dSPIN_RegsStruct->ABS_POS    = 0;
    dSPIN_RegsStruct->EL_POS     = 0;
    dSPIN_RegsStruct->MARK       = 0;
    dSPIN_RegsStruct->ACC        = 0x08A;
    dSPIN_RegsStruct->DEC        = 0x08A;
    dSPIN_RegsStruct->MAX_SPEED  = 0x041;
    dSPIN_RegsStruct->MIN_SPEED  = 0;
    dSPIN_RegsStruct->FS_SPD     = 0x027;
    dSPIN_RegsStruct->KVAL_HOLD  = 0x29;
    dSPIN_RegsStruct->KVAL_RUN   = 0x29;
    dSPIN_RegsStruct->KVAL_ACC   = 0x29;
    dSPIN_RegsStruct->KVAL_DEC   = 0x29;
    dSPIN_RegsStruct->INT_SPD    = 0x0408;
    dSPIN_RegsStruct->ST_SLP     = 0x19;
    dSPIN_RegsStruct->FN_SLP_ACC = 0x29;
    dSPIN_RegsStruct->FN_SLP_DEC = 0x29;
    dSPIN_RegsStruct->K_THERM    = 0;
    dSPIN_RegsStruct->OCD_TH     = 0x8;
    dSPIN_RegsStruct->STALL_TH   = 0x40;
    dSPIN_RegsStruct->STEP_MODE  = 0x7;
    dSPIN_RegsStruct->ALARM_EN   = 0xFF;
    dSPIN_RegsStruct->CONFIG     = 0x2E88;
}

/* Configures dSPIN internal registers with value in the config structure.
   *param_1: dSPIN_RegsStruct structure address(pointer to struct)
   *param_2: target device
   *retval: None
*/
void dSPIN_Registers_Set(dSPIN_RegsStruct_TypeDef* dSPIN_RegsStruct, dSPIN_Device_TypeDef Device)
{
	dSPIN_Set_Param(dSPIN_ABS_POS, dSPIN_RegsStruct->ABS_POS, Device);
	dSPIN_Set_Param(dSPIN_EL_POS, dSPIN_RegsStruct->EL_POS, Device);
	dSPIN_Set_Param(dSPIN_MARK, dSPIN_RegsStruct->MARK, Device);
	dSPIN_Set_Param(dSPIN_ACC, dSPIN_RegsStruct->ACC, Device);
	dSPIN_Set_Param(dSPIN_DEC, dSPIN_RegsStruct->DEC, Device);
	dSPIN_Set_Param(dSPIN_MAX_SPEED, dSPIN_RegsStruct->MAX_SPEED, Device);
	dSPIN_Set_Param(dSPIN_MIN_SPEED, dSPIN_RegsStruct->MIN_SPEED, Device);
	dSPIN_Set_Param(dSPIN_FS_SPD, dSPIN_RegsStruct->FS_SPD, Device);
	dSPIN_Set_Param(dSPIN_KVAL_HOLD, dSPIN_RegsStruct->KVAL_HOLD, Device);
	dSPIN_Set_Param(dSPIN_KVAL_RUN, dSPIN_RegsStruct->KVAL_RUN, Device);
	dSPIN_Set_Param(dSPIN_KVAL_ACC, dSPIN_RegsStruct->KVAL_ACC, Device);
	dSPIN_Set_Param(dSPIN_KVAL_DEC, dSPIN_RegsStruct->KVAL_DEC, Device);
	dSPIN_Set_Param(dSPIN_INT_SPD, dSPIN_RegsStruct->INT_SPD, Device);
	dSPIN_Set_Param(dSPIN_ST_SLP, dSPIN_RegsStruct->ST_SLP, Device);
	dSPIN_Set_Param(dSPIN_FN_SLP_ACC, dSPIN_RegsStruct->FN_SLP_ACC, Device);
	dSPIN_Set_Param(dSPIN_FN_SLP_DEC, dSPIN_RegsStruct->FN_SLP_DEC, Device);
	dSPIN_Set_Param(dSPIN_K_THERM, dSPIN_RegsStruct->K_THERM, Device);
	dSPIN_Set_Param(dSPIN_OCD_TH, dSPIN_RegsStruct->OCD_TH, Device);
	dSPIN_Set_Param(dSPIN_STALL_TH, dSPIN_RegsStruct->STALL_TH, Device);
	dSPIN_Set_Param(dSPIN_STEP_MODE, dSPIN_RegsStruct->STEP_MODE, Device);
	dSPIN_Set_Param(dSPIN_ALARM_EN, dSPIN_RegsStruct->ALARM_EN, Device);
	dSPIN_Set_Param(dSPIN_CONFIG, dSPIN_RegsStruct->CONFIG, Device);
}

/* Issues dSPIN NOP command. 
   *param: target device
   *retval: None
*/
void dSPIN_Nop(dSPIN_Device_TypeDef Device)
{
    dSPIN_Write_Byte(dSPIN_NOP, Device);
}

/* Issues dSPIN Set Param command. 
   *param_1: param dSPIN register address
   *param_2: value to be set
   *param_3: target device
   *retval: None
*/
void dSPIN_Set_Param(dSPIN_Registers_TypeDef param, uint32_t value, dSPIN_Device_TypeDef Device)
{
    /* Send SetParam operation code to dSPIN */
    dSPIN_Write_Byte((uint8_t)dSPIN_SET_PARAM | (uint8_t)param, Device);
    switch(param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK:    ;
            dSPIN_Write_Byte((uint8_t)(value >> 16), Device);
		case dSPIN_EL_POS: ;
        case dSPIN_ACC:     ;
        case dSPIN_DEC:     ;
        case dSPIN_MAX_SPEED:   ;
        case dSPIN_MIN_SPEED:   ;
        case dSPIN_FS_SPD:      ;
        case dSPIN_INT_SPD:     ;
        case dSPIN_CONFIG:      ;
        case dSPIN_STATUS:
            dSPIN_Write_Byte((uint8_t)(value >> 8), Device);
        default:
            dSPIN_Write_Byte((uint8_t)(value), Device);
    }
}

/* Issues dSPIN Get Param command.
   *param_1: param dSPIN register address
   *param_2: target device
   *retval: Register value - 1 to 3 bytes (depends on register)
*/

uint32_t dSPIN_Get_Param(dSPIN_Registers_TypeDef param, dSPIN_Device_TypeDef Device)
{
    uint32_t temp = 0;
    uint32_t rx = 0;

    temp = dSPIN_Write_Byte((uint8_t)dSPIN_GET_PARAM | (uint8_t)param, Device);
    temp = temp << 24;
    rx |= temp;

    switch(param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK:    ;
        case dSPIN_SPEED:    
            temp = dSPIN_Write_Byte((uint8_t)(0x00), Device);
            temp = temp << 16;
            rx |= temp;
		case dSPIN_EL_POS: ;
        case dSPIN_ACC: ;
		case dSPIN_DEC: ;
		case dSPIN_MAX_SPEED: ;
		case dSPIN_MIN_SPEED: ;
		case dSPIN_FS_SPD: ;
		case dSPIN_INT_SPD: ;
		case dSPIN_CONFIG: ;
		case dSPIN_STATUS:
		   	temp = dSPIN_Write_Byte((uint8_t)(0x00), Device);
			temp = temp << 8;
			rx |= temp;
		default:
		   	temp = dSPIN_Write_Byte((uint8_t)(0x00), Device);
			rx |= temp;
    }
    return rx;
}

/* Issues dSPIN Run command.
  *param_1: direction Movement direction (FWD, REV)
  *param_2: speed over 3 bytes
  *param_3: target device
  *retval: None
*/
void dSPIN_Run(dSPIN_Direction_TypeDef direction, uint32_t speed, dSPIN_Device_TypeDef Device)
{
	/* Send RUN operation code to dSPIN */
	dSPIN_Write_Byte((uint8_t)dSPIN_RUN | (uint8_t)direction, Device);
	/* Send speed - byte 2 data dSPIN */
	dSPIN_Write_Byte((uint8_t)(speed >> 16), Device);
	/* Send speed - byte 1 data dSPIN */
	dSPIN_Write_Byte((uint8_t)(speed >> 8), Device);
	/* Send speed - byte 0 data dSPIN */
	dSPIN_Write_Byte((uint8_t)(speed), Device);
}

/* Issues dSPIN Step Clock command.
   *param_1: direction Movement direction (FWD, REV)
   *param_2: target device
   *retval: None
*/
void dSPIN_Step_Clock(dSPIN_Direction_TypeDef direction, dSPIN_Device_TypeDef Device)
{
	/* Send StepClock operation code to dSPIN */
	dSPIN_Write_Byte((uint8_t)dSPIN_STEP_CLOCK | (uint8_t)direction, Device);
}

/* Issues dSPIN Move command.
   *param_1: direction mMovement direction
   *param_2: n_step number of steps
   *param_3: target device
   *retval: None
*/
void dSPIN_Move(dSPIN_Direction_TypeDef direction, uint32_t n_step, dSPIN_Device_TypeDef Device)
{
	/* Send Move operation code to dSPIN */
	dSPIN_Write_Byte(dSPIN_MOVE | direction, Device);
	/* Send n_step - byte 2 data dSPIN */
	dSPIN_Write_Byte((uint8_t)(n_step >> 16), Device);
	/* Send n_step - byte 1 data dSPIN */
	dSPIN_Write_Byte((uint8_t)(n_step >> 8), Device);
	/* Send n_step - byte 0 data dSPIN */
	dSPIN_Write_Byte((uint8_t)(n_step), Device);
}

/* Issues dSPIN Go To command.
   *param_1: abs_pos absolute position where requested to move
   *param_2: target device
   *retval: None
*/
void dSPIN_Go_To(uint32_t abs_pos, dSPIN_Device_TypeDef Device)
{
	/* Send GoTo operation code to dSPIN */
	dSPIN_Write_Byte(dSPIN_GO_TO, Device);
	/* Send absolute position parameter - byte 2 data to dSPIN */
	dSPIN_Write_Byte((uint8_t)(abs_pos >> 16), Device);
	/* Send absolute position parameter - byte 1 data to dSPIN */
	dSPIN_Write_Byte((uint8_t)(abs_pos >> 8), Device);
	/* Send absolute position parameter - byte 0 data to dSPIN */
	dSPIN_Write_Byte((uint8_t)(abs_pos), Device);
}


/* Issues dSPIN Go To Dir command.
   *param_1: direction movement direction
   *param_2: abs_pos absolute position where requested to move
   *param_3: target device
   *retval: None
*/
void dSPIN_Go_To_Dir(dSPIN_Direction_TypeDef direction, uint32_t abs_pos, dSPIN_Device_TypeDef Device)
{
	/* Send GoTo_DIR operation code to dSPIN */
	dSPIN_Write_Byte((uint8_t)dSPIN_GO_TO_DIR | (uint8_t)direction, Device);
	/* Send absolute position parameter - byte 2 data to dSPIN */
	dSPIN_Write_Byte((uint8_t)(abs_pos >> 16), Device);
	/* Send absolute position parameter - byte 1 data to dSPIN */
	dSPIN_Write_Byte((uint8_t)(abs_pos >> 8), Device);
	/* Send absolute position parameter - byte 0 data to dSPIN */
	dSPIN_Write_Byte((uint8_t)(abs_pos), Device);
}

/* Issues dSPIN Go Until command.
   *param_1: action
   *param_2: direction movement direction
   *param_3: speed
   *param_4: target device
   *retval: None
*/
void dSPIN_Go_Until(dSPIN_Action_TypeDef action, dSPIN_Direction_TypeDef direction, uint32_t speed, dSPIN_Device_TypeDef Device)
{
	/* Send GoUntil operation code to dSPIN */
	dSPIN_Write_Byte((uint8_t)dSPIN_GO_UNTIL | (uint8_t)action | (uint8_t)direction, Device);
	/* Send speed parameter - byte 2 data to dSPIN */
	dSPIN_Write_Byte((uint8_t)(speed >> 16), Device);
	/* Send speed parameter - byte 1 data to dSPIN */
	dSPIN_Write_Byte((uint8_t)(speed >> 8), Device);
	/* Send speed parameter - byte 0 data to dSPIN */
	dSPIN_Write_Byte((uint8_t)(speed), Device);
}

/* Issues dSPIN Release SW command.
   *param_1: action
   *param_2: direction movement direction
   *param_3: target device
   *retval: None
*/
void dSPIN_Release_SW(dSPIN_Action_TypeDef action, dSPIN_Direction_TypeDef direction, dSPIN_Device_TypeDef Device)
{
	/* Send ReleaseSW operation code to dSPIN */
	dSPIN_Write_Byte((uint8_t)dSPIN_RELEASE_SW | (uint8_t)action | (uint8_t)direction, Device);
}

/* Issues dSPIN Go Home command. (Shorted path to zero position)
   *param: target device
   *retval: None
*/
void dSPIN_Go_Home(dSPIN_Device_TypeDef Device)
{
	/* Send GoHome operation code to dSPIN */
	dSPIN_Write_Byte(dSPIN_GO_HOME, Device);
}

/* Issues dSPIN Go Mark command.
   *param: target device
   *retval: None
*/
void dSPIN_Go_Mark(dSPIN_Device_TypeDef Device)
{
	/* Send GoMark operation code to dSPIN */
	dSPIN_Write_Byte(dSPIN_GO_MARK, Device);
}

/* Issues dSPIN Reset Pos command.
   *param: target device
   *retval: None
*/
void dSPIN_Reset_Pos(dSPIN_Device_TypeDef Device)
{
	/* Send ResetPos operation code to dSPIN */
	dSPIN_Write_Byte(dSPIN_RESET_POS, Device);
}

/* Issues dSPIN Reset Device command.
   *param: None
   *retval: None
*/
void dSPIN_Reset_Device(dSPIN_Device_TypeDef Device)
{
	/* Send ResetDevice operation code to dSPIN */
	dSPIN_Write_Byte(dSPIN_RESET_DEVICE, Device);
}

/* Issues dSPIN Soft Stop command.
   *param: target device
   *retval: None
*/
void dSPIN_Soft_Stop(dSPIN_Device_TypeDef Device)
{
	/* Send SoftStop operation code to dSPIN */
	dSPIN_Write_Byte(dSPIN_SOFT_STOP, Device);
}

/* Issues dSPIN Hard Stop command.
   *param: target device
   *retval: None
*/
void dSPIN_Hard_Stop(dSPIN_Device_TypeDef Device)
{
	/* Send HardStop operation code to dSPIN */
	dSPIN_Write_Byte(dSPIN_HARD_STOP, Device);
}

/* Issues dSPIN Soft Hiz command.
   *param: target Device
   *retval: None
*/
void dSPIN_Soft_HiZ(dSPIN_Device_TypeDef Device)
{
	/* Send SoftHiZ operation code to dSPIN */
	dSPIN_Write_Byte(dSPIN_SOFT_HIZ, Device);
}

/* Issues dSPIN Hard Stop command.
   *param: target device
   *retval: None
*/
void dSPIN_Hard_HiZ(dSPIN_Device_TypeDef Device)
{
	/* Send HardHiZ operation code to dSPIN */
	dSPIN_Write_Byte(dSPIN_HARD_HIZ, Device);
}

/* Issues dSPIN Get Status command.
   *param: target device
   *retval: Status Register content
*/
uint16_t dSPIN_Get_Status(dSPIN_Device_TypeDef Device)
{
	uint16_t temp = 0;
	uint16_t rx = 0;

	/* Send GetStatus operation code to dSPIN */
	dSPIN_Write_Byte(dSPIN_GET_STATUS, Device);
	/* Send zero byte / receive MSByte from dSPIN */
	temp = dSPIN_Write_Byte((uint8_t)(0x00), Device);
	temp = temp << 8;
	rx |= temp;
	/* Send zero byte / receive LSByte from dSPIN */
	temp = dSPIN_Write_Byte((uint8_t)(0x00), Device);
	rx |= temp;
	return rx;
}

/* Checks if the dSPIN is Busy by hardware - active Busy signal.
   *param: target device
   *retval: one if chip is busy, otherwise zero
*/
uint8_t dSPIN_Busy_HW(dSPIN_Device_TypeDef Device)
{
	switch(Device)
	{
		case dSPIN_Device_X:
			if(!(GPIO_ReadInputDataBit(dSPIN_BUSYx_Port, dSPIN_BUSYx_Pin))) 
				return 0x01;
			else 
				return 0x00;
			break;

		case dSPIN_Device_Y:
			if(!(GPIO_ReadInputDataBit(dSPIN_BUSYy_Port, dSPIN_BUSYy_Pin))) 
				return 0x01;
			else 
				return 0x00;
			break;

		case dSPIN_Device_Z:
			if(!(GPIO_ReadInputDataBit(dSPIN_BUSYz_Port, dSPIN_BUSYz_Pin))) 
				return 0x01;
			else 
				return 0x00;
			break;

		default:
			break;
	}
	return 0;
}

/* Checks if the dSPIN is Busy by SPI - Busy flag bit in Status Register.
   *param: target device
   *retval: one if chip is busy, otherwise zero
*/
uint8_t dSPIN_Busy_SW(dSPIN_Device_TypeDef Device)
{
	if(!(dSPIN_Get_Status(Device) & dSPIN_STATUS_BUSY)) 
		return 0x01;
	else 
		return 0x00;
}

/* Checks dSPIN Flag signal.
   *param: target device
   *retval: one if Flag signal is active, otherwise zero
*/
uint8_t dSPIN_Flag(dSPIN_Device_TypeDef Device)
{
	switch(Device)
	{
		case dSPIN_Device_X:
			if(!(GPIO_ReadInputDataBit(dSPIN_FLAGx_Port, dSPIN_FLAGx_Pin))) 
				return 0x01;
			else 
				return 0x00;
			break;

		case dSPIN_Device_Y:
			if(!(GPIO_ReadInputDataBit(dSPIN_FLAGy_Port, dSPIN_FLAGy_Pin))) 
				return 0x01;
			else 
				return 0x00;
			break;

		case dSPIN_Device_Z:
			if(!(GPIO_ReadInputDataBit(dSPIN_FLAGz_Port, dSPIN_FLAGz_Pin))) 
				return 0x01;
			else 
				return 0x00;
			break;

		default:
			break;
	}	
	return 0;
}

/* Transmits/Receives one byte to/from dSPIN over SPI.
   *param_1: byte Transmited byte
   *param_2: target device
   *retval: Received byte
*/
uint8_t dSPIN_Write_Byte(uint8_t byte, dSPIN_Device_TypeDef Device)
{
	switch(Device)
	{
		/* X轴 */
		case dSPIN_Device_X:
    		GPIO_ResetBits(dSPIN_nSSx_Port, dSPIN_nSSx_Pin);
			SPI_I2S_SendData(SPI1, byte);
    		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET);
    		GPIO_SetBits(dSPIN_nSSx_Port, dSPIN_nSSx_Pin);
			break;

		/* Y轴 */
		case dSPIN_Device_Y:
			GPIO_ResetBits(dSPIN_nSSy_Port, dSPIN_nSSy_Pin);
			SPI_I2S_SendData(SPI1, byte);
    		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET);
    		GPIO_SetBits(dSPIN_nSSy_Port, dSPIN_nSSy_Pin);
			break;

		/* Z轴 */
		case dSPIN_Device_Z:
			GPIO_ResetBits(dSPIN_nSSz_Port, dSPIN_nSSz_Pin);
			SPI_I2S_SendData(SPI1, byte);
    		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET);
    		GPIO_SetBits(dSPIN_nSSz_Port, dSPIN_nSSz_Pin);
			break;

		default:
			break;
	}
	
    return (uint8_t)(SPI_I2S_ReceiveData(SPI1));
}

/*****************************@ ShowFlake @*******************************/
