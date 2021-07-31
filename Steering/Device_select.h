/* 
 * File:   Device_select.h
 * Author: evaota
 *
 * Created on 2015/07/11, 10:33
 */

#ifndef DEVICE_SELECT_H
#define	DEVICE_SELECT_H
//P制御各パラメーター
#define Ster1//モーターセレクト定義

#ifdef Ster1//
#define Direction1//方向モード
#define Adrs 0x20//スレーブアドレス
#define Kp 3//比例定数
#define Stee_Center 512//ステアリングのセンター
#define MinDevi 10//目標値と現在値の最小偏差
#define MaxSpeed 700//最大スピード
#define MinSpeed 200//最小スピード
#endif

#ifdef Ster2//
#define Direction1//方向モード
#define Adrs 0x21//スレーブアドレス
#define Kp 3//比例定数
#define Stee_Center 512//ステアリングのセンター
#define MinDevi 10//目標値と現在値の最小偏差
#define MaxSpeed 700//最大スピード
#define MinSpeed 200//最小スピード
#endif

#ifdef Ster3//
#define Direction1//方向モード
#define Adrs 0x22//スレーブアドレス
#define Kp 3//比例定数
#define Stee_Center 512//ステアリングのセンター
#define MinDevi 10//目標値と現在値の最小偏差
#define MaxSpeed 300//最大スピード
#define MinSpeed 0//最小スピード
#endif

#ifdef Ster3//
#define Direction1//方向モード
#define Adrs 0x23//スレーブアドレス
#define Kp 3//比例定数
#define Stee_Center 512//ステアリングのセンター
#define MinDevi 10//目標値と現在値の最小偏差
#define MaxSpeed 300//最大スピード
#define MinSpeed 0//最小スピード
#endif

#endif	/* DEVICE_SELECT_H */

