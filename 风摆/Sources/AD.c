void init_AD(){
  ATD0DIEN = 0x00;//禁止数字输入功能
  ATD0CTL1 = 0x20;//AD分辨率选择10位，且采样前不放电
  ATD0CTL2 = 0x40;//AD模块快速清零，禁止外部触发，禁止中断
  ATD0CTL3 = 0x88;//AD转换结果右对齐，每个序列1个通道，非FIFO模式
  ATD0CTL4 = 0x67;//采样时间4个周期，PRS=3
  ATD0CTL5 = 0x20;//对通道0单通道连续采样，同时启动AD转换序列 
}