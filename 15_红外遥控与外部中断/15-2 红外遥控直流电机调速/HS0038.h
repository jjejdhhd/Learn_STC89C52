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