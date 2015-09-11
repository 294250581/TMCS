#ifndef __TMCS_BSP_H__
#define __TMCS_BSP_H__

#include "dSPIN.h"
#include "motor.h"
#include "Encoder.h"

enum
{
	ke           = 0x00, /* 反电动势常数 */
	inductance   = 0x01, /* 电机相电感 */
	resistance   = 0x02, /* 电机相电阻 */
	k_therm      = 0x03, /* 相电阻温度补偿 */
	hold_current = 0x04, /* 锁定电流 A r.m.s */
	acc_current  = 0x05, /* 加速电流 A r.m.s */
	dec_current  = 0x06, /* 减速电流 A r.m.s */
	run_current  = 0x07, /* 恒速运行电流 A r.m.s */
	acc_speed    = 0x08, /* 加速度 */
	dec_speed    = 0x09, /* 减速度 */
	max_speed    = 0x0A, /* 最高速度 */
	min_speed    = 0x0B, /* 最小速度 */
	full_speed   = 0x0C, /* 阈值速度 */
	stall_th     = 0x0D, /* 失步电流 */
	over_current = 0x0E, /* 电流阈值 */
	step_mode    = 0x0F, /* 细分值 */
};

enum
{
	rfsn_stop = 0, /* 等待复位指令 */
	rfsn_xres = 1, /* x轴复位 */
	rfsn_yres = 2, /* y轴复位 */
	rfsn_zres = 3, /* Z轴复位 */
};

enum
{
	rfsc_stop = 0, /* 等待复位指令 */
	rfsc_xres = 1, /* x轴复位 */
	rfsc_yres = 2, /* y轴复位 */
	rfsc_zres = 3, /* z轴复位 */
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



















