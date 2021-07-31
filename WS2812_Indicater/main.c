/* * File:   main.c
 *
 * MPLAB-X V2.15  XC8 V1.32 Free
 */
#include <xc.h>//xc8 comipilar use
#include"UART_Driver.h"
#include"Colors.h"
#include"System.h"

//#define indicatertest//all led test mode
#define _XTAL_FREQ 32000000//internal clock 32MHz 8MHz*4(PLL)
__IDLOC(1056);//personal pic id is 1056

void Process(unsigned char *data);//データ処理関数
unsigned char ReciBuff[UART_Buff];//UARTデータ格納バッファー
LedData  gLed[ 10 ]	@ 0x25;//LED表示用バッファー

void main(void)//メイン関数
{
    unsigned int ReciData,EmergencyCount = 0;//UART受信用関数、ソフトウェアウォッチドッグタイマ用関数

    pic_init();//PIC初期化
#ifndef indicatertest
    InitUART(0,0,207);//UART初期化（テストモードでは初期化されない）
#endif
    
    while(1)//無限ループ
    {
#ifdef indicatertest
	  test();//テスト用関数
#else
        ReciData = UART_Read();//UARTからのデータを受信             
        if(ReciData==NoData) //データが来ていない場合
        {
            if(EmergencyCount<60000)//ソフトウェアウォッチドッグタイマを60000までカウント
                EmergencyCount++;
        }
        else
        {
            EmergencyCount = 0;//受信が来ている場合ソフトウェアウォッチドッグタイマを初期化
        }
        if(EmergencyCount>500)//2000カウントまでデータが来ていない場合は、NO CONECTION MODE
        {
            //EmergencyCount=501;
            gLed[LED1] = C_Red;//すべてのLEDを赤で点灯
            gLed[LED2] = C_Red;
            gLed[LED3] = C_Red;
            gLed[LED4] = C_Red;
            gLed[LED5] = C_Red;
            
            SendLed();//LEDに送信
            __delay_ms(500);//500ms待つ
            
            All_Clear();//すべてのLEDを消灯
            
            SendLed();//LEDに送信
            __delay_ms(500);//500ms待つ
        }
        
        else//データが来た場合
        {
            UART_Flush();//UARTのバッファーをクリアー
            
            ReciBuff[zone] = ReciData&0x03;//下位1,2ビット目をマスクする
            ReciBuff[charge] = (ReciData>>2)&0x07;//上位1ビット目下位3,4ビット目をマスクする
            
            Process(ReciBuff);//データを処理
            SendLed();//LEDに送信
        }
        
#endif  
    }
}

void Process(unsigned char *data)
{
    gLed[LED2] = Clear;
    gLed[LED4] = Clear;
    
    gLed[LED1] = C_Green;//一番目のLEDを通信中を示す緑を表示
            
    if(data[zone]==0)//データが0の場合赤を表示
        gLed[LED3] = C_Red;
    else if(data[zone]==1)//データが1の場合青を表示
        gLed[LED3] = C_Blue;

    if(data[charge]==0)//データが0の場合青を表示
        gLed[LED5] = C_Blue;
    else if(data[charge]==1)//データが1の場合赤を表示
        gLed[LED5] = C_Red;
    else if(data[charge]==2)//データが2の場合緑を表示
        gLed[LED5] = C_Green;
    else if(data[charge]==3)//データが3の場合白を表示
        gLed[LED5] = C_White;
}                

void interrupt commoninter(void)
{
     InterUART();//UART受信割り込み
}
