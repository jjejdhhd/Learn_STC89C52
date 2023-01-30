#include <REGX52.H>

// 下面可能需要更改
/**********************************/
// 对常用总线重命名
sbit SCL = P2^1;
sbit SDA = P2^0;
# define AT24C02_WR_ADDR 0xa0
# define AT24C02_RD_ADDR 0xa1

// 等待写周期的延时函数
void AT24C02_Delay5ms(){//@11.0592MHz
	unsigned char i, j;
	i = 9;
	j = 244;
	do
	{
		while (--j);
	} while (--i);
}
/**********************************/

// 固定格式的函数
/**********************************/
/**
  * @brief :采用I2C“字节写”的方式，向AT24C02写入1字节数据。
  * @param :wr_addr写入地址，wr_byte写入数据
  * @retval :代表写入是否成功。
 */
unsigned char AT24C02_WriteByte(unsigned char wr_addr,wr_byte){
  unsigned char err=0;
  unsigned char i;
  //起始条件
  SDA = 1;
  SCL = 1;
  SDA = 0;
  SCL = 0;
  //发送第一字节数据
  for(i=0;i<8;i++){
    SCL = 0;
    SDA = AT24C02_WR_ADDR&(0x80>>i);
    SCL = 1;
  }
  //接收第一次应答
  SCL = 0;
  SDA= 1; //开漏输出，释放SDA
  SCL = 1;
  if(SDA){err=1;}
  SCL = 0;
  if(err){return err;}
  //发送第二字节数据：地址
  for(i=0;i<8;i++){
    SCL = 0;
    SDA = wr_addr&(0x80>>i);
    SCL = 1;
  }
  //接收第二次应答
  SCL = 0;
  SDA= 1; //开漏输出，释放SDA
  SCL = 1;
  if(SDA){err=1;}
  SCL = 0;
  if(err){return err;}
  //发送第三字节数据：数据
  for(i=0;i<8;i++){
    SCL = 0;
    SDA = wr_byte&(0x80>>i);
    SCL = 1;
  }
  //接收第三次应答
  SCL = 0;
  SDA= 1; //开漏输出，释放SDA
  SCL = 1;
  if(SDA){err=1;}
  if(err){return err;}
  //终止条件
  SCL = 0;
  SDA = 0;
  SCL = 1;
  SDA = 1;
  //等待写周期完成
  AT24C02_Delay5ms();
  return err;
}

/**
  * @brief :采用I2C“随机读”的方式，从AT24C02指定地址读出1字节数据。
  * @param :rd_addr读出地址
  * @retval :读出的数据。
  * 一般读不要会出错，所以就不返回错误标志err了，但保留其定义。
 */
unsigned char AT24C02_ReadByte(unsigned char rd_addr){
  unsigned char rd_byte = 0x00;
  unsigned char err=0,wr_byte1=0xa0;
  unsigned char i;
  //起始条件
  SDA = 1;
  SCL = 1;
  SDA = 0;
  SCL = 0;
  //发送第一字节数据
  for(i=0;i<8;i++){
    SCL = 0;
    SDA = AT24C02_WR_ADDR&(0x80>>i);
    SCL = 1;
  }
  //接收第一次应答
  SCL = 0;
  SDA= 1; //开漏输出，释放SDA
  SCL = 1;
  err = SDA; 
  //发送第二字节数据：地址
  for(i=0;i<8;i++){
    SCL = 0;
    SDA = rd_addr&(0x80>>i);
    SCL = 1;
  }
  //接收第二次应答
  SCL = 0;
  SDA= 1; //开漏输出，释放SDA
  SCL = 1;
  err = SDA; 
  SCL = 0;
  //起始条件
  SDA = 1;
  SCL = 1;
  SDA = 0;
  SCL = 0;
  //发送第一字节数据
  for(i=0;i<8;i++){
    SCL = 0;
    SDA = AT24C02_RD_ADDR&(0x80>>i);
    SCL = 1;
  }
  //接收第三次应答
  SCL = 0;
  SDA= 1; //开漏输出，释放SDA
  SCL = 1;
  err = SDA;
  SCL = 0;
  //接收数据：数据
  for(i=0;i<8;i++){
    SCL = 0;
    if(SDA){rd_byte |= (0x80>>i);}
    SCL = 1;
  }
  //NO ACK：主机对SDA无动作
  SCL = 0;
  SDA = 1;
  SCL = 1;
  SCL = 0;
  //终止条件
  SCL = 0;
  SDA = 0;
  SCL = 1;
  SDA = 1;
  return rd_byte;
}