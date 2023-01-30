#include <REGX52.H>
#include "Delay.h"
#include "UART.h"

void main(){
  unsigned char num = 0x00; // 要发送的变量
  UART_Init(); // 串口初始化
  while(1){
    UART_SendByte(num);
    Delay(1000); // 软件延时，防止打乱定时器的操作
    num += 1;
  }
}
