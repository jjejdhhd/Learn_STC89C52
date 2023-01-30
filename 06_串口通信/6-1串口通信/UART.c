#include <REGX52.H>

/**
  * @brief :串口通信初始化，4800bps@11.0592MHz
  * @param :无
  * @retval :无
 */
void UART_Init(){
  SCON = 0x40; // 串口控制寄存器，暂时接收不使能
  PCON &= 0x7f; // 波特率选择特殊功能寄存器
  
  // 发送数据不需要开启中断，接收数据才需要
  
  // 下面配置定时器，用于波特率设置
  // 注：串口只能用定时器T1（8位自动重装模式）
  TMOD&=0x0F;TMOD|=0x20; //设置定时器模式
	TL1 = 0x66;  //设置定时初始值
	TH1 = 0xFA;  //设置定时初始值
	ET1 = 0;		 //禁止定时器中断，只要有溢出就可以生成波特率
	TR1 = 1;		 //定时器1开始计时
}

/**
  * @brief :通过串口发送8位数据
  * @param :需要发送的8位数据
  * @retval :无。但函数会直到该字节发送完成才返回。
 */
void UART_SendByte(unsigned char Byte){
  SBUF = Byte;
  while(TI==0); // 等待发送完毕
  TI = 0; // 软件复位
}