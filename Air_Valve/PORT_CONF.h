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
//LED�̃|�[�g��`
#define BlueLed LATA4
#define RedLed LATA5
#define GreenLed LATA6
#define WhiteLed LATC2

//�o�̓|�[�g�̒�`
#define BlueOut LATB4
#define RedOut LATB3
#define GreenOut LATB2
#define WhiteOut LATB1
#endif

#ifdef NewDriver
//LED�̃|�[�g��`
#define BlueLed LATA4
#define GreenLed LATA3

//�o�̓|�[�g�̒�`
#define YellowOut LATB5
#define BlackOut LATB4
#define BlueOut LATB3
#define RedOut LATB2
#define GreenOut LATB1
#define WhiteOut LATB0
#endif

#endif	/* PORT_CONF_H */

