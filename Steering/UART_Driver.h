/*
 * File:   UART_Driver.h
 * Author: Oishi
 *
 * Created on 2015/02/22, 19:27
 */
#ifndef UART_DRIVER_H
#define	UART_DRIVER_H

void InterUART( void ) ;
//機能：	UARTの割り込み時の読み出し関数

void InitUART(int rx, int tx, unsigned char brg) ;
//機能：	UART初期化関数
//引数：	rx...受信ピン設定（1938などは関係なし）
//			tx...送信ピン設定（1938などは関係なし）
//			brg...速度＠9600bit/sec（32MHz...207、16MHz...103）

void UART_Write(char dt) ;
//機能：	UART送信関数
//引数：	dt...送信データ

unsigned int UART_Read() ;
//機能：	UART読み出し関数
//返り値	データが送られてきたら、送られたUARTの読み出し値で、何もバッファに残っていなければ0xFFFF
void UART_Flush() ;
//機能：	UARTバッファクリア関数

#endif	/* UART_DRIVER_H */