
#include "Headfile.h"
#include "myiic.h"
/*
                       _oo0oo_
                      o8888888o
                      88" . "88
                      (| -_- |)
                      0\  =  /0
                    ___/`---'\___
                  .' \\|     |// '.
                 / \\|||  :  |||// \
                / _||||| -:- |||||- \
               |   | \\\  -  /// |   |
               | \_|  ''\---/''  |_/ |
               \  .-\__  '-'  ___/-. /
             ___'. .'  /--.--\  `. .'___
          ."" '<  `.___\_<|>_/___.' >' "".
         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
         \  \ `_.   \_ __\ /__ _/   .-` /  /
     =====`-.____`.___ \_____/___.-`___.-'=====
                       `=---='
*/

//-----------------------------------------------------
#define ICM20689_ADDRESS 0x68    // AD0 should be low

#define ICM20689_SELF_TEST_X_GYRO  0x00
#define ICM20689_SELF_TEST_Y_GYRO  0x01 
#define ICM20689_SELF_TEST_Z_GYRO  0x02
#define ICM20689_SELF_TEST_X_ACCEL 0x0D	 
#define ICM20689_SELF_TEST_Y_ACCEL 0x0E	 
#define ICM20689_SELF_TEST_Z_ACCEL 0x0F	 
#define ICM20689_XG_OFFS_USRH      0x13	
#define ICM20689_XG_OFFS_USRL      0x14
#define ICM20689_YG_OFFS_USRH      0x15
#define ICM20689_YG_OFFS_USRL      0x16
#define ICM20689_ZG_OFFS_USRH      0x17
#define ICM20689_ZG_OFFS_USRL      0x18
#define ICM20689_SMPLRT_DIV        0x19
#define ICM20689_CONFIG            0x1A
#define ICM20689_GYRO_CONFIG       0x1B
#define ICM20689_ACCEL_CONFIG      0x1C
#define ICM20689_ACCEL_CONFIG_2    0x1D

#define ICM20689_ACCEL_XOUT_H      0x3B
#define ICM20689_ACCEL_XOUT_L      0x3C
#define ICM20689_ACCEL_YOUT_H      0x3D
#define ICM20689_ACCEL_YOUT_L      0x3E
#define ICM20689_ACCEL_ZOUT_H      0x3F
#define ICM20689_ACCEL_ZOUT_L      0x40
#define ICM20689_TEMP_OUT_H        0x41
#define ICM20689_TEMP_OUT_L        0x42
#define ICM20689_GYRO_XOUT_H       0x43
#define ICM20689_GYRO_XOUT_L       0x44
#define ICM20689_GYRO_YOUT_H       0x45
#define ICM20689_GYRO_YOUT_L       0x46
#define ICM20689_GYRO_ZOUT_H       0x47
#define ICM20689_GYRO_ZOUT_L       0x48

#define ICM20689_PWR_MGMT_1        0x6B
#define ICM20689_PWR_MGMT_2        0x6C
#define ICM20689_WHO_AM_I          0x75
/* USER CODE END Private defines */


uint8_t ICM20689_Read_Reg(uint8_t reg)
{												
	return i2cRead(ICM20689_ADDRESS,reg);
}

void ICM20689_Write_Reg(uint8_t reg,uint8_t value)
{
  i2cWrite(MPU_ADRESS,reg,value);
}


uint8_t Init_ICM20689(void)
{	
	if(ICM20689_Read_Reg(ICM20689_WHO_AM_I)==0x98)
	{
		//printf("ICM20689 ready\n");
	}
	else
	{
		//printf("ICM20689 error\n");
		return 1;
	}
	ICM20689_Write_Reg(ICM20689_PWR_MGMT_1, 0x00);	//解除休眠状态
	ICM20689_Write_Reg(ICM20689_CONFIG, 0x07);      //低通滤波频率，典型值：0x07(3600Hz)此寄存器内决定Internal_Sample_Rate==8K
	
/*******************Init GYRO and ACCEL******************************/	
	ICM20689_Write_Reg(ICM20689_SMPLRT_DIV, 0x00);  //陀螺仪采样率，典型值：0x07(1kHz) (SAMPLE_RATE= Internal_Sample_Rate / (1 + SMPLRT_DIV) )
	ICM20689_Write_Reg(ICM20689_GYRO_CONFIG, 0x18); //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	ICM20689_Write_Reg(ICM20689_ACCEL_CONFIG, 0x18);//加速计自检、测量范围及高通滤波频率，典型值：0x18(不自检，16G)
	ICM20689_Write_Reg(ICM20689_ACCEL_CONFIG_2, 0x08);//加速计高通滤波频率 典型值 ：0x08  （1.13kHz）	
	return 0;
}

