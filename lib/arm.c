/*
** Copyright (c) 2016, Sean Karagianes
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
**     1. Redistributions of source code must retain the above copyright
**        notice, this list of conditions and the following disclaimer.
**     2. Redistributions in binary form must reproduce the above copyright
**        notice, this list of conditions and the following disclaimer in the
**        documentation and/or other materials provided with the distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGE.
*/
#include <p24FJ128GB206.h>
#include "arm.h"

#define ARM_MODE_STANDBY  0
#define ARM_MODE_MAGNET   1
#define ARM_MODE_READY    2
#define ARM_MODE_LEFT     3
#define ARM_MODE_RIGHT    4
#define ARM_MODE_FAILURE  5

#define ARM_MIN_SPEED 0x0200
#define ARM_T 5e-4 // Sec

#define ARM_CENTER 35600
#define ARM_LEFT   20000
#define ARM_RIGHT  48000
#define ARM_MAGNET 41000

// #define JOY_DUTY(f) max(0x0000, (uint16_t)min(f*65535, 65535))

_PIN *dirpin, *pwmpin, *potentiometer;

_ARM arm;

float __vel_tmp = 0;

// void __arm_standby(_ARM *self);
// void __arm_magnet(_ARM *self);
// void __arm_ready(_ARM *self);
void __arm_swing(_TIMER *timer);
void __arm_failure(_ARM *self);

void arm_move(uint16_t mode) {
    timer_every(arm.timer, ARM_T, *__arm_swing);

    switch (mode) {
        case ARM_MODE_STANDBY:
            arm.set_point = ARM_CENTER;
            arm.duty = 0x2000;
            break;
        case ARM_MODE_MAGNET:
            arm.set_point = ARM_MAGNET;
            arm.duty = 0x3000;
            break;
        case ARM_MODE_READY:
            arm.set_point = ARM_LEFT;
            arm.duty = 0x5000;
            break;
        case ARM_MODE_LEFT:
            arm.set_point = ARM_LEFT;
            arm.duty = 0x6000;
            break;
        case ARM_MODE_RIGHT:
            arm.set_point = ARM_RIGHT;
            arm.duty = 0x6000;
            break;
        case ARM_MODE_FAILURE:
            arm.set_point = ARM_CENTER;
            arm.duty = 0x2000;
            break;
    }
}

// void __arm_standby(_ARM *self){
//     self->position = pin_read(potentiometer);
//     self->set_point = ARM_CENTER;
//     self->error = abs((self->set_point - self->position));

//     if (self->set_point > self->position){
//         pin_clear(dirpin);
//         led_on(&led2);
//     }
//     else{
//         pin_set(dirpin);
//         led_on(&led3);
//     }

//     if (self->error > 1000){
//         led_on(&led1);
//         self->duty = 0x2000;
//         pin_write(pwmpin, self->duty);
//     }
//     else{
//         led_off(&led1);
//         pin_write(pwmpin, 0);
//     }
// }

// void __arm_magnet(_ARM *self){
//     self->position = pin_read(potentiometer);
//     self->set_point = ARM_RIGHT;
//     self->error = abs((self->set_point - self->position));

//     if (self->set_point > self->position){
//         pin_clear(dirpin);
//         led_on(&led2);
//     }
//     else{
//         pin_set(dirpin);
//         led_on(&led3);
//     }

//     if (self->error > 500){
//         led_on(&led1);
//         self->duty = 0x4000;
//         pin_write(pwmpin, self->duty);
//     }
//     else{
//         led_off(&led1);
//         pin_write(pwmpin, 0);
//     }
// }

// void __arm_ready(_ARM *self){
//     self->position = pin_read(potentiometer);
//     self->set_point = ARM_LEFT;
//     self->error = abs((self->set_point - self->position));

//     if (self->set_point > self->position){
//         pin_clear(dirpin);
//         led_on(&led2);
//     }
//     else{
//         pin_set(dirpin);
//         led_on(&led3);
//     }

//     if (self->error > 500){
//         led_on(&led1);
//         self->duty = 0x4000;
//         pin_write(pwmpin, self->duty);
//     }
//     else{
//         led_off(&led1);
//         pin_write(pwmpin, 0);
//     }
// }

void __arm_swing(_TIMER *timer){
    arm.position = pin_read(potentiometer);
    
    arm.error = abs((arm.set_point - arm.position));

    if (arm.set_point > arm.position){
        pin_clear(dirpin);
        led_on(&led2);
    }
    else{
        pin_set(dirpin);
        led_on(&led3);
    }

    if (arm.error > 500){
        led_on(&led1);
        pin_write(pwmpin, arm.duty);
    }
    else{
        led_off(&led1);
        pin_write(pwmpin, 0);
        timer_cancel(arm.timer);
    }

}

void __arm_failure(_ARM *self){

}

void init_arm(void) {
    arm_init(&arm, &timer1);
}

void arm_init(_ARM *self, _TIMER *timer) {
    self->mode = ARM_MODE_STANDBY;

    // proportional control constant
    self->K = 1.0;
    // velocity control constant (if necessary)
    self->B = 1.0;

    self->set_point = ARM_CENTER;
    self->duty = 0;

    self->timer = timer;

    self->position = 0;
    self->position_1 = 0;

    self->error = 0;

    self->vel = 0;
    self->vel_1 = 0;

    dirpin = &D[8];     //set direction control pin as 8
    pwmpin = &D[9];     //set PWM pin as 9
    potentiometer = &A[5];  //potentiometer input on 5

    //initialize the motor driver pins as outputs
    pin_digitalOut(dirpin);
    pin_digitalOut(pwmpin);

    //initialize analog input for potentiometer
    pin_analogIn(potentiometer);

    pin_clear(dirpin);
    pin_clear(pwmpin);

    oc_pwm(&oc1, pwmpin, &timer5, 1e3, 0);

    // timer_every(self->timer, ARM_T, *__arm_loop);
}

void wait_period(float period){
    timer_setPeriod(&timer4, period);
    timer_start(&timer4);

    while(1){
        if (timer_flag(&timer4)) {
        timer_lower(&timer4);
        break;
        }
    }  
}