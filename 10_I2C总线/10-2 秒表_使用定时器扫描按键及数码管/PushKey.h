#ifndef __PUSHKEY_H__
#define __PUSHKEY_H__

// 上一次按下的按键
unsigned char num_PushKey=0;

/**
  * @brief :告诉主函数按下了哪个按键。
  * @param :无
  * @retval :按键编号1~4
 */
unsigned char PushKey_GetKey(void){
  unsigned char temp;
  temp = num_PushKey;
  num_PushKey = 0;
  return temp;
}

/**
  * @brief :检测按下了哪个按键开关
  * @param :无
  * @retval :输出按键开关编号1~4，不按返回0，松开触发
 */
unsigned char PushKey_SingleDetect(void){
  unsigned char key = 0;
  if(!P3_1)     {key=1;}
  else if(!P3_0){key=2;}
  else if(!P3_2){key=3;}
  else if(!P3_3){key=4;}
  return key;
}

//定时器中断函数执行的按键扫描函数
void PushKey_Loop(void){
  static unsigned char state_last=0, state_curr=0;
  state_last = state_curr;
  state_curr = PushKey_SingleDetect();
  if     (state_last==1 && state_curr==0){num_PushKey=1;}
  else if(state_last==2 && state_curr==0){num_PushKey=2;}
  else if(state_last==3 && state_curr==0){num_PushKey=3;}
  else if(state_last==4 && state_curr==0){num_PushKey=4;}
}

#endif