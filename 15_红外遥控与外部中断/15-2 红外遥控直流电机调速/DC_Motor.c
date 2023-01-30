#include "Timer1.h"
#include "NixieTube.h"

#define motor P1_0
unsigned int PWM_Peri = 100;
unsigned int PWM_Duty = 0;

//电机初始化（主要是对PWM初始化）
void DC_Motor_Init(void){
  //初始化定时器
  Timer1_Init();
}

//调节电机（PWM）占空比
void DC_Motor_SetSpeed(unsigned char speed){
  unsigned char hunds,tens,ones;
  PWM_Duty = speed;
  ones = speed%10; speed = speed/10;
  tens = speed%10; speed = speed/10;
  hunds = speed%10;
  if(hunds==0){hunds = 10;}//如果百位是0，那数码管就不显示
  //数码管更新显示转速等级
  NixieTube_SetBuf(8,hunds);
  NixieTube_SetBuf(7,tens);
  NixieTube_SetBuf(6,ones);
}

/**********************************************/
// 定义定时器T1中断后要执行的动作
void Timer1_Routine() interrupt 3{
  static unsigned int count1_T1,count2_T1; //中断次数
  TH1 = 0xFF; TL1 = 0xA4; // 恢复溢出周期，近似100us
  //对于PWM所需的中断
  count1_T1++; //更新中断次数
  if(count1_T1>PWM_Peri){
    count1_T1 = 0;    
  }else{
    if(count1_T1<PWM_Duty){motor=1;}//电机运行
    else                  {motor=0;}//电机不运行
  }
  //数码管扫描所需的中断
  count2_T1++; //更新中断次数
  if(count2_T1>2){
    count2_T1 = 0;
    NixieTube_Loop();
  }
}