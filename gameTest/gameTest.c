#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
// #include "pin.h"
#include "segment.h"
#include "oc.h"
#include "arm.h"

#define STANDBY  0
#define MAGNET   1
#define READY    2
#define LEFT     3
#define RIGHT    4
#define FAILURE  5

_PIN *rope, *coin, *button;

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
        arm_move(STANDBY);
        showNumber(000);
    }

    //State Tasks

    if (pin_read(rope) == 0){
        rope_connected = 1;
        led_on(blue_led);  
    }
    else{
        rope_connected = 0;
        led_off(blue_led);
    }

    if (pin_read(coin) == 0){
        coin_entered = 1;
        led_on(red_led);
    }
    
    if (coin_entered == 1 && rope_connected == 0){
        arm_move(MAGNET);
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
        timer_start(&timer2);
        showNumber(000);
        wait_period(1);
        arm_move(LEFT);
    }

    //Perform State Tasks

    if (timer_flag(&timer2)) {
        timer_lower(&timer2);
        led_toggle(blue_led);
    }

    //State Transistion

    if (pin_read(button) != 0){
        state = gameplay;  
    }

    //Leaving State

    if (state != last_state){
        led_off(blue_led);
        timer_stop(&timer2);
    }
}

void gameplay(void){

    // Stuff to do when entering the state

    if (state != last_state){
        last_state = state;
        led_on(green_led);
    }

    //Perform State Tasks

    period_value = .5;

    arm_move(RIGHT);

    score_counter ++;
    showNumber(score_counter);

    wait_period(2);        
 
    arm_move(LEFT);

    score_counter ++;
    showNumber(score_counter);

    wait_period(2);  

    if(pin_read(rope) != 0){
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
        timer_start(&timer2);
        gameover_counter = 0;
        arm_move(FAILURE);
    }

    //Perform State Tasks


    if (timer_flag(&timer2)) {
        timer_lower(&timer2);
        led_toggle(red_led);
        showNumber(score_counter);
        gameover_counter ++;
        timer_stop(&timer2);
        timer_start(&timer3);
    }
    
    if(timer_flag(&timer3)){
        timer_lower(&timer3);
        led_toggle(red_led);
        showBlank();
        gameover_counter ++;
        timer_stop(&timer3);
        timer_start(&timer2);
    }
    //State Transistion

    if (gameover_counter == 10){
        state = pre_game;
    }

    //Leaving State

    if (state != last_state){
        led_off(red_led);
        score_counter = 0;
        timer_stop(&timer2);
        timer_stop(&timer3);
    }
}


int16_t main(void) {

    init_clock();
    init_ui();
    init_timer();
    init_pin();
    // init_spi();
    init_segment();
    init_oc();
    init_arm();

    rope = &D[13];
    coin = &D[7];
    button = &D[4];

    pin_digitalIn(rope);
    pin_digitalIn(coin);
    pin_digitalIn(button);

    green_led = &led2;
    red_led = &led1;
    blue_led = &led3;

    timer_setPeriod(&timer2, .5);
    timer_setPeriod(&timer3, .5);

    state = pre_game;
    last_state = (STATE_HANDLER_T)NULL;

    while (1) {
        state();
    }
}
