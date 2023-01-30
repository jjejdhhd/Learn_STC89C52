#include <REGX52.H>
#include <INTRINS.H> // 使用循环左移函数_rcol_()
#include "Timer0.h"
#include "Delay.h"
#include "PushKey.h"

unsigned char LED_model = 0; // 定义流水灯方向，默认左移
void main(){
  unsigned char key = 0; // 按键开关
  Timer0_Init(); // 定时器T0初始化
  P2 = 0xfe; // 初始化流水灯
  while(1){
    key = PushKey();
	  if(key==1){LED_model = (LED_model==0)?1:0;}
  }
}


// 定义定时器T0中断后要执行的动作
void Timer0_Rountine() interrupt 1{
  static unsigned int count_T0; // 静态局部变量，生命周期与全局变量一样，但作用域仅限此函数
  count_T0++; // 对中断过程进行计数
  TH0 = 0xfc; TL0 = 0x66; // 距离下一次溢出近似1ms
  if(count_T0>500){
    count_T0 = 0;
    P2 = LED_model ? _cror_(P2,1) : _crol_(P2,1);
  }  
}
