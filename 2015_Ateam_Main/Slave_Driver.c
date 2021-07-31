/* 
 * File:   Ctrl_main.c
 * Author: evaota
 *
 * Created on 20#
 *
 */
//character code Shift_JIS
#include <htc.h>
#include "UART.h"
#include "I2C.h"
#include "Tmr0.h"
#include "IOC.h"
#include "Slave_Driver.h"
#include "Ctrl_Dataformat.h"
#include "System.h"
//character code is Shift_JIS

bit Shot_Flg,Sservo,Swait,Sair;//�T�[�{�����p�ϐ�
bit Lock_Flg,Lock,Lwait,Lair;//�ێ��@�\�����p�ϐ�
bit Side_Flg,Sdwait,Sdair;//�T�C�h�X���[�����p�ϐ�
bit Catap_Flg,Ctpwait,Ctpair;//�J�^�p���g���u�p�ϐ�

unsigned char phase;//����J�E���g�ϐ�
unsigned char IOC5f;//phase3�d���يJ���t���O�p�ϐ�
unsigned int Wait_count;//�^�C�}�[�O�p�J�E���g�ϐ��i�ێ��@�\�E�F�C�g����j
unsigned int Side_count;//�^�C�}�[�O�p�J�E���g�ϐ��i�T�C�h�X���[�E�F�C�g����j
unsigned int Catap_count;//�^�C�}�[�O�p�J�E���g�ϐ��i�J�^�p���g�E�F�C�g����j

////////////////////////////////////////�쓮�֌W////////////////////////////////////
unsigned char Out_Motor(unsigned char ch,unsigned char dire,unsigned int Speed)//���[�^�[�o�͗p�֐�
{
    static const unsigned char adrs[4] = {MotrAdrs1,MotrAdrs2,MotrAdrs3,MotrAdrs4};//�A�h���X�p�z��
    unsigned char ans;//���M�m�F�ϐ�
    unsigned char SendDat[2];//���M�f�[�^�p�z��

    SendDat[0] = Speed>>2;//��ɕ������đ��M
    SendDat[1] = (Speed<<6)|dire;

    ans = I2C_Send(adrs[ch],2,SendDat);//���M
    
	return ans;//���M��Ԃ�Ԃ�
}

unsigned char Dead_Stop(unsigned int Speed)//�S���[�^�[���u���[�L��Ԃɂ���
{
    static const unsigned char adrs[4] = {MotrAdrs1,MotrAdrs2,MotrAdrs3,MotrAdrs4};//�A�h���X�p�z��
    unsigned char SendDat[2];//���M�f�[�^�p�z��
    unsigned char ans = 0;//���M�m�F�ϐ�

    SendDat[0] = Speed>>2;
    SendDat[1] = (Speed<<6)|Brake;

    for(unsigned char i = 0;i<4;i++)
    {
        ans += I2C_Send(adrs[i],2,SendDat);//���M
    }
    return ans;//���M��Ԃ�Ԃ�
}

unsigned char Set_Str(unsigned char select,int rad)//�w�肵���p�x���X�e�A�����O�ɑ��M����//inputdata -256^+256
{
    static const unsigned char adrs[4] = {SteeAdrs1,SteeAdrs2,SteeAdrs3,SteeAdrs4};//�A�h���X�p�z��
    unsigned char SteeDat[2];//���M�f�[�^�p�z��
	unsigned char ans;//���M�m�F�ϐ�
    
    SteeDat[0] = (rad+256)&0xff;//lower bit
    SteeDat[1] = (rad+256)>>8;//higher bit

    ans = I2C_Send(adrs[select],2,SteeDat);//���M
    
    return ans;//���M��Ԃ�Ԃ�
}

unsigned char Encorder_Str(unsigned char select,int rad)//�w�肵���p�x���X�e�A�����O�ɑ��M����+2048~-2048
{
    static const unsigned char adrs[4] = {SteeAdrs1,SteeAdrs2,SteeAdrs3,SteeAdrs4};//�A�h���X�p�z��
    unsigned char SteeDat[2];//���M�f�[�^�p�z��
	unsigned char ans;//���M�m�F�ϐ�
    
    SteeDat[0] = (rad+0x07ff)>>8;//higher bit
    SteeDat[1] = (rad+0x07ff)&0x00ff;//lower bit

    ans = I2C_Send(adrs[select],2,SteeDat);//���M
    
    return ans;//���M��Ԃ�Ԃ�
}
////////////////////////////////////////�G�A�[�֌W/////////////////////////////////////
unsigned char On_Valve(unsigned char select)//�w�肳�ꂽ�|�[�g�̓d���ق�HIGH�ɂ���
{
    static const unsigned char add = AirValve;//�A�h���X�p�ϐ�
    unsigned char ans;//���M�m�F�ϐ�
    unsigned char OnData = 1;//���M�f�[�^�p�ϐ�
    
    if(select==1)//�d���ق̊J���󋵂��}�X�^�[��LED�ɕ\��
        LED_GREEN1 = 1;
    else if(select==2)
        LED_GREEN2 = 1;
    else if(select==3)
        LED_BLUE1 = 1;
    else if(select==4)
        LED_BLUE2 = 1;
    
    OnData |= select<<4;

    ans = I2C_Send(add,2,&OnData);//���M

    return ans;//���M��Ԃ�Ԃ�
}

unsigned char Off_Valve(unsigned char select)//�w�肳�ꂽ�|�[�g�̓d���ق�LOW�ɂ���
{
    static const unsigned char add = AirValve;//�A�h���X�p�ϐ�
    unsigned char ans;//���M�m�F�ϐ�
    unsigned char OffData = 0;//���M�f�[�^�p�ϐ�
    
    if(select==1)//�d���ق̊J���󋵂��}�X�^�[��LED�ɕ\��
        LED_GREEN1 = 0;
    else if(select==2)
        LED_GREEN2 = 0;
    else if(select==3)
        LED_BLUE1 = 0;
    else if(select==4)
        LED_BLUE2 = 0;
    
    OffData |= select<<4;
  
    ans = I2C_Send(add,2,&OffData);//���M

    return ans;//���M��Ԃ�Ԃ�
}
///////////////////////////////////////���I���֌W//////////////////////////////////////
unsigned char Seikimatsu_ON(void)//���I���𓮍삳����
{
    static const unsigned char adrs = SeikimatsuAdrs;//�A�h���X�p�ϐ�
    unsigned char ans;//���M�m�F�ϐ�
	unsigned char ON; //���M�f�[�^�p�ϐ�
   
    ON = 1;

    ans = I2C_Send(adrs,1,&ON);//���M
    
    return ans;//���M��Ԃ�Ԃ�
}

unsigned char Seikimatsu_OFF(void)//���I�����~������
{
    static const unsigned char adrs = SeikimatsuAdrs;//�A�h���X�p�ϐ�
    unsigned char ans;//���M�m�F�ϐ�
	unsigned char OFF;//���M�f�[�^�p�ϐ�
    
    OFF = 0;

    ans = I2C_Send(adrs,1,&OFF);//���M
    
    return ans;//���M��Ԃ�Ԃ�
}
///////////////////////////////////////�\����֌W/////////////////////////////////
unsigned char System_Indicater(unsigned char Color,unsigned char LedStat,unsigned char Brightness)//�f�o�b�O�{�[�h��LED���w�肷��
{
    static const unsigned char Adrs = SysIndi_Adrs;//�A�h���X�p�ϐ�
    unsigned char SendDat[3];//���M�f�[�^�p�z��
    unsigned char ans;//���M�m�F�ϐ�
    
    SendDat[0] = Color;
    SendDat[1] = Brightness;
    SendDat[2] = LedStat;
    
    ans = I2C_Send(Adrs,3,SendDat);//���M
    
	return ans;//���M��Ԃ�Ԃ�
}
//�t���J���[�C���f�B�P�[�^�[�p�f�[�^���M�֐�
void System_Indicater2(unsigned char data1,unsigned char data2,unsigned char data3,unsigned char data4,unsigned char data5)
{
	unsigned char SendData=0;//UART���M�p�ϐ�
	
	SendData |= data1;//zone�f�[�^
	SendData |= data2<<2;//charge�f�[�^
	SendData |= (data3&0x03)<<5;//Cata�f�[�^
	
	UART_Write(SendData);//�\�����UART�o�R�ő��M
}
///////////////////////////////////�ێ��@�\�֌W////////////////////////////////
unsigned char Set_Lock(unsigned int set)////�w�肵���p�x��ێ��@�\�ɑ��M����
{
    static const unsigned char adrs = LockAdrs;//�A�h���X�p�ϐ�
    unsigned char LockDat[2];//���M�f�[�^�p�z��
	unsigned char ans;//���M�m�F�ϐ�
    
    LockDat[0] = set&0xff;//lower bit
    LockDat[1] = set>>8;//higher bit

	ans = I2C_Send(adrs,2,LockDat);//���M
    
    return ans;//���M��Ԃ�Ԃ�
}

void Lock_On(void)//�ێ��@�\����J�n�֐�
{
    if(Lock_Flg)return;//���쒆�͓��͖���
    Lock_Flg = 1;//�ێ��@�\����t���O�𗧂Ă�
    Lair = 1;//�d���كt���O�𗧂Ă�
    Wait_count = 0;//�E�F�C�g�J�E���g���N���A�[
}

void Lock_Acti(void)//�ێ��@�\�̓���
{
	static const unsigned int ServoRad[Radnum] = {10,60,120,120};//�T�[�{�ɑ��M����p�x�f�[�^
    
    if(phase==Radnum)phase = 0;//����J�E���g�����Z�b�g
    
	Out_Servo(0,ServoRad[phase]);//�T�[�{�Ɋp�x�𑗐M
    
	if(Lock_Flg)//�ێ��@�\����t���O�𗧂��Ă�����
    {
        if(Lair)//�d���يJ�����ԏ���
        {
            if(Wait_count>LockOnTime1&&phase==0)//phase��0��16000�J�E���g�����Ԃ���������B
            {
               	Lair = 0;//�d���كt���O��������
               	Lwait = 1;
				Wait_count = 0;
			}
            else if(Wait_count>LockOnTime2&&phase==1)//phase��1��8000�J�E���g�����Ԃ���������B
            {
               	Lair = 0;//�d���كt���O��������
               	Lwait = 1;
				Wait_count = 0;
			}
            else if(IOC5f&&phase==2)//RB5�̏�ԕω����荞�݂��N����܂œd���ق��J��
            {
                Lair = 0;//�d���كt���O��������
               	Lwait = 1;
				Wait_count = 0;
			}
			else if(Wait_count>LockOnTime4&&phase==3)//phase��3��4000�J�E���g�����Ԃ���������
			{
				Lair = 0;//�d���كt���O��������
				Lwait = 1;
				Wait_count = 0;
			}
            else//�d���ق�high�ɂ��鎞��
            {
                if(phase==3)//����J�E���g���R�ł���ꍇ�d���ق͓��삵�Ȃ�
                    Off_Valve(1);
                else//��L�ȊO�ł́A�d���ق𓮍삳����
                    On_Valve(1);
            }
        }
		
        if(Lwait)
        {
			Off_Valve(1);//�d���ق����
		
			if(Wait_count>LockOffTime)//�O�A�p���������k���̃C���^�[�o��
			{
				Lwait = 0;
				Lock = 1;
			}
		}
		
		if(Lock)
		{
			Lock = 0;
			Lock_Flg = 0;//�ێ��@�\����I��
			phase++;//����J�E���g���C���N�������g 
        } 
    }
    else //����t���O�������Ă��Ȃ���Γd���ق�low
    {
        Off_Valve(1);//AirValve is disable.
    }
}

void Side_On(void)
{
    Side_Flg = 1;//�ێ��@�\����t���O�𗧂Ă�
    Side_count = 0;//�E�F�C�g�J�E���g���N���A�[	
}

void Side_Acti(void)
{
	if(Side_Flg)
	{
		if(Side_count<SideOnTime)
		{
			On_Valve(2);//PORT2�̓d���ق�HIGH�ɂ���B(�T�C�h�X���[�̓d����)
		}
		else
		{
			Side_Flg = 0;
			Off_Valve(2);//PORT2�̓d���ق�LOW�ɂ���B
		}
	}
	else
	{
		Off_Valve(2);//PORT2�̓d���ق�LOW�ɂ���B
	}
}
void Catap_On(void)
{
    Catap_Flg = 1;//�ێ��@�\����t���O�𗧂Ă�
    Catap_count = 0;//�E�F�C�g�J�E���g���N���A�[	
}

void Catap_Acti(void)
{
	if(Catap_Flg)
	{
		if(Catap_count<CtpOnTime)
		{
			On_Valve(4);//PORT4�̓d���ق�HIGH�ɂ���B(�J�^�p���g�̓d����)
		}
		else
		{
			Catap_Flg = 0;
			Off_Valve(4);//PORT4�̓d���ق�LOW�ɂ���B
		}
	}
	else
		Off_Valve(4);//PORT4�̓d���ق�LOW�ɂ���B
	
}
unsigned char Out_Servo(unsigned char port,unsigned char rad)//�T�[�{�̊p�x�𑗐M����//rcvdata[0] = portdata,rcvdata[1] = raddata
{
    unsigned char add = ServoAdrs1;//�A�h���X�p�ϐ�
    unsigned char ans;//���M�m�F�ϐ�
    unsigned char SendDat[2];//���M�f�[�^�p�z��

    SendDat[0] = port;
    SendDat[1] = rad;

    ans = I2C_Send(add,2,SendDat);//���M

    return ans;//���M��Ԃ�Ԃ�
}

/*void Shot_On(void)
{
    if(Shot_Flg)return;
    Shot_Flg = 1;
    Sservo = 1;
}*/

/*void Shot_Acti(void)//servo version
{
    static unsigned char phase = 0;
    static const unsigned char ang[5][6] = {{LClose,LClose,LOpen,ROpen,RClose,RClose  },
                                            {LClose,LOpen,LOpen,ROpen,ROpen,RClose    },
                                            {LOpen,LOpen,LOpen,ROpen,ROpen,ROpen      },
                                            {Midole,Midole,Midole,Midole,Midole,Midole},
                                            {LClose,LClose,LClose,RClose,RClose,RClose}
                                            };
    if(Shot_Flg)//shot sequence start
    {
        if(Sservo)//servo
        {  
                if(phase>3)phase = 0;
                for(unsigned char out=1;out<7;out++)//send servo
                {
                    if(out<4)
                        Out_Servo(out,ang[phase][out-1]);//servo-module1 send
                    else
                        Out_Servo(out+1,ang[phase][out-1]);//servo-module2 send
                }
                phase++;
                Wait_count = 0;
                Swait = 1;
                Sservo = 0;
        }
        if(Swait)
        {
            if(Wait_count > 20000)
                {
                    Swait = 0;
                    Sair = 1;
                    Wait_count = 0;
                }
        }
        if(Sair)
        {
            if(Wait_count>20000)
            {
                Sair = 0;
                Shot_Flg=0;//shot sequence end
            }
            else
            {
                On_Valve(1);
            }
        }
    }
    else
    {
        Off_Valve(1);//AirValve is disable.
    }
}
*/

///////////////////////////////////�T�C�h�X���[�p�x�����@�\///////////////////////////////
unsigned char Shot_Stop(void)//�T�C�h�X���[���z�[���h
{
	static const unsigned char adrs = Shot_Rad;//�A�h���X�p�ϐ�
    unsigned char ans;//���M�m�F�ϐ�
    unsigned char SendDat[2];//���M�f�[�^�p�z��

    SendDat[0] = Shot_Rad_Speed>>2;
    SendDat[1] = (Shot_Rad_Speed<<6)|3;

    ans = I2C_Send(adrs,2,SendDat);//���M

    return ans;//���M��Ԃ�Ԃ�
}

unsigned char Shot_Up(void)//�T�C�h�X���[�̊p�x���グ��
{
	static const unsigned char adrs = Shot_Rad;//�A�h���X�p�ϐ�
    unsigned char ans;//���M�m�F�ϐ�
    unsigned char SendDat[2];//���M�f�[�^�p�z��

    SendDat[0] = Shot_Rad_Speed>>2;
    SendDat[1] = (Shot_Rad_Speed<<6)|1;

    ans = I2C_Send(adrs,2,SendDat);//���M

    return ans;//���M��Ԃ�Ԃ�
}
	
unsigned char Shot_Down(void)//�T�C�h�X���[�̊p�x��������
{
	static const unsigned char adrs = Shot_Rad;//�A�h���X�p�ϐ�
	unsigned char ans;//���M�m�F�ϐ�
	unsigned char SendDat[2];//���M�f�[�^�p�z��

    SendDat[0] = Shot_Rad_Speed>>2;
    SendDat[1] = (Shot_Rad_Speed<<6)|2;

    ans = I2C_Send(adrs,2,SendDat);//���M

    return ans;//���M��Ԃ�Ԃ�
}