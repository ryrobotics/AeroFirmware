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

#ifndef __WP_FLASH_H__
#define __WP_FLASH_H__

void EEPROM_Init(void);


#define PARAMETER_TABLE_STARTADDR   0x0000 
#define FLIGHT_PARAMETER_TABLE_NUM  100
typedef struct
{
   float Parameter_Table[FLIGHT_PARAMETER_TABLE_NUM];
}FLIGHT_PARAMETER;


typedef enum
{
   PITCH_OFFSET=0,
   ROLL_OFFSET=1,
   ACCEL_X_OFFSET=2,
   ACCEL_Y_OFFSET=3,
   ACCEL_Z_OFFSET=4,
   ACCEL_X_SCALE=5,
   ACCEL_Y_SCALE=6,
   ACCEL_Z_SCALE=7,
   MAG_X_OFFSET=8,
   MAG_Y_OFFSET=9,
   MAG_Z_OFFSET=10,
   RC_CH1_MAX=11,
   RC_CH1_MIN=12,
   RC_CH2_MAX=13,
   RC_CH2_MIN=14,
   RC_CH3_MAX=15,
   RC_CH3_MIN=16,
   RC_CH4_MAX=17,
   RC_CH4_MIN=18,
   RC_CH5_MAX=19,
   RC_CH5_MIN=20,
   RC_CH6_MAX=21,
   RC_CH6_MIN=22,
   RC_CH7_MAX=23,
   RC_CH7_MIN=24,
   RC_CH8_MAX=25,
   RC_CH8_MIN=26,
   PID1_PARAMETER_KP=27,
   PID1_PARAMETER_KI=28,
   PID1_PARAMETER_KD=29,
   PID2_PARAMETER_KP=30,
   PID2_PARAMETER_KI=31,
   PID2_PARAMETER_KD=32,
   PID3_PARAMETER_KP=33,
   PID3_PARAMETER_KI=34,
   PID3_PARAMETER_KD=35,
   PID4_PARAMETER_KP=36,
   PID4_PARAMETER_KI=37,
   PID4_PARAMETER_KD=38,
   PID5_PARAMETER_KP=39,
   PID5_PARAMETER_KI=40,
   PID5_PARAMETER_KD=41,
   PID6_PARAMETER_KP=42,
   PID6_PARAMETER_KI=43,
   PID6_PARAMETER_KD=44,
   PID7_PARAMETER_KP=45,
   PID7_PARAMETER_KI=46,
   PID7_PARAMETER_KD=47,
   PID8_PARAMETER_KP=48,
   PID8_PARAMETER_KI=49,
   PID8_PARAMETER_KD=50,
   PID9_PARAMETER_KP=51,
   PID9_PARAMETER_KI=52,
   PID9_PARAMETER_KD=53,
   PID10_PARAMETER_KP=54,
   PID10_PARAMETER_KI=55,
   PID10_PARAMETER_KD=56,
   PID11_PARAMETER_KP=57,
   PID11_PARAMETER_KI=58,
   PID11_PARAMETER_KD=59,
   PID12_PARAMETER_KP=60,
   PID12_PARAMETER_KI=61,
   PID12_PARAMETER_KD=62,
   PID13_PARAMETER_KP=63,
   PID13_PARAMETER_KI=64,
   PID13_PARAMETER_KD=65,
   PID14_PARAMETER_KP=66,
   PID14_PARAMETER_KI=67,
   PID14_PARAMETER_KD=68,
   PID15_PARAMETER_KP=69,
   PID15_PARAMETER_KI=70,
   PID15_PARAMETER_KD=71,
   PID16_PARAMETER_KP=72,
   PID16_PARAMETER_KI=73,
   PID16_PARAMETER_KD=74,   
   PID17_PARAMETER_KP=75,
   PID17_PARAMETER_KI=76,
   PID17_PARAMETER_KD=77,
   PID18_PARAMETER_KP=78,
   PID18_PARAMETER_KI=79,
   PID18_PARAMETER_KD=80,   
   PID19_PARAMETER_KP=81,
   PID19_PARAMETER_KI=82,
   PID19_PARAMETER_KD=83,
   ESC_CALIBRATION_FLAG=84,
   HEADLESS_MODE_YAW=85,
   HOR_CAL_ACCEL_X=86,
   HOR_CAL_ACCEL_Y=87,
   HOR_CAL_ACCEL_Z=88,
	 SDK_MODE_DEFAULT=89,
	 GROUND_DISTANCE_DEFAULT=90
}FLIGHT_PARAMETER_TABLE;

extern FLIGHT_PARAMETER Table_Parameter;

uint32_t STMFLASH_ReadWord(uint32_t faddr);		  	//读出字  
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);   		//从指定地址开始读出指定长度的数据

void ReadFlashParameterALL(FLIGHT_PARAMETER *WriteData);
void ReadFlashParameterOne(uint16_t Label,float *ReadData);
void ReadFlashParameterTwo(uint16_t Label,float *ReadData1,float *ReadData2);
void ReadFlashParameterThree(uint16_t Label,float *ReadData1,float *ReadData2,float *ReadData3);
   
   
void WriteFlashParameter(uint16_t Label,float WriteData,FLIGHT_PARAMETER *Table);
void WriteFlashParameter_Two(uint16_t Label,
                         float WriteData1,
                         float WriteData2,
                         FLIGHT_PARAMETER *Table);
void WriteFlashParameter_Three(uint16_t Label,
                         float WriteData1,
                         float WriteData2,
                         float WriteData3,
                         FLIGHT_PARAMETER *Table);
void ReadFlashParameterTwo(uint16_t Label,float *ReadData1,float *ReadData2);

#endif

