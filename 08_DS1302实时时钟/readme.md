# 8 DS1302实时时钟

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
注：工程及代码文件放在了本人的[Github仓库](https://github.com/jjejdhhd/Learn_STC89C52)。
***

## 8.1 芯片介绍：DS1302
RTC(Real Time Clock)实时时钟，是一种集成电路，通常称为时钟芯片。常见的时钟芯片有DS1302，是由美国DALLAS公司推出的具有涓细电流充电能力的低功耗实时时钟芯片。它可以对年、月、日、周、时、分、秒进行计时，且具有闰年补偿等多种功能。**本节的关键在于DS1302的时序。**

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DS1302%E5%AE%9E%E7%89%A9%E5%9B%BE.png" width=80%>
</div><div align=center>
图8-1 DS1302实物图
</div>

那为什么不使用单片机上的定时器来驱动实时时钟呢？
> 1. CPU内的定时器精度不高。
> 2. 使用定时器会占用CPU的内部资源。
> 3. 单片机定时器掉电不能继续工作。
>


<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DS1302%E5%8E%9F%E7%90%86%E5%9B%BE.png" width=40%>
</div><div align=center>
图8-2 DS1302原理图
</div>

> 芯片引脚说明：
> CE：输入，芯片使能引脚。
> SCLK：输入，串行接口时钟。
> I/O：双向，输入输出的串行数据，SCLK上升沿触发。

基本逻辑是利用下面的寄存器地址说明（图8-4），写入的时候按需更改相应的秒数，读出的时候则依次进行读出。前八位是读写的选择和读写的地址，后八位才是具体要进行读写的数据，且**先传输低位**。前八位也被称为<u>命令字，最高位写保护，控制能否串行写入（1/0），但一直可以读；R/C选择是RAM数据/时钟数据（1/0）；A4~A0则为5位寄存器地址；R/W位则控制读写（1/0）。</u>下面则给出了DS1602的读/写传输时序、寄存器操作地址：
<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DS1602%E6%95%B0%E6%8D%AE%E4%BC%A0%E8%BE%93%E6%97%B6%E5%BA%8F.png" width=60%>
</div><div align=center>
图8-3 DS1302数据传输时序
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/DS1602%E5%AF%84%E5%AD%98%E5%99%A8%E5%9C%B0%E5%9D%80.png" width=70%>
</div><div align=center>
图8-4 RTC相关寄存器

注：左侧的八位是直接给出了**命令字**。
</div>

值得注意的是，上面寄存器所显示的数据，都采用BCD码（Binary Coded Decimal）进行编码：
> - 用4位二进制数来表示1位十进制数
>   例：0001 0011表示13，1000 0101表示85，0001 1010不合法
>   在十六进制中的体现：0x13表示13，0x85表示85，0x1A不合法
> - BCD码转十进制：DEC=BCD/16\*10+BCD%16; （2位BCD）
>   十进制转BCD码：BCD=DEC/10\*16+DEC%10; （2位BCD）
> - CH位：时停控制位，写入此位为高电平，则时钟暂停。
> - 12/24位：设置12小时/24小时模式位。12小时模式，BIT5表示AM/PM，BIT4表示十位；24小时模式，[BIT5,BIT4]表示十位。
> - 关于星期：注意这个星期不是实际的星期，而是“处理日”，所以需要用户指定初始的星期偏置。


## 8.2 实验：DS1302时钟显示
需求：在LCD显示屏第一行显示日期“年-月-日”，第二行显示时间“时-分-秒”，24小时制。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8%E5%85%B3%E7%B3%BB-%E5%AE%9E%E6%97%B6%E6%97%B6%E9%92%9F.png" width=20%>
</div><div align=center>
图8-5 “实时时钟”代码调用关系
</div>

代码展示：
**- main.c**
```c
#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"

unsigned char DS1302_info[7];

void main(){
  unsigned char i=0;
  // 初始化LCD显示屏
  LCD_Init();
  LCD_ShowString(1,1,"2023-01-01");
  LCD_ShowString(2,1,"00:00:00");
  DS1302_Init();//初始化DS1302
  while(1){
    DS1302_ReadAll(&DS1302_info);
    LCD_ShowNum(1,3,DS1302_info[0],2);
    LCD_ShowNum(1,6,DS1302_info[1],2);
    LCD_ShowNum(1,9,DS1302_info[2],2);
    LCD_ShowDay(1,14,DS1302_info[3]);
    LCD_ShowNum(2,1,DS1302_info[4],2);
    LCD_ShowNum(2,4,DS1302_info[5],2);
    LCD_ShowNum(2,7,DS1302_info[6],2);
  }
}
```

**- DS1302.h**
```c
#ifndef __DS1302_H__
#define __DS1302_H__

void DS1302_Init(void);//初始化DS1302
unsigned char DS1302_ReadByte(unsigned char read_comd);//从DS1302中读出数据
void DS1302_WriteByte(unsigned char wri_comd,wri_byte);//向DS1302中写入数据
void DS1302_ReadAll(unsigned char *info);//读出所有信息

#endif
```

**- DS1302.c**
```c
#include <REGX52.H>

// 将端口重新定义
sbit DS1302_SCLK = P3^6;
sbit DS1302_IO   = P3^4;
sbit DS1302_CE   = P3^5;
#define DS1302_RSEC 0x81
#define DS1302_RMIN 0x83
#define DS1302_RHOU 0x85
#define DS1302_RDAT 0x87
#define DS1302_RMON 0x89
#define DS1302_RDAY 0x8B
#define DS1302_RYEA 0x8D
#define DS1302_RWP  0x8F

#define DS1302_WSEC 0x80
#define DS1302_WMIN 0x82
#define DS1302_WHOU 0x84
#define DS1302_WDAT 0x86
#define DS1302_WMON 0x88
#define DS1302_WDAY 0x8A
#define DS1302_WYEA 0x8C
#define DS1302_WWP  0x8E


/**
  * @brief :从DS1302中读出数据
  * @param :需要操作的命令字read_comd。
  * @retval :读出的数据read_byte。
 */
unsigned char DS1302_ReadByte(unsigned char read_comd){
  unsigned char read_byte = 0x00;
  unsigned char i;
  DS1302_CE = 1;
  // 写入命令字
  for(i=0;i<8;i++){
    DS1302_IO = read_comd&(0x01<<i);
    DS1302_SCLK = 0;
    DS1302_SCLK = 1; // 经过测试，不需延时
  }
  // 读出数据
  for(i=0;i<8;i++){
    DS1302_SCLK = 1;
    DS1302_SCLK = 0;
    read_byte = DS1302_IO ? (read_byte|(0x01<<i)) : read_byte;
  }
  DS1302_IO = 0; //注意最后一定要将数据线清零
  DS1302_CE = 0;
  return read_byte;
 }
 
 /**
  * @brief :向DS1302中写入数据
  * @param :需要操作的命令字wri_comd，需要写入的数据wri_byte。
  * @retval :无。
 */
void DS1302_WriteByte(unsigned char wri_comd,wri_byte){
  unsigned char i;
  DS1302_CE = 1;
  // 写入命令字
  for(i=0;i<8;i++){
    DS1302_IO = wri_comd&(0x01<<i);
    DS1302_SCLK = 0;
    DS1302_SCLK = 1; // 经过测试，不需延时
  }
  // 写入8位数据
  for(i=0;i<8;i++){
    DS1302_IO = wri_byte&(0x01<<i);
    DS1302_SCLK = 0;
    DS1302_SCLK = 1; // 经过测试，不需延时
  }
  DS1302_SCLK = 0;
  DS1302_CE = 0;
 }

 /**
  * @brief :初始化控制DS1302的引脚。
  * @param :无。
  * @retval :无。
 */
void DS1302_Init(void){
  DS1302_SCLK = 0;
  DS1302_IO   = 0;
  DS1302_CE   = 0;
  DS1302_WriteByte(DS1302_WWP,0x00);//解除写保护
  DS1302_WriteByte(DS1302_WSEC,0x00);//解除时钟暂停
  // 默认为24小时模式
}

/**
  * @brief :从单片机中读出年/月/日/星期/时/分/秒
  * @param :数组指针，存储年/月/日/星期/时/分/秒。
  * @retval :无。程序中通过指针解引用更改信息。
 */
void DS1302_ReadAll(unsigned char *info){
  unsigned char temp;
  // 年
  temp = DS1302_ReadByte(DS1302_RYEA);
  *info = (temp>>4)*10 + (temp&0x0f);
  // 月
  temp = DS1302_ReadByte(DS1302_RMON);
  *(info+1) = (temp>>4)*10 + (temp&0x0f);
  // 日
  temp = DS1302_ReadByte(DS1302_RDAT);
  *(info+2) = (temp>>4)*10 + (temp&0x0f);
  // 星期
  *(info+3) = DS1302_ReadByte(DS1302_RDAY);
  // 时
  temp = DS1302_ReadByte(DS1302_RHOU);
  if(!(temp&0x80)){*(info+4) = (temp>>4)*10 + (temp&0x0f);}
  else            {*(info+4) = ((temp&0x10)>>4)*10 + (temp&0x0f);}
  // 分
  temp = DS1302_ReadByte(DS1302_RMIN);
  *(info+5) = ((temp&0x70)>>4)*10 + (temp&0x0f);
  // 秒
  temp = DS1302_ReadByte(DS1302_RSEC);
  *(info+6) = ((temp&0x70)>>4)*10 + (temp&0x0f);
}
```

**- LCD1602.h**：增加了星期显示函数“LCD1602_ShowDay”，可以方便的将数字转换成三位的英文缩写。
```c
#ifndef __LCD1602_H__
#define __LCD1602_H__

void LCD_Init();
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char);
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String);
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length);
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowDay(unsigned char Line,Column,day);//将数字星期还原成3位英文缩写，并显示

#endif
```

**- LCD1602.c**
```c
#include <REGX52.H>

//引脚配置：
sbit LCD_RS=P2^6;
sbit LCD_RW=P2^5;
sbit LCD_EN=P2^7;
#define LCD_DataPort P0

//函数定义：
/**
  * @brief  LCD1602延时函数，12MHz调用可延时1ms
  * @param  无
  * @retval 无
  */
void LCD_Delay()
{
	unsigned char i, j;

	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
}

/**
  * @brief  LCD1602写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void LCD_WriteCommand(unsigned char Command)
{
	LCD_RS=0;
	LCD_RW=0;
	LCD_DataPort=Command;
	LCD_EN=1;
	LCD_Delay();
	LCD_EN=0;
	LCD_Delay();
}

/**
  * @brief  LCD1602写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void LCD_WriteData(unsigned char Data)
{
	LCD_RS=1;
	LCD_RW=0;
	LCD_DataPort=Data;
	LCD_EN=1;
	LCD_Delay();
	LCD_EN=0;
	LCD_Delay();
}

/**
  * @brief  LCD1602设置光标位置
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @retval 无
  */
void LCD_SetCursor(unsigned char Line,unsigned char Column)
{
	if(Line==1)
	{
		LCD_WriteCommand(0x80|(Column-1));
	}
	else if(Line==2)
	{
		LCD_WriteCommand(0x80|(Column-1+0x40));
	}
}

/**
  * @brief  LCD1602初始化函数
  * @param  无
  * @retval 无
  */
void LCD_Init()
{
	LCD_WriteCommand(0x38);//八位数据接口，两行显示，5*7点阵
	LCD_WriteCommand(0x0c);//显示开，光标关，闪烁关
	LCD_WriteCommand(0x06);//数据读写操作后，光标自动加一，画面不动
	LCD_WriteCommand(0x01);//光标复位，清屏
}

/**
  * @brief  在LCD1602指定位置上显示一个字符
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的字符
  * @retval 无
  */
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
	LCD_SetCursor(Line,Column);
	LCD_WriteData(Char);
}

/**
  * @brief  在LCD1602指定位置开始显示所给字符串
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串
  * @retval 无
  */
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=0;String[i]!='\0';i++)
	{
		LCD_WriteData(String[i]);
	}
}

/**
  * @brief  返回值=X的Y次方
  */
int LCD_Pow(int X,int Y)
{
	unsigned char i;
	int Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}

/**
  * @brief  在LCD1602指定位置开始显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~65535
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(10,i-1)%10+'0');
	}
}

/**
  * @brief  在LCD1602指定位置开始以有符号十进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-32768~32767
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length)
{
	unsigned char i;
	unsigned int Number1;
	LCD_SetCursor(Line,Column);
	if(Number>=0)
	{
		LCD_WriteData('+');
		Number1=Number;
	}
	else
	{
		LCD_WriteData('-');
		Number1=-Number;
	}
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number1/LCD_Pow(10,i-1)%10+'0');
	}
}

/**
  * @brief  在LCD1602指定位置开始以十六进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFF
  * @param  Length 要显示数字的长度，范围：1~4
  * @retval 无
  */
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i,SingleNumber;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		SingleNumber=Number/LCD_Pow(16,i-1)%16;
		if(SingleNumber<10)
		{
			LCD_WriteData(SingleNumber+'0');
		}
		else
		{
			LCD_WriteData(SingleNumber-10+'A');
		}
	}
}

/**
  * @brief  在LCD1602指定位置开始以二进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(2,i-1)%2+'0');
	}
}

/**
  * @brief  在LCD1602指定位置开始显示星期（英文）
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的星期，范围1~7
  * @retval 无
  */
void LCD_ShowDay(unsigned char Line,Column,day){
	unsigned char i;
  char abbr_day[7][3] = {"Sta","Sun","Mon","Tue","Wen","Thu","Fri"};
	LCD_SetCursor(Line,Column);
	for(i=0;abbr_day[day-1][i]!='\0';i++)
	{
		LCD_WriteData(abbr_day[day-1][i]);
	}
}
```

编程感想：
> 1. 注意 **单片机复位后引脚默认为1**，所以DS1302的控制引脚都需要初始化。
> 2. 若读出的数字不动，则有可能是芯片处于写保护状态；若读出的数字乱跳，需要注意读状态结束后，需要手动将IO线归零。

## 8.3 实验：可调时钟
需求：在上面8.2实验的基础上，增加可调整日期和时间的功能。
> 1. 按键K1用于切换显示模式/调节模式。
> 2. 按键K2用于切换所需要调节的位（年/月/日/时/分/秒），切换到哪个哪个就闪烁。
> 3. 按键K3增加计数，按键K4减少计数，秒只能归到0或30秒。这个地方注意不能违背闰月28天等常识。

上一节实验没有考虑星期显示的问题，本节首先改进。进过反复的测试，发现表示“星期”的字节有这样奇怪的属性：每次芯片掉电重启后自动归1；但如果不掉电，则会在23:59:59后随时间一起加1，但是**芯片本身不会根据日期自动生成“星期”**。所以每次对日期进行重写后，也要对“星期”进行重写使其变成1，于是芯片给出的“星期”的物理意义就变成了 **“从最近一次设定时间到现在经历的天数（星期表示）”**，即“处理日”。所以，还需要再添加一个存储“星期偏置”的常量，将其与“处理日”相结合便可以得到正确的星期。

另外本节实验，使用软件演示来完成按键检测；使用定时器延时来完成修改时间参数时的闪烁状态。

<div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8-%E5%8F%AF%E8%B0%83%E6%97%B6%E9%92%9F.png" width=20%>
</div><div align=center>
图8-6 “可调时钟”代码调用关系
</div>

代码展示：
**- mian.c**
```c
#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "PushKey.h"
#include "Timer0.h"

// 全局变量：存储年/月/日/星期/时/分/秒/星期偏置
unsigned char DS1302_info[8] = {23,1,22,0,23,59,55,7};
// 全局变量：闪烁标志信号
unsigned char flag_twinkle=0x00;

/**
  * @brief :RTC显示。
  * @param :数组指针，存储年/月/日/星期/时/分/秒/星期偏置。
  * @retval :无。
 */
void RTC_Display(unsigned char *info){
  LCD_ShowNum(1,3,info[0],2);
  LCD_ShowNum(1,6,info[1],2);
  LCD_ShowNum(1,9,info[2],2);
  LCD_ShowDay(1,14,info[3]);
  LCD_ShowNum(2,1,info[4],2);
  LCD_ShowNum(2,4,info[5],2);
  LCD_ShowNum(2,7,info[6],2);
}

/**
  * @brief :进入修改模式，用户输入日期数据，保证其合理性并更新。
  * @param :数组指针，存储年/月/日/星期/时/分/秒/星期偏置。
  * @retval :无。
 */
void RTC_Modify(unsigned char *info){
  unsigned char MODE = 1;//指示当前是否处于修改模式
  unsigned char key = 0;//按键开关
  unsigned char cursor = 1;//指示要修改第几个元素，范围1~7（4实际更改info的第8个元素）
  while(MODE==1){
    key = PushKey();
    if(key==1){
      // 退出修改模式
      MODE = 0;
      DS1302_SetAll(info);//更新数据
    }else if(key==2){
      //移动修改光标
      cursor = (cursor>=7) ? 1 : (cursor+1);
//      LCD_ShowNum(2,14,cursor,3);//仅供测试使用！！！
    }else if(key==3){
      //对当前光标指定值进行加计数
      switch(cursor){
        case 1: //年
          *info = (*info>=99) ? 0 : (*info)+1;
          break;
        
        case 2: //月
          *(info+1) = (*(info+1)>=12) ? 1 : (*(info+1))+1;
          break;
        
        case 3: //日
          //31天的月份
          if(*(info+1)==1 || *(info+1)==3 || *(info+1)==5 || *(info+1)==7 ||
             *(info+1)==8 || *(info+1)==10|| *(info+1)==12){
             *(info+2)=(*(info+2)>=31)?1:(*(info+2))+1;
          }//30天的月份
          else if(*(info+1)==4 || *(info+1)==6 || *(info+1)==9 || *(info+1)==11){
            *(info+2)=(*(info+2)>=30)?1:(*(info+2))+1;
          }//2月需要判断闰月
          else if(*(info+1)==2){
            if(*info%4==0){*(info+2)=(*(info+2)>=29)?1:(*(info+2))+1;}
            else          {*(info+2)=(*(info+2)>=28)?1:(*(info+2))+1;}
          }
          break;
          
        case 4://星期
          *(info+7) = (*(info+7)>=7) ? 1 : (*(info+7))+1;//更新偏置
          *(info+3) = (*(info+3)>=7) ? 1 : (*(info+3))+1;//更新显示
          break;
        
        case 5://时
          *(info+4) = (*(info+4)>=23) ? 0 : (*(info+4))+1;
          break;
        
        case 6://分
          *(info+5) = (*(info+5)>=59) ? 0 : (*(info+5))+1;
          break;
        
        case 7://秒
          *(info+6) = (*(info+6)>=59) ? 0 : (*(info+6))+1;
          break;
        
        default:;
      }
    }else if(key==4){
      //对当前光标指定值进行减计数
      switch(cursor){
        case 1: //年
          *info = (*info==0) ? 99 : (*info)-1;
          break;
        
        case 2: //月
          *(info+1) = (*(info+1)==1) ? 12 : (*(info+1))-1;
          break;
        
        case 3: //日
          //31天的月份
          if(*(info+1)==1 || *(info+1)==3 || *(info+1)==5 || *(info+1)==7 ||
             *(info+1)==8 || *(info+1)==10|| *(info+1)==12){
             *(info+2)=(*(info+2)==1)?31:(*(info+2))-1;
          }//30天的月份
          else if(*(info+1)==4 || *(info+1)==6 || *(info+1)==9 || *(info+1)==11){
            *(info+2)=(*(info+2)==1)?30:(*(info+2))-1;
          }//2月需要判断闰月
          else if(*(info+1)==2){
            if(*info%4==0){*(info+2)=(*(info+2)==1)?29:(*(info+2))-1;}
            else          {*(info+2)=(*(info+2)==1)?28:(*(info+2))-1;}
          }
          break;
          
        case 4://星期
          *(info+7) = (*(info+7)==1) ? 7 : (*(info+7))-1;//更新偏置（实际起作用）
          *(info+3) = (*(info+3)==1) ? 7 : (*(info+3))-1;//更新显示
          break;
        
        case 5://时
          *(info+4) = (*(info+4)==0) ? 23 : (*(info+4))-1;
          break;
        
        case 6://分
          *(info+5) = (*(info+5)==0) ? 59 : (*(info+5))-1;
          break;
        
        case 7://秒
          *(info+6) = (*(info+6)==0) ? 59 : (*(info+6))-1;
          break;
        
        default:;    
      }
    }
    //更新当前的显示
    if(!flag_twinkle){
      LCD_ShowNum(1,3,*info,2);
      LCD_ShowNum(1,6,*(info+1),2);
      LCD_ShowNum(1,9,*(info+2),2);
      LCD_ShowDay(1,14,*(info+3));
      LCD_ShowNum(2,1,*(info+4),2);
      LCD_ShowNum(2,4,*(info+5),2);
      LCD_ShowNum(2,7,*(info+6),2);
    }else{
      switch(cursor){
        case 1: LCD_ShowString(1,3,"  "); break;
        case 2: LCD_ShowString(1,6,"  "); break;
        case 3: LCD_ShowString(1,9,"  "); break;
        case 4: LCD_ShowString(1,14,"   "); break;
        case 5: LCD_ShowString(2,1,"  "); break;
        case 6: LCD_ShowString(2,4,"  "); break;
        case 7: LCD_ShowString(2,7,"  "); break;
        default:;
      }
    }
  }
}


void main(){
//  unsigned char i=0;
  unsigned char key = 0; //指示按下了哪个按键开关
  //初始化LCD显示屏
  LCD_Init();
  LCD_ShowString(1,1,"2023-01-22");
  LCD_ShowString(2,1,"00:00:00");
  //初始化DS1302
  DS1302_Init();
  DS1302_SetAll(&DS1302_info);
  //初始化定时器T0
  Timer0_Init();
  while(1){
    key = PushKey();
    if(key==1){
    //进入修改模式
      RTC_Modify(&DS1302_info);
    }
    else{
    //正常读取并显示时钟
      DS1302_ReadAll(&DS1302_info);
      RTC_Display(&DS1302_info);
//      LCD_ShowNum(2,11,*(DS1302_info+7),3);//仅供测试使用！！！
    }
  }
}

// 定义定时器T0中断后要执行的动作
void Timer0_Routine() interrupt 1{
  static unsigned int count_T0; //中断次数
  count_T0++; //更新中断次数
  TH0 = 0xfc; TL0 = 0x66; // 恢复溢出周期，近似1ms
  if(count_T0>200){
    count_T0 = 0;
    flag_twinkle = !flag_twinkle;
  }  
}
```

**- DS1302.h**
```c
#ifndef __DS1302_H__
#define __DS1302_H__

void DS1302_Init(void);//初始化DS1302
unsigned char DS1302_ReadByte(unsigned char read_comd);//从DS1302中读出数据
void DS1302_WriteByte(unsigned char wri_comd,wri_byte);//向DS1302中写入数据
void DS1302_ReadAll(unsigned char *info);//读出所有信息
void DS1302_SetAll(unsigned char *info);//写入所有信息

#endif
```

**- DS1302.c**
```c
#include <REGX52.H>

// 将端口重新定义
sbit DS1302_SCLK = P3^6;
sbit DS1302_IO   = P3^4;
sbit DS1302_CE   = P3^5;
#define DS1302_RSEC 0x81
#define DS1302_RMIN 0x83
#define DS1302_RHOU 0x85
#define DS1302_RDAT 0x87
#define DS1302_RMON 0x89
#define DS1302_RDAY 0x8B
#define DS1302_RYEA 0x8D
#define DS1302_RWP  0x8F

#define DS1302_WSEC 0x80
#define DS1302_WMIN 0x82
#define DS1302_WHOU 0x84
#define DS1302_WDAT 0x86
#define DS1302_WMON 0x88
#define DS1302_WDAY 0x8A
#define DS1302_WYEA 0x8C
#define DS1302_WWP  0x8E

/**
  * @brief :从DS1302中读出数据
  * @param :需要操作的命令字read_comd。
  * @retval :读出的数据read_byte。
 */
unsigned char DS1302_ReadByte(unsigned char read_comd){
  unsigned char read_byte = 0x00;
  unsigned char i;
  DS1302_CE = 1;
  // 写入命令字
  for(i=0;i<8;i++){
    DS1302_IO = read_comd&(0x01<<i);
    DS1302_SCLK = 0;
    DS1302_SCLK = 1; // 经过测试，不需延时
  }
  // 读出数据
  for(i=0;i<8;i++){
    DS1302_SCLK = 1;
    DS1302_SCLK = 0;
    read_byte = DS1302_IO ? (read_byte|(0x01<<i)) : read_byte;
  }
  DS1302_IO = 0; //注意最后一定要将数据线清零
  DS1302_CE = 0;
  return read_byte;
 }
 
 /**
  * @brief :向DS1302中写入数据
  * @param :需要操作的命令字wri_comd，需要写入的数据wri_byte。
  * @retval :无。
 */
void DS1302_WriteByte(unsigned char wri_comd,wri_byte){
  unsigned char i;
  DS1302_CE = 1;
  // 写入命令字
  for(i=0;i<8;i++){
    DS1302_IO = wri_comd&(0x01<<i);
    DS1302_SCLK = 0;
    DS1302_SCLK = 1; // 经过测试，不需延时
  }
  // 写入8位数据
  for(i=0;i<8;i++){
    DS1302_IO = wri_byte&(0x01<<i);
    DS1302_SCLK = 0;
    DS1302_SCLK = 1; // 经过测试，不需延时
  }
  DS1302_SCLK = 0;
  DS1302_CE = 0;
 }

/**
  * @brief :从单片机中读出年/月/日/星期/时/分/秒
  * @param :数组指针，存储年/月/日/星期/时/分/秒/星期偏置。
  * @retval :无。程序中通过指针解引用更改信息。
 */
void DS1302_ReadAll(unsigned char *info){
  unsigned char temp;
  // 年
  temp = DS1302_ReadByte(DS1302_RYEA);
  *info = (temp>>4)*10 + (temp&0x0f);
  // 月
  temp = DS1302_ReadByte(DS1302_RMON);
  *(info+1) = (temp>>4)*10 + (temp&0x0f);
  // 日
  temp = DS1302_ReadByte(DS1302_RDAT);
  *(info+2) = (temp>>4)*10 + (temp&0x0f);
  // 星期
  temp = DS1302_ReadByte(DS1302_RDAY);
//  *(info+3) = temp;
  temp = *(info+7) + (temp-1);
  *(info+3) = (temp>7) ? (temp-7) : temp;
  // 时
  temp = DS1302_ReadByte(DS1302_RHOU);
  if(!(temp&0x80)){*(info+4) = (temp>>4)*10 + (temp&0x0f);}
  else            {*(info+4) = ((temp&0x10)>>4)*10 + (temp&0x0f);}
  // 分
  temp = DS1302_ReadByte(DS1302_RMIN);
  *(info+5) = ((temp&0x70)>>4)*10 + (temp&0x0f);
  // 秒
  temp = DS1302_ReadByte(DS1302_RSEC);
  *(info+6) = ((temp&0x70)>>4)*10 + (temp&0x0f);
}

/**
  * @brief :向单片机写入年/月/日/时/分/秒
  * @param :数组指针，存储年/月/日/星期/时/分/秒/星期偏置。
  * @retval :无。
 */
void DS1302_SetAll(unsigned char *info){
  unsigned char temp;
  // 年
  temp = (*info)/10*16 + (*info)%10;
  DS1302_WriteByte(DS1302_WYEA,temp);
  // 月
  temp = (*(info+1))/10*16 + (*(info+1))%10;
  DS1302_WriteByte(DS1302_WMON,temp);
  // 日
  temp = (*(info+2))/10*16 + (*(info+2))%10;
  DS1302_WriteByte(DS1302_WDAT,temp);
  // 星期，每次更改则默认归1
  DS1302_WriteByte(DS1302_WDAY,0x01);
  // 时，24小时模式
  temp = (*(info+4))/10*16 + (*(info+4))%10;
  DS1302_WriteByte(DS1302_WHOU,temp);
  // 分
  temp = (*(info+5))/10*16 + (*(info+5))%10;
  DS1302_WriteByte(DS1302_WMIN,temp);
  // 秒
  temp = (*(info+6))/10*16 + (*(info+6))%10;
  DS1302_WriteByte(DS1302_WSEC,temp);
}

 /**
  * @brief :初始化控制DS1302的引脚。
  * @param :无。
  * @retval :无。
 */
void DS1302_Init(void){
  unsigned char init_info[8] = {0,1,1,0,0,0,0,6};
  DS1302_SCLK = 0;
  DS1302_IO   = 0;
  DS1302_CE   = 0;
  DS1302_WriteByte(DS1302_WWP,0x00);//解除写保护
  DS1302_WriteByte(DS1302_WSEC,0x00);//解除时钟暂停
  // 默认为24小时模式
  //默认复位成2000年1月1日星期六0点0时0分（24小说模式）
  DS1302_SetAll(&init_info);
}
```

**- LCD1602.h**
```c
#ifndef __LCD1602_H__
#define __LCD1602_H__

void LCD_Init();
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char);
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String);
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length);
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowDay(unsigned char Line,Column,day);//将数字星期还原成3位英文缩写，并显示

#endif
```

**- LCD1602.c**
```c
#include <REGX52.H>

//引脚配置：
sbit LCD_RS=P2^6;
sbit LCD_RW=P2^5;
sbit LCD_EN=P2^7;
#define LCD_DataPort P0

//函数定义：
/**
  * @brief  LCD1602延时函数，12MHz调用可延时1ms
  * @param  无
  * @retval 无
  */
void LCD_Delay()
{
	unsigned char i, j;

	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
}

/**
  * @brief  LCD1602写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void LCD_WriteCommand(unsigned char Command)
{
	LCD_RS=0;
	LCD_RW=0;
	LCD_DataPort=Command;
	LCD_EN=1;
	LCD_Delay();
	LCD_EN=0;
	LCD_Delay();
}

/**
  * @brief  LCD1602写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void LCD_WriteData(unsigned char Data)
{
	LCD_RS=1;
	LCD_RW=0;
	LCD_DataPort=Data;
	LCD_EN=1;
	LCD_Delay();
	LCD_EN=0;
	LCD_Delay();
}

/**
  * @brief  LCD1602设置光标位置
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @retval 无
  */
void LCD_SetCursor(unsigned char Line,unsigned char Column)
{
	if(Line==1)
	{
		LCD_WriteCommand(0x80|(Column-1));
	}
	else if(Line==2)
	{
		LCD_WriteCommand(0x80|(Column-1+0x40));
	}
}

/**
  * @brief  LCD1602初始化函数
  * @param  无
  * @retval 无
  */
void LCD_Init()
{
	LCD_WriteCommand(0x38);//八位数据接口，两行显示，5*7点阵
	LCD_WriteCommand(0x0c);//显示开，光标关，闪烁关
	LCD_WriteCommand(0x06);//数据读写操作后，光标自动加一，画面不动
	LCD_WriteCommand(0x01);//光标复位，清屏
}

/**
  * @brief  在LCD1602指定位置上显示一个字符
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的字符
  * @retval 无
  */
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
	LCD_SetCursor(Line,Column);
	LCD_WriteData(Char);
}

/**
  * @brief  在LCD1602指定位置开始显示所给字符串
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串
  * @retval 无
  */
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=0;String[i]!='\0';i++)
	{
		LCD_WriteData(String[i]);
	}
}

/**
  * @brief  返回值=X的Y次方
  */
int LCD_Pow(int X,int Y)
{
	unsigned char i;
	int Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}

/**
  * @brief  在LCD1602指定位置开始显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~65535
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(10,i-1)%10+'0');
	}
}

/**
  * @brief  在LCD1602指定位置开始以有符号十进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-32768~32767
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length)
{
	unsigned char i;
	unsigned int Number1;
	LCD_SetCursor(Line,Column);
	if(Number>=0)
	{
		LCD_WriteData('+');
		Number1=Number;
	}
	else
	{
		LCD_WriteData('-');
		Number1=-Number;
	}
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number1/LCD_Pow(10,i-1)%10+'0');
	}
}

/**
  * @brief  在LCD1602指定位置开始以十六进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFF
  * @param  Length 要显示数字的长度，范围：1~4
  * @retval 无
  */
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i,SingleNumber;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		SingleNumber=Number/LCD_Pow(16,i-1)%16;
		if(SingleNumber<10)
		{
			LCD_WriteData(SingleNumber+'0');
		}
		else
		{
			LCD_WriteData(SingleNumber-10+'A');
		}
	}
}

/**
  * @brief  在LCD1602指定位置开始以二进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(2,i-1)%2+'0');
	}
}

/**
  * @brief  在LCD1602指定位置开始显示星期（英文）
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的星期，范围1~7
  * @retval 无
  */
void LCD_ShowDay(unsigned char Line,Column,day){
	unsigned char i;
  char abbr_day[7][3] = {"Mon","Tue","Wen","Thu","Fri","Sta","Sun"};
	LCD_SetCursor(Line,Column);
	for(i=0;abbr_day[day-1][i]!='\0';i++)
	{
		LCD_WriteData(abbr_day[day-1][i]);
	}
}
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
  else if(!P3_0){Delay(10);while(!P3_0);Delay(10);key=2;}
  else if(!P3_2){Delay(10);while(!P3_2);Delay(10);key=3;}
  else if(!P3_3){Delay(10);while(!P3_3);Delay(10);key=4;}
  return key;
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

**- Delay.h**
```c
#ifndef __DEALY_H_
#define __DEALY_H_

// 延时cycles ms，晶振@11.0592MHz
void Delay(unsigned char cycles){
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


编程感想：
> 1. 天坑：每次复位后，“星期”的显示都归成1，意思为“处理日”，不是实际的星期。
> 2. 缺陷：由于按键检测使用 ```while``` 循环等待按键松开，所以按键不松手的时候时钟显示会“暂停”（但是不影响DS1302继续计数），只有松手后，画面才会继续显示。后续会使用定时器来检测按键的上升沿，可以改善这个问题。
> 3. 已解决错误：实测的时候，上电后第一次调整“星期”经常会出现自动加1，后需调整则不会出现这样的错误。
> > - 后续“蜂鸣器播放提示音”实验发现，原来是开关按键3过于灵敏，有时会自动检测到“按下”。TMD，还以为是我程序有问题，苦思冥想半天没结果，当时好伤心。
