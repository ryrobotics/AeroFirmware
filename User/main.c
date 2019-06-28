
#include "Headfile.h"
/***********************************************************
@函数名：main
@入口参数：无
@出口参数：无
@功能描述：主函数，对系统芯片资源、飞控外设进行初始化后，执行
while（1）里面非主要任务非主要任务是指对周期没有严格要求或者
执行时间严重耗时的子函数，例如：电压采集、按键扫描、显示屏刷
新、地面站发送、加速度计标定、磁力计标定、遥控器行程标定、参
数保存等。
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
int main(void)
{
  HardWave_Init();//芯片资源、飞控外设初始化
  while(1)//主循环
  {	
    Get_Battery_Voltage();//测量电池电压		
    Key_Scan(Key_Right_Release);//按键扫描
    QuadShow();//OLED显示
    Vcan_Send();//山外地面站（多功能调试助手）
    ANO_SEND_StateMachine();//ANO地面站发送
    Accel_Calibartion();//加速度计6面校准
    Mag_Calibartion_LS(&WP_Sensor.mag_raw,Circle_Angle);//磁力计椭球校准
    RC_Calibration_Check(PPM_Databuf);//遥控器行程校准
    Save_Or_Reset_PID_Parameter();//运用地面站，修改控制参数
  }
}


