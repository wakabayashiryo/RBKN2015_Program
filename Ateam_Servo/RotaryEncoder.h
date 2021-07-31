/* 
 * File:   RotalyEncorder.h
 * Author: oishi
 *
 * Created on 2015/06/26, 17:31
 */

#ifndef ROTARYENCORDER_H
#define	ROTARYENCORDER_H

//Clock_Freq_Define
#define KHz 000
#define MHz 000000
//---WANING---ここにて定義すること
#define _XTAL_FREQ 32MHz

//使用ポート
#define ENCODER_A RA1
#define ENCODER_B RC0

#define ENCODER_A_TRIS TRISAbits.TRISA1
#define ENCODER_B_TRIS TRISCbits.TRISC0


//使用タイマー
//#define USETMR2
//#define USETMR4
#define USETMR6
//#error shine
#ifdef USETMR2
#warning It might be used by ECCP modulate
#endif

//使用関数群

void _Tmr2_4_6InterruptInit(unsigned char n,unsigned char kHz,unsigned char config,void (*func)());
//機能：タイマーの初期化をします。

//引数：ｎ...使用するタイマーを指定します。
//      2..Timer2 4..Timer4 6..Timer6とします。

//　　　ｋHz...周波数を設定します（精度は低いです）

//　　　func...関数ポインタを指定します。簡単に言うと普通関数名を入れます。

//警告：必ずinterrupt_Tmr2_4_6とともに使うこと。

void RotaryEncorderInit(unsigned char tmr,unsigned char kHz,unsigned char Mode,unsigned int n);
//機能：ロータリーエンコーダの使用準備をします

//引数：tmr:使用するタイマーを指定します。
//      2..Timer2 4..Timer4 6..Timer6とします。

//　　　ｋHz...サンプリング周波数を設定します（精度は低いです）。

//      Mode...使用するモードを選択します

//      0...通常モードで、GetRotaryEncorderDataが呼び出されるまでの累積の値を出力します。nは無視されます
//      1...2^n回のサンプリングの値を出力します。

//      n...上記に説明したとおりでふ。

int GetRotaryEncorderData(void);
//機能：ロータリーエンコーダの回転量の値を返します

//引数：なし

//返り値：ロータリーエンコーダの回転量
void interrupt_Tmr2_4_6(void);
//割り込み時に呼び出す関数です。Tmr2_4_6InterruptInitのあと有効です。割り込み原因がTMR2,4,6にあるときは1が帰ります。その他の時は0が帰ります
#define Tmr2_4_6InterruptInit(x,y,z) {\
    _Tmr2_4_6InterruptInit(x,((_XTAL_FREQ/4)/256) < y?\
    (_XTAL_FREQ /4/y-1) : ((_XTAL_FREQ/4/4)/256)<y?\
    (_XTAL_FREQ /4/4/y-1) : (_XTAL_FREQ /4/4/4/y-1),\
    (((_XTAL_FREQ/4)/256) < y\
    ?0x04:((_XTAL_FREQ/4/4)/256) < y ?\
    0x05:0x06),z);}

#endif	/* ROTALYENCORDER_H */