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
#include "I2C.h"
#include "WP_DataType.h"
#include "IST8310.h"



Vector2f magn={0,0};
Vector3f mag_offset;
Butter_Parameter Mag_Parameter;
/***********************************************************
@函数名：IST8310_Init
@入口参数：无
@出口参数：无
功能描述：磁力计初始化配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void IST8310_Init(void)//磁力计初始化配置
{
	Single_WriteI2C(IST8310_SLAVE_ADDRESS,0x41,0x24);//开启16x内部平均
	Single_WriteI2C(IST8310_SLAVE_ADDRESS,0x42,0xC0);//Set/Reset内部平均
  Set_Cutoff_Frequency(40, 18,&Mag_Parameter);//传感器校准加计滤波值
}

/***********************************************************
@函数名：IST8310_Read_Data
@入口参数：vector3f *mag
@出口参数：无
功能描述：磁力计数据采集状态机
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
uint8_t IST8310_Read_Data(vector3f *mag)//读取磁力计数据状态机
{
  static uint16_t ist8310_sampling_cnt=0;
  uint8_t buf[6];
  ist8310_sampling_cnt++;
  if(ist8310_sampling_cnt==1)
  {
    Single_WriteI2C(IST8310_SLAVE_ADDRESS,IST8310_REG_CNTRL1,0x01);//Single Measurement Mode	
  }
  else if(ist8310_sampling_cnt==6)
  {
    buf[0]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x03);//OUT_X_L_A
    buf[1]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x04);//OUT_X_H_A
    buf[2]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x05);//OUT_Y_L_A
    buf[3]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x06);//OUT_Y_H_A
    buf[4]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x07);//OUT_Z_L_A
    buf[5]=Single_ReadI2C(IST8310_SLAVE_ADDRESS,0x08);//OUT_Z_H_A
    mag->x= (int16_t)((buf[1]<<8)|buf[0]);
    mag->y=-(int16_t)((buf[3]<<8)|buf[2]);
    mag->z= (int16_t)((buf[5]<<8)|buf[4]);
    ist8310_sampling_cnt=0;
    return 1;
  }
  return 0;
}



