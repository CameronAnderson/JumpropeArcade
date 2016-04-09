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
#include "motor_swing.h"

int16_t main(void) {
    init_clock();
    init_ui();
    init_timer();
    init_uart();
    init_pin();
    init_oc();
    init_motor_swing();


    while (1) {

        swing(0);

    }
}