#include "FLASH.h"

typedef struct 
{
	struct _t_save_motor t_save_motor0;
	struct _t_save_motor t_save_motor1;	
	struct _t_save_motor t_save_motor2;

	struct _t_save_axis  t_save_axisx;
	struct _t_save_axis  t_save_axisy;
	struct _t_save_axis  t_save_axisz;

	struct _t_save_iobus t_save_io_A[SUBn];
	struct _t_save_iobus t_save_io_B[SUBn];
}STRCT_STORE;

STRCT_STORE strctStore;

#define P_DAT_TO_STORE		(&strctStore)	//数组直接用名称，变量采用'&'
#define TYPE_DAT_TO_STORE	STRCT_STORE
#define NUM_DAT_TO_STORE	1

#define SIZE_STORE 		((INT32U)(sizeof(TYPE_DAT_TO_STORE)*NUM_DAT_TO_STORE))
#define NUM_PAGE_STORE	(((SIZE_STORE) >> 10) + 1)		//要多出来一页吧，不能用去尾法
#define PageSize   		((INT16U)0x400)     /* 页大小为400 */

#define StartAddr  ((INT32U)0x0803E000)	//留下8K存储数据 

void utStoreData()
{
	INT8U  FLASHStatus = FLASH_COMPLETE;
	INT16U len=0; 
	INT32U *pDat = (INT32U *)P_DAT_TO_STORE;
	INT32U EraseCounter = 0x0;			//要用32位的，否则下面乘出来会有问题

	utEnterCrt;

	FLASH_Unlock(); //首先解锁
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

	//擦除
	for(EraseCounter = 0; (EraseCounter < NUM_PAGE_STORE) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
	{
		FLASHStatus = FLASH_ErasePage(StartAddr + (PageSize * EraseCounter));
	}

	//写数据
	while((len < SIZE_STORE) && (FLASHStatus == FLASH_COMPLETE))
	{
		FLASHStatus = FLASH_ProgramWord((StartAddr+len), (pDat[len>>2]));	//我靠，32位的
		len+=4;
	}
	//写标志位
	FLASH_ProgramWord((StartAddr+SIZE_STORE), 0x55aa);						//标志位,最后2个字节

	utExitCrt;

	utPrt("一次存储完毕!\n");
}

void utGetData()
{
	INT16U 	len=0;
	INT32U  *pDat = (INT32U *)P_DAT_TO_STORE;

	//将数据恢复到存储数组中
	utEnterCrt;
	while(len < SIZE_STORE)
	{
		pDat[len>>2] = *((INT32U *)(StartAddr+len));
		len+=4;
	}
	utExitCrt;
}

BOOLEAN utIsStored()
{
	if(((INT16U)(*((INT32U *)(StartAddr+SIZE_STORE)))) == 0x55aa )
		return true;
	else
		return false;
}

void DetectCal(void)
{	
	INT8U i;
	
	if(utIsStored ())
	{
		utGetData();
		#if 0
		for(i=0; i<60; i++)
		{
			utPrt("motor 0 ..%d\n", strctStore.t_save_motor0.t_save_COORD.t_save_ABS[i]);
		}

		for(i=0; i<60; i++)
		{
			utPrt("motor 1 ..%d\n", strctStore.t_save_motor1.t_save_COORD.t_save_ABS[i]);
		}

		for(i=0; i<60; i++)
		{
			utPrt("motor 2 ..%d\n", strctStore.t_save_motor2.t_save_COORD.t_save_ABS[i]);
		}

		
		for(i=0; i<60; i++)
		{
			utPrt("motor 0x ..%d\n", strctStore.t_save_motor0.t_save_COORD.t_save_ENC[i]);
		}

		for(i=0; i<60; i++)
		{
			utPrt("motor 1x ..%d\n", strctStore.t_save_motor1.t_save_COORD.t_save_ENC[i]);
		}

		for(i=0; i<60; i++)
		{
			utPrt("motor 2x ..%d\n", strctStore.t_save_motor2.t_save_COORD.t_save_ENC[i]);
		}
		#endif

		/* 恢复数据 */
		memcpy(&t_tmcs_motor0 , &strctStore.t_save_motor0, sizeof(t_tmcs_motor0));
		memcpy(&t_tmcs_motor1 , &strctStore.t_save_motor1, sizeof(t_tmcs_motor1));
		memcpy(&t_tmcs_motor2 , &strctStore.t_save_motor2, sizeof(t_tmcs_motor2));

		memcpy(&motor_device_x, &strctStore.t_save_axisx, sizeof(motor_device_x));
		memcpy(&motor_device_y, &strctStore.t_save_axisy, sizeof(motor_device_y));
		memcpy(&motor_device_y, &strctStore.t_save_axisz, sizeof(motor_device_z));

		memcpy(&IOBUSA_TCB[SUB_A0], &strctStore.t_save_io_A[SUB_A0], sizeof(IOBUSA_TCB[SUB_A0]));
		memcpy(&IOBUSA_TCB[SUB_A1], &strctStore.t_save_io_A[SUB_A1], sizeof(IOBUSA_TCB[SUB_A1]));
		memcpy(&IOBUSA_TCB[SUB_A2], &strctStore.t_save_io_A[SUB_A2], sizeof(IOBUSA_TCB[SUB_A2]));
		memcpy(&IOBUSA_TCB[SUB_A3], &strctStore.t_save_io_A[SUB_A3], sizeof(IOBUSA_TCB[SUB_A3]));

		memcpy(&IOBUSB_TCB[SUB_A0], &strctStore.t_save_io_B[SUB_A0], sizeof(IOBUSB_TCB[SUB_A0]));
		memcpy(&IOBUSB_TCB[SUB_A1], &strctStore.t_save_io_B[SUB_A1], sizeof(IOBUSB_TCB[SUB_A1]));
		memcpy(&IOBUSB_TCB[SUB_A2], &strctStore.t_save_io_B[SUB_A2], sizeof(IOBUSB_TCB[SUB_A2]));
		memcpy(&IOBUSB_TCB[SUB_A3], &strctStore.t_save_io_B[SUB_A3], sizeof(IOBUSB_TCB[SUB_A3]));
	}
    else
    {
 
    }
}


/* 保存参数
   <!t_save_motor0 电机0位置参数
   <!t_save_motor1 电机1位置参数
   <!t_save_motor2 电机2位置参数
*/
void save_value(void)
{
	memcpy(&strctStore.t_save_motor0, &t_tmcs_motor0 , sizeof(t_tmcs_motor0));
	memcpy(&strctStore.t_save_motor1, &t_tmcs_motor1 , sizeof(t_tmcs_motor1));
	memcpy(&strctStore.t_save_motor2, &t_tmcs_motor2 , sizeof(t_tmcs_motor2));

	memcpy(&strctStore.t_save_axisx,  &motor_device_x, sizeof(motor_device_x));
	memcpy(&strctStore.t_save_axisy,  &motor_device_y, sizeof(motor_device_x));
	memcpy(&strctStore.t_save_axisz,  &motor_device_z, sizeof(motor_device_z));

	memcpy(&strctStore.t_save_io_A[SUB_A0], &IOBUSA_TCB[SUB_A0], sizeof(IOBUSA_TCB[SUB_A0]));
	memcpy(&strctStore.t_save_io_A[SUB_A1], &IOBUSA_TCB[SUB_A1], sizeof(IOBUSA_TCB[SUB_A1]));
	memcpy(&strctStore.t_save_io_A[SUB_A2], &IOBUSA_TCB[SUB_A2], sizeof(IOBUSA_TCB[SUB_A2]));
	memcpy(&strctStore.t_save_io_A[SUB_A3], &IOBUSA_TCB[SUB_A3], sizeof(IOBUSA_TCB[SUB_A3]));

	memcpy(&strctStore.t_save_io_B[SUB_A0], &IOBUSB_TCB[SUB_A0], sizeof(IOBUSB_TCB[SUB_A0]));
	memcpy(&strctStore.t_save_io_B[SUB_A1], &IOBUSB_TCB[SUB_A1], sizeof(IOBUSB_TCB[SUB_A1]));
	memcpy(&strctStore.t_save_io_B[SUB_A2], &IOBUSB_TCB[SUB_A2], sizeof(IOBUSB_TCB[SUB_A2]));
	memcpy(&strctStore.t_save_io_B[SUB_A3], &IOBUSB_TCB[SUB_A3], sizeof(IOBUSB_TCB[SUB_A3]));
	utStoreData();
}


