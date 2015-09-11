#ifndef _utDisplay_H
#define _utDisplay_H

#include "utGlobals.h"

void showHeating(void);
void showWaveCal(void);
void showToScan(void);
void showMain(void);
void gotoWave(void);

void showHeating (void);
void showWaveCal(void);
void showStr(void);
INT8U getAsciiOfKey(void);

void showIdle(void);
void showMainMenu(void);
void showSRInfo(void);

INT16U Round(float x);
void utTwk(INT8U cnt);


#endif
