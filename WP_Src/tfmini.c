#include "Headfile.h"
#include "tfmini.h"


uint8_t Ground_Sensor_Now_Mode=0x01;
float ground_sensor_default=0x01;

tfmini tfdata; 
uint16_t tfmini_tail,last_tfmini_tail,tfmini_offset;
void TFmini_Statemachine(void)
{
  static uint16_t tfmini_cnt=0;
	uint8_t check_sum=0;
	tfmini_cnt++;
  if(tfmini_cnt>=20)//100ms=10hz
  {
		tfmini_cnt=0;
		tfmini_tail=COM7_Rx_Buf.Tail;
//		if(tfmini_tail<9)
//		{
//		  tfmini_offset=9;
//		}
//    else
//		{
//		  tfmini_offset=0;
//		}			
		for(uint16_t i=tfmini_offset;i<tfmini_offset+9;i++)
		{
			if(COM7_Rx_Buf.Ring_Buff[i]==0x59
				&&COM7_Rx_Buf.Ring_Buff[i+1]==0x59)
			{
				for(uint16_t j=i;j<i+8;j++)  check_sum += COM7_Rx_Buf.Ring_Buff[j];//和校验
				if(check_sum==COM7_Rx_Buf.Ring_Buff[i+8])
				{
					tfdata.distance=((COM7_Rx_Buf.Ring_Buff[i+3]*256)+COM7_Rx_Buf.Ring_Buff[i+2])/1.0f;
					//tfdata.distance*=rMat[2][2];
					tfdata.strength=(COM7_Rx_Buf.Ring_Buff[i+4]*256)+COM7_Rx_Buf.Ring_Buff[i+5];
					tfdata.temperature=((COM7_Rx_Buf.Ring_Buff[i+6]*256)+COM7_Rx_Buf.Ring_Buff[i+7])/8.0f-256;
					tfdata.div=(tfdata.distance-tfdata.last_distance)/0.1f; //观测速度
					tfdata.acc=(tfdata.div-tfdata.last_div)/0.1f;					  //观测加速度
					tfdata.last_distance=tfdata.distance;										//观测距离保存
					tfdata.last_div=tfdata.div;															//观测速度保存
					WP_Sensor.tfmini_updtate_flag=1;
				}					
			}			
		}
		if(tfdata.distance<=1000
				&&tfdata.distance>0
					&&tfdata.strength>100
						&&tfdata.strength!=65535)  
			tfdata.health=1;
		else tfdata.health=0;
	}	
}


void Ground_Sensor_Statemachine(void)
{
	if(Ground_Sensor_Now_Mode==US100)		
		US_100_Statemachine();	 //超声波传感器状态机更新
	else TFmini_Statemachine();//激光TFMINI_PLUS传感器状态机更新
}

