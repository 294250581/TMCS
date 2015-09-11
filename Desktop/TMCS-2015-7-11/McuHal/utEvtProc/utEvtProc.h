#ifndef _utEVTPROC_H
#define _utEVTPROC_H

#include "utGlobals.h"

enum
{
	F_OnCONNECTED,
	F_MEDIT,
	F_OnDECTING,
	F_SCANX,
	F_SCAN0,
	F_SYNCED,	//已同步
	F_NOTHST,
	F_AUTOZERO
};//各个标志位

enum
{
	EVT_KEY1,
	EVT_KEY2
};//输入事件

enum
{
	EVT_DIS1,
	EVT_DIS2
};//输出事件

#if 1
enum
{
	ADDR_HOST = 1,
	ADDR_AUTO = 2,
	ADDR_PUMPA = 3,
	ADDR_PUMPB = 4,
	ADDR_PUMPC = 5,
	ADDR_PUMPD = 6,
	ADDR_COLL = 7,
	ADDR_DET = 8
};	

enum
{
	//0-30为主机设置和读取各种数据，70-99为同步信号发送区间，优先级均为较高
	//其它设备竞争发送。设备检测到数据就复位10ms计时器。本机发送时刻到时必须保证总线空闲了10ms
	TIME_HOST = 0,		//主机数据，优先级高
	TIME_AUTO = 30,		
	TIME_COLL = 35,
	TIME_PUMPA = 40,
	TIME_PUMPB = 45,
	TIME_PUMPC = 50,
	TIME_PUMPD = 55,
	TIME_RSV1 = 60,
	TIME_RSV2 = 65,
	#if 0				//为了避开10ms，80ms前10ms内不能定义设备
	TIME_RSV3 = 70,
	TIME_RSV4 = 75,	
	#endif
	TIME_DET = 80,		//同步发送数据，优先级高，正常情况下是6ms发到，但也有16ms情况，得留30ms
	TIME_DETSMP = ((TIME_DET-30)/10)	//提前30ms开采.10ms单位。 刚好50在中间，省得同步时有误差
};

enum
{
	CMD_HST_SYNC=1,		//同步
	CMD_HST_CALL, 		//点名
	CMD_HST_UPINFO,		//上传到主机信息
		
	//----------------------------
	CMD_DET_SET,		//设置参数: //偏移，放大倍数，滤波点数，扣除本底，输出DA放大倍数AU
	CMD_DET_GET,		//读取
	CMD_DET_SETX,		//各种开关量设置
	CMD_DET_CLRX,		//清除各个开关
	CMD_DET_GETX,		//各种开关量读取
	CMD_DET_START,		//开始特定波长检测
	CMD_DET_STOP,		//结束特定波长检测
	CMD_DET_SCAN,		//从XX到XX扫描
	CMD_DET_SAIR,		//扫描本底
	CMD_DET_DATA,		//工作站用的数据
	//----------------------------

	//----------------------------
	CMD_PMP_SET,
	CMD_PMP_GET,

	CMD_PMP_SET_FLOW,	//设置流量
	CMD_PMP_GET_PRESS,	//获得实时压力
	
	CMD_PMP_START,
	CMD_PMP_STOP,
	CMD_PMP_PURGE,
	//----------------------------

	CMD_AUT,
	CMD_COL,
	CMD_TMP,
};

enum
{
	iCOM_RND = 0,
	iCOM_SD  = 1,
	iCOM_CMD = 2,
	iCOM_LEN = 3,
	iCOM_DAT = 4
};

enum
{
	BT_DET_LAMP,
	BT_DET_ZERO,
	BT_DET_FAIR,
	BT_DET_ENRMV
};
#endif



//------------------------------------------------------------------------------
void utInitVariable(void);
void ut1msProc(void);
void ut10msProc(void);
void ut100msProc(void);
void ut1sProc(void);
void utInputProc(void);
void utOutputProc(void);

void test(void);
#endif
