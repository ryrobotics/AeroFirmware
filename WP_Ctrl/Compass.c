
#include "Headfile.h"
#include "Compass.h"

#define MAG_EXTERNAL_ENABLE  0//当采用外部磁力计时使能1
#define MAG_EXTERNAL_IST8310 0
#define MAG_EXTERNAL_QMC5883 1
#define MAG_EXTERNAL_TYPE    MAG_EXTERNAL_QMC5883


void Compass_Init(void)
{
#if (!MAG_EXTERNAL_ENABLE)//内置磁力计IST8310
	Single_WriteI2C(IST8310_SLAVE_ADDRESS,0x41,0x24);//开启16x内部平均
	Single_WriteI2C(IST8310_SLAVE_ADDRESS,0x42,0xC0);//Set/Reset内部平均
#else  										//外置磁力计IST8310、QMC5883
  I2C_GPIO_Config();
	Delay_Ms(10);
	if(MAG_EXTERNAL_TYPE==MAG_EXTERNAL_QMC5883) QMC5883L_Init();
  else		
	{
		Single_WriteI2C0(IST8310_SLAVE_ADDRESS,0x41,0x24);//开启16x内部平均
	  Single_WriteI2C0(IST8310_SLAVE_ADDRESS,0x42,0xC0);//Set/Reset内部平均
	}
#endif	
	Set_Cutoff_Frequency(40, 18,&Mag_Parameter);//传感器校准加计滤波值	
}

/***********************************************************
@函数名：Compass_Read_Data
@入口参数：vector3f *mag
@出口参数：无
功能描述：磁力计数据采集状态机
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
uint8_t Compass_Read_Data(vector3f *mag)//读取磁力计数据状态机
{
  static uint16_t compass_sampling_cnt=0;
  uint8_t buf[6];
  compass_sampling_cnt++;
  if(compass_sampling_cnt==1)
  {
#if (!MAG_EXTERNAL_ENABLE)//重新映射磁力计三轴数据
    Single_WriteI2C(IST8310_SLAVE_ADDRESS,IST8310_REG_CNTRL1,0x01);//Single Measurement Mode
#else	
		if(MAG_EXTERNAL_TYPE==MAG_EXTERNAL_IST8310)	Single_WriteI2C0(IST8310_SLAVE_ADDRESS,IST8310_REG_CNTRL1,0x01);//Single Measurement Mode
#endif		
  }
  else if(compass_sampling_cnt==6)
  {
#if (!MAG_EXTERNAL_ENABLE)//重新映射磁力计三轴数据
    buf[0]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x03);//OUT_X_L_A
    buf[1]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x04);//OUT_X_H_A
    buf[2]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x05);//OUT_Y_L_A
    buf[3]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x06);//OUT_Y_H_A
    buf[4]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x07);//OUT_Z_L_A
    buf[5]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x08);//OUT_Z_H_A
    mag->x= (int16_t)((buf[1]<<8)|buf[0]);
    mag->y=-(int16_t)((buf[3]<<8)|buf[2]);
    mag->z= (int16_t)((buf[5]<<8)|buf[4]);
#else	
		if(MAG_EXTERNAL_TYPE==MAG_EXTERNAL_QMC5883)
		{
			buf[0]=Single_ReadI2C0(QMC5883L_RD_ADDRESS,QMC5883L_DATA_READ_X_LSB);//OUT_X_L_A
			buf[1]=Single_ReadI2C0(QMC5883L_RD_ADDRESS,QMC5883L_DATA_READ_X_MSB);//OUT_X_H_A
			buf[2]=Single_ReadI2C0(QMC5883L_RD_ADDRESS,QMC5883L_DATA_READ_Y_LSB);//OUT_Y_L_A
			buf[3]=Single_ReadI2C0(QMC5883L_RD_ADDRESS,QMC5883L_DATA_READ_Y_MSB);//OUT_Y_H_A
			buf[4]=Single_ReadI2C0(QMC5883L_RD_ADDRESS,QMC5883L_DATA_READ_Z_LSB);//OUT_Z_L_A
			buf[5]=Single_ReadI2C0(QMC5883L_RD_ADDRESS,QMC5883L_DATA_READ_Z_MSB);//OUT_Z_H_A
			mag->y= (int16_t)((buf[1]<<8)|buf[0]);
			mag->x= (int16_t)((buf[3]<<8)|buf[2]);
			mag->z=-(int16_t)((buf[5]<<8)|buf[4]);
		}
		else if(MAG_EXTERNAL_TYPE==MAG_EXTERNAL_IST8310)
		{
			buf[0]=Single_ReadI2C0(IST8310_SLAVE_ADDRESS,0x03);//OUT_X_L_A
			buf[1]=Single_ReadI2C0(IST8310_SLAVE_ADDRESS,0x04);//OUT_X_H_A
			buf[2]=Single_ReadI2C0(IST8310_SLAVE_ADDRESS,0x05);//OUT_Y_L_A
			buf[3]=Single_ReadI2C0(IST8310_SLAVE_ADDRESS,0x06);//OUT_Y_H_A
			buf[4]=Single_ReadI2C0(IST8310_SLAVE_ADDRESS,0x07);//OUT_Z_L_A
			buf[5]=Single_ReadI2C0(IST8310_SLAVE_ADDRESS,0x08);//OUT_Z_H_A
			mag->x= (int16_t)((buf[1]<<8)|buf[0]);
			mag->y= (int16_t)((buf[3]<<8)|buf[2]);
			mag->z=-(int16_t)((buf[5]<<8)|buf[4]);
		}
#endif			
    compass_sampling_cnt=0;
    return 1;
  }
  return 0;
}

