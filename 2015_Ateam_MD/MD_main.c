#include<htc.h>//pic16f1938���g�p���������[//
#include<stdio.h>//�W�����o�͊֐�
#include"ECCP.h"
#include"I2C.h"
#include"UART.h"
#include"Device_select.h"

#define _XTAL_FREQ 32000000//�V�X�e���N���b�N8MHz*4(PLL)
__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_OFF&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_OFF&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_OFF&STVREN_OFF&LVP_OFF);
__IDLOC(1056);

void pic_init(void);//�f�o�C�X�����ݒ�֐�

void main (void)//���C���֐�
{
    unsigned char stat = 0;//I2C��M�X�e�[�^�X�ϐ�
    unsigned  int duty = 0;//���[�^�[�X�s�[�h�p�ϐ�
    unsigned char dire = 0;//���[�^�[��]�����p�ϐ�

    pic_init();//�f�o�C�X�����ݒ�
    I2C_init(Adrs);//I2C�����ݒ�
    InitUART(0,0,207);//UART�����ݒ�
    pwm_init();//ECCP���W���[�������ݒ�

    motor_ctrl(0,0);//free mode
    __delay_ms(10);//WAIT
    
    while(1)//�������[�v
    {
        stat = I2C_ReceiveCheck();//I2C��M�`�F�b�N

        if(stat)//��M���m�F������
        {
            CLRWDT();//�E�H�b�`�h�b�O�^�C�}�[������
            duty = rcv_data[0]<<2;//DUTY�̏��2�r�b�g��ǂݍ���
            duty |= (rcv_data[1]>>6);//DUTY�̉���6�r�b�g��ǂݍ���
            dire = (rcv_data[1]&0x03);//�����f�[�^��ǂݍ���
        }
        else//��M�����Ă��Ȃ�������
		{
            motor_ctrl(duty,dire);//���[�^�[�ɏo��
    	}
	}
}
void putch(char m)//UART�p�֐�
{
    UART_Write(m);
}
void pic_init(void)//�f�o�C�X�����ݒ�֐�
{
    OSCCON = 0xf0;  //internal clock 32MHz//
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
    
    WDTCON = 0x11;//�E�H�b�`�h�b�O�^�C�}�[�����ݒ�
}
void interrupt comu(void)//���荞�ݏ����֐�
{
    Slave_Interrupt();//Slave�p���荞�ݏ����֐�
    InterUART();//UART���荞�ݏ���
}