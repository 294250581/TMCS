#ifndef _utZLG7289_H
#define _utZLG7289_H

#include "utGlobals.h"

#define ZLG7289_Reset() ZLG7289_cmd(0xA4)
#define ZLG7289_Test() ZLG7289_cmd(0xBF)
//0…¡À∏£¨1≤ª…¡À∏
#define ZLG7289_Flash(x) {ZLG7289_cmd_dat(0x98,0xff); ZLG7289_cmd_dat(0x88,(x));}
void ZLG7289_cmd(char cmd);
void ZLG7289_cmd_dat(char cmd, char dat);

extern void ZLG7289CS(INT8U dat);
extern ZLG7289DAT(INT8U dat);
extern ZLG7289CLK(INT8U dat);
extern IsZLG7289DAT(void);
extern SetZLG7289IN(INT8U dat);

#endif
