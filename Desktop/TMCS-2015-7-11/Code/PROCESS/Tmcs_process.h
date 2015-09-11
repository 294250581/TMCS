#ifndef __TMCS_PROCESS_H__
#define __TMCS_PROCESS_H__

#include "utGlobals.h"
#include "stm32f10x.h"
#include "Tmcs_bsp.h"
#include "Iobus.h"

/* 位置参数
   <! "ABS"--电机绝对位置参数
   <! "ENC"--电机位置编码器参数
*/
struct _t_tmcs_COORD
{
	INT32S t_tmcs_ABS[60];
	INT32S t_tmcs_ENC[60];
};

/* 电机结构体参数 */
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
	soft_stop = 0x00,   /* 减速停止,并锁定 */
	hard_stop = 0x01,   /* 直接停止,并锁定 */
	soft_hiz  = 0x02,   /* 减速停止,不锁定 */
	hard_hiz  = 0x03,   /* 直接停止,不锁定 */
};

enum
{
	rfs_normal  = 0x00, /* 单次归零 */
	rfs_confirm = 0x01, /* 二次归零 */
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


