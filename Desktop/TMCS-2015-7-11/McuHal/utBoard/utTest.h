#ifndef _utTest_H
#define _utTest_H

#include "utMcu.h"

void utInitPrtUart(void);
void utInitInt(void);
void utInitBoard(void);

void swgInitMotor(void);
void swgInitDLamp(void);

void utPtn1(char *str);
void utPts1(char *str);

void utPtb3(char c);

#endif
