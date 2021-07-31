///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//木更津高専ロボット研究同好会　
//I2C通信用関数
//Use device microchip PIC16F1938
//MPLAB X IDE(ver.2.30)
//HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode  
//Last updata 2015/5/12/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef I2C_H
#define	I2C_
#define Slave //mode select macro(Master or Slave)//
#define pinModeSCK TRISCbits.TRISC3	//SCKpinを入力に設定//
#define pinModeSDA TRISCbits.TRISC4	//SDApinを入力に設定//

#define SND_DATA_LEN 8                  // 送信データバッファのサイズ
#define RCV_DATA_LEN 8                  // 受信データバッファのサイズ

/////////スレーブの受信データ、送信データ格納配列/////////////////
unsigned char rcv_data[RCV_DATA_LEN] ;  // 受信データバッファ
unsigned char snd_data[SND_DATA_LEN] ;  // 送信データバッファ

/**************************************Master_Mode********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef  Master
void Master_Interrupt( void );
int I2C_Send(unsigned char adrs,int len,unsigned char *buf);
int I2C_Receive(unsigned char adrs,int len,unsigned char *buf);
void I2C_IdleCheck(char mask);
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************Slave_Mode********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Slave
void Slave_Interrupt( void );
int I2C_ReceiveCheck() ;
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef  Master
void I2C_init(unsigned char speed);
#endif
#ifdef  Slave
void I2C_init(unsigned char add);
#endif

#endif	/* I2C_H */

