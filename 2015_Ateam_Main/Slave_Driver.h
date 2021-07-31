/* 
 * File:   Slave_Driver.h
 * Author: evaota
 *
 * Created on 2015/07/01, 22:29
 */
//charcter code is Shift_JIS
 
#ifndef SLAVE_DRIVER_H
#define	SLAVE_DRIVER_H

extern unsigned char phase;//動作カウント変数

////////////////////////////////////////駆動関係////////////////////////////////////
unsigned char Out_Motor(unsigned char ch,unsigned char dire,unsigned int Speed);
//I2C send dataformat
//senddata is 2byte.
//1byte 7~0bit(Speedの上位8bit)
//2byte 7~6bit(Speedの下位2bit) 5~2bit(none) 1~0bit(0~3(dire)のデータ)

unsigned char Dead_Stop(unsigned int Speed);
//I2C send dataformat
//senddata is 2byte.
//1byte 7~0bit(Speedの上位8bit)
//2byte 7~6bit(Speedの下位2bit) 5~2bit(none) 1~0bit(3(brake)のデータ)

unsigned char Set_Str(unsigned char select,int rad);
//I2C send dataformat
//senddata is 2byte.
//1byte 角度データ(0~511)の下位8bit
//2byte 角度データ(0~511)の上位8bit

unsigned char Encorder_Str(unsigned char select,int rad);//指定した角度をステアリングに送信する+2048~-2048
//I2C send dataformat
//senddata is 2byte.
//1byte 角度データ(0~4095)の上位8bit
//2byte 角度データ(0~4095)の下位8bit

////////////////////////////////////////エアー関係/////////////////////////////////////
unsigned char On_Valve(unsigned char select);
//I2C send dataformat
//senddata is 1byte.
//1byte 7~4bit(電磁弁のポート)　3~0bit(電磁弁開放データ(1))

unsigned char Off_Valve(unsigned char select);
//I2C send dataformat
//senddata is 1byte.
//1byte 7~4bit(電磁弁のポート)　3~0bit(電磁弁閉鎖データ(0)) 

///////////////////////////////////////世紀末関係//////////////////////////////////////
unsigned char Seikimatsu_ON(void);
//I2C send dataformat
//senddata is 1byte.
//1byte 7~1bit(none) 0bit(世紀末動作データ(1))

unsigned char Seikimatsu_OFF(void);
//I2C send dataformat
//senddata is 1byte.
//1byte 7~1bit(none) 0bit(世紀末停止データ(0))

///////////////////////////////////////表示器関係/////////////////////////////////
unsigned char System_Indicater(unsigned char Color,unsigned char LedStat,unsigned char Brightness);
//I2C send dataformat
//senddata is 3byte.
//1byte 7~0bit(インジケーターの操作するLEDを選択)
//2byte 7~0bit(1byteで選択したLEDの明るさ(256段階))
//3byte 7~0bit(選択したLEDのオンオフデータ(1bit))

void System_Indicater2(unsigned char data1,unsigned char data2,unsigned char data3,unsigned char data4,unsigned char data5);
//UART send dataformat
//senddata is 1byte.
//1byte 7~5bit(手動ウォッチドッグタイマーデータ) 4~2bit(保持機構の状態データ) 1~0bit(自分側ゾーンのデータ)

///////////////////////////////////保持機構関係////////////////////////////////
unsigned char Set_Lock(unsigned int set);
//I2C send dataformat
//senddata is 2byte.
//1byte 7~0bit(角度データの下位8bit) 
//2byte 7~0bit(角度データの上位8bit)

void Lock_On(void);
//this funcsion don't send data.

void Lock_Acti(void);//motor version
//this funcsion don't send data.

void Side_On(void);
//this funcsion don't send data.

void Side_Acti(void);
//this funcsion don't send data.

void Catap_On(void);
//this funcsion don't send data.

void Catap_Acti(void);
//this funcsion don't send data.

unsigned char Out_Servo(unsigned char port,unsigned char rad);
//I2C send dataformat
//senddata is 2byte.
//1byte 7~0bit(サーボの出力角度)
//2byte 7~0bit(サーボドライバーの操作するポート)

void Shot_On(void);
//this funcsion don't send data.

void Shot_Acti(void);
//this funcsion don't send data.

///////////////////////////////////サイドスロー角度調整機構///////////////////////////////
unsigned char Shot_Stop(void);
//I2C send dataformat
//senddata is 2byte.
//1byte 7~0bit(サイドスロー用Speedの上位8bit)
//2byte 7~6bit(サイドスロー用Speedの下位2bit) 5~2bit(none) 1~0bit(3(brake)のデータ)

unsigned char Shot_Up(void);
//I2C send dataformat
//senddata is byte.
//1byte 7~0bit(サイドスロー用Speedの上位8bit)
//2byte 7~6bit(サイドスロー用Speedの下位2bit) 5~2bit(none) 1~0bit(1(up)のデータ)

unsigned char Shot_Down(void);
//I2C send dataformat
//senddata is byte.
//1byte 7~0bit(サイドスロー用Speedの上位8bit)
//2byte 7~6bit(サイドスロー用Speedの下位2bit) 5~2bit(none) 1~0bit(2(down)のデータ)

#endif	/* SLAVE_DRIVER_H */

