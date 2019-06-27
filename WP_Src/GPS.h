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
#ifndef __GPS_H
#define	__GPS_H
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#define EARTHR 6371004
//-----GNSS fixType-----//
typedef enum
{
  NO_FIX = 0x00,//无定位
  DEAD_RECKONING_ONLY = 0x01,//仅惯导定位
  FIX_2D = 0x02,//2维定位
  FIX_3D = 0x03,//3维定位
  GNSS_DEAD_RECKONING = 0x04,//GPS与惯导定位
  TIME_ONLY_FIX = 0x05//只有时间定位
}GNSS_FixType_Def;

void GPS_Data_Prase(void);
void GPS_PVT_Parse(void);
void Ublox_Set_Output_PVT_10hz_Baud_Set(unsigned long Baud);
void Set_GPS_USART(void);

extern double Last_Longitude,Last_Latitude;
extern int32_t Longitude_Origion,Latitude_Origion;
extern double Longitude,Latitude;

extern double Longitude_Deg,Latitude_Deg;
extern double Longitude_Min,Latitude_Min;
extern u16 TimeBeijing[3];
extern char TimeStr[8];
extern float GPS_Quality;
extern uint16 GPS_Sate_Num,Last_GPS_Sate_Num;
extern float GPS_Yaw;
extern float GPS_Ground_Speed;
extern float GPS_Speed_Resolve[2];
extern Vector3_Nav GPS_Vel;
extern Vector3_Nav GPS_Vel_Div;
extern float Horizontal_Acc_Est;
extern float GPS_Pos_DOP;
extern uint8  GPS_Fix_Flag[4];

extern uint8 GPS_FixType;
extern float High_GPS;
extern uint8_t GPS_Update_Flag;
#endif
