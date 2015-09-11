#include "utDisplay.h"

//INT8U stMenu=0;
INT8U bDat;
INT8U itemChg=0;
INT8U posOfNum=0;
//INT8U fEdit=0;		//是否在编辑状态
int iDat;
float fDat;
#if 0
const INT8U lenOfNumChg[3] = {5,2,2};
const INT8U cursorChgTk[3][2] = {{0,0},{1,5},{1,11}};
const INT8U cursorRangeTk[2] = {0,11};
const INT8U cursorMediaTk[2] = {0,10};
const INT8U cursorZeroTk[2] = {0,9};
const INT8U cursorGradeTk[2] = {0,8};
const INT8U cursorChgNum[3][2] = {{0,2},{1,8},{1,14}};
#endif

const INT8U lenOfNumChgWave[2] = {3,4};		//可闪烁的总位数
const INT8U cursorChgWave[2][2] = {{1,2},{1,8}};	//第1个的起始行和列，第2个的起始行和列。0第一行，1第2行

const INT8U cursorDA[] = {1,12};

menuPage stMenu=MENU_MAIN;

INT16U Round(float x) 
{ 
	return (INT16U)(x+0.5); 
}

void showTest()
{
    DisplayClear();
	ramprt(0,0,"1234HPLC PUMP");
	ramprt(1,0,"5678HPLC PUMP");
}

void showInit()
{
    DisplayClear();
	ramprt(0,0,"   UV DETECTOR  ");
	ramprt(1,0,"    REV 1.39    ");	
}

void showWrg()
{
    DisplayClear();
	ramprt(0,0,"   5 times wrg  ");
	ramprt(1,0,"   locate err   ");	
}


void showHeating()
{
	//U  V	D  E  T  E C T O R
    DisplayClear();
	ramprt(0,0,"   UV DETECTOR  ");
	ramprt(1,0,"   D2 Heating * ");	
}

void showCheckHome()
{
    DisplayClear();
	ramprt(0,0,"Wating....");
	ramprt(1,0,"Check home pos");	
}

//P1
//此处要实时显示当前波长的信号(工作站上显示的)
//还要显示波长符号'lamda'
void showMain(void)
{
    DisplayClear();
	sprintf((char *)str,"signal:   %6.4f",(bkTfl/1000000.0));
	ramprt(0,0,str);
	sprintf((char *)str,"A:%3d t:%4.2f  ON",getNmFromStep(curStep),flDetTao[strctStore.pTao]);
	ramprt(1,0,str);	
}

#if 0
void showIdle(void)
{
	DisplayClear();
	sprintf((char *)str,"signal:   %6.4f",0.0);
	ramprt(0,0,str);
	sprintf((char *)str,"A:%3d t:1.00 OFF",getNmFromStep(curStep));
	ramprt(1,0,str);	
}
#endif

#if 0
void showMainMenu(void)
{
	DisplayClear();
	ramprt(0,0,"");
	sprintf((char *)str,"A:%3d t:1.00 OFF",getNmFromStep(curStep));
	ramprt(1,0,str);	
}
#endif

//P2: 0.1、0.2、0.5、1、2、5
//此处要动态显示放大倍数
//daRange

void showDAOut()
{
    DisplayClear();
	ramprt(0,0,"ANALOG OUT: [AU]");
	//sprintf((char *)str,"full scale: %4.2f",(strctStore.amp/1000.0));
	sprintf((char *)str,"full scale: %4.2f",(daRange[strctStore.pDaRange]));
	ramprt(1,0,str);
}

//P3
void showSRInfo()
{
    DisplayClear();
	
	#if 0
	ramprt(1,0,"INFO:  sig  1.27");
	ramprt(1,0,"       ref  0.82");
	#endif
	
	sprintf(str,"INFO:  sig %5.2f",(ftADSig16.datProc/25000.0));
	ramprt(0,0,str);
	sprintf(str,"       ref %5.2f",(ftADRef16.datProc/25000.0));
	ramprt(1,0,str);
}

//P4
void showGLP()
{
    DisplayClear();
	
	#if 0
	ramprt(1,0,"GLP      +1   11");
	ramprt(1,0,"    0920.0h  112");
	#endif

	sprintf(str,"GLP      +1   %02d",11);
	ramprt(0,0,str);
	sprintf(str,"    %6.1fh %4d",920.0,112);
	ramprt(1,0,str);

}

//P6
//S  :  5  2  0 0  6    V : 1 . 4 9
void showSN()
{
    DisplayClear();
	ramprt(0,0,"   UV DETECTOR  ");
	//ramprt(1,0," S:52006 V:1.39 ");	
	sprintf(str," S:%05d V:1.39 ",52006);
	ramprt(1,0,str);
}


#if 0
//P5: 预留，先不管
void showWaveCal()
{
    DisplayClear();
	ramprt(0,0,"A-calibration");
	sprintf((char *)str,"  0-calibration");
	ramprt(1,0,str);
}

void showToScan()
{
	DisplayClear();
	ramprt(0,0,"   UV DETECTOR  ");
	ramprt(1,0,"  Enter to Scan "); 
}

void showToDet()
{
	DisplayClear();
	ramprt(0,0,"   UV DETECTOR  ");
	ramprt(1,0,"   Enter to Det "); 
}


void showOnScan()
{
	DisplayClear();
	ramprt(0,0,"   UV DETECTOR  ");
	if(utIsFlag (F_SCANX))
		ramprt(1,0,"  on ScanningX  "); 
	else if(utIsFlag (F_SCAN0))
		ramprt(1,0,"  on Scanning0  ");
	else
		ramprt(1,0,"  on Scanning   "); 
}

void showWaveSet()
{

}

void showOnDetecting()
{
	utSetFlag (F_OnDECTING);
	DisplayClear();
	ramprt(0,0,"  UV DETECTOR   ");
	ramprt(1,0,"  on Detecting  "); 
}
#endif

void showStr()
{
    DisplayClear();
	ramprt(0,0,str);
}

void utTwk(INT8U cnt)
{
	utDelay1ms(1);
	if(++cnt1ms>cnt)
	{
		cnt1ms=0;
		LAMP_TOG;
	}
}

void keyProc()
{
	sprintf(str,"key=%x\n",KeyVal);
	utPtn(str);
	
	switch(stMenu)
	{
		case MENU_IDLE:
			{
				#if 0
				switch(KeyVal)
				{
					case KEY_ENTER:
						sti = STI_DETECTING;
						stMenu = MENU_MAIN;
						showMain();
						break;

					case KEY_MENU:
						showMainMenu ();
						stMenu = MENU_MAINMENU;
						sti = STI_IDLE;
						break;

					#if 0
					case KEY_MENU:
						showDAOut ();
						stMenu = MENU_DAOUT;
						break;					
					#endif	
				}
				#endif
			}
			break;

		#if 1
		case MENU_MAIN:				//相当于detecting
			switch(KeyVal)
			{
				case KEY_MENU:
					{
						showDAOut ();
						sti = STI_IDLE;
						stMenu = MENU_DAOUT;
					}
					break;
				
				case KEY_ENTER:
					#if 1
					if(sti != STI_DETECTING)	//如果不是检测状态就返回
					{
						break;
					}
					#endif

					//非编辑状态，现在进入编辑状态
					utSetFlag (F_MEDIT);
					//闪烁
					posOfNum = 0;
					SetCursorPos(cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]+posOfNum);
					BlinkOn();
					sti = STI_IDLE;
					stMenu = MENU_MAINMENU;
					break;

				case KEY_ZERO:
					ZeroOut();
					#if 0
					tfl = 1000.0*strctStore.offset;		//放大到uv
					tfl -= bkTfl;						//减去背景的
					strctStore.offset = (INT16U)(tfl/1000.0);	//折算回去
					#endif
					break;

				case KEY_D2:
					//LAMP
					break;
			}
			break;
		#endif
			
		case MENU_MAINMENU:
			{
				switch(KeyVal)
				{
					#if 1
					case KEY_MENU:
						{
							if(itemChg==1)
							{
								if(strctStore.pTao++>=5)
									strctStore.pTao = 0;

								SetCursorPos (cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]);
								//重新显示，滤除非法字符
								sprintf(str,"%4.2f",flDetTao[strctStore.pTao]);
								rampts(str);
								SetCursorPos (cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]);
							}
						}
						break;
					#endif

					#if 1
					case KEY_ENTER:
						if(utIsFlag(F_MEDIT))
						{
							if(itemChg==0)	//wave
							{								
								//读入数据
								sscanf((char *)&(cacheLCD[cursorChgWave[itemChg][0]][cursorChgWave[itemChg][1]]),"%d",&iDat);
								strctStore.wave = (INT16U)iDat;

								#if 0
								tfl=5000.0/3.1415926*asin(1.0*strctStore.wave/1585.0);
								nxtStep = Round(tfl);
								#endif
								nxtStep=getStepFromNm(strctStore.wave);							
															
								#if 1
								//设置显示的光标
								SetCursorPos (cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]);
								//重新显示，滤除非法字符
								sprintf(str,"%3d",iDat);
								rampts(str);
								#endif

								utPrt("wave=%d,old=%d,step=%d\n",iDat,curStep,nxtStep);
							}
							else	//t
							{
								#if 0
								sscanf((char *)&(cacheLCD[cursorChgWave[itemChg][0]][cursorChgWave[itemChg][1]]),"%f",&fDat);
								AppConfig.time = fDat;
															
								#if 1
								//设置显示的光标
								SetCursorPos (cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]);
								//重新显示，滤除非法字符
								if(fDat>=100)
									sprintf(str,"%4.0f",fDat);
								else if(fDat>=10)
									sprintf(str,"%4.1f",fDat);
								else
									sprintf(str,"%4.2f",fDat);
								rampts(str);
								#endif

								sprintf(str,"time=%f\n",fDat);
								utPtn(str);
								#endif
							}

							//切换到下一个
							if(++itemChg==2)
							{
								itemChg=0;
								storeData();

								//现在是光标会在后一个位置闪一下才消失，怎么回事?
								#if 0
								if(posOfNum>=lenOfNumChgWave[itemChg])	//光标弄回来
								{
									SetCursorPos(cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]+posOfNum-1);
								}
								#endif
								BlinkOff();								

								//退出编辑状态
								utClrFlag (F_MEDIT);
								
								#if 1
								//开始扫描输出
								cntFilt = 0;
								showMain();

								sti = STI_WTXnm;
								stMenu = MENU_MAIN;
								gotoWave ();	
								#endif
							}
							else				//闪烁下一个光标
							{
								posOfNum=0;
								SetCursorPos (cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]);
							}
							
							break;
						}
						utPtn("wrg\n");

						#if 0
						//非编辑状态，现在进入编辑状态
						utSetFlag (F_MEDIT);
						//闪烁
						posOfNum = 0;
						SetCursorPos(cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]+posOfNum);
						BlinkOn();
						#endif

						break;

					case KEY_CE:
						if(utIsFlag(F_MEDIT))
						{
							if(itemChg==0)	//wave
							{	
								#if 1
								//设置显示的光标
								SetCursorPos (cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]);
								//重新显示，滤除非法字符
								sprintf(str,"%3d",strctStore.wave);
								rampts(str);
								posOfNum = 0;
								SetCursorPos(cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]+posOfNum);
								#endif
							}
						}
						break;
					
					case KEY_D2:
					case KEY_ZERO:
						break;

					default:
						if(posOfNum>=lenOfNumChgWave[itemChg])	//超过长度，直接返回
							break;
						
						SetCursorPos(cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]+posOfNum);
						bDat = getAsciiOfKey();
						if(!bDat)	//不是要的数字直接返回
							break;
						
						ramptc(bDat);
						//cacheLCD[L_X][L_Y] = ' ';			//后面的缓存要自动变为空格，以截至
						VSTM0802SetCurChar(' ');
						posOfNum++;
						if(posOfNum>=lenOfNumChgWave[itemChg])	//光标弄回来
						{
							SetCursorPos(cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]+posOfNum-1);
							break;	
						}					
						break;
					#endif
		
				}
			}
			break;

		case MENU_DAOUT:
			{
				switch(KeyVal)
				{
					case KEY_MENU:
						{
							if(!utIsFlag(F_MEDIT))
							{
								showSRInfo ();
								stMenu = MENU_SRINFO;
							}
							else	//切换相应量程
							{
								if(strctStore.pDaRange++>=5)
									strctStore.pDaRange = 0;
								//strctStore.amp = (INT16U)(daRange[strctStore.pDaRange]*1000);
								showDAOut ();
								SetCursorPos(cursorDA[0], cursorDA[1]);
								//BlinkOn();		
							}
						}
						break;

					case KEY_ENTER:
						{
							if(utIsFlag(F_MEDIT))	
							{
								//blink off	
								BlinkOff();
								utClrFlag(F_MEDIT);
							}
							else
							{
								//blink on
								SetCursorPos(cursorDA[0], cursorDA[1]);
								BlinkOn();								
								utSetFlag(F_MEDIT);
							}
						}
						break;		
				}
			}
			break;			

		case MENU_SRINFO:
			{
				switch(KeyVal)
				{
					case KEY_MENU:
						{
							showGLP ();
							stMenu = MENU_GLP;
						}
						break;

					case KEY_ENTER:
						{
					
						}
						break;		
				}		
			}
			break;
		
		case MENU_GLP:
			{
				switch(KeyVal)
				{
					case KEY_MENU:
						{
							showSN ();
							stMenu = MENU_SN;
						}
						break;

					case KEY_ENTER:
						{
					
						}
						break;		
				}		
			}
			break;

		#if 0
		case MENU_WAVECAL:
			{
				switch(KeyVal)
				{
					case KEY_MENU:
						{

						}
						break;

					case KEY_ENTER:
						{
					
						}
						break;		
				}		
			}
			break;
		#endif

		case MENU_SN:
			{
				switch(KeyVal)
				{
					case KEY_MENU:
						{
							#if 0
							//showMain ();
							if(sti==STI_IDLE)
								showIdle ();
							else 
								showMain();
							stMenu = MENU_MAIN;	
							#else
							sti = STI_DETECTING;
							stMenu = MENU_MAIN;
							showMain();
							#endif
						}
						break;
		
					case KEY_ENTER:
						{
					
						}
						break;		
				}		
			}
			break;
	}
}

void keyProc1()
{
	//int i;
	
	if((sti==STI_SCAN)||(sti==STI_WtStNm)||(sti==STI_WTXnm)) 
		return;
	
	#if 0
	switch(stMenu)
	{
		case MENU_MAIN:
		{
			switch(KeyVal)
			{
				case KEY_MENU:
					showToScan();
					stMenu = MENU_SCAN;
					break;
				
				#if 0
				case KEY_D2:
					{
						if(sti==STI_IDLE)
						{
							sti = STI_SAMPLE;
							showOnScan();
						}	
					}
					break;

				case KEY_ZERO:
					showWaveSet();
					stMenu = MENU_DETECT;
					break;
				#endif
			}
		}
		break;

		case MENU_SCAN:
		{
			switch(KeyVal)
			{
				case KEY_MENU:
					//showToDet();
					//twnkle
					showMain();
					utClrFlag(F_MEDIT);	//进入非编辑状态等待Enter
					stMenu = MENU_DETECT;
					break;
					
				case KEY_ENTER:
					sti = STI_WtStNm;
					nxtStep = START_SCAN;
					gotoWave();					
					//showOnScan();
					showMain();
					break;

				//其它按键不响应
				
			}
		}
		break;		

		case MENU_DETECT:
		{
			switch(KeyVal)
			{
				case KEY_MENU:
					if((stMotor!=STMT_IDLE))	//电机寻nm的时候菜单无效
						break;

					if(utIsFlag(F_MEDIT))
						break;

					if(utIsFlag(F_OnDECTING))
					{
						utClrFlag (F_OnDECTING);
						//utPtn1("daout:\n");
						for(i=0;i<600;i++)
						{
							sprintf(str,"%d\n",daDat[i]);
							//utPtn1(str);
						}
						utDAC8560Write(0);
					}
					showInit();
					stMenu = MENU_MAIN;
					break;

				#if 1
				case KEY_ENTER:
					if(utIsFlag(F_MEDIT))
					{
						//退出编辑状态
						utClrFlag (F_MEDIT);
						BlinkOff();
						
						//读入数据
						sscanf((char *)&(cacheLCD[cursorChgWave[itemChg][0]][cursorChgWave[itemChg][1]]),"%d",&iDat);
						strctStore.wave = (INT16U)iDat;
						tfl=5000.0/3.1415926*asin(1.0*strctStore.wave/1585.0);
						nxtStep = Round(tfl);
						
						#if 0
						//nxtStep -= 1;
						if(nxtStep>1)
							nxtStep -= 1;
						#endif
						
						#if 1
						//设置显示的光标
						SetCursorPos (cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]);
						//重新显示，滤除非法字符
						sprintf(str,"%3d",iDat);
						rampts(str);
						#endif

						utPrt("wave=%d,old=%d,step=%d\n",iDat,curStep,nxtStep);
						sprintf(str,"\nwave=%d,old=%d,step=%d\n",iDat,curStep,nxtStep);
						//utPtn1(str);

						
						//编辑状态
						//开始扫描输出
						//cntDetect = 0;
						cntFilt = 0;
						//showOnDetecting ();
						showMain();

						sti = STI_WTXnm;
						gotoWave ();						
						break;
					}
					
					//非编辑状态，现在进入编辑状态
					utSetFlag (F_MEDIT);
					//闪烁
					posOfNum = 0;
					SetCursorPos(cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]+posOfNum);
					BlinkOn();

					break;
				
				case KEY_D2:
				case KEY_ZERO:
				case KEY_CE:
					break;

				default:
					if(posOfNum>=lenOfNumChgWave[itemChg])	//超过长度，直接返回
						break;
					
					SetCursorPos(cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]+posOfNum);
					bDat = getAsciiOfKey();
					if(!bDat)	//不是要的数字直接返回
						break;
					
					ramptc(bDat);
					cacheLCD[L_X][L_Y] = ' ';			//后面的缓存要自动变为空格，以截至
					posOfNum++;
					if(posOfNum>=lenOfNumChgWave[itemChg])	//光标弄回来
					{
						SetCursorPos(cursorChgWave[itemChg][0], cursorChgWave[itemChg][1]+posOfNum-1);
						break;	
					}					
					break;
				#endif
			}
		}
		break;
		
	}
	#endif
}

