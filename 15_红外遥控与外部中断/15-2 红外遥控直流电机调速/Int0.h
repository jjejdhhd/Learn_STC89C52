#ifndef __INT0_H__
#define __INT0_H__

//外部中断0的初始化函数
void Int0_Init(void){
  IT0 = 1;//下降沿触发
  IE0 = 0;//默认没有触发中断请求
  EX0 = 1;//允许外部中断0中断
  EA = 1;//全局中断允许
  PX0 = 1;//提升外部中断0的优先级
}

/*中断函数模板
//外部中断0中断后所需要执行的函数
void Int0_Routine(void) interrupt 0 {
  
}
*/
#endif