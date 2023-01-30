# 15 红外遥控

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
注：工程及代码文件放在了本人的[Github仓库](https://github.com/jjejdhhd/Learn_STC89C52)。
***

## 15.1 红外遥控与外部中断
**15.1.1 红外遥控器**
红外遥控是利用红外光进行通信的设备，由 **红外LED** 将调制后的信号发出，由专用的红外接收头进行解调输出。
> - 通信方式：单工，异步
> - 红外LED波长：940nm（还有一种850nm的可以微微看见红光）
> - 通信协议标准：NEC标准

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E7%BA%A2%E5%A4%96%E9%81%A5%E6%8E%A7%E5%AE%9E%E7%89%A9%E5%9B%BE.png" width=60%>
</div><div align=center>
图15-1 红外遥控实物图
</div>


<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E7%BA%A2%E5%A4%96%E5%8F%91%E5%B0%84%E5%92%8C%E6%8E%A5%E6%94%B6%E5%8E%9F%E7%90%86%E5%9B%BE.png" width=70%>
</div><div align=center>
图15-2 红外发射和接收原理图
</div>

对于红外发射电路来说，要发送的信号应该调制在一定的载频上，才能使得红外接收模块从自然界中识别出遥控器所发出的信号。要实现这个目的，本质上可以采用方式一进行硬件调制，此时输出的红外LED在IN输出高电平时不导通，而在IN输出低电平时，以38kHz的频率闪烁，从而有区别于自然光。软件调制则是提前将IN调制好再进行发送。
而红外接收时，则要完成一系列负载的解调、滤波工作，这些器件集成在红外接收头中，无需关心。最后输出的信号OUT与原来发送的信号IN一致。如下图所示：

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E7%BA%A2%E5%A4%96%E8%B0%83%E5%88%B6%E8%A7%A3%E8%B0%83%E7%A4%BA%E6%84%8F%E5%9B%BE.png" width=80%>
</div><div align=center>
图15-3 红外调制解调示意图
</div>

注意若没有接收到信号，接收端默认输出高电平；接收到信号才输出低电平。**行业内默认空闲信号高电平。**
> - 空闲状态：红外LED不亮，接收头输出高电平
> - 发送低电平：红外LED以38KHz频率闪烁发光，接收头输出低电平
> - 发送高电平：红外LED不亮，接收头输出高电平



**15.1.2 NEC编码协议**
上面介绍了最底层的物理层，已经集成好所以不需要过多关心。对于要发送的数据来说，红外遥控器主要采用NEC编码。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/NEC%E7%BC%96%E7%A0%81%E7%A4%BA%E6%84%8F%E5%9B%BE.png" width=70%>
</div><div align=center>
图15-4 NEC编码示意图
</div>

可以看到上述一个完整的数据帧包括4个字节的数据（反码主要用于数据验证），并且由以下三个部分组成：
> 1. Start起始位：红色部分。
> 2. DATA数据部分：蓝色部分。**低位在前，高位在后。** 注意发送0/1的区别主要在于高电平持续时间，并且在这一部分已经发送完成所有的32位信息。最后一个下降沿发送完成后，持续560us拉高结束。
> 3. Repeat重复标志：绿色部分。如果按下按键不松手，就会重复发送该标志。最后的下降沿完成后，也是持续560us的低电平，然后拉高结束。

上述32位数据中，后发送的16位“命令”，指的就是遥控器的键码值，如下图：

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E7%BA%A2%E5%A4%96%E9%81%A5%E6%8E%A7%E9%94%AE%E7%A0%81.png" width=30%>
</div><div align=center>
图15-5 红外遥控键码
</div>

**15.1.3 外部中断**
由于红外遥控发送信号的速度很快（几十ms），如果采用单片机内部中断，很可能会遗漏信号，此时就需要 **外部中断** 实时的采集信号。回顾之前所学，STC89C52有4个外部中断，但是单片机只引出了**2个外部中断**（P3.2/P3.3）。这些外部中断有两种触发方式：下降沿触发和低电平触发。根据NEC编码协议，显然需要 **下降沿触发**。
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

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/STC89C52%E4%B8%8A%E7%9A%84%E5%A4%96%E9%83%A8%E4%B8%AD%E6%96%AD.png" width=30%>
</div><div align=center>
图15-5 STC89C52上的外部中断
</div>

## 15.2 实验：红外遥控基本应用
需求：使用红外遥控发射信号，并将接收到的结果显示在LCD液晶屏上：
> - LCD第一行显示“ADDR COMD NUM”。
> - LCD第二行依次显示红外遥控的地址码、键码、音量按键（VOL+/VOL-）计数变量num。对于这个num，按下VOL+加1，按下VOL-则减一，要是一直按着不松手，那就快速增加/减小。
> 
> 下面是初始化外部中断的一个小参考：

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E7%AE%80%E5%8C%96%E4%B8%AD%E6%96%AD%E7%B3%BB%E7%BB%9F%E7%BB%93%E6%9E%84%E5%9B%BE.png" width=60%>
</div><div align=center>
图15-6 简化的STC89C42中断系统结构图
</div>

整个程序使用了状态机。注意和FPGA中的单片机不太一样的是，所有的判断过程都是在外部中断（下降沿）来临时产生的，所以系统在执行判断语句的时候，目前的波形早就过去了。简单一句话，FPGA所有的信号等着你判断完才运行，所以要注意实时性；单片机中实际信号在你判断的时候已经过去了，有错位。并且状态机是在外部中断0的中断函数中完成的，这个中断函数放在```HS0038.c```文件中。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E7%BA%A2%E5%A4%96%E9%81%A5%E6%8E%A7%E7%8A%B6%E6%80%81%E6%9C%BA.png" width=50%>
</div><div align=center>
图15-7 红外遥控状态机
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8-%E7%BA%A2%E5%A4%96%E9%81%A5%E6%8E%A7.png" width=20%>
</div><div align=center>
图15-7 “红外遥控”代码调用
</div>

代码展示：
**- main.c**
```c
#include <REGX52.H>
#include "LCD1602.h"
#include "HS0038.h"

void main(){
  unsigned char IR_addr, IR_comd, num=0x00;
  //LCD初始化
  LCD1602_Init();
  LCD1602_DispString(1,1,"ADDR COMD VOL");
  LCD1602_DispString(2,1,"  00   00 000");
  //红外接收初始化
  HS0038_Init();
  P2_0 = 1;
  while(1){
    if(HS0038_GetReadFlag() || HS0038_GetRepeFlag()){
      IR_addr = HS0038_GetAddress();
      IR_comd = HS0038_GetCommand();
      //更新显示
      if     (IR_comd==HS_VOL_MINUS && num>0){num--;}
      else if(IR_comd==HS_VOL_ADD && num<255){num++;}      
      LCD1602_DispUnInt_Hex(2,3,IR_addr,2);
      LCD1602_DispUnInt_Hex(2,8,IR_comd,2);
      LCD1602_DispUnInt(2,11,num,3);
    }
  }
}
```

**HS0038.h**
```c
#ifndef __HS0038_H__
#define __HS0038_H__

#define HS_POWER      0x45
#define HS_MODE       0x46
#define HS_MUTE       0x47
#define HS_START_STOP 0x44
#define HS_PREVIOUS   0x40
#define HS_NEXT       0x43
#define HS_EQ         0x07
#define HS_VOL_MINUS  0x15
#define HS_VOL_ADD    0x09
#define HS_0          0x16
#define HS_RPT        0x19
#define HS_USD        0x0D
#define HS_1          0x0C
#define HS_2          0x18
#define HS_3          0x5E
#define HS_4          0x08
#define HS_5          0x1C
#define HS_6          0x5A
#define HS_7          0x42
#define HS_8          0x52
#define HS_9          0x4A

void HS0038_Init(void);//红外接收初始化
unsigned char HS0038_GetReadFlag(void);//是否已经接收完红外数据
unsigned char HS0038_GetRepeFlag(void);//是否接收到重复数据
unsigned char HS0038_GetAddress(void);//读取地址码
unsigned char HS0038_GetCommand(void);//读取命名码（键码）

#endif
```

**HS0038.c**
```c
#include <REGX52.H>
#include "Timer0.h"
#include "Int0.h"

static unsigned char state = 0;        //状态变量
static unsigned int  time_count = 0;   //存储定时器T0计数值
static unsigned char rdata[4];         //存储接收的数据
static unsigned char ReadFlag = 0;     //是否有新数据
static unsigned char RepeFlag = 0;     //是否重复发送信号

//红外接收初始化
void HS0038_Init(void){
  Timer0_CountInit();//计数器0初始化
  Int0_Init();  //外部中断0初始化
}

//是否已经接收完红外数据
unsigned char HS0038_GetReadFlag(void){
  if(ReadFlag){
    ReadFlag = 0;
    return 1;
  }
  else{
    return 0;
  }
}

//是否接收到重复数据
unsigned char HS0038_GetRepeFlag(void){
  if(RepeFlag){
    RepeFlag = 0;
    return 1;
  }
  else{
    return 0;
  }
}

//读取地址码
unsigned char HS0038_GetAddress(void){
  return rdata[0];
}

//读取命名码（键码）
unsigned char HS0038_GetCommand(void){
  return rdata[2];
}


//外部中断0中断后所需要执行的函数
void Int0_Routine(void) interrupt 0 {
  static unsigned char i=0;//循环变量
  //空闲状态
  if(state==0){
    Timer0_SetCounter(0x0000);
    Timer0_Run(1);//计时器T0开始工作
    state = 1;
  }
  //判断状态
  else if(state==1){
    time_count = Timer0_GetCounter();
    Timer0_SetCounter(0x0000);
    //起始位Start
    if(time_count>12442-500 && time_count<12442+500){
      i = 0;
      rdata[0] = 0x00;
      rdata[1] = 0x00;
      rdata[2] = 0x00;
      rdata[3] = 0x00;
      state = 2;
    }
    //重复位Repeat
    else if(time_count>10368-500 && time_count<10368+500){
      RepeFlag = 1;
      state = 0;
    }
    //其他未知状态
    else{state = 0;}
  }
  //接收数据的状态
  else if(state==2){
    time_count = Timer0_GetCounter();
    Timer0_SetCounter(0x0000);
    //接收到0
    if(time_count>1032-500 && time_count<1032+500){
      rdata[i/8] &= ~(0x01<<(i%8));
    }
    //接收到1
    else if(time_count>2074-500 && time_count<2074+500){
      rdata[i/8] |= (0x01<<(i%8));
    }
    //接收到非0非1
    else{
      i=0;
      state = 0;
    }
    
    //判断数据是否接收完成
    if(i>=31){
      i=0;
      //判断接收是否正确
      if((rdata[0]==~rdata[1]) && (rdata[2]==~rdata[3])){
        ReadFlag = 1;
      }else{
        ReadFlag = 0;
      }
      Timer0_Run(0);//计时器T0停止工作
      state = 0;
    }else{
      i++;
      state = state;
    }
  }
}
```


**Int0.h**
```c
#ifndef __INT0_H__
#define __INT0_H__

//外部中断0的初始化函数
void Int0_Init(void){
  IT0 = 1;//下降沿触发
  IE0 = 0;//默认没有触发中断请求
  EX0 = 1;//允许外部中断0中断
  EA = 1;//全局中断允许
  PX0 = 1;//提升外部中断0的优先级
}

/*中断函数模板
//外部中断0中断后所需要执行的函数
void Int0_Routine(void) interrupt 0 {
  
}
*/
#endif
```

**Timer0.h**
```c
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
```

**Timer0.c**
```c
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
```

编程感想：
> 1. 验证外部中断初始化的小技巧：由于外部中断的引脚与按键开关的引脚相同，所以通过检测按下按键的次数，就可以实现对外部中断是否完成初始化进行验证。
> 2. 一般不使用外部中断进行按键检测。因为有可能会收到按键抖动的影响，并且也无法做到上升沿检测。
> 3. 关于检测高电平持续时间：由于只用到了外部中断0的下降沿触发，所以实际上测量的是整个周期的时间长度。当然，定时器计数也存在误差，所以定时器判断可以设定为经典值左右500us。
> > 注：Start-13500us、Data0-1120us、Data1-2250us、Repeat-11250us。
> > 转换到11.0592MHz晶振，对应关系为Start-12442周期、Data0-1032周期、Data1-2074周期、Repeat-10368周期。
> 4. 关于移位：**Keil中移位16位以上就不再正确。** 所以单次发送的32位数据可以考虑用数组```unsigned char Data[4]```来进行存储。
> 5. 卡了很久的bug：**注意移位默认填进来的是0，左移不能疏忽！** 如果接收到0，应该写 ```rdata[i/8] &= ~(0x01<<(i%8));```，而不能写```rdata[i/8] &= (0xfe<<(i%8));```。
> 6. 关于灵敏度。有时候按着按着就不灵敏了，过一段时间又好了。这种现象非常奇怪，后来我觉得应该是这个红外遥控抗干扰能力太弱了。如果用手捂着遥控和接收器，会发现其实还挺灵敏的。


## 15.3 红外遥控电机调速
需求：在之前“直流电机调速”的基础上，使用数码管显示直流电机当前转速，并使用红外遥控的0、1、2、3、4控制5档转速。直流电机转速等级：Stop、40%、50%、75%、100%。
> - 数码管的显示可以为：00、40、50、75、100。
> - 注：综合考虑程序效率和硬件效果，由于要设置PWM周期为100个定时器中断，且PWM调速频率应在10kHz量级，于是尽量调低定时器中断周期，最后设置为100us。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8-%E7%BA%A2%E5%A4%96%E9%81%A5%E6%8E%A7%E7%9B%B4%E6%B5%81%E7%94%B5%E6%9C%BA.png" width=20%>
</div><div align=center>
图15-8 “红外遥控直流电机转速”代码调用
</div>

代码如下：
**- main.c**
```c
#include <REGX52.H>
#include "DC_Motor.h"
#include "HS0038.h"

void main(){
  unsigned char command=0;//红外接收的命令（键码）
  unsigned char speed_index=0;//转速等级索引
  unsigned char speed_motor[5] = {0,40,50,75,100};
  
  //电机初始化
  DC_Motor_Init();
  //红外接收初始化
  HS0038_Init();
  
  while(1){
    if(HS0038_GetReadFlag() || HS0038_GetRepeFlag()){
      command = HS0038_GetCommand();
      switch(command){
        case 0x16: speed_index=0; break;
        case HS_1: speed_index=1; break;
        case HS_2: speed_index=2; break;
        case HS_3: speed_index=3; break;
        case HS_4: speed_index=4; break;
        default:;
      }
      //调整PWM占空比（转速）
      DC_Motor_SetSpeed(speed_motor[speed_index]); 
    }
  }
}
```

**- DC_Motor.h**
```c
#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__

//电机初始化（主要是对PWM初始化）
void DC_Motor_Init(void);
//调节电机（PWM）占空比
void DC_Motor_SetSpeed(unsigned char speed);

#endif
```

**- DC_Motor.c**
```c
#include "Timer1.h"
#include "NixieTube.h"

#define motor P1_0
unsigned int PWM_Peri = 100;
unsigned int PWM_Duty = 0;

//电机初始化（主要是对PWM初始化）
void DC_Motor_Init(void){
  //初始化定时器
  Timer1_Init();
}

//调节电机（PWM）占空比
void DC_Motor_SetSpeed(unsigned char speed){
  unsigned char hunds,tens,ones;
  PWM_Duty = speed;
  ones = speed%10; speed = speed/10;
  tens = speed%10; speed = speed/10;
  hunds = speed%10;
  if(hunds==0){hunds = 10;}//如果百位是0，那数码管就不显示
  //数码管更新显示转速等级
  NixieTube_SetBuf(8,hunds);
  NixieTube_SetBuf(7,tens);
  NixieTube_SetBuf(6,ones);
}

/**********************************************/
// 定义定时器T1中断后要执行的动作
void Timer1_Routine() interrupt 3{
  static unsigned int count1_T1,count2_T1; //中断次数
  TH1 = 0xFF; TL1 = 0xA4; // 恢复溢出周期，近似100us
  //对于PWM所需的中断
  count1_T1++; //更新中断次数
  if(count1_T1>PWM_Peri){
    count1_T1 = 0;    
  }else{
    if(count1_T1<PWM_Duty){motor=1;}//电机运行
    else                  {motor=0;}//电机不运行
  }
  //数码管扫描所需的中断
  count2_T1++; //更新中断次数
  if(count2_T1>2){
    count2_T1 = 0;
    NixieTube_Loop();
  }
}
```

**- Timer1.h**
```c
#ifndef __TIMER1_H__
#define __TIMER1_H__

#include <REGX52.H>
/**
  * @brief :对定时器1进行初始化，使定时器1计数、中断。
  * 注：对11.0592MHz进行12分频（脉冲周期1.0850694us）。
 */
void Timer1_Init(){
  // 配置定时器T1的相关寄存器
  TMOD&=0x0f; TMOD|=0x10; // 选择T1的GATE=0/允许计数/模式1
  TF1 = 0; TR1 = 1; // 溢出标志位清空，运行控制位置1
  TH1 = 0xfc; TL1 = 0x66; // 离溢出近似1ms
  // 配置中断寄存器
  EA = 1; ET1 = 1; // 不屏蔽所有中断，允许T0溢出中断
  PT1 = 0; // T1优先级保持默认，不写这句话也可以
}

/*中断函数模板
// 定义定时器T0中断后要执行的动作
void Timer1_Routine() interrupt 3{
  static unsigned int count_T1; //中断次数
  count_T1++; //更新中断次数
  TH1 = 0xfc; TL1 = 0x66; // 恢复溢出周期，近似1ms
  if(count_T1>500){
    count_T1 = 0;
    
  }  
}
*/
#endif
```

**- NixieTube.h**
```c
#ifndef __NIXIETUBE_H__
#define __NIXIETUBE_H__

//改变数码管数据缓冲区的数值
void NixieTube_SetBuf(unsigned char index,unsigned char num);
//在指定数码管显示指定的数字
void NixieTube_Signle(unsigned char led, unsigned char num);
//定时器中断函数执行的数码管扫描函数
void NixieTube_Loop(void);

#endif
```

**- NixieTube.c**
```c
#include <REGX52.H>

//重新命名端口信号
#define Nixie_posi P2
#define Nixie_info P0

// 数据缓冲区，注意每个数字显示的范围就是0~9
unsigned char NixieTube_buf[8] = {10,10,10,10,10,10,10,10};

// 给出数字0~9的定义（符合数组的索引），最后两个依次表示不显示/显示横杠
unsigned char NixieTube_number[12] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x40};

/**
  * @brief :改变数码管数据缓冲区的数值。
* @param :index索引1~8，num数值0~11(10表示不显示，11表示横杠)
  * @retval :无
 */
void NixieTube_SetBuf(unsigned char index,unsigned char num){
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
  Nixie_info = 0x00; // 数码管不显示，消影
  Nixie_posi = sel_led[led]; // 选择数码管：LED1
  Nixie_info = NixieTube_number[num];  // 数码管显示
}

//定时器中断函数执行的数码管扫描函数，每次只扫描一个数码管
void NixieTube_Loop(void){
  static unsigned char i=0;
  NixieTube_Signle(i,NixieTube_buf[i]);
  i++;
  if(i>=8){i=0;}
}
```

**HS0038.h**、**HS0038.c**、**Int0.h**、**Timer0.h**、**Timer0.c** 均与上一个实验相同。

编程感想：
> 1. 卡了很久的bug：红外初始化时，使用了原来的定时器T0初始化函数（允许T0中断），而不是最新的初始化函数（只允许T0计数而不能中断）。导致按下红外之后，整个系统没什么现象。
> 2. 关于电力资源。在找上述bug的过程中，使用LCD显示屏查看中间变量，最后调试完成。然后拆下LCD1602硬件（此时还保留着代码中使用LCD显示变量的语句）运行程序，会发现每次按下按键的时候，数码管会整体闪烁出“噪点”；当把代码中关于LCD1602的一切删除干净后，所有系统都可以正常工作，数码管也不会出现噪点。结论就是：**LCD显示的过程会占用很多引脚资源/功率资源，导致数码管/红外接收等其他元器件工作不灵敏。**
