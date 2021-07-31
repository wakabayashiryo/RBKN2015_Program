/****************************************************************************
	Ｉ２Ｃ通信で使うプロトタイプ宣言及びグローバル変数の宣言を行うヘッダーファイル
	
	マスター、スレーブともに共有でこちらをHeader Filesに追加してコンパイルしてください。
 ==========================================================================
 VERSION DATE			BY					CHANGE/COMMENT					
 --------------------------------------------------------------------------
  1.00    2011-12-23  きむ茶工房(きむしげ)  Create                            	
  2.00    2012-01-21  きむ茶工房(きむしげ)  スレーブからのACK待ち処理を変更   			
  3.00    2012-06-30  11-305 IMADA     16F1938用に改変、関数をヘッダに移行			
  3.01    2012-07-23  11-305 IMADA     一斉にsendするよう変更
  4.00    2013-02-01  11-413 KAMATA    Hファイル、Cファイルの書き換え マスター及びスレーブで共有化 
  									   name skI2C.c, skI2C.h
 ========================================================================== 
  PIC 12F1822 16F1827 --> 16F1938                                          
  MPLAB IDE(V8.60)                                                         
  HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode        
****************************************************************************/
#ifndef I2C_H
#define I2C_H   //多重読み込み防止の目印

#define SND_DATA_LEN 8                  // 送信データバッファのサイズ
#define RCV_DATA_LEN 8                  // 受信データバッファのサイズ

/***スレーブの受信データ、送信データ格納配列***/
unsigned char rcv_data[RCV_DATA_LEN] ;  // 受信データバッファ
unsigned char snd_data[SND_DATA_LEN] ;  // 送信データバッファ

/*  スレーブ用  */
void Slave_Interrupt( void );
void InitI2C_Slave(int address) ;
int I2C_ReceiveCheck() ;


/*　マスター用　*/
void Master_Interrupt( void );
void InitI2C_Master();
int I2C_Send(unsigned char adrs,int len,unsigned char *buf);
int I2C_Receive(unsigned char adrs,int len,unsigned char *buf);
int I2C_rStart_Receive(unsigned char adrs,unsigned char regist,unsigned char *buf);

void I2C_IdleCheck(char mask);

#endif 