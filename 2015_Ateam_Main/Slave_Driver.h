/* 
 * File:   Slave_Driver.h
 * Author: evaota
 *
 * Created on 2015/07/01, 22:29
 */
//charcter code is Shift_JIS
 
#ifndef SLAVE_DRIVER_H
#define	SLAVE_DRIVER_H

extern unsigned char phase;//����J�E���g�ϐ�

////////////////////////////////////////�쓮�֌W////////////////////////////////////
unsigned char Out_Motor(unsigned char ch,unsigned char dire,unsigned int Speed);
//I2C send dataformat
//senddata is 2byte.
//1byte 7~0bit(Speed�̏��8bit)
//2byte 7~6bit(Speed�̉���2bit) 5~2bit(none) 1~0bit(0~3(dire)�̃f�[�^)

unsigned char Dead_Stop(unsigned int Speed);
//I2C send dataformat
//senddata is 2byte.
//1byte 7~0bit(Speed�̏��8bit)
//2byte 7~6bit(Speed�̉���2bit) 5~2bit(none) 1~0bit(3(brake)�̃f�[�^)

unsigned char Set_Str(unsigned char select,int rad);
//I2C send dataformat
//senddata is 2byte.
//1byte �p�x�f�[�^(0~511)�̉���8bit
//2byte �p�x�f�[�^(0~511)�̏��8bit

unsigned char Encorder_Str(unsigned char select,int rad);//�w�肵���p�x���X�e�A�����O�ɑ��M����+2048~-2048
//I2C send dataformat
//senddata is 2byte.
//1byte �p�x�f�[�^(0~4095)�̏��8bit
//2byte �p�x�f�[�^(0~4095)�̉���8bit

////////////////////////////////////////�G�A�[�֌W/////////////////////////////////////
unsigned char On_Valve(unsigned char select);
//I2C send dataformat
//senddata is 1byte.
//1byte 7~4bit(�d���ق̃|�[�g)�@3~0bit(�d���يJ���f�[�^(1))

unsigned char Off_Valve(unsigned char select);
//I2C send dataformat
//senddata is 1byte.
//1byte 7~4bit(�d���ق̃|�[�g)�@3~0bit(�d���ٕ��f�[�^(0)) 

///////////////////////////////////////���I���֌W//////////////////////////////////////
unsigned char Seikimatsu_ON(void);
//I2C send dataformat
//senddata is 1byte.
//1byte 7~1bit(none) 0bit(���I������f�[�^(1))

unsigned char Seikimatsu_OFF(void);
//I2C send dataformat
//senddata is 1byte.
//1byte 7~1bit(none) 0bit(���I����~�f�[�^(0))

///////////////////////////////////////�\����֌W/////////////////////////////////
unsigned char System_Indicater(unsigned char Color,unsigned char LedStat,unsigned char Brightness);
//I2C send dataformat
//senddata is 3byte.
//1byte 7~0bit(�C���W�P�[�^�[�̑��삷��LED��I��)
//2byte 7~0bit(1byte�őI������LED�̖��邳(256�i�K))
//3byte 7~0bit(�I������LED�̃I���I�t�f�[�^(1bit))

void System_Indicater2(unsigned char data1,unsigned char data2,unsigned char data3,unsigned char data4,unsigned char data5);
//UART send dataformat
//senddata is 1byte.
//1byte 7~5bit(�蓮�E�H�b�`�h�b�O�^�C�}�[�f�[�^) 4~2bit(�ێ��@�\�̏�ԃf�[�^) 1~0bit(�������]�[���̃f�[�^)

///////////////////////////////////�ێ��@�\�֌W////////////////////////////////
unsigned char Set_Lock(unsigned int set);
//I2C send dataformat
//senddata is 2byte.
//1byte 7~0bit(�p�x�f�[�^�̉���8bit) 
//2byte 7~0bit(�p�x�f�[�^�̏��8bit)

void Lock_On(void);
//this funcsion don't send data.

void Lock_Acti(void);//motor version
//this funcsion don't send data.

void Side_On(void);
//this funcsion don't send data.

void Side_Acti(void);
//this funcsion don't send data.

void Catap_On(void);
//this funcsion don't send data.

void Catap_Acti(void);
//this funcsion don't send data.

unsigned char Out_Servo(unsigned char port,unsigned char rad);
//I2C send dataformat
//senddata is 2byte.
//1byte 7~0bit(�T�[�{�̏o�͊p�x)
//2byte 7~0bit(�T�[�{�h���C�o�[�̑��삷��|�[�g)

void Shot_On(void);
//this funcsion don't send data.

void Shot_Acti(void);
//this funcsion don't send data.

///////////////////////////////////�T�C�h�X���[�p�x�����@�\///////////////////////////////
unsigned char Shot_Stop(void);
//I2C send dataformat
//senddata is 2byte.
//1byte 7~0bit(�T�C�h�X���[�pSpeed�̏��8bit)
//2byte 7~6bit(�T�C�h�X���[�pSpeed�̉���2bit) 5~2bit(none) 1~0bit(3(brake)�̃f�[�^)

unsigned char Shot_Up(void);
//I2C send dataformat
//senddata is byte.
//1byte 7~0bit(�T�C�h�X���[�pSpeed�̏��8bit)
//2byte 7~6bit(�T�C�h�X���[�pSpeed�̉���2bit) 5~2bit(none) 1~0bit(1(up)�̃f�[�^)

unsigned char Shot_Down(void);
//I2C send dataformat
//senddata is byte.
//1byte 7~0bit(�T�C�h�X���[�pSpeed�̏��8bit)
//2byte 7~6bit(�T�C�h�X���[�pSpeed�̉���2bit) 5~2bit(none) 1~0bit(2(down)�̃f�[�^)

#endif	/* SLAVE_DRIVER_H */

