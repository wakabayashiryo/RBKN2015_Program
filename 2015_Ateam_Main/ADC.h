///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//木更津高専ロボット研究同好会　
//アナログデジタルコンバータ用関数
//Use device microchip PIC16F1938
//MPLAB X IDE(ver.2.30)
//HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode  
//Last updata 2015/5/145
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ADC_H
#define ADC_H

void adc_init(void);
//機能：	ADCを初期化します。

unsigned int AnalogRead(unsigned char ch);
//機能：	ADCで1回AD変換します。
//引数：	ch...チャンネル
//返り値：	AD変換結果

unsigned int Samp_AD(unsigned char select);
//機能：	ADCで64回AD変換します。
//引数：	select...チャンネル
//返り値：	AD変換結果の平均値

#endif	/* ADC_H */