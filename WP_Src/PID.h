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
#ifndef _PID_H_
#define _PID_H_

#define PID_STARTADDR  0x0803A800//0x0803A800
#define PID1_Address  0
#define PID2_Address  12
#define PID3_Address  24
#define PID4_Address  36
#define PID5_Address  48
#define PID6_Address  60
#define PID7_Address  72
#define PID8_Address  84
#define PID9_Address  96
#define PID10_Address 108
#define PID11_Address 120


#include "Filter.h"

typedef struct
{
 float p;
 float i;
 float d;
}Vector3f_pid;


typedef struct
{
    uint8 Err_Limit_Flag :1;//偏差限幅标志
    uint8 Integrate_Limit_Flag :1;//积分限幅标志
    uint8 Integrate_Separation_Flag :1;//积分分离标志
    float Expect;//期望
    float FeedBack;//反馈值
    float Err;//偏差
    float Last_Err;//上次偏差
    float Err_Max;//偏差限幅值
    float Integrate_Separation_Err;//积分分离偏差值
    float Integrate;//积分值
    float Integrate_Max;//积分限幅值
    float Kp;//控制参数Kp
    float Ki;//控制参数Ki
    float Kd;//控制参数Kd
    float Control_OutPut;//控制器总输出
    float Last_Control_OutPut;//上次控制器总输出
    float Control_OutPut_Limit;//输出限幅
    /***************************************/
    float Pre_Last_Err;//上上次偏差
    float Adaptable_Kd;//自适应微分参数
    float Last_FeedBack;//上次反馈值
    float Dis_Err;//微分量
    float Dis_Error_History[5];//历史微分量
    float Err_LPF;
    float Last_Err_LPF;
    float Dis_Err_LPF;
    float Last_Dis_Err_LPF;
    float Pre_Last_Dis_Err_LPF;
    Butter_BufferData Control_Device_LPF_Buffer;//控制器低通输入输出缓冲
    Testime PID_Controller_Dt;
    float Scale_Kp;
    float Scale_Ki;
    float Scale_Kd;
}PID_Controler;

typedef struct
{
     PID_Controler Pitch_Angle_Control;
     PID_Controler Pitch_Gyro_Control;
     PID_Controler Roll_Angle_Control;
     PID_Controler Roll_Gyro_Control;
     PID_Controler Yaw_Angle_Control;
     PID_Controler Yaw_Gyro_Control;
     PID_Controler High_Position_Control;
     PID_Controler High_Speed_Control;
     PID_Controler Longitude_Position_Control;
     PID_Controler Longitude_Speed_Control;
     PID_Controler Latitude_Position_Control;
     PID_Controler Latitude_Speed_Control;
     /*************加速度控制器，新加****************/
     PID_Controler High_Acce_Control;
     PID_Controler Longitude_Acce_Control;
     PID_Controler Latitude_Acce_Control;
     /*************光流控制器，新加****************/
     PID_Controler Optical_Position_Control;
     PID_Controler Optical_Speed_Control;
     PID_Controler SDK_Roll_Position_Control;
     PID_Controler SDK_Pitch_Position_Control;
		 PID_Controler IMU_Temperature_Control;
}AllControler;

typedef enum
{
     Pitch_Angle_Controler=0,
     Pitch_Gyro_Controler=1,
     Roll_Angle_Controler=2,
     Roll_Gyro_Controler=3,
     Yaw_Angle_Controler=4,
     Yaw_Gyro_Controler=5,
     High_Position_Controler=6,
     High_Speed_Controler=7,
     Longitude_Position_Controler=8,
     Longitude_Speed_Controler=9,
     Latitude_Position_Controler=10,
     Latitude_Speed_Controler=11,
     High_Acce_Controler=12,
     Longitude_Acce_Controler=13,
     Latitude_Acce_Controler=14,
     Optical_Position_Controler=15,
     Optical_Speed_Controler=16,
     SDK_Roll_Position_Controler=17,
     SDK_Pitch_Position_Controler=18,
     IMU_Temperature_Controler=19	
}Controler_Label;

extern AllControler Total_Controller;


void PID_Paramter_Init_With_Flash(void);
void  Total_PID_Init(void);
void  PID_Init(PID_Controler *Controler,Controler_Label Label);
float PID_Control(PID_Controler *Controler);
float PID_Control_Yaw(PID_Controler *Controler);
void  PID_LPF_Reset(PID_Controler *Controler,Controler_Label Label);
float PID_Control_Div_LPF(PID_Controler *Controler);
float PID_Control_Err_LPF(PID_Controler *Controler);
float Differential_Forward_PID_Control_Div_LPF(PID_Controler *Controler);
float PID_Control_Div_LPF_For_Gyro(PID_Controler *Controler);
float PID_Control_SDK_Err_LPF(PID_Controler *Controler,uint8_t Differential_Enable_Flag);
void  PID_Integrate_Reset(PID_Controler *Controler);
void  Take_Off_Reset(void);
void Throttle_Control_Reset(void);
void Save_Or_Reset_PID_Parameter(void);
void Save_PID_Parameter(void);
extern uint8_t Sort_PID_Cnt,Sort_PID_Flag;
#endif


