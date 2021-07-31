#include<htc.h>

void PWM_Set_Up(void)
{
    APFCON  = 0x51;
    
    CCP1CON = 0b00001100;//All CCP is PWMmode.
    CCP2CON = 0b00001100;
    CCP3CON = 0b00001100;
    //CCP4CON = 0b00001100;
    //CCP5CON = 0b00001100;
    
    CCPTMRS0= 0b00100100;//CCP1 use Timer2,CCP2 use Timer4,CCP3 use Timer6.
    CCPTMRS1= 0x00000000;//OtherCCP use Timer2
    PR2     = 0xFF;
    PR4     = 0xFF;
    PR6     = 0xFF;
    T2CON   = 0b00000101;//prescaler is 1:4.Frequency is 7.8kHz
    T4CON   = 0b00000101;//prescaler is 1:4.Frequency is 7.8kHz
    T6CON   = 0b00000101;//prescaler is 1:4.Frequency is 7.8kHz
}

void SetDuty(unsigned int duty_ratio,char basyo)
{

    switch(basyo)
    {
        case 1:
        CCPR1L = duty_ratio >> 2;
        DC1B0 = duty_ratio & 0b00000001;
        DC1B1 = (duty_ratio & 0b00000010) >> 1;
        break;
        case 2:
        CCPR2L = duty_ratio >> 2;
        DC2B0 = duty_ratio & 0b00000001;
        DC2B1 = (duty_ratio & 0b00000010) >> 1;
        break;
        case 3:
        CCPR3L = duty_ratio >> 2;
        DC3B0 = duty_ratio & 0b00000001;
        DC3B1 = (duty_ratio & 0b00000010) >> 1;
        break;
        case 4:
        CCPR4L = duty_ratio >> 2;
        DC4B0 = duty_ratio & 0b00000001;
        DC4B1 = (duty_ratio & 0b00000010) >> 1;
        break;
        case 5:
        CCPR5L = duty_ratio >> 2;
        DC5B0 = duty_ratio & 0b00000001;
        DC5B1 = (duty_ratio & 0b00000010) >> 1;
        break;
    }
}

