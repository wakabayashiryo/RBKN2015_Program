#ifndef TMR1_H
#define	TMR1_H

#define TmrValu  64536 //interrupt's time 1ms
unsigned int Turn_Count;
extern unsigned int SW_SampRate;
extern unsigned int CapaKeep;

void Tmr1_Inter(void);
void Tmr1_Init(void);
void SetTimer(unsigned int valu);
unsigned char Tmr1_Wait_ms(unsigned int wait);//only use fllow control.

#endif	/* TMR1_H */