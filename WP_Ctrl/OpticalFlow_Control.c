
#include "Headfile.h"
#include "OpticalFlow_Control.h"

extern Vector2f OpticalFlow_Position;
extern Vector2f OpticalFlow_Speed;
extern Vector2f OpticalFlow_Speed_Err;
extern Vector2f OpticalFlow_Position_Err;

Vector2f OpticalFlow_Pos_Ctrl_Expect={0};
Vector2f OpticalFlow_Pos_Ctrl_Err={0};
Vector2f OpticalFlow_Pos_Ctrl_Integrate={0};
Vector2f OpticalFlow_Pos_Ctrl_Output={0};

Vector2f OpticalFlow_Ctrl_Expect={0};
Vector2f OpticalFlow_Ctrl_Err={0};
Vector2f OpticalFlow_Ctrl_Integrate={0};
Vector2f OpticalFlow_Ctrl_Output={0};

void OpticalFlow_SINS_Reset(void)
{ 
  OpticalFlow_SINS.Position[_PITCH]=0;
  OpticalFlow_SINS.Speed[_PITCH]=0;
  OpticalFlow_SINS.Position[_ROLL]=0;
  OpticalFlow_SINS.Speed[_ROLL]=0;
  OpticalFlow_Position.x=0;
  OpticalFlow_Position.y=0;
}


void OpticalFlow_Ctrl_Reset(void)
{ 
  OpticalFlow_Ctrl_Integrate.x=0.0f;
  OpticalFlow_Ctrl_Integrate.y=0.0f;
  OpticalFlow_Pos_Ctrl_Integrate.x=0.0f;
  OpticalFlow_Pos_Ctrl_Integrate.y=0.0f;
  OpticalFlow_Pos_Ctrl_Expect.x=0;
  OpticalFlow_Pos_Ctrl_Expect.y=0;
}

void SDK_Pos_Ctrl_Reset(void)
{ 
	Total_Controller.SDK_Pitch_Position_Control.Integrate=0;
	Total_Controller.SDK_Pitch_Position_Control.Last_Err=0;
	Total_Controller.SDK_Pitch_Position_Control.Err=0;
	Total_Controller.SDK_Pitch_Position_Control.Last_Err_LPF=0;		

	Total_Controller.SDK_Roll_Position_Control.Integrate=0;
	Total_Controller.SDK_Roll_Position_Control.Last_Err=0;
	Total_Controller.SDK_Roll_Position_Control.Err=0;
	Total_Controller.SDK_Roll_Position_Control.Last_Err_LPF=0;
}
float OpticalFlow_Expect_Speed_Mapping(float input,uint16_t input_max,float output_max)
{
  float output_speed=0;
  float temp_scale=(float)(input/input_max);
  temp_scale=constrain_float(temp_scale,-1.0f, 1.0f);
  if(temp_scale>=0) output_speed=(float)(output_max*temp_scale*temp_scale);
  else output_speed=(float)(-output_max*temp_scale*temp_scale); 
  return output_speed;
}

void OpticalFlow_Set_Target_Point(Vector2f target)
{
  OpticalFlow_Pos_Ctrl_Expect.x=target.x;
  OpticalFlow_Pos_Ctrl_Expect.y=target.y;
}

void OpticalFlow_Set_Target_Vel(Vector2f target)
{
  OpticalFlow_Ctrl_Expect.x=target.x;
  OpticalFlow_Ctrl_Expect.y=target.y;
}
void OpticalFlow_Control(uint8_t force_brake_flag)
{
  static uint16_t SDK_Ctrl_Cnt=0;
  if(SDK_Ctrl_Mode==0)//SDK模式下，无数据接收，正常光流模式
  {
    if(Roll_Control==0
       &&Pitch_Control==0)//无水平遥控量给定
    {
      /**************************光流位置控制器************************************/
      if(OpticalFlow_Pos_Ctrl_Expect.x==0
         &&OpticalFlow_Pos_Ctrl_Expect.y==0)
      {
        if(force_brake_flag==1||(rMat[2][2]>=0.95f
                                 &&pythagorous2(OpticalFlow_SINS.Speed[_PITCH],OpticalFlow_SINS.Speed[_ROLL])<=40))//打杆回中后，根据当前速度、倾角判断是否进行悬停
        {
          OpticalFlow_Pos_Ctrl_Expect.x=OpticalFlow_SINS.Position[_PITCH];
          OpticalFlow_Pos_Ctrl_Expect.y=OpticalFlow_SINS.Position[_ROLL];
        }
        else  //打杆回中未满足悬停条件时，只进行速度控制 
        {
          OpticalFlow_Pos_Ctrl_Output.x=0;
          OpticalFlow_Pos_Ctrl_Output.y=0;
        }
      }
      else  OpticalFlow_Pos_Control();
      
      /***********当只需要速度控制时，开启以下注释，仅限调试时用*************/
      //OpticalFlow_Pos_Ctrl_Output.x=0;
      //OpticalFlow_Pos_Ctrl_Output.y=0;
      /**************************基于模型的加速度-姿态角映射，相比直接给姿态，参数差异大概在20倍左右************************************/
      OpticalFlow_Vel_Control(OpticalFlow_Pos_Ctrl_Output);//速度期望
    }
    else
    {
      OpticalFlow_Pos_Ctrl_Output.x=OpticalFlow_Expect_Speed_Mapping(-Target_Angle[1],Pit_Rol_Max,OpticalFlow_Speed_Control_Max);
      OpticalFlow_Pos_Ctrl_Output.y=OpticalFlow_Expect_Speed_Mapping(-Target_Angle[0],Pit_Rol_Max,OpticalFlow_Speed_Control_Max); 
      if(OpticalFlow_Speed_Control_Mode==1)
      {
        OpticalFlow_Vel_Control(OpticalFlow_Pos_Ctrl_Output);//速度期望
      }
      else
      {
        Total_Controller.Pitch_Angle_Control.Expect=Target_Angle[0];
        Total_Controller.Roll_Angle_Control.Expect=Target_Angle[1];      
      }
      OpticalFlow_Pos_Ctrl_Expect.x=0;
      OpticalFlow_Pos_Ctrl_Expect.y=0;
    }
    PID_LPF_Reset(&Total_Controller.SDK_Roll_Position_Control,SDK_Roll_Position_Controler);
    PID_LPF_Reset(&Total_Controller.SDK_Pitch_Position_Control,SDK_Pitch_Position_Controler);
  }
  else//SDK接收模式
  {
    if(Roll_Control==0
       &&Pitch_Control==0)//无水平遥控量给定
    {  
      if(SDK_Line.line_ctrl_enable==1)//线检测
      {
        /***********当只需要速度控制时，开启以下注释，仅限调试时用*************/
        SDK_Ctrl_Cnt++;
        if(SDK_Ctrl_Cnt>=4)//20ms
        {
          Total_Controller.SDK_Roll_Position_Control.Expect=0;
          Total_Controller.SDK_Roll_Position_Control.FeedBack=SDK_Target.x;
          PID_Control_SDK_Err_LPF(&Total_Controller.SDK_Roll_Position_Control,1);   
          accel_target.x=constrain_float(Total_Controller.SDK_Roll_Position_Control.Control_OutPut,
                                         -Total_Controller.Optical_Speed_Control.Control_OutPut_Limit,
                                         Total_Controller.Optical_Speed_Control.Control_OutPut_Limit);                             
          Total_Controller.Roll_Angle_Control.Expect=constrain_float(fast_atan(accel_target.x/(GRAVITY_MSS*100))*RAD2DEG,-30,30);//roll                                
          SDK_Ctrl_Cnt=0;
        }
        //if(ABS(SDK_Target_Yaw_Gyro)<=10) OpticalFlow_Y_Vel_Control(8);
        //else if(ABS(SDK_Target_Yaw_Gyro)<=20) OpticalFlow_Y_Vel_Control(5);
        //else if(ABS(SDK_Target_Yaw_Gyro)<=50) OpticalFlow_Y_Vel_Control(3);
        //else OpticalFlow_Y_Vel_Control(2);
        if(ABS(SDK_Target_Yaw_Gyro)<=10) OpticalFlow_Y_Vel_Control(5);
        else if(ABS(SDK_Target_Yaw_Gyro)<=20) OpticalFlow_Y_Vel_Control(3);
        else if(ABS(SDK_Target_Yaw_Gyro)<=50) OpticalFlow_Y_Vel_Control(2);
        else OpticalFlow_Y_Vel_Control(1);
        //OpticalFlow_Y_Vel_Control(0);
        OpticalFlow_Pos_Ctrl_Expect.x=0;
        OpticalFlow_Pos_Ctrl_Expect.y=0;
        force_brake_flag=1;
      }
      else if(SDK_Point.flag==1)//跟踪点检测
      {
        SDK_Ctrl_Cnt++;
        if(SDK_Ctrl_Cnt>=4)//20ms
        {
          Total_Controller.SDK_Roll_Position_Control.Expect=0;
          Total_Controller.SDK_Roll_Position_Control.FeedBack=SDK_Target.x;
          PID_Control_SDK_Err_LPF(&Total_Controller.SDK_Roll_Position_Control,SDK_Point.trust_flag);
          
          Total_Controller.SDK_Pitch_Position_Control.Expect=0;
          Total_Controller.SDK_Pitch_Position_Control.FeedBack=SDK_Target.y;
          PID_Control_SDK_Err_LPF(&Total_Controller.SDK_Pitch_Position_Control,SDK_Point.trust_flag);
          
          accel_target.x=constrain_float(Total_Controller.SDK_Roll_Position_Control.Control_OutPut,
                                         -Total_Controller.Optical_Speed_Control.Control_OutPut_Limit,
                                         Total_Controller.Optical_Speed_Control.Control_OutPut_Limit);                             
          Total_Controller.Roll_Angle_Control.Expect=constrain_float(fast_atan(accel_target.x/(GRAVITY_MSS*100))*RAD2DEG,-30,30);//roll
          
          
          accel_target.y=constrain_float(Total_Controller.SDK_Pitch_Position_Control.Control_OutPut,
                                         -Total_Controller.Optical_Speed_Control.Control_OutPut_Limit,
                                         Total_Controller.Optical_Speed_Control.Control_OutPut_Limit);//450
          Total_Controller.Pitch_Angle_Control.Expect=constrain_float(fast_atan(accel_target.y*Cos_Roll/(GRAVITY_MSS*100))*RAD2DEG,-30,30);//pitch
          
          SDK_Ctrl_Cnt=0;
        }
        OpticalFlow_Pos_Ctrl_Expect.x=0;
        OpticalFlow_Pos_Ctrl_Expect.y=0;
        force_brake_flag=1;
      }
      else//SDK模式下，未检测对于特征
      {
        /**************************光流位置控制器************************************/
        if(OpticalFlow_Pos_Ctrl_Expect.x==0
           &&OpticalFlow_Pos_Ctrl_Expect.y==0)
        {
          if(force_brake_flag==1||(rMat[2][2]>=0.95f
                                   &&pythagorous2(OpticalFlow_SINS.Speed[_PITCH],OpticalFlow_SINS.Speed[_ROLL])<=40))//打杆回中后，根据当前速度、倾角判断是否进行悬停
          {
            OpticalFlow_Pos_Ctrl_Expect.x=OpticalFlow_SINS.Position[_PITCH];
            OpticalFlow_Pos_Ctrl_Expect.y=OpticalFlow_SINS.Position[_ROLL];
          }
          else  //打杆回中未满足悬停条件时，只进行速度控制 
          {
            OpticalFlow_Pos_Ctrl_Output.x=0;
            OpticalFlow_Pos_Ctrl_Output.y=0;
          }
        }
        else  OpticalFlow_Pos_Control();
        /**************************基于模型的加速度-姿态角映射，相比直接给姿态，参数差异大概在20倍左右************************************/
        OpticalFlow_Vel_Control(OpticalFlow_Pos_Ctrl_Output);//速度期望        
        PID_LPF_Reset(&Total_Controller.SDK_Roll_Position_Control,SDK_Roll_Position_Controler);
        PID_LPF_Reset(&Total_Controller.SDK_Pitch_Position_Control,SDK_Pitch_Position_Controler);
				SDK_Pos_Ctrl_Reset();
			}
    }
    else//SDK模式下存在手动打杆操作时，控制量来源于遥控器给定
    {
      Total_Controller.Pitch_Angle_Control.Expect=Target_Angle[0];
      Total_Controller.Roll_Angle_Control.Expect=Target_Angle[1];
      OpticalFlow_Pos_Ctrl_Expect.x=0;
      OpticalFlow_Pos_Ctrl_Expect.y=0;
      force_brake_flag=1;
			PID_LPF_Reset(&Total_Controller.SDK_Roll_Position_Control,SDK_Roll_Position_Controler);
			PID_LPF_Reset(&Total_Controller.SDK_Pitch_Position_Control,SDK_Pitch_Position_Controler);
			SDK_Pos_Ctrl_Reset();
    }  
    SDK_Recieve_Flag=0; 
    OpticalFlow_Pos_Ctrl_Expect.x=0;
    OpticalFlow_Pos_Ctrl_Expect.y=0;
  }
}


void OpticalFlow_Control_Pure(uint8_t force_brake_flag)
{
	if(Roll_Control==0
		 &&Pitch_Control==0)//无水平遥控量给定
	{ 
		/**************************光流位置控制器************************************/
		if((OpticalFlow_Pos_Ctrl_Expect.x==0
			 &&OpticalFlow_Pos_Ctrl_Expect.y==0)||force_brake_flag==1)
		{
			OpticalFlow_Pos_Ctrl_Expect.x=OpticalFlow_SINS.Position[_PITCH];
			OpticalFlow_Pos_Ctrl_Expect.y=OpticalFlow_SINS.Position[_ROLL];
		}
		else  OpticalFlow_Pos_Control();
	 
		/***********当只需要速度控制时，开启以下注释，仅限调试时用*************/
		 //OpticalFlow_Pos_Ctrl_Output.x=0;
		 //OpticalFlow_Pos_Ctrl_Output.y=0;
		/**************************基于模型的加速度-姿态角映射，相比直接给姿态，参数差异大概在20倍左右************************************/
		OpticalFlow_Vel_Control(OpticalFlow_Pos_Ctrl_Output);//速度期望
	}
	else
	{
		OpticalFlow_Pos_Ctrl_Output.x=OpticalFlow_Expect_Speed_Mapping(-Target_Angle[1],Pit_Rol_Max,OpticalFlow_Speed_Control_Max);
		OpticalFlow_Pos_Ctrl_Output.y=OpticalFlow_Expect_Speed_Mapping(-Target_Angle[0],Pit_Rol_Max,OpticalFlow_Speed_Control_Max); 
		if(OpticalFlow_Speed_Control_Mode==1)
		{
			OpticalFlow_Vel_Control(OpticalFlow_Pos_Ctrl_Output);//速度期望
		}
		else
		{
			Total_Controller.Pitch_Angle_Control.Expect=Target_Angle[0];
			Total_Controller.Roll_Angle_Control.Expect=Target_Angle[1];      
		}
		OpticalFlow_Pos_Ctrl_Expect.x=0;
		OpticalFlow_Pos_Ctrl_Expect.y=0;
	}
	PID_LPF_Reset(&Total_Controller.SDK_Roll_Position_Control,SDK_Roll_Position_Controler);
	PID_LPF_Reset(&Total_Controller.SDK_Pitch_Position_Control,SDK_Pitch_Position_Controler);
}



void OpticalFlow_Pos_Control(void)
{
  static uint16_t OpticalFlow_Pos_Ctrl_Cnt=0;
  OpticalFlow_Pos_Ctrl_Cnt++;
  if(OpticalFlow_Pos_Ctrl_Cnt>=10)//50ms控制一次速度，避免输入频率过大，系统响应不过来
  {
    //计算位置偏差  
    OpticalFlow_Pos_Ctrl_Err.x=OpticalFlow_Pos_Ctrl_Expect.x-OpticalFlow_SINS.Position[_PITCH];
    OpticalFlow_Pos_Ctrl_Err.y=OpticalFlow_Pos_Ctrl_Expect.y-OpticalFlow_SINS.Position[_ROLL];
    //计算位置控制输出
    OpticalFlow_Pos_Ctrl_Output.x=Total_Controller.Optical_Position_Control.Kp*OpticalFlow_Pos_Ctrl_Err.x;
    OpticalFlow_Pos_Ctrl_Output.y=Total_Controller.Optical_Position_Control.Kp*OpticalFlow_Pos_Ctrl_Err.y;
    OpticalFlow_Pos_Ctrl_Cnt=0;
  }
}


void OpticalFlow_Vel_Control(Vector2f target)
{ 
  static uint16_t OpticalFlow_Vel_Ctrl_Cnt=0;
  OpticalFlow_Ctrl_Expect.x= target.x;//速度期望
  OpticalFlow_Ctrl_Expect.y= target.y;
  OpticalFlow_Vel_Ctrl_Cnt++;
  if(OpticalFlow_Vel_Ctrl_Cnt>=4)//20ms控制一次速度，避免输入频率过大，系统响应不过来
  {
    OpticalFlow_Ctrl_Err.x=constrain_float(OpticalFlow_Ctrl_Expect.x-OpticalFlow_SINS.Speed[_PITCH],-Total_Controller.Optical_Speed_Control.Err_Max,Total_Controller.Optical_Speed_Control.Err_Max);//30
    OpticalFlow_Ctrl_Err.y=constrain_float(OpticalFlow_Ctrl_Expect.y-OpticalFlow_SINS.Speed[_ROLL] ,-Total_Controller.Optical_Speed_Control.Err_Max,Total_Controller.Optical_Speed_Control.Err_Max);
    
    //if(ABS(OpticalFlow_Ctrl_Err.x)<=Total_Controller.Optical_Speed_Control.Integrate_Separation_Err)  
    OpticalFlow_Ctrl_Integrate.x+=Total_Controller.Optical_Speed_Control.Ki*OpticalFlow_Ctrl_Err.x;//0.1  15
    //if(ABS(OpticalFlow_Ctrl_Err.y)<=Total_Controller.Optical_Speed_Control.Integrate_Separation_Err)  
    OpticalFlow_Ctrl_Integrate.y+=Total_Controller.Optical_Speed_Control.Ki*OpticalFlow_Ctrl_Err.y;
    
    OpticalFlow_Ctrl_Integrate.x=constrain_float(OpticalFlow_Ctrl_Integrate.x,-Total_Controller.Optical_Speed_Control.Integrate_Max,Total_Controller.Optical_Speed_Control.Integrate_Max);
    OpticalFlow_Ctrl_Integrate.y=constrain_float(OpticalFlow_Ctrl_Integrate.y,-Total_Controller.Optical_Speed_Control.Integrate_Max,Total_Controller.Optical_Speed_Control.Integrate_Max);
    
    OpticalFlow_Ctrl_Output.x=OpticalFlow_Ctrl_Integrate.x+Total_Controller.Optical_Speed_Control.Kp*OpticalFlow_Ctrl_Err.x;//4.5
    OpticalFlow_Ctrl_Output.y=OpticalFlow_Ctrl_Integrate.y+Total_Controller.Optical_Speed_Control.Kp*OpticalFlow_Ctrl_Err.y;
    
    accel_target.y=-constrain_float(OpticalFlow_Ctrl_Output.y,-Total_Controller.Optical_Speed_Control.Control_OutPut_Limit,Total_Controller.Optical_Speed_Control.Control_OutPut_Limit);//450
    accel_target.x=-constrain_float(OpticalFlow_Ctrl_Output.x,-Total_Controller.Optical_Speed_Control.Control_OutPut_Limit,Total_Controller.Optical_Speed_Control.Control_OutPut_Limit);//期望运动加速度
    accel_to_lean_angles(accel_target,&angle_target);//期望运动加速度转期望姿态倾角
    Total_Controller.Pitch_Angle_Control.Expect=angle_target.y;
    Total_Controller.Roll_Angle_Control.Expect=angle_target.x;
    OpticalFlow_Vel_Ctrl_Cnt=0;
  }
}

void OpticalFlow_Y_Vel_Control(float target_y)
{ 
  static uint16_t OpticalFlow_Vel_Ctrl_Cnt=0;
  OpticalFlow_Ctrl_Expect.y= target_y;
  OpticalFlow_Vel_Ctrl_Cnt++;
  if(OpticalFlow_Vel_Ctrl_Cnt>=4)//20ms控制一次速度，避免输入频率过大，系统响应不过来
  {
    OpticalFlow_Ctrl_Err.y=constrain_float(OpticalFlow_Ctrl_Expect.y-OpticalFlow_SINS.Speed[_ROLL] ,-Total_Controller.Optical_Speed_Control.Err_Max,Total_Controller.Optical_Speed_Control.Err_Max);
    
    OpticalFlow_Ctrl_Integrate.y+=Total_Controller.Optical_Speed_Control.Ki*OpticalFlow_Ctrl_Err.y;
    
    OpticalFlow_Ctrl_Integrate.y=constrain_float(OpticalFlow_Ctrl_Integrate.y,-Total_Controller.Optical_Speed_Control.Integrate_Max,Total_Controller.Optical_Speed_Control.Integrate_Max);
    
    OpticalFlow_Ctrl_Output.y=OpticalFlow_Ctrl_Integrate.y+Total_Controller.Optical_Speed_Control.Kp*OpticalFlow_Ctrl_Err.y;
    
    accel_target.y=-constrain_float(OpticalFlow_Ctrl_Output.y,-Total_Controller.Optical_Speed_Control.Control_OutPut_Limit,Total_Controller.Optical_Speed_Control.Control_OutPut_Limit);//450
    accel_to_lean_angles(accel_target,&angle_target);//期望运动加速度转期望姿态倾角
    Total_Controller.Pitch_Angle_Control.Expect=angle_target.y;
    OpticalFlow_Vel_Ctrl_Cnt=0;
  }
}
