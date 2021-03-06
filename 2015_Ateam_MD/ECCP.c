///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//木更津高専ロボット研究同好会　
//フルブリッジフルN_MOSFETモータードライバー用関数
//Use device microchip PIC16F1938
//MPLAB X IDE(ver.2.30)
//HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode  
//Last updata 2015/5/13/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include<htc.h>
#include"ECCP.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void pwm_init(void)PWMを使用するための初期設定。
//・仕様 
//	PWM周波数 = ２０KHz @ 内部オシレータ32MHz -----(1)
//	タイマー２の動作開始　タイマー２のプリスケーラーは１：１-----(2)
//	ECCPモジュールの初期設定　順方向フルブリッジ　全出力アクティブlow-----(3)
//	CCP1はタイマー２を使用------(4)
//	再スタート無し------(5)
//	出力ステアリングの変更が次の PWM 周期で発生する また、出力ポートイナブル-----(6)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void pwm_init(void)
{
    PR2 = 200; 					//(1)
    T2CON = 0b00000101;		//(2)

    CCP1CON = 0b01001100;	//(3)	
    CCPTMRS0 = 0b11111100;	//(4)
    CCPTMRS1 = 0b00000011;	

    PWM1CON = 0b00000000;	//(5)
    PSTR1CON = 0b00011111;	//(6)
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//void motor_ctrl(unsigned int duty,unsigned char dire):モーターの方向、回転数を制御する関数。
//・duty = モーターの回転速度を設定
//	回転速度は最高速度の700段階で調整可能
//	ただし、貫通（FETの破損）を防ぐため95%（700段階）に制限
//・dire = モーター回転方向を設定
//	回転方向のデータ
//	+-------------------------------------------+
//	 |    データ(二進数) |     動作     ｜
//	+-------------------------------------------+
//	 |        0b0000            |   フリー　｜
//	 |        0b0001            |   正回転   |
//	 |        0b0010            |   逆回転   |
//	 |        0b0011            |  ブレーキ ｜
//	+-------------------------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void motor_ctrl(unsigned int duty,unsigned char dire)
{
    if(duty>700)//回転速度制限最大700まで//
        duty = 700;

//回転方向データ(0b0000) ,フリー動作 // 
   if(dire == 0)	
    {
        LED1 = 0;	//外部のLEDに状態を表示//
        LED2 = 0;
        CCP1CON = 0b01000000;	//ECCPモジュール停止//
        CCPR1L = 0; 	//回転速度を０にセット//
        P1A = 0;	//ECCP全出力low//
        P1B = 0;
        P1C = 0;
        P1D = 0;    

    }

//回転方向データ(0b0001) ,正回転動作 //
    else if(dire == 1)	
    {
        LED1 = 1;	//外部のLEDに状態を表示//
        LED2 = 0;
        CCP1CON = 0b01001100;   //ECCP正回転モード//
        CCPR1L = duty >> 2; //任意の回転速度を設定//
        DC1B0 = duty & 0b00000001;
        DC1B1 = (duty & 0b00000010) >> 1;
    }

//回転方向データ(0b0010) ,逆回転動作 //
     else if(dire == 2)	
    {
        LED1 = 0;	//外部のLEDに状態を表示//
        LED2 = 1;
        CCP1CON = 0b11001100;  //ECCP逆回転モード//
        CCPR1L = duty >> 2;  //任意の回転速度を設定//
        DC1B0 = duty & 0b00000001;
        DC1B1 = (duty & 0b00000010) >> 1;
    }

//回転方向データ(0b0011) ,ブレーキ動作 //
    if(dire == 3)	
    {
        LED1 = 1;	//外部のLEDに状態を表示//
        LED2 = 1;
        CCP1CON = 0b01000000;	//ECCPモジュール停止//
        CCPR1L = 0;	//回転速度を０にセット//
        P1A = 0;	//ECCP全出力low//
        P1B = 0;
        P1C = 0;
        P1D = 0;

        P1A = 0;    //ブレーキ状態//
        P1B = 1;
        P1C = 0;
        P1D = 1;

    }
}

