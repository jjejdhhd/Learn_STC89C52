#include <REGX52.H>
#include "Matrix_LED.h"
// 定义图案数据，注意是按列循环的
unsigned char led_face[8] = 
{0xc0,0xdb,0xd9,0xc5,0xc5,0xd9,0xdb,0xc0};
//            {{1,1,1,1,1,1,1,1},
//             {1,1,1,1,1,1,1,1},
//             {0,0,0,0,0,0,0,0},
//             {0,1,1,0,0,1,1,0},
//
//             {0,1,1,0,0,1,1,0},
//             {0,0,0,1,1,0,0,0},
//             {0,1,0,0,0,0,1,0},
//             {0,1,1,1,1,1,1,0},};

void main(){
  unsigned char i;
  P3_5 = 0; P3_6 = 0; // 对移动寄存器时钟复位
  while(1){
    for(i=0;i<8;i++){
      MatrixLED_ShowColumn(i,led_face[i]);
    }
  }
}
