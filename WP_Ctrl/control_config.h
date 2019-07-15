
#ifndef _CONTROL_CONFIG_H
#define _CONTROL_CONFIG_H

#include "control_althold.h"
#include "control_poshold.h"
#include "control_land.h"


#define Self_Balance_Mode 1//自稳、纯姿态加油门补偿
#define High_Hold_Mode    2//定高模式
#define Pos_Free_Mode 1//水平位置no fixed
#define Pos_Hold_Mode 2//定点模式
//GPS定点下打杆控速模式与直接姿态角
#define Speed_Mode 0  //GPS定点模式下，打杆控速
#define Angle_Mode 1  //GPS定点模式下，打杆直接给姿态期望角

/*****************遥控器行程设置**********************/
#define  Climb_Up_Speed_Max    300//向上最大攀爬速度，cm/s  400  300
#define  Climb_Down_Speed_Max  200//向下最大下降速度，cm/s  200  150

/****************************************************************
当油门推重比较小时，若上升下降期望加速度比较大，
会导致输出映射的里面的姿态控制量得不到充分输出，
使得快速上升下降时，姿态不平稳，若此过程持续时间长，
会导致姿态长期得不到修正，直致最后炸鸡，故推重比较
小时，可将期望加速度限小一点，或者做控制量优先级处理
*******************************************************************/
#define  Climb_Up_Acceleration_Max     400//向上最大攀爬加速度，cm/s^2  400  250
#define  Climb_Down_Acceleration_Max   250//向下最大下降加速度，cm/s^2  300  150



#define  Thr_Start  1050//起转油门量，油门倾角补偿用，太大会导致过补偿
#define  Thr_Fly_Start  1150//起飞油门量 1250
#define  Thr_Hover_Default 1500//默认悬停油门，直接定高起飞时用1500
#define  Thr_Min 1000
#define  Thr_Idle 1100//油门怠速，取接近起转油门值即可1150
#define  Nav_Speed_Max  500//最大期望水平速度为5m/s


/*****************一键起飞高度设置，单位为cm，比如100表示原地起飞到相对初始位置1米高的位置**********************/
#define  Auto_Launch_Target 75//一键起飞的目标高度，相对起飞高度，使用超声波时，请勿超过超过声波量程，推荐200以下



#endif

