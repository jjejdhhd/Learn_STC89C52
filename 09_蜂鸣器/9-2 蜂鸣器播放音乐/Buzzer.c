#include <REGX52.H>

// 重新定义端口名
sbit Buzzer_con = P2^5;

/**
  * @brief :蜂鸣器专用延时函数，@11.0592MHz晶振，延时500us
  * @param :需要延时500us的个数
  * @retval :无
 */
void Buzzer_Delay100us(unsigned int cycles){
	unsigned char i;
  do{
	i = 43;
	while (--i);
  }while(--cycles);
}


/**
  * @brief :蜂鸣器以默认频率1000Hz，持续响指定时间。
  * @param :cycles持续时间（响cycles个1ms）。
  * @retval :无
 */
void Buzzer_Time(unsigned int ms){
  unsigned int i;
  for(i=0;i<ms;i++){
    Buzzer_con = !Buzzer_con;//每500us翻转一次
    Buzzer_Delay100us(5);//控制蜂鸣器频率为1000Hz
  }
}