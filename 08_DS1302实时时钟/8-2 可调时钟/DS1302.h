#ifndef __DS1302_H__
#define __DS1302_H__

void DS1302_Init(void);//初始化DS1302
unsigned char DS1302_ReadByte(unsigned char read_comd);//从DS1302中读出数据
void DS1302_WriteByte(unsigned char wri_comd,wri_byte);//向DS1302中写入数据
void DS1302_ReadAll(unsigned char *info);//读出所有信息
void DS1302_SetAll(unsigned char *info);//写入所有信息

#endif