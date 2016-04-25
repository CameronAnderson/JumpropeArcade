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

int number = 0;
// int period_value = 1;  

// void delay(void){
//     if (period_value > .1){
//         period_value = (period_value - .1);
//         wait_period(period_value);
//         timer_start(&timer2);
//     }
//     else{
//         wait_period(.1);
//         timer_start(&timer2);
//     } 
// }

int16_t main(void) {

    init_clock();
    init_ui();
    init_timer();
    init_pin();
    init_spi();
    init_segment();

    timer_setPeriod(&timer2, 1);
    timer_start(&timer2);

    while (1) {

        if (timer_flag(&timer2)){
            timer_lower(&timer2);
            number ++;
        }
        showNumber(number);

        
        
    }
}

