/*
 * File:   UART_Driver.h
 * Author: Oishi
 *
 * Created on 2015/02/22, 19:27
 */
#ifndef UART_H
#define	UART_H

void InterUART( void ) ;
//機�???UARTの割り込み時�?読み出し関数

void InitUART(int rx, int tx, unsigned char brg) ;
//機�???UART初期化関数
//引数??rx...受信ピン設定�?938などは関係なし�?//			tx...送信ピン設定�?938などは関係なし�?//			brg...速度??600bit/sec??2MHz...207�?6MHz...103??
void UART_Write(char dt) ;
//機�???UART送信関数
//引数??dt...送信�??タ

unsigned int UART_Read() ;
//機�???UART読み出し関数
//返り値	�??タが�?られてきたら�?送られたUARTの読み出し�?で、何もバッファに残って�?��ければ0xFFFF
void UART_Flush() ;
//機�???UARTバッファクリア関数

#endif	/* UART_H */