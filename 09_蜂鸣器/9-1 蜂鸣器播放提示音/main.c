#include <REGX52.H>
#include "PushKey.h"
#include "NixieTube.h"
#include "Buzzer.h"

void main(){
  unsigned char key_num = 0;
  NixieTube(8,key_num); // 数码管显示初始化
  while(1){
    key_num = PushKey();
    if(key_num){
      NixieTube(8,key_num);
      Buzzer_Time(500);
    }
  }
}
