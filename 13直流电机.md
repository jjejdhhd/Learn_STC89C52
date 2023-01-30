# 13 直流电机

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
注：工程及代码文件放在了本人的[Github仓库](https://github.com/jjejdhhd/Learn_STC89C52)。
***

## 13.1 直流电机与PWM波
**13.1.1 直流电机**
直流电机是一种将电能转换为机械能的装置。一般的直流电机有两个电极，**当电极正接时，电机正转，当电极反接时，电机反转**。直流电机主要由永磁体（定子）、线圈（转子）和换向器组成。除直流电机外，常见的电机还有步进电机、舵机、无刷电机、空心杯电机等。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E7%9B%B4%E6%B5%81%E7%94%B5%E6%9C%BA%E5%AE%9E%E7%89%A9%E5%9B%BE.png" width=50%>
</div><div align=center>
图13-1 直流电机实物图
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E7%9B%B4%E6%B5%81%E7%94%B5%E6%9C%BA%E5%8E%9F%E7%90%86%E5%9B%BE%E5%8F%8A%E9%A9%B1%E5%8A%A8%E6%96%B9%E5%BC%8F.png" width=80%>
</div><div align=center>
图13-2 直流电机原理图及驱动方式
</div>

根据上述原理图可以看出，由于单片机的IO口没有驱动能力，所以还是依靠外接的ULN2003D芯片进行驱动。从OUT1~OUT4任选两个端口即可完成驱动。对于直流电机，主要有两种供电方式：
> 1. 大功率器件直接驱动：只能单向驱动。图中，续流二极管的作用是防止三极管截止时，直流电机中的电容击穿三极管。
> 2. H桥驱动：可以双向驱动。 
> 3. 当然，开发板上采用的**驱动芯片ULN2003D**，在之前的“9蜂鸣器”一节已经介绍过。


**13.1.2 PWM介绍**
UP主最开始自己接了电位器（滑动变阻器）来控制直流电机转速，而电源供电端保持电压不变。最后发现，只有当电阻非常小的时候直流电机才开始转。但是直流电机需要的电流大，很容易就使得变阻器发热甚至烧毁。所以此方法并不好，还是要控制电源端的变化，也就是下面的PWM。
PWM（Pulse Width Modulation）即脉冲宽度调制，**在具有惯性的系统中**，可以通过对一系列脉冲的宽度进行调制，来等效地获得所需要的模拟参量，常应用于电机控速、开关电源等领域。PWM重要参数：
> - 频率 = 1/T~S~。频率越高，平均电压波形的谐波含量就越低。但后续使用生成PWM时，定时器重装频率过快会使主函数执行效率大幅下降。一般设置10~20kHz。
> - 占空比 = T~ON~/T~S~
> - 精度 = 占空比变化步距

在软件层面，可以使用**纯软件延时的方式完成PWM波**，常用于“呼吸灯”等需要占空比频繁变化的场合；而对于电机调速这样的场合，期望程序在不调节时保持当前的占空比不变，**这种场合下更常用的方法是使用定时器产生PWM波**：
> 1. 对定时器进行计数，计数周期就是PWM波的周期。
> 2. 在PWM波发生函数中，设定一个比较阈值，用于控制占空比。
> 3. 显然，定时器的精度就是PWM波的精度。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/PWM%E6%B3%A2%E4%BA%A7%E7%94%9F%E5%8E%9F%E7%90%86.png" width=70%>
</div><div align=center>
图13-3 使用定时器产生PWM波原理
</div>


## 13.2 实验：LED呼吸灯
需求：全体LED以1秒为周期进行“呼吸式”亮度变化。
> 注：本实验建议纯软件延时完成PWM调节。即，只有一个主函数。

代码展示：
**- mian.c**
```c
#include <REGX52.H>

sbit LED0 = P2^0;
unsigned int PWM_Period = 100;
unsigned int PWM_Duty = 99;

// 延时函数一定要有，但单次延时应在10us以下
void Delay(unsigned int cycles){
	while(cycles--);
}

void main(){
  unsigned char i;
  P2 = 0xff; //LED初始全部熄灭
  while(1){
    for(PWM_Duty=0;PWM_Duty<PWM_Period;PWM_Duty++){
      for(i=0;i<10;i++){
        LED0 = 0;
        Delay(PWM_Duty);
        LED0 = 1;
        Delay(PWM_Period-PWM_Duty);
      }
    }
    for(PWM_Duty=PWM_Period-1;PWM_Duty>0;PWM_Duty--){
      for(i=0;i<10;i++){
        LED0 = 0;
        Delay(PWM_Duty);
        LED0 = 1;
        Delay(PWM_Period-PWM_Duty);
      }
    }
  }
}
```

编程感想：
> 1. 关于延时函数：TMD天坑啊！思路其实很简单，但是一定要注意呼吸灯的PWM频率一定要非常快！！量级在1KHz以上（由于PWM周期为100，所以意味着**延时函数在10us以下**）才会有一个很好的效果。否则就会发现灯亮一下又闪，大晚上一直盯着灯看眼都快瞎了……:confounded:

## 13.3 实验：直流电机调速
需求：设置直流电机转速等级：Stop、40%、50%、75%、100%。
> 每按下一次按键Key1，直流电机速度等级就上升一级（上升到最高归零）。
> LCD第一行显示“DC motor：”；LCD第二行显示“xxx% Power”，如果为0就<u>闪烁显示</u>“Stop!”。
> 注：综合考虑程序效率和硬件效果，可设置定时器中断周期100us，PWM周期为100个定时器中断。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8%E5%85%B3%E7%B3%BB-%E7%9B%B4%E6%B5%81%E7%94%B5%E6%9C%BA%E8%B0%83%E9%80%9F.png" width=20%>
</div><div align=center>
图13-4 “直流电机调速”代码调用
</div>

代码展示：
**-main.c**
```c
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
```

```LCD1602.h```、```LCD1602.c```与第12节的实验代码相同。```PushKey.h```、```Timer0.h```与13.2“温度报警器”的代码相同。


编程感想：
> 1. 只要直流电机连着，就没法下载程序。必须要拔掉直流电机下载好之后，断电插上直流电机，插电。推测是因为USB所需电流激增，导致电脑主板进行端口保护，就不再进行串口通信？？
> 2. 直流电机和LCD显示屏冲突：USB电源的电流一般不超过1000MA（一般限流在750mA），当电流大时，电压下降得厉害，不能启动LCD1602。 所以可以使用其他驱动电流小的元件（如数码管），来显示当前的速度等级。
> 3. 关于直流电机：注意电机引脚拉高时启动，因为**达林顿晶体管阵列是非门**。另外当PWM占空比过低时，也会导致直流电机平均启动电压过小而不转。
> 4. 下面是使用LED来指示转速的代码：

```c
//LCD更新显示转速等级
switch(speed_index){
    case 0:P2 = 0xff;break;
    case 1:P2 = 0xfc;break;
    case 2:P2 = 0xf0;break;
    case 3:P2 = 0xc0;break;
    case 4:P2 = 0x00;break;
    default:;
}
 ```

