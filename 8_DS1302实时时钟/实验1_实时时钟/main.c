#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"

unsigned char DS1302_info[7];

void main(){
  unsigned char i=0;
  // 初始化LCD显示屏
  LCD_Init();
  LCD_ShowString(1,1,"2023-01-01");
  LCD_ShowString(2,1,"00:00:00");
  DS1302_Init();//初始化DS1302
  while(1){
    DS1302_ReadAll(&DS1302_info);
    LCD_ShowNum(1,3,DS1302_info[0],2);
    LCD_ShowNum(1,6,DS1302_info[1],2);
    LCD_ShowNum(1,9,DS1302_info[2],2);
    LCD_ShowDay(1,14,DS1302_info[3]);
    LCD_ShowNum(2,1,DS1302_info[4],2);
    LCD_ShowNum(2,4,DS1302_info[5],2);
    LCD_ShowNum(2,7,DS1302_info[6],2);
  }
}