#include <REGX52.H>
#include "LCD1602.h"
#include "Timer0.h"

unsigned char hour=23,minute=59,second=55; // 全局变量赋初值
void main(){
  // 初始化定时器T0
  Timer0_Init();
  // 初始化LCD
  LCD_Init();
  LCD_ShowString(1,1,"Clock:");
  LCD_ShowString(2,3,":  :"); // 注意这里可以先全部定义
  while(1){
    LCD_ShowNum(2,1,hour,2);
    LCD_ShowNum(2,4,minute,2);
    LCD_ShowNum(2,7,second,2);
  }
}

// 定义定时器T0的中断子函数
void Timer0_Routine() interrupt 1{
  static unsigned int counter_T0;
  counter_T0++;
  TH0 = 0xfc; TL0 = 0x66; // 离溢出近似1ms
  if(counter_T0>1000){
    counter_T0 = 0;
    if(second==59){
      second = 0;
      if(minute==59){
        minute = 0;
        if(hour==23){hour = 0;}
        else {hour+=1;}
      }else{
        minute+=1;
      }
    }else{
      second+=1;
    }
  }
}
