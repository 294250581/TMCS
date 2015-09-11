#include "utEvtProc.h"
#include "Encoder.h"
#include "dSPIN.h"
#include "motor.h"
#include "Can_network.h"
#include "Iobus.h"


#if 0
INT32U motor_abs[20] = {1600, 3200, 4800, 6400, 8000, 9600, 11200, 12800, 14400,
                        16000, 17600, 19200, 20800, 22400, 24000, 25600, 27200,
                        28800, 30400, 32000};

INT32U enc_abs[20] = {500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000,
	                  5500, 6000, 6500, 7000, 7500, 8000, 8500, 9000, 9500, 10000};
#endif

#if 1
INT32U motor_abs[7] = {1280, 2560, 1280, 5120, 3840, 6400, 115200};

INT32U enc_abs[7] = {1000, 2000, 1000, 4000, 3000, 5000, 9000};
#endif


/* 多点重复测试 */
/********************************************/

INT8U multi_test_cmd = 0;
INT32U cal_step = 0;
INT16U abs_cal = 13;
#define upper_thread 15
#define lower_thread 5

INT8U multi_point_test(void)
{
	static INT32U i = 0, j = 0;
	static INT8U flag = 0;
	static INT16U Total_Time = 0;
	static INT16U Step_Count = 0;
	
	utBeginTask;
	
	utWait((multi_test_cmd == 1));
	multi_test_cmd = 0;
	utPtn("Multi test\n");

	/* 开始循环逼近 */
	for(j=0; j<10000; j++)
	{
		/*开始逼近  20个位置*/
		for(i=0; i<7; i++)
		{			
			/* 先运行到最终位置相差10步左右 */
			dSPIN_Go_To((motor_abs[i]-(abs_cal)), dSPIN_Device_X);
			utWait((sampler_get_motor_state(dSPIN_Device_X) == motor_stopped));
			utPrt("Number '%d' move stop1 Count = %d\r\n", i, TIM2->CNT);

			/* 矫正 */
			if((enc_abs[i] - TIM2->CNT) > upper_thread)
			{
//				printf("(enc_abs[i] - TIM2->CNT) > upper_thread \n");
				abs_cal = abs_cal - (uint32_t)((enc_abs[i] - TIM2->CNT - upper_thread)*3.2);
				dSPIN_Move(FWD, (uint32_t)((enc_abs[i] - TIM2->CNT - upper_thread)*4) , dSPIN_Device_X);
				utWait((sampler_get_motor_state(dSPIN_Device_X) == motor_stopped));
			}
			else if((enc_abs[i] - TIM2->CNT) < lower_thread)
			{
//				printf("(enc_abs[i] - TIM2->CNT) < lower_thread\n");
				abs_cal = abs_cal + (uint32_t)((lower_thread - (enc_abs[i] - TIM2->CNT))*3.2);
				dSPIN_Move(REV, (uint32_t)((lower_thread - (enc_abs[i] - TIM2->CNT))*4) , dSPIN_Device_X);
				utWait((sampler_get_motor_state(dSPIN_Device_X) == motor_stopped));
			}
			utPrt("Number '%d' move stop2 Count = %d\r\n", i, TIM2->CNT);

			while(TIM2->CNT != enc_abs[i])
			{
				if(flag == 0)
				{
					Total_Time = ut1ms;
					flag = 1;
				}
				Step_Count++;				
				dSPIN_Move(FWD, 1, dSPIN_Device_X);
				utWait((sampler_get_motor_state(dSPIN_Device_X) == motor_stopped));
			}
			
			Total_Time = (INT16U)(ut1ms-Total_Time);
			utPrt("Number '%d' Total_Time = %d\r\n", i, Total_Time);
//			printf("Number '%d' Step_Count = %d\r\n", i, Step_Count);
			utPrt("Number step_Count = %d\r\n", Step_Count);


			utDelay1ms(200);
			utPrt("Number '%d' Stable Count = %d\r\n", i,TIM2->CNT);
			utPrt("\n\n");
			flag = 0;
			Step_Count = 0;
			Total_Time = 0;
		}
	}
	utEndTask;
}

void utInitVariable(void)
{
	
}

void ut1msProc(void)
{
}

void ut10msProc(void)
{
	/* 重复性测试 */
	multi_point_test();

/********************************/
	tmcs_bsp_rfsn();
	tmcs_bsp_rfsc();
/********************************/
}

void ut100msProc(void)
{
	/* Can receive process */
	can_receive_process();
}

INT8U reset_flag = 0;
INT8U cc = 0;
void ut1sProc(void)
{
	LAMP_TOG;
	ENC_Deal();
	sprt("ABS = %d\n", dSPIN_Get_Param(dSPIN_ABS_POS,dSPIN_Device_X));

#if 0
	SendCanMsg(&g_tCanTxMsg);
#endif

#if 0
	if(reset_flag == 0)
	{
		multi_test_cmd = 1;
		reset_flag = 1;
	}
#endif

#if 0
	sprt("ke         = %f\n", motor_device_x.ke);
	sprt("inductance = %f\n", motor_device_x.motor_inductance);
	sprt("resistance = %f\n", motor_device_x.motor_resistance);	
	sprt("k_therm    = %d\n", motor_device_x.k_therm);	
	sprt("H_current  = %f\n", motor_device_x.hold_current);
	sprt("A_current  = %f\n", motor_device_x.acc_current);
	sprt("D_current  = %f\n", motor_device_x.dec_current);
	sprt("R_current  = %f\n", motor_device_x.run_current);
	sprt("acc_speed  = %f\n", motor_device_x.acc_speed);
	sprt("dec_speed  = %f\n", motor_device_x.dec_speed);
	sprt("max_speed  = %f\n", motor_device_x.max_speed);
	sprt("min_speed  = %f\n", motor_device_x.min_speed);
	sprt("full_speed = %f\n", motor_device_x.full_speed);
	sprt("stall_th   = %d\n", motor_device_x.stall_th);
	sprt("O_current  = %d\n", motor_device_x.over_current);
	sprt("step_mode  = %d\n", motor_device_x.step_mode);
#endif

#if 0
	utPrt("IOBUSA_TCB[SUB_A0].mode..%d .int..%d .intmode ..%d\n", 
		IOBUSA_TCB[SUB_A0].io_mode,IOBUSA_TCB[SUB_A0].io_int,IOBUSA_TCB[SUB_A0].io_intmode);
	utPrt("IOBUSA_TCB[SUB_A1].mode..%d .int..%d .intmode ..%d\n", 
		IOBUSA_TCB[SUB_A1].io_mode,IOBUSA_TCB[SUB_A1].io_int,IOBUSA_TCB[SUB_A1].io_intmode);
	utPrt("IOBUSA_TCB[SUB_A2].mode..%d .int..%d .intmode ..%d\n", 
		IOBUSA_TCB[SUB_A2].io_mode,IOBUSA_TCB[SUB_A2].io_int,IOBUSA_TCB[SUB_A2].io_intmode);
	utPrt("IOBUSA_TCB[SUB_A3].mode..%d .int..%d .intmode ..%d\n", 
		IOBUSA_TCB[SUB_A3].io_mode,IOBUSA_TCB[SUB_A3].io_int,IOBUSA_TCB[SUB_A3].io_intmode);

	utPrt("\n");

	utPrt("IOBUSB_TCB[SUB_A0].mode..%d .int..%d .intmode ..%d\n", 
		IOBUSB_TCB[SUB_A0].io_mode,IOBUSB_TCB[SUB_A0].io_int,IOBUSB_TCB[SUB_A0].io_intmode);
	utPrt("IOBUSB_TCB[SUB_A1].mode..%d .int..%d .intmode ..%d\n", 
		IOBUSB_TCB[SUB_A1].io_mode,IOBUSB_TCB[SUB_A1].io_int,IOBUSB_TCB[SUB_A1].io_intmode);
	utPrt("IOBUSB_TCB[SUB_A2].mode..%d .int..%d .intmode ..%d\n", 
		IOBUSB_TCB[SUB_A2].io_mode,IOBUSB_TCB[SUB_A2].io_int,IOBUSB_TCB[SUB_A2].io_intmode);
	utPrt("IOBUSA_TCB[SUB_A3].mode..%d .int..%d .intmode ..%d\n", 
		IOBUSB_TCB[SUB_A3].io_mode,IOBUSB_TCB[SUB_A3].io_int,IOBUSB_TCB[SUB_A3].io_intmode);
	utPrt("\n");

#endif
}

void utInputProc(void)
{
	
}

void utOutputProc(void)
{
	
}


