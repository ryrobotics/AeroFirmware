
#include "Headfile.h"
#include "Temperature_Ctrl.h"

#define Temperature_Ctrl_Enable   1

#define Temperature_Setpoint   50
const float Temp_Control_Unit[20]=
{1  ,1 ,1 ,0 ,0 ,0 , 0 ,50    ,30  ,0 ,80,  20.00  ,3.0   ,100.0  ,0    ,0 ,100 , 1 ,  1 ,  1 };//水平维度方向加速度控制器 20 3 50  //  10  3  80
/*
1偏差限幅标志；  2积分限幅标志；3积分分离标志；   4期望；
5反馈            6偏差；        7上次偏差；       8偏差限幅值；
9积分分离偏差值；10积分值       11积分限幅值；    12控制参数Kp；
13控制参数Ki；   14控制参数Kd； 15控制器总输出；  16上次控制器总输出
17总输出限幅度； 18变积分控制时的积分增益
*/		
void Temperature_Ctrl_Init(void)
{
  Total_Controller.IMU_Temperature_Control.Err_Limit_Flag=(uint8)(Temp_Control_Unit[0]);//1偏差限幅标志
  Total_Controller.IMU_Temperature_Control.Integrate_Limit_Flag=(uint8)(Temp_Control_Unit[1]);//2积分限幅标志
  Total_Controller.IMU_Temperature_Control.Integrate_Separation_Flag=(uint8)(Temp_Control_Unit[2]);//3积分分离标志
  Total_Controller.IMU_Temperature_Control.Expect=Temp_Control_Unit[3];//4期望
  Total_Controller.IMU_Temperature_Control.FeedBack=Temp_Control_Unit[4];//5反馈值
  Total_Controller.IMU_Temperature_Control.Err=Temp_Control_Unit[5];//6偏差
  Total_Controller.IMU_Temperature_Control.Last_Err=Temp_Control_Unit[6];//7上次偏差
  Total_Controller.IMU_Temperature_Control.Err_Max=Temp_Control_Unit[7];//8偏差限幅值
  Total_Controller.IMU_Temperature_Control.Integrate_Separation_Err=Temp_Control_Unit[8];//9积分分离偏差值
  Total_Controller.IMU_Temperature_Control.Integrate=Temp_Control_Unit[9];//10积分值
  Total_Controller.IMU_Temperature_Control.Integrate_Max=Temp_Control_Unit[10];//11积分限幅值
  Total_Controller.IMU_Temperature_Control.Kp=Temp_Control_Unit[11];//12控制参数Kp
  Total_Controller.IMU_Temperature_Control.Ki=Temp_Control_Unit[12];//13控制参数Ki
  Total_Controller.IMU_Temperature_Control.Kd=Temp_Control_Unit[13];//14控制参数Ki
  Total_Controller.IMU_Temperature_Control.Control_OutPut=Temp_Control_Unit[14];//15控制器总输出
  Total_Controller.IMU_Temperature_Control.Last_Control_OutPut=Temp_Control_Unit[15];//16上次控制器总输出
  Total_Controller.IMU_Temperature_Control.Control_OutPut_Limit=Temp_Control_Unit[16];//17上次控制器总输出
  Total_Controller.IMU_Temperature_Control.Scale_Kp=Temp_Control_Unit[17];
  Total_Controller.IMU_Temperature_Control.Scale_Ki=Temp_Control_Unit[18];
  Total_Controller.IMU_Temperature_Control.Scale_Kd=Temp_Control_Unit[19];  
}

void Simulation_PWM_Init(void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; 
  HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x000000FF;
  GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_2);
	GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_2,0);
}

#define Simulation_PWM_Period_MAX  100//100*1ms=0.1S
void Simulation_PWM_Output(uint16_t width)
{
#if Temperature_Ctrl_Enable
	uint16_t static cnt=0;
	cnt++;
	if(cnt>=Simulation_PWM_Period_MAX)  cnt=0;
  if(cnt<=width) GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_2,GPIO_PIN_2);
	else GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_2,0);
#else
	GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_2,0);
#endif
	
}


void Temperature_Ctrl(void)
{
#if Temperature_Ctrl_Enable
 uint16_t static cnt=0;
 cnt++;
 Total_Controller.IMU_Temperature_Control.Expect=Temperature_Setpoint;
 Total_Controller.IMU_Temperature_Control.FeedBack=WP_Sensor.temperature;
 if(cnt>=10)  //10*1ms=0.01S
 {
	 PID_Control(&Total_Controller.IMU_Temperature_Control);
	 Total_Controller.IMU_Temperature_Control.Control_OutPut=constrain_float(Total_Controller.IMU_Temperature_Control.Control_OutPut,0,Simulation_PWM_Period_MAX);
	 cnt=0;
 }
#else
	Total_Controller.IMU_Temperature_Control.Control_OutPut=0;
#endif
}

uint8_t Temperature_State_Check(void)
{
#if Temperature_Ctrl_Enable
  return (ABS(Total_Controller.IMU_Temperature_Control.Err))<=0.5f?1:0;
#else
	return 1;
#endif	
}
