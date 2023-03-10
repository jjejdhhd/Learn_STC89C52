# 10 I2C总线

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
注：工程及代码文件放在了本人的[Github仓库](https://github.com/jjejdhhd/Learn_STC89C52)。
***

## 10.1 AT24C02和I2C介绍
**10.1.1 存储器介绍**
<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E5%AD%98%E5%82%A8%E5%99%A8%E5%88%86%E7%B1%BB.png" width=65%>
</div><div align=center>
图10-1 存储器分类
</div>

一般来说，RAM读写速度极快，但掉电丢失；而ROM相对来说读写速度慢，但是可以长期存储数据。

从实现功能的角度来说，只需要关注“地址总线”和“数据总线”即可。为了使主机和从机完成对这些数据的交互，便渐渐的演变出一些实用的特定格式的通信方式，但这些通信方式本身最重要的都是控制“地址”和“数据”的读写。

**10.1.2 AT24C02介绍**
AT24C02是一种可以实现掉电不丢失的存储器，可用于保存单片机运行时想要永久保存的数据信息。
> 存储介质：E2PROM。
> 通讯接口：I2C总线。
> 容量：256字节，即地址位宽位16，范围是0~255。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/AT89C02%E5%AE%9E%E7%89%A9%E5%9B%BE.png" width=60%>
</div><div align=center>
图10-2 AT24C02实物图
</div>

根据下面芯片的引脚，只需要记住此开发板上EEPROM的“器件地址”为000，其余的只需要考虑SCL和SDA两根线的通信细节即可。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/24C02%E5%8E%9F%E7%90%86%E5%9B%BE%E5%8F%8A%E5%8A%9F%E8%83%BD.png" width=60%>
</div><div align=center>
图10-3 AT24C02原理图及功能
</div>


**10.1.3 I2C总线介绍**
I2C总线（Inter IC BUS）是由Philips公司开发的一种通用数据总线，只需要两根通信线：SCL（Serial Clock）、SDA（Serial Data）便可以实现同步、半双工通信。通信过程中包含**数据应答位ACK**，进一步提高了通信的可靠性。通用的I2C总线，可以使各种设备的通信标准统一，对于厂家来说，使用成熟的方案可以缩短芯片设计周期、提高稳定性；对于应用者来说，使用通用的通信协议可以避免学习各种各样的自定义协议，降低了学习和应用的难度。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BD%BF%E7%94%A8I2C%E6%80%BB%E7%BA%BF%E7%9A%84%E8%8A%AF%E7%89%87%E4%B8%BE%E4%BE%8B.png" width=60%>
</div><div align=center>
图10-4 使用I2C总线的器件举例
</div>

图10-5给出了I2C多机通信的示意图。可以看出：
> - 所有I2C设备的SCL连在一起，SDA连在一起。
> - 设备的SCL和SDA均要配置成开漏输出模式（高电平时引脚悬空，无上拉电阻）。
> - SCL和SDA各添加一个上拉电阻，阻值一般为4.7KΩ左右。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/I2C%E5%A4%9A%E6%9C%BA%E9%80%9A%E4%BF%A1%E7%A4%BA%E6%84%8F%E5%9B%BE.png" width=55%>
</div><div align=center>
图10-5 I2C多机通信示意图
</div>

之所以这样设计，是因为**开漏输出和上拉电阻的共同作用**实现了“线与”的功能，可以解决多机通信互相干扰的问题。

**10.1.4 AT24C02的数据帧**
下面给出I2C数据帧中需要关注的6个关键问题：
> - 起始条件：SCL高电平期间，SDA从高电平切换到低电平。
> - 终止条件：SCL高电平期间，SDA从低电平切换到高电平。
> - 发送一个字节：SCL低电平期间，主机将数据位依次放到SDA线上（高位在前），然后拉高SCL，从机将在SCL高电平期间读取数据位，所以SCL高电平期间SDA不允许有数据变化，依次循环上述过程8次，即可发送一个字节。
> - 接收一个字节：SCL低电平期间，从机将数据位依次放到SDA线上（高位在前），然后拉高SCL，主机将在SCL高电平期间读取数据位，所以SCL高电平期间SDA不允许有数据变化，依次循环上述过程8次，即可接收一个字节（主机在接收之前，需要释放SDA）。
> - 发送应答：在接收完一个字节之后，主机在下一个时钟发送一位数据，数据0表示应答，数据1表示非应答。
> - 接收应答：在发送完一个字节之后，主机在下一个时钟接收一位数据，判断从机是否应答，数据0表示应答，数据1表示非应答（主机在接收之前，需要释放SDA）。

可以看出，最重要的就是，**SDA在SCL低电平由主机改变数据，SDA在高电平有从机读出数据**。相比于FPGA中可以精细的控制SCL时钟，单片机并不是很在意SCL线周期是否稳定，只要高低电平对就行了。

下面介绍具体的数据帧结构。对于I2C通信来说，共有如下模式：
> - 写模式：**字节写**、页写。
> - 读模式：当前地址读、**随机读**、顺序读。
>
> 注：由于读模式中不能指定地址，所以“随机读”模式通过借鉴写模式，使用“哑写”改变当前的地址。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/IIC%E5%AD%97%E8%8A%82%E5%86%99.png" width=60%>
</div>
<div align=center>
图10-6 IIC“字节写”的数据帧结构
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/IIC%E9%A1%B5%E5%86%99%E6%95%B0%E6%8D%AE%E5%B8%A7%E6%A0%BC%E5%BC%8F.png" width=80%>
</div>
<div align=center>
图10-7 IIC“页写”的数据帧结构
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/IIC%E5%BD%93%E5%89%8D%E5%9C%B0%E5%9D%80%E8%AF%BB%E6%95%B0%E6%8D%AE%E5%B8%A7%E7%BB%93%E6%9E%84.png" width=45%>
</div>
<div align=center>
图10-8 IIC“当前地址读”的数据帧结构
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/IIC%E9%9A%8F%E6%9C%BA%E8%AF%BB%E6%95%B0%E6%8D%AE%E5%B8%A7%E6%A0%BC%E5%BC%8F.png" width=70%>
</div>
<div align=center>
图10-9 IIC“随机读”的数据帧结构
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/IIC%E9%A1%BA%E5%BA%8F%E8%AF%BB%E6%95%B0%E6%8D%AE%E5%B8%A7%E6%A0%BC%E5%BC%8F.png" width=80%>
</div>
<div align=center>
图10-10 IIC“顺序读”的数据帧结构
</div>

由于目前不需要大数据量的通信，且I2C一般用于控制信号的传输，所以可以不考虑传输效率的问题；此外，为了方便进行模块化编程，所以每次读/写都希望可以指定一个地址，于是**采用“字节写”、“随机读”两种模式作为读写操作**。注意这两种方式每次都只能读/写1字节的信息。注意每次传输的第一个字节为器件地址的相关信息，格式如下：
<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git-Img/main/IIC%E5%99%A8%E4%BB%B6%E3%80%81%E5%9C%B0%E5%9D%80.png" width=45%>
</div>
<div align=center>
10-11 IIC协议“器件地址”数据帧格式
</div>


## 10.2 AT24C02数据存储
需求：在LCD1602上显示5位数据(unsigned int)，使用按键进行操作：
> - key1进行加一操作；
> - key2进行减一操作；
> - key3进行将此数据存储到EEPROM芯片（AT24C02）中；
> - key4将其从EEPROM芯片中读出来。
> 
> 注：为了验证是否存储到了EEPROM中，可以将开发板断电后，再从EEPROM芯片中读取数据进行验证。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8%E5%85%B3%E7%B3%BB-AT24C02%E6%95%B0%E6%8D%AE%E5%AD%98%E5%82%A8.png" width=20%>
</div>
<div align=center>
10-12 “AT24C02存储数据”代码调用关系
</div>

代码展示：
**- main.c**
```c
#include <REGX52.H>
#include "LCD1602.h"
#include "PushKey.h"
#include "AT24C02.h"

void main(){
  unsigned int num = 0;//要存储的数据
  unsigned char num_H8, num_L8;//数据的高8位和低8位
  unsigned char err1,err2;//表示操作的错误
  unsigned char key;
  //LCD初始化
  LCD_Init();
  //计算两个字节
  num_H8 = num/256;
  num_L8 = num%256;
  while(1){
    key = PushKey();
    if(key){
      switch(key){
        case 1: //数据加一
          num++;
          num_H8 = num/256; num_L8 = num%256;
          LCD_ShowString(2,1,"          ");
          break;
        case 2: //数据减一
          num--;
          num_H8 = num/256; num_L8 = num%256;
          LCD_ShowString(2,1,"          ");
          break;
        case 3: //数据存储
          err1 = AT24C02_WriteByte(0x02,num_H8);
          err2 = AT24C02_WriteByte(0x03,num_L8);
          if((!err1) && (!err2)){LCD_ShowString(2,1,"Write OK! ");}
          else                  {LCD_ShowString(2,1,"Write ERR!");}
          break;
        case 4: //数据读取
          num_H8 = AT24C02_ReadByte(0x02);
          num_L8 = AT24C02_ReadByte(0x03);
          num = num_H8*256 + num_L8;
          LCD_ShowString(2,1,"Read OK!"); 
          break;
        default:;
      }
    }
    LCD_ShowNum(1,1,num,5);
  }
}
```

**- AT24C02.h**
```c
#ifndef __AT24C02_H__
#define __AT24C02_H__

unsigned char AT24C02_WriteByte(unsigned char wr_addr,wr_byte);//字节写
unsigned char AT24C02_ReadByte(unsigned char rd_addr);//随机读
  
#endif
```

**- AT24C02.c**
```c
#include <REGX52.H>

// 下面可能需要更改
/**********************************/
// 对常用总线重命名
sbit SCL = P2^1;
sbit SDA = P2^0;
# define AT24C02_WR_ADDR 0xa0
# define AT24C02_RD_ADDR 0xa1

// 等待写周期的延时函数
void AT24C02_Delay5ms(){//@11.0592MHz
	unsigned char i, j;
	i = 9;
	j = 244;
	do
	{
		while (--j);
	} while (--i);
}
/**********************************/

// 固定格式的函数
/**********************************/
/**
  * @brief :采用I2C“字节写”的方式，向AT24C02写入1字节数据。
  * @param :wr_addr写入地址，wr_byte写入数据
  * @retval :代表写入是否成功。
 */
unsigned char AT24C02_WriteByte(unsigned char wr_addr,wr_byte){
  unsigned char err=0;
  unsigned char i;
  //起始条件
  SDA = 1;
  SCL = 1;
  SDA = 0;
  SCL = 0;
  //发送第一字节数据
  for(i=0;i<8;i++){
    SCL = 0;
    SDA = AT24C02_WR_ADDR&(0x80>>i);
    SCL = 1;
  }
  //接收第一次应答
  SCL = 0;
  SDA= 1; //开漏输出，释放SDA
  SCL = 1;
  if(SDA){err=1;}
  SCL = 0;
  if(err){return err;}
  //发送第二字节数据：地址
  for(i=0;i<8;i++){
    SCL = 0;
    SDA = wr_addr&(0x80>>i);
    SCL = 1;
  }
  //接收第二次应答
  SCL = 0;
  SDA= 1; //开漏输出，释放SDA
  SCL = 1;
  if(SDA){err=1;}
  SCL = 0;
  if(err){return err;}
  //发送第三字节数据：数据
  for(i=0;i<8;i++){
    SCL = 0;
    SDA = wr_byte&(0x80>>i);
    SCL = 1;
  }
  //接收第三次应答
  SCL = 0;
  SDA= 1; //开漏输出，释放SDA
  SCL = 1;
  if(SDA){err=1;}
  if(err){return err;}
  //终止条件
  SCL = 0;
  SDA = 0;
  SCL = 1;
  SDA = 1;
  //等待写周期完成
  AT24C02_Delay5ms();
  return err;
}

/**
  * @brief :采用I2C“随机读”的方式，从AT24C02指定地址读出1字节数据。
  * @param :rd_addr读出地址
  * @retval :读出的数据。
  * 一般读不要会出错，所以就不返回错误标志err了，但保留其定义。
 */
unsigned char AT24C02_ReadByte(unsigned char rd_addr){
  unsigned char rd_byte = 0x00;
  unsigned char err=0,wr_byte1=0xa0;
  unsigned char i;
  //起始条件
  SDA = 1;
  SCL = 1;
  SDA = 0;
  SCL = 0;
  //发送第一字节数据
  for(i=0;i<8;i++){
    SCL = 0;
    SDA = AT24C02_WR_ADDR&(0x80>>i);
    SCL = 1;
  }
  //接收第一次应答
  SCL = 0;
  SDA= 1; //开漏输出，释放SDA
  SCL = 1;
  err = SDA; 
  //发送第二字节数据：地址
  for(i=0;i<8;i++){
    SCL = 0;
    SDA = rd_addr&(0x80>>i);
    SCL = 1;
  }
  //接收第二次应答
  SCL = 0;
  SDA= 1; //开漏输出，释放SDA
  SCL = 1;
  err = SDA; 
  SCL = 0;
  //起始条件
  SDA = 1;
  SCL = 1;
  SDA = 0;
  SCL = 0;
  //发送第一字节数据
  for(i=0;i<8;i++){
    SCL = 0;
    SDA = AT24C02_RD_ADDR&(0x80>>i);
    SCL = 1;
  }
  //接收第三次应答
  SCL = 0;
  SDA= 1; //开漏输出，释放SDA
  SCL = 1;
  err = SDA;
  SCL = 0;
  //接收数据：数据
  for(i=0;i<8;i++){
    SCL = 0;
    if(SDA){rd_byte |= (0x80>>i);}
    SCL = 1;
  }
  //NO ACK：主机对SDA无动作
  SCL = 0;
  SDA = 1;
  SCL = 1;
  SCL = 0;
  //终止条件
  SCL = 0;
  SDA = 0;
  SCL = 1;
  SDA = 1;
  return rd_byte;
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

**- LCD1602.h**、**- LCD1602.c**与第8节“8DS1302实时时钟”相同。

编程感想：
> - 单片机软件编程不用精细的考虑SCL线的变化，为了保证SDA的正确性，即使上一个步骤已经有```SCL=0```，下一个步骤的开始也可以再次执行```SCL=0```。
> - 卡了很久的bug：写操作时，第三个字节写数据其实是在“写地址”，不知怎么就一直没看出来。:sweat_smile:
> - 卡了很久的bug：在说明书中提到，“字节写”结束后，EEPROM会进入写周期（大概5ms），直到内部写完成后才会响应新的请求。所以**不能连续写两个字节！！ 要给写周期留有一定的延时。**
> - 卡了很久的bug：在读取数据的过程中，此语句进行赋值可行```if(SDA){rd_byte |= (0x80>>i);}```，而后面这个语句则不可行```rd_byte |= (SDA&(0x80>>i));```！可见，**软件编程中，跨位宽赋值时还是尽量用判断，避免位运算**。



## 10.3 AT24C02秒表（定时器扫描按键及数码管）
本实验重点在于**使用定时器扫描按键和数码管**，而不是像之前一样采用软件延时，从而会陷入到死循环当中。
1. 对于按键来说。设置一个静态全局变量，表示上一次按下的按键编号。每次中断计时（如20ms）来临时，进入按键检测子函数，看看当前是否为上升沿，从而更新静态全局变量。主函数每次请求按键，则返回这个静态全局变量。如果想在数码管上长久地看到这个按键，那就将其归零的过程放在返回函数中，而不是```PushKey_Loop()```函数中。
2. 对于数码管来说。设置一个8位静态全局数组，存储8个数码管所需要显示的数据。每次中断计时（如2ms）来临时，进入数码管刷新子函数。这个数码管刷新子函数只负责刷新数据缓冲区的数据（使用for保证每次只刷新一个数码管）。而对于这些数据的更改，则是在主函数中调用相应的函数实现。



需求：在数码管上显示秒表“时-分-秒”，并使用按键进行控制：
> - key1进行开始/暂停功能；
> - key2将当前时间清零；
> - key3将当前的数据写入到EEPROM芯片（AT24C02）中；
> - key4则从EEPROM中将数据读回来（需掉电验证）。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8%E7%BB%93%E6%9E%84-AT24C02%E7%A7%92%E8%A1%A8.png" width=20%>
</div>
<div align=center>
10-12 “AT24C02存储数据”代码调用关系
</div>

代码展示：
**- main.c**
```c
#include <REGX52.H>
#include "Timer0.h"
#include "PushKey.h"
#include "NixieTube.h"
#include "AT24C02.h"

// 定义分/秒/10倍毫秒/秒表工作标志
unsigned char mins=0,secs=0,ms10=0,StopWatch_flag=0;

void main(){
  unsigned char num_key=0;
  //定时器T0初始化
  Timer0_Init();
  //数码管显示初始化
  NixieTube_SetBuf(6,11);
  NixieTube_SetBuf(3,11);
  while(1){
    num_key = PushKey_GetKey();
    switch(num_key){
      // 开始/暂停秒表
      case 1:
        StopWatch_flag = !StopWatch_flag;
        break;
      // 秒表归零
      case 2:
        ms10 = 0;
        secs = 0;
        mins = 0;
        break;
      // 将当前数据写入到EEPROM中
      case 3:
        AT24C02_WriteByte(0x00,ms10);
        AT24C02_WriteByte(0x01,secs);
        AT24C02_WriteByte(0x02,mins);
        break;
      // 从EEPROM读取当前数据
      case 4:
        ms10 = AT24C02_ReadByte(0x00);
        secs = AT24C02_ReadByte(0x01);
        mins = AT24C02_ReadByte(0x02);
        break;
      default:;
    }
    NixieTube_SetBuf(1,ms10%10);
    NixieTube_SetBuf(2,ms10/10);
    NixieTube_SetBuf(4,secs%10);
    NixieTube_SetBuf(5,secs/10);
    NixieTube_SetBuf(7,mins%10);
    NixieTube_SetBuf(8,mins/10);
  }
}

// 秒表更新函数
void StopWatch_Loop(void){
  if(ms10>=99){
    ms10 = 0;
    if(secs>=59){
      secs = 0;
      if(mins>=59){mins=0;}
      else        {mins++;}
    }
    else{secs++;}
  }
  else{ms10++;}
}

// 定义定时器T0中断后要执行的动作
void Timer0_Routine() interrupt 1{
  static unsigned int count1_T0,count2_T0,count3_T0; //中断次数
  TH0 = 0xfc; TL0 = 0x66; // 恢复溢出周期，近似1ms
  //数码管扫描
  count1_T0++; //数码管使用的中断
  if(count1_T0>1){
    count1_T0 = 0;
    NixieTube_Loop();
  }
  //按键扫描
  count2_T0++; //按键使用的中断
  if(count2_T0>20){
    count2_T0 = 0;
    PushKey_Loop();
  }
  //秒表计数
  count3_T0++;
  if(count3_T0>10){
    count3_T0 = 0;
    if(StopWatch_flag){StopWatch_Loop();}
  }
}
```

**- NixieTube.h**
```c
#ifndef __NIXIETUBE_H__
#define __NIXIETUBE_H__

// 数据缓冲区，注意每个数字显示的范围就是0~9
unsigned char NixieTube_buf[8] = {1,2,3,4,5,6,7,8};

// 给出数字0~9的定义（符合数组的索引），最后两个依次表示不显示/显示横杠
unsigned char NixieTube_number[12] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x40};


/**
  * @brief :改变数码管数据缓冲区的数值。
* @param :index索引1~8，num数值0~11(10表示不显示，11表示横杠)
  * @retval :无
 */
void NixieTube_SetBuf(unsigned char index,num){
  NixieTube_buf[index-1] = num;
}

/**
  * @brief :在指定数码管显示指定的数字。
  * @param :led片选信号0~7，num表示显示的索引0~11
  * @retval :无。
 */
void NixieTube_Signle(unsigned char led, unsigned char num){
  // 给出选择的LED1~LED8的定义（实际上只用P2的2/3/4引脚）
  unsigned char sel_led[8] = {0x00,0x04,0x08,0x0c,0x10,0x14,0x18,0x1c};
  P0 = 0x00; // 数码管不显示，消影
  P2 = sel_led[led]; // 选择数码管：LED1
  P0 = NixieTube_number[num];  // 数码管显示
}

//定时器中断函数执行的数码管扫描函数，每次只扫描一个数码管
void NixieTube_Loop(void){
  static unsigned char i=0;
  NixieTube_Signle(i,NixieTube_buf[i]);
  i++;
  if(i>=8){i=0;}
}

#endif
```

**- PushKey.h**
```c
#ifndef __PUSHKEY_H__
#define __PUSHKEY_H__

// 上一次按下的按键
unsigned char num_PushKey=0;

/**
  * @brief :告诉主函数按下了哪个按键。
  * @param :无
  * @retval :按键编号1~4
 */
unsigned char PushKey_GetKey(void){
  unsigned char temp;
  temp = num_PushKey;
  num_PushKey = 0;
  return temp;
}

/**
  * @brief :检测按下了哪个按键开关
  * @param :无
  * @retval :输出按键开关编号1~4，不按返回0，松开触发
 */
unsigned char PushKey_SingleDetect(void){
  unsigned char key = 0;
  if(!P3_1)     {key=1;}
  else if(!P3_0){key=2;}
  else if(!P3_2){key=3;}
  else if(!P3_3){key=4;}
  return key;
}

//定时器中断函数执行的按键扫描函数
void PushKey_Loop(void){
  static unsigned char state_last=0, state_curr=0;
  state_last = state_curr;
  state_curr = PushKey_SingleDetect();
  if     (state_last==1 && state_curr==0){num_PushKey=1;}
  else if(state_last==2 && state_curr==0){num_PushKey=2;}
  else if(state_last==3 && state_curr==0){num_PushKey=3;}
  else if(state_last==4 && state_curr==0){num_PushKey=4;}
}

#endif
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

**AT24C02.h**、**AT24C02.c**与上一小节的实验相同。

编程感想：
> 1. 小bug：注意```static```和```const```不要写混了。
