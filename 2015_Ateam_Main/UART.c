#include "pic.h"
#include "UART.h"



#define _XTAL_FREQ 32000000
#define UART_BUFFER_SIZE   128// ＵＳＡＲＴの受信バッファサイズ

char UART_Buffer[UART_BUFFER_SIZE] ;    // 受信したデータを格納するバッファ
int  UART_Buffer_inptr ;                // 受信バッファの読み込みポインター
int  UART_Buffer_outptr ;               // 受信バッファの書き込みポインター


/*******************************************************************************
*  InterUART( void )                                                           *
*    ＵＳＡＲＴ関連の割り込み処理                                              *
*     この関数は受信を行う場合はメインプログラムの割込み関数で必ず呼びます     *
*******************************************************************************/
void InterUART( void )
{
     int  x ;
     char dt ;

     if (RCIF == 1) {          // 割込みはＵＳＡＲＴ通信の受信か？
          // レジスタからデータを受信
          dt = RCREG ;
          x = (UART_Buffer_outptr + 1) % UART_BUFFER_SIZE ;
          if (x != UART_Buffer_inptr) {
               // バッファが満杯でないならデータを保存する
               UART_Buffer[UART_Buffer_outptr] = dt ;
               // バッファ書き込みポインターを次に進める
               UART_Buffer_outptr = x ;
          }

          // オーバランエラー処理(RCREGバッファが３個目のデータを受信したらオーバフロー)
          if (OERR == 1) {
               CREN = 0 ;
               while( OERR ) ;
               CREN = 1 ;
          }

          // 割込み受信フラグをリセット
          RCIF = 0 ;
     }
}
/*******************************************************************************
*  InitUART(rx,tx,brg)                                                         *
*    ＵＳＡＲＴ通信の初期化を行う処理                                          *
*    rx  : 受信(RX)するピンの番号を指定する                                    *
*    tx  : 送信(TX)するピンの番号を指定する                                    *
*    brg : ボーレートジェネレータの値を指定します(通信速度になります)          *
*******************************************************************************/
void InitUART(int rx, int tx, unsigned int brg)
{

     TXSTA  = 0b00100100 ;         // 送信情報設定：非同期モード　８ビット・ノンパリティ
     RCSTA  = 0b10010000 ;         // 受信情報設定
     BAUDCON= 0b00100100;
     SPBRG  = brg ;                // ボーレートの設定
     RCIF   = 0 ;                  // USART割込み受信フラグの初期化
     RCIE   = 1 ;                  // USART割込み受信を有効にする
     PEIE   = 1 ;                  // 周辺装置割込みを有効にする
     GIE    = 1 ;                  // 全割込み処理を許可する
     UART_Buffer_inptr  = 0 ;      // 受信バッファの読み込みポインターの初期化
     UART_Buffer_outptr = 0 ;      // 受信バッファの書き込みポインターの初期化
}
/*******************************************************************************
*  UART_Send(dt,num)                                                           *
*    相手に指定した個数のデータを送信する処理                                  *
*                                                                              *
*    dt  : 送信するデータを格納した配列を指定します。                          *
*    num : 送信するデータの個数を指定します。                                  *
*******************************************************************************/
void UART_Send(const char *dt)
{
     while(1){
          while(TXIF==0) ;    // 送信可能になるまで待つ
          TXREG = *dt ; // 送信する
          if(*dt==0)break;
          *dt++;
     }
}
/*******************************************************************************
*  UART_Write(dt)                                                              *
*    相手に１バイトのデータを送信する処理                                      *
*                                                                              *
*    dt  : 送信するデータを指定します。                                        *
*******************************************************************************/
void UART_Write(char dt)
{
     while(TXIF==0) ;         // 送信可能になるまで待つ
     TXREG = dt ;              // 送信する
}
/*******************************************************************************
*  ans = UART_Available()                                                      *
*    受信したデータの個数を返す処理                                            *
*                                                                              *
*    ans : 受信したデータの個数を返します。                                    *
*******************************************************************************/
int UART_Available()
{
     return (UART_Buffer_outptr + UART_BUFFER_SIZE - UART_Buffer_inptr) % UART_BUFFER_SIZE ;
}
/*******************************************************************************
*  ans = UART_Read()                                                           *
*    受信したデータを１バイト読み込む処理                                      *
*                                                                              *
*    ans : 受信したデータを返します  0xffffを返したら受信データは空です。      *
*******************************************************************************/
unsigned int UART_Read()
{
     unsigned int  ans ;

     // バッファは空である
     if (UART_Buffer_inptr == UART_Buffer_outptr) return 0xffff;
     // バッファから読み出す
     ans = UART_Buffer[UART_Buffer_inptr] ;
     // バッファ読み込みポインターを次に進める
     UART_Buffer_inptr = (UART_Buffer_inptr + 1) % UART_BUFFER_SIZE ;

     return ans ;
}

int UART_READ_str(char *str){
    int a = 0;
    do{
        str[a]=UART_Buffer[UART_Buffer_inptr];
        str[0]=UART_Buffer[UART_Buffer_inptr];
        a++;
        UART_Buffer_inptr = (UART_Buffer_inptr + 1)%UART_BUFFER_SIZE;
    }while(UART_Buffer_inptr != UART_Buffer_outptr&&a<UART_BUFFER_SIZE);

}

unsigned int UART_READ_without_COUNT(void){
     unsigned int  ans ;
     // バッファは空である
     if (UART_Buffer_inptr == UART_Buffer_outptr) return 0xffff;
     // バッファから読み出す
     ans = UART_Buffer[(UART_Buffer_outptr - 1 + UART_BUFFER_SIZE) % UART_BUFFER_SIZE ] ;
     return ans ;
}
/*******************************************************************************
*  UART_Flush()                                                                *
*    受信バッファをクリアする処理                                              *
*******************************************************************************/
void UART_Flush()
{
     UART_Buffer_inptr  = 0 ;      // 受信バッファの読み込みポインターの初期化
     UART_Buffer_outptr = 0 ;      // 受信バッファの書き込みポインターの初期化
}