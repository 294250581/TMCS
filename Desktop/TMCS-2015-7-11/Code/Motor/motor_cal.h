/***********************************************
FileName: motor_cal.h
Author:   Tsui
Company:  SnowFlake
Version:  1.0
Date:     2015-1-6
Description: motor.h
Function List: 
***********************************************/
#ifndef __MOTOR_CAL_H__
#define __MOTOR_CAL_H__
#include "motor.h"

#define M_PI 3.1415

INT8U cal_kval_hold(struct _m_motor_app_param *motor_param);
INT8U cal_kval_acc(struct _m_motor_app_param *motor_param);
INT8U cal_kval_dec(struct _m_motor_app_param *motor_param);
INT8U cal_st_slp(struct _m_motor_app_param *motor_param);
INT16U cal_int_speed(struct _m_motor_app_param *motor_param);
INT8U cal_fn_slp_acc(struct _m_motor_app_param *motor_param);
INT8U cal_fn_slp_dec(struct _m_motor_app_param *motor_param);
INT8U cal_kval_run(struct _m_motor_app_param *motor_param);
INT8U motor_set(dSPIN_RegsStruct_TypeDef *motor_set, struct _m_motor_app_param *motor_param);

#endif

/*****************************@ ShowFlake @*******************************/
