/* 
 * File:   RTLY_ENC.h
 * Author: oishi
 *
 * Created on 2015/07/04, 12:00
 */

#ifndef RTLY_ENC_H
#define	RTLY_ENC_H

#include <htc.h>

#define ENCODER_CS LATAbits.LATA0
#define ENCODER_CK LATAbits.LATA1
#define ENCODER_DO RC0

#define ENCODER_CS_TRIS TRISAbits.TRISA0
#define ENCODER_CK_TRIS TRISAbits.TRISA1
#define ENCODER_DO_TRIS TRISCbits.TRISC0


#ifndef USETMR2
#ifndef USETMR4
#ifndef USETMR6
#define NON_TMR_2_4_6
#endif
#endif
#endif

void init_RTLY_ENC(void);
unsigned int getData(void);
void interrupt_Tmr2_4_6(void);

#endif	/* RTLY_ENC_H */