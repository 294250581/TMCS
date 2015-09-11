#include "Can_receive.h"

struct _t_can_state can_state = {0,0,0};
struct _t_can_rxmsg can_rxmsg;

/* 求和 */
INT16U can_sumcheck(INT8U *byte, INT8U number)
{
	INT16U sum = 0;
	INT8U  i = 0;

	for(i=0; i<number; i++)
	{
		sum = sum + *(byte+i);
	}

	/* 地址数据也在教研范围内 */
	sum = sum+can_parameter._t_std_id;

	/* 求和值超出8bit,取低8位 */
	if(sum > 255)
	{
		sum = (INT8U)sum;
	}

	return sum;
}


/* can 接收处理 */
INT8U can_receive_process(void)
{
	/* can已经接收到了数据 */
	if(can_state._t_can_receive == t_can_receive_true)
	{
		/* 接收标志清零 */
		utPrt("can received data\n");
		can_state._t_can_receive = t_can_receive_false;
	}

	/* can没有接收到数据 */
	else
		return 0;

	/* 一下部分的处理是在已经接收到数据的情况下 
	   程序临界段
	*/

	utEnterCrt;	

	/* 拷贝can接收数据到自定义缓存区 */
	memcpy(&can_rxmsg, &g_tCanRxMsg, sizeof(g_tCanRxMsg));

#if 1
	utPrt("StdID =   %d\n",   can_rxmsg.StdId);
	utPrt("ExtId =   %d\n",   can_rxmsg.ExtId);
	utPrt("IDE =     %d\n",     can_rxmsg.IDE);
	utPrt("RTR =     %d\n",     can_rxmsg.RTR);	
	utPrt("DLC =     %d\n",     can_rxmsg.DLC);	
	utPrt("Data[0] = %d\n", can_rxmsg.Data[0]);
	utPrt("Data[1] = %d\n", can_rxmsg.Data[1]);
	utPrt("Data[2] = %d\n", can_rxmsg.Data[2]);
	utPrt("Data[3] = %d\n", can_rxmsg.Data[3]);
	utPrt("Data[4] = %d\n", can_rxmsg.Data[4]);
	utPrt("Data[5] = %d\n", can_rxmsg.Data[5]);
	utPrt("Data[6] = %d\n", can_rxmsg.Data[6]);
	utPrt("Data[7] = %d\n", can_rxmsg.Data[7]);
	utPrt("FMI =     %d\n",     can_rxmsg.FMI);
	utPrt("\n\n");
#endif

	/* 求和检验 */
	if(can_sumcheck(&can_rxmsg.Data[0], 7) != can_rxmsg.Data[t_can_s_sumc])
	{
		utPrt("Checkout is error!\n");
		return 0;
	}

	/* 求和正确 开始解析数据 */
	switch(can_rxmsg.Data[t_can_s_instruction])
	{
		/* ROR(Rotate right)
           <!instructed to rotate with a specified velocity in right direction
           <!increasing the position counter 
		*/
		case t_can_ROR:
			utPrt("instruction..ROR\n");
			tmcs_ror(&can_rxmsg.Data[t_can_s_motor],&can_rxmsg.Data[t_can_s_byte3]);
		break;

		/* ROL(Rotate left)
           <!instructed to rotate with a specified velocity in left direction
           <!decreasing the position counter
		*/
		case t_can_ROL:
			utPrt("instruction..ROL\n");
			tmcs_rol(&can_rxmsg.Data[t_can_s_motor],&can_rxmsg.Data[t_can_s_byte3]);
		break;

		/* MST(motor stop)
           <!motor will be instructed stop
		*/
		case t_can_MST:
			utPrt("instruction..MST\n");
			tmcs_msp(&can_rxmsg.Data[t_can_s_type],&can_rxmsg.Data[t_can_s_motor]);
		break;

		/* MVP(move to position
		*/
		case t_can_MVP:
			utPrt("instruction..MVP\n");
			tmcs_mvp(&can_rxmsg.Data[t_can_s_type],&can_rxmsg.Data[t_can_s_motor],
				     &can_rxmsg.Data[t_can_s_byte3]);
		break;

		/* SAP(set axis parameter)
		   <!stored in SRAM and will be lost after power off
		*/
		case t_can_SAP:
			utPrt("instruction..SAP\n");
			tmcs_sap(&can_rxmsg.Data[t_can_s_type],&can_rxmsg.Data[t_can_s_motor],
				     &can_rxmsg.Data[t_can_s_byte3]);
		break;

		/* GAP(get axis parameter)
		*/
		case t_can_GAP:
			utPrt("instruction..GAP\n");
		break;

		/* STAP(store axis parameter) */
		case t_can_STAP:
			utPrt("instruction..STAP\n");
			tmcs_stap();
		break;

		/* RSAP(restore axis parameter) */
		case t_can_RSAP:
			utPrt("instruction..RSAP\n");
		break;

		/* SGP(set global parameter) */
		case t_can_SGP:
			utPrt("instruction..SGP\n");
		break;

		/* GGP(get global parameter) */
		case t_can_GGP:
			utPrt("instruction..GGP\n");
		break;

		/* STGP(store global parameter) */
		case t_can_STGP:
			utPrt("instruction..STGP\n");
			tmcs_stgp();
		break;

		/* RSGP(restore global parameter) */
		case t_can_RSGP:
			utPrt("instruction..RSGP\n");
		break;

		/* RFS(reference search) */
		case t_can_RFS:
			utPrt("instruction..RFS\n");
			tmcs_rfs(&can_rxmsg.Data[t_can_s_type],&can_rxmsg.Data[t_can_s_motor]);
		break;

		/* SIO(set output) */
		case t_can_SIO:
			utPrt("instruction..SIO\n");
			tmcs_sio(&can_rxmsg.Data[t_can_s_type],&can_rxmsg.Data[t_can_s_byte3]);
		break;

		case t_can_SIOI:
			utPrt("instruction..SIOI\n");
			tmcs_sioi(&can_rxmsg.Data[t_can_s_type], &can_rxmsg.Data[t_can_s_byte3]);
		break;

		/* GIO(get input/output) */
		case t_can_GIO:
			utPrt("instruction..GIO\n");
			tmcs_gio(&can_rxmsg.Data[t_can_s_type]);
		break;

		/* CALC(calculate) */
		case t_can_CALC:
			utPrt("instruction..CALC\n");
		break;

		/* set coordinate */
		case t_can_SCO:
			utPrt("instruction..SCO\n");
			tmcs_sco(&can_rxmsg.Data[t_can_s_type],
			&can_rxmsg.Data[t_can_s_motor],&can_rxmsg.Data[t_can_s_byte3]);
		break;

		/* GCO(get coordinate) */
		case t_can_GCO:
			utPrt("instruction..GCO\n");
			tmcs_gco(&can_rxmsg.Data[t_can_s_type], &can_rxmsg.Data[t_can_s_motor]);
		break;

		/* CCO(capture coordinate) */
		case t_can_CCO:
			utPrt("instruction..CCO\n");
		break;

		/* CLE(clear error flags) */
		case t_can_CLE:
			utPrt("instruction..CLE\n");
		break;

		/* VECT(set interrupt vector) */
		case t_can_VECT:
			utPrt("instruction..VECT\n");
		break;

		/* EI(enable interrupt) */
		case t_can_EI:
			utPrt("instruction..EI\n");
		break;

		/* DI(disable interrupt) */
		case t_can_DI:
			utPrt("instruction..DI\n");
		break;

		/* SCOS(set coordinate assist) */
		case t_can_SCOS:
			utPrt("instruction..SCOS\n");
			tmcs_scos(&can_rxmsg.Data[t_can_s_type],
			&can_rxmsg.Data[t_can_s_motor],&can_rxmsg.Data[t_can_s_byte3]);
		break;

		/* GCOS(get coordinate assist) */
		case t_can_GCOS:
			utPrt("instruction..GCOS\n");
			tmcs_gcos(&can_rxmsg.Data[t_can_s_type], &can_rxmsg.Data[t_can_s_motor]);
		break;

		/* STCO(store coordinate) */
		case t_can_STCO:
			utPrt("instruction..STCO\n");
			tmcs_stco();
		break;

	    /* STCOS(store coordinate assist */
		case t_can_STCOS:
			utPrt("instruction..STCOS\n");
			tmcs_stcos();
		break;

		default:
			utPrt("instruction..error!\n");
		break;
	}
	
	utExitCrt;

	return 0;
}



















