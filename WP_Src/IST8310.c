
#include "Headfile.h"
#include "I2C.h"
#include "WP_DataType.h"
#include "IST8310.h"



Vector2f magn={0,0};
Vector3f mag_offset;
Butter_Parameter Mag_Parameter;
/***********************************************************
@函数名：IST8310_Init
@入口参数：无
@出口参数：无
功能描述：磁力计初始化配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void IST8310_Init(void)//磁力计初始化配置
{
	Single_WriteI2C(IST8310_SLAVE_ADDRESS,0x41,0x24);//开启16x内部平均
	Single_WriteI2C(IST8310_SLAVE_ADDRESS,0x42,0xC0);//Set/Reset内部平均
  Set_Cutoff_Frequency(40, 18,&Mag_Parameter);//传感器校准加计滤波值
}

/***********************************************************
@函数名：IST8310_Read_Data
@入口参数：vector3f *mag
@出口参数：无
功能描述：磁力计数据采集状态机
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
uint8_t IST8310_Read_Data(vector3f *mag)//读取磁力计数据状态机
{
  static uint16_t ist8310_sampling_cnt=0;
  uint8_t buf[6];
  ist8310_sampling_cnt++;
  if(ist8310_sampling_cnt==1)
  {
    Single_WriteI2C(IST8310_SLAVE_ADDRESS,IST8310_REG_CNTRL1,0x01);//Single Measurement Mode	
  }
  else if(ist8310_sampling_cnt==6)
  {
    buf[0]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x03);//OUT_X_L_A
    buf[1]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x04);//OUT_X_H_A
    buf[2]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x05);//OUT_Y_L_A
    buf[3]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x06);//OUT_Y_H_A
    buf[4]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x07);//OUT_Z_L_A
    buf[5]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x08);//OUT_Z_H_A
    mag->x= (int16_t)((buf[1]<<8)|buf[0]);
    mag->y=-(int16_t)((buf[3]<<8)|buf[2]);
    mag->z= (int16_t)((buf[5]<<8)|buf[4]);
    ist8310_sampling_cnt=0;
    return 1;
  }
  return 0;
}



