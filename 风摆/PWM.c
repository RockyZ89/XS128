void init_PWM(){
  PWME = 0x00;          //��ֹPWMģ��
  
  PWMPRCLK = 0x01;      //CLOCKA 2��Ƶ
  PWMSCLA = 2;          //SAΪA��4��Ƶ
   
  PWMCLK_PCLK0 = 1;    //ѡ��SA��ΪPWMʱ��
  PWMCLK_PCLK1 = 1;  
  
  
  PWMPER0 = 200;       //ͨ��0���ڼĴ�������Ϊ200
  PWMPER1 = 200;       //ͨ��1���ڼĴ�������Ϊ200
  
  PWMDTY0 = 100;         //ͨ��0ռ�ձȼĴ�������
  PWMDTY1 = 100;         //ͨ��1ռ�ձȼĴ�������
  
  PWMPOL_PPOL0 = 1;     //PWM����ѡ���ȸߺ��
  PWMPOL_PPOL1 = 1; 
  
  PWMCAE = 0x00;        //��������
  PWMCTL = 0x00;        //������
  
  PWME_PWME0 = 1;
  PWME_PWME1 = 1; 
}
