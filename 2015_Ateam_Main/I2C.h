///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�؍X�Í��ꃍ�{�b�g�������D��@
//I2C�ʐM�p�֐�
//Use device microchip PIC16F1938
//MPLAB X IDE(ver.2.30)
//HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode  
//Last updata 2015/5/12/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef I2C_H
#define	I2C_H

#define Master //mode select macro(Master or Slave)//
//#define Master

#define pinModeSCK TRISCbits.TRISC3
#define pinModeSDA TRISCbits.TRISC4

#define SND_DATA_LEN 8                  // ���M�f�[�^�o�b�t�@�̃T�C�Y
#define RCV_DATA_LEN 8                  // ��M�f�[�^�o�b�t�@�̃T�C�Y

/////////�X���[�u�̎�M�f�[�^�A���M�f�[�^�i�[�z��/////////////////
unsigned char rcv_data[RCV_DATA_LEN] ;  // ��M�f�[�^�o�b�t�@
unsigned char snd_data[SND_DATA_LEN] ;  // ���M�f�[�^�o�b�t�@

/**************************************Master_Mode********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef  Master
void Master_Interrupt(void);
//�@�\�F�}�X�^�[�̊��荞�ݎ��̌Ăяo���֐�
int I2C_Send(unsigned char adrs,int len,unsigned char *buf);
//�@�\�F	I2C�̑��M�֐�
//�����F	adrs...���M��
//			len...����
//			buf...���M�f�[�^
int I2C_Receive(unsigned char adrs,int len,unsigned char *buf);
//�@�\�F	I2C�̎�M�֐�
//�����F	adrs...��M��
//			len...����
//			buf...��M�o�b�t�@
void I2C_IdleCheck(char mask);
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************Slave_Mode********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Slave
void Slave_Interrupt( void );
//�@�\�F	�X���[�u���荞�ݎ��Ԑ�
int I2C_ReceiveCheck() ;
//�@�\�F	��M�`�F�b�N
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef  Master
void I2C_init(unsigned char speed);
//�@�\�F	�}�X�^�[�������֐�
//�����F	speed...���x�i16MH��...26���炢�H�j
#endif
#ifdef  Slave
void I2C_init(unsigned char add);
//�@�\�F	�X���[�u�������֐�
//�����F	add...�X���[�u�̃A�h���X
#endif

#endif	/* I2C_H */

