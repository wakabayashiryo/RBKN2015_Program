#include<htc.h>//pic16f1938を使用中//
#include"ECCP.h"
#include"I2C.h"
#include"ADC.h"
#include"Device_select.h"
//character code Shift_JIS

#define _XTAL_FREQ 32000000//システムクロック8MHz*4(PLL)
__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_OFF&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_OFF&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_OFF&STVREN_OFF&LVP_OFF);
__IDLOC(1056);

void pic_init(void);//システム初期設定関数

void main (void)//メイン関数
{
    unsigned int M,duty;//現在の値用変数,出力スピード用変数
    unsigned char dire,stat;//方向用変数,I2C受信ステータス用変数
    int SET,Devi,Sti_Stat;//目標値、偏差(deviation)、目標値のデータ

    pic_init();//システム初期設定
    adc_init();//ADコンバータ初期設定
    I2C_init(Adrs);//I2C初期設定
    pwm_init();//ECCP初期設定
    motor_ctrl(0,0);//free mode
    while(!I2C_ReceiveCheck());//I2Cをチェック(受信できるまで待つ)

    while(1)//無限ループ
    {
        stat = I2C_ReceiveCheck();//受信確認

        if(stat==2)//rcv_data[0] = 下位ビット
        {           //rcv_data[1]=上位ビット
            CLRWDT();//ウォッチドッグタイマーをクリアー
            Sti_Stat=((rcv_data[1]<<8)+rcv_data[0]);//目標値のデータを取得
            SET = Stee_Center - Sti_Stat;//目標値を計算
        }
        
		M = Samp_AD(0);//ポテンションメータの現在位置を所得
        Devi = SET - M;//目標値と現在位置の偏差を計算

      if(Devi<-MinDevi)//偏差が0より低い場合（-devi）
	  {
#ifdef Direction1
	    dire = 1;//方向１
#endif
#ifdef Direction2
	    dire = 2;//方向２
#endif
        duty = MinSpeed;//最小スピードを代入 
        duty |= Kp*Devi;//偏差からスピード計算して加算
	  }
      else if(Devi>MinDevi)//偏差が0より多かったら（+devi）
	  {
#ifdef Direction1
        dire = 2;//方向２
#endif	  
#ifdef Direction2
        dire = 1;//方向1
#endif  
        duty = MinSpeed;//最小スピードを代入
        duty |= Kp*Devi;//偏差からスピード計算して加算
	  }
	  else//目標値の場合
	  {
		  dire = 3;//ブレーキをかける
		  duty = MaxSpeed;
	  }

	if(duty>MaxSpeed)duty = MaxSpeed;//モーター出力が最大スピード超えた場合抑える 

	motor_ctrl(duty,dire);//モーターに出力
    }
}

void pic_init(void)//デバイス初期設定関数
{
    OSCCON = 0xf0;  //internal clock 32MHz//
    TRISA = 0x01;   //input mode RA0//
    TRISB = 0x00;
    TRISC = 0b00011000;
    TRISE = 0x00;

    ANSELA = 0x01;
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
}