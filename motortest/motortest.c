#include <p24FJ128GB206.h>
#include <stdio.h>
#include <time.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "oc.h"
#include "uart.h"

_PIN *dirpin, *pwmpin, *potentiometer;

#define Kp .5
#define Right 22528
#define Left 48832

uint16_t PIDcalc(uint16_t set_point, uint16_t actual_position){
    uint16_t error;
    uint16_t duty; 
    uint16_t threshold = 500;
    // printf("actual_position %u\n\r",  actual_position);
    error = abs((set_point - actual_position));
    // printf("error %u\n\r",  error);
    if (error > threshold){
        duty = (Kp * error);
        // printf("duty %u\n\r",  duty);
        oc_pwm(&oc1, pwmpin, &timer5, 1e3, duty);
        PIDcalc(set_point, pin_read(potentiometer));  
    } 
}

void swing(uint8_t direction_flag, uint16_t actual_position){

    if (direction_flag == 0){;
        pin_clear(dirpin);
        PIDcalc(Left, actual_position);
        direction_flag = 1;
        wait_second();
        swing(direction_flag, actual_position); 
    }
    else{
        pin_set(dirpin);
        PIDcalc(Right, actual_position);
        direction_flag = 0;
        wait_second();
        swing(direction_flag, actual_position);         
    }
}

void wait_second(){
    timer_setPeriod(&timer2, .5);
    timer_start(&timer2);

    while(1){
        if (timer_flag(&timer2)) {
        timer_lower(&timer2);
        break;
        }
    }  
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_timer();
    init_uart();
    init_pin();
    init_oc();


    dirpin = &D[8];  //set direction control pin as pin 8
    pwmpin = &D[9];  //set PWM pin as pin 9 
    potentiometer = &A[5];  //read potentiometer pin as pin 5

    //initialize the two pins as digital outputs and then clear them
    pin_digitalOut(dirpin);
    pin_digitalOut(pwmpin);

    //initialize analog input for potentiometer 
    pin_analogIn(potentiometer);

    pin_clear(dirpin);
    pin_clear(pwmpin);

    uint16_t actual_position = 0;

    uint16_t duty = 0xffff;

    led_on(&led1);
    timer_setPeriod(&timer2, .5);
    timer_start(&timer2);

    uint8_t direction_flag = 0;


    while (1) {

        actual_position = pin_read(potentiometer);
        swing(direction_flag, actual_position);
        //PIDcalc(0, actual_position);
        // oc_pwm(&oc1, pwmpin, &timer5, 1e3, duty*.1);
        // printf("TEST1 %u\n\r",  actual_position);


    }
}