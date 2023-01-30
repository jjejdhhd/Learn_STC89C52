#include <REGX52.H>
#include "LCD1602.h"
#include "_1Wire.h"
#include "DS18B20.h"

void main(){
  float tempe = 10.5;
  //初始化LCD
  LCD_Init();
  LCD_ShowString(1,1,"Temperature:");
  while(1){
    DS18B02_ConvertTemp();// 温度转换
    tempe = DS18B02_ReadTemp();// 温度读取
//    LCD_ShowBinNum(2,1,tempe,16);//显示16位二进制数字
    if(tempe>0){LCD_ShowChar(2,1,'+');}
    else       {LCD_ShowChar(2,1,'-');tempe=-tempe;}
    LCD_ShowNum(2,2,(int)tempe,3);
    LCD_ShowString(2,5,".");
    LCD_ShowNum(2,6,(int)((tempe-(int)tempe)*10000),4);
    LCD_ShowString(2,11,"C");
  }
}
