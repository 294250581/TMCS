//==============================================================================
//头文件
#ifndef _utFIFO_H
#define _utFIFO_H

//==============================================================================
//包含全局定义变量文件
#include "utGlobals.h"

#define FIFO_LEN_TYPE INT16U

typedef struct
{
	FIFO_LEN_TYPE pHead;
	FIFO_LEN_TYPE pTail;
	FIFO_LEN_TYPE cnt;
	FIFO_LEN_TYPE maxLen;
}UT_FIFO;

#define FIFO_NOT_FULL(pFifo) 	(((pFifo)->cnt)!=((pFifo)->maxLen))
#define FIFO_NOT_EMPTY(pFifo) 	(((pFifo)->cnt)!=0)
#define FIFO_EMPTY(pFifo) 		(((pFifo)->cnt)==0)

void utInitFIFO(UT_FIFO *pFifo,FIFO_LEN_TYPE maxLen);		//初始化长度及清空FIFO
void utClrFIFO(UT_FIFO *pFifo);							//清空FIFO
FIFO_LEN_TYPE utIndWrtFIFO(UT_FIFO *pFifo);				//得到可以写的索引号
FIFO_LEN_TYPE utIndRdFIFO(UT_FIFO *pFifo);				//得到可以读的序列号
FIFO_LEN_TYPE utIndRdNFIFO(UT_FIFO *pFifo, FIFO_LEN_TYPE n);	//预读

//得到最老数据的索引号
#define utIndOdstFIFO(pFifo)	((pFifo)->pTail)


//==============================================================================
//外部符号预定义

//==============================================================================
//外部调用变量

//==============================================================================
//使用的外部变量

//==============================================================================
//外部调用函数

#endif
//==============================================================================
//end of the file

