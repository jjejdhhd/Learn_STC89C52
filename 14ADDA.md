# 14 ADDA

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
注：工程及代码文件放在了本人的[Github仓库](https://github.com/jjejdhhd/Learn_STC89C52)。
***

## 14.1 AD/DA简介
**14.1.1 AD/DA基本介绍**
> AD（Analog to Digital）：模拟-数字转换，将模拟信号转换为计算机可操作的数字信号。
> DA（Digital to Analog）：数字-模拟转换，将计算机输出的数字信号转换为模拟信号。

AD/DA转换打开了计算机与模拟信号的大门，极大的提高了计算机系统的应用范围，也为模拟信号数字化处理提供了可能。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/202301281011727.png" width=70%>
</div><div align=center>
图14-1 常见的模拟器件
</div>


AD通常是对电压进行转换（注意电阻阻值应大致相同），并且AD转换通常有多个输入通道，用多路选择开关连接至AD转换器，以实现AD多路复用的目的，提高硬件利用率。AD/DA与单片机数据传送可使用并口（速度快、原理简单），也可使用串口（接线少、使用方便），看需求而定。另外也可以将AD/DA模块直接集成在单片机内，这样直接写入/读出寄存器就可进行AD/DA转换，单片机的IO口可直接复用为AD/DA的通道。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/ADDA%E7%9A%84%E7%A1%AC%E4%BB%B6%E7%94%B5%E8%B7%AF%E6%A8%A1%E5%9E%8B.png" width=70%>
</div><div align=center>
图14-2 AD/DA的硬件电路模型
</div>

下面是古老的（也非常经典的）AD转换芯片ADC0809、DA转换芯片DAC0832，由于芯片体积大且接口太多，目前已经逐渐淘汰。
<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/ADC0809%E5%8E%9F%E7%90%86%E6%A1%86%E5%9B%BE.png" width=70%>
</div><div align=center>
图14-3 ADC0809原理图
</div>

> - IN~7~\~IN~0~：8路模拟输入。
> - DB~7~\~DB~0~：输出的8位数字信号。
> - ADDA、ADDB、ADDC、ALE：地址锁存，选择当前的模拟输入引脚。
> - START：开始AD转换。
> - EOC：转换结束。
> - CLOCK：时钟线。
> - OE：输出使能，控制三态门输出。
> - U~REF(+)~、U~REF(-)：参考电压。
<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DAC0832%E5%8E%9F%E7%90%86%E6%A1%86%E5%9B%BE.png" width=70%>
</div><div align=center>
图14-4 DAC0832原理图
</div>

> - D0~D7：输入的8位数字信号。
> - ILE、CS、WR1、WR2、XFER：控制信号。
> - 输入寄存器与DAC寄存器：用于多路同步。若有两个DAC0832要求其同步输出，那么就可以先将数据放入寄存器，通过控制信号便可实现数据同步。
> - D/A转换器：输出模拟信号。



目前，较为常用的AD/DA芯片是PCF8591（IIC总线），芯片包含4路AD和1路DA。相比于之前的ADC0809、DAC0832，本芯片更加简单高效，外接电路也不需要额外的电阻电容。下面是其原理图及引脚简介：
<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/PCF5891%E5%8E%9F%E7%90%86%E5%9B%BE.png" width=35%>
</div><div align=center>
图14-5 PCF8591原理图
</div>

> - AIN0~AIN3：4路输入的模拟通道。
> - A0~A2：IIC器件地址。
> - SCL、SDA：IIC通信总线。
> - AOUT：数据输出通道。
> - ……



**14.1.2 运放电路**
运算放大器（简称“运放”）是具有很高放大倍数的放大电路单元。**内部集成了差分放大器、电压放大器、功率放大器三级放大电路**，是一个性能完备、功能强大的通用放大电路单元，由于其应用十分广泛，现已作为基本的电路元件出现在电路图中。运算放大器可构成的电路有：电压比较器、反相放大器、同相放大器、电压跟随器、加法器、积分器、微分器等。






<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E8%BF%90%E6%94%BE%E7%A4%BA%E6%84%8F%E5%9B%BE.png" width=55%>
</div><div align=center>
图14-6 运放示意图
</div>

1. 理想运算放大器具备以下性质：
> - 无限大的输入阻抗： 理想的运算放大器输入端不容许任何电流流入，即输入信号V+与V-两端点的电流信号恒为零，即输入阻抗无限大。
> - 趋近于零的输出阻抗： 理想运算放大器的输出端是一个完美的电压源，无论流至放大器负载的电流如何变化，放大器的输出电压恒为一定值，即输出阻抗为零。
> - 无限大的开回路增益： 理想运算放大器的开回路的状态下，输入端的差动信号有无限大的电压增益，这个特性使得运算放大器十分适合在实际应用时加上负反馈组态。
> - 无限大的共模抑制比： 理想运算放大器只能对V+与V-两端点电压的差值(差分信号)有反应，即只放大V~+~−V~−~ 的部份。对于两输入信号的相同的部分(共模信号)将完全忽略不计。
>   注：共模信号：双端输入时，两个信号相同。差模信号：双端输入时，两个信号的相位相差180度。

集成运算放大器有两种工作状态：
> - 线性状态：当给集成运算放大器加上负反馈电路时，工作在线性状态。工作特点：
> > 1. 具有虚断特性及流入和流出输入端的电流都为0，I~-~ = I~+~ = 0A。
> > 2. 具有虚短的特性及两个输入端的电压相等，U~+~ = U~-~。
> - 非线性状态：如果给集成运算放大器加正反馈电路或当其在开环工作时，工作在非线性状态。工作特点：
> > 1. 当同相输入端电压大于反向输入端电压时，输出电压为高电平。
> > 2. 当同相输入端电压小于反向输入端电压时，输出电压为低电平。

运算放大器电路的分析方法：虚短、虚断（负反馈条件下）：
> - 虚短： 集成运算放大器的开环放大倍数很大，一般通用型的运算放大器的开环电压放大倍数都在80dB以上，但是运放的输出电压是有限制的，一般 在10V～14V，然而**运放的差模输入电压不足1 mV，因此可以输入两端可以近似等电位**，就相当于短路。 开环电压放大倍数越大，两输入端的电位越接近相等，这种特性称之为虚短。
> - 虚断： 集成运算放大器具有输入高阻抗的特性，一般同向输入端和反向输入端的输入电阻都在1MΩ以上，所以**输入端流入运放的电流往往小于1uA，远小于输入端外电路的电流**。所以这里通常可把运放的两输入端视为开路，并且运放的输入电阻越大，同向和反向输入两端越接近开路。在运放处于线性状态时，根据这个特性可以把两输入端视为等效开路，简称虚断。


<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E8%BF%90%E6%94%BE%E7%9A%84%E5%AE%9E%E9%99%85%E5%BA%94%E7%94%A8%E4%B8%BE%E4%BE%8B.png" width=65%>
</div><div align=center>
图14-7 运放的电路应用举例
</div>

1. 电压比较器：开环工作，非线性状态。利用运放的放大倍数无穷大，来进行数字化。同相输入电压更大输出V~CC~；反之输出GND。
$$
VCC=
\begin{cases}
VCC, & V_{IN+} > V_{IN+}\\
GND, & V_{IN+} < V_{IN+}
\end{cases}
$$
2. 反相放大器。根据“虚短”中间的电势都为0，根据“虚断”流经R1和R2的电流大小及方向都相同。依次推算出输入电压与输出电压的关系。
$$
V_{OUT} = -\frac{R2}{R1}\ V_{IN}
$$
3. 同向放大器。分析思路同上。
$$
V_{OUT} = (1+\frac{R2}{R1})\ V_{IN}
$$
4. 电压跟随器。主要用于提高电路的驱动能力。放大倍数为1时，虽然没有电压放大特性，但是可以放大功率。
$$
V_{OUT} = V_{IN}
$$

**14.1.3 DA原理**
<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DAC0832-DA%E8%BD%AC%E6%8D%A2%E5%AF%84%E5%AD%98%E5%99%A8%E6%A1%86%E5%9B%BE.png" width=65%>
</div><div align=center>
图14-8 DA0832中的8位D/A转换器
</div>

运放工作在负反馈状态，所以 I~01~ 和 I~02~ 所在的两个线上的电压都相当于接在GND上，这样就不会影响上面电阻网络的分压分流等状态。根据电阻的串并联状态，I~7~ \~ I~0~ 的大小为公比为0.5的等比数列，即 I~7~ \~ I~0~ 为位权。注意总电流 $I = V_{REF} / R$，所以通过R~fb~的电流大小就是I的 [D7~D0]/256 倍，并且这个输出精度就是256不可变。即，输出电压 
$$
V_O = -\ \frac{D7 \sim D0}{256} \frac{V_{REF}\ R_{fb}}{R}
$$
当 $R_{fb} = R$ 时，
$$
V_O = -\ \frac{D7 \sim D0}{256} V_{REF}
$$

而开发板上搭载的是 **PWM型DA转换器**，下图14-9所示。注意这种电路结构相比于图14-8所示的电路结构来说，极大的节省端口且精度高，但缺点就是需要更多资源来产生PWM波，并且低通滤波器性能不好时平均电压还会自带纹波。
<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/PWM%E5%9E%8BDA%E8%BD%AC%E6%8D%A2%E5%99%A8.png" width=50%>
</div><div align=center>
图14-9 PWM型DA转换器示意图
</div>

> - R1、C1、R2、C2：组成二阶低通滤波器。可以输出PWM波的平均电压（随着占空比的不同而不同），但这个平均电压并没有驱动能力。
> - 电压跟随器。进行功率放大，增强输入的平均电压的负载能力。
> - 平均电压：$V_{avg} = Duty * V_{CC}$，其中$Duty$指的是PWM波的占空比，范围0~1；$V_{CC}$则是指PWM波的高电平电压。

**14.1.4 AD原理**
AD转换的常见架构主要有积分型、逐次逼近型、并行比较型/串并行型、Σ-Δ调制型、电容阵列逐次比较型、压频变换型等。但目前最常用的是 **逐次逼近型** 的AD转换器：
<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E9%80%90%E6%AC%A1%E9%80%BC%E8%BF%91%E5%9E%8BAD%E8%BD%AC%E6%8D%A2%E5%99%A8.png" width=45%>
</div><div align=center>
图14-10 逐次逼近型AD转换器
</div>

端口说明在图14-3下已经介绍过。整个AD转换器的核心部件就是 “比较器” 和 “DAC”，针对外部输入的模拟电压，“DAC” 通过不断逼近这个输入电压，直到8位数字量全部试完（如二分法），便可认为当前DAC的数字量认为是外部模拟输入的数字量。该DAC的精度也就是内部DAC的位宽。即，输出的数字量为：
$$
(D7 \sim D0) = \frac{V_{IN}}{V_{REF}}*256 \ ......\ 结果取整
$$

**AD/DA的性能指标**：
> - 分辨率：指AD/DA数字量的精细程度，通常用位数表示。例如，对于5V电源系统来说，8位的AD分辨率为5V/256 = 0.01953125V。
> - 转换速度：表示AD/DA的最大采样/建立频率，通常用转换频率或者转换时间来表示，对于采样/输出高速信号，应注意AD/DA的转换速度

**14.1.5 XPT2046通信时序**

**开发板上搭载的AD/DA都不是很主流。** 
- AD转换器使用了触摸屏芯片XPT2046，与上述AD转换原理稍有不同。XPT2046是一款4线制电阻式触摸屏控制器，内含 **12位分辨率125KHz转换速率** 逐步逼近型AD转换器。
> - 支持从 1.5V 到 5.25V 的低电压 I/O 接口。
> - 内部自带 2.5V 参考电压，可以作为辅助输入、温度测量和电池监测之用，电池监测的电压范围 0V ~ 6V。
> - 内部自带温度传感器，工作温度范围为-40℃～+85℃。
> - 采用 3 线制 **SPI通信接口**。
 
- DA转换器则是输入一个PWM波（P21），后面的电路对这个PWM波进行低通滤波，从而输出模拟信号给LED灯。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E5%BC%80%E5%8F%91%E6%9D%BF%E4%B8%8AADDA%E5%8E%9F%E7%90%86%E5%9B%BE.png" width=90%>
</div><div align=center>
图14-3 开发板上搭载的AD/DA原理图
</div>

> 1. AD转换器——ET2046：
> - X+、X-、Y+、Y-：控制像素点的位置。
> - AUX：ADC辅助输入通道。
> - VBAT：电池监视输入端。
> - VREF：参考电压输入/输出。
> - IOVDD：数字电源输入端。
> - PENIRQ：笔接触中断引脚。
> - DIN、DOUT、CS、DCLK、BUSY：**SPI通信相关引脚。** DIN、DOUT就是数据的输入输出；CS片选信号；DCLK时钟线。在多机SPI通信中，DIN、DOUT、DCLK共用总线，CS则单独分开，以此区分不同的从机。
> 2. ADC模块右侧
> - AIN0：滑动变阻分压。
> - AIN1：热敏电阻NTC分压。
> - AIN2：光敏电阻GR分压。
> 2. DA转换器
> - P21：输入PWM波。
> - R31、C32：组成低通滤波器，对PWM进行滤波输出其平均电压。
> - 后面的是电压跟随器，R28、C29、R30、R33可以去掉。另外可以看到，LED直接是正极供电，可见经过运放后正极驱动能力足够强。

下图给出SPI的读写时序。**DCLK上升沿输入数据，DCLK下降沿输出数据。** 由于总线独立，所以输入和输出可以同时进行，以实现主机和从机的寄存器交换，但一般是先发再收。SPI刚开始建立通信时，主机首先发送的是<u>状态控制字</u>，然后主机就可以连续读出2个字节（12位的数字信号）。
<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/SPI%E9%80%9A%E4%BF%A1%E6%97%B6%E5%BA%8F.png" width=80%>
</div><div align=center>
图14-4 SPI通信时序图
</div>


<div align=center>
表14-1 控制字节各位描述
</div>
<div align=center>
<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-cly1{text-align:left;vertical-align:middle}
.tg .tg-09is{border-color:#333333;color:#333333;text-align:center;vertical-align:middle}
.tg .tg-wa1i{font-weight:bold;text-align:center;vertical-align:middle}
.tg .tg-sjc5{border-color:#333333;color:#333333;font-weight:bold;text-align:center;vertical-align:middle}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-sjc5">位</th>
    <th class="tg-sjc5">名称</th>
    <th class="tg-wa1i">功能描述</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-09is">7</td>
    <td class="tg-09is">S</td>
    <td class="tg-cly1">开始位。为1表示一个新的控制字节到来，为0则忽略PIN引脚上数据。</td>
  </tr>
  <tr>
    <td class="tg-09is">6~4</td>
    <td class="tg-09is">A2~A0</td>
    <td class="tg-cly1">通道选择位。单端时，001/011为AIN0、101为AIN1、010为AIN2、110为AIN3。</td>
  </tr>
  <tr>
    <td class="tg-09is">3</td>
    <td class="tg-09is">MODE</td>
    <td class="tg-cly1">12位/8位转换分辨率选择位（0/1）。</td>
  </tr>
  <tr>
    <td class="tg-09is">2</td>
    <td class="tg-09is">SER/DFR</td>
    <td class="tg-cly1">单端输入方式/差分输入方式选择位（1/0）。差分性能好，但此开发板只支持单端。</td>
  </tr>
  <tr>
    <td class="tg-09is">1~0</td>
    <td class="tg-09is">PD1~PD0</td>
    <td class="tg-cly1">低功率模式选择位。若为11，器件总处于供电状态;若为00，器件在变换之间处于低功率模式。</td>
  </tr>
</tbody>
</table>
</div>

注意XPT内部有一个2.5V的参考电压，PD1=1时启用。但是为了满量程测量（5V），调用外部的参考电压（引脚VREF），所以PD1~PD0设置为00，此时满量程4095就代表5V。

## 14.2 实验：AD模数转换
需求：在LCD上显示三个模拟元件的数值：可调电阻、光敏电阻(光强越大阻值越小)、负温度系数热敏电阻NTC(温度越高阻值越小)。注意根据原理图，光敏电阻和热敏电阻的阻值减小会导致分压减小。
> 注：由于12位数最大值为4095，所以可以使用四位数字进行显示。

代码展示：只有主函数。
**-main.c**
```c
#include <REGX52.H>
#include "LCD1602.h"

//对端口进行重新命名
sbit SPI_CS   = P3^5;
sbit SPI_DCLK = P3^6;
sbit SPI_DIN  = P3^4;
sbit SPI_DOUT = P3^7;
#define AIN0_12AD 0x94 //1001 0100 电位器
#define AIN1_12AD 0xd4 //1101 0100 热敏电阻
#define AIN2_12AD 0xa4 //1010 0100 光敏电阻

//延时函数
void Delay1000ms(){//@11.0592MHz
	unsigned char i, j, k;
	i = 8; j = 1; k = 243;
	do{
		do{
			while (--k);
		}while (--j);
	}while (--i);
}

//XPT2046读取AD转换数值，SPI通信
unsigned int XPT2046_ReadAD(unsigned char Commd){
  unsigned int AD_value = 0x0000;
  unsigned char i=0;
  // 初始化
  SPI_CS = 1;
  SPI_DCLK = 0;
  SPI_DIN = 0;
  //发送控制字
  SPI_CS = 0;
  for(i=0;i<8;i++){
    SPI_DIN = Commd&(0x80>>i);
    SPI_DCLK = 1;
    SPI_DCLK = 0;
  }
  SPI_DIN = 0;
  //接收数据
  for(i=0;i<16;i++){
    SPI_DCLK = 1;
    SPI_DCLK = 0;
    if(SPI_DOUT){AD_value|=(0x8000>>i);}
  }
  SPI_CS = 1;
  return AD_value;
}

void main(){
  unsigned int AIN0=0x0000,AIN1=0x0000,AIN2=0x0000;
  //LCD1602初始化
  LCD1602_Init();
  LCD1602_DispString(1,1,"AD   NTC  GR  ");
  LCD1602_DispString(2,1,"0000 0000 0000");
  while(1){
    AIN0 = XPT2046_AD(AIN0_12AD)/16;
    AIN1 = XPT2046_AD(AIN1_12AD)/16;
    AIN2 = XPT2046_AD(AIN2_12AD)/16;
    LCD1602_DispUnInt(2,1,AIN0,4);
    LCD1602_DispUnInt(2,6,AIN1,4);
    LCD1602_DispUnInt(2,11,AIN2,4);
    Delay1000ms();
  }
}
```

## 14.3 实验：DA呼吸灯
需求：使用PWM型DA转换器实现呼吸灯的效果。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8-DA%E5%91%BC%E5%90%B8%E7%81%AF.png" width=20%>
</div><div align=center>
图14-5 “DA呼吸灯”代码调用关系
</div>

代码如下：
**- mian.c**
```c
#include <REGX52.H>
#include "Timer0.h"

sbit DA_pwm = P2^1;
unsigned int PWM_Peri = 100;
unsigned int PWM_Duty = 0;

//延时函数10ms
void Delay10ms(){//@11.0592MHz
	unsigned char i, j;
	i = 18; j = 235;
	do{
		while (--j);
	}while (--i);
}

void main(){
  unsigned char i;
  //初始化定时器
  Timer0_Init();
  while(1){
    for(i=0;i<PWM_Peri;i++){
      PWM_Duty = i;
      Delay10ms();
    }
    for(i=PWM_Peri;i>0;i--){
      PWM_Duty = i;
      Delay10ms();
    }
  }
}

// 定义定时器T0中断后要执行的动作
void Timer0_Routine() interrupt 1{
  static unsigned int count1_T0; //中断次数
  TH0 = 0xFF; TL0 = 0xA4; // 恢复溢出周期，近似100us
  //对于PWM所需的中断
  count1_T0++; //更新中断次数
  if(count1_T0>PWM_Peri){
    count1_T0 = 0;    
  }else{
    if(count1_T0<PWM_Duty){DA_pwm=1;}//电机运行
    else                  {DA_pwm=0;}//电机不运行
  }
}
```

**Timer0.h**与之前相同。

编程感想：
> 1. 根据实验现象可以发现，PWM型DA转换器的LED亮度不如LED2。这是因为开发板的电路设计中，运放及其周围电路构成了放大倍数为2倍的“同向放大器”，而不是放大倍数为1的“电压跟随器”。这导致只要2.5V便达到饱和。UP主这么解释的，但是我觉得这样反而会更亮？？所以导致其变暗的原因应该是在于运放没有接差分电源吧。
