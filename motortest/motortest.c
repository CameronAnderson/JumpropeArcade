#include <p24FJ128GB206.h>
#include <stdio.h>
#include <time.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "oc.h"
#include "uart.h"

_PIN *dirpin1, *pwmpin1, *potentiometer1;

uint16_t actual_position1 = 0;

43700
20600
33500


int16_t main(void) {
    init_clock();
    init_ui();
    init_timer();
    init_pin();
    init_oc();
    init_uart();

    // dirpin1 = &D[8];  //set direction control pin as pin 8
    // pwmpin1 = &D[9];  //set PWM pin as pin 9 
    potentiometer1 = &A[5];

    //initialize the two pins as digital outputs and then clear them
    // pin_digitalOut(dirpin1);
    // pin_digitalOut(pwmpin1);
    pin_analogIn(potentiometer1);    


    // pin_clear(dirpin1);
    // pin_clear(pwmpin1);

    timer_setPeriod(&timer1, .1);
    timer_start(&timer1);

    // uint16_t duty1 = 0xffff;

    while (1) {

        if (timer_flag(&timer1)) {
            timer_lower(&timer1);
            actual_position1 = pin_read(potentiometer1);
            printf("Pot%u\n\r", actual_position1);            
        }


        // oc_pwm(&oc1, pwmpin1, &timer5, 1e3, duty1/10);

    }
}