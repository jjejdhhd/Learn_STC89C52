# 4 矩阵键盘

 [toc]

注：笔记主要参考B站江科大自化协教学视频“[51单片机入门教程-2020版 程序全程纯手打 从零开始入门](https://www.bilibili.com/video/BV1Mb411e7re?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click)”。
注：工程及代码文件放在了本人的[Github仓库](https://github.com/jjejdhhd/Learn_STC89C52)。
***

## 4.1 矩阵键盘介绍
在键盘中按键数量较多时，为了减少I/O口的占用，通常将按键排列成矩阵形式。采用逐行或逐列的“扫描”，就可以读出任何位置按键的状态。

> - 数码管扫描（输出扫描）
> 	原理：显示第1位→显示第2位→显示第3位→……，然后快速循环这个过程，最终实现所有数码管同时显示的效果。
> - 矩阵键盘扫描（输入扫描）
> 	原理：读取第1行(列)→读取第2行(列) →读取第3行(列) → ……，然后快速循环这个过程，最终实现所有按键同时检测的效果。但同时按下多个按键可能会造成误判。
> - 以上两种扫描方式的共性：节省I/O口

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E7%9F%A9%E9%98%B5%E9%94%AE%E7%9B%98%E5%AE%9E%E7%89%A9%E5%9B%BE%E5%8F%8A%E5%8E%9F%E7%90%86%E5%9B%BE.png" width=60%>
</div><div align=center>
图4-1 矩阵键盘实物图及原理图
</div>

比如，从上图所示，上四个接口表示行、下四个接口表示列。下面介绍具体的扫描过程：
> - 逐行扫描：
>   先将某一行置0，此时若该行有按键按下，那该行上对应的列也就为0；以此重复所有的行，即完成一次扫描。
> - 逐列扫描：
>   将“逐行扫描”的行、列互换即可。
> 
> 注：由于开发板IO资源有限，有一些开发板会把蜂鸣器IO口与矩阵键盘IO口复用，此时看情况更换扫描方式。

单片机的IO口是一种弱上拉（具有较弱的拉高电平或电位的能力）。关于其是如何实现双向口配置的，可以参考STC官方的器件手册“I/O口各种不同的工作模式及其配置介绍”。

## 4.2 实践：读取矩阵键盘    
需求：在LCD1602第一行显示字符串“Matrix key:”，第二行按照按键丝印编号显示按下了哪个键（松开触发）。

为了代码简洁，编写了单独的矩阵按键检测子函数“MatrixKey.h”，并将其单独作为一个文件独立出来。当然，函数声明和定义应该分别放在头文件、源文件中，但由于函数太少，所以为了方便管理我将函数较少的源文件合并在头文件中。下面是工程的组织架构：

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E8%AF%BB%E5%8F%96%E7%9F%A9%E9%98%B5%E9%94%AE%E7%9B%98%E4%BB%A3%E7%A0%81%E6%A1%86%E6%9E%B6.png" width=20%>
</div><div align=center>
图4-2 “读取矩阵键盘”实验代码调用关系
</div>

代码展示：
**- main.c**
```c
#include <REGX52.H>
#include "LCD1602.h"
#include "Delay.h"
#include "MatrixKey.h"

void main(){
  int key = 0;
  // 初始化LCD显示
  LCD_Init();
  LCD_ShowString(1,1,"Matrix key:");
  LCD_ShowString(2,1,"S00");
  while(1){
    key = MatrixKey();
    if(key){LCD_ShowNum(2,2,key,2);};
  }
}
```

**- MatrixKey.h**
```c
#ifndef __MATRIXKEY_H__
#define __MATRIXKEY_H__

#include <REGX52.H>
#include "Delay.h"
/**
  * @brief :按行扫描一次，检测矩阵键盘被按下的按键（松开返回）
  * @param :无
  * @retval :按下按键的标号（与丝印相同），若没有按下就返回0。
  * 注：若按键按下不松开，会一直停留在此函数，直到按键松开后。
 */
unsigned char MatrixKey(){
  unsigned char key = 0;

  // 扫描第一行
  P1 = 0xff;  P1_7 = 0;
  if(!P1_3 && P1_2 && P1_1 && P1_0){
    Delay(10);    // 按下消抖
    while(!P1_3); // 等着松开
    Delay(10);    // 松开消抖
    return key = 1;
  }else if(P1_3 && !P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_2); Delay(10);
    return key = 2;
  }else if(P1_3 && P1_2 && !P1_1 && P1_0){
    Delay(10); while(!P1_1); Delay(10);
    return key = 3;
  }else if(P1_3 && P1_2 && P1_1 && !P1_0){
    Delay(10); while(!P1_0); Delay(10);
    return key = 4;
  }
  // 扫描第二行
  P1 = 0xff;  P1_6 = 0;
  if(!P1_3 && P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_3); Delay(10);
    return key = 5;
  }else if(P1_3 && !P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_2); Delay(10);
    return key = 6;
  }else if(P1_3 && P1_2 && !P1_1 && P1_0){
    Delay(10); while(!P1_1); Delay(10);
    return key = 7;
  }else if(P1_3 && P1_2 && P1_1 && !P1_0){
    Delay(10); while(!P1_0); Delay(10);
    return key = 8;
  }
  // 扫描第三行
  P1 = 0xff;  P1_5 = 0;
  if(!P1_3 && P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_3); Delay(10);
    return key = 9;
  }else if(P1_3 && !P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_2); Delay(10);
    return key = 10;
  }else if(P1_3 && P1_2 && !P1_1 && P1_0){
    Delay(10); while(!P1_1); Delay(10);
    return key = 11;
  }else if(P1_3 && P1_2 && P1_1 && !P1_0){
    Delay(10); while(!P1_0); Delay(10);
    return key = 12;
  }
  // 扫描第四行
  P1 = 0xff;  P1_4 = 0;
  if(!P1_3 && P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_3); Delay(10);
    return key = 13;
  }else if(P1_3 && !P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_2); Delay(10);
    return key = 14;
  }else if(P1_3 && P1_2 && !P1_1 && P1_0){
    Delay(10); while(!P1_1); Delay(10);
    return key = 15;
  }else if(P1_3 && P1_2 && P1_1 && !P1_0){
    Delay(10); while(!P1_0); Delay(10);
    return key = 16;
  }
  
  return key;
}

#endif
```

**- Dealy.h**
```c
#ifndef __DEALY_H_
#define __DEALY_H_

// 延时cycles ms，晶振@11.0592MHz
void Delay(unsigned int cycles){
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


## 4.3 实践：矩阵键盘密码锁
需求：
实现一个矩阵键盘密码锁。S1\~S9代表数字1\~9，S10代表数字0，S11代表“确认”，S12代表“取消”。
> - LCD第一行左侧显示“Password:”，第二行显示输入的四位密码，每次按下按键会依次左移数字。
> - 按下“确认”时，判断密码是否正确，并显示在LCD第一行右侧“Err!”、“True!”，同时密码归位。
> - 按下“取消”时，当前密码归零。

 <div align=center>
<img src="https://raw.githubusercontent.com/jjejdhhd/Git_img2023/main/8051chip/%E7%9F%A9%E9%98%B5%E9%94%AE%E7%9B%98%E5%AF%86%E7%A0%81%E9%94%81%E4%BB%A3%E7%A0%81%E8%B0%83%E7%94%A8%E5%85%B3%E7%B3%BB.png" width=20%>
</div><div align=center>
图4-3 “矩阵键盘密码锁”实验代码调用关系
</div>

**- main.c**
```c
#include <REGX52.H>
#include "MatrixKey.h"
#include "LCD1602.h"

void main(){
  unsigned int correct = 123; // 正确的密码
  unsigned int password = 0; // 当前的密码值
  unsigned char key = 0; // 矩阵键盘当前按下的值
  // 显示屏初始化
  LCD_Init();
  LCD_ShowString(1,1,"Password:");
  LCD_ShowNum(2,1,0,4);
  while(1){
    key = MatrixKey();
    if(key==11){ // 按下确认键，则进行判断
      if(password == correct)
        LCD_ShowString(1,12,"True!");
      else
        LCD_ShowString(1,13,"Err!");
    }else if(key==12){ // 按下取消键，密码归零
      LCD_ShowString(1,12,"     "); // 清除对错提示
      password = 0;
      LCD_ShowNum(2,1,password,4);
    }else if(key>=1 && key<=10){ // 其他正常的数字键
      LCD_ShowString(1,12,"     "); // 清除对错提示
      key = (key==10) ? 0 : key; // 或者也可以对10取余
      password = (password%1000) * 10 + key;
      LCD_ShowNum(2,1,password,4);
    }
  }
}
```

其余的 **```MatrixKey.h```**、**```Delay.h```**、**```LCD1602.h```**、**```LCD1602.c```** 模块均与4.2节实验相同。
