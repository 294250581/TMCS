//==============================================================================
//头文件
#ifndef _utPID_H
#define _utPID_H

//==============================================================================
//包含全局定义变量文件
#include "utGlobals.h"

#define PID_TYPE	float

#define PID_UD_FILT_NUM		1
#define PID_OUT_FILT_NUM	1

#define PID_Ts	1.0				//采样时间
#define PID_Ti	50				//积分时间常数，多少时刻累积的误差开始生效，跟系统的滞后常数有关
#define PID_Td	3				//微分时间常数，相当于本来该几个时刻处理的事情要在这一时刻处理掉
#define PID_MAX	10				//最大输出
#define PID_Kp	2.0*0.6			//放大的倍数乘以0.6

#define PID_Ki  (PID_Ts/PID_Ti)	//1除以时间常数秒
#define PID_Kd	(PID_Td/PID_Ts)

typedef struct
{
    PID_TYPE set; 		// 设定值
    PID_TYPE uin,uout;
	
	PID_TYPE umax;
	PID_TYPE umin;

    PID_TYPE Kp; 		// 比例常数 
    PID_TYPE Ki; 		// 积分常数 
    PID_TYPE Kd; 		// 微分常数 

    PID_TYPE curErr; 	//保留2个时刻的误差
    PID_TYPE preErr; 	//保留2个时刻的误差
    PID_TYPE curdErr;	//保留2个误差的变化
    PID_TYPE predErr;	//保留2个误差的变化 

	#if (PID_UD_FILT_NUM > 1)
	PID_TYPE predUd;	//先前的微分项输出
	#endif

	#if (PID_OUT_FILT_NUM > 1)
	PID_TYPE predU;		//先前整体增量PID输出
	#endif
} utPID_DATA;

//==============================================================================
//外部符号预定义

//==============================================================================
//外部调用变量

//==============================================================================
//使用的外部变量

//==============================================================================
//外部调用函数

#endif
//==============================================================================
//end of the file

