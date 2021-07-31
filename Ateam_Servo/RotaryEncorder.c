#include <htc.h>
#include "RotaryEncoder.h"

#ifdef USETMR2
void (*TMR2_interrupt_fun)();
#endif
#ifdef USETMR4
void (*TMR4_interrupt_fun)();
#endif
#ifdef USETMR6
void (*TMR6_interrupt_fun)();
#endif

unsigned int SamplingTimes;
int RotaryEncoder_n;
int RotaryEncoder_ans;

void RotaryEncorderMainEvent(void);

int GetRotaryEncorderData(void){
    if(SamplingTimes==0){
        RotaryEncoder_n=0;
    }
    return RotaryEncoder_ans;
}

void RotaryEncorderInit(unsigned char tmr,unsigned char khz,unsigned char Mode,unsigned int n){

    ENCODER_A_TRIS=1;
    ENCODER_B_TRIS=1;

    if(Mode){
        SamplingTimes=n;
    }

    Tmr2_4_6InterruptInit(tmr,khz,RotaryEncorderMainEvent);

}

void RotaryEncorderMainEvent(void){
    static const int dir[] = { 0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0 }; /* 回転方向テーブル */
    static int i;                           /* インデックス */
    signed char n;
    static int Times;

    if(Times==0){
        RotaryEncoder_ans=RotaryEncoder_n;
    }
    else if(Times==SamplingTimes){
        RotaryEncoder_ans=RotaryEncoder_n;
        RotaryEncoder_n=0;
        Times=0;
    }

    i = (i << 2) + (((ENCODER_B)+(ENCODER_A)+(ENCODER_A)));   /* 前回値と今回値でインデックスとする */
    n = dir[i & 15];                    /* 変化パターンから動きを得る */
    if (n) {                            /* 動きがあったら位置更新 */
        RotaryEncoder_n += n;
    }
    Times++;
}

void interrupt_Tmr2_4_6(void){
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


void _Tmr2_4_6InterruptInit(unsigned char n,unsigned char speed,unsigned char config,void (*func)()){
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
            T2CON=config;     //4：1プリスケーラ
            PR2=speed;      //速度設定
            TMR2_interrupt_fun=func;
            TMR2IE=1;       //割り込み有効化
            TMR2IF=0;       //割り込みフラグリセット
            break;
#endif
#ifdef USETMR4
        case 4:
            T4CON=config;
            PR4=speed;
            TMR4_interrupt_fun=func;
            TMR4IE=1;
            TMR4IF=0;
            break;
#endif
#ifdef USETMR6
        case 6:
            T6CON=config;
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