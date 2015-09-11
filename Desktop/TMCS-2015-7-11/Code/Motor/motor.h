/***********************************************
FileName: motor.h
Author:   Tsui
Company:  SnowFlake
Version:  1.0
Date:     2015-1-6
Description: motor.cͷ�ļ�
Function List: 
***********************************************/
#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "dSPIN.h"

/* �����ѹ */
#define voltage_supply 24

/* ��������ض��� */
#define X_POSITIVE FWD
#define X_NEGATIVE REV

#define Y_POSITIVE FWD
#define Y_NEGATIVE REV

#define Z_POSITIVE FWD
#define Z_NEGATIVE REV

#define S_POSITIVE REV
#define S_NEGATIVE FWD

/* ����λ�����ֵ */
#define ABS_FULL_SCALE  4194304

/*************************************/

/* X��΢�� */
#define X_MICROSTEP_N   dSPIN_Move(X_NEGATIVE, 50, dSPIN_Device_X);
#define X_MICROSTEP_P   dSPIN_Move(X_POSITIVE, 50, dSPIN_Device_X);
/* Y��΢�� */
#define Y_MICROSTEP_N   dSPIN_Move(Y_NEGATIVE, 50, dSPIN_Device_Y);
#define Y_MICROSTEP_p   dSPIN_Move(Y_POSITIVE, 50, dSPIN_Device_Y);
/* Z��΢�� */
#define Z_MICROSTEP_N   dSPIN_Move(Z_NEGATIVE, 10, dSPIN_Device_Z);
#define Z_MICROSTEP_P   dSPIN_Move(Z_POSITIVE, 10, dSPIN_Device_Z);

/* ��������ṹ�� */
struct _m_motor_app_param
{
	float ke;              /* ���綯�Ƴ��� */
	float motor_inductance;/* ������� */
	float motor_resistance;/* �������� */
	INT8U k_therm;         /* ������¶Ȳ��� */
	
	float hold_current;  /* ��������A r.m.s */
	float acc_current;   /* ���ٶȵ��� A r.m.s */
	float dec_current;   /* ���ٶȵ��� A r.m.s */
	float run_current;   /* �������е��� A r.m.s */

	float acc_speed;    /* ���ٶ�(stap/s^2) */
	float dec_speed;    /* ���ٶ�(stap/s^2) */
	
	float max_speed;    /* ����ٶ�step/s */
	float min_speed;    /* ��С�ٶ�step/s */
	float full_speed;   /* ��ֵ�ٶ�step/s */

	INT16U stall_th;     /* ʧ������ */
	INT8U over_current;  /* ������ֵ(��������) */
	INT8U step_mode;     /* ϸ��ֵ */
};

/* У׼λ����ʱ��� */
struct _m_cal_temp
{
	INT32U x_cal_temp;
	INT32U y_cal_temp;
	INT32U z_cal_temp;
};

/* У׼���� */
struct _m_cal_param
{
	INT32U x_cal_param;   /* X */
	INT32U y_cal_param;   /* Y */

	INT32U sx_cal_param;  /* Load */
	INT32U sy_cal_param;
	
	INT32U wx_cal_param;  /* ϴ�� */
	INT32U wy_cal_param;
	
	INT32U z_cal_param;   /* Zȡ�� */
	INT32U zv_cal_param;  /* Zload */
	INT32U zw_cal_param;  /* Zwash */
};

/* ��Ʒ�̾���λ�� */
struct _m_plate_pos
{
	INT32U x_plate_pos;
	INT32U y_plate_pos;
};

/* ִ��λ�úͽ����� */
struct _m_execute_param
{
	INT8U x_execute_pos;
	INT8U y_execute_pos;
	INT8U sample_volume;
};

/* ����������� */
typedef enum
{
	motor_err_null,                   /* û�д��� */
	motor_err_overcurrent,            /* ���� */
	motor_err_thermal_shutdown,       /* ���¹ر� */
	motor_err_thermal_warning,        /* ����Ԥ�� */
	motor_err_under_voltage,          /* Ƿѹ���� */
	motor_err_stall_det,              /* ʧ�� */
	motor_err_sw_on,                  /* SW�պ�(���������,��λ����) */
	motor_err_wrong_nperf_cmd,        /* ����򲻿�ִ��ָ�� */
}MOTOR_ERROR;

/* ���״̬ */
typedef enum
{
	motor_stopped,           /* ���ֹͣ */
	motor_acceleration,      /* ������� */
	motor_deceleration,      /* ������� */
	motor_constant_speed,    /* ������� */
}MOTOR_STATE;

/* 6ͨ��λ�� */
typedef enum
{	
	motor_s_null = 0,
	motor_s_load = 1,        /* ȡ��λ�� */
	motor_s_inject = 2,      /* ע��λ�� */
	motor_s_run_load = 3,    /* ����ȡ��λ���� */
	motor_s_run_inject = 4,  /* ����ע��λ���� */
}MOTOR_S_STATE;

/* Z��λ�� */
typedef enum
{
	motor_z_pos_sampler,
	motor_z_pos_v,
	motor_z_pos_w,
}MOTOS_Z_POS;

extern struct _m_cal_temp cal_temp;
extern struct _m_cal_param cal_param;
extern struct _m_motor_app_param motor_device_x;
extern struct _m_motor_app_param motor_device_y;
extern struct _m_motor_app_param motor_device_z;
extern struct _m_motor_app_param motor_device_s;
extern struct _m_plate_pos plate_pos[6][12];
extern struct _m_execute_param execute_param;

extern INT8U start_sys_alarm;
extern INT8U start_xy_reset;
extern INT8U start_z_reset;
extern INT8U start_motor_s;
extern INT8U start_cal_xy;
extern INT8U start_cal_s;
extern INT8U start_cal_wash;
extern INT8U start_cal_z_sample;
extern INT8U start_cal_z_s;
extern INT8U start_cal_z_wash;
extern INT8U start_s_reset;
extern INT8U test_plate_pos;
extern INT8U start_execute_process;
extern INT8U syringe_motor_state;

extern INT8U end_cal_xy;
extern INT8U end_cal_z_sample;
extern INT8U end_cal_z_s;
extern INT8U end_cal_z_wash;
extern INT8U end_cal_s;
extern INT8U end_cal_wash;
extern INT8U end_xy_reset;
extern INT8U end_z_reset;
extern INT8U end_s_select;

INT8U motor_init_set(void);
INT8U motor_param_set(void);
INT8U motor_erro_info(dSPIN_Device_TypeDef Device);
INT8U sampler_get_motor_state(dSPIN_Device_TypeDef Device);

#endif

/*****************************@ ShowFlake @*******************************/
