#ifndef _utBoard_H
#define _utBoard_H

#include "utMcu.h"

extern char str[];

void utInitPrtUart(void);
void utInitInt(void);
void utInitBoard(void);

void utPtn1(char *str);
void utPts1(char *str);

void utInitDebug(void);
void utInitUart3(void);

#endif
