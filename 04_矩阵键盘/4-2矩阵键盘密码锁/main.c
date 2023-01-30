#include <REGX52.H>
#include "MatrixKey.h"
#include "LCD1602.h"

void main(){
  unsigned int correct = 123; // 正确的密码
  unsigned int password = 0; // 当前的密码值
  unsigned char key = 0; // 矩阵键盘当前按下的值
  // 显示屏初始化
  LCD_Init();
  LCD_ShowString(1,1,"Password:");
  LCD_ShowNum(2,1,0,4);
  while(1){
    key = MatrixKey();
    if(key==11){ // 按下确认键，则进行判断
      if(password == correct)
        LCD_ShowString(1,12,"True!");
      else
        LCD_ShowString(1,13,"Err!");
    }else if(key==12){ // 按下取消键，密码归零
      LCD_ShowString(1,12,"     "); // 清除对错提示
      password = 0;
      LCD_ShowNum(2,1,password,4);
    }else if(key>=1 && key<=10){ // 其他正常的数字键
      LCD_ShowString(1,12,"     "); // 清除对错提示
      key = (key==10) ? 0 : key; // 或者也可以对10取余
      password = (password%1000) * 10 + key;
      LCD_ShowNum(2,1,password,4);
    }
  }
}
