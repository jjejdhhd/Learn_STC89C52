#include <REGX52.H>

sbit LED0 = P2^0;
unsigned int PWM_Period = 100;
unsigned int PWM_Duty = 99;

// 延时函数一定要有，但单次延时应在10us以下
void Delay(unsigned int cycles){
	while(cycles--);
}

void main(){
  unsigned char i;
  P2 = 0xff; //LED初始全部熄灭
  while(1){
    for(PWM_Duty=0;PWM_Duty<PWM_Period;PWM_Duty++){
      for(i=0;i<10;i++){
        LED0 = 0;
        Delay(PWM_Duty);
        LED0 = 1;
        Delay(PWM_Period-PWM_Duty);
      }
    }
    for(PWM_Duty=PWM_Period-1;PWM_Duty>0;PWM_Duty--){
      for(i=0;i<10;i++){
        LED0 = 0;
        Delay(PWM_Duty);
        LED0 = 1;
        Delay(PWM_Period-PWM_Duty);
      }
    }
  }
}
