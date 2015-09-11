#ifndef __CAN_RECEIVE_H
#define __CAN_RECEIVE_H
#include "utGlobals.h"
#include "Can_network.h"
#include "Tmcs_Process.h"

/* cmd instruction */
enum t_can_instruction
{
	t_can_ROR = 0x01,   /* rotate right */
	t_can_ROL = 0x02,   /* rotate left  */
	t_can_MST = 0x03,   /* motor stop   */
	t_can_MVP = 0x04,   /* move to position */
	t_can_SAP = 0x05,   /* set axis parameter */
	t_can_GAP = 0x06,   /* get axis parameter */
	t_can_STAP= 0x07,   /* store axis parameter */
	t_can_RSAP= 0x08,   /* restore axis parameter */
	t_can_SGP = 0x09,   /* set global parameter */
	t_can_GGP = 0x0A,   /* get global parameter */
	t_can_STGP= 0x0B,   /* store global parameter */
	t_can_RSGP= 0x0C,   /* restore global parameter */
	t_can_RFS = 0x0D,   /* reference search */
	t_can_SIO = 0x0E,   /* set output or input */
	t_can_SIOI= 0x2E,   /* set input interrupt */
	t_can_GIO = 0x0F,   /* get input/output */
	/*-------------*/

	t_can_CALC = 0x13,  /* calculate */
	
	/*-------------*/
	t_can_SCO = 0x1E,   /* set coordinate */
	t_can_GCO = 0x1F,   /* get coordinate */
	t_can_CCO = 0x20,   /* capture coordinate */
	/*-------------*/

	/*-------------*/
	t_can_STCO = 0x9E,  /* store coordinate */
	t_can_STCOS = 0x90, /* store coordinate assist */ 	 
	/*-------------*/

	t_can_CLE = 0x24,   /* clear error flags */
	t_can_VECT = 0x25,  /* set interrupt vector */
	t_can_EI  = 0x19,   /* enable interrupt */
	t_can_DI  = 0x1A,   /* disable interrupt */

	/*-------------*/
	t_can_SCOS= 0x10,   /* set coordinate assist */
	t_can_GCOS= 0x11,   /* get coordinate assist */
};

/* type of MVP */
enum t_can_MVP_type
{
	t_can_MVP_ABS    = 0x00,   /* absolute */
	t_can_MVP_REL    = 0x01,   /* relative */
	t_can_MVP_COORD  = 0x02, /* coordinate */
};

/* type of axis parameter */
enum t_can_AXIS_type
{
	t_can_AXIS_ke          = 0x01,
	t_can_AXIS_inductance  = 0x02,
	t_can_AXIS_resistance  = 0x03,
	t_can_AXIS_ktherm      = 0x04,

	t_can_AXIS_holdi       = 0x05,
	t_can_AXIS_acci        = 0x06,
	t_can_AXIS_deci        = 0x07,
	t_can_AXIS_runi        = 0x08,

	t_can_AXIS_accspeed    = 0x09,  
	t_can_AXIS_decspeed    = 0x0A,

	t_can_AXIS_maxspeed    = 0x0B,
	t_can_AXIS_minspeed    = 0x0C,
	t_can_AXIS_fullspeed   = 0x0D,

	t_can_AXIS_stallth     = 0x0E,
	t_can_AXIS_overcurrent = 0x0F,
	t_can_AXIS_stepmode    = 0x10,
};

/* type of global parameter */
enum t_can_GLOBAL_Type
{
	t_can_GLOBAL_canid     = 0x01,
	t_can_GLOBAL_canrate   = 0x02,
};

/* type of coordinate parameter 
   60 numbers
*/
enum t_can_COORD_type
{
	t_can_COORD_N0   = 0x00,
	t_can_COORD_N1   = 0x01,	
	t_can_COORD_N2   = 0x02,
	t_can_COORD_N3   = 0x03,
	t_can_COORD_N4   = 0x04,
	t_can_COORD_N5   = 0x05,
	t_can_COORD_N6   = 0x06,
	t_can_COORD_N7   = 0x07,
	t_can_COORD_N8   = 0x08,
	t_can_COORD_N9   = 0x09,
	t_can_COORD_N10  = 0x0A,
	t_can_COORD_N11  = 0x0B,
	t_can_COORD_N12  = 0x0C,
	t_can_COORD_N13  = 0x0D,
	t_can_COORD_N14  = 0x0E,
	t_can_COORD_N15  = 0x0F,
	t_can_COORD_N16  = 0x10,
	t_can_COORD_N17  = 0x11,
	t_can_COORD_N18  = 0x12,
	t_can_COORD_N19  = 0x13,
	t_can_COORD_N20  = 0x14,
	t_can_COORD_N21  = 0x15,
	t_can_COORD_N22  = 0x16,
	t_can_COORD_N23  = 0x17,
	t_can_COORD_N24  = 0x18,
	t_can_COORD_N25  = 0x19,
	t_can_COORD_N26  = 0x1A,
	t_can_COORD_N27  = 0x1B,
	t_can_COORD_N28  = 0x1C,
	t_can_COORD_N29  = 0x1D,
	t_can_COORD_N30  = 0x1E,

	t_can_COORD_N31  = 0x1F,
	t_can_COORD_N32  = 0x20,	
	t_can_COORD_N33  = 0x21,
	t_can_COORD_N34  = 0x22,
	t_can_COORD_N35  = 0x23,
	t_can_COORD_N36  = 0x24,
	t_can_COORD_N37  = 0x25,
	t_can_COORD_N38  = 0x26,
	t_can_COORD_N39  = 0x27,
	t_can_COORD_N40  = 0x28,
	t_can_COORD_N41  = 0x29,
	t_can_COORD_N42  = 0x2A,
	t_can_COORD_N43  = 0x2B,
	t_can_COORD_N44  = 0x2C,
	t_can_COORD_N45  = 0x2D,
	t_can_COORD_N46  = 0x2E,
	t_can_COORD_N47  = 0x2F,
	t_can_COORD_N48  = 0x30,
	t_can_COORD_N49  = 0x31,
	t_can_COORD_N50  = 0x32,
	t_can_COORD_N51  = 0x33,
	t_can_COORD_N52  = 0x34,
	t_can_COORD_N53  = 0x35,
	t_can_COORD_N54  = 0x36,
	t_can_COORD_N55  = 0x37,
	t_can_COORD_N56  = 0x38,
	t_can_COORD_N57  = 0x39,
	t_can_COORD_N58  = 0x3A,
	t_can_COORD_N59  = 0x3B,
};

/* motor number */
enum t_can_motor
{
	t_can_motor0,   /* motor number 0 */
	t_can_motor1,   /* motor number 1 */
	t_can_motor2,   /* motor number 2 */
};

/* can stream */
enum t_can_stream
{
	t_can_s_instruction = 0x00,
	t_can_s_type  = 0x01,
	t_can_s_motor = 0x02,
	t_can_s_byte3 = 0x03,
	t_can_s_byte2 = 0x04,
	t_can_s_byte1 = 0x05,
	t_can_s_byte0 = 0x06,
	t_can_s_sumc  = 0x07,
};

/* Can数据流 */
struct _t_can_stream
{
	INT16U _t_can_address;
	INT8U  _t_can_instruction;
	INT8U  _t_can_type;
	INT8U  _t_can_motor;
	INT8U  _t_can_len;
	INT8U  _t_can_value[8];
	INT8U  _t_can_sumc;
};


enum t_can_state
{
	t_can_receive_true = 0x01,
	t_can_receive_false = 0x00,

	t_can_send_true = 0x01,
	t_can_send_false = 0x00,
};

/* Can状态 */
struct _t_can_state
{
	INT8U _t_can_receive;
	INT8U _t_can_send;
	INT8U _t_can_error;
};

/* Can接收缓存 */
struct _t_can_rxmsg
{
	INT32U StdId;  /*!< Specifies the standard identifier.
	                    This parameter can be a value between 0 to 0x7FF. */

	INT32U ExtId;  /*!< Specifies the extended identifier.
	                    This parameter can be a value between 0 to 0x1FFFFFFF. */

	INT8U IDE;     /*!< Specifies the type of identifier for the message that will be received.
	                    This parameter can be a value of @ref CAN_identifier_type */

	INT8U RTR;     /*!< Specifies the type of frame for the received message.
	                    This parameter can be a value of @ref CAN_remote_transmission_request */

	INT8U DLC;     /*!< Specifies the length of the frame that will be received.
	                    This parameter can be a value between 0 to 8 */

	INT8U Data[8]; /*!< Contains the data to be received. It ranges from 0 to 0xFF. */

	INT8U FMI;     /*!< Specifies the index of the filter the message stored in the mailbox passes through.
	                    This parameter can be a value between 0 to 0xFF */
};


extern struct _t_can_state can_state;


/* Function list */
INT8U can_receive_process(void);
void  tmcs_stgp(void);

#endif





















