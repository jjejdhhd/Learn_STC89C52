#include <REGX52.H>
#include"LCD1602.h"

void main(){
  int a = 0; // 注意若编译器较老，那么不能中途定义变量
  
//  // 注意下面的代码不要放在while(1)里面，否则LCD会闪
//  LCD_Init();
//  LCD_ShowChar(1,1,'A');
//  LCD_ShowString(1,2,"HelloWorld");
//  LCD_ShowNum(1,12,666,3);
//  LCD_ShowSignedNum(1,15,-9,2);	
//  LCD_ShowHexNum(2,1,0xA8,2);
//  LCD_ShowBinNum(2,5,0xAA,8);
  
  // 下面是实际想用的场景:展示代码运行结果
  LCD_Init();
  while(1){
    LCD_ShowNum(1,1,a++,8);
  }
}

