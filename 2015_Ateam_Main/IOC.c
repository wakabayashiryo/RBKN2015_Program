#include <htc.h>
#include "System.h"
#include "IOC.h"

void IOC_Init(unsigned char UpEdge,unsigned char DownEdge)
{
    IOCBP = UpEdge;
    IOCBN = DownEdge;
    IOCBF = 0;
    
    IOCIE = 1;
    IOCIF = 0;
    GIE = 1;
}

void Inter_IOC(void)
{
    if(IOCIF&&IOCIE)//��ԕω����荞�݂�����������
    {
        if(IOCBF5&&(!IOC5f))//phase2�d���كt���O���~��Ă��āARB5�̏�ԕω����荞�݃t���O���������Ă�����
            IOC5f = 1;//phase2�d���كt���O�𗧂Ă�
     
        IOCIF = 0;
        IOCBF = 0;
    }    
}

void EN_IOC(void)//��ԕω����荞�݂�L��
{
    IOCBF = 0;
    IOCIF = 0;
    IOCIE = 1; 
}

void DI_IOC(void)//��ԕω����荞�݂𖳌�
{
    IOCBF = 0;
    IOCIF = 0;
    IOCIE = 0; 
}