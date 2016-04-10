#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
// #include "uart.h"

int16_t main(void) {
    init_clock();
    init_ui();
    init_timer();
    init_pin();
    // init_uart();

    // uint16_t coin = &D[7];
    pin_digitalIn(&D[7]);

// test for Saarth
   
    while (1) {
        if(pin_read(&D[7]) == 0){
            led_toggle(&led3);
            
        }

        // uint16_t value = pin_read(rope);
        // if(value == 0){
        //     led_on(&led1);
        //     led_off(&led2);
        //     printf("Low%s\n\r");
        // }
        // if(value == 1){
        //     led_on(&led2);
        //     led_off(&led1);
        //     printf("High%s\n\r");
        // }
        }
            

    }


