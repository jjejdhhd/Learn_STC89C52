#include <REGX52.H>
#include "Timer0.h"

// 下面的乐谱可以替换
/***************************************************/
// 《小星星》乐谱定义
#define  music_len 42//指定乐谱长度
unsigned char code music_freq_sel[2][music_len] = {
{12,12,19,19, 21,21,19,    17,17,16,16, 14,14,12,    19,19,17,17, 16,16,14,   
 19,19,17,17, 16,16,14,    12,12,19,19, 21,21,19,    17,17,16,16, 14,14,12   },
{4,4,4,4, 4,4,8, 4,4,4,4, 4,4,8, 4,4,4,4, 4,4,8,
 4,4,4,4, 4,4,8, 4,4,4,4, 4,4,8, 4,4,4,4, 4,4,8}};
/***************************************************/

// 通用延时函数,1ms
void Delay1ms(unsigned int cycles){
  unsigned char i, j;
  do{
    i = 2;
    j = 199;
    do{
      while (--j);
    }while (--i);
  }while(--cycles);
}

// 重新定义端口名
sbit Buzzer_con = P2^5;

// 定义四分音符时长（500ms）
#define SPEED 500

//不同频率所对应的定时器重装载值
unsigned int freq_reload[37] =
{0,63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64524,
   64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
   65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283};
//定义所需要选择的频率索引
unsigned char freq_sel = 0;
//定义音乐的索引变换，时长变换(时长以1/16音符为基准)
#define N0  0
#define L1  1
#define L1_ 2
#define L2  3
#define L2_ 4
#define L3  5
#define L4  6
#define L4_ 7
#define L5  8
#define L5_ 9
#define L6  10
#define L6_ 11
#define L7  12
#define M1  13
#define M1_ 14
#define M2  15
#define M2_ 16
#define M3  17
#define M4  18
#define M4_ 19
#define M5  20
#define M5_ 21
#define M6  22
#define M6_ 23
#define M7  24
#define H1  25
#define H1_ 26
#define H2  27
#define H2_ 28
#define H3  29
#define H4  30
#define H4_ 31
#define H5  32
#define H5_ 33
#define H6  34
#define H6_ 35
#define H7  36

void main(){
  unsigned int i;
  //定时器T0初始化
  Timer0_Init();
  freq_sel = 0;
  while(1){
    for(i=0;i<music_len;i++){
      freq_sel = music_freq_sel[0][i];
      Delay1ms(SPEED/4*music_freq_sel[1][i]);
      TR0 = 0;
      Delay1ms(5);
      TR0 = 1;
    }
    TR0 = 0;
    Delay1ms(5000); //暂停一段时间
    TR0 = 1;
  }
}

// 定义定时器T0中断后要执行的动作
void Timer0_Routine() interrupt 1{
  //当频率不是“0”，才进行相应的振荡
  if(freq_reload[freq_sel]){
    TH0 = freq_reload[freq_sel]/256;
    TL0 = freq_reload[freq_sel]%256;
    Buzzer_con = !Buzzer_con;
  }  
}
