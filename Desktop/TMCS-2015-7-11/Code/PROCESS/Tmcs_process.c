#include "Tmcs_process.h"
#include "Flash.h"

#define Debug 1

struct _t_tmcs_motor t_tmcs_motor0;
struct _t_tmcs_motor t_tmcs_motor1;
struct _t_tmcs_motor t_tmcs_motor2;

/* ROR (电机左转指令) */
void tmcs_ror(INT8U *motor, INT8U *value)
{
	INT32U speed;
	speed = (*value << 24)    | (*(value+1) << 16) | 
		    (*(value+2) << 8) | (*(value+3));

	if(*motor <= 2)
	{
		utPrt("motor is legal\n");
		switch(*motor)
		{
			case motor_0:
				utPrt("motor 0\n");
				tmcs_bsp_ror(dSPIN_Device_X, speed);
			break;

			case motor_1:
				utPrt("motor 1\n");
				tmcs_bsp_ror(dSPIN_Device_Y, speed);
			break;

			case motor_2:
				utPrt("motor 2\n");
				tmcs_bsp_ror(dSPIN_Device_Z, speed);
			break;
		}
	}
	else
	{
		utPrt("motor is illegal\n");
	}
}

/* ROL (电机右转指令) */
void tmcs_rol(INT8U *motor, INT8U *value)
{
	INT32U speed;
	speed = (*value << 24)    | (*(value+1) << 16) | 
		    (*(value+2) << 8) | (*(value+3));

	if(*motor <= 2)
	{
		utPrt("motor is legal\n");
		switch(*motor)
		{
			case motor_0:
				utPrt("motor 0\n");
				tmcs_bsp_rol(dSPIN_Device_X, speed);
			break;

			case motor_1:
				utPrt("motor 1\n");
				tmcs_bsp_rol(dSPIN_Device_Y, speed);
			break;

			case motor_2:
				utPrt("motor 2\n");
				tmcs_bsp_rol(dSPIN_Device_Z, speed);
			break;
		}
	}
	else
	{
		utPrt("motor is illegal\n");
	}
}

void tmcs_msp(INT8U *type, INT8U *motor)
{
	if((*type <= 3) && (*motor <= 2))
	{
		utPrt("type and motor is legal\n");
		switch(*motor)
		{
			case motor_0:
				utPrt("motor 0\n");
				tmcs_bsp_msp(dSPIN_Device_X, *type);
			break;

			case motor_1:
				utPrt("motor 1\n");
				tmcs_bsp_msp(dSPIN_Device_Y, *type);
			break;

			case motor_2:
				utPrt("motor 2\n");
				tmcs_bsp_msp(dSPIN_Device_Z, *type);
			break;
		}
	}
	else
	{
		utPrt("type and motor is illegal\n");
	}
}

void tmcs_mvp(INT8U *type, INT8U *motor, INT8U *value)
{
	INT32S parameter;

	parameter = (*value << 24)    | (*(value+1) << 16) | 
		        (*(value+2) << 8) | (*(value+3));

	sprt("parameter = %d\n", parameter);

	if( (*type <= 3) && (*motor <= 2) )
	{
		utPrt("type and motor is legal\n");

		switch(*type)
		{
			case ABS:
				utPrt("type is ABS\n");
				tmcs_bsp_abs(parameter, (dSPIN_Device_TypeDef)*motor);
			break;

			case REL:
				utPrt("type is REL\n");
				tmcs_bsp_rel(parameter, (dSPIN_Device_TypeDef)*motor);
			break;

			case COORD:
				utPrt("type is COORD\n");
			break;
		}
	}

	else
	{
		utPrt("type and motor is illegal\n");
	}
}

/* store axis parameter */
void tmcs_sap(INT8U *type, INT8U *motor, INT8U *value)
{
	INT32U parameter;

	parameter = (*value << 24)    | (*(value+1) << 16) | 
		        (*(value+2) << 8) | (*(value+3));
	
	if((*type <= 0x0F) && (*motor <= 2))
	{
		utPrt("type and motor is legal\n");
		switch(*motor)
		{
			case motor_0:
				utPrt("motor 0\n");
				tmcs_bsp_sap(dSPIN_Device_X, *type, parameter);
			break;

			case motor_1:
				utPrt("motor 1\n");
				tmcs_bsp_sap(dSPIN_Device_Y, *type, parameter);
			break;

			case motor_2:
				utPrt("motor 2\n");
				tmcs_bsp_sap(dSPIN_Device_Z, *type, parameter);
			break;
		}
	}
	else
	{
		utPrt("type and motor is illegal\n");
	}
}

/* store axis parameter(保存到Flash) */
void tmcs_stap(void)
{
	utPrt("save axis parameter\n");
	save_value();
}

void tmcs_stgp(void)
{
	save_value();
}

/* set coordinate (绝对位置) 
   只设置,并不保存
*/
void tmcs_sco(INT8U *type, INT8U *motor, INT8U *value)
{
	INT32S abs_pos;

#if Debug	
	utPrt("SCO type..%d\n",   *type);
	utPrt("SCO motor..%d\n",  *motor);
	utPrt("SCO value0..%d\n", *value);
	utPrt("SCO value1..%d\n", *(value+1));
	utPrt("SCO value2..%d\n", *(value+2));
	utPrt("SCO value3..%d\n", *(value+3));
	utPrt("\n");
#endif

	abs_pos = (*value << 24)    | (*(value+1) << 16) | 
		      (*(value+2) << 8) | (*(value+3));

	/* type和motor参数是否超限 */
	if((*type  <= 59) && (*motor <= 2))
	{
		utPrt("type and motor is legal\n");
		switch(*motor)
		{
			case motor_0:
				utPrt("motor 0\n");
				t_tmcs_motor0.t_tmcs_COORD.t_tmcs_ABS[*type] = abs_pos;
			break;

			case motor_1:
				utPrt("motor 1\n");
				t_tmcs_motor1.t_tmcs_COORD.t_tmcs_ABS[*type] = abs_pos;
			break;

			case motor_2:
				utPrt("motor 2\n");
				t_tmcs_motor2.t_tmcs_COORD.t_tmcs_ABS[*type] = abs_pos;
			break;
		}
	}
	else
	{
		utPrt("type and motor is illegal\n");
	}
}

/* get coordinate (绝对位置) */
void tmcs_gco(INT8U *type, INT8U *motor)
{
	INT32S abs_pos;
	char buff[50];
	
	utPrt("GCO type..%d\n",   *type);
	utPrt("GCO motor..%d\n",  *motor);
	utPrt("\n");

	if((*type <= 59) && (*motor <= 2))
	{
		utPrt("type and motor is legal\n");
		switch(*motor)
		{
			case motor_0:
				utPrt("motor 0\n");
				abs_pos = t_tmcs_motor0.t_tmcs_COORD.t_tmcs_ABS[*type];
			break;

			case motor_1:
				utPrt("motor 1\n");
				abs_pos = t_tmcs_motor1.t_tmcs_COORD.t_tmcs_ABS[*type];
			break;

			case motor_2:
				utPrt("motor 2\n");
				abs_pos = t_tmcs_motor2.t_tmcs_COORD.t_tmcs_ABS[*type];
			break;
		}
	}
	else
	{
		utPrt("type and motor is illegal\n");
	}

	sprintf(buff, "abs_pos = %d\n", abs_pos);
	utPtn(buff);
}

/* set coordinate assist (编码位置) */
void tmcs_scos(INT8U *type, INT8U *motor, INT8U *value)
{
	INT32S enc_pos;

#if Debug
	utPrt("SCOS type..%d\n",   *type);
	utPrt("SCOS motor..%d\n",  *motor);
	utPrt("SCOS value0..%d\n", *value);
	utPrt("SCOS value1..%d\n", *(value+1));
	utPrt("SCOS value2..%d\n", *(value+2));
	utPrt("SCOS value3..%d\n", *(value+3));
	utPrt("\n");
#endif

	enc_pos = (*value << 24)    | (*(value+1) << 16) | 
		      (*(value+2) << 8) | (*(value+3));

	/* type和motor参数是否超限 */
	if((*type  <= 59) && (*motor <= 2))
	{
		utPrt("type and motor is legal\n");
		switch(*motor)
		{
			case motor_0:
				utPrt("motor 0\n");
				t_tmcs_motor0.t_tmcs_COORD.t_tmcs_ENC[*type] = enc_pos;
			break;

			case motor_1:
				utPrt("motor 1\n");
				t_tmcs_motor1.t_tmcs_COORD.t_tmcs_ENC[*type] = enc_pos;
			break;

			case motor_2:
				utPrt("motor 2\n");
				t_tmcs_motor2.t_tmcs_COORD.t_tmcs_ENC[*type] = enc_pos;
			break;
		}
	}
	else
	{
		utPrt("type and motor is illegal\n");
	}
}

/* get coordinate assist (编码位置) */
void tmcs_gcos(INT8U *type, INT8U *motor)
{
	INT32S enc_pos;
	char buff[50];
	
	utPrt("GCOS type..%d\n",   *type);
	utPrt("GCOS motor..%d\n",  *motor);
	utPrt("\n");

	if((*type <= 59) && (*motor <= 2))
	{
		utPrt("type and motor is legal\n");
		switch(*motor)
		{
			case motor_0:
				utPrt("motor 0\n");
				enc_pos = t_tmcs_motor0.t_tmcs_COORD.t_tmcs_ENC[*type];
			break;

			case motor_1:
				utPrt("motor 1\n");
				enc_pos = t_tmcs_motor1.t_tmcs_COORD.t_tmcs_ENC[*type];
			break;

			case motor_2:
				utPrt("motor 2\n");
				enc_pos = t_tmcs_motor2.t_tmcs_COORD.t_tmcs_ENC[*type];
			break;
		}
	}
	else
	{
		utPrt("type and motor is illegal\n");
	}

	sprintf(buff, "abs_pos = %d\n", enc_pos);
	utPtn(buff);
}

/* store coordinate */
void tmcs_stco(void)
{
	/* 保持数据 */
	utPrt("save coordinate\n");
	save_value();
}

/* store coordinate assist */
void tmcs_stcos(void)
{
	utPrt("save coordinate assist\n");
	save_value();
}

void tmcs_sio(INT8U *type, INT8U *value)
{
	INT8U sub  = *(value+2);
	INT8U mode = *(value+3);
	/* set BUS A */
	if(*type == 0x00)
	{
		utPrt("set BUS A\n");
		/* output */
		if(mode == 0)
		{
			utPrt("output\n");
			iobus_set_mode(BUS_A, IOBUS_OUTPUT);
		}
		/* input */
		else
		{
			utPrt("input\n");
			iobus_set_mode(BUS_A, IOBUS_INPUT);
		}
	}
	/* set BUS B */
	if(*type == 0x01)
	{
		utPrt("set BUS B\n");
		/* output */
		if(mode == 0)
		{
			utPrt("output\n");
			iobus_set_mode(BUS_B, IOBUS_OUTPUT);
		}
		/* input */
		else
		{
			utPrt("input\n");
			iobus_set_mode(BUS_B, IOBUS_INPUT);
		}
	}

	/* set BUS A output value */
	if(*type == 0x02)
	{
		iobus_set_output(BUS_A,(IOSUB_Typedef)sub, (OUTPUT_Typedef)mode);
	}
	if(*type == 0x03)
	{
		iobus_set_output(BUS_B,(IOSUB_Typedef)sub, (OUTPUT_Typedef)mode);
	}
}

void tmcs_gio(INT8U *value)
{
	INT16U num = (*value << 24)    | (*(value+1) << 16) | 
		         (*(value+2) << 8) | (*(value+3));
	INT8U  IOValue;
	utPrt("num .. %d\n", num);

	if(num <= 3)
		IOValue = iobus_get_ioinput(BUS_A, (IOSUB_Typedef)num);
	if(num >=4 && num <= 7)
		IOValue = iobus_get_ioinput(BUS_B, (IOSUB_Typedef)(num-4));

	utPrt("IOValue..%d\n", IOValue);
}

void tmcs_sioi(INT8U *type, INT8U *value)
{
	INT8U  ints = *(value+2);
	INT8U  mode = *(value+3);
	INT_Typedef ioint;
	INTMODE_Typedef intmode;

	if(ints == 0)
		ioint = INT_OFF;
	else
		ioint = INT_ON;

	if(mode == 0)
		intmode = INT_RISING;
	if(mode == 1)
		intmode = INT_FALLING;
	if(mode == 2)
		intmode = INT_RANDF;
	
	utPrt("ints .. %d\n", ints);
	utPrt("intmode .. %d\n", intmode);

	if(*type == 0)
	{
		iobus_set_nvic(BUS_A, SUB_A0, (INT_Typedef)ioint, (INTMODE_Typedef)intmode);
		
		iobus_set_tcbn(&IOBUSA_TCB[SUB_A0], TCB_IO_INT, ioint);
		iobus_set_tcbn(&IOBUSA_TCB[SUB_A0], TCB_IO_INTMODE, intmode);
	}
	if(*type >= 4 && *type <=6)
	{
		iobus_set_nvic(BUS_B, (IOSUB_Typedef)(*type-4), (INT_Typedef)ioint, (INTMODE_Typedef)intmode);

		iobus_set_tcbn(&IOBUSB_TCB[*type - 4], TCB_IO_INT, ioint);
		iobus_set_tcbn(&IOBUSB_TCB[*type - 4], TCB_IO_INTMODE, intmode);
	}
}

/* 归零 */
void tmcs_rfs(INT8U *type, INT8U *motor)
{
	if(*type <= 1 && *motor <= 2)
	{
		utPrt("type and motor is legal\n");
		if(*type == rfs_normal)
		{
			utPrt("rfs normale\n");
			tmcs_rfsn_ctr = *motor+1;
		}
		if(*type == rfs_confirm)
		{
			utPrt("rfs confirm\n");
			tmcs_rfsc_ctr = *motor+1;
		}
	}
	else
	{
		utPrt("type and motor is illegal\n");
	}
}

