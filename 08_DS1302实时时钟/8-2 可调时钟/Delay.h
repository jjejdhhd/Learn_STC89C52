#ifndef __DEALY_H_
#define __DEALY_H_

// 延时cycles ms，晶振@11.0592MHz
void Delay(unsigned char cycles){
  unsigned char i, j;
  do{
    i = 2;
    j = 199;
    do{
      while (--j);
    }while (--i);
  }while(--cycles);
}

#endif
