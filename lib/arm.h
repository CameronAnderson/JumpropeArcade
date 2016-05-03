/*
** Copyright (c) 2016, Sean Karagianes
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
**     1. Redistributions of source code must retain the above copyright
**        notice, this list of conditions and the following disclaimer.
**     2. Redistributions in binary form must reproduce the above copyright
**        notice, this list of conditions and the following disclaimer in the
**        documentation and/or other materials provided with the distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef _ARM_H_
#define _ARM_H_

#include <stdint.h>
#include "common.h"
#include "timer.h"
#include "pin.h"
#include "ui.h"
#include "oc.h"

#include <float.h>

void init_arm(void);

typedef struct {
    uint16_t mode;

    // proportional control constant
    float K;
    // velocity control constant (if necessary)
    float B;

    uint16_t set_point;
    uint16_t duty;

    // variables for storing the position information
    uint16_t position;
    uint16_t position_1;

    uint16_t error;

    // variables for storing the velocity information (if necessary)
    float vel;
    float vel_1;

    _TIMER *timer;

    uint16_t time_count;
} _ARM;

extern _ARM arm;

void arm_init(_ARM *self, _TIMER *timer);

void arm_move(uint16_t mode);

void wait_period(uint16_t time_count);

#endif
