#include "utOsal.h"

void utOsalKillTimer(INT8U timer_id)
{
	osalTimerRec_t *newTimer;

	//必须保护，否则会丢失
	utEnterCrt;
	newTimer = &timers[timer_id];
	newTimer->active = false;
	utExitCrt;	
}

void utOsalClrEvent(EVENT event_flag)
{
	//必须保护，否则会丢失
	utEnterCrt;
	evtSys &= ~event_flag;
	utExitCrt;
}

