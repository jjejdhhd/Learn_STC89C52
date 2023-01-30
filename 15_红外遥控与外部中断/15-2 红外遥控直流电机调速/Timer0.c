#include <REGX52.H>

//下面的函数实现正常计数中断
/*******************************************/
/**
  * @brief :对定时器0进行初始化，初始化完成后定时器0即可正常工作。
  * 注：对11.0592MHz进行12分频（脉冲周期1.0850694us）。
  * 注：配置过程中，由于掉电复位后中断都默认不开启，所以只需配置定时器0
  *     相关的寄存器即可，不要定义其他中断的寄存器，以保证程序的复用性。
 */
void Timer0_Init(void){
  // 配置定时器T0的相关寄存器
  TMOD&=0xf0; TMOD|=0x01; // 选择T0的GATE=0/允许计数/模式1
  // 上面这个方法目的是不干扰高四位，对低四位先清零再加值。
  TF0 = 0; TR0 = 1; // 溢出标志位清空，运行控制位置1
  TH0 = 0xfc; TL0 = 0x66; // 离溢出近似1ms
  // 注：上面这个初值只在第一次溢出生效，后面都是从0开始计数。
  // 配置中断寄存器
  EA = 1; ET0 = 1; // 不屏蔽所有中断，允许T0溢出中断
  PT0 = 0; // T0优先级保持默认，不写这句话也可以
}

/*中断函数模板
// 定义定时器T0中断后要执行的动作
void Timer0_Routine(void) interrupt 1{
  static unsigned int count_T0; //中断次数
  count_T0++; //更新中断次数
  TH0 = 0xfc; TL0 = 0x66; // 恢复溢出周期，近似1ms
  if(count_T0>500){
    count_T0 = 0;
    
  }  
}
*/
/*******************************************/

//下面的函数是为了实现计数的功能，不能中断
/*******************************************/
/**
* @brief :对定时器0进行初始化计数配置，不中断，默认不工作。
  * 注：对11.0592MHz进行12分频（脉冲周期1.0850694us）。
 */
void Timer0_CountInit(void){
  // 配置定时器T0的相关寄存器
  TMOD&=0xf0; TMOD|=0x01; //选择T0的GATE=0/允许计数/模式1
  TF0 = 0; //溢出标志位清空
  TR0 = 0; //运行控制位置0，先不工作
  TH0 = 0;
  TL0 = 0;
  // 配置中断寄存器
  ET0 = 0; //不允许T0溢出中断
}
//设置计数器0当前的值
void Timer0_SetCounter(unsigned int num_count){
  TH0 = num_count%256;
  TL0 = num_count/256;
}

//读取计数器0当前的值
unsigned int Timer0_GetCounter(void){
  unsigned int cur_counter=0;
  cur_counter = TH0*256 + TL0;
  return cur_counter;
}

//计数器0是否运行（1/0）
void Timer0_Run(unsigned char run_flag){
  TR0 = run_flag;
}
/*******************************************/