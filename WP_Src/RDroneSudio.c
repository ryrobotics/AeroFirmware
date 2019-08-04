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
#include "RDroneSudio.h"

/* 浮点数到十六进制转换 */
/* floatNum：浮点型变量 */
/* byteArry：分解成 4 个字节的首地址 */
void FloatToByte(float floatNum,unsigned char* byteArry)
{
	int i;
	char* pchar=(char*)&floatNum;
	for(i=0;i<sizeof(float);i++)
	{
		*byteArry=*pchar;
		pchar++;
		byteArry++;
	}
}

void RDroneStudio_Send_Attitude(float rd_pitch,float rd_roll,float rd_yaw)
{
  unsigned char rd_buf[16];
	rd_buf[0]=0x55;
	rd_buf[1]=0xAA;
  rd_buf[2]=0x00;
  FloatToByte(rd_pitch,&rd_buf[3]);
  FloatToByte(rd_roll,&rd_buf[7]);
  FloatToByte(rd_yaw,&rd_buf[11]);
	rd_buf[15]=0x55;
	USART0_Send(rd_buf, 16);
}
void RDroneStudio_Send_Pos(float pos_z,float pos_x,float pos_y)
{
  unsigned char rd_buf[16];
	rd_buf[0]=0x55;
	rd_buf[1]=0xAA;
  rd_buf[2]=0x01;
  FloatToByte(pos_z,&rd_buf[3]);
  FloatToByte(pos_x,&rd_buf[7]);
  FloatToByte(pos_y,&rd_buf[11]);
	rd_buf[15]=0x55;
	USART0_Send(rd_buf, 16);
}
void RDroneStudio_Send_Gyro(float rd_pitch_gyro,float rd_roll_gyro,float rd_yaw_gyro)
{
  unsigned char rd_buf[16];
	rd_buf[0]=0x55;
	rd_buf[1]=0xAA;
  rd_buf[2]=0x20;
  FloatToByte(rd_pitch_gyro,&rd_buf[3]);
  FloatToByte(rd_roll_gyro,&rd_buf[7]);
  FloatToByte(rd_yaw_gyro,&rd_buf[11]);
	rd_buf[15]=0x00;
	USART0_Send(rd_buf, 16);
}

void RDroneStudio_Send_AcceData(float x,float y,float z)
{
  unsigned char rd_buf[16];
	rd_buf[0]=0x55;
	rd_buf[1]=0xAA;
  rd_buf[2]=0x21;
  FloatToByte(x,&rd_buf[3]);
  FloatToByte(y,&rd_buf[7]);
  FloatToByte(z,&rd_buf[11]);
	rd_buf[15]=0x00;
	USART0_Send(rd_buf, 16);
}

void RDroneStudio_Send_MagData(float x,float y,float z)
{
  unsigned char rd_buf[16];
	rd_buf[0]=0x55;
	rd_buf[1]=0xAA;
  rd_buf[2]=0x22;
  FloatToByte(x,&rd_buf[3]);
  FloatToByte(y,&rd_buf[7]);
  FloatToByte(z,&rd_buf[11]);
	rd_buf[15]=0x00;
	USART0_Send(rd_buf, 16);
}

void RDroneStudio_Send_PID(unsigned char label,float kp,float ki,float kd)
{
  unsigned char rd_buf[16];
	rd_buf[0]=0x55;
	rd_buf[1]=0xAA;
  rd_buf[2]=label;
  FloatToByte(kp,&rd_buf[3]);
  FloatToByte(ki,&rd_buf[7]);
  FloatToByte(kd,&rd_buf[11]);
	rd_buf[15]=0x00;
	USART0_Send(rd_buf, 16);
}

void RDroneStudio_Send_PID_ALL(void)
{
	RDroneStudio_Send_PID(0x02,Total_Controller.Pitch_Angle_Control.Kp
	                          ,Total_Controller.Pitch_Angle_Control.Ki
                            ,Total_Controller.Pitch_Angle_Control.Kd);
 	RDroneStudio_Send_PID(0x03,Total_Controller.Roll_Angle_Control.Kp
	                          ,Total_Controller.Roll_Angle_Control.Ki
                            ,Total_Controller.Roll_Angle_Control.Kd);
	RDroneStudio_Send_PID(0x04,Total_Controller.Pitch_Gyro_Control.Kp
	                          ,Total_Controller.Pitch_Gyro_Control.Ki
                            ,Total_Controller.Pitch_Gyro_Control.Kd);
 	RDroneStudio_Send_PID(0x05,Total_Controller.Roll_Gyro_Control.Kp
	                          ,Total_Controller.Roll_Gyro_Control.Ki
                            ,Total_Controller.Roll_Gyro_Control.Kd);	
  RDroneStudio_Send_PID(0x06,Total_Controller.Yaw_Angle_Control.Kp
	                          ,Total_Controller.Yaw_Angle_Control.Ki
                            ,Total_Controller.Yaw_Angle_Control.Kd);
  RDroneStudio_Send_PID(0x07,Total_Controller.Yaw_Gyro_Control.Kp
	                          ,Total_Controller.Yaw_Gyro_Control.Ki
                            ,Total_Controller.Yaw_Gyro_Control.Kd);
	
	RDroneStudio_Send_PID(0x08,Total_Controller.High_Position_Control.Kp
	                          ,Total_Controller.High_Position_Control.Ki
                            ,Total_Controller.High_Position_Control.Kd);
  RDroneStudio_Send_PID(0x09,Total_Controller.High_Speed_Control.Kp
	                          ,Total_Controller.High_Speed_Control.Ki
                            ,Total_Controller.High_Speed_Control.Kd);
	
  RDroneStudio_Send_PID(0x0A,Total_Controller.Latitude_Position_Control.Kp
	                          ,Total_Controller.Latitude_Position_Control.Ki
                            ,Total_Controller.Latitude_Position_Control.Kd);
  RDroneStudio_Send_PID(0x0B,Total_Controller.Latitude_Speed_Control.Kp
	                          ,Total_Controller.Latitude_Speed_Control.Ki
                            ,Total_Controller.Latitude_Speed_Control.Kd);
	RDroneStudio_Send_PID(0x0C,Total_Controller.Longitude_Position_Control.Kp
	                          ,Total_Controller.Longitude_Position_Control.Ki
                            ,Total_Controller.Longitude_Position_Control.Kd);
  RDroneStudio_Send_PID(0x0D,Total_Controller.Longitude_Speed_Control.Kp
	                          ,Total_Controller.Longitude_Speed_Control.Ki
                            ,Total_Controller.Longitude_Speed_Control.Kd);													
	
}  

u8 RDroneStudio_RxBuffer[50];	
void RDroneStudio_Data_Anl(unsigned char *PCData);
void RDroneStudio_Receive(u8 data)
{	
	static u8 _data_cnt = 0;
	static u8 state = 0;
	if(state==0&&data==0x55)
	{
		state=1;
		RDroneStudio_RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAA)
	{
		state=2;
		RDroneStudio_RxBuffer[1]=data;
	}
	else if(state==2)
	{
		state=3;
		RDroneStudio_RxBuffer[2]=data;
	}
	else if(state==3&&_data_cnt<12)
	{
		RDroneStudio_RxBuffer[_data_cnt++]=data;
		state = 4;
	}
	else if(state==4)
	{
		state = 0;
		RDroneStudio_RxBuffer[15]=data;
		//RDroneStudio_Data_Anl(RDroneStudio_RxBuffer);
	}
	else state = 0;
}



uint8_t RDroneStudio_Send_PID_FLAG=0;
void RDroneStudio_Send(void)
{
	RDroneStudio_Send_Attitude(Pitch,Roll,Yaw);
	RDroneStudio_Send_Gyro(Pitch_Gyro,Roll_Gyro,Yaw_Gyro);
	RDroneStudio_Send_AcceData(accel.x/AcceMax,accel.y/AcceMax,accel.z/AcceMax);
	RDroneStudio_Send_MagData(mag.x,mag.y,mag.z);
	RDroneStudio_Send_Pos(NamelessQuad.Position[_YAW]/100.0f,NamelessQuad.Position[_PITCH]/100.0f,NamelessQuad.Position[_ROLL]/100.0f);
	if(RDroneStudio_Send_PID_FLAG==1)
	{
		RDroneStudio_Send_PID_ALL();
		RDroneStudio_Send_PID_FLAG=0;
	}
}

float_union HexToFloat;
float UnsignedcharToFloat(unsigned char *DataofPC,unsigned char sequence)
{
    HexToFloat.sv[0] = (((uint16_t)DataofPC[sequence+1]) << 8 | DataofPC[sequence]);
    HexToFloat.sv[1] = (((uint16_t)DataofPC[sequence+3]) << 8 | DataofPC[sequence+2]);
    return HexToFloat.fv;
}

void RDroneStudio_Data_Anl(unsigned char *PCData)
{
        /*读取PID*/
        if(PCData[2]==0x03)
        {
           RDroneStudio_Send_PID_FLAG=1;
        }
        /* 设置Pitch的PID参数 */
        else if(PCData[2]==0x07)
				{
            Total_Controller.Pitch_Angle_Control.Kp= UnsignedcharToFloat(PCData,3);
            Total_Controller.Pitch_Angle_Control.Ki = UnsignedcharToFloat(PCData,7);
            Total_Controller.Pitch_Angle_Control.Kd = UnsignedcharToFloat(PCData,11);
            RDroneStudio_Send_PID_FLAG=1;
            //Write_Config();
        }
        /* 设置ROll的PID参数 */
        else if(PCData[2]==0x08)
				{
            Total_Controller.Roll_Angle_Control.Kp = UnsignedcharToFloat(PCData,3);
            Total_Controller.Roll_Angle_Control.Ki = UnsignedcharToFloat(PCData,7);
            Total_Controller.Roll_Angle_Control.Kd = UnsignedcharToFloat(PCData,11);
            RDroneStudio_Send_PID_FLAG=1;
            //Write_Config();
        }
        /* 设置Yaw的PID参数 */
        else if(PCData[2]==0x09)
				{
            Total_Controller.Yaw_Angle_Control.Kp = UnsignedcharToFloat(PCData,3);
            Total_Controller.Yaw_Angle_Control.Ki = UnsignedcharToFloat(PCData,7);
            Total_Controller.Yaw_Angle_Control.Kd = UnsignedcharToFloat(PCData,11);
            RDroneStudio_Send_PID_FLAG=1;
            //Write_Config();
        }
        /* 设置Height的PID参数 */
        else if(PCData[2]==0x0A)
				{
            Total_Controller.High_Position_Control.Kp = UnsignedcharToFloat(PCData,3);
            Total_Controller.High_Position_Control.Ki = UnsignedcharToFloat(PCData,7);
            Total_Controller.High_Position_Control.Kd = UnsignedcharToFloat(PCData,11);
            RDroneStudio_Send_PID_FLAG=1;
            //Write_Config();
        }
        /* 设置accPitch的PID参数 */
        else if(PCData[2]==0x0B)
				{
            Total_Controller.Pitch_Gyro_Control.Kp = UnsignedcharToFloat(PCData,3);
            Total_Controller.Pitch_Gyro_Control.Ki = UnsignedcharToFloat(PCData,7);
            Total_Controller.Pitch_Gyro_Control.Kd = UnsignedcharToFloat(PCData,11);
            RDroneStudio_Send_PID_FLAG=1;
            //Write_Config();
        }
        /* 设置accRoll的PID参数 */
        else if(PCData[2]==0x0C)
				{
            Total_Controller.Roll_Gyro_Control.Kp = UnsignedcharToFloat(PCData,3);
            Total_Controller.Roll_Gyro_Control.Ki = UnsignedcharToFloat(PCData,7);
            Total_Controller.Roll_Gyro_Control.Kd = UnsignedcharToFloat(PCData,11);
            RDroneStudio_Send_PID_FLAG=1;
            //Write_Config();
        }
        /* 设置accYaw的PID参数 */
        else if(PCData[2]==0x0D)
				{
            Total_Controller.Yaw_Gyro_Control.Kp = UnsignedcharToFloat(PCData,3);
            Total_Controller.Yaw_Gyro_Control.Ki = UnsignedcharToFloat(PCData,7);
            Total_Controller.Yaw_Gyro_Control.Kd = UnsignedcharToFloat(PCData,11);
            RDroneStudio_Send_PID_FLAG=1;
            //Write_Config();
        }
        /* 设置accHeight的PID参数 */
        else if(PCData[2]==0x0E)
				{
            Total_Controller.High_Speed_Control.Kp = UnsignedcharToFloat(PCData,3);
            Total_Controller.High_Speed_Control.Ki = UnsignedcharToFloat(PCData,7);
            Total_Controller.High_Speed_Control.Kd = UnsignedcharToFloat(PCData,11);
            RDroneStudio_Send_PID_FLAG=1;
            //Write_Config();
        }
        /* 设置PositionX的PID参数 */
        else if(PCData[2]==0x12)
				{
            Total_Controller.Latitude_Position_Control.Kp = UnsignedcharToFloat(PCData,3);
            Total_Controller.Latitude_Position_Control.Ki = UnsignedcharToFloat(PCData,7);
            Total_Controller.Latitude_Position_Control.Kd = UnsignedcharToFloat(PCData,11);
            RDroneStudio_Send_PID_FLAG=1;
            //Write_Config();
        }
        /* 设置PositionY的PID参数 */
        else if(PCData[2]==0x13)
				{
            Total_Controller.Longitude_Position_Control.Kp = UnsignedcharToFloat(PCData,3);
            Total_Controller.Longitude_Position_Control.Ki = UnsignedcharToFloat(PCData,7);
            Total_Controller.Longitude_Position_Control.Kd = UnsignedcharToFloat(PCData,11);
            RDroneStudio_Send_PID_FLAG=1;
            //Write_Config();
        }
        /* 设置PositionVX的PID参数 */
        else if(PCData[2]==0x14)
				{
            Total_Controller.Latitude_Speed_Control.Kp = UnsignedcharToFloat(PCData,3);
            Total_Controller.Latitude_Speed_Control.Ki = UnsignedcharToFloat(PCData,7);
            Total_Controller.Latitude_Speed_Control.Kd = UnsignedcharToFloat(PCData,11);
            RDroneStudio_Send_PID_FLAG=1;
            //Write_Config();
        }
        /* 设置PositionVY的PID参数 */
        else if(PCData[2]==0x15)
				{
            Total_Controller.Longitude_Speed_Control.Kp = UnsignedcharToFloat(PCData,3);
            Total_Controller.Longitude_Speed_Control.Ki = UnsignedcharToFloat(PCData,7);
            Total_Controller.Longitude_Speed_Control.Kd = UnsignedcharToFloat(PCData,11);
            RDroneStudio_Send_PID_FLAG=1;
            //Write_Config();
        }
        /*陀螺仪校准*/
        else if(PCData[2]==0x30)
        {
            //OffsetData.GyroX  = RT_Info.GyroX ;
            //OffsetData.GyroY  = RT_Info.GyroY ;
           // OffsetData.GyroZ  = RT_Info.GyroZ ;
           // Write_Config();
        }
}

