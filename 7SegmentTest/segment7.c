#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "uart.h"


_PIN *segmentClock, *segmentLatch, *segmentData;

void showNumber(int value){
    
    printf("value %i\n\r", value);

    int number = abs(value); 
    int x;
    for(x = 0 ; x < 2 ; x++){
        int remainder1 = number % 10;
        printf("Remainder %i\n\r", remainder1);
        postNumber(remainder1, 0);
        number /= 10;
    }
    pin_clear(segmentLatch);
    pin_set(segmentLatch); 
}

void postNumber(int number, uint8_t decimal){

    uint16_t segments = 0b00000000;

    #define a  0b1<<0
    #define b  0b1<<6
    #define c  0b1<<5
    #define d  0b1<<4
    #define e  0b1<<3
    #define f  0b1<<1
    #define g  0b1<<2
    #define dp 0b1<<7

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
    printf("segments %i\n\r", segments);
    int y;
    for (y = 0 ; y < 8 ; y++){
        uint8_t transferSegment = segments & (1 << (7 - y));
        printf("transferSegment %i\n\r", transferSegment);
        pin_clear(segmentClock);
        pin_write(segmentData, transferSegment);
        pin_set(segmentClock); 
    }
}

int16_t main(void) {

    init_clock();
    init_ui();
    init_timer();
    init_pin();
    init_uart();
    init_spi();


    segmentClock = &D[0];
    segmentLatch = &D[1];
    segmentData = &D[2];

    pin_digitalOut(segmentClock);
    pin_digitalOut(segmentLatch);
    pin_digitalOut(segmentData);

    pin_clear(segmentClock);
    pin_clear(segmentLatch);
    pin_clear(segmentData);
    
    timer_setPeriod(&timer2, 1);
    timer_start(&timer2);

    int number = 0;  

// test for Saarth
   
    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            led_toggle(&led1);

            
            // pin_write(segmentClock, 0);
            // pin_write(segmentData, 0b11111111);
            // pin_write(segmentClock, 1); 
            
            // pin_write(segmentClock, 0);
            // pin_write(segmentData, 0b11111111);
            // pin_write(segmentClock, 1); 
            
            // pin_write(segmentClock, 0);
            // pin_write(segmentData, 0b11111111);
            // pin_write(segmentClock, 1); 
            
            // pin_write(segmentClock, 0);
            // pin_write(segmentData, 0b11111111);
            // pin_write(segmentClock, 1); 
            
            // pin_write(segmentClock, 0);
            // pin_write(segmentData, 0b11111111);
            // pin_write(segmentClock, 1); 
            
            // pin_write(segmentClock, 0);
            // pin_write(segmentData, 0b11111111);
            // pin_write(segmentClock, 1); 
            
            // pin_write(segmentClock, 0);
            // pin_write(segmentData, 0b11111111);
            // pin_write(segmentClock, 1); 

            // pin_write(segmentClock, 0);
            // pin_write(segmentData, 0b11111111);
            // pin_write(segmentClock, 1); 

            // pin_write(segmentLatch, 0);
            // pin_write(segmentLatch, 1);

            showNumber(number);
            number = number + 1;
            number %= 100;

            printf("Number %i\n\r", number);
        }
    }
            

}

