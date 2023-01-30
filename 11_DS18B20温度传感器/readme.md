# 11 DS18B02温度传感器

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
***

## 11.1 温度传感器与单总线通信
**11.1.1 DS18B20温度传感器**
DS18B20是一种常见的数字温度传感器，其控制命令和数据都是以数字信号的方式输入输出，相比较于模拟温度传感器，具有功能强大、硬件简单、易扩展、抗干扰性强等特点。
> 测温范围：-55°C 到 +125°C
> 通信接口：1-Wire（单总线）
> 其它特征：可形成总线结构、内置温度报警功能、可寄生供电。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DS18B02%E5%AE%9E%E7%89%A9%E5%9B%BE.png" width=65%>
</div><div align=center>
图11-1 DS18B20实物图
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DS18B20%E5%8E%9F%E7%90%86%E5%9B%BE.png" width=65%>
</div><div align=center>
图11-2 DS18B20原理图
</div>

> 小知识：
> VCC是表示电路的意思，指的是接入电路；
> VDD表示器件的意思，指的是器件内部的工作电压；
> VSS表示公共连接段的意思，指的是公共接地端电压的意思。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DS18B20%E5%8E%9F%E7%90%86%E6%A1%86%E5%9B%BE.png" width=80%>
</div><div align=center>
图11-3 DS18B20结构框图
</div>

> - 左侧方框为“寄生供电”电路：当VDD无法供电时，DQ会承担起供电的任务，但为保证内部电路正常工作需要DQ强上拉，本次实验不涉及。
> - 中间方框为ROM和单总线接口：ROM中会存放器件地址，用于单总线通信。
> - MEMROY CONTROL LOGIC：相当于“管家”，在进行单总线通信时，将存储在SRATCHPAD(RAM)中的数据放在总线上。
> - SCRATCHPAD（暂存器）：本质上是RAM，用于总线的数据交互，存储温度等信息。
> - EEPROM：用于保存温度触发阈值、配置参数（如温度的分辨率）等信息。
> 
> 注：出厂默认温度分辨率为最高：0.0125℃。


<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DS18B20%E5%AD%98%E5%82%A8%E9%98%B5%E5%88%97.png" width=70%>
</div><div align=center>
图11-4 DS18B20存储阵列
</div>

可见，上面的“暂存器”只包含9个字节的空间：
> - Byte 0&1：存储温度信息。高5位全是符号位，1负0正；其他11位组成数字，注意低4位是小数部分。
> <img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DS18B20%E6%B8%A9%E5%BA%A6%E6%A0%BC%E5%BC%8F.png" width=60%>
> - Byte 2/3：分别存储温度阈值上限及下限；
> - Byte 4：配置寄存器，用于控制温度的分辨率。当然，分辨率越高，温度转换时间越长。
> <img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DS18B20%E9%85%8D%E7%BD%AE%E5%AF%84%E5%AD%98%E5%99%A8.png" width=40%>
> - Byte 5~6：保留。
> - Byte 7：CRC校验。
> 
> <u>通过单总线上的指令，可以实现传感器上的温度信息存入暂存器中，然后再从暂存器将信息通过单总线传输出去。</u>

**11.1.2 单总线协议及电路规范**
单总线（1-Wire BUS）是由Dallas公司开发的一种通用数据总线。通过**一根通信线DQ**，实现**异步、半双工通信**。当采用寄生供电时，还可以省去设备的VDD线路，此时，供电加通信只需要DQ和GND两根线。但实际上，使用单总线进行通信的器件非常少，实际应用中几乎只有DS18B20比较出名。

设备的DQ均要配置成开漏输出模式（输出1引脚悬空，无驱动能力），所以DQ需要添加一个上拉电阻，阻值一般为4.7KΩ左右。若此总线的从机采取寄生供电，则主机还应配一个强上拉输出电路。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DS18B20%E4%BE%9B%E7%94%B5.png" width=60%>
</div><div align=center>
图11-5 DS18B20供电示意图
</div>

**11.1.3 单总线时序结构**
本部分将依次介绍：初始化→发送一位→接收一位→发送一个字节→接收一个字节。**单总线通信对于时间的把控很重要。**
> - **初始化**：主机将总线拉低至少480us，然后释放总线，等待15\~60us后，存在的从机会拉低总线60\~240us以响应主机，之后从机将释放总线。
> <img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E5%8D%95%E6%80%BB%E7%BA%BF-%E5%88%9D%E5%A7%8B%E5%8C%96%E6%97%B6%E5%BA%8F.png" width=60%>
> - 发送一位：主机将总线拉低60\~120us，然后释放总线，表示发送0；主机将总线拉低1\~15us，然后释放总线，表示发送1。从机将在总线拉低30us后（典型值）读取电平，整个时间片应大于60us。
> <img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E5%8D%95%E6%80%BB%E7%BA%BF-%E5%8F%91%E9%80%81%E4%B8%80%E4%BD%8D.png" width=60%>
> - 接收一位：主机将总线拉低1~15us，然后释放总线，并在拉低后15us内读取总线电平（尽量贴近15us的末尾），读取为低电平则为接收0，读取为高电平则为接收1 ，整个时间片应大于60us。
> <img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E5%8D%95%E6%80%BB%E7%BA%BF-%E6%8E%A5%E6%94%B6%E4%B8%80%E4%BD%8D.png" width=60%>
> - 发送一个字节：连续调用8次发送一位的时序，依次发送一个字节的8位 **（低位在前）**。
> - 接收一个字节：连续调用8次接收一位的时序，依次接收一个字节的8位 **（低位在前）**。





**11.1.4 单总线数据帧**
DS18B20操作流程为：
> 1. 初始化：从机复位，主机判断从机是否响应。
> 2. ROM操作：ROM指令+本指令需要的读写操作。
> 3. 功能操作：功能指令+本指令需要的读写操作。
>
> 注：总体思路其实就是先找器件地址（ROM操作），然后执行相应的功能（功能操作）。

下面介绍ROM指令：
> - SEARCH ROM[F0h]：	用于确定挂接在同一总线上 DS1820 的个数和识别 64 位 ROM 地址。为操作各器件作好准备。
> - READ ROM[33h]：读DS1820温度传感器ROM中的编码（即64位地址）
> - MATCH ROM[55h]：发出此命令之后，接着发出 64 位 ROM 编码，访问单总线上与该编码相对应的 DS1820 使之作出响应，为下一步对该 DS1820 的读写作准备。
> - **SKIP ROM[CCh]**：忽略 64 位 ROM 地址，直接向 DS1820 发温度变换命令。适用于**单片工作。**
> - ALARM SEARCH[ECh]：	执行后只有温度超过设定值上限或下限的片子才做出响应。

下面介绍功能指令：
> - **CONVERT T[44h]**：启动DS1820进行温度转换，12位转换时最长为750ms（9位为93.75ms）。结果存入内部第0、1字节RAM中。
> - WRITE SCRATCHPAD[4Eh]：发出向内部RAM的第2、3和4字节写上、下限温度数据命令，紧跟该命令之后，是传送三字节的数据。
> - **READ SCRATCHPAD[BEh]**：连续读取内部RAM中9个字节的内容
> - COPY SCRATCHPAD[48h]：将RAM中第2、3和4字节字节的内容复制到EEPROM中。
> - RECALL E2[B8h]：将EEPROM中内容恢复到RAM中的第2、3和4字节。
> - READ POWER SUPPLY[B4h]：读DS1820的供电模式。寄生供电时DS1820发送“ 0 ”，外接电源供电 DS1820发送“ 1 ”。

下面是程序中会用到的两个数据帧：
> 1. 温度变换：初始化 → 跳过ROM（发送字节0xcc）→ 开始温度变换（发送字节0x44）。作用就是使传感器将当前温度存放在暂存器的前两个字节上。
> 2. 温度读取：初始化 → 跳过ROM（发送字节0xcc）→ 读暂存器（发送字节0xbe）→ 连续的读操作（只读取前两个比特）。作用就是去取出温度和阈值上下限。

## 11.2 实验：DS18B02温度读取
需求：使用DS18B02的最高分辨率，在LCD上显示屏幕上显示温度。LCD第一行显示“Temperature:”，第二行显示温度。
注：由于延时1us时间太短，所以直接调用软件延时函数的误差极大（循环跳转语句的延时可能比演示本身所需的时间更长）；而使用定时器又涉及到中断等操作，会使调用逻辑更复杂。所以推荐直接根据延时时间，使用“软件延时函数”中最核心的延迟部分即可，如延时480us。
> 延时注释：对于12MHz时钟来说
> - ```_nop()_```函数大约延时1us；
> - 进入函数大约延时4us；
> - 进入一次while大概延时2us；
> - 总结：**在使用“软件延时计算器”时，如果要延时5us，就要记住进入函数大约4us，所以要截取“延时9us”的核心部分。**

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8%E5%85%B3%E7%B3%BB-%E6%B8%A9%E5%BA%A6%E8%AF%BB%E5%8F%96.png" width=20%>
</div><div align=center>
图11-6 “DS18B20温度读取”代码调用关系
</div>

代码展示：
**- main.c**
```c
#include <REGX52.H>
#include "LCD1602.h"
#include "_1Wire.h"
#include "DS18B20.h"

void main(){
  float tempe = 10.5;
  //初始化LCD
  LCD_Init();
  LCD_ShowString(1,1,"Temperature:");
  while(1){
    DS18B02_ConvertTemp();// 温度转换
    tempe = DS18B02_ReadTemp();// 温度读取
//    LCD_ShowBinNum(2,1,tempe,16);//显示16位二进制数字
    if(tempe>0){LCD_ShowChar(2,1,'+');}
    else       {LCD_ShowChar(2,1,'-');tempe=-tempe;}
    LCD_ShowNum(2,2,(int)tempe,3);
    LCD_ShowString(2,5,".");
    LCD_ShowNum(2,6,(int)((tempe-(int)tempe)*10000),4);
    LCD_ShowString(2,11,"C");
  }
}
```

**- _1Wire.h**
```c
#ifndef __1WIRE_H__
#define __1WIRE_H__

unsigned char _1Wire_Init(void);//初始化
void _1Wire_SendBit(unsigned char sdata);// 发送1位
unsigned char _1Wire_ReceBit(void);// 接收1位
void _1Wire_SendByte(unsigned char sbyte);// 发送一个字节
unsigned char _1Wire_ReceByte(void);// 接收一个字节

#endif
```

**- _1Wire.c**
```c
#include <REGX52.H>

// 总线重新命名
sbit _1Wire_DQ = P3^7;

/**
  * @brief :单总线初始化。
  * @param :无
  * @retval :从机是否应答（1/0）。
 */
unsigned char _1Wire_Init(void){
  unsigned char i, ack=0;
  _1Wire_DQ = 1;
  _1Wire_DQ = 0;
  i = 230; while (--i); //延时500us
  _1Wire_DQ = 1;
  i = 36;	while (--i); //延时80us
  if(!_1Wire_DQ){ack = 1;}
  i = 230; while (--i); //延时500us
  return ack;
}

/**
  * @brief :单总线发送一位数据。
  * @param :sdata发送数据，最低位表示要发送的比特
  * @retval :无
 */
void _1Wire_SendBit(unsigned char sdata){// 发送1位
  unsigned char i;
  _1Wire_DQ = 1;
  _1Wire_DQ = 0;
  i = 4; while (--i); // 延时10us
  _1Wire_DQ = sdata;
  i = 23;	while (--i); // 延时50us，等待从机采样完毕
  _1Wire_DQ = 1;
}

/**
  * @brief :单总线接收一位数据。
  * @param :无
  * @retval :rdata接收数据，最低位表示接收的比特
 */
unsigned char _1Wire_ReceBit(void){// 接收1位
  unsigned char i, rdata=0x00;
  _1Wire_DQ = 1;
  _1Wire_DQ = 0;
  i = 2; while (--i); // 延时5us
  _1Wire_DQ = 1;
  i = 4; while (--i);// 延时10us
  if(_1Wire_DQ) rdata = 0x01;
  i = 20;	while (--i); // 延时45us，等待时间片结束
  //此处由从机释放总线
  return rdata;
}

/**
  * @brief :单总线发送一个字节。
  * @param :sbyte发送的字节。
  * @retval :无。
 */
void _1Wire_SendByte(unsigned char sbyte){// 发送一个字节
  unsigned char i;
  for(i=0;i<8;i++){
    _1Wire_SendBit(sbyte&(0x01<<i));//先发低位
  }
}

/**
  * @brief :单总线接收一个字节。
  * @param :无。
  * @retval :rbyte接收的字节。
 */
unsigned char _1Wire_ReceByte(void){// 接收一个字节
  unsigned char i,rbyte = 0x00;
  for(i=0;i<8;i++){
    if(_1Wire_ReceBit()){
      rbyte |= (0x01<<i);//先收低位
    }
  }
  return rbyte;
}
```

**- DS18B20.h**
```c
#ifndef __DS18B02_H__
#define __DS18B02_H__

void DS18B02_ConvertTemp(void);// 温度转换
float DS18B02_ReadTemp(void);// 温度读取

#endif
```

**- DS18B20.c**
```c
#include "_1Wire.h"

// ROM指令、功能指令
#define SKIP_ROM 0xCC
#define CONVERT_T 0x44
#define READ_SCRATCHPAD 0xBE

/**
  * @brief :将传感器温度值存放到暂存器中。
  * @param :无。
  * @retval :无。
 */
void DS18B02_ConvertTemp(void){// 温度转换
  //1.初始化
  _1Wire_Init();
  //2.跳过ROM
  _1Wire_SendByte(SKIP_ROM);
  //3.开始温度变换
  _1Wire_SendByte(CONVERT_T);
  //4.不必延时等待
}

/**
  * @brief :将暂存器中的温度值读出。
  * @param :无。
  * @retval :返回单精度浮点类的数据。
 */
float DS18B02_ReadTemp(void){// 温度读取
  unsigned char temp_H,temp_L;
  int temperature;
  //1.初始化
  _1Wire_Init();
  //2.跳过ROM
  _1Wire_SendByte(SKIP_ROM);
  //3.读暂存器
  _1Wire_SendByte(READ_SCRATCHPAD);
  //4.读前两个比特
  temp_L = _1Wire_ReceByte();
  temp_H = _1Wire_ReceByte();
  //5.复位，不再读取后续信息
  _1Wire_Init();
  //6.计算读取到的温度信息
  if(temp_H&0xf8){//负数
    temperature = -(~(temp_H*256+temp_L) + 1);
  }else{
    temperature = temp_H*256 + temp_L;
  }
  return (float)(temperature)*0.0625;
}
```

**LCD1602.h**、**LCD1602.c**均与之前的实验相同。

## 11.3 实验：DS18B20温度报警器
需求：LCD第一行的左侧显示当前温度，右侧显示“High!”/“Low!”的温度报警提示（若在范围内则什么都不显示）；LCD第二行显示设定的温度范围上下界。并且要求按键功能：
> - key1：将上限温度加一，并存储在AT24C02中；
> - key2：将上限温度减一，并存储在AT24C02中；
> - key3：将下限温度加一，并存储在AT24C02中；
> - key4：将下限温度减一，并存储在AT24C02中；
>
> 注：每次上电都自动读取AT24C02的值。初始温度范围设置为15~20摄氏度。
> 注：本实验相比于上一个实验的难点在于，定时器扫描按键。而不是真的将温度的上下限传输到DS18B20中。 
> 为显示方便，设置温度的上下限范围都在-60~127内，精度为1℃。


<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8-%E6%B8%A9%E5%BA%A6%E6%8A%A5%E8%AD%A6%E5%99%A8.png" width=20%>
</div><div align=center>
图11-7 “DS18B20温度报警器”代码调用关系
</div>

代码如下：
**- main.c**
```c
#include <REGX52.H>
#include "LCD1602.h"
#include "_1Wire.h"
#include "DS18B20.h"
#include "PushKey.h"
#include "Timer0.h"
#include "AT24C02.h"

void main(){
  float tempe = 20.5, tempe_show = 20.5;
  char gate_up=20,gate_bo=15; //默认阈值
  unsigned char key_num=0;
  
  //初始化定时器T0
  Timer0_Init();
  
  //从AT24C02中读取温度数据
//  AT24C02_WriteByte(0x00,125);
//  AT24C02_WriteByte(0x01,-125);//此两句仅供测试使用
  gate_up = AT24C02_ReadByte(0x00);
  gate_bo = AT24C02_ReadByte(0x01);
  if(gate_up<=gate_bo || gate_up>127 || gate_bo<-60){
    gate_up = 20; gate_bo = 15;
  }
  
  //初始化LCD
  LCD_Init();
  LCD_ShowString(1,1,"U:     L:");
  LCD_ShowSignedNum(1,3,(int)gate_up,3);
  LCD_ShowSignedNum(1,10,(int)gate_bo,3);
  
  while(1){
    key_num = PushKey_GetKey();
    if(key_num){
      switch(key_num){
        case 1://温度上限+1
          if(gate_up<127){gate_up++;}
          AT24C02_WriteByte(0x00,gate_up);
          break;
        case 2://温度上限-1
          if(gate_up>-59){gate_up--;}
          if(gate_up<=gate_bo){gate_bo--;}
          AT24C02_WriteByte(0x00,gate_up);
          AT24C02_WriteByte(0x01,gate_bo);
          break;
        case 3://温度下限+1
          if(gate_bo<126){gate_bo++;}
          if(gate_up<=gate_bo){gate_up++;}
          AT24C02_WriteByte(0x00,gate_up);
          AT24C02_WriteByte(0x01,gate_bo);
          break;
        case 4://温度下限-1
          if(gate_bo>-60){gate_bo--;}
          AT24C02_WriteByte(0x01,gate_bo);
          break;
        default:;
      }
      //在显示屏上更新显示阈值
      LCD_ShowSignedNum(1,3,(int)gate_up,3);
      LCD_ShowSignedNum(1,10,(int)gate_bo,3);
    }
    
    DS18B20_ConvertTemp();// 温度转换
    tempe = DS18B20_ReadTemp();// 温度读取
    tempe_show = tempe;
    //阈值判断
    if(tempe>gate_up)     {LCD_ShowString(2,12,"High!");}
    else if(tempe<gate_bo){LCD_ShowString(2,12," Low!");}
    else                  {LCD_ShowString(2,12,"Norm!");}
    //温度显示
    if(tempe>0){LCD_ShowChar(2,1,'+');}
    else       {LCD_ShowChar(2,1,'-');tempe_show=-tempe_show;}
    LCD_ShowNum(2,2,(int)tempe_show,3);
    LCD_ShowString(2,5,".");
    LCD_ShowNum(2,6,(int)((tempe_show-(int)tempe_show)*100),2);
    LCD_ShowString(2,9,"C");
  }
}

// 定义定时器T0中断后要执行的动作
void Timer0_Routine() interrupt 1{
  static unsigned int count_T0; //中断次数
  count_T0++; //更新中断次数
  TH0 = 0xfc; TL0 = 0x66; // 恢复溢出周期，近似1ms
  if(count_T0>20){
    count_T0 = 0;
    PushKey_Loop();
  }  
}
```

**- DS18B20.c**只是更改了全局中断的关闭和开启。
```c
#include "_1Wire.h"
#include <REGX52.H>

// ROM指令、功能指令
#define SKIP_ROM 0xCC
#define CONVERT_T 0x44
#define READ_SCRATCHPAD 0xBE

/**
  * @brief :将传感器温度值存放到暂存器中。
  * @param :无。
  * @retval :无。
 */
void DS18B20_ConvertTemp(void){// 温度转换
  EA = 0;
  //1.初始化
  _1Wire_Init();
  //2.跳过ROM
  _1Wire_SendByte(SKIP_ROM);
  //3.开始温度变换
  _1Wire_SendByte(CONVERT_T);
  //4.不必延时等待
  EA = 1;
}

/**
  * @brief :将暂存器中的温度值读出。
  * @param :无。
  * @retval :返回单精度浮点类的数据。
 */
float DS18B20_ReadTemp(void){// 温度读取
  unsigned char temp_H,temp_L;
  int temperature;
  EA = 0;
  //1.初始化
  _1Wire_Init();
  //2.跳过ROM
  _1Wire_SendByte(SKIP_ROM);
  //3.读暂存器
  _1Wire_SendByte(READ_SCRATCHPAD);
  //4.读前两个比特
  temp_L = _1Wire_ReceByte();
  temp_H = _1Wire_ReceByte();
  //5.复位，不再读取后续信息
  _1Wire_Init();
  EA = 1;
  //6.计算读取到的温度信息
  if(temp_H&0xf8){//负数
    temperature = -(~(temp_H*256+temp_L) + 1);
  }else{
    temperature = temp_H*256 + temp_L;
  }
  return (float)(temperature)*0.0625;
}
```

**DS18B20.h**、**_1Wire.h**、**_1Wire.c**、**LCD1602.h**、**LCD1602.c**与上一节实验相同；**AT24C02.h**、**AT24C02.c**、**PushKey.h**、**Timer0.h**、与12-2实验“秒表（定时器扫描按键和数码管）”相同。

编程感想：
> 1. 单总线操作会被定时器中断打乱，进而使得温度的显示“闪烁”。那么在单总线上操作时，直接关闭全局中断，单总线传输结束后，再开启全局中断。但注意，这个方法的缺点就是，不能对定时器的中断要求严格（如秒表等应用场景）。
> 2. 对单总线的评价：单总线工作时不允许中断，这就很大程度上限制了单总线的应用；而IIC总线只与自己的时钟线有关，所以允许任意时刻的中断，IIC也就应用得更广泛一些。
