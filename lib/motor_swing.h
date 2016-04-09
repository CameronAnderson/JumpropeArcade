#ifndef _MOTORSWING_H_
#define _MOTORSWING_H_

#include <stdint.h>

typedef struct {
	uint16_t set_point;
	uint16_t actual_position; 
	uint8_t direction_flag;
	float period;
} _Testing;
	
uint16_t PIDcalc(uint16_t set_point, uint16_t actual_position);

void swing(uint8_t direction_flag);

void wait_period(float period);

#endif



