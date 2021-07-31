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

bit Shot_Flg,Sservo,Swait,Sair;//サーボ処理用変数
bit Lock_Flg,Lock,Lwait,Lair;//保持機構処理用変数
bit Side_Flg,Sdwait,Sdair;//サイドスロー処理用変数
bit Catap_Flg,Ctpwait,Ctpair;//カタパルト処置用変数

unsigned char phase;//動作カウント変数
unsigned char IOC5f;//phase3電磁弁開放フラグ用変数
unsigned int Wait_count;//タイマー０用カウント変数（保持機構ウェイト動作）
unsigned int Side_count;//タイマー０用カウント変数（サイドスローウェイト動作）
unsigned int Catap_count;//タイマー０用カウント変数（カタパルトウェイト動作）

////////////////////////////////////////駆動関係////////////////////////////////////
unsigned char Out_Motor(unsigned char ch,unsigned char dire,unsigned int Speed)//モーター出力用関数
{
    static const unsigned char adrs[4] = {MotrAdrs1,MotrAdrs2,MotrAdrs3,MotrAdrs4};//アドレス用配列
    unsigned char ans;//送信確認変数
    unsigned char SendDat[2];//送信データ用配列

    SendDat[0] = Speed>>2;//二つに分割して送信
    SendDat[1] = (Speed<<6)|dire;

    ans = I2C_Send(adrs[ch],2,SendDat);//送信
    
	return ans;//送信状態を返す
}

unsigned char Dead_Stop(unsigned int Speed)//全モーターをブレーキ状態にする
{
    static const unsigned char adrs[4] = {MotrAdrs1,MotrAdrs2,MotrAdrs3,MotrAdrs4};//アドレス用配列
    unsigned char SendDat[2];//送信データ用配列
    unsigned char ans = 0;//送信確認変数

    SendDat[0] = Speed>>2;
    SendDat[1] = (Speed<<6)|Brake;

    for(unsigned char i = 0;i<4;i++)
    {
        ans += I2C_Send(adrs[i],2,SendDat);//送信
    }
    return ans;//送信状態を返す
}

unsigned char Set_Str(unsigned char select,int rad)//指定した角度をステアリングに送信する//inputdata -256^+256
{
    static const unsigned char adrs[4] = {SteeAdrs1,SteeAdrs2,SteeAdrs3,SteeAdrs4};//アドレス用配列
    unsigned char SteeDat[2];//送信データ用配列
	unsigned char ans;//送信確認変数
    
    SteeDat[0] = (rad+256)&0xff;//lower bit
    SteeDat[1] = (rad+256)>>8;//higher bit

    ans = I2C_Send(adrs[select],2,SteeDat);//送信
    
    return ans;//送信状態を返す
}

unsigned char Encorder_Str(unsigned char select,int rad)//指定した角度をステアリングに送信する+2048~-2048
{
    static const unsigned char adrs[4] = {SteeAdrs1,SteeAdrs2,SteeAdrs3,SteeAdrs4};//アドレス用配列
    unsigned char SteeDat[2];//送信データ用配列
	unsigned char ans;//送信確認変数
    
    SteeDat[0] = (rad+0x07ff)>>8;//higher bit
    SteeDat[1] = (rad+0x07ff)&0x00ff;//lower bit

    ans = I2C_Send(adrs[select],2,SteeDat);//送信
    
    return ans;//送信状態を返す
}
////////////////////////////////////////エアー関係/////////////////////////////////////
unsigned char On_Valve(unsigned char select)//指定されたポートの電磁弁をHIGHにする
{
    static const unsigned char add = AirValve;//アドレス用変数
    unsigned char ans;//送信確認変数
    unsigned char OnData = 1;//送信データ用変数
    
    if(select==1)//電磁弁の開放状況をマスターのLEDに表示
        LED_GREEN1 = 1;
    else if(select==2)
        LED_GREEN2 = 1;
    else if(select==3)
        LED_BLUE1 = 1;
    else if(select==4)
        LED_BLUE2 = 1;
    
    OnData |= select<<4;

    ans = I2C_Send(add,2,&OnData);//送信

    return ans;//送信状態を返す
}

unsigned char Off_Valve(unsigned char select)//指定されたポートの電磁弁をLOWにする
{
    static const unsigned char add = AirValve;//アドレス用変数
    unsigned char ans;//送信確認変数
    unsigned char OffData = 0;//送信データ用変数
    
    if(select==1)//電磁弁の開放状況をマスターのLEDに表示
        LED_GREEN1 = 0;
    else if(select==2)
        LED_GREEN2 = 0;
    else if(select==3)
        LED_BLUE1 = 0;
    else if(select==4)
        LED_BLUE2 = 0;
    
    OffData |= select<<4;
  
    ans = I2C_Send(add,2,&OffData);//送信

    return ans;//送信状態を返す
}
///////////////////////////////////////世紀末関係//////////////////////////////////////
unsigned char Seikimatsu_ON(void)//世紀末を動作させる
{
    static const unsigned char adrs = SeikimatsuAdrs;//アドレス用変数
    unsigned char ans;//送信確認変数
	unsigned char ON; //送信データ用変数
   
    ON = 1;

    ans = I2C_Send(adrs,1,&ON);//送信
    
    return ans;//送信状態を返す
}

unsigned char Seikimatsu_OFF(void)//世紀末を停止させる
{
    static const unsigned char adrs = SeikimatsuAdrs;//アドレス用変数
    unsigned char ans;//送信確認変数
	unsigned char OFF;//送信データ用変数
    
    OFF = 0;

    ans = I2C_Send(adrs,1,&OFF);//送信
    
    return ans;//送信状態を返す
}
///////////////////////////////////////表示器関係/////////////////////////////////
unsigned char System_Indicater(unsigned char Color,unsigned char LedStat,unsigned char Brightness)//デバッグボードのLEDを指定する
{
    static const unsigned char Adrs = SysIndi_Adrs;//アドレス用変数
    unsigned char SendDat[3];//送信データ用配列
    unsigned char ans;//送信確認変数
    
    SendDat[0] = Color;
    SendDat[1] = Brightness;
    SendDat[2] = LedStat;
    
    ans = I2C_Send(Adrs,3,SendDat);//送信
    
	return ans;//送信状態を返す
}
//フルカラーインディケーター用データ送信関数
void System_Indicater2(unsigned char data1,unsigned char data2,unsigned char data3,unsigned char data4,unsigned char data5)
{
	unsigned char SendData=0;//UART送信用変数
	
	SendData |= data1;//zoneデータ
	SendData |= data2<<2;//chargeデータ
	SendData |= (data3&0x03)<<5;//Cataデータ
	
	UART_Write(SendData);//表示器にUART経由で送信
}
///////////////////////////////////保持機構関係////////////////////////////////
unsigned char Set_Lock(unsigned int set)////指定した角度を保持機構に送信する
{
    static const unsigned char adrs = LockAdrs;//アドレス用変数
    unsigned char LockDat[2];//送信データ用配列
	unsigned char ans;//送信確認変数
    
    LockDat[0] = set&0xff;//lower bit
    LockDat[1] = set>>8;//higher bit

	ans = I2C_Send(adrs,2,LockDat);//送信
    
    return ans;//送信状態を返す
}

void Lock_On(void)//保持機構動作開始関数
{
    if(Lock_Flg)return;//動作中は入力無視
    Lock_Flg = 1;//保持機構動作フラグを立てる
    Lair = 1;//電磁弁フラグを立てる
    Wait_count = 0;//ウェイトカウントをクリアー
}

void Lock_Acti(void)//保持機構の動作
{
	static const unsigned int ServoRad[Radnum] = {10,60,120,120};//サーボに送信する角度データ
    
    if(phase==Radnum)phase = 0;//動作カウントをリセット
    
	Out_Servo(0,ServoRad[phase]);//サーボに角度を送信
    
	if(Lock_Flg)//保持機構動作フラグを立っていたら
    {
        if(Lair)//電磁弁開放時間処理
        {
            if(Wait_count>LockOnTime1&&phase==0)//phaseが0で16000カウント分時間がたったら。
            {
               	Lair = 0;//電磁弁フラグを下げる
               	Lwait = 1;
				Wait_count = 0;
			}
            else if(Wait_count>LockOnTime2&&phase==1)//phaseが1で8000カウント分時間がたったら。
            {
               	Lair = 0;//電磁弁フラグを下げる
               	Lwait = 1;
				Wait_count = 0;
			}
            else if(IOC5f&&phase==2)//RB5の状態変化割り込みが起きるまで電磁弁を開放
            {
                Lair = 0;//電磁弁フラグを下げる
               	Lwait = 1;
				Wait_count = 0;
			}
			else if(Wait_count>LockOnTime4&&phase==3)//phaseが3で4000カウント分時間がたったら
			{
				Lair = 0;//電磁弁フラグを下げる
				Lwait = 1;
				Wait_count = 0;
			}
            else//電磁弁をhighにする時間
            {
                if(phase==3)//動作カウントが３である場合電磁弁は動作しない
                    Off_Valve(1);
                else//上記以外では、電磁弁を動作させる
                    On_Valve(1);
            }
        }
		
        if(Lwait)
        {
			Off_Valve(1);//電磁弁を閉じる
		
			if(Wait_count>LockOffTime)//三連パラレル収縮時のインターバル
			{
				Lwait = 0;
				Lock = 1;
			}
		}
		
		if(Lock)
		{
			Lock = 0;
			Lock_Flg = 0;//保持機構動作終了
			phase++;//動作カウントをインクリメント 
        } 
    }
    else //動作フラグが立っていなければ電磁弁はlow
    {
        Off_Valve(1);//AirValve is disable.
    }
}

void Side_On(void)
{
    Side_Flg = 1;//保持機構動作フラグを立てる
    Side_count = 0;//ウェイトカウントをクリアー	
}

void Side_Acti(void)
{
	if(Side_Flg)
	{
		if(Side_count<SideOnTime)
		{
			On_Valve(2);//PORT2の電磁弁をHIGHにする。(サイドスローの電磁弁)
		}
		else
		{
			Side_Flg = 0;
			Off_Valve(2);//PORT2の電磁弁をLOWにする。
		}
	}
	else
	{
		Off_Valve(2);//PORT2の電磁弁をLOWにする。
	}
}
void Catap_On(void)
{
    Catap_Flg = 1;//保持機構動作フラグを立てる
    Catap_count = 0;//ウェイトカウントをクリアー	
}

void Catap_Acti(void)
{
	if(Catap_Flg)
	{
		if(Catap_count<CtpOnTime)
		{
			On_Valve(4);//PORT4の電磁弁をHIGHにする。(カタパルトの電磁弁)
		}
		else
		{
			Catap_Flg = 0;
			Off_Valve(4);//PORT4の電磁弁をLOWにする。
		}
	}
	else
		Off_Valve(4);//PORT4の電磁弁をLOWにする。
	
}
unsigned char Out_Servo(unsigned char port,unsigned char rad)//サーボの角度を送信する//rcvdata[0] = portdata,rcvdata[1] = raddata
{
    unsigned char add = ServoAdrs1;//アドレス用変数
    unsigned char ans;//送信確認変数
    unsigned char SendDat[2];//送信データ用配列

    SendDat[0] = port;
    SendDat[1] = rad;

    ans = I2C_Send(add,2,SendDat);//送信

    return ans;//送信状態を返す
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

///////////////////////////////////サイドスロー角度調整機構///////////////////////////////
unsigned char Shot_Stop(void)//サイドスローをホールド
{
	static const unsigned char adrs = Shot_Rad;//アドレス用変数
    unsigned char ans;//送信確認変数
    unsigned char SendDat[2];//送信データ用配列

    SendDat[0] = Shot_Rad_Speed>>2;
    SendDat[1] = (Shot_Rad_Speed<<6)|3;

    ans = I2C_Send(adrs,2,SendDat);//送信

    return ans;//送信状態を返す
}

unsigned char Shot_Up(void)//サイドスローの角度を上げる
{
	static const unsigned char adrs = Shot_Rad;//アドレス用変数
    unsigned char ans;//送信確認変数
    unsigned char SendDat[2];//送信データ用配列

    SendDat[0] = Shot_Rad_Speed>>2;
    SendDat[1] = (Shot_Rad_Speed<<6)|1;

    ans = I2C_Send(adrs,2,SendDat);//送信

    return ans;//送信状態を返す
}
	
unsigned char Shot_Down(void)//サイドスローの角度を下げる
{
	static const unsigned char adrs = Shot_Rad;//アドレス用変数
	unsigned char ans;//送信確認変数
	unsigned char SendDat[2];//送信データ用配列

    SendDat[0] = Shot_Rad_Speed>>2;
    SendDat[1] = (Shot_Rad_Speed<<6)|2;

    ans = I2C_Send(adrs,2,SendDat);//送信

    return ans;//送信状態を返す
}