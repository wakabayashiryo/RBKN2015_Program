#include<htc.h>//pic16f1938を使用中だぜぇー//
#include"I2C.h"
#include"PORT_CONF.h"
//charcter code Shifh_JIS

#define _XTAL_FREQ 32000000//システムクロック8MHz*4(PLL)
#define Adrs 0x30//I2Cスレーブアドレス
__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_OFF&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_OFF&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_OFF&STVREN_OFF&LVP_OFF);
__IDLOC(1056);

void pic_init(void);//デバイス初期設定関数

void main (void)//メイン関数
{
    unsigned char stat,select,valve;//I2C受信ステータス変数、ポート選択用変数、電磁弁開閉用変数
    stat = select = valve = 0;//各変数初期化

    pic_init();//デバイス初期設定
    I2C_init(Adrs);//I2C初期設定
    
    while(1)//無限ループ
    {
        stat = I2C_ReceiveCheck();//I2C受信確認

        if(stat)//受信を確認したら
        {
            
            GreenLed ^= 1;
            CLRWDT();//ウォッチドッグタイマをクリアー
            valve = rcv_data[0]&0x0f;//開閉データを取得
            select = rcv_data[0]>>4;//ポート選択データを所得

            switch(select)//ポートを選択
            {
                case 1:
                    if(valve>0)//ポート１の開く
                    {
                        BlueOut = 1;
                        //BlueLed = 1;
                    }
                    else//ポート１の閉じる
                    {
                        BlueOut = 0;
                        //BlueLed = 0;
                    }
                break;
                
                case 2:
                    if(valve>0)//ポート２の開く
                    {
                        RedOut = 1;
                        //RedLed = 1;
                    }
                    else//ポート２の閉じる
                    {
                        RedOut = 0;
                        //RedLed = 0;
                    }
                break;
                
                case 3:
                    if(valve>0)//ポート３の開く
                    {
                        GreenOut = 1;
                        //GreenLed = 1;
                    }
                    else//ポート３の閉じる
                    {
                        GreenOut = 0;
                        //GreenLed = 0;
                    }
                break;
                
                case 4:
                    if(valve>0)//ポート４の開く
                    {
                        WhiteOut = 1;
                        //WhiteLed = 1;
                    }
                    else//ポート４の閉じる
                    {
                        WhiteOut = 0;
                       // WhiteLed = 0;
                    }
                break;
                
                case 5:
                    if(valve>0)//ポート４の開く
                    {
                        BlackOut = 1;
                    }
                    else//ポート４の閉じる
                    {
                        BlackOut = 0;
                    }
                break;
                
                case 6:
                    if(valve>0)//ポート４の開く
                    {
                        YellowOut = 1;
                    }
                    else//ポート４の閉じる
                    {
                        YellowOut = 0;
                    }
                break;
            }
        }
    }
}

void pic_init(void)//デバイス初期設定関数
{
    OSCCON = 0xf0;  //internal clock 32MHz//
    TRISA = 0x00;   //input mode RA0//
    TRISB = 0x00;
    TRISC = 0b11011000;//マスターのRXとの干渉防止にTXを入力に設定
    TRISE = 0x00;

    ANSELA = 0x00;
    ANSELB = 0x00;

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTE = 0x00;
    
    WDTCON = 0x11;//ウォッチドッグタイマー初期設定
}

void interrupt comu(void)//割り込み処理用関数
{
    Slave_Interrupt();//Slave用割り込み処理関数
}