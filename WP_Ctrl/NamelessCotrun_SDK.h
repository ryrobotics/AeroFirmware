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
#ifndef __NAMELESSCOTRUN_SDK_H
#define __NAMELESSCOTRUN_SDK_H

#define SDK_Duty_Max 10

typedef struct 
{
  uint8_t Start_Flag;
  uint8_t Execute_Flag;
  uint8_t End_flag;
}Duty_Status;

typedef struct 
{
  Duty_Status Status[SDK_Duty_Max];
  uint16_t Transition_Time[SDK_Duty_Max];
}SDK_Status;



typedef struct
{
  int16_t x;
  int16_t y; 
  uint16_t Pixel;
  uint8_t flag;
}Color_Block;//色块

typedef struct
{
  int16_t x;
  int16_t y; 
  uint16_t Pixel;
  uint8_t flag;
	uint16_t trust_Cnt;
	uint8_t trust_flag;
}Point;//点检测

typedef struct
{
  int16_t x;
  int16_t y; 
  uint16_t data;
  uint8_t flag;
}QR_Code;//二维码检测


typedef struct
{
  int16_t x;
  int16_t y; 
  uint16_t data;
  uint16_t line_angle;
  uint8_t up_ok;
  uint8_t down_ok;
  uint8_t left_ok;
  uint8_t right_ok;
  uint8_t flag;
  uint8_t line_ctrl_enable;
}Line;//线检测

extern uint8_t SDK_Mode_Set,SDK_Now_Mode;
extern Line  SDK_Line;
extern Point SDK_Point;
extern float SDK_Target_Yaw_Gyro;
extern Vector2f SDK_Target;
extern Vector2f SDK_Target_Offset;
extern uint8_t SDK_Recieve_Flag;
extern uint16_t SDK_Transition_Time;
extern SDK_Status SDK_Duty_Status;
void NCQ_SDK_Reset(void);
uint8_t move_with_speed_target(float x_target,float y_target,float delta,SDK_Status *Status,uint16_t number);
uint8_t move_with_xy_target(float pos_x_target,float pos_y_target,SDK_Status *Status,uint16_t number);
uint8_t move_with_z_target(float z_target,float z_vel,float delta,SDK_Status *Status,uint16_t number);
uint8_t move_with_openmv_speed(float x_target,float y_target,float delta,uint8_t OpenMv_Mode,SDK_Status *Status,uint16_t number);
uint8_t move_with_openmv_time(float delta,uint8_t OpenMv_Mode,SDK_Status *Status,uint16_t number);
uint8_t take_off(float z_target,SDK_Status *Status,uint16_t number);
void NCQ_SDK_Run(void);
uint8_t NCQ_SDK_Circle(void);
void SDK_DT_Send_Check(unsigned char mode);
void SDK_Init(void);
void SDK_Data_Prase(void);
void SDK_Data_Receive_Prepare(u8 data);

void SDK_DT_Send_Check_Right(unsigned char mode);
void SDK_Init_Right(void);
void SDK_Data_Prase_Right(void);
void SDK_Data_Receive_Prepare_Right(u8 data);

extern uint8_t take_off_flag;

#define POINT_MODE  0x01	//色块
#define QR_MODE     0x02	//二维码
#define STICK_MODE  0x03	//杆
//#define CIRCLE_MODE 0x04
//#define RGB_MODE    0x05
#define WAIT_MODE   0x0F

#define MODE_OFFSET 0xF0

#endif

