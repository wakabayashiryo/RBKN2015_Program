#include <htc.h>
#include "RTLY_ENC.h"
#define _XTAL_FREQ 32000000

void init_RTLY_ENC(void)
{
    ENCODER_CS_TRIS =   0;
    ENCODER_CK_TRIS =   0;
    ENCODER_DO_TRIS =   1;
    ENCODER_CS  =   1;
    ENCODER_CK  =   1;
    __delay_ms(50);
}

unsigned int getData(void)
{
    unsigned int ans = 0;
    ENCODER_CS = 0;
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
    ENCODER_CK = 0;
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
    ENCODER_CK = 1;
        NOP();
        NOP();
        NOP();
    for(unsigned char i=0;i<10;i++){
        ans|=ENCODER_DO<<(9-i);
        if(i!=9)ENCODER_CK = 0;
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        ENCODER_CK = 1;
        NOP();
        NOP();
        NOP();
    }
    ENCODER_CS = 1;
    return ans;
}

#ifdef USETMR2
void (*TMR2_interrupt_fun)();
#endif
#ifdef USETMR4
void (*TMR4_interrupt_fun)();
#endif
#ifdef USETMR6
void (*TMR6_interrupt_fun)();
#endif

void interrupt_Tmr2_4_6(void)
{
#ifdef USETMR2
    if(TMR2IF){
        TMR2_interrupt_fun();
        TMR2IF=0;
    }
#endif
#ifdef USETMR4
#ifdef USETMR2
    else
#endif
        if(TMR4IF){
        TMR4_interrupt_fun();
        TMR4IF=0;
    }
#endif
#ifdef USETMR6
#ifdef USETMR2
     else
#endif
#ifndef USETMR2
#ifdef USETMR4
     else
#endif
#endif
         if(TMR6IF){
            TMR6_interrupt_fun();
            TMR6IF=0;
        }
#endif
}

#ifndef NON_TMR_2_4_6
void _Tmr2_4_6InterruptInit(unsigned char n,unsigned char speed,void (*func)()){
    if(!(
#ifdef USETMR2
            n==2
#endif
#ifdef USETMR4
#ifdef USETMR2
            ||
#endif
            n==4
#endif
#ifdef USETMR6
#ifdef USETMR2
            ||
#endif
#ifndef USETMR2
#ifdef USETMR4
            ||
#endif
#endif
            n==6
#endif
            ))
        return;

    switch(n){
#ifdef USETMR2
        case 2:
            T2CON=0x05;     //4：1プリスケーラ
            PR2=speed;      //速度設定
            TMR2_interrupt_fun=func;
            TMR2IE=1;       //割り込み有効化
            TMR2IF=0;       //割り込みフラグリセット
            break;
#endif
#ifdef USETMR4
        case 4:
            T4CON=0x05;
            PR4=speed;
            TMR4_interrupt_fun=func;
            TMR4IE=1;
            TMR4IF=0;
            break;
#endif
#ifdef USETMR6
        case 6:
            T6CON=0x05;
            PR6=speed;
            TMR6_interrupt_fun=func;
            TMR6IE=1;           //割り込み有効化
            TMR6IF=0;           //割り込みフラグリセット
            break;
#endif
   }
   PEIE   = 1 ;                  // 周辺装置割込みを有効にする
   GIE    = 1 ;                  // 全割込み処理を許可する
}
#endif