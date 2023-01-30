# 1 简介及点灯

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
注：工程及代码文件放在了本人的[Github仓库](https://github.com/jjejdhhd/Learn_STC89C52)。
***

## 1.1 51单片机简介

**1.1.1 安装软件**
应该装的软件主要有三个：
- Keil5：编写程序。
- STC-ISP：下载比特流文件到板子上，另外还有一些小工具（如串口助手、自动生成代码等）。
- ch341ser.exe：驱动程序。使电脑能识别单片机的端口。
- 注意：Keil5 C51和Keil5 MDK都是Keil系列软件，但前者是用来开发51单片机的，后者是用来开发ARM系列，比如STM32的。

&emsp;&emsp;Keil可以在其官网[arm KEIL](https://www.keil.com/)下载，点击Download可以看到四款产品，分别针对不同的芯片（在软件下方都有说明）。下载最新版免费，但继续使用则需要license破解程序（有非常掉san的音乐）。破解软件可以按照UP主提供的百度网盘链接获取，也可以找淘宝店家索取，具体的安装过程可以参考视频 [[1-2]开发工具及软件介绍](https://www.bilibili.com/video/BV1Mb411e7re?p=3) 。

&emsp;&emsp;STC-ISP可以在[STC官网](http://www.stcmcudata.com/)找到，但是作为单片机届的泥石流 :satisfied:，有可能会暂时关闭网站，所以还是需要下载UP的资源。这个软件无需安装、破解，可以直接拿来用。

&emsp;&emsp;关于驱动程序，直接运行安装即可。若出现问题，详情可以参考 “STC-USB驱动安装说明” 。


**1.1.2 单片机简介**
&emsp;&emsp; **单片机（Micro Controller Unit, MCU）** 内部集成了CPU、RAM、ROM、定时器、中断系统、通讯接口等一系列电脑的常用硬件功能。 <u>单片机的任务</u>是信息采集（依靠传感器）、处理（依靠CPU）和硬件设备（例如电机，LED等）的控制。**单片机算是一个袖珍版计算机**，一个芯片就能构成完整的计算机系统。虽然在性能上，单片机与计算机相差甚远，但单片机成本低、体积小、结构简单，在生活和工业控制领域大有所用同时，学习使用单片机是了解计算机原理与结构的最佳选择。

&emsp;&emsp; **51单片机** 是指80年代Intel开发的8051单片机内核的统称，就是因为这个“8051”有个51，所以凡是与“8051”内核一样的单片机都统称为51系列单片机。
&emsp;&emsp;**内核**，通俗点理解就是一种代码，单片机的指令系统，即硬件制作就按照这些指令去完成，如灌溉一样，要灌溉那些地方，就把通路打开，而这些通路‘水渠和门阀’都是早就修建好的硬设备，只是具体的灌溉地点需要指令来完成。而51单片机就是一样的“水渠和门阀”，想灌溉，所有51单片机都使用的一样的灌溉指令。
注：[51单片机](https://baike.baidu.com/item/51%E5%8D%95%E7%89%87%E6%9C%BA/5344633) 百度百科。

下面给出一些 **命名规则** ：

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git-Img/main/STC89C51RC/RD%E5%91%BD%E5%90%8D%E8%A7%84%E5%88%99.png" width="70%">

图1-1 STC89C51RC/RD+系列的命名规则
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git-Img/main/STC89C52%E7%B3%BB%E5%88%97%E5%91%BD%E5%90%8D%E8%A7%84%E5%88%99.png" width="70%">

图1-2 STC89C52系列的命名规则
</div>

注：想要查看更多单片机的资料，可以查看[STC宏晶科技官网](https://www.stcmcudata.com/)。更多元器件的资料，可以在 [datasheet网站](https://www.alldatasheet.com/) 自行搜索器件参数。
再注：宏晶科技的官网就是花花绿绿的，至于为啥，我觉得可能是“土到极致就是潮”吧哈哈哈。知乎文章：[STC单片机的网站让我笑喷了](https://zhuanlan.zhihu.com/p/310144201)。


<div align=center>

表1-1 单片机参数整理（不定期更新）

|单片机型号|所属系列|公司|位数|RAM|ROM|工作频率|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|STC89C52|51单片机系列|STC公司（中国）|8位|512字节|8K (Flash)|12MHz|
|STC89C516RD+|51单片机系列|STC公司（中国）|8位|1280字节|62K (Flash)|40MHz|

</div>

我自己使用的芯片完整型号描述是：STC89C516RD+ 40I-PDIP40 2110CVK416.X90C。引脚图如下：

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git-Img/main/STC89C516-90C%E7%89%88%E6%9C%AC%E5%BC%95%E8%84%9A%E5%9B%BE.png" width="30%">

图1-3 90C版本的引脚图
</div>

可以注意到，由于单片机是8位的，所以P0、P1、P2、P3引脚都是8个为一组。

&emsp;&emsp;STC89C51RC/RD+单片机中包含中央处理器（CPU）、程序存储器（Flash）、数据存储器（SRAM）、定时/计数器、UART串口、I/0接口、EEPROM、看门狗等模块。STC89C51RC/RD+系列单片机几乎包含了数据采集和控制中所需的所有单元模块，可称得上一个片上系统。
![]()
<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/51%E7%B3%BB%E5%88%97%E5%8D%95%E7%89%87%E6%9C%BA%E7%AE%80%E4%BB%8B.png" width="70%">

图1-4 STC89C51RC/RD+系列单片机简介
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git-Img/main/STC89C51RD%E7%B3%BB%E5%88%97%E5%86%85%E9%83%A8%E7%BB%93%E6%9E%84%E6%A1%86%E5%9B%BE.png" width="70%">

图1-5 STC89C51RC/RD+系列内部结构框图（总线结构）
</div>


&emsp;&emsp;虽然单片机具备一个系统完善的模块，但是要正常工作还是有一些基本需求。下面是STC89C51RC/RD+系统的最小应用系统：

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git-Img/main/STC89C51RD%E7%B3%BB%E7%BB%9F%E7%9A%84%E6%9C%80%E5%B0%8F%E5%BA%94%E7%94%A8%E7%B3%BB%E7%BB%9F.png" width="85%">

图1-6 STC89C51RC/RD+系列的最小应用系统
</div>

> - 右上角是电源电路，C5和C6是电源的滤波电容，滤除电源中不稳定的高频分量，以提供更稳定的直流。
> - 左下角是时钟系统，电容C2、C3作用是帮助晶振起振，电阻R2的作用可能是为了帮助时钟系统更稳定的工作。
> - 左侧中间位置是高电平复位电路，刚上电时电容充电为高电平，充满电之后为低电平，所以每次上电都会复位会使得芯片从第一条指令开始执行。一些自制的小产品可以不接此电路。
> 
> **记清IO口的位置很重要。**
> - 40正极，20接地。
> P0到3，镜像Z形。左侧上下，右侧外里。
> 9脚复位，P0上拉。
> 31引脚，电平决定，接高访内，接低访外。
>
> - P3复用：RXD/TXD/INT0/INT1/T0/T1/WR/RD，收发断断时时写读。

&emsp;&emsp;最后，关于 **板载资源** 的介绍，可以参考普中科技官网的 [普中-单核-A4](http://www.prechin.cn/a/jiaoxue/51/89.html) 的介绍。注意到A4和A3的区别仅在于STC的芯片不同，开发板貌似没有什么区别。进一步查看引脚等信息，则需要查看开发板 <u>原理图文件</u> 。

## 1.2 8051通用开发流程
创建一个工程的步骤是：
1. 打开Keil，顶部菜单栏选择“Project → New μVersion Project”;
2. 在指定文件夹下（路径可以有中文、空格），输入工程名称，“OK”不需要起始文件”；
 > - 由于文件夹可以随意命名，所以工程名称一般就命名为“Project”。
 > - 在选择CPU型号时，由于STC是一家中国公司，而Keil是一款外国软件，所以对STC的支持不友好。但STC89C系列的芯片是完全兼容8051的，所以可以直接选择 **Atmel的 AT89C51 / AT89C52**，是完全兼容的（我选择AT89C52）。
4. 创建main文件。在左侧“Source Group 1”右键 → "Add New Item to..." → 语言、文件名 → “OK”；
5. 创建hex比特流文件。编写好程序后，在"Options for Target"左键 → Output → "Create Hex File"，然后编译。
6. 使用STC-ISP下载到芯片中。注意需要重启电源开关才能被识别。




## 1.2 LED灯

### 1.2.1 点亮一个LED

**1.2.1.1原理分析**
&emsp;&emsp;人类电灯：白炽灯 → 节能灯 → LED灯。发光二极管（Light Emitting Diode，简称LED）省电且亮度高，常用于照明、广告灯、指引灯、屏幕。LED灯珠内，较小的电极（一般对应引脚较短）为负极，大的电极（一般对应引脚较长）为正极。

电阻（电容）数值读取：
> 102 = 10 00 = 1kΩ
> 473 = 47 000 = 47kΩ
> 1001 = 100 0 = 1kΩ

&emsp;&emsp;单片机电平为TTL电平，高电平5V，低电平0V。根据开发板的原理图示意，在控制LED模块时，芯片引脚输出高电平为灭灯，输出低电平为亮灯。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git-Img/main/%E5%BC%80%E5%8F%91%E6%9D%BF%E5%8E%9F%E7%90%86%E5%9B%BE-LED%E6%A8%A1%E5%9D%97.png" width="35%">

图2-1 普中A4开发板LED模块
</div>

**1.2.1.2 程序代码**
根据2.1节的原理，给出程序代码：
 ```c
 #include <REGX52.H> // 包含单片机的引脚映射关系
// 上面的头文件可以右键直接添加
void main(){
    P2 = 0x80;// 1000_0000 注意左侧高位
    //keil不支持C语言中的二进制数
    while(1){
    // 为了使LED常亮
    };
}
 ```

 结果可以发现A4开发板上的D8灯被点亮。

### 1.2.2 LED闪烁

**1.2.2.1原理分析**

 &emsp;&emsp;根据“点亮一个LED”的原理，如果要使LED灯以0.5s变化一次亮灭状态的速度进行闪烁，只需要添加延时函数即可。这可以使用STC-ISP软件中自带的“软件延时计算器”。
1. 由于需要延时0.5s，所以“定时长度”设置为500毫秒；
2. “系统频率”根据晶振标注“Y11.0592”可知为11.0592MHz；
3. 小工具中的“8051指令集”选择“STC-Y1”，以支持STC89C系列。

然后“复制代码”到main函数中，就可以直接调用了。

**1.2.2.2程序代码及现象**
 ```c
 #include <REGX52.H>
 #include <INTRINS.H>//调用延时函数中的空语句“_nop_函数”

//使用SRC-ISP生成的延时函数
void Delay500ms() {	//@11.0592MHz
   unsigned char i, j, k;
   _nop_();//延时1us左右，删掉也没关系
   i = 4;
   j = 129;
   k = 119;
   do {
   	do {
   		while (--k);
   	} while (--j);
   } while (--i);
}

//主函数
void main(){
   while(1){
   	P2 = 0x7F; // 0111_1111
   	Delay500ms(); //调用延时子函数
   	P2 = 0xFF; // 1111_1111
   	Delay500ms(); //调用延时子函数
   }
}
 ```

 下载程序现象：D8灯珠以0.5s的速度改变亮灭状态。

 注意，如果遇到复制出来的代码是乱码的状态，可以选择“Edit → Configuration → Encoding”设置为与编辑器相同的编码方式即可（比如都为UTF-8）。

### 1.2.3 LED流水灯
 &emsp;&emsp;原理和上面没什么区别，直接放代码：

 ```c
 #include <REGX52.H>
 #include <INTRINS.H>//调用延时函数中的空语句“_nop_函数”

//使用SRC-ISP生成的延时函数
void Delay500ms() {	//@11.0592MHz
   unsigned char i, j, k;
   _nop_();//延时1us左右，删掉也没关系
   i = 4;
   j = 129;
   k = 119;
   do {
   	do {
   		while (--k);
   	} while (--j);
   } while (--i);
}

//主函数
void main(){
  while(1){
    P2 = 0x7f; // 0111 1111
    Delay500ms();
    P2 = 0xbf; // 1011 1111
    Delay500ms();
    P2 = 0xdf; // 1101 1111
    Delay500ms();
    P2 = 0xef; // 1110 1111
    Delay500ms();
    P2 = 0xf7; // 1111 0111
    Delay500ms();
    P2 = 0xfb; // 1111 1011
    Delay500ms();
    P2 = 0xfd; // 1111 1101
    Delay500ms();
    P2 = 0xfe; // 1111 1110
    Delay500ms();
  }
}
 ```
 运行效果：LED从D8开始每过0.5s就向左亮一个灯，形成流水灯效果。

 ### 1.2.4 LED流水灯Pro
 &emsp;&emsp;然而上述代码有一个很大的缺点，就是每次想修改流水灯的变化间隔时间，就需要重新生成一次延时函数，过于麻烦。解决思路是生成一个延时1ms的代码，每次想延时多少ms就调用几次。

<div align=center>
表 C51单片机数据类型
<table>
    <tr>
        <th>类型</th>
        <th>符号</th>
        <th>关键字</th>
        <th>所占位数</th>
        <th>数的表示范围</th>
    </tr>
    <tr>
        <td rowspan="6">整型</td>
        <td rowspan="3">有</td>
        <td>(signed) int</td><td>16</td><td>[-2^15, 2^15-1]</td>
    </tr>
    <tr>
        <td>(signed) short</td><td>16</td><td>[-2^15, 2^15-1]</td>
    </tr>
    <tr>
        <td>(signed) long</td><td>32</td><td>[-2^31, 2^31-1]</td>
    </tr>
    <tr>
        <td rowspan="3">无</td>
        <td>unsigned int</td><td>16</td><td>[0, 2^16-1]</td>
    </tr>
    <tr>
        <td>unsigned short int</td><td>16</td><td>[0, 2^16-1]</td>
    </tr>
    <tr>
        <td>unsigned long int</td><td>32</td><td>[0, 2^32-1]</td>
    </tr>
    <tr>
        <td rowspan="2">浮点型</td>
        <td>有</td><td>float</td><td>32</td><td>(-2^128, 2^128)</td>
    </tr>
    <tr>
        <td>无</td><td>double</td><td>64</td><td>(-2^1024, 2^1024)</td>
    </tr>
    <tr>
        <td rowspan="2">字符型</td>
        <td>有</td><td>char</td><td>8</td><td>[-2^7, 2^7-1]</td>
    </tr>
    <tr>
        <td>无</td><td>unsigned char</td><td>8</td><td>[0, 2^8-1]</td>
    </tr>
</table>
</div>
<div align=center>
注：在计算机中，int表示32位，与单片机数据类型有所出入。
</div>

 &emsp;&emsp;于是根据上图，可以写出一个有输入参数的延时函数，该函数按照输入的无符号整型参数将函数体进行循环。代码如下：
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

//主函数
void main(){
  unsigned int cycle1 = 100;
  unsigned int cycle2 = 1000;
  while(1){
    P2 = 0x7f; // 0111 1111
    Delay1ms(cycle2);
    P2 = 0xbf; // 1011 1111
    Delay1ms(cycle2);
    P2 = 0xdf; // 1101 1111
    Delay1ms(cycle1);
    P2 = 0xef; // 1110 1111
    Delay1ms(cycle1);
    P2 = 0xf7; // 1111 0111
    Delay1ms(cycle1);
    P2 = 0xfb; // 1111 1011
    Delay1ms(cycle1);
    P2 = 0xfd; // 1111 1101
    Delay1ms(cycle1);
    P2 = 0xfe; // 1111 1110
    Delay1ms(cycle1);
  }
}
 ```

## 1.3 独立按键控制LED灯亮灭
 &emsp;&emsp; **轻触按键** 相当于是一种电子开关，按下时开关接通，松开时开关断开，实现原理是通过轻触按键内部的金属弹片受力弹动来实现接通和断开。内部连线和开发板原理图如下：

 

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git-Img/main/%E8%BD%BB%E8%A7%A6%E6%8C%89%E9%94%AE%E5%86%85%E9%83%A8%E8%BF%9E%E7%BA%BF.png" width="30%">

图 轻触按键内部连线

<img src="https://raw.githubusercontent.com/jjejdhhd/Git-Img/main/%E7%8B%AC%E7%AB%8B%E6%8C%89%E9%94%AE%E5%BC%80%E5%8F%91%E6%9D%BF%E5%8E%9F%E7%90%86%E5%9B%BE.png" width="50%">

图 独立按键开发板原理图

</div>

> - 四个按键分别是连接到MCU的P31、P30、P32、P33。
> - 上电后，所有IO口默认是高电平。按键松开高电平，按下低电平。
> 
>  **小技巧：** 右键打开头文件<REGX52.H>就可以查看IO的声明（比如一位IO口的声明）。

 &emsp;&emsp;下面讲一下按键开关的抖动问题。对于机械开关，当机械触点断开、闭合时，由于机械触点的弹性作用，一个开关在闭合时不会马上稳定地接通，在断开时也不会一下子断开，所以在开关闭合及断开的瞬间会伴随一连串的抖动。如下图所示：

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git-Img/main/%E6%8C%89%E9%94%AE%E5%BC%80%E5%85%B3%E6%8A%96%E5%8A%A8%E7%A4%BA%E6%84%8F%E5%9B%BE.png" width="50%">

图 轻触按键抖动示意图
</div>

软件消抖的原理就是，检测到按键按下后，延时10ms，再观察按键是否按下。注意这和FPGA中使用三个触发器消抖的思路不同。

 现在提出几个功能：
- 1.按键K1按下时，LED8亮；K1松开，LED8灭；
- 2.K2每次按下后，只有松开的时候，LED7改变一次状态；
- 3.K3每次按下后，8位LED自加1（要求D1是高位）；
- 4.K4每次按下后，8位LED左移一位。

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
  unsigned char LED_r=0;//由于无符号字符是8位，所以常用其表示一个寄存器
  while(1){
    //按键K1按下，LED8亮；松开，灭。
    if(!P3_1){ // P3_1按键按下
      Delay1ms(10);//软件消抖				
      while(!P3_1){ // 按键一直按着
        P2_7 = 0;
      };
      Delay1ms(10); // 按键松开时也消抖
      if(P3_1){
        P2_7 = 1;
      }
    }

    //按键K2每次按下，松开时LED7改变状态。
    if(!P3_0){
      // 下面这三句话连起来的作用：保证按键稳定松开后执行动作
      Delay1ms(10);//这里使用软件消抖
      while(!P3_0);//等着松开
      Delay1ms(10);
      
      if(P3_0){
        P2_6 = ~P2_6;
      }
    }

    //按键K3按下后松开时，8个LED在当前的状态下+1（D7是高位，LED灯亮表示1）
    if(!P3_2){
      Delay1ms(10);//这里使用软件消抖
      while(!P3_2);//等着松开
      Delay1ms(10);
      if(P3_2){
        // 方法一：三行引入变量，是为了防止"P2 = ~(++P2)"可能会造成的死锁
        // 比如P2 = 1111_1111 时，程序会锁在这个值不动。
        P2 = ~(P2++);
//        LED_r = ~P2;
//        LED_r++;
//        P2 = ~LED_r;
        
//        // 方法二
//        P2--;
      }
    }

    //按键K4按下后松开时，8个LED在当前的状态下左移1位
    if(!P3_3){
      Delay1ms(10);//这里使用软件消抖
      while(!P3_3);//等着松开
      Delay1ms(10);
      if(P3_3){
        if(P2 == 0xfe | P2 == 0xff)
          P2 = 0x7f;
        else
          P2 = ~(~P2>>1);//由于D8是高位，所以右移
      }
    }
  }
}
 ```

 &emsp;&emsp;注意到上面的函数中，每一个按键的事件是独立的，也就是，当按下一个按键后，其他按键将不会发生作用，直到当前按键的功能执行完成。另外，51单片机中常用 **无符号字符** 表示一个8位的寄存器。

