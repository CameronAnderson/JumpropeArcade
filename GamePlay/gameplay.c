#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "segment.h"
#include "motor_swing.h"
#include "oc.h"

#define Right 22528
#define Left 48832

typedef void (*STATE_HANDLER_T)(void);

void pre_game(void);
void ready(void);
void gameplay(void);
void gameover(void);

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

void pre_game(void){

    // Stuff to do when entering the state

    if (state != last_state){
        last_state = state;
        led_on(green_led);
        showNumber(000);
    }

    //State Tasks

    if (sw_read(&sw1) == 0){
        rope_connected = 1;  
    }
    if (pin_read(&D[7]) == 0){
        coin_entered = 1;
        led_on(red_led);
    }

    //State Transistion?

    if (coin_entered + rope_connected == 2){
        state = ready;
    }

    //Leaving State

    if (state != last_state){
        led_off(green_led);
        led_off(red_led);
        rope_connected = 0;
        coin_entered = 0;
    }
}

void ready(void){

    // Stuff to do when entering the state

    if (state != last_state){
        last_state = state;
        led_on(blue_led);
        timer_start(&timer1);
        PIDcalc(48832);
    }

    //Perform State Tasks

    if (timer_flag(&timer1)) {
        timer_lower(&timer1);
        led_toggle(blue_led);
    }

    //State Transistion

    if (sw_read(&sw3) == 0){
        state = gameplay;  
    }

    //Leaving State

    if (state != last_state){
        led_off(blue_led);
        timer_stop(&timer1);
    }
}

void gameplay(void){

    // Stuff to do when entering the state

    if (state != last_state){
        led_on(green_led);
    }

    //Perform State Tasks

    // period_value = .5;
    if (direction_flag1 == 0){
        clear_dirpin();
        PIDcalc(Left);
        direction_flag1 = 1;
        score_counter ++;
        showNumber(score_counter);        
        // if (period_value){
        //     period_value = (period_value - .25);
        //     wait_period(period_value);   
        // }
        // else{
        //     wait_period(.5);
        // }
    }
    if (direction_flag1 == 1){;
        set_dirpin();
        PIDcalc(Right);
        direction_flag1 = 0;
        score_counter ++;
        showNumber(score_counter);
        // if (period_value > 1){
        //     period_value = (period_value - .25);
        //     wait_period(period_value);   
        // }
        // else{
        //     wait_period(.5);
        // } 
    }


    //State Transistion

    if(sw_read(&sw2) == 0){
        state = gameover;

    }

    //Leaving State

    if (state != last_state){
        led_off(green_led);
    }
}

void gameover(void){

    // Stuff to do when entering the state

    if (state != last_state){
        last_state = state;
        led_on(red_led);
        timer_start(&timer1);
    }

    //Perform State Tasks

    if (timer_flag(&timer1)) {
        timer_lower(&timer1);
        led_toggle(red_led);
        showNumber(score_counter);
        gameover_counter ++;

    }
    showNumber(000);

    //State Transistion

    if (gameover_counter == 20){
        state = pre_game;  
    }

    //Leaving State

    if (state != last_state){
        led_off(red_led);
        score_counter = 0;
        timer_stop(&timer1);
    }
}


int16_t main(void) {

    init_clock();
    init_ui();
    init_timer();
    init_pin();
    init_spi();
    init_segment();
    init_motor_swing();
    init_oc();

    green_led = &led2;
    red_led = &led1;
    blue_led = &led3;

    timer_setPeriod(&timer1, .5);
    timer_setPeriod(&timer3, .25);

    state = pre_game;
    last_state = (STATE_HANDLER_T)NULL;

    while (1) {
        state();
    }
}

