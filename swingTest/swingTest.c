#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "motor_swing.h"
#include "oc.h"

#define Right 44800
#define Left 26400
#define Center 35600

// _PIN *rope, *coin, *button;

rope = &D[13];
coin = &D[7];
button = &D[4];

//PIN DEFINITIONS   dirpin = &D[8]
//                  pwmpin = &D[9]
//                  potentiometer = &A[5]
//                  segmentClock = &D[0];
//                  segmentLatch = &D[1];
//                  segmentData = &D[2];  
//                  rope = &D[13];
//                  coin = &D[7];
//                  button = &D[4];


typedef void (*STATE_HANDLER_T)(void);

STATE_HANDLER_T state, last_state;

uint16_t score_counter;
uint16_t period_counter;
uint16_t pre_counter;
uint16_t rope_connected;
uint16_t coin_entered;
uint16_t button_pressed;
uint16_t direction_flag1 = 1;
uint16_t gameover_counter;
float period_value;

_LED *green_led, *red_led, *blue_led;

int16_t main(void) {

    init_clock();
    init_ui();
    init_timer();
    init_pin();
    init_spi();
    init_motor_swing();
    init_oc();

    pin_digitalIn(rope);
 
    timer_setPeriod(&timer1, 1);
    timer_start(&timer1);

    uint16_t duty = 0x8000;


    while(1){
        while (pin_read(rope) == 0){

            set_dirpin();

            PIDcalc(35600);

            // move(30000, duty);

            // led_on(&led1);
            // led_off(&led2);
            // led_off(&led3);

            // while (!sw_read(&sw2)){
                // led_on(&led2);
            //     move(40000, duty);
            // }

            // while (!sw_read(&sw3)){
            //     // led_on(&led3);
            //     move(30000, duty);
            // }
        }
    }
}
