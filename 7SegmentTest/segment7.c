#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "uart.h"

#define a  1<<0
#define b  1<<6
#define c  1<<5
#define d  1<<4
#define e  1<<3
#define f  1<<1
#define g  1<<2
#define dp 1<<7

_PIN *segmentClock, *segmentLatch, *segmentData;

void showNumber(float value){
  int x;
  int8_t number = abs(value); 
  
  for (x = 0 ; x < 2 ; x++){
    int8_t leftover = number % 10;
    postNumber(leftover, 0);
    number /= 10;
  }
  pin_write(segmentLatch, 0);
  pin_write(segmentLatch, 1); 
}

void postNumber(int8_t number, uint8_t decimal){
    int x;
    uint8_t segments;

    switch (number){
    case 1: segments = b | c; break;
    case 2: segments = a | b | d | e | g; break;
    case 3: segments = a | b | c | d | g; break;
    case 4: segments = f | g | b | c; break;
    case 5: segments = a | f | g | c | d; break;
    case 6: segments = a | f | g | e | c | d; break;
    case 7: segments = a | b | c; break;
    case 8: segments = a | b | c | d | e | f | g; break;
    case 9: segments = a | b | c | d | f | g; break;
    case 0: segments = a | b | c | d | e | f; break;
    case ' ': segments = 0; break;
    case 'c': segments = g | e | d; break;
    case '-': segments = g; break;
  }

   if (decimal == 1){
    segments |= dp;
    }

   for (x = 0 ; x < 8 ; x++)
  {
    pin_write(segmentClock, 0);
    pin_write(segmentData, segments & 1 << (7 - x));
    pin_write(segmentClock, 1); 
}
}

int16_t main(void) {

    init_clock();
    init_ui();
    init_timer();
    init_pin();
    init_uart();

    segmentClock = &D[0];
    segmentLatch = &D[1];
    segmentData = &D[2];

    pin_digitalOut(segmentClock);
    pin_digitalOut(segmentLatch);
    pin_digitalOut(segmentData);

    pin_clear(segmentClock);
    pin_clear(segmentLatch);
    pin_clear(segmentData);

    timer_setPeriod(&timer2, .5);
    timer_start(&timer2);

    int number = 0;  

// test for Saarth
   
    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            led_toggle(&led1);
            
            showNumber(number);
            number = number + 1;
            number %= 100;

            printf("Number %s\n", number);
        }
    }
            

}


