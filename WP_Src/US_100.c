
#include "Headfile.h"
#include "US_100.h"



float US_Distance=0,Last_US_Distance=0,US_Distance_Div=0,Last_US_Distance_Div=0;
float US_Distance_Acc=0;
uint16_t Start_Tail=0,Last_Start_Tail;
/***********************************************************
@函数名：US_100_Distance
@入口参数：uint8 MSB,uint8 LSB
@出口参数：无
@功能描述：超声波高度数据合成
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
float US_100_Distance(uint8 MSB,uint8 LSB)
{
  return (256*(MSB)+(LSB))/10.0f;//单位cm
}

/***********************************************************
@函数名：US_100_Temperature
@入口参数：uint8 data
@出口参数：无
@功能描述：超声波温度数据合成
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
float US_100_Temperature(uint8 data)
{
  return (data-45)/1.0;
}

/***********************************************************
@函数名：US_100_Start
@入口参数：无
@出口参数：无
@功能描述：超声波高度数据触发
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void US_100_Start(void)
{	
  UARTCharPut(UART7_BASE, US_100_Distance_CMD);
}


uint8_t US_100_Update_Flag=0;
uint8 US_100_Cnt=0;

/***********************************************************
@函数名：US_100_Statemachine
@入口参数：无
@出口参数：无
@功能描述：超声波数据解析状态机
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void US_100_Statemachine(void)
{
  US_100_Cnt++;
  if(US_100_Cnt>=20)//100ms
  {
		Last_US_Distance=US_Distance;
    US_100_Start();//超声波触发，接收在外部中断里面处理
    US_100_Cnt=0;
    Start_Tail=COM7_Rx_Buf.Tail;
    if(ABS(Start_Tail-Last_Start_Tail)==2)
    {
      if(Start_Tail==0)
      {
        US_Distance=US_100_Distance(COM7_Rx_Buf.Ring_Buff[2],
                                    COM7_Rx_Buf.Ring_Buff[3]);
        US_100_Update_Flag=1;
      }
      else if(Start_Tail==1)
      {
        US_Distance=US_100_Distance(COM7_Rx_Buf.Ring_Buff[3],
                                    COM7_Rx_Buf.Ring_Buff[0]);
        US_100_Update_Flag=1;
      }
      else if(Start_Tail==2)
      {
        US_Distance=US_100_Distance(COM7_Rx_Buf.Ring_Buff[0],
                                    COM7_Rx_Buf.Ring_Buff[1]);
        US_100_Update_Flag=1;
      }
      else if(Start_Tail==3)
      {
        US_Distance=US_100_Distance(COM7_Rx_Buf.Ring_Buff[1],
                                    COM7_Rx_Buf.Ring_Buff[2]);
        US_100_Update_Flag=1;
      }
    }
    Last_Start_Tail=Start_Tail;
		US_Distance_Div=(US_Distance-Last_US_Distance)/0.1f;
		US_Distance_Acc=(US_Distance_Div-Last_US_Distance_Div)/0.1f;
		Last_US_Distance_Div=US_Distance_Div;
  }
  if(US_Distance<=User_Height_Max&&US_Distance>0)  Sensor_Flag.Hcsr04_Health=1;
  else  Sensor_Flag.Hcsr04_Health=0; 
}

