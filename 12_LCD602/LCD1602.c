#include <REGX52.H>

//下面这些有可能需要更改
/***************************************************************/
// 重新命名端口
sbit LCD1602_RS = P2^6;
sbit LCD1602_RW = P2^5;
sbit LCD1602_EN = P2^7;
#define  LCD1602_DB P0


// LCD1602专用延时函数，晶振11.0592MHz，延时1ms
void LCD1602_Delay1ms(void){//@11.0592MHz
	unsigned char i, j;
	i = 2;
	j = 199;
	do{
		while (--j);
	}while (--i);
}
/***************************************************************/

/***************************************************************/
/**
  * @brief :LCD1602写指令。
  * @param :wByte指令。
  * @retval :无。
 */
void LCD1602_WriteCommd(unsigned char wByte){// LCD1602写指令
  LCD1602_EN = 0;
  LCD1602_RS = 0;
  LCD1602_RW = 0;
  LCD1602_DB = wByte;
  LCD1602_EN = 1;
  LCD1602_Delay1ms();//延时1ms
  LCD1602_EN = 0;
  LCD1602_Delay1ms();//延时1ms
}

/**
  * @brief :LCD1602写数据。
  * @param :wByte数据。
  * @retval :无。
 */
void LCD1602_WriteData(unsigned char wByte){// LCD1602写数据
  LCD1602_EN = 0;
  LCD1602_RS = 1;
  LCD1602_RW = 0;
  LCD1602_DB = wByte;
  LCD1602_EN = 1;
  LCD1602_Delay1ms();//延时1ms
  LCD1602_EN = 0;
  LCD1602_Delay1ms();//延时1ms
}

/**
  * @brief :设置光标位置。
  * @param :Row行，范围1~2。
  * @param :Column列，范围1~40。
  * @retval :无。
 */
void LCD1602_SetCursor(unsigned char Row,unsigned char Column){
    unsigned char LCD_AC=0x00;
    // 更新坐标位置
    if(Row==1){LCD_AC += (Column-1);}
    else      {LCD_AC += (0x40+Column-1);}
    // 设置光标位置
    LCD1602_WriteCommd(0x80|LCD_AC);
}

/**
  * @brief :无符号数的指数运算。
  * @param :x无符号整数基底。
  * @param :y无符号整数幂次。
  * @retval :result无符号整数运算结果。
 */
int LCD1602_Pow(int x, int y){//无符号数的指数运算
  unsigned int i;
  unsigned int result = 1;
  for(i=0;i<y;i++){
    result *= x;
  }
  return result;
}

/**
  * @brief :初始化函数。
  * @param :无。
  * @retval :无。
 */
void LCD1602_Init(void){// 初始化函数
  LCD1602_WriteCommd(0x38);//八位数据接口，两行显示，5*7点阵
  LCD1602_WriteCommd(0x0c);//显示开，光标关，闪烁关
  LCD1602_WriteCommd(0x06);//数据读写操作后，光标自动加一，画面不动
  LCD1602_WriteCommd(0x01);//清屏
}

/**
  * @brief :显示字符
  * @param :Row行，范围1~2。
  * @param :Column列，范围1~40。
  * @param :wChar待显示的字符。
  * @retval :无。
 */
void LCD1602_DispChar(unsigned char Row,unsigned char Column,unsigned char wChar){//显示单个字符
  // 首先保证行列的位置有效，才运行函数
  if((Row==1 || Row==2) && Column>=1 && Column<=40){
    // 更新光标位置
    LCD1602_SetCursor(Row,Column);
    // 发送要显示的数据
    LCD1602_WriteData(wChar);
  }
}

/**
  * @brief :显示字符串
  * @param :Row行，范围1~2。
  * @param :Column列，范围1~40。
  * @param :wString[]待显示的字符串。
  * @retval :无。
 */
void LCD1602_DispString(unsigned char Row,unsigned char Column,char wString[]){//显示字符串
  unsigned char i=0;
  // 首先保证行列的位置有效，才运行函数
  if((Row==1 || Row==2) && Column>=1 && Column<=40){
    // 更新光标位置
    LCD1602_SetCursor(Row,Column);
    // 发送要显示的数据
    while(wString[i]!='\0'){
      LCD1602_WriteData(wString[i]);
      i++;
    }    
  }
}
  
/**
  * @brief :显示无符号数字
  * @param :Row行，范围1~2。
  * @param :Column列，范围1~40。
  * @param :wNum待显示的无符号数字，范围0~65535。
  * @param :Length在屏幕上显示的长度，范围1~16。
  * @retval :无。
 */
void LCD1602_DispUnInt(unsigned char Row, unsigned char Column,
                       unsigned int wNum, unsigned char Length){//显示无符号数字
  unsigned char i=0;
  unsigned char temp=0;//存储单个位上的数据
  // 首先保证行、列、显示长度有效，才运行函数
  if((Row==1 || Row==2) && Column>=1 && Column<=40 && Length>=1){
    // 1. 更新光标位置
    LCD1602_SetCursor(Row,Column);
    // 2.发送要显示的数据
    // 2.1 超过5位的长度都显示0
    if(Length>16){Length=16;}
    while(Length>5){
      LCD1602_WriteData(0x30);//直接发送'0'的ASCII码
      Length--;
    }
    // 2.2 5位以内的真实数值
    for(i=0;i<Length;i++){
      temp = (wNum/LCD1602_Pow(10,Length-1-i))%10;
      LCD1602_WriteData(0x30|temp);//直接计算ASCII码
    } 
  }
}

/**
  * @brief :显示有符号数字
  * @param :Row行，范围1~2。
  * @param :Column列，范围1~40。
  * @param :wNum待显示的有符号数字，范围-32768~32767。
  * @param :Length在屏幕上显示的数字长度（包括正负号），范围1~16。
  * @retval :无。
 */
void LCD1602_DispInt(unsigned char Row, unsigned char Column,
                     int wNum, unsigned char Length){//显示有符号数字
  unsigned char i=0;
  unsigned char temp=0;//存储单个位上的数据
  unsigned int un_wNum=0;
  // 首先保证行、列、显示长度有效，才运行函数
  if((Row==1 || Row==2) && Column>=1 && Column<=40 && Length>=2){
    // 1. 更新光标位置
    LCD1602_SetCursor(Row,Column);
    // 2. 发送要显示的数据
    // 2.1 显示正负号，并将原来的数字转换成正数
    if(Length>16){Length=16;}
    if(wNum>=0){LCD1602_WriteData('+'); un_wNum=wNum;}
    else       {LCD1602_WriteData('-'); un_wNum=-wNum;}
    Length--;
    // 2.2 超过5位的长度都显示0
    while(Length>5){
      LCD1602_WriteData(0x30);//直接发送'0'的ASCII码
      Length--;
    }
    // 2.3 5位以内的真实数值
    for(i=0;i<Length;i++){
      temp = (un_wNum/LCD1602_Pow(10,Length-1-i))%10;
      LCD1602_WriteData(0x30|temp);//直接计算ASCII码
    } 
  }
}

/**
  * @brief :显示无符号整型的16进制表示。
  * @param :Row行，范围1~2。
  * @param :Column列，范围1~40。
  * @param :wNum待显示的无符号数字，范围0~65535。
  * @param :Length在屏幕上显示的16进制长度，范围1~4。
  * @retval :无。
 */
void LCD1602_DispUnInt_Hex(unsigned char Row, unsigned char Column,
                           unsigned int wNum, unsigned char Length){//显示无符号整型的16进制表示
  unsigned char i=0;
  unsigned char temp=0;//存储单个位上的数据
  // 首先保证行、列、显示长度有效，才运行函数
  if((Row==1 || Row==2) && Column>=1 && Column<=40&& Length>=1){
    // 1. 更新光标位置
    LCD1602_SetCursor(Row,Column);
    // 2. 发送要显示的数据
    if(Length>4){Length=4;}
    else        {wNum = (wNum<<(4*(4-Length)))>>(4*(4-Length));}//根据要显示的长度截取有用的位
    for(i=0;i<Length;i++){
      temp = (wNum>>((Length-1-i)*4))%16;
      if(temp>=0 && temp<=9){LCD1602_WriteData('0' + temp);}
      else                  {LCD1602_WriteData('A' + temp - 10);}
    } 
  }
}

/**
  * @brief :显示无符号整型的2进制表示。
  * @param :Row行，范围1~2。
  * @param :Column列，范围1~40。
  * @param :wNum待显示的无符号数字，范围0~65535。
  * @param :Length在屏幕上显示的2进制长度，范围1~16。
  * @retval :无。
 */
void LCD1602_DispUnInt_Bin(unsigned char Row, unsigned char Column,
                           unsigned int wNum, unsigned char Length){//显示无符号整型的2进制表示
  unsigned char i=0;
  // 首先保证行、列、显示长度有效，才运行函数
  if((Row==1 || Row==2) && Column>=1 && Column<=40&& Length>=1){
    // 1. 更新光标位置
    LCD1602_SetCursor(Row,Column);
    // 2. 发送要显示的数据
    if(Length>16){Length=16;}
    for(i=0;i<Length;i++){
      if(wNum&(0x0001<<(Length-1-i))) {LCD1602_WriteData('1');}
      else                            {LCD1602_WriteData('0');}
    } 
  }
}

/**
  * @brief :显示单精度浮点数（有符号）。
  * @param :Row行，范围1~2。
  * @param :Column列，范围1~40。
  * @param :wNum待显示的小数，范围-3.4E38～3.4E38。
  * @param :Total_Len在屏幕上显示的总长度长度（包括正负号），范围3~16。
  * @param :Frac_len在屏幕上显示的小数部分的长度，范围1~16。
  * @retval :无。
  * 说明：优先显示小数部分。
 */
void LCD1602_DispFloat(unsigned char Row, unsigned char Column,
                       float wNum, unsigned char Total_Len,
                       unsigned char Frac_len){//显示单精度浮点数（有符号）
  unsigned char Int_len = Total_Len-2-Frac_len; //整数段长度
  unsigned int Int_part=0, Fra_part=0;
  // 首先保证行、列、显示长度有效，才运行函数
  if((Row==1 || Row==2) && Column>=1 && Column<=40 && Total_Len>=3){
    //1. 显示正负号，并将原来的数字转化成绝对值
    if(wNum>=0){LCD1602_DispChar(Row,Column,'+');}
    else       {LCD1602_DispChar(Row,Column,'-'); wNum=-wNum;}
    //2. 获取整数部分、小数部分
    Int_part=(unsigned int)wNum;
    Fra_part = ((unsigned int)(wNum*LCD1602_Pow(10,Frac_len)))%LCD1602_Pow(10,Frac_len);
    //3. 显示整数部分
    LCD1602_DispUnInt(Row,Column+1,Int_part,Int_len);
    //4. 显示小数点
    LCD1602_DispChar(Row,Column+1+Int_len,'.');
    //5. 显示小数部分
    LCD1602_DispUnInt(Row,Column+2+Int_len,Fra_part,Frac_len);

  }
}