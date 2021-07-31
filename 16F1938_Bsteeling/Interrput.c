/********************************************

割り込み処理を総括する関数。
この割り込み関数にタイマー0割り込み、状態変化割り込みなどの
関数をぶち込んで使う。

*********************************************/
#include "GV.h"

void interrupt function( void )
{
	Slave_Interrupt();     //I2Cスレーブの割り込み
}