#include<htc.h>//pic16f1938���g�p���������[//
#include"I2C.h"
#include"PORT_CONF.h"
//charcter code Shifh_JIS

#define _XTAL_FREQ 32000000//�V�X�e���N���b�N8MHz*4(PLL)
#define Adrs 0x30//I2C�X���[�u�A�h���X
__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_OFF&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_OFF&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_OFF&STVREN_OFF&LVP_OFF);
__IDLOC(1056);

void pic_init(void);//�f�o�C�X�����ݒ�֐�

void main (void)//���C���֐�
{
    unsigned char stat,select,valve;//I2C��M�X�e�[�^�X�ϐ��A�|�[�g�I��p�ϐ��A�d���يJ�p�ϐ�
    stat = select = valve = 0;//�e�ϐ�������

    pic_init();//�f�o�C�X�����ݒ�
    I2C_init(Adrs);//I2C�����ݒ�
    
    while(1)//�������[�v
    {
        stat = I2C_ReceiveCheck();//I2C��M�m�F

        if(stat)//��M���m�F������
        {
            
            GreenLed ^= 1;
            CLRWDT();//�E�H�b�`�h�b�O�^�C�}���N���A�[
            valve = rcv_data[0]&0x0f;//�J�f�[�^���擾
            select = rcv_data[0]>>4;//�|�[�g�I���f�[�^������

            switch(select)//�|�[�g��I��
            {
                case 1:
                    if(valve>0)//�|�[�g�P�̊J��
                    {
                        BlueOut = 1;
                        //BlueLed = 1;
                    }
                    else//�|�[�g�P�̕���
                    {
                        BlueOut = 0;
                        //BlueLed = 0;
                    }
                break;
                
                case 2:
                    if(valve>0)//�|�[�g�Q�̊J��
                    {
                        RedOut = 1;
                        //RedLed = 1;
                    }
                    else//�|�[�g�Q�̕���
                    {
                        RedOut = 0;
                        //RedLed = 0;
                    }
                break;
                
                case 3:
                    if(valve>0)//�|�[�g�R�̊J��
                    {
                        GreenOut = 1;
                        //GreenLed = 1;
                    }
                    else//�|�[�g�R�̕���
                    {
                        GreenOut = 0;
                        //GreenLed = 0;
                    }
                break;
                
                case 4:
                    if(valve>0)//�|�[�g�S�̊J��
                    {
                        WhiteOut = 1;
                        //WhiteLed = 1;
                    }
                    else//�|�[�g�S�̕���
                    {
                        WhiteOut = 0;
                       // WhiteLed = 0;
                    }
                break;
                
                case 5:
                    if(valve>0)//�|�[�g�S�̊J��
                    {
                        BlackOut = 1;
                    }
                    else//�|�[�g�S�̕���
                    {
                        BlackOut = 0;
                    }
                break;
                
                case 6:
                    if(valve>0)//�|�[�g�S�̊J��
                    {
                        YellowOut = 1;
                    }
                    else//�|�[�g�S�̕���
                    {
                        YellowOut = 0;
                    }
                break;
            }
        }
    }
}

void pic_init(void)//�f�o�C�X�����ݒ�֐�
{
    OSCCON = 0xf0;  //internal clock 32MHz//
    TRISA = 0x00;   //input mode RA0//
    TRISB = 0x00;
    TRISC = 0b11011000;//�}�X�^�[��RX�Ƃ̊��h�~��TX����͂ɐݒ�
    TRISE = 0x00;

    ANSELA = 0x00;
    ANSELB = 0x00;

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTE = 0x00;
    
    WDTCON = 0x11;//�E�H�b�`�h�b�O�^�C�}�[�����ݒ�
}

void interrupt comu(void)//���荞�ݏ����p�֐�
{
    Slave_Interrupt();//Slave�p���荞�ݏ����֐�
}