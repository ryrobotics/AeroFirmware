
#ifndef _NAMELESSCOTRUN_OPTICALFLOW_
#define _NAMELESSCOTRUN_OPTICALFLOW_

#define LC306_OPTICAL_FLOW 1
#define CL_OPTICAL_FLOW 0

#if (CL_OPTICAL_FLOW==1)
	#define FLOW_BUF_ONE_FRAME  9
#endif

#if (LC306_OPTICAL_FLOW==1)
	#define FLOW_BUF_ONE_FRAME  14
#endif

#define FLOW_BUF_CNT  FLOW_BUF_ONE_FRAME*2


typedef struct{
  unsigned short frame_count_since_last_readout;
  signed short pixel_flow_x_integral;
  signed short pixel_flow_y_integral;
  signed short gyro_x_rate_integral;
  signed short gyro_y_rate_integral;
  signed short gyro_z_rate_integral;
  unsigned int integration_timespan;
  unsigned int sonar_timestamp;
  unsigned short ground_distance;
  signed short gyro_temperature;
  unsigned char qual;
	unsigned char sum;
}flow_integral_frame;

typedef struct{
  float x;
  float y;
  unsigned short dt;
  unsigned char qual;
  unsigned char update;
}flow_float;

extern  flow_integral_frame opt_origin_data;
extern Vector2f opt_filter_data,opt_gyro_filter_data,opt_gyro_data,gyro_filter_data;
extern SINS OpticalFlow_SINS;
void OpticalFlow_Init(void);
uint8_t Optflow_Prase(void);
void Optflow_Statemachine(void);
float OpticalFlow_Rotate_Complementary_Filter(float optflow_gyro,float gyro,uint8_t axis);
extern flow_float opt_data;
extern uint8_t OpticalFlow_Is_Work;
extern Vector2f OpticalFlow_Speed,OpticalFlow_Speed_Err,OpticalFlow_Position;



#endif



