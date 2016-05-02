#include <p24FJ128GB206.h>
#include <stdio.h>
#include <time.h>
#include "pin.h"
#include "oc.h"
#include "ui.h"
#include "motor_swing.h"

_PIN *dirpin, *pwmpin, *potentiometer;

#define Kp .25
#define Right 44800
#define Left 26400

uint8_t direction_flag = 0;
uint16_t actual_position = 0;
uint16_t duty = 0;

void init_motor_swing(void){
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

    oc_pwm(&oc1, pwmpin, &timer5, 1e3, 0);
}

uint16_t PIDcalc(uint16_t set_point){
    actual_position = pin_read(potentiometer);
    uint16_t error;
    uint16_t duty; 
    uint16_t threshold = 500;
    // printf("actual_position %u\n\r",  actual_position);
    error = abs((set_point - actual_position));
    // printf("error %u\n\r",  error);
    if (error > threshold){
        duty = (Kp * error);
        // printf("duty %u\n\r",  duty);
        pin_write(pwmpin, duty);
        PIDcalc(set_point);  
    }
    else{
        pin_write(pwmpin, 0); 
    } 
}

// void moveArm(_TIMER *self){
//     actual_position = pin_read(potentiometer);
//     uint16_t error;
//     uint16_t threshold = 500;

//     error = abs((set_point - actual_position));

//     // set the direction of motor swing based on difference between set point and actual position
//     if (set_point > actual_position){
//         pin_clear(dirpin);
//         led_on(&led2);
//     }
//     else{
//         pin_set(dirpin);
//         led_on(&led3);
//     }

//     if (error > threshold){
//         led_on(&led1);
//         pin_write(pwmpin, 0x8000);
//         moveArm(set_point, duty);
//     }
//     else{
//         led_off(&led1);
//         pin_write(pwmpin, 0);
//     }
// }


void clear_dirpin(void){
    pin_clear(dirpin);    
}

void set_dirpin(void){
    pin_set(dirpin);
}

void swing(uint8_t direction_flag){

    if (direction_flag == 0){;
        clear_dirpin();
        PIDcalc(Left);
        direction_flag = 1;
        wait_period(.5);
        swing(direction_flag); 
    }
    else{
        set_dirpin();
        PIDcalc(Right);
        direction_flag = 0;
        wait_period(.5);
        swing(direction_flag);         
    }
}

void wait_period(float period){
    timer_setPeriod(&timer2, period);
    timer_start(&timer2);

    while(1){
        if (timer_flag(&timer2)) {
        timer_lower(&timer2);
        break;
        }
    }  
}