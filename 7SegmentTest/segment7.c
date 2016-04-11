#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "segment.h"

int16_t main(void) {

    init_clock();
    init_ui();
    init_timer();
    init_pin();
    init_spi();
    init_segment();

    timer_setPeriod(&timer2, .01);
    timer_start(&timer2);

    int number = 0;  
   
    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            led_toggle(&led1);
        
            showNumber(999);
/*            number = number + 1;
            number %= 1000;*/
        }
        if (sw_read(&sw1) == 0){
            showNumber(111);  
        }
    }
}

