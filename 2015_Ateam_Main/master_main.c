/*
 * File:   main.c
 * Author: evaota
 *
 * Created on 2015/06/15, 21:14
 */
//character code Shift_JIS

#include <htc.h>
#include <stdio.h>//標準入出力関数
#include <stdlib.h>
#include "UART.h"
#include "I2C.h"
#include "ADC.h"
#include "Tmr0.h"
#include "Tmr1.h"
#include "IOC.h"
#include "System.h"
#include "Ctrl_Dataformat.h"
#include "Slave_Driver.h"

//#define Debug//printfを使ってデバッグする場合は、Debugを定義してください。
#define _XTAL_FREQ 32000000//システムクロック8MHz*4(PLL)
__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_ON&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_ON&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_ON&STVREN_OFF&LVP_OFF); //コンフィグレーション設定
__IDLOC(1056);

void pic_init(void); //システム初期設定関数
unsigned char ZoneRecognize(unsigned char zone);//ゾーン確認用変数
void putch(char m); //printf用関数プロトタイプ宣言
void Ctrl_Main(unsigned char *prcdat); //データ処理関数
void Led_Flu(void); //受信確認LED用関数
unsigned int Calu_Speed(signed char y); //モータースピード計算関数

bit keepflag, SwWait;
int Fp_X, Fp_Y; //アナログスティック初期値用変数
unsigned char phase; //動作カウント変数(自ポール側発射機構)
unsigned char Shot_flg; //発射機構フラグ、ゾーン指定フラグ
unsigned int SW_SampRate; //スイッチ用サンプリングレート用変数

void main(void)//メイン関数
{
    unsigned char Revdata[8] = {0}; //受信データ格納配列
    unsigned int add, Whidat; //各受信データのアドレス、bluetoothからの受信データ
    unsigned char CntForCal = 0; //アナログスティックキャリブレーション用変数

    pic_init(); //マイコン初期設定
    IOC_Init(0,32);//状態変化割り込み初期設定(RB5)
    Tmr0_Init(); //タイマー０初期設定
    Tmr1_Init(); //タイマー１初期設定
    InitUART(0,0,207); //UART初期設定(boudlate 9600bps)
    I2C_init(0x4f); //I2C初期設定(マスターモード)
    adc_init(); //ADコンバーター初期設定

    while (UART_Read() == 0xFFFF); //UARTのデータが来るまで待機

    Led_Flu(); //受信確認用LEDを点滅
    EXT_OUT2 = 1;
           
    while (1)//無限ループ
    {
        Whidat = UART_Read(); //Bluetooth read
        add = Whidat >> 5; //read address 
        Revdata[add] = Whidat & 0x001f; //リモコンの各データ読み込み

        if (Whidat == 0xffff)//正常なデータが来ない場合
        {
            continue; //ループの先頭に戻る
        }

        if (add == 7)//7個目のデータが受信出来たら
        {
            if (CntForCal >= 8)
            {
                CLRWDT(); //ウォッチドッグタイマ初期化

                LED_RED1 ^= 1; //UART受信間隔計測用(GREEN_LED1)
#ifndef Debug
                System_Indicater2(0,phase,0,0,0);//send WS2812 indicater
#endif			
                Ctrl_Main(Revdata); //データ処理関数に値を渡す。
                
                UART_Flush(); //UARTのバッファーをクリア
            }
            else//analogstick's value is get average.
            {
                if (((15 - StopBorder) < Revdata[RX] && Revdata[RX]<(15 + StopBorder))&&((15 - StopBorder) < Revdata[LY] && Revdata[LY]<(15 + StopBorder))) 
                {
                    Fp_X += Revdata[RX];
                    Fp_Y += Revdata[LY];

                    CntForCal++;
                }
                if (CntForCal == 8)
                {
                    Fp_X >>= 3;
                    Fp_Y >>= 3;
                }
            }
        }
        ;
    }
}

#ifdef Debug

void putch(char m)//printf用関数
{
    UART_Write(m);
}
#endif

void Led_Flu(void)//LED点滅用関数
{
    unsigned char count, che;
    for (che = 0; che < 5; che++)
    {
        LED_BLUE1 = 1; //pic16f1939's LED
        LED_BLUE2 = 1; //pic16f1939's LED

        for (count = 0; count < 20; count++) 
        {
            __delay_ms(10);
            CLRWDT();
        }

        LED_BLUE1 = 0; //pic16f1939's LED
        LED_BLUE2 = 0; //pic16f1939's LED

        for (count = 0; count < 20; count++) 
        {
            __delay_ms(10);
            CLRWDT();
        }
    }
}

unsigned int Calu_Speed(signed char y)//モータースピード計算関数(引数はアナログスティックの絶対値-15~+15)
{
    unsigned int ResSpd; //計算結果用変数

    y = abs(y); //絶対値を計算
    ResSpd = (short) MaxSpeed * (short) (y << 1) >> 5; //モータースピードを計算

    return ResSpd; //結果を返す
}

void pic_init(void)//システム初期設定関数
{
    OSCCON = 0xf0; //internal clock 32MHz//

    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x80;
    TRISD = 0x00;
    TRISE = 0x00;

    ANSELA = 0b00000000;
    ANSELB = 0b00000000;
    ANSELD = 0b00000000;
    ANSELE = 0b00000000;

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;

    WDTCON = 0x11; //ウオッチドッグタイマー設定(256ms)
    
    SETUPCON1_A1(INPUT); //三連パラレルサーボ切り替えスイッチ用(RA5)
    SETUPCON2_A1(INPUT); //コネクター２のA1を入力モードに設定
    SETUPCON3_A1(INPUT); //三連パラレル最終投擲用センサー入力(RB5)
    SETUPCON4_A1(INPUT); //コネクター4のA1を入力モードに設定

    ANALOGSETCON2_A1(ANALOG); //コネクター２のA1をアナログモードに設定
    ANALOGSETCON4_A1(ANALOG); //コネクター4のA1をアナログモードに設定

    WPUB = 0x20;//RB5 is enabled Weak Pull up
    nWPUEN =0;//Weak pull up enable
}

void interrupt common_inter(void)//割り込み処理関数
{
    Master_Interrupt(); //I2C送信割り込み関数
    InterUART(); //UART受信割り込み関数
    Inter_Count(); //タイマー0割り込み関数
    Tmr1_Inter(); //タイマー１割り込み関数
    Inter_IOC();//状態変化割り込み関数
}

void Ctrl_Main(unsigned char *prcdat)//データ処理用関数
{
    ////////////////////////Seikimatsu_Process////////////////////////
    if (!(prcdat[Marutoka] & Circle))//記号ボタンの丸が押されたら
        Seikimatsu_ON(); //世紀末をリミットスイッチが反応するまで動かす。
    else
        Seikimatsu_OFF(); //押されていなければ、ホールド
    ////////////////////////SidearmThrow_Process////////////////////////
    if (!(prcdat[Marutoka] & Sq))//記号ボタンの四角が押されたら
    {
        Side_On(); //サイドスロー機構動作開始
    }
    else
        Side_Acti(); //ボタンがはされたら電磁弁を開放する
    /////////////////////////ThreeParallel_Process////////////////////////
    if (!(prcdat[Marutoka] & Tri))//記号ボタンの三角が押されたら
    {
        Lock_On(); //保持機構をONにする。(自分ポール機構)
    }
    Lock_Acti(); //(フロー制御)
//    
//    if(IOC5f)//phase3電磁弁開放フラグを表示
//        LED_RED2 = 1;
//    else 
//        LED_RED2 = 0;
    if(phase==2)//phaseが２の時は状態変化割り込みを有効にする
    {
        EN_IOC();//phase2以外で状態変化割り込みが起こるのを防ぐため
    }
    else//それ以外の時は無効にする
    {
        IOC5f = 0;//電磁弁開放フラグを落とす
        DI_IOC();
    }
    ////////////////////////ManualServo_Process////////////////////////
    if ((RA5 && (SwWait == 0)) || ((!(prcdat[LR] & L1)) && SwWait == 0))//外付けスイッチかコントローラー(L1)でサーボの位置を変更（タイマーカウント中は入力禁止）
    {
        SwWait = 1; //入力禁止カウント開始
        SW_SampRate = 0; //カウント用変数初期化
        phase++; //ロック機構のサーボの位置を変更
    }
    if (SwWait == 1 && SW_SampRate > SW_SampTime) SwWait = 0; //入力禁止カウント停止。また、入力を許可
    ////////////////////////Catapult_Process////////////////////////
    if(!(prcdat[Zyuzi]&Left))
        On_Valve(3);
    else if(!(prcdat[Zyuzi]&Right))
        On_Valve(4);
    else if (!(prcdat[LR]&R2))//コントローラーのR2が押されたらカタパルト両方開放
    {
        On_Valve(3); //PORT3の電磁弁をHIGHにする。(カタパルトの電磁弁)
        On_Valve(4); //PORT4の電磁弁をHIGHにする。(カタパルトの電磁弁)
    }
    else
    {
        Off_Valve(3); //PORT3の電磁弁をLOWする。(カタパルトの電磁弁)  
        Off_Valve(4); //PORT4の電磁弁をLOWする。(カタパルトの電磁弁)    
    }
    /////////////////////////////Drive_Process////////////////////////////////
    static bit StrFlag; //ステア条件分岐用変数
    int Now_X, Now_Y, Left_X; //アナログスティック現在位置用変数
    static unsigned int A_Steer, B_Steer;
    unsigned int ASpeed = 0, BSpeed = 0; //左右モータースピード用変数

    Now_X = Fp_X - prcdat[RX]; //右アナログスティックのX軸の絶対値を計算
    Now_Y = Fp_Y - prcdat[LY]; //左アナログスティックのY軸の絶対値を計算
    Left_X = Fp_X - prcdat[LX]; //左アナログスティックのX軸の絶対値を計算

    if (!(prcdat[LR] & R1))//旋回処理　R1が押されたらステアを変え一定時間に経過したらアナログスティックの値に応じてモーターに出力
    {
        StrFlag = 0; // アナログスティックによるステアリング操作を無効化

        Encorder_Str(A_Ster, ASteer); //ステアを45度回転(磁気式バージョン)
        Encorder_Str(B_Ster, BSteer);

        if (Tmr1_Wait_ms(TurnInterval) || keepflag)//R1が押されて指定時間はブレーキ
        {
            keepflag = 1;

            if (Left_X <= -StopBorder)
            {
                Out_Motor(A_Drv, Forward, Calu_Speed(Left_X)); //ステアのスピードでモーターに出力
                Out_Motor(B_Drv, Forward, Calu_Speed(Left_X));

            }
            else if (Left_X > StopBorder) 
            {
                Out_Motor(A_Drv, Reversal, Calu_Speed(Left_X)); //ステアのスピードでモーターに出力
                Out_Motor(B_Drv, Reversal, Calu_Speed(Left_X));
            }
            else 
            {
                Out_Motor(A_Drv, Brake, ASterSpeed); //ブレーキモード
                Out_Motor(B_Drv, Brake, BSterSpeed);
            }
        }
        else
        {
            Out_Motor(A_Drv, Brake, ASterSpeed); //ブレーキモード
            Out_Motor(B_Drv, Brake, BSterSpeed);
        }
    }
    else if (Now_Y <= -StopBorder)//前後進処理
    {
        StrFlag = 1; // アナログスティックによるステアリング操作は有効

        if (!(prcdat[LR] & L2))//Speed is half,when User pushed R2button.
        {
            ASpeed = BSpeed = Calu_Speed(Now_Y) >> 1; //スピードを半分に計算
        }
        else
        {
            ASpeed = BSpeed = Calu_Speed(Now_Y);
        }

        Out_Motor(A_Drv, Forward, (unsigned int) (ASpeed * ((float) Samp_AD(0) / 1000.f))); //左右モーターに出力
        Out_Motor(B_Drv, Reversal, (unsigned int) (BSpeed * ((float) Samp_AD(8) / 1000.f)));
    }
    else if (Now_Y > StopBorder)//前後進処理
    {
        StrFlag = 1; // アナログスティックによるステアリング操作は有効

        if (!(prcdat[LR] & L2))//Speed is half,when User pushed R2button.
        {
            ASpeed = BSpeed = Calu_Speed(Now_Y) >> 1; //スピードを半分に計算
        }
        else
        {
            ASpeed = BSpeed = Calu_Speed(Now_Y);
        }

        Out_Motor(A_Drv, Reversal, (unsigned int) (ASpeed * ((float) Samp_AD(0) / 1000.f))); //左右モーターに出力
        Out_Motor(B_Drv, Forward, (unsigned int) (BSpeed * ((float) Samp_AD(8) / 1000.f)));
    }
    else//analogstick's position is center.
    {
        keepflag = 0;
        Tmr1_Wait_ms(0);

        StrFlag = 1; //アナログスティックによるステアリング操作は有効
        Dead_Stop(500); //全駆動モーターブレーキ
    }
    ///////////////////////Steering_Process//////////////////////////
    if (StrFlag)//アナログスティックによるステアリングが有効なら
    {
        Encorder_Str(A_Ster, (int) (-Now_X * AStePala)); //アナログスティックの値でステアの角度変更(磁気式バージョン)
        Encorder_Str(B_Ster, (int) (-Now_X * BStePala));
    }
}