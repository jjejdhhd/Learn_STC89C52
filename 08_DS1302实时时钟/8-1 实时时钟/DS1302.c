#include <REGX52.H>

// 将端口重新定义
sbit DS1302_SCLK = P3^6;
sbit DS1302_IO   = P3^4;
sbit DS1302_CE   = P3^5;
#define DS1302_RSEC 0x81
#define DS1302_RMIN 0x83
#define DS1302_RHOU 0x85
#define DS1302_RDAT 0x87
#define DS1302_RMON 0x89
#define DS1302_RDAY 0x8B
#define DS1302_RYEA 0x8D
#define DS1302_RWP  0x8F

#define DS1302_WSEC 0x80
#define DS1302_WMIN 0x82
#define DS1302_WHOU 0x84
#define DS1302_WDAT 0x86
#define DS1302_WMON 0x88
#define DS1302_WDAY 0x8A
#define DS1302_WYEA 0x8C
#define DS1302_WWP  0x8E


/**
  * @brief :从DS1302中读出数据
  * @param :需要操作的命令字read_comd。
  * @retval :读出的数据read_byte。
 */
unsigned char DS1302_ReadByte(unsigned char read_comd){
  unsigned char read_byte = 0x00;
  unsigned char i;
  DS1302_CE = 1;
  // 写入命令字
  for(i=0;i<8;i++){
    DS1302_IO = read_comd&(0x01<<i);
    DS1302_SCLK = 0;
    DS1302_SCLK = 1; // 经过测试，不需延时
  }
  // 读出数据
  for(i=0;i<8;i++){
    DS1302_SCLK = 1;
    DS1302_SCLK = 0;
    read_byte = DS1302_IO ? (read_byte|(0x01<<i)) : read_byte;
  }
  DS1302_IO = 0; //注意最后一定要将数据线清零
  DS1302_CE = 0;
  return read_byte;
 }
 
 /**
  * @brief :向DS1302中写入数据
  * @param :需要操作的命令字wri_comd，需要写入的数据wri_byte。
  * @retval :无。
 */
void DS1302_WriteByte(unsigned char wri_comd,wri_byte){
  unsigned char i;
  DS1302_CE = 1;
  // 写入命令字
  for(i=0;i<8;i++){
    DS1302_IO = wri_comd&(0x01<<i);
    DS1302_SCLK = 0;
    DS1302_SCLK = 1; // 经过测试，不需延时
  }
  // 写入8位数据
  for(i=0;i<8;i++){
    DS1302_IO = wri_byte&(0x01<<i);
    DS1302_SCLK = 0;
    DS1302_SCLK = 1; // 经过测试，不需延时
  }
  DS1302_SCLK = 0;
  DS1302_CE = 0;
 }

 /**
  * @brief :初始化控制DS1302的引脚。
  * @param :无。
  * @retval :无。
 */
void DS1302_Init(void){
  DS1302_SCLK = 0;
  DS1302_IO   = 0;
  DS1302_CE   = 0;
  DS1302_WriteByte(DS1302_WWP,0x00);//解除写保护
  DS1302_WriteByte(DS1302_WSEC,0x00);//解除时钟暂停
  // 默认为24小时模式
}

/**
  * @brief :从单片机中读出年/月/日/星期/时/分/秒
  * @param :数组指针，存储年/月/日/星期/时/分/秒。
  * @retval :无。程序中通过指针解引用更改信息。
 */
void DS1302_ReadAll(unsigned char *info){
  unsigned char temp;
  // 年
  temp = DS1302_ReadByte(DS1302_RYEA);
  *info = (temp>>4)*10 + (temp&0x0f);
  // 月
  temp = DS1302_ReadByte(DS1302_RMON);
  *(info+1) = (temp>>4)*10 + (temp&0x0f);
  // 日
  temp = DS1302_ReadByte(DS1302_RDAT);
  *(info+2) = (temp>>4)*10 + (temp&0x0f);
  // 星期
  *(info+3) = DS1302_ReadByte(DS1302_RDAY);
  // 时
  temp = DS1302_ReadByte(DS1302_RHOU);
  if(!(temp&0x80)){*(info+4) = (temp>>4)*10 + (temp&0x0f);}
  else            {*(info+4) = ((temp&0x10)>>4)*10 + (temp&0x0f);}
  // 分
  temp = DS1302_ReadByte(DS1302_RMIN);
  *(info+5) = ((temp&0x70)>>4)*10 + (temp&0x0f);
  // 秒
  temp = DS1302_ReadByte(DS1302_RSEC);
  *(info+6) = ((temp&0x70)>>4)*10 + (temp&0x0f);
}

///**
//  * @brief :向单片机写入年/月/日/时/分/秒
//  * @param :数组指针，存储年/月/日/星期/时/分/秒。
//  * @retval :无。
// */
//void DS1302_SetAll(unsigned char *info){
//  unsigned char temp;
//  // 年
//  temp = DS1302_ReadByte(DS1302_RYEA);
//  *info = (temp>>4)*10 + (temp&0x0f);
//  // 月
//  temp = DS1302_ReadByte(DS1302_RMON);
//  *(info+1) = (temp>>4)*10 + (temp&0x0f);
//  // 日
//  temp = DS1302_ReadByte(DS1302_RDAT);
//  *(info+2) = (temp>>4)*10 + (temp&0x0f);
//  // 星期
//  *(info+3) = DS1302_ReadByte(DS1302_RDAY);
//  // 时
//  temp = DS1302_ReadByte(DS1302_RHOU);
//  if(!(temp&0x80)){*(info+4) = (temp>>4)*10 + (temp&0x0f);}
//  else            {*(info+4) = ((temp&0x10)>>4)*10 + (temp&0x0f);}
//  // 分
//  temp = DS1302_ReadByte(DS1302_RMIN);
//  *(info+5) = ((temp&0x70)>>4)*10 + (temp&0x0f);
//  // 秒
//  temp = DS1302_ReadByte(DS1302_RSEC);
//  *(info+6) = ((temp&0x70)>>4)*10 + (temp&0x0f);
//}