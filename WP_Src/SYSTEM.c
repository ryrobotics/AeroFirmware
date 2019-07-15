
#include "Headfile.h"
#include "SYSTEM.h"

/***********************************************************
@函数名：HardWave_Init
@入口参数：无
@出口参数：无
功能描述：对系统芯片资源常规硬件资源进行一系列初始化，具体见
各初始化后的注释
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void HardWave_Init(void)
{
	WP_Init();//飞控初始化
  Butterworth_Parameter_Init();//滤波器参数初始化
  PPM_Init();//PPM解析初始化
  Init_PWM();//PWM初始化
  Bling_Init();//LED状态指示灯初始化
  Quad_Start_Bling();//开机LED预显示
  Key_Init();//板载按键初始化
  RC_Calibration_Trigger();//遥控器行程标定检查
  Horizontal_Calibration_Init();//机架水平标定初始化
  PID_Paramter_Init_With_Flash();//PID参数初始化
  Init_I2C();//硬件I2C初始化
  Init_MPU6050();//MPU6050陀螺仪、加速度计配置初始化
//  Compass_Init();//磁力计配置初始化
  SPL06_Init();//SPL06气压计配置初始化
  Parameter_Init();//加速度计、磁力计校准参数初始化
  WP_Quad_Init();//根据观测传感器加速度计、磁力计对姿态四元数初始化
  ConfigureUART1();//串口1初始化，匿名地面站
  ConfigureUART3();//串口3初始化，OPENMV、树莓派视觉模组、SDK模式串口
  ConfigureUART2();//串口2初始化，副OPENMV
//  Set_GPS_USART();//串口2初始化，使用UBLOX协议配置GPS模块
  ConfigureUART7();//串口7初始化.超声波模块/TOF解析串口	
  ConfigureUART6();//串口6初始化，光流模块LC306数据解析
  SDK_Init();//SDK模式初始化
	Temperature_Ctrl_Init();
  Time_init();//飞控调度定时器初始化
  delay_ms(200);//初始化延时	
}

