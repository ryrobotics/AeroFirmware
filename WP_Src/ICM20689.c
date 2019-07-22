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
#include "myiic.h"
/*
                       _oo0oo_
                      o8888888o
                      88" . "88
                      (| -_- |)
                      0\  =  /0
                    ___/`---'\___
                  .' \\|     |// '.
                 / \\|||  :  |||// \
                / _||||| -:- |||||- \
               |   | \\\  -  /// |   |
               | \_|  ''\---/''  |_/ |
               \  .-\__  '-'  ___/-. /
             ___'. .'  /--.--\  `. .'___
          ."" '<  `.___\_<|>_/___.' >' "".
         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
         \  \ `_.   \_ __\ /__ _/   .-` /  /
     =====`-.____`.___ \_____/___.-`___.-'=====
                       `=---='
*/

//-----------------------------------------------------
#define ICM20689_ADDRESS 0x68    // AD0 should be low

#define ICM20689_SELF_TEST_X_GYRO  0x00
#define ICM20689_SELF_TEST_Y_GYRO  0x01 
#define ICM20689_SELF_TEST_Z_GYRO  0x02
#define ICM20689_SELF_TEST_X_ACCEL 0x0D	 
#define ICM20689_SELF_TEST_Y_ACCEL 0x0E	 
#define ICM20689_SELF_TEST_Z_ACCEL 0x0F	 
#define ICM20689_XG_OFFS_USRH      0x13	
#define ICM20689_XG_OFFS_USRL      0x14
#define ICM20689_YG_OFFS_USRH      0x15
#define ICM20689_YG_OFFS_USRL      0x16
#define ICM20689_ZG_OFFS_USRH      0x17
#define ICM20689_ZG_OFFS_USRL      0x18
#define ICM20689_SMPLRT_DIV        0x19
#define ICM20689_CONFIG            0x1A
#define ICM20689_GYRO_CONFIG       0x1B
#define ICM20689_ACCEL_CONFIG      0x1C
#define ICM20689_ACCEL_CONFIG_2    0x1D

#define ICM20689_ACCEL_XOUT_H      0x3B
#define ICM20689_ACCEL_XOUT_L      0x3C
#define ICM20689_ACCEL_YOUT_H      0x3D
#define ICM20689_ACCEL_YOUT_L      0x3E
#define ICM20689_ACCEL_ZOUT_H      0x3F
#define ICM20689_ACCEL_ZOUT_L      0x40
#define ICM20689_TEMP_OUT_H        0x41
#define ICM20689_TEMP_OUT_L        0x42
#define ICM20689_GYRO_XOUT_H       0x43
#define ICM20689_GYRO_XOUT_L       0x44
#define ICM20689_GYRO_YOUT_H       0x45
#define ICM20689_GYRO_YOUT_L       0x46
#define ICM20689_GYRO_ZOUT_H       0x47
#define ICM20689_GYRO_ZOUT_L       0x48

#define ICM20689_PWR_MGMT_1        0x6B
#define ICM20689_PWR_MGMT_2        0x6C
#define ICM20689_WHO_AM_I          0x75
/* USER CODE END Private defines */


uint8_t ICM20689_Read_Reg(uint8_t reg)
{												
	return i2cRead(ICM20689_ADDRESS,reg);
}

void ICM20689_Write_Reg(uint8_t reg,uint8_t value)
{
  i2cWrite(MPU_ADRESS,reg,value);
}


uint8_t Init_ICM20689(void)
{	
	if(ICM20689_Read_Reg(ICM20689_WHO_AM_I)==0x98)
	{
		//printf("ICM20689 ready\n");
	}
	else
	{
		//printf("ICM20689 error\n");
		return 1;
	}
	ICM20689_Write_Reg(ICM20689_PWR_MGMT_1, 0x00);	//解除休眠状态
	ICM20689_Write_Reg(ICM20689_CONFIG, 0x07);      //低通滤波频率，典型值：0x07(3600Hz)此寄存器内决定Internal_Sample_Rate==8K
	
/*******************Init GYRO and ACCEL******************************/	
	ICM20689_Write_Reg(ICM20689_SMPLRT_DIV, 0x00);  //陀螺仪采样率，典型值：0x07(1kHz) (SAMPLE_RATE= Internal_Sample_Rate / (1 + SMPLRT_DIV) )
	ICM20689_Write_Reg(ICM20689_GYRO_CONFIG, 0x18); //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	ICM20689_Write_Reg(ICM20689_ACCEL_CONFIG, 0x18);//加速计自检、测量范围及高通滤波频率，典型值：0x18(不自检，16G)
	ICM20689_Write_Reg(ICM20689_ACCEL_CONFIG_2, 0x08);//加速计高通滤波频率 典型值 ：0x08  （1.13kHz）	
	return 0;
}

