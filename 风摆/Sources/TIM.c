void init_TIM_IC(){
  TSCR2 = 0X06;  //�ر�����ж���64��Ƶ
  TIOS_IOS0 = 0;  //��ͨ��0Ϊ���벶׽��1Ϊ����Ƚ�
  TCTL4 = 0X01;   //�����ز�׽��0x02Ϊ�½��ز�׽
  TIE_C0I = 1;   //ͨ��0�ж�ʹ��
  TSCR1 = 0X80;   //�����ܶ�ʱ���ж�
  TFLG1_C0F = 1;   //��־λ����
}
#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt 8 TIM(void){
  TFLG1_C0F = 1;//��־λ��0
  TSCR1 = 0X80; //�����ж�
}
#pragma CODE_SEG DEFAULT