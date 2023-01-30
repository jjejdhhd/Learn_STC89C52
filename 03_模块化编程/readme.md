# 3 模块化编程

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
注：工程及代码文件放在了本人的[Github仓库](https://github.com/jjejdhhd/Learn_STC89C52)。
***

## 3.1 模块化编程
传统方式编程：所有的函数均放在main.c里，若使用的模块比较多，则一个文件内会有很多的代码，不利于代码的组织和管理，而且很影响编程者的思路。
 **模块化编程：** 把各个模块的代码放在不同的.c文件里，在.h文件里提供外部可调用函数的声明，其它.c文件想使用其中的代码时，只需要#include "XXX.h"文件即可。使用模块化编程可极大的提高代码的可阅读性、可维护性、可移植性等。
 
 比如下面这张图，将```Dealy```函数进行了头文件声明、源文件定义，便可以在其他的源文件中调用了。

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E6%A8%A1%E5%9D%97%E5%8C%96%E7%BC%96%E7%A8%8B.png" width=80%>
</div><div align=center>
图3-1 模块化编程示意图
</div>

注意事项：
> - .c文件：函数、变量的定义。
> - .h文件：可被外部调用的函数、变量的声明。
> - 任何自定义的变量、函数，在某个.c文件中被调用前必须有定义或声明。
> - 使用到的自定义函数的.c文件必须添加到工程参与编译。
> - 使用到的.h文件必须要放在编译器可寻找到的地方（工程文件夹根目录、安装目录、自定义）。




<div align=center>
表3-1 C语言预编译举例
</div><div align=center>
<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-amwm{font-weight:bold;text-align:center;vertical-align:top}
.tg .tg-0lax{text-align:left;vertical-align:top}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-amwm">预编译</th>
    <th class="tg-amwm">意义</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-0lax">#include &lt;REGX52.H&gt;</td>
    <td class="tg-0lax">把REGX52.H文件的内容搬到此处。<br>< >一般是用在系统自带，“ ”一般是用在自己写的。</td>
  </tr>
  <tr>
    <td class="tg-0lax">#define PI 3.14</td>
    <td class="tg-0lax">定义PI，将PI替换为3.14</td>
  </tr>
  <tr>
    <td class="tg-0lax">#define ABC</td>
    <td class="tg-0lax">定义ABC</td>
  </tr>
  <tr>
    <td class="tg-0lax">#ifndef __XX_H__	</td>
    <td class="tg-0lax">如果没有定义__XX_H__</td>
  </tr>
  <tr>
    <td class="tg-0lax">#endif</td>
    <td class="tg-0lax">与#ifndef,#if匹配，组成“括号”</td>
  </tr>
</tbody>
</table>
</div>

C语言的预编译以```#```开头，作用是在真正的编译开始之前，对代码做一些处理（预编译），如上表所示。此外还有```#ifdef,#if,#else,#elif,#undef```等。这些预编译指令最重要的作用就是**避免头文件被多次包含而失效**（保证头文件只被包含一次）。而最常用的预编译指令结构如下（如上图3-1演示的那样）：
>#ifndef MY_HEADER_FILE **\_H\_**
>#define MY_HEADER_FILE **\_H\_**
>&ensp;&ensp;&ensp;&ensp;//头文件内容
>#endif

## 3.2 LCD1602调试工具
**3.2.1 LCD1602液晶屏介绍**
1602液晶也叫1602字符型液晶，能显示 **16个字符*2行** 的信息。它是由若干个 5x7 或者 5x10 的点阵字符位组成，每个点阵字符位都可以用显示一个字符，每位之间有一个点距的间隔，每行之间也有间隔，起到了字符间距和行间距的作用。正因如此，它不能很好的显示图片，而专门用来显示字母、数字、符号。

使用LCD1602液晶屏作为调试窗口，提供类似```printf```函数的功能，可实时观察单片机内部数据的变换情况，便于调试和演示。本视频提供的LCD1602代码属于模块化的代码，使用者只需要知道所提供函数的作用和使用方法就可以很容易的使用LCD1602。
注：当然也可以通过数码管来进行显示，但是数码管需要不断扫描，占用CPU资源较多所以不推荐。另外也可以通过串口通信的方式进行调试，但是其原理更加复杂且需要外接电脑不是很方便，所以也不推荐。

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/LCD1602%E6%8E%A5%E5%8F%A3%E5%8E%9F%E7%90%86%E5%9B%BE.png" width=30%>
</div><div align=center>
图3-2 LCD1602接口原理图
</div>

> - LCD RS：数据/命令选择端（H/L）。
> - LCD WR：写读选择端（L/H）。
> - LCD EN：使能信号。
> - 在第12部分会详细的介绍其工作原理及相应的代码编写。

<div align=center>
表3-2 UP主自己编写的LCD相关函数
</div><div align=center>
<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-amwm{font-weight:bold;text-align:center;vertical-align:top}
.tg .tg-0lax{text-align:left;vertical-align:top}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-amwm"><span style="font-weight:bold">函数</span></th>
    <th class="tg-amwm"><span style="font-weight:bold">作用</span></th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-0lax">LCD_Init();</td>
    <td class="tg-0lax">初始化</td>
  </tr>
  <tr>
    <td class="tg-0lax">LCD_ShowChar(1,1,'A');</td>
    <td class="tg-0lax">在指定位置显示一个字符</td>
  </tr>
  <tr>
    <td class="tg-0lax">LCD_ShowString(1,3,"Hello");</td>
    <td class="tg-0lax">在指定位置显示字符串</td>
  </tr>
  <tr>
    <td class="tg-0lax">LCD_ShowNum(1,9,123,3);</td>
    <td class="tg-0lax">在指定位置显示十进制数字</td>
  </tr>
  <tr>
    <td class="tg-0lax">LCD_ShowSignedNum(1,13,-66,3);</td>
    <td class="tg-0lax">在指定位置显示有符号十进制数字</td>
  </tr>
  <tr>
    <td class="tg-0lax">LCD_ShowHexNum(2,1,0xA8,2);</td>
    <td class="tg-0lax">在指定位置显示十六进制数字</td>
  </tr>
  <tr>
    <td class="tg-0lax">LCD_ShowBinNum(2,4,0xAA,8);</td>
    <td class="tg-0lax">在指定位置显示二进制数字</td>
  </tr>
</tbody>
</table>
</div>

注：Keil5在创建头文件后，不会自动将其添加进工程中来，但是文件会放在当前目录下，可以右键手动添加，以方便管理。

**3.2.2 代码展示**
需求：试一试表3-1中所有的功能，然后使用液晶屏观察中间变量的变化。

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/3%E6%A8%A1%E5%9D%97%E5%8C%96%E7%BC%96%E7%A8%8B-%E8%B0%83%E7%94%A8%E5%85%B3%E7%B3%BB.png" width=20%>
</div><div align=center>
图3-3 函数调用关系
</div>

上图给出了本工程的函数调用关系，下面是代码展示：
**- main源文件**
```c
#include <REGX52.H>
#include"LCD1602.h"

void main(){
  int a = 0; // 注意若编译器较老，那么不能中途定义变量
  
//  // 注意下面的代码不要放在while(1)里面，否则LCD会闪
//  LCD_Init();
//  LCD_ShowChar(1,1,'A');
//  LCD_ShowString(1,2,"HelloWorld");
//  LCD_ShowNum(1,12,666,3);
//  LCD_ShowSignedNum(1,15,-9,2);	
//  LCD_ShowHexNum(2,1,0xA8,2);
//  LCD_ShowBinNum(2,5,0xAA,8);
  
  // 下面是实际想用的场景:展示代码运行结果
  LCD_Init();
  while(1){
    LCD_ShowNum(1,1,a++,8);
  }
}
```

**- LCD1602.h**
```c
#ifndef __LCD1602_H__
#define __LCD1602_H__

//用户调用函数：
void LCD_Init();
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char);
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String);
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length);
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);

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
```
