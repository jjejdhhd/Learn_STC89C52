#include <REGX52.H>
#include "Matrix_LED.h"

// 定义图案数据，注意是按列循环的
// 下面这个数据如果不加"code"，就会存储在片内RAM中，可能会导致存储空间不足。
// 添加"code"之后，这个数组就会存储在片外Flash上，空间很大，但不允许更改。
unsigned char code led_animation[] = 
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x0A,0x7E,0x1C,0x7E,0x0E,0x04,0x04,
  0x30,0xFB,0xFF,0x3C,0xFF,0xFF,0x34,0x0C,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//
void main(){
  unsigned char cur_pic[8];
  unsigned char col=0,offset=0;//取出当前需要显示的数据
  unsigned int MAX_OFFSET = sizeof(led_animation)-8;
  unsigned int cnt_show = 0;//显示次数计数
  unsigned char MAX_CNT = 15;//控制显示速率
  MatrixLED_Init(); //LED点阵屏初始化
  while(1){
    for(offset=0;offset<MAX_OFFSET;offset+=8){
      cnt_show = 0;
      // 取出当前需要显示的数据
      for(col=0;col<8;col++){
        cur_pic[col]=led_animation[col+offset];
      }
      // 重复显示以控制动画速度
      do{
        MatrixLED_ShowPic(cur_pic);
        cnt_show++;
      }while(cnt_show<MAX_CNT);
    }
  }
}
