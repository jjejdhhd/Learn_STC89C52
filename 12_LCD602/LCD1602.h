#ifndef __LCD1602_H__
#define __LCD1602_H__

// LCD1602写指令
void LCD1602_WriteCommd(unsigned char wByte);
// LCD1602写数据
void LCD1602_WriteData(unsigned char wByte);
// 初始化函数
void LCD1602_Init(void);
//显示单个字符
void LCD1602_DispChar(unsigned char Row,unsigned char Column,unsigned char wChar);
//显示字符串
void LCD1602_DispString(unsigned char Row,unsigned char Column,char wString[]);
//显示无符号数字
void LCD1602_DispUnInt(unsigned char Row, unsigned char Column, unsigned int wNum, unsigned char Length);
//显示有符号数字
void LCD1602_DispInt(unsigned char Row, unsigned char Column,int wNum, unsigned char Length);
//显示无符号整型的16进制表示
void LCD1602_DispUnInt_Hex(unsigned char Row, unsigned char Column, unsigned int wNum, unsigned char Length);
//显示无符号整型的2进制表示
void LCD1602_DispUnInt_Bin(unsigned char Row, unsigned char Column, unsigned int wNum, unsigned char Length);
//显示单精度浮点数（有符号）
void LCD1602_DispFloat(unsigned char Row, unsigned char Column, float wNum, unsigned char Total_Len, unsigned char Frac_len);

#endif