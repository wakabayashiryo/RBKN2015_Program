#include<htc.h>//pic16f1938//
#include<stdlib.h>
#include"ECCP.h"
#include"I2C.h"
#include"Device_select.h"
#include"RTLY_ENC.h"
#include"Steer_Fun.h"
//character code Shift_JIS

#define eepadrs 0//EEPROM�A�N�Z�X�A�h���X
#define _XTAL_FREQ 32000000//�V�X�e���N���b�N8MHz*4(PLL)

__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_OFF&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_OFF&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_OFF&STVREN_OFF&LVP_OFF);
__IDLOC(1056);
__EEPROM_DATA(0,0,0,0,0,0,0,0);//EEPROM������

void main (void)//���C���֐�
{
    unsigned int M,duty;//���݂̒l�p�ϐ�,�o�̓X�s�[�h�p�ϐ�   
    unsigned char dire,stat;//�����p�ϐ�,I2C��M�X�e�[�^�X�p�ϐ�
    int SET,Devi;//�ڕW�l�A�΍�(deviation)    
    static bit ZeroFlag;//�����V�[�N�G���X�t���O

    pic_init();//�V�X�e�������ݒ�
    I2C_init(Adrs);//I2C�����ݒ�
    pwm_init();//ECCP�����ݒ�
    init_RTLY_ENC();//���C���G���R�[�_�[�p�֐������ݒ�

    motor_ctrl(0,0);//free mode
    while(!I2C_ReceiveCheck());//I2C���`�F�b�N(��M�ł���܂ő҂�)
    TrPort = 1;//potention power is on.
    ZeroFlag  = 1;//firstposition to move
    SET = eeprom_read(eepadrs);//���Z�b�g�O�̒l��ǂݍ���
	__delay_ms(10);    

    while(1)//�������[�v
    {
        M = getData();//�G���R�[�_�[�̌��݈ʒu������
       
		if(ZeroFlag)//�����V�[�N�G���X
        {
            if(0<SET&&SET<512)
            {
                dire = Detect_Dire(1);
                Devi = Calu_Devi(ZeroFlag,M,SET);            
            }
            else
            {
                dire = Detect_Dire(0);
                Devi = Calu_Devi(ZeroFlag,M,SET);
            }
            
            duty = MinSpeed;
            duty |= Kp * abs(Devi);
            if(Devi<MinDevi||Devi>-MinDevi)
            {
                ZeroFlag = 0;
                Tr_Reset();
            }
        }
        else
        {
            stat = I2C_ReceiveCheck();
            if(stat==2)//rcv_data[0] = lower bits
            {          //rcv_data[1] = higher bits
                CLRWDT();  //�E�H�b�`�h�b�O�^�C�}�[���N���A�[          
                SET=((rcv_data[1]<<8)+rcv_data[0])-256;//�ڕW�l�̃f�[�^���擾//-256~+256
            }
            if(M<=512)
                M += 512;
            else if(M<=1023)
                M -= 512;
            Devi = Calu_Devi(ZeroFlag,M,SET);//�ڕW�l�ƌ��݈ʒu�̕΍����v�Z
            if(Devi<-MinDevi)//�΍���0���Ⴂ�ꍇ�i-devi�j
            {
                dire = Detect_Dire(0);//���[�^�[�̏o�͕���������
                duty = MinSpeed;//�ŏ��X�s�[�h����
                duty |= Kp*-Devi;//�΍�����X�s�[�h�v�Z���ĉ��Z
            }
            else if(Devi>MinDevi)//�΍���0��葽��������i+devi�j
            {     
                dire = Detect_Dire(1);//���[�^�[�̏o�͕���������
                duty = MinSpeed;//�ŏ��X�s�[�h����
                duty |= Kp*Devi;//�΍�����X�s�[�h�v�Z���ĉ��Z
            }
            else//�ڕW�l�̏ꍇ
            {
                dire = 3;//�u���[�L��������
                duty = MaxSpeed;
            }
        }
        if(duty>MaxSpeed)duty = MaxSpeed;//���[�^�[�o�͂��ő�X�s�[�h�������ꍇ�}���� 
        eeprom_write(eepadrs,M);
        motor_ctrl(duty,dire);//���[�^�[�ɏo��
    }
}

void interrupt common_interrupt(void)//���荞�ݏ����֐�
{
    Slave_Interrupt();//Slave�p���荞�ݏ����֐�
}