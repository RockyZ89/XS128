


void init_PIT(){
  PITCFLMT = 0x00;//��ֹPITģ��
  PITCE_PCE0 = 1; //ʹ�ܶ�ʱ��ͨ��0
  PITMUX = 0x00;  //��ʱ��ͨ��0ʹ�� ΢��ʱ��׼0
  PITMTLD0 = 0xff;//����8λ��������ֵ
  PITLD0 = 0x7A11;//����16λ��������ֵ  ��ʱ���� 1s
  PITINTE = 0x01; //ʹ��PIT��ʱ��ͨ��0�ж�
  PITCFLMT = 0x80;//ʹ��PITģ��
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void){
  PITTF_PTF0 = 1;//����жϱ�־
  sec++;
  if(sec>=100) sec=0; 
  draw_curve_12864(now_T);     //��ʵʱ�¶�����ͼ
  write_num_12864(sec,0x86);   //12864��ʾ����ʱ��RT
  write_str_12864(nt,0x92,5);  //12864��ʾʵʱ�¶�
}
#pragma CODE_SEG DEFAULT
