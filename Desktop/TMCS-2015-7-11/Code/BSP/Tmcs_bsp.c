#include "Tmcs_bsp.h"
#include <stdlib.h>

/* motor roate in right direction */
void tmcs_bsp_ror(dSPIN_Device_TypeDef motor, INT32U speed)
{
	dSPIN_Run(FWD, Speed_Steps_to_Par(speed), motor);
}

/* motor roate in left direction */
void tmcs_bsp_rol(dSPIN_Device_TypeDef motor, INT32U speed)
{
	dSPIN_Run(REV, Speed_Steps_to_Par(speed), motor);
}

void tmcs_bsp_msp(dSPIN_Device_TypeDef motor, INT8U mode)
{
	switch(mode)
	{
		/* soft stop */
		case 0x00:
			dSPIN_Soft_Stop(motor);
		break;

		/* hard stop */
		case 0x01:
			dSPIN_Hard_Stop(motor);
		break;

		/* soft hiz */
		case 0x02:
			dSPIN_Soft_HiZ(motor);
		break;

		/* hard hiz */
		case 0x03:
			dSPIN_Hard_HiZ(motor);
		break;
	}
}

void tmcs_bsp_sap(dSPIN_Device_TypeDef motor, INT8U type, INT32U value)
{
	struct _m_motor_app_param *motor_app_param;
	
	switch(motor)
	{
		case dSPIN_Device_X:
			motor_app_param = &motor_device_x;
		break;

		case dSPIN_Device_Y:
			motor_app_param = &motor_device_y;
		break;

		case dSPIN_Device_Z:
			motor_app_param = &motor_device_z;
		break;
	}
	
	switch(type)
	{
		case ke:
			utPrt("type is ke\n");
			motor_app_param->ke = ((float)value)/100000;
		break;

		case inductance:
			utPrt("type is inductance\n");
			motor_app_param->motor_inductance = ((float)value)/1000;
		break;

		case resistance:
			utPrt("type is resistance\n");
			motor_app_param->motor_resistance = ((float)value)/1000;
		break;

		case k_therm:
			utPrt("type is k_therm\n");
			motor_app_param->k_therm = (INT8U)value;
		break;

		case hold_current:
			utPrt("type is hold_current\n");
			motor_app_param->hold_current = ((float)value)/1000;
		break;

		case acc_current:
			utPrt("type is acc_current\n");
			motor_app_param->acc_current = ((float)value)/1000;
		break;

		case dec_current:
			utPrt("type is dec_current\n");
			motor_app_param->dec_current = ((float)value)/1000;
		break;

		case run_current:
			utPrt("type is run_current\n");
			motor_app_param->run_current = ((float)value)/1000;
		break;

		case acc_speed:
			utPrt("type is acc_speed\n");
			motor_app_param->acc_speed = (float)value;
		break;

		case dec_speed:
			utPrt("type is dec_speed\n");
			motor_app_param->dec_speed = (float)value;
		break;

		case max_speed:
			utPrt("type is max_speed\n");
			motor_app_param->max_speed = (float)value;
		break;
		
		case min_speed:
			utPrt("type is min_speed\n");
			motor_app_param->min_speed = (float)value;	
		break;

		case full_speed:
			utPrt("type is full_speed\n");
			motor_app_param->full_speed = (float)value;
		break;

		case stall_th:
			utPrt("type is stall_th\n");
			motor_app_param->stall_th= (INT16U)value;
		break;

		case over_current:
			utPrt("type is over_current\n");
			motor_app_param->over_current = (INT8U)value;
		break;

		case step_mode:
			utPrt("type is step_mode\n");
			motor_app_param->step_mode = (INT8U)value;
		break;
	}

	/* 重置电机参数 */
	motor_param_set();
}

/* 绝对位置 */
void tmcs_bsp_abs(INT32S parameter, dSPIN_Device_TypeDef motor)
{
	switch(motor)
	{
		case dSPIN_Device_X:
			utPrt("motor X\n");
			dSPIN_Go_To(parameter,dSPIN_Device_X);
		break;

		case dSPIN_Device_Y:
			utPrt("motor Y\n");
			dSPIN_Go_To(parameter,dSPIN_Device_Y);
		break;

		case dSPIN_Device_Z:
			utPrt("motor Z\n");
			dSPIN_Go_To(parameter,dSPIN_Device_Z);
		break;
	}
}

/* 相对位置 */
void tmcs_bsp_rel(INT32S parameter, dSPIN_Device_TypeDef motor)
{
	dSPIN_Direction_TypeDef direction;

	if(parameter < 0)
		direction = REV;
	if(parameter >=0 )
		direction = FWD;
	
	switch(motor)
	{
		case dSPIN_Device_X:
			utPrt("motor X\n");
			dSPIN_Move(direction, abs(parameter), dSPIN_Device_X);
		break;

		case dSPIN_Device_Y:
			utPrt("motor Y\n");
			dSPIN_Move(direction, abs(parameter), dSPIN_Device_Y);
		break;

		case dSPIN_Device_Z:
			utPrt("motor Z\n");
			dSPIN_Move(direction, abs(parameter), dSPIN_Device_Z);
		break;
	}
}

/* 查找零点,单次 */
INT8U tmcs_rfsn_ctr = rfsn_stop;
INT8U tmcs_bsp_rfsn(void)
{
	static dSPIN_Device_TypeDef dSPIN_Device;

	utBeginTask;

	utWait((tmcs_rfsn_ctr != 0));
	switch(tmcs_rfsn_ctr)
	{
		case rfsn_xres:
			utPrt("X reset start\n");
			dSPIN_Device = dSPIN_Device_X;
		break;
		
		case rfsn_yres:
			utPrt("Y reset start\n");
			dSPIN_Device = dSPIN_Device_Y;
		break;
		
		case rfsn_zres:
			utPrt("Z reset start\n");
			dSPIN_Device = dSPIN_Device_Z;
		break;
	}
	tmcs_rfsn_ctr = rfsn_stop;

	/*开始复位 */
	dSPIN_Go_Until(ACTION_RESET, FWD, Speed_Steps_to_Par(200), dSPIN_Device);
	/* 等待复位结束 */
	utWait((sampler_get_motor_state(dSPIN_Device) == motor_stopped));
	utPrt("Already found the Zero, motor stopped\n");
	
	dSPIN_Reset_Pos(dSPIN_Device);
		
	sprt("abs == %d\n",dSPIN_Get_Param(dSPIN_ABS_POS,dSPIN_Device));

	utWait( utSleep(50) );
	/* 编码器值归位 */
	ENC_Clean();

	utEndTask;
}

/* 查找零点,二次确认 */
INT8U tmcs_rfsc_ctr = rfsc_stop;
INT8U tmcs_bsp_rfsc(void)
{
	static dSPIN_Device_TypeDef dSPIN_Device;

	utBeginTask;

	utWait((tmcs_rfsc_ctr != 0));
	switch(tmcs_rfsc_ctr)
	{
		case rfsc_xres:
			utPrt("X reset start\n");
			dSPIN_Device = dSPIN_Device_X;
		break;
		
		case rfsc_yres:
			utPrt("Y reset start\n");
			dSPIN_Device = dSPIN_Device_Y;
		break;
		
		case rfsc_zres:
			utPrt("Z reset start\n");
			dSPIN_Device = dSPIN_Device_Z;
		break;
	}
	tmcs_rfsc_ctr = rfsc_stop;

	/*开始复位 */
	dSPIN_Go_Until(ACTION_RESET, FWD, Speed_Steps_to_Par(200), dSPIN_Device);
	/* 等待复位结束 */
	utWait((sampler_get_motor_state(dSPIN_Device) == motor_stopped));
	utPrt("Already found the Zero\n");

	/* 二次归零 */
	dSPIN_Get_Param(dSPIN_ABS_POS, dSPIN_Device);
	dSPIN_Release_SW(ACTION_RESET, REV, dSPIN_Device);
	utWait((sampler_get_motor_state(dSPIN_Device) == motor_stopped));
	utPrt("confirm zero point, motor stopped\n");

	dSPIN_Reset_Pos(dSPIN_Device);
		
	sprt("abs == %d\n",dSPIN_Get_Param(dSPIN_ABS_POS,dSPIN_Device));

	utWait( utSleep(50) );
	/* 编码器值归位 */
	ENC_Clean();

	utEndTask;	
}


