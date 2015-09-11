#ifndef _utKey_H
#define _utKey_H

#include "utGlobals.h"


void keyProc(void);

#define KEY_MENU        0x01B7
#define KEY_MENU_LONG   0x81B7
#define KEY_ENTER		0x0177
#define KEY_DOT			0x017B
#define KEY_NUM0		0x01BB
#define KEY_NUM1		0x01AF
#define KEY_NUM2		0x016F
#define KEY_NUM3		0x00EF
#define KEY_NUM4		0x01BE
#define KEY_NUM5		0x017E
#define KEY_NUM6		0x00FE
#define KEY_NUM7		0x01BD
#define KEY_NUM8		0x017D
#define KEY_NUM9		0x00FD
#define KEY_CE			0x00FB
#define KEY_D2			0x01CF
#define KEY_ZERO		0x01DE

#define KEY_UP           (0x0000001e)
#define KEY_DOWN         (0x0000000f)

extern INT32U KeyVal;



#endif
