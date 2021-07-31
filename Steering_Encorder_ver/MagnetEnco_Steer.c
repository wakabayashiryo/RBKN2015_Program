#include<htc.h>//pic16f1938//
#include<stdlib.h>
#include"ECCP.h"
#include"I2C.h"
#include"Device_select.h"
#include"RTLY_ENC.h"
#include"Steer_Fun.h"
//character code Shift_JIS

#define eepadrs 0//EEPROMアクセスアドレス
#define _XTAL_FREQ 32000000//システムクロック8MHz*4(PLL)

__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_OFF&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_OFF&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_OFF&STVREN_OFF&LVP_OFF);
__IDLOC(1056);
__EEPROM_DATA(0,0,0,0,0,0,0,0);//EEPROM初期化

void main (void)//メイン関数
{
    unsigned int M,duty;//現在の値用変数,出力スピード用変数   
    unsigned char dire,stat;//方向用変数,I2C受信ステータス用変数
    int SET,Devi;//目標値、偏差(deviation)    
    static bit ZeroFlag;//初期シークエンスフラグ

    pic_init();//システム初期設定
    I2C_init(Adrs);//I2C初期設定
    pwm_init();//ECCP初期設定
    init_RTLY_ENC();//磁気式エンコーダー用関数初期設定

    motor_ctrl(0,0);//free mode
    while(!I2C_ReceiveCheck());//I2Cをチェック(受信できるまで待つ)
    TrPort = 1;//potention power is on.
    ZeroFlag  = 1;//firstposition to move
    SET = eeprom_read(eepadrs);//リセット前の値を読み込む
	__delay_ms(10);    

    while(1)//無限ループ
    {
        M = getData();//エンコーダーの現在位置を所得
       
		if(ZeroFlag)//初期シークエンス
        {
            if(0<SET&&SET<512)
            {
                dire = Detect_Dire(1);
                Devi = Calu_Devi(ZeroFlag,M,SET);            
            }
            else
            {
                dire = Detect_Dire(0);
                Devi = Calu_Devi(ZeroFlag,M,SET);
            }
            
            duty = MinSpeed;
            duty |= Kp * abs(Devi);
            if(Devi<MinDevi||Devi>-MinDevi)
            {
                ZeroFlag = 0;
                Tr_Reset();
            }
        }
        else
        {
            stat = I2C_ReceiveCheck();
            if(stat==2)//rcv_data[0] = lower bits
            {          //rcv_data[1] = higher bits
                CLRWDT();  //ウォッチドッグタイマーをクリアー          
                SET=((rcv_data[1]<<8)+rcv_data[0])-256;//目標値のデータを取得//-256~+256
            }
            if(M<=512)
                M += 512;
            else if(M<=1023)
                M -= 512;
            Devi = Calu_Devi(ZeroFlag,M,SET);//目標値と現在位置の偏差を計算
            if(Devi<-MinDevi)//偏差が0より低い場合（-devi）
            {
                dire = Detect_Dire(0);//モーターの出力方向を決定
                duty = MinSpeed;//最小スピードを代入
                duty |= Kp*-Devi;//偏差からスピード計算して加算
            }
            else if(Devi>MinDevi)//偏差が0より多かったら（+devi）
            {     
                dire = Detect_Dire(1);//モーターの出力方向を決定
                duty = MinSpeed;//最小スピードを代入
                duty |= Kp*Devi;//偏差からスピード計算して加算
            }
            else//目標値の場合
            {
                dire = 3;//ブレーキをかける
                duty = MaxSpeed;
            }
        }
        if(duty>MaxSpeed)duty = MaxSpeed;//モーター出力が最大スピード超えた場合抑える 
        eeprom_write(eepadrs,M);
        motor_ctrl(duty,dire);//モーターに出力
    }
}

void interrupt common_interrupt(void)//割り込み処理関数
{
    Slave_Interrupt();//Slave用割り込み処理関数
}