#include <REGX52.H>
#include "LCD1602.h"

//对端口进行重新命名
sbit SPI_CS   = P3^5;
sbit SPI_DCLK = P3^6;
sbit SPI_DIN  = P3^4;
sbit SPI_DOUT = P3^7;
#define AIN0_12AD 0x94 //1001 0100 电位器
#define AIN1_12AD 0xd4 //1101 0100 热敏电阻
#define AIN2_12AD 0xa4 //1010 0100 光敏电阻

//延时函数
void Delay1000ms(){//@11.0592MHz
	unsigned char i, j, k;
	i = 8; j = 1; k = 243;
	do{
		do{
			while (--k);
		}while (--j);
	}while (--i);
}

//XPT2046读取AD转换数值，SPI通信
unsigned int XPT2046_ReadAD(unsigned char Commd){
  unsigned int AD_value = 0x0000;
  unsigned char i=0;
  // 初始化
  SPI_CS = 1;
  SPI_DCLK = 0;
  SPI_DIN = 0;
  //发送控制字
  SPI_CS = 0;
  for(i=0;i<8;i++){
    SPI_DIN = Commd&(0x80>>i);
    SPI_DCLK = 1;
    SPI_DCLK = 0;
  }
  SPI_DIN = 0;
  //接收数据
  for(i=0;i<16;i++){
    SPI_DCLK = 1;
    SPI_DCLK = 0;
    if(SPI_DOUT){AD_value|=(0x8000>>i);}
  }
  SPI_CS = 1;
  return AD_value;
}

void main(){
  unsigned int AIN0=0x0000,AIN1=0x0000,AIN2=0x0000;
  //LCD1602初始化
  LCD1602_Init();
  LCD1602_DispString(1,1,"AD   NTC  GR  ");
  LCD1602_DispString(2,1,"0000 0000 0000");
  while(1){
    AIN0 = XPT2046_AD(AIN0_12AD)/16;
    AIN1 = XPT2046_AD(AIN1_12AD)/16;
    AIN2 = XPT2046_AD(AIN2_12AD)/16;
    LCD1602_DispUnInt(2,1,AIN0,4);
    LCD1602_DispUnInt(2,6,AIN1,4);
    LCD1602_DispUnInt(2,11,AIN2,4);
    Delay1000ms();
  }
}
