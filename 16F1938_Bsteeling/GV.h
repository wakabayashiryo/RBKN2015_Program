/************************************************

includeするヘッダファイルを管理するヘッダファイル
使いたいヘッダをincludeして、
ソースファイルとヘッダファイルをそれぞれ
Source Files と Header Filesに追加すればおｋ
************************************************/

#include <pic.h>
#include <htc.h>     
#define _XTAL_FREQ 32000000    // delay用に必要(クロック32MHzを指定)

#include "skI2C.h"             // I2C用ヘッダ


