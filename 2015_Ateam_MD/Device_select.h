/* 
 * File:   Device_select.h
 * Author: evaota
 *
 * Created on 2015/07/11, 10:39
 */

#ifndef DEVICE_SELECT_H
#define	DEVICE_SELECT_H
//�e���[�^�[�h���C�o�[�Z���N�g
#define BDriver//<<��`���邱�ƂŃA�h���X���I������܂��B

#ifdef ADriver
#define Adrs 0x10//Green tape
#endif

#ifdef BDriver
#define Adrs 0x11//Yellow tape
#endif

#ifdef CDriver
#define Adrs 0x12//undecided colar
#endif

#ifdef DDriver
#define Adrs 0x13//undecided calar
#endif

#ifdef Shot_Rad
#define Adrs 0x14//undecided calar
#endif
#endif	/* DEVICE_SELECT_H */

