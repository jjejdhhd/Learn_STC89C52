#ifndef __PUSHKEY_H__
#define __PUSHKEY_H__

#include "Delay.h"
/**
  * @brief :检测按下了哪个按键开关
  * @param :无
  * @retval :输出按键开关编号1~4，不按返回0，松开触发
 */
unsigned char PushKey(){
  unsigned char key = 0;
  if(!P3_1){Delay(10);while(!P3_1);Delay(10);key=1;}
  else if(!P3_0){Delay(10);while(!P3_0);Delay(10);key=2;}
  else if(!P3_2){Delay(10);while(!P3_2);Delay(10);key=3;}
  else if(!P3_3){Delay(10);while(!P3_3);Delay(10);key=4;}
  return key;
}

#endif