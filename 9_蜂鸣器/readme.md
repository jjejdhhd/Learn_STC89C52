# 9 蜂鸣器

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
注：工程及代码文件放在了本人的[Github仓库](https://github.com/jjejdhhd/Learn_STC89C52)。
***

## 9.1 蜂鸣器简介
**蜂鸣器** 是一种将电信号转换为声音信号的器件，常用来产生设备的按键音、报警音等提示信号。蜂鸣器按驱动方式可分为有源蜂鸣器和无源蜂鸣器：
> 有源蜂鸣器：内部自带振荡源，将正负极接上直流电压即可持续发声，频率固定。
> 无源蜂鸣器：内部不带振荡源，需要控制器提供振荡脉冲（一般是PWM波）才可发声，调整提供振荡脉冲的频率，可发出不同频率的声音。
> 注：两者外观几乎没有差别，购买的时候要注意。
> 再注：单片机上自带的蜂鸣器为**无源蜂鸣器**。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E8%9C%82%E9%B8%A3%E5%99%A8%E5%AE%9E%E7%89%A9%E5%9B%BE.png" width=40%>
</div><div align=center>
图9-1 DS1302实物图
</div>

由于单片机引脚的驱动能力弱，所以一般不能直接用于驱动信号，而是作为开关信号来控制驱动能力更强的信号，从而驱动元器件正常工作。驱动蜂鸣器的方式主要有两种：三极管驱动（图8-2）、集成电路驱动（图8-3）。单片机采用集成电路芯片 **达林顿晶体管阵列 ULN2003D** 驱动蜂鸣器。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E8%9C%82%E9%B8%A3%E5%99%A8-%E4%B8%89%E6%9E%81%E7%AE%A1%E9%A9%B1%E5%8A%A8.png" width=45%>
</div><div align=center>
图9-2 蜂鸣器-三极管驱动方式
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E8%9C%82%E9%B8%A3%E5%99%A8-%E9%9B%86%E6%88%90%E7%94%B5%E8%B7%AF%E9%A9%B1%E5%8A%A8.png" width=45%>
</div><div align=center>
图9-3 蜂鸣器-集成电路驱动方式
</div>

达林顿管又称复合管。他将两个三极管串联，以组成一只等效的新的三极管。这只等效三极管的放大倍数是原二者之积，因此它的特点是 **放大倍数非常高**。<u>达林顿管的作用一般是在高灵敏的放大电路中放大非常微小的信号</u>，如大功率开关电路。在电子学电路设计中，达林顿接法常用于功率放大器和稳压电源中。**达林顿晶体管相当于非门**，但是输出的“1”是高阻态，没有驱动能力；输出的“0”则有驱动能力。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/ULN2003%E9%80%BB%E8%BE%91%E6%A1%86%E5%9B%BE.png" width=55%>
</div><div align=center>
图9-4 ULN2003逻辑框图
</div>

下面来分析乐理。无源蜂鸣器只有在由通路变成断路时才会发出声音，反之由断路变成通路并不发出声音。根据电路原理图可知，只要控制P2_5下降沿的频率就可以控制蜂鸣器发出不同的声音，而调节占空比则可以控制蜂鸣器的音量。下图8-5中，按照一组音调依次分成4组：大字组、小字组、小字1组、小字2组。组与组之间相差“8度”；相邻的两个键之间（包括白键与黑键）相差半音； <u>数字简谱左上标“#”表示升高半音，“b”表示降低半音，升降号在本小节有效。</u> 下图8-6给出了“小字组、小字1组、小字2组”音符与频率的对应关系。也就是说，**每一个音符都对应了一个特定的频率。**
而为了按照正常的节奏演奏出整首音乐，还需要调整每个音符所占用的时间。下图8-7给出了不同的音符的时值比，可以看出<u>在音符的右侧和下面加横线可以实现2的幂次的调整，而在右侧加点则表示乘以1.5倍。</u> **一般设置四分音符的持续时间为500ms。**
于是根据下面两张图，整体思路如下：**钢琴五线谱→数字简谱→蜂鸣器频率。** <u>通过不断改变定时器的复位值，进而控制蜂鸣器的频率变化。</u>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E9%94%AE%E7%9B%98%E4%B8%8E%E9%9F%B3%E7%AC%A6%E5%AF%B9%E7%85%A7.png" width=60%>
</div><div align=center>
图9-5 钢琴谱与数字简谱的转换关系
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/C%E8%B0%83%E9%9F%B3%E7%AC%A6%E4%B8%8E%E9%A2%91%E7%8E%87%E5%AF%B9%E7%85%A7%E8%A1%A8.png" width=35%>
</div><div align=center>
图9-6 数字简谱与蜂鸣器频率对应关系
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E9%9F%B3%E7%AC%A6.png" width=45%>
</div><div align=center>
图9-7 不同的音符的时值比
</div>



## 9.2 实验：蜂鸣器播放提示音
需求：对于四个独立按键开关，每当按键按下，蜂鸣器给出提示音，并且在数码管上显示按下的按键值。

本实验使用软件延时，完成蜂鸣器的频率调整和蜂鸣持续时间。代码如下：

**- main.c**
```c
#include <REGX52.H>
#include "PushKey.h"
#include "NixieTube.h"
#include "Buzzer.h"

void main(){
  unsigned char key_num = 0;
  NixieTube(8,key_num); // 数码管显示初始化
  while(1){
    key_num = PushKey();
    if(key_num){
      NixieTube(8,key_num);
      Buzzer_Time(500);
    }
  }
}
```

**- Buzzer.h**
```c
#ifndef __BUZZER_H__
#define __BUZZER_H__

void Buzzer_Time(unsigned int ms);//1000Hz，持续ms个毫秒

#endif
```

**- Buzzer.c**
```c
#include <REGX52.H>

// 重新定义端口名
sbit Buzzer_con = P2^5;

/**
  * @brief :蜂鸣器专用延时函数，@11.0592MHz晶振，延时500us
  * @param :需要延时500us的个数
  * @retval :无
 */
void Buzzer_Delay100us(unsigned int cycles){
	unsigned char i;
  do{
	i = 43;
	while (--i);
  }while(--cycles);
}


/**
  * @brief :蜂鸣器以默认频率1000Hz，持续响指定时间。
  * @param :cycles持续时间（响cycles个1ms）。
  * @retval :无
 */
void Buzzer_Time(unsigned int ms){
  unsigned int i;
  for(i=0;i<ms;i++){
    Buzzer_con = !Buzzer_con;//每500us翻转一次
    Buzzer_Delay100us(5);//控制蜂鸣器频率为1000Hz
  }
}
```

**- PushKey.h**
```c
#ifndef __PUSHKEY_H__
#define __PUSHKEY_H__

// 延时cycles ms，晶振@11.0592MHz
void PushKey_Delay(unsigned char cycles){
  unsigned char i, j;
  do{
    i = 2;
    j = 199;
    do{
      while (--j);
    }while (--i);
  }while(--cycles);
}

/**
  * @brief :检测按下了哪个按键开关
  * @param :无
  * @retval :输出按键开关编号1~4，不按返回0，松开触发
 */
unsigned char PushKey(){
  unsigned char key = 0;
  if(!P3_1)     {PushKey_Delay(10);while(!P3_1);PushKey_Delay(10);key=1;}
  else if(!P3_0){PushKey_Delay(10);while(!P3_0);PushKey_Delay(10);key=2;}
  else if(!P3_2){PushKey_Delay(10);while(!P3_2);PushKey_Delay(10);key=3;}
  else if(!P3_3){PushKey_Delay(10);while(!P3_3);PushKey_Delay(10);key=4;}
  return key;
}

#endif
```

**- NixieTube.h**
```c
#ifndef __NIXIETUBE_H__
#define __NIXIETUBE_H__

void NixieTube(unsigned char led, unsigned char num){
  // 给出数字0~9的定义（符合数组的索引）
  unsigned char number[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  // 给出选择的LED1~LED8的定义（实际上只用P2的2/3/4引脚）
  unsigned char sel_led[8] = {0x00,0x04,0x08,0x0c,0x10,0x14,0x18,0x1c};
  P0 = 0x00; // 数码管不显示，消影
  P2 = sel_led[led-1]; // 选择数码管：LED1
  P0 = number[num];  // 数码管显示7
}

#endif
```

## 9.3 实验：蜂鸣器播放音乐
需求：使用蜂鸣器播放小星星、天空之城。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E6%9B%B2%E8%B0%B1-%E5%B0%8F%E6%98%9F%E6%98%9F.png" width=45%>
</div><div align=center>
图9-8 《小星星》曲谱
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E6%9B%B2%E8%B0%B1-%E5%A4%A9%E7%A9%BA%E4%B9%8B%E5%9F%8E.png" width=45%>
</div><div align=center>
图9-9 《天空之城》曲谱
</div>

由于单片机性能有限，实现播放音乐的功能之后几乎就没什么资源了，所以就不进行模块化了。为了便捷的控制蜂鸣器频率，使用定时器T0，通过对其溢出复位值得调整，以控制蜂鸣器周期。下面是代码的调用逻辑：

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8-%E8%9C%82%E9%B8%A3%E5%99%A8%E6%92%AD%E6%94%BE%E9%9F%B3%E4%B9%90.png" width=20%>
</div><div align=center>
图9-10 “蜂鸣器播放音乐”的代码调用关系
</div>


代码展示：

**- main.c**
```c
#include <REGX52.H>
#include "Timer0.h"

// 下面的乐谱可以替换
/***************************************************/
// 《小星星》乐谱定义
#define  music_len 42//指定乐谱长度
unsigned char code music_freq_sel[2][music_len] = {
{12,12,19,19, 21,21,19,    17,17,16,16, 14,14,12,    19,19,17,17, 16,16,14,   
 19,19,17,17, 16,16,14,    12,12,19,19, 21,21,19,    17,17,16,16, 14,14,12   },
{4,4,4,4, 4,4,8, 4,4,4,4, 4,4,8, 4,4,4,4, 4,4,8,
 4,4,4,4, 4,4,8, 4,4,4,4, 4,4,8, 4,4,4,4, 4,4,8}};
/***************************************************/

// 通用延时函数,1ms
void Delay1ms(unsigned int cycles){
  unsigned char i, j;
  do{
    i = 2;
    j = 199;
    do{
      while (--j);
    }while (--i);
  }while(--cycles);
}

// 重新定义端口名
sbit Buzzer_con = P2^5;

// 定义四分音符时长（500ms）
#define SPEED 500

//不同频率所对应的定时器重装载值
unsigned int freq_reload[37] =
{0,63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64524,
   64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
   65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283};
//定义所需要选择的频率索引
unsigned char freq_sel = 0;
//定义音乐的索引变换，时长变换(时长以1/16音符为基准)
#define N0  0
#define L1  1
#define L1_ 2
#define L2  3
#define L2_ 4
#define L3  5
#define L4  6
#define L4_ 7
#define L5  8
#define L5_ 9
#define L6  10
#define L6_ 11
#define L7  12
#define M1  13
#define M1_ 14
#define M2  15
#define M2_ 16
#define M3  17
#define M4  18
#define M4_ 19
#define M5  20
#define M5_ 21
#define M6  22
#define M6_ 23
#define M7  24
#define H1  25
#define H1_ 26
#define H2  27
#define H2_ 28
#define H3  29
#define H4  30
#define H4_ 31
#define H5  32
#define H5_ 33
#define H6  34
#define H6_ 35
#define H7  36

void main(){
  unsigned int i;
  //定时器T0初始化
  Timer0_Init();
  freq_sel = 0;
  while(1){
    for(i=0;i<music_len;i++){
      freq_sel = music_freq_sel[0][i];
      Delay1ms(SPEED/4*music_freq_sel[1][i]);
      TR0 = 0;
      Delay1ms(5);
      TR0 = 1;
    }
    TR0 = 0;
    Delay1ms(5000); //暂停一段时间
    TR0 = 1;
  }
}

// 定义定时器T0中断后要执行的动作
void Timer0_Routine() interrupt 1{
  //当频率不是“0”，才进行相应的振荡
  if(freq_reload[freq_sel]){
    TH0 = freq_reload[freq_sel]/256;
    TL0 = freq_reload[freq_sel]%256;
    Buzzer_con = !Buzzer_con;
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

/*中断函数模板
// 定义定时器T0中断后要执行的动作
void Timer0_Routine() interrupt 1{
  static unsigned int count_T0; //中断次数
  count_T0++; //更新中断次数
  TH0 = 0xfc; TL0 = 0x66; // 恢复溢出周期，近似1ms
  if(count_T0>500){
    count_T0 = 0;
    
  }  
}
*/
#endif
```


编程感想：
> 1. 关于位运算。在FPGA中经常使用位运算提升运算效率，即使位宽不同也可以很方便的指定位的索引；但是在单片机中，运算中通常不能指定位的索引，所以在进行不同位宽的位运算时，会出现一些奇怪的bug。即，**在单片机中，除了一些特殊的场合（如寄存器赋值），应避免使用复杂的位运算。**
> 2. 愚蠢的错误：音乐播放完之后，不会继续播放，而是持续的嘈杂。原因：定义音乐的数组长度太长了，导致最后的音是空的。即数组的溢出错误，代码飘到哪里就不知道了。
> 3. 下面是其他音乐的一些乐谱：

```c
/***************************************************/
// 《天空之城》曲谱
#define  music_len 121//指定乐谱长度
unsigned char code music_freq_sel[2][music_len] = {
{ N0,N0,N0,M6,M7,  H1,M7,H1,H3,  M7,M3,M3,
  M6,M5,M6,H1,  M5,M3,  M4,M3,M4,H1,
  M3,N0,H1,H1,H1, M7,M4_,M4_,M7,  M7,N0,M6,M7,
  H1,M7,H1,H3,  M7,M3,M3,  M6,M5,M6,H1,
  M5,M2,M3,  M4,H1,M7,H1,  H2,H2,H3,H1,
  H1,M7,M6,M6,M7,M5_,  M6,H1,H2,  H3,H2,H3,H5,
  H2,M5,M5,  H1,M7,H1,H3,  H3,
  M6,M7,H1,M7,H2,H2,  H1,M5,  H4,H3,H2,H1,
  H3,H3,  H6,H5,H5,  H3,H2,H1,N0,H1,
  H2,H1,H2,H2,H5,  H3,H3,  H6,H5,
  H3,H2,H1,N0,H1,  H2,H1,H2,M7,  M6,M6,M7},
{ 4,4,4,2,2,  6,2,4,4,  12,2,2,
  6,2,4,4,  12,4,  6,2,4,4,
  8,2,2,2,2,  6,2,4,4,  8,4,2,2,
  6,2,4,4,  12,2,2,  6,2,4,4,
  12,2,2,  4,2,4,4, 2,2,2,8,
  2,2,2,2,4,4,  12,2,2,  6,2,4,4,
  12,2,2,  6,2,4,4,  16,
  2,2,4,4,2,2,  6,10,  4,4,4,4,
  12,4,  8,4,4,  2,2,8,2,2,
  4,2,2,4,4,  12,4,  8,8,
  2,2,8,2,2,  4,2,6,4,  12,2,2}
};
/***************************************************/
```










