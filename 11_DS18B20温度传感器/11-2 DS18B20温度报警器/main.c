#include <REGX52.H>
#include "LCD1602.h"
#include "_1Wire.h"
#include "DS18B20.h"
#include "PushKey.h"
#include "Timer0.h"
#include "AT24C02.h"

void main(){
  float tempe = 20.5, tempe_show = 20.5;
  char gate_up=20,gate_bo=15; //默认阈值
  unsigned char key_num=0;
  
  //初始化定时器T0
  Timer0_Init();
  
  //从AT24C02中读取温度数据
//  AT24C02_WriteByte(0x00,125);
//  AT24C02_WriteByte(0x01,-125);//此两句仅供测试使用
  gate_up = AT24C02_ReadByte(0x00);
  gate_bo = AT24C02_ReadByte(0x01);
  if(gate_up<=gate_bo || gate_up>127 || gate_bo<-60){
    gate_up = 20; gate_bo = 15;
  }
  
  //初始化LCD
  LCD_Init();
  LCD_ShowString(1,1,"U:     L:");
  LCD_ShowSignedNum(1,3,(int)gate_up,3);
  LCD_ShowSignedNum(1,10,(int)gate_bo,3);
  
  while(1){
    key_num = PushKey_GetKey();
    if(key_num){
      switch(key_num){
        case 1://温度上限+1
          if(gate_up<127){gate_up++;}
          AT24C02_WriteByte(0x00,gate_up);
          break;
        case 2://温度上限-1
          if(gate_up>-59){gate_up--;}
          if(gate_up<=gate_bo){gate_bo--;}
          AT24C02_WriteByte(0x00,gate_up);
          AT24C02_WriteByte(0x01,gate_bo);
          break;
        case 3://温度下限+1
          if(gate_bo<126){gate_bo++;}
          if(gate_up<=gate_bo){gate_up++;}
          AT24C02_WriteByte(0x00,gate_up);
          AT24C02_WriteByte(0x01,gate_bo);
          break;
        case 4://温度下限-1
          if(gate_bo>-60){gate_bo--;}
          AT24C02_WriteByte(0x01,gate_bo);
          break;
        default:;
      }
      //在显示屏上更新显示阈值
      LCD_ShowSignedNum(1,3,(int)gate_up,3);
      LCD_ShowSignedNum(1,10,(int)gate_bo,3);
    }
    
    DS18B20_ConvertTemp();// 温度转换
    tempe = DS18B20_ReadTemp();// 温度读取
    tempe_show = tempe;
    //阈值判断
    if(tempe>gate_up)     {LCD_ShowString(2,12,"High!");}
    else if(tempe<gate_bo){LCD_ShowString(2,12," Low!");}
    else                  {LCD_ShowString(2,12,"Norm!");}
    //温度显示
    if(tempe>0){LCD_ShowChar(2,1,'+');}
    else       {LCD_ShowChar(2,1,'-');tempe_show=-tempe_show;}
    LCD_ShowNum(2,2,(int)tempe_show,3);
    LCD_ShowString(2,5,".");
    LCD_ShowNum(2,6,(int)((tempe_show-(int)tempe_show)*100),2);
    LCD_ShowString(2,9,"C");
  }
}

// 定义定时器T0中断后要执行的动作
void Timer0_Routine() interrupt 1{
  static unsigned int count_T0; //中断次数
  count_T0++; //更新中断次数
  TH0 = 0xfc; TL0 = 0x66; // 恢复溢出周期，近似1ms
  if(count_T0>20){
    count_T0 = 0;
    PushKey_Loop();
  }  
}
