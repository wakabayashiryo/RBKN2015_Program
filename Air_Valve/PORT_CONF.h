/* 
 * File:   PORT_CONF.h
 * Author: evaota
 *
 * Created on 2015/07/04, 14:16
 */

#ifndef PORT_CONF_H
#define	PORT_CONF_H
#define NewDriver

#ifdef BeforeDriver 
//LEDのポート定義
#define BlueLed LATA4
#define RedLed LATA5
#define GreenLed LATA6
#define WhiteLed LATC2

//出力ポートの定義
#define BlueOut LATB4
#define RedOut LATB3
#define GreenOut LATB2
#define WhiteOut LATB1
#endif

#ifdef NewDriver
//LEDのポート定義
#define BlueLed LATA4
#define GreenLed LATA3

//出力ポートの定義
#define YellowOut LATB5
#define BlackOut LATB4
#define BlueOut LATB3
#define RedOut LATB2
#define GreenOut LATB1
#define WhiteOut LATB0
#endif

#endif	/* PORT_CONF_H */

