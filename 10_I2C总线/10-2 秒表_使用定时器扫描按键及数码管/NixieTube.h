#ifndef __NIXIETUBE_H__
#define __NIXIETUBE_H__

// 数据缓冲区，注意每个数字显示的范围就是0~9
unsigned char NixieTube_buf[8] = {1,2,3,4,5,6,7,8};

// 给出数字0~9的定义（符合数组的索引），最后两个依次表示不显示/显示横杠
unsigned char NixieTube_number[12] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x40};


/**
  * @brief :改变数码管数据缓冲区的数值。
* @param :index索引1~8，num数值0~11(10表示不显示，11表示横杠)
  * @retval :无
 */
void NixieTube_SetBuf(unsigned char index,num){
  NixieTube_buf[index-1] = num;
}

/**
  * @brief :在指定数码管显示指定的数字。
  * @param :led片选信号0~7，num表示显示的索引0~11
  * @retval :无。
 */
void NixieTube_Signle(unsigned char led, unsigned char num){
  // 给出选择的LED1~LED8的定义（实际上只用P2的2/3/4引脚）
  unsigned char sel_led[8] = {0x00,0x04,0x08,0x0c,0x10,0x14,0x18,0x1c};
  P0 = 0x00; // 数码管不显示，消影
  P2 = sel_led[led]; // 选择数码管：LED1
  P0 = NixieTube_number[num];  // 数码管显示
}

//定时器中断函数执行的数码管扫描函数，每次只扫描一个数码管
void NixieTube_Loop(void){
  static unsigned char i=0;
  NixieTube_Signle(i,NixieTube_buf[i]);
  i++;
  if(i>=8){i=0;}
}

#endif