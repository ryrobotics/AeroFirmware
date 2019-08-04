/* Copyright (c)  2018-2025 Wuhan Nameless Innovation Technology Co.,Ltd. All rights reserved.*/
/*----------------------------------------------------------------------------------------------------------------------/
*               本程序只供购买者学习使用，版权著作权属于无名科创团队，无名科创团队将飞控程序源码提供给购买者，
*               购买者要为无名科创团队提供保护，未经作者许可，不得将源代码提供给他人，不得将源代码放到网上供他人免费下载， 
*               更不能以此销售牟利，如发现上述行为，无名科创团队将诉之以法律解决！！！
-----------------------------------------------------------------------------------------------------------------------/
*               生命不息、奋斗不止；前人栽树，后人乘凉！！！
*               开源不易，且学且珍惜，祝早日逆袭、进阶成功！！！
*               学习优秀者，简历可推荐到DJI、ZEROTECH、XAG、AEE、GDU、AUTEL、EWATT、HIGH GREAT等公司就业
*               求职简历请发送：15671678205@163.com，需备注求职意向单位、岗位、待遇等
*               无名科创开源飞控QQ群：540707961
*               CSDN博客：http://blog.csdn.net/u011992534
*               优酷ID：NamelessCotrun无名小哥
*               B站教学视频：https://space.bilibili.com/67803559/#/video
*               客户使用心得、改进意见征集贴：http://www.openedv.com/forum.php?mod=viewthread&tid=234214&extra=page=1
*               淘宝店铺：https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
*               百度贴吧:无名科创开源飞控
*               公司官网:www.nameless.tech
*               修改日期:2019/4/12
*               版本：躺赢者——CarryPilot_V1.0
*               版权所有，盗版必究。
*               Copyright(C) 2017-2025 武汉无名创新科技有限公司 
*               All rights reserved
*               重要提示：
*               正常淘宝咸鱼转手的飞控、赠送朋友、传给学弟的都可以进售后群学习交流，
*               不得直接在网上销售无名创新资料，无名创新代码有声明版权，他人不得将
*               资料代码传网上供他人下载，不得以谋利为目的销售资料代码，发现有此操
*               作者，公司会提前告知，请1天内及时处理，否则你的学校、单位、姓名、电
*               话、地址信息会被贴出在公司官网、官方微信公众平台、官方技术博客、知乎
*               专栏以及淘宝店铺首页予以公示公告，此种所作所为，会成为个人污点，影响
*               升学、找工作、社会声誉、很快就很在无人机界出名，后果很严重。
*               因此行为给公司造成重大损失者，会以法律途径解决，感谢您的合作，谢谢！！！
----------------------------------------------------------------------------------------------------------------------*/
#include "Headfile.h"
#include "myiic.h"
#include "mpu6050.h"
Butter_Parameter Ins_Accel_Parameter;
Butter_Parameter Accel_Parameter;
Butter_Parameter Gyro_Parameter;
Butter_Parameter Calibrate_Parameter;
uint8_t IMU_ID=0x68; 
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
	IMU_ID=Single_ReadI2C(MPU_ADRESS,WHO_AM_I);
  if(IMU_ID==IMU_MPU6050_ID)//MPU6050
	{
		i2cWrite(MPU_ADRESS,PWR_MGMT_1  , 0x00);//关闭所有中断,解除休眠
		i2cWrite(MPU_ADRESS,SMPLRT_DIV  , 0x00); // sample rate.  Fsample= 1Khz/(<this value>+1) = 1000Hz
		//i2cWrite(MPU_ADRESS,MPU_CONFIG  , 0x02); //内部低通滤波频率，加速度计94hz，陀螺仪98hz
		i2cWrite(MPU_ADRESS,MPU_CONFIG  , 0x03);//内部低通滤波频率，加速度计44hz，陀螺仪42hz
		i2cWrite(MPU_ADRESS,GYRO_CONFIG , 0x10);//1000deg/s
		i2cWrite(MPU_ADRESS,ACCEL_CONFIG, 0x18);// Accel scale 16g (2048 LSB/g)
	}
	else if(IMU_ID==IMU_ICM20689_ID)//ICM20689
	{	
		i2cWrite(MPU_ADRESS,PWR_MGMT_1  , 0x00);//关闭所有中断,解除休眠
		i2cWrite(MPU_ADRESS,SMPLRT_DIV  , 0x00); // sample rate.  Fsample= 1Khz/(<this value>+1) = 1000Hz	
		i2cWrite(MPU_ADRESS,MPU_CONFIG  , 0x03);//设置陀螺仪、温度内部低通滤波频率范围，陀螺仪41hz，噪声带宽59hz，温度42hz	
		i2cWrite(MPU_ADRESS,GYRO_CONFIG , 0x10);//设置陀螺仪量程，1000deg/s
		i2cWrite(MPU_ADRESS,ACCEL_CONFIG, 0x18);// Accel scale 16g (2048 LSB/g)	
		i2cWrite(MPU_ADRESS,ACCEL_CONFIG2,0x03);//设置加速度计内部低通滤波频率范围，加速度44.8hz，噪声带宽61.5hz
	}
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
  accel->y=-Double_ReadI2C(MPU_ADRESS,ACCEL_YOUT_H);
  accel->z=-Double_ReadI2C(MPU_ADRESS,ACCEL_ZOUT_H);
	
  gyro->x=Double_ReadI2C(MPU_ADRESS,GYRO_XOUT_H);
  gyro->y=-Double_ReadI2C(MPU_ADRESS,GYRO_YOUT_H);
  gyro->z=-Double_ReadI2C(MPU_ADRESS,GYRO_ZOUT_H);
	
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
	if(IMU_ID==IMU_MPU6050_ID)
	{
		//MPU6050
		//Sensitivity=340 LSB/°C
		//Temperature Offset=35 °C
		*temperature=36.53+((double)Double_ReadI2C(MPU_ADRESS,TEMP_OUT_H))/340;
	}
	if(IMU_ID==IMU_ICM20689_ID)
	{
		//ICM20689
		//Room Temperature Offset=25 °C
		//Sensitivity=326.8 LSB/°C
		//TEMP_degC = ((TEMP_OUT –RoomTemp_Offset)/Temp_Sensitivity) +25degC
		*temperature=25.0f+((double)Double_ReadI2C(MPU_ADRESS,TEMP_OUT_H)-25.0f)/326.8;
	}
	
  //ICM20602
	//Room Temperature Offset=25 °C
	//Sensitivity=326.8 LSB/°C
	//TEMP_degC =(TEMP_OUT)/Temp_Sensitivity +25degC
	//*temperature=25.0f+(double)Double_ReadI2C(MPU_ADRESS,TEMP_OUT_H)/326.8;
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

