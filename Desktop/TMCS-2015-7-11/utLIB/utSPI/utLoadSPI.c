//==============================================================================
//C文件
//与硬件无关的常用库函数
#include "utSPI.h"

//==============================================================================
//内部符号预定义

//==============================================================================
//内部调用变量

//==============================================================================
//内部调用函数定义

//==============================================================================
//外部调用函数

//==============================================================================
//外部调用函数


//##############################################################################
//模块
//##############################################################################
//------------------------------------------------------------------------------
INT8U utLoadSPI(INT8U dat)		//本函数既写又读，MOSI，MISO各一根线
{
	INT8U rdat = 0;
	INT8U i;

	//开始之前CLK = 0
	for (i = 0; i < 8; i++)
	{
		//第一次出来是利用的CS的下降沿，第二次是用上一个字节最后一个CLK的下降沿
		rdat <<= 1; //把位空出来
		if (utIsSPIIn())
			rdat |= BIT0;

		//送出,先把数据放好，再来上升沿
		if (dat &BIT7)
			utSPIOut(1);
		else
			utSPIOut(0);

		utSPIClk(1);  	//上升沿写数	
		utSPIClk(0); 	//下一次
		dat <<= 1; 		//把下一次要写的数准备好
	}

	return rdat;
}
//##############################################################################

//==============================================================================
//内部调用函数

//==============================================================================
//end of the file


//------------------------------------------------------------------------------  




