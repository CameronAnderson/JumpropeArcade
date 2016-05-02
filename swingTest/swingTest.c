#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
// #include "motor_swing.h"
#include "oc.h"
#include "arm.h"

#define STANDBY   0
#define MAGNET    1
#define READY     2
#define LEFT      3
#define RIGHT     4
#define FAILURE   5

_PIN *rope; // *coin, *button;

int16_t main(void) {
    init_clock();
    init_ui();
    init_timer();
    init_pin();
    // init_spi();
    // init_motor_swing();
    init_oc();
    init_arm();

    rope = &D[13];
    pin_digitalIn(rope);

    while (1) {
        if (!sw_read(&sw1)){
            arm_move(READY);
        }
        else if (!sw_read(&sw3)){
            arm_move(MAGNET);
        }
        else if (!sw_read(&sw2)){
            arm_move(FAILURE);
        }
    }
}