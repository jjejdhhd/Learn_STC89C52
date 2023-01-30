#include <REGX52.H>

/*随开发板的不同，此段代码中的端口定义会有所变化，使用前注意更新*/
/**********************************************************/
// LED点阵通用，延时cycles ms，晶振@11.0592MHz
static void Delay(unsigned int cycles){
  unsigned char i, j;
  do{
    i = 2;
    j = 199;
    do{
      while (--j);
    }while (--i);
  }while(--cycles);
}

// 为了方便，先给几个引脚重新起名
sbit RCK = P3^5; // 重复了所以不叫RCLK
sbit SCK = P3^6; // SRCLK
sbit SER = P3^4;
#define MATRIX_LED_PORT P0
/*********************************************************/

/*下面的代码无需更新*/
/*********************************************************/
/**
  * @brief :向74HC595串行输入口输入8bit数据。
  * @param :输入的8bit数据，高位先进。
  * @retval :无。
 */
void _74HC595_WriteByte(unsigned char Byte){
  unsigned char i;
  for(i=0;i<8;i++){
    SER = Byte&(0x80>>i); // 右边只要不是0，1bit宽度的SER就为1
    SCK = 1;
    SCK = 0;
  }
  RCK = 1;
  RCK = 0;
}

/**
  * @brief :在LED点阵的指定列，输入数据。
  * @param :Column，列位置，范围0~7。
  * @param :Data，该列需要显示的数据。
  * @retval :无。
 */
void MatrixLED_ShowColumn(unsigned char Column,Data){
  _74HC595_WriteByte(Data); // 段选信号
  MATRIX_LED_PORT = ~(0x80>>Column); // 片选信号
  Delay(1);
  MATRIX_LED_PORT = 0xff; // 消影
}
/*********************************************************/