/* 
 * File:   Interrupt.c
 * Author: evaota
 *
 * Created on 2015/07/13, 18:19
 */
#include<htc.h>
#include"Tmr0.h"

void Tmr0_Init(void)
{
    PS0 = 1;//prescaler 256:1
    PS1 = 1;
    PS2 = 1;
    TMR0CS  = 0;//internal clock 
    TMR0 = 0;//256 overfllow
    
	T0IE = 1;
    T0IF = 0;
    GIE = 1;
}
void Inter_Count(void)
{
    if(T0IF)//warikomi 8ms
    {
        Wait_count++;
		Side_count++;
        T0IF = 0;
        TMR0 = 0;
    }
}

