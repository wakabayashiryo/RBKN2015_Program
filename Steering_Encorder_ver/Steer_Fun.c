#include<htc.h>
#include"Steer_Fun.h"
#include"Device_select.h"
#define _XTAL_FREQ 32000000

void pic_init(void)
{
    OSCCON = 0xf0;  //internal clock 32MHz//
    TRISA = 0x00;   //input mode RA0//
    TRISB = 0x00;
    TRISC = 0b00011000;
    TRISE = 0x00;

    ANSELA = 0x00;
    ANSELB = 0x00;

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTE = 0x00;
    
    WDTCON = 0x11;
}

void Tr_Reset(void)
{
  TrPort = 0;
  __delay_ms(10);
  TrPort = 1;
}
unsigned char Detect_Dire(unsigned char select)
{
    unsigned char dire;
    if(select)
    {
#ifdef Direction1
	    dire = 2;
#endif
#ifdef Direction2
	    dire = 1;
#endif
    }
    else
    {
#ifdef Direction1
        dire = 1;
#endif	  
#ifdef Direction2
        dire = 2;
#endif   
    }
        return dire;
}
int Calu_Devi(unsigned char ZeroFlag,unsigned int M,int SET)
{
    int devi;
    if(ZeroFlag)
        devi = 1024 - SET - M;
    else
        devi = SET - M;
    
    return devi;
}
