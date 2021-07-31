/* 
 * File:   RotalyEncorder.h
 * Author: oishi
 *
 * Created on 2015/06/26, 17:31
 */

#ifndef ROTARYENCORDER_H
#define	ROTARYENCORDER_H

//Clock_Freq_Define
#define KHz 000
#define MHz 000000
//---WANING---�����ɂĒ�`���邱��
#define _XTAL_FREQ 32MHz

//�g�p�|�[�g
#define ENCODER_A RA1
#define ENCODER_B RC0

#define ENCODER_A_TRIS TRISAbits.TRISA1
#define ENCODER_B_TRIS TRISCbits.TRISC0


//�g�p�^�C�}�[
//#define USETMR2
//#define USETMR4
#define USETMR6
//#error shine
#ifdef USETMR2
#warning It might be used by ECCP modulate
#endif

//�g�p�֐��Q

void _Tmr2_4_6InterruptInit(unsigned char n,unsigned char kHz,unsigned char config,void (*func)());
//�@�\�F�^�C�}�[�̏����������܂��B

//�����F��...�g�p����^�C�}�[���w�肵�܂��B
//      2..Timer2 4..Timer4 6..Timer6�Ƃ��܂��B

//�@�@�@��Hz...���g����ݒ肵�܂��i���x�͒Ⴂ�ł��j

//�@�@�@func...�֐��|�C���^���w�肵�܂��B�ȒP�Ɍ����ƕ��ʊ֐��������܂��B

//�x���F�K��interrupt_Tmr2_4_6�ƂƂ��Ɏg�����ƁB

void RotaryEncorderInit(unsigned char tmr,unsigned char kHz,unsigned char Mode,unsigned int n);
//�@�\�F���[�^���[�G���R�[�_�̎g�p���������܂�

//�����Ftmr:�g�p����^�C�}�[���w�肵�܂��B
//      2..Timer2 4..Timer4 6..Timer6�Ƃ��܂��B

//�@�@�@��Hz...�T���v�����O���g����ݒ肵�܂��i���x�͒Ⴂ�ł��j�B

//      Mode...�g�p���郂�[�h��I�����܂�

//      0...�ʏ탂�[�h�ŁAGetRotaryEncorderData���Ăяo�����܂ł̗ݐς̒l���o�͂��܂��Bn�͖�������܂�
//      1...2^n��̃T���v�����O�̒l���o�͂��܂��B

//      n...��L�ɐ��������Ƃ���łӁB

int GetRotaryEncorderData(void);
//�@�\�F���[�^���[�G���R�[�_�̉�]�ʂ̒l��Ԃ��܂�

//�����F�Ȃ�

//�Ԃ�l�F���[�^���[�G���R�[�_�̉�]��
void interrupt_Tmr2_4_6(void);
//���荞�ݎ��ɌĂяo���֐��ł��BTmr2_4_6InterruptInit�̂��ƗL���ł��B���荞�݌�����TMR2,4,6�ɂ���Ƃ���1���A��܂��B���̑��̎���0���A��܂�
#define Tmr2_4_6InterruptInit(x,y,z) {\
    _Tmr2_4_6InterruptInit(x,((_XTAL_FREQ/4)/256) < y?\
    (_XTAL_FREQ /4/y-1) : ((_XTAL_FREQ/4/4)/256)<y?\
    (_XTAL_FREQ /4/4/y-1) : (_XTAL_FREQ /4/4/4/y-1),\
    (((_XTAL_FREQ/4)/256) < y\
    ?0x04:((_XTAL_FREQ/4/4)/256) < y ?\
    0x05:0x06),z);}

#endif	/* ROTALYENCORDER_H */