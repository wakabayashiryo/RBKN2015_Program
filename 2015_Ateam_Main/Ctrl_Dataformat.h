/* 
 * File:   Ctrl_Dataformat.h
 * Author: evaota
 *
 * Created on 2015/06/30, 23:55
 */

#ifndef CTRL_DATAFORMAT_H
#define	CTRL_DATAFORMAT_H


#define Left    0x08
#define Down	0x04
#define Up      0x01
#define Right   0x02

#define Circle  0x02
#define Tri     0x01
#define Sq      0x08
#define Cross	0x04

#define L1	0x08
#define L2	0x04
#define R1	0x02
#define R2	0x01

#define None 0x00
#define Zyuzi	0x01
#define Marutoka	0x02
#define LR	0x03
#define LY	0x04
#define LX	0x05
#define RY	0x06
#define RX	0x07

#define StopBorder 4
//////////////////MotorSpeed///////////////////////
#define MaxSpeed 700

#define ASterSpeed 400
#define BSterSpeed 400

#define Shot_Rad_Speed 300
///////////////////Driver_palamater/////////////////
#define ASteer 512
#define BSteer 512

#define ATurnDecel 0.75f
#define BTurnDecel 0.75f

#define AStePala 64.0f
#define BStePala 64.0f

#define SideOnTime 12000
#define CtpOnTime 6000

#define LockOnTime1 18000
#define LockOnTime2 8000
#define LockOnTime3 4000 
#define LockOnTime4 2000 

#define LockOffTime 40000

//////////////////MoterStatus///////////////////////
#define Free 0
#define Forward 1
#define Reversal 2
#define Brake 3
//////////////////SteerName///////////////////////
#define A_Ster 0
#define B_Ster 1
#define C_Ster 2
#define D_Ster 3
//////////////////DriveMotorName/////////////////
#define A_Drv 0
#define B_Drv 1
#define C_Drv 2
#define D_Drv 3
//////////////////Other///////////////////////
#define TurnInterval 100
#define SteerOffset 100
#define A_Ster_Acce 10
#define B_Ster_Acce 10

#define SterVal -180//!< ポテンションメーター式ステア
#define Radnum 4//!<送信するサーボの角度の個数

#define SW_SampTime 500 
//////////////////SlaveAdress///////////////////////
#define MotrAdrs1 0x10//ADriver
#define MotrAdrs2 0x11//BDriver
#define MotrAdrs3 0x12//undecided colar
#define MotrAdrs4 0x13//undecided calar
#define Shot_Rad 0x14

#define ServoAdrs1 0x40//undecided

#define AirValve 0x30//decided

#define SteeAdrs1 0x20//ASteer
#define SteeAdrs2 0x21//BSteer
#define SteeAdrs3 0x22
#define SteeAdrs4 0x23

#define LockAdrs 0x50

#define SeikimatsuAdrs 0x60

#define SysIndi_Adrs 0x70
//////////////////Servo///////////////////////

/////////////////////external_indicater(motordriver)////////////////
#define GreenLed LATA3
#define BlueLed LATA4
#define ZoneBlue LATB3
#define ZoneRed LATB5

#endif	/* CTRL_DATAFORMAT_H */

