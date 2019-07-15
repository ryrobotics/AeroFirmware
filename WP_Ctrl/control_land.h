
#ifndef _CONTROL_LAND_H
#define _CONTROL_LAND_H

#define Faraway_Distance 600//距离home很远，计为A区域
#define Near_Distance 150//距离home较远，计为B区域
#define Arrive_Distance 0//达到home点，计为C区域


#define First_Nav_Rate   100//从A->B，开启一级巡航速度，单位cm
#define Second_Nav_Rate  80//从B->C，开启二级巡航速度，单位cm
#define Third_Nav_Rate   60//处于C点附近，开启三级巡航速度, 单位cm
#define Default_Nav_rate 50//默认巡航速度


#define Nav_Safety_Height 800//距离返航点较远时，返航所需的安全巡航高度，单位cm
#define Nav_Climb_Rate  80//当距离返航点较远时，且当前高度小于Nav_Safety_Height时，原地上升时的速度
#define Nav_Decline_Rate -40//当返航至home点正上方时，原地下降至地面时的速度
#define Nav_Rapid_Decline_Rate -80//快速下降至地面时的速度


#define Nav_Transition_Period 400//5ms*400=2S


void land_state_check(void);
void land_reset(void);
void landon_earth_check(void);
void land_run(void);
bool GPS_ok(void);
bool land_althold(float taret_climb_rate,float target_climb_alt);


#endif

