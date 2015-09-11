#ifndef __TMCS_PROCESS_H__
#define __TMCS_PROCESS_H__

#include "utGlobals.h"
#include "stm32f10x.h"
#include "Tmcs_bsp.h"
#include "Iobus.h"

/* λ�ò���
   <! "ABS"--�������λ�ò���
   <! "ENC"--���λ�ñ���������
*/
struct _t_tmcs_COORD
{
	INT32S t_tmcs_ABS[60];
	INT32S t_tmcs_ENC[60];
};

/* ����ṹ����� */
struct _t_tmcs_motor
{
	struct _t_tmcs_COORD t_tmcs_COORD;
};

enum
{
	motor_0 = 0x00,
	motor_1 = 0x01,
	motor_2 = 0x02,
};

enum
{
	soft_stop = 0x00,   /* ����ֹͣ,������ */
	hard_stop = 0x01,   /* ֱ��ֹͣ,������ */
	soft_hiz  = 0x02,   /* ����ֹͣ,������ */
	hard_hiz  = 0x03,   /* ֱ��ֹͣ,������ */
};

enum
{
	rfs_normal  = 0x00, /* ���ι��� */
	rfs_confirm = 0x01, /* ���ι��� */
}; 

enum
{
	ABS = 0x00,
	REL = 0x01,
	COORD = 0x02,
};


extern struct _t_tmcs_motor t_tmcs_motor0;
extern struct _t_tmcs_motor t_tmcs_motor1;
extern struct _t_tmcs_motor t_tmcs_motor2;

void tmcs_ror(INT8U *motor, INT8U *value);
void tmcs_rol(INT8U *motor, INT8U *value);
void tmcs_msp(INT8U *type, INT8U *motor);
void tmcs_sap(INT8U *type, INT8U *motor, INT8U *value);
void tmcs_stap(void);
void tmcs_sco(INT8U *type, INT8U *motor, INT8U *value);
void tmcs_gco(INT8U *type, INT8U *motor);
void tmcs_scos(INT8U *type, INT8U *motor, INT8U *value);
void tmcs_gcos(INT8U *type, INT8U *motor);
void tmcs_stco(void);
void tmcs_stcos(void);
void tmcs_sio(INT8U *type, INT8U *value);
void tmcs_gio(INT8U *value);
void tmcs_sioi(INT8U *type, INT8U *value);
void tmcs_rfs(INT8U *type, INT8U *motor);
void tmcs_mvp(INT8U *type, INT8U *motor, INT8U *value);

#endif


