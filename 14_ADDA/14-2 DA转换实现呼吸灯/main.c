#include <REGX52.H>
#include "Timer0.h"

sbit DA_pwm = P2^1;
unsigned int PWM_Peri = 100;
unsigned int PWM_Duty = 0;

//延时函数10ms
void Delay10ms(){//@11.0592MHz
	unsigned char i, j;
	i = 18; j = 235;
	do{
		while (--j);
	}while (--i);
}

void main(){
  unsigned char i;
  //初始化定时器
  Timer0_Init();
  while(1){
    for(i=0;i<PWM_Peri;i++){
      PWM_Duty = i;
      Delay10ms();
    }
    for(i=PWM_Peri;i>0;i--){
      PWM_Duty = i;
      Delay10ms();
    }
  }
}

// 定义定时器T0中断后要执行的动作
void Timer0_Routine() interrupt 1{
  static unsigned int count1_T0; //中断次数
  TH0 = 0xFF; TL0 = 0xA4; // 恢复溢出周期，近似100us
  //对于PWM所需的中断
  count1_T0++; //更新中断次数
  if(count1_T0>PWM_Peri){
    count1_T0 = 0;    
  }else{
    if(count1_T0<PWM_Duty){DA_pwm=0;}//电机运行
    else                  {DA_pwm=1;}//电机不运行
  }
}
