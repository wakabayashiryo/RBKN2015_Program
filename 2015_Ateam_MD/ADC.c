#include<htc.h>
#include"ADC.h"
#define  _XTAL_FREQ 32000000

void adc_init(void)
{
    ADCON0 = 0x00;
    ADCON1 = 0xa0;//Tad = 1us(32MHz)//
    ADRESH = 0;
    ADRESL = 0;
    ADON = 1;
}
unsigned int AnalogRead(unsigned char ch)
{
    static unsigned char BefCh;
    ADCON0 = (ch<<2)|0x01;
    if(BefCh!=ch){
        __delay_us(20);
        BefCh=ch;
    }
    GO = 1;
    while(GO);
    return((ADRESH<<8)+ADRESL);
}
unsigned int Samp_AD(unsigned char select)
{
    unsigned int sum = 0;
    for(unsigned char ad = 0;ad < 64;ad++)
        {
            sum += AnalogRead(select);
        }
        return sum>>6;
}