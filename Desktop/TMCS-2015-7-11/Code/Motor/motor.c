/***********************************************
FileName: motor.c
Author:   Tsui
Company:  SnowFlake
Version:  1.0
Date:     2015-1-6
Description: motor.c
Function List: 
	INT8U motor_init_set(void);
	void motor_init_tube(void);
***********************************************/
#include "motor.h"
#include "motor_cal.h"

dSPIN_RegsStruct_TypeDef dSPIN_RegsStruct;

/* 电机初始化设定 
   *param: None
   *retval: 0->设定成功;其他->设定失败
*/
INT8U motor_init_set(void)
{
	/* 寄存器复位 */
	dSPIN_Regs_Struct_Reset(&dSPIN_RegsStruct);
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_X);
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Y);
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Z);

	/* X轴参数设定 */
	if(motor_set(&dSPIN_RegsStruct, &motor_device_x))
	{
		/* 设置错误 */
		return 1;
	}
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_X);
	
	/* Y轴参数设定 */
	if(motor_set(&dSPIN_RegsStruct, &motor_device_y))
	{
		/* 设置错误 */
		return 1;
	}	
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Y);
	
	/* Z轴参数设定 */
	if(motor_set(&dSPIN_RegsStruct, &motor_device_z))
	{
		/* 设置错误 */
		return 1;
	}	
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Z);

	/* 绝对位置需清零 */
	dSPIN_Set_Param(dSPIN_ABS_POS, 0, dSPIN_Device_X);
	dSPIN_Set_Param(dSPIN_ABS_POS, 0, dSPIN_Device_Y);
	dSPIN_Set_Param(dSPIN_ABS_POS, 0, dSPIN_Device_Z);

	/* 读状态位,清除错误标志(FLAG) */
	dSPIN_Get_Status(dSPIN_Device_X);
	dSPIN_Get_Status(dSPIN_Device_Y);
	dSPIN_Get_Status(dSPIN_Device_Z);

	/* 锁定电机 */
	dSPIN_Soft_Stop(dSPIN_Device_X);
	dSPIN_Soft_Stop(dSPIN_Device_Y);
	dSPIN_Soft_Stop(dSPIN_Device_Z);

	return 0;
}

/* motor set */
INT8U motor_param_set(void)
{
	/* 寄存器复位 */
	dSPIN_Regs_Struct_Reset(&dSPIN_RegsStruct);
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_X);
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Y);
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Z);

	dSPIN_Hard_HiZ(dSPIN_Device_X);
	dSPIN_Hard_HiZ(dSPIN_Device_Y);	
	dSPIN_Hard_HiZ(dSPIN_Device_Z);
	
	/* X轴参数设定 */
	if(motor_set(&dSPIN_RegsStruct, &motor_device_x))
	{
		/* 设置错误 */
		return 1;
	}
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_X);
	
	/* Y轴参数设定 */
	if(motor_set(&dSPIN_RegsStruct, &motor_device_y))
	{
		/* 设置错误 */
		return 1;
	}	
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Y);
	
	/* Z轴参数设定 */
	if(motor_set(&dSPIN_RegsStruct, &motor_device_z))
	{
		/* 设置错误 */
		return 1;
	}	
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Z);

	/* 绝对位置需清零 */
	dSPIN_Set_Param(dSPIN_ABS_POS, 0, dSPIN_Device_X);
	dSPIN_Set_Param(dSPIN_ABS_POS, 0, dSPIN_Device_Y);
	dSPIN_Set_Param(dSPIN_ABS_POS, 0, dSPIN_Device_Z);

	/* 读状态位,清除错误标志(FLAG) */
	dSPIN_Get_Status(dSPIN_Device_X);
	dSPIN_Get_Status(dSPIN_Device_Y);
	dSPIN_Get_Status(dSPIN_Device_Z);

	return 0;
}


/* 获取电机错误原因 
   * param: Device
   *retval: 0->没有错误; 其他->错误类型
*/
INT8U motor_erro_info(dSPIN_Device_TypeDef Device)
{
	INT16U error;
	MOTOR_ERROR motor_err = motor_err_null;
	
	/* 读取电机状态 
       *读状态后FLAG变为高电平
	*/
	error = dSPIN_Get_Status(Device);

	/* 错误指令 */
	if(error & 0x0180)
	{
		utPtn("__错误指令或有不可执行的指令\r\n");
		motor_err = motor_err_wrong_nperf_cmd;
	}	

	/* 温度预警 */
	if(!(error & 0x0400))
	{
		utPtn("__温度预警\r\n");
		motor_err = motor_err_thermal_warning;
	}
	
	/* 温度过高关闭 */
	if(!(error & 0x0800))
	{
		utPtn("__温度过高关闭\r\n");
		motor_err = motor_err_thermal_shutdown;
	}

	/* 欠压锁定 */
	if(!(error & 0x0200))
	{
		utPtn("__欠压锁定\r\n");
		motor_err = motor_err_under_voltage;
	}

	/* 失步 */
	if(!(error & 0x6000))
	{
		utPtn("__A或B相失步\r\n");
		motor_err = motor_err_stall_det;
	}

	/* SW闭合 */
	if(error & 0x08)
	{
		utPtn("__SW闭合\r\n");
		motor_err = motor_err_sw_on;
	}
	
	/* 过流 */
	if(!(error & 0x1000))
	{
		utPtn("__过流\r\n");
		motor_err = motor_err_overcurrent;
	}

	return motor_err;
}

/* 获取电机状态 
   *param: 电机号
   *retval: 电机状态
   		*motor_stopped.
   		*motor_acc.
   		*motor_dec.
   		*motor_con.
*/
INT8U sampler_get_motor_state(dSPIN_Device_TypeDef Device)
{
	MOTOR_STATE current_state = motor_stopped;
	INT16U motor_status = 0;

	/* 读取状态寄存器 */
	motor_status = dSPIN_Get_Param(dSPIN_STATUS, Device);

	/* 取电机状态位 */
	motor_status = (motor_status & 0x0060) >> 5;

	switch(motor_status)
	{
		/* 电机停止 */
		case motor_stopped:
			current_state = motor_stopped;
			break;

		/* 电机加速 */
		case motor_acceleration:
			current_state = motor_acceleration;
			break;

		/* 电机减速 */
		case motor_deceleration:
			current_state = motor_deceleration;
			break;

		/* 电机恒速 */
		case motor_constant_speed:
			current_state = motor_constant_speed;
			break;
	}
	
	return current_state;
}

/*****************************@ ShowFlake @*******************************/
