/***********************************************
FileName: motor.h
Author:   Tsui
Company:  SnowFlake
Version:  1.0
Date:     2015-1-6
Description: motor.c头文件
Function List: 
***********************************************/
#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "dSPIN.h"

/* 供电电压 */
#define voltage_supply 24

/* 电机方向重定义 */
#define X_POSITIVE FWD
#define X_NEGATIVE REV

#define Y_POSITIVE FWD
#define Y_NEGATIVE REV

#define Z_POSITIVE FWD
#define Z_NEGATIVE REV

#define S_POSITIVE REV
#define S_NEGATIVE FWD

/* 绝对位置最大值 */
#define ABS_FULL_SCALE  4194304

/*************************************/

/* X轴微步 */
#define X_MICROSTEP_N   dSPIN_Move(X_NEGATIVE, 50, dSPIN_Device_X);
#define X_MICROSTEP_P   dSPIN_Move(X_POSITIVE, 50, dSPIN_Device_X);
/* Y轴微步 */
#define Y_MICROSTEP_N   dSPIN_Move(Y_NEGATIVE, 50, dSPIN_Device_Y);
#define Y_MICROSTEP_p   dSPIN_Move(Y_POSITIVE, 50, dSPIN_Device_Y);
/* Z轴微步 */
#define Z_MICROSTEP_N   dSPIN_Move(Z_NEGATIVE, 10, dSPIN_Device_Z);
#define Z_MICROSTEP_P   dSPIN_Move(Z_POSITIVE, 10, dSPIN_Device_Z);

/* 电机参数结构体 */
struct _m_motor_app_param
{
	float ke;              /* 反电动势常数 */
	float motor_inductance;/* 电机相电感 */
	float motor_resistance;/* 电机相电阻 */
	INT8U k_therm;         /* 相电阻温度补偿 */
	
	float hold_current;  /* 锁定电流A r.m.s */
	float acc_current;   /* 加速度电流 A r.m.s */
	float dec_current;   /* 减速度电流 A r.m.s */
	float run_current;   /* 恒速运行电流 A r.m.s */

	float acc_speed;    /* 加速度(stap/s^2) */
	float dec_speed;    /* 减速度(stap/s^2) */
	
	float max_speed;    /* 最高速度step/s */
	float min_speed;    /* 最小速度step/s */
	float full_speed;   /* 阈值速度step/s */

	INT16U stall_th;     /* 失步电流 */
	INT8U over_current;  /* 电流阈值(过流电流) */
	INT8U step_mode;     /* 细分值 */
};

/* 校准位置临时存放 */
struct _m_cal_temp
{
	INT32U x_cal_temp;
	INT32U y_cal_temp;
	INT32U z_cal_temp;
};

/* 校准参数 */
struct _m_cal_param
{
	INT32U x_cal_param;   /* X */
	INT32U y_cal_param;   /* Y */

	INT32U sx_cal_param;  /* Load */
	INT32U sy_cal_param;
	
	INT32U wx_cal_param;  /* 洗针 */
	INT32U wy_cal_param;
	
	INT32U z_cal_param;   /* Z取样 */
	INT32U zv_cal_param;  /* Zload */
	INT32U zw_cal_param;  /* Zwash */
};

/* 样品盘绝对位置 */
struct _m_plate_pos
{
	INT32U x_plate_pos;
	INT32U y_plate_pos;
};

/* 执行位置和进样量 */
struct _m_execute_param
{
	INT8U x_execute_pos;
	INT8U y_execute_pos;
	INT8U sample_volume;
};

/* 电机故障类型 */
typedef enum
{
	motor_err_null,                   /* 没有错误 */
	motor_err_overcurrent,            /* 过流 */
	motor_err_thermal_shutdown,       /* 高温关闭 */
	motor_err_thermal_warning,        /* 高温预警 */
	motor_err_under_voltage,          /* 欠压锁定 */
	motor_err_stall_det,              /* 失步 */
	motor_err_sw_on,                  /* SW闭合(不能算错误,限位开关) */
	motor_err_wrong_nperf_cmd,        /* 错误或不可执行指令 */
}MOTOR_ERROR;

/* 电机状态 */
typedef enum
{
	motor_stopped,           /* 电机停止 */
	motor_acceleration,      /* 电机加速 */
	motor_deceleration,      /* 电机减速 */
	motor_constant_speed,    /* 电机恒速 */
}MOTOR_STATE;

/* 6通阀位置 */
typedef enum
{	
	motor_s_null = 0,
	motor_s_load = 1,        /* 取样位置 */
	motor_s_inject = 2,      /* 注射位置 */
	motor_s_run_load = 3,    /* 查找取样位置中 */
	motor_s_run_inject = 4,  /* 查找注射位置中 */
}MOTOR_S_STATE;

/* Z轴位置 */
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
