# 2 数码管显示

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
注：工程及代码文件放在了本人的[Github仓库](https://github.com/jjejdhhd/Learn_STC89C52)。
***

## 2.1 静态数码管显示

 **2.1.1 原理介绍**
 &emsp;&emsp;LED数码管：数码管是一种简单、廉价的显示器，是由多个发光二极管封装在一起组成“8”字型的器件。本实验中使用 **共阴极数码管**（高电平点亮），原理图如下：
<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E5%8A%A8%E6%80%81%E6%95%B0%E7%A0%81%E7%AE%A1%E6%A8%A1%E5%9D%97%E5%8E%9F%E7%90%86%E5%9B%BE.png" width=80%>
</div><div align=center>
图 动态数码管模块原理图
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/138%E8%AF%91%E7%A0%81%E5%99%A8%E5%8E%9F%E7%90%86%E5%9B%BE.png" width=30%>
</div><div align=center>
图 74HC138译码器模块原理图
</div>

原理非常简单：
> - 1. 74HC138译码器：决定控制显示哪个数码管，固定工作在正常模式下。
> - 2. 然后通过8位P0信号控制数码管所显示的数字。74HC245（双向数据缓冲器）固定工作在A端输入、B段输出的模式下。其作用主要是就是增大电流驱动，防止单片机IO供电不足。

下面给出38译码器和数码管显示的真值表：

<div align=center>
表 74HC138译码器真值表
</div>
<div align=center>
<table>
    <tr>
        <td>输入<br>[P24,P23,P22]</td>
        <td>3'b000</td>
        <td>3'b001</td>
        <td>3'b010</td>
        <td>3'b011</td>
        <td>3'b100</td>
        <td>3'b101</td>
        <td>3'b110</td>
        <td>3'b111</td>
    </tr>
    <tr>
        <td>选中输出</td>
        <td>LED1</td>
        <td>LED2</td>
        <td>LED3</td>
        <td>LED4</td>
        <td>LED5</td>
        <td>LED6</td>
        <td>LED7</td>
        <td>LED8</td>
    </tr>
</table>
</div>

<div align=center>
表 共阴极数码管显示真值表
</div>
<div align=center>
<table>
    <tr>
        <td>输入[P07~P00]</td>
        <td>0x00</td>
        <td>0x3f</td>
        <td>0x06</td>
        <td>0x5b</td>
        <td>0x4f</td>
        <td>0x66</td>
        <td>0x6d</td>
        <td>0x7d</td>
        <td>0x07</td>
        <td>0x7f</td>
        <td>0x6f</td>
        <td>0x77</td>
        <td>0x7c</td>
        <td>0x39</td>
        <td>0x5e</td>
        <td>0x79</td>
        <td>0x71</td>
    </tr>
    <tr>
        <td>数码管显示数字</td>
        <td>无显示</td>
        <td>0</td>
        <td>1</td>
        <td>2</td>
        <td>3</td>
        <td>4</td>
        <td>5</td>
        <td>6</td>
        <td>7</td>
        <td>8</td>
        <td>9</td>
        <td>a</td>
        <td>b</td>
        <td>c</td>
        <td>d</td>
        <td>e</td>
        <td>f</td>
    </tr>
</table>
</div>

**2.1.2 代码展示**
下面要求实现功能：LED4数码管显示3。
代码如下：
```c
 #include <REGX52.H>

void main(){
  while(1){
    P2 = 0x0c; // 0000_1100，第234位选择数码管：LED4
    P0 = 0x4f; // 共阴极数码管显示3 
  }
}
```


## 2.2 动态数码管显示

**2.2.1 原理介绍**
为了时使数码管动态显示，就需要用到人眼的“视觉暂留效应”。和电视类似，只要数码管刷新的够快，比如8ms刷新一次，那么在人眼看来就像是数码管在动态的显示数字一样。

**2.2.2 代码展示**
需求1：使用8位数码管显示数字，20230117
代码如下：
```c
 #include <REGX52.H>

//延时函数：延时1ms
void Delay1ms(unsigned int cycles){ //@11.0592MHz
  unsigned char i, j;
  do{
    // 一定注意下面这两个定义放在大循环里面
    i = 2;
	  j = 199;
    do{
      while (--j);
    }while (--i);
  }while(--cycles);
}

void main(){
  // 给出数字0~9的定义（符合数组的索引）
  const unsigned char number[10] = {0x3f,0x06,0x5b,0x4f,0x66,
                                   0x6d,0x7d,0x07,0x7f,0x6f};
  // 给出选择的LED1~LED8的定义
  const unsigned char sel_led[8] = {0x00,0x04,0x08,0x0c,
                                    0x10,0x14,0x18,0x1c};
  while(1){
    P2 = sel_led[0]; // 选择数码管：LED1
    P0 = number[7];  // 数码管显示7 
    Delay1ms(1);
    
    P2 = sel_led[1]; // 选择数码管：LED2
    P0 = number[1];  // 数码管显示1 
    Delay1ms(1);
    
    P2 = sel_led[2]; // 选择数码管：LED3
    P0 = number[1];  // 数码管显示1 
    Delay1ms(1);
    
    P2 = sel_led[3]; // 选择数码管：LED4
    P0 = number[0];  // 数码管显示0
    Delay1ms(1);
    
    P2 = sel_led[4]; // 选择数码管：LED5
    P0 = number[3];  // 数码管显示3
    Delay1ms(1);
    
    P2 = sel_led[5]; // 选择数码管：LED6
    P0 = number[2];  // 数码管显示2
    Delay1ms(1);
    
    P2 = sel_led[6]; // 选择数码管：LED7
    P0 = number[0];  // 数码管显示0
    Delay1ms(1);
    
    P2 = sel_led[7]; // 选择数码管：LED8
    P0 = number[2];  // 数码管显示2
    Delay1ms(1);
  }
}
```
需求2：8位数码管每0.1s累加一次，直到9999后归零继续累加。
代码如下：
```c
 #include <REGX52.H>

//延时函数：延时1ms
void Delay1ms(unsigned int cycles){ //@11.0592MHz
  unsigned char i, j;
  do{
    // 一定注意下面这两个定义放在大循环里面
    i = 2;
	  j = 199;
    do{
      while (--j);
    }while (--i);
  }while(--cycles);
}

void main(){
  // 给出数字0~9的定义（符合数组的索引）
  const unsigned char number[10] = {0x3f,0x06,0x5b,0x4f,0x66,
                                   0x6d,0x7d,0x07,0x7f,0x6f};
  // 给出选择的LED1~LED8的定义
  const unsigned char sel_led[8] = {0x00,0x04,0x08,0x0c,
                                    0x10,0x14,0x18,0x1c};
  // 定义数码管显示的数字
  unsigned long int current = 999970;
  int num1=0, num2=0, num3=0, num4=0,
       num5=0, num6=0, num7=0, num8=0; // 每一位所显示的数字
  unsigned long int temp = 0;
  
  int i=0;// 初始化for循环的循环变量
  
  while(1){
    // 获取每一位所显示的数字
    num1 = current%10; temp = (current-num1)/10;
    num2 = temp%10; temp = (temp-num2)/10;
    num3 = temp%10; temp = (temp-num3)/10;
    num4 = temp%10; temp = (temp-num4)/10;
    num5 = temp%10; temp = (temp-num5)/10;
    num6 = temp%10; temp = (temp-num6)/10;
    num7 = temp%10; temp = (temp-num7)/10;
    num8 = temp%10;
    
    // 显示当前数字1s
    for(i=0;i<125;i++){
      P2 = sel_led[0]; // 选择数码管：LED1
      P0 = number[num1];  // 数码管显示7 
      Delay1ms(1);
      P2 = sel_led[1]; // 选择数码管：LED2
      P0 = number[num2];  // 数码管显示1 
      Delay1ms(1);
      P2 = sel_led[2]; // 选择数码管：LED3
      P0 = number[num3];  // 数码管显示1 
      Delay1ms(1);
      P2 = sel_led[3]; // 选择数码管：LED4
      P0 = number[num4];  // 数码管显示0
      Delay1ms(1);
      P2 = sel_led[4]; // 选择数码管：LED5
      P0 = number[num5];  // 数码管显示3
      Delay1ms(1);
      P2 = sel_led[5]; // 选择数码管：LED6
      P0 = number[num6];  // 数码管显示2
      Delay1ms(1);
      P2 = sel_led[6]; // 选择数码管：LED7
      P0 = number[num7];  // 数码管显示0
      Delay1ms(1);
      P2 = sel_led[7]; // 选择数码管：LED8
      P0 = number[num8];  // 数码管显示2
      Delay1ms(1);
    }
    
    // 更新当前需要显示的数字
    if(current > 9999999)
      current = 0;
    else
      current++;
  }
}
```
上面这个程序逻辑清晰，显然可以完成需求，但是硬件实测会有**频闪现象**和**拖影现象**。
> 1. 为了解决频闪的问题，将上述计算块与显示块融合在一起，使用计算语句来代替数码管显示语句中的延迟，便可以消除频闪现象，代价是代码结果不清晰。
> 2. 在“段选→片选→段选→片选→...”的不断循环中，“段选→片选”会使得上一个数码管的值被带到下一个数码管，从而造成“拖影现象”。造成为了“消影”，可以在片选开始前，将段选信号设置为不显示，直到计算出新的段选信号。
> 3. 模块化编程：同时由于反复调用了数码管显示的语句，所以将数码管显示这个功能单独编成一个模块。
```c
 #include <REGX52.H>

void NixieTube(unsigned char led, unsigned char num){
  // 给出数字0~9的定义（符合数组的索引）
  const unsigned char number[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  // 给出选择的LED1~LED8的定义
  const unsigned char sel_led[8] = {0x00,0x04,0x08,0x0c,0x10,0x14,0x18,0x1c};
  P0 = 0x00; // 数码管不显示，消影
  P2 = sel_led[led-1]; // 选择数码管：LED1
  P0 = number[num];  // 数码管显示7
}

void main(){
  // 定义数码管显示的数字
  unsigned long int current = 999970; // 显示初始值
  unsigned int num; // 每一位所显示的数字
  unsigned long int temp = 0;
  // 初始化for循环的循环变量
  int i=0;// 影响数码管更新速度
  int j=0;// 作为数码管的片选信号
  
  while(1){
    // 显示当前数字1s，同时获取每一位所显示的数字
    // 用计算来代替延时函数
    for(i=0;i<12;i++){
      for(j=1;j<9;j++){
        if(j==1){
          num = current%10;
          temp = current/10;
        }
        else{
          num = temp%10;
          temp = temp/10;
        }
        NixieTube(j,num);
      }
    }
    
    // 更新当前需要显示的数字
    if(current > 9999999)
      current = 0;
    else
      current++;
  }
}
```

## 2.3 数码管扫描方式
现在来回顾一下整个实验。不管是静态数码管还是动态数码管实验，要驱动数码管都需要3位片选信号+8位段选信号，共计 **11个引脚来驱动一个小小的数码管！！**（一共40位引脚）。显然这太浪费了，所以实际上会有专门的数码管驱动芯片来维持数码管的显示。也就是说，数码管有两种扫描方式：
> - 单片机直接扫描：硬件设备简单，但会耗费大量的单片机CPU时间。
> - 专用驱动芯片：内部自带显存、扫描电路，单片机只需告诉它显示什么即可，比如TM1640用两根信号线便可驱动16个数码管；两个74HC595移位寄存器可以用3根数据线两根电源线驱动8个数码管（但也需要CPU扫描）。


