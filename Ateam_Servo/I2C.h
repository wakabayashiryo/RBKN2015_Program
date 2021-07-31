///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�؍X�Í��ꃍ�{�b�g�������D��@
//I2C�ʐM�p�֐�
//Use device microchip PIC16F1938
//MPLAB X IDE(ver.2.30)
//HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode  
//Last updata 2015/5/12/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef I2C_H
#define	I2C_
#define Slave //mode select macro(Master or Slave)//
#define pinModeSCK TRISCbits.TRISC3	//SCKpin����͂ɐݒ�//
#define pinModeSDA TRISCbits.TRISC4	//SDApin����͂ɐݒ�//

#define SND_DATA_LEN 8                  // ���M�f�[�^�o�b�t�@�̃T�C�Y
#define RCV_DATA_LEN 8                  // ��M�f�[�^�o�b�t�@�̃T�C�Y

/////////�X���[�u�̎�M�f�[�^�A���M�f�[�^�i�[�z��/////////////////
unsigned char rcv_data[RCV_DATA_LEN] ;  // ��M�f�[�^�o�b�t�@
unsigned char snd_data[SND_DATA_LEN] ;  // ���M�f�[�^�o�b�t�@

/**************************************Master_Mode********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef  Master
void Master_Interrupt( void );
int I2C_Send(unsigned char adrs,int len,unsigned char *buf);
int I2C_Receive(unsigned char adrs,int len,unsigned char *buf);
void I2C_IdleCheck(char mask);
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************Slave_Mode********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Slave
void Slave_Interrupt( void );
int I2C_ReceiveCheck() ;
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef  Master
void I2C_init(unsigned char speed);
#endif
#ifdef  Slave
void I2C_init(unsigned char add);
#endif

#endif	/* I2C_H */

