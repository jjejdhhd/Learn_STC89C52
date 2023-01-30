#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "PushKey.h"
#include "Timer0.h"

// 全局变量：存储年/月/日/星期/时/分/秒/星期偏置
unsigned char DS1302_info[8] = {23,1,22,0,23,59,55,7};
// 全局变量：闪烁标志信号
unsigned char flag_twinkle=0x00;

/**
  * @brief :RTC显示。
  * @param :数组指针，存储年/月/日/星期/时/分/秒/星期偏置。
  * @retval :无。
 */
void RTC_Display(unsigned char *info){
  LCD_ShowNum(1,3,info[0],2);
  LCD_ShowNum(1,6,info[1],2);
  LCD_ShowNum(1,9,info[2],2);
  LCD_ShowDay(1,14,info[3]);
  LCD_ShowNum(2,1,info[4],2);
  LCD_ShowNum(2,4,info[5],2);
  LCD_ShowNum(2,7,info[6],2);
}

/**
  * @brief :进入修改模式，用户输入日期数据，保证其合理性并更新。
  * @param :数组指针，存储年/月/日/星期/时/分/秒/星期偏置。
  * @retval :无。
 */
void RTC_Modify(unsigned char *info){
  unsigned char MODE = 1;//指示当前是否处于修改模式
  unsigned char key = 0;//按键开关
  unsigned char cursor = 1;//指示要修改第几个元素，范围1~7（4实际更改info的第8个元素）
  while(MODE==1){
    key = PushKey();
    if(key==1){
      // 退出修改模式
      MODE = 0;
      DS1302_SetAll(info);//更新数据
    }else if(key==2){
      //移动修改光标
      cursor = (cursor>=7) ? 1 : (cursor+1);
//      LCD_ShowNum(2,14,cursor,3);//仅供测试使用！！！
    }else if(key==3){
      //对当前光标指定值进行加计数
      switch(cursor){
        case 1: //年
          *info = (*info>=99) ? 0 : (*info)+1;
          break;
        
        case 2: //月
          *(info+1) = (*(info+1)>=12) ? 1 : (*(info+1))+1;
          break;
        
        case 3: //日
          //31天的月份
          if(*(info+1)==1 || *(info+1)==3 || *(info+1)==5 || *(info+1)==7 ||
             *(info+1)==8 || *(info+1)==10|| *(info+1)==12){
             *(info+2)=(*(info+2)>=31)?1:(*(info+2))+1;
          }//30天的月份
          else if(*(info+1)==4 || *(info+1)==6 || *(info+1)==9 || *(info+1)==11){
            *(info+2)=(*(info+2)>=30)?1:(*(info+2))+1;
          }//2月需要判断闰月
          else if(*(info+1)==2){
            if(*info%4==0){*(info+2)=(*(info+2)>=29)?1:(*(info+2))+1;}
            else          {*(info+2)=(*(info+2)>=28)?1:(*(info+2))+1;}
          }
          break;
          
        case 4://星期
          *(info+7) = (*(info+7)>=7) ? 1 : (*(info+7))+1;//更新偏置
          *(info+3) = (*(info+3)>=7) ? 1 : (*(info+3))+1;//更新显示
          break;
        
        case 5://时
          *(info+4) = (*(info+4)>=23) ? 0 : (*(info+4))+1;
          break;
        
        case 6://分
          *(info+5) = (*(info+5)>=59) ? 0 : (*(info+5))+1;
          break;
        
        case 7://秒
          *(info+6) = (*(info+6)>=59) ? 0 : (*(info+6))+1;
          break;
        
        default:;
      }
    }else if(key==4){
      //对当前光标指定值进行减计数
      switch(cursor){
        case 1: //年
          *info = (*info==0) ? 99 : (*info)-1;
          break;
        
        case 2: //月
          *(info+1) = (*(info+1)==1) ? 12 : (*(info+1))-1;
          break;
        
        case 3: //日
          //31天的月份
          if(*(info+1)==1 || *(info+1)==3 || *(info+1)==5 || *(info+1)==7 ||
             *(info+1)==8 || *(info+1)==10|| *(info+1)==12){
             *(info+2)=(*(info+2)==1)?31:(*(info+2))-1;
          }//30天的月份
          else if(*(info+1)==4 || *(info+1)==6 || *(info+1)==9 || *(info+1)==11){
            *(info+2)=(*(info+2)==1)?30:(*(info+2))-1;
          }//2月需要判断闰月
          else if(*(info+1)==2){
            if(*info%4==0){*(info+2)=(*(info+2)==1)?29:(*(info+2))-1;}
            else          {*(info+2)=(*(info+2)==1)?28:(*(info+2))-1;}
          }
          break;
          
        case 4://星期
          *(info+7) = (*(info+7)==1) ? 7 : (*(info+7))-1;//更新偏置（实际起作用）
          *(info+3) = (*(info+3)==1) ? 7 : (*(info+3))-1;//更新显示
          break;
        
        case 5://时
          *(info+4) = (*(info+4)==0) ? 23 : (*(info+4))-1;
          break;
        
        case 6://分
          *(info+5) = (*(info+5)==0) ? 59 : (*(info+5))-1;
          break;
        
        case 7://秒
          *(info+6) = (*(info+6)==0) ? 59 : (*(info+6))-1;
          break;
        
        default:;    
      }
    }
    //更新当前的显示
    if(!flag_twinkle){
      LCD_ShowNum(1,3,*info,2);
      LCD_ShowNum(1,6,*(info+1),2);
      LCD_ShowNum(1,9,*(info+2),2);
      LCD_ShowDay(1,14,*(info+3));
      LCD_ShowNum(2,1,*(info+4),2);
      LCD_ShowNum(2,4,*(info+5),2);
      LCD_ShowNum(2,7,*(info+6),2);
    }else{
      switch(cursor){
        case 1: LCD_ShowString(1,3,"  "); break;
        case 2: LCD_ShowString(1,6,"  "); break;
        case 3: LCD_ShowString(1,9,"  "); break;
        case 4: LCD_ShowString(1,14,"   "); break;
        case 5: LCD_ShowString(2,1,"  "); break;
        case 6: LCD_ShowString(2,4,"  "); break;
        case 7: LCD_ShowString(2,7,"  "); break;
        default:;
      }
    }
  }
}


void main(){
//  unsigned char i=0;
  unsigned char key = 0; //指示按下了哪个按键开关
  //初始化LCD显示屏
  LCD_Init();
  LCD_ShowString(1,1,"2023-01-22");
  LCD_ShowString(2,1,"00:00:00");
  //初始化DS1302
  DS1302_Init();
  DS1302_SetAll(&DS1302_info);
  //初始化定时器T0
  Timer0_Init();
  while(1){
    key = PushKey();
    if(key==1){
    //进入修改模式
      RTC_Modify(&DS1302_info);
    }
    else{
    //正常读取并显示时钟
      DS1302_ReadAll(&DS1302_info);
      RTC_Display(&DS1302_info);
//      LCD_ShowNum(2,11,*(DS1302_info+7),3);//仅供测试使用！！！
    }
  }
}

// 定义定时器T0中断后要执行的动作
void Timer0_Routine() interrupt 1{
  static unsigned int count_T0; //中断次数
  count_T0++; //更新中断次数
  TH0 = 0xfc; TL0 = 0x66; // 恢复溢出周期，近似1ms
  if(count_T0>400){
    count_T0 = 0;
    flag_twinkle = !flag_twinkle;
  }  
}
