#include "utOsal.h"

void utOsalKillTimer(INT8U timer_id)
{
	osalTimerRec_t *newTimer;

	//���뱣��������ᶪʧ
	utEnterCrt;
	newTimer = &timers[timer_id];
	newTimer->active = false;
	utExitCrt;	
}

void utOsalClrEvent(EVENT event_flag)
{
	//���뱣��������ᶪʧ
	utEnterCrt;
	evtSys &= ~event_flag;
	utExitCrt;
}

