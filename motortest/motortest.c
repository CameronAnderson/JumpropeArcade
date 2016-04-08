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

#define Kp 0.1
#define Right 23104
#define Left -16704

float PIDcalc(float set_point, float actual_position){
    float error;
    float output; 

    error = set_point - actual_position;
    output = Kp * error; 

    return output;
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

    while (1) {

        actual_position = pin_read(potentiometer);
        oc_pwm(&oc1, pwmpin, &timer5, 1e3, duty*.1);

        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            led_toggle(&led1);
            printf("TEST1 %u\n\r",  actual_position);
        }

        // led_off(&led2);
        // led_off(&led3);


        actual_position = pin_read(potentiometer);

        // while (!sw_read(&sw2)){
        //     led_on(&led2);
        //     pin_clear(dirpin);
        //     oc_pwm(&oc1, pwmpin, &timer5, 1e3, duty);
        // }

        // while (!sw_read(&sw3)){
        //     led_on(&led3);
        //     pin_set(dirpin);
        //     oc_pwm(&oc1, pwmpin, &timer5, 1e3, duty);
        //     }

    }
}