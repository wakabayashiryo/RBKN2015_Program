#include<htc.h>
#include"ECCP.h"
#include"I2C.h"
#include"ADC.h"
#include"Tmr0.h"

#define Adrs 0x60 //I2Cスレーブアドレス
#define SeikiSpeed 350//世紀末の固定スピード
#define SeikiDire 1//方向定義
#define SensorRef 512//センサーのリファレンス値
#define _XTAL_FREQ 32000000//システムクロック8MHz*4(PLL)

__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_OFF&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_OFF&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_OFF&STVREN_OFF&LVP_OFF);
__IDLOC(1056);

void pic_init(void);//システム初期設定関数

unsigned int Wait_count;//タイマー０用カウント変数

void main (void)//メイン関数
{
    unsigned char stat = 0;//I2C受信ステータス変数
    static bit flag = 0,wait = 0;//停止用フラグ、初期動作用フラグ
    
    pic_init();//システム初期設定
    Tmr0_Init();//タイマー０初期設定
    I2C_init(Adrs);//I2Cスレーブ初期設定
    adc_init();//ADコンバータ初期設定
    pwm_init();//ECCP初期設定

    motor_ctrl(0,0);//free mode
    __delay_ms(10);
    
    while(1)//無限ループ
    {
        stat = I2C_ReceiveCheck();//I2C受信確認

        if(stat)//受信したら
        {
            CLRWDT();//ウォッチドッグタイマーをクリアー
            if(rcv_data[0]==0x01)//マスターから
            {
                wait = 1;//初期動作を開始
                Wait_count = 0;//タイマー０用カウント変数を初期化
            }   
        }
       
        if(wait)//初期動作
        {
            if(Wait_count>7000)//左の回数までカウントしたら
            {
                wait = 0;//初期動作を終了
                flag  = 1;//停止用フラグを立てる
            }
            else //上のカウント数まで出力
                motor_ctrl(SeikiSpeed,SeikiDire);//モーターに出力
        }
        else if(flag==1)//停止用フラグが立ったら
        {
            motor_ctrl(SeikiSpeed,SeikiDire);//モーターに出力

            if(Samp_AD(0)>SensorRef)//センサーがリファレンスを超えたら
            {
                flag = 0;//フラグを落として、停止させる
            }
        }         
         else//フラグが落ちていたら
        {
            motor_ctrl(SeikiSpeed,3);//ブレーキを出力
        }
    }
}

void pic_init(void)//デバイス初期設定関数
{
    OSCCON = 0xf0;  //internal clock 32MHz//
    TRISA = 0x01;   //input mode RA0//
    TRISB = 0x00;
    TRISC = 0b11011000;//マスターのRXとの干渉防止にTXを入力に設定
    TRISE = 0x00;

    ANSELA = 0x01;
    ANSELB = 0x00;

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTE = 0x00;
    
    WDTCON = 0x011;//ウォッチドッグタイマー設定
}

void interrupt comu(void)//割り込み処理用関数
{
    Slave_Interrupt();//I2C割り込み処理
    Inter_Count();//タイマ0－処理
}