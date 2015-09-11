#ifndef __TMCS_BSP_H__
#define __TMCS_BSP_H__

#include "dSPIN.h"
#include "motor.h"
#include "Encoder.h"

enum
{
	ke           = 0x00, /* ���綯�Ƴ��� */
	inductance   = 0x01, /* ������� */
	resistance   = 0x02, /* �������� */
	k_therm      = 0x03, /* ������¶Ȳ��� */
	hold_current = 0x04, /* �������� A r.m.s */
	acc_current  = 0x05, /* ���ٵ��� A r.m.s */
	dec_current  = 0x06, /* ���ٵ��� A r.m.s */
	run_current  = 0x07, /* �������е��� A r.m.s */
	acc_speed    = 0x08, /* ���ٶ� */
	dec_speed    = 0x09, /* ���ٶ� */
	max_speed    = 0x0A, /* ����ٶ� */
	min_speed    = 0x0B, /* ��С�ٶ� */
	full_speed   = 0x0C, /* ��ֵ�ٶ� */
	stall_th     = 0x0D, /* ʧ������ */
	over_current = 0x0E, /* ������ֵ */
	step_mode    = 0x0F, /* ϸ��ֵ */
};

enum
{
	rfsn_stop = 0, /* �ȴ���λָ�� */
	rfsn_xres = 1, /* x�Ḵλ */
	rfsn_yres = 2, /* y�Ḵλ */
	rfsn_zres = 3, /* Z�Ḵλ */
};

enum
{
	rfsc_stop = 0, /* �ȴ���λָ�� */
	rfsc_xres = 1, /* x�Ḵλ */
	rfsc_yres = 2, /* y�Ḵλ */
	rfsc_zres = 3, /* z�Ḵλ */
};

extern INT8U tmcs_rfsn_ctr;
extern INT8U tmcs_rfsc_ctr;

void tmcs_bsp_ror(dSPIN_Device_TypeDef motor, INT32U speed);
void tmcs_bsp_rol(dSPIN_Device_TypeDef motor, INT32U speed);
void tmcs_bsp_msp(dSPIN_Device_TypeDef motor, INT8U mode);
void tmcs_bsp_sap(dSPIN_Device_TypeDef motor, INT8U type, INT32U value);
void tmcs_bsp_abs(INT32S parameter, dSPIN_Device_TypeDef motor);
void tmcs_bsp_rel(INT32S parameter, dSPIN_Device_TypeDef motor);
INT8U tmcs_bsp_rfsn(void);
INT8U tmcs_bsp_rfsc(void);

#endif



















