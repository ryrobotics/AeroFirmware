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
#ifndef _WP_CTRL_
#define _WP_CTRL_

#define Aerocraft_Axis_Mode_Default  4
#define Aerocraft_Axis_Mode Aerocraft_Axis_Mode_Default
          
#if (Aerocraft_Axis_Mode==6)//六轴控制器输出映射表
/*
     机头
    3   1
    *   *   
      
5 *   *   * 6

    *   *
    2   4
其中：1、4、5逆时针转动
     2、3、6逆时针转动
*/
#define Moter1_Thr_Scale 1.0f
#define Moter2_Thr_Scale 1.0f
#define Moter3_Thr_Scale 1.0f
#define Moter4_Thr_Scale 1.0f
#define Moter5_Thr_Scale 1.0f
#define Moter6_Thr_Scale 1.0f


#define Moter1_Roll_Scale -0.75f//-0.5f
#define Moter2_Roll_Scale  0.75f//0.5f
#define Moter3_Roll_Scale  0.75f//0.5f
#define Moter4_Roll_Scale -0.75f//-0.5f
#define Moter5_Roll_Scale  0.75f//1.0f
#define Moter6_Roll_Scale -0.75f//-1.0f
          
#define Moter1_Pitch_Scale  0.866025f
#define Moter2_Pitch_Scale -0.866025f
#define Moter3_Pitch_Scale  0.866025f
#define Moter4_Pitch_Scale -0.866025f
#define Moter5_Pitch_Scale  0.0f
#define Moter6_Pitch_Scale  0.0f
          
#define Moter1_Yaw_Scale -1.0f
#define Moter2_Yaw_Scale 1.0f
#define Moter3_Yaw_Scale 1.0f
#define Moter4_Yaw_Scale -1.0f
#define Moter5_Yaw_Scale -1.0f
#define Moter6_Yaw_Scale 1.0f

#else//四轴控制器输出映射表
/*
      机头
    3     1
    *     *   
       * 
    *     *
    2     4
其中：1、2逆时针转动
      3、4逆时针转动
*/
#define Moter1_Thr_Scale 1.0f
#define Moter2_Thr_Scale 1.0f
#define Moter3_Thr_Scale 1.0f
#define Moter4_Thr_Scale 1.0f
#define Moter5_Thr_Scale 0.0f
#define Moter6_Thr_Scale 0.0f

#define Moter1_Roll_Scale -1.0f
#define Moter2_Roll_Scale 1.0f
#define Moter3_Roll_Scale 1.0f
#define Moter4_Roll_Scale -1.0f
#define Moter5_Roll_Scale 0.0f
#define Moter6_Roll_Scale 0.0f
          
#define Moter1_Pitch_Scale 1.0f
#define Moter2_Pitch_Scale -1.0f
#define Moter3_Pitch_Scale 1.0f
#define Moter4_Pitch_Scale -1.0f
#define Moter5_Pitch_Scale 0.0f
#define Moter6_Pitch_Scale 0.0f
          
#define Moter1_Yaw_Scale -1.0f
#define Moter2_Yaw_Scale -1.0f
#define Moter3_Yaw_Scale 1.0f
#define Moter4_Yaw_Scale 1.0f
#define Moter5_Yaw_Scale 0.0f
#define Moter6_Yaw_Scale 0.0f
#endif
          
          
//注释以下行开启三环定高模式，即竖直高度位置+竖直速度+竖直加速度
#define YAW_Pos_Control_Accel_Disable  0
#define YAW_VEL_FEEDFORWARD  0//竖直速度前馈控制器

#define Optical_Enable  1//光流辅助悬停标志

#define ADRC_MODE  0
#define PID_MODE   1
#define TEST_MODE  2
//#define GYRO_CONTROL_MODE  TEST_MODE
#define GYRO_CONTROL_MODE  PID_MODE
//#define GYRO_CONTROL_MODE  ADRC_MODE


#define SDK_Take_Over_Ctrl  1//SDK控制接管来源于OPENMV、无线数传
//#define SDK_Take_Over_Ctrl  2//SDK控制接管来源于人为事先指定


void Total_Control(void);
void Control_Output(void);
void Landon_Earth_Check(void);//自检触地进入怠速模式
void CarryPilot_Control(void);
	


extern float LPButter_Vel_Error(float curr_input);
extern uint8_t Controler_High_Mode,SDK_Ctrl_Mode;
extern uint8_t Controler_Horizontal_Mode,Last_Controler_Horizontal_Mode;
extern uint16_t High_Hold_Throttle;
extern uint8_t Landon_Earth_Flag;
extern uint16 Throttle;
extern uint8_t Last_Landon_Earth_Flag,Landon_Earth_Flag;
extern uint16_t Landon_Earth_Cnt;
extern uint16_t Throttle_Output;
extern float Active_Para1,Active_Para2;
extern uint8_t Landon_Earth_Flag;
extern uint8_t  Pos_Hold_SetFlag;
extern uint16 Motor_PWM_1,Motor_PWM_2,Motor_PWM_3,Motor_PWM_4,Motor_PWM_5,Motor_PWM_6;//四个电机输出PWM
extern uint8_t Reserve_Mode_Fast_Exchange_Flag;
extern Vector2_Ang Body_Frame_Pos_Err;
extern Vector3_Nav Earth_Frame_Pos_Err;
#endif

