#include "Headfile.h"
#include "tfmini.h"

tfmini tfdata; 
uint16_t tfmini_tail,last_tfmini_tail,tfmini_offset;
void TFmini_Statemachine(void)
{
  static uint16_t tfmini_cnt=0;
	tfmini_cnt++;
  if(tfmini_cnt>=4)//20ms=50hz
  {
		tfmini_cnt=0;
		tfmini_tail=COM1_Rx_Buf.Tail;
		if(tfmini_tail<9)
		{
		  tfmini_offset=9;
		}
    else
		{
		  tfmini_offset=0;
		}			
		for(uint16_t i=tfmini_offset;i<=tfmini_offset+9;i++)
		{
			if(COM7_Rx_Buf.Ring_Buff[i]==0x59
				&&COM7_Rx_Buf.Ring_Buff[i+1]==0x59)
			{
			  tfdata.distance=(COM7_Rx_Buf.Ring_Buff[i+3]*256)+COM7_Rx_Buf.Ring_Buff[i+2];
				tfdata.strength=(COM7_Rx_Buf.Ring_Buff[i+4]*256)+COM7_Rx_Buf.Ring_Buff[i+5];
				tfdata.temperature=((COM7_Rx_Buf.Ring_Buff[i+6]*256)+COM7_Rx_Buf.Ring_Buff[i+7])/8.0f-256;
				tfdata.div=(tfdata.distance-tfdata.last_distance)/0.02f;//观测速度
				tfdata.acc=(tfdata.div-tfdata.last_div)/0.02f;					//观测加速度
				tfdata.last_distance=tfdata.distance;										//观测距离保存
				tfdata.last_div=tfdata.div;															//观测速度保存				
			}
			WP_Sensor.tfmini_updtate_flag=1;
		}
		if(tfdata.distance<=1200&&tfdata.distance>0)  tfdata.health=1;
		else tfdata.health=0;
	}	
}
