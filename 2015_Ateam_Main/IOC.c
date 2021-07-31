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
    if(IOCIF&&IOCIE)//状態変化割り込みがかかったら
    {
        if(IOCBF5&&(!IOC5f))//phase2電磁弁フラグが降りていて、RB5の状態変化割り込みフラグがかかっていたら
            IOC5f = 1;//phase2電磁弁フラグを立てる
     
        IOCIF = 0;
        IOCBF = 0;
    }    
}

void EN_IOC(void)//状態変化割り込みを有効
{
    IOCBF = 0;
    IOCIF = 0;
    IOCIE = 1; 
}

void DI_IOC(void)//状態変化割り込みを無効
{
    IOCBF = 0;
    IOCIF = 0;
    IOCIE = 0; 
}