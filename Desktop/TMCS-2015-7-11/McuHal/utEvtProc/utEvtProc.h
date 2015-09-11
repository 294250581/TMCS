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
	F_SYNCED,	//��ͬ��
	F_NOTHST,
	F_AUTOZERO
};//������־λ

enum
{
	EVT_KEY1,
	EVT_KEY2
};//�����¼�

enum
{
	EVT_DIS1,
	EVT_DIS2
};//����¼�

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
	//0-30Ϊ�������úͶ�ȡ�������ݣ�70-99Ϊͬ���źŷ������䣬���ȼ���Ϊ�ϸ�
	//�����豸�������͡��豸��⵽���ݾ͸�λ10ms��ʱ������������ʱ�̵�ʱ���뱣֤���߿�����10ms
	TIME_HOST = 0,		//�������ݣ����ȼ���
	TIME_AUTO = 30,		
	TIME_COLL = 35,
	TIME_PUMPA = 40,
	TIME_PUMPB = 45,
	TIME_PUMPC = 50,
	TIME_PUMPD = 55,
	TIME_RSV1 = 60,
	TIME_RSV2 = 65,
	#if 0				//Ϊ�˱ܿ�10ms��80msǰ10ms�ڲ��ܶ����豸
	TIME_RSV3 = 70,
	TIME_RSV4 = 75,	
	#endif
	TIME_DET = 80,		//ͬ���������ݣ����ȼ��ߣ������������6ms��������Ҳ��16ms���������30ms
	TIME_DETSMP = ((TIME_DET-30)/10)	//��ǰ30ms����.10ms��λ�� �պ�50���м䣬ʡ��ͬ��ʱ�����
};

enum
{
	CMD_HST_SYNC=1,		//ͬ��
	CMD_HST_CALL, 		//����
	CMD_HST_UPINFO,		//�ϴ���������Ϣ
		
	//----------------------------
	CMD_DET_SET,		//���ò���: //ƫ�ƣ��Ŵ������˲��������۳����ף����DA�Ŵ���AU
	CMD_DET_GET,		//��ȡ
	CMD_DET_SETX,		//���ֿ���������
	CMD_DET_CLRX,		//�����������
	CMD_DET_GETX,		//���ֿ�������ȡ
	CMD_DET_START,		//��ʼ�ض��������
	CMD_DET_STOP,		//�����ض��������
	CMD_DET_SCAN,		//��XX��XXɨ��
	CMD_DET_SAIR,		//ɨ�豾��
	CMD_DET_DATA,		//����վ�õ�����
	//----------------------------

	//----------------------------
	CMD_PMP_SET,
	CMD_PMP_GET,

	CMD_PMP_SET_FLOW,	//��������
	CMD_PMP_GET_PRESS,	//���ʵʱѹ��
	
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
