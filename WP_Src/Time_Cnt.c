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
#include "Time_Cnt.h"
/***********************************************************
@函数名：Time_init
@入口参数：无
@出口参数：无
@功能描述：系统调度定时器初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Time_init(void)//系统调度定时器初始化
{
	Schedule_init();//定时器使能
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);							//定时器0使能				
  TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);						//32位周期定时器				
  TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/200);		//设定装载值,（80M/200）*1/80M=5ms				
  IntEnable(INT_TIMER0A);																		//总中断使能				
  TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT); 					//中断输出, 设置模式;			
  TimerIntRegister(TIMER0_BASE,TIMER_A,TIMER0A_Handler);		//中断函数注册
  IntMasterEnable();			
  TimerEnable(TIMER0_BASE,TIMER_A); 												//定时器使能开始计数
  IntPrioritySet(INT_TIMER0A,USER_INT7);
}

Sensor WP_Sensor;
Testime Time0_Delta;
/***********************************************************
@函数名：TIMER0A_Handler
@入口参数：无
@出口参数：无
@功能描述：系统调度定时器中断服务函数：主要进行遥控器解析、
传感数据采集、数字滤波、姿态解算、惯性导航、控制等对周期有
严格要求的函数
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void TIMER0A_Handler(void)				//系统调度中断函数
{
  Test_Period(&Time0_Delta);
  Remote_Control();								//遥控器数据解析
  Get_Status_Feedback();					//获取姿态数据、水平与竖直方向惯导数据
#if (Ground_Distance_Sensor==US100)	
		US_100_Statemachine();					//超声波传感器状态机更新
#else
		TFmini_Statemachine();          //激光TFMINI_PLUS传感器状态机更新
#endif	
  Optflow_Statemachine();					//光流状态机，初始化时存在光流外设
  SDK_Data_Prase();								//SDK数据解析
  GPS_Data_Prase();								//GPS数据解析
  KalmanFilter_Horizontal();			//水平位置GPS双观测量Kalman滤波融合	
  CarryPilot_Control();						//总控制器
	Accel_Calibration_Check();			//加速度校准检测
  Mag_Calibration_Check();				//磁力计校准检测
  ESC_Calibration_Check();				//电调行程校准检测
  Horizontal_Calibration_Check();	//机架水平校准检测
  Bling_Working(Bling_Mode);			//状态指示灯运行
  TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
}

