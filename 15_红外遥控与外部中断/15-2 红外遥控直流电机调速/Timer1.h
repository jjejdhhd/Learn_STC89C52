#ifndef __TIMER1_H__
#define __TIMER1_H__

#include <REGX52.H>
/**
  * @brief :对定时器1进行初始化，使定时器1计数、中断。
  * 注：对11.0592MHz进行12分频（脉冲周期1.0850694us）。
 */
void Timer1_Init(){
  // 配置定时器T1的相关寄存器
  TMOD&=0x0f; TMOD|=0x10; // 选择T1的GATE=0/允许计数/模式1
  TF1 = 0; TR1 = 1; // 溢出标志位清空，运行控制位置1
  TH1 = 0xfc; TL1 = 0x66; // 离溢出近似1ms
  // 配置中断寄存器
  EA = 1; ET1 = 1; // 不屏蔽所有中断，允许T0溢出中断
  PT1 = 0; // T1优先级保持默认，不写这句话也可以
}

/*中断函数模板
// 定义定时器T0中断后要执行的动作
void Timer1_Routine() interrupt 3{
  static unsigned int count_T1; //中断次数
  count_T1++; //更新中断次数
  TH1 = 0xfc; TL1 = 0x66; // 恢复溢出周期，近似1ms
  if(count_T1>500){
    count_T1 = 0;
    
  }  
}
*/
#endif