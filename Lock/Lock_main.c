#include<htc.h>//pic16f1938���g�p��//
#include"ECCP.h"
#include"I2C.h"
#include"ADC.h"
#include"Device_select.h"
//character code Shift_JIS

#define _XTAL_FREQ 32000000//�V�X�e���N���b�N8MHz*4(PLL)
__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_OFF&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_OFF&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_OFF&STVREN_OFF&LVP_OFF);
__IDLOC(1056);

void pic_init(void);//�V�X�e�������ݒ�֐�

void main (void)//���C���֐�
{
    unsigned int M,duty;//���݂̒l�p�ϐ�,�o�̓X�s�[�h�p�ϐ�
    unsigned char dire,stat;//�����p�ϐ�,I2C��M�X�e�[�^�X�p�ϐ�
    int SET,Devi,Sti_Stat;//�ڕW�l�A�΍�(deviation)�A�ڕW�l�̃f�[�^

    pic_init();//�V�X�e�������ݒ�
    adc_init();//AD�R���o�[�^�����ݒ�
    I2C_init(Adrs);//I2C�����ݒ�
    pwm_init();//ECCP�����ݒ�
    motor_ctrl(0,0);//free mode
    while(!I2C_ReceiveCheck());//I2C���`�F�b�N(��M�ł���܂ő҂�)

    while(1)//�������[�v
    {
        stat = I2C_ReceiveCheck();//��M�m�F

        if(stat==2)//rcv_data[0] = ���ʃr�b�g
        {           //rcv_data[1]=��ʃr�b�g
            CLRWDT();//�E�H�b�`�h�b�O�^�C�}�[���N���A�[
            Sti_Stat=((rcv_data[1]<<8)+rcv_data[0]);//�ڕW�l�̃f�[�^���擾
            SET = Stee_Center - Sti_Stat;//�ڕW�l���v�Z
        }
        
		M = Samp_AD(0);//�|�e���V�������[�^�̌��݈ʒu������
        Devi = SET - M;//�ڕW�l�ƌ��݈ʒu�̕΍����v�Z

      if(Devi<-MinDevi)//�΍���0���Ⴂ�ꍇ�i-devi�j
	  {
#ifdef Direction1
	    dire = 1;//�����P
#endif
#ifdef Direction2
	    dire = 2;//�����Q
#endif
        duty = MinSpeed;//�ŏ��X�s�[�h���� 
        duty |= Kp*Devi;//�΍�����X�s�[�h�v�Z���ĉ��Z
	  }
      else if(Devi>MinDevi)//�΍���0��葽��������i+devi�j
	  {
#ifdef Direction1
        dire = 2;//�����Q
#endif	  
#ifdef Direction2
        dire = 1;//����1
#endif  
        duty = MinSpeed;//�ŏ��X�s�[�h����
        duty |= Kp*Devi;//�΍�����X�s�[�h�v�Z���ĉ��Z
	  }
	  else//�ڕW�l�̏ꍇ
	  {
		  dire = 3;//�u���[�L��������
		  duty = MaxSpeed;
	  }

	if(duty>MaxSpeed)duty = MaxSpeed;//���[�^�[�o�͂��ő�X�s�[�h�������ꍇ�}���� 

	motor_ctrl(duty,dire);//���[�^�[�ɏo��
    }
}

void pic_init(void)//�f�o�C�X�����ݒ�֐�
{
    OSCCON = 0xf0;  //internal clock 32MHz//
    TRISA = 0x01;   //input mode RA0//
    TRISB = 0x00;
    TRISC = 0b00011000;
    TRISE = 0x00;

    ANSELA = 0x01;
    ANSELB = 0x00;

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTE = 0x00;
    
    WDTCON = 0x11;//�E�H�b�`�h�b�O�^�C�}�[�����ݒ�
}

void interrupt comu(void)//���荞�ݏ����֐�
{
    Slave_Interrupt();//Slave�p���荞�ݏ����֐�
}