#ifndef __CAN_CONFIGURE_H
#define __CAN_CONFIGURE_H
#include "stm32f10x.h"


enum t_bit_rate
{
	t_bit_1Mbps   = 0x00,
	t_bit_800kbps = 0x01,
	t_bit_500kbps = 0x02,
	t_bit_400kbps = 0x03,
	t_bit_250kbps = 0x04,
	t_bit_200kbps = 0x05,
	t_bit_125kbps = 0x06,
	t_bit_100kbps = 0x07,
	t_bit_50kbps  = 0x08,
	t_bit_20kbps  = 0x09,
	t_bit_10kbps  = 0x0A,
	t_bit_5kbps   = 0x0B,
};

struct _t_can_param
{
	u8  _t_std_id;       /* CAN Ö¡ID */
	u16 _t_bit_rate;     /* CAN ²¨ÌØÂÊ */
};

extern struct _t_can_param can_parameter;

#endif




















