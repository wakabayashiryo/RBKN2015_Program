#include<htc.h>//pic16f1938を使用中だぜぇー//
#include<stdio.h>//標準入出力関数
#include"ECCP.h"
#include"I2C.h"
#include"UART.h"
#include"Device_select.h"

#define _XTAL_FREQ 32000000//システムクロック8MHz*4(PLL)
__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_OFF&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_OFF&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_OFF&STVREN_OFF&LVP_OFF);
__IDLOC(1056);

void pic_init(void);//デバイス初期設定関数

void main (void)//メイン関数
{
    unsigned char stat = 0;//I2C受信ステータス変数
    unsigned  int duty = 0;//モータースピード用変数
    unsigned char dire = 0;//モーター回転方向用変数

    pic_init();//デバイス初期設定
    I2C_init(Adrs);//I2C初期設定
    InitUART(0,0,207);//UART初期設定
    pwm_init();//ECCPモジュール初期設定

    motor_ctrl(0,0);//free mode
    __delay_ms(10);//WAIT
    
    while(1)//無限ループ
    {
        stat = I2C_ReceiveCheck();//I2C受信チェック

        if(stat)//受信を確認したら
        {
            CLRWDT();//ウォッチドッグタイマー初期化
            duty = rcv_data[0]<<2;//DUTYの上位2ビットを読み込む
            duty |= (rcv_data[1]>>6);//DUTYの下位6ビットを読み込む
            dire = (rcv_data[1]&0x03);//方向データを読み込む
        }
        else//受信がきていなかったら
		{
            motor_ctrl(duty,dire);//モーターに出力
    	}
	}
}
void putch(char m)//UART用関数
{
    UART_Write(m);
}
void pic_init(void)//デバイス初期設定関数
{
    OSCCON = 0xf0;  //internal clock 32MHz//
    TRISA = 0x00;   
    TRISB = 0x00;
    TRISC = 0b00011000;
    TRISE = 0x00;

    ANSELA = 0x00;
    ANSELB = 0x00;

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTE = 0x00;
    
    WDTCON = 0x11;//ウォッチドッグタイマー初期設定
}
void interrupt comu(void)//割り込み処理関数
{
    Slave_Interrupt();//Slave用割り込み処理関数
    InterUART();//UART割り込み処理
}