#include <REGX52.H>
#include "UART.h"

void main(){
  UART_Init(); // 串口初始化
  while(1){}
}

// 串口接收中断子函数
// 注意发送和接收两种中断都会进入到这个子函数中来
void UART_Routine() interrupt 4{
  if(RI==1){
    P2 = ~SBUF; // 存储接收到的字节
    RI = 0; // 软件清空接收完成标志
    UART_SendByte(SBUF);
  }
}
