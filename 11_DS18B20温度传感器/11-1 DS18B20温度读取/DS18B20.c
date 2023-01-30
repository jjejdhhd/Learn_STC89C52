#include "_1Wire.h"

// ROM指令、功能指令
#define SKIP_ROM 0xCC
#define CONVERT_T 0x44
#define READ_SCRATCHPAD 0xBE

/**
  * @brief :将传感器温度值存放到暂存器中。
  * @param :无。
  * @retval :无。
 */
void DS18B02_ConvertTemp(void){// 温度转换
  //1.初始化
  _1Wire_Init();
  //2.跳过ROM
  _1Wire_SendByte(SKIP_ROM);
  //3.开始温度变换
  _1Wire_SendByte(CONVERT_T);
  //4.不必延时等待
}

/**
  * @brief :将暂存器中的温度值读出。
  * @param :无。
  * @retval :返回单精度浮点类的数据。
 */
float DS18B02_ReadTemp(void){// 温度读取
  unsigned char temp_H,temp_L;
  int temperature;
  //1.初始化
  _1Wire_Init();
  //2.跳过ROM
  _1Wire_SendByte(SKIP_ROM);
  //3.读暂存器
  _1Wire_SendByte(READ_SCRATCHPAD);
  //4.读前两个比特
  temp_L = _1Wire_ReceByte();
  temp_H = _1Wire_ReceByte();
  //5.复位，不再读取后续信息
  _1Wire_Init();
  //6.计算读取到的温度信息
  if(temp_H&0xf8){//负数
    temperature = -(~(temp_H*256+temp_L) + 1);
  }else{
    temperature = temp_H*256 + temp_L;
  }
  return (float)(temperature)*0.0625;
}