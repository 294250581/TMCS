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

/* �����ʼ���趨 
   *param: None
   *retval: 0->�趨�ɹ�;����->�趨ʧ��
*/
INT8U motor_init_set(void)
{
	/* �Ĵ�����λ */
	dSPIN_Regs_Struct_Reset(&dSPIN_RegsStruct);
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_X);
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Y);
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Z);

	/* X������趨 */
	if(motor_set(&dSPIN_RegsStruct, &motor_device_x))
	{
		/* ���ô��� */
		return 1;
	}
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_X);
	
	/* Y������趨 */
	if(motor_set(&dSPIN_RegsStruct, &motor_device_y))
	{
		/* ���ô��� */
		return 1;
	}	
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Y);
	
	/* Z������趨 */
	if(motor_set(&dSPIN_RegsStruct, &motor_device_z))
	{
		/* ���ô��� */
		return 1;
	}	
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Z);

	/* ����λ�������� */
	dSPIN_Set_Param(dSPIN_ABS_POS, 0, dSPIN_Device_X);
	dSPIN_Set_Param(dSPIN_ABS_POS, 0, dSPIN_Device_Y);
	dSPIN_Set_Param(dSPIN_ABS_POS, 0, dSPIN_Device_Z);

	/* ��״̬λ,��������־(FLAG) */
	dSPIN_Get_Status(dSPIN_Device_X);
	dSPIN_Get_Status(dSPIN_Device_Y);
	dSPIN_Get_Status(dSPIN_Device_Z);

	/* ������� */
	dSPIN_Soft_Stop(dSPIN_Device_X);
	dSPIN_Soft_Stop(dSPIN_Device_Y);
	dSPIN_Soft_Stop(dSPIN_Device_Z);

	return 0;
}

/* motor set */
INT8U motor_param_set(void)
{
	/* �Ĵ�����λ */
	dSPIN_Regs_Struct_Reset(&dSPIN_RegsStruct);
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_X);
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Y);
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Z);

	dSPIN_Hard_HiZ(dSPIN_Device_X);
	dSPIN_Hard_HiZ(dSPIN_Device_Y);	
	dSPIN_Hard_HiZ(dSPIN_Device_Z);
	
	/* X������趨 */
	if(motor_set(&dSPIN_RegsStruct, &motor_device_x))
	{
		/* ���ô��� */
		return 1;
	}
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_X);
	
	/* Y������趨 */
	if(motor_set(&dSPIN_RegsStruct, &motor_device_y))
	{
		/* ���ô��� */
		return 1;
	}	
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Y);
	
	/* Z������趨 */
	if(motor_set(&dSPIN_RegsStruct, &motor_device_z))
	{
		/* ���ô��� */
		return 1;
	}	
	dSPIN_Registers_Set(&dSPIN_RegsStruct, dSPIN_Device_Z);

	/* ����λ�������� */
	dSPIN_Set_Param(dSPIN_ABS_POS, 0, dSPIN_Device_X);
	dSPIN_Set_Param(dSPIN_ABS_POS, 0, dSPIN_Device_Y);
	dSPIN_Set_Param(dSPIN_ABS_POS, 0, dSPIN_Device_Z);

	/* ��״̬λ,��������־(FLAG) */
	dSPIN_Get_Status(dSPIN_Device_X);
	dSPIN_Get_Status(dSPIN_Device_Y);
	dSPIN_Get_Status(dSPIN_Device_Z);

	return 0;
}


/* ��ȡ�������ԭ�� 
   * param: Device
   *retval: 0->û�д���; ����->��������
*/
INT8U motor_erro_info(dSPIN_Device_TypeDef Device)
{
	INT16U error;
	MOTOR_ERROR motor_err = motor_err_null;
	
	/* ��ȡ���״̬ 
       *��״̬��FLAG��Ϊ�ߵ�ƽ
	*/
	error = dSPIN_Get_Status(Device);

	/* ����ָ�� */
	if(error & 0x0180)
	{
		utPtn("__����ָ����в���ִ�е�ָ��\r\n");
		motor_err = motor_err_wrong_nperf_cmd;
	}	

	/* �¶�Ԥ�� */
	if(!(error & 0x0400))
	{
		utPtn("__�¶�Ԥ��\r\n");
		motor_err = motor_err_thermal_warning;
	}
	
	/* �¶ȹ��߹ر� */
	if(!(error & 0x0800))
	{
		utPtn("__�¶ȹ��߹ر�\r\n");
		motor_err = motor_err_thermal_shutdown;
	}

	/* Ƿѹ���� */
	if(!(error & 0x0200))
	{
		utPtn("__Ƿѹ����\r\n");
		motor_err = motor_err_under_voltage;
	}

	/* ʧ�� */
	if(!(error & 0x6000))
	{
		utPtn("__A��B��ʧ��\r\n");
		motor_err = motor_err_stall_det;
	}

	/* SW�պ� */
	if(error & 0x08)
	{
		utPtn("__SW�պ�\r\n");
		motor_err = motor_err_sw_on;
	}
	
	/* ���� */
	if(!(error & 0x1000))
	{
		utPtn("__����\r\n");
		motor_err = motor_err_overcurrent;
	}

	return motor_err;
}

/* ��ȡ���״̬ 
   *param: �����
   *retval: ���״̬
   		*motor_stopped.
   		*motor_acc.
   		*motor_dec.
   		*motor_con.
*/
INT8U sampler_get_motor_state(dSPIN_Device_TypeDef Device)
{
	MOTOR_STATE current_state = motor_stopped;
	INT16U motor_status = 0;

	/* ��ȡ״̬�Ĵ��� */
	motor_status = dSPIN_Get_Param(dSPIN_STATUS, Device);

	/* ȡ���״̬λ */
	motor_status = (motor_status & 0x0060) >> 5;

	switch(motor_status)
	{
		/* ���ֹͣ */
		case motor_stopped:
			current_state = motor_stopped;
			break;

		/* ������� */
		case motor_acceleration:
			current_state = motor_acceleration;
			break;

		/* ������� */
		case motor_deceleration:
			current_state = motor_deceleration;
			break;

		/* ������� */
		case motor_constant_speed:
			current_state = motor_constant_speed;
			break;
	}
	
	return current_state;
}

/*****************************@ ShowFlake @*******************************/
