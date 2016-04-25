#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "uart.h"

_LED *green_led, *red_led, *blue_led;

int16_t main(void) {
    init_clock();
    init_ui();
    init_timer();
    init_pin();
    init_uart();

    uint16_t rope = &D[13];
    pin_digitalIn(rope);
    uint16_t button = &D[4];
    pin_digitalIn(button);

    green_led = &led2;
    red_led = &led1;
    blue_led = &led3;
// test for Saarth
   
    while (1) {
        uint16_t value = pin_read(button);
        if(value == 0){
            led_on(green_led);
            led_off(red_led);
            printf("Low%s\n\r");
        }
        if(value == 1){
            led_on(red_led);
            led_off(green_led);
            printf("High%s\n\r");
        }
        }
            

    }


