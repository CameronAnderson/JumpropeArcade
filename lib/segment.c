#include <p24FJ128GB206.h>
#include "common.h"
#include "pin.h"
#include "segment.h"

_PIN *segmentClock, *segmentLatch, *segmentData;

void init_segment(void){

    segmentClock = &D[0];
    segmentLatch = &D[1];
    segmentData = &D[2];

    pin_digitalOut(segmentClock);
    pin_digitalOut(segmentLatch);
    pin_digitalOut(segmentData);

    pin_clear(segmentClock);
    pin_clear(segmentLatch);
    pin_clear(segmentData);	
}

void showBlank(void){

    volatile uint8_t segments = 0b00000000;
    volatile uint8_t segmentsZero = 0b00000000;

    int a;
    for(a = 0; a < 3; a++){
        int z;
        for (z = 0 ; z < 8 ; z++){
            volatile uint8_t transferSegment = segments & (1 << (7 - z));
            pin_clear(segmentClock);
            pin_write(segmentData, transferSegment);
            pin_set(segmentClock); 
        }
    } 
    pin_clear(segmentLatch);
    pin_set(segmentLatch);     
}

void showNumber(int value){

    int number = abs(value); 
    int x;

    for(x = 0 ; x < 3; x++){
        int remainder1 = number % 10;
        postNumber(remainder1, 0);
        number /= 10;
    }
    pin_clear(segmentLatch);
    pin_set(segmentLatch); 
}

void postNumber(int number, uint8_t decimal){

    volatile uint8_t segments = 0b00000000;
    volatile uint8_t segmentsZero = 0b00000000;

    #define a  1<<0
    #define b  1<<6
    #define c  1<<5
    #define d  1<<4
    #define e  1<<3
    #define f  1<<1
    #define g  1<<2
    #define dp 1<<7


    switch (number){
        case 1: segments = b | c | segmentsZero; break;
        case 2: segments = a | b | d | e | g | segmentsZero; break;
        case 3: segments = a | b | c | d | g | segmentsZero; break;
        case 4: segments = f | g | b | c | segmentsZero; break;
        case 5: segments = a | f | g | c | d | segmentsZero; break;
        case 6: segments = a | f | g | e | c | d | segmentsZero; break;
        case 7: segments = a | b | c | segmentsZero; break;
        case 8: segments = a | b | c | d | e | f | g | segmentsZero; break;
        case 9: segments = a | b | c | d | f | g | segmentsZero; break;
        case 0: segments = a | b | c | d | e | f | segmentsZero; break;
        case ' ': segments = 0 | segmentsZero; break;
        case 'c': segments = g | e | d | segmentsZero; break;
        case '-': segments = g | segmentsZero; break;
    }

    if (decimal == 1){
        segments |= dp;
    }
    int y;
    for (y = 0 ; y < 8 ; y++){
        volatile uint8_t transferSegment = segments & (1 << (7 - y));
        pin_clear(segmentClock);
        pin_write(segmentData, transferSegment);
        pin_set(segmentClock); 
    }
}