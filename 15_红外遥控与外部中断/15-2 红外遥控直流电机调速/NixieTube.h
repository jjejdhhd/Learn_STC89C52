#ifndef __NIXIETUBE_H__
#define __NIXIETUBE_H__

//改变数码管数据缓冲区的数值
void NixieTube_SetBuf(unsigned char index,unsigned char num);
//在指定数码管显示指定的数字
void NixieTube_Signle(unsigned char led, unsigned char num);
//定时器中断函数执行的数码管扫描函数
void NixieTube_Loop(void);

#endif