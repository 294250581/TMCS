//==============================================================================
//ͷ�ļ�
#ifndef _utFIFO_H
#define _utFIFO_H

//==============================================================================
//����ȫ�ֶ�������ļ�
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

void utInitFIFO(UT_FIFO *pFifo,FIFO_LEN_TYPE maxLen);		//��ʼ�����ȼ����FIFO
void utClrFIFO(UT_FIFO *pFifo);							//���FIFO
FIFO_LEN_TYPE utIndWrtFIFO(UT_FIFO *pFifo);				//�õ�����д��������
FIFO_LEN_TYPE utIndRdFIFO(UT_FIFO *pFifo);				//�õ����Զ������к�
FIFO_LEN_TYPE utIndRdNFIFO(UT_FIFO *pFifo, FIFO_LEN_TYPE n);	//Ԥ��

//�õ��������ݵ�������
#define utIndOdstFIFO(pFifo)	((pFifo)->pTail)


//==============================================================================
//�ⲿ����Ԥ����

//==============================================================================
//�ⲿ���ñ���

//==============================================================================
//ʹ�õ��ⲿ����

//==============================================================================
//�ⲿ���ú���

#endif
//==============================================================================
//end of the file

