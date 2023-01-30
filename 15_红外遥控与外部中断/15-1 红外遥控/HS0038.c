#include <REGX52.H>
#include "Timer0.h"
#include "Int0.h"

static unsigned char state = 0;        //状态变量
static unsigned int  time_count = 0;   //存储定时器T0计数值
static unsigned char rdata[4];         //存储接收的数据
static unsigned char ReadFlag = 0;     //是否有新数据
static unsigned char RepeFlag = 0;     //是否重复发送信号

//红外接收初始化
void HS0038_Init(void){
  Timer0_CountInit();//计数器0初始化
  Int0_Init();  //外部中断0初始化
}

//是否已经接收完红外数据
unsigned char HS0038_GetReadFlag(void){
  if(ReadFlag){
    ReadFlag = 0;
    return 1;
  }
  else{
    return 0;
  }
}

//是否接收到重复数据
unsigned char HS0038_GetRepeFlag(void){
  if(RepeFlag){
    RepeFlag = 0;
    return 1;
  }
  else{
    return 0;
  }
}

//读取地址码
unsigned char HS0038_GetAddress(void){
  return rdata[0];
}

//读取命名码（键码）
unsigned char HS0038_GetCommand(void){
  return rdata[2];
}


//外部中断0中断后所需要执行的函数
void Int0_Routine(void) interrupt 0 {
  static unsigned char i=0;//循环变量
  //空闲状态
  if(state==0){
    Timer0_SetCounter(0x0000);
    Timer0_Run(1);//计时器T0开始工作
    state = 1;
  }
  //判断状态
  else if(state==1){
    time_count = Timer0_GetCounter();
    Timer0_SetCounter(0x0000);
    //起始位Start
    if(time_count>12442-500 && time_count<12442+500){
      i = 0;
      rdata[0] = 0x00;
      rdata[1] = 0x00;
      rdata[2] = 0x00;
      rdata[3] = 0x00;
      state = 2;
    }
    //重复位Repeat
    else if(time_count>10368-500 && time_count<10368+500){
      RepeFlag = 1;
      state = 0;
    }
    //其他未知状态
    else{state = 0;}
  }
  //接收数据的状态
  else if(state==2){
    time_count = Timer0_GetCounter();
    Timer0_SetCounter(0x0000);
    //接收到0
    if(time_count>1032-500 && time_count<1032+500){
      rdata[i/8] &= ~(0x01<<(i%8));
    }
    //接收到1
    else if(time_count>2074-500 && time_count<2074+500){
      rdata[i/8] |= (0x01<<(i%8));
    }
    //接收到非0非1
    else{
      i=0;
      state = 0;
    }
    
    //判断数据是否接收完成
    if(i>=31){
      i=0;
      //判断接收是否正确
      if((rdata[0]==~rdata[1]) && (rdata[2]==~rdata[3])){
        ReadFlag = 1;
      }else{
        ReadFlag = 0;
      }
      Timer0_Run(0);//计时器T0停止工作
      state = 0;
    }else{
      i++;
      state = state;
    }
  }
}