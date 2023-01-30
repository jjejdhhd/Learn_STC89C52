#ifndef __1WIRE_H__
#define __1WIRE_H__

unsigned char _1Wire_Init(void);//初始化
void _1Wire_SendBit(unsigned char sdata);// 发送1位
unsigned char _1Wire_ReceBit(void);// 接收1位
void _1Wire_SendByte(unsigned char sbyte);// 发送一个字节
unsigned char _1Wire_ReceByte(void);// 接收一个字节

#endif