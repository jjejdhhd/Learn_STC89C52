#ifndef __TIMER0_H__
#define __TIMER0_H__

//下面的函数实现正常计数中断
void Timer0_Init(void);//定时器0正常初始化

//下面的函数是为了实现计数的功能，不能中断
void Timer0_CountInit(void);//定时器0计数初始化
void Timer0_SetCounter(unsigned int num_count);//设置计数器0当前的值
unsigned int Timer0_GetCounter(void);//读取计数器0当前的值
void Timer0_Run(unsigned char run_flag);//计数器0是否运行（1/0）

#endif