#include <REGX52.H>
#include "Timer0.h"
#include "PushKey.h"
#include "NixieTube.h"
#include "AT24C02.h"

// 定义分/秒/10倍毫秒/秒表工作标志
unsigned char mins=0,secs=0,ms10=0,StopWatch_flag=0;

void main(){
  unsigned char num_key=0;
  //定时器T0初始化
  Timer0_Init();
  //数码管显示初始化
  NixieTube_SetBuf(6,11);
  NixieTube_SetBuf(3,11);
  while(1){
    num_key = PushKey_GetKey();
    switch(num_key){
      // 开始/暂停秒表
      case 1:
        StopWatch_flag = !StopWatch_flag;
        break;
      // 秒表归零
      case 2:
        ms10 = 0;
        secs = 0;
        mins = 0;
        break;
      // 将当前数据写入到EEPROM中
      case 3:
        AT24C02_WriteByte(0x00,ms10);
        AT24C02_WriteByte(0x01,secs);
        AT24C02_WriteByte(0x02,mins);
        break;
      // 从EEPROM读取当前数据
      case 4:
        ms10 = AT24C02_ReadByte(0x00);
        secs = AT24C02_ReadByte(0x01);
        mins = AT24C02_ReadByte(0x02);
        break;
      default:;
    }
    NixieTube_SetBuf(1,ms10%10);
    NixieTube_SetBuf(2,ms10/10);
    NixieTube_SetBuf(4,secs%10);
    NixieTube_SetBuf(5,secs/10);
    NixieTube_SetBuf(7,mins%10);
    NixieTube_SetBuf(8,mins/10);
  }
}

// 秒表更新函数
void StopWatch_Loop(void){
  if(ms10>=99){
    ms10 = 0;
    if(secs>=59){
      secs = 0;
      if(mins>=59){mins=0;}
      else        {mins++;}
    }
    else{secs++;}
  }
  else{ms10++;}
}

// 定义定时器T0中断后要执行的动作
void Timer0_Routine() interrupt 1{
  static unsigned int count1_T0,count2_T0,count3_T0; //中断次数
  TH0 = 0xfc; TL0 = 0x66; // 恢复溢出周期，近似1ms
  //数码管扫描
  count1_T0++; //数码管使用的中断
  if(count1_T0>1){
    count1_T0 = 0;
    NixieTube_Loop();
  }
  //按键扫描
  count2_T0++; //按键使用的中断
  if(count2_T0>20){
    count2_T0 = 0;
    PushKey_Loop();
  }
  //秒表计数
  count3_T0++;
  if(count3_T0>10){
    count3_T0 = 0;
    if(StopWatch_flag){StopWatch_Loop();}
  }
}
