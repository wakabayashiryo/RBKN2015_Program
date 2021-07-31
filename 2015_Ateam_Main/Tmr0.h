/* 
 * File:   Interrupt.h
 * Author: evaota
 *
 * Created on 2015/07/13, 18:19
 */

#ifndef TMR0_H
#define	TMR0_H

extern unsigned int Wait_count;
extern unsigned int Side_count;

void Tmr0_Init(void);
void Inter_Count(void);

#endif	/* INTERRUPT_H */

