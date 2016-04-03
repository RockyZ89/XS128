#ifndef _PERIPHERALS_H
#define _PERIPHERALS_H

/*--------------------ͼ���----------------------------*/
#define ROW     62          //����ͼ��ɼ�������240 / 4 ��    ��ǰ�ֱ��� = ��λ����߷ֱ���/16
#define COLUMN  82         //����ͼ��ɼ�������320 / 4 ��
#define THRESHOLD 240      //�̶���ֵ
#define INTERVAL 4        //��Ч�вɼ����

extern uchar Image_Data[ROW][COLUMN];    //ͼ������
extern uchar RowData[ROW];
extern uchar ColData[COLUMN];
extern uchar VSYN_flg;         //����ɱ�־
extern uint_16 HREF_C;          //����ͳ��ֵ
extern uchar Line_C;          //��Ч�м���






/*-------------------------������------------------------*/
void Buzzer_ms(uint ms);

/*---------------------------����ͼ����λ��------------------------*/
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
void LED_Fill(uchar ucData);                                             //ȫ�����

void LED_P6x8Char(uchar ucIdxX,uchar ucIdxY,uchar ucData);               //X:0-122 Y:0-7  6*8 ASCLL�ַ�
void LED_P6x8Str(uchar ucIdxX,uchar ucIdxY,uchar ucDataStr[]);           //6*8 �ַ���
void LED_P8x16Str(uchar ucIdxX,uchar ucIdxY,uchar ucDataStr[]);          //X:0-120 Y:0-3  8*16 ASCLL�ַ�
void LED_PXx16MixStr(uchar ucIdxX, uchar ucIdxY, uchar ucDataStr[]);     //X:0-114 Y:0-3  14*16 ����ַ���


void LED_PrintChar(uchar ucIdxX, uchar ucIdxY, char cData);              //X:0-122 Y:0-7  6*8 char��λ�����������
void LED_PrintShort(uchar ucIdxX, uchar ucIdxY, int_16 sData);           //X:0-120 Y:0-7  6*8 short��λ�����������
void LED_PrintImage(uchar *pucTable, uint_16 usRowNum, uint_16 usColumnNum);  //ͼ�����





#endif