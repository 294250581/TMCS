//==============================================================================
//C�ļ�
//��Ӳ���޹صĳ��ÿ⺯��
#include "utFIFO.h"

//==============================================================================
//�ڲ�����Ԥ����

//==============================================================================
//�ڲ����ñ���

//==============================================================================
//�ڲ����ú�������

//==============================================================================
//�ⲿ���ú���

//==============================================================================
//�ⲿ���ú���


//##############################################################################
//ģ��
//##############################################################################
//------------------------------------------------------------------------------

//��ʼ�����ȼ����FIFO
void utInitFIFO(UT_FIFO *pFifo,FIFO_LEN_TYPE maxLen)
{
	(pFifo->maxLen) = maxLen;
	utClrFIFO (pFifo);
}

//���FIFO
void utClrFIFO(UT_FIFO *pFifo)
{
	(pFifo->pHead) = 0;
	(pFifo->pTail) = 0;
	(pFifo->cnt) = 0;
}

//�õ�����д��������
FIFO_LEN_TYPE utIndWrtFIFO(UT_FIFO *pFifo)
{
	FIFO_LEN_TYPE ind;

	ind = (pFifo->pHead);
									
	if(++(pFifo->pHead)==(pFifo->maxLen))
		(pFifo->pHead)=0;
	
	(pFifo->cnt)++;

	return ind;	
}

//�õ����Զ������к�
FIFO_LEN_TYPE utIndRdFIFO(UT_FIFO *pFifo)
{
	FIFO_LEN_TYPE ind;
	
	ind = (pFifo->pTail);
	
	if(++(pFifo->pTail)==(pFifo->maxLen))
		(pFifo->pTail)=0;

	(pFifo->cnt)--;
	
	return ind;
}

//Ԥ��ѯ��N��λ���ϵ�Ԫ��
FIFO_LEN_TYPE utIndRdNFIFO(UT_FIFO *pFifo, FIFO_LEN_TYPE n)
{
	FIFO_LEN_TYPE ind;

	ind = (pFifo->pTail);
	
	if(n>=pFifo->cnt)						//λ�ò��ԣ�����0
		return 0;
	else if((ind + n) >= (pFifo->maxLen))	//����
		return ((ind + n)-(pFifo->maxLen));
	else 									//
		return (ind + n);	
}
//##############################################################################

//==============================================================================
//�ڲ����ú���

//==============================================================================
//end of the file


//------------------------------------------------------------------------------  



