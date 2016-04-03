#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "mpu6050.h"
#include "delay.h"
#include "12864.h"

 int GYRO_OFFSET_OK= 1;   //校准MPU6050角速度标志位
 int ACC_OFFSET_OK = 1;	  //校准MPU6050加速度标志位
//extern Three	MPU6050_ACC_LAST,MPU6050_GYRO_LAST;		//最新一次读取值
//extern Three GYRO_OFFSET={0,0,0},ACC_OFFSET={0,0,0};			//零漂

extern int anglex,angley,anglez;
extern int gyrox,gyroy,gyroz;
extern int Eranglex,Erangley,Eranglez;
extern int Ergyrox,Ergyroy,Ergyroz;

extern int angleX,angleY;  
extern int angrateX,angrateY;   

uchar agX[4],agY[4];
uchar arX[4],arY[4];                         
static unsigned char BUF[16];       //接收数据缓存区
unsigned char INIT[6] = {0X00,0X07,0X06,0X18,0X18};



void MPU6050_Init(void)
{
  i2c_write_component(MPU6050_Addr,PWR_MGMT_1,1,&INIT[0]);	//解除休眠状态
	i2c_write_component(MPU6050_Addr,SMPLRT_DIV,1,&INIT[1]);
	i2c_write_component(MPU6050_Addr,CONFIG,1,&INIT[2]);
	i2c_write_component(MPU6050_Addr,GYRO_CONFIG,1,&INIT[3]); //陀螺仪范围+-2000/s
	i2c_write_component(MPU6050_Addr,ACCEL_CONFIG,1,&INIT[4]); //加速度范围+-16g
}
//******读取MPU6050数据****************************************
void READ_MPU6050(void)
{
   i2c_read_component(MPU6050_Addr,GYRO_XOUT_L,1,&BUF[0]);  	//角速度
   i2c_read_component(MPU6050_Addr,GYRO_XOUT_H,1,&BUF[1]);
   gyrox=	((BUF[1]<<8)|BUF[0])-Ergyrox;
   //MPU6050_GYRO_LAST.X=MPU6050_GYRO_LAST.X/16.4//-0xfffe; 						   //读取计算X轴数据

   i2c_read_component(MPU6050_Addr,GYRO_YOUT_L,1,&BUF[2]);
   i2c_read_component(MPU6050_Addr,GYRO_YOUT_H,1,&BUF[3]);
   gyroy=	((BUF[3]<<8)|BUF[2])-Ergyroy;
   //MPU6050_GYRO_LAST.Y=MPU6050_GYRO_LAST.Y/16.4; 						   //读取计算Y轴数据
   
   i2c_read_component(MPU6050_Addr,GYRO_ZOUT_L,1,&BUF[4]);
   i2c_read_component(MPU6050_Addr,GYRO_ZOUT_H,1,&BUF[5]);
   gyroz=	((BUF[5]<<8)|BUF[4])-Ergyroz;
   // MPU6050_GYRO_LAST.Z=MPU6050_GYRO_LAST.Z/16.4; 					       //读取计算Z轴数据

   i2c_read_component(MPU6050_Addr,ACCEL_XOUT_L,1,&BUF[8]);
   i2c_read_component(MPU6050_Addr,ACCEL_XOUT_H,1,&BUF[9]);
   anglex=	((BUF[9]<<8)|BUF[8])-Eranglex;//-0x10;
   // MPU6050_ACC_LAST.X=(float)((float)MPU6050_ACC_LAST.X/2048)*100; 		//扩大100倍	       //读取计算X轴数据

   i2c_read_component(MPU6050_Addr,ACCEL_YOUT_L,1,&BUF[10]);
   i2c_read_component(MPU6050_Addr,ACCEL_YOUT_H,1,&BUF[11]);
   angley=	((BUF[11]<<8)|BUF[10])-Erangley;//-0x0f;
   // MPU6050_ACC_LAST.Y=(float)((float)MPU6050_ACC_LAST.Y/2048)*100; 						   //读取计算Y轴数据

   i2c_read_component(MPU6050_Addr,ACCEL_ZOUT_L,1,&BUF[12]);
   i2c_read_component(MPU6050_Addr,ACCEL_ZOUT_H,1,&BUF[13]);
   anglez=	((BUF[13]<<8)|BUF[12])-Eranglez;
   // MPU6050_ACC_LAST.Z=(float)((float)MPU6050_ACC_LAST.Z/2048)*100; 					       //读取计算Z轴数据

   if(!GYRO_OFFSET_OK)		//初始化GYRO_OFFSET_OK=1
	  {
		static int	tempgx=0,tempgy=0,tempgz=0;
		static char cnt_g=0;
		if(cnt_g==0)
		{
		  Ergyrox=0;
			Ergyroy=0;
			Ergyroz=0;
			tempgx = 0;
			tempgy = 0;
			tempgz = 0;
			cnt_g = 1;
			return;
		}
		tempgx+= gyrox;
		tempgy+= gyroy;
		tempgz+= gyroz;
		if(cnt_g==100)
		{
			Ergyrox=tempgx/cnt_g;
			Ergyroy=tempgy/cnt_g;
			Ergyroz=tempgz/cnt_g;
			cnt_g = 0;
			GYRO_OFFSET_OK = 1;
			return;
		}
		cnt_g++;
	}
	if(!ACC_OFFSET_OK)		 //初始化ACC_OFFSET_OK=1
	{
		static int	tempax=0,tempay=0,tempaz=0;
		static char cnt_a=0;
		if(cnt_a==0)
		{
			Eranglex = 0;
			Erangley = 0;
			Eranglez = 0;
			tempax = 0;
			tempay = 0;
			tempaz = 0;
			cnt_a = 1;
			return;
		}
		tempax+= anglex;
		tempay+= angley;
		//tempaz+= MPU6050_ACC_LAST.Z;
		if(cnt_a==200)
		{
			Eranglex=tempax/cnt_a;
			Erangley=tempay/cnt_a;
			Eranglez=tempaz/cnt_a;
			cnt_a = 0;
			ACC_OFFSET_OK = 1;
			return;
		}
		cnt_a++;		
	}
	angrateX = gyroy / 16.4 *10;
	angrateY = gyrox / 16.4 *10;
	angleX = anglex/2000.0*90 *10;
	angleY = angley/2000.0*90 *10;
	
	write_ang_12864(angleX,0x88);
	write_ang_12864(angleY,0x88+4);
	write_ang_12864(angrateX,0x98);
  write_ang_12864(angrateY,0x98+4);   
	
}



