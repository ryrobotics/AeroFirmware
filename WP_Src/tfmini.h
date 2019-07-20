#ifndef _TFMINI_H_
#define _TFMINI_H_




typedef struct
{
  uint16_t distance;
	uint16_t strength;
	uint16_t temperature;
  uint8_t mode;
	uint16_t last_distance;
	uint16_t last_strength;
	float div;
	float acc;
	float last_div;
	uint8_t health;
}tfmini;

extern tfmini tfdata; 
void TFmini_Statemachine(void);





#endif


