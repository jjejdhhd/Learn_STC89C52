#include <REGX52.H>
#include "LCD1602.h"

void Delay500ms(){//@11.0592MHz
	unsigned char i, j, k;
	i = 4;
	j = 129;
	k = 119;
	do{
		do{
			while (--k);
		}while (--j);
	}while (--i);
}

void main(){
  //LCD初始化
  LCD1602_Init();
  // LCD显示
  LCD1602_DispChar(1,1,'A');//显示单个字符
  LCD1602_DispString(1,3,"$_$");//显示字符串
  LCD1602_DispUnInt(1,7,99,3);//显示无符号数字
  LCD1602_DispInt(1,11,-32768,6);//显示有符号数字
  LCD1602_DispUnInt_Hex(2,1,255,2);//显示无符号整型的16进制表示
  LCD1602_DispUnInt_Bin(2,4,3,3);//显示无符号整型的2进制表示
  LCD1602_DispFloat(2,8,-20.99,7,3);//显示单精度浮点数-20.99
  LCD1602_DispChar(2,15,0xdf);//显示特殊符号（主要就是查看字模表）
  //LCD移屏
  LCD1602_DispString(1,20,"Fries on the pier!");
  while(1){
    LCD1602_WriteCommd(0x18);
    Delay500ms();
    Delay500ms();
  }
}