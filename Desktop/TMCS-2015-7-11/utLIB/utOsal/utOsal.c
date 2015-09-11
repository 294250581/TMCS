#include "utOsal.h"


//##############################################################################
//事件标志，在mainProc中需要
//##############################################################################
#define utSetEvt(flag,bit)	{flag |= bit;}
#define utClrEvt(flag,bit)	{flag &= ~bit;}
#define utIsEvt(flag,bit)		(flag & bit)
#define utProcEvt(flag,bit,proc)		\
if(utIsEvt (flag, bit))				\
{										\
	utClrEvt (flag, bit);				\
	proc;								\
}
//##############################################################################


BIT_OF_CPU stInt=0;
BIT_OF_CPU ut1ms=0;


static EVENT evtSys;
#define EVENT_1ms					BIT0	
#define EVENT_10ms					BIT1	
#define EVENT_100ms					BIT2
#define EVENT_1s					BIT3
#define EVENT_INPUT					BIT4
#define EVENT_OUTPUT				BIT5

void utOsalTimerUpdate() 
{
	static BIT_OF_CPU cntFstTicks = 0;
	static BIT_OF_CPU cntTicks = 0;

	utEnterCrt;

	ut1ms++;
	evtSys |= EVENT_1ms;
	
	if(++cntFstTicks==10)
	{
		cntFstTicks = 0;
		
		evtSys |= EVENT_10ms;			//每个周期一个，10ms

		if(((++cntTicks)%10) == 0)
		{
			evtSys |= EVENT_100ms;		//10个周期一个，100ms
			
			if(cntTicks == 100)
			{
				cntTicks = 0;
				evtSys |= EVENT_1s;		//100个周期一个，1s
			}
		}
	}

	utExitCrt;
}

void utInitSys()
{
	utInitCpuClk();
	utInitDebug();
	utInitPrt(); 			
	utInitOsalClk();
}

static EVENT	utEvtInput;
static EVENT	utEvtOutput;
static UT_FLAG	utFlag;

//------------------------------------------------------------------------------
void utStartOS(void)
{
	while (1)	
	{			
		if(!evtSys) 				//没有事件
		{
			continue;
		}

		utProcEvt(evtSys, EVENT_1ms, {ut1msProc();});		//处理了本事件，还要再处理
		utProcEvt(evtSys, EVENT_10ms, {ut10msProc();});	//处理了本事件，还要再处理
		utProcEvt(evtSys, EVENT_100ms, {ut100msProc();});	//处理了本事件，还要再处理
		utProcEvt(evtSys, EVENT_1s, {ut1sProc();});		//处理了本事件，没什么要处理的了
		
		//输入信息处理
		utProcEvt(evtSys, EVENT_INPUT, {utInputProc();{if(utEvtInput) evtSys |= EVENT_INPUT;}; continue;});		
		
		//输出信息处理
		utProcEvt(evtSys, EVENT_OUTPUT, {utOutputProc();{if(utEvtOutput) evtSys |= EVENT_OUTPUT;}; continue;});
	}		
}

void utSetInputEvt(EVENT x)	
{
	utEnterCrt;
	utEvtInput |= (BIT0<<x);
	evtSys |= EVENT_INPUT;
	utExitCrt;
}

void utClrInputEvt(EVENT x)	
{
	utEnterCrt;
	utEvtInput &= ~(BIT0<<x);
	utExitCrt;
}

BOOLEAN utIsInputEvt(EVENT x)
{
	return (utEvtInput & (BIT0<<x));
}

void utSetOutputEvt(EVENT x)	
{
	utEnterCrt;
	utEvtOutput |= (BIT0<<x);
	evtSys |= EVENT_OUTPUT;
	utExitCrt;
}

void utClrOutputEvt(EVENT x)	
{
	utEnterCrt;
	utEvtOutput &= ~(BIT0<<x);
	utExitCrt;
}

BOOLEAN utIsOutputEvt(EVENT x)
{
	return (utEvtOutput & (BIT0<<x));
}

void utSetFlag(UT_FLAG x)	
{
	utEnterCrt;
	utFlag |= (BIT0<<x);
	utExitCrt;
}

void utClrFlag(UT_FLAG x)	
{
	utEnterCrt;
	utFlag &= ~(BIT0<<x);
	utExitCrt;
}

BOOLEAN utIsFlag(UT_FLAG x)
{
	return (utFlag & (BIT0<<x));
}

INT8U utSleep(INT32U time)
{
	static INT32U Itime;
	utBeginTask;

	Itime = ut1ms;
	sprt("Itime = %d\n", Itime);

	utWait( ((INT32U)(ut1ms-Itime)>time) );

	utPrt("Time finshed\n");
	
	utEndTask;
}
