#ifndef __NIXIETUBE_H__
#define __NIXIETUBE_H__

void NixieTube(unsigned char led, unsigned char num){
  // 给出数字0~9的定义（符合数组的索引）
  unsigned char number[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  // 给出选择的LED1~LED8的定义（实际上只用P2的2/3/4引脚）
  unsigned char sel_led[8] = {0x00,0x04,0x08,0x0c,0x10,0x14,0x18,0x1c};
  P0 = 0x00; // 数码管不显示，消影
  P2 = sel_led[led-1]; // 选择数码管：LED1
  P0 = number[num];  // 数码管显示7
}

#endif