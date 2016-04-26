#include <p24FJ128GB206.h>
#include <stdio.h>
#include <time.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "oc.h"
// #include "uart.h"

_PIN *dirpin, *pwmpin;
 
 int16_t main(void) {
     init_clock();
     init_ui();
     init_timer();
     init_pin();
     init_oc();
 
     dirpin = &D[8];  //set direction control pin as pin 8
     pwmpin = &D[9];  //set PWM pin as pin 9 
 
     //initialize the two pins as digital outputs and then clear them
     pin_digitalOut(dirpin);
     pin_digitalOut(pwmpin);
 
     pin_clear(dirpin);
     pin_clear(pwmpin);
 
     timer_setPeriod(&timer1, 1);
     timer_start(&timer1);
 
     uint16_t duty = 0x8000;
     float freq = 1000;
 
     while (1) {
 
         led_off(&led2);
         led_off(&led3);
         oc_pwm(&oc2, pwmpin, NULL, freq, 0);
 
         while (!sw_read(&sw2)){
             led_on(&led2);
             pin_clear(dirpin);
             pin_write(pwmpin, duty);
         }
 
         while (!sw_read(&sw3)){
             led_on(&led3);
             pin_set(dirpin);
             pin_write(pwmpin, duty);
         }
 
         // oc_pwm(&oc1, &D[9], &timer5, freq, 0);
         // pin_write(&D[9], duty);
         
         // if (timer_flag(&timer1)) {
 
         //     timer_lower(&timer1);
         //     led_toggle(&led1);
 
         //     pin_toggle(dirpin);
 
         // }
     }
 } 