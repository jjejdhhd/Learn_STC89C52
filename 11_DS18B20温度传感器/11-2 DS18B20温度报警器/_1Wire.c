#include <REGX52.H>

// 总线重新命名
sbit _1Wire_DQ = P3^7;

/**
  * @brief :单总线初始化。
  * @param :无
  * @retval :从机是否应答（1/0）。
 */
unsigned char _1Wire_Init(void){
  unsigned char i, ack=0;
  _1Wire_DQ = 1;
  _1Wire_DQ = 0;
  i = 230; while (--i); //延时500us
  _1Wire_DQ = 1;
  i = 36;	while (--i); //延时80us
  if(!_1Wire_DQ){ack = 1;}
  i = 230; while (--i); //延时500us
  return ack;
}

/**
  * @brief :单总线发送一位数据。
  * @param :sdata发送数据，最低位表示要发送的比特
  * @retval :无
 */
void _1Wire_SendBit(unsigned char sdata){// 发送1位
  unsigned char i;
  _1Wire_DQ = 1;
  _1Wire_DQ = 0;
  i = 4; while (--i); // 延时10us
  _1Wire_DQ = sdata;
  i = 23;	while (--i); // 延时50us，等待从机采样完毕
  _1Wire_DQ = 1;
}

/**
  * @brief :单总线接收一位数据。
  * @param :无
  * @retval :rdata接收数据，最低位表示接收的比特
 */
unsigned char _1Wire_ReceBit(void){// 接收1位
  unsigned char i, rdata=0x00;
  _1Wire_DQ = 1;
  _1Wire_DQ = 0;
  i = 2; while (--i); // 延时5us
  _1Wire_DQ = 1;
  i = 4; while (--i);// 延时10us
  if(_1Wire_DQ) rdata = 0x01;
  i = 20;	while (--i); // 延时45us，等待时间片结束
  //此处由从机释放总线
  return rdata;
}

/**
  * @brief :单总线发送一个字节。
  * @param :sbyte发送的字节。
  * @retval :无。
 */
void _1Wire_SendByte(unsigned char sbyte){// 发送一个字节
  unsigned char i;
  for(i=0;i<8;i++){
    _1Wire_SendBit(sbyte&(0x01<<i));//先发低位
  }
}

/**
  * @brief :单总线接收一个字节。
  * @param :无。
  * @retval :rbyte接收的字节。
 */
unsigned char _1Wire_ReceByte(void){// 接收一个字节
  unsigned char i,rbyte = 0x00;
  for(i=0;i<8;i++){
    if(_1Wire_ReceBit()){
      rbyte |= (0x01<<i);//先收低位
    }
  }
  return rbyte;
}