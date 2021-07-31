/* 
 * File:   Data_Format.h
 * Author: evaota
 *
 * Created on 2015/07/30, 22:36
 */

#ifndef CTRL_DATAFORMAT_H
#define	CTRL_DATAFORMAT_H

#define Adrs 0x70
#define Pwm_Red 2
#define Pwm_Green 1
#define Pwm_Blue 3
/////////////////////////PortData/////////////////////////
//---------------------------------------------------------
//|                                                       |
//|   ------------                                        |
//|  | pic16f1938 | B1 G1 R1 B2 G2 R2 B3 G3 R3 B4 G4 R4   |
//|   ------------                                        |
//|                                                       |
//---------------------------------------------------------
//////////////////////ColorNumber/////////////////////
#define All_Clear 0

#define Blue_Num1   1
#define Green_Num1  2
#define Red_Num1    3

#define Blue_Num2   4
#define Green_Num2  5
#define Red_Num2    6

#define Blue_Num3   7
#define Green_Num3  8
#define Red_Num3    9

#define Blue_Num4   10
#define Green_Num4  11
#define Red_Num4    12
/////////////////////////LedPort//////////////////////
#define Blue1   LATB1
#define Green1  LATB0
#define Red1    LATB2

#define Blue2   LATA0
#define Green2  LATA1
#define Red2    LATA2

#define Blue3   LATA3
#define Green3  LATA4
#define Red3    LATA5

#define Blue4   LATA7
#define Green4  LATA6
#define Red4    LATC0

#endif