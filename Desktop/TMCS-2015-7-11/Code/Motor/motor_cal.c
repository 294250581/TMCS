/*****************************************************************
FileName: motor_cal.c
Author:   Tsui
Company:  SnowFlake
Version:  1.0
Date:     2015-1-6
Description: motor.c
	used to calculate the parameter value of L6470 
Function List: 
	INT8U cal_kval_hold(struct _m_motor_app_param *motor_param);
	INT8U cal_kval_acc(struct _m_motor_app_param *motor_param);
	INT8U cal_kval_dec(struct _m_motor_app_param *motor_param);
	INT8U cal_st_slp(struct _m_motor_app_param *motor_param);
	INT16U cal_int_speed(struct _m_motor_app_param *motor_param);
	INT8U cal_fn_slp_acc(struct _m_motor_app_param *motor_param);
	INT8U cal_fn_slp_dec(struct _m_motor_app_param *motor_param); 	INT8U cal_kval_run(struct _m_motor_app_param *motor_param);	
******************************************************************/
#include "motor_cal.h"

/* Kval_Hold 
   *param: _m_motor_app_param
   *retval: Kval_Hold
*/
INT8U cal_kval_hold(struct _m_motor_app_param *motor_param)
{
	INT8U kval_hold;
	float rm; 
	float i_hold; 	

	rm = motor_param->motor_resistance;
	i_hold = motor_param->hold_current;

	kval_hold = floor((rm*i_hold/voltage_supply)*256+0.5);

	return kval_hold;
}

/* Kval_Acc 
   *param: _m_motor_app_param
   *retval: Kval_Acc
*/
INT8U cal_kval_acc(struct _m_motor_app_param *motor_param)
{
	INT8U kval_acc;
	float rm;
	float i_acc;

	rm = motor_param->motor_resistance;
	i_acc = motor_param->acc_current;

	kval_acc = floor((rm*i_acc/(voltage_supply*0.707))*256+0.5);

	return kval_acc;
}

/* Kval_Dec 
   *param: _m_motor_app_param
   *retval: Kval_Dec
*/
INT8U cal_kval_dec(struct _m_motor_app_param *motor_param)
{
	INT8U kval_dec;
	float rm;
	float i_dec;

	rm = motor_param->motor_resistance;
	i_dec = motor_param->dec_current;

	kval_dec = floor((rm*i_dec/(voltage_supply*0.707))*256+0.5);

	return kval_dec;
}

/* St_Slp 
   *param: _m_motor_app_param
   *retval: St_Slp
*/
INT8U cal_st_slp(struct _m_motor_app_param *motor_param)
{
	float ke;
	INT8U st_slp;

	ke = motor_param->ke;
	st_slp = floor(((ke/4)/voltage_supply)*65536+0.5);
	
	return st_slp;
}

/* Int_Speed 
   *param: _m_motor_app_param
   *retval: Int_Speed
*/
INT16U cal_int_speed(struct _m_motor_app_param *motor_param)
{
	float rm;
	float lm;
	INT16U int_speed;

	rm = motor_param->motor_resistance;
	lm = motor_param->motor_inductance;

	int_speed = floor((4*rm/(2*M_PI*lm*0.001))*16.777+0.5);

	return int_speed;
}

/* Fn_Slp_Acc 
   *param: _m_motor_app_param
   *retval: Fn_Slp_Acc
*/
INT8U cal_fn_slp_acc(struct _m_motor_app_param *motor_param)
{
	float ke;
	float i_acc;
	float lm;
	INT8U fn_slp_acc;

	ke = motor_param->ke;
	i_acc = motor_param->acc_current;
	lm = motor_param->motor_inductance;

	fn_slp_acc = floor(((2*M_PI*lm*0.001*i_acc/0.707+ke)/4/voltage_supply)*65536+0.5);

	return fn_slp_acc;
}

/* Fn_Slp_dec 
   *param: _m_motor_app_param
   *retval: Fn_Slp_Dec
*/
INT8U cal_fn_slp_dec(struct _m_motor_app_param *motor_param)
{
	float ke;
	float i_dec;
	float lm;
	INT8U fn_slp_dec;

	ke = motor_param->ke;
	i_dec = motor_param->dec_current;
	lm = motor_param->motor_inductance;

	fn_slp_dec = floor(((2*M_PI*lm*0.001*i_dec/0.707+ke)/4/voltage_supply)*65536+0.5);

	return fn_slp_dec;
}

/* Kval_Run
   *param: _m_motor_app_param
   *retval: Kval_Run
*/
INT8U cal_kval_run(struct _m_motor_app_param *motor_param)
{
	float i_run;
	float i_acc;
	float i_dec;
	float rm;
	float lm;
	float ke;
	float step;
	INT8U kval_run;

	float kval_acc_p;
	float kval_run_p;
	float int_speed_p;
	float st_slp_p;
	float fn_slp_acc_p;
	float vout;

	i_run = motor_param->run_current;
	i_acc = motor_param->acc_current;
	i_dec = motor_param->dec_current;

	/* Keep_target Current */
	if(i_run == i_acc && i_acc == i_dec)
	{
		kval_run = cal_kval_acc(motor_param);
	}
	/* different target Current */
	else
	{
		rm = motor_param->motor_resistance;
		lm = motor_param->motor_inductance;
		ke = motor_param->ke;
		step = motor_param->max_speed;
		i_run = motor_param->run_current;
		i_acc = motor_param->acc_current;
		i_dec = motor_param->dec_current;
		
		kval_acc_p  = rm*i_acc*100/(0.707*voltage_supply);
		int_speed_p = 4*rm/(2*M_PI*lm*0.001);
		st_slp_p    = ke*100/4/voltage_supply;
		fn_slp_acc_p= (2*M_PI*lm*0.001*i_acc/0.707+ke)*100/(4*voltage_supply);
		vout = 2*M_PI*step*lm*0.001*i_run/2.828+ke*step/4;
		kval_run_p = (kval_acc_p+vout*100/voltage_supply-(fn_slp_acc_p*step+\
			          st_slp_p*int_speed_p+kval_acc_p-fn_slp_acc_p*int_speed_p));
		kval_run = floor(kval_run_p/100*256+0.5);
	}
	return kval_run;
}

/* 配置参数
   *param_1: dSPIN_RegsStruct_TypeDef
   *param: *motor_param
   *retval: 0->设置正确;其他->错误
*/
INT8U motor_set(dSPIN_RegsStruct_TypeDef *motor_set, struct _m_motor_app_param *motor_param)
{
	/* 基本参数设置 */
	motor_set->ACC = AccDec_Steps_to_Par(motor_param->acc_speed);
	motor_set->DEC = AccDec_Steps_to_Par(motor_param->dec_speed);
	motor_set->MAX_SPEED = MaxSpd_Steps_to_Par(motor_param->max_speed);
	motor_set->MIN_SPEED = MinSpd_Steps_to_Par(motor_param->min_speed);
	motor_set->FS_SPD = FSSpd_Steps_to_Par(motor_param->full_speed);
	motor_set->KVAL_HOLD = cal_kval_hold(motor_param);
	motor_set->KVAL_RUN = cal_kval_run(motor_param);
	motor_set->KVAL_ACC = cal_kval_acc(motor_param);
	motor_set->KVAL_DEC = cal_kval_dec(motor_param);
	motor_set->INT_SPD = cal_int_speed(motor_param);
	motor_set->ST_SLP = cal_st_slp(motor_param);
	motor_set->FN_SLP_ACC = cal_fn_slp_acc(motor_param);
	motor_set->FN_SLP_DEC = cal_fn_slp_dec(motor_param);
	motor_set->K_THERM = KTherm_to_Par(motor_param->k_therm);
	motor_set->OCD_TH = motor_param->over_current;
	motor_set->STALL_TH = StallTh_to_Par(motor_param->stall_th);
	motor_set->STEP_MODE = motor_param->step_mode;

	/* 设置警报位 
	   *过流;温度;欠压;失速检测;SW;错误指令
	*/
	#if 1
    motor_set->ALARM_EN 	= dSPIN_ALARM_EN_OVERCURRENT | dSPIN_ALARM_EN_THERMAL_SHUTDOWN
		| dSPIN_ALARM_EN_THERMAL_WARNING | dSPIN_ALARM_EN_UNDER_VOLTAGE | dSPIN_ALARM_EN_STALL_DET_A
		| dSPIN_ALARM_EN_STALL_DET_B | dSPIN_ALARM_EN_SW_TURN_ON | dSPIN_ALARM_EN_WRONG_NPERF_CMD;
	#endif

	/* Internal oscillator, 2MHz OSCOUT clock, supply voltage compensation disabled, *
	 * overcurrent shutdown enabled, slew-rate = 260 V/us, PWM frequency = 15.6kHz   */
	motor_set->CONFIG 	= dSPIN_CONFIG_INT_16MHZ_OSCOUT_2MHZ | dSPIN_CONFIG_SW_USER
		| dSPIN_CONFIG_VS_COMP_DISABLE | dSPIN_CONFIG_OC_SD_ENABLE | dSPIN_CONFIG_SR_260V_us
		| dSPIN_CONFIG_PWM_DIV_1 | dSPIN_CONFIG_PWM_MUL_1_25;

	return 0;
}


/*****************************@ ShowFlake @*******************************/
