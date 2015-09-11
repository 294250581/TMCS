#ifndef _utSPI_H
#define _utSPI_H

#include "utGlobals.h"

extern INT8U utIsSPIIn(void);
extern void utSPIOut(INT8U dat);
extern void utSPIClk(INT8U dat);

INT8U utLoadSPI(INT8U dat);	//本函数既写又读，MOSI，MISO各一根线
void utSPIWrt(INT8U dat);		//MOSI，MISO复用，本函数单写
INT8U utSPIRd(void);			//MOSI，MISO复用，本函数单读

#endif
