#include <REGX52.H>
#include "LCD1602.h"
#include "PushKey.h"
#include "Timer0.h"

#define motor P1_0
unsigned int PWM_Peri = 100;
unsigned int PWM_Duty = 0;
unsigned char speed_motor[5] = {0,40,50,75,100};

void main(){
  unsigned char key_num=0;//按键的编号
  unsigned char speed_index=0;//转速等级索引
  //LCD1602初始化
  LCD1602_Init();
  LCD1602_DispString(1,1,"DC motor:");
  LCD1602_DispString(2,1,"Stop!");
  //初始化定时器
  Timer0_Init();
  
  while(1){
    key_num = PushKey_GetKey();
    if(key_num==1){
      speed_index++;
      speed_index%=5;
      //调整PWM占空比（转速）
      PWM_Duty = speed_motor[speed_index];  
    }
    //LCD更新显示转速等级
    switch(speed_index){
      case 0:LCD1602_DispString(2,1,"Stop!     ");break;
      case 1:LCD1602_DispString(2,1," 40% Power");break;
      case 2:LCD1602_DispString(2,1," 50% Power");break;
      case 3:LCD1602_DispString(2,1," 75% Power");break;
      case 4:LCD1602_DispString(2,1,"100% Power");break;
      default:;
    }
  }
}

// 定义定时器T0中断后要执行的动作
void Timer0_Routine() interrupt 1{
  static unsigned int count1_T0,count2_T0; //中断次数
  TH0 = 0xFF; TL0 = 0xA4; // 恢复溢出周期，近似100us
  //对于PWM所需的中断
  count1_T0++; //更新中断次数
  if(count1_T0>PWM_Peri){
    count1_T0 = 0;    
  }else{
    if(count1_T0<PWM_Duty){motor=1;}//电机运行
    else                  {motor=0;}//电机不运行
  }
  //按键检测所需的中断
  count2_T0++; //更新中断次数
  if(count2_T0>20){
    count2_T0 = 0;
    PushKey_Loop();
  }
}
