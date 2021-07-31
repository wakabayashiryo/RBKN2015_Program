/* 
 * Author: oishi * File:   main.c

 *
 * Created on 2015/07/30, 14:48
 */
#include <htc.h>
#include"PWM.h"
#include"I2C.h"
#include"Data_Format.h"

//#define IndicaterTest
#define _XTAL_FREQ 32000000
__CONFIG(FOSC_INTOSC&WDTE_OFF&PWRTE_ON&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_ON&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_ON&STVREN_OFF&LVP_OFF);
__IDLOC(1056);

void pic_init(void);
void TestIndicater(void);
void Clear(void);

void main(void)
{
    unsigned char stat = 0,color,brightness,LedStat;
    unsigned int EmergencyCount = 0;
    pic_init();
    I2C_init(Adrs);
    PWM_Set_Up();
    SetDuty(0,1);
    SetDuty(0,2);
    SetDuty(0,3);
    
    while(1)
    {
#ifdef IndicaterTest
      TestIndicater();  
#else
      stat = I2C_ReceiveCheck();
      if(stat==3)
        {
            EmergencyCount = 0;
            
            color = rcv_data[0];
            brightness = rcv_data[1];
            LedStat = rcv_data[2];
        }
      else
        {
            if(EmergencyCount<60000)
                EmergencyCount++;
        }
      if(EmergencyCount > 50000)
        { 
            Red1 = 1;
            Red2 = 1;
            Red3 = 1; 
            Red4 = 1;
            SetDuty(1023,Pwm_Red);
            __delay_ms(250);
            Clear();
            __delay_ms(250);
        }
      else 
        {
          switch(color)
            { 
                case All_Clear:
                    Clear();
                break;
    /////////////////////////////////////////////////////////
                case Blue_Num1:
                    if(LedStat)
                        Blue1 = 1;
                    else
                        Blue1 = 0;
                    SetDuty(brightness<<2,Pwm_Blue);
                break;
                case Green_Num1:
                    if(LedStat)
                        Green1 = 1;
                    else
                        Green1 = 0;
                    SetDuty(brightness<<2,Pwm_Green);
                break;
                case Red_Num1:
                    if(LedStat)
                        Red1 = 1;
                    else
                        Red1 = 0;
                    SetDuty(brightness<<2,Pwm_Red);
                break;
    /////////////////////////////////////////////////////////
                case Blue_Num2:
                    if(LedStat)
                        Blue2 = 1;
                    else
                        Blue2 = 0;
                    SetDuty(brightness<<2,Pwm_Blue);
                break;
                case Green_Num2:
                    if(LedStat)
                        Green2 = 1;
                    else
                        Green2 = 0;
                    SetDuty(brightness<<2,Pwm_Green);
                break;
                case Red_Num2:
                    if(LedStat)
                        Red2 = 1;
                    else
                        Red2 = 0;
                    SetDuty(brightness<<2,Pwm_Red);
                break;
    /////////////////////////////////////////////////////////
                case Blue_Num3:
                    if(LedStat)
                        Blue3 = 1;
                    else
                        Blue3 = 0;
                    SetDuty(brightness<<2,Pwm_Blue);
                break;
                case Green_Num3:
                    if(LedStat)
                        Green3 = 1;
                    else
                        Green3 = 0;
                    SetDuty(brightness<<2,Pwm_Green);
                break;
                case Red_Num3:
                    if(LedStat)
                        Red3 = 1;
                    else
                        Red3 = 0;
                    SetDuty(brightness<<2,Pwm_Red);
                break;
    /////////////////////////////////////////////////////////
                case Blue_Num4:
                    if(LedStat)
                        Blue4 = 1;
                    else
                        Blue4 = 0;
                    SetDuty(brightness<<2,Pwm_Blue);
                break;
                case Green_Num4:
                    if(LedStat)
                        Green4 = 1;
                    else
                        Green4 = 0;
                    SetDuty(brightness<<2,Pwm_Green);
                break;
                case Red_Num4:
                    if(LedStat)
                        Red4 = 1;
                    else
                        Red4 = 0;
                    SetDuty(brightness<<2,Pwm_Red);
                break;
            }
        }
#endif
   }
}
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
}
void interrupt common_inter(void)
{
   Slave_Interrupt();
}
void TestIndicater(void)
{
    static unsigned char select = 1;
    unsigned int count,led;
    
    if(select==5)
        select=1;
    switch(select)
    {
        case 1:
            Blue1 = 1;
            Green1 = 1;
            Red1 = 1;
            Blue4 = 0;
            Green4 = 0;
            Red4 = 0;
        break;
        case 2:
            Blue1 = 0;
            Green1 = 0;
            Red1 = 0;
            Blue2 = 1;
            Green2 = 1;
            Red2 = 1;
        break;  
        case 3:
            Blue2 = 0;
            Green2 = 0;
            Red2 = 0;
            Blue3 = 1;
            Green3 = 1;
            Red3 = 1;
        break;
        case 4:
            Blue3 = 0;
            Green3 = 0;
            Red3 = 0;
            Blue4 = 1;
            Green4 = 1;
            Red4 = 1;
        break;
    }
    select++;
    for(led = 1;led < 4;led++)
    {
        for(count = 0;count<1023;count++)
        {
            SetDuty(count,led);
             __delay_ms(1);
        }
        for(count = 1023;count>0;count--)
        {
            SetDuty(count,led);
             __delay_ms(1);
        }
    }
}
void Clear(void)
{
    PORTA = 0x00;
    LATB0 = 0;
    LATB1 = 0;
    LATB2 = 0;
    LATC0 = 0;
    SetDuty(0,1);
    SetDuty(0,2);
    SetDuty(0,3);
}