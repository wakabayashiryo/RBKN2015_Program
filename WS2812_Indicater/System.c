#include<xc.h>
#include"System.h"
#include"Colors.h"

#define _XTAL_FREQ 32000000

void pic_init(void)
{
 
	OSCCON = 0x70;  //8MHz x 4
    APFCON = 0x80;
    TRISA = 0x20; 
    ANSELA = 0x00;

    PORTA = 0x00;
	  
}
void Dimming(unsigned char Valu)
{
    unsigned char i;
    for(i= 0;i<LedNum;i++)
    {
        gLed[i].blue /= Valu;
        gLed[i].red /= Valu;
        gLed[i].green /= Valu;
    }
}
void All_Send(LedData data,unsigned char dimming)
{
    unsigned char count;
    for(count=0;count<LedNum;count++)
    {
        gLed[count] = data;
    }
    Dimming(dimming);
    SendLed();
}
void All_Clear(void)
{
    for(unsigned char clear=0;clear<LedNum;clear++)
        gLed[clear] = Clear;
}
void test(void)
{
     unsigned char test,count,phase;
     LedData put = {0};
     for(phase = 0;phase < 14;phase++)
     {
	  for(test = 0;test<255;test++)
	  {
	       if(phase==0)
		    put.green=test;
	       else if(phase==1)
		    put.red = test;
	       else if(phase==2)
		    put.green = 255-test;
	       else if(phase==3)
		    put.blue = test;
	       else if(phase==4)
		    put.red = 255-test;
	       else if(phase==5)
		    put.green = test;
	       else if(phase==6)
		    put.red = test; 
	       else if(phase==7)
		    put.red=255-test;
	       else if(phase==8)
		    put.green = 255-test;
	       else if(phase==9)
		    put.red = test;
	       else if(phase==10)
		    put.blue = 255-test;
	       else if(phase==11)
		    put.green = test;
	       else if(phase==12)
		    put.red = 255-test;
	       else if(phase==13)
		    put.green = 255-test;
	       All_Send(put,1);
	       __delay_ms(50);
	  }
     }
}
/***************************************
 * SendLed
 * LED10個で310uS程掛かる
 ***************************************/
void SendLed(void)
{
	//go
	di();
#asm
	banksel	_byteCount
	movf	FSR0L, W
	movwf	_saveFsr
	movf	FSR0H, W
	movwf	_saveFsr+1		;save FSR0

	movlw	30				;LED 10個 x 3byte
	movwf	_byteCount
	movlw	_gLed			;先頭アドレス
	movwf	FSR0L
	clrf	FSR0H
	clrf	_bitCount
	goto	ByteLoop

Adjust:
	nop						; 1
	rlf		_dt, F			; 1
	goto	BitLoop			; 2

ByteLoop:
	moviw	FSR0++			; 1
	movwf	_dt				; 1
	bsf		_bitCount, 3	; 1  set 8
BitLoop:
	bsf		pLedDi			; 1
	btfsc	_dt, 7			; 1
	goto	Send1			; 2 or 1
	bcf		pLedDi			; 1			375nS
	decfsz	_bitCount, F	; 1
	goto	Adjust			; 2
	decfsz	_byteCount, F	; 1
	goto	ByteLoop		; 2
	goto	SendEnd
Send1:
	rlf		_dt, F			; 1
	nop						; 1
	nop						; 1
	bcf		pLedDi			; 1			875nS
	decfsz	_bitCount, F	; 1
	goto	BitLoop			; 2
	decfsz	_byteCount, F	; 1
	goto	ByteLoop		; 2
SendEnd:
	movf	_saveFsr, W
	movwf	FSR0L
	movf	_saveFsr+1, W
	movwf	FSR0H
#endasm
	ei();
}
