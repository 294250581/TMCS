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

