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

_PIN *rope, *coin, *button, *button_led;

//PIN DEFINITIONS   dirpin = &D[8]
//                  pwmpin = &D[9]
//                  potentiometer = &A[5]
//                  segmentClock = &D[0];
//                  segmentLatch = &D[1];
//                  segmentData = &D[2];  
//                  rope = &D[13];
//                  coin = &D[7];
//                  button = &D[4];
//                  button_led = &D[10];


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
uint16_t swing_time = 100;
float period_value;
uint16_t magnet_flag = 0;


_LED *green_led, *red_led, *blue_led;

void swing_loop(void){

    arm_move(RIGHT);
 
    showNumber(score_counter);

    wait_period(swing_time);

    if(pin_read(rope) != 0){
        return;
    }

    score_counter ++;      
 
    arm_move(LEFT);

    showNumber(score_counter);

    wait_period(swing_time); 

    if(pin_read(rope) != 0){
        return;
    }
  
    score_counter ++;

    if((score_counter >= 20) && (swing_time >= 50)){
        swing_time --;
    }

}

void pre_game(void){

    // Stuff to do when entering the state

    if (state != last_state){
        last_state = state;
        led_on(green_led);
        showNumber(888);
        arm_move(STANDBY);

    }

    //State Tasks

    if (pin_read(rope) == 0){
        rope_connected = 1;
        led_on(blue_led);  
    }
    else{
        rope_connected = 0;
        led_off(blue_led);
        // arm_move(MAGNET);
    }

    if (pin_read(coin) == 0){
        coin_entered = 1;
        led_on(red_led);
    }
    
    if ((coin_entered == 1) && (rope_connected == 0) && (magnet_flag == 0)){
        led_on(blue_led);
        magnet_flag = 1;
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
        magnet_flag = 0;
    }
}

void ready(void){

    // Stuff to do when entering the state

    if (state != last_state){
        last_state = state;
        led_on(blue_led);
        timer_start(&timer2);
        wait_period(100);
        arm_move(LEFT);
        showNumber(003);
        wait_period(100);
        showNumber(002);
        wait_period(100);
        showNumber(001);
        wait_period(100);
        showNumber(000);
    }

    //Perform State Tasks

    if (timer_flag(&timer2)) {
        timer_lower(&timer2);
        showNumber(000);
        pin_toggle(button_led);
    }

    if (pin_read(rope) != 0){
        showBlank();
        coin_entered = 1;
        state = pre_game; 
    }

    //State Transistion

    if (pin_read(button) != 0){
        showNumber(000);
        state = gameplay;  
    }

    //Leaving State

    if (state != last_state){
        showNumber(000);
        led_off(blue_led);
        timer_stop(&timer2);
    }
}

void gameplay(void){

    // Stuff to do when entering the state

    if (state != last_state){
        last_state = state;
        led_on(green_led);

        showNumber(003);
        wait_period(100);
        showNumber(002);
        wait_period(100);
        showNumber(001);
        wait_period(100);
        showNumber(000);

    }

    //Perform State Tasks

    // period_value = .5;

    swing_loop();

    if(pin_read(rope) != 0){
        state = gameover;
    }

    //Leaving State

    if (state != last_state){
        led_off(green_led);
        swing_time = 100;
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

    if (gameover_counter == 12){
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
    button_led = &D[10];

    pin_digitalIn(rope);
    pin_digitalIn(coin);
    pin_digitalIn(button);

    pin_digitalOut(button_led);

    green_led = &led2;
    red_led = &led1;
    blue_led = &led3;

    timer_setPeriod(&timer2, .75);
    timer_setPeriod(&timer3, .75);

    state = pre_game;
    last_state = (STATE_HANDLER_T)NULL;

    while (1) {
        state();
    }
}

