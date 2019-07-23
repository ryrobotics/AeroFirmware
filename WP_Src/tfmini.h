#ifndef _TFMINI_H_
#define _TFMINI_H_




typedef struct
{
  float distance;
	float strength;
	float temperature;
  uint8_t mode;
	float last_distance;
	float last_strength;
	float div;
	float acc;
	float last_div;
	uint8_t health;
}tfmini;

extern tfmini tfdata; 
extern uint8_t Ground_Sensor_Now_Mode;
extern float ground_sensor_default;
void TFmini_Statemachine(void);
void Ground_Sensor_Statemachine(void);




#endif


