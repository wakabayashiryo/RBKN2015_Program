/****************************************************************************
	Ｉ２Ｃ通信を行うコードのライブラリ
	
	マスター、スレーブともに共有でこちらをSourceファイルに追加してコンパイルしてください。
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

#include "GV.h"                 

unsigned int  rcv_flg ;       // 受信情報(受信データの個数を格納)
unsigned char *Sdtp ;         // 送信データバッファのアドレスポインター
unsigned char *Rdtp ;         // 受信データバッファのアドレスポインター

int AckCheck ;
int success ;

/********************************************************************************
InitI2C_Master  I2C通信のマスターモードで初期化を行う処理

	ans = I2C_Send(adrs,len,buf); スレーブ(adrs)に指定した個数(len)だけデータ(buf)を送信する処理.その後ansにスレーブの応答を代入
	    
	    adrs : スレーブのアドレスを指定する(8～119を推奨)                        
	   	len  : 送信するデータ(バイト)の個数を指定する
		buf  : 送信するデータを格納した配列を指定する   
	  	ans  : 0=正常(送信完了)　1=スレーブが返答しない　2=相手がデータの受信を拒否した                   

		buf[0] : 今回は送信するデータとして使用
		*buf :　受信したデータ
*******************************************************************************/
void InitI2C_Master()
{
     SSPSTAT= 0b10000000 ;   // 標準速度モードに設定する(100kHz)
     SSPCON1= 0b00101000 ;   // SDA/SCLピンはI2Cで使用し、マスターモードとする
     SSPADD = 0x13       ;   // クロック=FOSC/((SSPADD + 1)*4) 8MHz/((0x13+1)*4)=0.1(100KHz)
     SSPIE = 1 ;             // SSP(I2C)割り込みを許可する
     BCLIE = 1 ;             // MSSP(I2C)バス衝突割り込みを許可する
     PEIE   = 1 ;             // 周辺装置割り込みを許可する
     GIE    = 1 ;             // 全割り込み処理を許可する 
     SSPIF = 0 ;             // SSP(I2C)割り込みフラグをクリアする
     BCLIF = 0 ;             // MSSP(I2C)バス衝突割り込みフラグをクリアする
     TRISC   = TRISC | 0b00011000 ;  // TRISCのRC3、RC4を入力にしておく
}

/******************************

    　　　Master用割り込み処理

******************************/
void Master_Interrupt( void )
{
     if (SSPIF == 1) {       // SSP(I2C)割り込みが発生したか判断
	     if (AckCheck == 1) AckCheck = 0 ;
          SSPIF = 0 ;        // フラグクリア
     }
     else if (BCLIF == 1) {       // MSSP(I2C)バス衝突割り込みが発生したか判断
          BCLIF = 0 ;        // 今回はフラグのみクリアする(無処理)
     } 
}


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
               SSPCON2bits.RCEN = 1 ;  // 受信を許可する
               I2C_IdleCheck(0x4) ;
               *(buf+i) = SSPBUF ;         // 受信
               //data = SSPBUF; //受信a
               //buf++ ;
               I2C_IdleCheck(0x5) ;
               if (i==len-1) SSPCON2bits.ACKDT = 1 ; // ACKデータはNOACK
               else       SSPCON2bits.ACKDT = 0 ; // ACKデータはACK
               SSPCON2bits.ACKEN = 1 ;            // ACKデータを返す
          }
     }
     // ストップ(STOP CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.PEN = 1 ;
     return ans ;
}

int I2C_rStart_Receive(unsigned char adrs,unsigned char regist,unsigned char *buf)
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
     
     //レジスタアドレス送信
     if (ans == 0) {
     	I2C_IdleCheck(0x5) ;
        AckCheck = 1 ;
        SSPBUF = regist ;        // データを送信
       	while (AckCheck) ;       // 相手からのACK返答を待つ
        ans = SSPCON2bits.ACKSTAT ;
        if (ans != 0) {
        	ans = 2 ;                // 相手がNOACKを返した
        	return 2; 
        }	    
	 } 
	 
	 // リピート・スタート(REPEATED START CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.RSEN = 1 ;
     // [スレーブのアドレス]を送信する
     I2C_IdleCheck(0x5) ;
     AckCheck = 1 ;
     SSPBUF = (char)((adrs<<1)+1) ;   // アドレス + Rを送信
     while (AckCheck) ;                 // 相手からのACK返答を待つ
     ans = SSPCON2bits.ACKSTAT ;

	if(ans == 0){
		I2C_IdleCheck(0x5) ;
        SSPCON2bits.RCEN = 1 ;  // 受信を許可する
        I2C_IdleCheck(0x4) ;
        *(buf) = SSPBUF ;         // 受信
        I2C_IdleCheck(0x5) ;
        SSPCON2bits.ACKDT = 1 ; // ACKデータはNOACK
        SSPCON2bits.ACKEN = 1 ; // ACKデータを返す
	}	
	
     // ストップ(STOP CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.PEN = 1 ;
     return ans ;	
}	

/* I2C_IdleCheck(char mask) : アイドル状態のチェック	*/
// ACKEN RCEN PEN RSEN SEN R/W BF が全て０ならＯＫ

/* InitI2C_Master() : Ｉ２Ｃ通信のマスターモードで初期化を行う処理*/                              
// 注)CPUｸﾛｯｸ(FOSC)8MHzでの設定、他のｸﾛｯｸはSSP1ADDを変更する必要有り

/* 
	I2C_Send(adrs,len,buf) : スレーブにデータを指定した個数(len)だけ送信し、int型で返す                       
                                                                              
    adrs : スレーブのアドレスを指定する(8～119を推奨)                         
    len  : 送信するデータ(バイト)の個数を指定する                            
    buf  : 送信するデータを格納した配列を指定する        
*/

/*
	int I2C_Receive(adrs,len,buf) : スレーブからデータを指定した個数(len)だけ受信し、値をint型で返す
                                                                              
    data : 受信データ保管用変数
*/

void I2C_IdleCheck(char mask)
{
     while (( SSPCON2 & 0x1F ) | (SSPSTAT & mask)) ;
}

/*******************************************************************************
	InitI2C_Slave(address): Ｉ２Ｃ通信のスレーブモードで初期化を行う処理
							一括呼び出しの同報通知は無効の設定となっています。                       *
	address	: スレーブのマイアドレスを設定する(８～１１９を推奨)              
*******************************************************************************/
void InitI2C_Slave(int address)
{
     SSPSTAT= 0b10000000 ;   // 標準速度モードに設定する(100kHz)
     SSPCON1= 0b00100110 ;   // SDA/SCLピンはI2Cで使用し、スレーブモードとする
     SSPCON2bits.SEN  = 1;   // SCL制御(クロックストレッチ)を行う
//   SSPCON2bits.GCEN = 1;   // 一括呼び出しの同報通知を有効にする
     SSPADD = address << 1 ;   // マイアドレスの設定
     SSPMSK = 0b11111110 ;   // アドレス比較用マスクデータ
     SSPIE = 1 ;             // SSP(I2C)割り込みを許可する
     BCLIE = 1 ;             // MSSP(I2C)バス衝突割り込みを許可する
     PEIE   = 1 ;             // 周辺装置割り込みを許可する
     GIE    = 1 ;             // 全割り込み処理を許可する 
     SSPIF = 0 ;             // SSP(I2C)割り込みフラグをクリアする
     BCLIF = 0 ;             // MSSP(I2C)バス衝突割り込みフラグをクリアする
     TRISC   = TRISC | 0b00011000 ;  // TRISCのRC3、RC4を入力にしておく
     rcv_flg = 0 ;
}

/**************************************

　　　　　　　　Slave用割り込み処理関数

**************************************/
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

/*******************************************************************************
	ans = I2C_ReceiveCheck() : マスターからの受信状態をチェックする処理                                  *
	ans : 戻り値が０なら未受信で、受信したら受信したデータの個数を返す       
*******************************************************************************/
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