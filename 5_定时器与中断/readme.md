# 5 定时器与中断

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
注：工程及代码文件放在了本人的[Github仓库](https://github.com/jjejdhhd/Learn_STC89C52)。
***

## 5.1 定时器原理与中断系统
**5.1.1 定时器原理**
CPU的时序指标有：
> 1. 振荡周期：为单片机提供定时信号的振荡源的周期（晶振周期或外加振荡周期）
> 2. 状态周期：2个振荡周期为1个状态周期，用S表示。振荡周期又称S周期或时钟周期。
> 3. 机器周期：1个机器周期含6个状态周期，12个振荡周期。
> 4. 指令周期：完成1条指令所占用的全部时间，它以机器周期为单位。
> 
> 例：外接晶振为12MHz时，51单片机相关周期的具体值为：振荡周期=1/12us；状态周期=1/6us；机器周期=1us；指令周期=1~4us。



51单片机的 **定时器** 也叫 **计数器**，属于单片机的 <u>内部资源</u>，其电路的连接和运转均在单片机内部完成。定时器的主要作用有：
> 1. 用于计时系统，可实现软件计时，或者使程序每隔一固定时间完成一项操作。
> 2. 替代长时间的Delay，提高CPU的运行效率和处理速度。
> 3. ...

STC89C52有 **3个定时器** （T0、T1、T2）。T0和T1与传统的51单片机兼容，T2是此型号单片机增加的资源。
注意：定时器的资源和单片机的型号是关联在一起的，不同的型号可能会有不同的定时器个数和操作方式，但一般来说，T0和T1的操作方式是所有51单片机所共有的。

定时器本质就是一个计数器，对计数脉冲进行计数（这个计数脉冲可以是系统时钟、外部引脚信号等），当计数器数值增加到 **最高位溢出**，计数单元就会向中断系统发出 **中断申请**（下一小节介绍），使程序跳转到中断服务函数中执行。如下图所示：



STC89C52的 **T0和T1定时器** 均有四种工作模式：
> 模式0：13位定时器/计数器（极不常用）
> 模式1：16位定时器/计数器 **（常用）**
> 模式2：8位自动重装模式（常用，串口通信波特率设置），比模式1更精确
> 模式3：两个8位计数器（极不常用）

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E5%AE%9A%E6%97%B6%E5%99%A8%E6%A8%A1%E5%BC%8F1%E6%A1%86%E5%9B%BE.png" width=80%>
</div><div align=center>
图5-2 定时器T1的工作模式1框图
</div>

> - SYSclk：系统时钟。后面接的12分频和6分频，可以在烧录软件（STC-ISP）中进行设置。
> - T1 Pin：外部引脚，根据单片机引脚说明图，为P3.5。T0为P3.4。
> - C/T：选择控制位，0为定时模式，对系统时钟进行计数；1为计数模式，对外部引脚进行计数。
> - [TH1, TL1]：16位计数的核心单元。
> - TF1：当计数器最高位溢出时，发出中断指令。
> - TR1、GATE、INT1为一些控制位，5.1.3小节介绍。




**5.1.2 中断系统**
中断系统是为使CPU具有对外界 **紧急事件的实时处理能力** 而设置的，很多外设都会用到。中断系统可以使CPU暂时放下手中的活（保护现场），优先处理中断指令给出的任务，处理完了之后再继续去处理之前的任务。执行中断指令的任务时，可以再次中断，形成中断嵌套。中断主要由以下优点：
> 1. 解决了快速主机与慢速I/O设备的数据传送问题；
> 2. 分时操作。CPU可以分时为多个I/O设备服务，提高了计算机的利用率；
> 3. 实时响应。CPU能够及时处理应用系统的随机事件，系统的实时性大大增强；
> 4. 可靠性高。CPU具有处理设备故障及掉电等突发性事件能力，从而使系统可靠性提高。


 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%B8%AD%E6%96%AD%E8%BF%87%E7%A8%8B%E7%A4%BA%E6%84%8F%E5%9B%BE.png" width=35%>
</div><div align=center>
图5-3 单级中断过程示意图
</div>

中断的资源和单片机的型号是关联在一起的，不同的型号可能会有不同的中断资源，例如中断源个数不同、中断优先级个数不同等等。STC89C52具有 **8个中断源**：外部中断0、定时器0中断、外部中断1、定时器1中断、串口中断、定时器2中断、外部中断2、外部中断3。每个中断源都具有<u>4个中断优先级</u>和独立的<u>开中断/关中断控制信号</u>，都可以通过软件设置。

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%B8%AD%E6%96%AD%E7%B3%BB%E7%BB%9F%E7%BB%93%E6%9E%84%E5%9B%BE.png" width=60%>
</div><div align=center>
图5-4 中断系统结构图
</div>

> 中断触发条件：
> - 4种外部中断（INT0、INT1、INT2、INT3）：低电平/下降沿触发。
> - 3种定时器中断（T0、T1、T2）：最高位溢出触发。
> - 串口中断（UART）：发送或接收完成。

如果使用C语言编程，中断查询次序号就是中断号，例如下面就是8种中断后要执行的中断子程序定义：
```c
// 中断后，就执行下面这些中断子程序
void Int0_Routine(void)    interrupt 0 { 函数体 }
void Timer0_Routine(void) interrupt 1 { 函数体 }
void Int1_Routine(void)    interrupt 2 { 函数体 }
void Timer1_Rountine(void) interrupt 3 { 函数体 }
void UART_Routine(void)    interrupt 4 { 函数体 }
void Timer2_Routine(void)  interrupt 5 { 函数体 }
void Int2_Routine(void)    interrupt 6 { 函数体 }
void Int3_Routine(void)    interrupt 7 { 函数体 }
```




**5.1.3 定时器和中断系统**
寄存器是连接软硬件的媒介，单片机通过配置寄存器来控制内部电路的链接。在单片机中，寄存器就是一段特殊的RAM存储器，不仅可以存储和读取数据，也可以控制电路的连接方式。寄存器相当于一个复杂机器的“操作按钮”。

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E5%AE%9A%E6%97%B6%E5%99%A8%E7%9B%B8%E5%85%B3%E5%AF%84%E5%AD%98%E5%99%A8.png" width=80%>
</div><div align=center>
图5-5 定时器相关寄存器
</div>

> - **定时器控制寄存器TCON** (Timer Control，可位寻址)：
> TF1：定时器T1溢出标志位。最高位溢出时置1，发出中断请求，直到CPU响应该请求时，硬件归0。一般只是**读**这一位。
> TR1：定时器T1运行控制位。软件控制定时器T1是否计数。GATE=0，TR1控制允许/禁止计数（1/0）；GATE=1，TR1=1且INT1=1（无外部中断1）才允许计数。
> TF0：定时器T0溢出标志位。同TF1。
> TR0：定时器T0运行控制位。同TR1，只是换成INT0。
> IE1：外部中断1（int1/P3.3）请求源标志位。IE1=1时外部中断1请求中断，只有CPU响应该请求时归0。
> IT1：外部中断1（int1/P3.3）触发控制位。控制中断触发条件是低电平/下降沿触发（0/1）。
> IE0：外部中断0（int0/P3.2）请求源标志位。同IE1。
> IT0：外部中断0（int0/P3.2）触发控制位。同IT1。
>
> - **定时器模式寄存器TMOD** (Timer Model，不可位寻址)：高四位控制定时器T1，低四位控制定时器T0。
> GATE：影响相应的定时器是否计数（见上）。
> C/T：控制使用内部时钟/外部引脚作为计数脉冲（0/1）。
> [M1,M0]：选择定时器的4种工作模式（模式1为[0,1]）。
>
> - **[TH1,TL1]**、**[TH1,TL1]**：存储计数的16位寄存器。
> 
> 
 
 
 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%B8%AD%E6%96%AD%E5%AF%84%E5%AD%98%E5%99%A8.png" width=80%>
</div><div align=center>
图5-6 中断寄存器
</div>

> - **中断允许寄存器IE** (Interrupt Enable，可位寻址)：
> EA：CPU的总中断允许控制位。控制开放/屏蔽所有中断（1/0）。
> ET2、ET1、ET0：3个定时器的溢出中断允许位。
> ES：1个串口中断允许位。
> EX1、EX0：外部中断1和外部中断0的中断允许位。
> - **辅助中断控制寄存器XICON** (Auxiliary Interrupt Control，可位寻址)：
> EX3、EX2：外部中断3和外部中断2的中断允许控制位。
> IE3、IE2：外部中断3和外部中断2的中断请求标志位。
> IT3、IT2：外部中断3和外部中断2的中断触发方式。1表示下降沿触发；0表示低电平触发。
> - **中断优先级控制寄存器高IPH**（不可位寻址）、**中断优先级控制寄存器IP低**（可位寻址）、**XICON**：
> [PX3H,PX3]、[PX2H,PX2]、[PX1H,PX1]、[PX0H,PX0]：4个外部中断的优先级控制位。
> [PT2H,PT2]、[PT1H,PT1]、[PT0H,PT0]：3个定时器优先级控制位。
> [PSH,PS]：1个串口优先级控制位。
> 
> 注：“不可位寻址”的含义就是，编程时只能整体赋值，而不能单独对某一位进行操作。

总结一下，要实现中断，主要关心的有3类信号：
> 1. 定时器的相关寄存器：把和所需中断源相关的信号都配置一遍。
> 2. 中断允许：全局中断允许、当前中断源允许。
> 3. 选配信号。比如优先级、触发方式等。
> 
> 注：更多关于“中断系统”和“定时器”的详细介绍，可以参考 器件手册。


## 5.2 实验：按键控制LED流水灯模式
需求：按下独立按键K1后，LED流水灯转向。要求使用定时器完成功能。
注：使用软件延时，由于流水灯的延时，按键需要按下足够长的时间才能被检测到。但是定时器独立于程序外，其中断信号可以很准确地给出，进而使得按键可以很灵敏地改变流水灯的方向。

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E6%8C%89%E9%94%AE%E6%8E%A7%E5%88%B6%E6%B5%81%E6%B0%B4%E7%81%AF%E5%87%BD%E6%95%B0%E8%B0%83%E7%94%A8.png" width=20%>
</div><div align=center>
图5-7 “按键控制流水灯”代码调用关系
</div>

代码展示：
**- main.c**
```c
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
void Timer0_Routine() interrupt 1{
  static unsigned int count_T0; // 静态局部变量，生命周期与全局变量一样，但作用域仅限此函数
  count_T0++; // 对中断过程进行计数
  TH0 = 0xfc; TL0 = 0x66; // 距离下一次溢出近似1ms
  if(count_T0>500){
    count_T0 = 0;
    P2 = LED_model ? _cror_(P2,1) : _crol_(P2,1);
  }  
}
```

**- Timer0.h**
```c
#ifndef __TIMER0_H__
#define __TIMER0_H__

#include <REGX52.H>
/**
  * @brief :对定时器0进行初始化，初始化完成后定时器0即可正常工作。
  * 注：对11.0592MHz进行12分频（脉冲周期1.0850694us）。
  * 注：配置过程中，由于掉电复位后中断都默认不开启，所以只需配置定时器0
  *     相关的寄存器即可，不要定义其他中断的寄存器，以保证程序的复用性。
 */
void Timer0_Init(){
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

#endif
```

**- PushKey.h**
```c
#ifndef __PUSHKEY_H__
#define __PUSHKEY_H__

#include "Delay.h"
/**
  * @brief :检测按下了哪个按键开关
  * @param :无
  * @retval :输出按键开关编号1~4，不按返回0，松开触发
 */
unsigned char PushKey(){
  unsigned char key = 0;
  if(!P3_1){Delay(10);while(!P3_1);Delay(10);key=1;}
  else if(!P3_0){Delay(10);while(!P3_1);Delay(10);key=2;}
  else if(!P3_2){Delay(10);while(!P3_1);Delay(10);key=3;}
  else if(!P3_3){Delay(10);while(!P3_1);Delay(10);key=4;}
  return key;
}

#endif
```

**- Delay.h**
```c
#ifndef __DEALY_H_
#define __DEALY_H_

// 延时cycles ms，晶振@11.0592MHz
void Delay(unsigned int cycles){
  unsigned char i, j;
  do{
    i = 2;
    j = 199;
    do{
      while (--j);
    }while (--i);
  }while(--cycles);
}

#endif
```

代码评价：
> 本工程中，没有将函数的声明与定义分开写。本质上是因为头文件只包含一个函数声明，显然是太浪费了，文件太多也不好管理。
> 
STC-ISP软件中也有小程序：“定时器计算器”，可以帮助给出定时器的相关寄存器配置，但是缺少中断寄存器配置（使能全局中断、当前中断，如```EA =1; ET0=1;```）。偷懒的时候可以借鉴一下。

**进一步讨论：**
> - 执行中断的时候是并发吗？
> 51单片机属8位单核心单进程，不具有“并发”的能力，但是中断定时器具有独立计数的能力。遇到定时器给出的中断信号后，CPU将当前程序压栈，系统进入中断服务程序，中断处理完后再弹栈，继续当前程序。


## 5.3 实验：定时器时钟
需求：在LCD1602第一行显示“clock:”，第二行显示时间，格式为“时:分:秒”，要求使用定时器实现。

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E5%AE%9A%E6%97%B6%E5%99%A8%E6%97%B6%E9%92%9F%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8%E5%85%B3%E7%B3%BB.png" width=20%>
</div><div align=center>
图5-8 “定时器时钟”代码调用关系
</div>

代码展示：
**- main.c**
```c
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
```

**```Timer0.h```** 代码同5.2节“按键控制LED流水灯”实验，**```LCD1602.h```**、**```LCD1602.c```** 代码同3.2节“LCD1602调试工具”中相应的代码。

代码评价：
> 注意不要在中断函数中执行过程任务！也就是中断函数一般就执行一些简短的任务，以**防止颠倒主次**。类似于LCD显示这样耗费时间较长的任务最好不要放在中断函数中。
> ```！```是逻辑取反，```~```是按位取反。
