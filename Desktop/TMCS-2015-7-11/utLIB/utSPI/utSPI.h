#ifndef _utSPI_H
#define _utSPI_H

#include "utGlobals.h"

extern INT8U utIsSPIIn(void);
extern void utSPIOut(INT8U dat);
extern void utSPIClk(INT8U dat);

INT8U utLoadSPI(INT8U dat);	//��������д�ֶ���MOSI��MISO��һ����
void utSPIWrt(INT8U dat);		//MOSI��MISO���ã���������д
INT8U utSPIRd(void);			//MOSI��MISO���ã�����������

#endif
