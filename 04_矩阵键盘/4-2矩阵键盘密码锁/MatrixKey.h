#ifndef __MATRIXKEY_H__
#define __MATRIXKEY_H__

#include <REGX52.H>
#include "Delay.h"
/**
  * @brief :按行扫描一次，检测矩阵键盘被按下的按键（松开返回）
  * @param :无
  * @retval :按下按键的标号（与丝印相同），若没有按下就返回0。
  * 注：若按键按下不松开，会一直停留在此函数，直到按键松开后。
 */
unsigned char MatrixKey(){
  unsigned char key = 0;

  // 扫描第一行
  P1 = 0xff;  P1_7 = 0;
  if(!P1_3 && P1_2 && P1_1 && P1_0){
    Delay(10);    // 按下消抖
    while(!P1_3); // 等着松开
    Delay(10);    // 松开消抖
    return key = 1;
  }else if(P1_3 && !P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_2); Delay(10);
    return key = 2;
  }else if(P1_3 && P1_2 && !P1_1 && P1_0){
    Delay(10); while(!P1_1); Delay(10);
    return key = 3;
  }else if(P1_3 && P1_2 && P1_1 && !P1_0){
    Delay(10); while(!P1_0); Delay(10);
    return key = 4;
  }
  // 扫描第二行
  P1 = 0xff;  P1_6 = 0;
  if(!P1_3 && P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_3); Delay(10);
    return key = 5;
  }else if(P1_3 && !P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_2); Delay(10);
    return key = 6;
  }else if(P1_3 && P1_2 && !P1_1 && P1_0){
    Delay(10); while(!P1_1); Delay(10);
    return key = 7;
  }else if(P1_3 && P1_2 && P1_1 && !P1_0){
    Delay(10); while(!P1_0); Delay(10);
    return key = 8;
  }
  // 扫描第三行
  P1 = 0xff;  P1_5 = 0;
  if(!P1_3 && P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_3); Delay(10);
    return key = 9;
  }else if(P1_3 && !P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_2); Delay(10);
    return key = 10;
  }else if(P1_3 && P1_2 && !P1_1 && P1_0){
    Delay(10); while(!P1_1); Delay(10);
    return key = 11;
  }else if(P1_3 && P1_2 && P1_1 && !P1_0){
    Delay(10); while(!P1_0); Delay(10);
    return key = 12;
  }
  // 扫描第四行
  P1 = 0xff;  P1_4 = 0;
  if(!P1_3 && P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_3); Delay(10);
    return key = 13;
  }else if(P1_3 && !P1_2 && P1_1 && P1_0){
    Delay(10); while(!P1_2); Delay(10);
    return key = 14;
  }else if(P1_3 && P1_2 && !P1_1 && P1_0){
    Delay(10); while(!P1_1); Delay(10);
    return key = 15;
  }else if(P1_3 && P1_2 && P1_1 && !P1_0){
    Delay(10); while(!P1_0); Delay(10);
    return key = 16;
  }
  
  return key;
}

#endif