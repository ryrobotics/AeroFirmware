
#ifndef _CONTROL_ALTHOLD_H
#define _CONTROL_ALTHOLD_H


void Thr_Scale_Set(Vector_RC *rc_date);
void ncq_control_althold(void);
float get_stopping_point_z(Vector3f *stopping_point);
uint8_t Thr_Push_Over_Deadband(void);


extern float Yaw_Vel_Feedforward_Output;//竖直速度前馈控制器输出;
extern float Yaw_Vel_Feedforward_Rate;//竖直速度前馈控制器，APM里面为1、0.45;
extern float Yaw_Vel_Feedforward_Delta;//竖直期望速度变化率;
extern float Last_Yaw_Vel_Target;
extern float Yaw_Vel_Target;
extern Vector3f UAV_Cushion_Stop_Point;

extern uint16_t  Deadband;//油门中位死区
extern uint16_t  Deadzone_Min;
extern uint16_t  Deadzone_Max;
extern uint16_t  Thr_Top;//油门最大上行程
extern uint16_t  Thr_Buttom;//油门最大下行程
extern uint8_t Thr_Push_Over_State;
#endif

