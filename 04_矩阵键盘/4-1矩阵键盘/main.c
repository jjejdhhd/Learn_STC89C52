#include <REGX52.H>
#include "LCD1602.h"
#include "Delay.h"
#include "MatrixKey.h"

void main(){
  int key = 0;
  // 初始化LCD显示
  LCD_Init();
  LCD_ShowString(1,1,"Matrix key:");
  LCD_ShowString(2,1,"S00");
  while(1){
    key = MatrixKey();
    if(key){LCD_ShowNum(2,2,key,2);};
  }
}
