/* 
 * File:   UART_Driver.h
 * Author: Oishi
 *
 * Created on 2015/02/22, 19:27
 */

#ifndef UART_H
#define	UART_H

int Scanf(char *m,char *format, ...);
unsigned char ReadDat(char *dat);

void InterUART( void ) ;
void InitUART(int rx, int tx, unsigned int brg) ;
void UART_Send(const char *dt) ;
void UART_Write(char dt) ;
int  UART_Available() ;
unsigned int UART_Read() ;
void UART_Flush() ;

void UART_Write(char dt) ;
void PWM_Set_Up(void);
void SetDuty(unsigned int duty_ratio,char basyo);

void SoftWarePWM(void);

#endif	/* UART_DRIVER_H */