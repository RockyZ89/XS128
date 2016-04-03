#ifndef _PERIPHERALS_H
#define _PERIPHERALS_H

/*--------------------图像宏----------------------------*/
#define ROW     62          //定义图像采集行数：240 / 4 行    当前分辨率 = 定位眼最高分辨率/16
#define COLUMN  82         //定义图像采集列数：320 / 4 列
#define THRESHOLD 240      //固定阈值
#define INTERVAL 4        //有效行采集间隔

extern uchar Image_Data[ROW][COLUMN];    //图像数组
extern uchar RowData[ROW];
extern uchar ColData[COLUMN];
extern uchar VSYN_flg;         //场完成标志
extern uint_16 HREF_C;          //行数统计值
extern uchar Line_C;          //有效行计数






/*-------------------------蜂鸣器------------------------*/
void Buzzer_ms(uint ms);

/*---------------------------发送图像到上位机------------------------*/
void SCI1_Transmit(void);


/*--------------------------OLED-----------------------------*/
#define LED_SCL       PORTB_PB0  
#define LED_SDA       PORTB_PB1
#define LED_RST       PORTB_PB2 
#define LED_DC        PORTB_PB3 
#define LED_CS        PORTB_PB4
#define LED_PORT      DDRB

#define LED_IMAGE_WHITE       1
#define LED_IMAGE_BLACK       0

#define LED_MAX_ROW_NUM      64
#define LED_MAX_COLUMN_NUM  128

void LED_Init(void);
void LED_SetPos(uchar ucIdxX, uchar ucIdxY); 
void LED_Fill(uchar ucData);                                             //全屏填充

void LED_P6x8Char(uchar ucIdxX,uchar ucIdxY,uchar ucData);               //X:0-122 Y:0-7  6*8 ASCLL字符
void LED_P6x8Str(uchar ucIdxX,uchar ucIdxY,uchar ucDataStr[]);           //6*8 字符串
void LED_P8x16Str(uchar ucIdxX,uchar ucIdxY,uchar ucDataStr[]);          //X:0-120 Y:0-3  8*16 ASCLL字符
void LED_PXx16MixStr(uchar ucIdxX, uchar ucIdxY, uchar ucDataStr[]);     //X:0-114 Y:0-3  14*16 混合字符串


void LED_PrintChar(uchar ucIdxX, uchar ucIdxY, char cData);              //X:0-122 Y:0-7  6*8 char三位数带正负输出
void LED_PrintShort(uchar ucIdxX, uchar ucIdxY, int_16 sData);           //X:0-120 Y:0-7  6*8 short五位数带正负输出
void LED_PrintImage(uchar *pucTable, uint_16 usRowNum, uint_16 usColumnNum);  //图像输出





#endif