/****************************************************************************
	�h�Q�b�ʐM���s���R�[�h�̃��C�u����
	
	�}�X�^�[�A�X���[�u�Ƃ��ɋ��L�ł������Source�t�@�C���ɒǉ����ăR���p�C�����Ă��������B
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

#include "GV.h"                 

unsigned int  rcv_flg ;       // ��M���(��M�f�[�^�̌����i�[)
unsigned char *Sdtp ;         // ���M�f�[�^�o�b�t�@�̃A�h���X�|�C���^�[
unsigned char *Rdtp ;         // ��M�f�[�^�o�b�t�@�̃A�h���X�|�C���^�[

int AckCheck ;
int success ;

/********************************************************************************
InitI2C_Master  I2C�ʐM�̃}�X�^�[���[�h�ŏ��������s������

	ans = I2C_Send(adrs,len,buf); �X���[�u(adrs)�Ɏw�肵����(len)�����f�[�^(buf)�𑗐M���鏈��.���̌�ans�ɃX���[�u�̉�������
	    
	    adrs : �X���[�u�̃A�h���X���w�肷��(8�`119�𐄏�)                        
	   	len  : ���M����f�[�^(�o�C�g)�̌����w�肷��
		buf  : ���M����f�[�^���i�[�����z����w�肷��   
	  	ans  : 0=����(���M����)�@1=�X���[�u���ԓ����Ȃ��@2=���肪�f�[�^�̎�M�����ۂ���                   

		buf[0] : ����͑��M����f�[�^�Ƃ��Ďg�p
		*buf :�@��M�����f�[�^
*******************************************************************************/
void InitI2C_Master()
{
     SSPSTAT= 0b10000000 ;   // �W�����x���[�h�ɐݒ肷��(100kHz)
     SSPCON1= 0b00101000 ;   // SDA/SCL�s����I2C�Ŏg�p���A�}�X�^�[���[�h�Ƃ���
     SSPADD = 0x13       ;   // �N���b�N=FOSC/((SSPADD + 1)*4) 8MHz/((0x13+1)*4)=0.1(100KHz)
     SSPIE = 1 ;             // SSP(I2C)���荞�݂�������
     BCLIE = 1 ;             // MSSP(I2C)�o�X�Փˊ��荞�݂�������
     PEIE   = 1 ;             // ���ӑ��u���荞�݂�������
     GIE    = 1 ;             // �S���荞�ݏ����������� 
     SSPIF = 0 ;             // SSP(I2C)���荞�݃t���O���N���A����
     BCLIF = 0 ;             // MSSP(I2C)�o�X�Փˊ��荞�݃t���O���N���A����
     TRISC   = TRISC | 0b00011000 ;  // TRISC��RC3�ARC4����͂ɂ��Ă���
}

/******************************

    �@�@�@Master�p���荞�ݏ���

******************************/
void Master_Interrupt( void )
{
     if (SSPIF == 1) {       // SSP(I2C)���荞�݂��������������f
	     if (AckCheck == 1) AckCheck = 0 ;
          SSPIF = 0 ;        // �t���O�N���A
     }
     else if (BCLIF == 1) {       // MSSP(I2C)�o�X�Փˊ��荞�݂��������������f
          BCLIF = 0 ;        // ����̓t���O�̂݃N���A����(������)
     } 
}


int I2C_Send(unsigned char adrs,int len,unsigned char *buf)
{
     int i , ans ;

     // �X�^�[�g(START CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.SEN = 1 ;
     // [�X���[�u�̃A�h���X+�X���[�u�͎�M��v��]�𑗐M����
     I2C_IdleCheck(0x5) ;
     AckCheck = 1 ;
     SSPBUF = (char)(adrs<<1) ;             // �A�h���X�𑗐M R/W=0
     while (AckCheck) ;                      // ���肩���ACK�ԓ���҂�
     ans = SSPCON2bits.ACKSTAT ;
     if (ans == 0) {
          for (i=0 ; i<len ; i++) {			// [�f�[�^]�𑗐M����
               I2C_IdleCheck(0x5) ;
               AckCheck = 1 ;
               SSPBUF = (unsigned char)*buf ;        // �f�[�^�𑗐M
               buf++ ;
               while (AckCheck) ;            // ���肩���ACK�ԓ���҂�
               ans = SSPCON2bits.ACKSTAT ;
               if (ans != 0) {
                    ans = 2 ;                // ���肪NOACK��Ԃ���
                    success = 1;			 //�m�F�p
                    break ;
               }
          }
     }
     // �X�g�b�v(STOP CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.PEN = 1 ;
     return ans ;
}
int I2C_Receive(unsigned char adrs,int len,unsigned char *buf)
{
	 int data = 0 ;

     int i , ans ;
     
     // �X�^�[�g(START CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.SEN = 1 ;
     // [�X���[�u�̃A�h���X+�X���[�u�փf�[�^�v��]�𑗐M����
     I2C_IdleCheck(0x5) ;
     AckCheck = 1 ;
     SSPBUF = (char)((adrs<<1)+1) ;    // �A�h���X�𑗐M R/W=1
     while (AckCheck) ;                 // ���肩���ACK�ԓ���҂�
     ans = SSPCON2bits.ACKSTAT ;
     if (ans == 0) {
          for (i=0 ; i<len ; i++) { 				// [�f�[�^]����M����
               I2C_IdleCheck(0x5) ;
               SSPCON2bits.RCEN = 1 ;  // ��M��������
               I2C_IdleCheck(0x4) ;
               *(buf+i) = SSPBUF ;         // ��M
               //data = SSPBUF; //��Ma
               //buf++ ;
               I2C_IdleCheck(0x5) ;
               if (i==len-1) SSPCON2bits.ACKDT = 1 ; // ACK�f�[�^��NOACK
               else       SSPCON2bits.ACKDT = 0 ; // ACK�f�[�^��ACK
               SSPCON2bits.ACKEN = 1 ;            // ACK�f�[�^��Ԃ�
          }
     }
     // �X�g�b�v(STOP CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.PEN = 1 ;
     return ans ;
}

int I2C_rStart_Receive(unsigned char adrs,unsigned char regist,unsigned char *buf)
{
     int i , ans ;

     // �X�^�[�g(START CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.SEN = 1 ;
     // [�X���[�u�̃A�h���X+�X���[�u�͎�M��v��]�𑗐M����
     I2C_IdleCheck(0x5) ;
     AckCheck = 1 ;
     SSPBUF = (char)(adrs<<1) ;             // �A�h���X�𑗐M R/W=0
     while (AckCheck) ;                      // ���肩���ACK�ԓ���҂�
     ans = SSPCON2bits.ACKSTAT ;
     
     //���W�X�^�A�h���X���M
     if (ans == 0) {
     	I2C_IdleCheck(0x5) ;
        AckCheck = 1 ;
        SSPBUF = regist ;        // �f�[�^�𑗐M
       	while (AckCheck) ;       // ���肩���ACK�ԓ���҂�
        ans = SSPCON2bits.ACKSTAT ;
        if (ans != 0) {
        	ans = 2 ;                // ���肪NOACK��Ԃ���
        	return 2; 
        }	    
	 } 
	 
	 // ���s�[�g�E�X�^�[�g(REPEATED START CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.RSEN = 1 ;
     // [�X���[�u�̃A�h���X]�𑗐M����
     I2C_IdleCheck(0x5) ;
     AckCheck = 1 ;
     SSPBUF = (char)((adrs<<1)+1) ;   // �A�h���X + R�𑗐M
     while (AckCheck) ;                 // ���肩���ACK�ԓ���҂�
     ans = SSPCON2bits.ACKSTAT ;

	if(ans == 0){
		I2C_IdleCheck(0x5) ;
        SSPCON2bits.RCEN = 1 ;  // ��M��������
        I2C_IdleCheck(0x4) ;
        *(buf) = SSPBUF ;         // ��M
        I2C_IdleCheck(0x5) ;
        SSPCON2bits.ACKDT = 1 ; // ACK�f�[�^��NOACK
        SSPCON2bits.ACKEN = 1 ; // ACK�f�[�^��Ԃ�
	}	
	
     // �X�g�b�v(STOP CONDITION)
     I2C_IdleCheck(0x5) ;
     SSPCON2bits.PEN = 1 ;
     return ans ;	
}	

/* I2C_IdleCheck(char mask) : �A�C�h����Ԃ̃`�F�b�N	*/
// ACKEN RCEN PEN RSEN SEN R/W BF ���S�ĂO�Ȃ�n�j

/* InitI2C_Master() : �h�Q�b�ʐM�̃}�X�^�[���[�h�ŏ��������s������*/                              
// ��)CPU�ۯ�(FOSC)8MHz�ł̐ݒ�A���̸ۯ���SSP1ADD��ύX����K�v�L��

/* 
	I2C_Send(adrs,len,buf) : �X���[�u�Ƀf�[�^���w�肵����(len)�������M���Aint�^�ŕԂ�                       
                                                                              
    adrs : �X���[�u�̃A�h���X���w�肷��(8�`119�𐄏�)                         
    len  : ���M����f�[�^(�o�C�g)�̌����w�肷��                            
    buf  : ���M����f�[�^���i�[�����z����w�肷��        
*/

/*
	int I2C_Receive(adrs,len,buf) : �X���[�u����f�[�^���w�肵����(len)������M���A�l��int�^�ŕԂ�
                                                                              
    data : ��M�f�[�^�ۊǗp�ϐ�
*/

void I2C_IdleCheck(char mask)
{
     while (( SSPCON2 & 0x1F ) | (SSPSTAT & mask)) ;
}

/*******************************************************************************
	InitI2C_Slave(address): �h�Q�b�ʐM�̃X���[�u���[�h�ŏ��������s������
							�ꊇ�Ăяo���̓���ʒm�͖����̐ݒ�ƂȂ��Ă��܂��B                       *
	address	: �X���[�u�̃}�C�A�h���X��ݒ肷��(�W�`�P�P�X�𐄏�)              
*******************************************************************************/
void InitI2C_Slave(int address)
{
     SSPSTAT= 0b10000000 ;   // �W�����x���[�h�ɐݒ肷��(100kHz)
     SSPCON1= 0b00100110 ;   // SDA/SCL�s����I2C�Ŏg�p���A�X���[�u���[�h�Ƃ���
     SSPCON2bits.SEN  = 1;   // SCL����(�N���b�N�X�g���b�`)���s��
//   SSPCON2bits.GCEN = 1;   // �ꊇ�Ăяo���̓���ʒm��L���ɂ���
     SSPADD = address << 1 ;   // �}�C�A�h���X�̐ݒ�
     SSPMSK = 0b11111110 ;   // �A�h���X��r�p�}�X�N�f�[�^
     SSPIE = 1 ;             // SSP(I2C)���荞�݂�������
     BCLIE = 1 ;             // MSSP(I2C)�o�X�Փˊ��荞�݂�������
     PEIE   = 1 ;             // ���ӑ��u���荞�݂�������
     GIE    = 1 ;             // �S���荞�ݏ����������� 
     SSPIF = 0 ;             // SSP(I2C)���荞�݃t���O���N���A����
     BCLIF = 0 ;             // MSSP(I2C)�o�X�Փˊ��荞�݃t���O���N���A����
     TRISC   = TRISC | 0b00011000 ;  // TRISC��RC3�ARC4����͂ɂ��Ă���
     rcv_flg = 0 ;
}

/**************************************

�@�@�@�@�@�@�@�@Slave�p���荞�ݏ����֐�

**************************************/
void Slave_Interrupt( void )
{
	char x ;

  /*** SSP(I2C)���荞�ݔ������̏���***/
     if (SSPIF == 1) {  // 
          if (SSPSTATbits.R_nW == 0) {
               /******* �}�X�^�[����̏������ݗv��(�X���[�u�͎�M) *******/
               if (SSPSTATbits.D_nA == 0) {
                    // ��M�o�C�g�̓A�h���X
                    Rdtp = (char *)rcv_data ;
                    x = SSPBUF ;       // �A�h���X�f�[�^����ǂ݂���
                    rcv_flg = 0 ;
               } else {
                    // ��M�o�C�g�̓f�[�^
                    *Rdtp = SSPBUF ;   // �f�[�^��Ǎ���(ACK��PIC�������I�ɕԂ�)
                    Rdtp++ ;
                    rcv_flg++ ;
               }
               SSPIF = 0 ;             // �����݃t���O�N���A
               SSPCON1bits.CKP = 1 ;   // SCL���C�����J������(�ʐM�̍ĊJ)
          } else {
               /******* �}�X�^�[����̓ǂݏo���v��(�X���[�u�͑��M) *******/
               if (SSPSTATbits.BF == 1) {
                    // �A�h���X��M��̊��荞�݂Ɣ��f����
                    Sdtp = (char *)snd_data ;
                    x = SSPBUF ;       // �A�h���X�f�[�^����ǂ݂���
                    while((SSPCON1bits.CKP)|(SSPSTATbits.BF)) ;
                    SSPBUF = *Sdtp ;             // ���M�f�[�^�̃Z�b�g
                    Sdtp++ ;
                    SSPIF = 0 ;                  // �����݃t���O�N���A
                    SSPCON1bits.CKP = 1 ;        // SCL���C�����J������(�ʐM�̍ĊJ)
               } else {
                    // �f�[�^�̑��M���ACK�󂯎��ɂ�銄�荞�݂Ɣ��f����
                    if (SSPCON2bits.ACKSTAT==0) {
                         // �}�X�^�[����ACK�����Ȃ玟�̃f�[�^�𑗐M����
                         while((SSPCON1bits.CKP)|(SSPSTATbits.BF)) ;
                         SSPBUF = *Sdtp ;        // ���M�f�[�^�̃Z�b�g
                         Sdtp++ ;
                         SSPIF = 0 ;             // �����݃t���O�N���A
                         SSPCON1bits.CKP = 1 ;   // SCL���C�����J������(�ʐM�̍ĊJ)
                    } else {
                         // �}�X�^�[�����NOACK�ŉ������ꂽ��
                         SSPIF = 0 ;             // �����݃t���O�N���A
                    }
               }
          }
     }
     
    /* MSSP(I2C)�o�X�Փˊ��荞�������̏���*/
     if (BCLIF == 1) {
          BCLIF = 0 ;   // ����̓t���O�̂݃N���A����(������)
     }		
}

/*******************************************************************************
	ans = I2C_ReceiveCheck() : �}�X�^�[����̎�M��Ԃ��`�F�b�N���鏈��                                  *
	ans : �߂�l���O�Ȃ疢��M�ŁA��M�������M�����f�[�^�̌���Ԃ�       
*******************************************************************************/
int I2C_ReceiveCheck()
{
     int ans ;

     ans = 0 ;
     if (rcv_flg != 0) { // ��M��������
          if ((SSPSTATbits.S==0)&&(SSPSTATbits.P==1)) { // �į�ߺ��ި��ݔ��s���ꂽ����
               ans = rcv_flg ;
               rcv_flg = 0 ;
          }
     }
     return(ans) ;
}