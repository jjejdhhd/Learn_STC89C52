# 7 点阵屏

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
注：工程及代码文件放在了本人的[Github仓库](https://github.com/jjejdhhd/Learn_STC89C52)。
***


## 7.1 LED点阵屏介绍
LED点阵屏由若干个独立的LED组成，LED以矩阵的形式排列，以灯珠亮灭来显示文字、图片、视频等。LED点阵屏广泛应用于各种公共场合，如汽车报站器、广告屏以及公告牌等。LED点阵屏分类：
> 按颜色：单色、双色、全彩。
> 按像素：8\*8、16\*16等（大规模的LED点阵通常由很多个小点阵拼接而成）。


**LED点阵屏的结构类似于数码管**，只不过是数码管把每一列的像素以“8”字型排列而已。LED点阵屏与数码管一样，有共阴和共阳两种接法，不同的接法对应的电路结构不同（单色LED点阵没有区别）。LED点阵屏需要进行逐行或逐列扫描，才能使所有LED同时显示。与数码管使用38译码器(74HC138)作为片选信号节省引脚相同，<u>LED点阵屏使用74HC595作为片选信号节省引脚</u>。**所以本实验的难点在于对74HC595的控制。**

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/LED%E7%82%B9%E9%98%B5%E5%B1%8F%E5%8E%9F%E7%90%86%E5%9B%BE.png" width=80%>
</div><div align=center>
图7-1 74HC595 及 LED点阵屏 原理图
</div>


74HC595是串行输入并行输出的移位寄存器，可用3根线输入串行数据，8根线输出并行数据，将串行输出、串行输入进行多片级联后，可输出16位、24位、32位等，常用于IO口扩展。下面介绍其引脚及功能真值表：
> - 引脚16、8：VCC、GND电源供电。
> - 引脚15，1~7：8位并行输出端。
> - 引脚9：串行数据输出。
> - 引脚14：串行输入。
> - 剩下的，引脚13为输出有效（高电平）；引脚10为主复位（低电平）；引脚11为移位寄存器时钟；引脚12为存储寄存器时钟。
>


<div align=center>
表7-1 74HC595控制引脚真值表
</div><div align=center>
<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-9wq8{border-color:inherit;text-align:center;vertical-align:middle}
.tg .tg-wa1i{font-weight:bold;text-align:center;vertical-align:middle}
.tg .tg-uzvj{border-color:inherit;font-weight:bold;text-align:center;vertical-align:middle}
.tg .tg-nrix{text-align:center;vertical-align:middle}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-uzvj">引脚13/OE</th>
    <th class="tg-uzvj">SRCLR/VCC</th>
    <th class="tg-uzvj">SRCLK/P36</th>
    <th class="tg-wa1i">RCLK/P35</th>
    <th class="tg-uzvj">功能</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-9wq8">H</td>
    <td class="tg-9wq8">X</td>
    <td class="tg-9wq8">X</td>
    <td class="tg-nrix">X</td>
    <td class="tg-9wq8">清空移位寄存器<br>并行输出为高阻状态</td>
  </tr>
  <tr>
    <td class="tg-9wq8">L</td>
    <td class="tg-9wq8">L</td>
    <td class="tg-9wq8">X</td>
    <td class="tg-nrix">X</td>
    <td class="tg-9wq8">清空移位寄存器</td>
  </tr>
  <tr>
    <td class="tg-9wq8">L</td>
    <td class="tg-9wq8">H</td>
    <td class="tg-9wq8">↑</td>
    <td class="tg-nrix">X</td>
    <td class="tg-9wq8">移位寄存器锁定<br>QN=Q(N-1), Q'=SER</td>
  </tr>
  <tr>
    <td class="tg-9wq8">L</td>
    <td class="tg-9wq8">H</td>
    <td class="tg-9wq8">X</td>
    <td class="tg-nrix">↑</td>
    <td class="tg-9wq8">移位寄存器的内容移入到输出寄存器</td>
  </tr>
</tbody>
</table>
</div>

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/74HC595%E9%80%BB%E8%BE%91%E6%A1%86%E5%9B%BE.png" width=80%>
</div><div align=center>
图7-2 74HC595逻辑框图
</div>

通过上面的真值表可以看出，要想遍历每一行LED，首先硬件上OE引脚保持度电平，SRCLR保持高电平。软件方面，串行输入只需要最开始为1，而后均为0。然后，每次都进行移位寄存器的更新及输出寄存器的更新，即可遍历到每一行LED。


下面是会用到的51单片机语法：

> - **```sfr```（special function register）**：特殊功能寄存器声明。
	例：sfr P0 = 0x80;
	声明P0口寄存器，物理地址为0x80。
> - **```sbit```（special bit）**：特殊位声明
	例：sbit P0_1 = 0x81;    或    sbit P0_1 = P0^1;//P0的第1位
	声明P0寄存器的第1位
> - **可位寻址/不可位寻址**：在单片机系统中，操作任意寄存器或者某一位的数据时，必须给出其物理地址，又因为一个寄存器里有8位，所以位的数量是寄存器数量的8倍，单片机无法对所有位进行编码，故每8个寄存器中，只有一个是可以位寻址的。对不可位寻址的寄存器，若要只操作其中一位而不影响其它位时，可用“&=”、“|=”、“^=”的方法进行位操作


## 7.2 实验：LED点阵屏显示图片
需求：在LED点阵屏上显示一个“我的世界”的方脸。

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8%E5%85%B3%E7%B3%BB-LED%E7%82%B9%E9%98%B5%E6%98%BE%E7%A4%BA%E5%9B%BE%E7%89%87.png" width=20%>
</div><div align=center>
图7-3 “LED点阵显示图片”代码调用关系
</div>

代码展示：
**- main.c**
```c
#include <REGX52.H>
#include "Matrix_LED.h"

// 定义图案数据，注意是按列循环的
unsigned char led_face[8] = 
{0xc0,0xdb,0xd9,0xc5,0xc5,0xd9,0xdb,0xc0};
//            {{1,1,1,1,1,1,1,1},
//             {1,1,1,1,1,1,1,1},
//             {0,0,0,0,0,0,0,0},
//             {0,1,1,0,0,1,1,0},
//
//             {0,1,1,0,0,1,1,0},
//             {0,0,0,1,1,0,0,0},
//             {0,1,0,0,0,0,1,0},
//             {0,1,1,1,1,1,1,0},};

void main(){
  unsigned char i;
  P3_5 = 0; P3_6 = 0; // 对移动寄存器时钟复位
  while(1){
    for(i=0;i<8;i++){
      MatrixLED_ShowColumn(i,led_face[i]);
    }
  }
}
```

**- Matrix_LED.h**
```c
#ifndef __MATRIX_LED_H__
#define __MATRIX_LED_H__

void MatrixLED_ShowColumn(unsigned char Column,Data); // 控制LED点阵的某一列显示

#endif
```

**- Matrix_LED.c**
```c
#include <REGX52.H>

/*随开发板的不同，此段代码中的端口定义会有所变化，使用前注意更新*/
/**********************************************************/
// LED点阵通用，延时cycles ms，晶振@11.0592MHz
// 静态函数，只能在本源文件中被调用
static void Delay(unsigned int cycles){
  unsigned char i, j;
  do{
    i = 2;
    j = 199;
    do{
      while (--j);
    }while (--i);
  }while(--cycles);
}

// 为了方便，先给几个引脚重新起名
sbit RCK = P3^5; // 重复了所以不叫RCLK
sbit SCK = P3^6; // SRCLK
sbit SER = P3^4;
#define MATRIX_LED_PORT P0
/*********************************************************/

/*下面的代码无需更新*/
/*********************************************************/
/**
  * @brief :向74HC595串行输入口输入8bit数据。
  * @param :输入的8bit数据，高位先进。
  * @retval :无。
 */
void _74HC595_WriteByte(unsigned char Byte){
  unsigned char i;
  for(i=0;i<8;i++){
    SER = Byte&(0x80>>i); // 右边只要不是0，1bit宽度的SER就为1
    SCK = 1;
    SCK = 0;
  }
  RCK = 1;
  RCK = 0;
}

/**
  * @brief :在LED点阵的指定列，输入数据。
  * @param :Column，列位置，范围0~7。
  * @param :Data，该列需要显示的数据。
  * @retval :无。
 */
void MatrixLED_ShowColumn(unsigned char Column,Data){
  _74HC595_WriteByte(Data); // 段选信号
  MATRIX_LED_PORT = ~(0x80>>Column); // 片选信号
  Delay(1);
  MATRIX_LED_PORT = 0xff; // 消影
}
/*********************************************************/
```

编程感想：
> - 回顾数码管、LED点阵屏的实验，发现二极管的正极都是被其他芯片强化过的，而不是直接通过单片机引脚进行供电。本质上这是因为，单片机的引脚高电平只是弱上拉，但是低电平则具有很强的吸收电流的功能。所以单片机引脚专门连LED的低电平。**注意编程时按列输出！**
> - 另外，涉及到“片选→段选→片选→段选→……”这样的结果，为了防止在“段选→片选”过程中出现“拖影现象”。有两种方法可以消影，一个是“片选→段选→**段选归0**→片选→段选→……”，另一个是“片选→**延时、片选归0**→段选→片选→延时、段选归0→段选→……”。为了程序效率，一般选择归0操作更简单的方式。


## 7.3 实验：LED点阵屏显示动画
需求：在显示屏上显示流动的飞机。

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8%E5%85%B3%E7%B3%BBLED%E7%82%B9%E9%98%B5%E6%98%BE%E7%A4%BA%E5%8A%A8%E7%94%BB.png" width=20%>
</div><div align=center>
图7-4 “LED点阵显示动画”代码调用关系
</div>

我将上一节```MatrixLED.c```文件又优化了一下，代码如下：
**- main.c**
```c
#include <REGX52.H>
#include "Matrix_LED.h"

// 定义图案数据，注意是按列循环的
// 下面这个数据如果不加"code"，就会存储在片内RAM中，可能会导致存储空间不足。
// 添加"code"之后，这个数组就会存储在片外Flash上，空间很大，但不允许更改。
unsigned char code led_animation[] = 
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x18,0x3C,0x7E,0xDB,0x99,0x18,0x18,0x18,
 0x18,0x99,0xBD,0xFF,0x18,0x24,0x42,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void main(){
  unsigned char cur_pic[8];
  unsigned char fra,offset;//取出当前需要显示的数据
  unsigned int MAX_OFFSET = sizeof(led_animation)-8;
  unsigned int cnt_show = 0;//显示次数计数
  unsigned char MAX_CNT = 5;//控制显示速率
  MatrixLED_Init(); //LED点阵屏初始化
  while(1){
    for(offset=0;offset<MAX_OFFSET;offset++){
      cnt_show = 0;
      // 取出当前需要显示的数据
      for(fra=0;fra<8;fra++){
        cur_pic[fra]=led_animation[fra+offset];
      }
      // 重复显示以控制动画速度
      do{
        MatrixLED_ShowPic(cur_pic);
        cnt_show++;
      }while(cnt_show<MAX_CNT);
    }
  }
}
```

**- MatrixLED.h**
```c
#ifndef __MATRIX_LED_H__
#define __MATRIX_LED_H__

void MatrixLED_Init();//LED点阵屏初始化
void MatrixLED_ShowColumn(unsigned char Column,Data); //控制LED点阵的某一列显示
void MatrixLED_ShowPic(unsigned char Data[8]); //控制LED点阵显示一副图案
  
#endif
```

**- MatrixLED.c**
```c
#include <REGX52.H>

/*随开发板的不同，此段代码中的端口定义会有所变化，使用前注意更新*/
/**********************************************************/
// LED点阵通用，延时cycles ms，晶振@11.0592MHz
static void Delay(unsigned int cycles){
  unsigned char i, j;
  do{
    i = 2;
    j = 199;
    do{
      while (--j);
    }while (--i);
  }while(--cycles);
}

// 为了方便，先给几个引脚重新起名
sbit RCK = P3^5; // 重复了所以不叫RCLK
sbit SCK = P3^6; // SRCLK
sbit SER = P3^4;
#define MATRIX_LED_PORT P0
/*********************************************************/

/*下面的代码无需更新*/
/*********************************************************/
/**
  * @brief :LED点阵屏初始化。
  * @param :无。
  * @retval :无。
 */
void MatrixLED_Init(){
  // 对移位寄存器时钟复位
  RCK = 0;
  SCK = 0;
}

/**
  * @brief :向74HC595串行输入口输入8bit数据。
  * @param :输入的8bit数据，高位先进。
  * @retval :无。
 */
void _74HC595_WriteByte(unsigned char Byte){
  unsigned char i;
  for(i=0;i<8;i++){
    SER = Byte&(0x80>>i); // 右边只要不是0，1bit宽度的SER就为1
    SCK = 1;
    SCK = 0;
  }
  RCK = 1;
  RCK = 0;
}

/**
  * @brief :在LED点阵的指定列，输入数据。
  * @param :Column，列位置，范围0~7。
  * @param :Data，该列需要显示的数据。
  * @retval :无。
 */
void MatrixLED_ShowColumn(unsigned char Column,Data){
  _74HC595_WriteByte(Data); // 段选信号
  MATRIX_LED_PORT = ~(0x80>>Column); // 片选信号
  Delay(1);
  MATRIX_LED_PORT = 0xff; // 消影
}

/**
  * @brief :在LED点阵显示一幅图案。
  * @param :Data[8]，输入数据是一维数组，长度为8，表示一幅图案。
  * @retval :无。
 */
void MatrixLED_ShowPic(unsigned char Data[8]){
  unsigned char i;
  for(i=0;i<8;i++){
      MatrixLED_ShowColumn(i,Data[i]);
  }
}
/*********************************************************/
```

编程感想：
> 1. 做滚动动画的时候，可以在此动画前后添加空屏，就以保证动画的连续程度。
> 2. 注意滚动动画，每次偏移量为1；视频动画每次偏移量为8（一帧）。
> 3. 注意数组太长，片上的RAM会存不下，可以使用 **```code```** 存放在片上ROM(Flash)中，如```unsigned char code led_animation[]```。
> 4. 下面给给出一些其他动画。

```c
//小球下落，视频动画
unsigned char code led_animation[] = 
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
 0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,
 0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
 0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,
 0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,
 0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,
 0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,
 0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

// 方框回缩与放大，视频动画
 unsigned char code led_animation[] = 
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0xFF,
 0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00,
 0x00,0x00,0x3C,0x24,0x24,0x3C,0x00,0x00,
 0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00, //中心轴
 0x00,0x00,0x3C,0x24,0x24,0x3C,0x00,0x00,
 0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00,
 0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0xFF,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
```

