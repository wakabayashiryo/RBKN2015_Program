#include<htc.h>
#include"ECCP.h"
#include"I2C.h"
#include"ADC.h"
#include"Tmr0.h"

#define Adrs 0x60 //I2C�X���[�u�A�h���X
#define SeikiSpeed 350//���I���̌Œ�X�s�[�h
#define SeikiDire 1//������`
#define SensorRef 512//�Z���T�[�̃��t�@�����X�l
#define _XTAL_FREQ 32000000//�V�X�e���N���b�N8MHz*4(PLL)

__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_OFF&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_OFF&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_OFF&STVREN_OFF&LVP_OFF);
__IDLOC(1056);

void pic_init(void);//�V�X�e�������ݒ�֐�

unsigned int Wait_count;//�^�C�}�[�O�p�J�E���g�ϐ�

void main (void)//���C���֐�
{
    unsigned char stat = 0;//I2C��M�X�e�[�^�X�ϐ�
    static bit flag = 0,wait = 0;//��~�p�t���O�A��������p�t���O
    
    pic_init();//�V�X�e�������ݒ�
    Tmr0_Init();//�^�C�}�[�O�����ݒ�
    I2C_init(Adrs);//I2C�X���[�u�����ݒ�
    adc_init();//AD�R���o�[�^�����ݒ�
    pwm_init();//ECCP�����ݒ�

    motor_ctrl(0,0);//free mode
    __delay_ms(10);
    
    while(1)//�������[�v
    {
        stat = I2C_ReceiveCheck();//I2C��M�m�F

        if(stat)//��M������
        {
            CLRWDT();//�E�H�b�`�h�b�O�^�C�}�[���N���A�[
            if(rcv_data[0]==0x01)//�}�X�^�[����
            {
                wait = 1;//����������J�n
                Wait_count = 0;//�^�C�}�[�O�p�J�E���g�ϐ���������
            }   
        }
       
        if(wait)//��������
        {
            if(Wait_count>7000)//���̉񐔂܂ŃJ�E���g������
            {
                wait = 0;//����������I��
                flag  = 1;//��~�p�t���O�𗧂Ă�
            }
            else //��̃J�E���g���܂ŏo��
                motor_ctrl(SeikiSpeed,SeikiDire);//���[�^�[�ɏo��
        }
        else if(flag==1)//��~�p�t���O����������
        {
            motor_ctrl(SeikiSpeed,SeikiDire);//���[�^�[�ɏo��

            if(Samp_AD(0)>SensorRef)//�Z���T�[�����t�@�����X�𒴂�����
            {
                flag = 0;//�t���O�𗎂Ƃ��āA��~������
            }
        }         
         else//�t���O�������Ă�����
        {
            motor_ctrl(SeikiSpeed,3);//�u���[�L���o��
        }
    }
}

void pic_init(void)//�f�o�C�X�����ݒ�֐�
{
    OSCCON = 0xf0;  //internal clock 32MHz//
    TRISA = 0x01;   //input mode RA0//
    TRISB = 0x00;
    TRISC = 0b11011000;//�}�X�^�[��RX�Ƃ̊��h�~��TX����͂ɐݒ�
    TRISE = 0x00;

    ANSELA = 0x01;
    ANSELB = 0x00;

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTE = 0x00;
    
    WDTCON = 0x011;//�E�H�b�`�h�b�O�^�C�}�[�ݒ�
}

void interrupt comu(void)//���荞�ݏ����p�֐�
{
    Slave_Interrupt();//I2C���荞�ݏ���
    Inter_Count();//�^�C�}0�|����
}