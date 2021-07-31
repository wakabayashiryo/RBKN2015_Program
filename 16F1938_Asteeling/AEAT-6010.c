#include <htc.h>
#include "GV.h"                 

#define _XTAL_FREQ 32000000

#define CS	LATA0
#define CK	LATA1
#define DAT	RC0

#define LED_Green LATA3
#define LED_Blue LATA4


//0x20 steeling left
__EEPROM_DATA(0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00);

unsigned char i;
unsigned int pos;


void PWMInit(void);
void PWMConfig(char turn,char pwm);
unsigned int AEAT601x();

__CONFIG(
	FOSC_INTOSC&
	WDTE_OFF&
	PWRTE_ON&
	MCLRE_OFF&
	CP_OFF&
	CPD_OFF&
	BOREN_OFF&
	CLKOUTEN_OFF&
	IESO_OFF&
	FCMEN_OFF
	);
__CONFIG(
	WRT_OFF&
	VCAPEN_OFF&
	PLLEN_ON&
	STVREN_OFF&
	BORV_19&
	LVP_OFF
	);



void main(void){
	OSCCON = 0b11110000;  //32MHz
	__delay_ms(10);
	ANSELA = 0x00;
	ANSELB = 0x00;
	CM1CON0 = 0x00;
	CM1CON1 = 0x03;

    TRISA = 0b11100100;
    TRISC = 0b11111111;

    //

    ADCON0 = 0b00001000;
    ADCON1 = 0b10100000;

	//

    unsigned char ans = 0;

//    unsigned int ref = 127;
    unsigned int ref = 2047;
    unsigned int diff = 0;
    unsigned char dir = 0;
    unsigned char tmp = 0;
    unsigned char duty = 0;

    unsigned int offset = 127;

    pos = AEAT601x();    //��ǂ�
    __delay_ms(1);
    offset = (4096 - AEAT601x());

    //

    LED_Green = 1;
    LED_Blue = 1;
    __delay_ms(100);

    LED_Green = 0;
    LED_Blue = 0;
    __delay_ms(100);

    LED_Green = 1;
    LED_Blue = 1;
    __delay_ms(100);

    LED_Green = 0;
    LED_Blue = 0;

    PWMInit();
    PWMConfig(0x00,0x00);

    InitI2C_Slave(EEPROM_READ(0x00));

    while(1){
      ans = I2C_ReceiveCheck();
      if(ans != 0){  //��M����
        ref = (rcv_data[0]<<8) + rcv_data[1];
      }
      else{
         //��M�G���[
      }	


      pos = AEAT601x();

//�N�����̊p�x��2048�Ƃ��čl����ϊ�
      pos = ((pos + offset + 2048) & 0x0FFF);

//��]������duty
      if(pos < ref){
        diff = ref - pos;
        dir = 0x01;
      }
      else if(ref < pos){
        diff = pos - ref;
        dir = 0x02;
      }
      else{
        dir = 0x03;
        diff = 0xFF;
      }

//�Q�C������
      diff = (diff >> 2) + (duty >> 2);

      if(diff > 0xFF){
        duty = 0xFF;
      }
      else{
        duty = diff;
      }


//��
      PWMConfig(dir,duty);

//LED�ŉ�]�����\��
      if(dir == 0x01){
        LED_Green = 1;
        LED_Blue = 0;
      }
      else if(dir == 0x02){
        LED_Green = 0;
        LED_Blue = 1;
      }
      else{
        LED_Green = 1;
        LED_Blue = 1;
      }

      __delay_us(500);
   }
}

unsigned int AEAT601x(){
//6010��4092�܂łS�ޯč���
//6012��4095�܂łP�r�b�g����

  unsigned int law = 0;

  CS = 0;
  __delay_us(1);
  CK = 0;
  __delay_us(1);

  for(i=0;i<12;i++){
    CK = 1;
    __delay_us(1);
    law = (law << 1);
    law += DAT;
    CK = 0;
    __delay_us(1);
  }

  CS = 1;

  return law;

}

void PWMInit(void){
//����ňꉞPWM���g����E�E�E�΂��B

	CCPTMRS0 = (CCPTMRS0&0b11111100);
	PR2 = 0xFF;
	T2CON	= 0b00000110;
	CCP1CON = 0b01001100;

	PWM1CON = 0b00001111;
	PSTR1CON = 0b00001111;
	TRISB = (TRISB & 0b111101001);
	TRISC = (TRISC & 0b111111011);
}

void PWMConfig(char turn,char pwm){
//turn��bit0����2�r�b�g����]�����Ƃ�
//00:free
//01:forward
//10:reverse
//11:PWM-brake

//turn�̏��2�r�b�g��PWM�̉���2�r�b�g�B
//pwm��pwm�̏��8�r�b�g�B

	if((turn&0x03)==0x03){
		CCP1CON = 0b01000000;
		RC2 = 0;//a
		RB2 = 0;//b
		RB4 = 0;//d
		RB1 = 0;//c
		RB2 = 1;//b
		RB4 = 1;//d
		CCPR1L = 0x00;
	}
	else if(turn&0x01){
		CCP1CON = 0b01001100;
		CCP1CON = (CCP1CON|((turn&0xC0)>>2));
		CCPR1L = pwm;
	}
	else if(turn&0x02){
		CCP1CON = 0b11001100;
		CCP1CON = (CCP1CON|((turn&0xC0)>>2));
		CCPR1L = pwm;
	}
	else{
		CCP1CON = 0b01000000;
		RC2 = 0;//a
		RB2 = 0;//b
		RB1 = 0;//c
		RB4 = 0;//d
		CCPR1L = 0x00;
	}
}
