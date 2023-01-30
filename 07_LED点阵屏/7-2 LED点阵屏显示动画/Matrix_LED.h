#ifndef __MATRIX_LED_H__
#define __MATRIX_LED_H__

void MatrixLED_Init();//LED点阵屏初始化
void MatrixLED_ShowColumn(unsigned char Column,Data); //控制LED点阵的某一列显示
void MatrixLED_ShowPic(unsigned char Data[8]); //控制LED点阵显示一副图案
  
#endif