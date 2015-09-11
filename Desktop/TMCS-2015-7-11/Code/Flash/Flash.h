#ifndef __FLASH_H__
#define __FALSH_H__
#include "utGlobals.h"
#include "Tmcs_process.h"
#include "motor.h"
#include "Iobus.h"

struct _t_save_COORD
{
	INT32S t_save_ABS[60];
	INT32S t_save_ENC[60];
};

struct _t_save_motor
{
	struct _t_save_COORD t_save_COORD;
};

struct _t_save_axis
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

struct _t_save_iobus
{	
	INT8U io_mode;    /* input or output */
	INT8U io_int;     /* interrupt on or off */
	INT8U io_intmode; /* int mode <Rising Falling Randf> */
};

/**********************************************/
BOOLEAN utIsStored(void);
void utGetData(void);
void utStoreData(void);
void DetectCal(void);
void save_value(void);
/**********************************************/
#endif

