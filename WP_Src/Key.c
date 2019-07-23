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
#include "Key.h"
/***********************************************************
@函数名：Key_Init
@入口参数：无
@出口参数：无
功能描述：按键初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Key_Init(void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  // Unlock PF0 so we can change it to a GPIO input
  // Once we have enabled (unlocked) the commit register then re-lock it
  // to prevent further changes.  PF0 is muxed with NMI thus a special case.
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
  HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
  GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_2,GPIO_DIR_MODE_IN);//SW1
  GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_2,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
  GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_DIR_MODE_IN);//SW2
  GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}




int16_t Page_Number=0;
uint8_t Key_Right_Release=0;
/***********************************************************
@函数名：Key_Scan
@入口参数：release
@出口参数：bool
功能描述：按键扫描，入口参数release决定是否开放按键扫描权限
正常扫描返回TRUE，按键按下时为低电平，释放后IO配置的是上拉
输入模式，悬空为高电平
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
bool Key_Scan(uint8_t release)
{
  if(release==1)  return FALSE;
  if(QuadKey1==0)
  {
    delay_ms(10);//延时去抖
    if(QuadKey1==0)
    {
      while(QuadKey1==0);//等待按键释放
      Page_Number++;
      if(Page_Number>14) Page_Number=0;
      LCD_CLS();
    }
  }
  
  if(QuadKey2==0)
  {
    delay_ms(10);//延时去抖
    if(QuadKey2==0)
    {
      while(QuadKey2==0);//等待按键释放
      Page_Number--;
      if(Page_Number<0) Page_Number=14;
      LCD_CLS();
    }
  }
  return TRUE;
}



/***********************************************************
@函数名：QuadShow
@入口参数：无
@出口参数：无
功能描述：显示屏刷新，每一页每一行数据结合使用说明书来观看，
本函数翻页需结合按键扫描来实现
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void QuadShow()
{
  uint16_t i=0;
  if(Page_Number==0)
  {
    LCD_clear_L(0,0);  LCD_P6x8Str(0,0,"Basic");           write_6_8_number(40,0,Time0_Delta.Time_Delta);  								  write_6_8_number(70,0,Page_Number+1);write_6_8_number(90,0,Battery_Voltage);
    LCD_clear_L(0,1);  LCD_P6x8Str(0,1,"Yaw:");            write_6_8_number(40,1,Yaw);                                      write_6_8_number(90,1,Yaw_Gyro);
    LCD_clear_L(0,2);  LCD_P6x8Str(0,2,"Pitch:");          write_6_8_number(40,2,Pitch);                                    write_6_8_number(90,2,Pitch_Gyro);
    LCD_clear_L(0,3);  LCD_P6x8Str(0,3,"Roll:");           write_6_8_number(40,3,Roll);                            					write_6_8_number(90,3,Roll_Gyro);
    LCD_clear_L(0,4);  LCD_P6x8Str(0,4,"Baro:");           write_6_8_number(40,4,(uint32_t)(WP_Sensor.baro_pressure_raw));  write_6_8_number(90,4,WP_Sensor.baro_altitude);
    LCD_clear_L(0,5);  LCD_P6x8Str(0,5,"HC04:");           write_6_8_number(40,5,US_Distance);               			          write_6_8_number(80,5,NamelessQuad.Position[_YAW]);
    LCD_clear_L(0,6);  LCD_P6x8Str(0,6,"Sate_N:");         write_6_8_number(40,6,GPS_Sate_Num);                    					write_6_8_number(80,6,NamelessQuad.Speed[_YAW]);
    LCD_clear_L(0,7);  write_6_8_number(0,7,tfdata.distance);
		                                                       write_6_8_number(20,7,Gyro_Safety_Calibration_Flag);  
		write_6_8_number(30,7,WP_Sensor.temperature);          write_6_8_number(75,7,Origion_NamelessQuad.Acceleration[_YAW]);
		
  }
  else  if(Page_Number==1)
  {
    LCD_clear_L(0,0);  LCD_P6x8Str(0,0,"A_Scale & A_Off:");write_6_8_number(100,0,Page_Number+1);
    LCD_clear_L(0,1);  LCD_P6x8Str(0,1,"X:");              write_6_8_number(20,1,K[0]);                            write_6_8_number(80,1,B[0]);
    LCD_clear_L(0,2);  LCD_P6x8Str(0,2,"Y:");              write_6_8_number(20,2,K[1]);                            write_6_8_number(80,2,B[1]);
    LCD_clear_L(0,3);  LCD_P6x8Str(0,3,"Z:");              write_6_8_number(20,3,K[2]);                            write_6_8_number(80,3,B[2]);
    LCD_clear_L(0,4);  LCD_P6x8Str(0,4,"M_intensity:");    write_6_8_number(80,4,sqrt(mag_filter.x*mag_filter.x
																																										+ mag_filter.y*mag_filter.y
																																										+ mag_filter.z*mag_filter.z)/330);
    LCD_clear_L(0,5);  LCD_P6x8Str(0,5,"MX_Offset:");      write_6_8_number(80,5,Mag_Offset_Read.x);
    LCD_clear_L(0,6);  LCD_P6x8Str(0,6,"MY_Offset:");      write_6_8_number(80,6,Mag_Offset_Read.y);
    LCD_clear_L(0,7);  LCD_P6x8Str(0,7,"MZ_Offset:");      write_6_8_number(80,7,Mag_Offset_Read.z);
  }
  else  if(Page_Number==2)
  {
    LCD_clear_L(0,0);  LCD_P6x8Str(0,0,"RC Data");         write_6_8_number(90,0,Page_Number+1);
    LCD_clear_L(0,1);  LCD_P6x8Str(0,1,"Yaw:");            write_6_8_number(40,1,Yaw_Control);                     write_6_8_number(90,1,PPM_Databuf[3]);
    LCD_clear_L(0,2);  LCD_P6x8Str(0,2,"Pitch:");          write_6_8_number(40,2,Pitch_Control);                   write_6_8_number(90,2,PPM_Databuf[1]);
    LCD_clear_L(0,3);  LCD_P6x8Str(0,3,"Roll:");           write_6_8_number(40,3,Roll_Control);                    write_6_8_number(90,3,PPM_Databuf[0]);
    LCD_clear_L(0,4);  LCD_P6x8Str(0,4,"Thr:");            write_6_8_number(40,4,Throttle_Control);                write_6_8_number(90,4,PPM_Databuf[2]);
    LCD_clear_L(0,5);  LCD_P6x8Str(0,5,"ch5-6:");          write_6_8_number(40,5,PPM_Databuf[4]);                  write_6_8_number(90,5,PPM_Databuf[5]);
    LCD_clear_L(0,6);  LCD_P6x8Str(0,6,"ch7-8:");          write_6_8_number(40,6,PPM_Databuf[6]);                  write_6_8_number(90,6,PPM_Databuf[7]);
    
    LCD_clear_L(0,7);  LCD_P6x8Str(0,7,"mode");             
    if(Controler_High_Mode==1)        LCD_P6x8Str(30,7,"Manual_Ctrl_Alt");
    else if(Controler_High_Mode==2&&Controler_Horizontal_Mode==1)   LCD_P6x8Str(30,7,"Auto_Ctrl_Alt");
    else if(Controler_High_Mode==2&&Controler_Horizontal_Mode==2)   LCD_P6x8Str(30,7,"Auto_Ctrl_Hover");
    else LCD_P6x8Str(30,7,"Manual_Ctrl_Alt"); 
  }
  else if(Page_Number==3)
  {   
    LCD_clear_L(0,0);  LCD_P6x8Str(0,0,"Ctrl Data P  I");  write_6_8_number(100,0,Page_Number+1);
    LCD_clear_L(0,1);  LCD_P6x8Str(0,1,"Pit_Ang:");        write_6_8_number(50,1,Total_Controller.Pitch_Angle_Control.Kp);           write_6_8_number(90,1,Total_Controller.Pitch_Angle_Control.Ki); 
    LCD_clear_L(0,2);  LCD_P6x8Str(0,2,"Rol_Ang:");        write_6_8_number(50,2,Total_Controller.Roll_Angle_Control.Kp);            write_6_8_number(90,2,Total_Controller.Roll_Angle_Control.Ki);    
    LCD_clear_L(0,3);  LCD_P6x8Str(0,3,"Yaw_Ang:");        write_6_8_number(50,3,Total_Controller.Yaw_Angle_Control.Kp);             write_6_8_number(90,3,Total_Controller.Yaw_Angle_Control.Ki);    
    LCD_clear_L(0,4);  LCD_P6x8Str(0,4,"Pit_Gyr:");        write_6_8_number(50,4,Total_Controller.Pitch_Gyro_Control.Kp);            write_6_8_number(90,4,Total_Controller.Pitch_Gyro_Control.Ki);    
    LCD_clear_L(0,5);  LCD_P6x8Str(0,5,"Rol_Gyr:");        write_6_8_number(50,5,Total_Controller.Roll_Gyro_Control.Kp);             write_6_8_number(90,5,Total_Controller.Roll_Gyro_Control.Ki);    
    LCD_clear_L(0,6);  LCD_P6x8Str(0,6,"Yaw_Gyr:");        write_6_8_number(50,6,Total_Controller.Yaw_Gyro_Control.Kp);              write_6_8_number(90,6,Total_Controller.Yaw_Gyro_Control.Ki);
    LCD_clear_L(0,7);  LCD_P6x8Str(0,7,"Gyro_D:");         write_6_8_number(40,7,Total_Controller.Pitch_Gyro_Control.Kd);            write_6_8_number(70,7,Total_Controller.Roll_Gyro_Control.Kd);                  write_6_8_number(95,7,Total_Controller.Yaw_Gyro_Control.Kd);
  }
  else if(Page_Number==4)
  {
    LCD_clear_L(0,0);
    LCD_P6x8Str(0,0,"GPS_Date"); write_6_8_number(50,0,0);write_6_8_number(100,0,Page_Number+1);
    LCD_clear_L(0,1);
    LCD_P6x8Str(0,1,"Lo_deg:");
    write_6_8_number(50,1,Longitude_Deg);
    LCD_clear_L(0,2);
    LCD_P6x8Str(0,2,"d/100w:");
    write_6_8_number(50,2,Longitude_Min);
    
    LCD_clear_L(0,3);
    LCD_P6x8Str(0,3,"La_deg:");
    write_6_8_number(50,3,Latitude_Deg);
    LCD_clear_L(0,4);
    LCD_P6x8Str(0,4,"d/100w:");
    write_6_8_number(50,4,Latitude_Min);
    
    LCD_clear_L(0,5);
    if(GPS_Fix_Flag[0]==0x01)
      LCD_P6x8Str(0,5,"fix");
    else LCD_P6x8Str(0,5,"nof");
    
    if(GPS_FixType==0x03)
      LCD_P6x8Str(25,5,"3D");
    else if(GPS_FixType==0x02)
      LCD_P6x8Str(25,5,"2D");
    else LCD_P6x8Str(25,5,"0D");
    write_6_8_number(42,5,GPS_Sate_Num);
    write_6_8_number(60,5,TimeBeijing[0]);
    LCD_P6x8Str(75,5,":");
    write_6_8_number(84,5,TimeBeijing[1]);
    LCD_P6x8Str(100,5,":");
    write_6_8_number(105,5,TimeBeijing[2]);
    LCD_clear_L(0,6);
    LCD_P6x8Str(0,6,"V:");
    write_6_8_number(10,6,GPS_Vel.N);
    write_6_8_number(65,6,GPS_Vel.E);
    LCD_clear_L(0,7);
    write_6_8_number(0,7,GPS_Vel.U);
    write_6_8_number(80,7,GPS_Quality);
  }
  else if(Page_Number==5)
  {
    LCD_clear_L(0,0);  LCD_P6x8Str(0,0,"Optical");  			write_6_8_number(100,0,Page_Number+1);
    LCD_clear_L(0,1);  LCD_P6x8Str(0,1,"flow_x:");        write_6_8_number(50,1,opt_origin_data.pixel_flow_x_integral);             write_6_8_number(90,1,opt_gyro_data.x); 
    LCD_clear_L(0,2);  LCD_P6x8Str(0,2,"flow_y:");        write_6_8_number(50,2,opt_origin_data.pixel_flow_y_integral);             write_6_8_number(90,2,opt_gyro_data.y);    
    LCD_clear_L(0,3);  LCD_P6x8Str(0,3,"time:");       		write_6_8_number(50,3,opt_origin_data.integration_timespan);              write_6_8_number(90,3,opt_data.dt);    
    LCD_clear_L(0,4);  LCD_P6x8Str(0,4,"qual:");      		write_6_8_number(50,4,opt_origin_data.qual);            									  
    LCD_clear_L(0,5);  LCD_P6x8Str(0,5,"gyro_x:");        write_6_8_number(50,5,opt_gyro_data.x);              
    LCD_clear_L(0,6);  LCD_P6x8Str(0,6,"gyro_y:");        write_6_8_number(50,6,opt_gyro_data.y);           
    LCD_clear_L(0,7);  LCD_P6x8Str(0,7,"opt_flow:");      write_6_8_number(40,7,opt_gyro_filter_data.x);            write_6_8_number(70,7,opt_gyro_filter_data.y);                
  }
  else if(Page_Number==6)
  {
    uint16_t x1,x2;
    x1=(uint16_t)(-32*FastTan(SDK_Target_Yaw_Gyro*DEG2RAD)+128*(80-SDK_Line.x)/80);//((SDK_Line.x-40)/80);//SDK_Target_Yaw_Gyro
    x2=(uint16_t)(32*FastTan(SDK_Target_Yaw_Gyro*DEG2RAD)+128*(80-SDK_Line.x)/80);
    ssd1306_clear_display();
    ssd1306_draw_line(x1,0,x2,63,WHITE);
    ssd1306_display();
    LCD_P6x8Str(0,0,"line_x:");  write_6_8_number(65,0,SDK_Line.x);
    LCD_P6x8Str(0,1,"line_ang:");write_6_8_number(65,1,SDK_Line.line_angle);
    LCD_P6x8Str(0,2,"line_fg:"); write_6_8_number(65,2,SDK_Line.up_ok);write_6_8_number(75,2,SDK_Line.down_ok);
    write_6_8_number(90,2,SDK_Line.left_ok);write_6_8_number(105,2,SDK_Line.right_ok);
    LCD_P6x8Str(0,3,"Tar_x:");   write_6_8_number(65,3,SDK_Target.x);
    LCD_P6x8Str(0,4,"Tar_a:");   write_6_8_number(65,4,SDK_Target_Yaw_Gyro);
  }
  else if(Page_Number==7)
  {
    ssd1306_clear_display();
    ssd1306_draw_line(0,32,128,32,WHITE);
    ssd1306_draw_line(64,0,64,64,WHITE);
    
    //uint16_t x=0,y=0;
    //y=32+32*constrain_float(SDK_Target.y,-100,100)/100;
    //x=64-32*constrain_float(SDK_Target.x,-100,100)/100; 
    //ssd1306_fill_circle(x,y,20*constrain_float(SDK_Point.Pixel,200*SDK_Point.flag,1000)/1000,WHITE);
    ssd1306_display();
    LCD_P6x8Str(0,0,"point_x:");  write_6_8_number(95,0,SDK_Point.x);
    LCD_P6x8Str(0,1,"point_y::"); write_6_8_number(95,1,SDK_Point.y);
    LCD_P6x8Str(0,2,"point_s:");  write_6_8_number(95,2,SDK_Point.Pixel);
    LCD_P6x8Str(0,3,"point_f:");  write_6_8_number(95,3,SDK_Point.flag);
    LCD_P6x8Str(0,4,"Tar_x:");    write_6_8_number(95,4,SDK_Target.x);
    LCD_P6x8Str(0,5,"Tar_y:");    write_6_8_number(95,5,SDK_Target.y);  
  }
  else if(Page_Number==8)
  {
    uint16_t x,y,x1,y1;
    ssd1306_clear_display();
    ssd1306_draw_line(32,32,96,32,WHITE);
    ssd1306_draw_line(64,0,64,64,WHITE);
    ssd1306_draw_circle(64,32,32,WHITE);
    x1=64-32*Sin_Yaw;
    y1=32-32*Cos_Yaw;
    ssd1306_draw_line(64,32,x1,y1,WHITE);
    y=32+32*constrain_float(Pitch,-90,90)/90;
    x=64+32*constrain_float(Roll,-90,90)/90;
    ssd1306_fill_circle(x,y,5,WHITE);
    ssd1306_display();
    LCD_P6x8Str(60,0,"N");
    LCD_P6x8Str(60,7,"S");
    LCD_P6x8Str(0,1,"Yaw:");            write_6_8_number(90,1,Yaw);                             
    LCD_P6x8Str(0,2,"Pitch:");          write_6_8_number(90,2,Pitch); 
    LCD_P6x8Str(0,3,"Roll:");           write_6_8_number(90,3,Roll);  
  }
  else if(Page_Number==9)
  {
    LCD_clear_L(0,0); LCD_clear_L(0,1);  
    LCD_P8x16Str(32,0,"SDK_Mode:");write_6_8_number(110,1,SDK_Mode_Set);
    
    LCD_clear_L(0,2); LCD_clear_L(0,3);
    if(SDK_Mode_Set==0x00)     LCD_P8x16Str(0,2,"Default Mode");//默认模式
    else if(SDK_Mode_Set==0x01)     LCD_P8x16Str(32,2,"Point Mode");
    else if(SDK_Mode_Set==0x02)     LCD_P8x16Str(32,2,"Line Mode");
    
    LCD_clear_L(0,4); LCD_clear_L(0,5);  
    LCD_P8x16Str(32,4,"Now_Mode:");
    
    LCD_clear_L(0,6); LCD_clear_L(0,7); 
    if(SDK_Now_Mode==0x01)         LCD_P8x16Str(32,6,"Point Mode");
    else if(SDK_Now_Mode==0x02)      LCD_P8x16Str(32,6,"Line Mode");
    
    if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
    {
      delay_ms(10);
      if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
      {
        while(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max);
        SDK_Mode_Set++;
        if(SDK_Mode_Set>2) SDK_Mode_Set=1;
        SDK_DT_Send_Check(SDK_Mode_Set);
        WriteFlashParameter(SDK_MODE_DEFAULT,
                            SDK_Mode_Set,
                            &Table_Parameter);
      }
    }
    
    if(Pitch_Control<=-Scale_Pecent_Max*Pit_Rol_Max)
    {
      delay_ms(10);
      if(Pitch_Control<=-Scale_Pecent_Max*Pit_Rol_Max)
      {
        while(Pitch_Control<=-Scale_Pecent_Max*Pit_Rol_Max);
        SDK_Mode_Set--;
        if(SDK_Mode_Set<1) SDK_Mode_Set=2;
        SDK_DT_Send_Check(SDK_Mode_Set);
        WriteFlashParameter(SDK_MODE_DEFAULT,
                            SDK_Mode_Set,
                            &Table_Parameter);
      }
    } 
  }
  else if(Page_Number==10)
  {
    LCD_clear_L(0,0);
    LCD_P6x8Str(10,0,"Accel_Correct");
    write_6_8_number(100,0,flight_direction);write_6_8_number(110,0,Page_Number+1);
    LCD_clear_L(0,1);
    write_6_8_number(0,1,acce_sample[0].x);
    write_6_8_number(40,1,acce_sample[0].y);
    write_6_8_number(70,1,acce_sample[0].z);
    LCD_clear_L(0,2);
    write_6_8_number(0,2,acce_sample[1].x);
    write_6_8_number(40,2,acce_sample[1].y);
    write_6_8_number(70,2,acce_sample[1].z);
    LCD_clear_L(0,3);
    write_6_8_number(0,3,acce_sample[2].x);
    write_6_8_number(40,3,acce_sample[2].y);
    write_6_8_number(70,3,acce_sample[2].z);
    LCD_clear_L(0,4);
    write_6_8_number(0,4,acce_sample[3].x);
    write_6_8_number(40,4,acce_sample[3].y);
    write_6_8_number(70,4,acce_sample[3].z);
    LCD_clear_L(0,5);
    write_6_8_number(0,5,acce_sample[4].x);
    write_6_8_number(40,5,acce_sample[4].y);
    write_6_8_number(70,5,acce_sample[4].z);
    LCD_clear_L(0,6);
    write_6_8_number(0,6,acce_sample[5].x);
    write_6_8_number(40,6,acce_sample[5].y);
    write_6_8_number(70,6,acce_sample[5].z);
  }
  else if(Page_Number==11)
  {
    LCD_clear_L(0,0);
    LCD_P6x8Str(10,0,"Mag_Correct");
    write_6_8_number(90,0,Mag_Calibration_Mode);write_6_8_number(100,0,Page_Number+1);
    LCD_clear_L(0,1);
    write_6_8_number(0,1, (int16_t)(WP_Sensor.accel_raw.x));
    write_6_8_number(40,1,(int16_t)(WP_Sensor.accel_raw.y));
    write_6_8_number(70,1,(int16_t)(WP_Sensor.accel_raw.z));
    LCD_clear_L(0,2);
    LCD_P6x8Str(0,2,"0 To 360");
    write_6_8_number(70,2,Yaw_Correct);
    LCD_clear_L(0,3);
    LCD_P6x8Str(0,3,"Mag Is Okay?");
    write_6_8_number(80,3,Mag_Is_Okay_Flag[0]);
    write_6_8_number(90,3,Mag_Is_Okay_Flag[1]);
    write_6_8_number(105,3,Mag_Is_Okay_Flag[2]);
    LCD_clear_L(0,4);
    for(i=0;i<12;i++)
    {
      write_6_8_number(10*i,4,Mag_360_Flag[0][3*i]);
    }
    LCD_clear_L(0,5);
    for(i=0;i<12;i++)
    {
      write_6_8_number(10*i,5,Mag_360_Flag[1][3*i]);
    }
    LCD_clear_L(0,6);
    for(i=0;i<12;i++)
    {
      write_6_8_number(10*i,6,Mag_360_Flag[2][3*i]);
    }
    LCD_clear_L(0,7);
    if(Mag_Calibration_Mode==0) LCD_P6x8Str(0,7,"Make Z+ Upside Sky");
    else if(Mag_Calibration_Mode==1) LCD_P6x8Str(0,7,"Make Y+ Upside Sky");
    else if(Mag_Calibration_Mode==2) LCD_P6x8Str(0,7,"Make X+ Upside Sky");
    else if(Mag_Calibration_Mode==3) LCD_P6x8Str(0,7,"Start With Yaw Move"); 
  }
  else if(Page_Number==12)
  {
    LCD_clear_L(0,0);
    LCD_P6x8Str(10,0,"Sensor Data");   write_6_8_number(100,0,Page_Number+1);
    LCD_clear_L(0,2);LCD_P6x8Str(0,2,"mag:");
    write_6_8_number(30,2,WP_Sensor.mag_raw.x);
    write_6_8_number(65,2,WP_Sensor.mag_raw.y);
    write_6_8_number(100,2,WP_Sensor.mag_raw.z);
    LCD_clear_L(0,3);LCD_P6x8Str(0,3,"acc:");
    write_6_8_number(30,3,WP_Sensor.accel_raw.x);
    write_6_8_number(65,3,WP_Sensor.accel_raw.y);
    write_6_8_number(100,3,WP_Sensor.accel_raw.z);
    LCD_clear_L(0,4);LCD_P6x8Str(0,4,"gyr:");
    write_6_8_number(30,4,WP_Sensor.gyro_raw.x);
    write_6_8_number(65,4,WP_Sensor.gyro_raw.y);
    write_6_8_number(100,4,WP_Sensor.gyro_raw.z);
    LCD_clear_L(0,5);LCD_P6x8Str(0,5,"bar:");
    write_6_8_number(30,5,WP_Sensor.baro_pressure_raw);
    write_6_8_number(65,5,WP_Sensor.baro_temp_raw);
		LCD_clear_L(0,6);LCD_P6x8Str(0,6,"opt:");
		write_6_8_number(30,6,opt_origin_data.pixel_flow_x_integral);
		write_6_8_number(65,6,opt_origin_data.pixel_flow_y_integral);
		LCD_clear_L(0,7);LCD_P6x8Str(0,7,"tfm:");
		write_6_8_number(30,7,tfdata.distance);
		write_6_8_number(65,7,tfdata.strength);
		write_6_8_number(100,7,tfdata.temperature);
  }
	else if(Page_Number==13)
  {
    LCD_clear_L(0,0); LCD_clear_L(0,1);  
    LCD_P8x16Str(5,0,"Ground_Sensor");write_6_8_number(110,1,Ground_Sensor_Now_Mode);
    
    LCD_clear_L(0,2); LCD_clear_L(0,3);
		LCD_P8x16Str(5,2,"Now_Mode");
		
    LCD_clear_L(0,4); LCD_clear_L(0,5);  
    if(Ground_Sensor_Now_Mode==0x01)         	 LCD_P8x16Str(5,4,"US100 Mode");
    else if(Ground_Sensor_Now_Mode==0x02)      LCD_P8x16Str(5,4,"tfmini Mode");
    
    LCD_clear_L(0,6); LCD_clear_L(0,7); 
    if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
    {
      delay_ms(10);
      if(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max)
      {
        while(Pitch_Control>=Scale_Pecent_Max*Pit_Rol_Max);
        Ground_Sensor_Now_Mode++;
        if(Ground_Sensor_Now_Mode>2) Ground_Sensor_Now_Mode=1;
        WriteFlashParameter(GROUND_DISTANCE_DEFAULT,
                            Ground_Sensor_Now_Mode,
                            &Table_Parameter);
				ConfigureUART7();
      }
    }
    
    if(Pitch_Control<=-Scale_Pecent_Max*Pit_Rol_Max)
    {
      delay_ms(10);
      if(Pitch_Control<=-Scale_Pecent_Max*Pit_Rol_Max)
      {
        while(Pitch_Control<=-Scale_Pecent_Max*Pit_Rol_Max);
        Ground_Sensor_Now_Mode--;
        if(Ground_Sensor_Now_Mode<1) Ground_Sensor_Now_Mode=2;
        WriteFlashParameter(GROUND_DISTANCE_DEFAULT,
                            Ground_Sensor_Now_Mode,
                            &Table_Parameter);
				ConfigureUART7();
      }
    } 
		LCD_clear_L(0,6);LCD_P6x8Str(0,6,"tfmini:");  							LCD_P6x8Str(75,6,"us100:");
		LCD_clear_L(0,7);write_6_8_number(0,7,tfdata.distance);     write_6_8_number(75,7,US_Distance);
  }
  else if(Page_Number==14)
  {
    LCD_clear_L(0,0);  LCD_P6x8Str(0,0,"ch1:");write_6_8_number(25,0,PPM_Databuf[0]);write_6_8_number(55,0,RC_Calibration[0].max);write_6_8_number(90,0,RC_Calibration[0].min);
    LCD_clear_L(0,1);  LCD_P6x8Str(0,1,"ch2:");write_6_8_number(25,1,PPM_Databuf[1]);write_6_8_number(55,1,RC_Calibration[1].max);write_6_8_number(90,1,RC_Calibration[1].min);
    LCD_clear_L(0,2);  LCD_P6x8Str(0,2,"ch3:");write_6_8_number(25,2,PPM_Databuf[2]);write_6_8_number(55,2,RC_Calibration[2].max);write_6_8_number(90,2,RC_Calibration[2].min);
    LCD_clear_L(0,3);  LCD_P6x8Str(0,3,"ch4:");write_6_8_number(25,3,PPM_Databuf[3]);write_6_8_number(55,3,RC_Calibration[3].max);write_6_8_number(90,3,RC_Calibration[3].min);
    LCD_clear_L(0,4);  LCD_P6x8Str(0,4,"ch5:");write_6_8_number(25,4,PPM_Databuf[4]);write_6_8_number(55,4,RC_Calibration[4].max);write_6_8_number(90,4,RC_Calibration[4].min);
    LCD_clear_L(0,5);  LCD_P6x8Str(0,5,"ch6:");write_6_8_number(25,5,PPM_Databuf[5]);write_6_8_number(55,5,RC_Calibration[5].max);write_6_8_number(90,5,RC_Calibration[5].min);
    LCD_clear_L(0,6);  LCD_P6x8Str(0,6,"ch7:");write_6_8_number(25,6,PPM_Databuf[6]);write_6_8_number(55,6,RC_Calibration[6].max);write_6_8_number(90,6,RC_Calibration[6].min);
    LCD_clear_L(0,7);  LCD_P6x8Str(0,7,"ch8:");write_6_8_number(25,7,PPM_Databuf[7]);write_6_8_number(55,7,RC_Calibration[7].max);write_6_8_number(90,7,RC_Calibration[7].min);
  }
  else if(Page_Number==15)
  {
    LCD_clear_L(0,0);LCD_clear_L(0,1);LCD_P8x16Str(0,0,"Please Move Thr");
    LCD_clear_L(0,2);LCD_clear_L(0,3);LCD_P8x16Str(0,2,"Up And Pull Out");
    LCD_clear_L(0,4);LCD_clear_L(0,5);LCD_P8x16Str(0,4,"The Power Supply");
    LCD_clear_L(0,6);LCD_clear_L(0,7);LCD_P8x16Str(0,6,"ESC Calibration");
  }
	else if(Page_Number==16)
  {
    Copyright_Show();
  }
  
}

