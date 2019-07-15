
#ifndef _CONTROL_POSHOLD_H
#define _CONTROL_POSHOLD_H

float ncq_speed_mapping(float input,uint16_t input_max,float output_max);
void ncq_control_poshold(void);
uint8_t get_stopping_point_xy(Vector3f *stopping_point);
void accel_to_lean_angles(Vector2f _accel_target,Vector2f *target_angle);


extern Vector3_Nav Earth_Frame_Accel_Target;   //导航（地理）坐标系，正东、正北方向目标运动加速度期望
extern Vector3_Nav Earth_Frame_Pos_Err;        //导航（地理）坐标系，正东、正北方向w位置偏差
extern Vector2_Ang Body_Frame_Accel_Target;      //导航（地理）坐标系，机体横滚(Y正)、俯仰(X正)方向目标运动加速度期望
extern Vector2_Ang Body_Frame_Speed_Feedback;    //导航（地理）坐标系，机体横滚(Y正)、俯仰(X正)方向目标运动速度反馈
extern Vector2_Ang Body_Frame_Pos_Err;           //机体方向上位置偏差
extern Vector2_Ang Body_Frame_Brake_Speed;       //机体方向上刹车速度
extern uint8 GPS_Speed_Control_Mode;
extern Vector2f accel_target,angle_target;
#endif

