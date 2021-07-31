/****************************************************************************
	�h�Q�b�ʐM�Ŏg���v���g�^�C�v�錾�y�уO���[�o���ϐ��̐錾���s���w�b�_�[�t�@�C��
	
	�}�X�^�[�A�X���[�u�Ƃ��ɋ��L�ł������Header Files�ɒǉ����ăR���p�C�����Ă��������B
 ==========================================================================
 VERSION DATE			BY					CHANGE/COMMENT					
 --------------------------------------------------------------------------
  1.00    2011-12-23  ���ޒ��H�[(���ނ���)  Create                            	
  2.00    2012-01-21  ���ޒ��H�[(���ނ���)  �X���[�u�����ACK�҂�������ύX   			
  3.00    2012-06-30  11-305 IMADA     16F1938�p�ɉ��ρA�֐����w�b�_�Ɉڍs			
  3.01    2012-07-23  11-305 IMADA     ��Ă�send����悤�ύX
  4.00    2013-02-01  11-413 KAMATA    H�t�@�C���AC�t�@�C���̏������� �}�X�^�[�y�уX���[�u�ŋ��L�� 
  									   name skI2C.c, skI2C.h
 ========================================================================== 
  PIC 12F1822 16F1827 --> 16F1938                                          
  MPLAB IDE(V8.60)                                                         
  HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode        
****************************************************************************/
#ifndef I2C_H
#define I2C_H   //���d�ǂݍ��ݖh�~�̖ڈ�

#define SND_DATA_LEN 8                  // ���M�f�[�^�o�b�t�@�̃T�C�Y
#define RCV_DATA_LEN 8                  // ��M�f�[�^�o�b�t�@�̃T�C�Y

/***�X���[�u�̎�M�f�[�^�A���M�f�[�^�i�[�z��***/
unsigned char rcv_data[RCV_DATA_LEN] ;  // ��M�f�[�^�o�b�t�@
unsigned char snd_data[SND_DATA_LEN] ;  // ���M�f�[�^�o�b�t�@

/*  �X���[�u�p  */
void Slave_Interrupt( void );
void InitI2C_Slave(int address) ;
int I2C_ReceiveCheck() ;


/*�@�}�X�^�[�p�@*/
void Master_Interrupt( void );
void InitI2C_Master();
int I2C_Send(unsigned char adrs,int len,unsigned char *buf);
int I2C_Receive(unsigned char adrs,int len,unsigned char *buf);
int I2C_rStart_Receive(unsigned char adrs,unsigned char regist,unsigned char *buf);

void I2C_IdleCheck(char mask);

#endif 