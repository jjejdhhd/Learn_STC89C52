#ifndef __PUSHKEY_H__
#define __PUSHKEY_H__

// 延时cycles ms，晶振@11.0592MHz
void PushKey_Delay(unsigned char cycles){
  unsigned char i, j;
  do{
    i = 2;
    j = 199;
    do{
      while (--j);
    }while (--i);
  }while(--cycles);
}

/**
  * @brief :检测按下了哪个按键开关
  * @param :无
  * @retval :输出按键开关编号1~4，不按返回0，松开触发
 */
unsigned char PushKey(){
  unsigned char key = 0;
  if(!P3_1)     {PushKey_Delay(10);while(!P3_1);PushKey_Delay(10);key=1;}
  else if(!P3_0){PushKey_Delay(10);while(!P3_0);PushKey_Delay(10);key=2;}
  else if(!P3_2){PushKey_Delay(10);while(!P3_2);PushKey_Delay(10);key=3;}
  else if(!P3_3){PushKey_Delay(10);while(!P3_3);PushKey_Delay(10);key=4;}
  return key;
}

#endif