/* 
 * File:   Device_select.h
 * Author: evaota
 *
 * Created on 2015/07/11, 10:33
 */

#ifndef DEVICE_SELECT_H
#define	DEVICE_SELECT_H
//P����e�p�����[�^�[
#define Ster1//���[�^�[�Z���N�g��`

#ifdef Ster1//
#define Direction1//�������[�h
#define Adrs 0x20//�X���[�u�A�h���X
#define Kp 3//���萔
#define Stee_Center 512//�X�e�A�����O�̃Z���^�[
#define MinDevi 10//�ڕW�l�ƌ��ݒl�̍ŏ��΍�
#define MaxSpeed 700//�ő�X�s�[�h
#define MinSpeed 200//�ŏ��X�s�[�h
#endif

#ifdef Ster2//
#define Direction1//�������[�h
#define Adrs 0x21//�X���[�u�A�h���X
#define Kp 3//���萔
#define Stee_Center 512//�X�e�A�����O�̃Z���^�[
#define MinDevi 10//�ڕW�l�ƌ��ݒl�̍ŏ��΍�
#define MaxSpeed 700//�ő�X�s�[�h
#define MinSpeed 200//�ŏ��X�s�[�h
#endif

#ifdef Ster3//
#define Direction1//�������[�h
#define Adrs 0x22//�X���[�u�A�h���X
#define Kp 3//���萔
#define Stee_Center 512//�X�e�A�����O�̃Z���^�[
#define MinDevi 10//�ڕW�l�ƌ��ݒl�̍ŏ��΍�
#define MaxSpeed 300//�ő�X�s�[�h
#define MinSpeed 0//�ŏ��X�s�[�h
#endif

#ifdef Ster3//
#define Direction1//�������[�h
#define Adrs 0x23//�X���[�u�A�h���X
#define Kp 3//���萔
#define Stee_Center 512//�X�e�A�����O�̃Z���^�[
#define MinDevi 10//�ڕW�l�ƌ��ݒl�̍ŏ��΍�
#define MaxSpeed 300//�ő�X�s�[�h
#define MinSpeed 0//�ŏ��X�s�[�h
#endif

#endif	/* DEVICE_SELECT_H */

