
#include "Headfile.h"
#include "myiic.h"
#include "mpu6050.h"
Butter_Parameter Ins_Accel_Parameter;
Butter_Parameter Accel_Parameter;
Butter_Parameter Gyro_Parameter;
Butter_Parameter Calibrate_Parameter;
/***********************************************************
@函数名：Init_MPU6050
@入口参数：无
@出口参数：无
功能描述：MPU6050初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Init_MPU6050(void)//MPU6050初始化
{
  i2cWrite(MPU_ADRESS,PWR_MGMT_1  , 0x00);//关闭所有中断,解除休眠
  i2cWrite(MPU_ADRESS,SMPLRT_DIV  , 0x00); // sample rate.  Fsample= 1Khz/(<this value>+1) = 1000Hz
  //i2cWrite(MPU_ADRESS,MPU_CONFIG  , 0x02); //内部低通滤波频率，加速度计94hz，陀螺仪98hz
  i2cWrite(MPU_ADRESS,MPU_CONFIG  , 0x03);//内部低通滤波频率，加速度计44hz，陀螺仪42hz
  i2cWrite(MPU_ADRESS,GYRO_CONFIG , 0x10);//1000deg/s
  i2cWrite(MPU_ADRESS,ACCEL_CONFIG, 0x18);// Accel scale 16g (2048 LSB/g)
  delay_ms(500);
  IMU_Calibration();
  Set_Cutoff_Frequency(Imu_Sampling_Freq, 50,&Gyro_Parameter);//姿态角速度反馈滤波参数  50
  Set_Cutoff_Frequency(Imu_Sampling_Freq, 30,&Accel_Parameter);//姿态解算加计修正滤波值 30
  Set_Cutoff_Frequency(Imu_Sampling_Freq, 2,&Calibrate_Parameter);//传感器校准加计滤波值
  Set_Cutoff_Frequency(Imu_Sampling_Freq, 60,&Ins_Accel_Parameter);//INS加计滤波值
}

/***********************************************************
@函数名：MPU6050_Read_Data
@入口参数：vector3f *gyro,vector3f *accel
@出口参数：无
功能描述：MPU6050数据采集
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void MPU6050_Read_Data(vector3f *gyro,vector3f *accel)//读取MPU6050数据
{
  accel->x=Double_ReadI2C(MPU_ADRESS,ACCEL_XOUT_H);
  accel->y=Double_ReadI2C(MPU_ADRESS,ACCEL_YOUT_H);
  accel->z=Double_ReadI2C(MPU_ADRESS,ACCEL_ZOUT_H);
	
  gyro->x=Double_ReadI2C(MPU_ADRESS,GYRO_XOUT_H);
  gyro->y=Double_ReadI2C(MPU_ADRESS,GYRO_YOUT_H);
  gyro->z=Double_ReadI2C(MPU_ADRESS,GYRO_ZOUT_H);
}


/***********************************************************
@MPU6050_Read_Temperature
@入口参数：float *temperature
@出口参数：无
功能描述：MPU6050温度数据采集
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void MPU6050_Read_Temperature(float *temperature)
{
  *temperature=36.53+((double)Double_ReadI2C(MPU_ADRESS,TEMP_OUT_H))/340;
}
Vector3f gyro_offset;
s32 g_Gyro_xoffset = 0, g_Gyro_yoffset = 0, g_Gyro_zoffset = 0;
/***********************************************************
@函数名：IMU_Calibration
@入口参数：无
@出口参数：无
功能描述：陀螺仪开机零偏标定
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void IMU_Calibration(void)
{
  u8 i;
  delay_ms(500);
  for (i = 0; i < 100; i++)			//连续采样30次，一共耗时30*3=90ms
  {
    g_Gyro_xoffset +=Double_ReadI2C(MPU_ADRESS,GYRO_XOUT_H);
    g_Gyro_yoffset +=Double_ReadI2C(MPU_ADRESS,GYRO_YOUT_H);
    g_Gyro_zoffset +=Double_ReadI2C(MPU_ADRESS,GYRO_ZOUT_H);
    delay_ms(10);
  }
  gyro_offset.x =(g_Gyro_xoffset/100);//得到标定偏移
  gyro_offset.y =(g_Gyro_yoffset/100);
  gyro_offset.z =(g_Gyro_zoffset/100);
}

