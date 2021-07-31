///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//木更津高専ロボット研究同好会　
//I2C通信用関数
//Use device microchip PIC16F1938
//MPLAB X IDE(ver.2.30)
//HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode  
//Last updata 2015/5/12/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef I2C_H
#define	I2C_H

#define Master //mode select macro(Master or Slave)//
//#define Master

#define pinModeSCK TRISCbits.TRISC3
#define pinModeSDA TRISCbits.TRISC4

#define SND_DATA_LEN 8                  // 送信データバッファのサイズ
#define RCV_DATA_LEN 8                  // 受信データバッファのサイズ

/////////スレーブの受信データ、送信データ格納配列/////////////////
unsigned char rcv_data[RCV_DATA_LEN] ;  // 受信データバッファ
unsigned char snd_data[SND_DATA_LEN] ;  // 送信データバッファ

/**************************************Master_Mode********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef  Master
void Master_Interrupt(void);
//機能：マスターの割り込み時の呼び出し関数
int I2C_Send(unsigned char adrs,int len,unsigned char *buf);
//機能：	I2Cの送信関数
//引数：	adrs...送信先
//			len...長さ
//			buf...送信データ
int I2C_Receive(unsigned char adrs,int len,unsigned char *buf);
//機能：	I2Cの受信関数
//引数：	adrs...受信先
//			len...長さ
//			buf...受信バッファ
void I2C_IdleCheck(char mask);
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************Slave_Mode********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Slave
void Slave_Interrupt( void );
//機能：	スレーブ割り込み時間数
int I2C_ReceiveCheck() ;
//機能：	受信チェック
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef  Master
void I2C_init(unsigned char speed);
//機能：	マスター初期化関数
//引数：	speed...速度（16MHｚ...26くらい？）
#endif
#ifdef  Slave
void I2C_init(unsigned char add);
//機能：	スレーブ初期化関数
//引数：	add...スレーブのアドレス
#endif

#endif	/* I2C_H */

