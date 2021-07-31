#include<htc.h>
#include"Tmr1.h"

void Tmr1_Init(void)
{
	T1CON = 0x31;//PRESCALER IS 1:8  tmr's count = 1us 64536
	T1GCON = 0x00;
	SetTimer(TmrValu);
	TMR1IE = 1;
	TMR1IF = 0;
	PEIE = 1;
	GIE = 1;
}

void Tmr1_Inter(void)
{
	if(TMR1IF)	
	{	
		TMR1IF = 0;
		SetTimer(TmrValu);
		Turn_Count++;
        SW_SampRate++;
    }
}
void SetTimer(unsigned int valu)
{
	TMR1ON  = 0;
	TMR1H = valu >> 8;
	TMR1L = valu & 0x00ff;
	TMR1ON = 1;
}
unsigned char Tmr1_Wait_ms(unsigned int wait)//only use fllow control. 
{
    static unsigned int waitcount = 0;//waitcount is set zero by "Tmr1_Wait_ms(0)";
    static unsigned char startflag = 1;
    unsigned char endflag = 0;
    
    if(!startflag)
    {
        waitcount += Turn_Count;
    }
    
	Turn_Count = 0;
    startflag = 0;
    
	if(waitcount>wait)
    {
        waitcount = 0;
        endflag = 1;
        startflag = 1;
    }
    
	return endflag;
}