
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
uint8_t move_with_openmv(uint8_t SDK_Mode_Set,float delta,SDK_Status *Status,uint16_t number);
void NCQ_SDK_Run(void);
uint8_t NCQ_SDK_Circle(void);
void SDK_DT_Send_Check(unsigned char mode);
void SDK_DT_Send_Check_Front(unsigned char mode);
void SDK_Init(void);
void SDK_Data_Prase(void);
void SDK_Data_Prase_Front(void);//前向openmv数据解析
void SDK_Data_Receive_Prepare(u8 data);

//by RayWong
#define POINT_MODE 0x01
#define LINE_MODE  0x02
#define REC_MODE   0x03
#define WAIT_MODE  0x0F

#endif

