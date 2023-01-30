#include <REGX52.H>
#include "DC_Motor.h"
#include "HS0038.h"

void main(){
  unsigned char command=0;//红外接收的命令（键码）
  unsigned char speed_index=0;//转速等级索引
  unsigned char speed_motor[5] = {0,40,50,75,100};
  
  //电机初始化
  DC_Motor_Init();
  //红外接收初始化
  HS0038_Init();
  
  while(1){
    if(HS0038_GetReadFlag() || HS0038_GetRepeFlag()){
      command = HS0038_GetCommand();
      switch(command){
        case 0x16: speed_index=0; break;
        case HS_1: speed_index=1; break;
        case HS_2: speed_index=2; break;
        case HS_3: speed_index=3; break;
        case HS_4: speed_index=4; break;
        default:;
      }
      //调整PWM占空比（转速）
      DC_Motor_SetSpeed(speed_motor[speed_index]); 
    }
  }
}
