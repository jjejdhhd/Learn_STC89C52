#include <REGX52.H>
#include "LCD1602.h"
#include "HS0038.h"

void main(){
  unsigned char IR_addr, IR_comd, num=0x00;
  //LCD初始化
  LCD1602_Init();
  LCD1602_DispString(1,1,"ADDR COMD VOL");
  LCD1602_DispString(2,1,"  00   00 000");
  //红外接收初始化
  HS0038_Init();
  P2_0 = 1;
  while(1){
    if(HS0038_GetReadFlag() || HS0038_GetRepeFlag()){
      IR_addr = HS0038_GetAddress();
      IR_comd = HS0038_GetCommand();
      //更新显示
      if     (IR_comd==HS_VOL_MINUS && num>0){num--;}
      else if(IR_comd==HS_VOL_ADD && num<255){num++;}      
      LCD1602_DispUnInt_Hex(2,3,IR_addr,2);
      LCD1602_DispUnInt_Hex(2,8,IR_comd,2);
      LCD1602_DispUnInt(2,11,num,3);
    }
  }
}
