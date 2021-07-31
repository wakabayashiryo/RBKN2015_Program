///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//木更津高専ロボット�?究同好会�?
//フルブリ�?��フルN_MOSFETモータードライバ�?用関数
//Use device microchip PIC16F1938
//MPLAB X IDE(ver.2.30)
//HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode  
//Last updata 2015/5/12/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ECCP_H
#define ECCP_H

#define P1A LATC2	//ECCPモジュールの出力�?ー�?/
#define P1B LATB2
#define P1C LATB1
#define P1D LATB4

#define LED1 LATA3	//状態表示用LEDポ�?�?/
#define LED2 LATA4

void pwm_init(void);
//機�???   PWMを使用するための初期設定�?//

void motor_ctrl(unsigned int duty,unsigned char dire);
//機�???   モーターの方向�?回転数を制御する関数�?//引数??   duty...0-800までの整数
//          dire...0->フリー動作�?1->正転�?->�?���?->ブレーキ

#endif	/* ECCP_H */