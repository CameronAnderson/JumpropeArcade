#ifndef _MOTORSWING_H_
#define _MOTORSWING_H_

#include <stdint.h>

typedef struct {
	uint16_t set_point; 
	uint8_t direction_flag;
	float period;
	uint16_t duty;
} _Testing;
	
uint16_t PIDcalc(uint16_t set_point);

void swing(uint8_t direction_flag);

void wait_period(float period);

void clear_dirpin(void);

void set_dirpin(void);

void move(uint16_t set_point, uint16_t duty);

#endif