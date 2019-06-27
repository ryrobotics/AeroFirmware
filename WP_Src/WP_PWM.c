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
#include "WP_PWM.h"
#define PWM_Period_MAX  3125//2.5ms————400hz
static uint16_t period;
/***********************************************************
@函数名：Init_PWM
@入口参数：无
@出口参数：无
@功能描述：PWM初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Init_PWM(void)
{
  SysCtlPWMClockSet(SYSCTL_PWMDIV_64); // Set divider to 80M/8=10M=0.1us
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0); // Enable PWM peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // Enable GPIOB peripheral
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC); // Enable GPIOC peripheral
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // Enable GPIOE peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  // Use alternate function
  GPIOPinConfigure(GPIO_PB6_M0PWM0);
  GPIOPinConfigure(GPIO_PB7_M0PWM1);
  GPIOPinConfigure(GPIO_PB4_M0PWM2);
  GPIOPinConfigure(GPIO_PB5_M0PWM3);
  
  GPIOPinConfigure(GPIO_PE4_M0PWM4);
  GPIOPinConfigure(GPIO_PE5_M0PWM5);
  GPIOPinConfigure(GPIO_PC4_M0PWM6);
  GPIOPinConfigure(GPIO_PC5_M0PWM7);
  // Use pin with PWM peripheral
  GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);//M0PWM0
  GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);//M0PWM1
  GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);//M0PWM2
  GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);//M0PWM3
  
  GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);//M0PWM4
  GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);//M0PWM5
  GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);//M0PWM6
  GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);//M0PWM7
  // Configure the PWM generator for count down mode with immediate updates to the parameters
  PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  
  PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  // The period is set to 2.5ms (400 Hz)
  period = PWM_Period_MAX; 
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, period); // Set the period
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, period);
  
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, period); // Set the period
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, period);
  // Start the timers in generator 0 and 1
  PWMGenEnable(PWM0_BASE, PWM_GEN_0);
  PWMGenEnable(PWM0_BASE, PWM_GEN_1);
  
  PWMGenEnable(PWM0_BASE, PWM_GEN_2);
  PWMGenEnable(PWM0_BASE, PWM_GEN_3);
  // Enable the outputs
  PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT
                 | PWM_OUT_4_BIT | PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);
  PWM_Output(1250,1250,1250,1250,1250,1250,1250,1250);
}


/***********************************************************
@函数名：PWM_Output
@入口参数：uint16_t width1,uint16_t width2,uint16_t width3,
uint16_t width4,uint16_t width5,uint16_t width6,
uint16_t width7,uint16_t width8
@出口参数：无
@功能描述：PWM输出
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void PWM_Output(uint16_t width1,uint16_t width2,uint16_t width3,uint16_t width4,
                uint16_t width5,uint16_t width6,uint16_t width7,uint16_t width8)
{
  PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6,width1);//PC4 
  PWMPulseWidthSet(PWM0_BASE,PWM_OUT_7,width2);//PC5  
  PWMPulseWidthSet(PWM0_BASE,PWM_OUT_1,width3);//PB7
  PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,width4);//PB6
  PWMPulseWidthSet(PWM0_BASE,PWM_OUT_5,width5);//PE5
  PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,width6);//P43
  PWMPulseWidthSet(PWM0_BASE,PWM_OUT_2,width7);//PB4
  PWMPulseWidthSet(PWM0_BASE,PWM_OUT_3,width8);//PB5
}


