/* 
 * File:   System.h
 * Author: evaota
 *
 * Created on 2015/08/08, 13:26
 */

#ifndef SYSTEM_H
#define	SYSTEM_H


// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF        // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = HI        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), high trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

/********************LED_MACRO***********************/
#define pLedDi	PORTA,2//LED出力ポート設定(before ver PORTA4)(new ver PORTA,2)
#define LedNum 10//LEDの個数を定義(最大10個まで)
/********************UART_DATAMACRO********************/
#define UART_Buff 5//UART受信バッファーの容量設定
#define NoData 0xffff//
/***************UART_CONTET_ADDRESS***************/
#define Cata 0x00//
#define zone 0x01//
#define charge 0x02//
/*************LED_NUMBER**************/
#define LED1 0x00
#define LED2 0x01
#define LED3 0x02
#define LED4 0x03
#define LED5 0x04

typedef struct  {
	unsigned char		green;
	unsigned char		red;
	unsigned char		blue;
} LedData;

extern LedData      gLed[ 10 ]	@ 0x25;

//全部PORTAと同じBANKで　BANK2にRAMが有るデバイスだとLATA可
volatile char	dt		@ 0x20;
volatile char	bitCount @ 0x21;
volatile char	byteCount @ 0x22;
volatile int	saveFsr	@ 0x23;

void pic_init(void);
void SendLed(void);
void All_Send(LedData data,unsigned char dimming);
void All_Clear(void);
void Dimming(unsigned char Valu);
void test(void);

#endif	/* SYSTEM_H */

