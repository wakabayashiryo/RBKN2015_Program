#include <htc.h>//this program was compailed by XC8,because compailer outputed warning when Hitech-c was compailed.
#include "I2C.h"
#include "main.h"
#include "RotaryEncoder.h"

unsigned int limcount = 0;
unsigned char setdata[4] = {100,100,100,100};//Middle

void main (void){
  unsigned char ans;
  int i1,i2;

  pic_init();
  I2C_init(ADDRES);
  Tmr2_4_6InterruptInit(6,70000,Pwm_Interrupt);

/*  while(1){
      for(i2=0;i2<180;i2++){
         for(i1=0;i1<4;i1++){
            setAngle(i2,i1);
         }
         __delay_ms(10);
      }
  }*/

  while(1){
      ans = I2C_ReceiveCheck();
      if(ans){
          setAngle(rcv_data[1],rcv_data[0]);
      }
  }
}

void setAngle(unsigned char angle,unsigned char port){
    setdata[port]=angle*7/9+44;
}

void interrupt servo(void){
    interrupt_Tmr2_4_6();
    Slave_Interrupt();
}

void pic_init(void){
  OSCCON = 0xF0;  //internal clock 32MHz//

  TRISA = 0x00;
  TRISB = 0x00;
  TRISC = 0b00011000;
  TRISE = 0x00;

  ANSELA = 0x00;
  ANSELB = 0x00;

  PORTA = 0x00;
  PORTB = 0x00;
  PORTC = 0x00;
  PORTE = 0x00;
}

void Pwm_Interrupt(void){
    static unsigned char cnt;
    static bit flg1,flg2,flg3,flg4,flg0;

  //Ver2.1(b)
  if(flg0||cnt == 250){
      flg0=1;
      flg1=0;
      flg2=0;
      flg3=0;
      flg4=0;
    limcount++;
    if(limcount == (1300-250)){
        limcount = 0;
        cnt=0;
        flg0=0;
    }
    return;
  }

  if(flg1||cnt==setdata[0]){
     servo1 = 0;
     flg1=1;
  }
  else
      servo1 = 1;
  if(flg2||cnt==setdata[1]){
      servo2 = 0;
      flg2=1;
  }
   else
      servo2 = 1;
  if(flg3||cnt==setdata[2]){
      servo3 = 0;
      flg3=1;
  }
  else
      servo3 = 1;
  if(flg4||cnt==setdata[3]){
      servo4 = 0;
      flg4=1;
  }
  else
      servo4 = 1;
  cnt++;
}