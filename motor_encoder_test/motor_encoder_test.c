/*
** Copyright (c) 2018, Bradley A. Minch
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
#include "elecanisms.h"

#define PWM_MIN_WIDTH     900e-6
#define PWM_MAX_WIDTH     2.1e-3

// D5 is pwm
// D6 is dir

static const dir = 1;

uint16_t pwm_offset, pwm_multiplier;

WORD32 pwm_temp; // I think this is for type-punning between word and unsigned long

int16_t main(void) {

    init_elecanisms();

    pwm_offset = (uint16_t)(FCY * PWM_MIN_WIDTH);
    pwm_multiplier = (uint16_t)(FCY * (PWM_MAX_WIDTH - PWM_MIN_WIDTH));

    // Set direction pin
    D6_DIR = OUT;
    D6 = 1; 

    // Set PWM pin (it'll get overwritten by PWM hardware)
    D5_DIR = OUT;
    D5 = 0;

    uint8_t *RPOR = (uint8_t *)&RPOR0;   // What is this?
    uint8_t *RPINR = (uint8_t *)&RPINR0; // Srsly, what is this?

    __builtin_write_OSCCONL(OSCCON & 0xBF);
    RPOR[D5_RP] = OC1_RP;  // connect the OC1 module output to pin D5
    __builtin_write_OSCCONL(OSCCON | 0x40);

    OC1CON1 = 0x1C0F;   // configure OC1 module to use the peripheral
                        //   clock (i.e., FCY, OCTSEL<2:0> = 0b111),
                        //   TRIGSTAT = 1, and to operate in center-aligned 
                        //   PWM mode (OCM<2:0> = 0b111)
    OC1CON2 = 0x008B;   // configure OC1 module to trigger from Timer1
                        //   (OCTRIG = 1 and SYNCSEL<4:0> = 0b01011)

    // set OC1 pulse width to 1.5ms (i.e. halfway between 0.9ms and 2.1ms)
    pwm_temp.ul = 0x8000 * (uint32_t)pwm_multiplier;
    OC1RS = pwm_offset + pwm_temp.w[1];
    OC1R = 1; // No clue
    OC1TMR = 0;

    T1CON = 0x0010;     // configure Timer1 to have a period of 20ms
    PR1 = 0x9C3F;

    TMR1 = 0;
    T1CONbits.TON = 1;

    while(1) { }

}
