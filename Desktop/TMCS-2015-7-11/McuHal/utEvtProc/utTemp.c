#include "utTemp.h"

INT8U fEnUpDat = 0;

INT8U key;
INT8U keyCode[]={0x2,0xa,0x0,0x4, 0x1,0xf,0x5,0xc, 0x3,0x6,0xb,0x8, 0xe,0x7,0xd,0x9};	//wifi

//��ʼ��ʱ��utRandSeed��һ����������
//�Ժ����utRandINT8�Ϳ��Եõ����INT8���͵��������
INT32U utRandSeed;
INT8U utRandINT8(void)
{
    utRandSeed *= 0x48C27395;    // 5 ^ 13 = 0x48C27395
    return (INT8U)(utRandSeed >> 24);
}

INT8U cntSnSync = 0;

void ZeroOut(void)
{
	#if 0
	static float tfl;
	tfl = 1000.0*strctStore.offset; 			//�Ŵ�uv
	tfl -= bkTfl;								//��ȥ������
	strctStore.offset = (INT16U)(tfl/1000.0);	//�����ȥ	
	#else
	strctStore.flZero = orgTfl;
	#endif
}

//INT8U cnt1msSend;
INT8U fSended = 1;

INT8U cntUpInfo=0;
INT8U fOnUpInfo =0;

INT16U cnt10ms = 0;

float daRange[] = {0.1,0.2,0.5,1,2,5};
float flDetTao[] = {0.1,0.2,0.5,1,2,5};

INT8U cntConnectedTout = 0;

INT8U fTx2Send = 0;

#define MAX_FILT	20

INT16U MAX_SCAN = 728;

INT16U bkStartStep = 0;
INT16U bkStopStep =0;

INT8U fOutWaveLow;

INT8U rx1Buf[LEN_UART1] = {0}; 
INT8U rx1Dat[LEN_UART1] = {0};
INT8U rx1Len;
INT8U tx1Buf[LEN_UART1] = {0}; 
INT8U tx1BufLen;
INT8U tx1Dat[LEN_UART1] = {0};
INT8U tx1Len;

INT8U snDet = 0;


STRCT_STORE strctStore;


//INT8U crc;

void utDAC8560WriteF(float fDat);

float flFilt = 0;



#if 0
#define RX_FIFO_DAT INT8U
#define MAX_LEN_RXBUF	200	//Ҫ�����ֽ�ָ��
#define MAX_LEN_TXBUF	200

UT_FIFO rx1FIFO;
RX_FIFO_DAT rx1FIFODat[MAX_LEN_RXBUF];
TIMER_CNT rxUart1Tout,rxUart2Tout;	//���ճ�ʱ

//�Ӵ���1���գ����������Ե���
void rxUart1(INT8U dat)
{	
	if(FIFO_NOT_FULL(&rx1FIFO))
	{
		rx1FIFODat[utIndWrtFIFO(&rx1FIFO)] = dat;
		utStTmr(rxUart1Tout);				//����һ���ַ����,�����Ѿ�������to_at����
	}
	else									//�������ݣ�������ʲô
	{
		utPtb('&');
		while(FIFO_NOT_EMPTY(&rx1FIFO))		//���
		{
			utPtb(rx1FIFODat[utIndRdFIFO(&rx1FIFO)]);
		}		
	}
}
#endif





void getFilt(MY_FILT *pTb, TYPE_FILT datNew)
{
	pTb->datProc+=(1.0*datNew-1.0*(pTb->tb[pTb->pDat]))/strctStore.numFilt;
	pTb->tb[pTb->pDat] = datNew;	//������������
	if(++(pTb->pDat)==strctStore.numFilt)
		pTb->pDat = 0;
}

MY_FILT ftADRef16,ftADSig16;

INT16U curStep = 0;
INT16U nxtStep = 0;
INT8U INPUT_KEY=0;
float tfl=0,bkTfl=0, orgTfl=0;

#define TYPE_OF_FILT	INT16U
INT8U cntDetect = 0;

INT8U  LAST_DIR=0;	//Ĭ����ת


APP_CONFIG AppConfig;

void resetFIR(void)
{
	int i;
	
	for(i=0;i<MAX_FILT;i++)
	{
		ftADRef16.tb[i] = 0;
		ftADSig16.tb[i] = 0;
	}	

	ftADRef16.pDat = 0;
	ftADSig16.pDat = 0;
	
	ftADRef16.datProc = 0.0;
	ftADSig16.datProc = 0.0;
}


TYPE_OF_FILT BubbleSort1(TYPE_OF_FILT *a, int n, int m);

INT16U tbMotorInv[8] = {0x29,0x31,0x21,0x27,0x25,0x3d,0x2d,0x2b};	//MA
//INT16U tbMotor[8] = {0x2F,0x2D,0x3D,0x25,0x23,0x21,0x31,0x29};	//2916

INT16U tbMotor[8] = {0x2b,0x2d,0x3d,0x25,0x27,0x21,0x31,0x29};
//INT16U tbMotor[8] = {0x29,0x3B,0x21,0x27,0x25,0x3d,0x2d,0x2b};

#define BR_CTRL0
#define BR_CTRL1

#define RLY_ON		GPIO_SetBits(GPIOB,GPIO_Pin_2); 
#define RLY_OFF		GPIO_ResetBits(GPIOB,GPIO_Pin_2); 


INT8U stMotor = 0;
INT16U cntMotorPos = 0;
INT16U cntMotorInv = 0;
INT8U cnt1ms = 0;

enum
{
	CAL_SCANX=0x55,
	CAL_WAVE254=0x56,
	CAL_SCAN0=0x57,
	GO2_WAVE=0x58,
	GO2_ENDWAVE=0x59,
	RGE_SCAN=0x5A,
	SET_PARA=0x5B,
	READ_PARA=0x5c
};

enum
{
	WV_254,
	WV_360,
	WV_446,
	WV_536
};

INT16U wvCal[10];

INT16U wDat;

INT8U sti = STI_IDLE;
INT8U pMPhase=0;
//INT16U curStep=1;
INT16U dat=0,dat1,dat2;
//FP64 tfp1,tfp2;

//���������Ժ�У�������û�������ô�
INT16U daDat[800];


//INT8U 
INT32U x1=0,x2=0;
INT8U cntsmp=0;
INT8U cnt100ms=0;
INT16U maxRef=0;
INT16U pMaxRef=0;
INT8U cntFilt=0;
INT16U cntDA = 0;
INT16U tbWd1[MAX_FILT],tbWd2[MAX_FILT];

void setADRefBitsAmp(INT8U x, INT8U y)
{
	INT8U dat = 0;

	if(x==16)
		dat |= 0x0c;
	else if(x==15)
		dat |= 0x08;
	else if(x==14)
		dat |= 4;

	if(y==2)
		dat |= 1;
	else if(y==4)
		dat |= 2;
	else if(y==8)
		dat |= 3;

	WriteDataCfgReg(0x90,dat); //0E->16λ,�Ŵ�4�����Ŵ�8��200nm��ref�����
}

void setAD16Amp(INT8U x)
{
	INT8U dat = 0x0C;

	if(x==2)
		dat |= 1;
	else if(x==4)
		dat |= 2;
	else if(x==8)
		dat |= 3;
	
	WriteDataCfgReg(0x90,dat); //0E->16λ,�Ŵ�4�����Ŵ�8��200nm��ref�����
	WriteDataCfgReg(0x92,dat); //0E->16λ,�Ŵ�4�����Ŵ�8��200nm��ref�����
}

void swgPlot()
{
	FP32 tSig;
	
	getFilt (&ftADRef16, ReadDataCfgReg(0x91,0x01));
	getFilt (&ftADSig16, ReadDataCfgReg(0x93,0x01));
	
	//����ÿ100ms DA���һ��
	if(((INT16U)(ftADSig16.datProc))==0)
	{
		utDAC8560Write(0);
	}
	else
	{
		//#if RMV_AIR
		//if(utIsFlag (F_SCANX))
		//if(strctStore.fAir)			
		//У����,����ʹ����ȥ������
		if((strctStore.fAir)&&(strctStore.enRmvAir))
		{
			//��ֵ��ԭ������ˣ���lg��ԭ��С�ˣ������ձ�ö���
			tSig = (ftADSig16.datProc)*(strctStore.bsicAir[curStep])/((strctStore.bsicAir[curStep])-(ftADSig16.datProc));
			//������Ϊ0�����
			if(tSig<=0.00000)
			{
				tSig = 0.0001;
				sprintf(str,"\n!!!!%f,%f,%f,%d !!!!\n",(ftADSig16.datProc),(strctStore.bsicAir[curStep]),(tSig),curStep);
				utPtn(str);
				#if 1
				while(1)
				{
					LAMP_TOG;
					utDelay1ms(80);
				}
				#endif
			}
		}
		else
		//#endif
			tSig = (ftADSig16.datProc);					//ֱ�ӵ��ڲ���ֵ

		//����������Ŵ�1000����ΧΪ(-600,1000)
		tfl=1000.0*(log10((ftADRef16.datProc)/tSig));	//ȡ������Ҫ�Ŵ�1000��,����(-600,1000)uV
		//�ȷŴ�������ƫ��
		//�����5000��Ӧ�����5
		//tfl*=(0.035543*strctStore.amp);				//1000ʱ��Ϊ(-600,1000)mV
		#if 0
		tfl*=(0.035543*strctStore.amp);					//1000ʱ��Ϊ(-600,1000)mV???
		tfl*=(daRange[strctStore.pDaRange]);			
		#else
		tfl*=(1.184767*strctStore.amp);					//1000ʱ��Ϊ(-600,1000)mV???
		tfl/=(daRange[strctStore.pDaRange]);			
		#endif
		//tfl��λΪuV
		tfl+=1000.0*(strctStore.offset);				//0ffSet��λΪmV,1000ʱ��Ϊ(400,2000mV)
		orgTfl = 0.9*tfl+0.1*orgTfl;					//��΢����һ��

		tfl-=strctStore.flZero;

		#if 1	//��΢����һ��
		flFilt = 0.1*flDetTao[strctStore.pTao];
		tfl = flFilt*bkTfl+(1-flFilt)*tfl;
		bkTfl = tfl;
		#endif

		if(utIsFlag (F_AUTOZERO))
		{
			ZeroOut();
		}
								
		//��У��ģʽ(���ģʽ)����������
		if(!(strctStore.fOnCal))
		{
			utDAC8560WriteF(bkTfl);
		}

		//�����������tfl����dat2ֻ��Ϊ����ʾ�õ�
		//dat2���������Ժ�У�������û�������ô�
		#if 1
		if(bkTfl>=65535.0)
			dat2 = 65535;
		else
			dat2 = ((INT16U)bkTfl);
		#endif		
	}
	
	if(++cntFilt>=strctStore.numFilt)
	{
		cntFilt = 0;

		if(utIsFlag (F_AUTOZERO))
		{
			utClrFlag (F_AUTOZERO);

			ZeroOut();

			#if 0
			tfl = 1000.0*strctStore.offset; 	//�Ŵ�uv
			tfl -= bkTfl;						//��ȥ������
			strctStore.offset = (INT16U)(tfl/1000.0);	//�����ȥ	
			#endif
		}		

		#if 0
		if((strctStore.fOnCal))	//ɨ��ģʽ��ֻ��һ����
		{
			if(sti == STI_SCAN)
				//utDAC8560Write(dat2);
				utDAC8560WriteF(tfl);
			else if(sti == STI_DETECTING)		
				//utDAC8560Write(dat2-25000);
				//utDAC8560Write(dat2-10000);
				utDAC8560WriteF(tfl-10000.0);
		}
		#else
		if((strctStore.fOnCal))	//У��ģʽ��ֻ��һ����
		{
			utDAC8560WriteF(bkTfl);
		}		
		#endif

		#if 0
		//�����һ���˲������
		//sprintf(str,"%x,%x,%d\n",(INT16U)(ftADRef16.datProc),(INT16U)(tSig),curStep);
		sprintf(str,"%x,%x,%4.4f,%d\n",(INT16U)(ftADRef16.datProc),(INT16U)(tSig),tfl,curStep);
		//sprintf(str,"%f,%f,%d\n",(ftADRef16.datProc),(tSig),cntStep);
		utPtn(str);
		//utPtn1(str);
		#endif

		if(sti == STI_SCAN)
		{
			if(curStep==384)	//385��Ӧ380nm
				FILT_ON;
			
			//if(cntStep<=MAX_SCAN)	//���ھͲ�ת��
			if(curStep<MAX_SCAN)	//���ھ����ڱ���MAX_SCAN, ������һ����
				motor();

			if(utIsFlag (F_SCAN0))			//��������ʱ��,������ʱ������
			{
				strctStore.bsicAir[curStep] = (ftADSig16.datProc);
				//sprintf(str,"ba=%f,%d\n",basicAir[cntStep],cntStep);
				//utPtn(str);
			}
			
			daDat[curStep] = dat2; 	//���������dat2
			#if 0
			utPtb1(0x30+(curStep%10));
			if((curStep%10)==9)
				utPtc1('\n');
			#endif

			//����ִ����ϣ�����һ��ȥ
			curStep++;
		}

		#if 0
		else
		{
			daDat[cntDA++] = dat2; 	//���������dat2
			if(cntDA>600)
				cntDA = 600;		//201+600=800��
		}
		#endif
	}
}

#define WVS_360		365
#define WVS_446		454
#define WVS_536		549

#define WVINT		30

void calWaveX()
{
	#if 0
	int i;
	
	maxRef = 0;
	pMaxRef = 0;	
	for(i=(WVS_360-WVINT);i<(WVS_360+WVINT);i++)		//675-690,5
	{
		if(daDat[i]>maxRef)
		{
			maxRef = daDat[i];
			pMaxRef = i;
		}
	}
	wvCal[WV_360] = pMaxRef;

	maxRef = 0;
	pMaxRef = 0;	
	for(i=(WVS_446-WVINT);i<(WVS_446+WVINT);i++)		//675-690,5
	{
		if(daDat[i]>maxRef)
		{
			maxRef = daDat[i];
			pMaxRef = i;
		}
	}
	wvCal[WV_446] = pMaxRef;

	maxRef = 0;
	pMaxRef = 0;	
	for(i=(WVS_536-WVINT);i<(WVS_536+WVINT);i++)		//675-690,5
	{
		if(daDat[i]>maxRef)
		{
			maxRef = daDat[i];
			pMaxRef = i;
		}
	}
	wvCal[WV_536] = pMaxRef;	

	utPrt("\n360=%d,446=%d,536=%d\n\n",wvCal[WV_360],wvCal[WV_446],wvCal[WV_536]);
	//sprintf(str,"\n360=%d,446=%d,536=%d\n\n",wvCal[WV_360]-5,wvCal[WV_446]-8,wvCal[WV_536]-13);
	sprintf(str,"\n360=%d,446=%d,536=%d\n\n",wvCal[WV_360],wvCal[WV_446],wvCal[WV_536]);
	//utPtn1(str);
	#endif
}

#if 0
//------------------------------------------------------------------------------
void checkAScan()
{
	if(sti==STI_SCAN)
	{
		if(curStep>MAX_SCAN)			//700nm
		{
			#if 0
			#if HEAT_LAMP
			//��뮵�
			GPIO_ResetBits(GPIOC,GPIO_Pin_9);	//CTRL0->0
   			GPIO_SetBits(GPIOA,GPIO_Pin_13);	//CTRL1->1
			#endif
			#endif

			FILT_OFF;	//ɨ��͹��˹�Ƭ��ʡ��
			
			curStep=MAX_SCAN;		//���л���728��
			strctStore.fOnCal = 0;
			sti = STI_IDLE;

			cntDA = 0;

			cntFilt = 0;
			utDAC8560Write(0);
			//showInit();

			if(utIsFlag (F_SCANX))
			{
				utClrFlag (F_SCANX);
				calWaveX();
				//У��
			}
			else if(utIsFlag (F_SCAN0))
			{
				utClrFlag (F_SCAN0);
				strctStore.fAir = 1;
				storeData();
			}

			#if 0
			for(i=START_SCAN;i<=MAX_SCAN;i++)
			{
				utPrt("%x,%d\n",daDat[i],i);
			}
			#endif
			
			return;
		}

		swgPlot();
	}
}
#endif

#if 0
const INT8U myChar[8][8]={																			 //��ʾ������ͷ����ģ����
				{0x00,BIT2,BIT1|BIT2|BIT3,BIT0|BIT2|BIT4,BIT2,BIT2,BIT2,0x00},/* (5 X 8 )���ϵļ�ͷ  */
				{0x00,BIT2,BIT2,BIT2,BIT0|BIT2|BIT4,BIT1|BIT2|BIT3,BIT2,0x00},/* (5 X 8 )���µļ�ͷ*/
				{0x00,0x00,BIT2,BIT1,0x1f,BIT1,BIT2,0x00},/* (5 X 8 )���ҵļ�ͷ*/
				{0},
				{0},
				{0},
				{0},
				{0}
			 };
#endif

TYPE_OF_FILT BubbleSort1(TYPE_OF_FILT *a, int n, int m)  
{  
   int i, j;  
   TYPE_OF_FILT t;
   float x1 = 0;
   
   for (i = 0; i < n; i++) 
   {
      for (j = 1; j < n - i; j++) 
      {
         if (a[j - 1] > a[j])
         {
          	//Swap(a[j - 1], a[j]);  
          	t = a[j];
			a[j] = a[j-1];
			a[j-1] = t;
         }
      }
   }
	
   for(i=m;i<(n-m);i++)
   {
		x1+=a[i];
   }

   x1/=(n-m*2);

   return (TYPE_OF_FILT)x1;
} 

void swgFindUpEdge(void)
{
	int i,j;
	INT16U dat;	
	float tfl=0;
	
	//��ת800������Ϊ��ǰ�϶�����Ч�����ڣ����0-800
	//���740nm����Ӧ773��
	for(j=0;j<800;j++)
	{
		motorInv();				
		utDelay1ms (2);
	}

	//��12λ����
	//WriteDataCfgReg(0x90,0x00);	//00->12λ
	setADRefBitsAmp (12, 1);
	utDelay1ms (5);
	
	showCheckHome ();
	//���㼶����������
	dat = 0;
	while(1)
	{
		#if 1
		tfl = 0.8*dat+0.2*(ReadDataCfgReg(0x91,0x01));	//ԭ��dat����ȫ�ֱ���������?
		dat = (INT16U)tfl;
		#else
		dat=ReadDataCfgReg(0x91,0x01);	//�ѵ����и���? ����0.5�˲�
		#endif

		#if 1
		if(dat>VT_BG1)
		{
			//utPrt("d=%d\n",dat);
			sprintf(str,"d=%d\n",dat);
			utPtn(str);
			//utPtn1(str);
		}
		#endif
		
		if(dat>VT_BG2)	//��Ӧ��Լ900mv
		{
			#if 0
			//for(i=0;i<3;i++)
			{
				motor();
				utDelay1ms(2);				
			}
			#endif
			break;
		}
		
		motor();
		utDelay1ms(5);
		if(((++i)%30)==29)
		{
			utPtb('&');
			if(i>10000)
			{
				while(1)
				{
					utDelay1ms(50);
					LAMP_TOG;
				}
			}
		}
	}

	//��Ϊ14λ��ȷ��������
	//WriteDataCfgReg(0x90,0x04);	
	setADRefBitsAmp (14, 1);	//14λ, �Ŵ�1����ʵ������1.7V����
	utDelay1ms(20);				//�ȴ������ȶ�,20msʵ����14λ1��ת��
}

//��ʱ�����656���������壬���൱��658nm��681���崦
BOOLEAN swgCal656nm(void)
{
	int i,j;
	INT16U twd,wdMax=0;
	INT16U tbWd[NUM_OF_FILT];
	//INT16U tb656nm[5];

	swgFindUpEdge();
	//�ҵ����㼶��������

	j = 0;
	//�����ֵ
	while(1)
	{
		for(i=0;i<NUM_OF_FILT;i++)
		{
			tbWd[i]=ReadDataCfgReg(0x91,0x01);
			utDelay1ms(20);
		}
		twd = BubbleSort1(tbWd,NUM_OF_FILT,NUM_OF_RMV);		
		
		++j;
		utPrt("@%d:%x\n",j,twd);
		sprintf(str,"@%d:%x\n",j,twd);
		//utPtn1(str);
		
		if(twd<wdMax)
		{
			utPrt("begin@%d:%x\n",j,twd);
			sprintf(str,"begin@%d:%x\n",j,twd);
			//utPtn1(str);

			if(twd<VT_MAX)
			{
				//���ֵ������ͷ����
				sprintf(str,"tw=%x\n",twd);
				//sprintf(str,"begin@%d:%x\n",j,twd);
				//utPtn1(str);
				showStr();
				swgFindUpEdge();	
				continue;
			}
			
			break;
		}
		else 
		{
			wdMax = twd;	//�������ֵ
			motor ();		//����ת
		}
	}

	#if 0	//Ϊ�˲��Կ�����sig
	for(i=0;i<200;i++)			//�Ѵ�Լǰ200ȥ��
	{
		motor ();
		utDelay1ms (4);
	}

	setAD16Amp (2);	//14λ, 8��
	utDelay1ms (100);			//�ȴ������ȶ�
	for(i=200;i<700;i++)
	{
		dat=ReadDataCfgReg(0x91,0x01);
		dat1=ReadDataCfgReg(0x93,0x01);
		utPrt("%x,%x,%d\n",dat,dat1,i);
		motor();
		utDelay1ms(100);
	}
	while(1)
	{
		LAMP_TOG;
		utDelay1ms(200);
	}
	#endif
	

	//�ҵ���ߵ���,ǰǰһ������ߵ㣬�����ǵ�2��
	//for(i=0;i<681;i++)		//680
	for(i=0;i<671;i++)		//680
	{
		motor ();
		utDelay1ms (2);
	}

	//����̫С�ˣ�Ҫ�Ŵ�8������
	//WriteDataCfgReg(0x90,0x07);	
	setADRefBitsAmp (14, 8);	//14λ, 8��
	utDelay1ms (20);			//�ȴ������ȶ�

	//��ʼ683��
	//�Ҵ�ʱ�ļ�ֵ
	pMaxRef = 0;
	maxRef = 0;
	for(j=0;j<MAX_656SCAN;j++)		//675-690,5
	{
		for(i=0;i<NUM_OF_FILT;i++)
		{
			tbWd[i]=ReadDataCfgReg(0x91,0x01);
			utDelay1ms(20);
		}
		motor ();			//ת����һ��
		twd = BubbleSort1(tbWd,NUM_OF_FILT,NUM_OF_RMV);
		if(twd>maxRef)
		{
			maxRef = twd;
			pMaxRef = j;
		}
		//tb656nm[j] = twd;
		utPrt("%x,%d\n",twd,j);
		sprintf(str,"%x,%d\n",twd,j);
		//utPtn1(str);

		utDelay1ms(40);	//�ȴ�100ms�ȶ�
	}

	sprintf(str,"max=%x,%d\n\n",maxRef,pMaxRef);
	//utPtn1(str);
	utPtn(str);

	//if(tb656nm[2]<0x0400)
	if(maxRef<VT_MAX656)
	{
		sprintf(str,"656=%x",maxRef);
		showStr();
		return false;
	}

	if((pMaxRef==0)||(pMaxRef==(MAX_656SCAN-1)))
	{
		sprintf(str,"maxref=%x",pMaxRef);
		showStr();
		return false;		
	}

	curStep = 679+(MAX_656SCAN-1-pMaxRef);	//679��Ӧ656nm

	//��λOK
	utPtn("location 656 ok!\n");

	//�ص�16λ,�̶������Ŵ�����
	//WriteDataCfgReg(0x90,0x0D);	//0D->16λ,�Ŵ�2��
	setAD16Amp(2);
	utDelay1ms(20);
	return true;
}

void swgLightBoerDLamp()
{
	int i;

	#if HEAT_LAMP
	//����10s
	GPIO_SetBits(GPIOA,GPIO_Pin_13); 	//ctrl1
	GPIO_SetBits(GPIOC,GPIO_Pin_9);		//ctrl0
	utPrt("\nbegin heat...\n");
	showHeating ();

	for(i=0;i<100;i++)
	{
		utPtb('#');
		LAMP_TOG;
		utDelay1ms(100);
	}

	//��ʼ����
	GPIO_ResetBits(GPIOA,GPIO_Pin_13);	//ctrl1 ->0
	utPrt("begin enlight lamp\n");
	#endif

	//�ȴ�5s�ȶ�
	showHeating ();
	for(i=0;i<100;i++)
	{
		utPtb('*');
		LAMP_TOG;
		utDelay1ms (50);
	}
	utPtn("\nlamp stable\n\n");
}

void bgCal656nm(void)
{
	int i;
	
	#if HEAT_LAMP
	swgLightBoerDLamp();
	#endif
	//showWaveCal();	

	for(i=0;i<5;i++)
	{
		if(swgCal656nm())
			break;
	}
	//У������656nm
	if(i==5)
	{
		while(1)
		{
			utDelay1ms (100);
			LAMP_TOG;
		}
	}

	//LAST_DIR = 0;	//��ʱһֱ����ת

	#if 0
	sti = STI_WtStNm;
	nxtStep = START_SCAN;
	
	gotoWave();	
	showOnScan();
	#endif

	#if 0
	while(1)
	{
		LAMP_TOG;
		utDelay1ms(200);
	}
	#endif
}

void gotoWave()
{
	if(nxtStep<curStep)
	{	
		cntMotorInv = curStep-nxtStep;	//��ת 
		stMotor = STMT_INV;
	}
	else if(nxtStep>curStep)
	{	
		cntMotorPos = nxtStep-curStep;	//��ת
		stMotor = STMT_POS;
	}
	else
		stMotor = STMT_IDLE;

	utSetFlag (F_AUTOZERO);
}


#if 1	//DAC8560, �´ε�����һ���ļ�
#define DAC8560_PORT	GPIOB
#define DAC8560_CS		GPIO_Pin_7
#define DAC8560_CLK		GPIO_Pin_6
#define DAC8560_DAT		GPIO_Pin_5

#define DAC8560_CS_HIGH  	GPIO_SetBits(DAC8560_PORT, DAC8560_CS)
#define DAC8560_CLK_HIGH	GPIO_SetBits(DAC8560_PORT, DAC8560_CLK) 
#define DAC8560_DAT_HIGH	GPIO_SetBits(DAC8560_PORT, DAC8560_DAT)
#define DAC8560_CS_LOW  	GPIO_ResetBits(DAC8560_PORT, DAC8560_CS)
#define DAC8560_CLK_LOW		GPIO_ResetBits(DAC8560_PORT, DAC8560_CLK) 
#define DAC8560_DAT_LOW		GPIO_ResetBits(DAC8560_PORT, DAC8560_DAT)


void swgInitDAC8560(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	DAC8560_CS_HIGH;
	DAC8560_CLK_HIGH; 
	DAC8560_DAT_HIGH;

	//��ʼ������Һ������DAC���ƹܽ�
	//AFIO->MAPR |= BIT26;                            //����JTAG�ͷ�IO	
	GPIO_InitStructure.GPIO_Pin = DAC8560_CS+DAC8560_CLK+DAC8560_DAT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  
	GPIO_Init(DAC8560_PORT, &GPIO_InitStructure); 

	DAC8560_CS_HIGH;
	DAC8560_CLK_HIGH; 
	DAC8560_DAT_HIGH;

	utDelay1us(1);
	
	utDAC8560Write(0);	//��ʼ���̶����0
}

DT_PC dtPC;

//Ҫ���壬��ʱ�����ܷ���
void FormAMsg(INT8U cmd, INT8U len, INT8U *pDat)
{
	int i;
	INT8U iCOM_CRC;
	INT8U rnd;

	if(!utIsFlag (F_SYNCED))
	{
		utPtn("det unsynced\n");
		return;
	}

	rnd = utRandINT8();
	if(rnd==0)
		rnd = 0x5a;
	else if(rnd==0xff)
		rnd = 0xa5;
		
	key = (((keyCode[HHB(rnd)])<<4)+(keyCode[LHB(rnd)]));	//�õ��������ӣ�Ҳ�ǽ������ӣ����ܳ�

	#if ENCRYPT
	tx1Buf[iCOM_RND] = rnd;
	#else
	key = 0;	
	tx1Buf[iCOM_RND] = 0x55;
	#endif
	
	tx1Buf[iCOM_SD] = 0x81;
	tx1Buf[iCOM_CMD] = cmd;
	tx1Buf[iCOM_LEN] = len;

	//�������
	memcpy(((char *)(&(tx1Buf[iCOM_DAT]))),(char *)pDat,len);

	//У��
	iCOM_CRC = (iCOM_DAT+tx1Buf[iCOM_LEN]);
	tx1Buf[iCOM_CRC] = tx1Buf[0];
	for(i=1;i<iCOM_CRC;i++)
	{
		tx1Buf[i]^=key;
		tx1Buf[iCOM_CRC] += tx1Buf[i];	
	}
	
	tx1BufLen = iCOM_CRC+1;
	
	//if(cmd == CMD_DET_DATA)
	//��ǰ�Ļ�δ�������
	if(fTx2Send)
	{
		utPrt("\n%d:unsend find, new sn=%x\n",nowTime,tx1Buf[iCOM_DAT]);
	}
	
	fTx2Send = 1;
}

//ֱ���ܷ��͵�
void FormAAck(INT8U cmd, INT8U len, INT8U *pDat)
{
	int i;
	INT8U iCOM_CRC;
	INT8U rnd;

	//�õ��������?
	fTx2Send = 0;

	rnd = utRandINT8();
	if(rnd==0)
		rnd = 0x5a;
	else if(rnd==0xff)
		rnd = 0xa5;
		
	key = (((keyCode[HHB(rnd)])<<4)+(keyCode[LHB(rnd)]));	//�õ��������ӣ�Ҳ�ǽ������ӣ����ܳ�

	#if ENCRYPT
	tx1Dat[iCOM_RND] = rnd;
	#else
	key = 0;
	tx1Dat[iCOM_RND] = 0x55;
	#endif
	
	tx1Dat[iCOM_SD] = 0x81;
	tx1Dat[iCOM_CMD] = cmd;
	tx1Dat[iCOM_LEN] = len;

	//�������
	memcpy(((char *)(&(tx1Dat[iCOM_DAT]))),(char *)pDat,len);

	//У��,��CRC֮��������ֽ�
	iCOM_CRC = (iCOM_DAT+tx1Dat[iCOM_LEN]);
	tx1Dat[iCOM_CRC] = tx1Dat[0];
	for(i=1;i<iCOM_CRC;i++)
	{
		tx1Dat[i] ^= key;
		tx1Dat[iCOM_CRC] += tx1Dat[i];	
	}
	
	tx1Len = iCOM_CRC+1;

	//���̷���		
	//���־�����ó��ȣ���ʹ��
	fSendOver = 0;
	DMA1_Channel4->CNDTR = tx1Len;
	DMA_Cmd(DMA1_Channel4, ENABLE);	
	//utPtn("ack\n");
}


//16λDAC,���뵥λΪuV
void utDAC8560WriteF(float fDat)
{
	static INT8U btDetDat[5];
	INT8U i;
	float x;
	//static float oldDat;
	INT16U dat=0;

	x = fDat;

	#if 1
	if(fEnUpDat)
	{
		//�����߼����Ӻ��������
		if(utIsFlag (F_OnCONNECTED))
		{
			dtPC.fl = x;
			btDetDat[0] = snDet++;
			memcpy((char *)(btDetDat+1),(char *)(dtPC.bt),4);
			FormAMsg(CMD_DET_DATA,5,btDetDat);
		}
	}
	else
		snDet = 0;
	#endif

	#if 1
	//��ΧΪ����1V
	if(x<-1000000.0)
	{
		x = 1000000.0;
	}
	else if(x>1000000.0)
		x = 1000000.0;

	#if 1
	x+=1000000.0;					//ƫ�Ƶ�1V
	#endif

	x = (x*65536.0/2500000.0);		//���㵽DA��ֵ
	dat = (INT16U)x;
	#endif
	
	DAC8560_CLK_LOW;
	DAC8560_CS_LOW;

	//����8��0
	for(i=0;i<8;i++)
	{
		DAC8560_CLK_HIGH;	//������֪ͨ�������
		DAC8560_DAT_LOW;	//������0
		utDelay1us(1);		//������
		DAC8560_CLK_LOW;	//�½��ؽ���������DA
		utDelay1us(1);		//������
	}

	//����16λ���ݣ�����ǰ
	for(i=0;i<16;i++)
	{
		DAC8560_CLK_HIGH;	//������֪ͨ�������
		
		if(dat&BIT15)		//��15����Ū��16��
			DAC8560_DAT_HIGH;
		else
			DAC8560_DAT_LOW;

		dat <<= 1;			//����һλ	
		utDelay1us(1);		//������
		
		DAC8560_CLK_LOW;	//�½��ؽ���������DA
		utDelay1us(1);		//������
	}

	for(i=0;i<2;i++)		//�ٶ��ͼ���
	{
		DAC8560_CLK_HIGH;	//������֪ͨ�������
		DAC8560_DAT_LOW;	//������0
		utDelay1us(1);		//������
		DAC8560_CLK_LOW;	//�½��ؽ���������DA
		utDelay1us(1);		//������
	}

	//�����CLK,CS���ظ�
	DAC8560_CLK_HIGH;
	DAC8560_CS_HIGH;

	//utDelay1us(1);			//��ʱ�����´�ֱ��д
}

//16λDAC
void utDAC8560Write(INT16U dat)
{
	INT8U i;
	//float x;
	//static INT16U oldDat;

	//x = 0.95*dat+0.05*oldDat;
	dat = (INT16U)dat;
	//oldDat = dat;
	
	DAC8560_CLK_LOW;
	DAC8560_CS_LOW;

	//����8��0
	for(i=0;i<8;i++)
	{
		DAC8560_CLK_HIGH;	//������֪ͨ�������
		DAC8560_DAT_LOW;	//������0
		utDelay1us(1);		//������
		DAC8560_CLK_LOW;	//�½��ؽ���������DA
		utDelay1us(1);		//������
	}

	//����16λ���ݣ�����ǰ
	for(i=0;i<16;i++)
	{
		DAC8560_CLK_HIGH;	//������֪ͨ�������
		
		if(dat&BIT15)		//��15����Ū��16��
			DAC8560_DAT_HIGH;
		else
			DAC8560_DAT_LOW;

		dat <<= 1;			//����һλ	
		utDelay1us(1);		//������
		
		DAC8560_CLK_LOW;	//�½��ؽ���������DA
		utDelay1us(1);		//������
	}

	for(i=0;i<2;i++)		//�ٶ��ͼ���
	{
		DAC8560_CLK_HIGH;	//������֪ͨ�������
		DAC8560_DAT_LOW;	//������0
		utDelay1us(1);		//������
		DAC8560_CLK_LOW;	//�½��ؽ���������DA
		utDelay1us(1);		//������
	}

	//�����CLK,CS���ظ�
	DAC8560_CLK_HIGH;
	DAC8560_CS_HIGH;

	utDelay1us(1);			//��ʱ�����´�ֱ��д
}

#endif

#if 1	//�����ת�ͷ�תһ��
enum
{
	DIR_POS = 0,
	DIR_INV = 1
};

#define INV_DIR	{pMPhase = (7- pMPhase);}//{pMPhase = (9- pMPhase)%8;}

void motor(void)
{
	
	if(LAST_DIR != DIR_POS)	//ԭ���������ģ�Ҫ��ת
	{
		INV_DIR;
		LAST_DIR = DIR_POS;	//����״̬Ϊ����
	}
	
	dat = (GPIOB->ODR);
	dat &= ~(0xfc00);
	dat |= (tbMotor[pMPhase]<<10);
	(GPIOB->ODR) = dat;
	
	if(++pMPhase==8)
		pMPhase = 0;
}

void motorInv(void)
{
	if(LAST_DIR != DIR_INV)	//ԭ�����Ƿ��ģ�Ҫ��ת������
	{
		INV_DIR;
		LAST_DIR = DIR_INV;	//��Ϊ����״̬
	}
	
	dat = (GPIOB->ODR);
	dat &= ~(0xfc00);
	dat |= (tbMotorInv[pMPhase]<<10);
	(GPIOB->ODR) = dat;
	
	if(++pMPhase==8)
		pMPhase = 0;
}
#endif

#if 0
void get
strctStore.wave = wDat;
tfl=5000.0/3.1415926*asin(1.0*strctStore.wave/1585.0);
nxtStep = Round(tfl);
#endif

INT16U getNmFromStep(INT16U step)
{
	static float tfl;
	tfl = 1585.0*sin(0.036*step*3.1415926/180.0);
    return ((INT16U)(tfl+0.5));
}

INT16U getStepFromNm(INT16U nm)
{
	static float tfl;
	tfl=5000.0/3.1415926*asin(1.0*nm/1585.0);	
    return ((INT16U)(tfl+0.5));
}

