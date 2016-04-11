#ifndef _SEGMENT_H_
#define _SEGMENT_H_

#include <stdint.h>

typedef struct {
	int value;
	int number; 
	uint8_t decimal;
} _Test;

void init_segment(void);

void showNumber(int value);

void postNumber(int number, uint8_t decimal);

#endif




