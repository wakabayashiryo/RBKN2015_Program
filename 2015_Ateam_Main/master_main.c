/*
 * File:   main.c
 * Author: evaota
 *
 * Created on 2015/06/15, 21:14
 */
//character code Shift_JIS

#include <htc.h>
#include <stdio.h>//�W�����o�͊֐�
#include <stdlib.h>
#include "UART.h"
#include "I2C.h"
#include "ADC.h"
#include "Tmr0.h"
#include "Tmr1.h"
#include "IOC.h"
#include "System.h"
#include "Ctrl_Dataformat.h"
#include "Slave_Driver.h"

//#define Debug//printf���g���ăf�o�b�O����ꍇ�́ADebug���`���Ă��������B
#define _XTAL_FREQ 32000000//�V�X�e���N���b�N8MHz*4(PLL)
__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_ON&MCLRE_OFF&CP_OFF&CPD_OFF&BOREN_ON&CLKOUTEN_OFF&IESO_OFF&FCMEN_OFF&WRT_OFF&VCAPEN_OFF&PLLEN_ON&STVREN_OFF&LVP_OFF); //�R���t�B�O���[�V�����ݒ�
__IDLOC(1056);

void pic_init(void); //�V�X�e�������ݒ�֐�
unsigned char ZoneRecognize(unsigned char zone);//�]�[���m�F�p�ϐ�
void putch(char m); //printf�p�֐��v���g�^�C�v�錾
void Ctrl_Main(unsigned char *prcdat); //�f�[�^�����֐�
void Led_Flu(void); //��M�m�FLED�p�֐�
unsigned int Calu_Speed(signed char y); //���[�^�[�X�s�[�h�v�Z�֐�

bit keepflag, SwWait;
int Fp_X, Fp_Y; //�A�i���O�X�e�B�b�N�����l�p�ϐ�
unsigned char phase; //����J�E���g�ϐ�(���|�[�������ˋ@�\)
unsigned char Shot_flg; //���ˋ@�\�t���O�A�]�[���w��t���O
unsigned int SW_SampRate; //�X�C�b�`�p�T���v�����O���[�g�p�ϐ�

void main(void)//���C���֐�
{
    unsigned char Revdata[8] = {0}; //��M�f�[�^�i�[�z��
    unsigned int add, Whidat; //�e��M�f�[�^�̃A�h���X�Abluetooth����̎�M�f�[�^
    unsigned char CntForCal = 0; //�A�i���O�X�e�B�b�N�L�����u���[�V�����p�ϐ�

    pic_init(); //�}�C�R�������ݒ�
    IOC_Init(0,32);//��ԕω����荞�ݏ����ݒ�(RB5)
    Tmr0_Init(); //�^�C�}�[�O�����ݒ�
    Tmr1_Init(); //�^�C�}�[�P�����ݒ�
    InitUART(0,0,207); //UART�����ݒ�(boudlate 9600bps)
    I2C_init(0x4f); //I2C�����ݒ�(�}�X�^�[���[�h)
    adc_init(); //AD�R���o�[�^�[�����ݒ�

    while (UART_Read() == 0xFFFF); //UART�̃f�[�^������܂őҋ@

    Led_Flu(); //��M�m�F�pLED��_��
    EXT_OUT2 = 1;
           
    while (1)//�������[�v
    {
        Whidat = UART_Read(); //Bluetooth read
        add = Whidat >> 5; //read address 
        Revdata[add] = Whidat & 0x001f; //�����R���̊e�f�[�^�ǂݍ���

        if (Whidat == 0xffff)//����ȃf�[�^�����Ȃ��ꍇ
        {
            continue; //���[�v�̐擪�ɖ߂�
        }

        if (add == 7)//7�ڂ̃f�[�^����M�o������
        {
            if (CntForCal >= 8)
            {
                CLRWDT(); //�E�H�b�`�h�b�O�^�C�}������

                LED_RED1 ^= 1; //UART��M�Ԋu�v���p(GREEN_LED1)
#ifndef Debug
                System_Indicater2(0,phase,0,0,0);//send WS2812 indicater
#endif			
                Ctrl_Main(Revdata); //�f�[�^�����֐��ɒl��n���B
                
                UART_Flush(); //UART�̃o�b�t�@�[���N���A
            }
            else//analogstick's value is get average.
            {
                if (((15 - StopBorder) < Revdata[RX] && Revdata[RX]<(15 + StopBorder))&&((15 - StopBorder) < Revdata[LY] && Revdata[LY]<(15 + StopBorder))) 
                {
                    Fp_X += Revdata[RX];
                    Fp_Y += Revdata[LY];

                    CntForCal++;
                }
                if (CntForCal == 8)
                {
                    Fp_X >>= 3;
                    Fp_Y >>= 3;
                }
            }
        }
        ;
    }
}

#ifdef Debug

void putch(char m)//printf�p�֐�
{
    UART_Write(m);
}
#endif

void Led_Flu(void)//LED�_�ŗp�֐�
{
    unsigned char count, che;
    for (che = 0; che < 5; che++)
    {
        LED_BLUE1 = 1; //pic16f1939's LED
        LED_BLUE2 = 1; //pic16f1939's LED

        for (count = 0; count < 20; count++) 
        {
            __delay_ms(10);
            CLRWDT();
        }

        LED_BLUE1 = 0; //pic16f1939's LED
        LED_BLUE2 = 0; //pic16f1939's LED

        for (count = 0; count < 20; count++) 
        {
            __delay_ms(10);
            CLRWDT();
        }
    }
}

unsigned int Calu_Speed(signed char y)//���[�^�[�X�s�[�h�v�Z�֐�(�����̓A�i���O�X�e�B�b�N�̐�Βl-15~+15)
{
    unsigned int ResSpd; //�v�Z���ʗp�ϐ�

    y = abs(y); //��Βl���v�Z
    ResSpd = (short) MaxSpeed * (short) (y << 1) >> 5; //���[�^�[�X�s�[�h���v�Z

    return ResSpd; //���ʂ�Ԃ�
}

void pic_init(void)//�V�X�e�������ݒ�֐�
{
    OSCCON = 0xf0; //internal clock 32MHz//

    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x80;
    TRISD = 0x00;
    TRISE = 0x00;

    ANSELA = 0b00000000;
    ANSELB = 0b00000000;
    ANSELD = 0b00000000;
    ANSELE = 0b00000000;

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;

    WDTCON = 0x11; //�E�I�b�`�h�b�O�^�C�}�[�ݒ�(256ms)
    
    SETUPCON1_A1(INPUT); //�O�A�p�������T�[�{�؂�ւ��X�C�b�`�p(RA5)
    SETUPCON2_A1(INPUT); //�R�l�N�^�[�Q��A1����̓��[�h�ɐݒ�
    SETUPCON3_A1(INPUT); //�O�A�p�������ŏI�����p�Z���T�[����(RB5)
    SETUPCON4_A1(INPUT); //�R�l�N�^�[4��A1����̓��[�h�ɐݒ�

    ANALOGSETCON2_A1(ANALOG); //�R�l�N�^�[�Q��A1���A�i���O���[�h�ɐݒ�
    ANALOGSETCON4_A1(ANALOG); //�R�l�N�^�[4��A1���A�i���O���[�h�ɐݒ�

    WPUB = 0x20;//RB5 is enabled Weak Pull up
    nWPUEN =0;//Weak pull up enable
}

void interrupt common_inter(void)//���荞�ݏ����֐�
{
    Master_Interrupt(); //I2C���M���荞�݊֐�
    InterUART(); //UART��M���荞�݊֐�
    Inter_Count(); //�^�C�}�[0���荞�݊֐�
    Tmr1_Inter(); //�^�C�}�[�P���荞�݊֐�
    Inter_IOC();//��ԕω����荞�݊֐�
}

void Ctrl_Main(unsigned char *prcdat)//�f�[�^�����p�֐�
{
    ////////////////////////Seikimatsu_Process////////////////////////
    if (!(prcdat[Marutoka] & Circle))//�L���{�^���̊ۂ������ꂽ��
        Seikimatsu_ON(); //���I�������~�b�g�X�C�b�`����������܂œ������B
    else
        Seikimatsu_OFF(); //������Ă��Ȃ���΁A�z�[���h
    ////////////////////////SidearmThrow_Process////////////////////////
    if (!(prcdat[Marutoka] & Sq))//�L���{�^���̎l�p�������ꂽ��
    {
        Side_On(); //�T�C�h�X���[�@�\����J�n
    }
    else
        Side_Acti(); //�{�^�����͂��ꂽ��d���ق��J������
    /////////////////////////ThreeParallel_Process////////////////////////
    if (!(prcdat[Marutoka] & Tri))//�L���{�^���̎O�p�������ꂽ��
    {
        Lock_On(); //�ێ��@�\��ON�ɂ���B(�����|�[���@�\)
    }
    Lock_Acti(); //(�t���[����)
//    
//    if(IOC5f)//phase3�d���يJ���t���O��\��
//        LED_RED2 = 1;
//    else 
//        LED_RED2 = 0;
    if(phase==2)//phase���Q�̎��͏�ԕω����荞�݂�L���ɂ���
    {
        EN_IOC();//phase2�ȊO�ŏ�ԕω����荞�݂��N����̂�h������
    }
    else//����ȊO�̎��͖����ɂ���
    {
        IOC5f = 0;//�d���يJ���t���O�𗎂Ƃ�
        DI_IOC();
    }
    ////////////////////////ManualServo_Process////////////////////////
    if ((RA5 && (SwWait == 0)) || ((!(prcdat[LR] & L1)) && SwWait == 0))//�O�t���X�C�b�`���R���g���[���[(L1)�ŃT�[�{�̈ʒu��ύX�i�^�C�}�[�J�E���g���͓��͋֎~�j
    {
        SwWait = 1; //���͋֎~�J�E���g�J�n
        SW_SampRate = 0; //�J�E���g�p�ϐ�������
        phase++; //���b�N�@�\�̃T�[�{�̈ʒu��ύX
    }
    if (SwWait == 1 && SW_SampRate > SW_SampTime) SwWait = 0; //���͋֎~�J�E���g��~�B�܂��A���͂�����
    ////////////////////////Catapult_Process////////////////////////
    if(!(prcdat[Zyuzi]&Left))
        On_Valve(3);
    else if(!(prcdat[Zyuzi]&Right))
        On_Valve(4);
    else if (!(prcdat[LR]&R2))//�R���g���[���[��R2�������ꂽ��J�^�p���g�����J��
    {
        On_Valve(3); //PORT3�̓d���ق�HIGH�ɂ���B(�J�^�p���g�̓d����)
        On_Valve(4); //PORT4�̓d���ق�HIGH�ɂ���B(�J�^�p���g�̓d����)
    }
    else
    {
        Off_Valve(3); //PORT3�̓d���ق�LOW����B(�J�^�p���g�̓d����)  
        Off_Valve(4); //PORT4�̓d���ق�LOW����B(�J�^�p���g�̓d����)    
    }
    /////////////////////////////Drive_Process////////////////////////////////
    static bit StrFlag; //�X�e�A��������p�ϐ�
    int Now_X, Now_Y, Left_X; //�A�i���O�X�e�B�b�N���݈ʒu�p�ϐ�
    static unsigned int A_Steer, B_Steer;
    unsigned int ASpeed = 0, BSpeed = 0; //���E���[�^�[�X�s�[�h�p�ϐ�

    Now_X = Fp_X - prcdat[RX]; //�E�A�i���O�X�e�B�b�N��X���̐�Βl���v�Z
    Now_Y = Fp_Y - prcdat[LY]; //���A�i���O�X�e�B�b�N��Y���̐�Βl���v�Z
    Left_X = Fp_X - prcdat[LX]; //���A�i���O�X�e�B�b�N��X���̐�Βl���v�Z

    if (!(prcdat[LR] & R1))//���񏈗��@R1�������ꂽ��X�e�A��ς���莞�ԂɌo�߂�����A�i���O�X�e�B�b�N�̒l�ɉ����ă��[�^�[�ɏo��
    {
        StrFlag = 0; // �A�i���O�X�e�B�b�N�ɂ��X�e�A�����O����𖳌���

        Encorder_Str(A_Ster, ASteer); //�X�e�A��45�x��](���C���o�[�W����)
        Encorder_Str(B_Ster, BSteer);

        if (Tmr1_Wait_ms(TurnInterval) || keepflag)//R1��������Ďw�莞�Ԃ̓u���[�L
        {
            keepflag = 1;

            if (Left_X <= -StopBorder)
            {
                Out_Motor(A_Drv, Forward, Calu_Speed(Left_X)); //�X�e�A�̃X�s�[�h�Ń��[�^�[�ɏo��
                Out_Motor(B_Drv, Forward, Calu_Speed(Left_X));

            }
            else if (Left_X > StopBorder) 
            {
                Out_Motor(A_Drv, Reversal, Calu_Speed(Left_X)); //�X�e�A�̃X�s�[�h�Ń��[�^�[�ɏo��
                Out_Motor(B_Drv, Reversal, Calu_Speed(Left_X));
            }
            else 
            {
                Out_Motor(A_Drv, Brake, ASterSpeed); //�u���[�L���[�h
                Out_Motor(B_Drv, Brake, BSterSpeed);
            }
        }
        else
        {
            Out_Motor(A_Drv, Brake, ASterSpeed); //�u���[�L���[�h
            Out_Motor(B_Drv, Brake, BSterSpeed);
        }
    }
    else if (Now_Y <= -StopBorder)//�O��i����
    {
        StrFlag = 1; // �A�i���O�X�e�B�b�N�ɂ��X�e�A�����O����͗L��

        if (!(prcdat[LR] & L2))//Speed is half,when User pushed R2button.
        {
            ASpeed = BSpeed = Calu_Speed(Now_Y) >> 1; //�X�s�[�h�𔼕��Ɍv�Z
        }
        else
        {
            ASpeed = BSpeed = Calu_Speed(Now_Y);
        }

        Out_Motor(A_Drv, Forward, (unsigned int) (ASpeed * ((float) Samp_AD(0) / 1000.f))); //���E���[�^�[�ɏo��
        Out_Motor(B_Drv, Reversal, (unsigned int) (BSpeed * ((float) Samp_AD(8) / 1000.f)));
    }
    else if (Now_Y > StopBorder)//�O��i����
    {
        StrFlag = 1; // �A�i���O�X�e�B�b�N�ɂ��X�e�A�����O����͗L��

        if (!(prcdat[LR] & L2))//Speed is half,when User pushed R2button.
        {
            ASpeed = BSpeed = Calu_Speed(Now_Y) >> 1; //�X�s�[�h�𔼕��Ɍv�Z
        }
        else
        {
            ASpeed = BSpeed = Calu_Speed(Now_Y);
        }

        Out_Motor(A_Drv, Reversal, (unsigned int) (ASpeed * ((float) Samp_AD(0) / 1000.f))); //���E���[�^�[�ɏo��
        Out_Motor(B_Drv, Forward, (unsigned int) (BSpeed * ((float) Samp_AD(8) / 1000.f)));
    }
    else//analogstick's position is center.
    {
        keepflag = 0;
        Tmr1_Wait_ms(0);

        StrFlag = 1; //�A�i���O�X�e�B�b�N�ɂ��X�e�A�����O����͗L��
        Dead_Stop(500); //�S�쓮���[�^�[�u���[�L
    }
    ///////////////////////Steering_Process//////////////////////////
    if (StrFlag)//�A�i���O�X�e�B�b�N�ɂ��X�e�A�����O���L���Ȃ�
    {
        Encorder_Str(A_Ster, (int) (-Now_X * AStePala)); //�A�i���O�X�e�B�b�N�̒l�ŃX�e�A�̊p�x�ύX(���C���o�[�W����)
        Encorder_Str(B_Ster, (int) (-Now_X * BStePala));
    }
}