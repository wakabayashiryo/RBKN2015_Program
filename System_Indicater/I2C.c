///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//木更津高専ロボット研究同好会　
//I2C通信用関数
//Use device microchip PIC16F1938
//MPLAB X IDE(ver.2.30)
//HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode  
//Last updata 2015/5/13/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include<htc.h>
#include"I2C.h"
unsigned char rcv_data[RCV_DATA_LEN] ;  // 受信データバッファ
unsigned char snd_data[SND_DATA_LEN] ;  // 送信データバッファ


unsigned int  rcv_flg ;       // 受信情報(受信データの個数を格納)
unsigned char *Sdtp ;         // 送信データバッファのアドレスポインター
unsigned char *Rdtp ;         // 受信データバッファのアドレスポインター

int AckCheck ;
int success ;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//I2Cモジュールの初期設定関数
//・動作　		・記述
//	Master	void I2C_init(unsigned char speed);　　spped = マスター側のクロック周波数設定(default 0x4f)(Fosc = 32MHz)
//	Slave		void I2C_init(unsigned char add);	add = 使用するデバイスのアドレスを設定（1Byte）
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef  Master
void I2C_init(unsigned char speed)
#endif
#ifdef  Slave
void I2C_init(unsigned char add)
#endif
{
#ifdef Master
    SSPSTAT = 0x80;
    SSPCON1 = 0x28;
    SSPADD = speed;
    SSPIE = 1 ;             // SSP(I2C)割り込みを許可する
    BCLIE = 1 ;             // MSSP(I2C)バス衝突割り込みを許可する
    PEIE   = 1 ;             // 周辺装置割り込みを許可する
    GIE    = 1 ;             // 全割り込み処理を許可する
    SSPIF = 0 ;             // SSP(I2C)割り込みフラグをクリアする
    BCLIF = 0 ;             // MSSP(I2C)バス衝突割り込みフラグをクリアする
    pinModeSCK  = 1;
    pinModeSDA = 1;

#endif
#ifdef Slave
    SSPSTAT = 0x00;
    SSPCON1 = 0x26;
    SEN = 1;
    SSPADD = add<<1;
    SSPMSK = 0xfe;
    SSPIE = 1 ;             // SSP(I2C)割り込みを許可する
    BCLIE = 1 ;             // MSSP(I2C)バス衝突割り込みを許可する
    PEIE   = 1 ;             // 周辺装置割り込みを許可する
    GIE    = 1 ;             // 全割り込み処理を許可する
    SSPIF = 0 ;             // SSP(I2C)割り込みフラグをクリアする
    BCLIF = 0 ;
    pinModeSCK = 1;
    pinModeSDA = 1;
 #endif
}
/**************************************Master_Mode********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Master用割り込み処理関数
//	必ずinterrupt **(void)の中に入れてください。
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Master
void Master_Interrupt( void )
{
     if (SSPIF == 1) {       // SSP(I2C)割り込みが発生したか判断
	     if (AckCheck == 1) AckCheck = 0 ;
          SSPIF = 0 ;        // フラグクリア
     }
     else if (BCLIF == 1) {       // MSSP(I2C)バス衝突割り込みが発生したか判断
          BCLIF = 0 ;       // 今回はフラグのみクリアする(無処理)
     }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ans = I2C_Send(adrs,len,buf);アドレスで指定したSlaveに(len)Byte分のデータを送信する関数
//
//・adrs = スレーブのアドレスを指定する(8?119を推奨)
//・len  : 送信するデータ(バイト)の個数を指定する
//・buf  : 送信するデータを格納した配列を指定する
//・ans  : 0=正常(送信完了)　1=スレーブが返答しない　2=相手がデータの受信を拒否した
//	送信したあと、ansで状態を確認
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int I2C_Send(unsigned char adrs,int len,unsigned char *buf)
{
     int i , ans ;

     // スタート(START CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.SEN = 1 ;
     // [スレーブのアドレス+スレーブは受信を要求]を送信する
     I2C_IdleCheck(0x5) ;
     AckCheck = 1 ;
     SSPBUF = (char)(adrs<<1) ;             // アドレスを送信 R/W=0
     while (AckCheck) ;                      // 相手からのACK返答を待つ
     ans = SSPCON2bits.ACKSTAT ;
     if (ans == 0) {
          for (i=0 ; i<len ; i++) {			// [データ]を送信する
               I2C_IdleCheck(0x5) ;
               AckCheck = 1 ;
               SSPBUF = (unsigned char)*buf ;        // データを送信
               buf++ ;
               while (AckCheck) ;            // 相手からのACK返答を待つ
               ans = SSPCON2bits.ACKSTAT ;
               if (ans != 0) {
                    ans = 2 ;                // 相手がNOACKを返した
                    success = 1;			 //確認用
                    break ;
               }
          }
     }
     // ストップ(STOP CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.PEN = 1 ;
     return ans ;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int I2C_Receive(adrs,len,*buf)アドレスで指定したSlaveから(len)Byte分のデータを受信する関数
//・data : 受信データ保管用変数
//・adrs = スレーブのアドレスを指定する(8?119を推奨)
//・len　 = 送信するデータ(バイト)の個数を指定する
//・ans  : 0=正常(受信完了)　1=スレーブが返答しない　2=相手がデータの受信を拒否した
//・*buf :　受信したデータ
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int I2C_Receive(unsigned char adrs,int len,unsigned char *buf)
{
	 int data = 0 ;

     int i , ans ;

     // スタート(START CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.SEN = 1 ;
     // [スレーブのアドレス+スレーブへデータ要求]を送信する
     I2C_IdleCheck(0x5) ;
     AckCheck = 1 ;
     SSPBUF = (char)((adrs<<1)+1) ;    // アドレスを送信 R/W=1
     while (AckCheck) ;                 // 相手からのACK返答を待つ
     ans = SSPCON2bits.ACKSTAT ;
     if (ans == 0) {
          for (i=0 ; i<len ; i++) { 				// [データ]を受信する
               I2C_IdleCheck(0x5) ;
               SSPCON2bits.RCEN = 1;  // 受信を許可する
               I2C_IdleCheck(0x4) ;
               *(buf+i) = SSPBUF ;         // 受信
               //data = SSPBUF; //受信a
               //buf++ ;
               I2C_IdleCheck(0x5) ;
               if (i=len) SSPCON2bits.ACKDT = 1 ; // ACKデータはNOACK
               else       SSPCON2bits.ACKDT = 0 ; // ACKデータはACK
               SSPCON2bits.ACKEN = 1 ;            // ACKデータを返す
          }
     }
     // ストップ(STOP CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.PEN = 1 ;
     return ans ;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///* I2C_IdleCheck(char mask) : アイドル状態のチェック	*/
// ACKEN RCEN PEN RSEN SEN R/W BF が全て０ならＯＫ
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_IdleCheck(char mask)
{
     while (( SSPCON2 & 0x1F ) | (SSPSTAT & mask)) ;
}
#endif
/***************************************Slave_Mode*********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Slave用割り込み処理関数
//必ずvoid interrupt **(void)の中に入れてください。
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Slave
void Slave_Interrupt( void )
{
	char x ;

  /*** SSP(I2C)割り込み発生時の処理***/
     if (SSPIF == 1) {  //
          if (SSPSTATbits.R_nW == 0) {
               /******* マスターからの書き込み要求(スレーブは受信) *******/
               if (SSPSTATbits.D_nA == 0) {
                    // 受信バイトはアドレス
                    Rdtp = (char *)rcv_data ;
                    x = SSPBUF ;       // アドレスデータを空読みする
                    rcv_flg = 0 ;
               } else {
                    // 受信バイトはデータ
                    *Rdtp = SSPBUF ;   // データを読込む(ACKはPICが自動的に返す)
                    Rdtp++ ;
                    rcv_flg++ ;
               }
               SSPIF = 0 ;             // 割込みフラグクリア
               SSPCON1bits.CKP = 1 ;   // SCLラインを開放する(通信の再開)
          } else {
               /******* マスターからの読み出し要求(スレーブは送信) *******/
               if (SSPSTATbits.BF == 1) {
                    // アドレス受信後の割り込みと判断する
                    Sdtp = (char *)snd_data ;
                    x = SSPBUF ;       // アドレスデータを空読みする
                    while((SSPCON1bits.CKP)|(SSPSTATbits.BF)) ;
                    SSPBUF = *Sdtp ;             // 送信データのセット
                    Sdtp++ ;
                    SSPIF = 0 ;                  // 割込みフラグクリア
                    SSPCON1bits.CKP = 1 ;        // SCLラインを開放する(通信の再開)
               } else {
                    // データの送信後のACK受け取りによる割り込みと判断する
                    if (SSPCON2bits.ACKSTAT==0) {
                         // マスターからACK応答なら次のデータを送信する
                         while((SSPCON1bits.CKP)|(SSPSTATbits.BF)) ;
                         SSPBUF = *Sdtp ;        // 送信データのセット
                         Sdtp++ ;
                         SSPIF = 0 ;             // 割込みフラグクリア
                         SSPCON1bits.CKP = 1 ;   // SCLラインを開放する(通信の再開)
                    } else {
                         // マスターからはNOACKで応答された時
                         SSPIF = 0 ;             // 割込みフラグクリア
                    }
               }
          }
     }

    /* MSSP(I2C)バス衝突割り込発生時の処理*/
     if (BCLIF == 1) {
          BCLIF = 0 ;   // 今回はフラグのみクリアする(無処理)
     }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//・int I2C_ReceiveCheck() : マスターからの受信状態をチェックする処理                                  *
//・ans : 戻り値が０なら未受信で、受信したら受信したデータの個数を返す
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int I2C_ReceiveCheck()
{
     int ans ;

     ans = 0 ;
     if (rcv_flg != 0) { // 受信したぞぉ
          if ((SSPSTATbits.S==0)&&(SSPSTATbits.P==1)) { // ｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝ発行されたぞぉ
               ans = rcv_flg ;
               rcv_flg = 0 ;
          }
     }
     return(ans) ;
}
#endif