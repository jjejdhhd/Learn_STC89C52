#include <REGX52.H>
#include "LCD1602.h"
#include "PushKey.h"
#include "AT24C02.h"

void main(){
  unsigned int num = 0;//要存储的数据
  unsigned char num_H8, num_L8;//数据的高8位和低8位
  unsigned char err1,err2;//表示操作的错误
  unsigned char key;
  //LCD初始化
  LCD_Init();
  //计算两个字节
  num_H8 = num/256;
  num_L8 = num%256;
  while(1){
    key = PushKey();
    if(key){
      switch(key){
        case 1: //数据加一
          num++;
          num_H8 = num/256; num_L8 = num%256;
          LCD_ShowString(2,1,"          ");
          break;
        case 2: //数据减一
          num--;
          num_H8 = num/256; num_L8 = num%256;
          LCD_ShowString(2,1,"          ");
          break;
        case 3: //数据存储
          err1 = AT24C02_WriteByte(0x02,num_H8);
          err2 = AT24C02_WriteByte(0x03,num_L8);
          if((!err1) && (!err2)){LCD_ShowString(2,1,"Write OK! ");}
          else                  {LCD_ShowString(2,1,"Write ERR!");}
          break;
        case 4: //数据读取
          num_H8 = AT24C02_ReadByte(0x02);
          num_L8 = AT24C02_ReadByte(0x03);
          num = num_H8*256 + num_L8;
          LCD_ShowString(2,1,"Read OK!"); 
          break;
        default:;
      }
    }
    LCD_ShowNum(1,1,num,5);
  }
}
