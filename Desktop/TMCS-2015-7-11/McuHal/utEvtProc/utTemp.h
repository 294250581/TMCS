#ifndef _utTEMP_H
#define _utTEMP_H

#include "utGlobals.h"


extern char str[];

void testDAC8560(void);
void swgInitDAC8560(void);
void utDAC8560Write(INT16U dat);
void utDAC8560WriteF(float fDat);

void swgInitRelay(void);
void swgInitBell(void);
void swgInitKey(void);
void bgCal656nm(void);



extern INT8U INPUT_KEY;

#if 0
enum
{
	INPUT_KEY = 0,
}UT_INPUT_EVENT;
#endif

typedef union
{
	INT8U bt[4];
	float fl;
}DT_PC;

extern DT_PC dtPC;

void rxUart1(INT8U dat);

void checkKey(void);
void showInit(void);
//void rx1Proc(void);

extern INT16U daDat[];

enum
{
	STI_IDLE,
	STI_LAMP_ON,
	STI_TO200nm,
	STI_WtStNm,
	STI_WTXnm,
	STI_SCAN,
	STI_DETECTING
};

extern INT8U sti;

void showOnScan(void);

void storeData(void);
void getData(void);
BOOLEAN isStored(void);

extern INT8U cntFilt;
void showOnDetecting(void);
//extern INT8U fSendOver;
extern INT8U fRcvOver;

#if 1
typedef struct _APP_CONFIG
{
	INT8U lLen;		//要求小端存放
	INT8U cmd;
	INT16U wave;
	float time;

	#if 0
	INT32U sn;
	INT32U vol;
	
	float flow;
	float flowcoff;

	INT32U presszero;
	float presscoff;
	INT8U pressmin;	
	INT8U pressmax;		
	INT8U grade;
	INT8U tmp[1];
	#endif
}APP_CONFIG;
//这里面只记住当前的
//得被4整除，否则读取时会出问题，是因为STM32保存FLASH的问题
#endif

extern APP_CONFIG AppConfig;

typedef struct
{
	//FP32 basicAir[728+1];	//实际上索引有MAX_SCAN的
	FP32 bsicAir[728+1];	//本底的数据
	
	INT16U amp;				//内部微调的放大倍数
	INT16U offset;			//内部微调的偏移
	INT8U pTao;				//时间常数索引
	INT8U pDaRange;			//范围索引
	INT8U numFilt;			//设置的滤波点数
	INT8U fAir;				//证明扫描过本底了
	
	INT16U wave;			//上次的波长
	
	INT8U enRmvAir;			//这个暂时不用记忆
	INT8U fOnCal;			//这个不用记忆，每次清空
	
	FP32  flZero;			//这个不用记忆，每次清空
}STRCT_STORE;

extern STRCT_STORE strctStore;

#if 0
#define VT_BG1	350//700
#define VT_BG2  500//900
#define VT_MAX	0x0a00//0x1200
#define VT_MAX656	0x1e0//0x280//0x400，好像都小了很多啊
#define START_SCAN	201
//#define MAX_SCAN	600//728
#define HEAT_LAMP	1//1
#define MAX_656SCAN	18
#define NUM_OF_FILT	40
#define NUM_OF_RMV	10
#define RMV_AIR		0

#else
#if 0	//自己的大些
#define VT_BG1	700
#define VT_BG2  900
#define VT_MAX	0x1200
#define VT_MAX656	0x170
#else	//北京的小些
#define VT_BG1	350//700
#define VT_BG2  500//900
#define VT_MAX	0x0a00//0x1200
#define VT_MAX656	0x1e0//0x280//0x400，好像都小了很多啊
#endif
#define NUM_RMV		0//0
#define START_SCAN	201//630
//#define MAX_SCAN	580//700
#define HEAT_LAMP 	0
#define MAX_656SCAN	18
#define NUM_OF_FILT	25
#define NUM_OF_RMV	5
#define RMV_AIR		0
#endif

enum
{
	STMT_IDLE,
	STMT_POS,
	STMT_INV
};

extern INT8U stMotor;
extern INT16U cntMotorPos;
extern INT16U cntMotorInv;
extern INT8U cnt1ms;

#define utNow1ms(x)	((INT32U)(ut1ms-x))

typedef enum 
{
	MENU_IDLE,	
	MENU_MAIN,	//P1
	MENU_MAINMENU,
	MENU_DAOUT,	//P2
	MENU_SRINFO,//P3
	MENU_GLP,	//P4
	MENU_WAVECAL,	//P5
	MENU_SN,		//P6
	
	MENU_SCAN,
	#if 0
    MENU_CHANGE,
    MENU_RANGE,
    MENU_METER,
    MENU_RANGE_FP,
    MENU_ZERO,
    MENU_GRADE,
    MENU_GLP,
    MENU_SN,
    MENU_WNMAX,
    MENU_WNMIN
    #endif
}menuPage;

extern menuPage stMenu;

#define MAX_FILT	20

extern INT16U MAX_SCAN;


extern INT16U curStep;
extern INT16U nxtStep;
extern INT8U INPUT_KEY;
void motor(void);
void motorInv(void);
extern float tfl;

extern INT16U cntDA;

void resetFIR(void);
void setAD16Amp(INT8U x);
void swgPlot(void);
void checkAScan(void);

#define LEN_UART1	256
extern INT8U rx1Buf[LEN_UART1]; 
extern INT8U rx1Dat[LEN_UART1];
extern INT8U rx1Len;
extern INT8U tx1Buf[LEN_UART1]; 
extern INT8U tx1Dat[LEN_UART1];
extern INT8U tx1Len;
extern INT8U tx1BufLen;

extern INT8U fTx2Send;

extern INT16U dat;
extern INT16U wDat;

void FormAMsg(INT8U cmd, INT8U len, INT8U *pDat);
//直接能发送的
void FormAAck(INT8U cmd, INT8U len, INT8U *pDat);

extern INT8U cntConnectedTout;

extern INT8U snDet;

extern BIT_OF_CPU	cnt1msSendOver;

INT16U getNmFromStep(INT16U step);
void showCheckHome(void);

//extern INT8U cnt1msSend;
//extern INT8U fSended = 1;

extern INT16U cnt10ms;
void sample(void);

extern float daRange[];
extern float bkTfl;

extern INT8U cntUpInfo;
extern INT8U fOnUpInfo;

extern INT8U keyCode[];
extern INT8U key;

extern INT32U utRandSeed;
INT8U utRandINT8(void);

extern BIT_OF_CPU	cnt1msSync;
extern INT8U cntSnSync;
#define nowTime ((INT8U)(utNow1ms(cnt1msSync)%100))
//#define nowTime ((INT16U)((cntSnSync*100)+(utNow1ms(cnt1msSync)%100)))

#define ENCRYPT		1


#define TYPE_FILT	INT16U

typedef struct
{
	TYPE_FILT tb[MAX_FILT];
	INT8U pDat;
	FP32 datProc;
}MY_FILT;


void startWave(void);
void ZeroOut(void);
extern float orgTfl;
extern INT8U fEnUpDat;
extern MY_FILT ftADRef16,ftADSig16;

extern float daRange[];
extern float flDetTao[];

INT16U getStepFromNm(INT16U nm);


#endif
